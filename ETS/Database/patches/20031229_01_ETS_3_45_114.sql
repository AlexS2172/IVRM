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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.113' or (keyValue = '3.45.114')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.114 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.114' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_BoFutureRoot_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Del]
GO

CREATE  PROCEDURE dbo.usp_BoFutureRoot_Del
	@iFutureRootID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future root (with all futures and futures options)

	set nocount on
	
	declare @error int		set @error = 0

	-- check future root ID
	if @iFutureRootID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Future root to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

	-- create temporary table with contracts to delete
	create table #FutureRootsContractsForDelete(contractID int, contractTypeID int)
	
	insert into #FutureRootsContractsForDelete
		select futureID, 4 from Future
			where futureRootID = @iFutureRootID
		union select fo.futureOptionID, 5 from FutureOption fo
			inner join Future f on fo.futureID = f.futureID
			where f.futureRootID = @iFutureRootID

	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future root, because its futures or its future options has trades or position!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future root, because its futures or its future options has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran

	-- delete future options data
	delete FutureOption 
	where futureOptionID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 5)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future data
	delete Future
	where futureRootID = @iFutureRootID
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete future root
	delete FutureRoot
	where futureRootID = @iFutureRootID
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	-- delete common future and future options contracts data
	delete ContractPrice 
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end

	update ContractExchangeLink set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

	update Contract set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.114')
	begin
		update DataInfo set keyValue = '3.45.114' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.114' '3.45.114 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
