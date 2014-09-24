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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.130' or (keyValue = '4.49.131')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.131 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.131' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


----------------------------------------------------------------------------------------------

ALTER TABLE [IndexBeta] DROP CONSTRAINT [FK_IndexBeta_Index]
GO
ALTER TABLE [IndexBeta] ADD CONSTRAINT [FK_IndexBeta_Index] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [Contract] (
		[contractID]
	)
GO

----------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where name = 'HedgeSymbolsBeta' and id = object_id(N'[dbo].[HedgeSymbolsBeta]'))
DROP TABLE HedgeSymbolsBeta
GO

CREATE TABLE [HedgeSymbolsBeta] (
	[HedgeSymbol] [nvarchar] (20) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[Symbol] [nvarchar] (20) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL ,
	[Beta] [float] NOT NULL
) ON [PRIMARY]
GO

----------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where name = 'usp_HedgeSymbolsBeta_Import' and id = object_id(N'[dbo].[usp_HedgeSymbolsBeta_Import]'))
DROP PROCEDURE dbo.usp_HedgeSymbolsBeta_Import
GO

CREATE	PROCEDURE dbo.usp_HedgeSymbolsBeta_Import
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	truncate table HedgeSymbolsBeta
	insert into HedgeSymbolsBeta (HedgeSymbol, Symbol, Beta)
	(
		select vcHedgeSymbol, vcSymbol, fBeta
		from IVData.IV.DBO.FT_Hedge_Symbols hs
		where fBeta is not null
	)


	truncate table IndexBeta
	insert into IndexBeta (indexID, contractID, beta)
	(
		select 	cv1.ContractID, cv2.ContractID, Beta
		from HedgeSymbolsBeta hs
			join ContractView cv1 on cv1.symbol = hs.HedgeSymbol and cv1.contractTypeID in (1, 2)
			join ContractView cv2 on cv2.symbol = hs.Symbol and cv2.contractTypeID in (1, 2)
	)

	truncate table HedgeSymbolsBeta

	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------


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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.131')
	begin
		update DataInfo set keyValue = '4.49.131' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.131' '4.49.131 update message:' 
	select 'The database update succeeded'
--
--	COMMIT TRANSACTION
--END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction