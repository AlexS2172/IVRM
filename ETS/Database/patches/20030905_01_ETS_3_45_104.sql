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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.103' or (keyValue = '3.45.104')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.104 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.104' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- usp_BoTrade_StoreToHistoryAndClear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrade_StoreToHistoryAndClear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrade_StoreToHistoryAndClear]
GO

CREATE PROCEDURE dbo.usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
/*
	Created by Sharky
*/
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	-- clear simulated and manual deleted trades log
	delete from TradeLog 
		where seqNum in (select seqNum from Trade 
					where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0))
	if @@error != 0 begin set @error = -1 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear simulated and manual deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0)
	if @@error !=0 begin set @error = -2 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades log
	delete from TradeLog 
		where seqNum in (select seqNum from Trade
							where tradeID in (select tradeID from Trade where isnull(status, 0) in (0, 3) and actionID = 0))
	if @@error !=0 begin set @error = -3 if @@TranCount = 1 Rollback Tran goto finish end	

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
	
			c.contractID as iUnderlyingContractID, c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol, c.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			cp.priceClose as fUnderlyingPriceClose,
	
			null as iFutureID, null as vcFutureRootSymbol, null as vcFutureSymbol,
			null as vcFutureExportSymbol, null as dtFutureMaturity, null as iFutureLotSize,
			null as fFuturePriceClose,
	
			null as iOptionRootID, null as vcOptionRootSymbol, null as iOptionRootLotSize,
	
			null as dtOptionExpiry, null as fOptionStrike, null as tiOptionIsCall, null as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference
	
		from TradeView tr
			inner join ContractView c on tr.contractID = c.contractID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
			and c.contractTypeID < 3
		union
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
	
			null as iFutureID, null as vcFutureRootSymbol, null as vcFutureSymbol,
			null as vcFutureExportSymbol, null as dtFutureMaturity, null as iFutureLotSize,
			null as fFuturePriceClose,
	
			ort.optionRootID as iOptionRootID, ort.symbol as vcOptionRootSymbol, ort.lotSize as iOptionRootLotSize,
	
			ec.expiryDate as dtOptionExpiry, o.strike as fOptionStrike, o.isCall as tiOptionIsCall,
			cp.priceTheo as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference
	
		from TradeView tr
			inner join ContractView c on tr.contractID = c.contractID
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			inner join Contract uc on ort.underlyingID = uc.contractID
			join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join IndexView i on uc.contractID = i.indexID
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
			and c.contractTypeID = 3
	
		set @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 if @@TranCount = 1 Rollback Tran goto finish end

		-- clear actual and actual matched trades log
		delete from TradeLog 
		where seqNum in (select seqNum from Trade
						where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0)
		if @@error !=0 begin set @error = -6 if @@TranCount = 1 Rollback Tran goto finish end	

		-- clear actual and actual matched trades
		delete from Trade 
			where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0
		if @@error !=0 begin set @error = -7 if @@TranCount = 1 Rollback Tran goto finish end
	end

	commit tran

finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_ExpiredOptions_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiredOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExpiredOptions_Del]
GO

CREATE PROCEDURE dbo.usp_ExpiredOptions_Del
	@iContractID int -- stock or index identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Create table #Options_for_delete(OptionID int) 

	Create table #O_with_trades(symbol varchar(20)) 


	--=============================================================
	-- select options for remove
	--=============================================================
	insert into #Options_for_delete
		select O.optionID
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where ( E.expiryDate < GetDate() 
					or ( datediff(d, O.actionDate, GetDate()) > 0
					and O.isManualyEntered = 0 ))
					and O.actionID < 3
					and O.optionID not in (select distinct ContractID from Trade)
					and O.optionID not in (select distinct ContractID from [Order])
					and UC.contractID = @iContractID

	--=============================================================
	-- select options, which have a trades or orders
	--=============================================================
	insert into #O_with_trades
		select C.symbol
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C on O.optionID = C.contractID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where ( E.expiryDate < GetDate() 
					or ( datediff(d, O.actionDate, GetDate()) > 0
					and O.isManualyEntered = 0 ))
					and O.actionID < 3
					and (O.optionID in (select distinct ContractID from Trade) 
					or O.optionID in (select distinct ContractID from [Order]))
					and UC.contractID = @iContractID

	exec usp_Option_Del
		if @@error != 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

	select symbol as vcSymbol from #O_with_trades

set @error = 0

finish:
	Return(@error)

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.104')
	begin
		update DataInfo set keyValue = '3.45.104' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.104' '3.45.104 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
