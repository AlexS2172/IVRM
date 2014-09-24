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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.121' or (keyValue = '3.46.122')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.122 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.122' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- CustomStrikeSkewPoint
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID' and id = object_id('dbo.CustomStrikeSkewPoint'))
	drop index dbo.[CustomStrikeSkewPoint].IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID
GO

CREATE NONCLUSTERED INDEX IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID ON [dbo].[CustomStrikeSkewPoint] ([volaSurfaceDataID], [type], [actionID])
GO

----------------------------------------------------------------------------------------------------
-- usp_CreateDefaultVolaSurface_Save
----------------------------------------------------------------------------------------------------
if exists (select * from sysobjects where id = object_id('dbo.usp_CreateDefaultVolaSurface_Save') and sysstat & 0xf = 4)
	drop procedure dbo.usp_CreateDefaultVolaSurface_Save
GO

CREATE PROC dbo.usp_CreateDefaultVolaSurface_Save
	@iContractID int
as
/*
	Description: this procedure create default surface by ContractID
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iVolaSurfaceDataID int
	if @iContractID is not null and not exists(select * from VolaSurfaceDataView where contractID=@iContractID and isDefaultSurface = 1)
		begin
			declare @fPrice float
		
			select @fPrice = priceClose
				/*case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end*/
			from ContractPrice
			where contractID = @iContractID and exchangeID is null 

			insert into VolaSurfaceData(contractID,isDefaultSurface,baseUnderlinePrice)
				values (@iContractID,1,isnull(@fPrice,0))
			select @iVolaSurfaceDataID = @@identity
		end
	else 
		select @iVolaSurfaceDataID = VolaSurfaceDataID 
			from VolaSurfaceDataView 
			where contractID=@iContractID and isDefaultSurface = 1 and @iContractID is not null

	return(@iVolaSurfaceDataID)

GO

----------------------------------------------------------------------------------------------------
-- usp_CustomStrikeSkewPoint_Get
----------------------------------------------------------------------------------------------------
if exists (select * from sysobjects where id = object_id('dbo.usp_CustomStrikeSkewPoint_Get') and sysstat & 0xf = 4)
	drop procedure dbo.usp_CustomStrikeSkewPoint_Get
GO

CREATE PROC dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null
as
/*
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
		from CustomStrikeSkewPoint CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			--join VolaSurfaceData vsd on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
		where 
			CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
			and CSSP.type = @iType
			and CSSP.actionID < 3
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
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			--convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPointView CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			--join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			--join Contract C on  VolaSurfaceData.contractID = C.contractID
			--join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			customStrikeSkewPointID = isnull(@iCustomStrikeSkewPointID, customStrikeSkewPointID)
			and CSSP.volaSurfaceDataID = isnull(@iVolaSurfaceDataID, CSSP.volaSurfaceDataID) 
			and type = isnull(@iType, type) 
			and CSSP.expiryID = isnull(@iExpiryID, CSSP.expiryID)
			--and E.expiryID = isnull(@iExpiryID, E.expiryID)
	end

	set @error = @@error
	return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_VolaSurfaceData_Get
----------------------------------------------------------------------------------------------------
if exists (select * from sysobjects where id = object_id('dbo.usp_VolaSurfaceData_Get') and sysstat & 0xf = 4)
	drop procedure dbo.usp_VolaSurfaceData_Get
GO

CREATE PROC dbo.usp_VolaSurfaceData_Get
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
			where
				VolaSurfaceData.contractID = isnull(@iContractID, VolaSurfaceData.contractID)
				and VolaSurfaceData.volaSurfaceDataID = isnull(@ivolaSurfaceDataID, VolaSurfaceData.volaSurfaceDataID)
	end
	
	return @@error
GO

/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.122')
	begin
		update DataInfo set keyValue = '3.46.122' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.122' '3.46.122 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
