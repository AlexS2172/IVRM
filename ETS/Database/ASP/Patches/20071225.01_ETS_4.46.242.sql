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
/* Adding [expiryOV] and [TradingClose] fields into the FutureOption and Option. SP are changed accordingly. */
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.241'  or (keyValue = '3.46.242') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.242 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.242' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
/* ---------------------------- */

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[FutureOption]') and (name = 'expiryOV' or name = 'TradingClose'))
begin
	ALTER TABLE [FutureOption] 
		ADD [expiryOV] [datetime] NOT NULL DEFAULT ('2005-01-01 16:15'),
		    [TradingClose] [datetime] NOT NULL DEFAULT ('2005-01-01 16:30')
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[EodFutureOption]') and (name = 'expiryOV' or name = 'TradingClose'))
begin
	ALTER TABLE [EodFutureOption] 
		ADD [expiryOV] [datetime] NOT NULL DEFAULT ('2005-01-01 16:15'),
		    [TradingClose] [datetime] NOT NULL DEFAULT ('2005-01-01 16:30')
end
GO

UPDATE [FutureOption] set expiryOV = expiryDate - 1
GO

UPDATE [EodFutureOption] set expiryOV = expiryDate - 1
GO


ALTER proc dbo.usp_BoFutureOptionWithPrice_Get
	  @iFutureID int		 
  as
	select	c.contractID as iContractID, c.symbol as vcSymbol, c.contractName as vcContractName,
		null as iOptionRootID,
		fo.expiryDate as dtExpiry, fo.expiryOV as dtExpiryOV, fo.TradingClose as dtTradeClose,
		fo.strike as fStrike, fo.isCall as bIsCall,
		cp.priceClose as fPriceClose, cp.priceTheo as fPriceTheo, c.exportSymbol as vcExportSymbol
	from FutureOption fo
		inner join ContractView c on c.contractID = fo.futureOptionID
		inner join ContractView fc on fc.contractID = fo.futureID
		--inner join ExpiryInCalendarView ec on fo.expiryID = ec.expiryID and fc.expCalendarID = ec.expCalendarID
		left join ContractPrice cp on fo.futureOptionID = cp.contractID and cp.exchangeID is null
	where	fo.futureID = @iFutureID and datediff(d, GetDate(), fo.expiryDate) >= 0
	order by fo.expiryDate, fo.strike, fo.isCall desc

  return @@error

GO

GO


ALTER   PROC dbo.usp_BoFutureOption_Save
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

GO

/* ---------------------------- */

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[Option]') and (name = 'expiryOV' or name = 'TradingClose'))
begin
	ALTER TABLE [Option]
		ADD [expiryOV] [datetime] NOT NULL DEFAULT ('2005-01-01 16:15'),
		    [TradingClose] [datetime] NOT NULL DEFAULT ('2005-01-01 16:30')
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[EodOption]') and (name = 'expiryOV' or name = 'TradingClose'))
begin
	ALTER TABLE [EodOption] 
		ADD [expiryOV] [datetime] NOT NULL DEFAULT ('2005-01-01 16:15'),
		    [TradingClose] [datetime] NOT NULL DEFAULT ('2005-01-01 16:30')
end
GO

UPDATE [Option] set expiryOV = expiryDate - 1
GO

UPDATE [EodOption] set expiryOV = expiryDate - 1
GO


ALTER View dbo.OptionView
as
	Select  optionID,
		optionRootID,
		isCall,
		strike,
		expiryDate,
		actionID,
		actionDate,
		isManualyEntered,
		expiryOV,
		TradingClose
	from [Option] where actionID < 3
GO


ALTER proc dbo.usp_BoOptionWithPrice_Get
	  @iUnderlyingID int		 
  as
	select	c.contractID as iContractID, c.symbol as vcSymbol, c.contractName as vcContractName,
		o.optionRootID as iOptionRootID,
		o.expiryDate as dtExpiry, o.expiryOV as dtExpiryOV, o.TradingClose as dtTradeClose,
		o.strike as fStrike, o.isCall as bIsCall,
		cp.priceClose as fPriceClose, cp.priceTheo as fPriceTheo,
		c.exportSymbol as vcExportSymbol,
		o.IsManualyEntered as vcIsFlex
	from OptionRoot ort
		inner join [Option] o on o.optionRootID = ort.optionRootID
		inner join ContractView c on c.contractID = o.optionID
		left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
	where	ort.underlyingID = @iUnderlyingID and datediff(d, GetDate(), o.expiryDate) >= 0
	order by ort.optionRootID, o.expiryDate, o.strike, o.isCall desc
 
  return @@error

GO

GO


ALTER procedure dbo.usp_Option_Save
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

GO


/* ---------------------------- */

ALTER PROC dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
	@iFutureID int,
	@dtExpiryDate datetime,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@fPriceClose float,
	@dtExpiryOV datetime = null,
	@dtTradeClose datetime = null,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	declare	@iFutureOptionID int,
		@iFutureOptionID_bad int

	select @iFutureOptionID = futureOptionID
	from FutureOption with(nolock, index = IX_FutureOption_futureID_isCall_strike_expiryID)
	where futureID = @iFutureID
		and isCall = @iIsCall
		and abs(strike - @fStrike) < 0.009
		and abs(datediff( d, expiryDate, @dtExpiryDate ))<10

	if @iFutureOptionID is not null
	begin
		select @iFutureOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID) --IX_Contract_symbol_contractID_contractTypeID_actionID)
		where symbol = @vcSymbol
			--and contractID != @iFutureOptionID
			and contractTypeID = 5
			and actionID < 3

		if @iFutureOptionID_bad is not null and @iFutureOptionID_bad != @iFutureOptionID
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFutureOption
							@iFutureOptionID = @iFutureOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to delete existing future option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -1
				goto finish
			end
		end
		
		update Contract set
			symbol = @vcSymbol,
			contractName = @vcContractName
		where contractID = @iFutureOptionID
			and actionID < 3

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to update option contract info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -2
			goto finish
		end

		update FutureOption set
			actionDate = getdate(),
			expiryDate = @dtExpiryDate
			/*, ExpiryOV = @dtExpiryOV,
			TradingClose = @dtTradeClose -- should be expiration date filter also I guess */
		where futureOptionID = @iFutureOptionID

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to update option info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -3
			goto finish
		end
	end
	else
	begin
		select @iFutureOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
		where symbol = @vcSymbol
			and contractTypeID = 5
			and actionID < 3

		if @iFutureOptionID_bad is not null
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFutureOption
							@iFutureOptionID = @iFutureOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to delete existing option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -4
				goto finish
			end
		end

		insert into Contract
			(contractTypeID, symbol, contractName)
		values	(5, @vcSymbol, @vcContractName)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to insert new option contract', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -5
			goto finish
		end

		set @iFutureOptionID = @@identity

		insert into [FutureOption]
			(futureOptionID, futureID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
		values	(@iFutureOptionID, @iFutureID, @iIsCall, @fStrike, @dtExpiryDate, @dtExpiryOV, @dtTradeClose)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to insert new option', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -6
			goto finish
		end
	end 
	 
	if not exists(select 1 from ContractPrice (nolock) where contractID = @iFutureOptionID and exchangeID is null)
	begin
		insert into ContractPrice
			(contractID, exchangeID, priceClose)
		values (@iFutureOptionID, null, @fPriceClose)
	end 
	else
	begin
		update ContractPrice
		set priceClose = @fPriceClose
		where contractID = @iFutureOptionID
			and exchangeID is null
	end

	if @@error != 0 or @error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to update option close price', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -7
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName

finish:
	if @error = 0
		return @iFutureOptionID
	else
		return @error

GO

GO

/* ---------------------------- */

ALTER PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	update FutureRoot
	set futureLotSize = eod_frm.eodFutureLotSize,
		optionLotSize = eod_frm.eodOptionLotSize
	from FutureRoot f
		inner join EodFutureRootMap eod_frm on eod_frm.futureRootID = f.futureRootID

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future options: Fail to update future root info', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -1
		goto finish
	end

	truncate table EodFuture

	insert EodFuture (eodFutureID, futureRootID, symbol, contractName, maturityDate, priceClose)
	select
		eod_f.futureID,
		eod_frm.futureRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_f.maturityDate,
		eod_cp.priceClose
	from
		ETS_ASP_EOD.dbo.Future eod_f
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_f.futureID
		inner join EodFutureRootMap eod_frm
				on eod_frm.eodFutureRootID = eod_f.futureRootID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	truncate table EodFutureOption
	
	insert EodFutureOption (eodFutureID, symbol, contractName, expiryDate, strike, isCall, priceClose, ExpiryOV, TradingClose)
	select	eod_fo.futureID,
		eod_c.symbol,
		eod_c.contractName,
		eod_fo.expiryDate,
		eod_fo.strike,
		eod_fo.isCall,
		eod_cp.priceClose,
		eod_fo.ExpiryOV,
		eod_fo.TradingClose
	from ETS_ASP_EOD.dbo.FutureOption eod_fo
		inner join ETS_ASP_EOD.dbo.Contract eod_c 
			on eod_c.contractID = eod_fo.futureOptionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodFutureID int,
		@futureRootID int,
		@symbol varchar(20),
		@maturityDate datetime,
		@expiryDate datetime,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@futureOptionID int,
		@dtOVExpiry datetime,
		@dtCloseTrade datetime,
		@futureID int

	declare cur_Future cursor local fast_forward read_only
	for select eodFutureID, futureRootID, symbol, contractName, maturityDate, priceClose
		from EodFuture (nolock)

	open cur_Future
	
	fetch next from cur_Future
	into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityDate, @priceClose

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
			exec @futureID = dbo.usp_AspEodUpdateMarketStructure_ImportFuture
							@iFutureRootID = @futureRootID,
							@vcSymbol = @symbol,
							@vcContractName = @contractName,
							@dtMaturityDate = @maturityDate,
							@fPriceClose = @priceClose
	
			if @@error = 0 and isnull(@futureID, 0) > 0
			begin
				declare cur_FutureOption cursor local fast_forward read_only
				for select symbol, contractName, expiryDate, strike, isCall, priceClose, ExpiryOV, TradingClose
					from EodFutureOption with (nolock, index = IX_EodFutureOption_eodFutureID)
					where eodFutureID = @eodFutureID

				open cur_FutureOption
	
				fetch next from cur_FutureOption
				into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose, @dtOVExpiry, @dtCloseTrade
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin		
						exec @futureOptionID = dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
											@iFutureID = @futureID,
											@dtExpiryDate = @expiryDate,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose,
											@dtExpiryOV = @dtOVExpiry,
											@dtTradeClose = @dtCloseTrade
						fetch next from cur_FutureOption
						into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose, @dtOVExpiry, @dtCloseTrade
					end
				end
	
				close cur_FutureOption
				deallocate cur_FutureOption
			end
		end

		fetch next from cur_Future
		into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityDate, @priceClose
	end

	close cur_Future
	deallocate cur_Future

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error

GO

GO


/* ---------------------------- */

ALTER PROC dbo.usp_AspEodUpdateMarketStructure_ImportOption
	@iRootID int,
	@dtExpiryDate datetime,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@fPriceClose float,
	@dtExpiryOV datetime = null,
	@dtTradeClose datetime = null,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	declare	@iOptionID int,
		@iOptionID_bad int

	select @iOptionID = optionID
	from [Option] with(nolock, index = IX_Option_optionRootID_isCall_strike_expiryID_actionID)
	where optionRootID = @iRootID
		and isCall = @iIsCall
		and abs(strike - @fStrike) < 0.009
		and abs(datediff(d, expiryDate, @dtExpiryDate))<10 
		and actionID < 3

	if @iOptionID is not null
	begin
		select @iOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID) --IX_Contract_symbol_contractID_contractTypeID_actionID)
		where symbol = @vcSymbol
			--and contractID != @iOptionID
			and contractTypeID = 3
			and actionID < 3

		if @iOptionID_bad is not null and @iOptionID_bad != @iOptionID
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteOption
							@iOptionID = @iOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to delete existing option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -1
				goto finish
			end
		end
		
		update Contract set
			symbol = @vcSymbol,
			contractName = @vcContractName
		where contractID = @iOptionID
			and actionID < 3

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option contract info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -2
			goto finish
		end

		update [Option]
		set isManualyEntered = 0, expiryDate = @dtExpiryDate /* is ExpiryOV field update required here? */
		where optionID = @iOptionID
			and actionID < 3

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -3
			goto finish
		end
	end
	else
	begin
		select @iOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
		where symbol = @vcSymbol
			and contractTypeID = 3
			and actionID < 3

		if @iOptionID_bad is not null
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteOption
							@iOptionID = @iOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to delete existing option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -4
				goto finish
			end
		end

		insert into Contract
			(contractTypeID, symbol, contractName)
		values	(3, @vcSymbol, @vcContractName)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to insert new option contract', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -5
			goto finish
		end

		set @iOptionID = @@identity

		insert into [Option]
			(optionID, optionRootID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
		values	(@iOptionID, @iRootID, @iIsCall, @fStrike, @dtExpiryDate, @dtExpiryOV, @dtTradeClose)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to insert new option', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -6
			goto finish
		end
	end 
	 
	if not exists(select 1 from ContractPrice (nolock) where contractID = @iOptionID and exchangeID is null)
	begin
		insert into ContractPrice
			(contractID, exchangeID, priceClose)
		values (@iOptionID, null, @fPriceClose)
	end 
	else
	begin
		update ContractPrice
		set priceClose = @fPriceClose
		where contractID = @iOptionID
			and exchangeID is null
	end

	if @@error != 0 or @error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option close price', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -7
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName

finish:
	if @error = 0
		return @iOptionID
	else
		return @error

GO

GO


/* ---------------------------- */

ALTER PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	truncate table EodOptionRoot

	insert EodOptionRoot (eodOptionRootID, underlyingID, symbol, lotSize)
	select
		eod_ort.optionRootID,
		eod_um.contractID,
		eod_ort.symbol,
		eod_ort.lotSize
	from ETS_ASP_EOD.dbo.OptionRoot eod_ort
		inner join EodUnderlyingMap eod_um
				on eod_ort.underlyingID = eod_um.eodContractID


	truncate table EodOption
	
	insert EodOption (eodOptionRootID, symbol, contractName, expiryDate, strike, isCall, priceClose, ExpiryOV, TradingClose)
	select	eod_o.optionRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_o.expiryDate,
		eod_o.strike,
		eod_o.isCall,
		eod_cp.priceClose,
		eod_o.ExpiryOV,
		eod_o.TradingClose
	from	ETS_ASP_EOD.dbo.[Option] eod_o
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_o.optionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodOptionRootID int,
		@underlyingID int,
		@symbol varchar(20),
		@lotSize int,
		@expiryDate datetime,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@optionID int,
		@dtOVExpiry datetime,
		@dtCloseTrade datetime,
		@optionRootID int

	declare cur_OptionRoot cursor local fast_forward read_only
	for select eodOptionRootID, underlyingID, symbol, lotSize
		from EodOptionRoot (nolock)

	open cur_OptionRoot
	
	fetch next from cur_OptionRoot
	into @eodOptionRootID, @underlyingID, @symbol, @lotSize

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
	
			exec @optionRootID = dbo.usp_AspEodUpdateMarketStructure_ImportOptionRoot
							@iUnderlyingID = @underlyingID, @vcSymbol = @symbol, @iLotSize = @lotSize
	
			if @@error = 0 and isnull(@optionRootID, 0) > 0
			begin
				declare cur_Option cursor local fast_forward read_only
				for select symbol, contractName, expiryDate, strike, isCall, priceClose, ExpiryOV, TradingClose
					from EodOption with (nolock, index = IX_EodOption_eodOptionRootID)
					where eodOptionRootID = @eodOptionRootID

				open cur_Option
	
				fetch next from cur_Option
				into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose, @dtOVExpiry, @dtCloseTrade
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
						exec @optionID = dbo.usp_AspEodUpdateMarketStructure_ImportOption
											@iRootID = @optionRootID,
											@dtExpiryDate = @expiryDate,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose,
											@dtExpiryOV = @dtOVExpiry,
											@dtTradeClose = @dtCloseTrade
						fetch next from cur_Option
						into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose, @dtOVExpiry, @dtCloseTrade
					end
				end
	
				close cur_Option
				deallocate cur_Option
			end
		end

		fetch next from cur_OptionRoot
		into @eodOptionRootID, @underlyingID, @symbol, @lotSize
	end

	close cur_OptionRoot
	deallocate cur_OptionRoot

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error


GO

GO

/* ---------------------------- */

if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.242')
	begin
		update DataInfo set keyValue = '3.46.242' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.242' '3.46.242 update message:' 
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