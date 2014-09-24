/*
MLine00001559
*/

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.42.078' or (keyValue = '3.42.079')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.42.079 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.42.079' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Stock_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Stock_Del
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Index_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Index_Del
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE PROCEDURE dbo.usp_Stock_Del
        @iStockID int,
        @bisRemovePermanent bit = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from Trade(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Unable to delete the Stock. There is an open position on it', 16, 1)
		Return (-1)
	end 


	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iStockID

	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Stock. There are an open positions on its options', 16, 1)
			Return (-1)
		end 

	Begin Transaction StockDel
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		exec usp_TradeLog_Del null, @iStockID

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from NextDaySurface where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete VolaBands where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update IndexBeta set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update ContractExchangeLink set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end


        delete ContractPrice where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iStockID)
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		if (@bisRemovePermanent = 1) 
			begin
        		delete from ContractInGroup where contractID = @iStockID 
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
				delete from ContractExchangeLink where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete SyntheticUnderlyingParam where syntheticOptRootID in
					(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete SyntheticRootIndexBeta where syntheticOptRootID in
					(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete SyntheticOptionRoot where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iStockID)
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete IndexBeta where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

				delete OptionRoot where underlyingID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_Index_Del
		@iIndexID int
  as
/*
	Created by Zaur Nuraliev 
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	 if exists (select tradeID from Trade
					 where Trade.contractID = @iIndexID)
	 begin
	   RAISERROR ('Unable to delete the Index. There is an open position on it', 16, 1)
	   RETURN (-1)
	 end

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iIndexID

	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Index. There are an open positions on its options', 16, 1)
			Return (-1)
		end 

	Begin Transaction IndexDel

		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = 0
			end

		delete from IndexCorrelation where indexID1 = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID2 = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexDataHistory where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexDefinition set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 
			where contractID in 
				(select OptionID 
					from [Option] O 
							join OptionRoot ORt on O.optionRootID=ORt.optionRootID and ORt.underlyingID=@iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran IndexDel goto finish end
		
		set @error = @@error
	Commit Transaction IndexDel
finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.42.079')
	begin
		update DataInfo set keyValue = '3.42.079' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.42.079' '3.42.079 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
