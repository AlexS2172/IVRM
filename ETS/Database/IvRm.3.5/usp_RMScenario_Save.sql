SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Save]
GO


CREATE      Procedure dbo.usp_RMScenario_Save
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
	@iShowChange int = Null,
	@iVolaSpecificShift int = Null,
	@iSpotShift int = Null,
	@iSpotSpecificShift int = Null,
	@iCalcModel int = Null,
	@iCorrIndex int = Null,
	@vcSpecificShiftParams varchar(512) = Null
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
					showChange,
					volaSpecificShiftType,
					spotShiftType,
					spotSpecificShiftType,
					calcModel,
					corrIndex,
					specificShiftParams
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
					@iShowChange,
					@iVolaSpecificShift,
					@iSpotShift,
					@iSpotSpecificShift,
					@iCalcModel,
					@iCorrIndex,
					@vcSpecificShiftParams)
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
				showChange = isnull(@iShowChange, showChange),
				volaSpecificShiftType = isnull(@iVolaSpecificShift, volaSpecificShiftType),
				spotShiftType = isnull(@iSpotShift, spotShiftType),
				spotSpecificShiftType = isnull(@iSpotSpecificShift, spotSpecificShiftType),
				calcModel = isnull(@iCalcModel, calcModel),
				corrIndex = isnull(@iCorrIndex, corrIndex),
				specificShiftParams = isnull(@vcSpecificShiftParams, specificShiftParams)
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

