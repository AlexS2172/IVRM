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
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.142' or (keyValue = '4.49.143')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.143 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.143' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

--------------------------------------------------------------------------------------------

ALTER   proc usp_AspEod_DefaultIVSurface_Load
as
SET NOCOUNT ON

BEGIN TRANSACTION
	insert into VolaSurfaceData(contractID, isDefaultSurface, baseUnderlinePrice)
		select c.ContractID,1,0 from Contract c
			where   contractTypeId in (1, 2, 6)
				and exists(select top 1 1 from optionRoot where c.ContractId = underlyingid) -- check if optionable
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
			volatility = isnull(IV.fIV * 100, volatility)
--		select 	vsd.VolaSurfaceDataID , IV.fStrike,IV.iExpiryID ,IV.fIV
		from CustomStrikeSkewPoint cssp, ContractView c
			inner join IVUnderlyingPointsRawData IV on c.ContractID = IV.iContractID
			inner join VolaSurfaceData vsd on vsd.contractID = c.contractid
		where vsd.VolaSurfaceDataID = cssp.volaSurfaceDataID
		and abs(cssp.strike - IV.fStrike) < 0.009	
		and cssp.ExpiryId = IV.iExpiryID

			  
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
			expiryID)
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
		      IV.iExpiryID	     as expiryID
		from IVUnderlyingPointsRawData IV
			inner join ContractView c on c.ContractID = IV.iContractID
			inner join VolaSurfaceData vsd on vsd.contractID = c.contractid
		where 
			not exists(
				  select top 1 1 
	                                 from CustomStrikeSkewPoint cssp 
	                                 where cssp.volaSurfaceDataID = vsd.volaSurfaceDataID 
					   and cssp.actionID < 3 
	                                   and cssp.ExpiryId = IV.iExpiryID
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

/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.143')
	begin
		update DataInfo set keyValue = '4.49.143' where dataInfoID = 1
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
	---SELECT * FROM #tmpErrors
END
GO
IF @@TRANCOUNT > 0 
BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.143' '4.49.143 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
