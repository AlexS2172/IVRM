/*
	Contains stored procedures for ETS ASP EOD intermediate market structure database
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.122' or (keyValue = '4.48.123')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.48.123 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.48.123' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- IX_OptionRoot_underlyingID_symbol_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_OptionRoot_underlyingID_symbol_actionID' and id = object_id('dbo.OptionRoot'))
	drop index dbo.OptionRoot.IX_OptionRoot_underlyingID_symbol_actionID
GO
CREATE NONCLUSTERED INDEX IX_OptionRoot_underlyingID_symbol_actionID ON dbo.OptionRoot (underlyingID, symbol, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_OptionRoot_optionRootID_underlyingID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_OptionRoot_optionRootID_underlyingID_actionID' and id = object_id('dbo.OptionRoot'))
	drop index dbo.OptionRoot.IX_OptionRoot_optionRootID_underlyingID_actionID
GO
CREATE NONCLUSTERED INDEX IX_OptionRoot_optionRootID_underlyingID_actionID ON dbo.OptionRoot (optionRootID, underlyingID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_Option_optionRootID_isCall_strike_expiryID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Option_optionRootID_isCall_strike_expiryID_actionID' and id = object_id('dbo.[Option]'))
	drop index dbo.[Option].IX_Option_optionRootID_isCall_strike_expiryID_actionID
GO
CREATE NONCLUSTERED INDEX IX_Option_optionRootID_isCall_strike_expiryID_actionID ON dbo.[Option] (optionRootID, isCall, strike, expiryID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_Contract_symbol_contractID_contractTypeID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Contract_symbol_contractID_contractTypeID_actionID' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.IX_Contract_symbol_contractID_contractTypeID_actionID
GO
CREATE NONCLUSTERED INDEX IX_Contract_symbol_contractID_contractTypeID_actionID ON dbo.Contract (symbol, contractID, contractTypeID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_Contract_symbol_contractTypeID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Contract_symbol_contractTypeID_actionID' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.IX_Contract_symbol_contractTypeID_actionID
GO
CREATE NONCLUSTERED INDEX IX_Contract_symbol_contractTypeID_actionID ON dbo.Contract (symbol, contractTypeID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- Contract24 (drop)
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'Contract24' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.Contract24
GO

----------------------------------------------------------------------------------------------------
-- IX_Contract_contractID_expCalendarID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Contract_contractID_expCalendarID_actionID' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.IX_Contract_contractID_expCalendarID_actionID
GO

CREATE NONCLUSTERED INDEX IX_Contract_contractID_expCalendarID_actionID ON dbo.Contract (contractID, expCalendarID, actionID)
GO


----------------------------------------------------------------------------------------------------
-- IX_Future_futureRootID_maturityID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Future_futureRootID_maturityID' and id = object_id('dbo.Future'))
	drop index dbo.Future.IX_Future_futureRootID_maturityID
GO
CREATE NONCLUSTERED INDEX IX_Future_futureRootID_maturityID ON dbo.Future (futureRootID, maturityID)
GO

----------------------------------------------------------------------------------------------------
-- IX_FutureOption_futureID_isCall_strike_expiryID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_FutureOption_futureID_isCall_strike_expiryID' and id = object_id('dbo.FutureOption'))
	drop index dbo.FutureOption.IX_FutureOption_futureID_isCall_strike_expiryID
GO
CREATE NONCLUSTERED INDEX IX_FutureOption_futureID_isCall_strike_expiryID ON dbo.FutureOption (futureID, isCall, strike, expiryID)
GO


----------------------------------------------------------------------------------------------------
-- usp_EodBasketIndexComp_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodBasketIndexComp_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodBasketIndexComp_Get]
GO

CREATE   PROCEDURE dbo.usp_EodBasketIndexComp_Get 
	@iBasketIndexID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select
		i.indexID as iBasketIndexID,
		idef.stockID as iUnderlyingID,
		idef.weight as fWeight
	from
		IndexDefinition idef
		inner join [Index] i on idef.indexID = i.indexID and isnull(i.isBasket, 0) != 0
	where
		idef.indexID = isnull(@iBasketIndexID, idef.indexID)
		and isnull(idef.weight, 0) != 0
		
	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_CreateDefaultVolaSurface_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CreateDefaultVolaSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_CreateDefaultVolaSurface_Save]
GO

CREATE PROC dbo.usp_CreateDefaultVolaSurface_Save
	@iContractID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iVolaSurfaceDataID int
	if @iContractID is not null
		and not exists(select 1 from VolaSurfaceData where contractID=@iContractID and isDefaultSurface = 1)
	begin
		declare @fPrice float
	
		select @fPrice = priceClose
		from ContractPrice
		where contractID = @iContractID and exchangeID is null 

		insert into VolaSurfaceData(contractID,isDefaultSurface,baseUnderlinePrice)
			values (@iContractID,1,isnull(@fPrice,0))
		select @iVolaSurfaceDataID = @@identity
	end
	else 
		select @iVolaSurfaceDataID = VolaSurfaceDataID 
			from VolaSurfaceDataView 
			where contractID=@iContractID and isDefaultSurface = 1 and @iContractID is not null

	return(@iVolaSurfaceDataID)


GO

----------------------------------------------------------------------------------------------------
-- usp_EodDefaultIvPoint_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodDefaultIvPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodDefaultIvPoint_Save]
GO

CREATE   Procedure dbo.usp_EodDefaultIvPoint_Save 
	@iContractID int,
	@fStrike float,
	@iExpiryID int,
	@fVolatility float
AS
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

	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID)
	
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
GO

----------------------------------------------------------------------------------------------------
-- usp_EodExpirationDate_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodExpirationDate_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodExpirationDate_Get]
GO

CREATE    Procedure dbo.usp_EodExpirationDate_Get
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select expiryDate as dtExpiryDate
	from ExpiryInCalendar
	where expiryDate > getdate()
	group by expiryDate
	order by expiryDate

	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFutureOption_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureOption_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFutureOption_Del]
GO

CREATE PROC dbo.usp_EodFutureOption_Del
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
	where futureOptionID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -1 if @@trancount = 1 rollback tran goto finish end
	
	-- delete common
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
	
	delete Contract
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFutureOption_Import
----------------------------------------------------------------------------------------------------
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
	@fPriceClose float = null
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

----------------------------------------------------------------------------------------------------
-- usp_EodFutureRoot_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFutureRoot_Get]
GO

CREATE PROC dbo.usp_EodFutureRoot_Get
AS
	-- retrieve all future roots

	set nocount on

	select
		fr.underlyingID as iIndexID,
		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.futureRootName as vcFutureRootName
	from FutureRoot fr
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuture_Del]
GO

CREATE PROC dbo.usp_EodFuture_Del
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

	delete Contract
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Import
----------------------------------------------------------------------------------------------------
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
	@fPriceClose float = null
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

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
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
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID, exchangeID, priceClose)
						values(@iFutureID, null, @fPriceClose)
					if (@@error <> 0) begin set @error = -14 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -15 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 1

		Commit Transaction
	end 
	 
   	set @error = @iFutureID
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuturesOptionWithPositions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuturesOptionWithPositions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuturesOptionWithPositions_Get]
GO

CREATE PROCEDURE dbo.usp_EodFuturesOptionWithPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

	select distinct 
		FO.futureID as iFutureID,
		FC.symbol as vcFutureSymbol,
		FOC.symbol as vcOptionSymbol,
		FO.futureOptionID as iID,
		FO.isCall as tiIsCall,
		FO.strike as fStrike,
		EC.expiryDate as dtExpiry
	from
		FutureOption FO
		inner join Contract FOC on FOC.contractID = FO.futureOptionID
		inner join Contract FC on FO.futureID = FC.contractID
		inner join ExpiryInCalendar EC on FO.expiryID = EC.expiryID and EC.expCalendarID = FC.expCalendarID
	where
		ec.expiryDate >= getdate()

/*
	select distinct 
		FO.futureID as iFutureID,
		FC.symbol as vcFutureSymbol,
		FOC.symbol as vcOptionSymbol,
		FO.futureOptionID as iID,
		FO.isCall as tiIsCall,
		FO.strike as fStrike,
		EC.expiryDate as dtExpiry
	from
		TradeView tv  
		inner join FutureOption FO on FO.futureOptionID = tv.contractID
		inner join Contract FOC on FOC.contractID = tv.contractID
		inner join Contract FC on FO.futureID = FC.contractID
		inner join ExpiryInCalendar EC on FO.expiryID = EC.expiryID and EC.expCalendarID = FC.expCalendarID
	where
		ec.expiryDate >= getdate()
*/	
	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuturesWithOptionPositions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuturesWithOptionPositions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuturesWithOptionPositions_Get]
GO

CREATE PROCEDURE dbo.usp_EodFuturesWithOptionPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

--=================================================================
-- select futures with positions
--=================================================================
	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityID as iMaturityID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fOptionPosition,
		cp.priceClose as fPriceClose
	from TradeView T  
		inner join FutureOption FO on T.contractID=FO.futureOptionID
		inner join Future F on F.futureID=FO.futureID
		inner join Contract FC on FC.contractID=F.futureID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
	order by FR.underlyingID 
	
	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuturesWithPositions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuturesWithPositions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuturesWithPositions_Get]
GO

CREATE PROCEDURE dbo.usp_EodFuturesWithPositions_Get
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
		0 as fPosition,
		cp.priceClose as fPriceClose
	from Future F
		inner join Contract FC on FC.contractID=F.futureID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
	order by FR.underlyingID 

/*
	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityID as iMaturityID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,
		cp.priceClose as fPriceClose
	from TradeView T  
		inner join Future F on T.contractID=F.futureID
		inner join Contract FC on FC.contractID=T.contractID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
	order by FR.underlyingID 
*/	
	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodIndexUnderlyingBeta_Clear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodIndexUnderlyingBeta_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodIndexUnderlyingBeta_Clear]
GO

CREATE      Procedure dbo.usp_EodIndexUnderlyingBeta_Clear
	@iIndexID int = null, 	--the index identifier
	@iContractID int = null 	--the stock identifier
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	delete IndexBeta
		where contractID = isnull(@iContractID, contractID)
			and indexID = isnull(@iIndexID, indexID)

	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

    set @error = 0
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodIrCurve_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodIrCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodIrCurve_Get]
GO

CREATE   PROCEDURE dbo.usp_EodIrCurve_Get
	@iCurveID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	select
		curveID as iCurveID,
		ruleTypeID as iRuleTypeID, 
		positionThreshold as mPositionThreshold
	from
		IRCurve
	where
		curveID = isnull(@iCurveID, curveID)

	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodIrPoint_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodIrPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodIrPoint_Get]
GO

CREATE   PROCEDURE dbo.usp_EodIrPoint_Get
	@iCurveID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @dtToday datetime
	set @dtToday = getdate()
	
	select
		curveID as iCurveID,
		case periodTypeID
			when 1 then datediff(day, @dtToday, dateadd(day, num, @dtToday))
			when 2 then datediff(day, @dtToday, dateadd(week, num, @dtToday))
			when 3 then datediff(day, @dtToday, dateadd(month, num, @dtToday))
			when 4 then datediff(day, @dtToday, dateadd(year, num, @dtToday))
		end as iDTE,
		shortRate as fRateShort, 
		longRate as fRateLong,
		HTBRate as fRateHTB
	from
		IRPoint
	where
		curveID = isnull(@iCurveID, curveID)
	order by
		iDTE

	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodMsLoad_Complete
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsLoad_Complete]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodMsLoad_Complete]
GO

CREATE   PROCEDURE dbo.usp_EodMsLoad_Complete
as
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------

	select distinct symbol as vcUnderlyingSymbol
	from Contract C
		inner join MsUnderlyingCache MUC on C.contractID = MUC.underlyingID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodMsLoad_Init
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsLoad_Init]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodMsLoad_Init]
GO

CREATE    PROCEDURE dbo.usp_EodMsLoad_Init
	@iUnderlyingID int = null	
as
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0

	if @iUnderlyingID is null
	begin
		Begin Transaction MsCacheClear	
	
		TRUNCATE TABLE MsOptionCache
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		TRUNCATE TABLE MsUnderlyingCache
		if @@error <> 0 begin set @error = -2 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		Commit Transaction MsCacheClear
	end
	else
	begin
		Begin Transaction MsCacheClear	
	
		Delete from MsOptionCache where underlyingID = @iUnderlyingID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		Delete from MsUnderlyingCache where underlyingID = @iUnderlyingID
		if @@error <> 0 begin set @error = -2 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		Commit Transaction MsCacheClear
	end

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodMsOptionCache_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsOptionCache_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodMsOptionCache_Get]
GO

CREATE    PROCEDURE dbo.usp_EodMsOptionCache_Get
	@iUndID int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select
		O.optionID as iOptionID,
		O.symbol as vcSymbol,
		O.expiryDate as dtExpiryDate,
		O.strike as fStrike,
		O.isCall as tiIsCall,
		O.lotSize as iLotSize
	from MsOptionCache O
	where O.underlyingID = @iUndID
	
	Return(@@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodMsOptionCache_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsOptionCache_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodMsOptionCache_Save]
GO

CREATE   PROCEDURE dbo.usp_EodMsOptionCache_Save
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@dtExpiry datetime,
	@fStrike float,
	@tiIsCall tinyint,
	@iLotSize int
AS
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0
	declare @vcUnderlyingSymbol varchar(20)
	declare @vcError as varchar(8000)

	-- check parameter
	if @iUnderlyingID is null or @vcSymbol is null or @dtExpiry is null or isnull(@fStrike, 0) <= 0
	begin
		raiserror ('Invalid parameter!', 16, 1)
		set @error = -1
		goto finish 
	end

	-- check underlying existance
	if not exists (select 1 from MsUnderlyingCache where underlyingID = @iUnderlyingID)
	begin
		raiserror ('Invalid underlying contract!', 16, 1)
		set @error = -2
		goto finish 
	end

	-- check the Expiry
	declare @iExpiryID int
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists (select 1 from Expiry where expiryID = @iExpiryID)
	begin
		set @vcError = 'Invalid expiry ''' + Convert(varchar(50),@dtExpiry) + ''' for option ''' + @vcSymbol + ''' !'
		Raiserror(@vcError, 16, 1)
		set @error = -3
		goto finish 
	end

	-- check option root symbol
	if len(@vcSymbol) < 3
	begin
		set @vcError = 'Invalid option symbol ''' + @vcSymbol + ''': at least 3 characters expected!'
		Raiserror(@vcError, 16, 1)
		set @error = -4
		goto finish 
	end

	-- correct lotsize
	if isnull(@iLotSize, 0) <= 0
		set @iLotSize = 100

	insert into MsOptionCache
		(underlyingID, symbol, expiryDate, strike, isCall, lotSize)
	values
		(@iUnderlyingID, @vcSymbol, @dtExpiry, @fStrike, @tiIsCall, @iLotSize)
	
	if @@error <> 0 begin set @error = -5 goto finish end

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodMsUnderlyingCache_Check
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsUnderlyingCache_Check]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodMsUnderlyingCache_Check]
GO

CREATE	PROCEDURE dbo.usp_EodMsUnderlyingCache_Check
	@iContractID int
as
/*
	Checked if no options of the underlying exist in MsOptionCache then delete underlying record
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if not exists(select 1 from MsOptionCache O where O.underlyingID = @iContractID)
		begin
			delete from MsUnderlyingCache where underlyingID=@iContractID
			
			if @@error <> 0 set @error = @@error
		end
	 
   set @error = 0
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodMsUnderlyingCache_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsUnderlyingCache_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodMsUnderlyingCache_Get]
GO

CREATE    PROCEDURE dbo.usp_EodMsUnderlyingCache_Get
	@iUndID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select
		MS.underlyingID as iUnderlyingID,
		C.symbol as vcUndSymbol,
		MS.updateDate as dtUpdateDate
	from MsUnderlyingCache MS
		inner join Contract C on C.contractID = MS.underlyingID
	where MS.underlyingID = isnull(@iUndID, MS.underlyingID)
	
	Return(@@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodMsUnderlyingCache_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsUnderlyingCache_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodMsUnderlyingCache_Save]
GO

CREATE  PROCEDURE dbo.usp_EodMsUnderlyingCache_Save
	@iUnderlyingID int,
	@dtUpdateDate datetime = null
AS
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0

	-- check parameter
	if @iUnderlyingID is null
	begin
		raiserror ('Invalid parameter!', 16, 1)
		set @error = -1
		goto finish 
	end

	-- check underlying existance
	if not exists (select 1 from Contract where contractID = @iUnderlyingID and contractTypeID < 3)
	begin
		raiserror ('Invalid underlying contract!', 16, 1)
		set @error = -2
		goto finish 
	end

	if @dtUpdateDate is null set @dtUpdateDate = getdate()

	-- update underlying cache
	Begin Transaction MsUnderlyingCache

	if not exists (select 1 from MsUnderlyingCache where underlyingID = @iUnderlyingID)
	begin
		insert into MsUnderlyingCache
			(underlyingID, updateDate)
		values
			(@iUnderlyingID, @dtUpdateDate)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0  Rollback Transaction MsUnderlyingCache goto finish end
	end
	else
	begin
		update
			MsUnderlyingCache
		set
			updateDate = @dtUpdateDate
		where
			underlyingID = @iUnderlyingID and updateDate < @dtUpdateDate

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction MsUnderlyingCache goto finish end
	end

	Commit Transaction MsUnderlyingCache

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOldFutureOptions_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOldFutureOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOldFutureOptions_Del]
GO

CREATE PROCEDURE dbo.usp_EodOldFutureOptions_Del
	@iFutureID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Create table #FutureContractsForDelete(contractID int) 

	insert into #FutureContractsForDelete
		select FO.futureOptionID
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
					( 
						E.expiryDate < GetDate() or datediff(d, FO.actionDate, GetDate()) > 0 
					)
					and FO.futureID = @iFutureID

	exec @error = usp_EodFutureOption_Del

	select null as vcSymbol from #FutureContractsForDelete where contractID is null

finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOldFutures_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOldFutures_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOldFutures_Del]
GO

CREATE PROCEDURE dbo.usp_EodOldFutures_Del
	@iFutureRootID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Create table #FutureContractsForDelete(contractID int) 

	insert into #FutureContractsForDelete
		select F.futureID
			from 
				Future F
					join Contract C on F.futureID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and F.futureRootID = @iFutureRootID
				)

	insert into #FutureContractsForDelete
		select FO.futureOptionID
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and F.futureRootID = @iFutureRootID
				)

	exec @error = usp_EodFuture_Del

	select null as vcSymbol from #FutureContractsForDelete where contractID is null

finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptClosePrice_Complete_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptClosePrice_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptClosePrice_Complete_Save]
GO

CREATE      procedure dbo.usp_EodOptClosePrice_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODOP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	if object_id(@vcFullTableName) is null
		begin
	    	Raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
	       	Return (-1)
	   	end

	Begin Transaction
		set @vcExecString1 ='update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate
		set @vcExecString2 =' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
		exec (@vcExecString1 + @vcExecString2)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 
	Commit Transaction

    set @error = 0
finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptClosePrice_Init_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptClosePrice_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptClosePrice_Init_Save]
GO

CREATE         procedure dbo.usp_EodOptClosePrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	update ContractPrice
		set priceClose = null
	from (select contractPriceID from ContractPrice CP 
			right join TradeView T on T.contractID = CP.contractID
			join OptionView O on O.optionID = T.contractID
			where CP.exchangeID is null) as CP2
	where ContractPrice.contractPriceID=CP2.contractPriceID

	set @error = @@error
	if @error<>0 goto finish

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODOP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODOP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODOP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceClose float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptClosePrice_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptClosePrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptClosePrice_Save]
GO

CREATE          procedure dbo.usp_EodOptClosePrice_Save
	@iContractID int,
	@fPriceClose float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODOP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	set @vcExecString1 = 'insert into ' + @vcTableName + '(contractID, priceClose) values(' + cast(@iContractID as varchar(10)) + ',' + cast(@fPriceClose as varchar(500)) + ')'

	if object_id(@vcFullTableName) is not null
		begin

			exec(@vcExecString1)
				if @@error<>0 begin set @error = @@error Return(@error) end 
		end

	set @vcExecString1 = 'if (select count(1) from ' + @vcTableName + '(nolock)) > 1999' + 
		' begin update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate + 
		' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
	set @vcExecString2 =' insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate +
		' from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)' +
		' truncate table ' + @vcTableName + ' end'

	if object_id(@vcFullTableName) is not null
		begin
			exec (@vcExecString1 + @vcExecString2)
		end

finish:
    set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptTheoPrice_Complete_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptTheoPrice_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptTheoPrice_Complete_Save]
GO

CREATE      procedure dbo.usp_EodOptTheoPrice_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODTP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	if object_id(@vcFullTableName) is null
		begin
	    	Raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
	       	Return (-1)
	   	end

	Begin Transaction
		set @vcExecString1 ='update ContractPrice set priceTheo=EOD.priceTheo,actionDate=' + @vcDate
		set @vcExecString2 =' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
		exec (@vcExecString1 + @vcExecString2)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceTheo, actionDate) select contractID, priceTheo,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 
	Commit Transaction

    set @error = 0
finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptTheoPrice_Init_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptTheoPrice_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptTheoPrice_Init_Save]
GO

CREATE         procedure dbo.usp_EodOptTheoPrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODTP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODTP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODTP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select 1 from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceTheo float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptTheoPrice_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptTheoPrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptTheoPrice_Save]
GO

CREATE  procedure dbo.usp_EodOptTheoPrice_Save
	@iContractID int,
	@fPriceTheo float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODTP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	set @vcExecString1 = 'insert into ' + @vcTableName + '(contractID, priceTheo) values(' + cast(@iContractID as varchar(10)) + ',' + cast(@fPriceTheo as varchar(500)) + ')'

	if object_id(@vcFullTableName) is not null
		begin

			exec(@vcExecString1)
				if @@error<>0 begin set @error = @@error Return(@error) end 
		end

	set @vcExecString1 = 'if (select count(1) from ' + @vcTableName + '(nolock)) > 1999' + 
		' begin update ContractPrice set priceTheo=EOD.priceTheo,actionDate=' + @vcDate + 
		' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
	set @vcExecString2 =' insert into ContractPrice (contractID, priceTheo, actionDate) select contractID, priceTheo,' + @vcDate +
		' from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)' +
		' truncate table ' + @vcTableName + ' end'

	if object_id(@vcFullTableName) is not null
		begin
			exec (@vcExecString1 + @vcExecString2)
		end

finish:
    set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptionName_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptionName_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptionName_Get]
GO

CREATE proc dbo.usp_EodOptionName_Get
	@iOptionID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select symbol as vcSymbol,
		contractName as vcContractName
	from Contract
	where contractID = @iOptionID

	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptionRoot_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptionRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptionRoot_Get]
GO

CREATE    PROCEDURE dbo.usp_EodOptionRoot_Get 
	@iUnderlyingID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select distinct
		ORT.underlyingID as iUnderlyingID,
		ORT.optionRootID as iRootID,
		ORT.symbol as vcSymbol,
		ORT.IsSynthetic as tiIsSynthetic, 
		ORT.lotSize as iLotSize, 
		SOR.cashValue as fCash,
		SOR.synthSkew as fSyntheticSkew, 
		SOR.synthKurt as fSyntheticKurt,
		SOR.synthYield as fSyntheticYield, 
		SOR.synthIsBasket as tiSyntheticIsBasket
	from
		OptionRoot as ORT
		left join SyntheticOptionRoot as SOR on ORT.optionRootID = SOR.optionRootID
		join [Option] O on O.optionRootID=ORT.optionRootID  
	where
		ORT.underlyingID = isnull(@iUnderlyingID, ORT.underlyingID)
		
	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptionRoot_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptionRoot_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptionRoot_Import]
GO

CREATE   PROCEDURE dbo.usp_EodOptionRoot_Import
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@iLotSize int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @iOptionRootID int,
		@iUnderlyingID_bad int,
   		@ERR_MSG varchar(8000)

    select @iOptionRootID = optionRootID,
		@iUnderlyingID_bad = underlyingID
	from OptionRoot
	where symbol = @vcSymbol
		--and underlyingID != @iUnderlyingID
	
    if @iOptionRootID is not null and @iUnderlyingID_bad != @iUnderlyingID
	begin
		declare @error int set @error = 0

		exec @error = dbo.usp_EodOptionRoot_Del @iOptionRootID = @iOptionRootID

		if @@error != 0 or @error != 0
		begin		
			set @ERR_MSG = 'Fail to delete old assigned option root with name ''' + @vcSymbol + '''.'			
			set @iOptionRootID = -1
			goto finish
		end

		set @iOptionRootID = null
	end  
	
	/*select @iRootID = optionRootID
	from OptionRoot
	where symbol = @vcSymbol
		and underlyingID = @iUnderlyingID*/

	if @iOptionRootID is null
	begin
		insert into OptionRoot (underlyingID, symbol, isSynthetic, lotSize)
		values (@iUnderlyingID,@vcSymbol,0,@iLotSize)

		if @@error <> 0 set @iOptionRootID = -2
		else set @iOptionRootID = @@identity
	end

finish:
	select @ERR_MSG	as vcErrorMsg
	Return(@iOptionRootID)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptionRoot_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptionRoot_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptionRoot_Del]
GO

CREATE PROC dbo.usp_EodOptionRoot_Del
	@iOptionRootID int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	Begin Transaction DelOptRoot

	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end

	truncate table #Options_for_delete

	insert into #Options_for_delete (optionID)
	select optionID
	from [Option]
	where actionID < 3 and optionRootID = @iOptionRootID

	delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end
	delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end
	delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end
	delete from DualQuadraticParams where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end
	delete [Option] where OptionID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end
	delete Contract where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end

	delete SyntheticUnderlyingParam  where syntheticOptRootID in 
		(select syntheticOptRootID from SyntheticOptionRoot where optionRootID = @iOptionRootID)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end

	delete SyntheticRootIndexBeta  where syntheticOptRootID in 
		(select syntheticOptRootID from SyntheticOptionRoot where optionRootID = @iOptionRootID)
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end

	delete SyntheticOptionRoot where optionRootID = @iOptionRootID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end

	delete OptionRoot where optionRootID = @iOptionRootID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOptRoot goto finish end

	Commit Transaction DelOptRoot

finish:
	Return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptionWithPos_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptionWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptionWithPos_Get]
GO

CREATE     PROCEDURE dbo.usp_EodOptionWithPos_Get
	@iUnderlyingID int = null,
	@dtMinExpDate datetime = Null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @dtMinExpDate is null set @dtMinExpDate = getdate()

	select distinct
		ort.underlyingID as iUnderlyingID,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootName,
		ort.IsSynthetic as tiIsSynthetic, 
		ort.lotSize as iLotSize, 
		sor.cashValue as fCash,
		sor.synthSkew as fSyntheticSkew, 
		sor.synthKurt as fSyntheticKurt,
		sor.synthYield as fSyntheticYield, 
		sor.synthIsBasket as tiSyntheticIsBasket,
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		ec.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
	    cp.priceClose as fPriceClose
	from
		[Option] o
		inner join Contract c on c.contractID = o.optionID
		inner join OptionRoot ort on o.optionRootID = ort.optionRootID
		inner join Contract uc on ort.underlyingID = uc.contractID
		inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		left join SyntheticOptionRoot as sor on ort.optionRootID = sor.optionRootID
		left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
	where
		o.actionID < 3
		and c.contractTypeID = 3
		and ort.underlyingID = isnull(@iUnderlyingID, ort.underlyingID)
		and ec.expiryDate >= @dtMinExpDate
	order by ort.underlyingID,ort.optionRootID

/*	select distinct
		ort.underlyingID as iUnderlyingID,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootName,
		ort.IsSynthetic as tiIsSynthetic, 
		ort.lotSize as iLotSize, 
		sor.cashValue as fCash,
		sor.synthSkew as fSyntheticSkew, 
		sor.synthKurt as fSyntheticKurt,
		sor.synthYield as fSyntheticYield, 
		sor.synthIsBasket as tiSyntheticIsBasket,
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		ec.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
	    cp.priceClose as fPriceClose
	from
		TradeView tv
		left join [Option] o on o.optionID = tv.contractID
		left join Contract c on c.contractID = tv.contractID
		left join OptionRoot ort on o.optionRootID = ort.optionRootID
		left join SyntheticOptionRoot as sor on ort.optionRootID = sor.optionRootID
		left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
		left join Contract uc on ort.underlyingID = uc.contractID
		left join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
	where
		c.contractTypeID = 3
		and ort.underlyingID = isnull(@iUnderlyingID, ort.underlyingID)
		and ec.expiryDate >= @dtMinExpDate
	order by ort.underlyingID,ort.optionRootID*/
		
	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOption_Import
----------------------------------------------------------------------------------------------------
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
	@fPriceClose float = null
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

	declare	@OptionID int

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	select @OptionID = optionID
	from [Option] with(nolock, index = IX_Option_optionRootID_isCall_strike_expiryID_actionID)
	where optionRootID = @iRootID
		and isCall = @iIsCall
		and abs(strike - @fStrike) < 0.009
		and expiryID = @iExpiryID
		and actionID < 3

	if @OptionID is not null
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
			where symbol = @vcSymbol
				--and contractID <> @OptionID
				and contractTypeID = 3
				and actionID < 3

			if @iOptionID_bad is not null and @iOptionID_bad != @OptionID
			begin
				exec @error = usp_EodOption_Del @iOptionID = @iOptionID_bad			
	
				if @@error != 0 or @error != 0
				begin 
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end
			end
				
			update Contract 
			set symbol = @vcSymbol, contractName = @vcContractName
			where contractID=@OptionID and actionID < 3
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]  
			set isManualyEntered = 0 
			where optionID=@OptionID and actionID < 3
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

			update ContractPrice
			set priceClose = @fPriceClose
			where contractID = @OptionID and exchangeID is null
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
			where symbol = @vcSymbol
				and contractTypeID = 3
				and actionID < 3

			if @iOptionID_bad is not null
			begin
				exec @error = usp_EodOption_Del @iOptionID = @iOptionID_bad			
	
				if @@error != 0 or @error != 0
				begin 
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end
			end

			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@vcContractName)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	
			if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @OptionID)
			begin
				insert into ContractPrice(contractID, exchangeID, priceClose)
					values(@OptionID, null, @fPriceClose)
				if (@@error <> 0) begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
		
		Commit Transaction
	end 
	 
   	set @error = @OptionID
finish:
	return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOption_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOption_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOption_Del]
GO

CREATE PROC dbo.usp_EodOption_Del
	@iOptionID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction DelOpt

	delete OptionDataCache where optionID = @iOptionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
	delete from ContractPrice where ContractID = @iOptionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
	delete from ContractMarketData where contractID = @iOptionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
	delete from DualQuadraticParams where contractID = @iOptionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
	delete [Option] where OptionID = @iOptionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
	delete Contract where contractID = @iOptionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	Commit Transaction DelOpt

finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOptions_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOptions_Del]
GO

CREATE PROCEDURE dbo.usp_EodOptions_Del
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if object_ID('tempdb..#Options_for_delete') is null
		return 0

	Begin Transaction DelOpt

	delete OptionDataCache
	from OptionDataCache t
		inner join #Options_for_delete d on t.optionID = d.optionID
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete from ContractPrice
	from ContractPrice t
		inner join #Options_for_delete d on t.contractID = d.optionID
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete ContractMarketData
	from ContractMarketData t
		inner join #Options_for_delete d on t.contractID = d.optionID
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete DualQuadraticParams
	from DualQuadraticParams t
		inner join #Options_for_delete d on t.contractID = d.optionID
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete [Option]
	from [Option] t
		inner join #Options_for_delete d on t.optionID = d.optionID
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete Contract 
	from Contract t
		inner join #Options_for_delete d on t.contractID = d.optionID
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	Commit Transaction DelOpt

finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodEmptyOptionRoots_Del
----------------------------------------------------------------------------------------------------
/*if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodEmptyOptionRoots_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodEmptyOptionRoots_Del]*/
GO

/*CREATE PROCEDURE dbo.usp_EodEmptyOptionRoots_Del
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	Begin Transaction DelOpt

	delete SyntheticUnderlyingParam  where syntheticOptRootID not in 
		(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete SyntheticRootIndexBeta  where syntheticOptRootID not in 
		(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete SyntheticOptionRoot where optionRootID not in (select optionRootID from OptionView)
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	delete OptionRoot where optionRootID not in (select optionRootID from OptionView)
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

	Commit Transaction DelOpt

finish:
	Return(@error)*/
GO

----------------------------------------------------------------------------------------------------
-- usp_Option_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Option_Del]
GO

CREATE PROCEDURE dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0,
	@bisRemoveFromContract bit = 1,
	@bisRaiseError bit = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an open position on it.', 16, 1)
						end
					Return (-2)
				end   
			if exists (select 1 from [Order] where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an order on it.', 16, 1)
						end
					Return (-3)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end

	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		--IndexDefinition by contractID
		delete IndexBeta where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from DualQuadraticParams where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaBands where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete SyntheticUnderlyingParam  where syntheticOptRootID not in 
			(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete SyntheticRootIndexBeta  where syntheticOptRootID not in 
			(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete SyntheticOptionRoot where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete OptionRoot where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		if @bisRemovePermanent = 1
			begin
				--IndexBeta by contractID
				delete from IndexBeta where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				if @bisRemoveFromContract = 1 
					begin
						delete Contract where contractID in (select OptionID from #Options_for_delete)
							if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
					end

				delete from SyntheticUnderlyingParam where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticRootIndexBeta where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from OptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodSynthRootCompWithPosition_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodSynthRootCompWithPosition_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodSynthRootCompWithPosition_Get]
GO

CREATE    PROCEDURE dbo.usp_EodSynthRootCompWithPosition_Get 
	@iUnderlyingID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select distinct
		ort.underlyingID as iUnderlyingID,
		ort.optionRootID as iRootID,
		sup.underlyingID as iCompUndID, 
		sup.SyntheticCoeff as fWeight
	from
		SyntheticUnderlyingParam as sup
		inner join SyntheticOptionRoot as sor
				on sor.syntheticOptRootID = sup.syntheticOptRootID
		inner join OptionRoot as ort
				on sor.optionRootID = ort.optionRootID
		inner join Contract as C
				on C.contractID = sup.underlyingID
		join OptionView O on O.optionRootID=ort.optionRootID  
		join TradeView TV on TV.contractID=O.optionID
	where
		ort.underlyingID = isnull(@iUnderlyingID, ort.underlyingID)
		
	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUndClosePrice_Complete_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUndClosePrice_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUndClosePrice_Complete_Save]
GO

CREATE     procedure dbo.usp_EodUndClosePrice_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODUP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	if object_id(@vcFullTableName) is null
		begin
	    	Raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
	       	Return (-1)
	   	end

	Begin Transaction
		set @vcExecString1 ='update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate
		set @vcExecString2 =' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
		exec (@vcExecString1 + @vcExecString2)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 
	Commit Transaction

    set @error = 0
finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUndClosePrice_Init_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUndClosePrice_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUndClosePrice_Init_Save]
GO

CREATE       procedure dbo.usp_EodUndClosePrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	update ContractPrice
		set priceClose = null
		from (select contractPriceID from ContractPrice CP
		right join TradeView T on T.contractID = CP.contractID
		left join ContractView C on C.contractID = T.contractID
		where CP.exchangeID is null 
			and C.contractTypeID < 3) as CP2
		where ContractPrice.contractPriceID=CP2.contractPriceID

	set @error = @@error
	if @error<>0 goto finish

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODUP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODUP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODUP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'			
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select 1 from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceClose float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUndClosePrice_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUndClosePrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUndClosePrice_Save]
GO

CREATE         procedure dbo.usp_EodUndClosePrice_Save
	@iContractID int,
	@fPriceClose float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODUP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	set @vcExecString1 = 'insert into ' + @vcTableName + '(contractID, priceClose) values(' + cast(@iContractID as varchar(10)) + ',' + cast(@fPriceClose as varchar(500)) + ')'

	if object_id(@vcFullTableName) is not null
		begin

			exec(@vcExecString1)
				if @@error<>0 begin set @error = @@error Return(@error) end 
		end

	set @vcExecString1 = 'if (select count(1) from ' + @vcTableName + '(nolock)) > 1999' + 
		' begin update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate + 
		' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
	set @vcExecString2 =' insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate +
		' from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)' +
		' truncate table ' + @vcTableName + ' end'

	if object_id(@vcFullTableName) is not null
		begin
			exec (@vcExecString1 + @vcExecString2)
		end

finish:
    set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUndExpirationsWithEmptyIvSurface_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUndExpirationsWithEmptyIvSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUndExpirationsWithEmptyIvSurface_Get]
GO

CREATE  PROCEDURE dbo.usp_EodUndExpirationsWithEmptyIvSurface_Get
	@iUndID int = null
as
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	select distinct 
		CV.contractID as iContractID,
		OV.expiryID as iExpiryID
	from [Option] OV
		join OptionRoot ORT on ORT.optionRootID = OV.optionRootID
		join Contract CV on ORT.underlyingID = CV.contractID
		left join VolaSurfaceData VSD on ORT.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
		left join CustomStrikeSkewPoint CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
			and OV.expiryID = CSSP.expiryID
	where 
		CV.contractTypeID < 3
		and CV.contractID = isnull(@iUndID, CV.contractID)
		and CSSP.volaSurfaceDataID is null
	order by CV.contractID,OV.expiryID
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUndWithPositions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUndWithPositions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUndWithPositions_Get]
GO

CREATE    PROCEDURE dbo.usp_EodUndWithPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = -1

	select 
		c.contractID as iUnderlyingID,
		0 as fPosition
	from Contract c
	where c.contractTypeID < 3 and c.actionID < 3
/*
	select 
		t.contractID as iUnderlyingID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition
	from TradeView T 
		inner join Contract c on T.contractID = c.contractID and c.contractTypeID < 3 and c.actionID < 3
	group by t.contractID
*/
	
set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUnderlyingAverageVolumes_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlyingAverageVolumes_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlyingAverageVolumes_Save]
GO

CREATE  PROCEDURE dbo.usp_EodUnderlyingAverageVolumes_Save 
	@iUndID int, 
	@iVolume10Day int = null,
	@iVolume5Expiration int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction
	if not exists(select 1 from ContractPrice where contractID=@iUndID and isnull(exchangeID,0)=0)
		begin
			insert into ContractPrice (contractID,
					exchangeID, 
					volume10Day,
					volume5Expiration)
			  values (@iUndID,
					null,
					@iVolume10Day,
					@iVolume5Expiration)

			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		end
	else
		begin

			update ContractPrice set 
				volume10Day = isnull(@iVolume10Day, volume10Day),
				volume5Expiration = isnull(@iVolume5Expiration, volume5Expiration)
			where contractID = @iUndID and isnull(exchangeID,0)=0

   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end
	Commit Transaction


    set @error = 0
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUnderlyingIndexBeta_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlyingIndexBeta_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlyingIndexBeta_Save]
GO

CREATE     Procedure dbo.usp_EodUnderlyingIndexBeta_Save 
	@iUnderlyingID int,
	@iIndexID int,
	@fBeta float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	Begin Transaction
		if not exists(select 1 From IndexBeta where indexID = @iIndexID and contractID = @iUnderlyingID) 
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
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUnderlyingSkewKurt_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlyingSkewKurt_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlyingSkewKurt_Save]
GO

CREATE  PROCEDURE dbo.usp_EodUnderlyingSkewKurt_Save 
	@iUndID int,
	@fSkew float = null,
	@fKurt float = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractTypeID int

	select @iContractTypeID = ContractTypeID from Contract
		where contractTypeID < 3 and contractID=@iUndID

	Begin Transaction
	if @iContractTypeID = 1
		begin
			update [Index]
				set skew = isnull(@fSkew, skew),
					kurt = isnull(@fKurt, kurt)
			where indexID = @iUndID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		end
	else
		begin
			update Stock set 
				skew = isnull(@fSkew, skew),
				kurt = isnull(@fKurt, kurt)
			where stockID = @iUndID
   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end
	Commit Transaction

    set @error = 0
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodUnderlying_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlying_Get]
GO

CREATE      PROCEDURE dbo.usp_EodUnderlying_Get
	@iUnderlyingID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

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
		and c.contractID = isnull(@iUnderlyingID, c.contractID)
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
		and c.contractID = isnull(@iUnderlyingID, c.contractID)
		
	set @error = @@error
	Return(@error)
GO


----------------------------------------------------------------------------------------------------
-- usp_EodUnderlying_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlying_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlying_Import]
GO

CREATE PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceClose float = null
as
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
	if exists(select 1 from ContractPrice where exchangeID is null and contractID = @iContractID)
	begin
		update ContractPrice
		set priceClose = @fPriceClose
		where exchangeID is null and contractID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update close price for default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish
			end
	end
	else
	begin
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, @fPriceClose)
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
	if (@bIsStock <> 0 and exists(select 1 from Stock where stockID=@iContractID))
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

----------------------------------------------------------------------------------------------------
-- usp_EodVolaBands_Complete_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodVolaBands_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodVolaBands_Complete_Save]
GO

CREATE   proc dbo.usp_EodVolaBands_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000)

	set @vcTableName = '##ETSEODSRVB' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	if object_id(@vcFullTableName) is null 
		begin
		   raiserror ('Cannot complete EOD. Cannot find save data of IV bands.', 16, 1)
		   return (-1)
		end

	Begin tran 
		set @vcExecString1 = 'delete VolaBands where contractID in (select distinct contractID from ' + @vcTableName + ')'
		set @vcExecString2 = ' insert into VolaBands (contractID,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)' + 
				'select contractID,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi from ' + @vcTableName +
				' truncate table ' + @vcTableName
		
		exec (@vcExecString1 + @vcExecString2)
 		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit tran

    set @error = 0

finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodVolaBands_Init_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodVolaBands_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodVolaBands_Init_Save]
GO

CREATE   proc dbo.usp_EodVolaBands_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000)


	set @vcTableName = '##ETSEODSRVB' + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_VolaBand' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'VolaBand_contractID_ind' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select 1 from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of Vola Bands EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		expPeriod int not null,
		strikeMoneyness float not null,
		IVBandLow float not null,
		IVBandHi float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' ([contractID])')

	set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodVolaBands_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodVolaBands_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodVolaBands_Save]
GO

CREATE   proc dbo.usp_EodVolaBands_Save
	@iUndID int,
	@iExpPeriod int,
	@fStrikeMoneyness float,
	@fIVBandLow float,
	@fIVBandHi float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000)

	set @vcTableName = '##ETSEODSRVB' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName


	set @vcExecString1 = 'insert into ' + @vcTableName + ' (contractID,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)'
							+ 'values (' + cast(@iUndID as varchar(50)) + ',' + cast(@iExpPeriod as varchar(50)) + ',' +  cast(@fStrikeMoneyness as varchar(50)) + ',' +  cast(@fIVBandLow as varchar(50)) + ',' + cast(@fIVBandHi as varchar(50)) + ')'

	exec(@vcExecString1)
		if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end

	set @vcExecString1 = 'if (select count(1) from ' + @vcTableName + '(nolock)) > 1999' + 
			' begin delete VolaBands where contractID in (select distinct contractID from ' + @vcTableName + ')' 
	set @vcExecString2 = 'insert into VolaBands (contractID,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)' + 
			' select contractID,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi from ' + @vcTableName +
			' truncate table ' + @vcTableName + ' end'
	
	exec (@vcExecString1 + @vcExecString2)

finish:
    set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodWeightedVega_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodWeightedVega_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodWeightedVega_Save]
GO

CREATE       Procedure dbo.usp_EodWeightedVega_Save
	@dtExpiryDate smalldatetime,
	@fVegaWeight float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	update ExpiryInCalendar
	   	set vegaWeight = @fVegaWeight
	where datediff(d, expiryDate, @dtExpiryDate) = 0

    set @error = @@error
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodExpiredOptions_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodExpiredOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodExpiredOptions_Del]
GO

CREATE PROCEDURE dbo.usp_EodExpiredOptions_Del
	@iContractID int -- stock or index identifier
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Create table #Options_for_delete(OptionID int) 

	insert into #Options_for_delete
		select O.optionID
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where (E.expiryDate < GetDate() or datediff(d, O.actionDate, GetDate()) > 0)
					and O.actionID < 3
					and UC.contractID = @iContractID


	if exists (select 1 from #Options_for_delete)
	begin
		begin tran
	
		delete OptionDataCache
		from OptionDataCache t
			inner join #Options_for_delete d on t.optionID = d.optionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete from ContractPrice
		from ContractPrice t
			inner join #Options_for_delete d on t.contractID = d.optionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete ContractMarketData
		from ContractMarketData t
			inner join #Options_for_delete d on t.contractID = d.optionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete DualQuadraticParams
		from DualQuadraticParams t
			inner join #Options_for_delete d on t.contractID = d.optionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete [Option]
		from [Option] t
			inner join #Options_for_delete d on t.optionID = d.optionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete Contract 
		from Contract t
			inner join #Options_for_delete d on t.contractID = d.optionID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end

		Create table #OptionRoots_for_delete (optionRootID int) 
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end

		insert #OptionRoots_for_delete (optionRootID)
		select ort.optionRootID
		from OptionRoot ort
			left join [Option] o on o.optionRootID = ort.optionRootID and o.actionID < 3
		where ort.actionID < 3
			and ort.underlyingID = @iContractID
			and o.optionRootID is null
		group by ort.optionRootID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end

		delete SyntheticUnderlyingParam
		from SyntheticUnderlyingParam t
			inner join SyntheticOptionRoot t2 on t2.syntheticOptRootID = t.syntheticOptRootID
			inner join #OptionRoots_for_delete d on d.optionRootID = t2.optionRootID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete SyntheticRootIndexBeta
		from SyntheticRootIndexBeta t
			inner join SyntheticOptionRoot t2 on t2.syntheticOptRootID = t.syntheticOptRootID
			inner join #OptionRoots_for_delete d on d.optionRootID = t2.optionRootID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete SyntheticOptionRoot
		from SyntheticOptionRoot t
			inner join #OptionRoots_for_delete d on d.optionRootID = t.optionRootID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	
		delete OptionRoot
		from OptionRoot t
			inner join #OptionRoots_for_delete d on d.optionRootID = t.optionRootID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end

		commit tran
	end

	select null as vcSymbol from #Options_for_delete where OptionID is null


finish:
	if @error != 0
		raiserror ('Error. Can''t delete useless options!', 16, 1)
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.48.123')
	begin
		update DataInfo set keyValue = '4.48.123' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.48.123' '4.48.123 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
