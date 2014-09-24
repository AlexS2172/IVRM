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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.186'  or (keyValue = '3.46.187') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.187 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.187' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*BEGIN DATA MODIFICATION SECTION*/

/*FutUnd_4_Procs***************************/
--Created by Nikita Kychkin
--14.08.2006
/*******************************************/

/*==========================================================================================*/
/*PROCEDURES*/
/*==========================================================================================*/
/*usp_ActiveSurface_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Get]
go
create            Procedure usp_ActiveSurface_Get 
	@vcSymbolName varchar(20) --the contract symbol
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iContractID int
		select @iContractID = contractID from ContractView where symbol = @vcSymbolName and  contractTypeID in (1,2,6)

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
	set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select	@iVolaSurfaceDataID as iVolaSurfaceDataID


	set @error = @@error
Return(@error)
go


/*usp_BoCommodity_Del*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCommodity_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoCommodity_Del]
go
create  PROC usp_BoCommodity_Del
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
                /* ?? ?????? */
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
go

/*usp_BoCommodity_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoCommodity_Get]
go
create PROC usp_BoCommodity_Get
		@iInterestID int = null
	AS
		-- retrieve future
	
		set nocount on
	
		if @iInterestID is not null
			SELECT 	c.tiGreeksCalculationType as GreeksCalcType,
				c.tiActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.iFutureUnderlyingID as iContractID,
				'' as expCalendarName,
				--ecv.expCalendarName as expCalendarName,
				c.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID

			FROM         dbo.FutureUnderlying C INNER JOIN
		                      dbo.ContractView CV ON C.iFutureUnderlyingID = CV.contractID
				--inner join ExpiryCalendarView ecv on cv.expCalendarID = ecv.expCalendarID
			WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 6
			Order by CV.Symbol
	
		else
		
			SELECT 	c.tiGreeksCalculationType as GreeksCalcType,
				c.tiActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.iFutureUnderlyingID as iContractID,
				'' as expCalendarName,
				--ecv.expCalendarName as expCalendarName,
				c.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID

			FROM         dbo.FutureUnderlying C INNER JOIN
		                      dbo.ContractView CV ON C.iFutureUnderlyingID = CV.contractID
				--inner join ExpiryCalendarView ecv on cv.expCalendarID = ecv.expCalendarID
			WHERE 	CV.ContractTypeID = 6
			Order by CV.Symbol
go

/*usp_BoContractsSymbol_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoContractsSymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoContractsSymbol_Get]
go
create proc usp_BoContractsSymbol_Get
	@iContractTypeID int = null
as
	if isnull(@iContractTypeID, 0) <> 0
		select	
			C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol ,
			fu.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID
		from ContractView c  
		left join FutureUnderlying fu on C.contractID = fu.iFutureUnderlyingID
		where c.contractTypeID = @iContractTypeID
		order by C.symbol
	else
		select	
			C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol 
		from ContractView c  
		where c.contractTypeID in (1, 2, 4)
		order by C.symbol

Return (@@Error)
go

/*usp_BoFutureOptionWithPrice_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoFutureOptionWithPrice_Get]
go
create proc usp_BoFutureOptionWithPrice_Get
	  @iFutureID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			null as iOptionRootID,
			ec.expiryDate as dtExpiry,
			fo.strike as fStrike,
			fo.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol
	from FutureOption fo
			inner join ContractView c on c.contractID = fo.futureOptionID
			inner join ContractView fc on fc.contractID = fo.futureID
			inner join ExpiryInCalendarView ec on fo.expiryID = ec.expiryID and c.expCalendarID = ec.expCalendarID -- fc.expCalendarID
			left join ContractPrice cp on fo.futureOptionID = cp.contractID and cp.exchangeID is null
	where	fo.futureID = @iFutureID
			and ec.expiryDate >= getdate()
	order by year(EC.expiryDate), month(EC.expiryDate), fo.strike, fo.isCall desc

  return @@error
go

/*usp_BoFutureRootWithSymbol_Exist*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRootWithSymbol_Exist]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoFutureRootWithSymbol_Exist]
go
create   PROC usp_BoFutureRootWithSymbol_Exist
	@vcFutureRootSymbol varchar(20)

AS

declare @result as tinyint

if exists(select 1 from FutureRoot where futureRootSymbol = @vcFutureRootSymbol)
	begin
		set @result = 1
	end
else
	begin
		set @result = 0
	end

return @result
go

/*usp_BoFutureRoot_Del*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoFutureRoot_Del]
go
create PROC usp_BoFutureRoot_Del
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
go

/*usp_BoFutureRoot_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoFutureRoot_Get]
go
create  PROC usp_BoFutureRoot_Get
  @iUnderlyingID int = null
AS
  -- retrieve future root(s) by underlying

  set nocount on

  if @iUnderlyingID is not null
    select
      fr.futureRootID as iFutureRootID,
      uc.contractID as iUnderlyingID,
      uc.contractTypeID as iUnderlyingContractTypeID,
      uc.symbol as vcUnderlyingSymbol,
      fr.futureRootSymbol as vcFutureRootSymbol,
      fr.futureRootName as vcFutureRootName,
      fr.futureLotSize as iFutureLotSize,
      fr.optionLotSize as iOptionLotSize,
      fr.exportSymbol as vcExportSymbol,
      fr.matCalendarID as iMatCalendarID,
      fr.actionDate as dtActionDate,
      fr.tiQuoteFormat,
      fr.iQuotationUnitAmmount,
      fr.iQuotationUnitID,
      fr.fQuoteDenomination,
      fr.fTickSize,
      fr.fTickValue,
      fr.vcQuotationUnitCurrency as vcQuotationUnitName

    from FutureRoot fr
      inner join ContractView uc on fr.underlyingID = uc.contractID
      left join QuotationUnit qu on fr.iQuotationUnitID = qu.iQuotationUnitID
    where
      fr.underlyingID = @iUnderlyingID
    order by
      fr.futureRootSymbol
  else
    select
      fr.futureRootID as iFutureRootID,
      uc.contractID as iUnderlyingID,
      uc.contractTypeID as iUnderlyingContractTypeID,
      uc.symbol as vcUnderlyingSymbol,
      fr.futureRootSymbol as vcFutureRootSymbol,
      fr.futureRootName as vcFutureRootName,
      fr.futureLotSize as iFutureLotSize,
      fr.optionLotSize as iOptionLotSize,
      fr.exportSymbol as vcExportSymbol,
      fr.matCalendarID as iMatCalendarID,
      fr.actionDate as dtActionDate,
      fr.tiQuoteFormat,
      fr.iQuotationUnitAmmount,
      fr.iQuotationUnitID,
      fr.fQuoteDenomination,
      fr.fTickSize,
      fr.fTickValue,
      fr.vcQuotationUnitCurrency as vcQuotationUnitName

    from FutureRoot fr
      inner join ContractView uc on fr.underlyingID = uc.contractID
      left join QuotationUnit qu on fr.iQuotationUnitID = qu.iQuotationUnitID
    order by
      uc.symbol, fr.futureRootSymbol
go

/*usp_BoFutureRoot_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoFutureRoot_Save]
go
create  PROC usp_BoFutureRoot_Save
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

                /*begin FT_CQ00007114*/
                update Contract set expCalendarID = isnull(@iMatCalendarID, expCalendarID) where contractID in
                        (select futureID from Future f
                                inner join FutureRoot fr on f.futureRootID = fr.futureRootID
                                where f.futureRootID = @iFutureRootID)
                /*end FT_CQ00007114*/

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
go

/*usp_BoFuture_Import*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoFuture_Import]
go
create  PROCEDURE usp_BoFuture_Import
	@iFutureID int,
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
        @fPriceClose float,
	@vcBadFutureName varchar(255) output,
        @fTickSize float = null,
        @iQuotationUnitID int = null,
        @vcQuotationUnitCurrency varchar(3) = null,
	@dtExpiryDate as datetime = null	
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare @tiIsActive tinyint
	
	if @iFutureRootID is null
		select @iFutureRootID = futureRootID
			from Future (nolock)
			where futureID = @iFutureID
			
	exec @error = usp_EodFuture_Import @iFutureRootID,@iExpiryID,@vcSymbol,@vcContractName,@iLotSizeID,@iOptionLotSizeID, @tiIsActive output, @vcBadFutureName output, @fPriceClose, @fTickSize, @iQuotationUnitID, @vcQuotationUnitCurrency, @dtExpiryDate
	 
finish:
Return(@error)
go

/*usp_BoIndex_Del*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoIndex_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoIndex_Del]
go
create PROC usp_BoIndex_Del
	@iIndexID int
as
/*
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @expCalendarID int
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade where contractID = @iIndexID)
	   return -1

	if exists (select 1 from [Order] where contractID = @iIndexID)
	   return -2

	Create table #Options_for_delete(OptionID int)

	select @expCalendarID = e.expCalendarID from ContractView cv inner join ExpiryCalendar e on e.expCalendarName = cv.symbol where cv.ContractID = @iIndexID and cv.contractTypeID = 1

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iIndexID

	if exists (select 1 from Trade where contractID in (select optionID from #Options_for_delete))
		return -3

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		return -4

	Begin Transaction IndexDel

		exec @error = usp_TradeLog_Del @iIndexID
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = 0
			end

		exec @error = usp_BoFutureRootByUnderlying_Del @iIndexID, 0
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID1 = @iIndexID
			if @@error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID2 = @iIndexID
			if @@error != 0 begin set @error = -6 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexDataHistory where indexID = @iIndexID
			if @@error != 0 begin set @error = -7 if @@TranCount != 0 Rollback Tran IndexDel goto finish end							
		
		delete from ExpiryInCalendar where expCalendarID = @expCalendarID  					
		delete from ExpiryCalendar where expCalendarID = @expCalendarID

		update IndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexDefinition set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -9 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -11 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 
			where contractID in 
				(select OptionID 
					from [Option] O 
							join OptionRoot ORt on O.optionRootID=ORt.optionRootID and ORt.underlyingID=@iIndexID)
			if @@error != 0 begin set @error = -12 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iIndexID or syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -13 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -14 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -15 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -16 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -17 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iIndexID
			if @@error != 0 begin set @error = -18 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update TraderContract set actionID=3 where contractID = @iIndexID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction IndexDel goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran IndexDel goto finish end		
		
		set @error = @@error
	Commit Transaction IndexDel
finish:
	return @error
go

/*usp_BoInterest_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoInterest_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoInterest_Get]
go
create  PROC usp_BoInterest_Get
		@iInterestID int = null
	AS
		-- retrieve future
	
		set nocount on
	
		if @iInterestID is not null
			SELECT 	c.GreeksCalculationType as GreeksCalcType,
				c.IsActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.CommodityID as iContractID 
			FROM         dbo.Commodity C INNER JOIN
		                      dbo.ContractView CV ON C.CommodityID = CV.contractID
			WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 6
			Order by CV.Symbol
	
		else
		
			SELECT 	c.GreeksCalculationType as GreeksCalcType,
				c.IsActive as tiIsActive, 
				CV.contractTypeID as iContractTypeID,
				CV.symbol as vcSymbol, 
		                CV.contractName as vcContractName, 
				c.CommodityID as iContractID 
			FROM         dbo.Commodity C INNER JOIN
		                      dbo.ContractView CV ON C.CommodityID = CV.contractID
			WHERE 	CV.ContractTypeID = 6
			Order by CV.Symbol
go

/*usp_BoTrade_StoreToHistoryAndClear*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrade_StoreToHistoryAndClear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoTrade_StoreToHistoryAndClear]
go
create PROC usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	-- clear simulated and manual deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0)
	if @@error !=0 begin set @error = -2 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (0, 3) and actionID = 0)
	if @@error !=0 begin set @error = -4 if @@TranCount = 1 Rollback Tran goto finish end

	if isnull(@tiStoreToHistory, 0) <> 0
	begin
		-- move actual and actual matched trades to history
		insert into TradeHistory
		select
			getdate() as dtActionDate,
	
			tr.tradeID as iTradeID, tr.tradeDate as dtTradeDate, tr.quantity as iQuantity,
			tr.price as fPrice, tr.isBuy as tiIsBuy, tr.status as tiStatus, tr.execID as vcExecID,
			tr.mark as iTradeMark,
			
			c.contractID as iContractID, c.contractTypeID as iContractTypeID, c.Symbol as vcSymbol,
			c.exportSymbol as vcExportSymbol, cp.priceClose as fPriceClose,
	
			uc.contractID as iUnderlyingContractID, uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol, uc.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			ucp.priceClose as fUnderlyingPriceClose,
	
			f.futureID as iFutureID, fr.futureRootSymbol as vcFutureRootSymbol,
			fc.symbol as vcFutureSymbol, fc.exportSymbol as vcFutureExportSymbol,
			fmc.expiryDate as dtFutureMaturity, fr.futureLotSize as iFutureLotSize,
			fcp.priceClose as fFuturePriceClose,
	
			ort.optionRootID as iOptionRootID, ort.symbol as vcOptionRootSymbol, ort.lotSize as iOptionRootLotSize,
	
			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtOptionExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fOptionStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiOptionIsCall,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference,

			fr.futureRootID as iFutureRootID,
			fr.optionLotSize as iFutureOptionLotSize
	
		from TradeView tr
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join StockView s on tr.underlyingID = s.stockID
			left join IndexView i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- additional
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
	
		set @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 if @@TranCount = 1 Rollback Tran goto finish end
	end

	-- clear actual and actual matched trades
	delete from Trade 
		where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0
	if @@error !=0 begin set @error = -7 if @@TranCount = 1 Rollback Tran goto finish end

	commit tran

finish:
	return @error
go

/*usp_BoUnderlyings_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoUnderlyings_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoUnderlyings_Get]
go
create PROC usp_BoUnderlyings_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------	
	select
		contractID as iContractID, 
		contractTypeID as iContractTypeID, 
		symbol as vcSymbol
	from ContractView
	where contractTypeID in (1, 2, 6)
	order by vcSymbol

	return @@error
go

/*usp_ContractInGroup_Import*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroup_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroup_Import]
go
create    procedure usp_ContractInGroup_Import  
-- Created by Artem Shylaev Egar T  
-- 2005 - 08 - 24  
-- Modified by Anar Alimov
-- 2005 - 11 - 03
  @vXMLData ntext  
As  
  DECLARE @idoc int  
  EXEC sp_xml_preparedocument @idoc OUTPUT, @vXMLData  
  
  select V_XML.Symbol As SymbolsNotExist  
         from ( SELECT * FROM OPENXML(@idoc, '/Groups/Group',1)   
			 WITH (GroupId [varchar] (255) , Symbol[varchar](255) )      
               ) V_XML  
	 where V_XML.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS not in (
		select symbol from contract where ContractTypeID in (1, 2) and actionID < 3
	 )  
    
  Begin Tran  
  
    update ContractInGroup set actionid = 1 where actionid = 2  
    update ContractGroup set actionid = 1 where actionid = 2  
    delete ContractInGroup where groupid in (
	select groupid from ContractGroup where actionid = 3)  
    delete ContractGroup where actionid = 3  
    delete ContractInGroup where actionid = 3  
    delete ContractInGroup where groupid in ( 
	select CG.groupid from ContractGroup  CG  
		join ( SELECT distinct GroupId FROM OPENXML (@idoc, '/Groups/Group',1)   
						    WITH (GroupId [varchar] (255)
		     )      
                ) V_XML on  V_XML.GroupID =  CG.GroupName COLLATE SQL_Latin1_General_CP1_CI_AS
	)   
  
  
insert into ContractGroup   ( groupName , [description] )  
    select V_XML.GroupId As groupName , '' As [description]  
         from
		( SELECT distinct GroupId FROM OPENXML (@idoc, '/Groups/Group',1)   
					  WITH (GroupId [varchar] (255) )      
		) V_XML  
		where V_XML.GroupId COLLATE SQL_Latin1_General_CP1_CI_AS not in ( 
			select groupName from ContractGroup 
			)   
    if @@Error <> 0  
	Begin
	   rollback  
	   return 1 
	End   
  
 insert into ContractInGroup   ( contractID , groupID )  
      select Cn.ContractID, CG.groupID   
           from 
		( SELECT * FROM OPENXML(@idoc, '/Groups/Group',1)   
			   WITH (GroupId [varchar] (255), Symbol [varchar](255) )      
                ) V_XML  
    	   join ContractGroup CG on CG.groupName = V_XML.GroupId COLLATE SQL_Latin1_General_CP1_CI_AS  
	   join Contract Cn 	 on Cn.symbol = V_XML.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS  
	   where Cn.ContractID not in (
			 select distinct contractID from  ContractInGroup   
			 where groupID = CG.groupID
                         )  
		 and Cn.contractTypeID in (1,2,4)
		 and Cn.actionID<3  

  Commit  
    
  
  EXEC sp_xml_removedocument @idoc   
  return 0
go

/*usp_Contract_ClosePrice_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_ClosePrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_ClosePrice_Save]
go
create proc usp_Contract_ClosePrice_Save
	  @iContractID int,
	  @dContractClosePrice money	
  as
	if @iContractID is not null
	begin
	  update ContractPrice set priceClose = @dContractClosePrice 
		where ContractID = @iContractID
	  IF @@ROWCOUNT = 0 AND @@ERROR = 0
	  begin
		insert into ContractPrice (contractID,priceClose) values (@iContractID, @dContractClosePrice)
	  end	
	end

	return @@error
go

/*usp_CustomStrikeSkewDefaultPoint2_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewDefaultPoint2_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewDefaultPoint2_Save]
go
create      procedure usp_CustomStrikeSkewDefaultPoint2_Save
	@iContractID int,
	@iExpiryID int,
	@iType int,
	@fVolatility float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @RootType int

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	if @iVolaSurfaceDataID is not null
	begin
		exec @RootType =  usp_RootType_Get @iContractID
		-- option root
		if @RootType = 0 
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
					select distinct 
						@iVolaSurfaceDataID, round(ov.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
					from OptionView ov
						join OptionRootView ort on ov.optionRootID = ort.optionRootID
					where ov.expiryID = @iExpiryID 
						and ort.underlyingID = @iContractID
						and not exists(select 1 
										from CustomStrikeSkewPointView cssp
										where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
												round(cssp.strike, 4) = round(ov.strike, 4)
												and cssp.expiryID = ov.expiryID)
	
		end
		

		-- future option root
		if @RootType = 1 
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
				select distinct 
					@iVolaSurfaceDataID, round(fo.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
				from    dbo.Future f inner JOIN
				        dbo.FutureOption fo  on f.futureID = fo.futureID inner JOIN
				                      dbo.Expiry e ON fo.expiryID = e.expiryID
				where   f.MaturityID = @iExpiryID 
					and f.futureID = @iContractID
					and not exists(select 1 
									from CustomStrikeSkewPointView cssp
									where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
											round(cssp.strike, 4) = round(fo.strike, 4)
											and cssp.expiryID = fo.ExpiryID)

		end
	
	end
go

/*usp_CustomStrikeSkewDefaultPointFromIV_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewDefaultPointFromIV_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewDefaultPointFromIV_Save]
go
create   Procedure usp_CustomStrikeSkewDefaultPointFromIV_Save 
	@iContractID int,
	@fStrike float = Null,
	@dtExpDate datetime = Null,
	@iType int,
	@fVolatility float
AS
/*
	Created by Michael Malyshkin
	Description: this procedure save inf. into CustomStrikeSkewPoint table
	Edited by Zaur Nuraliev 07/08/2002
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	DECLARE @expInCalID AS INT 
		
	SELECT @expInCalID = eic.expiryInCalendarID
	FROM ExpiryInCalendar eic
	JOIN Contract c ON c.contractID = @iContractID
	WHERE eic.expiryID = @iExpiryID AND  eic.expCalendarID = c.expCalendarID		

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
			and expiryInCalendarID = @expInCalID
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID, expiryInCalendarID )
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID, @expInCalID)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID),
					expiryInCalendarID = isnull( @expInCalID, expiryInCalendarID )
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
Return(@error)
go

/*usp_CustomStrikeSkewPoint_Copy*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Copy]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Copy]
go
create    Procedure usp_CustomStrikeSkewPoint_Copy 
	@iVolaSurfaceDataIDSrc int,
	@iVolaSurfaceDataIDDest int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is move information inside CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint,
				expiryInCalendarID
			into #SourcePoints
			from CustomStrikeSkewPoint
			where volaSurfaceDataID=@iVolaSurfaceDataIDSrc
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		delete CustomStrikeSkewPoint where volaSurfaceDataID=@iVolaSurfaceDataIDDest
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		insert into CustomStrikeSkewPoint (volaSurfaceDataID,strike,type,volatility,expiryID,isBasePoint, expiryInCalendarID)
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint,
				expiryInCalendarID
				from #SourcePoints
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
	drop table #SourcePoints
Return(@error)
go

/*usp_CustomVolatilityPointIsExist2_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist2_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist2_Get]
go
create  Procedure usp_CustomVolatilityPointIsExist2_Get
 @vcSymbolName nvarchar(20)
as
 ----------------------------------------------------------------------
 set nocount on
 ----------------------------------------------------------------------
 
     select 
          Ort.underlyingID as iContractID,
          OV.expiryID as iExpiryID
     from OptionView OV
          inner join OptionRoot ort  on ort.optionRootID = OV.optionRootID
          inner join ContractView CV on Ort.underlyingID = CV.contractID AND CV.symbol = @vcSymbolName AND CV.contractTypeID < 3
      left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
      left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
            and OV.expiryID = CSSP.expiryID 
     where CSSP.volaSurfaceDataID is null 
         group by Ort.underlyingID, ov.expiryID
 union
     select 
          f.futureID as iContractID,
          f.maturityID as iExpiryID
     from dbo.future f
      inner join dbo.Contract c  ON f.futureID = c.contractID and c.symbol = @vcSymbolName and c.contractTypeID = 4
      left join VolaSurfaceDataView VSD on f.futureID = VSD.contractID and VSD.isDefaultSurface = 1
      left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
      and f.maturityID = CSSP.expiryID  
           where CSSP.volaSurfaceDataID is null 
       group by f.futureid, f.maturityID
go

/*usp_DefaultIVSurfaceFutureUnd_Get*/
--bought to script 189

/*usp_DefaultIVSurfaceStock_Get*/
--bought to script 189

/*usp_EodCommodityFutureRoot_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodityFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodCommodityFutureRoot_Get]
go
create  PROC usp_EodCommodityFutureRoot_Get
AS
	-- retrieve all future roots

	set nocount on

	SELECT     fr.underlyingID AS iIndexID, 
		   fr.futureRootID AS iFutureRootID, 
		   fr.futureRootSymbol AS vcFutureRootSymbol, 
	           fr.futureRootName AS vcFutureRootName
	FROM         dbo.FutureRoot fr INNER JOIN
	                      dbo.Contract c ON fr.underlyingID = c.contractID
	WHERE     (c.contractTypeID = 6)
go

/*usp_EodCommodity_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodCommodity_Get]
go
create PROC usp_EodCommodity_Get
	@iInterestID int = null
AS
	-- retrieve future

	set nocount on

	if @iInterestID is not null
		SELECT 	c.GreeksCalculationType as GreeksCalcType,
			c.IsActive as tiIsActive, 
			CV.contractTypeID as iContractTypeID,
			CV.symbol as vcSymbol, 
	                CV.contractName as vcContractName, 
			c.CommodityID as iContractID 
		FROM         dbo.Commodity C INNER JOIN
	                      dbo.ContractView CV ON C.CommodityID = CV.contractID
		WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 7
		Order by CV.Symbol

	else
	
		SELECT 	c.GreeksCalculationType as GreeksCalcType,
			c.IsActive as tiIsActive, 
			CV.contractTypeID as iContractTypeID,
			CV.symbol as vcSymbol, 
	                CV.contractName as vcContractName, 
			c.CommodityID as iContractID 
		FROM         dbo.Commodity C INNER JOIN
	                      dbo.ContractView CV ON C.CommodityID = CV.contractID
		WHERE 	CV.ContractTypeID = 7
		Order by CV.Symbol
go

/*usp_EodContractClosePrice_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodContractClosePrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodContractClosePrice_Get]
go
create Procedure usp_EodContractClosePrice_Get 
	@iContractID int = Null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	select PriceClose from ContractPrice where ContractId  = @iContractID
go

/*usp_EodDefaultIvPoint_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodDefaultIvPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodDefaultIvPoint_Save]
go
create    Procedure usp_EodDefaultIvPoint_Save 
	@iContractID int,
	@fStrike float,
	@iExpiryID int,
	@fVolatility float
AS
/*
	Created by Michael Malyshkin
	Description: this procedure save Default Iv points
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	declare @iType int
		set @iType = 2 --OTM_IVTYPE

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	-- mark olf strile skew points as deleted
	--update CustomStrikeSkewPoint set actionID = 3
	delete from CustomStrikeSkewPoint
	where 
		volaSurfaceDataID = @iVolaSurfaceDataID and
		actionID < 3 and
		datediff(d, actionDate, getDate())>0

	DECLARE @expInCalID AS INT 
	SELECT @expInCalID  = expCalendarID FROM Contract WHERE contractID = @iContractID		
		
	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
			and expiryInCalendarID = @expInCalID
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID, expiryInCalendarID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID, @expInCalID )
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
Return(@error)
go


/*usp_EodFutureOption_Import*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodFutureOption_Import]
go
create PROC usp_EodFutureOption_Import
	@iFutureID int,
	@fStrike float,
	@iExpiryID int,
	@tiIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@vcBadFutureOptionName varchar(255) output,
	@fPriceClose float = null,
	@dtExpiryDate as smalldatetime = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if not exists(select 1 from Expiry where expiryID = isnull(@iExpiryID, 0))
	begin
		raiserror ('Wrong expiry date.', 16, 1)
		set @error = -100
		goto finish
	end

	declare	@iFutureOptionID int
	declare @iFutureOptionID_bad int
	declare @iExpCalendarID int 
	
	--===================================================================			
	-- select the @iFutureOptionID
	--===================================================================			
	select @iFutureOptionID = futureOptionID
	from FutureOption (nolock)
	where futureID = @iFutureID
		and isCall = @tiIsCall
		and abs(strike - @fStrike) < 0.009
		and expiryID = @iExpiryID
				
	if not exists (select 1 from tempdb..sysobjects where id=object_id('tempdb..#FutureContractsForDelete'))
		Create table #FutureContractsForDelete(contractID int)				
	if @@error <> 0 begin set @error = -1 goto finish end
				
	if @iFutureOptionID is not null
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID
			from Contract
			where symbol=@vcSymbol
				--and contractID<>@iFutureOptionID
				and contractTypeID=5

			if @iFutureOptionID_bad is not null and @iFutureOptionID_bad != @iFutureOptionID
			begin
				select @vcBadFutureOptionName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureOptionName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureOptionID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFutureOption_Del

				if @error <> 0
				begin 
					set @error = -5
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			end

			if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Opt'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureOptionID--@iFutureID
					end
			end			

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureOptionID
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureOption set actionDate = getdate() where futureOptionID=@iFutureOptionID
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			update ContractPrice
			set priceClose = @fPriceClose
			where exchangeID is null and contractID = @iFutureOptionID
			if (@@error <> 0) begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID
			from Contract
			where symbol=@vcSymbol
				and contractTypeID=5

			if @iFutureOptionID_bad is not null
			begin	
				select @vcBadFutureOptionName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureOptionID_bad
				
				if @vcBadFutureOptionName is not null							
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureOptionName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null							
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureOptionID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFutureOption_Del

				if @error <> 0
				begin 
					set @error = -5
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			end			
				
			insert into Contract (contractTypeID, symbol, contractName)
				 values (5, @vcSymbol,@vcContractName)
			set @iFutureOptionID = @@identity
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into FutureOption (futureOptionID, futureID, isCall, expiryID, strike)
				 values (@iFutureOptionID, @iFutureID, @tiIsCall, @iExpiryID, @fStrike)
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end

			if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Opt'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureOptionID--@iFutureID
					end
			end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
			begin
				insert into ContractPrice(contractID, exchangeID, priceClose)
					values(@iFutureOptionID, null, @fPriceClose)
				if (@@error <> 0) begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

		Commit Transaction
	end 
	 
   	set @error = @iFutureOptionID
finish:
	Return(@error)
go

/*usp_EodFutureRootWOCommodity_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureRootWOCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodFutureRootWOCommodity_Get]
go
create  PROC usp_EodFutureRootWOCommodity_Get
AS
	-- retrieve all future roots

	set nocount on

	SELECT     fr.underlyingID AS iIndexID, 
		   fr.futureRootID AS iFutureRootID, 
		   fr.futureRootSymbol AS vcFutureRootSymbol, 
	           fr.futureRootName AS vcFutureRootName
	FROM         dbo.FutureRoot fr INNER JOIN
	                      dbo.Contract c ON fr.underlyingID = c.contractID
	WHERE     (c.contractTypeID < 6)
go

/*usp_EodFuturesWithPositions2_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuturesWithPositions2_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodFuturesWithPositions2_Get]
go
create  PROCEDURE usp_EodFuturesWithPositions2_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityID as iMaturityID,
		F.calcOptionType As IsAmerican,
		0 as fPosition,
		cp.priceClose as fPriceClose
	from Future F
		inner join Contract FC on FC.contractID=F.futureID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose, f.CalcOptionType
	order by FR.underlyingID
go

/*usp_EodFuture_Import*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodFuture_Import]
go
create  PROC usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output,
	@vcBadFutureName varchar(255) output,
	@fPriceClose float = null,
        @fTickSize float = null,
        @iQuotationUnitID int = null,
        @vcQuotationUnitCurrency varchar(3) = null,
	@dtExpiryDate as datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @error int		set @error = 0

	if not exists(select 1 from Expiry where expiryID = isnull(@iExpiryID, 0))
	begin
		raiserror ('Wrong matiruty date.', 16, 1)
		set @error = -100
		goto finish
	end

	declare	@iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int,
			@iFutureID_bad int
	declare @iExpCalendarID int

	if not exists (select * from tempdb..sysobjects where id=object_id('tempdb..#FutureContractsForDelete'))
		Create table #FutureContractsForDelete(contractID int)				
	if @@error <> 0 begin set @error = -1 goto finish end
	
	--===================================================================			
	-- select the @iFutureID
	--===================================================================			
	select @iFutureID = futureID, @tiIsActive = isActive
			from Future (nolock)
			where futureRootID = @iFutureRootID and 
				maturityID = @iExpiryID

	if @iFutureID is not null
	begin
		Begin Transaction
			select @iFutureID_bad = contractID
			from Contract
			where symbol=@vcSymbol
				--and contractID<>@iFutureID
				and contractTypeID=4

			if @iFutureID_bad is not null and @iFutureID_bad != @iFutureID
			begin
				select @vcBadFutureName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							Trade t on t.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					
					
				select @vcBadFutureName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							[Order] ord on ord.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

				insert into #FutureContractsForDelete (contractID)  values (@iFutureID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

				insert into #FutureContractsForDelete 
					select futureoptionID from FutureOption where futureID=@iFutureID_bad
				if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFuture_Del

				if @error <> 0
				begin 
					set @error = -6
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			end

			if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Fut'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureID
					end
			end			

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

                                        if @iLotSizeID <> 0 
           			     update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
                                        else
                                             update FutureRoot  set optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
			
			update ContractPrice
			set priceClose = @fPriceClose
			where exchangeID is null and contractID = @iFutureID
			if @@error <> 0 begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureID_bad = contractID
			from Contract
				where symbol=@vcSymbol
				and contractTypeID=4

			if @iFutureID_bad is not null
			begin
				select @vcBadFutureName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							Trade t on t.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					
					
				select @vcBadFutureName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							[Order] ord on ord.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

				insert into #FutureContractsForDelete 
					select futureoptionID from FutureOption where futureID=@iFutureID_bad
				if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFuture_Del

				if @error <> 0
				begin 
					set @error = -6
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			
			end
			
			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

			-- check underlying price profile
			if not exists(select 1 from PriceProfile where priceProfileID = @iUndPriceProfileID)
			begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfile
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfile where priceProfileID = @iOptPriceProfileID)
			begin set @error = -11 if @@TranCount = 1 Rollback Transaction goto finish end
				

			insert into Contract
				(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			set @iFutureID = @@identity
			if @@error <> 0 begin set @error = -12 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Future (futureID, futureRootID, maturityID)
				 values (@iFutureID, @iFutureRootID, @iExpiryID)
			if @@error <> 0 begin set @error = -13 if @@TranCount = 1 Rollback Transaction goto finish end

			if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Fut'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureID
					end
			end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID, exchangeID, priceClose)
						values(@iFutureID, null, @fPriceClose)
					if (@@error <> 0) begin set @error = -14 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

                                        if @iLotSizeID <> 0
        			    update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
                                        else
                                            update FutureRoot  set optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -15 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 1

		Commit Transaction
	end 
	 
   	set @error = @iFutureID
finish:
	Return(@error)
go

/*usp_EodInterestFutureRoot_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodInterestFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodInterestFutureRoot_Get]
go
create  PROC usp_EodInterestFutureRoot_Get
AS
	-- retrieve all future roots

	set nocount on

	SELECT     fr.underlyingID AS iIndexID, 
		   fr.futureRootID AS iFutureRootID, 
		   fr.futureRootSymbol AS vcFutureRootSymbol, 
	           fr.futureRootName AS vcFutureRootName
	FROM         dbo.FutureRoot fr INNER JOIN
	                      dbo.Contract c ON fr.underlyingID = c.contractID
	WHERE     (c.contractTypeID = 6)
go

/*usp_EodInterest_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodInterest_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodInterest_Get]
go
create   PROC usp_EodInterest_Get
	@iInterestID int = null
AS
	-- retrieve future

	set nocount on

	if @iInterestID is not null
		SELECT 	c.GreeksCalculationType as GreeksCalcType,
			c.IsActive as tiIsActive, 
			CV.contractTypeID as iContractTypeID,
			CV.symbol as vcSymbol, 
	                CV.contractName as vcContractName, 
			c.CommodityID as iContractID 
		FROM         dbo.Commodity C INNER JOIN
	                      dbo.ContractView CV ON C.CommodityID = CV.contractID
		WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 6
		Order by CV.Symbol

	else
	
		SELECT 	c.GreeksCalculationType as GreeksCalcType,
			c.IsActive as tiIsActive, 
			CV.contractTypeID as iContractTypeID,
			CV.symbol as vcSymbol, 
	                CV.contractName as vcContractName, 
			c.CommodityID as iContractID 
		FROM         dbo.Commodity C INNER JOIN
	                      dbo.ContractView CV ON C.CommodityID = CV.contractID
		WHERE 	CV.ContractTypeID = 6
		Order by CV.Symbol
go

/*usp_EodOptionWithPos_Get ???*/

/*usp_EodOption_Import*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodOption_Import]
go
create PROC usp_EodOption_Import
	@iRootID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iOptionID_bad int output,
	@dtExpiry smalldatetime = null
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if not exists(select top 1 1 from ExpiryView where expiryID = isnull(@iExpiryID, 0))
	begin
		raiserror ('Wrong expiry date.', 16, 1)
		set @error = -100
		goto finish
	end

	declare	@OptionID int
	declare @iExpCalendarID int

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	if @OptionID is not null
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from ContractView where symbol = @vcSymbol and contractID <> @OptionID and contractTypeID=3
			if @iOptionID_bad is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0				
	
				if @error <> -1
				begin 
					if @error = 3 set @error = -4
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end
				else set @error = 0
			end
			
				
			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@OptionID and actionID<>3
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]  set isManualyEntered = 0 where optionID=@OptionID and actionID<>3
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

			if @dtExpiry is not null
			begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iRootID and cv.contractTypeID = 1
				
					if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiry, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
			end

		Commit Transaction
	end
	else
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from ContractView where symbol = @vcSymbol and contractTypeID=3 

			if @iOptionID_bad is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0				
	
				if @error <> -1
				begin 
					if @error = 3 set @error = -4
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end
				else set @error = 0
			end


			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@vcContractName)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

			if @dtExpiry is not null
			begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iRootID and cv.contractTypeID = 1
				
					if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiry, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
			end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

		Commit Transaction
	end 
	 
   	set @error = @OptionID
finish:
	return(@error)
go

/*usp_EodUnderlying_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodUnderlying_Get]
go
create      PROCEDURE usp_EodUnderlying_Get
	@iUnderlyingID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is null
	begin
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			s.isActive as tiIsActive,
			s.calcOptionType as tiCalcOptionType,
			s.isHTB as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
			
			s.skew as fSkew,
			s.kurt as fKurt
			
		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where 
			c.contractTypeID = 2
		union
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			i.isActive as tiIsActive,
			i.calcOptionType as tiCalcOptionType,
			i.isHTB as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
			
			i.skew as fSkew,
			i.kurt as fKurt
	
		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
		union
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			null as fPriceClose,
			fu.tiActive as tiIsActive,
			fu.tiGreeksCalculationType as tiCalcOptionType,
			null as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
			
			null as fSkew,
			null as fKurt
			
		from ContractView c 
        		inner join FutureUnderlying fu on c.contractID = fu.iFutureUnderlyingID
			
		where
			c.contractTypeID = 6			
	end
	else
	begin
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			s.isActive as tiIsActive,
			s.calcOptionType as tiCalcOptionType,
			s.isHTB as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
			
			s.skew as fSkew,
			s.kurt as fKurt
			
		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 2
			and c.contractID = @iUnderlyingID
		union
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			i.isActive as tiIsActive,
			i.calcOptionType as tiCalcOptionType,
			i.isHTB as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
			
			i.skew as fSkew,
			i.kurt as fKurt
	
		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
			and c.contractID = @iUnderlyingID
		union
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			null as fPriceClose,
			fu.tiActive as tiIsActive,
			fu.tiGreeksCalculationType as tiCalcOptionType,
			null as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
			
			null as fSkew,
			null as fKurt
			
		from ContractView c 
	        	inner join FutureUnderlying fu on c.contractID = fu.iFutureUnderlyingID
			
		where
			c.contractTypeID = 6
			and c.contractID = @iUnderlyingID
	end
		
	set @error = @@error
Return(@error)
go

/*usp_ExpiryCalendar_Del*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Del]
go
create  PROCEDURE usp_ExpiryCalendar_Del 
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
go

/*usp_ExpiryCalendar_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Save]
go
create  PROCEDURE usp_ExpiryCalendar_Save 
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
go

/*usp_FixOldOrder_Del ???*/

/*usp_FixOrderAvgPrice_Calc ???*/

/*usp_FutureIC_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureIC_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureIC_Get]
go
create  PROC usp_FutureIC_Get
		@iICID int = null
	AS
		-- retrieve future
		set nocount on
		if @iICID is not null
		SELECT     dbo.Future.futureID, dbo.Commodity.CommodityID
		FROM         dbo.FutureRoot INNER JOIN
		                      dbo.Future ON dbo.FutureRoot.futureRootID = dbo.Future.futureRootID INNER JOIN
		                      dbo.Contract ON dbo.FutureRoot.underlyingID = dbo.Contract.contractID INNER JOIN
		                      dbo.Commodity ON dbo.Contract.contractID = dbo.Commodity.CommodityID
		WHERE     (dbo.Future.futureID = @iICID)
go

/*usp_FutureUnderlyingType_Del*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureUnderlyingType_Del]
go
create  PROCEDURE usp_FutureUnderlyingType_Del
	@iCptID int,
	@tiIsRaiseError tinyint = 1 
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if exists(select 1 from FutureUnderlyingType where iFutureUnderlyingTypeID = @iCptID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove future underlying type, because it''s used in root defenitions!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	Begin Transaction TntCounterParty

	delete FutureUnderlyingType where iFutureUnderlyingTypeID = @iCptID
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

	Commit Transaction TntCounterParty

finish:
	return @error
go

/*usp_FutureUnderlyingType_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureUnderlyingType_Get]
go
create  PROCEDURE usp_FutureUnderlyingType_Get
	@iFutureUnderlyingTypeID int = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iFutureUnderlyingTypeID is null
		select
			cpt.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			FutureUnderlyingType cpt
	else
		select
			cpt.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			FutureUnderlyingType cpt
		where
			cpt.iFutureUnderlyingTypeID = @iFutureUnderlyingTypeID

return @@error
go

/*usp_FutureUnderlyingType_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureUnderlyingType_Save]
go
create  PROCEDURE usp_FutureUnderlyingType_Save
	@iCptID int = null,
	@vcName varchar(255),
	@vcDesc varchar(255) = null,
	@tiRaiseError tinyint = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iCptID is null and isnull(@vcName, '') = ''
	begin
		raiserror ('Name for new future underlying type can not be empty!', 16, 1)
		set @error = -1
		goto finish
	end

	if @iCptID is null
	begin
		select
			@iCptID = cpt.iFutureUnderlyingTypeID
		from
			FutureUnderlyingType cpt
		where
			cpt.vcName = @vcName

		if @iCptID is not null and isnull(@tiRaiseError, 0) <> 0 
		begin
			raiserror ('Future underlying type with the same name is already exists!', 16, 1)
			set @error = -2
			goto finish
		end
	end

	Begin Transaction TntCounterParty

	if @iCptID is null
	begin
		insert into FutureUnderlyingType
			(vcName, vcDesc)
		values
			(@vcName, @vcDesc)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
		set @iCptID = @@identity
	end
	else
	begin
		update FutureUnderlyingType set 
			vcName = isnull(@vcName, vcName),
			vcDesc = isnull(@vcDesc, vcDesc)
		where
			iFutureUnderlyingTypeID = @iCptID

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
	end

	Commit Transaction TntCounterParty

finish:
	if @error = 0
		return @iCptID
	else
		return @error
go

/*usp_GetClosePriceContr*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GetClosePriceContr]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GetClosePriceContr]
go
create procedure usp_GetClosePriceContr
as
set nocount on
select fr.futureRootSymbol, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName,  null as  isCall, null as strike, cp.PriceClose, 'fut' as typeContr 
from ContractPrice cp
inner join Future f on f.futureID = cp.ContractID
inner join FutureRoot fr on fr.futureRootID = f.futureRootID
inner join Expiry e on e.expiryID = f.MaturityID
where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)


union all


select Case when isnull(fu.iFutureUnderlyingTypeID,0) > 0 then dbo.fn_OptS2FutS(c.Symbol) else  fr.futureRootSymbol end as futureRootSymbol,  
e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName, fo.isCall, fo.strike, cp.PriceClose, 'opt' as typeContr
from ContractPrice cp
inner join Contract C on C.ContractID = cp.ContractID  
inner join FutureOption fo on fo.futureOptionID = cp.ContractID
inner join Future f on f.futureID = fo.futureID
inner join FutureRoot fr on fr.futureRootID = f.futureRootID
inner join ContractView fc on f.FutureID = fc.contractID
inner join Expiry e on e.expiryID = fo.expiryID
Left join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.UnderlyingID
where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)
go

/*usp_GetClosePriceVolContr*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GetClosePriceVolContr]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GetClosePriceVolContr]
go
create Procedure usp_GetClosePriceVolContr

as

delete TmpClosePriceVolETS


insert into TmpClosePriceVolETS
select fr.futureRootSymbol as Symbol , fr.futureRootSymbol, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName,  null as  isCall, null as strike, cp.PriceClose, null as Volatility, 'fut' as typeContr 
from ContractPrice cp
inner join Future f on f.futureID = cp.ContractID
inner join FutureRoot fr on fr.futureRootID = f.futureRootID
inner join Expiry e on e.expiryID = f.MaturityID
where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)

union all

select distinct co.Symbol, Case when isnull(fu.iFutureUnderlyingTypeID,0) > 0 then dbo.fn_OptS2FutS(co.Symbol) else  fr.futureRootSymbol end as futureRootSymbol
, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName, fo.isCall, fo.strike, cp.PriceClose, VC.Volatility, 'opt' as typeContr 
from ContractPrice cp
inner join Contract co on co.ContractID = cp.ContractID
inner join FutureOption fo on fo.futureOptionID = cp.ContractID
inner join Future f on f.futureID = fo.futureID
inner join FutureRoot fr on fr.futureRootID = f.futureRootID
inner join Expiry e on e.expiryID = fo.expiryID
Left join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.UnderlyingID
Left join Contract c on c.ContractID = fu.iFutureUnderlyingID
Left join VolaSurfaceData vsd on vsd.ContractID = c.ContractID
Left join CustomStrikeSkewPoint VC on VC.volaSurfaceDataID = vsd.VolaSurfaceDataID and fo.strike = VC.strike and VC.expiryID = e.expiryID
where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)
go

/*usp_GetClosePriceVolFocus*/
--brought to script 189

/*usp_GetVolaContr*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GetVolaContr]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GetVolaContr]
go
create procedure usp_GetVolaContr
as
select distinct co.Symbol, Case when isnull(fu.iFutureUnderlyingTypeID,0) > 0 then dbo.fn_OptS2FutS(co.Symbol) else  fr.futureRootSymbol end as futureRootSymbol
, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName, fo.isCall, fo.strike, cp.PriceClose, VC.Volatility  
from ContractPrice cp
inner join Contract co on co.ContractID = cp.ContractID
inner join FutureOption fo on fo.futureOptionID = cp.ContractID
inner join Future f on f.futureID = fo.futureID
inner join FutureRoot fr on fr.futureRootID = f.futureRootID
inner join Expiry e on e.expiryID = fo.expiryID
Left join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.UnderlyingID
Left join Contract c on c.ContractID = fu.iFutureUnderlyingID
Left join VolaSurfaceData vsd on vsd.ContractID = c.ContractID
Left join CustomStrikeSkewPoint VC on VC.volaSurfaceDataID = vsd.VolaSurfaceDataID and fo.strike = VC.strike
where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)
order by e.ExpiryPeriod, fr.futureRootSymbol, fo.strike, fo.isCall
go

/*usp_ICFutures_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ICFutures_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ICFutures_Get]
go
create      PROC usp_ICFutures_Get
	@iICID int = null
AS
	-- retrieve future

	set nocount on

	if @iICID is not null
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.iFutureUnderlyingID  AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID,
		CP.priceClose		AS priceClose
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID And Fr.underlyingID =  @iICID
	inner join FutureUnderlying  CM on Cm.iFutureUnderlyingID  =  @iICID	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	left join ContractPrice CP on CP.ContractID = CF.ContractId
	ORDER BY FutureName
	else
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.iFutureUnderlyingID  AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID,
		CP.priceClose		AS priceClose
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID 
	inner join FutureUnderlying  CM on  CM.iFutureUnderlyingID  is not null	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	left join ContractPrice CP on CP.ContractID = CF.ContractId
	ORDER BY FutureName
go

/*usp_IC_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IC_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IC_Save]
go
create   PROCEDURE usp_IC_Save
      @iICID int,
      @vcSymbol varchar(20) = null,
      @vcName varchar(255) = null,
      @iICType int,   
      @tiGreeksCalcType tinyint = null,
      @tiIsActive tinyint = null,
      @iFutureUnderlyingTypeID int,
      @tiCheckExistance tinyint = null,
      @iExpCalendarID int = null,
      @iUndPriceProfileID int = null output,
      @iOptPriceProfileID int = null output
  
     as
    ---------------
    set nocount on
    ---------------
  --  if ((@iICType <> 6) And (@iICType <> 7 ))
  --    begin
  --      Raiserror ('Wrong Interest/FutureUnderlying type value', 16, 1)
  --      Return (-1)
  --    end
  
    if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID = @iICType and actionID < 3 and (@iICID <> contractID or @iICID is null))
      begin
        if isnull(@tiCheckExistance, 1) <> 0
          Raiserror ('Interest or FutureUnderlying with this ticker already exists!', 16, 1)
        Return (-1)
      end   
  
     if (@iICID is Null)  
     begin
       
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

      if @iICType = 6
      begin		
          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          --set @iExpCalendarID = scope_identity()

               insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName, null, @iUndPriceProfileID, @iOptPriceProfileID)
      end
      else
      begin
          insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          set @iExpCalendarID = scope_identity()  

               insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)	
      end
  
         select @iICID = @@identity      
           if (@@error <> 0)
              begin
              RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
              ROLLBACK TRAN
              RETURN (-1)
              end
         insert into [FutureUnderlying] (
                iFutureUnderlyingID, 
                tiActive,
                tiGreeksCalculationType,
                iFutureUnderlyingTypeID)
             values (
                @iICID, 
                @tiIsActive,
                @tiGreeksCalcType,
                @iFutureUnderlyingTypeID)
           if (@@error <> 0)
            begin
               RAISERROR ('Error. Can''t insert into [FutureUnderlying]!', 16, 1)
               ROLLBACK TRAN
               RETURN (-1)
            end       
       commit tran
     end
     else
     begin
      begin tran
         update Contract
          set symbol = isNull(@vcSymbol,symbol),
            contractName = isNull(@vcName,contractName),
            expCalendarID = isNull(@iExpCalendarID, expCalendarID),
            undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
            optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
          where contractID = @iICID
        if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update Contract!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end
        update [FutureUnderlying]
           set  tiGreeksCalculationType = isnull(@tiGreeksCalcType, tiGreeksCalculationType),
            tiActive = isnull(@tiIsActive, tiActive),
            iFutureUnderlyingTypeID = isnull(@iFutureUnderlyingTypeID, iFutureUnderlyingTypeID)
           where iFutureUnderlyingID = @iICID
         if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update [FutureUnderlying]!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end       
       commit tran
     end
  
    --========================================================================
    -- make record with null exchange for this index
    --========================================================================
  --  if not exists(select * from ContractPrice where exchangeID is null and contractID = @iICID)
  --    begin
  --      insert into ContractPrice(contractID,exchangeID)
  --        values(@iICID,null)
  --      if (@@error <> 0) begin rollback tran return(-1) end              
  --    end   
  
  if (@@error <> 0)
    return (-1)
  else  
    return @iICID
go

/*usp_IndexUnderlyingBeta_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexUnderlyingBeta_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexUnderlyingBeta_Save]
go
create     Procedure usp_IndexUnderlyingBeta_Save 
	@iIndexID int = null, --the index identifier
	@iUnderlyingID int = null, --the underlying identifier
	@fBeta float,
	@vcIndexSymbol varchar(8) = null,
	@vcUnderlyingSymbol varchar(8) = null
as
/*
	Created by Mike Malyshkin
	Description: this procedure save data into IndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iUnderlyingID is null and @vcIndexSymbol is null and @vcUnderlyingSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Underlying and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcUnderlyingSymbol is not null and @iUnderlyingID is null 
		select	@iUnderlyingID = contractID from Contract where symbol = @vcUnderlyingSymbol and contractTypeID in (1, 2, 4) and actionID < 3
		if @iUnderlyingID is null 
			begin
				Raiserror ('Impossible define identifier of the Underlying!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
		if not exists(select * From IndexBetaView where indexID = @iIndexID and contractID = @iUnderlyingID) 
			begin
				insert into IndexBeta (indexID,contractID,beta) 
				values (@iIndexID,@iUnderlyingID,@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexBeta
					set beta = isnull(@fBeta, beta)
					where indexID = @iIndexID and contractID = @iUnderlyingID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end

	Commit Transaction
    set @error = 0

finish:
Return(@error)
go

/*usp_Index_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
go
create  PROCEDURE usp_Index_Save
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
		@SOQ as integer = 0,
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
							kurt,
							SOQ)
					 values (
							@iIndexID, 
							@fYield,
							isNull(@bIsHTB,0),
							@tiCalcOptionType,
							@tiCalcModelType,
							isnull(@tiIsBasket,0), 
							@tiIsActive,
							@fSkew,
							@fKurt,
							@SOQ)
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
					kurt = isnull(@fKurt,Kurt),
					SOQ = isnull(@SOQ,SOQ)
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
go

/*usp_MmFutureByIndex_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureByIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFutureByIndex_Get]
go
create PROC usp_MmFutureByIndex_Get
	@iIndexID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		fc.contractID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
                fec.expiryDate as dtMaturity,
		iv.indexID as indexID,
                f.futureRootID as iFutureRootID
	from IndexView iv
                inner join FutureRoot fr on fr.underlyingID = iv.indexID
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.FutureID = fc.contractID				
                inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
	where iv.indexID = @iIndexID
		and fec.expiryDate >= @dtMinExpiry
				
	return @@error
go

/*usp_MmFutureByUnderlying_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFutureByUnderlying_Get]
go
create   PROC usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0	

	select
		f.futureID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
--                e.ExpiryPeriod as vcMaturity,
                fec.expiryDate as dtMaturity,
		fu.ContractId as iFutureUnderlyingID,
                f.futureRootID as iFutureRootID,
		isnull(ff.tiGreeksCalculationType, 1) as tiGreeksCalculationType
		
	from ContractView fu
                inner join FutureRoot fr on fr.underlyingID = fu.ContractId
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.futureID = fc.ContractId
--                inner join Expiry e on e.expiryID = f.MaturityID
                inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		left join FutureUnderlying ff on ff.iFutureUnderlyingID = fu.ContractId
	where fu.ContractId = @iUnderlyingID
		and fec.expiryDate >= @dtMinExpiry		

	return @@error
go

/*usp_MmFutureExchangesByUnderlying_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureExchangesByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFutureExchangesByUnderlying_Get]
go
create PROC usp_MmFutureExchangesByUnderlying_Get
	@iUnderlyingID int
as
	set nocount on

	select e.exchangeID   as iExchangeID,
	       e.isUnderlying as tiIsUnderlying,
	       e.isOption     as tiIsOption,
	       f.FutureID   as iFutureID	
	from future f
	inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
	inner join Contract u on u.contractId = fr.underlyingID 
	inner join ContractExchangeLinkView e on f.FutureId = e.ContractID
 
	return @@error
go

/*usp_MmFutureOptionsByUnderlying_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureOptionsByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFutureOptionsByUnderlying_Get]
go
create PROC usp_MmFutureOptionsByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null 
		begin
			select
				foc.contractID as iContractID,
				foc.contractTypeID as iContractTypeID,
				foc.symbol as vcSymbol,
				fo.strike as fStrike, 
				fec.expiryDate as dtExpiry,
				fo.isCall as tiIsCall,
				f.futureID as iFutureID
			from Future f
				inner join ContractView fc on f.futureID = fc.contractID
				inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
				inner join FutureOption fo on f.futureID = fo.futureID
				inner join ContractView foc on fo.futureOptionID = foc.contractID
				inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID
		end
	else
		begin
			select
				foc.contractID as iContractID,
				foc.contractTypeID as iContractTypeID,
				foc.symbol as vcSymbol,
				fo.strike as fStrike, 
				fec.expiryDate as dtExpiry,
				fo.isCall as tiIsCall,
				f.futureID as iFutureID
			from Future f
				inner join ContractView fc on f.futureID = fc.contractID
				inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
				inner join FutureOption fo on f.futureID = fo.futureID
				inner join ContractView foc on fo.futureOptionID = foc.contractID
				inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID
			where  fec.expiryDate >= @dtMinExpiry
		end 

	return @@error
go

/*usp_MmFutureRoot_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFutureRoot_Get]
go
create  PROC usp_MmFutureRoot_Get
	@iUnderlyingID int = null
AS
	-- retrieve future root(s) by underlying
	set nocount on

	if @iUnderlyingID is null
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.iQuotationUnitID as iQuotationUnitID,
			fr.vcQuotationUnitCurrency as vcQuotationUnitName,
			fr.fTickSize		 as fTickSize,
			fr.fTickValue		 as fTickValue,
			fr.tiQuoteFormat 	 as tiQuoteFormat,
			fr.iQuotationUnitAmmount as iQuotationUnitAmmount
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
	else
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.iQuotationUnitID as iQuotationUnitID,
			fr.vcQuotationUnitCurrency as vcQuotationUnitName,
			fr.fTickSize		 as fTickSize,
			fr.fTickValue		 as fTickValue,
			fr.tiQuoteFormat 	 as tiQuoteFormat,
			fr.iQuotationUnitAmmount as iQuotationUnitAmmount
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			fr.underlyingID = @iUnderlyingID
go

/*usp_MmFuture_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFuture_Get]
go
create   PROC usp_MmFuture_Get
	@iFutureRootID int = null
AS
	-- retrieve futures by future root
	set nocount on

	if @iFutureRootID is null
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
		    fcp.priceClose as fPriceClose,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
,			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
			inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fc.expCalendarID --fr.matCalendarID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
	else
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
		    fcp.priceClose as fPriceClose,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
,			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
			inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fc.expCalendarID --fr.matCalendarID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
		where
			f.futureRootID = @iFutureRootID
go

/*usp_MmTradeByTrader_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmTradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmTradeByTrader_Get]
go
create   PROC usp_MmTradeByTrader_Get
	@iTraderID int = null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and 
				fec.expCalendarID = /*case when uc.contracttypeid = 6 then c.expCalendarID else*/ fr.matCalendarID /*end*/
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and 
				fec.expCalendarID = /*case when uc.contracttypeid = 6 then c.expCalendarID else*/ fr.matCalendarID /*end*/
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error
go

/*usp_MmTrades_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmTrades_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmTrades_Get]
go
create    PROC usp_MmTrades_Get
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			te.acronym as traderAcronym,
			te.traderName as traderName,
			tr.strategyID as iStrategyID, 
			st.StrategyName as strategyName,
			tr.brokerID as iBrokerID,
			br.BrokerName as brokerName,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			brc.BrokerName as clearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			Case when c.contractTypeID in (4,5) then fr.futureRootID else uc.contractID end as iUnderlyingContractID,
	                Case when c.contractTypeID in (4,5) then fr.futureRootSymbol else uc.Symbol end as underlyingContractSymbol,
	                ort.Symbol as optionRootSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
	                fr.futureRootSymbol as futureRoot,
        	        fu.Symbol as Future,
			tr.actionID as ActionID,
			isnull(o.strike, fo.strike)            as OptionStrike,
			isnull(ec.expiryDate, fec.expiryDate)  as OptionExpityDate	
			
		from Trade tr 
			inner join ( 
				select tradeID, max(seqNum) as seqNum from Trade group by tradeID
			 ) tr2 on tr.seqNum = tr2.seqNum

			-- contract and underlying contract
			inner join ContractView  c  on tr.contractID   = c.contractID
			inner join ContractView  uc on tr.underlyingID = uc.contractID
			left  join ContractPrice cp on tr.contractID   = cp.contractID and cp.exchangeID is null
			left  join ContractView  fu on fu.ContractID   = tr.FutureID
			inner join Trader        te on te.TraderID    = tr.traderID
			left  join Strategy  	 st  on st.StrategyID = tr.StrategyID
			left  join Broker     	 br  on br.brokerID = tr.brokerID 
			left  join Broker        brc on br.brokerID = tr.ClearingBrokerID

			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = c.expCalendarID

		order by TradeDate
go

/*usp_MmUnderlying_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmUnderlying_Get]
go
create    PROC usp_MmUnderlying_Get
	@iTraderID int = null
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null
	else
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			tc.traderID as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null

	return @@error
go

/*usp_Option_Import*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
go
create   PROCEDURE usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iLotSize int,
	@iMsCacheOptionID int = NULL,
	@vcSymbolUnderlying varchar(20) = NULL,
	@fPriceClose float = NULL
as

/*

	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int,
			@vcUnderlyingSymbol varchar(8),
			@ERR_MSG varchar(8000)
	declare @iExpCalendarID int
	declare @dtIndexCalendarExpity datetime

	set @dtIndexCalendarExpity = @dtExpiry
	if @vcSymbolUnderlying is null
      		select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	else
		set  @vcUnderlyingSymbol = @vcSymbolUnderlying

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			set @ERR_MSG = 'Incorrect expiration! '  + '''' +  Convert(varchar(50),@dtExpiry) + '''' + ' for symbol=' + '''' + @vcSymbol + '''' +  ' UnderlyingSymbol=' + '''' + @vcUnderlyingSymbol + '''' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	--===================================================================			
	-- check symbol of option root
	--===================================================================			  
   DECLARE @vcOptionRootSymbol varchar(5)	
   set 	@vcOptionRootSymbol = left(@vcSymbol,len(@vcSymbol)-2)

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = @vcOptionRootSymbol and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = @vcOptionRootSymbol and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + @vcOptionRootSymbol + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
			Raiserror(@ERR_MSG,16,1)
			RETURN (-1)
		end  


	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptionRootSymbol
			and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry,@iExpiryID = @iExpiryID,@fStrike = @fStrike,@iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@bRemoveOption = 1,@ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)


  	if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID, @vcOptionRootSymbol, isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			if @dtExpiry is not null
			begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iUnderlyingContractID and cv.contractTypeID = 1
				
					if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtIndexCalendarExpity, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
			end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			DECLARE @error_var int, @rowcount_var int
			UPDATE ContractPrice 
				SET priceClose = isnull(@fPriceClose,priceClose)
			WHERE exchangeID is null and contractID = @OptionID

			SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
			if @error_var = 0
			begin
			  if @rowcount_var = 0
			  begin
				insert into ContractPrice(contractID,exchangeID,priceClose)
					values(@OptionID, null, @fPriceClose)
				if (@@error <> 0) begin rollback tran return(-1) end           
			  end	
			end
                        else
			 if (@@error <> 0) begin rollback tran return(-1) end           



			if @iMsCacheOptionID is null
			begin
				delete from MsOptionCache
				where (
					symbol = @vcSymbol 
					and underlyingID = @iUnderlyingContractID 
					and isCall = @iIsCall 
					and abs(strike - @fStrike) < 0.009
					and expiryDate = @dtExpiry )
			end
			else
			begin
				delete from MsOptionCache
				where optionID = @iMsCacheOptionID
			end

		Commit Transaction
	end 
 	else
	begin
		update Contract
			set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 

		if @iMsCacheOptionID is null
		begin
			delete from MsOptionCache
			where (
				symbol = @vcSymbol 
				and underlyingID = @iUnderlyingContractID 
				and isCall = @iIsCall 
				and abs(strike - @fStrike) < 0.009
				and expiryDate = @dtExpiry )
		end
		else
		begin
			delete from MsOptionCache
			where optionID = @iMsCacheOptionID
		end

		if @iOptionRootID is not null
		begin
		  update OptionRoot
				set
					lotSize = isnull(@iLotSize,lotSize)
				where optionRootID = @iOptionRootID
		end

		UPDATE ContractPrice 
			SET priceClose = isnull(@fPriceClose,priceClose)
		WHERE exchangeID is null and contractID = @OptionID

		update [Option]
			set 
			IsCall = isnull(@iIsCall,IsCall),
			strike = isnull(@fStrike,Strike),
			expiryID = isnull(@iExpiryID,expiryID),
			isManualyEntered = 0
		where optionID = @OptionID

		if @dtExpiry is not null
		begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iUnderlyingContractID and cv.contractTypeID = 1
				
				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtIndexCalendarExpity, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
		end

	   if (@@error <> 0)
		   begin
			  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
			  RETURN (-1)
		   end 
	end 
	 
   set @error = 0
finish:
Return(@error)
go

/*usp_QuotationUnit_Del*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Del]
go
create  PROCEDURE usp_QuotationUnit_Del
	@iCptID int,
	@tiIsRaiseError tinyint = 1 
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if exists(select 1 from QuotationUnit where iQuotationUnitID = @iCptID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove Quotation unit, because it''s used in root defenitions!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	Begin Transaction TntCounterParty

	delete QuotationUnit where iQuotationUnitID = @iCptID
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

	Commit Transaction TntCounterParty

finish:
	return @error
go

/*usp_QuotationUnit_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Get]
go
create  PROCEDURE usp_QuotationUnit_Get
	@iQuotationUnitID int = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iQuotationUnitID is null
		select
			cpt.iQuotationUnitID as iQuotationUnitID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			QuotationUnit cpt
	else
		select
			cpt.iQuotationUnitID as iQuotationUnitID,
			cpt.vcName as vcName,
			cpt.vcDesc as vcDesc
		from
			QuotationUnit cpt
		where
			cpt.iQuotationUnitID = @iQuotationUnitID

return @@error
go

/*usp_QuotationUnit_Save*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Save]
go
create  PROCEDURE usp_QuotationUnit_Save
	@iCptID int = null,
	@vcName varchar(255),
	@vcDesc varchar(255) = null,
	@tiRaiseError tinyint = null
AS
/*
	Created by Pavel Ananyev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iCptID is null and isnull(@vcName, '') = ''
	begin
		raiserror ('Name for new future underlying type can not be empty!', 16, 1)
		set @error = -1
		goto finish
	end

	if @iCptID is null
	begin
		select
			@iCptID = cpt.iQuotationUnitID
		from
			QuotationUnit cpt
		where
			cpt.vcName = @vcName

		if @iCptID is not null and isnull(@tiRaiseError, 0) <> 0 
		begin
			raiserror ('Quotation unit with the same name is already exists!', 16, 1)
			set @error = -2
			goto finish
		end
	end

	Begin Transaction TntCounterParty

	if @iCptID is null
	begin
		insert into QuotationUnit
			(vcName, vcDesc)
		values
			(@vcName, @vcDesc)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
		set @iCptID = @@identity
	end
	else
	begin
		update QuotationUnit set 
			vcName = isnull(@vcName, vcName),
			vcDesc = isnull(@vcDesc, vcDesc)
		where
			iQuotationUnitID = @iCptID

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
	end

	Commit Transaction TntCounterParty

finish:
	if @error = 0
		return @iCptID
	else
		return @error
go

/*usp_RootType_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RootType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RootType_Get]
go
create   Procedure usp_RootType_Get 
		@ContractID int
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @RootType int		set @RootType = -1
	declare @RootID int		set @RootID = -1
	 
	SELECT  @RootID = fr.FutureRootId	
	FROM  dbo.Future f INNER JOIN dbo.FutureRoot fr ON f.futureRootID = fr.futureRootID
	WHERE     (f.futureID = @ContractID)

	if @RootID = -1
	  begin
		select  @RootID = OptionRootId  from OptionRoot where UnderlyingID = @ContractID
		if @RootID <> -1
		  begin
			Set @RootType = 0
		  end
		else
		  begin
			declare @ContractType int		set @ContractType = -1
				 
			select  @ContractType = c.contracttypeid
			from  dbo.Contract c 
			where     (c.contractid = @ContractID)
			if @ContractType = 4 
			  begin
				Set @RootType = 1
			  end

		  end
		
	  end
	else 	
	  begin
		
			Set @RootType = 1
	  end 

select @RootType as RootType
return (@RootType)
go

/*usp_Stock_Import*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Import]
go
create   proc usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@vcExchangeCode varchar(20) = null,
	@fPriceAsk float = -1, -- fPriceAsk < 0 ????????, ??? ????????? ????????? ????????????
	@fPriceBid float = 0,
	@fPriceOpen float = 0,
	@fPriceClose float = 0,
	@fPriceLow float = 0,
	@fPriceHigh float = 0,
	@iVolume int = 0,
	@iExchangeID int = null  --the ExchangeID special for save data into ContractPrice table
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update Contract
		    set ContractName = @vcContractName,
				MSupdateDate = getDate()
	 	where contractID = @iContractID
	
		delete from TradeLog
		where underlyingID = @iContractID

		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		if @fPriceAsk >= 0
			begin
			 if @iExchangeID is null
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID 
			    end
                            else
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID and exchangeID=@iExchangeID
			    end

				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update Contract Price!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iContractID,null)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 

		declare @exchangeID as int
		set @exchangeID = null
		if (@vcExchangeCode is not null)
		begin
			set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
		end		

		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from stock where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq,
						primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate and primaryExchangeID!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID
go

/*usp_TradeFeed_1_New*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_1_New]
go
create     PROC usp_TradeFeed_1_New
	@vcTraderAcronym varchar(12),
	@vcExecID nvarchar(128),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(20),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(20) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null, 
	@vcOptRootSymbol varchar(20) = null,
	@iMark int = null,
	@vcStrategyName varchar(12) = null,
	@fStockRefPrice float = null,
	@vcOrderID varchar(50) = null,
	@iExecStatus int = null,
	@vcBrokerName nvarchar(64) = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iMinSeqNum int 
	declare @iSeqNum int 

	-- find existing trade
	select @iSeqNum = max(seqNum)
	from Trade 
	where execID = @vcExecID and 
		Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)
	group by
	 	execID, Convert(varchar(8),tradeDate,112), tradeID
	having
		min(actionID) = 1

	Begin Transaction

	if @iSeqNum is not null
	begin
		if @tiIsUpdateEnabled = 1
		begin
			-- delete existing trade
			--select @iMinSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where seqNum = @iSeqNum
			if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end
			
			set @iMinSeqNum = @@identity
		end
		else
		begin
			--trade already exists
			select -1 as iMaxSeqNum, @iSeqNum as iMinSeqNum, -1 as iStructureMissed
			Rollback Transaction
			Return(-1)
		end 
	end

	Declare @iTradeContractID int, 
			@iUnderlyingContractID int,
			@iStructureMissed int

	set @iStructureMissed = 0

	--check underlying symbol
	if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
		set @vcUnderlyingSymbol = @vcContractSymbol

	if @iContractTypeID = 3 and @vcUnderlyingSymbol is null
        begin
		select @vcUnderlyingSymbol = C.Symbol from Contract C
			inner join OptionRoot O on O.symbol = @vcOptRootSymbol and O.underlyingID = C.ContractID 
		where C.contractTypeID < 3
		if @vcUnderlyingSymbol is NULL
		begin
			select @vcUnderlyingSymbol = C.Symbol from ETS_ASP_EOD.dbo.Contract C
				inner join ETS_ASP_EOD.dbo.OptionRoot O on O.symbol = @vcOptRootSymbol and O.underlyingID = C.ContractID 
			where C.contractTypeID < 3
			if @vcUnderlyingSymbol is NULL
			begin
				-- unknown underlying
				select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
				Rollback Transaction
				Return(-1)			
			end	
		end
        end
       
	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3

	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			set @iStructureMissed = 1
		end
	else
		begin  --check this instruction carefully!
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int,
					@iOptionRootID int

			Select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptRootSymbol
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where optionRootID = @iOptionRootID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID < 3
	
			if @iOptionID is null
			--create new option
				begin
					if isnull(@vcContractSymbol, '') <> ''
					begin
						Declare @vcContractName varchar(255)
						  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
										' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
											 ' ' + cast(round(@fStrike, 2) as varchar)
		
						insert into Contract (contractTypeID, symbol, contractName) values (3, @vcContractSymbol,@vcContractName)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
							set @iOptionID = @@identity
						
						if @iOptionRootID is null
							begin
								insert into OptionRoot(underlyingID,symbol,lotSize) values(@iUnderlyingContractID,@vcOptRootSymbol, isnull(@iTradeLotSize, 100))
									set @iOptionRootID = @@identity
							end

						insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
							 values (@iOptionID, @iOptionRootID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end

					set @iStructureMissed = @iStructureMissed | 2
				end

			set @iTradeContractID = @iOptionID
		end
		else
			set @iTradeContractID = @iUnderlyingContractID


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym, @vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	Declare @iBrokerID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym
		if @vcBrokerName is not null
		begin
			select @iBrokerId = traderId from TradetView where acronym = @vcBrokerName
			if @iBrokerId is NULL
			begin
			insert into Trader(acronym,traderName) values(@vcBrokerName, @vcBrokerName)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iBrokerId = @@identity
			end
		end
		else
			set @iBrokerId = null
			
	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	
	declare @iStrategyID int
	set @vcStrategyName = ltrim(rtrim(isnull(@vcStrategyName, '')))

	if len(@vcStrategyName) > 0
	begin
		select @iStrategyID = strategyID from StrategyView where @vcStrategyName = strategyName
		if @iStrategyID is null
		begin
			insert into Strategy (strategyName) values (@vcStrategyName)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iStrategyID = @@identity
		end
	end

	declare @iNewTradeID int
	--select @iNewTradeID = isnull(max(tradeID), 0) + 1 from Trade
	insert TradeIDs default values
	set @iNewTradeID = @@identity
	
	--select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade
	

	--check if was an execution
	declare @iOrderID int
	if not (@vcOrderID is null)
	begin
		select @iOrderID = orderID from [Order] where @vcOrderID = clOrderID
		if @iOrderID is NULL
		begin
			-- unknown Client OrderID
			select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
			Rollback Transaction
			Return(-1)			
		end
	end
	else
		set @iOrderID = null
		
	--create new trade	
	insert into Trade (execID, tradeID, actionID, contractID, tradeDate, 
		quantity, price, traderID, strategyID, isBuy, isPosition, status, mark,
		underlyingID, optionRootID, futureRootID, futureID, spotReference, orderID, execStatus)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy, 0, 0, @iMark,
		@iUnderlyingContractID, @iOptionRootID, null, null, @fStockRefPrice, @iOrderID, @iExecStatus)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	set @iSeqNum = @@identity

	--calculate order AvgPrice
	if not (@iOrderID is null)
	begin
		declare @fAvgPrice float
		declare @iCumQty int		set @iCumQty = null
		
		exec usp_FixOrderAvgPrice_Calc @iOrderID, @fAvgPrice output, @iCumQty output
	
		update [Order]
			set 
				status = @iExecStatus,
				avgPrice = isnull(@fAvgPrice, avgPrice), 
				cumQty = isnull(@iCumQty, cumQty) 
			where orderID = @iOrderID
	end

	--write into TradeLog 
	if @iStructureMissed <> 0
		begin
			if exists(select 1 from TradeLog where underlyingID = @iUnderlyingContractID)
			begin
				update TradeLog set eventTypeID = isnull(eventTypeID, 0) | @iStructureMissed
				where underlyingID = @iUnderlyingContractID
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
			else
			begin
				insert into TradeLog (underlyingID, eventTypeID) values (@iUnderlyingContractID, @iStructureMissed)
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
		end

	--save the last successful trade time
	if @iLastTradeTime is not null
		begin
			update DataInfo set keyValue = @iLastTradeTime where keyName='lastTradeTime'
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--return output recordset
	select isnull(@iMinSeqNum,@iSeqNum) as iMinSeqNum,@iSeqNum as iMaxSeqNum, @iStructureMissed as iStructureMissed

	Commit Transaction

    set @error = 0
	return @error

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
	return @@error
go

GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.187')
	begin
		update DataInfo set keyValue = '3.46.187' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.187' '3.46.187 update message:' 
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