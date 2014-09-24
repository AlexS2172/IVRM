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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.254' or (keyValue = '4.46.255') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.255 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.255' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





ALTER        PROC dbo.usp_TradeSeq_Get
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null,
	@dtTradeDate datetime = null,
	@tiExtendedInfo tinyint = null,
	@iTraderID int = null
AS
	set nocount on

	declare @dtMaxDate datetime
	if @dtTradeDate is not null set @dtMaxDate = dateadd(d, 1, @dtTradeDate)

	if @iMinSeqNum = 0 set @iMinSeqNum = Null
	if @iMaxSeqNum = 0 set @iMaxSeqNum = Null
	if @tiExtendedInfo is null or @tiExtendedInfo = 0
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.contractID as iContractID,
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
			tr.underlyingID as iUnderlyingID,
			tr.optionRootID as iOptionRootID,
			tr.futureRootID as iFutureRootID,
			tr.futureID as iFutureID,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus
		from Trade tr
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by seqNum
	else
		if @iMinSeqNum is not null and @iMaxSeqNum is not null and @dtTradeDate is null and @tiExtendedInfo is not null and @iTraderID is null and @iMinSeqNum = @iMaxSeqNum
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
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
			tr.underlyingID as iUnderlyingID,
			tr.optionRootID as iOptionRootID,
			tr.futureRootID as iFutureRootID,
			tr.futureID as iFutureID,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.priceClose as fPriceClose,

			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			uc.priceClose as fUnderlyingPriceClose,

			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,

			case when c.contractTypeID = 3 then o.expiryDate else fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			case when c.contractTypeID = 3 then c.priceTheo else fcp.priceTheo end as fPriceTheoClose,
			case when c.contractTypeID = 3 then o.expiryOV else fo.expiryOV end as dtExpiryOV,
			case when c.contractTypeID = 3 then o.TradingClose else fo.TradingClose end as dtTradingClose,
			case when c.contractTypeID < 3 then 1
				when c.contractTypeID = 3 then ort.lotSize
				when c.contractTypeID = 4 then fr.futureLotSize
				when c.contractTypeID = 5 then fr.optionLotSize
			end as iLotSize,

			ort.isSynthetic as tiIsSyntheticRoot,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join ContractPriceView c on tr.contractID = c.contractID
			inner join ContractPriceView uc on tr.underlyingID = uc.contractID
			--left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			--left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- underlying
			left join Stock s on tr.underlyingID = s.stockID
			left join [Index] i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where  tr.seqNum = @iMinSeqNum 
		order by tr.seqNum

		else
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
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
			tr.underlyingID as iUnderlyingID,
			tr.optionRootID as iOptionRootID,
			tr.futureRootID as iFutureRootID,
			tr.futureID as iFutureID,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			cp.priceClose as fPriceClose,

			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ucp.priceClose as fUnderlyingPriceClose,

			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,

			case when c.contractTypeID = 3 then o.expiryDate else fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			case when c.contractTypeID = 3 then o.expiryOV else fo.expiryOV end as dtExpiryOV,
			case when c.contractTypeID = 3 then o.TradingClose else fo.TradingClose end as dtTradingClose,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,

			case when c.contractTypeID < 3 then 1
				when c.contractTypeID = 3 then ort.lotSize
				when c.contractTypeID = 4 then fr.futureLotSize
				when c.contractTypeID = 5 then fr.optionLotSize
			end as iLotSize,

			ort.isSynthetic as tiIsSyntheticRoot,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- underlying
			left join Stock s on tr.underlyingID = s.stockID
			left join [Index] i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.255')
	begin
		update DataInfo set keyValue = '4.46.255' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.255' '4.46.255 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


