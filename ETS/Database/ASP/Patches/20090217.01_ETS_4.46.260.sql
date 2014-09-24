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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.259' or (keyValue = '4.46.260') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.260 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.260' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/
---------------------------------------------------------------------------------------------------------------------------------------------

GO
ALTER TABLE dbo.BatchReportingShedules DROP COLUMN [ID]

GO
ALTER TABLE dbo.BatchReportingShedules ADD [ID] int identity(1,1)

GO
------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER	PROC dbo.usp_BR_Shedule_Save
	@iID 		int,
	@dtTime	datetime,
	@tiStatus	tinyint

AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @iRet int
	select @iRet = @iID	

	if exists(select [ID] from BatchReportingShedules where [ID] = @iID)
		begin
			update 
				BatchReportingShedules 
			set 	
				[Time] = @dtTime, 
				[Status] = @tiStatus
			where 
				[ID] = @iID
			
		end
	else
		begin
			insert into BatchReportingShedules( 	[Time],
								[Status])
					values( @dtTime,
						@tiStatus)
			
			select @iRet = @@identity
		end
 
	return @iRet

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-----------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER	PROC dbo.usp_BR_Shedule_Del
	@iID 	int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	delete BatchReportingShedules where [ID] = @iID
 
	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-----------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER	PROC dbo.usp_BR_Shedule_Get
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-----------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER	PROC dbo.usp_BR_Shedule_Complete
	@iID	 	int,
	@dtDate	datetime
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	update BatchReportingShedules set [LastRun] = @dtDate
	where [ID] = @iID

	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-----------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.260')
	begin
		update DataInfo set keyValue = '4.46.260' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.46.260' '4.46.260 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


