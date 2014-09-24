/*
	speed up of usp_CustomVolatilityPointIsExist_Get
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.42.079' or (keyValue = '3.42.080')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.42.080 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.42.080' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION SECTION*/
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_CustomVolatilityPointIsExist_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_CustomVolatilityPointIsExist_Get
GO

CREATE PROCEDURE dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(20) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @vcSymbolName is null
		select distinct 
				CV.contractID as iContractID,
				CV.symbol as vcSymbolName,
				EC.expiryDate as dtExpiry,
				case when CSSP.volaSurfaceDataID is null then 0  
				else 1 end 'CustomVolatilityPointsCount'
			from OptionView OV
				join optionRoot ort on ort.optionRootID = OV.optionRootID
				join ContractView CV on Ort.underlyingID = CV.contractID
				join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
				left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
					and OV.expiryID = CSSP.expiryID
			where 
				CV.contractTypeID < 3
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID,EC.expiryDate
	else
		select distinct 
				CV.contractID as iContractID,
				CV.symbol as vcSymbolName,
				EC.expiryDate as dtExpiry,
				case when CSSP.volaSurfaceDataID is null then 0  
				else 1 end 'CustomVolatilityPointsCount'
			from OptionView OV
				join optionRoot ort on ort.optionRootID = OV.optionRootID
				join ContractView CV on Ort.underlyingID = CV.contractID
				join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
				left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
					and OV.expiryID = CSSP.expiryID
			where 
				CV.contractTypeID < 3
				and CV.symbol = @vcSymbolName
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID,EC.expiryDate

GO

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.42.080')
	begin
		update DataInfo set keyValue = '3.42.080' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.42.080' '3.42.080 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
