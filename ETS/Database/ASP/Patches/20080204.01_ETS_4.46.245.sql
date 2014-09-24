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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.244' or (keyValue = '3.46.245') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '3.46.245 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '3.46.245' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BatchReportingReports]')
						and OBJECTPROPERTY(id, N'IsTable') = 1)
      drop table [dbo].[BatchReportingReports] 
GO
CREATE TABLE BatchReportingReports
(
	[ID]			int	IDENTITY(1,1)	not null,
	[UserName]		varchar(80)		not null,
	[Status]		tinyint			not null,
	[LayoutFile]		varchar(260)		not null,
	[GroupID]		int			not null,
	[TraderID]		int			not null,
	[StrategyID]		int			not null,
	[TraderGroupID]		int			not null,
	[ExpandedPositions]	tinyint			not null,
	[ResultFilePath]	varchar(260)		not null,
	[LastRun]		datetime,
	[ResultFile]		varchar(260)
) 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Report_Save]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Report_Save]
GO
CREATE	PROC dbo.usp_BR_Report_Save
	@iReportID 		int,
	@vcUserName		varchar(80),
	@tiStatus		tinyint,
	@vcLayoutFile		varchar(260),
	@iGroupID		int,
	@iTraderID		int,
	@iStrategyID		int,
	@iTraderGroupID		int,
	@tiPositions		tinyint,
	@vcFilePath		varchar(260)
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
								[ResultFilePath] )
					values( @vcUserName,
						@tiStatus,
						@vcLayoutFile,
						@iGroupID,
						@iTraderID,
						@iStrategyID,
						@iTraderGroupID,
						@tiPositions,
						@vcFilePath )
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
								[ResultFilePath] = @vcFilePath
			where [ID] = @iReportID
		end
 
	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Reports_Get]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Reports_Get]
GO
CREATE	PROC dbo.usp_BR_Reports_Get
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
		[ResultFile]		as	vcResultFile
	from	BatchReportingReports
	where	[ID] = @iReportID or @iReportID is null
 
	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Report_Complete]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Report_Complete]
GO
CREATE	PROC dbo.usp_BR_Report_Complete
	@iReportID 	int,
	@dtDate		datetime,
	@vcFileName	varchar(260)
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	update BatchReportingReports set [LastRun] = @dtDate, [ResultFile] = @vcFileName
	where [ID] = @iReportID

	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Report_Del]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Report_Del]
GO
CREATE	PROC dbo.usp_BR_Report_Del
	@iReportID 	int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	delete BatchReportingReports where [ID] = @iReportID
 
	return @@error
GO

-----------------------------------------------------------------------------------------------------------


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BatchReportingShedules]')
						and OBJECTPROPERTY(id, N'IsTable') = 1)
      drop table [dbo].[BatchReportingShedules]
GO
CREATE TABLE BatchReportingShedules
(
	[ID]			int			not null,
	[Time]			datetime		not null,
	[Status]		tinyint			not null,
	[LastRun]		datetime
) 
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Shedule_Save]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Shedule_Save]
GO
CREATE	PROC dbo.usp_BR_Shedule_Save
	@iID 		int,
	@dtTime		datetime,
	@tiStatus	tinyint

AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if exists(select [ID] from BatchReportingShedules where [ID] = @iID)
		begin
			update BatchReportingShedules set [Time] = @dtTime, [Status] = @tiStatus
			where [ID] = @iID
		end
	else
		begin
			insert into BatchReportingShedules( 	[ID],
								[Time],
								[Status])
					values( @iID,
						@dtTime,
						@tiStatus)
		end
 
	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Shedule_Get]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Shedule_Get]
GO
CREATE	PROC dbo.usp_BR_Shedule_Get
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select	[ID]		as	iID,
		[Time] 		as 	dtTime,
		[Status]	as	tiStatus,
		[LastRun]	as	dtLastRun

	from	BatchReportingShedules
 
	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Shedule_Del]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Shedule_Del]
GO
CREATE	PROC dbo.usp_BR_Shedule_Del
	@iID 	int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	delete BatchReportingShedules where [ID] = @iID
 
	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BR_Shedule_Complete]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop proc [dbo].[usp_BR_Shedule_Complete]
GO
CREATE	PROC dbo.usp_BR_Shedule_Complete
	@iID 	int,
	@dtDate		datetime
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	update BatchReportingShedules set [LastRun] = @dtDate
	where [ID] = @iID

	return @@error
GO

exec usp_BR_Shedule_Save 1, 0, 0
GO
exec usp_BR_Shedule_Save 2, 0, 0
GO
exec usp_BR_Shedule_Save 3, 0, 0
GO


GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.245')
	begin
		update DataInfo set keyValue = '3.46.245' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0 OR 
   not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 3.46.245' '3.46.245 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO







