/*
	this script make the ability to store lot size, recived from Price provider 
*/
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.01')
	begin
		update DataInfo set keyValue = '3.01' where DataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.01'
	end
GO

-- check exists of 'null-exchange records' in ContractPrice table
insert into ContractPrice (contractID,exchangeID)
	select contractID,null 
		from Contract 
			where not exists(select contractID 
					from ContractPrice 
					where contractID=Contract.contractID and ContractPrice.exchangeID is null)
	--and Contract.contractTypeID in (2,3)

--alter of SP's
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER     proc usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @idefaultModelTypeID int = Null,
        @idefaultSurfaceTypeID int = Null,
        @ibaseIndexID int,
        @vcSymbol varchar(8) = Null,
        @vcContractName varchar(255) = Null,
		@bIsHTB  bit = Null,
        @iDivFreq int = Null
  as

	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @StockID int

	if @bIsHTB is null 
		set @bIsHTB=0
   
	if exists (select * from Contract where symbol = @vcSymbol and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			Raiserror ('Stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iStockID is Null)  
     begin
         begin tran
            insert into Contract (contractTypeID, Symbol, ContractName)
                  values (2, @vcSymbol, @vcContractName)

            set @StockID = @@identity         

			if (@@error <> 0) begin rollback tran return(-1) end         
            insert into Stock (stockID, defaultModelTypeID, defaultSurfaceTypeID,
                                        DivFreq, baseIndexID, isHTB,primaryExchangeID)
                  values (@StockID, @idefaultModelTypeID, @idefaultSurfaceTypeID,
                                    @iDivFreq, @ibaseIndexID,@bIsHTB,@iexchangeID)
			if (@@error <> 0) begin rollback tran return(-1) end           

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID=@StockID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@StockID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
         commit tran
     end
   else
     begin
        declare @df int
	        --select @StockID = 0
        select @df = DivFreq from Stock where stockID = @iStockID        
        begin tran
           update Contract
                set symbol = isNull(@vcSymbol,symbol),
                    contractName = isNull(@vcContractName,contractName)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  defaultModelTypeID = isNull(@idefaultModelTypeID,defaultModelTypeID), 
	                    defaultSurfaceTypeID = isNull(@idefaultSurfaceTypeID,defaultSurfaceTypeID),
	                    divFreq = isNull(@iDivFreq,divFreq), 
	                    baseIndexID = isNull(@ibaseIndexID,baseIndexID),
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID)
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         
           if ((@df = 0) and (@iDivFreq > 0))
             begin
               delete Dividend where stockID = @iStockID
	    	       if (@@error <> 0) begin rollback tran return(-1) end         
    	      end
         commit tran
     end        

 if (@@error <> 0)
    return (-1)
  else  
    return @StockID





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER    proc usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceAsk float,
	@fPriceBid float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iExchangeID int = null,  --the ExchangeID special for save data into ContractPrice table
	@iLotSize int = null
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
		    set ContractName = @vcContractName
	 	where contractID = @iContractID
	
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		update ContractPrice 
			set priceAsk = @fPriceAsk,
			priceBid = @fPriceBid,
			PriceOpen = @fPriceOpen,
			PriceClose = @fPriceClose,
			PriceLow = @fPriceLow,
			PriceHigh = @fPriceHigh,
			Volume = @iVolume,
			lotSize = isnull(@iLotSize,lotSize)
		where contractID = @iContractID and exchangeID=@iExchangeID

		if (@@error <> 0)
			begin
				Raiserror ('Error. Can''t update Contract Price!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID,lotSize)
					values(@iContractID,null,@iLotSize)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 

		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0)
			begin 
				declare @df smallint
					update Stock 
						set divAmt=@fDivAmt,divDate=@dtDivDate 
					where stockID=@iContractID
	
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
	
				if not exists(select * from Dividend where stockID=@iContractID and divDate=@dtDivDate)
					insert into Dividend (stockID, DivAmt, DivDate)
						values (@iContractID, @fDivAmt, @dtDivDate)
				else
					update Dividend
						set DivAmt = isnull(@fDivAmt,DivAmt)
					where stockID=@iContractID and DivDate=@dtDivDate
			end

	Commit Transaction

    set @error = 0

finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER          Procedure usp_Option_Import
	@iUnderlyingContractID int,  --underlying contractID
	@vcSymbol varchar(8),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null, 
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
			@cn varchar(255)
			

	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
 
  if (@OptionID is Null) 
	select @OptionID = optionID
		from [Option] (nolock)
		where underlyingContractID = @iUnderlyingContractID and 
			isCall = @iIsCall and
			strike = @fStrike and
			month(expiry) = month(@dtExpiry) and
			year(expiry) = year(@dtExpiry)

	if exists (select * from Contract where symbol = @vcSymbol and contractTypeID=3 and actionID < 3 and (@OptionID <> contractID or @OptionID is null))
		begin
			Raiserror ('Option with this symbol already exists!', 16, 1)
			Return (-1)
		end   

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
	
			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiry)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @dtExpiry)
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

