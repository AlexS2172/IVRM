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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.178'  or (keyValue = '3.46.179') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.179 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.179' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
----------------------------------------------------------------
GO
GO

/* begin fixing FT8431 */
----------------------------------------------------------------
-- Add column to the FutureRoot table

if not exists(select 1 from syscolumns 
  where id=object_ID('ExpiryCalendar') AND Name='bBelongsToFutureRoot')
  ALTER TABLE dbo.ExpiryCalendar ADD bBelongsToFutureRoot tinyint NULL
GO
/* end fixing FT8431 */

ALTER  PROC dbo.usp_BoFutureRoot_Save
	@iFutureRootID int out,
	@iUnderlyingID int, -- should be specified for new future root (ignored for update)
	@vcFutureRootSymbol varchar(20) = null,
	@vcFutureRootName varchar(255) = null,
	@iFutureLotSize int = null,
	@iOptionLotSize int = null,
	@vcExportSymbol varchar(20) = null,
	@iMatCalendarID int = null,
	@tiIsRaiseError tinyint = 1,
	@tiQuoteFormat  tinyint = null,
	@iQuotationUnitAmmount int = null,
	@iQuotationUnitID int = null,
	@fQuoteDenomination float = null,
	@fTickSize float = null,
	@fTickValue float = null

AS
	-- create/update future root

	set nocount on

	declare @iExpCalendarID int
	declare @FutureRootSymbol as varchar(20)	
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
			futureLotSize, optionLotSize, exportSymbol, matCalendarID, tiQuoteFormat,
			iQuotationUnitAmmount,
			iQuotationUnitID,
			fQuoteDenomination,
			fTickSize,
			fTickValue
)
		values
			(@iUnderlyingID, @vcFutureRootSymbol, @vcFutureRootName,
			@iFutureLotSize, @iOptionLotSize, @vcExportSymbol, @iMatCalendarID, @tiQuoteFormat,
			@iQuotationUnitAmmount,
			@iQuotationUnitID,
			@fQuoteDenomination,
			@fTickSize,
			@fTickValue
)
		if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

		set @iFutureRootID = SCOPE_IDENTITY()

                /* begin fixing FT8413 */
		insert into ExpiryCalendar (expCalendarName, expCalendarDesc, bBelongsToFutureRoot) 
                  values(@vcFutureRootSymbol+'_Fut', @vcFutureRootName+' Fut', 1)		
		insert into ExpiryCalendar (expCalendarName, expCalendarDesc, bBelongsToFutureRoot) 
                  values(@vcFutureRootSymbol+'_Opt', @vcFutureRootName+' Opt', 1)
                /* end fixing FT8413 */

		set @iExpCalendarID = SCOPE_IDENTITY()
		update FutureRoot set MatCalendarID = @iExpCalendarID where FutureRootID = @iFutureRootID

		commit tran
		return @iFutureRootID
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

		select @FutureRootSymbol = FutureRootSymbol from FutureRoot where futureRootID = @iFutureRootID
		
		update ExpiryCalendar
		set expCalendarName = @vcFutureRootSymbol+'_Fut', 
		      expCalendarDesc = @vcFutureRootName+' Fut'
		where expCalendarID in (select expCalendarID from ExpiryCalendar where expCalendarName = @FutureRootSymbol+'_Fut')
		
		update ExpiryCalendar
		set expCalendarName = @vcFutureRootSymbol+'_Opt', 
		      expCalendarDesc = @vcFutureRootName+' Opt'
		where expCalendarID in (select expCalendarID from ExpiryCalendar where expCalendarName = @FutureRootSymbol+'_Opt')

		update Contract set expCalendarID = isnull(@iMatCalendarID, expCalendarID) where contractID in 
			(select fo.futureOptionID from FutureOption fo 
				inner join Future f on f.futureID = fo.futureID
				where f.futureRootID = @iFutureRootID)

		-- update future root
		update FutureRoot
		set futureRootSymbol = isnull(@vcFutureRootSymbol, futureRootSymbol),
			futureRootName = isnull(@vcFutureRootName, futureRootName),
			futureLotSize = isnull(@iFutureLotSize, futureLotSize),
			optionLotSize = isnull(@iOptionLotSize, optionLotSize),
			exportSymbol = isnull(@vcExportSymbol, exportSymbol),
			matCalendarID = isnull(@iMatCalendarID, matCalendarID),
			tiQuoteFormat = isnull(@tiQuoteFormat, tiQuoteFormat),
			iQuotationUnitAmmount = isnull(@iQuotationUnitAmmount, iQuotationUnitAmmount),
			iQuotationUnitID = isnull(@iQuotationUnitID, iQuotationUnitID),
			fQuoteDenomination = isnull(@fQuoteDenomination, fQuoteDenomination),
			fTickSize = isnull(@fTickSize, fTickSize),
			fTickValue = isnull(@fTickValue, fTickValue)
		where
			futureRootID = @iFutureRootID
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end		

		commit tran
	end

finish:
	return @error


GO

ALTER  PROCEDURE dbo.usp_ExpiryCalendar_Del 
	@iExpCalendarID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iExpCalendarID <= 3 
	begin
		Raiserror('You can''t delete system defined calendar!',16,1)
		Return(-1)
	end

        /*begin fixing FT8411*/
        /*--------*/
        if 1 = (select bBelongsToFutureRoot from ExpiryCalendar where expCalendarID=@iExpCalendarID)
        begin
		Raiserror('You can''t delete calendar belonging to some future root!',16,1)
		Return(-1)
        end
        /*end fixing FT8411*/

        begin tran
	        delete from ExpiryInCalendar where expCalendarID  = @iExpCalendarID 
	        update ExpiryInCalendar set actionID = 3 where expCalendarID  = @iExpCalendarID 
		        if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
	
--        	delete from ExpiryCalendar where expCalendarID  = @iExpCalendarID 
	        update ExpiryCalendar set actionID = 3 where expCalendarID  = @iExpCalendarID 
		        if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
        commit tran


	set @error = @@error

finish:
Return(@error)

/* 1 Field expCalendarName in table ExpiryCalendar must be unique*/

/* 2 It must be provided (check on 'delete' allready exists
     check on 'modify' must be added) that
     exactly two calendars exist with name 
     (i. e. futureRootSymbol+'_Opt') and (futureRootSymbol+'_Fut') respectively
       (those calendars must be removed or renamed 
         only together with future root 
         to which they belong))*/

/* 3 usp_BoFutureRoot_Del must be extended
   Check must by added whether some other future root points to one of it's 
     default calendars (i. e. calendars with name 
     futureRootSymbol+'_Opt' and futureRootSymbol+'_Fut') */

/* 4 In Fields 'Maturity calendar' and 'Expiration calendar' calendars assigned by user
   are shown. Not always they must be default calendars 
   (i. e. calendars with name 
     futureRootSymbol+'_Opt' and futureRootSymbol+'_Fut')*/

/* 1 */
alter table ExpiryCalendar
  add constraint AK_ExpiryCalendarexpCalendarName
  unique (expCalendarName)

GO
/* 2 */
--altering usp_ExpiryCalendar_Save


ALTER  PROCEDURE dbo.usp_ExpiryCalendar_Save 
	@iExpCalendarID int = null output, 
	@vcExpCalendarName varchar(50) = null,
	@vcExpCalendarDesc varchar(250) = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

        /* begin FT8412*/
        if @iExpCalendarID is not null
        begin
          if 1 = (select bBelongsToFutureRoot from ExpiryCalendar 
            where 
              expCalendarID=@iExpCalendarID
              and
              expCalendarName <> @vcExpCalendarName)
          begin
		Raiserror('You can''t modify a name of calendar belonging to some future root!',16,1)
		Return(-1)
          end
        end
        /* end   FT8412*/

	Begin Transaction
--		if not exists(select * From ExpiryCalendar where expCalendarID = @iExpCalendarID) 
		if @iExpCalendarID is null 
			begin
				insert into ExpiryCalendar 
					(expCalendarName, expCalendarDesc) 
					values (@vcExpCalendarName,@vcExpCalendarDesc)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iExpCalendarID = @@identity
			end
		else
			begin
				update ExpiryCalendar set 
					expCalendarName = isnull(@vcExpCalendarName, expCalendarName),
					expCalendarDesc = isnull(@vcExpCalendarDesc, expCalendarDesc)
				where expCalendarID = @iExpCalendarID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error=0 
	    set @error = @iExpCalendarID

Return(@error)

GO
/* 3 */
/* altering usp_BoFutureRoot_Del*/
ALTER PROC dbo.usp_BoFutureRoot_Del
	@iFutureRootID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future root (with all futures and futures options)

	set nocount on
	
	declare @expCalendarID_Opt int
	declare @expCalendarID_Fut int
	declare @error int		set @error = 0

	-- check future root ID
	if @iFutureRootID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Future root to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

        /*begin FT_CQ00008412*/
        declare @futureRootSymbol varchar(20)

        set @futureRootSymbol = (select futureRootSymbol from FutureRoot where futureRootID = @iFutureRootID)

        if exists 
          (select 1 from FutureRoot where 
            futureRootID <> @iFutureRootID
            and
            ( (matCalendarID in 
                (select expCalendarID from ExpiryCalendar where expCalendarName = @futureRootSymbol + '_Opt'))
              or
              (matCalendarID in 
                (select expCalendarID from ExpiryCalendar where expCalendarName = @futureRootSymbol + '_Fut')) ))
        begin
          raiserror('Can''t delete future root because some other future root points to it''s default calendar', 16, 1)
		set @error = -4
		goto finish
        end        
        /*end   FT_CQ00008412*/

	-- create temporary table with contracts to delete
	create table #FutureRootsContractsForDelete(contractID int, contractTypeID int)
	
	insert into #FutureRootsContractsForDelete
		select futureID, 4 from Future
			where futureRootID = @iFutureRootID
		union select fo.futureOptionID, 5 from FutureOption fo
			inner join Future f on fo.futureID = f.futureID
			where f.futureRootID = @iFutureRootID

	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureRootsContractsForDelete)
										or futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4))
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

	select @expCalendarID_Opt = e.expCalendarID from FutureRoot fr inner join ExpiryCalendar e on e.expCalendarName = (fr.FutureRootSymbol + '_Fut') where fr.futureRootID = @iFutureRootID
	select @expCalendarID_Fut = e.expCalendarID from FutureRoot fr inner join ExpiryCalendar e on e.expCalendarName = (fr.FutureRootSymbol + '_Opt') where fr.futureRootID = @iFutureRootID

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

	delete from ExpiryInCalendar where expCalendarID = @expCalendarID_Opt
	delete from ExpiryCalendar where expCalendarID = @expCalendarID_Opt

	delete from ExpiryInCalendar where expCalendarID = @expCalendarID_Fut
	delete from ExpiryCalendar where expCalendarID = @expCalendarID_Fut

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

/* altering usp_BoCommodity_Del*/
ALTER  PROC dbo.usp_BoCommodity_Del
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


	
        /*begin fixing FT8413*/
        create table #Deleted_FutureRootSymbols
                  (futureRootSymbol varchar(20), 
                  expCalendarName_Fut varchar(50),
                  expCalendarName_Opt varchar(50))

       create table #ExpiryCalendar_ForDelete(expCalendarID int)

       insert into #Deleted_FutureRootSymbols (futureRootSymbol)
                  (select futureRootSymbol from FutureRoot where underlyingID = @iCID)

       update #Deleted_FutureRootSymbols set expCalendarName_Fut = futureRootSymbol + '_Fut'
       update #Deleted_FutureRootSymbols set expCalendarName_Opt = futureRootSymbol + '_Opt'                

       insert into #ExpiryCalendar_ForDelete
                  select expCalendarID from ExpiryCalendar
                    where expCalendarName in 
                      (select expCalendarName_Fut from #Deleted_FutureRootSymbols)
       insert into #ExpiryCalendar_ForDelete
                  select expCalendarID from ExpiryCalendar
                    where expCalendarName in 
                      (select expCalendarName_Opt from #Deleted_FutureRootSymbols)

       /*begin FTCQ00008412*/
       if exists (select 1 from FutureRoot where 
                  underlyingID <> @iCID
                  and
                  matCalendarID in (select expCalendarID from #ExpiryCalendar_ForDelete))
                begin 
                   raiserror('Can''t delete future root because some other future root points to it''s default calendar', 16, 1)
                   return -3
                end
                /*end   FTCQ00008412*/
                /* По тексту */
                /*end fixing FT8413*/
	Begin Transaction CommodityDel
		exec @error = usp_TradeLog_Del @iCID
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		exec @error = usp_BoFutureRootByUnderlying_Del @iCID, 0
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update IndexBeta set actionID=3 where indexID = @iCID
			if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update ContractInGroup set actionID=3 where contractID = @iCID 
			if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

		delete FutureUnderlying where
		iFutureUnderlyingID = @iCID
		if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran CommodityDel goto finish end

		update TraderContract set actionID=3 where contractID = @iCID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction CommodityDel goto finish end

		update Contract set actionID=3 where contractID = @iCID
			if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

                /*begin update fixing FT8413*/
                update ExpiryCalendar set actionID=3
                  where expCalendarID in 
                    (select expCalendarID from #ExpiryCalendar_ForDelete)
                    if @@error <> 0 begin set @error = -22 if @@trancount = 1 rollback tran CommodityDel goto finish end

                update ExpiryInCalendar set actionID=3
                  where expCalendarID in 
                  (select expCalendarID from #ExpiryCalendar_ForDelete)
                  if @@error <> 0 begin set @error = -23 if @@trancount = 1 rollback tran CommodityDel goto finish end
                /*end update fixing FT8413*/

		set @error = @@error
	Commit Transaction CommodityDel

finish:
	return @error

GO

/* fixing FT_CQ00008370*/
ALTER PROCEDURE dbo.usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(20) = null,
		@vcContractName varchar(255) = null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsBasket tinyint = null,
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null,
		@tiCheckExistance tinyint = 1,
		@iUndPriceProfileID int = null output,
		@iOptPriceProfileID int = null output
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			if isnull(@tiCheckExistance, 1) <> 0
				Raiserror ('Index or stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		--Michael Malyshkin
		if @iExpCalendarID is null or @iExpCalendarID = 0
			set @iExpCalendarID = 2
		--Michael Malyshkin
		 
		 begin tran
			if @iUndPriceProfileID is null
			begin
				select
					@iUndPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 3
					and isOptionProfile = 0
			end

			if @iOptPriceProfileID is null
			begin
				select
					@iOptPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 1
			end

            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                  values (1, @vcSymbol, @vcContractName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)

			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (
							indexID, 
							yield,
							IsHTB,
							calcOptionType,
							calcModelType,
							isBasket, 
							isActive,
							skew,
							kurt)
					 values (
							@iIndexID, 
							@fYield,
							isNull(@bIsHTB,0),
							@tiCalcOptionType,
							@tiCalcModelType,
							isnull(@tiIsBasket,0), 
							@tiIsActive,
							@fSkew,
							@fKurt)
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 		
   		 insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, @vcSymbol)
                 /* begin FT_CQ00008370*/
                 declare @iExpCalendarID_Last int
                 set @iExpCalendarID_Last = SCOPE_IDENTITY()

                 update Contract set expCalendarID = @iExpCalendarID_Last
                   where contractID = @iIndexID
                 /* end   FT_CQ00008370*/
	
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@iExpCalendarID, expCalendarID),
					undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
					optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	yield = isnull(@fYield,yield),
					isHTB = isnull(@bIsHTB,isHTB),
					calcOptionType = isnull(@tiCalcOptionType, calcOptionType),
					calcModelType = isnull(@tiCalcModelType, calcModelType),
					isBasket = isnull(@tiIsBasket,isBasket),
					isActive = isnull(@tiIsActive, isActive),
					skew = isnull(@fSkew,Skew),
					kurt = isnull(@fKurt,Kurt)
			   where indexID = @iIndexID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   
		 commit tran
	 end

	--========================================================================
	-- make record with null exchange for this index
	--========================================================================
	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iIndexID)
		begin
			insert into ContractPrice(contractID,exchangeID)
				values(@iIndexID,null)
			if (@@error <> 0) begin rollback tran return(-1) end           		
		end		

if (@@error <> 0)
	return (-1)
else	
	return @iIndexID

GO
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.179')
	begin
		update DataInfo set keyValue = '3.46.179' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.179' '3.46.179 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO