/*
EMM_FW BO Error which was fixed: 
	Option Data save error: 'PCAR' 'PAQFL' info: 
	INSERT statement conflicted with COLUMN FOREIGN KEY 
	constraint 'FK_Option_Expiry'. 
	The conflict occurred in database 'EMM_FW_Ravi', table 'Expiry', column 'expiryID'.
*/
set nocount on 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER          Procedure dbo.usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(8),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			Declare @ERR_MSG varchar(8000)
			set @ERR_MSG = 'Incorrect expiration! '  + '"' +  Convert(varchar(50),@dtExpiry) + '"' + ' for symbol=' + '"' + @vcSymbol + '"' +  ' UnderlyingSymbol=' + '"' + @vcUnderlyingSymbol + '"' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendar where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from Contract 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3
	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where underlyingContractID = @iUnderlyingContractID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- try to find Option with same ticker in DB
	--===================================================================			
	declare @iIncorrectOptionID int

	select @iIncorrectOptionID = contractID 
		from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
		where symbol = @vcSymbol and contractTypeID = 3 and 
			[Option].actionID < 3 and (@OptionID <> contractID) -- or @OptionID is null

	if @iIncorrectOptionID is not null
		begin
			-- mark as delete
			update [Option] set [Option].actionID = 3 
				where optionID = @iIncorrectOptionID and expiryID < @iLastExpiryID

			if @@rowcount = 0 
				begin
					--Raiserror ('Option with this symbol already exists!', 16, 1)
					--Return (-1)
					update [Option] set [Option].actionID = 3 
						where optionID = @iIncorrectOptionID
					update Contract set actionID = 3 
						where ContractID = @iIncorrectOptionID
				end   
			else
				update Contract set actionID = 3 
					where ContractID = @iIncorrectOptionID
		end

	--===================================================================			
	-- make the contract name
	--===================================================================			
--  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
--					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
--						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

  if (@OptionID is Null) 
	begin
		Begin Transaction
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest,lotSize)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest,@iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID,lotSize)
						values(@OptionID,null,@iLotSize)
					if (@@error <> 0) begin rollback tran return(-1) end           
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
		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest,
					lotSize = isnull(@iLotSize,lotSize)
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 

			update [Option]
				set 
				IsCall = isnull(@iIsCall,IsCall),
				strike = isnull(@fStrike,Strike),
				expiryID = isnull(@iExpiryID,expiryID)
			where optionID = @OptionID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
				  RETURN (-1)
			   end 

	end 
	 
-- return @OptionID 
    set @error = 0
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--============================================================================================================
--change DB version
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.26')
	begin
		update DataInfo set keyValue = '3.26' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.26'
	end

