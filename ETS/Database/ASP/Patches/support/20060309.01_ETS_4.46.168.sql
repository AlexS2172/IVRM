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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.167'  or (keyValue = '3.46.168') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.168 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.168' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

GO
Alter Procedure dbo.usp_RMScenario_Del    
 @iRMScenarioID int    
as    
 -------------------------------------------------    
 set nocount on    
 -------------------------------------------------    
 declare @error int  set @error = -1    
-- update RMScenario set actionID=3 where RMScenarioID = @iRMScenarioID    
 delete RMScenario Where RMScenarioID = @iRMScenarioID    
    
 set @error = @@error    
        Return(@error)    
    
GO



GO

ALTER   Procedure dbo.usp_RMScenario_Save
	@iRMScenarioID int = Null,
	@vcScenarioName varchar(30) = Null,
	@vcDescription varchar(120) = Null,
    	@iPoints1 int = Null,
	@fStep1 int = Null,
	@tiIsAbs1 tinyint = Null,
	@tiType1 tinyint = Null,
	@iPoints2 int = Null,
	@fStep2 float= Null,
	@tiIsAbs2 tinyint = Null,
	@tiType2 tinyint = Null,
	@tiVolaShift tinyint = Null
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
					volaShift
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
					@tiVolaShift)
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
				volaShift = isnull(@tiVolaShift, volaShift)
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

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.168')
	begin
		update DataInfo set keyValue = '3.46.168' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.168' '3.46.168 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
