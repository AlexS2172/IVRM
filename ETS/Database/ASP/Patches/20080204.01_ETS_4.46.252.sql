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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.251' or (keyValue = '4.46.252') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.252 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.252' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





ALTER  procedure dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@dtExpiryOV datetime = null,
		@dtTradeClose datetime = null,
		@tiIsManualyEntered tinyint /* now this flag if not 0, means that this option is flex */
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 
 	  
	   if len(@vcSymbol) < 3 and @iOptionID is null
	   begin
		 RAISERROR ('Error. Option symbol can''t be less three symbols!', 16, 1)
		 RETURN (-1)
	   end	 

	   if exists (select optionRootID from OptionRootView
		   where symbol = left(@vcSymbol,len(@vcSymbol)-2) and underlyingID != @iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	declare	@iOptionRootID int,
		@ContractName varchar(255) --,
		--@iExpiryID int,
		--@iExpCalID INT

	select @iOptionRootID = optionRootID from OptionRootView 
		where symbol = left(@vcSymbol,len(@vcSymbol)-2) and 
			underlyingID = @iUnderlyingContractID

	declare @ERR_MSG varchar(8000),
		@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol from ContractView 
		where contractID = @iUnderlyingContractID and contractTypeID <= 2

	--set @iExpiryID = cast (@dtExpiry As int) --(year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	

	execute usp_Option_Check @iOptionID = @iOptionID, 
					@vcSymbol = @vcSymbol,	
					@iOptionRootID = @iOptionRootID,
					@dtExpiry = @dtExpiry,
					--@iExpiryID = @iExpiryID,
					@fStrike = @fStrike,
					@iIsCall = @iIsCall,
					@vcUnderlyingSymbol = @vcUnderlyingSymbol,
					@ERR_MSG = @ERR_MSG output

		/*	select @vcSymbol '@vcSymbol',@iOptionRootID '@iOptionRootID',@dtExpiry '@dtExpiry',@iExpiryID '@iExpiryID',
				@fStrike '@fStrike',@iIsCall '@iIsCall',@vcUnderlyingSymbol '@vcUnderlyingSymbol',@ERR_MSG '@ERR_MSG'
			select @iOptionID = 0 */

	if isnull(@ERR_MSG,'')!= ''
		begin
			if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end
	IF @tiIsManualyEntered = 0 
   		set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
					' ' + cast(round(@fStrike, 2) as varchar)
	ELSE
		set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
					' ' + cast(round(@fStrike, 2) as varchar) + ' (OTC)'

	Begin Transaction

	   if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),100
				set @iOptionRootID = @@identity
			end 

	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
			values (3, @vcSymbol, @ContractName)
					
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradeClose)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			/* Change ExpiryOV for all Options with the same Root */
			update [Option]
				set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV) where optionRootID = @iOptionRootID and expiryDate = @dtExpiry
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract set symbol = isNull(@vcSymbol,symbol), 
				contractName = isNull(@ContractName,contractName)
			where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			declare @iOldOptionID as int
			select @iOldOptionID = optionRootID from [Option] 
				where optionID = @iOptionID

			update [Option]
				set optionRootID = @iOptionRootID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryDate = isNull(@dtExpiry, expiryDate),
					ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
					TradingClose = isnull(@dtTradeClose, TradingClose)
					--expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			/* Change ExpiryOV for all Options with the same Root */
			update [Option]
				set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV) where optionRootID = @iOptionRootID and expiryDate = @dtExpiry
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iOldOptionID is not null and @iOptionRootID is not null
			begin
			  update Trade set OptionRootId = @iOptionRootID where 	OptionRootId = @iOldOptionID
	  		  if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		 end		

		if @tiIsManualyEntered in (0,1) 
		begin
			update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end 



	Commit Transaction

    set @error = 0
finish:
if (@error != 0) return (@error) else return @iOptionID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





ALTER    PROC dbo.usp_BoFutureOption_Save
		@iFutureOptionID int out,
		@iFutureID int, -- should be specified for new future option (ignored for update)
		@vcFutureOptionSymbol varchar(20) = null,
		@vcFutureOptionName varchar(255) = null, -- calculated if not specified (for new option)
		@vcExportSymbol varchar(20) = null,
		@tiIsCall tinyint = null,
		@fStrike float = null,
		--@iExpiryID int = null, -- calculated by @dtExpiryDate if not specified
		@dtExpiryDate datetime = null, -- ignored if @iExpiryID is specified
		@dtExpiryOV datetime = null,
		@dtTradeClose datetime = null,
		@tiIsRaiseError tinyint = 1 
AS
	-- create/update future

	set nocount on
	
	declare @error int set @error = 0

	if @iFutureOptionID is null -- new option
	begin
		-- check new future option future ID
		if @iFutureID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Future for new future option is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		declare @vcFutureSymbol varchar(20)
		declare @iFutureContractTypeID int
		--M declare @iFutureExpCalendarID int

		-- find future name and contract type
		select @iFutureContractTypeID = contractTypeID,	@vcFutureSymbol = symbol --, @iFutureExpCalendarID = expCalendarID
		from ContractView where contractID = @iFutureID		

		-- check future existence
		if @iFutureContractTypeID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying contract for new future option is invalid!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future contract type
		if @iFutureContractTypeID <> 4
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying contract for new future option is not a future!', 16, 1)
			set @error = -3
			goto finish
		end

		-- check future option symbol
		set @vcFutureOptionSymbol = isnull(@vcFutureOptionSymbol, '')
		if len(ltrim(@vcFutureOptionSymbol)) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future option is not specified!', 16, 1)
			set @error = -4
			goto finish
		end

		-- check strike
		if isnull(@fStrike, 0) <= 0
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future option strike specified!', 16, 1)
			set @error = -5
			goto finish
		end

		-- round strike upto 3 decimals
		set @fStrike = round(@fStrike, 3)

		-- calculate expiry ID by expiry date if expiry ID is not specified
		--if @iExpiryID is null and @dtExpiryDate is not null
		--	set @iExpiryID = cast (@dtExpiryDate as int ) --(year(@dtExpiryDate) - 2000) * 12 + month(@dtExpiryDate)

		-- check expiry ID/Date
		-- select @dtExpiryDate = expiryDate
		-- from ExpiryInCalendarView
		-- where expiryID = @iExpiryID and expCalendarID = @iFutureExpCalendarID
		--if @iExpiryID is not null  
		--	set @dtExpiryDate = cast (@iExpiryID as DateTime)
		
		if @dtExpiryDate is null
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future option expiry specified!', 16, 1)
			set @error = -6
			goto finish
		end

		-- correct call/put
		set @tiIsCall = isnull(@tiIsCall, 0)
		if @tiIsCall <> 0
			set @tiIsCall = 1

		-- check option duplacates by symbol
		if exists(select 1 from ContractView where contractTypeID = 5 and symbol = @vcFutureOptionSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future option with symbol ''%s'' is already exists!', 16, 1, @vcFutureOptionSymbol)
			set @error = -7
			goto finish
		end

		-- check option duplacates by parameters
		if exists (select 1 from FutureOption
				where futureID = @iFutureID
					and datediff(d, expiryDate, @dtExpiryDate) = 0 and isCall = @tiIsCall
					and abs(strike - @fStrike) < 0.001) -- check equality upto 3 decimals
		begin
			if @tiIsRaiseError = 1 raiserror('Future option with the same parameters is already exists for specified future!', 16, 1)
			set @error = -8
			goto finish
		end

		-- make default future option contract name if not specified
		if @vcFutureOptionName is null
			set @vcFutureOptionName = @vcFutureSymbol +
						' ' + datename(month, @dtExpiryDate) + 
						' ' + datename(year, @dtExpiryDate) +
						' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
						' ' + cast(floor(@fStrike) as varchar)

		begin tran

		-- save new contract data
		insert into Contract
			(contractTypeID, symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
		values	(5, @vcFutureOptionSymbol, @vcFutureOptionName/*, null*/, null, null)
		if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

		set @iFutureOptionID = @@identity

		-- save new future option data
		insert into FutureOption
			(futureOptionID, futureID, isCall, expiryDate, ExpiryOV, TradingClose, strike)
		values	(@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @dtExpiryOV, @dtTradeClose, @fStrike)
		if @@error <> 0 begin set @error = -10 if @@trancount = 1 rollback tran goto finish end

		/* Change ExpiryOV for all Options with the same Root */
		update [FutureOption]
			set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV) where futureOptionID = @iFutureOptionID and expiryDate = @dtExpiryDate
		if @@error <> 0 begin set @error = -18 if @@trancount = 1 rollback tran goto finish end

		-- save price record for default exchange
		if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
		begin
			insert into ContractPrice
				(contractID, exchangeID)
			values	(@iFutureOptionID, null)
			if @@error <> 0 begin set @error = -11 if @@trancount = 1 rollback tran goto finish end
		end

		commit tran
	end
	else
	begin
		-- correct future option name (prevent updates to empty string)
		if len(ltrim(@vcFutureOptionSymbol)) = 0
			set @vcFutureOptionSymbol = null

		-- check future option name
		if @vcFutureOptionSymbol is not null
			if exists(select 1 from ContractView where contractTypeID = 5
						and contractID <> @iFutureOptionID and symbol = @vcFutureOptionSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future option with symbol ''%s'' is already exists!', 16, 1, @vcFutureOptionSymbol)
				set @error = -12
				goto finish
			end

		-- correct strike (prevent updates to zero)
		if @fStrike = 0
			set @fStrike = null

		-- check strike
		if @fStrike is not null
		begin
			if @fStrike <= 0
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future option strike specified!', 16, 1)
				set @error = -13
				goto finish
			end

			-- round strike upto 3 decimals
			set @fStrike = round(@fStrike, 3)
		end

		-- correct call/put
		if @tiIsCall is not null
			if @tiIsCall <> 0
				set @tiIsCall = 1

		-- correct expiry ID (prevent updates to zero)
		--if @iExpiryID = 0
		--	set @iExpiryID = null

		-- calculate expiry ID by expiry date if expiry ID is not specified
		--if @iExpiryID is null and @dtExpiryDate is not null
		--	set @iExpiryID = cast (@dtExpiryDate as int) --(year(@dtExpiryDate) - 2000) * 12 + month(@dtExpiryDate)


		if @tiIsCall is not null or @fStrike is not null or @dtExpiryDate is not null
		begin
			declare @tiIsCallToCheck tinyint
			declare @fStrikeToCheck float
			declare @dtExpiryDateToCheck DateTime
	
			-- prepare parameters to check option duplacates
			select @tiIsCallToCheck = isnull(@tiIsCall, isCall),
				@fStrikeToCheck = isnull(@fStrike, strike),
				@dtExpiryDateToCheck = isnull(@dtExpiryDate, expiryDate)
			from FutureOption
			where futureOptionID = @iFutureOptionID
	
			-- check option duplacates by parameters
			if exists (select 1 from FutureOption
					where futureOptionID <> @iFutureOptionID and futureID = @iFutureID
						and datediff(d, expiryDate, @dtExpiryDateToCheck) = 0 and isCall = @tiIsCallToCheck
						and abs(strike - @fStrikeToCheck) < 0.001) -- check equality upto 3 decimals
			begin
				if @tiIsRaiseError = 1 raiserror('Future option with the same parameters is already exists for specified future!', 16, 1)
				set @error = -15
				goto finish
			end
		end

		begin tran

		-- update contract data
		update Contract
		set symbol = isnull(@vcFutureOptionSymbol, symbol),
			contractName = isnull(@vcFutureOptionName, contractName)
		where contractID = @iFutureOptionID
		if @@error <> 0 begin set @error = -16 if @@trancount = 1 rollback tran goto finish end

		-- save future option data
		update FutureOption
		set isCall = isnull(@tiIsCall, isCall),
			expiryDate = isnull(@dtExpiryDate, expiryDate),
			ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
			TradingClose = isnull(@dtTradeClose, TradingClose),
			strike = isnull(@fStrike, strike)
		where futureOptionID = @iFutureOptionID
		if @@error <> 0 begin set @error = -17 if @@trancount = 1 rollback tran goto finish end

		/* Change ExpiryOV for all Options with the same Root */
		update [FutureOption]
			set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV) where futureOptionID = @iFutureOptionID and expiryDate = @dtExpiryDate
		if @@error <> 0 begin set @error = -18 if @@trancount = 1 rollback tran goto finish end
		commit tran
	end

finish:
if (@error <> 0) return (@error) else return @iFutureOptionID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.252')
	begin
		update DataInfo set keyValue = '4.46.252' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.252' '4.46.252 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


