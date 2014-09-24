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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.111' or (keyValue = '3.45.112')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.112 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.112' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_BoFutureRootByUnderlying_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRootByUnderlying_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRootByUnderlying_Del]
GO

CREATE PROC dbo.usp_BoFutureRootByUnderlying_Del
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

	-- delete future options data
	delete FutureOption 
	where futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future data
	delete Future
	where futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete future root
	delete FutureRoot
	where underlyingID = @iUnderlyingID
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

----------------------------------------------------------------------------------------------------
-- usp_BoFutureRoot_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Save]
GO

CREATE PROC dbo.usp_BoFutureRoot_Save
	@iFutureRootID int out,
	@iUnderlyingID int, -- should be specified for new future root (ignored for update)
	@vcFutureRootSymbol varchar(20) = null,
	@vcFutureRootName varchar(255) = null,
	@iFutureLotSize int = null,
	@iOptionLotSize int = null,
	@vcExportSymbol varchar(20) = null,
	@iMatCalendarID int = null,
	@tiIsRaiseError tinyint = 1 
AS
	-- create/update future root

	set nocount on
	
	declare @error int		set @error = 0

	if @iFutureRootID is null -- new future root
	begin
		-- check underlying ID
		if @iUnderlyingID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying for new future root is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		-- check future root symbol
		set @vcFutureRootSymbol = ltrim(isnull(@vcFutureRootSymbol, ''))
		if len(@vcFutureRootSymbol) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future root is not specified!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future root
		if exists(select 1 from FutureRoot where futureRootSymbol = @vcFutureRootSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
			set @error = -3
			goto finish
		end

		set @vcExportSymbol = ltrim(isnull(@vcExportSymbol, ''))
		if len(@vcExportSymbol) > 0
		begin
			-- check export symbol
			if exists(select 1 from FutureRoot
					where exportSymbol = @vcExportSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future root with import ID ''%s'' is already exists!', 16, 1, @vcExportSymbol)
				set @error = -104
				goto finish
			end
		end

		-- correct lot sizes
		if isnull(@iFutureLotSize, 0) <= 0
			set @iFutureLotSize = 100

		if isnull(@iOptionLotSize, 0) <= 0
			set @iOptionLotSize = 1

		-- correct maturity calendar ID
		if isnull(@iMatCalendarID, 0) = 0
			set @iMatCalendarID = 1

		-- check maturity calendar ID
		if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
			set @error = -4
			goto finish
		end

		begin tran

		-- insert new future root
		insert into FutureRoot
			(underlyingID, futureRootSymbol, futureRootName,
			futureLotSize, optionLotSize, exportSymbol, matCalendarID)
		values
			(@iUnderlyingID, @vcFutureRootSymbol, @vcFutureRootName,
			@iFutureLotSize, @iOptionLotSize, @vcExportSymbol, @iMatCalendarID)
		if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

		set @iFutureRootID = @@identity

		commit tran
	end
	else
	begin

		-- correct future root name (prevent updates to empty string)
		if len(ltrim(@vcFutureRootSymbol)) = 0
			set @vcFutureRootSymbol = null

		-- check future root
		if @vcFutureRootSymbol is not null
		begin
			if exists(select 1 from FutureRoot where futureRootID <> @iFutureRootID and futureRootSymbol = @vcFutureRootSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
				set @error = -6
				goto finish
			end
		end

		-- check future root export symbol
		if @vcExportSymbol is not null
		begin
			set @vcExportSymbol = ltrim(@vcExportSymbol)
			
			if len(@vcExportSymbol) > 0 -- export symbol update
			begin
				-- check export symbol
				if exists(select 1 from FutureRoot
						where futureRootID <> @iFutureRootID and exportSymbol = @vcExportSymbol)
				begin
					if @tiIsRaiseError = 1 raiserror('Future root with import ID ''%s'' is already exists!', 16, 1, @vcExportSymbol)
					set @error = -104
					goto finish
				end
			end
		end
		
		-- correct future lot size (prevent updates to invalid lot size)
		if @iFutureLotSize <= 0
			set @iFutureLotSize = null

		-- correct option lot size (prevent updates to invalid lot size)
		if @iOptionLotSize <= 0
			set @iOptionLotSize = null

		-- correct maturity calendar ID (prevent updates to zero calendar)
		if @iMatCalendarID = 0
			set @iMatCalendarID = null

		-- check maturity calendar
		if @iMatCalendarID is not null
		begin
			if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
				set @error = -7
				goto finish
			end
		end

		begin tran

		-- update future root
		update FutureRoot
		set futureRootSymbol = isnull(@vcFutureRootSymbol, futureRootSymbol),
			futureRootName = isnull(@vcFutureRootName, futureRootName),
			futureLotSize = isnull(@iFutureLotSize, futureLotSize),
			optionLotSize = isnull(@iOptionLotSize, optionLotSize),
			exportSymbol = isnull(@vcExportSymbol, exportSymbol),
			matCalendarID = isnull(@iMatCalendarID, matCalendarID)
		where
			futureRootID = @iFutureRootID
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end

finish:
	return @error

GO

/* RESULTS ANALYTHING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.112')
	begin
		update DataInfo set keyValue = '3.45.112' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.112' '3.45.112 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
