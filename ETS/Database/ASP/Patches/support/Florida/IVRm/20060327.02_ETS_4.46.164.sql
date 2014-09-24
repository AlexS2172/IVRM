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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
ALTER  PROC dbo.usp_EodOption_Import
	@iRootID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iOptionID_bad int output,
	@fPriceClose float = null,
        @bSaveClosePrice bit = null
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

			if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
                        begin 	
				update ContractPrice
					set priceClose = @fPriceClose
					where contractID = @OptionID and exchangeID is null
				if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
                        end
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
	  		    if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
				insert into ContractPrice(contractID, exchangeID, priceClose)
					values(@OptionID, null, @fPriceClose)
                            else
				insert into ContractPrice(contractID, exchangeID, priceClose)
					values(@OptionID, null, null)

			    if (@@error <> 0) begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
		
		Commit Transaction
	end 
	 
   	set @error = @OptionID
finish:
	return(@error)

GO
GO


ALTER  PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceClose float = null,
        @bSaveClosePrice bit = null

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
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
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
	end
	else
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, @fPriceClose)
	      else
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, null)
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.164')
	begin
		update DataInfo set keyValue = '4.49.164' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.164' '4.49.164 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodStocksClosePricesFromIV_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodStocksClosePricesFromIV_Update]
GO

CREATE PROC dbo.usp_AspEodStocksClosePricesFromIV_Update 
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	UPDATE ContractPrice  SET
		[PriceClose] = IV.Price 
  	FROM ContractPrice CP
		INNER join Contract C ON C.ContractID = CP.ContractID and C.contractTypeId in (1,2)
		INNER join IVData.IV.Dbo.FT_Stocks_EOD IV ON IV.symbol = C.symbol

	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodOptionsClosePricesFromIV_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodOptionsClosePricesFromIV_Update]
GO

CREATE PROC dbo.usp_AspEodOptionsClosePricesFromIV_Update 
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	UPDATE ContractPrice 
		SET PriceClose = (IV.Bid+IV.Ask)/2
	FROM  ContractPrice CP
		INNER join Contract C ON C.ContractID = CP.ContractID and C.contractTypeId = 3
		INNER join IVData.IV.Dbo.FT_Options_EOD IV on IV.option_ticker = C.symbol
	
	return @@error
GO
