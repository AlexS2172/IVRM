/*
	this script set up 3.08 version of DB
*/
--==================================================================
--created dropped relations at the [Option] table
--==================================================================
if object_id('FK_Option_Contract') is null
	ALTER TABLE [dbo].[Option] ADD CONSTRAINT [FK_Option_Contract] FOREIGN KEY (optionID)  REFERENCES [dbo].[Contract] (contractID) 
GO
if object_id('FK_Option_Contract1') is null
	ALTER TABLE [dbo].[Option] ADD CONSTRAINT [FK_Option_Contract1] FOREIGN KEY (underlyingContractID)  REFERENCES [dbo].[Contract] (contractID) 
GO
--==================================================================
--add 'lotSize' field in Trade table
--==================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='lotSize')
	begin
		Alter Table Trade
			Add lotSize int not null default(1)
	end
GO
update Trade set lotSize = 100 
	from Trade
			join Contract on Trade.contractID = Contract.contractID and contractTypeID = 3
GO
update Trade set lotSize = 1 
	from Trade
			join Contract on Trade.contractID = Contract.contractID and contractTypeID < 3
GO
--=================================================================================
-- put priceTheo data to ContractPrice table and drop ContractPriceHistory table
--=================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='priceTheo')
	begin
		Alter Table ContractPrice 
			Add priceTheo float null
	end
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractPriceHistory]') and OBJECTPROPERTY(id, N'IsTable') = 1)
	begin
		Update ContractPrice 
			set priceTheo=A.priceTheo
		from ContractPrice
			join (select CPH.contractID,CPH.exchangeID,CPH.priceTheo
					from ContractPriceHistory CPH
							inner join 
								(select contractID,exchangeID,max(priceDate) as priceDate 
									from ContractPriceHistory group by contractID,exchangeID) LastPrice
							on 	CPH.contractID = LastPrice.contractID and 
								CPH.priceDate = LastPrice.priceDate and 
								isnull(CPH.exchangeID,0)=isnull(LastPrice.exchangeID,0)) A
				on ContractPrice.contractID=A.contractID and isnull(ContractPrice.exchangeID,0)=isnull(A.exchangeID,0)
	end
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractPriceHistory]') and OBJECTPROPERTY(id, N'IsTable') = 1)
	drop table ContractPriceHistory
GO
--=================================================================================
-- changes in [Index] and Stock tables
--=================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='calcOptionType')
	begin
		Alter Table Stock
			Add calcOptionType tinyint default(1)
	end
GO
update Stock set calcOptionType = 1 where calcOptionType is null
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='calcModelType')
	begin
		Alter Table Stock
			Add calcModelType tinyint default(1)
	end
GO
update Stock set calcModelType = 1 where calcModelType is null
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='calcOptionType')
	begin
		Alter Table [Index]
			Add calcOptionType tinyint default(0)
	end
GO
update [Index] set calcOptionType = 0 where calcOptionType is null
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='calcModelType')
	begin
		Alter Table [Index]
			Add calcModelType tinyint default(1)
	end
GO
update [Index] set calcModelType = 1 where calcModelType is null
GO
--==========================================================================
-- changes in Trade table - add new field 'isSimulated'
--==========================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='isSimulated')
	begin
		Alter Table Trade
			Add isSimulated tinyint default(0)
	end
GO
update Trade set isSimulated = 0 where isSimulated is null 
GO
--=====================================================================================
-- changed in Stock and Index tables + column expCalendarTypeID tinyint with defaults
--=====================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='expCalendarTypeID')
	begin
		Alter Table Stock
			Add expCalendarTypeID tinyint default(1)
	end
GO
update Stock set expCalendarTypeID = 1 where expCalendarTypeID is null
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='expCalendarTypeID')
	begin
		Alter Table [Index]
			Add expCalendarTypeID tinyint default(2)
	end
GO
update [Index] set expCalendarTypeID = 2 where expCalendarTypeID is null
GO

--==================================================================
-- DML section of script
--==================================================================
--unchanged and unusefull - they will be droped !
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_ByUnderlyingForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_ByUnderlyingForRisk_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeForRisk_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeTotalForCID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeTotalForCID_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingForRisk_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarData_Get]
GO
-- changes in save of PriceTheo
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceHistory_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceHistory_Save]
GO
--this views was changed in 3.08 version of DB

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexView]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StockView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StockView]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE   View dbo.IndexView
as
	Select 
		indexID,
		defaultModelTypeID,
		yield,
		isHTB,
		actionID,
		actionDate,
		calcOptionType,
		calcModelType
	from [Index]
	where actionID < 3
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    View dbo.StockView
as
	Select 
		stockID,
		defaultModelTypeID,
		defaultSurfaceTypeID,
		divFreq,
		baseIndexID,
		isHTB,
		primaryExchangeID,
		divAmt,
		divDate,
		divAmtCustom,
		divDateCustom,
		divFreqCustom,
		isDivCustom,
		actionID,
		actionDate,
		calcOptionType,
		calcModelType
	from Stock
	where actionID < 3
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradeView]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[vALLStockEOD]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[vALLStockEOD]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE  view dbo.vALLStockEOD
AS
SELECT 	C.contractID AS iContractID,   
		C.contractTypeID AS iContractTypeID,    
		C.Symbol AS vcSymbol, 
    	S.divFreq AS iDivFreq, 
		S.divAmt AS fDivAmt,  
		S.divDate AS dtDivDate,
		S.isHTB as bIsHTB, 
		S.divAmtCustom,
		S.divDateCustom,
		S.divFreqCustom,
		S.isDivCustom,
		S.CalcOptionType as tiCalcOptionType,
		S.CalcModelType as tiCalcModelType
	FROM Contract C (nolock), StockView S (nolock)
	WHERE S.stockID = C.contractID
UNION
SELECT 	C.contractID AS iContractID,  
		C.contractTypeID AS iContractTypeID,
	    C.Symbol AS vcSymbol,  
		NULL AS iDivFreq, 
		I.yield AS fDivAmt, 
		NULL AS dtDivDate,
		I.isHTB as bIsHTB, 
		null as divAmtCustom,
		null as divDateCustom,
		null as divFreqCustom,
		null as isDivCustom,
		I.CalcOptionType as tiCalcOptionType,
		I.CalcModelType as tiCalcModelType
	FROM Contract C (nolock), IndexView I (nolock)
	WHERE I.indexID = C.contractID
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE view dbo.TradeView
AS
	Select 	tradeID,
		contractID, 
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		isBuy, 
		bookID, 
		strategyID, 
		traderID, 
		brokerID, 
		clearingBrokerID, 
		brokerCommissionID, 
       	clearingBrokerCommissionID, 
		tradedIV, 
		spotReference,
		isPosition,
		lotSize,
		isSimulated
	from dbo.Trade
	GROUP BY tradeID, contractID, isClose, 
		 tradeDate, quantity, price, 
		 isBuy, bookID, strategyID, 
		 traderID, brokerID, clearingBrokerID, 
		 brokerCommissionID,clearingBrokerCommissionID,
		tradedIV, spotReference,isPosition,lotSize,isSimulated
	HAVING  (MIN(actionID) = 1)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPositionForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPositionForRisk_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceTheo_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceTheo_Complete_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceTheo_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceTheo_Init_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CreateDefaultVolaSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CreateDefaultVolaSurface_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_Get]
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TP]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TP]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Update]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractForHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContractForHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockEOD_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE proc usp_ContractPositionForRisk_Get
		@iContractID int,
		@iBookID int,
		@iStrategyID int,
		@iTraderID int,
		@iUnderlyingID int = Null,
		@dtTradeDate datetime = Null,
		@tiIsClose tinyint = Null
as
begin
  if (@iContractID is Null)
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	   from TradeView t, ContractView c 
	  where (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
		and (@iUnderlyingID is Null or @iUnderlyingID is not Null and t.contractID = @iUnderlyingID)
		and c.contractID = t.contractID
		and c.contractTypeID in (1, 2)
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	  group by c.contractID, c.contractTypeID, c.symbol 
	 union
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	  from TradeView t, OptionView o, ContractView c 
	  where (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
	        and o.optionID = t.contractID
		and (@iUnderlyingID is Null or @iUnderlyingID is not Null and o.underlyingContractID = @iUnderlyingID)
		and c.contractID = t.contractID	   
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	 group by c.contractID, c.contractTypeID, c.symbol 
	else
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	   from ContractView c , Trade t
	  where c.contractID = @iContractID
		and c.contractTypeID in (1, 2)
		and t.contractID = c.contractID
	        and (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	  group by c.contractID, c.contractTypeID, c.symbol
	 union
	 select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		SUM(t.quantity * t.lotSize) as iQuantity
	  from OptionView o, ContractView c , Trade t
	 where o.underlyingContractID = @iContractID
	   and c.contractID = o.optionID
	   and t.contractID = o.optionID	   
	        and (@iBookID is Null or @iBookID is not Null and t.bookID = @iBookID)
		and (@iStrategyID is Null or @iStrategyID is not Null and t.strategyID = @iStrategyID)
		and (@iTraderID is Null or @iTraderID is not Null and t.traderID = @iTraderID)
		and (@dtTradeDate is Null or @dtTradeDate is not Null and t.TradeDate = @dtTradeDate)
		and (@tiIsClose is Null or @tiIsClose is not Null and t.isClose = @tiIsClose)
	 group by c.contractID, c.contractTypeID, c.symbol
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

CREATE   procedure dbo.usp_ContractPriceTheo_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	Begin tran 
		if object_id('tempdb..##ContractPriceTheo') is null
		   begin
		       raiserror ( 'Cannot complete EOD. Cannot find save data of theo price.', 16, 1)
		       return (-1)
		   end
	
		update ContractPrice
			set priceTheo = S.priceTheo,
				priceDate = getdate()
		from ContractPrice CP join ##ContractPriceTheo S on CP.contractID = S.contractID and CP.exchangeID is null
--		from ContractPrice CP join ##ContractPriceTheo S on CP.contractID = S.contractID and isnull(CP.exchangeID, 0) = isnull(S.exchangeID, 0)

/*		insert into ContractPrice (contractID, exchangeID, priceTheo, priceDate)
			select contractID, exchangeID, priceTheo, getdate()
				from ##ContractPriceTheo B
				where not exists(select * from ContractPrice C where C.contractID = B.contractID and isnull(C.exchangeID, 0) = isnull(B.exchangeID, 0))
*/

	Commit tran

	if object_id('tempdb..##ContractPriceTheo') is not null
		drop table ##ContractPriceTheo

	set @error = @@error
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
CREATE  procedure dbo.usp_ContractPriceTheo_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if object_id('tempdb..##ContractPriceTheo') is not null
	begin
		raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
		return (-1) 
		-- drop table ##ContractPriceTheo
	end

	--create table ##ContractPriceTheo(contractID int, exchangeID int null, priceTheo float Null)
		create table ##ContractPriceTheo(contractID int, priceTheo float Null)
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO


CREATE      Procedure dbo.usp_Contract_Get
	@iContractID int = null,	-- contract's identifier
	@iContractTypeID int = null,	-- contract's type identifier
	@bAllWithoutOptions bit = 1,	-- flag for data output
	@dtActionDate datetime = null
as
/*
	Created by Andrey Sinkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @bAllWithoutOptions is null		set @bAllWithoutOptions = 0

	if @bAllWithoutOptions = 1
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.calcOptionType is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where contractTypeID < 3
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	else
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.calcOptionType is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where (@iContractTypeID is null or contractTypeID = @iContractTypeID)
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	
Return(0)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE procedure usp_CreateDefaultVolaSurface_Save
	@iContractID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure create default surface by ContractID
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iVolaSurfaceDataID int
	if not exists(select * from VolaSurfaceDataView 
					where contractID=@iContractID and isDefaultSurface = 1) and @iContractID is not null
		begin
			declare @fPrice float
		

/*			select Top 1 @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPriceHistory 
					where contractID = @iContractID
					order by PriceDate desc */
			
			select @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPrice
					where contractID = @iContractID and exchangeID is null 

			insert into VolaSurfaceData(contractID,isDefaultSurface,baseUnderlinePrice)
				values (@iContractID,1,isnull(@fPrice,0))
			select @iVolaSurfaceDataID = @@identity
		end
	else 
		select @iVolaSurfaceDataID = VolaSurfaceDataID 
			from VolaSurfaceDataView 
			where contractID=@iContractID and isDefaultSurface = 1 and @iContractID is not null
Return(@iVolaSurfaceDataID)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE     Procedure dbo.usp_IndexDefinition_Save 
	@iIndexID int = null, --the index identifier
	@iStockID int = null, --the stock identifier
	@fWeight int = null,  --the weight
	@fBeta float = null,
	@vcIndexSymbol varchar(8) = null,
	@vcStockSymbol varchar(8) = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into IndexDefinition table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iStockID is null and @vcIndexSymbol is null and @vcStockSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Stock and Index!', 16, 1)
			Return (-1)
		end   


	if @vcIndexSymbol is not null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
	if @vcStockSymbol is not null 
		select	@iStockID = contractID from Contract where symbol = @vcStockSymbol and contractTypeID = 2 and actionID < 3


	if @iIndexID is null 
		begin
			Raiserror ('Impossible define identifier of the Index!', 16, 1)
			Return (-1)
		end   
	if @iStockID is null 
		begin
			Raiserror ('Impossible define identifier of the Stock!', 16, 1)
			Return (-1)
		end   
/*
	if exists (select * from IndexDefinition where indexID = @iIndexID and stockID=@iStockID and actionID < 3)
		begin
			Raiserror ('Row in IndexDefinition table with same indexID and StockID already exists!', 16, 1)
			Return (-1)
		end   
*/
	Begin Transaction
		if not exists(select * From IndexDefinition where indexID = @iIndexID and stockID = @iStockID) 
			begin
				insert into IndexDefinition (indexID,stockID,weight,beta) 
				values (@iIndexID,@iStockID,@fWeight,@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexDefinition
					set weight = isnull(@fWeight, weight),
						beta=isnull(@fBeta,beta)
					where indexID = @iIndexID and stockID = @iStockID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
    set @error = 0
finish:
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
CREATE       proc usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@dtActionDate datetime = Null,
	@tiWithPosition tinyint = 0
  as
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractView C 	
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
		else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractInGroupView cig
						join ContractView C  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
	end
	else
	begin
		if (@iGroupID is Null)
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractView c 	
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,I.ExpCalendarTypeID
				order by C.symbol
		else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractInGroupView cig
						join ContractView C  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,I.ExpCalendarTypeID
				order by C.symbol
	end
if (@@error <> 0)
	Return (-1)
else  
	Return (0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE	proc dbo.usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(8) = Null,
		@vcContractName varchar(255) = Null,
		@iDefaultModelTypeID int = Null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@tiExpCalendarTypeID tinyint = null
   as
	---------------
	set nocount on
	---------------

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
			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (indexID, defaultModelTypeID, yield,IsHTB,calcOptionType,calcModelType,expCalendarTypeID)
					 values (@iIndexID, @iDefaultModelTypeID, @fYield,isNull(@bIsHTB,0),@tiCalcOptionType,@tiCalcModelType,@tiExpCalendarTypeID)
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
					isHTB = isNull(@bIsHTB,isHTB),
					calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
					calcModelType = isNull(@tiCalcModelType, calcModelType),
					expCalendarTypeID = isNull(@tiExpCalendarTypeID, expCalendarTypeID)
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

CREATE    proc usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0
  as
/*
	Created by Zaur Nuraliev
	[Description]: this procedure return inf. from OptionView table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
				  from ContractView C  
						inner join [Option] O on O.optionID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				order by o.underlyingContractID, year(o.expiry), month(o.expiry), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
				  from ContractView C  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID,CP.lotSize
				order by o.underlyingContractID, year(o.expiry), month(o.expiry), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID,CP.lotSize
	end
	set @error = @@error
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
CREATE  proc dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(8) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null
   as
   if exists (select contractID from ContractView
					where contractID = @iUnderlyingContractID
					  and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 
   if exists (select optionID
				 from OptionView
				where underlyingContractID = @iUnderlyingContractID
					and isCall = @iIsCall  
					and strike = @fStrike
					and MONTH(expiry) = MONTH(@dtExpiry)
					and YEAR(expiry) = YEAR(@dtExpiry)
					and ((@iOptionID is Null) or (optionID <> @iOptionID)))
	begin
	  RAISERROR ('Error. Can''t insert or update Option values. There are ambiguous Option in DB!', 16, 1)
	  RETURN (-1)
	end  
   
   declare @OptionID int, @cp int, @cn varchar(255)
   select @cn = @vcSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
--   select @cp = CASE @iIsCall WHEN 1 THEN 3 ELSE 4 END
   select @cp = 3
   
   if (@iOptionID is Null)	
	 begin
		 
		 begin tran
			 insert into Contract (contractTypeID, symbol, contractName)
				  values (@cp, @vcSymbol, @cn)
			 select @OptionID = @@identity			  
			 insert into [Option] (optionID, underlyingContractID, isCall, strike, expiry)
					 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @dtExpiry)
		 commit tran
	 end
   else
	 begin
		select @OptionID = 0
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@cn,contractName)
				where contractID = @iOptionID
			update [Option]
			   set underlyingContractID = @iUnderlyingContractID,
					isCall = isNull(@iIsCall,isCall),
					strike = isNull(@fStrike,strike),
					expiry = isNull(@dtExpiry,expiry)
				where optionID = @iOptionID
		 commit tran
	 end		
 if (@@error <> 0)
	return (-1)
 else  
	return @OptionID
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO
CREATE   PROCEDURE dbo.usp_Position_Get
        @iUnderlyingID int = Null,
        @iTraderID int = Null,
        @iBookID int = Null,
        @iStrategyID int = Null,
	@dtMaxDate datetime = null
AS
	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
	
	from TradeView tr 
		left join ContractView c  on tr.contractID = c.contractID
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield
	
	union
	
	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,

		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		o.expiry as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when tr.tradeDate < @dtToday then isnull(cp.priceTheo, 0) else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cp.priceTheo as fPriceTheoClose
	
	from TradeView tr 
		left join ContractView c  on tr.contractID = c.contractID
		left join OptionView o on c.contractID = o.optionID
		left join Contract uc on o.underlyingContractID = uc.contractID
		inner join ContractPrice cp on c.contractID = cp.contractID and exchangeID is null
		left join StockView s on uc.contractID = s.stockID
		left join IndexView i on uc.contractID = i.indexID
/*		left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
					from ContractPriceHistory
					inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
						on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
					cph on c.contractID = cph.contractID */
	where c.contractTypeID = 3
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		o.expiry, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cp.priceTheo

	order by vcSymbol
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE                       Procedure dbo.usp_ReportStockTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null,
	@dtBeginDate datetime = Null,
	@dtEndDate datetime = Null
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
	--cut the time part of date
	select	@dtBeginDate = convert(datetime,convert(varchar(10),@dtBeginDate,112)),
			@dtEndDate = convert(datetime,convert(varchar(10),@dtEndDate,112))
	--=====================================================
	-- data selection
	--=====================================================
	select 
		Contract.contractID as iContractID,
		T.lotSize * (case T.isBuy when 0 then T.quantity * -1 else T.quantity end) as iQuantity,
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
			and 
			(
			(@dtBeginDate is not null and @dtEndDate is not null and (convert(datetime,convert(varchar(10),T.tradeDate,112)) between @dtBeginDate and @dtEndDate) )	or 
			(@dtBeginDate is not null and @dtEndDate is null and convert(datetime,convert(varchar(10),T.tradeDate,112)) >= @dtBeginDate) or 
			(@dtBeginDate is null and @dtEndDate is not null and convert(datetime,convert(varchar(10),T.tradeDate,112)) <= @dtEndDate) or
			(@dtBeginDate is null and @dtEndDate is null)
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
CREATE    Procedure dbo.usp_ReportStock_Get
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
			Stock.CalcModelType as tiCalcModelType
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
			I.CalcModelType as tiCalcModelType
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join IndexView I on Contract.contractID = I.indexID
					left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
									(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
								on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
						on S2.indexID=I.indexID
			where (
				(@iStockID is null and (I.indexID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and @iFilterID is not null and (Contract.ContractID=@iStockID and (I.indexID in (select stockID from #FilteredStocks))))
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE       Procedure dbo.usp_ReportTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null,
	@dtBeginDate datetime = Null,
	@dtEndDate datetime = Null
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
--	select * from #T	
	select	@dtBeginDate = convert(datetime,convert(varchar(10),@dtBeginDate,112)),
			@dtEndDate = convert(datetime,convert(varchar(10),@dtEndDate,112))
	select 
		O.underlyingContractID as iUnderlyingContractID,
		O.expiry as dtExpiry,
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
		T.lotSize as iLotSize
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join TradeView T on O.optionID = T.contractID 
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
				/* left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
							from ContractPriceHistory
							inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
								on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
							cph on contract.contractID = cph.contractID */

		where O.expiry > GetDate() and
			((
				(@iStockID is not null and @iFilterID is null and underlyingContractID=@iStockID) or 
				(@iStockID is null and @iFilterID is null) or
				(@iStockID is null and @iFilterID is not null and (underlyingContractID in (select stockID from #FilteredStocks)) and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)  or 
				(@iStockID is not null and @iFilterID is not null 
					and underlyingContractID=@iStockID 
					and (underlyingContractID in (select stockID from #FilteredStocks)) 
					and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0))
			))
				and
				(
				(@dtBeginDate is  not null and @dtEndDate is not null and (convert(datetime,convert(varchar(10),T.tradeDate,112)) between @dtBeginDate and @dtEndDate)) or 
				(@dtBeginDate is not null and @dtEndDate is null and convert(datetime,convert(varchar(10),T.tradeDate,112)) >= @dtBeginDate) or
				(@dtBeginDate is null and @dtEndDate is not null and convert(datetime,convert(varchar(10),T.tradeDate,112)) <= @dtEndDate) or
				(@dtBeginDate is null and @dtEndDate is null)
				)
			)
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
CREATE   proc dbo.usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Stock,Contract tables
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
				select	C.contractID as iContractID, 
						C.contractTypeID as iContractTypeID, 
						C.Symbol as vcSymbol, 
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
						S.primaryExchangeID as iPrimaryExchangeID, 
						S.DivDate as dtDivDate,
						S.DivAmt as fDivAmt, 
						S.isHTB as bIsHTB,
						S.actionDate as dtActionDate,
						S.actionID as tiActionID,
						S.isDivCustom as tiDivCustom,
						S.divAmtCustom as fDivAmtCustom,
						S.divDateCustom as dtDivDateCustom,
						S.divFreqCustom as iDivFreqCustom,
						CP.lotSize as iLotSize,
						S.CalcOptionType as tiCalcOptionType,
						S.CalcModelType as tiCalcModelType,
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractView C  
						join Stock S on S.stockID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				   order by c.Symbol
		else
				select	C.contractID as iContractID,
						C.contractTypeID as iContractTypeID,
						C.Symbol as vcSymbol,
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
						S.primaryExchangeID as iPrimaryExchangeID, 
						S.DivDate as dtDivDate,
						S.DivAmt as fDivAmt, 
						S.isHTB as bIsHTB,
						S.actionDate as dtActionDate,
						S.actionID as tiActionID,
						S.isDivCustom as tiDivCustom,
						S.divAmtCustom as fDivAmtCustom,
						S.divDateCustom as dtDivDateCustom,
						S.divFreqCustom as iDivFreqCustom,
						CP.lotSize as iLotSize,
						S.CalcOptionType as tiCalcOptionType,
						S.CalcModelType as tiCalcModelType,
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractInGroupView cig
						join ContractView C  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				   order by c.Symbol
	end
	else
	begin
		if (@iGroupID is Null)
					select	C.contractID as iContractID, 
						C.contractTypeID as iContractTypeID, 
						C.Symbol as vcSymbol, 
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
						S.primaryExchangeID as iPrimaryExchangeID, 
						S.DivDate as dtDivDate,
						S.DivAmt as fDivAmt, 
						S.isHTB as bIsHTB,
						S.actionDate as dtActionDate,
						S.actionID as tiActionID,
						sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
						S.isDivCustom as tiDivCustom,
						S.divAmtCustom as fDivAmtCustom,
						S.divDateCustom as dtDivDateCustom,
						S.divFreqCustom as iDivFreqCustom,
						CP.lotSize as iLotSize,
						S.CalcOptionType as tiCalcOptionType,
						S.CalcModelType as tiCalcModelType,
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractView c  
						join Stock S on S.stockID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
						S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
						S.CalcOptionType,S.CalcModelType,S.ExpCalendarTypeID
					order by c.Symbol
		else
					select	C.contractID as iContractID,
						C.contractTypeID as iContractTypeID,
						C.Symbol as vcSymbol,
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
						S.primaryExchangeID as iPrimaryExchangeID, 
						S.DivDate as dtDivDate,
						S.DivAmt as fDivAmt, 
						S.isHTB as bIsHTB,
						S.actionDate as dtActionDate,
						S.actionID as tiActionID,
						sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
						S.isDivCustom as tiDivCustom,
						S.divAmtCustom as fDivAmtCustom,
						S.divDateCustom as dtDivDateCustom,
						S.divFreqCustom as iDivFreqCustom,
						CP.lotSize as iLotSize,
						S.CalcOptionType as tiCalcOptionType,
						S.CalcModelType as tiCalcModelType,
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
						S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
						S.CalcOptionType,S.CalcModelType,S.ExpCalendarTypeID
					order by c.Symbol
	end
	
	set @error = @@error
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

CREATE       proc usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceAsk float,
	@fPriceBid float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iExchangeID int = null,  --the ExchangeID special for save data into ContractPrice table
	@iLotSize int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update Contract
		    set ContractName = @vcContractName
	 	where contractID = @iContractID
	
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		update ContractPrice 
			set priceAsk = @fPriceAsk,
			priceBid = @fPriceBid,
			PriceOpen = @fPriceOpen,
			PriceClose = @fPriceClose,
			PriceLow = @fPriceLow,
			PriceHigh = @fPriceHigh,
			Volume = @iVolume,
			lotSize = isnull(@iLotSize,lotSize)
		where contractID = @iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
		if (@@error <> 0)
			begin
				Raiserror ('Error. Can''t update Contract Price!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID,lotSize)
					values(@iContractID,null,@iLotSize)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from stock where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE          proc usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @iDefaultModelTypeID int = Null,
        @iDefaultSurfaceTypeID int = Null,
        @iBaseIndexID int = Null,
        @vcSymbol varchar(8) = Null,
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
		@tiExpCalendarTypeID tinyint = null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

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
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
				defaultModelTypeID, 
				defaultSurfaceTypeID,
                divFreq, 
				baseIndexID, 
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
				expCalendarTypeID)
            values (
				@iStockID, 
				@idefaultModelTypeID, 
				@idefaultSurfaceTypeID,
                @iDivFreq, 
				@ibaseIndexID,
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
				@tiExpCalendarTypeID)
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
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType),
						expCalendarTypeID = isNull(@tiExpCalendarTypeID,expCalendarTypeID)
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE proc dbo.usp_TP
	@iContractID int,
	@fPriceTheo float = Null
--	,@iExchangeID int = Null, 
as
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPriceHistory table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

--	if object_id('tempdb..##ContractPriceTheo') is not null
--		begin
--			insert into ##ContractPriceTheo (contractID, exchangeID, priceTheo)
--				values (@iContractID, @iExchangeID, @fPriceTheo)
--		end
	insert into ##ContractPriceTheo (contractID, priceTheo)
		values (@iContractID, @fPriceTheo)

    set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO


CREATE   PROC usp_TradeSeq_Get
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null,
	@dtTradeDate datetime = null,
	@tiExtendedInfo tinyint = null
AS
	set @dtTradeDate = cast(floor(cast(@dtTradeDate as float)) as datetime)
	if @iMinSeqNum = 0 set @iMinSeqNum = Null
	if @iMaxSeqNum = 0 set @iMaxSeqNum = Null
	if @tiExtendedInfo is null or @tiExtendedInfo = 0
		select
			seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tradeID as iTradeID,
			tr.actionID as tiActionID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			tr.brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			tr.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
		order by seqNum
	else
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
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractId and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID in (1, 2)
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tr.tradeDate as float)) as datetime) = @dtTradeDate)
		union
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
			o.expiry as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			tr.lotSize as iTradeLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join ContractView uc on o.underlyingContractID = uc.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tr.tradeDate as float)) as datetime) = @dtTradeDate)
		order by tr.seqNum


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE   proc usp_Trade_Del
        @iTradeID int
as
	if @iTradeID is not null
	begin
		begin tran
		declare @nCount int,
			@iNewSeqNum int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iTradeID
		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated
			from Trade
			where tradeID = @iTradeID
			if @@error = 0
			begin
				set @iNewSeqNum = @@identity
				commit tran
				return @iNewSeqNum
			end
			else
			begin
				rollback tran
				return 0
			end
		end
		else
		begin
			rollback tran
			return 0
		end
	end
	else
		return 0
/*
        delete Trade where tradeID = @iTradeID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
*/

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE       proc usp_Trade_Get
  @iTradeID int = Null,
  @iContractID int = Null,
  @tiExtendedInfo tinyint = 0,
  @iUnderlyingID int = Null,
     @iTraderID int = Null,
     @iBookID int = Null,
     @iStrategyID int = Null,
     @iContractGroupID int = Null,
  
  @dtMinTradeDate datetime = null
as
 if @tiExtendedInfo is null or @tiExtendedInfo = 0
  select
   tradeID as iTradeID,
   contractID as iContractID,
   isClose as tiIsClose,
   tradeDate as dtTradeDate,
   quantity as iQuantity,
   price as fPrice,
   isBuy as tiIsBuy,
   bookID as iBookID,     
   strategyID as iStrategyID, 
   traderID as iTraderID,
   brokerID as iBrokerID,
   clearingBrokerID as iClearingBrokerID,
   brokerCommissionID as iBrokerCommissionID,
   clearingBrokerCommissionID as iClearingBrokerCommissionID,
   tradedIV as fTradedIV,
   spotReference as fSpotReference,
   isPosition as tiIsPosition,
   lotSize as iLotSize,
   isSimulated as tiIsSimulated
  from Trade 
  where 
   (@iTradeID is null or @iTradeID is not null and tradeID = @iTradeID)
   and (@iContractID is null or @iContractID is not null and contractID = @iContractID)
   and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tradeDate >= @dtMinTradeDate)
  group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,lotSize,isSimulated
  having min(actionID) = 1
  order by TradeDate
 else if @iContractGroupID is null
   select
    tr.tradeID as iTradeID,
 
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
    cp.priceClose as fPriceClose,
    cp.priceClose as fUnderlyingPriceClose,
    null as fPriceTheoClose,
 
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
    tr.brokerCommissionID as iBrokerCommissionID,
    tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
    tr.tradedIV as fTradedIV,
    tr.spotReference as fSpotReference,
    tr.isPosition as tiIsPosition,
 
    tr.lotSize as iTradeLotSize,
    cp.lotSize as iLotSize,
    cp.lotSize as iUnderlyingLotSize,
 
    tr.isSimulated as tiIsSimulated
   from Trade tr
    inner join Contract c on tr.contractID = c.contractID
    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
    left join Stock s on c.contractID = s.stockID
    left join [Index] i on c.contractID = i.indexID
   where c.contractTypeID in (1, 2)
    and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
    and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
    and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
    and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
    and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
    and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
    and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
   group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
    tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
    tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
    tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,cp.priceClose,cp.lotSize
   having min(tr.actionID) = 1
   union
   select
    tr.tradeID as iTradeID,
    c.contractID as iContractID,
    c.contractTypeID as iContractTypeID,
    c.Symbol as vcSymbol,
    s.divFreq as iDivFreq,
    s.divDate as dtDivDate,
    s.divAmt as fDivAmt,
    i.yield as fYield,
    o.expiry as dtExpiry,
    o.strike as fStrike,
    o.isCall as bIsCall,
    uc.contractID as iUnderlyingContractID,
    uc.contractTypeID as iUnderlyingContractTypeID,
    uc.Symbol as vcUnderlyingSymbol,
    cp.priceClose as fPriceClose,
    ucp.priceClose as fUnderlyingPriceClose,
    cp.priceTheo as fPriceTheoClose,
 
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
    tr.brokerCommissionID as iBrokerCommissionID,
    tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
    tr.tradedIV as fTradedIV,
    tr.spotReference as fSpotReference,
    tr.isPosition as tiIsPosition,
 
    tr.lotSize as iTradeLotSize,
    cp.lotSize as iLotSize,
    ucp.lotSize as iUnderlyingLotSize,
 
    tr.isSimulated as tiIsSimulated
   from Trade tr inner join Contract c on tr.contractID = c.contractID
    inner join [Option] o on c.contractID = o.optionID
    inner join Contract uc on o.underlyingContractID = uc.contractID
    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
    inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
    left join Stock s on uc.contractID = s.stockID
    left join [Index] i on uc.contractID = i.indexID
   where c.contractTypeID = 3
    and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
    and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
    and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
    and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
    and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
    and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
    and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
   group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
    o.expiry,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
    tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
    tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
    tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
	cp.lotSize,cp.priceClose,ucp.lotSize,ucp.priceClose
   having min(tr.actionID) = 1
   order by TradeDate
  else
   select
    tr.tradeID as iTradeID,
 
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
    cp.priceClose as fPriceClose,
    cp.priceClose as fUnderlyingPriceClose,
    null as fPriceTheoClose,
 
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
    tr.brokerCommissionID as iBrokerCommissionID,
    tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
    tr.tradedIV as fTradedIV,
    tr.spotReference as fSpotReference,
    tr.isPosition as tiIsPosition,

    tr.lotSize as iTradeLotSize,
    cp.lotSize as iLotSize,
    cp.lotSize as iUnderlyingLotSize,

    tr.isSimulated as tiIsSimulated
   from Trade tr
    inner join Contract c on tr.contractID = c.contractID
    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
    inner join ContractInGroup cig on c.contractID = cig.contractID
    left join Stock s on c.contractID = s.stockID
    left join [Index] i on c.contractID = i.indexID
   where c.contractTypeID in (1, 2)
    and cig.groupID = @iContractGroupID
    and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
    and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
    and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
    and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
    and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
    and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
    and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
   group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
    tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
    tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
    tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,cp.priceClose,cp.lotSize
   having min(tr.actionID) = 1
   union
   select
    tr.tradeID as iTradeID,
    c.contractID as iContractID,
    c.contractTypeID as iContractTypeID,
    c.Symbol as vcSymbol,
    s.divFreq as iDivFreq,
    s.divDate as dtDivDate,
    s.divAmt as fDivAmt,
    i.yield as fYield,
    o.expiry as dtExpiry,
    o.strike as fStrike,
    o.isCall as bIsCall,
    uc.contractID as iUnderlyingContractID,
    uc.contractTypeID as iUnderlyingContractTypeID,
    uc.Symbol as vcUnderlyingSymbol,
    cp.priceClose as fPriceClose,
    ucp.priceClose as fUnderlyingPriceClose,
    cp.priceTheo as fPriceTheoClose,
 
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
    tr.brokerCommissionID as iBrokerCommissionID,
    tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
    tr.tradedIV as fTradedIV,
    tr.spotReference as fSpotReference,
    tr.isPosition as tiIsPosition,

    tr.lotSize as iTradeLotSize,
    cp.lotSize as iLotSize,
    ucp.lotSize as iUnderlyingLotSize,

    tr.isSimulated as tiIsSimulated
   from Trade tr inner join Contract c on tr.contractID = c.contractID
    inner join [Option] o on c.contractID = o.optionID
    inner join Contract uc on o.underlyingContractID = uc.contractID
    inner join ContractInGroup cig on o.underlyingContractID = cig.contractID
    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
    inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
    left join Stock s on uc.contractID = s.stockID
    left join [Index] i on uc.contractID = i.indexID
   where c.contractTypeID = 3
    and cig.groupID = @iContractGroupID
    and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
    and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
    and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
    and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
    and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
    and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
    and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
   group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
    o.expiry,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
    tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
    tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
    tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
	cp.lotSize,cp.priceClose,ucp.lotSize, ucp.priceClose
   having min(tr.actionID) = 1
   order by TradeDate

     Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc usp_Trade_New
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID int =Null,
        @iClearingBrokerCommissionID int = Null,
        @fTradedIV  float = Null,
		@fSpotReference  float = Null,
		@tiIsPosition tinyint = 0,
		@iNewTradeID int out,
		@dtNewTradeDate datetime = Null out,
		@iLotSize int = Null,
		@tiIsSimulated tinyint = Null
  as
	begin tran
	declare @iNewSeqNum int
	select @iNewTradeID = max(tradeID) + 1 from Trade
	if @iNewTradeID is null set @iNewTradeID = 1
	if @dtNewTradeDate is null set @dtNewTradeDate = getdate()
	insert into Trade (
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
		brokerID, 
		clearingBrokerID, 
		isBuy, 
		brokerCommissionID, 
		clearingBrokerCommissionID,
		tradedIV,
		spotReference,
		isPosition,
		lotSize,
		isSimulated)
	values (
		@iNewTradeID,
		1,
		@iContractID, 
		@tiIsClose, 
		@dtNewTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iBookID, 
		@iStrategyID, 
		@iTraderID, 
		@iBrokerID,
		@iClearingBrokerID,
		@tiIsBuy, 
		@iBrokerCommissionID,
		@iClearingBrokerCommissionID,
		@fTradedIV,
		@fSpotReference,
		@tiIsPosition,
		@iLotSize,
		@tiIsSimulated)
	if @@error = 0
	begin
		set @iNewSeqNum = @@identity 
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end
/*
   if not exists(select * from Trade where tradeID = @iTradeID)  
     begin
         insert into Trade (
				contractID,
				seqNum,
				publisherID, 
				isClose, 
				tradeDate, 
				quantity, 
				price, 
				bookID, 
				strategyID, 
				traderID, 
				brokerID, 
				clearingBrokerID, 
				isBuy, 
				brokerCommission, 
				clearingBrokerCommission,
				tradedIV,
				spotReference)
                values (
				@iContractID, 
				@iSeqNum,
				@vcPublisherID,
				@iIsClose, 
				@dtTradeDate, 
				@iquantity, 
				@fPrice, 
				@iBookID, 
				@iStrategyID, 
				@iTraderID, 
				@iBrokerID,
				@iClearingBrokerID,
				@iIsBuy, 
				@fBrokerCommission,
				@fClearingBrokerCommission,
				@fTradedIV,
				@fSpotReference)
         if (@@error <> 0)
            return (-1)
          else  
            return @@identity
     end
   else 
     begin
        update Trade 
             set contractID = isnull(@iContractID,contractID),
				seqNum = isnull(@iSeqNum,seqNum),
				publisherID = isnull(@vcPublisherID,publisherID),
				isClose = isnull(@iIsClose,isClose),
				tradeDate = isnull(@dtTradeDate,tradeDate),
				quantity = isnull(@iquantity,quantity),
				price = isnull(@fPrice,price),
				bookID = isnull(@iBookID,bookID),
				strategyID = isnull(@iStrategyID,strategyID),
				traderID = isnull(@iTraderID,traderID),
				brokerID = isnull(@iBrokerID,brokerID),
				clearingBrokerID = isnull(@iClearingBrokerID,clearingBrokerID),
				isBuy = isnull(@iIsBuy,isBuy),
				brokerCommission = isnull(@fBrokerCommission,brokerCommission),
				ClearingBrokerCommission = isnull(@fClearingBrokerCommission,ClearingBrokerCommission),
				tradedIV = isnull(@fTradedIV,tradedIV),
				spotReference = isnull(@fSpotReference,spotReference)
          where tradeID = @iTradeID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
     end
*/

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO
CREATE   proc usp_Trade_Update
		@iOldTradeID int,
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID float =Null,
        @iClearingBrokerCommissionID float = Null,
        @fTradedIV  float = Null,
		@fSpotReference  float = Null,
		@tiIsPosition tinyint = 0,
		@iNewTradeID int out,
        @dtNewTradeDate datetime out,
		@iLotSize int = Null,
		@tiIsSimulated tinyint = null
  as
	begin tran
	declare @bSucceed bit,
		@iNewSeqNum int
	set @bSucceed = 0
	if @iOldTradeID is not null
	begin
		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				lotSize,
				isSimulated
			from Trade
			where tradeID = @iOldTradeID
		
			if @@error = 0 set @bSucceed = 1
		end
	end
	if @bSucceed = 1
	begin
		select @iNewTradeID = max(tradeID) + 1 from Trade
		if @iNewTradeID is null set @iNewTradeID = 1
		set @dtNewTradeDate = getdate()		
	
		insert into Trade (
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
			brokerID, 
			clearingBrokerID, 
			isBuy, 
			brokerCommissionID, 
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			lotSize,
			isSimulated)
		values (
			@iNewTradeID,
			1,
			@iContractID, 
			@tiIsClose, 
			@dtNewTradeDate, 
			@iQuantity, 
			@fPrice, 
			@iBookID, 
			@iStrategyID, 
			@iTraderID, 
			@iBrokerID,
			@iClearingBrokerID,
			@tiIsBuy, 
			@iBrokerCommissionID,
			@iClearingBrokerCommissionID,
			@fTradedIV,
			@fSpotReference,
			@tiIsPosition,
			@iLotSize,
			@tiIsSimulated)
		if @@error = 0
			set @bSucceed = 1
		else
			set @bSucceed = 0
	end
	if @bSucceed = 1
	begin
		set @iNewSeqNum = @@identity
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE   Procedure dbo.usp_TraderContractForHH_Get 
	@iTraderID int = null, -- trader ID
	@vcAcronym varchar(12) = null, -- trader acronym
	@tiInfoType tinyint = 0, -- information required: 
								-- 0 - available underlyiings list,
								-- 1 - market structure updates,
								-- 2 - contracts for price feed,
								-- 3 - index definition,
								-- 4 - contract prices
	@iContractTypeID int = Null, -- contract type ID for @tiInfoType = 1, if Null or 0 then retrieve general contract updates
	@dtMinUpdateDate datetime = Null, -- minimal update date for @tiInfoType = 1, if Null then retrieve full actual info (w/o "deleted" records)
	@iContractID int = Null -- contract ID for @tiInfoType = 1
as
/*
	Created by Sharky
	Description: this procedure is for getting information for HendHeld initialize
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	if @iContractID = 0 set @iContractID = Null
	-- check tarder parameters
	if isnull(@iTraderID, 0) = 0
	begin
		if not @vcAcronym is null
			select top 1 @iTraderID = traderID
			from TraderView
			where acronym = @vcAcronym
		if isnull(@iTraderID, 0) = 0
		begin
			raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
			return -1
		end
	end
	else if not exists(select 1 from Trader where traderID = @iTraderID)
	begin
		raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
		return -1
	end
	set @tiInfoType = isnull(@tiInfoType, 0)
	if @tiInfoType = 0 -- available underlyings list
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			ce.exchangeID as iExchangeID,
			isnull(convert(bit, left(ce.exchangeLinkType, 1)), 0) as iType1,
			isnull(convert(bit, right(ce.exchangeLinkType, 1)), 0) as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLink ce on c.contractID = ce.contractID
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			null as iExchangeID,
			1 as iType1,
			1 as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
		where
			tc.traderID = @iTraderID
	else if @tiInfoType = 1 -- market structure updates
	begin
		if @dtMinUpdateDate is not null -- updates info
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join Contract c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
					/* left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID */
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

	
				from TraderContractView tc
					join [Option] o on tc.contractID = o.underlyingContractID
					join Contract c on o.optionID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
					/* left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID */
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					o.underlyingContractID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					o.expiry as dtExpiry,
	
					o.actionID as tiActionID,
					o.actionDate as dtActionDate
					
				from TraderContractView tc
					join [Option] o on tc.contractID = o.underlyingContractID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or o.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					s.baseIndexID as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,
					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join Stock s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or s.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join [Index] i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
		else
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join ContractView c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
					/*left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID */
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

	
				from TraderContractView tc
					join OptionView o on tc.contractID = o.underlyingContractID
					join ContractView c on o.optionID = c.contractID
					left join ContractPrice cph on c.contractID = cph.contractID
					/* left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID */
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					o.underlyingContractID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					o.expiry as dtExpiry,
	
					o.actionID as tiActionID,
					o.actionDate as dtActionDate
					
				from TraderContractView tc
					join OptionView o on tc.contractID = o.underlyingContractID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					s.baseIndexID as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,

					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join StockView s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join IndexView i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
	end
	else if @tiInfoType = 2 -- contracts for price feed
	begin
		declare @dtToday datetime
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			e.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLink ce on c.contractID = ce.contractID
			join ExchangeView e on ce.exchangeID = e.exchangeID
		where
			tc.traderID = @iTraderID
			and isnull(convert(bit, left(ce.exchangeLinkType,1)), 0) <> 0
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			null as vcExchangeCode
		from TraderContractView tc 
			join ContractView c on tc.contractID = c.contractID
		where 
			tc.traderID = @iTraderID
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			e.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join OptionView o on tc.contractID = o.underlyingContractID
			join ContractView c on o.optionID = c.contractID
			join ContractExchangeLink ce on o.underlyingContractID = ce.contractID
			join ExchangeView e on ce.exchangeID = e.exchangeID
		where
			tc.traderID = @iTraderID
			and isnull(convert(bit, right(ce.exchangeLinkType,1)), 0) <> 0
			and o.expiry > @dtToday
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			null as vcExchangeCode
		from TraderContractView tc	
			join OptionView o on tc.contractID = o.underlyingContractID
			join ContractView c on o.optionID = c.contractID
		where
			tc.traderID = @iTraderID
			and o.expiry > @dtToday
	end
	else if @tiInfoType = 3 -- index definition
	begin
		if @dtMinUpdateDate is not null -- updates info
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				i.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate

			from
				IndexDefinition i
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
				order by dtActionDate
		else
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				i.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate
			from
				IndexDefinitionView i
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				order by dtActionDate
	end
	else if @tiInfoType = 4 -- contract prices
	begin
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			null as fPriceTheoClose
		from TraderContractView tc
			join Contract c on tc.contractID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID 
			/* left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
						from ContractPriceHistory
						inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
							on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
						cph on c.contractID = cph.contractID */
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			cph.priceTheo as fPriceTheoClose
		from TraderContractView tc
			join [Option] o on tc.contractID = o.underlyingContractID
			join Contract c on o.optionID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID
			/*left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
						from ContractPriceHistory
						inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
							on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
						cph on c.contractID = cph.contractID */
		where
			tc.traderID = @iTraderID
	end
	else
	begin
		raiserror('Error. Invalid information type was specified.', 16, 1)
		return -1
	end
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

CREATE      proc usp_allStockEOD_Get
	@iFormat int = 1	-- 1 - list only
				   		-- 2 - list with prices by exchange
  as
	---------------
	set nocount on
	---------------

--=================================================================
-- select stocks for which trades was
--=================================================================
	if object_ID('tempdb..#SelStocks') is null
		Create table #SelStocks(stockID int not null)
	
	insert into #SelStocks
		select stockID 
			from Stock
		where 
		stockID in (select distinct O.underlyingContractID from OptionView O join TradeView TV on TV.contractID=O.optionID)
	union 
		select distinct stockID 
			from Stock 
		where stockID in (select contractID from TradeView)

--=================================================================
--base section 
--=================================================================
	if @iFormat = 1
		select	iContractID, 
				iContractTypeID, 
				vcSymbol,
				tiCalcOptionType,
				tiCalcModelType
			from vALLStockEOD A
		where iContractID in (select stockID from #SelStocks)
	else
		-- use NULL exchange only
		select iContractID, 
				iContractTypeID, 
				vcSymbol, 
				null as vcExchangeCode,  
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceClose as fPriceClose,
				A.bIsHTB as bIsHTB,
				tiCalcOptionType,
				tiCalcModelType 
			from vALLStockEOD A, ContractPrice CP (nolock)
			where A.iContractID *= CP.contractID and CP.exchangeID is null
			and A.iContractID in (select stockID from #SelStocks)

if object_ID('tempdb..#SelStocks') is not null
	Drop table #SelStocks

if (@@error <> 0)
	Return (-1)
else  
   Return(0)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.08')
	begin
		update DataInfo set keyValue = '3.08' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.08'
	end
GO