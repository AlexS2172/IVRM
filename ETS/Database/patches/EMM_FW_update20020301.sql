/*
	update database (version 3.00) for ETSMM Bask Office 
	MLine00000174 Incorrect data  Last Div. Amount , after Import current symbol 
*/



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

	declare @StockID int,
			@StockID_original int

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
            set @StockID_original = @@identity            
			if (@@error <> 0) begin rollback tran return(-1) end         
            insert into Stock (stockID, defaultModelTypeID, defaultSurfaceTypeID,
                                        DivFreq, baseIndexID, isHTB,primaryExchangeID)
                  values (@StockID, @idefaultModelTypeID, @idefaultSurfaceTypeID,
                                    @iDivFreq, @ibaseIndexID,@bIsHTB,@iexchangeID)
			if (@@error <> 0) begin rollback tran return(-1) end           

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID=isnull(@StockID,@StockID_original))
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(isnull(@StockID,@StockID_original),null)
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


ALTER   proc usp_Stock_Import
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
	  @iExchangeID int = Null  --the ExchangeID special for save data into ContractPrice table
   
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
			Volume = @iVolume
		where contractID = @iContractID and exchangeID=@iExchangeID

		if (@@error <> 0)
			begin
				Raiserror ('Error. Can''t update Contract Price!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
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

/*	if (@bIsStock > 0)
	    begin 
			declare @df smallint
			select @df = DivFreq from Stock where stockID = @iContractID
			if (@df is not Null)
				begin
					if ((@df = 0 and @iDivFreq > 0) or (@df <> 0))
					begin
						delete Dividend where stockID = @iContractID
							if (@@error <> 0) begin rollback tran return(-1) end
						if (@df <> @iDivFreq)
						begin
							update Stock
								set DivFreq = @iDivFreq
							where stockID = @iContractID
							if (@@error <> 0) begin rollback tran return(-1) end
						end
						insert into Dividend (stockID, DivAmt, DivDate)
							values (@iContractID, @fDivAmt, @dtDivDate)
						if (@@error <> 0) begin rollback tran return(-1) end
					end
				end
			if @dtDivDate>=(select max(DivDate) from Dividend where stockID=@iContractID)
				begin
					update Stock 
						set divAmt=@fDivAmt,divDate=@dtDivDate 
					where stockID=@iContractID
				end
	      end */
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

