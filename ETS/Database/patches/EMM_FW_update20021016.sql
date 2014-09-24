/*
	changes for support FOC & SAGE RTTF at the DB level
*/

/*PREPARATIONS FOR ERROR HANDLING*/
SET NOCOUNT ON
GO
IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#tmpErrors')) DROP TABLE #tmpErrors
GO
CREATE TABLE #tmpErrors (Error int)
GO
SET XACT_ABORT ON
GO
/*SCRIPTS MAIN SECTION*/
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.37.032' or (keyValue = '3.37.033')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.37.033' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_1_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_1_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   procedure dbo.usp_TradeFeed_1_Del
	@vcExecID varchar(20),
	@dtTradeDate datetime
/*
	Created by Zaur Nuraliev
	Description: this procedure is mark as delete information in Trade table
*/
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iSeqNum int,
			@iNewSeqNum int

	select 	@iSeqNum = seqNum 
		from Trade 
		where execID = @vcExecID 
			and actionID > 0 
			and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112)

	if @iSeqNum is null
		begin
			select 	@iSeqNum = seqNum 
				from Trade 
				where execID = @vcExecID 
					and actionID <> 0 
					and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112)

			if @iSeqNum is  not null
				select @iSeqNum as iMaxSeqNum, -1 as iMinSeqNum
			else 
				select -1 as iMaxSeqNum, 0 as iMinSeqNum

			Return(-1)
		end

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

	Begin Transaction trade
		insert into Trade(tradeID,
			actionID,
			contractID,
			isClose,
			tradeDate,
			quantity,
			price,
			isBuy,
			bookID,
			strategyID,
			traderID,
			brokerID,
			clearingBrokerID,
			brokerCommissionID,
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			lotSize,
			isSimulated,
			execID,
			seqNum)
		select tradeID,
			0,
			contractID,
			isClose,
			tradeDate,
			quantity,
			price,
			isBuy,
			bookID,
			strategyID,
			traderID,
			brokerID,
			clearingBrokerID,
			brokerCommissionID,
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			lotSize,
			isSimulated,
			execID,
			@iNewSeqNum
		from Trade
		where seqNum = @iSeqNum
				
		if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end
	
		select @iSeqNum as iMinSeqNum,@iNewSeqNum as iMaxSeqNum
	Commit Transaction trade

	set @error = 0
finish:
		select -1 as iMinSeqNum, -1 as iMaxSeqNum
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    PROCEDURE dbo.usp_TradeFeed_1_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(8),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(8) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@tiIsUpdateEnabled tinyint = 0
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iMinSeqNum int 
		select @iMinSeqNum = seqNum 
			from Trade 
			where 	execID = @vcExecID and 
					actionID > 0 and 
					Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112)

	if @tiIsUpdateEnabled = 1
		begin
			if @iMinSeqNum is not null
				exec usp_TradeFeed_1_Del @vcExecID,@dtTradeDate
		end
	else if @iMinSeqNum is not null and @tiIsUpdateEnabled = 0
		begin
			select -1 as iMaxSeqNum, @iMinSeqNum as iMinSeqNum, -1 as iStructureMissed
			Return(-1)
		end 

	Declare @iTradeContractID int, 
			@iUnderlyingContractID int,
			@iStructureMissed int

	set @iStructureMissed = 0

	--check underlying symbol
		if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
			set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3


	Begin Transaction
	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			if @iTradeLotSize is null set @iTradeLotSize = 1

			insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			set @iStructureMissed = 1

			insert into TradeLog (contractID, eventTypeID) values (@iUnderlyingContractID, 0)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

		end
	else
		begin  --check this instruction carefully!
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where underlyingContractID = @iUnderlyingContractID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3
	
			if @iOptionID is null
			--create new option
				begin
					if isnull(@vcContractSymbol, '') <> ''
					begin
						Declare @vcContractName varchar(255)
						  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
										' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
											 ' ' + cast(round(@fStrike, 2) as varchar)
		
						insert into Contract (contractTypeID, symbol, contractName) values (3, @vcContractSymbol,@vcContractName)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
							set @iOptionID = @@identity
				
						insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
							 values (@iOptionID, @iUnderlyingContractID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID,lotSize) values(@iOptionID,null,@iTradeLotSize)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

						insert into TradeLog (contractID, eventTypeID) values (@iOptionID, 0)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end

					set @iStructureMissed = @iStructureMissed + 2
				end

			set @iTradeContractID = @iOptionID
		end
		else
			set @iTradeContractID = @iUnderlyingContractID


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym,@vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	if @iTradeLotSize is null
		select @iTradeLotSize = lotSize from ContractPrice 
			where exchangeID is null and 
					(contractID = @iOptionID and @iContractTypeID = 3) or
					(contractID = @iUnderlyingContractID and @iContractTypeID < 3)

	if @iTradeLotSize is null
		if @iContractTypeID = 3 
			set @iTradeLotSize = 100
		else if @iContractTypeID < 3 
			set @iTradeLotSize = 1

	declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iNewTradeID is null set @iNewTradeID = 1

	--create new trade	
	insert into Trade (
		execID,
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		traderID, 
		isBuy, 
		isPosition,
		lotSize,
		isSimulated,
		brokerCommissionID,
		clearingBrokerCommissionID,
		seqNum)
	values (
		@vcExecID,
		@iNewTradeID,
		1,
		@iTradeContractID, 
		0, 
		@dtTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iTraderID, 
		@tiIsBuy, 
		0,
		@iTradeLotSize,
		0,
		null,
		null,
		@iSeqNum)

		if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

		select @iSeqNum as iMaxSeqNum,isnull(@iMinSeqNum,0) as iMinSeqNum, @iStructureMissed as iStructureMissed
	Commit Transaction


    set @error = 0

Return(@error)

finish:
	select -1 as iMaxSeqNum,-1 as iMinSeqNum, -1 as iStructureMissed
	Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE          proc usp_Trader_Get
	@iTraderID int = Null,
	@bInTrade bit = 0,
	@bShortFormat bit = 0,
	@dtActionDate datetime = null
  as
/*
	Edited by Zaur Nuraliev
	Description: this procedure is getting information from TraderView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
     if (@iTraderID is Null)
        if (@bInTrade = 0 and @bShortFormat = 0) 
			select Trader.traderID as iTraderID, 
					Trader.acronym as vcAcronym, 
					Trader.traderName as vcTraderName, 
					Trader.clearingTraderCode as vcClearingTraderCode,
					Trader.traderRoleID as iTraderRoleID,
					Trader.traderRefID as iTraderRefID,
					TraderRole.roleName as vcRoleName,
					T.traderName as vcTraderRefName,
					Trader.actionDate as dtActionDate,
					Trader.actionID as tiActionID 
				from Trader
					left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
					left join TraderView t on Trader.traderRefID=T.traderID
				where	(Trader.actionDate>=@dtActionDate or @dtActionDate is null) and 
						(@dtActionDate is not null or @dtActionDate is null and Trader.actionID<3)
				order by Trader.TraderName
		else if (@bInTrade = 0 and @bShortFormat = 1) 
				select 	traderID as iTraderID, 
						traderName as vcTraderName 
				from Trader
				where	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
				order by Trader.acronym	--TraderName
				
        else
			select Trader.traderID as iTraderID, 
					Trader.acronym as vcAcronym, 
					Trader.traderName as vcTraderName, 
					Trader.clearingTraderCode as vcClearingTraderCode,
					Trader.traderRoleID as iTraderRoleID,
					Trader.traderRefID as iTraderRefID,
					TraderRole.roleName as vcRoleName,
					T.traderName as vcTraderRefName,
					Trader.actionDate as dtActionDate,
					Trader.actionID as tiActionID 
				from Trader 
					left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
					left join TraderView t on Trader.traderRefID=T.traderID
				where exists (select tr.traderID from TradeView tr  where tr.traderID = Trader.traderID)
					and	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
	            order by Trader.TraderName
      else 
		select Trader.traderID as iTraderID, 
				Trader.acronym as vcAcronym, 
				Trader.traderName as vcTraderName, 
				Trader.clearingTraderCode as vcClearingTraderCode,
				Trader.traderRoleID as iTraderRoleID,
				Trader.traderRefID as iTraderRefID,
				TraderRole.roleName as vcRoleName,
				T.traderName as vcTraderRefName,
				Trader.actionDate as dtActionDate,
				Trader.actionID as tiActionID  
			from Trader 
				left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
				left join TraderView T on Trader.traderRefID=T.traderID
			where Trader.traderID = @iTraderID
					and	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
	set @error = @@error
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.033')
	begin
		update DataInfo set keyValue = '3.37.033' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
IF EXISTS (SELECT * FROM #tmpErrors)
BEGIN
	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
END
GO
IF @@TRANCOUNT > 0 
BEGIN
	PRINT 'The database update succeeded'
	PRINT 'DB VERSION WAS CHANGED TO 3.37.033'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
