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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.135' or (keyValue = '4.49.136')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.136 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.136' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 --BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEod_RawData_StocksDefaultIVSurface_Load]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEod_RawData_StocksDefaultIVSurface_Load]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEod_RawData_FuturesDefaultIVSurface_Load]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEod_RawData_FuturesDefaultIVSurface_Load]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEod_RawData_DefaultIVSurface_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEod_RawData_DefaultIVSurface_Clear]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[IVUnderlyingPointsRawData]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [IVUnderlyingPointsRawData]
GO

CREATE TABLE IVUnderlyingPointsRawData
(
 iContractID  int, 
 dtExpDate    datetime, 
 iExpiryID    int,
 fStrike      float, 
 fIV          float,
 dtActionDate datetime
)

-- Create DefaultSurfaces if not exists
GO
create proc usp_AspEod_RawData_DefaultIVSurface_Clear
AS
 SET NOCOUNT ON
 truncate table IVUnderlyingPointsRawData

GO
create proc usp_AspEod_RawData_StocksDefaultIVSurface_Load
as

 insert into IVUnderlyingPointsRawData 
		(iContractID, dtExpDate, iExpiryID, fStrike, fIV, dtActionDate)
	 select cv.ContractID, 
                IV.dtExpDate, 
                (DATEPART(yyyy, IV.dtExpDate)-2000)*12+DATEPART(m, IV.dtExpDate) as iExpiryID,
                IV.fStrike,
                IV.fIV,
		GetDate() as dtActionDate
         from IVData.IV.Dbo.FT_DefaultIV IV
	 inner join ContractView cv on cv.Symbol = IV.vcSymbol and cv.ContractTypeID in (1,2)	


GO
create proc usp_AspEod_RawData_FuturesDefaultIVSurface_Load
as
 SET NOCOUNT ON

 insert into IVUnderlyingPointsRawData 
		(iContractID, dtExpDate, iExpiryID, fStrike, fIV, dtActionDate)
 select 
	QQ.ContractID   as ContractID,
	IV.dtExpDate	as dtExpDate,
	(DATEPART(yyyy, IV.dtExpDate)-2000)*12+DATEPART(m, IV.dtExpDate) as ExpiryID,
	IV.fStrike	as fStrike,
        cast( IV.fIV as float)*100	as fIV,
	GetDate() 	as dtActionDate
 from 
    (
	select 	cu.ContractID,
		IV.fStrike, 
		IV.dtExpDate, 
		fr.UnderlyingID, 
		max(fr.futureLotSize) as maxLosSize,
		count( fr.futureRootID) as fRootsNumber
	from FutureRoot fr 
		inner join IVData.IV.Dbo.FT_DefaultIV_Futures IV on IV.vcSymbol = fr.futureRootSymbol
		inner join ContractView cu on cu.ContractID = fr.underlyingID and cu.ContractTypeID = 6
	group by IV.fStrike, IV.dtExpDate,fr.UnderlyingId, cu.ContractID
   ) QQ
   inner join FutureRoot gh on gh.underlyingID=qq.UnderlyingID and gh.futureLotSize = qq.maxLosSize
   inner join IVData.IV.Dbo.FT_DefaultIV_Futures IV on IV.vcSymbol = gh.futureRootSymbol and qq.fStrike = IV.fStrike and qq.dtExpDate = iv.dtExpDate
   order by qq.UnderlyingID, qq.dtExpDate, qq.fStrike

GO

/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.136')
	begin
		update DataInfo set keyValue = '4.49.136' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.136' '4.49.136 update message:' 
	select 'The database update succeeded'
--
--	COMMIT TRANSACTION
--END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction