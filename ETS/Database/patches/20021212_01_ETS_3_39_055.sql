/*
fix for MLine00000780 ---ETS Back Office
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.054' or (keyValue = '3.39.055')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.39.055 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.055' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItem_Del]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_BookRuleItem_Del 
	@iBookRuleItemID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from BookRuleItem table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete BookRuleItemMask where bookRuleItemID = @iBookRuleItemID 
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		delete BookRuleItem	where bookRuleItemID = @iBookRuleItemID 
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
finish:
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.055')
	begin
		update DataInfo set keyValue = '3.39.055' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.39.055' '3.39.055 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

