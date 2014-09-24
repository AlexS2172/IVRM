/*
fix for MLine00001256 in ETS 1.10
*/

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.41.067' or (keyValue = '3.41.068')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.41.068 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.41.068' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if not exists(select * from sysindexes where id=object_id('OptionRoot') and name='underlyingID_ind')
	create index underlyingID_ind on OptionRoot(underlyingID)
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
if not exists(select * from sysindexes where id=object_id('Option') and name='optionRootID_ind')
	create index optionRootID_ind on [Option](optionRootID)
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist2_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist2_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceData_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare	@iExpiryID int

	-- Sharky: faster request for special parameters' set
	if @iExpiryID is null and @iCustomStrikeSkewPointID is null
		and @iVolaSurfaceDataID is not null and @iType is not null
	begin
		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPointView CSSP
			join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
		where 
			CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
			and CSSP.type = @iType
	end
	else
	begin
		-- Sharky: old variant
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPointView CSSP
			join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			join Contract C on  VolaSurfaceData.contractID = C.contractID
			join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			(@iCustomStrikeSkewPointID is null or customStrikeSkewPointID=@iCustomStrikeSkewPointID)
			and (@iVolaSurfaceDataID is null or CSSP.volaSurfaceDataID=@iVolaSurfaceDataID) 
			and (@iType is null or type=@iType) 
			and (@iExpiryID is null or E.expiryID = @iExpiryID)
	end

	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE         Procedure dbo.usp_CustomVolatilityPointIsExist2_Get
	@vcSymbolName varchar(20)
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------

	select 
		Ort.underlyingID as iContractID,
		OV.expiryID as iExpiryID
	from OptionView OV
		join OptionRoot ort on ort.optionRootID = OV.optionRootID
		join ContractView CV on Ort.underlyingID = CV.contractID
		left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
		left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
													and OV.expiryID = CSSP.expiryID
	where 
		CV.symbol = @vcSymbolName
		and CV.contractTypeID < 3
		and CSSP.volaSurfaceDataID is null
	group by Ort.underlyingID, ov.expiryID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE           Procedure dbo.usp_VolaSurfaceData_Get
	@iContractID int = null,
	@iVolaSurfaceDataID int = null
as
/*
	this procedure gets data from VolaManagementData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	-- Sharky: faster request for special parameters' set
	if @iContractID is null and @iVolaSurfaceDataID is not null
	begin
		select	VolaSurfaceData.volaSurfaceDataID as iVolaSurfaceDataID, 
				VolaSurfaceData.contractID as icontractID, 
				baseUnderlinePrice as fBaseUnderlinePrice, 
				smileAccelerator as fSmileAccelerator,
				interpolationFactor as fInterpolationFactor,
				strikeMoneynessHi as fStrikeMoneynessHi,
				strikeMoneynessLo as fStrikeMoneynessLo,
				surfaceName as vcSurfaceName,
				isPriceOverride as bIsPriceOverride,
				isDiscreteAcceleration as bIsDiscreteAcceleration,
				isDefaultSurface as bIsDefaultSurface,
				ATMVolatility as fATMVolatility,
				Contract.symbol as vcSymbolName,
				case when NextDaySurface.volaSurfaceDataID is not null then 1
					else 0 end bIsNextDaySurface,
				contract.contractTypeID as iContractTypeID --,
				--VolaSurfaceData.actionDate as dtActionDate,
				--VolaSurfaceData.actionID as tiActionID
			from VolaSurfaceDataView VolaSurfaceData
					join ContractView Contract on VolaSurfaceData.contractID=Contract.contractID
					left join NextDaySurface on VolaSurfaceData.volaSurfaceDataID=NextDaySurface.volaSurfaceDataID
			where
				VolaSurfaceData.volaSurfaceDataID = @ivolaSurfaceDataID
	end
	else
	begin
		select	VolaSurfaceData.volaSurfaceDataID as iVolaSurfaceDataID, 
				VolaSurfaceData.contractID as icontractID, 
				baseUnderlinePrice as fBaseUnderlinePrice, 
				smileAccelerator as fSmileAccelerator,
				interpolationFactor as fInterpolationFactor,
				strikeMoneynessHi as fStrikeMoneynessHi,
				strikeMoneynessLo as fStrikeMoneynessLo,
				surfaceName as vcSurfaceName,
				isPriceOverride as bIsPriceOverride,
				isDiscreteAcceleration as bIsDiscreteAcceleration,
				isDefaultSurface as bIsDefaultSurface,
				ATMVolatility as fATMVolatility,
				Contract.symbol as vcSymbolName,
				case when NextDaySurface.volaSurfaceDataID is not null then 1
					else 0 end bIsNextDaySurface,
				contract.contractTypeID as iContractTypeID --,
				--VolaSurfaceData.actionDate as dtActionDate,
				--VolaSurfaceData.actionID as tiActionID
			from VolaSurfaceDataView VolaSurfaceData
					join ContractView Contract on VolaSurfaceData.contractID=Contract.contractID
					left join NextDaySurface on VolaSurfaceData.volaSurfaceDataID=NextDaySurface.volaSurfaceDataID
			where	((@iContractID is Null or VolaSurfaceData.contractID = @iContractID) and 
					(@ivolaSurfaceDataID Is Null or VolaSurfaceData.volaSurfaceDataID = @ivolaSurfaceDataID))
	end
	
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.41.068')
	begin
		update DataInfo set keyValue = '3.41.068' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.41.068' '3.41.068 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO