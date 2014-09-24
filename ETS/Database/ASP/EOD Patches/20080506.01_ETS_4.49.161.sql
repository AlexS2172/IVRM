/*
	Contains stored procedures for ETS ASP (Institutional) database
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
--BEGIN TRANSACTION
--GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
--GO

/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.160' or (keyValue = '4.49.161')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.161 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.161' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO

/*DATA MODIFICATION SECTION*/
/* EodManager Option Import Procedures updated for working with expiryOV */

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='IVUnderlyingPointsRawData' and COLUMN_NAME='dtExpDateOV')
ALTER TABLE [IVUnderlyingPointsRawData]
	ADD [dtExpDateOV] [datetime] NULL 

--==============================================================================================

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER PROC usp_AspEod_RawData_StocksDefaultIVSurface_Load
AS
set nocount on
    INSERT INTO IVUnderlyingPointsRawData (iContractID, dtExpDate, fStrike, fIV, dtActionDate, dtExpDateOV)
	SELECT cv.ContractID, 
		op.ExpiryDate, 
		IV.fStrike,
                IV.fIV,
		GETDATE() AS dtActionDate,
		op.ExpiryOV
	FROM IVData.IV.Dbo.FT_DefaultIV IV
	INNER JOIN ContractView cv ON cv.Symbol = IV.vcSymbol AND cv.ContractTypeID IN (1,2)
	INNER JOIN OptionRoot r on r.UnderlyingID = cv.ContractID
	INNER JOIN [Option] op on op.OptionRootID = r.OptionRootID and  abs(op.Strike - iv.fStrike) <.005 
		and abs(datediff(d,op.ExpiryDate,IV.dtExpDate)) < 5
	GROUP BY cv.ContractID, op.ExpiryDate, IV.fStrike, IV.fIV, op.ExpiryOV

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--================================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER PROC usp_AspEod_RawData_FuturesDefaultIVSurface_Load
AS
 SET NOCOUNT ON

 INSERT INTO IVUnderlyingPointsRawData (iContractID, dtExpDate, fStrike, fIV, dtActionDate, dtExpDateOV)
 	SELECT 
		QQ.ContractID   AS ContractID, 
        	fo.expiryDate	AS dtExpDate,
		IV.fStrike	AS fStrike,
		CAST( IV.fIV AS FLOAT) AS fIV, 
                GETDATE() AS dtActionDate,
		fo.expiryOV	as dtExpDateOV
	FROM (
		SELECT	cu.ContractID, IV.fStrike, IV.dtExpDate, fr.UnderlyingID, 
				MAX(fr.futureLotSize) AS maxLosSize, COUNT( fr.futureRootID) AS fRootsNumber
		FROM FutureRoot fr 
		INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = fr.futureRootSymbol
		INNER JOIN ContractView cu ON cu.ContractID = fr.underlyingID AND cu.ContractTypeID = 6
		GROUP BY IV.fStrike, IV.dtExpDate,fr.UnderlyingId, cu.ContractID
	) QQ
	INNER JOIN FutureRoot gh ON gh.underlyingID=qq.UnderlyingID AND gh.futureLotSize = qq.maxLosSize
	INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = gh.futureRootSymbol AND qq.fStrike = IV.fStrike and qq.dtExpDate = iv.dtExpDate
	INNER join Future f on f.futureRootID = gh.futureRootID
	INNER join FutureOption fo on fo.futureID = f.FutureID and fo.strike = iv.fStrike 
		and abs(datediff(d,fo.expiryDate,iv.dtExpDate)) < 5
	ORDER BY qq.UnderlyingID, qq.dtExpDate, qq.fStrike

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--==================================================================================================
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='CustomStrikeSkewPoint' and COLUMN_NAME='ExpiryOV')
ALTER TABLE [CustomStrikeSkewPoint]
	ADD [ExpiryOV] [datetime] NULL 
--==================================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER proc usp_AspEod_DefaultIVSurface_Load
as
SET NOCOUNT ON

BEGIN TRANSACTION
	insert into VolaSurfaceData(contractID, isDefaultSurface, baseUnderlinePrice)
	select c.ContractID,1,0 from Contract c
	where   contractTypeId in (1, 2, 6)
		and (exists(select top 1 1 from optionRoot where c.ContractId = underlyingid) -- check if optionable
			or
			exists(select top 1 1 from futureRoot where c.ContractId = underlyingid) 
		)
		and not exists(select top 1 1 from VolaSurfaceData where c.ContractId = ContractId and isDefaultSurface = 1) -- check no default IV

	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Insert Default Volasurfaces failed', 16, 1)
		return (-1)
	end

-- Delete obsolete strike points for instruments which has IV surfaces in IV database
	delete from CustomStrikeSkewPoint where  customStrikeSkewPointID in (
		select cssp.customStrikeSkewPointID from ContractView cv
		inner join VolaSurfaceData vsd on vsd.contractID = cv.contractid
		inner join CustomStrikeSkewPoint cssp on cssp.volaSurfaceDataID=vsd.volaSurfaceDataID and datediff(d, cssp.actionDate, getDate())>0
		where cv.contractTypeId in (1, 2, 6) and cssp.actionID < 3 
			and exists( select top 1 1 from IVUnderlyingPointsRawData where iContractID = cv.ContractID)
	)

	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Delete Obsolete skew points failed', 16, 1)
		return (-2)
	end


-- Update existing
	update CustomStrikeSkewPoint set
		volatility = isnull(IV.fIV * 100, volatility),
		ExpiryDate = IV.dtExpDate,
		ExpiryOV = IV.dtExpDateOV
--	select 	vsd.VolaSurfaceDataID , IV.fStrike, IV.iExpiryID, IV.fIV
	from CustomStrikeSkewPoint cssp, ContractView c
	inner join IVUnderlyingPointsRawData IV on c.ContractID = IV.iContractID
	inner join VolaSurfaceData vsd on vsd.contractID = c.contractid
	where vsd.VolaSurfaceDataID = cssp.volaSurfaceDataID
		and abs(cssp.strike - IV.fStrike) < 0.009	
		and abs(datediff(d,cssp.ExpiryDate , IV.dtExpDate)) <= 3
			  
	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Update existing skew points failed', 16, 1)
		return (-3)
	end


-- Insert new IV points from IV database
	insert into CustomStrikeSkewPoint (
			volaSurfaceDataID,
			type,
			strike,
			volatility,
			isBasePoint,
			expiryDate,
			expiryOV)
	select vsd.VolaSurfaceDataID as volaSurfaceDataID,
	       2                     as Type,
               IV.fStrike            as Strike,
	       /*IV.fIV                as */
		volatility =
		case
			when IV.fIV is null then 20
			when IV.fIV = 0 then 20
			when IV.fIV < 0 then 20
			else IV.fIV * 100
		end,
               	-1                    as isBasePoint,
	      	IV.dtExpDate	     as expiryDate,
	      	IV.dtExpDateOV	     as expiryOV
	from IVUnderlyingPointsRawData IV
	inner join ContractView c on c.ContractID = IV.iContractID
	inner join VolaSurfaceData vsd on vsd.contractID = c.contractid
	where 
		not exists(select top 1 1 from CustomStrikeSkewPoint cssp 
	                   where cssp.volaSurfaceDataID = vsd.volaSurfaceDataID 
				   and cssp.actionID < 3 
	                           and abs(datediff(d,cssp.ExpiryDate, IV.dtExpDate)) <= 3
				   and abs(cssp.strike - IV.fStrike) < 0.009	
			  )

	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Insert new skew points failed', 16, 1)
		return (-4)
	end

COMMIT TRANSACTION


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--------------------------------------------------------------------------------------------------------------------------------------------------------
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.161')
	begin
		update DataInfo set keyValue = '4.49.161' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
--GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
--	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.161' '4.49.161 update message:' 
	select 'The database update succeeded'
--
IF @@TRANCOUNT > 0 
BEGIN
	COMMIT TRANSACTION
END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
