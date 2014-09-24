/*
MLine00001802 fixing
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.089' or (keyValue = '3.43.090')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.090 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.090' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_ClearDeleted' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_ClearDeleted
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   PROCEDURE dbo.usp_Trade_ClearDeleted 
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	Begin Transaction Trade

		delete from TradeLog 
		where seqNum in (select seqNum
						from Trade 
						where tradeID in (select tradeID from Trade where actionID = 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade where tradeID in (select tradeID from Trade where actionID = 0) 
			if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

		set @error = @@error

		delete from TradeLog 
		where seqNum in (select seqNum
						from Trade T join [Option] O on O.optionID = T.contractID
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						join Contract UC on ORt.underlyingID = UC.contractID 
						join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
						where E.expiryDate < GetDate() )
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete Trade from Trade T join [Option] O on O.optionID = T.contractID
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						join Contract UC on ORt.underlyingID = UC.contractID 
						join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
						where E.expiryDate < GetDate() 
		if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

		set @error = @@error

	Commit Transaction Trade

finish:
Return(@error)


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
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.090')
	begin
		update DataInfo set keyValue = '3.43.090' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.090' '3.43.090 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
