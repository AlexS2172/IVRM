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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.257' or (keyValue = '4.46.258') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.258 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.258' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/
---------------------------------------------------------------------------------------------------------------------------------------------
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='BatchReportingReports' and COLUMN_NAME='FileNameHeader')
ALTER TABLE [BatchReportingReports]
	ADD [FileNameHeader] [varchar] (260) COLLATE Latin1_General_CI_AS NULL
GO

GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='BatchReportingReports' and COLUMN_NAME='ReportFormat')
ALTER TABLE [BatchReportingReports]
	ADD [ReportFormat] [int] NULL
GO

----------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER	PROC dbo.usp_BR_Reports_Get
	@iReportID 	int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select	[ID]			as	iID,
		[UserName] 		as 	vcUserName,
		[Status] 		as 	tiStatus,
		[LayoutFile] 		as 	vcLayoutFile,
		[GroupID] 		as 	iGroupID,
		[TraderID] 		as 	iTraderID,
		[StrategyID] 		as 	iStrategyID,
		[TraderGroupID] 	as 	iTraderGroupID,
		[ExpandedPositions] 	as 	tiPositions,
		[ResultFilePath] 	as 	vcFilePath,
		[LastRun]		as	dtLastRun,
		[ResultFile]		as	vcResultFile,
		[FileNameHeader]	as	vcFileNameHeader,
		[ReportFormat]		as 	iReportFormat
	from	BatchReportingReports
	where	[ID] = @iReportID or @iReportID is null
 
	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------------------------


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
	@vcResultFile		varchar(260),
	@vcFileNameHeader	varchar(260),
	@iReportFormat		int
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
								[ResultFile],
								[FileNameHeader],
								[ReportFormat] )
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
						@vcResultFile,
						@vcFileNameHeader,
						@iReportFormat )
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
								[ResultFile] = @vcResultFile,
								[FileNameHeader] = @vcFileNameHeader,
								[ReportFormat] = @iReportFormat
			where [ID] = @iReportID
		end
 
	return @@error



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

-----------------------------------------------------------------------------------------------------------------

GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='RMScenario' and COLUMN_NAME='showChange')
ALTER TABLE [RMScenario]
	ADD [showChange] [int] NULL DEFAULT (0)
GO

--------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER        Procedure dbo.usp_RMScenario_Get
	@iRMScenarioID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    select rs.RMScenarioID as iRMScenarioID, 
			rs.ScenarioName as vcScenarioName, 
			rs.[Description] as vcDescription,
			points1 as iPoints1,
			step1 as fStep1,
			isAbs1 as tiIsAbs1,
			type1 as tiType1,
			points2 as iPoints2,
			step2 as fStep2,
			isAbs2 as tiIsAbs2,
			type2 as tiType2,
			volaShift as tiVolaShift,
			actionDate as dtActionDate,
			actionID as tiActionID,
			hours as iHours,
			minutes as iMinutes,
			showChange as iShowChange
		from RMScenario rs
		where ((@iRMScenarioID is Null) 
			or (@iRMScenarioID is Not Null and rs.RMScenarioID = @iRMScenarioID))

set @error = @@error
    Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


-------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER     Procedure dbo.usp_RMScenario_Save
	@iRMScenarioID int = Null,
	@vcScenarioName varchar(30) = Null,
	@vcDescription varchar(120) = Null,
    	@iPoints1 int = Null,
	@fStep1 float = Null,
	@tiIsAbs1 tinyint = Null,
	@tiType1 tinyint = Null,
	@iPoints2 int = Null,
	@fStep2 float= Null,
	@tiIsAbs2 tinyint = Null,
	@tiType2 tinyint = Null,
	@tiVolaShift tinyint = Null,
	@iHours int = Null,
	@iMinutes int = Null,
	@iShowChange int = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
/*
	if exists (select * from RMScenario where scenarioName = @vcScenarioName and actionID < 3 and (@iRMScenarioID <> RMScenarioID or @iRMScenarioID is null))
		begin
			Raiserror ('Row in RMScenario table with same scenario name already exists!', 16, 1)
			Return (-1)
		end
*/   
	DECLARE @ErrorSave as int
	DECLARE @iRmNewScenatioID as int 

	set @ErrorSave = 0

	if @iRMScenarioID is null
	
	--if not exists(select * from RMScenario where RMScenarioID = @iRMScenarioID)
		begin
			insert into RMScenario(
					ScenarioName,
					[Description],
					points1,
					step1,
					isAbs1,
					type1,
					points2,
					step2,
					isAbs2,
					type2,
					volaShift,
					hours,
					minutes,
					showChange
					) 
				values (
					@vcScenarioName,
					@vcDescription,
				   	@iPoints1,
					@fStep1,
					@tiIsAbs1,
					@tiType1,
					@iPoints2,
					@fStep2,
					@tiIsAbs2,
					@tiType2,
					@tiVolaShift,
					@iHours,
					@iMinutes,
					@iShowChange)
				IF (@@ERROR <> 0)
                                	begin 
    						SET @ErrorSave = @@ERROR
						set @iRmNewScenatioID = 0
	                                end
				else
					begin                               
						set @iRmNewScenatioID = @@IDENTITY
					end

		end
	else
		begin
			update RMScenario set
				ScenarioName = isNull(@vcScenarioName, ScenarioName),
				[Description] = isNull(@vcDescription, [Description]),
				points1 = isNull(@iPoints1, points1),
				step1 = isNull(@fStep1, step1),
				isAbs1 = isNull(@tiIsAbs1, isAbs1),
				type1 = isNull(@tiType1, type1),
				points2 = isNull(@iPoints2, points2),
				step2 = isNull(@fStep2, step2),
				isAbs2 = isNull(@tiIsAbs2, isAbs2),
				type2 = isNull(@tiType2, type2),
				volaShift = isnull(@tiVolaShift, volaShift),
				hours = isnull(@iHours, hours),
				minutes = isnull(@iMinutes, minutes),
				showChange = isnull(@iShowChange, showChange)
				where RMScenarioID = @iRMScenarioID
				IF (@@ERROR <> 0)
                                	begin 
    						SET @ErrorSave = @@ERROR
						set @iRmNewScenatioID = 0
	                                end
				else
					begin                               
						set @iRmNewScenatioID = @iRMScenarioID
					end

		end
if @ErrorSave <> 0
	return -1
else
	return 	@iRmNewScenatioID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.258')
	begin
		update DataInfo set keyValue = '4.46.258' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.46.258' '4.46.258 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


