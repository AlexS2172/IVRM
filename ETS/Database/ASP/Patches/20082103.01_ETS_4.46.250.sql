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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.249' or (keyValue = '4.46.250') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.250 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.250' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradesForExport_Get]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_BoTradesForExport_Get] 
GO

CREATE  procedure dbo.usp_BoTradesForExport_Get
as
	set nocount on
	select 
		tr.ContractID as lContractID,
		trd.SeqNum as lSeqNum,
		trd.ActionDate as dtActionDate,
		tr.TradeDate as dtTradeDate,
		tr.quantity as lQty,
		case when c.contractTypeID = 3 then ort.LotSize else ftr.futurelotSize end as lLotSize,
		tr.TradeID as lID,
		tr.Price as fPrice,
		tr.brokerCommission as fBrokerCommission,
		tr.IsBuy as bIsBuy,
		tr.IsPosition as bIsPosition,
		c.Symbol as vcSymbol,
		c.exportSymbol as vcImportID,
		c.ContractTypeID as lContractType,
		trdr.acronym as vcTraderName,
		stg.StrategyName as vcStrategyName,
		bkr.brokerName as vcBrokerName,
		tr.UnderlyingID as lUnderlyingID,
		tr.TraderID as lTraderID,
		tr.StrategyID as lStrategyID,
		tr.BrokerID as lBrokerID,
		tr.FutureID as lFutureID,
		tr.OptionRootID as lOptionRootID,
		tr.FutureRootID as lFutureRootID,
		case when c.contractTypeID = 3 then opt.strike else fop.strike end as dStrike,
		case when c.contractTypeID = 3 then opt.isCall else fop.isCall end as bIsCall,
		case when c.contractTypeID = 3 then opt.ExpiryDate else fop.ExpiryDate end as dtExpiry,
		case when c.contractTypeID = 3 then ort.Symbol else ftr.FutureRootSymbol end as vcRootSymbol,
		case when stk.stockID is null then idx.primaryExchangeID else stk.primaryExchangeID end as lExchangeID
	from 
		TradeView tr
		inner join Trade trd on trd.TradeId = tr.TradeId
		left join Contract c on c.contractid = tr.contractid
		left join Strategy stg on stg.StrategyID = tr.strategyID
		left join Broker bkr on bkr.brokerID = tr.brokerID
		left join Trader trdr on trdr.traderID = tr.traderID
		left join OptionRoot ort on ort.optionRootID = tr.OptionRootID
		left join FutureRoot ftr on ftr.futureRootID = tr.futureRootID
		left join [Option] opt on opt.OptionID = tr.ContractID
		left join [futureOption] fop on fop.futureoptionid = tr.ContractID
		left join Stock stk on stk.stockID = tr.underlyingid and ISNULL(stk.actionid, 3) < 3
		left join [Index] idx on idx.indexID = tr.underlyingid and ISNULL(idx.actionid, 3) < 3
	where
		c.ActionID < 3


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO






--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.250')
	begin
		update DataInfo set keyValue = '4.46.250' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.46.250' '4.46.250 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
