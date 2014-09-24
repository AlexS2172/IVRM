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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.140' or (keyValue = '4.49.141')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.141 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.140' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
-- these constraints are meaningless now. delete them.
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[FK_Option_Expiry]') AND OBJECTPROPERTY(ID, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Option] DROP CONSTRAINT [FK_Option_Expiry]
GO
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[FK_FutureOption_Expiry]') AND OBJECTPROPERTY(ID, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[FutureOption] DROP CONSTRAINT [FK_FutureOption_Expiry]
GO
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[FK_Future_Maturity]') AND OBJECTPROPERTY(ID, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Future] DROP CONSTRAINT [FK_Future_Maturity]
GO
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[FK_CustomStrikeSkewPoint_Expiry]') AND OBJECTPROPERTY(ID, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[CustomStrikeSkewPoint] DROP CONSTRAINT [FK_CustomStrikeSkewPoint_Expiry]
GO
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[expCalInIdCheck]') AND OBJECTPROPERTY(ID, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[CustomStrikeSkewPoint] DROP CONSTRAINT [expCalInIdCheck]
GO

-- temporaly disable triggers in options, future and futureoptions tables
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[tU_Option]') AND OBJECTPROPERTY(ID, N'IsTrigger') = 1)
DROP TRIGGER [dbo].[tU_Option]
GO
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[tU_FutureOption]') AND OBJECTPROPERTY(ID, N'IsTrigger') = 1)
DROP TRIGGER [dbo].[tU_FutureOption]
GO
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[tU_Future]') AND OBJECTPROPERTY(ID, N'IsTrigger') = 1)
DROP TRIGGER [dbo].[tU_Future]
GO
IF EXISTS (SELECT * FROM dbo.sysobjects WHERE ID = OBJECT_ID(N'[dbo].[tU_CustomStrikeSkewPoint]') AND OBJECTPROPERTY(ID, N'IsTrigger') = 1)
DROP TRIGGER [dbo].[tU_CustomStrikeSkewPoint]
GO
IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='CustomStrikeSkewPoint' AND COLUMN_NAME='expiryInCalendarID')
ALTER TABLE CustomStrikeSkewPoint DROP COLUMN expiryInCalendarID

GO

IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='EodFuture' AND COLUMN_NAME='maturityDate')
ALTER TABLE EodFuture ADD maturityDate DATETIME NOT NULL DEFAULT( GETDATE())
GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='EodOption' AND COLUMN_NAME='expiryDate')
ALTER TABLE EodOption ADD expiryDate DATETIME NOT NULL DEFAULT( GETDATE())
GO

IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='EodFutureOption' AND COLUMN_NAME='expiryDate')
ALTER TABLE EodFutureOption ADD expiryDate DATETIME NOT NULL DEFAULT( GETDATE())

GO

IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Option' AND COLUMN_NAME='expiryDate')
ALTER TABLE [Option] ADD expiryDate DATETIME NOT NULL DEFAULT( '2005-01-01' )
GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='FutureOption' AND COLUMN_NAME='expiryDate')
ALTER TABLE FutureOption ADD expiryDate DATETIME NOT NULL DEFAULT( '2005-01-01' )
GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Future' AND COLUMN_NAME='maturityDate')
ALTER TABLE Future ADD maturityDate DATETIME NOT NULL DEFAULT( '2005-01-01' )
GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='CustomStrikeSkewPoint' AND COLUMN_NAME='expiryDate')
ALTER TABLE CustomStrikeSkewPoint ADD expiryDate DATETIME NOT NULL DEFAULT( '2005-01-01' )
GO


-- now expiryID/maturityID field should contain date of expiration/maturity

UPDATE [option]
SET [option].expiryDate =  eic.expirydate 
FROM [option] o
JOIN contract oc ON oc.contractid = o.optionid
JOIN optionroot ort ON o.optionrootid = ort.optionrootid
JOIN contract uc ON ort.underlyingid = uc.contractid
JOIN expiryincalendar eic ON eic.expcalendarid = ISNULL(oc.expcalendarid,uc.expcalendarid) AND eic.expiryid = o.expiryid
WHERE o.expiryid < 35000 
GO
UPDATE futureoption
SET expiryDate = eic.ExpiryDate
FROM futureoption fo
JOIN contract foc ON foc.contractid = fo.futureoptionid
JOIN future f ON fo.futureid = f.futureid
JOIN futureroot fr ON f.futurerootid = fr.futurerootid
JOIN expiryincalendar eic ON eic.expcalendarid = ISNULL(foc.expcalendarid,fr.matcalendarid) AND eic.expiryid = fo.expiryid
WHERE fo.expiryid < 35000
GO
UPDATE future
SET maturityDAte = eic.expirydate 
FROM future f 
JOIN contract fc ON fc.contractid = f.futureid
JOIN expiryincalendar eic ON eic.expcalendarid = fc.expcalendarid AND eic.expiryid = f.maturityid
WHERE f.maturityid < 35000
GO
UPDATE CustomStrikeSkewPoint
SET CustomStrikeSkewPoint.expiryDate =  eic.expiryDate
FROM CustomStrikeSkewPoint cssp
JOIN VolaSurfaceData vsd ON vsd.volaSurfaceDataID = cssp.volaSurfaceDataID
JOIN Contract uc ON vsd.contractid = uc.contractid
JOIN ExpiryInCalendar eic ON eic.expiryID = cssp.expiryID AND eic.expCalendarID = uc.expCalendarID
WHERE cssp.expiryID < 35000
GO

delete from CustomStrikeSkewPoint where 
datediff(d,expiryDate, getDate()) > 0
GO

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.141')
	begin
		update DataInfo set keyValue = '4.49.141' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.141' '4.49.141 update message:' 
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
-- unable to execute this script in transaction