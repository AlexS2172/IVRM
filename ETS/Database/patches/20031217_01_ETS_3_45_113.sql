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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.112' or (keyValue = '3.45.113')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.113 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.113' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_Trade_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_New]
GO

CREATE PROC dbo.usp_Trade_New
    @iContractID int,
    @iQuantity int,
    @fPrice float,
    @tiIsBuy tinyint,  
    @iTraderID int,
    @iBrokerID int = Null,
    @fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
    @fTradedIV float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint,
	@iMark int = null,
    @iStrategyID int = null out,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
    @vcImportID varchar(20) = null out,
	@tiAssignStrategy tinyint = 0
as
	set nocount on

	declare @error int		set @error = 0

	-- check for valid contract parameters
	exec @error = usp_ContractParameters_CheckValidity @iContractID, @iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID
	if @error <> 0 goto finish

	begin tran

	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	if isnull(@tiAssignStrategy, 0) != 0
	begin
		-- determine strategy for trade
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
		if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
	end

	-- get new tradeID ane seqNum
	declare @iNewSeqNum int

	select 
		@iNewTradeID = isnull(max(tradeID), 0) + 1,
		@iNewSeqNum = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade
	insert into Trade (tradeID, actionID, contractID, tradeDate,  quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iNewSeqNum, @iMark,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	select @vcImportID = exportSymbol from ContractView where contractID = @iContractID

	if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @iStrategyID = null
		set @vcImportID = null
		return @error
	end

GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Update
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Update]
GO

CREATE PROC dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiIsBuy tinyint,
	@iTraderID int,
	@iBrokerID int = Null,
	@fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
	@fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime out,
	@tiStatus tinyint,
	@iMark int = null,
    @iNewTradeStrategyID int = Null out,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
    @vcImportID varchar(20) = Null out,
	@tiAssignStrategy tinyint = 0
as
	set nocount on

	declare @error int		set @error = 0

	declare @iNewSeqNum int,
			@vcExecID varchar(20)

	if @iOldTradeID is not null
	begin
		begin tran

		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
			select @vcExecID = execID from Trade where tradeID = @iOldTradeID

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
			from Trade
			where tradeID = @iOldTradeID

			if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
		end

		if @dtNewTradeDate is null	
		begin
			declare @iDeltaGMT int

			exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
	
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end
	
		select 
			@iNewTradeID = isnull(max(tradeID), 0) + 1,
			@iNewSeqNum = isnull(max(seqNum), 0) + 1
		from Trade
	
		if isnull(@tiAssignStrategy, 0) != 0
		begin
			-- determine strategy for trade
			exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iNewTradeStrategyID out
			if @@error <> 0 or @error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, seqNum, execID, mark,
			underlyingID, optionRootID, futureRootID, futureID)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iNewTradeStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@iNewSeqNum, @vcExecID, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		select @vcImportID = exportSymbol from ContractView where contractID = @iContractID
	
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

		commit tran
	end
	else
		set @error = -4

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @iNewTradeStrategyID = null
		set @vcImportID = null
		return @error
	end

GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_PositionTransfer
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_PositionTransfer]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_PositionTransfer]
GO

CREATE PROC dbo.usp_Trade_PositionTransfer
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiStatus tinyint,

	@iTraderIDFrom int,
	@iMarkFrom int,
    @tiIsBuyFrom tinyint,  

	@iTraderIDTo int,
	@iMarkTo int,
    @tiIsBuyTo tinyint,  

	@iSeqNumFrom int out,
	@iTradeIDFrom int out,
	@iStrategyIDFrom int out,
	@dtTradeDateFrom datetime out,

	@iSeqNumTo int out,
	@iTradeIDTo int out,
	@iStrategyIDTo int out,
	@dtTradeDateTo datetime out,

	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
	@tiAssignStrategy tinyint = 0
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	declare @iDeltaGMT int
	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	-- Trade 'from' creation
	if @dtTradeDateFrom is null	
		set @dtTradeDateFrom = dateadd(minute, @iDeltaGMT, getdate())

	if isnull(@tiAssignStrategy, 0) != 0
	begin
		-- determine strategy for trade from
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderIDFrom, @iContractID, @iStrategyIDFrom out
		if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
	end

	-- get new tradeID ane seqNum for trade from
	select 
		@iTradeIDFrom = isnull(max(tradeID), 0) + 1,
		@iSeqNumFrom = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade from
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDFrom, 1, @iContractID, @dtTradeDateFrom, @iQuantity, @fPrice, @iTraderIDFrom, @iStrategyIDFrom,
		@tiIsBuyFrom, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumFrom, @iMarkFrom,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	-- Trade 'to' creation
	if @dtTradeDateTo is null	
		set @dtTradeDateTo = dateadd(minute, @iDeltaGMT, getdate())

	if isnull(@tiAssignStrategy, 0) != 0
	begin
		-- determine strategy for trade to
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderIDTo, @iContractID, @iStrategyIDTo out
		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
	end

	-- get new tradeID ane seqNum for trade to
	select 
		@iTradeIDTo = isnull(max(tradeID), 0) + 1,
		@iSeqNumTo = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade to
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDTo, 1, @iContractID, @dtTradeDateTo, @iQuantity, @fPrice, @iTraderIDTo, @iStrategyIDTo,
		@tiIsBuyTo, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumTo, @iMarkTo,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error <> 0
	begin
		set @iSeqNumFrom = null
		set @iTradeIDFrom = null
		set @iStrategyIDFrom = null
		set @dtTradeDateFrom = null
	
		set @iSeqNumTo = null
		set @iTradeIDTo = null
		set @iStrategyIDTo = null
		set @dtTradeDateTo = null
	end

	return @error

GO


----------------------------------------------------------------------------------------------------
-- usp_BoTraderContract_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderContract_Save]
GO

CREATE PROC dbo.usp_BoTraderContract_Save
	@iTraderID int,
	@iContractID int,
	@iStrategyID int -- set to 'null' to remove strategy association
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	declare @error int set @error = 0

	begin transaction
	
	if exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iContractID)		
		update TraderContract
			set strategyID = @iStrategyID
		where
			traderID = @iTraderID 
			and contractID = @iContractID
			and actionID < 3
	else
		insert into TraderContract
			(traderID, contractID, strategyID)
		values
			(@iTraderID, @iContractID, @iStrategyID)
			
	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end			

	commit transaction
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrade_StoreToHistoryAndClear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrade_StoreToHistoryAndClear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrade_StoreToHistoryAndClear]
GO

CREATE PROC dbo.usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	-- clear simulated and manual deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0)
	if @@error !=0 begin set @error = -2 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (0, 3) and actionID = 0)
	if @@error !=0 begin set @error = -4 if @@TranCount = 1 Rollback Tran goto finish end

	if isnull(@tiStoreToHistory, 0) <> 0
	begin
		-- move actual and actual matched trades to history
		insert into TradeHistory
		select
			getdate() as dtActionDate,
	
			tr.tradeID as iTradeID, tr.tradeDate as dtTradeDate, tr.quantity as iQuantity,
			tr.price as fPrice, tr.isBuy as tiIsBuy, tr.status as tiStatus, tr.execID as vcExecID,
			tr.mark as iTradeMark,
			
			c.contractID as iContractID, c.contractTypeID as iContractTypeID, c.Symbol as vcSymbol,
			c.exportSymbol as vcExportSymbol, cp.priceClose as fPriceClose,
	
			uc.contractID as iUnderlyingContractID, uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol, uc.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			ucp.priceClose as fUnderlyingPriceClose,
	
			f.futureID as iFutureID, fr.futureRootSymbol as vcFutureRootSymbol,
			fc.symbol as vcFutureSymbol, fc.exportSymbol as vcFutureExportSymbol,
			fmc.expiryDate as dtFutureMaturity, fr.futureLotSize as iFutureLotSize,
			fcp.priceClose as fFuturePriceClose,
	
			ort.optionRootID as iOptionRootID, ort.symbol as vcOptionRootSymbol, ort.lotSize as iOptionRootLotSize,
	
			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtOptionExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fOptionStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiOptionIsCall,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference,

			fr.futureRootID as iFutureRootID,
			fr.optionLotSize as iFutureOptionLotSize
	
		from TradeView tr
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join StockView s on tr.underlyingID = s.stockID
			left join IndexView i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- additional
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
	
		set @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 if @@TranCount = 1 Rollback Tran goto finish end
	end

	-- clear actual and actual matched trades
	delete from Trade 
		where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0
	if @@error !=0 begin set @error = -7 if @@TranCount = 1 Rollback Tran goto finish end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmStrategy_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmStrategy_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmStrategy_Save]
GO

CREATE PROC dbo.usp_MmStrategy_Save
    @iStrategyID int,
    @vcStrategyName varchar(12),
    @vcDescription varchar(120)
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
   
	if @iStrategyID is null
	begin
		select @iStrategyID = strategyID 
		from StrategyView 
		where strategyName = @vcStrategyName
	
		if @iStrategyID is not null
			goto finish
		
		Begin Transaction
	    
	    insert into Strategy (StrategyName, [Description])
	        values (@vcStrategyName, @vcDescription)
		if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		set @error = @@identity
		
		Commit Transaction
	end 	
	else 
	begin
		if exists(select * from StrategyView where @vcStrategyName = strategyName and strategyID <> @iStrategyID)
		begin
			set @error = -2
			goto finish
		end
		
		Begin Transaction
	
	    update Strategy 
		set StrategyName = isnull(@vcStrategyName, StrategyName),
			[Description] = isnull(@vcDescription, [Description])
	    where strategyID = @iStrategyID
	    
		if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		Commit Transaction
	end

finish:
	if @error = 0
		return @iStrategyID
	else
		return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrader_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrader_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrader_Save]
GO

CREATE PROC dbo.usp_BoTrader_Save
    @iTraderID int,
    @vcAcronym varchar(12) = null,
    @vcTraderName varchar(120) = null,
    @vcClearingTraderCode varchar(11) = null,
    @iTraderGroupID int = null, -- null group == Default group !!!
    @iDefStrategyID int = null, -- null strategy == no default strategy
	@tiRaiseError tinyint = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if exists (select 1 from TraderView where TraderName = @vcTraderName and (@iTraderID is null or @iTraderID <> traderID))
		begin
			if isnull(@tiRaiseError, 0) <> 0 raiserror ('Trader with this name already exists!', 16, 1)
			set @error = -1
			goto finish
		end
   
	if exists (select 1 from TraderView where Acronym = @vcAcronym and (@iTraderID is null or @iTraderID <> traderID))
		begin
			if isnull(@tiRaiseError, 0) <> 0 raiserror ('Trader with this acronym already exists!', 16, 1)
			set @error = -2
			goto finish
		end   

	begin transaction
	
	if @iTraderID is null
	begin
		-- insert new trader
		insert into Trader (Acronym, TraderName, clearingTraderCode, traderGroupID, defStrategyID)
			values (@vcAcronym, @vcTraderName, @vcClearingTraderCode, @iTraderGroupID, @iDefStrategyID)

		if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		set @iTraderID = @@identity
	end
	else
	begin
		-- update trader
		update Trader 
        set acronym = isnull(@vcAcronym, acronym),
	        traderName = isnull(@vcTraderName, traderName),
			clearingTraderCode = isnull(@vcClearingTraderCode, clearingTraderCode),
			traderGroupID = @iTraderGroupID,
			defStrategyID = @iDefStrategyID
	    where traderID = @iTraderID

		if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	end

	commit transaction

finish:
	if @error = 0
		return @iTraderID
	else
		return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Exec
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Exec]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Exec]
GO

CREATE PROC dbo.usp_Trade_Exec
	@tiExercise tinyint,
	@iOptTradeID int,
	@iOptQuantity int,
	@tiOptIsBuy tinyint,
	@tiOptStatus tinyint,
	@fOptBrokerCommission float,
	@fOptClearingBrokerCommission float,
	@iUndQuantity int,
	@fUndPrice float,
	@tiUndIsBuy tinyint,
	@tiUndStatus tinyint,
	@fUndBrokerCommission float,
	@fUndClearingBrokerCommission float,
	@tiIsFutures tinyint,
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

	select @iNewOptSeqNum = isnull(max(seqNum), 0) + 1 from Trade

	if @dtNewOptTradeDate is null	
		set @dtNewOptTradeDate = dateadd(minute, @iDeltaGMT, getdate())

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	select @iNewOptTradeID, 1, contractID, @dtNewOptTradeDate, @iOptQuantity, 0, @tiOptIsBuy,
		traderID, strategyID, brokerID, @fOptBrokerCommission, clearingBrokerID, @fOptClearingBrokerCommission,
		tradedIV, spotReference, 0, @tiOptStatus, null, @iNewOptSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID
	from Trade
	where tradeID = @iOptTradeID and actionID = 1

	if @@error <> 0 set @bSucceed = 0

	if @bSucceed = 1 and isnull(@tiExercise, 0) <> 0
	begin
		select @iNewUndTradeID = max(tradeID) + 1 from Trade
			if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
		select @iNewUndSeqNum = isnull(max(seqNum), 0) + 1 from Trade
	
		if @dtNewUndTradeDate is null	
			set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	
		if isnull(@tiIsFutures, 0) = 0
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select @iNewUndTradeID, 1, underlyingID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
				underlyingID, null, null, null
			from Trade
			where tradeID = @iOptTradeID and actionID = 1
		else
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select @iNewUndTradeID, 1, futureID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
				underlyingID, null, futureRootID, futureID
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

/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.113')
	begin
		update DataInfo set keyValue = '3.45.113' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.113' '3.45.113 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
