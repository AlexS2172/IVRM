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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.134'  or (keyValue = '3.46.135') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.135 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.135' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OTCFutureOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OTCFutureOptions_Get]
GO


CREATE  PROC dbo.usp_OTCFutureOptions_Get
	@futureID int,
	@expiryDate datetime,
	@strike float
AS
	set nocount on

	select
	foc.symbol
	from Future f
		inner join ContractView fc on f.futureID = fc.contractID
		inner join FutureOption fo on f.futureID = fo.futureID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
		inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
	where
		f.futureID = @futureID
		and fec.expiryDate = @expiryDate
		and fo.strike = @strike



GO

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.135')
	begin
		update DataInfo set keyValue = '3.46.135' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.135' '3.46.135 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

