/*
	changes for RTTF feeds (SAGE & FOC)
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.38.039' or (keyValue = '3.38.040')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.38.040' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionRootHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionRootHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  PROC dbo.usp_OptionRootHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select
		optionRootID as iOptionRootID , 
		underlyingID as iUnderlyingID,
		symbol as vcSymbol,
		isSynthetic as tiIsSynthetic,
		lotSize as iLotSize,
		actionDate as dtActionDate,
		actionID as tiActionID
	from OptionRoot
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO



CREATE    PROC dbo.usp_TradeSeq_Get
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
			tr.isPosition as tiIsPosition,
--			tr.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by seqNum
	else
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
			1 as iLotSize,
			tr.isSimulated as tiIsSimulated,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			case when exists(select * from TradeLog where contractID = tr.contractID) then 1 else 0 end iStructureMissed,
			null as iOptionRootID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID < 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)

			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
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
			ORt.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
		case when exists(select * from TradeLog where contractID = UC.contractID) then 1
			 when exists(select * from TradeLog where contractID = O.optionID) then 2
		     when exists(select * from TradeLog where contractID = UC.contractID)
				and exists(select * from TradeLog where contractID = O.optionID) then 3
             else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum


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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.38.040')
	begin
		update DataInfo set keyValue = '3.38.040' where dataInfoID = 1
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
	PRINT 'DB VERSION WAS CHANGED TO 3.38.040'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
