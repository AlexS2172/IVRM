/*
	Contains stored procedures for ETS ASP (Institutional) database
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
--BEGIN TRANSACTION
--GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
--GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.137' or (keyValue = '4.49.138')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.138 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.137' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
/*==========================================================================================*/
/*VIEWS*/
/*==========================================================================================*/

/*ContractPriceView*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractPriceView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractPriceView]
go
create   VIEW ContractPriceView with schemabinding 
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
go

/*SyntheticUnderlyingParamView*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SyntheticUnderlyingParamView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[SyntheticUnderlyingParamView]
go
create    VIEW SyntheticUnderlyingParamView
AS
	Select
		syntheticUndParamID, 
		syntheticOptRootID, 
		underlyingID, 
		syntheticCoeff, 
		strikeSettlement,
		actionID, 
		actionDate
		from SyntheticUnderlyingParam
		where actionID < 3
go

/*TradeViewExt*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeViewExt]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradeViewExt]
go
create VIEW TradeViewExt
AS
	select
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID
	from 
		dbo.Trade
	group by
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID
	having
		(min(actionID) = 1)

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.138')
	begin
		update DataInfo set keyValue = '4.49.138' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
--GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
--	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.138' '4.49.138 update message:' 
	select 'The database update succeeded'
--
IF @@TRANCOUNT > 0 
BEGIN
	COMMIT TRANSACTION
END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction