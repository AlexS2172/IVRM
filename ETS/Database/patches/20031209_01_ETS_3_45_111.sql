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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.110' or (keyValue = '3.45.111')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.111 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.111' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_BoFuture_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Get]
GO

CREATE PROC dbo.usp_BoFuture_Get
	@iFutureID int = null
AS
	-- retrieve future

	set nocount on

	if @iFutureID is not null
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		where
			f.futureID = @iFutureID
	else
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		order by
			fc.symbol
GO

----------------------------------------------------------------------------------------------------
-- usp_BoPositionForReconcile_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoPositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoPositionForReconcile_Get]
GO

CREATE PROC dbo.usp_BoPositionForReconcile_Get
	@lastUTCTradeDate datetime = null
AS
	set nocount on

	if @lastUTCTradeDate is null set @lastUTCTradeDate = getdate()

	select 
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		cp.priceClose as fPriceClose,

		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,

		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		case when c.contractTypeID < 3 then 1
			when c.contractTypeID = 3 then ort.lotSize
			when c.contractTypeID = 4 then fr.futureLotSize
			when c.contractTypeID = 5 then fr.optionLotSize
		end as iLotSize,

		case when c.contractTypeID = 3 then o.expiryID else fo.expiryID end as iExpiryID,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,

		trdr.TraderID as iTraderID,
		trdr.acronym as vcAcronym,
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

		tr.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.exportSymbol as vcFutureRootExportSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		f.maturityID as iFutureMaturityID, 
		fr.futureLotSize as iFutureLotSize,
		fr.optionLotSize as iFutureOptionLotSize
	from TradeView tr 
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- trader and strategy
		left join TraderView trdr on tr.traderID = trdr.traderID
		left join StrategyView st on st.strategyID = tr.strategyID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where tr.status in (0, 3)
		and datediff(day, tr.tradeDate, @lastUTCTradeDate) >= 0
	group by c.contractID, c.contractTypeID, c.Symbol, ort.optionRootID, ort.symbol,
		o.expiryID, o.strike, o.isCall, fo.expiryID, fo.strike, fo.isCall, 
		uc.contractID, uc.contractTypeID, uc.Symbol,
		ort.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym, tr.strategyID, st.strategyName,
		fr.futureRootID, f.futureID, f.maturityID, fr.futureLotSize, fr.optionLotSize,
		fr.futureRootSymbol, fr.exportSymbol, fc.symbol
	order by vcSymbol
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTradeNew_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeNew_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTradeNew_Get]
GO

CREATE PROC dbo.usp_BoTradeNew_Get
	@dtLastTradeDateUTC datetime = null -- return all trades were made earlier or at @dtLastTradeDateUTC
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		tr.tradeID as iTradeID,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,

		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		case when c.contractTypeID = 3 then o.expiryID else fo.expiryID end as iExpiryID,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		case when c.contractTypeID < 3 then 1
			when c.contractTypeID = 3 then ort.lotSize
			when c.contractTypeID = 4 then fr.futureLotSize
			when c.contractTypeID = 5 then fr.optionLotSize
		end as iLotSize,

		t.traderID as iTraderID,
		t.acronym as vcTraderName,

		br.brokerID as iBrokerID,
		br.brokerName as vcBrokerName,
		tr.brokerCommission as fBrokerCommission,

		tr.mark as iTradeMark,

		st.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		trg.trgID as iTraderGroupID,
		trg.trgName as vcTraderGroupName,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.exportSymbol as vcFutureRootExportSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		fc.exportSymbol as vcFutureExportSymbol,
		f.maturityID as iFutureMaturityID, 
		fr.futureLotSize as iFutureLotSize,
		fr.optionLotSize as iFutureOptionLotSize
	from
		TradeView tr
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- trade data
		left join TraderView t on tr.traderID = t.traderID
		left join StrategyView st on st.strategyID = tr.strategyID
		left join TraderGroup trg on t.traderGroupID = trg.trgID
		left join BrokerView br on tr.brokerID = br.brokerID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where
		tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and len(isnull(tr.execID, '')) = 0 -- only non-RTTF trades should be reported (make sure that all RTTF trades has execID)
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTradeNewContract_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeNewContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTradeNewContract_Get]
GO

CREATE PROC dbo.usp_BoTradeNewContract_Get
	@dtLastTradeDateUTC datetime = null -- return all contracts for trades were made earlier or at @dtLastTradeDateUTC
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		case when c.contractTypeID = 3 then o.expiryID else fo.expiryID end as iExpiryID,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.exportSymbol as vcFutureRootExportSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		fc.exportSymbol as vcFutureExportSymbol,
		f.maturityID as iFutureMaturityID
	from
		TradeView tr
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where
		tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
		and c.contractID not in (select contractID from TradeView 
									where isnull(isPosition, 0) = 1)
	group by
		c.contractID, c.contractTypeID, c.symbol, c.exportSymbol, o.expiryID,
		o.strike, o.isCall, fo.expiryID, fo.strike, fo.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
		uc.exportSymbol, ort.optionRootID, ort.symbol,
		fr.futureRootID, fr.futureRootSymbol, fr.exportSymbol, f.futureID,
		fc.symbol, fc.exportSymbol, f.maturityID
GO

/* RESULTS ANALYTHING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.111')
	begin
		update DataInfo set keyValue = '3.45.111' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.111' '3.45.111 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
