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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.216'  or (keyValue = '3.46.217') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.217 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.217' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionRootID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionRootID_Get]

GO

CREATE  PROCEDURE dbo.usp_OptionRootID_Get 
	@sRootName varchar(20) = null,
	@iUnderlyingID int = null
AS
/*
	Description: this procedure is getting information from OptionRoot table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select  orv.OptionRootID as iOptionRootID, orv.underlyingID as iUnderlyingID,
		orv.symbol as vcSymbol,
		orv.IsSynthetic as tiIsSynthetic, 
		orv.lotSize as iLotSize, 
		orv.isFitEnabled as tiIsFitEnabled,
		sor.cashValue as fCash,
		sor.synthSkew as fSyntheticSkew, 
		sor.synthKurt as fSyntheticKurt,
		sor.synthYield as fSyntheticYield, 
		sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView as orv
		left join SyntheticOptionRootView as sor 
		on orv.optionRootID = sor.optionRootID
		where	(orv.underlyingID = @iUnderlyingID or @iUnderlyingID is null)
			and (orv.symbol=@sRootName or @sRootName is null)


	set @error = @@error

Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.217')
	begin
		update DataInfo set keyValue = '3.46.217' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.217' '3.46.217 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
