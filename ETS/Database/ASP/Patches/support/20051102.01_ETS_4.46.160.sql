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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.159'  or (keyValue = '3.46.160') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.160 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.160' as  'Version in script' from DataInfo where dataInfoID = 1
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractPriceView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractPriceView]
GO





CREATE    VIEW [dbo].[ContractPriceView] WITH SCHEMABINDING 
	AS SELECT  [dbo].[contract].[contracttypeid] as ContractTypeId, 
                   [dbo].[contract].[ActionID] as ActionID,  
                   [dbo].[contractprice].[ExchangeId] as ExchangeId,  
                   [dbo].[contract].[contractid] as contractid,  
                   [dbo].[contract].[symbol] as Symbol,  
                   [dbo].[contract].[expcalendarid] as expcalendarid,
                   [dbo].[contract].[contractname] as ContractName,  
                   [dbo].[contract].[undpriceprofileid] as undpriceprofileid,  
                   [dbo].[contract].[optpriceprofileid] as optpriceprofileid,  
		   [dbo].[contractprice].priceTheo as priceTheo,
                   [dbo].[contractprice].[priceclose] as priceclose,  
                   [dbo].[contractprice].[volume10day] as volume10day,
                   [dbo].[contractprice].[volume5expiration] as volume5expiration,
                   [dbo].[contractprice].[ContractPriceId] as ContractPriceId 
--        FROM  [dbo].[contract],  [dbo].[contractprice]   WHERE ( [dbo].[contract].[contractid] = [dbo].[contractprice].[contractid] ) 
        FROM  [dbo].[contract]
		left JOIN [dbo].[contractprice] ON ( [dbo].[contract].[contractid] = [dbo].[contractprice].[contractid]  AND [dbo].[contractprice].exchangeID is null ) 
 




GO
ALTER     PROC dbo.usp_TradeSeq_Get
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
			tr.futureID as iFutureID
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

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			case when c.contractTypeID = 3 then c.priceTheo else fcp.priceTheo end as fPriceTheoClose,

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
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join Stock s on tr.underlyingID = s.stockID
			left join [Index] i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
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

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
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
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join Stock s on tr.underlyingID = s.stockID
			left join [Index] i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum




GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.160')
	begin
		update DataInfo set keyValue = '3.46.160' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.160' '3.46.160 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

