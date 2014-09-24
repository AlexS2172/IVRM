SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.156'  or (keyValue = '3.46.157') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.157 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.157' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
GO

ALTER  PROC dbo.usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
	declare @error  int set @error = 0
	declare @dtTrade datetime
	
	set @iStoredTradesCount = 0
	if @dtLastTradeDateUTC is null	
		set @dtTrade = DateAdd(yyyy, 25, GetUtcDate())
	else	
		set @dtTrade = @dtLastTradeDateUTC

	begin tran

	-- clear simulated and manual deleted trades
	delete from Trade 
		where status in (1, 2) and actionID = 0 --and isPosition = 0
	if @@error !=0 begin set @error = -2 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades
	delete from Trade 
		where status in (0, 3) and actionID = 0 --and isPosition = 0
	if @@error !=0 begin set @error = -4 Rollback Tran goto finish end


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
			status in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtTrade) >= 0
	
		set @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 Rollback Tran goto finish end
	end

	-- clear actual and actual matched trades
	delete from Trade 
		where status in (0, 3) and DateDiff(day, tradeDate, @dtTrade) >= 0
	if @@error !=0 begin set @error = -7 Rollback Tran goto finish end

	commit tran

finish:
	return @error


GO
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.157')
	begin
		update DataInfo set keyValue = '3.46.157' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.157' '3.46.157 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

