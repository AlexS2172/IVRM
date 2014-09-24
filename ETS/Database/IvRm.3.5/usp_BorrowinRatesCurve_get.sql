SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BorrowingRatesCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BorrowingRatesCurve_Get]
GO

create  procedure [dbo].[usp_BorrowingRatesCurve_Get]
	@iCurveID 	int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select
		irp.IRpointID as iIRpointID, 
		irp.curveID as iCurveID, 
		irp.periodTypeID as iPeriodTypeID, 
		irp.num as iNum, 
		irp.shortRate as fShortRate, 
		irp.HTBRate as fHTBRate, 
		irp.longRate as fLongRate,
		irp.actionDate as dtActionDate,
		irp.actionID as tiActionID,
		irp.neutralRate as neutralRate,
		irp.ContractID as iContractID,
		curve.ruleTypeID as iRule,
		curve.positionThreshold as iPosThreshold,
		curve.curveName as vcCurveName
	from
		IRPointView irp
		left join dbo.IRCurveView curve on curve.curveID = irp.CurveID
	where
		isnull(irp.ContractID, 0) = isnull(@iCurveID, 0) 
	order by periodTypeID, num 

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

