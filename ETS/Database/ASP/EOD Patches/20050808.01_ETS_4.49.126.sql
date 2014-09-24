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
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.125' or (keyValue = '4.49.126')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.126 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.126' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
ALTER  PROC dbo.usp_EodFuture_Del
AS
	set nocount on
	
	declare @error int		set @error = 0

	if object_ID('tempdb..#FutureContractsForDelete') is null
	begin
		set @error = -1
		goto finish
	end

	begin tran
		
	-- delete future option
	delete FutureOption
	where futureID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
	
	-- delete future
	delete Future where
	futureID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end


	-- delete common future and future options
	
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end
	
	delete ContractExchangeLink
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	delete  VolaSurfaceData
	where contractID in (select contractID from #FutureContractsForDelete)

	delete Contract
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error


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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.126')
	begin
		update DataInfo set keyValue = '4.49.126' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.126' '4.49.126 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction
