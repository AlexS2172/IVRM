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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.141' or (keyValue = '4.49.142')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.142 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.141' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[VegaWeight]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[VegaWeight]
GO

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[VegaWeight]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
 BEGIN
CREATE TABLE [dbo].[VegaWeight] (
	[ExpiryDate] [datetime] NOT NULL ,
	[VegaWeight] [float] NOT NULL 
) ON [PRIMARY]
END

GO

----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IX_Option_optionRootID_isCall_strike_expiryID_actionID' 
                                            and id = object_id(N'[dbo].[Option]'))
drop index [dbo].[Option].[IX_Option_optionRootID_isCall_strike_expiryID_actionID]
GO
CREATE 
  INDEX [IX_Option_optionRootID_isCall_strike_expiryID_actionID] ON [dbo].[Option] ([optionRootID], [isCall], [strike], [actionID], [expiryDate])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IDX_Option_28' 
                                            and id = object_id(N'[dbo].[Option]'))
drop index [dbo].[Option].[IDX_Option_28]
GO
CREATE 
  INDEX [IDX_Option_28] ON [dbo].[Option] ([optionID], [isCall], [strike], [expiryDate])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IX_Future_futureRootID_maturityID' 
                                            and id = object_id(N'[dbo].[Future]'))
drop index [dbo].[Future].[IX_Future_futureRootID_maturityID]
GO
CREATE 
  INDEX [IX_Future_futureRootID_maturityID] ON [dbo].[Future] ([futureRootID], [maturityDate])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IDX_FutureOption_ID' 
                                            and id = object_id(N'[dbo].[FutureOption]'))
drop index [dbo].[FutureOption].[IDX_FutureOption_ID]
GO
CREATE 
  INDEX [IDX_FutureOption_ID] ON [dbo].[FutureOption] ([futureOptionID], [isCall], [expiryDate], [strike])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IX_FutureOption_futureID_isCall_strike_expiryID' 
                                            and id = object_id(N'[dbo].[FutureOption]'))
drop index [dbo].[FutureOption].[IX_FutureOption_futureID_isCall_strike_expiryID]
GO
CREATE 
  INDEX [IX_FutureOption_futureID_isCall_strike_expiryID] ON [dbo].[FutureOption] ([futureID], [isCall], [strike], [expiryDate])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'OptionRoot38' 
                                            and id = object_id(N'[dbo].[OptionRoot]'))
drop index [dbo].[OptionRoot].[OptionRoot38]
GO
CREATE 
  INDEX [OptionRoot38] ON OptionRoot ([optionRootID], [underlyingID], [lotSize])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IX_FutureOption_Option' 
                                            and id = object_id(N'[dbo].[FutureOption]'))
drop index [dbo].[FutureOption].[IX_FutureOption_Option]
GO
CREATE 
  INDEX [IX_FutureOption_Option] ON [dbo].[FutureOption] ([futureID], [isCall], [expiryDate], [strike])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IDX_VegaWeight_Expiry' 
                                            and id = object_id(N'[dbo].[VegaWeight]'))
drop index [dbo].[VegaWeight].[IDX_VegaWeight_Expiry]
GO
CREATE UNIQUE 
  INDEX [IDX_VegaWeight_Expiry] ON [dbo].[VegaWeight] ([ExpiryDate], [VegaWeight])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'Option28' 
                                            and id = object_id(N'[dbo].[Option]'))
drop index [dbo].[Option].[Option28]
GO
CREATE 
  INDEX [Option28] ON [dbo].[Option] ([expiryDate], [actionID])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IX_FutureOption_ExpiryID' 
                                            and id = object_id(N'[dbo].[FutureOption]'))
drop index [dbo].[FutureOption].[IX_FutureOption_ExpiryID]
GO
CREATE 
  INDEX [IX_FutureOption_ExpiryID] ON [dbo].[FutureOption] ([expiryDate])
ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'IX_Future_Maturity' 
                                            and id = object_id(N'[dbo].[Future]'))
drop index [dbo].[Future].[IX_Future_Maturity]
GO
CREATE 
  INDEX [IX_Future_Maturity] ON [dbo].[Future] ([maturityDate])
ON [PRIMARY]

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


IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='EodOption' AND COLUMN_NAME='expiryID')
ALTER TABLE EodOption DROP COLUMN expiryID
GO

IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='EodFutureOption' AND COLUMN_NAME='expiryID')
ALTER TABLE EodFutureOption DROP COLUMN expiryID
GO

IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='EodFuture' AND COLUMN_NAME='maturityID')
ALTER TABLE EodFuture DROP COLUMN maturityID
GO


-- now, restore triggers

GO

if  exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_Option]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_Option]
GO

CREATE  Trigger tU_Option ON dbo.[Option]
    FOR UPDATE
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	SET NOCOUNT ON
	
	-- 1st step - update actionDate if record not already deleted
	UPDATE [Option]
		SET actionDate = GETDATE()
	FROM [Option] O 
			JOIN inserted ON O.optionID = inserted.optionID
			JOIN deleted ON O.optionID = deleted.optionID
	WHERE deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	UPDATE [Option]
		SET actionID=2
	FROM [Option] O
			JOIN inserted ON O.optionID = inserted.optionID
			JOIN deleted ON O.optionID = deleted.optionID
	WHERE deleted.actionID<3 AND inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	UPDATE [Option] 
		SET actionID = 3
	FROM [Option] O
			JOIN inserted ON O.optionID = inserted.optionID
			JOIN deleted ON O.optionID = deleted.optionID
	WHERE deleted.actionID=3

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_Future]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_Future]
GO


CREATE TRIGGER dbo.tU_Future ON [dbo].[Future]
    FOR UPDATE
AS
	SET NOCOUNT ON
	
	UPDATE Future
		SET actionDate = GETDATE()
	FROM Future f 
		JOIN inserted ON f.futureID = inserted.futureID

GO
if  exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_FutureOption]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_FutureOption]

GO
CREATE TRIGGER dbo.tU_FutureOption ON [dbo].[FutureOption]
    FOR UPDATE
AS
	SET NOCOUNT ON
	
	UPDATE FutureOption
		SET actionDate = GETDATE()
	FROM FutureOption fo 
		JOIN inserted ON fo.futureOptionID = inserted.futureOptionID
 
GO

if  exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_CustomStrikeSkewPoint]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_CustomStrikeSkewPoint]

GO
CREATE    Trigger tU_CustomStrikeSkewPoint ON dbo.CustomStrikeSkewPoint
    FOR UPDATE
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	SET NOCOUNT ON
	
	-- 1st step - update actionDate if record not already deleted
	UPDATE CustomStrikeSkewPoint 
		SET actionDate = GETDATE()
	FROM CustomStrikeSkewPoint CSSP 
			JOIN inserted ON CSSP.CustomStrikeSkewPointID = inserted.CustomStrikeSkewPointID
			JOIN deleted ON CSSP.CustomStrikeSkewPointID = deleted.CustomStrikeSkewPointID
	WHERE deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	UPDATE CustomStrikeSkewPoint
		SET actionID=2
	FROM CustomStrikeSkewPoint CSSP
			JOIN inserted ON CSSP.CustomStrikeSkewPointID = inserted.CustomStrikeSkewPointID
			JOIN deleted ON CSSP.CustomStrikeSkewPointID = deleted.CustomStrikeSkewPointID
	WHERE deleted.actionID<3 AND inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	UPDATE CustomStrikeSkewPoint 
		SET actionID = 3
	FROM CustomStrikeSkewPoint CSSP
			JOIN inserted ON CSSP.CustomStrikeSkewPointID = inserted.CustomStrikeSkewPointID
			JOIN deleted ON CSSP.CustomStrikeSkewPointID = deleted.CustomStrikeSkewPointID
	WHERE deleted.actionID=3

GO

if  exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ICFutures_Get]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[usp_ICFutures_Get]

GO
ALTER	PROC usp_ICFutures_Get
		@iICID INT = NULL
AS
	-- retrieve future
	SET NOCOUNT ON
	IF @iICID IS NOT NULL
		SELECT 
			CAST( F.maturityID AS DATETIME) AS MaturityDate, Cm.iFutureUnderlyingID  AS CommodityID, 
			Fr.futureRootName  AS FutureRootName, Fr.futureLotSize  AS FutureLotSize, 
			Fr.futureRootSymbol  AS FutureRootSymbol, CF.symbol   AS vcSymbol, Fr.optionLotSize  AS OptioLotSize, 
			Fr.exportSymbol  AS FRExportSymbol, CF.contractName  AS FutureName, F.futureRootID   AS FutureRootID,
			CP.priceClose  AS priceClose
		FROM 
			Future F
				INNER JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID And Fr.underlyingID =  @iICID
				INNER JOIN FutureUnderlying  CM ON Cm.iFutureUnderlyingID  =  @iICID 
				INNER JOIN Contract   CF  ON CF.ContractId    =  F.futureID
				LEFT JOIN ContractPrice CP ON CP.ContractID = CF.ContractId
		ORDER BY FutureName
	ELSE
		SELECT 
			CAST( F.maturityID AS DATETIME) AS MaturityDate, Cm.iFutureUnderlyingID  AS CommodityID, 
			Fr.futureRootName  AS FutureRootName, Fr.futureLotSize  AS FutureLotSize, 
			Fr.futureRootSymbol  AS FutureRootSymbol, CF.symbol   AS vcSymbol, Fr.optionLotSize  AS OptioLotSize, 
			Fr.exportSymbol  AS FRExportSymbol, CF.contractName  AS FutureName, F.futureRootID   AS FutureRootID,
			CP.priceClose  AS priceClose
		FROM
			Future F
				INNER JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID 
				INNER JOIN FutureUnderlying  CM ON  CM.iFutureUnderlyingID  IS NOT NULL 
				INNER JOIN Contract   CF  ON CF.ContractId    =  F.futureID
				LEFT JOIN ContractPrice CP ON CP.ContractID = CF.ContractId
		ORDER BY 
			FutureName

GO

GO
if exists (select * from dbo.sysindexes where name = N'CustomStrikeSkewPoint_9' and id = object_id(N'[dbo].[CustomStrikeSkewPoint]'))
drop index [dbo].[CustomStrikeSkewPoint].[CustomStrikeSkewPoint_9]
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.142')
	begin
		update DataInfo set keyValue = '4.49.142' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.142' '4.49.142 update message:' 
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