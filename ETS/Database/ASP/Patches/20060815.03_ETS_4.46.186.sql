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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.185'  or (keyValue = '3.46.186') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.186 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.186' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*BEGIN DATA MODIFICATION SECTION*/

/*FutUnd_4_Views***************************/
--Created by Nikita Kychkin
--14.08.2006
/*******************************************/

/*==========================================================================================*/
/*VIEWS*/
/*==========================================================================================*/
/*OptionRootView*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionRootView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[OptionRootView]
go
create        View OptionRootView
as
	Select 
		optionRootID, 
		underlyingID, 
		symbol, 
		isSynthetic, 
		lotSize, 
		actionID,
		actionDate,
                isFitEnabled
	from optionRoot
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
go

GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.186')
	begin
		update DataInfo set keyValue = '3.46.186' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.186' '3.46.186 update message:' 
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