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
--BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.186'  or (keyValue = '3.46.187') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.187 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.187' as  'Version in script' from DataInfo where dataInfoID = 1
		--INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
----------------------------------------------------------------

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DefaultIVSurfaceStock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DefaultIVSurfaceStock_Get]
GO

create proc dbo.usp_DefaultIVSurfaceStock_Get
	@iUnderlyingID int
as
if @iUnderlyingID is not null
begin
	select	cv.ContractID,
		CONVERT(datetime, IV.dtExpDate, 112) as dtExpDate,
		(DATEPART(yyyy, IV.dtExpDate)-2000)*12+DATEPART(m, IV.dtExpDate) as iExpiryID,
		IV.fStrike,
		IV.fIV,
		GetDate() as dtActionDate
	from IVData.IV.Dbo.FT_DefaultIV IV
		inner join ContractView cv on cv.Symbol = IV.vcSymbol and cv.ContractTypeID in (1,2)
			and cv.ContractID = @iUnderlyingID
end
return @@error

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DefaultIVSurfaceFutureUnd_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DefaultIVSurfaceFutureUnd_Get]
GO

create proc dbo.usp_DefaultIVSurfaceFutureUnd_Get
	@iFutUnderlyingID int
as
if @iFutUnderlyingID is not null
begin
	select	QQ.ContractID as ContractID,
		CONVERT(datetime, IV.dtExpDate, 112) as dtExpDate,
		(DATEPART(yyyy, IV.dtExpDate)-2000)*12+DATEPART(m, IV.dtExpDate) as iExpiryID,
		IV.fStrike as fStrike,
		cast(IV.fIV as float) as fIV,
		GetDate() as dtActionDate
	from	(select	CU.ContractID,
			IV.fStrike,
			IV.dtExpDate,
			FR.UnderlyingID,
			max(FR.futureLotSize) as maxLosSize,
			count(FR.futureRootID) as fRootsNumber
		from FutureRoot FR
			inner join IVData.IV.Dbo.FT_DefaultIV_Futures IV on IV.vcSymbol = FR.futureRootSymbol
			inner join ContractView CU on CU.ContractID = FR.underlyingID and CU.ContractTypeID = 6
				and CU.ContractID = @iFutUnderlyingID
		group by IV.fStrike, IV.dtExpDate, FR.UnderlyingId, CU.ContractID
		) QQ
		inner join FutureRoot GH on GH.underlyingID = QQ.UnderlyingID and GH.futureLotSize = QQ.maxLosSize
		inner join IVData.IV.Dbo.FT_DefaultIV_Futures IV on IV.vcSymbol = GH.futureRootSymbol and QQ.fStrike = IV.fStrike and QQ.dtExpDate = IV.dtExpDate
	order by QQ.UnderlyingID, QQ.dtExpDate, QQ.fStrike
end
return @@error

GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.187')
	begin
		update DataInfo set keyValue = '3.46.187' where dataInfoID = 1
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
--	--SELECT * FROM #tmpErrors
--END
--GO
--IF @@TRANCOUNT > 0 
--BEGIN
--	select 'DB VERSION WAS CHANGED TO 3.46.187' '3.46.187 update message:' 
--	select 'The database update succeeded'
--
--	COMMIT TRANSACTION
--END
--	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO