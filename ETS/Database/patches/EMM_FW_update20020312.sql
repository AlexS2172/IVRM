/*
	this script make the ability to save dividents information in EMM_FW Back Office
*/
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.04')
	begin
		update DataInfo set keyValue = '3.04' where DataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.04'
	end
GO



SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER      proc usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @idefaultModelTypeID int = Null,
        @idefaultSurfaceTypeID int = Null,
        @ibaseIndexID int,
        @vcSymbol varchar(8) = Null,
        @vcContractName varchar(255) = Null,
		@bIsHTB  bit = Null,
        @iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null
  as

	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @StockID int,
			@StockID_original int

	if @bIsHTB is null 
		set @bIsHTB=0
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=2 and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
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
                                        DivFreq, baseIndexID, isHTB,primaryExchangeID,DivAmt,DivDate)
                  values (@StockID, @idefaultModelTypeID, @idefaultSurfaceTypeID,
                                    @iDivFreq, @ibaseIndexID,@bIsHTB,@iexchangeID,@fDivAmt,@dtDivDate)
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
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate)
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         

           if ((@df = 0) and (@iDivFreq > 0))
             begin
               delete Dividend where stockID = @iStockID
	    	       if (@@error <> 0) begin rollback tran return(-1) end         
    	      end
			if @iDivFreq = 0 
				begin
					update Stock set divAmt = null, divDate = null where stockID = @iStockID
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

