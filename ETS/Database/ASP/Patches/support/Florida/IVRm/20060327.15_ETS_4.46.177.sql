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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.176'  or (keyValue = '3.46.177') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.177 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.177' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
----------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuture_Import]
GO


CREATE PROC dbo.usp_EodFuture_Import
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
	@dtExpiryDate as smalldatetime = null 
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
GO 
--------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFutureOption_Import]
GO


CREATE PROC dbo.usp_EodFutureOption_Import
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
GO
---------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOption_Import]
GO

CREATE PROC dbo.usp_EodOption_Import
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
GO
---------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlying_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlying_Import]
GO


CREATE PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime
as
/*
	Created by Michael Malyshkin
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

		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
	
		delete from TradeLog
		where underlyingID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t delete record from TradeLog!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iContractID,null)
				if @@error <> 0 
					begin 
						Raiserror ('Error. Can''t insert default exchange!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish
					end
			end 
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from StockView where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update Dividends!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	Return(@error)

GO 

----------------------------------------------------------------
-- Add column to the FutureRoot table

if not exists(select 1 from syscolumns 
  where id=object_ID('FutureRoot') AND Name='iQuotationUnitID')
  ALTER TABLE dbo.FutureRoot ADD iQuotationUnitID int NULL
GO

----------------------------------------------------------------
-- Update usp_MmUnderlying_Get


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmUnderlying_Get]
GO

CREATE  PROC dbo.usp_MmUnderlying_Get
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
		
		    c.priceClose as fPriceClose,

			null as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    c.volume10Day as iVolume10Day,
		    c.volume5Expiration as iVolume5Expiration

		from ContractPriceView c 
		    --inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		where
			(c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3
		order by c.Symbol
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
		
		    c.priceClose as fPriceClose,

			tc.traderID as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    c.volume10Day as iVolume10Day,
		    c.volume5Expiration as iVolume5Expiration

		from ContractPriceView c 
		   -- inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			(c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3
		order by c.Symbol
	return @@error
GO

----------------------------------------------------------------
-- PriceProfile table (column), view & usp

--column
if not exists(select 1 from syscolumns 
  where id=object_ID('PriceProfile') AND Name='ZeroBidRuleForPnL')
  ALTER TABLE dbo.PriceProfile ADD ZeroBidRuleForPnL tinyint NULL
GO

-- view
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PriceProfileView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[PriceProfileView]
GO

CREATE VIEW dbo.PriceProfileView
AS
	select
		priceProfileID,
		priceProfileName,
		priceProfileDesc,
		defaultID,
		isOptionProfile,
		alwaysLast,
		midmarketPnL,
		singlePriceRule,
		bothPriceRule,
                           ZeroBidRuleForPnL
	from
		PriceProfile
	where
		actionID < 3
GO

-- usp
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PriceProfile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_PriceProfile_Get]
GO

CREATE PROCEDURE dbo.usp_PriceProfile_Get 
	@iPriceProfileID int = null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iPriceProfileID is null	
		select
			priceProfileID as iPriceProfileID,
			priceProfileName as vcPriceProfileName,
			priceProfileDesc as vcPriceProfileDesc,
			defaultID as tiDefaultID,
			isOptionProfile as tiIsOptionProfile,
			alwaysLast as tiAlwaysLast,
			midmarketPnL as tiMidmarketPnL,
			singlePriceRule as tiSinglePriceRule,
			bothPriceRule as tiBothPriceRule,
                                        ZeroBidRuleForPnL as tiZeroBidRuleForPnL
		from
			PriceProfileView
	else
		select
			priceProfileID as iPriceProfileID,
			priceProfileName as vcPriceProfileName,
			priceProfileDesc as vcPriceProfileDesc,
			defaultID as tiDefaultID,
			isOptionProfile as tiIsOptionProfile,
			alwaysLast as tiAlwaysLast,
			midmarketPnL as tiMidmarketPnL,
			singlePriceRule as tiSinglePriceRule,
			bothPriceRule as tiBothPriceRule,
                                        ZeroBidRuleForPnL as tiZeroBidRuleForPnL
		from
			PriceProfileView
		where
			priceProfileID = @iPriceProfileID

	set @error = @@error

	return @error
GO

----------------------------------------------------------------
-- usp_MmCustomDividend_Get

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmCustomDividend_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmCustomDividend_Get]
GO

CREATE   PROC dbo.usp_MmCustomDividend_Get 

	@iStockID int = null
AS
	set nocount on

	if isnull(@iStockID, 0) = 0
		select * from CustomDividend
		order by StockID, DivYtes, DivAmnt
	else
		select * from CustomDividend 
		where StockID = @iStockID
		order by DivYtes, DivAmnt
	return @@error

GO

----------------------------------------------------------------
-- OptionRoot table (column), view & usp (usp_MmOptionRoot_Get)

-- column
if not exists(select 1 from syscolumns 
  where id=object_ID('OptionRoot') AND Name='isFitEnabled')
  ALTER TABLE dbo.OptionRoot ADD isFitEnabled tinyint NULL
GO

-- view
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionRootView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[OptionRootView]
GO

CREATE        View dbo.OptionRootView
as
	Select 
		optionRootID, 
		underlyingID, 
		symbol, 
		isSynthetic, 
		lotSize, 
		actionID,
		actionDate,
                isFitEnabled
	from optionRoot
	where actionID < 3

GO

-- usp
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmOptionRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmOptionRoot_Get]
GO

CREATE PROC dbo.usp_MmOptionRoot_Get
	@iUnderlyingID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iUnderlyingID is null
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize,
                                        orv.IsFitEnabled as tiIsFitEnabled, 
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView orv
			left join SyntheticOptionRootView sor on orv.optionRootID = sor.optionRootID
	else
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize, 
                                        orv.IsFitEnabled as tiIsFitEnabled,
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView orv
			left join SyntheticOptionRootView sor on orv.optionRootID = sor.optionRootID
		where
			orv.underlyingID = @iUnderlyingID
	
	return @@error
GO

----------------------------------------------------------------
-- IRPoint table (column), view & usp (usp_IRPoint_Get)

-- column
if not exists(select 1 from syscolumns 
  where id=object_ID('IRPoint') AND Name='neutralRate')
  ALTER TABLE dbo.IRPoint ADD neutralRate float NULL
GO

-- view
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IRPointView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IRPointView]
GO

CREATE  View IRPointView
as
	Select 
		IRpointID,
		curveID,
		periodTypeID,
		num,
		shortRate,
		HTBRate,
		longRate,
		actionID,
		actionDate,
                           neutralRate
	from IRPoint
	where actionID < 3
GO

-- usp
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IRPoint_Get]
GO

CREATE PROC dbo.usp_IRPoint_Get
	@iCurveID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	if @iCurveID is not null
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID,
                                        neutralRate as neutralRate
		from
			IRPointView
		where
			curveID = @iCurveID
		order by periodTypeID, num 
	else
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID,
                                        neutralRate as neutralRate
		from
			IRPointView
		order by periodTypeID, num 

	set @error = @@error
Return(@error)
GO
-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Import]
GO

CREATE PROCEDURE dbo.usp_BoFuture_Import
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
	@dtExpiryDate as smalldatetime = null	
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
GO

----------------------------------------------------------------------------------------------
-- FutureRoot column

if not exists(select 1 from syscolumns 
  where id=object_ID('FutureRoot') AND Name='vcQuotationUnitCurrency')
  ALTER TABLE dbo.FutureRoot ADD vcQuotationUnitCurrency varchar(3) NULL
GO

----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Get]
GO

CREATE  PROC dbo.usp_BoFutureRoot_Get
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IC_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IC_Save]
GO

CREATE   PROCEDURE dbo.usp_IC_Save
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmFutureByUnderlying_Get]
GO

CREATE PROC dbo.usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
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
--                e.ExpiryPeriod as vcMaturity,
                fec.expiryDate as dtMaturity,
		fu.iFutureUnderlyingID as iFutureUnderlyingID,
                           f.futureRootID as iFutureRootID
	from FutureUnderlying fu
                inner join FutureRoot fr on fr.underlyingID = fu.iFutureUnderlyingID
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.FutureID = fc.contractID				
--                inner join Expiry e on e.expiryID = f.MaturityID
                inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
	where fu.iFutureUnderlyingID = @iUnderlyingID
		and fec.expiryDate >= @dtMinExpiry		

	return @@error
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodityFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodCommodityFutureRoot_Get]
GO

CREATE  PROC dbo.usp_EodCommodityFutureRoot_Get
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_CustomStrikeSkewPoint_Get]
GO

CREATE PROC dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null
as
/*
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare	@iExpiryID int
	declare @ContractTypeID int
	declare @UnderlyingContractID int

	-- Sharky: faster request for special parameters' set
	if @iExpiryID is null and @iCustomStrikeSkewPointID is null
		and @iVolaSurfaceDataID is not null and @iType is not null
	begin
		select @ContractTypeID = c.ContractTypeID from VolaSurfaceData vsd
			inner join Contract c on c.ContractID = vsd.ContractID
			where volaSurfaceDataID = @iVolaSurfaceDataID		
		if @ContractTypeID = 6  -- underlying
		begin
			select @UnderlyingContractID = vsd.ContractID from VolaSurfaceData vsd
			inner join Contract c on c.ContractID = vsd.ContractID
			where volaSurfaceDataID = @iVolaSurfaceDataID
			
			select 						
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
			from ContractView c
			inner join Future f on f.futureID = c.ContractID
			inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
			inner join VolaSurfaceData vsd on vsd.ContractID = c.ContractID
			inner join CustomStrikeSkewPoint CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock) 
	                           on CSSP.VolaSurfaceDataID = vsd.VolaSurfaceDataID	
			--join VolaSurfaceData vsd on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			where 
			--CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
			fr.UnderlyingID = @UnderlyingContractID
			and CSSP.type = @iType
			and CSSP.actionID < 3
		end
		else
		begin
			select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
			from CustomStrikeSkewPoint CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			--join VolaSurfaceData vsd on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			where 
			CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
			and CSSP.type = @iType
			and CSSP.actionID < 3
		end
	end
	else
	begin
		-- Sharky: old variant
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			--convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPointView CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			--join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			--join Contract C on  VolaSurfaceData.contractID = C.contractID
			--join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			customStrikeSkewPointID = isnull(@iCustomStrikeSkewPointID, customStrikeSkewPointID)
			and CSSP.volaSurfaceDataID = isnull(@iVolaSurfaceDataID, CSSP.volaSurfaceDataID) 
			and type = isnull(@iType, type) 
			and CSSP.expiryID = isnull(@iExpiryID, CSSP.expiryID)
			--and E.expiryID = isnull(@iExpiryID, E.expiryID)
	end

	set @error = @@error
	return(@error)
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlying_Get]
GO

CREATE      PROCEDURE dbo.usp_EodUnderlying_Get
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmFutureRoot_Get]
GO

CREATE PROC dbo.usp_MmFutureRoot_Get
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
			fr.vcQuotationUnitCurrency as vcQuotationUnitName
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
			fr.vcQuotationUnitCurrency as vcQuotationUnitName
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			fr.underlyingID = @iUnderlyingID
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Save]
GO

CREATE  PROC dbo.usp_BoFutureRoot_Save
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

		insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcFutureRootSymbol+'_Fut', @vcFutureRootName+' Fut')		
		insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcFutureRootSymbol+'_Opt', @vcFutureRootName+' Opt')

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

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Del]
GO

CREATE PROC dbo.usp_BoFutureRoot_Del
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

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoCommodity_Get]
GO

CREATE PROC dbo.usp_BoCommodity_Get
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmOptionByFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmOptionByFuture_Get]
GO

CREATE PROC dbo.usp_MmOptionByFuture_Get
	@iFutureID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

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
		inner join FutureOption fo on f.futureID = fo.futureID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
		inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID
	where f.futureID = @iFutureID
		and fec.expiryDate >= @dtMinExpiry

	return @@error
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmFuture_Get]
GO

CREATE PROC dbo.usp_MmFuture_Get
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
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
			inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fc.expCalendarID --fr.matCalendarID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
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
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
			inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fc.expCalendarID --fr.matCalendarID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
		where
			f.futureRootID = @iFutureRootID
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureByIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmFutureByIndex_Get]
GO

CREATE PROC dbo.usp_MmFutureByIndex_Get
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureOptionWithPrice_Get]
GO

CREATE proc dbo.usp_BoFutureOptionWithPrice_Get
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Index_Save]
GO

CREATE PROCEDURE dbo.usp_Index_Save
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

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoIndex_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoIndex_Del]
GO

CREATE PROC dbo.usp_BoIndex_Del
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Option_Import]
GO

CREATE  PROCEDURE usp_Option_Import
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
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiry, null, @iExpCalendarID
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
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiry, null, @iExpCalendarID
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
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoOptionWithPrice_Get]
GO

CREATE proc dbo.usp_BoOptionWithPrice_Get
	  @iUnderlyingID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			o.optionRootID as iOptionRootID,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol
	from OptionRoot ort
			inner join OptionView o on o.optionRootID = ort.optionRootID
			inner join ContractView c on c.contractID = o.optionID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and c.expCalendarID = ec.expCalendarID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
	where	ort.underlyingID = @iUnderlyingID
			and ec.expiryDate >= getdate()
	order by ort.optionRootID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
 
  return @@error
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmOptionByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmOptionByUnderlying_Get]
GO

CREATE   PROC dbo.usp_MmOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		ec.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
		ort.isFitEnabled as tiIsFitEnabled,
		ort.lotSize as iLotSize,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol
	from ContractView uc
		inner join OptionRoot ort on uc.contractID = ort.underlyingID
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on o.optionID = c.contractID
		inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
	where uc.contractID = @iUnderlyingID
		and ec.expiryDate >= @dtMinExpiry
	--order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	return @@error
GO

-----------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ActiveSurface_Get]
GO

CREATE            Procedure usp_ActiveSurface_Get 
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
		select @iContractID = contractID from ContractView where symbol = @vcSymbolName and  contractTypeID < 7--3

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
	set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select	@iVolaSurfaceDataID as iVolaSurfaceDataID


	set @error = @@error
Return(@error)
GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.177')
	begin
		update DataInfo set keyValue = '3.46.177' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.177' '3.46.177 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO