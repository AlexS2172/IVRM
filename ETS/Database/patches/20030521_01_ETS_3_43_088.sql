/*
	fix for MLine00001728
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.087' or (keyValue = '3.43.088')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.088 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.088' as  'Version in script' from DataInfo where dataInfoID = 1
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
	   WHERE  name = N'usp_Index_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Index_Del
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

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iIndexID or syntheticOptRootID in 
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

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Stock_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Stock_Del
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

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iStockID or syntheticOptRootID in 
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

				delete SyntheticUnderlyingParam where underlyingID = @iStockID or syntheticOptRootID in
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

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_SyntheticParams_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_SyntheticParams_Get
GO

CREATE PROCEDURE dbo.usp_SyntheticParams_Get 
	@iRootID int,
	@bIsWithPrice bit = 0,
	@bIsWithHV bit = 0
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from SyntheticUnderlyingParam table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @bIsWithPrice = 0
		select 	
			sor.optionRootID as iRootID,
			sup.underlyingID as iUnderlyingID, 
			sup.SyntheticCoeff as fWeight
		from SyntheticUnderlyingParamView as sup
			join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
			join ContractView as c on c.contractID = sup.underlyingID
		where sor.optionRootID = @iRootID
	else
		begin
		
		if	@bIsWithHV = 0
			-- use NULL exchange only
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				A.iContractID as iContractID,  
				A.iContractTypeID as iContractTypeID,
				A.vcSymbol as vcSymbol,
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				A.tiCalcOptionType as tiCalcOptionType,
				A.tiCalcModelType as tiCalcModelType,
				A.tiExpCalendarID as tiExpCalendarID,
				A.tiIsBasket as tiIsBasket
			from SyntheticUnderlyingParamView sup 
				join vALLStockEOD A	on A.iContractID = sup.underlyingID
				join ContractView as c on c.contractID = sup.underlyingID
				join SyntheticOptionRootView as sor	on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock)
				on A.iContractID = CP.contractID where CP.exchangeID is null and sor.optionRootID = @iRootID
		else
			begin
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				case when isDivCustom = 1 then divFreqCustom else S.divfreq end iDivFreq,
				case when isDivCustom = 1 then divAmtCustom else S.divAmt end fDivAmt,
				case when isDivCustom = 1 then divDateCustom else S.DivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.ExpCalendarID as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C on C.contractID = sup.underlyingID
				join StockView S on S.stockID = C.contractID
				join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock) on S.stockID = CP.contractID 
				left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory 
				join (select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
					on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
				on S2.stockID=S.stockID where CP.exchangeID is null and sor.optionRootID = @iRootID
			union
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				0 as iDivFreq,
				I.yield as fDivAmt,
				null as dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.ExpCalendarID as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C on C.contractID = sup.underlyingID
				join IndexView I on I.indexID = C.contractID
				join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock) on I.indexID = CP.contractID
				left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
					(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
					on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
				on S2.indexID=I.indexID where CP.exchangeID is null and sor.optionRootID = @iRootID 
			end
		end
	set @error = @@error
Return(@error)

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_ExpiredOptions_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_ExpiredOptions_Del
GO

CREATE procedure dbo.usp_ExpiredOptions_Del
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
			where ( E.expiryDate < GetDate() 
					or ( datediff(d, O.actionDate, UC.MSupdateDate) > 0
					and O.isManualyEntered = 0 ))
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
			where ( E.expiryDate < GetDate() 
					or ( datediff(d, O.actionDate, UC.MSupdateDate) > 0
					and O.isManualyEntered = 0 ))
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.088')
	begin
		update DataInfo set keyValue = '3.43.088' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.088' '3.43.088 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
