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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

GO


-- save Interest or Commodity/fx
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IC_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IC_Save]
GO
CREATE  PROCEDURE dbo.usp_IC_Save
		@iICID int,
		@vcSymbol varchar(20) = null,
		@vcName varchar(255) = null,
		@iICType int,		
		@tiGreeksCalcType tinyint = null,
		@tiIsActive tinyint = null,
		@tiCheckExistance tinyint = null,
		@iExpCalendarID int = null,
		@iUndPriceProfileID int = null output,
		@iOptPriceProfileID int = null output

   as
	---------------
	set nocount on
	---------------
--	if ((@iICType <> 6) And (@iICType <> 7 ))
--		begin
--			Raiserror ('Wrong Interest/Commodity type value', 16, 1)
--			Return (-1)
--		end

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID = @iICType and actionID < 3 and (@iICID <> contractID or @iICID is null))
		begin
			if isnull(@tiCheckExistance, 1) <> 0
				Raiserror ('Interest or commodity with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iICID is Null)  
	 begin
		 
		 begin tran
			if @iUndPriceProfileID is null
			begin
				select
					@iUndPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 3
					and isOptionProfile = 0
			end

			if @iOptPriceProfileID is null
			begin
				select
					@iOptPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 1
			end

            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                  values ( @iICType, @vcSymbol, @vcName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)

			 select @iICID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Commodity] (
							CommodityID, 
							isActive,
							GreeksCalculationType)
					 values (
							@iICID, 
							@tiIsActive,
							@tiGreeksCalcType)
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Commodity]!', 16, 1)
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
					contractName = isNull(@vcName,contractName),
					expCalendarID = isNull(@iExpCalendarID, expCalendarID),
					undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
					optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
				where contractID = @iICID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Commodity]
			   set	GreeksCalculationType = isnull(@tiGreeksCalcType, GreeksCalculationType),
					IsActive = isnull(@tiIsActive, IsActive)
			   where CommodityID = @iICID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Commodity]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   
		 commit tran
	 end

	--========================================================================
	-- make record with null exchange for this index
	--========================================================================
--	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iICID)
--		begin
--			insert into ContractPrice(contractID,exchangeID)
--				values(@iICID,null)
--			if (@@error <> 0) begin rollback tran return(-1) end           		
--		end		

if (@@error <> 0)
	return (-1)
else	
	return @iICID



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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.166')
	begin
		update DataInfo set keyValue = '4.49.166' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.166' '4.49.166 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
