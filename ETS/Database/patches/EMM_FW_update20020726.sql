/*
fix for MLine00000594, MLine00000493

DB version 3.36.016
*/

set nocount on

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingActive_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingActive_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE      proc dbo.usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(20) = null,
		@vcContractName varchar(255) = null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsBasket tinyint = null,
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			Raiserror ('Index or stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		 
		 begin tran
			 insert into Contract (
							contractTypeID, 
							symbol, 
							contractName,
							expCalendarID)
				  values (
							1, 
							@vcSymbol, 
							@vcContractName,
							@iExpCalendarID)
			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (
							indexID, 
							yield,
							IsHTB,
							calcOptionType,
							calcModelType,
							isBasket, 
							isActive,
							skew,
							kurt)
					 values (
							@iIndexID, 
							@fYield,
							isNull(@bIsHTB,0),
							@tiCalcOptionType,
							@tiCalcModelType,
							isnull(@tiIsBasket,0), 
							@tiIsActive,
							@fSkew,
							@fKurt)
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
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@iExpCalendarID, expCalendarID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	yield = isnull(@fYield,yield),
					isHTB = isnull(@bIsHTB,isHTB),
					calcOptionType = isnull(@tiCalcOptionType, calcOptionType),
					calcModelType = isnull(@tiCalcModelType, calcModelType),
					isBasket = isnull(@tiIsBasket,isBasket),
					isActive = isnull(@tiIsActive, isActive),
					skew = isnull(@fSkew,Skew),
					kurt = isnull(@fKurt,Kurt)
			   where indexID = @iIndexID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   
		 commit tran
	 end

	--========================================================================
	-- make record with null exchange for this index
	--========================================================================
	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iIndexID)
		begin
			insert into ContractPrice(contractID,exchangeID)
				values(@iIndexID,null)
			if (@@error <> 0) begin rollback tran return(-1) end           		
		end		


if (@@error <> 0)
	return (-1)
else	
	return @iIndexID





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE       proc dbo.usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @vcSymbol varchar(20) = Null,
        @vcContractName varchar(255) = Null,
		@bIsHTB  bit = Null,
        @iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null,
		@tiIsDivCustom tinyint = 0,
		@fDivAmtCustom float = Null,
		@iDivFreqCustom int = Null,
		@dtDivDateCustom smalldatetime = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	if @iExpCalendarID is null or @iExpCalendarID = 0
		set @iExpCalendarID = 1
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			Raiserror ('Stock or index with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID)
                  values (2, @vcSymbol, @vcContractName, @iExpCalendarID)
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
                divFreq, 
				isHTB,
				primaryExchangeID,
				divAmt,
				divDate,
				isDivCustom,
				divAmtCustom,
				divFreqCustom,
				divDateCustom,
				calcOptionType,
				calcModelType,
				isActive,
				skew,
				kurt)
            values (
				@iStockID, 
                @iDivFreq, 
				@bIsHTB,
				@iExchangeID,
				@fDivAmt,
				@dtDivDate,
				@tiIsDivCustom,
				@fDivAmtCustom,
				@iDivFreqCustom,
				@dtDivDateCustom,
				@tiCalcOptionType,
				@tiCalcModelType,
				@tiIsActive,
				@fSkew,
				@fKurt)
			if (@@error <> 0) begin rollback tran return(-1) end           
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iStockID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iStockID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
         commit tran
     end
   else
     begin

        declare @df int

        select @df = DivFreq from Stock where stockID = @iStockID        
        begin tran
           update Contract
                set symbol = isNull(@vcSymbol,symbol),
                    contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@iExpCalendarID,expCalendarID)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  
	                    divFreq = isNull(@iDivFreq,divFreq), 
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType),
						isActive = isnull(@tiIsActive, isActive),
						skew = isNull(@fSkew,skew),
						kurt = isNull(@fKurt,kurt) 
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         

			if @iDivFreq = 0 
				begin
					update Stock set divAmt = null, divDate = null where stockID = @iStockID
				end

			if @iDivFreqCustom = 0 
				begin
					update Stock set divAmtCustom = null, divDateCustom = null where stockID = @iStockID
				end

         commit tran
     end        

 if (@@error <> 0)
    return (-1)
  else  
    return @iStockID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE   procedure dbo.usp_UnderlyingActive_Save
	@iContractID int = null,
	@tiIsActiveMode tinyint = 0 -- 0 - set all inactive, 1 - set all active, 2 - set active with position only,
AS
	set nocount on

	begin tran	

	set @tiIsActiveMode	= isnull(@tiIsActiveMode, 0)

	if @tiIsActiveMode = 0 or @tiIsActiveMode = 1
	begin
		update Stock
			set isActive = @tiIsActiveMode
		where 
			actionID < 3 and (@iContractID is null or stockID = @iContractID)
			and isnull(isActive, 0) <> @tiIsActiveMode
		if (@@error <> 0) begin rollback tran return(-1) end        

		update [Index]
			set isActive = @tiIsActiveMode
		where 
			actionID < 3 and (@iContractID is null or indexID = @iContractID)
			and isnull(isActive, 0) <> @tiIsActiveMode
		if (@@error <> 0) begin rollback tran return(-2) end        
	end
	else
	begin
/*		update Stock
			set isActive = 0
		where 
			actionID < 3 and (@iContractID is null or stockID = @iContractID)
			and isnull(isActive, 0) <> 0 
		if (@@error <> 0) begin rollback tran return(-3) end        

		update [Index]
			set isActive = 0
		where 
			actionID < 3 and (@iContractID is null or indexID = @iContractID)
			and isnull(isActive, 0) <> 0
		if (@@error <> 0) begin rollback tran return(-4) end        */

		update Stock
			set isActive = 1
		where 
			actionID < 3
			and isnull(isActive, 0) <> 1
			and stockID in
				(
						select 
							c.contractID
						from TradeView tr 
							inner join ContractView c on c.contractID = tr.contractID 
						where
							c.contractTypeID = 2
							and (@iContractID is null or c.contractID = @iContractID)
					union
						select 
							uc.contractID
						from TradeView tr 
							inner join OptionView o on o.optionID = tr.contractID 
							inner join ContractView uc on uc.contractID = o.underlyingContractID 
						where
							uc.contractTypeID = 2
							and (@iContractID is null or uc.contractID = @iContractID)
				)
		if (@@error <> 0) begin rollback tran return(-5) end        

		update [Index]
			set isActive = 1
		where 
			actionID < 3
			and isnull(isActive, 0) <> 1
			and indexID in
				(
						select 
							c.contractID
						from TradeView tr 
							inner join ContractView c on c.contractID = tr.contractID 
						where
							c.contractTypeID = 1
							and (@iContractID is null or c.contractID = @iContractID)
					union
						select 
							uc.contractID
						from TradeView tr 
							inner join OptionView o on o.optionID = tr.contractID 
							inner join ContractView uc on uc.contractID = o.underlyingContractID 
						where
							uc.contractTypeID = 1
							and (@iContractID is null or uc.contractID = @iContractID)
				)
		if (@@error <> 0) begin rollback tran return(-6) end        
	end

	commit tran
	Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.36.016')
	begin
		update DataInfo set keyValue = '3.36.016' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.36.016'
	end