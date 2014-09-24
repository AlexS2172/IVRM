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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.248' or (keyValue = '4.46.249') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.249 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.249' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/
/*  LastRun ResultFile Save */

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER	PROC dbo.usp_BR_Report_Save
	@iReportID 		int,
	@vcUserName		varchar(80),
	@tiStatus		tinyint,
	@vcLayoutFile		varchar(260),
	@iGroupID		int,
	@iTraderID		int,
	@iStrategyID		int,
	@iTraderGroupID		int,
	@tiPositions		tinyint,
	@vcFilePath		varchar(260),
	@dtLastRun		datetime,
	@vcResultFile		varchar(260)
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iReportID is null
		begin
			insert into BatchReportingReports( 	[UserName],
								[Status],
								[LayoutFile],
								[GroupID],
								[TraderID],
								[StrategyID],
								[TraderGroupID],
								[ExpandedPositions],
								[ResultFilePath],
								[LastRun],
								[ResultFile] )
					values( @vcUserName,
						@tiStatus,
						@vcLayoutFile,
						@iGroupID,
						@iTraderID,
						@iStrategyID,
						@iTraderGroupID,
						@tiPositions,
						@vcFilePath,
						@dtLastRun,
						@vcResultFile )
		end
	else
		begin
			update BatchReportingReports set	[UserName] = @vcUserName,
								[Status] = @tiStatus,
								[LayoutFile] = @vcLayoutFile,
								[GroupID] = @iGroupID,
								[TraderID] = @iTraderID,
								[StrategyID] = @iStrategyID,
								[TraderGroupID] = @iTraderGroupID,
								[ExpandedPositions] = @tiPositions,
								[ResultFilePath] = @vcFilePath,
								[LastRun] = @dtLastRun,
								[ResultFile] = @vcResultFile
			where [ID] = @iReportID
		end
 
	return @@error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.249')
	begin
		update DataInfo set keyValue = '4.46.249' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.46.249' '4.46.249 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
