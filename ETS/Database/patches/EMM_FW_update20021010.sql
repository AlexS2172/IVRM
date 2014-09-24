/*
	Changes for last release of feed
	DB version: 3.37.031
*/
set nocount on

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_1_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_1_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE procedure dbo.usp_TradeFeed_1_Del
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
			select -2 as iSeqNum
			--Raiserror('Record with this execID does not exists in DB!',16,1)
			--Return(-1)
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
	
		--select @iSeqNum as  iSeqNum
		select @iNewSeqNum as  iSeqNum
	Commit Transaction trade

	set @error = 0
finish:
	select -1 as iSeqNum
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

CREATE PROCEDURE dbo.usp_TradeFeed_1_New
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
	@iTradeLotSize int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	if exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with same execID alreadty exists in DB!',16,1)
			--Select null as iSeqNum
			Select 0 as iSeqNum, 0 as iStructureMissed
			Return(-1)

		end

	Declare @iTradeContractID int, 
			@iStructureMissed int,
			@iUnderlyingContractID int 

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

		select @iSeqNum as iSeqNum, @iStructureMissed as iStructureMissed
	Commit Transaction


    set @error = 0
finish:
	select -1 as iSeqNum, -1 as iStructureMissed
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


CREATE  PROCEDURE usp_TradeFeed_Get
	@vcExecID varchar(20),
	@dtTradeDate datetime,
	@iActionID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is check trade by execID 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare	@iStructureMissed int,
			@iSeqNumber int


	select
		tr.seqNum as iSeqNum,
		tr.actionDate as dtActionDate,
		tr.tradeID as iTradeID,
		tr.actionID as tiActionID,
		tr.isClose as tiIsClose,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,
		tr.bookID as iBookID,     
		tr.strategyID as iStrategyID, 
		tr.traderID as iTraderID,
		tr.brokerID as iBrokerID,
		tr.clearingBrokerID as iClearingBrokerID,
		tr.BrokerCommissionID as iBrokerCommissionID,
		tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
		cv1.CommissionValue as fBrokerCommission,
		cv2.CommissionValue as fClearingBrokerCommission,
		tr.tradedIV as fTradedIV,
		tr.spotReference as fSpotReference,
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		i.yield as fYield,
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		null as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		tr.lotSize as iTradeLotSize,
		cp.lotSize as iUnderlyingLotSize,
		cp.lotSize as iLotSize,
		tr.isSimulated as tiIsSimulated,
		null as fPriceClose,
		null as fUnderlyingPriceClose,
		case when exists(select * from TradeLog where contractID = tr.contractID) then 1 else 0 end iStructureMissed
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where (tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112) )
		or (tr.actionID=0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112) )

	union 
	select
		tr.seqNum as iSeqNum,
		tr.actionDate as dtActionDate,
		tr.tradeID as iTradeID,
		tr.actionID as tiActionID,
		tr.isClose as tiIsClose,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,
		tr.bookID as iBookID,     
		tr.strategyID as iStrategyID, 
		tr.traderID as iTraderID,
		tr.brokerID as iBrokerID,
		tr.clearingBrokerID as iClearingBrokerID,
		tr.BrokerCommissionID as iBrokerCommissionID,
		tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
		cv1.CommissionValue as fBrokerCommission,
		cv2.CommissionValue as fClearingBrokerCommission,
		tr.tradedIV as fTradedIV,
		tr.spotReference as fSpotReference,
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		i.yield as fYield,
		EC.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		cp.priceTheo as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		tr.lotSize as iTradeLotSize,
		ucp.lotSize as iUnderlyingLotSize,
		cp.lotSize as iLotSize,
		tr.isSimulated as tiIsSimulated,
		CP.PriceClose as fPriceClose,
		UCP.priceClose as fUnderlyingPriceClose,
		case when exists(select * from TradeLog where contractID = UC.contractID) then 1
			 when exists(select * from TradeLog where contractID = O.optionID) then 2
		     when exists(select * from TradeLog where contractID = UC.contractID)
				and exists(select * from TradeLog where contractID = O.optionID) then 3
             else 0 	
			end iStructureMissed
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		inner join [Option] O on C.contractID = O.optionID
		inner join Contract UC on O.underlyingContractID = UC.contractID
		inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
		inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
		join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
		left join StockView s on uc.contractID = s.stockID
		left join [IndexView] i on uc.contractID = i.indexID
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where (tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112) )
		or (tr.actionID=0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112) )

	Return(@@Error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--==================================================================================
--change DB version
--===================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.031')
	begin
		update DataInfo set keyValue = '3.37.031' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.37.031'
	end

