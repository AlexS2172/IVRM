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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.150' or (keyValue = '4.49.151')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.151 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.150' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 --BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[IVUnderlyingPointsRawData]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IVUnderlyingPointsRawData]
GO
CREATE TABLE [IVUnderlyingPointsRawData] (
	[iContractID] [int] NULL ,
	[dtExpDate] [datetime] NULL ,
	[fStrike] [float] NULL ,
	[fIV] [float] NULL ,
	[dtActionDate] [datetime] NULL 
) ON [PRIMARY]
GO

ALTER  PROC usp_AspEod_RawData_StocksDefaultIVSurface_Load
AS
set nocount on
    INSERT INTO IVUnderlyingPointsRawData (iContractID, dtExpDate, fStrike, fIV, dtActionDate)
	   SELECT cv.ContractID, 
		  IV.dtExpDate, 
 		  IV.fStrike,
                  IV.fIV,GETDATE() AS dtActionDate
		FROM 
			IVData.IV.Dbo.FT_DefaultIV IV
	 			INNER JOIN ContractView cv ON cv.Symbol = IV.vcSymbol AND cv.ContractTypeID IN (1,2)
				

GO


ALTER    PROC usp_AspEod_RawData_FuturesDefaultIVSurface_Load
AS
 SET NOCOUNT ON

 INSERT INTO IVUnderlyingPointsRawData (iContractID, dtExpDate, fStrike, fIV, dtActionDate)
 	SELECT 
		QQ.ContractID   AS ContractID, 
        	IV.dtExpDate	AS dtExpDate,
		IV.fStrike	AS fStrike,
		CAST( IV.fIV AS FLOAT) AS fIV, 
                GETDATE() AS dtActionDate
	FROM 
		(
			SELECT	cu.ContractID, IV.fStrike, IV.dtExpDate, fr.UnderlyingID, 
					MAX(fr.futureLotSize) AS maxLosSize, COUNT( fr.futureRootID) AS fRootsNumber
			FROM 
				FutureRoot fr 
				INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = fr.futureRootSymbol
				INNER JOIN ContractView cu ON cu.ContractID = fr.underlyingID AND cu.ContractTypeID = 6
			GROUP BY IV.fStrike, IV.dtExpDate,fr.UnderlyingId, cu.ContractID
		) QQ
		INNER JOIN FutureRoot gh ON gh.underlyingID=qq.UnderlyingID AND gh.futureLotSize = qq.maxLosSize
		INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = gh.futureRootSymbol AND qq.fStrike = IV.fStrike and qq.dtExpDate = iv.dtExpDate
		ORDER BY qq.UnderlyingID, qq.dtExpDate, qq.fStrike

GO
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.151')
	begin
		update DataInfo set keyValue = '4.49.151' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.151' '4.49.151 update message:' 
	select 'The database update succeeded'
--
-- @@TRANCOUNT > 0 
--GIN
--	COMMIT TRANSACTION
--D
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction