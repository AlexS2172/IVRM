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
--SET TRANSACTION ISOLATION LEVEL REPEATABLEREAD
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.226'  or (keyValue = '3.46.227') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.227 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.227' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO



GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_LoadFromIV]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IndexDefinition_LoadFromIV]
GO


CREATE      Procedure dbo.usp_IndexDefinition_LoadFromIV
	@vcIndexSymbol varchar(20) = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if @vcIndexSymbol is null
	begin
		select *
		from IVData.IV.DBO.FT_Components
	end
	else
	begin
		select *
		from IVData.IV.DBO.FT_Components
		where index_symbol = @vcIndexSymbol
	end

	set @error = @@error
Return(@error)
GO



--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.227')
	begin
		update DataInfo set keyValue = '3.46.227' where dataInfoID = 1
	end
GO

/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
	--ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 3.46.227' '3.46.227 update message:' 
	select 'The database update succeeded'

--	COMMIT TRANSACTION
--END
	--ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO