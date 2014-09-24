/* 
	this patch fixes error message 'Stock with this ticker already exists!', 
	if you attempt to add new stock in case when option with this symbol 
	exists in DB.
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


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



Alter	proc usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(8) = Null,
		@vcContractName varchar(255) = Null,
		@iDefaultModelTypeID int = Null,
		@fYield float = Null,
		@bIsHTB bit = Null
   as

   declare @IndexID int
   


	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=1 and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			Raiserror ('Index with this ticker already exists!', 16, 1)
			Return (-1)
		end   



   if (@iIndexID is Null)  
	 begin
		 
		 begin tran

			 insert into Contract (contractTypeID, symbol, contractName)
				  values (1, @vcSymbol, @vcContractName)

			 select @IndexID = @@identity			 

			 if (@@error <> 0)
				  begin
					RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				  end

			 insert into [Index] (indexID, defaultModelTypeID, yield,IsHTB)
					 values (@IndexID, @iDefaultModelTypeID, @fYield,isNull(@bIsHTB,0))

			 if (@@error <> 0)
				begin
				   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
				   ROLLBACK TRAN
				   RETURN (-1)
				end 			

		 commit tran

	 end
   else
	 begin

		select @IndexID = 0

		begin tran

		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName)
				where contractID = @iIndexID

			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end

		  update [Index]
			   set	defaultModelTypeID = isNull(@iDefaultModelTypeID,defaultModelTypeID),
					yield = isNull(@fYield,yield),
					isHTB = isNull(@bIsHTB,isHTB)
			   where indexID = @iIndexID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   

		 commit tran
		
	 end		

		 if (@@error <> 0)
			return (-1)
		  else	
			return @IndexID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



