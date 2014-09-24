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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.202'  or (keyValue = '3.46.203') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.203 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.203' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

ALTER   PROC dbo.usp_MmFuture_Save
	@iFutureID int,
	@vcFutureName varchar(255) = null,
	@iExpCalendarID int = null,
	@tiCalcOptionType tinyint = null,
	@tiIsActive tinyint = null,
	@iUndPriceProfileID int = null,
	@iOptPriceProfileID int = null
AS
	-- update future

	set nocount on
	
	declare @error int		set @error = 0

	exec @error = usp_BoFuture_Save @iFutureID,
									null,
									null,
									@vcFutureName,
									null,
									--null,
									null,
									@iExpCalendarID,
									@tiCalcOptionType,
									@tiIsActive,
									@iUndPriceProfileID,
									@iOptPriceProfileID,
									1

	if @@error <> 0 set @error = -20

	return @error

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.203')
	begin
		update DataInfo set keyValue = '3.46.203' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.203' '3.46.203 update message:' 
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