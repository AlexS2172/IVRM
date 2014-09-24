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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.233'  or (keyValue = '3.46.234') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.234 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.234' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
---------------------------------------------------------------------------------------------------------------------------


IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmTradeByTrader_PriceInfo_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmTradeByTrader_PriceInfo_Get
GO

CREATE   PROC usp_MmTradeByTrader_PriceInfo_Get
	@iTraderID int = null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

--			uc.contractID as iUnderlyingContractID,

--			case when c.contractTypeID = 3 then o.expiryDate else  fo.expiryDate end as dtExpiry,
--			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
--			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
--			ort.optionRootID as iOptionRootID,

--			fr.futureRootID as iFutureRootID,
--			f.futureID as iFutureID,
--			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
--			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
--			left join [Option] o on tr.contractID = o.optionID
--			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
--			left join ContractView fc on tr.futureID = fc.contractID
--			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
--			left join Future f on tr.futureID = f.futureID
--			left join FutureOption fo on tr.contractID = fo.futureOptionID
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

--			uc.contractID as iUnderlyingContractID,

--			case when c.contractTypeID = 3 then o.expiryDate else  fo.expiryDate end as dtExpiry,
--			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
--			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
--			ort.optionRootID as iOptionRootID,

--			fr.futureRootID as iFutureRootID,
--			f.futureID as iFutureID,
--			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
--			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
--			left join [Option] o on tr.contractID = o.optionID
--			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
--			left join ContractView fc on tr.futureID = fc.contractID
--			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
--			left join Future f on tr.futureID = f.futureID
--			left join FutureOption fo on tr.contractID = fo.futureOptionID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error
GO



IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmUnderlying_PriceInfo_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmUnderlying_PriceInfo_Get
GO

CREATE           PROC usp_MmUnderlying_PriceInfo_Get
	@iTraderID int = null
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
--			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
--			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
--		    i.yield as fYield,
--			i.isBasket as tiIsBasket,
--		    s.divFreq as iDivFreq,
--		    s.divDate as dtDivDate,
--		    s.divAmt as fDivAmt,
		
--		    s.isDivCustom as tiIsDivCustom,
--			case when i.indexID is null then s.isDivCustom else i.isDivCustom end as tiIsDivCustom,
--		    s.divFreqCustom as iDivFreqCustom,
--		    s.divDateCustom as dtDivDateCustom,
--		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,
		    cp.priceTheo as fPriceTheoClose,

--			null as iTraderID,

--			case when i.indexID is null then s.skew else i.skew end as fSkew,
--			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
--			case when i.indexID is null then s.isHedgeSymbol else i.isHedgeSymbol end as tiIsHedgeSymbol,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
--			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration
--		    i.SOQ As SOQ,
--		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID,
--		    mp.manualPrice as dManualActivePrice,
--		    vsd.IsManualVol as fManualVol
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
--		    left join StockView s on s.stockID = c.contractID
--		    left join IndexView i on i.indexID = c.contractID
--		    left join ManualPrice mp on mp.contractID = c.contractID
--		    left join VolaSurfaceData vsd on vsd.contractID = c.contractID 
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null
	else
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
--			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
--			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
--		    i.yield as fYield,
--			i.isBasket as tiIsBasket,
--		    s.divFreq as iDivFreq,
--		    s.divDate as dtDivDate,
--		    s.divAmt as fDivAmt,
		
--		    s.isDivCustom as tiIsDivCustom,
--			case when i.indexID is null then s.isDivCustom else i.isDivCustom end as tiIsDivCustom,
--		    s.divFreqCustom as iDivFreqCustom,
--		    s.divDateCustom as dtDivDateCustom,
--		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

--			tc.traderID as iTraderID,

--			case when i.indexID is null then s.skew else i.skew end as fSkew,
--			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
--			case when i.indexID is null then s.isHedgeSymbol else i.isHedgeSymbol end as tiIsHedgeSymbol,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
--			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration
--		    i.SOQ As SOQ,
--		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID,
--		    mp.manualPrice as dManualActivePrice,
--		    vsd.IsManualVol as fManualVol
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
--		    left join StockView s on s.stockID = c.contractID
--		    left join IndexView i on i.indexID = c.contractID
--		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
--		    left join ManualPrice mp on mp.contractID = c.contractID
--		    left join VolaSurfaceData vsd on vsd.contractID = c.contractID 
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null

	return @@error
GO



---------------------------------------------------------------------------------------------------------------------------
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.234')
	begin
		update DataInfo set keyValue = '3.46.234' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.234' '3.46.234 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




