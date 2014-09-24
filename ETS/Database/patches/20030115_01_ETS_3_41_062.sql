/*
patch for last release of ETS
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.40.061' or (keyValue = '3.41.062')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.41.062 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.41.062' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
-- add [volume10Day] column
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='volume10Day')
	begin
		Alter table ContractPrice
			Add [volume10Day] int null
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
-- add [volume5Expiration] column
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='volume5Expiration')
	begin
		Alter table ContractPrice
			Add [volume5Expiration] int null
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportStockTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportStockTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportStock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportStock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeedWithBookAndStrategy_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeedWithBookAndStrategy_New]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE	proc dbo.usp_ContractBySymbol_Get
		@vcSymbol varchar(20)
as
begin
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					1 as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.actionDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName,
					CP.volume10Day as iVolume10Day,
					CP.volume5Expiration as iVolume5Expiration
			from ContractView c 
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where C.symbol = @vcSymbol	
					and c.contractTypeID < 3
	union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					ort.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.actionDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName,
					CP.volume10Day as iVolume10Day,
					CP.volume5Expiration as iVolume5Expiration
			from ContractView c 
					join OptionView O on O.optionID = C.contractID
					join OptionRoot ort on ort.optionRootID = o.optionRootID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID				
		   where c.symbol = @vcSymbol  
					and c.contractTypeID > 2
end











GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO











CREATE      proc dbo.usp_Contract_Save
	@iContractID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSize int,	--currently not used!
	@fPriceBid float,
	@fPriceAsk float,
	@iExchangeID int,
	@fPriceLast float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iOpenInterest int,
	@iVolume10Day int = null,
	@iVolume5Expiration int = null

  as

-------------------------------------------------
set nocount on
-------------------------------------------------
  begin tran
	update Contract
		 set symbol = @vcSymbol,
			   contractName = CASE WHEN contractTypeID in (1, 2) THEN @vcContractName ELSE contractName END
	  where contractID = @iContractID
   if (@@error <> 0) begin rollback tran return @@error end  

	if exists(select * from ContractPrice where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID)	
		begin
			update ContractPrice 
				set --lotSize = @iLotSize,
					priceBid = @fPriceBid,
					priceAsk = @fPriceAsk,
					priceLast = @fPriceLast,
					priceOpen = @fPriceOpen,
					priceClose = @fPriceClose,
					priceLow = @fPriceLow,
					priceHigh = @fPriceHigh,
					volume = @iVolume,
					volume10Day = isnull(@iVolume10Day,volume10Day),
					volume5Expiration = isnull(@iVolume5Expiration,volume5Expiration)
				where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID
			   if (@@error <> 0) begin rollback tran return @@error end  
		end
	else 
		begin
			insert into ContractPrice (contractID,
					exchangeID,
					priceBid,
					priceAsk,
					priceLast,
					priceOpen,
					priceClose,
					priceLow,
					priceHigh,
					volume,
					volume10Day,
					volume5Expiration)
			values (@iContractID,
					@iExchangeID,
					@fPriceBid,
					@fPriceAsk,
					@fPriceLast,
					@fPriceOpen,
					@fPriceClose,
					@fPriceLow,
					@fPriceHigh,
					@iVolume,
					@iVolume10Day,
					@iVolume5Expiration)
		   if (@@error <> 0) begin rollback tran return @@error end  
		end

   if (@iOpenInterest is not Null)
	  begin
		declare @ctID int
		select @ctID = contractTypeID from Contract where contractID = @iContractID
		if (@ctID = 3)
		  begin
			update ContractPrice
				set openInterest = @iOpenInterest
				where contractID = @iContractID and isnull(exchangeID,0) = isnull(@iExchangeID,0)
			   if (@@error <> 0) begin rollback tran return @@error end  
		  end	
	  end
  commit tran

return @@error











GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


















CREATE                        Procedure dbo.usp_ReportStockTrade_Get
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
					@iBookID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iBookID output, @iStrategyID output
			if (@iTraderID<>0 or @iBookID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iBookID,@iStrategyID
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
		T.bookID as iBookID,
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO













CREATE   Proc dbo.usp_ReportStock_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	--=====================================================
	-- work with filter
	--=====================================================
	create table #FilteredStocks(stockID int)
              insert into #FilteredStocks 
                      exec usp_FilterData_Get @iFilterID
	--=====================================================
	-- data selection
	--=====================================================
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			case when isDivCustom = 1 then divFreqCustom else Stock.divfreq end iDivFreq,
			case when isDivCustom = 1 then divAmtCustom else Stock.divAmt end fDivAmt,
			case when isDivCustom = 1 then divDateCustom else Stock.DivDate end dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		Stock.isHTB as bIsHTB,
			Stock.isDivCustom as tiIsDivCustom,
			Stock.CalcOptionType as tiCalcOptionType,
			Stock.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID,
			null as tiIsBasket,
			Stock.Skew as fSkew,
			Stock.Kurt as fKurt,
			CP.volume10Day as iVolume10Day,
			CP.volume5Expiration as iVolume5Expiration
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join StockView Stock on Contract.contractID = Stock.StockID
					left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory join
									(select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
								on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
						on S2.stockID=Stock.stockID
	
			where (
				(@iStockID is null and (Stock.stockID in (select stockID from #FilteredStocks)))
				or 
				(@iStockID is not null and Contract.ContractID=@iStockID 
					and (Stock.stockID in (select stockID from #FilteredStocks) or @iFilterID is null))
				)
				and Contract.contractTypeID = 2
	union
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			0 as iDivFreq,
			I.yield as fDivAmt,
	  		null as dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		I.isHTB as bIsHTB,
			null as tiIsDivCustom,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID,
			I.IsBasket as tiIsBasket,
			I.Skew as fSkew,
			I.Kurt as fKurt,
			CP.volume10Day as iVolume10Day,
			CP.volume5Expiration as iVolume5Expiration
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join IndexView I on Contract.contractID = I.indexID
					left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
									(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
								on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
						on S2.indexID=I.indexID
			where (
				(@iStockID is null and (I.indexID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and @iFilterID is not null and (Contract.ContractID=@iStockID and (I.indexID in (select stockID from #FilteredStocks)))) or 
				(@iStockID is not null and I.indexID = @iStockID and @iFilterID is null)
				)
				and Contract.contractTypeID = 1
		order by Contract.symbol


	set @error = @@error
             drop table #FilteredStocks
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


















CREATE               Procedure dbo.usp_ReportTrade_Get
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
					@iBookID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iBookID output, @iStrategyID output
			if (@iTraderID<>0 or @iBookID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iBookID,@iStrategyID
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
		T.bookID as iBookID,
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE       PROCEDURE dbo.usp_TradeFeedWithBookAndStrategy_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(8),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(8) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@vcOptionRootSymbol varchar(20) = null,
	@vcBookName varchar(12) = null,
	@vcStrategyName varchar(12) = null	
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into Trade table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with same execID alreadty exists in DB!',16,1)
			Select null as iSeqNum
			Return(-1)

		end

	
	--check the option root symbol
	declare @iOptionRootID int,
		@iBookID int,
		@iStrategyID int
	
	set @iBookID = null
	set @iStrategyID = null

	Begin Transaction

	Declare @iTradeContractID int, @iStructureMissed int
	set @iStructureMissed = 0

	--check underlying symbol
	Declare @iUnderlyingContractID int

		if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
			set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID from ContractView where symbol = @vcUnderlyingSymbol and contractTypeID in(1,2)
	
	--select bookID
	if @vcBookName is not null
		begin
			select @iBookID = bookID 
				from BookView where bookName = @vcBookName

			if @iBookID is null
			begin
				exec @iBookID = usp_Book_Save null, @vcBookName, null 

				if @iBookID < 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		end
	
	--select strategyID
	if @vcStrategyName is not null
		begin
			select @iStrategyID = strategyID 
				from StrategyView where strategyName = @vcStrategyName

			if @iStrategyID is null
			begin
				exec @iStrategyID = usp_Strategy_Save null, @vcStrategyName, null
				if @iStrategyID < 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		end


	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iStructureMissed = 1

		end
	else
		begin
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			if @vcOptionRootSymbol is not null
			begin
				select @iOptionRootID = optionRootID 
					from OptionRootView where symbol = @vcOptionRootSymbol

				if @iOptionRootID is null
				begin
					insert into OptionRoot (underlyingID,symbol,lotSize)
						values (@iUnderlyingContractID,@vcOptionRootSymbol,isnull(@iTradeLotSize,100))

					set @iOptionRootID = @@identity
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			end

			select @iOptionID = O.optionID
				from OptionView O (nolock)
				where optionRootID = @iOptionRootID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID --and O.actionID<>3
	
			if @iOptionID is null
			--create new option
				begin
					if isnull(@vcContractSymbol, '') <> ''
					begin
						Declare @vcContractName varchar(255)
						  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
										' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
											 ' ' + cast(round(@fStrike, 2) as varchar)
		
						insert into Contract (contractTypeID, symbol, contractName) values (3, @vcContractSymbol,@vcContractName)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
							set @iOptionID = @@identity
				
						insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
							 values (@iOptionID, @iOptionRootID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end

					set @iStructureMissed = @iStructureMissed + 2
				end

			set @iTradeContractID = @iOptionID
		end
		else
			set @iTradeContractID = @iUnderlyingContractID


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym,@vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--create new trade

	Declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	Declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iNewTradeID is null 
			set @iNewTradeID = 1
	
	insert into Trade (
		execID,
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		bookID, 
		strategyID, 
		traderID, 
		isBuy, 
		isPosition,
		status,
		brokerCommissionID,
		clearingBrokerCommissionID,
		seqNum)
	values (
		@vcExecID,
		@iNewTradeID,
		1,
		@iTradeContractID, 
		0, 
		@dtTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iBookID, 
		@iStrategyID, 
		@iTraderID, 
		@tiIsBuy, 
		0,
		0,
		null,
		null,
		@iSeqNum)

		--set @iSeqNum = @@identity

		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		--write into TradeLog 
		if @iStructureMissed = 1
			begin
				insert into TradeLog (contractID, eventTypeID, seqNum) values (@iUnderlyingContractID, 2, @iSeqNum)
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
		else if @iStructureMissed = 2
			begin
				insert into TradeLog (contractID, eventTypeID, seqNum) values (@iOptionID, 3, @iSeqNum)
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
		else if @iStructureMissed = 3
			begin
				insert into TradeLog (contractID, eventTypeID, seqNum) values (@iOptionID, 3, @iSeqNum)
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

				insert into TradeLog (contractID, eventTypeID, seqNum) values (@iUnderlyingContractID, 2, @iSeqNum)
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end

	Commit Transaction

 --if all OK
	if @iContractTypeID < 3 
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			null as fPriceClose,
			null as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

	else if @iContractTypeID =3
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ort.lotSize as iLotSize,
			tr.status as tiStatus,
			CP.PriceClose as fPriceClose,
			UCP.priceClose as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			inner join [Option] O on C.contractID = O.optionID
			join OptionRoot ORt on ORt.optionRootID = O.optionRootID
			join Contract UC on ORt.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID

			inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			inner join ContractPrice UCP on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

    set @error = 0

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
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.41.062')
	begin
		update DataInfo set keyValue = '3.41.062' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.41.062' '3.41.062 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
