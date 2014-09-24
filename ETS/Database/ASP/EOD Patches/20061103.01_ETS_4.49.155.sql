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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.154' or (keyValue = '4.49.155')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.155 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.154' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*
DECLARE @RC int
EXEC @RC = [ETS_ASP_EOD].[dbo].[usp_AspEod_RawData_DefaultIVSurface_Clear] 
DECLARE @PrnLine nvarchar(4000)
PRINT 'Stored Procedure: ETS_ASP_EOD.dbo.usp_AspEod_RawData_DefaultIVSurface_Clear'
SELECT @PrnLine = '	Return Code = ' + CONVERT(nvarchar, @RC)
PRINT @PrnLine
*/
GO

delete from customstrikeskewpoint where customstrikeskewpointid in (
select distinct( cssp.customstrikeskewpointid) from customstrikeskewpoint cssp 
join customstrikeskewpoint cssp1 on cssp.volasurfacedataid = cssp1.volasurfacedataid 
and cssp.strike = cssp1.strike and cssp.expirydate = cssp1.expirydate and cssp.customstrikeskewpointid <> cssp1.customstrikeskewpointid 
and cssp.customstrikeskewpointid < cssp1.customstrikeskewpointid )
GO

ALTER      PROC usp_AspEod_RawData_StocksDefaultIVSurface_Load
AS
set nocount on
    INSERT INTO IVUnderlyingPointsRawData (iContractID, dtExpDate, fStrike, fIV, dtActionDate)
	   SELECT cv.ContractID, 
		  op.ExpiryDate, 
 		  IV.fStrike,
                  IV.fIV,GETDATE() AS dtActionDate
		FROM 
			IVData.IV.Dbo.FT_DefaultIV IV
	 			INNER JOIN ContractView cv ON cv.Symbol = IV.vcSymbol AND cv.ContractTypeID IN (1,2)
				INNER JOIN OptionRoot r on r.UnderlyingID = cv.ContractID
				INNER JOIN [Option] op on op.OptionRootID = r.OptionRootID and  abs(op.Strike - iv.fStrike) <.005 and abs(datediff(d,op.ExpiryDate,IV.dtExpDate))<5
	   GROUP BY cv.ContractID, op.ExpiryDate, IV.fStrike, IV.fIV


GO
/*
DECLARE @RC1 int
EXEC @RC1 = [ETS_ASP_EOD].[dbo].[usp_AspEod_RawData_StocksDefaultIVSurface_Load] 
DECLARE @PrnLine nvarchar(4000)
PRINT 'Stored Procedure: ETS_ASP_EOD.dbo.usp_AspEod_RawData_StocksDefaultIVSurface_Load'
SELECT @PrnLine = '	Return Code = ' + CONVERT(nvarchar, @RC1)
PRINT @PrnLine
*/
GO
--------------------------------------------------------------------------------------------------------------------------------------------------------
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.155')
	begin
		update DataInfo set keyValue = '4.49.155' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.155' '4.49.155 update message:' 
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

