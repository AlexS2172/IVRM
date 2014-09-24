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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.119' or (keyValue = '3.46.120')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.120 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.120' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- usp_BoIndexWithBasketInfo_ClearWeights
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_BoIndexWithBasketInfo_ClearWeights') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_BoIndexWithBasketInfo_ClearWeights
GO

CREATE PROC dbo.usp_BoIndexWithBasketInfo_ClearWeights
	@iIndexID int = null
AS
	set nocount on
	declare @error int set @error = 0

	Begin Transaction

	if isnull(@iIndexID, 0) = 0
		delete IndexDefinition
		--update IndexDefinition
		--set actionID = 3
		--where actionID < 3
	else
		delete IndexDefinition
		where indexID = @iIndexID
		--update IndexDefinition
		--set actionID = 3
		--where indexID = @iIndexID and actionID < 3

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_EodFuture_Import') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_EodFuture_Import
GO

CREATE PROC dbo.usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output,
	@vcBadFutureName varchar(255) output
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
			select @iFutureID_bad = contractID from ContractView where symbol=@vcSymbol and contractID<>@iFutureID and contractTypeID=4
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

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID and actionID<>3
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
			
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureID_bad = contractID from ContractView where symbol=@vcSymbol and contractTypeID=4
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
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
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
					insert into ContractPrice(contractID,exchangeID)
						values(@iFutureID,null)
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
go

----------------------------------------------------------------------------------------------------
-- usp_EodFutureOption_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_EodFutureOption_Import') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_EodFutureOption_Import
GO

CREATE PROC dbo.usp_EodFutureOption_Import
	@iFutureID int,
	@fStrike float,
	@iExpiryID int,
	@tiIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@vcBadFutureOptionName varchar(255) output
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

	declare	@iFutureOptionID int
	declare @iFutureOptionID_bad int

	--===================================================================			
	-- select the @iFutureOptionID
	--===================================================================			
	select @iFutureOptionID = futureOptionID
			from FutureOption (nolock)
			where futureID = @iFutureID and 
				isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID
				
	if not exists (select * from tempdb..sysobjects where id=object_id('tempdb..#FutureContractsForDelete'))
		Create table #FutureContractsForDelete(contractID int)				
	if @@error <> 0 begin set @error = -1 goto finish end
				
	if @iFutureOptionID is not null
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID from ContractView where symbol=@vcSymbol and contractID<>@iFutureOptionID and contractTypeID=5
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

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureOptionID and actionID<>3
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureOption set actionDate = getdate() where futureOptionID=@iFutureOptionID
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID from ContractView where symbol=@vcSymbol and contractTypeID=5
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
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iFutureOptionID,null)
				if (@@error <> 0) begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

		Commit Transaction
	end 
	 
   	set @error = @iFutureOptionID
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_EodOption_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_EodOption_Import') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_EodOption_Import
GO

CREATE PROC dbo.usp_EodOption_Import
	@iRootID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iOptionID_bad int output
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.120')
	begin
		update DataInfo set keyValue = '3.46.120' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.120' '3.46.120 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
