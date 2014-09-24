use ETS_ASP_IV

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
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '1.1.2'  or (keyValue = '1.1.3') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '1.1.3 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '1.1.2' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

GO
if exists (select * from dbo.sysindexes where name = N'IX_IVSymbol_ilSymb' and id = object_id(N'[dbo].[IVSymbol]'))
drop index [dbo].[IVSymbol].[IX_IVSymbol_ilSymb]
GO


 CREATE  INDEX [IX_IVSymbol_ilSymb] ON [dbo].[IVSymbol]([ilVSymbolID]) WITH  FILLFACTOR = 50 ON [PRIMARY]
GO


if exists (select * from dbo.sysindexes where name = N'IX_IVSymbol_1' and id = object_id(N'[dbo].[IVSymbol]'))
drop index [dbo].[IVSymbol].[IX_IVSymbol_1]
GO

CREATE NONCLUSTERED INDEX IX_IVSymbol_1 ON dbo.IVSymbol
	(
	vcSymbolName
	) ON [PRIMARY]
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '1.1.3')
	begin
		update DataInfo set keyValue = '1.1.3' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 1.1.3' '1.1.3 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

