/*

Fixed error while save Indexes uploading from IV

DB version 3.36.017
*/

set nocount on

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc dbo.usp_Index_Save
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
		--Michael Malyshkin
		if @iExpCalendarID is null or @iExpCalendarID = 0
			set @iExpCalendarID = 2
		--Michael Malyshkin
		 
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



--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.36.017')
	begin
		update DataInfo set keyValue = '3.36.017' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.36.017'
	end