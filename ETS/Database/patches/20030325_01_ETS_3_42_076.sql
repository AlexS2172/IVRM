/*
MLine00001365,MLine00001492,MLine00001502
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.42.075' or (keyValue = '3.42.076')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.42.076 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.42.076' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiredOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiredOptions_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractType_Change]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractType_Change]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE        procedure dbo.usp_ExpiredOptions_Del
	@iContractID int -- stock or index identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Create table #Options_for_delete(OptionID int) 

	Create table #O_with_trades(symbol varchar(20)) 


	--=============================================================
	-- select options for remove
	--=============================================================
	insert into #Options_for_delete
		select O.optionID
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where (E.expiryDate < GetDate() or (O.actionDate < UC.MSupdateDate))
					and O.isManualyEntered = 0
					and O.actionID < 3
					and O.optionID not in (select distinct ContractID from Trade)
					and UC.contractID = @iContractID

	--=============================================================
	-- select options, which have a trades
	--=============================================================
	insert into #O_with_trades
		select C.symbol
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C on O.optionID = C.contractID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where E.expiryDate < GetDate() 
					and O.isManualyEntered = 0
					and O.actionID < 3
					and O.optionID in (select distinct ContractID from Trade)
					and UC.contractID = @iContractID

	exec usp_Option_Del
		if @@error != 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

	select symbol as vcSymbol from #O_with_trades

set @error = 0

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


CREATE     PROCEDURE dbo.usp_ContractType_Change 
	@iContractID int
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractTypeID int 
		select @iContractTypeID = contractTypeID from Contract where contractID = @iContractID

	if @iContractTypeID > 2 
		begin
			Raiserror('Can''t perform changes with this contract!',16,1)
		end

	Begin Transaction t1
		if @iContractTypeID = 1 
		--change the contract type from index to stock
			begin
				delete from IndexCorrelation where indexID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexCorrelation where indexID2 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDataHistory where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDefinition where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexBeta where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from SyntheticRootIndexBeta where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from [Index] where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				insert into Stock(stockID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			end
		else if @iContractTypeID = 2 
		--change the contract type from stock to index
			begin
				delete from StockDataHistory where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID2  = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from IndexDefinition where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from Stock where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				insert into [Index](indexID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			end
			--change the contractType

			if @iContractTypeID = 1 
				set @iContractTypeID = 2 
			else 
				set @iContractTypeID = 1

			update Contract 
				set contractTypeID = @iContractTypeID 
				where contractID = @iContractID
			if @@error != 0 begin if @@TranCount != 0 Rollback Tran t1 goto finish end

	Commit Transaction t1

    set @error = 0
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


CREATE  PROCEDURE dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0,
	@bisRemoveFromContract bit = 1,
	@bisRaiseError bit = 1
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an open position on it.', 16, 1)
						end
					Return (-2)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end

	Begin Transaction DelOpt
		delete from TradeLog where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		--IndexDefinition by contractID
		update IndexBeta set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from DualQuadraticParams where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaBands where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticUnderlyingParam  set actionID=3 where syntheticOptRootID not in 
			(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticRootIndexBeta  set actionID=3 where syntheticOptRootID not in 
			(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update OptionRoot set actionID = 3 where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end


		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		if @bisRemovePermanent = 1
			begin
				--IndexBeta by contractID
				delete from IndexBeta where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				if @bisRemoveFromContract = 1 
					begin
						delete Contract where contractID in (select OptionID from #Options_for_delete)
							if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
					end

				delete from SyntheticUnderlyingParam where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticRootIndexBeta where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from OptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.42.076')
	begin
		update DataInfo set keyValue = '3.42.076' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.42.076' '3.42.076 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
