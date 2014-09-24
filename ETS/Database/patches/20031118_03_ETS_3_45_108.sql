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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.107' or (keyValue = '3.45.108')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.108 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.108' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_ReportTrade_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ReportTrade_Get]
GO

CREATE PROC dbo.usp_ReportTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from OptionView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	create table #FilteredStocks(stockID int)
	
	-- for filtering by trades
	create table #T(tradeID int)
	if @iFilterID is not null
		begin
			declare @iTraderID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iStrategyID output
			if (@iTraderID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iStrategyID
				end
		end

	select 
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		case T.isBuy when 0 then T.quantity * -1 else T.quantity end as iQuantity,
		T.tradeDate as dtTradeDate,
		T.price as fPriceTrade,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceLast as fPriceLast,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceTheo as fPriceTheo,
		T.traderID as iTraderID,
		T.strategyID as iStrategyID,
		CP.priceClose as fPriceClose,
		Contract.symbol as vcSymbol,
		ORt.lotSize as iLotSize,
		EC.vegaWeight as fVegaWeight,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join TradeView T on O.optionID = T.contractID 
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
		where 
			((
				(@iStockID is not null and @iFilterID is null and ORt.underlyingID=@iStockID) 
				or (@iStockID is null and @iFilterID is null) 
				or
				(
					@iStockID is null and @iFilterID is not null 
					and (ORt.underlyingID in (select stockID from #FilteredStocks)) 
					and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)  
					or 	
					(
						@iStockID is not null and @iFilterID is not null 
						and ORt.underlyingID=@iStockID 
						and (ORt.underlyingID in (select stockID from #FilteredStocks)) 
						and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)
					)
				)
			))
	set @error = @@error
	if @iFilterID is not null
		begin
			drop table #FilteredStocks
		end
Return(@error)


GO

----------------------------------------------------------------------------------------------------
-- usp_ReportStockTrade_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportStockTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ReportStockTrade_Get]
GO
CREATE PROC dbo.usp_ReportStockTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from OptionView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	--=======================================================
	--prepare operations for work with filter
	--=======================================================
	create table #FilteredStocks(stockID int)
	-- for filtering by trades
	create table #T(tradeID int)
	if @iFilterID is not null
		begin
			declare @iTraderID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iStrategyID output
			if (@iTraderID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iStrategyID
				end
		end
	--=====================================================
	-- data selection
	--=====================================================
	select 
		Contract.contractID as iContractID,
		(case T.isBuy when 0 then T.quantity * -1 else T.quantity end) as iQuantity,
		T.tradeDate as dtTradeDate,
		T.price as fPriceTrade,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		T.traderID as iTraderID,
		T.strategyID as iStrategyID,
		CP.priceClose as fPriceClose
		from ContractView Contract
				join TradeView T on Contract.ContractID = T.contractID
				join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
		where 
			(
			(@iStockID is not null and @iFilterID is null and Contract.ContractID=@iStockID)
				or 
			(@iStockID is null and @iFilterID is null) 
				or
			(@iStockID is null and @iFilterID is not null and (Contract.ContractID in (select stockID from #FilteredStocks)) and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0))  
				or 
			(@iStockID is not null and @iFilterID is not null 
				and Contract.ContractID=@iStockID 
				and (Contract.ContractID in (select stockID from #FilteredStocks)) 
				and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)) 
			)
			and Contract.ContractTypeID < 3
	set @error = @@error
	if @iFilterID is not null
		begin
			drop table #FilteredStocks
		end
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_Filter_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Filter_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Filter_Save]
GO
CREATE PROC dbo.usp_Filter_Save 
	@iFilterID int = Null Output,		--the row identifier
	@vcFilterName varchar(80) = Null, 	--the filter name
	@iTraderID int = Null,  		
	@iStrategyID int = Null,  	
	@iContractGroupID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into Filter structure
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from Filter where filterName = @vcFilterName and (@iFilterID <> filterID or @iFilterID is null))
		begin
			Raiserror ('Filter with this name already exists!', 16, 1)
			Return (-1)
		end   
	Begin Transaction
		if not exists(select * from Filter where filterID = @iFilterID) 
			begin
				insert into Filter (filterName) 
					values (@vcFilterName)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iFilterID = @@identity 
			end
		else
			begin
				update Filter set 
					filterName = isnull(@vcFilterName, filterName)
					where filterID = @iFilterID
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	--==========================================================================
	--update filter content 
	--==========================================================================
--	if @iTraderID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 1)
				begin
					update FilterContent 
						set fieldValue = @iTraderID
						where filterID = @iFilterID and filterFieldID = 1
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,1,@iTraderID) 
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
--		end
--	if @iStrategyID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 3)
				begin
					update FilterContent 
						set fieldValue = @iStrategyID
						where filterID = @iFilterID and filterFieldID = 3
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,3,@iStrategyID) 
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
--		end
--	if @iContractGroupID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 4)
				begin
					update FilterContent 
						set fieldValue = @iContractGroupID
						where filterID = @iFilterID and filterFieldID = 4
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,4,@iContractGroupID) 
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
--		end
Commit Transaction
    set @error = 0
finish:
Return(@error)

GO


----------------------------------------------------------------------------------------------------
-- usp_FilterData_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_FilterData_Get]
GO
CREATE PROC dbo.usp_FilterData_Get
	@iFilterID int, --the row identifier
	@iTraderID int = null output,
	@iStrategyID int = null output
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return data from Filter structure
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcSelect1 varchar(8000)
	declare @vcSelect2 varchar(8000)
	declare @vcSelect3 varchar(8000)

	set @vcSelect1 = 'select distinct case isnull(ORt.underlyingID,0) when 0 then T.contractID else ORt.underlyingID end ''contractID'''
			+ ' from TradeView T left join OptionView O on T.contractID=O.optionID'
			+ ' left join OptionRoot ORt on ORt.optionRootID=O.optionRootID'

	set @vcSelect2 =''
	set @vcSelect3 = ' where 1=1'  
	declare @vcFiledValue varchar(50)
	-- TraderID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 1)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=1
			set @vcSelect3 = ' and T.traderID=' + @vcFiledValue
			select @iTraderID = convert(int,@vcFiledValue)
		end
	-- StrategyID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 3)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=3
			set @iStrategyID = convert(int,@vcFiledValue)
 			set @vcSelect3 = @vcSelect3 + ' and T.strategyID=' + @vcFiledValue
		end
	
	-- ContractGroupID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 4)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=4
			--set @vcSelect3 = @vcSelect3 + ' and T.contractID in (select contractID from ContractInGroupView where groupID=' + @vcFiledValue + ')'  
			set @vcSelect3 = @vcSelect3 + ' and case isnull(ORt.underlyingID,0) when 0 then T.contractID else ORt.underlyingID end 
				in (select contractID from ContractInGroupView where groupID=' + @vcFiledValue + ')'  
		end
	--select @vcSelect1+@vcSelect2+@vcSelect3
	execute (@vcSelect1 + @vcSelect2 + @vcSelect3)


	set @error = @@error
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_FilterTradeData_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterTradeData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_FilterTradeData_Get]
GO
CREATE PROC dbo.usp_FilterTradeData_Get
	@iTraderID int = null,
	@iStrategyID int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return data from Filter structure
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcSelect1 varchar(8000)
	declare @vcSelect2 varchar(8000)
	set @vcSelect1 = 'Select distinct tradeID from TradeView where '
	set @vcSelect2 =''
	-- TraderID
	if @iTraderID is not null
		begin
			set @vcSelect2 = 'traderID=' + Convert(varchar(15),@iTraderID)
		end
	else set @vcSelect2 = ''
	-- StrategyID
	if @iStrategyID is not null
		begin
			if @vcSelect2 = ''
				set @vcSelect2 = ' strategyID=' + Convert(varchar(15),@iStrategyID)
			else
				set @vcSelect2 = @vcSelect2 + ' and strategyID=' + Convert(varchar(15),@iStrategyID)
		end
	--else set @vcSelect2 = ''
	if @vcSelect2 =''
		set @vcSelect2=' 1=1'
	
	--====================================================================
	-- execute created script
	--====================================================================
	
	execute (@vcSelect1 + @vcSelect2)
	set @error = @@error
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- remove BookID from FilterField and FilterContent
----------------------------------------------------------------------------------------------------
delete FilterContent where filterFieldID = 2
GO

delete FilterField where filterFieldID = 2
GO

 
----------------------------------------------------------------------------------------------------
-- usp_BoFutureRoot_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Save]
GO
CREATE PROC dbo.usp_BoFutureRoot_Save
	@iFutureRootID int out,
	@iUnderlyingID int, -- should be specified for new future root (ignored for update)
	@vcFutureRootSymbol varchar(20) = null,
	@vcFutureRootName varchar(255) = null,
	@iFutureLotSize int = null,
	@iOptionLotSize int = null,
	@vcExportSymbol varchar(20) = null,
	@iMatCalendarID int = null,
	@tiIsRaiseError tinyint = 1 
AS
	-- create/update future root

	set nocount on
	
	declare @error int		set @error = 0

	if @iFutureRootID is null -- new future root
	begin
		-- check underlying ID
		if @iUnderlyingID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying for new future root is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		-- check future root symbol
		set @vcFutureRootSymbol = isnull(@vcFutureRootSymbol, '')
		if len(ltrim(@vcFutureRootSymbol)) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future root is not specified!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future root
		if exists(select 1 from FutureRoot where futureRootSymbol = @vcFutureRootSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
			set @error = -3
			goto finish
		end

		-- correct lot sizes
		if isnull(@iFutureLotSize, 0) <= 0
			set @iFutureLotSize = 100

		if isnull(@iOptionLotSize, 0) <= 0
			set @iOptionLotSize = 1

		-- correct maturity calendar ID
		if isnull(@iMatCalendarID, 0) = 0
			set @iMatCalendarID = 1

		-- check maturity calendar ID
		if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
			set @error = -4
			goto finish
		end

		begin tran

		-- insert new future root
		insert into FutureRoot
			(underlyingID, futureRootSymbol, futureRootName,
			futureLotSize, optionLotSize, exportSymbol, matCalendarID)
		values
			(@iUnderlyingID, @vcFutureRootSymbol, @vcFutureRootName,
			@iFutureLotSize, @iOptionLotSize, @vcExportSymbol, @iMatCalendarID)
		if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

		set @iFutureRootID = @@identity

		commit tran
	end
	else
	begin

		-- check future root
		if @vcFutureRootSymbol is not null
		begin
			if exists(select 1 from FutureRoot where futureRootID <> @iFutureRootID and futureRootSymbol = @vcFutureRootSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
				set @error = -6
				goto finish
			end
		end
		
		-- correct future root name (prevent updates to empty string)
		if len(ltrim(@vcFutureRootSymbol)) = 0
			set @vcFutureRootSymbol = null

		-- correct future lot size (prevent updates to invalid lot size)
		if @iFutureLotSize <= 0
			set @iFutureLotSize = null

		-- correct option lot size (prevent updates to invalid lot size)
		if @iOptionLotSize <= 0
			set @iOptionLotSize = null

		-- correct maturity calendar ID (prevent updates to zero calendar)
		if @iMatCalendarID = 0
			set @iMatCalendarID = null

		-- check maturity calendar
		if @iMatCalendarID is not null
		begin
			if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
				set @error = -7
				goto finish
			end
		end

		begin tran

		-- update future root
		update FutureRoot
		set futureRootSymbol = isnull(@vcFutureRootSymbol, futureRootSymbol),
			futureRootName = isnull(@vcFutureRootName, futureRootName),
			futureLotSize = isnull(@iFutureLotSize, futureLotSize),
			optionLotSize = isnull(@iOptionLotSize, optionLotSize),
			exportSymbol = isnull(@vcExportSymbol, exportSymbol),
			matCalendarID = isnull(@iMatCalendarID, matCalendarID)
		where
			futureRootID = @iFutureRootID
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end

finish:
	return @error
	
GO
 
----------------------------------------------------------------------------------------------------
-- usp_BoTrade_StoreToHistoryAndClear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrade_StoreToHistoryAndClear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrade_StoreToHistoryAndClear]
GO
CREATE PROC dbo.usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	-- clear simulated and manual deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0)
	if @@error !=0 begin set @error = -2 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (0, 3) and actionID = 0)
	if @@error !=0 begin set @error = -4 if @@TranCount = 1 Rollback Tran goto finish end

	if isnull(@tiStoreToHistory, 0) <> 0
	begin
		-- move actual and actual matched trades to history
		insert into TradeHistory
		select
			getdate() as dtActionDate,
	
			tr.tradeID as iTradeID, tr.tradeDate as dtTradeDate, tr.quantity as iQuantity,
			tr.price as fPrice, tr.isBuy as tiIsBuy, tr.status as tiStatus, tr.execID as vcExecID,
			tr.mark as iTradeMark,
			
			c.contractID as iContractID, c.contractTypeID as iContractTypeID, c.Symbol as vcSymbol,
			c.exportSymbol as vcExportSymbol, cp.priceClose as fPriceClose,
	
			uc.contractID as iUnderlyingContractID, uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol, uc.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			ucp.priceClose as fUnderlyingPriceClose,
	
			f.futureID as iFutureID, fr.futureRootSymbol as vcFutureRootSymbol,
			fc.symbol as vcFutureSymbol, fc.exportSymbol as vcFutureExportSymbol,
			fmc.expiryDate as dtFutureMaturity, fr.futureLotSize as iFutureLotSize,
			fcp.priceClose as fFuturePriceClose,
	
			ort.optionRootID as iOptionRootID, ort.symbol as vcOptionRootSymbol, ort.lotSize as iOptionRootLotSize,
	
			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtOptionExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fOptionStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiOptionIsCall,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference,

			fr.futureRootID as iFutureRootID,
			fr.optionLotSize as iFutureOptionLotSize
	
		from TradeView tr
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join StockView s on tr.underlyingID = s.stockID
			left join IndexView i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- additional
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
	
		set @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 if @@TranCount = 1 Rollback Tran goto finish end

		-- clear actual and actual matched trades
		delete from Trade 
			where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0
		if @@error !=0 begin set @error = -7 if @@TranCount = 1 Rollback Tran goto finish end
	end

	commit tran

finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuture_Import]
GO
CREATE PROCEDURE dbo.usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int

	--===================================================================			
	-- select the @iFutureID
	--===================================================================			
	select @iFutureID = futureID, @tiIsActive = isActive
			from Future (nolock)
			where futureRootID = @iFutureRootID and 
				maturityID = @iExpiryID

	if @iFutureID is not null
	begin
		Begin Transaction
			if exists(select 1 from ContractView where symbol=@vcSymbol and contractID<>@iFutureID and contractTypeID=4)
				begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID and actionID<>3
			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
			
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			if exists(select 1 from ContractView where symbol=@vcSymbol and contractTypeID=4)
				begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

			-- check underlying price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
			begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Contract
				(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			set @iFutureID = @@identity
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Future (futureID, futureRootID, maturityID)
				 values (@iFutureID, @iFutureRootID, @iExpiryID)
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iFutureID,null)
					if (@@error <> 0) begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -11 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 1

		Commit Transaction
	end 
	 
   set @error = @iFutureID
finish:
Return(@error)

GO
 
----------------------------------------------------------------------------------------------------
-- usp_EodFutureOption_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFutureOption_Import]
GO
CREATE PROCEDURE dbo.usp_EodFutureOption_Import
	@iFutureID int,
	@fStrike float,
	@iExpiryID int,
	@tiIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@vcBadFutureOptionName varchar(255) output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@iFutureOptionID int
	declare @iFutureOptionID_bad int

	--===================================================================			
	-- select the @iFutureOptionID
	--===================================================================			
	select @iFutureOptionID = futureOptionID
			from FutureOption (nolock)
			where futureID = @iFutureID and 
				isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID
				
	if not exists (select * from tempdb..sysobjects where id=object_id('tempdb..#FutureContractsForDelete'))
		Create table #FutureContractsForDelete(contractID int)				
	if @@error <> 0 begin set @error = -1 goto finish end
				
	if @iFutureOptionID is not null
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID from ContractView where symbol=@vcSymbol and contractID<>@iFutureOptionID and contractTypeID=5
			if @iFutureOptionID_bad is not null
			begin
										
				select @vcBadFutureOptionName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureOptionName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureOptionID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFutureOption_Del

				if @error <> 0
				begin 
					set @error = -5
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			end

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureOptionID and actionID<>3
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureOption set actionDate = getdate() where futureOptionID=@iFutureOptionID
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureOptionID_bad = contractID from ContractView where symbol=@vcSymbol and contractTypeID=5
			if @iFutureOptionID_bad is not null
			begin	
				select @vcBadFutureOptionName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureOptionID_bad
				
				if @vcBadFutureOptionName is not null							
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureOptionName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureOptionID_bad

				if @vcBadFutureOptionName is not null							
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureOptionID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFutureOption_Del

				if @error <> 0
				begin 
					set @error = -5
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			end
				

			insert into Contract (contractTypeID, symbol, contractName)
				 values (5, @vcSymbol,@vcContractName)
			set @iFutureOptionID = @@identity
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into FutureOption (futureOptionID, futureID, isCall, expiryID, strike)
				 values (@iFutureOptionID, @iFutureID, @tiIsCall, @iExpiryID, @fStrike)
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iFutureOptionID,null)
				if (@@error <> 0) begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

		Commit Transaction
	end 
	 
   set @error = @iFutureOptionID
finish:
Return(@error)

GO
  

----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuture_Import]
GO
CREATE PROCEDURE dbo.usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output,
	@vcBadFutureName varchar(255) output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
		
	declare @error int		set @error = 0
	declare	@iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int,
			@iFutureID_bad int

	if not exists (select * from tempdb..sysobjects where id=object_id('tempdb..#FutureContractsForDelete'))
		Create table #FutureContractsForDelete(contractID int)				
	if @@error <> 0 begin set @error = -1 goto finish end
	
	--===================================================================			
	-- select the @iFutureID
	--===================================================================			
	select @iFutureID = futureID, @tiIsActive = isActive
			from Future (nolock)
			where futureRootID = @iFutureRootID and 
				maturityID = @iExpiryID

	if @iFutureID is not null
	begin
		Begin Transaction
			select @iFutureID_bad = contractID from ContractView where symbol=@vcSymbol and contractID<>@iFutureID and contractTypeID=4
			if @iFutureID_bad is not null
			begin
				select @vcBadFutureName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							Trade t on t.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					
					
				select @vcBadFutureName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							[Order] ord on ord.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

				insert into #FutureContractsForDelete 
					select futureoptionID from FutureOption where futureID=@iFutureID_bad
				if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFuture_Del

				if @error <> 0
				begin 
					set @error = -6
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			
			end

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID and actionID<>3
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
			
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureID_bad = contractID from ContractView where symbol=@vcSymbol and contractTypeID=4
			if @iFutureID_bad is not null
			begin
				select @vcBadFutureName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							Trade t on t.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					
					
				select @vcBadFutureName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							[Order] ord on ord.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

				insert into #FutureContractsForDelete 
					select futureoptionID from FutureOption where futureID=@iFutureID_bad
				if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFuture_Del

				if @error <> 0
				begin 
					set @error = -6
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			
			end
			
			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

			-- check underlying price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
			begin set @error = -11 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Contract
				(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			set @iFutureID = @@identity
			if @@error <> 0 begin set @error = -12 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Future (futureID, futureRootID, maturityID)
				 values (@iFutureID, @iFutureRootID, @iExpiryID)
			if @@error <> 0 begin set @error = -13 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iFutureID,null)
					if (@@error <> 0) begin set @error = -14 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -15 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 1

		Commit Transaction
	end 
	 
   set @error = @iFutureID
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_BoFuture_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Import]
GO
CREATE PROCEDURE dbo.usp_BoFuture_Import
	@iFutureID int,
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@vcBadFutureName varchar(255) output	
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare @tiIsActive tinyint
	
	if @iFutureRootID is null
		select @iFutureRootID = futureRootID
			from Future (nolock)
			where futureID = @iFutureID
			
	exec @error = usp_EodFuture_Import @iFutureRootID,@iExpiryID,@vcSymbol,@vcContractName,@iLotSizeID,@iOptionLotSizeID, @tiIsActive output, @vcBadFutureName output
	 
finish:
Return(@error)

GO
----------------------------------------------------------------------------------------------------
-- usp_EodOption_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOption_Import]
GO
CREATE PROCEDURE dbo.usp_EodOption_Import
	@iRootID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iOptionID_bad int output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@OptionID int

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	if @OptionID is not null
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from ContractView where symbol = @vcSymbol and contractID <> @OptionID and contractTypeID=3
			if @iOptionID_bad is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0				
	
				if @error <> -1
				begin 
					if @error = 3 set @error = -4
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end
				else set @error = 0
			end
			
				
			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@OptionID and actionID<>3
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]  set isManualyEntered = 0 where optionID=@OptionID and actionID<>3
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end

		Commit Transaction
	end
	else
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from ContractView where symbol = @vcSymbol and contractTypeID=3 

			if @iOptionID_bad is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0				
	
				if @error <> -1
				begin 
					if @error = 3 set @error = -4
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end
				else set @error = 0
			end


			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@vcContractName)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

		Commit Transaction
	end 
	 
   set @error = @OptionID
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_BoFutureOptionPair_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOptionPair_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureOptionPair_Del]
GO
CREATE PROCEDURE dbo.usp_BoFutureOptionPair_Del
	@iOptionIDCall int,
	@iOptionIDPut int
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	Create table #FutureContractsForDelete(contractID int)
	if  @iOptionIDCall is not null
		begin
			insert into #FutureContractsForDelete values(@iOptionIDCall)
				if @@error > 0 begin set @error = -1 goto finish end
		end
	if  @iOptionIDPut is not null
		begin
			insert into #FutureContractsForDelete values(@iOptionIDPut)
			if @@error > 0 begin set @error = -1 goto finish end
		end
	if exists (select tradeID from Trade where contractID in (select contractID from #FutureContractsForDelete))
		begin
			Raiserror ('Unable to delete the future option. There is an open position on it.', 16, 1)
			Return (-2)
		end 
	if exists (select 1 from [Order] where contractID in (select contractID from #FutureContractsForDelete))
		begin
			Raiserror ('Unable to delete the future option. There is an order on it.', 16, 1)
			Return (-3)
		end 
	if object_ID('tempdb..#FutureContractsForDelete') is not null
		begin
			exec usp_EodFutureOption_Del
			if @@error <> 0 set @error = -4 goto finish
		end
	set @error = @@error
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_BoOptionWithPrice_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoOptionWithPrice_Get]
GO
CREATE proc dbo.usp_BoOptionWithPrice_Get
	  @iUnderlyingID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			o.optionRootID as iOptionRootID,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol
	from OptionRoot ort
			inner join OptionView o on o.optionRootID = ort.optionRootID
			inner join ContractView c on c.contractID = o.optionID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and uc.expCalendarID = ec.expCalendarID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
	where	ort.underlyingID = @iUnderlyingID
			and ec.expiryDate >= getdate()
	order by ort.optionRootID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
 
  return @@error

GO
  
----------------------------------------------------------------------------------------------------
-- usp_Trade_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_New]
GO
CREATE PROC dbo.usp_Trade_New
    @iContractID int,
    @iQuantity int,
    @fPrice float,
    @tiIsBuy tinyint,  
    @iTraderID int,
    @iBrokerID int = Null,
    @fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
    @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint,
	@iMark int = null,
    @iStrategyID int = null out,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
    @vcImportID varchar(20) = null out
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	-- determine strategy for trade
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
	if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	-- get new tradeID ane seqNum
	declare @iNewSeqNum int

	select 
		@iNewTradeID = isnull(max(tradeID), 0) + 1,
		@iNewSeqNum = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade
	insert into Trade (tradeID, actionID, contractID, tradeDate,  quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iNewSeqNum, @iMark,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	select @vcImportID = exportSymbol from ContractView where contractID = @iContractID

	if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @iStrategyID = null
		set @vcImportID = null
		return @error
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Update
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Update]
GO
CREATE PROC dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiIsBuy tinyint,
	@iTraderID int,
	@iBrokerID int = Null,
	@fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
	@fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime out,
	@tiStatus tinyint,
	@iMark int = null,
    @iNewTradeStrategyID int = Null out,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
    @vcImportID varchar(20) = Null out
as
	set nocount on

	declare @error int		set @error = 0

	declare @iNewSeqNum int,
			@vcExecID varchar(20)

	if @iOldTradeID is not null
	begin
		begin tran

		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
			select @vcExecID = execID from Trade where tradeID = @iOldTradeID

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
			from Trade
			where tradeID = @iOldTradeID

			if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
		end

		if @dtNewTradeDate is null	
		begin
			declare @iDeltaGMT int

			exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
	
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end
	
		select 
			@iNewTradeID = isnull(max(tradeID), 0) + 1,
			@iNewSeqNum = isnull(max(seqNum), 0) + 1
		from Trade
	
		-- determine strategy for trade
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iNewTradeStrategyID out
		if @@error <> 0 or @error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, seqNum, execID, mark,
			underlyingID, optionRootID, futureRootID, futureID)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iNewTradeStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@iNewSeqNum, @vcExecID, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		select @vcImportID = exportSymbol from ContractView where contractID = @iContractID
	
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

		commit tran
	end
	else
		set @error = -4

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @iNewTradeStrategyID = null
		set @vcImportID = null
		return @error
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_MmTradeByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmTradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmTradeByTrader_Get]
GO
CREATE PROC dbo.usp_MmTradeByTrader_Get
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error
GO
 
----------------------------------------------------------------------------------------------------
-- usp_MmTradeSeqByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmTradeSeqByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmTradeSeqByTrader_Get]
GO
CREATE PROC dbo.usp_MmTradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null
AS
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
		select
			tr.seqNum as iSeqNum,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.exportSymbol as vcImportID,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		order by tr.seqNum
	else
		select
			tr.seqNum as iSeqNum,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum
GO
 
----------------------------------------------------------------------------------------------------
-- usp_BoFutureOptionWithPrice_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureOptionWithPrice_Get]
GO
CREATE proc dbo.usp_BoFutureOptionWithPrice_Get
	  @iFutureID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			null as iOptionRootID,
			ec.expiryDate as dtExpiry,
			fo.strike as fStrike,
			fo.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol
	from FutureOption fo
			inner join ContractView c on c.contractID = fo.futureOptionID
			inner join ContractView fc on fc.contractID = fo.futureID
			inner join ExpiryInCalendarView ec on fo.expiryID = ec.expiryID and fc.expCalendarID = ec.expCalendarID
			left join ContractPrice cp on fo.futureOptionID = cp.contractID and cp.exchangeID is null
	where	fo.futureID = @iFutureID
			and ec.expiryDate >= getdate()
	order by year(EC.expiryDate), month(EC.expiryDate), fo.strike, fo.isCall desc

  return @@error

GO
/* RESULTS ANALYTHING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.108')
	begin
		update DataInfo set keyValue = '3.45.108' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.108' '3.45.108 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
