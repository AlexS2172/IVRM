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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.211'  or (keyValue = '3.46.212') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.212 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.212' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO



ALTER TABLE [dbo].[VolaSurfaceData] DROP CONSTRAINT [FK_VolaSurfaceData_Contract]


GO
ALTER TABLE [dbo].[VolaSurfaceData] ADD CONSTRAINT [FK_VolaSurfaceData_Contract] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [Contract] (
		[contractID]
	) 
GO



ALTER    PROC dbo.usp_BoFutureRootByUnderlying_Del
	@iUnderlyingID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future roots by underlying (with all futures and futures options)

	set nocount on
	
	declare @error int		set @error = 0

	-- check underlying ID
	if @iUnderlyingID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Underlying for future roots to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

	-- create temporary table with contracts to delete
	create table #FutureRootsContractsForDelete(contractID int, contractTypeID int)
	
	insert into #FutureRootsContractsForDelete
		select 
			f.futureID, 4
		from FutureRoot fr
			inner join Future f on fr.futureRootID = f.futureRootID
		where fr.underlyingID = @iUnderlyingID
		union select 
			fo.futureOptionID, 5 
		from FutureRoot fr
			inner join Future f on fr.futureRootID = f.futureRootID
			inner join FutureOption fo on fo.futureID = f.futureID
		where fr.underlyingID = @iUnderlyingID

	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future roots, because its futures or its future options has trades or position!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future roots, because its futures or its future options has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran

	-- delete vola
	delete CustomStrikeSkewPoint from CustomStrikeSkewPoint cs
	inner join VolaSurfaceData vs on vs.volaSurfaceDataID = cs.volaSurfaceDataID
	where vs.contractID = @iUnderlyingID
	if @@error <> 0 begin set @error = -10 if @@TranCount != 0 rollback tran goto finish end

	delete VolaSurfaceData where contractID = @iUnderlyingID
	if @@error <> 0 begin set @error = -11 if @@TranCount != 0 rollback tran goto finish end

	-- delete future options data
	delete FutureOption from FutureOption fo
	inner join #FutureRootsContractsForDelete fd on fd.contractID= fo.futureID
	where fd.contractTypeID = 4
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future data
	delete Future from Future f
	inner join #FutureRootsContractsForDelete fd on fd.contractID= f.futureID
	where fd.contractTypeID = 4
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete future root
	delete FutureRoot
	where underlyingID = @iUnderlyingID
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end


	-- delete common future and future options contracts data
	delete ContractPrice from ContractPrice cp
	inner join #FutureRootsContractsForDelete fd on fd.contractID= cp.contractID
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end

	delete ContractExchangeLink from ContractExchangeLink ce
	inner join #FutureRootsContractsForDelete fd on fd.contractID= ce.contractID
	where fd.contractTypeID = 4
	if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

	delete Contract from Contract c 
	inner join #FutureRootsContractsForDelete fd on fd.contractID= c.contractID
	if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO



ALTER       PROC usp_BoCommodity_Del 
	@iCID int,
	@tiIsRaiseError tinyint = 1 
as
/*
	Description: Commodity Delete procedure
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade where contractID = @iCID)
	   return -1

	if exists (select 1 from [Order] where contractID = @iCID)
	   return -2


	Begin Transaction CommodityDel
		exec @error = usp_TradeLog_Del @iCID
			if  @error != 0 begin 
                            set @error = -20 + @error 
                            if @@TranCount != 0 Rollback Tran CommodityDel 
                            goto finish 
                        end

		exec @error = usp_BoFutureRootByUnderlying_Del @iCID, 0
			if @error != 0 begin 
                           set @error = -100 + @error 
                           if @@TranCount != 0 Rollback Tran CommodityDel 
                           goto finish
                end

		delete IndexBeta where indexID = @iCID
		if @@error != 0 begin set @error = -5 if @@TranCount <> 0 Rollback Tran CommodityDel goto finish end

		delete ContractInGroup where contractID = @iCID 
		if @@error != 0 begin set @error = -6 if @@TranCount <> 0 Rollback Tran CommodityDel goto finish end

		delete FutureUnderlying where iFutureUnderlyingID = @iCID
		if @@error <> 0 begin set @error = -3 if @@trancount <> 1 rollback tran CommodityDel goto finish end

		delete TraderContract where contractID = @iCID  
			if @@error <> 0 begin set @error = -7 if @@TranCount <> 0 Rollback Transaction CommodityDel goto finish end

		delete Contract where contractID = @iCID
			if @@error != 0 begin set @error = -8 if @@TranCount <> 0 Rollback Tran CommodityDel goto finish end

		set @error = @@error
	Commit Transaction CommodityDel

finish:
	return @error
GO


GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.212')
	begin
		update DataInfo set keyValue = '3.46.212' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.212' '3.46.212 update message:' 
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
