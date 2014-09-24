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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.190'  or (keyValue = '3.46.191') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.191 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.191' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

ALTER   VIEW CustomStrikeSkewPointView

AS

            SELECT 
                        customStrikeSkewPointID,
                        volaSurfaceDataID,
                        strike,
                        type,
                        volatility,
                        expiryID,
                        isBasePoint,
                        actionID,
                        actionDate
            FROM
                        CustomStrikeSkewPoint
            WHERE
                        actionID < 3 

GO

ALTER    PROC usp_MmTradeByTrader_Get
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

			uc.contractID as iUnderlyingContractID,

			-- case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then cast( o.expiryID as datetime ) else cast( fo.expiryID as datetime ) end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and 
			--	fec.expCalendarID = /*case when uc.contracttypeid = 6 then c.expCalendarID else*/ fr.matCalendarID /*end*/
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

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then cast( o.expiryID as datetime) else cast( fo.expiryID as datetime) end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and 
			--	fec.expCalendarID = /*case when uc.contracttypeid = 6 then c.expCalendarID else*/ fr.matCalendarID /*end*/
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error

GO

ALTER      PROC dbo.usp_MmOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		-- ec.expiryDate as dtExpiry,
		cast( o.expiryID as datetime ) as dtExpiry,
		o.isCall as tiIsCall,
		ort.isFitEnabled as tiIsFitEnabled,
		ort.lotSize as iLotSize,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,
		o.isManualyEntered as iIsFlex			

	from ContractView uc
		inner join OptionRoot ort on uc.contractID = ort.underlyingID
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on o.optionID = c.contractID
		-- inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
	where uc.contractID = @iUnderlyingID
		and o.expiryID >= cast( @dtMinExpiry as int )
		-- and ec.expiryDate >= @dtMinExpiry
	--order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	return @@error

GO

ALTER   PROC dbo.usp_MmContractForTradeByUnd_Get
	@iUnderlyingID int
as
-- changed by VK - flex option support added
	-- return 2 recordsets: 1st for option contracts, 2nd futures & futures options contracts
	set nocount on

	declare @dtToday datetime

	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	-- options rowset
	select	
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.contractName as vcContractName, 
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose,
		
		-- ec.expiryDate as dtExpiry,
		cast( o.expiryID as datetime ) as dtExpiry,
		o.strike as fStrike,
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID
	from OptionRootView ort
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on c.contractID = o.optionID
		inner join Contract uc on uc.contractID = ort.underlyingID
		-- inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		-- and ec.expiryDate >= @dtToday
		and o.expiryID >= cast( @dtToday as int )
	-- order by year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
	order by o.expiryID, o.strike, o.isCall desc

	-- futures & futures options rowset
	select * from
	(
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			c.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join ContractView c on c.contractID = f.futureID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			-- ec.expiryDate as dtExpiry,
			cast( o.expiryID as datetime ) as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			fc.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join Contract fc on fc.contractID = f.futureID
			inner join FutureOption o on o.futureID = f.futureID
			inner join ContractView c on c.contractID = o.futureOptionID
			-- inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = fc.expCalendarID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
			-- and ec.expiryDate >= @dtToday
			and o.expiryID >= cast( @dtToday as int )
	) as c order by c.vcFutureSymbol, year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.tiIsCall desc

	return @@error


GO

ALTER    PROC dbo.usp_MmTradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null
AS
-- changed by VK - flex option support added
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
		select
			tr.seqNum as iSeqNum,
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
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.exportSymbol as vcImportID,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			-- case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then cast( o.expiryID as datetime ) else cast( fo.expiryID as datetime ) end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		order by tr.seqNum
	else
		select
			tr.seqNum as iSeqNum,
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
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			-- case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then cast( o.expiryID as datetime ) else cast( fo.expiryID as datetime ) end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum



GO

ALTER   PROCEDURE dbo.usp_OrderByTrader_Get
	@iTraderID int = null
AS
/*
	Created by Sharky
*/
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iTraderID is null
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			c.contractTypeID < 3
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			-- ec.expiryDate as dtExpiry,
			cast(o.expiryID as datetime) as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			-- left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull(c.expCalendarID, uc.expCalendarID )
		where
			c.contractTypeID = 3
	end
	else
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			c.contractTypeID < 3
			and ord.traderID = @iTraderID
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			-- ec.expiryDate as dtExpiry,
			cast(o.expiryID as datetime) as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			--left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull(c.expCalendarID, uc.expCalendarID )
		where
			c.contractTypeID = 3
			and ord.traderID = @iTraderID
	end

return @@error

GO

ALTER   PROC dbo.usp_CustomStrikeSkewPoint_Get 
            @iCustomStrikeSkewPointID INT = NULL, 
            @iVolaSurfaceDataID INT = NULL,
            @iType INT = NULL,
            @dtExpDate DATETIME = NULL
AS
            ------------------------------------------------
            SET NOCOUNT ON
            ------------------------------------------------
            DECLARE @error INT               SET @error = -1
            DECLARE        @iExpiryID INT
 
            -- Sharky: faster request for special parameters' set
            IF @iExpiryID IS NULL AND @iCustomStrikeSkewPointID IS NULL
                        AND @iVolaSurfaceDataID IS NOT NULL AND @iType IS NOT NULL
            BEGIN
                        SELECT 
                                   CSSP.customStrikeSkewPointID AS iCustomStrikeSkewPointID,
                                   CSSP.volaSurfaceDataID AS iVolaSurfaceDataID,
                                   CSSP.strike AS fStrike, 
                                   CSSP.type AS iType,
                                   CSSP.volatility AS fVolatility,
                                   CAST( CSSP.expiryID AS DATETIME ) AS dtExpDate,
                                   CSSP.isBasePoint AS iIsBasePoint
                        FROM 
                                   CustomStrikeSkewPoint CSSP WITH(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, NOLOCK)
                        WHERE 
                                   CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
                                   AND CSSP.type = @iType
                                   AND CSSP.actionID < 3
            END
            ELSE
            BEGIN
                        -- Sharky: old variant
                        SET @iExpiryID = CAST( @dtExpDate AS INT)
 
                        SELECT 
                                   CSSP.customStrikeSkewPointID AS iCustomStrikeSkewPointID,
                                   CSSP.volaSurfaceDataID AS iVolaSurfaceDataID,
                                   CSSP.strike AS fStrike, 
                                   CSSP.type AS iType,
                                    CSSP.volatility AS fVolatility,
                                   CAST( CSSP.expiryID AS DATETIME ) AS dtExpDate,
                                   CSSP.isBasePoint AS iIsBasePoint 
                        FROM CustomStrikeSkewPointView CSSP WITH(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, NOLOCK)
                        WHERE 
                                   customStrikeSkewPointID = ISNULL(@iCustomStrikeSkewPointID, customStrikeSkewPointID) AND
                                   CSSP.volaSurfaceDataID = ISNULL(@iVolaSurfaceDataID, CSSP.volaSurfaceDataID) AND
                                   type = ISNULL(@iType, type) AND
                                   CSSP.expiryID = ISNULL(@iExpiryID, CSSP.expiryID)
            END

            SET @error = @@ERROR
            RETURN(@error)

GO

ALTER    PROC dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
-- changed by VK - flex option support added
	set nocount on
	
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				if isnull(@iUnderlyingID, 0) <> 0
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						--ec.expiryDate as dtExpiry,
						cast(o.expiryID as datetime) as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						--inner join Contract uc on ort.underlyingID = uc.contractID
						--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
					where c.contractTypeID = 3
						and ort.underlyingID = @iUnderlyingID
						and o.expiryID >= cast(@dtMinExpiry as int)
					order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
				else
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						--ec.expiryDate as dtExpiry,
						cast(o.expiryID as datetime) as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						--inner join Contract uc on ort.underlyingID = uc.contractID
						--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and o.expiryID >= cast(@dtMinExpiry as int)
					order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					--ec.expiryDate as dtExpiry,
					cast(o.expiryID as datetime) as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (o.expiryID >= cast(@dtMinExpiry as int))
				order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					--ec.expiryDate as dtExpiry,
					cast(o.expiryID as datetime) as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID
							--join ContractView uc on ort.underlyingID = uc.contractID
							inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (o.expiryID >= cast(@dtMinExpiry as int))
				order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					--ec.expiryDate as dtExpiry,
					cast(o.expiryID as datetime) as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID					
							join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (o.expiryID >= cast(@dtMinExpiry as int))
				order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					--ec.expiryDate as dtExpiry,
					cast(o.expiryID as datetime) as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					--inner join Contract uc on ort.underlyingID = uc.contractID
					--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (o.expiryID >= cast(@dtMinExpiry as int))
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryID, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					--ec.expiryDate as dtExpiry,
					cast(o.expiryID as datetime) as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (o.expiryID >= cast(@dtMinExpiry as int))
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryID, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					--ec.expiryDate as dtExpiry,
					cast(o.expiryID as datetime) as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					--inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (o.expiryID >= cast(@dtMinExpiry as int))
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryID, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					--ec.expiryDate as dtExpiry,
					cast(o.expiryID as datetime) as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					--inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (o.expiryID >= cast(@dtMinExpiry as int))
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryID, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryID, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
		end
	end

	return @@error

GO

ALTER   PROC dbo.usp_UnderlyingOptionsForOrder_Get
	@iUnderlyingID int
AS
-- changed by VK - flex option support added
	set nocount on

	select
		ort.optionRootID as iOptionRootID,
		o.optionID as iOptionID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		--ec.expiryDate as dtExpiry,
		cast(o.expiryID as datetime) as dtExpiry
	from OptionRootView ort
		left join OptionView o on ort.optionRootID = o.optionRootID
		left join ContractView c on c.contractID = o.optionID
		--left join Contract uc on ort.underlyingID = uc.contractID
		--left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
	where
		ort.underlyingID = @iUnderlyingID
		and c.contractTypeID = 3
		and o.expiryID >= cast(getdate() as int)
	order by ort.underlyingID, o.expiryID,/*year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc

Return (0)


GO

ALTER        Procedure dbo.usp_OptionVM_Get
	@iOptionID int = null,			-- option's identifier
	@iUnderlyingID int,				-- underlying's identifier
	@dtExpDateMin datetime = null,	-- expiration's date
	@bIsCall bit = null,
	@iSortOrder int = 1	--			-- 1, 2 or 3. see comments below
as
/*
	Created by Andrey Sinkin
	This procedure returns option's data
*/
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @dtExpDateMin is null set @dtExpDateMin = 0

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID,
				isCall as bIsCall, 
				strike as fStrike, 
				--EC.expiryDate as  dtExpDate,
				cast(O.expiryID as datetime) as dtExpDate,
				C.symbol as vcSymbolName, 
				null as iExchangeID,
				C.contractName as vcDescription,
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic
				from OptionView O
						join ContractView C on O.OptionID = C.contractID
						join ContractPrice CP on CP.contractID = C.contractID and exchangeID is null
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						--join Contract C1 on ORt.underlyingID = C1.contractID
						--join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
				where ORt.underlyingID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and (O.expiryID >= cast(@dtExpDateMin as int))
						and (@bIsCall is null or O.isCall = @bIsCall)
						and  O.isManualyEntered  = 0
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike

		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				cast(O.expiryID as datetime) as dtExpDate,
				C.symbol as vcSymbolName, 
				null iExchangeID,
				C.contractName as vcDescription, 
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					--join Contract C1 on ORt.underlyingID = C1.contractID
					--join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and (O.expiryID >= cast(@dtExpDateMin as int))
					and (@bIsCall is null or O.isCall = @bIsCall)
					and  O.isManualyEntered  = 0
			order by isCall, O.expiryID, /*year(EC.expiryDate),  month(EC.expiryDate),*/ strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				cast(O.expiryID as datetime) as dtExpDate,
				C.symbol vcSymbolName, 
				null iExchangeID,
				C.contractName vcDescription,	
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize iLotSize,
				ORt.isSynthetic as tiIsSynthetic
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					--join Contract C1 on ORt.underlyingID = C1.contractID
					--join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and (O.expiryID >= cast(@dtExpDateMin as int))
					and (@bIsCall is null or O.isCall = @bIsCall)
					and  O.isManualyEntered  = 0
			order by O.expiryID, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
	set @error = @@error
Return(@error)

GO

ALTER            Procedure dbo.usp_CustomStrikeSkewPoint_Save 
	@iCustomStrikeSkewPointID int = Null output,
	@iVolaSurfaceDataID int = Null,
	@iType int = Null,
	@fStrike float = Null,
	@fVolatility float = Null,
	@dtExpDate datetime = Null,
	@iIsBasePoint int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into CustomStrikeSkewPoint table
*/
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	DECLARE @expInCalendarID AS INT
	declare	@iExpiryID int

	set @iExpiryID = cast(@dtExpDate as int)

	if exists (
		select * 
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and 
			type = @iType and expiryID = @iExpiryID and actionID < 3 and 
			(@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null)
	)
	begin
		if @iCustomStrikeSkewPointID is null
			select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
			from CustomStrikeSkewPoint 
			where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and 
				type = @iType and expiryID = @iExpiryID and actionID < 3
			else if @iCustomStrikeSkewPointID is not null 
				begin
					Raiserror ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
					Return (-1)
				end
		end 
	
	DECLARE @errStrk1 AS INT
	DECLARE @errStrk2 AS INT

	IF @dtExpDate IS NULL
	BEGIN
		SET @errStrk1 = CAST( @fStrike AS INT ) 
		SET @errStrk2 = CAST( ( @fStrike - FLOOR( @fStrike ) ) * 10 AS INT )
		RAISERROR ('Expiration date was not supplied for strike %d.%d, VolSurf %d', 16, 1, @errStrk1, @errStrk2 , @iVolaSurfaceDataID ) WITH LOG
		RETURN -1
	END			

-- 	SELECT @expInCalendarID = expiryInCalendarID
-- 	FROM ExpiryInCalendarView 
-- 	WHERE expiryDate = @dtExpDate

	Begin Transaction

	if @iCustomStrikeSkewPointID is null 
	begin
		insert into CustomStrikeSkewPoint ( volaSurfaceDataID, type, strike, volatility, expiryID, 
							isBasePoint) 
			values (@iVolaSurfaceDataID, @iType, @fStrike, @fVolatility,  @iExpiryID, 
							@iIsBasePoint)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iCustomStrikeSkewPointID=@@IDENTITY
	end
	else
	begin
		update CustomStrikeSkewPoint set 
			volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
			type = isnull(@iType,type),
			strike = isnull(@fStrike,strike),
			volatility = isnull(@fVolatility,volatility),
			expiryID = isnull(@iExpiryID,@iExpiryID),
			isBasePoint = isnull(@iIsBasePoint,isBasePoint)
		where customStrikeSkewPointID=@iCustomStrikeSkewPointID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	end
	Commit Transaction

	set @error = 0
finish:

Return(@error)

GO

ALTER     PROC dbo.usp_BoCustomStrikeSkewPoint_Default_Save 
	@iContractID int,
	@fStrike float,
	@iExpiryID int,
	@fVolatility float
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	declare @iType int
		set @iType = 2 --OTM_IVTYPE

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
-- VK on
	--DECLARE @expInCalID AS INT
	--SELECT  @expInCalID = EIC.expiryInCalendarID
	--FROM volaSurfaceData vsd
	--JOIN Contract c ON c.contractID = vsd.contractID
	--JOIN ExpiryInCalendar EIC on EIC.expiryID = @iExpiryID AND EIC.expCalendarID = c.expCalendarID
	--WHERE vsd.volaSurfaceDataID = @iVolaSurfaceDataID
-- VK off
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID)
	
				if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,expiryID )
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

finish:
	Return(@error)

GO

ALTER       procedure usp_CustomStrikeSkewDefaultPoint2_Save
	@iContractID int,
	@iExpiryID int,
	@iType int,
	@fVolatility float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @RootType int

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	if @iVolaSurfaceDataID is not null
	begin
		exec @RootType =  usp_RootType_Get @iContractID
		-- option root
		if @RootType = 0 
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
					select distinct 
						@iVolaSurfaceDataID, round(ov.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
					from OptionView ov
						join OptionRootView ort on ov.optionRootID = ort.optionRootID
					where ov.expiryID = @iExpiryID 
						and ort.underlyingID = @iContractID
						and not exists(select 1 
										from CustomStrikeSkewPointView cssp
										where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
												round(cssp.strike, 4) = round(ov.strike, 4)
												and cssp.expiryID = ov.expiryID)
	
		end
		

		-- future option root
		if @RootType = 1 
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
				select distinct 
					@iVolaSurfaceDataID, round(fo.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
				from    dbo.Future f inner JOIN
				        dbo.FutureOption fo  on f.futureID = fo.futureID inner JOIN
				                      dbo.Expiry e ON fo.expiryID = e.expiryID
				where   f.MaturityID = @iExpiryID 
					and f.futureID = @iContractID
					and not exists(select 1 
									from CustomStrikeSkewPointView cssp
									where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
											round(cssp.strike, 4) = round(fo.strike, 4)
											and cssp.expiryID = fo.ExpiryID)

		end
	
	end

GO

ALTER     Procedure usp_CustomStrikeSkewPoint_Copy 
	@iVolaSurfaceDataIDSrc int,
	@iVolaSurfaceDataIDDest int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is move information inside CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint
			into #SourcePoints
			from CustomStrikeSkewPoint
			where volaSurfaceDataID=@iVolaSurfaceDataIDSrc
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		delete CustomStrikeSkewPoint where volaSurfaceDataID=@iVolaSurfaceDataIDDest
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		insert into CustomStrikeSkewPoint (volaSurfaceDataID,strike,type,volatility,expiryID,isBasePoint)
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint
				from #SourcePoints
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
	drop table #SourcePoints
Return(@error)

GO


ALTER     Procedure usp_EodDefaultIvPoint_Save 
	@iContractID int,
	@fStrike float,
	@iExpiryID int,
	@fVolatility float
AS
/*
	Created by Michael Malyshkin
	Description: this procedure save Default Iv points
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	declare @iType int
		set @iType = 2 --OTM_IVTYPE

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	-- mark olf strile skew points as deleted
	--update CustomStrikeSkewPoint set actionID = 3
	delete from CustomStrikeSkewPoint
	where 
		volaSurfaceDataID = @iVolaSurfaceDataID and
		actionID < 3 and
		datediff(d, actionDate, getDate())>0

	--DECLARE @expInCalID AS INT 
	--SELECT @expInCalID  = expCalendarID FROM Contract WHERE contractID = @iContractID		
		
	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
			--and expiryInCalendarID = @expInCalID
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
Return(@error)

GO

ALTER     PROC usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0	

	select
		f.futureID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
		--e.ExpiryPeriod as vcMaturity,
                cast(f.maturityID as datetime) as dtMaturity,
		fu.ContractId as iFutureUnderlyingID,
                f.futureRootID as iFutureRootID,
		isnull(ff.tiGreeksCalculationType, 1) as tiGreeksCalculationType,
		cp.priceClose as fPriceClose
		
	from ContractView fu
                inner join FutureRoot fr on fr.underlyingID = fu.ContractId
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.futureID = fc.ContractId
--                inner join Expiry e on e.expiryID = f.MaturityID
                --inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		left join ContractPrice cp on f.futureID = cp.ContractID
		left join FutureUnderlying ff on ff.iFutureUnderlyingID = fu.ContractId
	where fu.ContractId = @iUnderlyingID
		and f.maturityID >= cast(@dtMinExpiry as int)

	return @@error


GO

ALTER     PROC usp_MmFuture_Get
	@iFutureRootID int = null
AS
	-- retrieve futures by future root
	set nocount on

	if @iFutureRootID is null
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			fr.underlyingID as iUnderlyingID,
			fcp.priceClose as fPriceClose,
			f.maturityID as iMaturityID,
			cast(f.maturityID as datetime) as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
,			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			--inner join ContractView uc on uc.contractID = fr.underlyingID
			--inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fc.expCalendarID --fr.matCalendarID
			left join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.underlyingID
	else
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			fr.underlyingID as iUnderlyingID,
		    fcp.priceClose as fPriceClose,
			f.maturityID as iMaturityID,
			cast(f.maturityID as datetime) as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
,			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			--inner join ContractView uc on uc.contractID = fr.underlyingID
			--inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fc.expCalendarID --fr.matCalendarID
			left join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.underlyingID
		where
			f.futureRootID = @iFutureRootID


GO

ALTER   PROC usp_MmFutureOptionsByUnderlying_Get
            @iUnderlyingID int,
            @dtMinExpiry datetime = Null
as
            -------------------------------------------------
            set nocount on
            -------------------------------------------------
            if @dtMinExpiry is null 
                        begin
                                   select
                                               foc.contractID as iContractID,
                                               foc.contractTypeID as iContractTypeID,
                                               foc.symbol as vcSymbol,
                                               fo.strike as fStrike, 
                                               cast(fo.expiryID as datetime) as dtExpiry,
                                               fo.isCall as tiIsCall,
                                               fo.futureID as iFutureID
                                   from FutureOption fo
                                               --inner join ContractView fc on f.futureID = fc.contractID
                                               inner join Future f on f.futureID = fo.futureID
                                               inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
                                               --inner join FutureOption fo on f.futureID = fo.futureID
                                               inner join ContractView foc on fo.futureOptionID = foc.contractID
                                               --inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID
                        end
            else
                        begin
                                   select
                                               foc.contractID as iContractID,
                                               foc.contractTypeID as iContractTypeID,
                                               foc.symbol as vcSymbol,
                                               fo.strike as fStrike, 
                                               cast(fo.expiryID as datetime) as dtExpiry,
                                               fo.isCall as tiIsCall,
                                               fo.futureID as iFutureID
                                   from FutureOption fo
                                               inner join Future f on f.futureID = fo.futureID
                                               --inner join ContractView fc on f.futureID = fc.contractID
                                               inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
                                               --inner join FutureOption fo on f.futureID = fo.futureID
                                               inner join ContractView foc on fo.futureOptionID = foc.contractID
                                               --inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID
                                   where  fo.expiryID >= cast(@dtMinExpiry as int)
                        end 
 
            return @@error

 
GO

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.191')
	begin
		update DataInfo set keyValue = '3.46.191' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.191' '3.46.191 update message:' 
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