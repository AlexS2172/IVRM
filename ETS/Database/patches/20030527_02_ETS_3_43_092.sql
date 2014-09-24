/*
	MLine00001743
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.091' or (keyValue = '3.43.092')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.092 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.092' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- usp_Position_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TimeZoneActimeBias_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TimeZoneActimeBias_Get
GO

CREATE  procedure dbo.usp_TimeZoneActimeBias_Get
AS
	declare @iActiveTimeBias int
	exec master.dbo.xp_regread 'HKEY_LOCAL_MACHINE', 'SYSTEM\CurrentControlSet\Control\TimeZoneInformation', 'ActiveTimeBias', @iActiveTimeBias OUT
	return isnull(@iActiveTimeBias, 0)

GO

----------------------------------------------------------------------------------------------------
-- usp_Position_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Position_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Position_Get
GO

CREATE   PROCEDURE dbo.usp_Position_Get
	@iUnderlyingID int = Null,
	@iTraderID int = Null,
	@iBookID int = Null,
	@iStrategyID int = Null,
	@dtMaxDate datetime = null,
	@dtToday datetime = null
AS
	set nocount on

	if @dtToday is null
	begin
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
		declare @iDeltaGMT int
		 
		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
		 
		set @dtToday = dateadd(minute, @iDeltaGMT, @dtToday)
	end

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
	
	from TradeView tr 
		join ContractView c  on tr.contractID = c.contractID
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield
	
	union

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,

		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		EC.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * ORt.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when tr.tradeDate < @dtToday then isnull(cp.priceTheo, 0) else 0 end) * tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cp.priceTheo as fPriceTheoClose

	from TradeView tr 
		join ContractView C on tr.contractID = C.contractID
		join ContractPrice CP on C.contractID = cp.contractID and exchangeID is null
		left join (OptionView o 
						join OptionRoot ORt on o.optionRootID = ORt.optionRootID 
						join Contract UC on ORt.underlyingID = UC.contractID) on c.contractID = o.optionID
--		left join (OptionView o join Contract UC on o.underlyingContractID = UC.contractID) on c.contractID = o.optionID
		join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
		left join StockView s on uc.contractID = s.stockID
		left join IndexView i on uc.contractID = i.indexID

	where c.contractTypeID = 3
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)

	group by C.contractID, C.contractTypeID, C.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cp.priceTheo

	order by vcSymbol

GO

----------------------------------------------------------------------------------------------------
-- usp_TradeCA_Save
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TradeCA_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TradeCA_Save
GO

CREATE      proc dbo.usp_TradeCA_Save
		@iCorpActionID int,
        @iTradeID int = null, --for new trade is null
		@iQuantity int, 
		@fPrice float, 
		@iContractID int = null, 
		@tiIsClose tinyint = 0, 
		@tiIsBuy tinyint = 0, 
		@iTraderID int = null, 
		@tiIsPosition tinyint = 0,
		@tiStatus tinyint = 0
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction TU
		--delete of trade, which we plan to update
		if (select count(*) from Trade where tradeID = @iTradeID)!=2 and @iTradeID is not null
			begin
				exec @error = usp_TradeCA_Del @iCorpActionID,@iTradeID
					if @@error <> 0 begin Rollback Transaction TU Return(0) end
			end

		Declare @iNewTradeID int
			select @iNewTradeID = isnull(max(tradeID),0) + 1 from Trade
	
		Declare @iNewSeqNum int 
			select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
		--===============================================
		-- variables for work with CorporateActionLog
		--================================================
		declare @vcTableName sysname,
				@iCActionID int, --1 for insert, 2 - for update, 3 - for delete
				@vcKeyRow varchar(50),  -- name of field with identity atribute
				@iRowID int,			-- tipically is identity value
				@vcFieldName varchar(50),
				@vcFieldValue varchar(250),
				@vcFieldType varchar(50)

		select @iCActionID = 1, @vcTableName = 'Trade', @vcKeyRow = 'seqNum'


		--update existing trade
		if @iTradeID is not null
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				status,
				SeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate)
			select @iNewTradeID,
				1,
				contractID,
				isClose,
				@iQuantity,
				@fPrice,
				isBuy,
				traderID,
				isPosition,
				status,
				@iNewSeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate
			from Trade
			where tradeID = @iTradeID and actionID=1

			set @error = @@error
				if @error != 0 begin Rollback Transaction TU Return(-1) end
		end
	else
		begin
			--create the tradeDate
			declare @iDeltaGMT int,
					@dtNewTradeDate datetime

			exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())

			-- insert new trade
			insert into Trade(
				tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				status,
				seqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate)
			select 
				@iNewTradeID,
				1,
				@iContractID,
				@tiIsClose,
				@iQuantity,
				@fPrice,
				@tiIsBuy,
				@iTraderID,
				@tiIsPosition,
				@tiStatus,
				@iNewSeqNum,
				Null,
				Null,
				@dtNewTradeDate

			set @error = @@error
				if @error != 0 begin Rollback Transaction TU Return(-1) end
		end
	
		--write into CA log
		select 	@vcFieldName = 'seqNum', 
				@vcFieldValue = @iNewSeqNum,
				@vcFieldType = 'int',
				@iRowID = @iNewSeqNum
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCActionID,@iRowID,@vcKeyRow

		if @@error = 0 
			begin 
				Commit Transaction TU 
				Return @iNewSeqNum 
			end 
		else 
			begin 
				Rollback Transaction TU 
				Return(-1) 
			end
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Exec
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_Exec' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_Exec
GO

CREATE  procedure dbo.usp_Trade_Exec
	@tiExercise tinyint,
	@iOptTradeID int,
	@iOptQuantity int,
	@tiOptIsBuy tinyint,
	@tiOptStatus tinyint,
	@iUndContractID int,
	@iUndQuantity int,
	@fUndPrice float,
	@tiUndIsBuy tinyint,
	@tiUndStatus tinyint,
	@iUndBrokerCommissionID int,
	@iUndClearingBrokerCommissionID int,
	@iNewOptSeqNum int = null out,
	@iNewOptTradeID int = null out,
	@dtNewOptTradeDate datetime = null out,
	@iNewUndSeqNum int = null out,
	@iNewUndTradeID int = null out,
	@dtNewUndTradeDate datetime = null out
as
	set nocount on

	begin tran

	declare @bSucceed bit
		
	set @bSucceed = 1

	declare @iDeltaGMT int

	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	select @iNewOptTradeID = max(tradeID) + 1 from Trade
		if @iNewOptTradeID is null set @iNewOptTradeID = 1

	select @iNewOptSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @dtNewOptTradeDate is null	
	begin
		set @dtNewOptTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
		price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
		brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
		isPosition, status, execID, SeqNum)
	select @iNewOptTradeID, 1, contractID, isClose, @dtNewOptTradeDate, @iOptQuantity, 0, @tiOptIsBuy,
		bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
		clearingBrokerCommissionID, tradedIV, spotReference, 0, @tiOptStatus,
		null, @iNewOptSeqNum
	from Trade
	where tradeID = @iOptTradeID and actionID = 1

	if @@error <> 0 set @bSucceed = 0

	if @bSucceed = 1 and isnull(@tiExercise, 0) <> 0
	begin
		select @iNewUndTradeID = max(tradeID) + 1 from Trade
			if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
		select @iNewUndSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
		if @dtNewUndTradeDate is null	
		begin
			set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end
	
		insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
			price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
			brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
			isPosition, status, execID, SeqNum)
		select @iNewUndTradeID, 1, @iUndContractID, isClose, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
			bookID, strategyID, traderID, brokerID, clearingBrokerID, @iUndBrokerCommissionID,
			@iUndClearingBrokerCommissionID, 0, 0, 0, @tiUndStatus,
			null, @iNewUndSeqNum
		from Trade
		where tradeID = @iOptTradeID and actionID = 1
	
		if @@error <> 0 set @bSucceed = 0
	end


	if @bSucceed = 1
	begin
		commit tran
		if isnull(@tiExercise, 0) <> 0
			return @iNewUndSeqNum
		else
			return @iNewOptSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewOptSeqNum = null
		set @iNewOptTradeID = null
		set @dtNewOptTradeDate = null
		set @iNewUndSeqNum = null
		set @iNewUndTradeID = null
		set @dtNewUndTradeDate = null
		return 0
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Match
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_Match' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_Match
GO

CREATE   procedure dbo.usp_Trade_Match
	@iOldManualTradeID int,
	@iOldActualTradeID int,
	@iOldManualSeqNum int out,
	@iOldActualSeqNum int out,
	@iNewActualSeqNum int out,
	@iNewActualTradeID int out,
	@dtNewActualTradeDate datetime out
as
	set nocount on

	begin tran

	declare @bSucceed bit,
			@nCount int
		
	set @bSucceed = 1

	if @iOldManualTradeID is not null -- delete old manual trade
	begin
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldManualTradeID
		
		if @nCount = 1
		begin
			select @iOldManualSeqNum = isnull(max(seqNum),0) + 1 from Trade

			insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
				price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
				brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
				isPosition, status, execID, SeqNum)
			select tradeID, 0, contractID, isClose, tradeDate, quantity, price, isBuy,
				bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
				clearingBrokerCommissionID, tradedIV, spotReference, isPosition, status,
				execID, @iOldManualSeqNum
			from Trade
			where tradeID = @iOldManualTradeID
		
			if @@error <> 0 set @bSucceed = 0
		end
	end

	if @bSucceed = 1 and @iOldActualTradeID is not null -- delete old actual trade and make new with matched status
	begin
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldActualTradeID
		
		if @nCount = 1
		begin
			select @iOldActualSeqNum = isnull(max(seqNum),0) + 1 from Trade

			insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
				price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
				brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
				isPosition, status, execID, SeqNum)
			select tradeID, 0, contractID, isClose, tradeDate, quantity, price, isBuy,
				bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
				clearingBrokerCommissionID, tradedIV, spotReference, isPosition, status,
				execID, @iOldActualSeqNum
			from Trade
			where tradeID = @iOldActualTradeID
		
			if @@error <> 0 set @bSucceed = 0
		end

		if @bSucceed = 1 -- create new actual matched trade
		begin
			select @iNewActualTradeID = max(tradeID) + 1 from Trade
			if @iNewActualTradeID is null set @iNewActualTradeID = 1
	
			if @dtNewActualTradeDate is null	
			begin
				declare @iDeltaGMT int

				exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
		
				set @dtNewActualTradeDate = dateadd(minute, @iDeltaGMT, getdate())
			end
		
			select @iNewActualSeqNum = isnull(max(seqNum),0) + 1 from Trade
		
			insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
				price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
				brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
				isPosition, status, execID, SeqNum)
			select @iNewActualTradeID, 1, contractID, isClose, tradeDate, quantity, price, isBuy,
				bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
				clearingBrokerCommissionID, tradedIV, spotReference, isPosition, 3,
				execID, @iNewActualSeqNum
			from Trade
			where tradeID = @iOldActualTradeID and actionID = 0

			if @@error <> 0 set @bSucceed = 0
		end
	end

	if @bSucceed = 1
	begin
		commit tran
		return @iNewActualSeqNum
	end
	else  
	begin
		rollback tran
		set @iOldManualSeqNum = null
		set @iOldActualSeqNum = null
		set @iNewActualSeqNum = null
		set @iNewActualTradeID = null
		set @dtNewActualTradeDate = null
		return 0
	end

GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_New
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_New' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_New
GO

CREATE   procedure dbo.usp_Trade_New
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID int =Null,
        @iClearingBrokerCommissionID int = Null,
        @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint = Null
  as
	set nocount on

	begin tran
	
	Declare @iNewSeqNum int
	
	select @iNewTradeID = max(tradeID) + 1 from Trade
		if @iNewTradeID is null set @iNewTradeID = 1

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade


	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	insert into Trade (
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		bookID, 
		strategyID, 
		traderID, 
		brokerID, 
		clearingBrokerID, 
		isBuy, 
		brokerCommissionID, 
		clearingBrokerCommissionID,
		tradedIV,
		spotReference,
		isPosition,
		status,
		seqNum)
	values (
		@iNewTradeID,
		1,
		@iContractID, 
		@tiIsClose, 
		@dtNewTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iBookID, 
		@iStrategyID, 
		@iTraderID, 
		@iBrokerID,
		@iClearingBrokerID,
		@tiIsBuy, 
		@iBrokerCommissionID,
		@iClearingBrokerCommissionID,
		@fTradedIV,
		@fSpotReference,
		@tiIsPosition,
		@tiStatus,
		@iNewSeqNum)
	if @@error = 0
	begin
		--set @iNewSeqNum = @@identity 
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Update
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_Update' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_Update
GO

CREATE  procedure dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int = Null,
	@tiIsClose tinyint = Null,
	@iQuantity int = Null,
	@fPrice float = Null,
	@tiIsBuy tinyint = Null,  
	@iBookID int = Null,
	@iStrategyID int = Null,
	@iTraderID int = Null,
	@iBrokerID int = Null,
	@iClearingBrokerID int = Null,
	@iBrokerCommissionID float =Null,
	@iClearingBrokerCommissionID float = Null,
	@fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime out,
--	@iLotSize int = Null,
	@tiStatus tinyint = null
  as
	begin tran

	declare @bSucceed bit,
			@iNewSeqNum int,
			@vcExecID varchar(20)

	set @bSucceed = 0

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iOldTradeID is not null

	begin
		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			select @vcExecID = execID from Trade where tradeID = @iOldTradeID

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
				status,
				execID,
				SeqNum)
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
				status,
				execID,
				@iNewSeqNum
			from Trade
			where tradeID = @iOldTradeID
		
			if @@error = 0 set @bSucceed = 1
		end
	end
	if @bSucceed = 1
	begin
		select @iNewTradeID = max(tradeID) + 1 from Trade
		if @iNewTradeID is null set @iNewTradeID = 1

		if @dtNewTradeDate is null	
		begin
			declare @iDeltaGMT int

			exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
	
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end
	
		select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
		insert into Trade (
			tradeID,
			actionID, 
			contractID,
			isClose, 
			tradeDate, 
			quantity, 
			price, 
			bookID, 
			strategyID, 
			traderID, 
			brokerID, 
			clearingBrokerID, 
			isBuy, 
			brokerCommissionID, 
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			status,
			seqNum,
			execID)
		values (
			@iNewTradeID,
			1,
			@iContractID, 
			@tiIsClose, 
			@dtNewTradeDate, 
			@iQuantity, 
			@fPrice, 
			@iBookID, 
			@iStrategyID, 
			@iTraderID, 
			@iBrokerID,
			@iClearingBrokerID,
			@tiIsBuy, 
			@iBrokerCommissionID,
			@iClearingBrokerCommissionID,
			@fTradedIV,
			@fSpotReference,
			@tiIsPosition,
			@tiStatus,
			@iNewSeqNum,
			@vcExecID)
		if @@error = 0
			set @bSucceed = 1
		else
			set @bSucceed = 0
	end
	if @bSucceed = 1
	begin
--		set @iNewSeqNum = @@identity
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end
GO

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.092')
	begin
		update DataInfo set keyValue = '3.43.092' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.092' '3.43.092 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
