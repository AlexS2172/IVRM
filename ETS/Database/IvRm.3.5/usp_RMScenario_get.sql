SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Get]
GO

CREATE         Procedure dbo.usp_RMScenario_Get
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
			showChange as iShowChange,
			volaSpecificShiftType as iVolaSpecificShift,
			spotShiftType as iSpotShift,
			spotSpecificShiftType as iSpotSpecificShift,
			calcModel as iCalcModel,
			corrIndex as iCorrIndex,
			specificShiftParams as vcSpecificShiftParams
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

