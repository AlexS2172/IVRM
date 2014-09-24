/*
	PLEASE BACKUP DATABASE BEFORE RUN THIS SCRIPT !
	this script change DB version to 3.07
	and adds the ability to save close price in EMM_FW Back Office (End of Day operation) 
	and the ability to work with custom dividends
*/
--================================================================================================
-- altering Stock table add custom dividend columns
--================================================================================================
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='isDivCustom')
	begin
		Alter Table Stock
			Add isDivCustom tinyint not null default(0)
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='divAmtCustom')
	begin
		Alter Table Stock
			Add divAmtCustom float null
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='divDateCustom')
	begin
		Alter Table Stock
			Add divDateCustom  smalldatetime null
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='divFreqCustom')
	begin
		Alter Table Stock
			Add divFreqCustom int null
	end
GO
--====================================================================
-- alter Stock table (isHTB column - convert bit to tinyint datatype)
--====================================================================
Alter table dbo.Stock 
	Alter Column divFreq int null
GO
if object_id('DF_Stocks_IsHTB') is not null
	begin
		ALTER TABLE dbo.Stock
			DROP CONSTRAINT DF_Stocks_IsHTB
	end 
GO
Alter table dbo.Stock 
	Alter Column isHTB tinyint null
GO
ALTER TABLE dbo.Stock ADD CONSTRAINT
	DF_Stocks_isHTB DEFAULT (0) FOR isHTB
GO
--====================================================================
-- alter Index table (isHTB column - convert bit to tinyint datatype)
--====================================================================
if object_id('DF_Indexes_isHTB') is not null
	begin
		ALTER TABLE dbo.[Index]
			DROP CONSTRAINT DF_Indexes_isHTB
	end 
GO
Alter table dbo.[Index] 
	Alter Column isHTB tinyint null
GO
ALTER TABLE dbo.[Index] ADD CONSTRAINT
	DF_Indexes_isHTB DEFAULT (0) FOR isHTB
GO
--================================================================================================
-- Drop unusefull Dividend table
--================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Dividend]') and OBJECTPROPERTY(id, N'IsTable') = 1)
	drop table [dbo].[Dividend]
GO
--================================================================================================
-- changes in referenced SP's and Views
--================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItemMask_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItemMask_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItemMask_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItem_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItem_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRuleItem_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRule_GetAll]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRule_GetAll]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookSwap_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookSwap_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Book_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Book_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Book_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BrokerRate_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BrokerRate_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Broker_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Broker_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Broker_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTradeByCacheID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTradeByCacheID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTrade_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTrade_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Commission_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Commission_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Commission_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractByGroupID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractByGroupID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractByID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractByID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractExchangeLink_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractExchangeLink_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractForTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGenerationRule_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGenerationRule_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroupItem_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroupItem_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractIDForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractIDForTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInExchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInExchange_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupByContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroupByContract_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupByContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroupByContract_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupByContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroupByContract_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroup_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractMarketData_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractMarketData_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractMarketData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractMarketData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractMarketData_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractMarketData_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPositionForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPositionForRisk_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceHistory_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceHistory_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPrice_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractStockAndIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractStockAndIndex_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractSymbolForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractSymbolForRisk_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractSymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractSymbol_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractVM_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractWithSpot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractWithSpot_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CreateDefaultVolaSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CreateDefaultVolaSurface_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CurrentDate_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CurrentDate_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewDefaultPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewDefaultPoint_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Copy]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Copy]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataInfo_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DataInfo_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Dividend_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Dividend_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Dividend_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Dividend_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Dividend_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Dividend_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHoliday_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHoliday_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHoliday_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHoliday_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHoliday_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExchangeHoliday_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FilterData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FilterList_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FilterTradeData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FilterTradeData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Filter_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Filter_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Filter_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Filter_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Filter_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Filter_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationGroupItemsClear_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationGroupItemsClear_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRuleGroup_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRuleGroup_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRuleGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRuleGroup_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRuleGroup_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRuleGroup_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRule_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRule_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRule_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRule_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GenerationRule_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_GenerationRule_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HistRangesCache_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_HistRangesCache_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HistRangesCache_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_HistRangesCache_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HistRangesCache_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_HistRangesCache_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Holiday_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Holiday_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Holiday_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Holiday_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Holiday_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Holiday_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurve_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRCurve_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRCurve_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurve_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRCurve_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexCorrelationDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexCorrelationDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexCorrelationDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexCorrelationDH_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDataHistoryDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDataHistoryDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDataHistory_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDataHistory_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexStockCorrelationDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexStockCorrelationDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexStockCorrelationDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexStockCorrelationDH_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexStockCorrelation_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexStockCorrelation_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexVM_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexYield_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexYield_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IsSymbolExists_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IsSymbolExists_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastCacheID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastCacheID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastTradeSeqNum_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastTradeSeqNum_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastTradeTime_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastTradeTime_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastTradeTime_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastTradeTime_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MLState_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MLState_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MLState_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MLState_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MLState_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MLState_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_NextDaySurface_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_NextDaySurface_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_NextDaySurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_NextDaySurface_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_NextDaySurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_NextDaySurface_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionDataCache_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionDataCache_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionDataCache_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionDataCache_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionDataCache_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionDataCache_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionPair_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionPair_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionPermanent_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionPermanent_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionUnderlying_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionVM_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionVM_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionVM_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_expDate_Sync]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_expDate_Sync]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_ByUnderlyingForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_ByUnderlyingForRisk_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PriceCloseDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PriceCloseDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PriceCloseDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PriceCloseDH_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportMarketStructure_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportMarketStructure_Get]
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RuleItemType_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RuleItemType_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RuleItemType_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockCorrelationDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockCorrelationDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockCorrelationDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockCorrelationDH_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockDataHistoryDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockDataHistoryDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockDataHistory_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockDataHistory_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockReportDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockReportDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockVM_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Strategy_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Strategy_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Strategy_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Symbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Symbol_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeBySymbol_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeForRisk_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeTotalForCID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeTotalForCID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Clear]
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Split]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Split]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Update]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractForHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContractForHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRM_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRM_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRM_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRole_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRole_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRole_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderRole_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingForRisk_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarIndex_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarIndex_Set]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarIndex_Set]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarStockIndex_Set]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarStockIndex_Set]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceData_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceData_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceData_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaSurfaceData_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_WtdVegaMaturity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_WtdVegaMaturity_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_WtdVegaMaturity_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_WtdVegaMaturity_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockEOD_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockOptionEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockOptionEOD_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_completeContractPriceEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_completeContractPriceEOD_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_initContractPriceEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_initContractPriceEOD_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexDefinitionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexDefinitionView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[vALLStockEOD]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[vALLStockEOD]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CustomStrikeSkewPointView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[CustomStrikeSkewPointView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StockView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StockView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractInGroupView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractInGroupView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradeView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TraderContractView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[VolaSurfaceDataView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[VolaSurfaceDataView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CommissionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[CommissionView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExchangeHolidayView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExchangeHolidayView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IRPointView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IRPointView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TraderView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BookView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[BookView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BrokerView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[BrokerView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractGroupView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractGroupView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExchangeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExchangeView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExpiryView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExpiryView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[HolidayView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[HolidayView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IRCurveView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IRCurveView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[OptionView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[RMScenarioView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[RMScenarioView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StrategyView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StrategyView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderRoleView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TraderRoleView]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.BookView
as
	Select 
		bookID,
		bookName,
		[description],
		priority,
		actionID,
		actionDate
	from Book
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

CREATE View dbo.BrokerView
as
	Select 
		brokerID,
		brokerName,
		[description],
		isClearing,
		actionID,
		actionDate
	from Broker
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.ContractGroupView
as
	Select 
		[groupID],
		[groupName],
		[description],
		actionID,
		actionDate
	from ContractGroup
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

CREATE  View dbo.ExchangeView
as
	Select 
		exchangeID,
		exchangeName,
		[description],
		exchangeCode,
		actionID,
		actionDate
	from Exchange
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.ExpiryView
as
	Select 
		expiryID,
		expiryPeriod,
		expiryDate,
		vegaWeight,
		actionID,
		actionDate
	from Expiry
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

CREATE  View dbo.HolidayView
as
	Select 
		holidayID,
		holidayName,
		holidayDate,
		actionID,
		actionDate
	from Holiday
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

Create View IRCurveView
as
	Select 
		curveID,
		curveName,
		ruleTypeID,
		curveTypeID,
		positionThreshold,
		actionID,
		actionDate
	from IRCurve
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.OptionView
as
	Select 
		optionID,
		underlyingContractID,
		isCall,
		strike,
		expiry,
		actionID,
		actionDate
	from [Option]
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.RMScenarioView
as
	Select 
		RMScenarioID,
		scenarioName,
		[description],
		points1,
		step1,
		isAbs1,
		type1,
		points2,
		step2,
		isAbs2,
		type2,
		actionID,
		actionDate
	from RMScenario
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.StrategyView
as
	Select 
		strategyID,
		strategyName,
		[description],
		actionID,
		actionDate
	from Strategy
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.TraderRoleView
as
	Select 
		traderRoleID,
		roleName,
		actionID,
		actionDate
	from TraderRole
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

CREATE  View dbo.CommissionView
as
	Select 
		commissionID,
		brokerID,
		commissionDate,
		commissionValue,
		actionID,
		actionDate
	from Commission
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

CREATE  View dbo.ExchangeHolidayView
as
	Select 
		exchangeID,
		holidayID,
		actionID,
		actionDate
	from ExchangeHoliday
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

Create View IRPointView
as
	Select 
		IRpointID,
		curveID,
		periodTypeID,
		num,
		shortRate,
		HTBRate,
		longRate,
		actionID,
		actionDate
	from IRPoint
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

CREATE  View TraderView
as
	Select 
		traderID,
		acronym,
		traderName,
		clearingTraderCode,
		traderRoleID,
		traderRefID,
		actionID,
		actionDate
	from Trader
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create  View dbo.ContractView
as
	Select 
		contractID,
		contractTypeID,
		symbol,
		contractName,
		ruleID,
		groupID,
		actionID,
		actionDate
	from Contract
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

CREATE  View dbo.ContractInGroupView
as
	Select 
		contractID,
		groupID,
		actionID,
		actionDate
	from ContractInGroup
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

CREATE   View dbo.IndexView
as
	Select 
		indexID,
		defaultModelTypeID,
		yield,
		isHTB,
		actionID,
		actionDate
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
		spotReference
	from dbo.Trade
	GROUP BY tradeID, contractID, isClose, 
		 tradeDate, quantity, price, 
		 isBuy, bookID, strategyID, 
		 traderID, brokerID, clearingBrokerID, 
		 brokerCommissionID,clearingBrokerCommissionID,
		tradedIV, spotReference
	HAVING  (MIN(actionID) = 1)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE VIEW dbo.TraderContractView
AS
SELECT     traderID, contractID, actionID, actionDate
FROM         dbo.TraderContract
WHERE     (actionID < 3)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create View VolaSurfaceDataView
as
	Select 
		volaSurfaceDataID,
		contractID,
		baseUnderlinePrice,
		smileAccelerator,
		interpolationFactor,
		strikeMoneynessHi,
		strikeMoneynessLo,
		surfaceName,
		isPriceOverride,
		isDiscreteAcceleration,
		isDefaultSurface,
		ATMVolatility,
		actionID,
		actionDate
	from VolaSurfaceData
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

Create View CustomStrikeSkewPointView
as
	Select 
		customStrikeSkewPointID,
		volaSurfaceDataID,
		strike,
		type,
		volatility,
		expDate,
		isBasePoint,
		actionID,
		actionDate
	from CustomStrikeSkewPoint
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

CREATE   View dbo.StockView
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
		actionID,
		actionDate
	from Stock
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

CREATE  View dbo.IndexDefinitionView
as
	Select 
		indexID,
		stockID,
		weight,
		beta,
		correlationPrice,
		correlationVola,
		actionID,
		actionDate
	from IndexDefinition
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

CREATE    view dbo.vALLStockEOD
AS
SELECT 	C.contractID AS iContractID,   
		C.contractTypeID AS iContractTypeID,    
		C.Symbol AS vcSymbol, 
    	S.divFreq AS iDivFreq, 
		S.divAmt AS fDivAmt,  
		S.divDate AS dtDivDate,
		S.isHTB as bIsHTB 
	FROM ContractView C (nolock), StockView S (nolock)
	WHERE S.stockID = C.contractID
UNION
SELECT 	C.contractID AS iContractID,  
		C.contractTypeID AS iContractTypeID,
	    C.Symbol AS vcSymbol,  
		NULL AS iDivFreq, 
		I.yield AS fDivAmt, 
		NULL AS dtDivDate,
		I.isHTB as bIsHTB 
	FROM ContractView C (nolock), IndexView I (nolock)
	WHERE I.indexID = C.contractID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE           Procedure dbo.usp_ActiveSurface_Get 
	@vcSymbolName varchar(12) --the contract symbol
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iContractID int
		select @iContractID = contractID from Contract where symbol = @vcSymbolName 
	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID
	select	@iVolaSurfaceDataID as iVolaSurfaceDataID
/*
	select	volaSurfaceDataID as iVolaSurfaceDataID
		from VolaSurfaceDataView V
				join ContractView C on V.contractID=C.contractID
		where C.symbol = @vcSymbolName 
				and V.isDefaultSurface=1
*/
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

CREATE    Procedure dbo.usp_ActiveSurface_Save
	@vcSymbolName varchar(12), --the contract symbol
	@iVolaSurfaceDataID int -- the VolaSurfaceData row ID
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
--clear isDefaultSurface Status
	update VolaSurfaceData 
		set isDefaultSurface=0
		from VolaSurfaceData 
				join Contract on VolaSurfaceData.contractID=Contract.contractID
		where Contract.symbol = @vcSymbolName
	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
--set isDefaultSurface Status
	update VolaSurfaceData 
		set isDefaultSurface=1
		from VolaSurfaceData 
				join Contract on VolaSurfaceData.contractID=Contract.contractID
		where Contract.symbol = @vcSymbolName and volaSurfaceDataID=@iVolaSurfaceDataID
	
	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	set @error = @@error
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

CREATE Procedure dbo.usp_BookRuleItemMask_Del 
	@iBookRuleItemMaskID int 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from BookRuleItemMask table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete BookRuleItemMask	where bookRuleItemMaskID = @iBookRuleItemMaskID 
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
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

CREATE Procedure dbo.usp_BookRuleItemMask_Get 
	@iBookRuleItemID int = Null 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from BookRuleItemMask table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 
		bookRuleItemMaskID as iBookRuleItemMaskID,
		bookRuleItemID as iBookRuleItemID,
		maskName as vcMaskName
		from BookRuleItemMask	
		where (bookRuleItemID = @iBookRuleItemID or @iBookRuleItemID is null)
	set @error = @@error
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

CREATE Procedure dbo.usp_BookRuleItemMask_Save 
	@iBookRuleItemMaskID int = Null Output, --the row identifier
	@iBookRuleItemID int = Null,  			--the book rule item ID
	@vcMaskName varchar(255) = Null 		--the mask name
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into BookRuleItemMask
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if not exists(select * From BookRuleItemMask where bookRuleItemMaskID = @iBookRuleItemMaskID) 
			begin
				insert into BookRuleItemMask (bookRuleItemID,maskName) 
				values (@iBookRuleItemID,@vcMaskName)
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iBookRuleItemMaskID = @@identity
			end
		else
			begin
				update BookRuleItemMask set 
					bookRuleItemID = isnull(@iBookRuleItemID,bookRuleItemID),
					maskName = isnull(@vcMaskName, maskName)
				where bookRuleItemMaskID = @iBookRuleItemMaskID
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

CREATE Procedure dbo.usp_BookRuleItem_Del 
	@iBookRuleItemID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from BookRuleItem table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete BookRuleItem	where bookRuleItemID = @iBookRuleItemID 
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
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

CREATE  Procedure dbo.usp_BookRuleItem_Get 
	@iBookID int = Null  
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from BookRuleItem table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 
		bookRuleItemID as iBookRuleItemID,
		itemName as vcItemName,
		BookRuleItem.ruleItemTypeID as iRuleItemTypeID,
		bookID as iBookID,
		itemTypeName as vcItemTypeName 
		from BookRuleItem
				join RuleItemType on BookRuleItem.ruleItemTypeID=RuleItemType.ruleItemTypeID
		where (BookRuleItem.bookID=@iBookID)
	set @error = @@error
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

CREATE Procedure dbo.usp_BookRuleItem_Save 
	@iBookRuleItemID int = Null Output, --the row identifier
	@vcItemName varchar(255) = Null, --the name of item
	@iRuleItemTypeID int = Null,  --the type ID
	@iBookID int -- the parent table row identifier
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save information into BookRuleItem table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if not exists(select * From BookRuleItem where bookRuleItemID = @iBookRuleItemID) 
			begin
				insert into BookRuleItem (bookID,ruleItemTypeID,itemName) 
					values (@iBookID,@iRuleItemTypeID,@vcItemName)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iBookRuleItemID = @@identity
			end
		else
			begin
				update BookRuleItem set 
					bookID = isnull(@iBookID, bookID),
					itemName = isnull(@vcItemName, itemName),
					ruleItemTypeID = isnull(@iRuleItemTypeID, ruleItemTypeID)
				where bookRuleItemID = @iBookRuleItemID
	
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

CREATE procedure usp_BookRule_GetAll
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select B.BookName as PortfolioName,
			BRI.ItemName , 
			M.MaskName as Mask, 
			T.itemTypeName as TypeName
			from BookView b
				join BookRuleItem BRI on B.bookID = BRI.bookID
				join RuleItemType T on T.ruleItemTypeID = BRI.ruleItemTypeID
				join BookRuleItemMask M on BRI.bookRuleItemID = M.bookRuleItemID
			order by B.Priority
	set @error = @@error
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

Create  proc usp_BookSwap_Save
		@iBook1ID int, 
		@iBook2ID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure saves priorities
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		Declare @iPriority1 int,@iPriority2 int
		select @iPriority1 = priority from BookView where bookID=@iBook1ID
		select @iPriority2 = priority from BookView where bookID=@iBook2ID
		update Book set priority = @iPriority1 where bookID=@iBook2ID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update Book set priority = @iPriority2 where bookID=@iBook1ID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
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

CREATE   proc usp_Book_Del
		@iBookID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		declare @iPriority int
			select @iPriority = priority from Book where bookID=@iBookID
		update Book set priority = priority-1 where priority > @iPriority
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	
		update Book set actionID=3 where bookID = @iBookID
--		delete Book where bookID = @iBookID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
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

CREATE     proc usp_Book_Get
	@iBookID int = Null,
	@bInTrades bit = 0,
	@bShortFormat bit = 0,
	@dtActionDate datetime = null 
as
/*
	Edited by Zaur Nuraliev
	Description: this procedure is getting information from Book table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if (@iBookID is Null)
		begin
			if (@bInTrades = 0 and @bShortFormat=0)
				select
					bookID as iBookID,
					bookName as vcBookName,
					[Description] as vcDescription,
					priority as iPriority,
					actionDate as dtActionDate,
					actionID as tiActionID
				from BookView
				where
					(actionDate>=@dtActionDate or @dtActionDate is null)
					and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
				order by priority
			else if (@bInTrades = 0 and @bShortFormat=1)
				select
					bookID as iBookID,
					bookName as vcBookName,
					actionDate as dtActionDate,
					actionID as tiActionID
				from BookView
				where
					(actionDate>=@dtActionDate or @dtActionDate is null)
					and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
				order by bookName
			else
				select
					bookID as iBookID,
					bookName as vcBookName,
					[Description] as vcDescription,
					priority as iPriority,
					actionDate as dtActionDate,
					actionID as tiActionID
				from BookView
				where exists (select tr.bookID from TradeView tr  where tr.bookID = BookView.bookID)
					and (actionDate>=@dtActionDate or @dtActionDate is null)
					and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
				order by priority
		end
	else
		begin
			select
				bookID as iBookID,
				bookName as vcBookName,
				[Description] as vcDescription,
				priority as iPriority,
				actionDate as dtActionDate,
				actionID as tiActionID
			from BookView
			where bookID = @iBookID
				and (actionDate>=@dtActionDate or @dtActionDate is null)
				and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
		end
	set @error = @@error
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

CREATE     proc usp_Book_Save
		@iBookID int = null,
		@vcBookName varchar(12) = null,
		@vcDescription varchar(120) = null
as
	----------------------------------------------------
	set nocount on
	----------------------------------------------------
	if exists (select * from Book where BookName = @vcBookName and actionID < 3 and (@iBookID <> bookID or @iBookID is null))
		begin
			Raiserror ('Book with this name already exists!', 16, 1)
			Return (-1)
		end   
	Declare @iPriority int
		select @iPriority = max(priority) + 1 from Book
	if @iPriority is null
		set @iPriority = 1
   
	if (@iBookID is Null)  
		begin
			insert into Book
				(bookName, [Description],priority)
			values
				(@vcBookName, @vcDescription, @iPriority)
	
			if (@@error <> 0)
				return (-1)
			else	
				return @@identity
		end
	else 
		begin
			update Book
			set bookName = @vcBookName,
				[Description] = @vcDescription
			where bookID = @iBookID
	
			if (@@error <> 0)
				return (-1)
			else  
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

CREATE proc usp_BrokerRate_Get
	@iBrokerID int,
	@dtValueDate smalldatetime
as
	set @dtValueDate = cast(floor(cast(@dtValueDate as float)) as datetime)
	select top 1
		commissionValue as fRate
	from CommissionView 
	where brokerID = @iBrokerID and commissionDate <= @dtValueDate
	order by commissionDate desc
/*	select
		IDENTITY(int, 1,1) AS ID,
		c.commissionDate,
		c.commissionValue
	into #s 
	from Commission c
	where c.brokerID = @iBrokerID
	order by c.commissionDate
	select @fRate = Rates.commissionValue
	from
		(select s1.commissionValue, s1.commissionDate 'Dt1', s2.commissionDate 'Dt2'
		from #s s1, #s s2
		where s2.id =* s1.id + 1) as Rates
	where (Rates.Dt1 <= @dtValueDate) and ((@dtValueDate < Rates.Dt2) or (Rates.Dt2 is Null))
	
	drop table #s */
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

CREATE        proc usp_Broker_Del
		@iBrokerID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Broker table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
--	delete Commission where brokerID = @iBrokerID
	update Commission set actionID=3 where brokerID = @iBrokerID
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
--	delete Trade where brokerID = @iBrokerID
	update Trade set actionID=0 where brokerID = @iBrokerID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	update Broker set actionID=3 where brokerID = @iBrokerID
--	delete Broker where brokerID = @iBrokerID
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
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

create    proc dbo.usp_Broker_Get
	@tiClearing tinyint = Null,
	@iBrokerID int = Null,
	@bInTrades bit = 0,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Broker table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 	brokerID as iBrokerID,
			brokerName as vcBrokerName,
			[Description] as vcDescription,
			isClearing as tiIsClearing,
			actionDate as dtActionDate,
			actionID as tiActionID
			from Broker
			where (isClearing = @tiClearing or @tiClearing is null)
				and (brokerID = @iBrokerID or @iBrokerID is Null)
				and (exists(select tr.clearingBrokerID from Trade tr where tr.clearingBrokerID=Broker.brokerID) or @bInTrades = 0)
				and (actionDate>=@dtActionDate or @dtActionDate is null)
				and (@dtActionDate is not null or @dtActionDate is null and actionID<3)				
			order by BrokerName
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

CREATE    proc usp_Broker_Save
	   @iBrokerID int,
	   @vcBrokerName varchar(12) = null,
	   @vcDescription varchar(120) = null ,
	   @tiClearing tinyint 
  as
	if exists (select * from Broker where BrokerName = @vcBrokerName and actionID < 3 and (@iBrokerID <> brokerID or @iBrokerID is null))
		begin
			Raiserror ('Broker with this name already exists!', 16, 1)
			Return (-1)
		end   
	if @iBrokerID is null 
	--if not exists(select * from Broker where brokerID=@iBrokerID)	  
		begin
		   insert into Broker (BrokerName, [Description], isClearing)
			  values (@vcBrokerName, @vcDescription, @tiClearing)
		   
		   if (@@error <> 0)   
			  return (-1)
			else
			  return @@identity
		end
	   else
		begin 
		   update Broker
			   set BrokerName = @vcBrokerName,
					 [Description] = @vcDescription,
					 isClearing = @tiClearing
			where brokerID = @iBrokerID
		  
		   if (@@error <> 0)   
			  return (-1)
			else
			  return 0
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

Create procedure usp_ClearingHouseTradeByCacheID_Get
 	@iCacheID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now int
		exec usp_CurrentDate_Get @Now output
	select * 
		from ClearingHouseTrade 
		where iDateOfTrade = @Now and iCacheID = @iCacheID
	set @error = @@error
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

Create procedure usp_ClearingHouseTrade_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now integer
		exec usp_CurrentDate_Get @Now output
	select * from ClearingHouseTrade where iDateOfTrade = @Now
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

Create procedure usp_ClearingHouseTrade_Save
				 	@iCacheID     int ,
					@ins_upd_del_flag tinyint,
					@put_call	 tinyint,
					@buy_sell tinyint,
					@origin   tinyint,
					@open_close tinyint,
					@trade_id   varchar(10),
					@user_id    varchar(10),
					@trading_sym varchar(7),
					@base_sym	varchar(7),
					@parent_account varchar(11),
					@account varchar(11),
					@q_account varchar(11),
					@security_type varchar(3),
					@security_subtype varchar(3),
					@clearing_firm varchar(5),
					@cusip varchar(10),
					@basket_id varchar(6),
					@giveup_firm varchar(5),	
					@contra_firm varchar(5),
					@exec_broker varchar(4),
					@contra_broker varchar(4),
					@mmcs_sub_acct varchar(3),
					@exercise_multiplier float(53),
					@extended_premium float(53),
					@premium float(53),
					@strike float(53),
					@price float(53),
					@quantity int,
					@tag int,
					@expiration_date int,
					@execution_date int,
					@last_upd_date int,
					@contract_date int,
					@source_system int ,
					@trade_id_num int ,
					@trade_group_id int ,
					@settlement_date int,
					@entry_firm varchar(5),
					@matched_flag tinyint,
					@spread_ind tinyint,
					@premium_multiplier float(53),
					@source_code varchar(3),
					@parent_acct_intrl_id varchar(14),
					@acct_intrl_id varchar(14),
					@q_acct_intrl_id varchar(14),
					@cap_interval float(53),
					@trade_exchange int ,
					@settled_unsettled_ind tinyint,
					@stock_origin tinyint,
					@mm_card_number varchar(8),
					@basket_tag int ,
					@basket_price float(53),
					@short_sale tinyint,
					@commission_code varchar(3),
					@commission float(53),
					@same_day_exer tinyint,
					@terminal_name varchar(11),
					@portfolio_name	varchar(255)
as
	Declare @Now integer
		exec usp_CurrentDate_Get @Now output
if exists (select * from ClearingHouseTrade where iDateOfTrade = @Now and iCacheID = @iCacheID)
	Return(0)
insert into ClearingHouseTrade values
(
	@Now,
	@iCacheID,
	@ins_upd_del_flag,
	@put_call	 ,
	@buy_sell ,
	@origin  , 
	@open_close ,
	@trade_id   ,
	@user_id    ,
	@trading_sym ,
	@base_sym	,
	@parent_account ,
	@account ,
	@q_account ,
	@security_type ,
	@security_subtype ,
	@clearing_firm ,
	@cusip ,
	@basket_id ,
	@giveup_firm ,	
	@contra_firm ,
	@exec_broker ,
	@contra_broker ,
	@mmcs_sub_acct ,
	@exercise_multiplier ,
	@extended_premium ,
	@premium ,
	@strike ,
	@price ,
	@quantity ,
	@tag,
	@expiration_date ,
	@execution_date ,
	@last_upd_date ,
	@contract_date ,
	@source_system ,
	@trade_id_num ,
	@trade_group_id ,
	@settlement_date ,
	@entry_firm ,
	@matched_flag ,
	@spread_ind ,
	@premium_multiplier ,
	@source_code ,
	@parent_acct_intrl_id ,
	@acct_intrl_id ,
	@q_acct_intrl_id ,
	@cap_interval ,
	@trade_exchange ,
	@settled_unsettled_ind ,
	@stock_origin ,
	@mm_card_number ,
	@basket_tag ,
	@basket_price ,
	@short_sale ,
	@commission_code ,
	@commission ,
	@same_day_exer ,
	@terminal_name,
	@portfolio_name
)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc usp_Commission_Del
		@iCommissionID int,
		@iBrokerID int = Null
	  
  as
	   if (@iCommissionID is Null) and (@iBrokerID is Null)
		  return (-2)
	
	   if (@iCommissionID is Null) 
			update Commission set actionID=3 where brokerID = @iBrokerID
		  --delete Commission where brokerID = @iBrokerID
		else
			update Commission set actionID=3 where commissionID = @iCommissionID
		  --delete Commission where commissionID = @iCommissionID
   if (@@error <> 0)
	  return (-1)
	else
	  return 0				   

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc usp_Commission_Get
	@iBrokerID int = null,
	@iCommissionID int = null,
	@dtActionDate datetime = null
  as
	   if (@iCommissionID is Null)
		  begin
			  if (@iBrokerID is Null)
				 select commissionID as iCommissionID,
					brokerID as iBrokerID,
					commissionDate as dtCommissionDate,
					commissionValue as fCommissionValue,
					actionDate as dtActionDate,
					actionID as tiActionID 
				from Commission
				where	(actionDate>=@dtActionDate or @dtActionDate is null and actionID < 3)
				order by brokerID, commissionDate
			   else
					select commissionID as iCommissionID,
						brokerID as iBrokerID,
						commissionDate as dtCommissionDate,
						commissionValue as fCommissionValue,
						actionDate as dtActionDate,
						actionID as tiActionID 
					from Commission
					where brokerID = @iBrokerID and 
						(actionDate>=@dtActionDate or @dtActionDate is null and actionID < 3)
					order by commissionDate 	 
		  end
		else 
			select commissionID as iCommissionID,
					brokerID as iBrokerID,
					commissionDate as dtCommissionDate,
					commissionValue as fCommissionValue,
					actionDate as dtActionDate,
					actionID as tiActionID 
				from Commission 
				where commissionID = @iCommissionID and 
					(actionDate>=@dtActionDate or @dtActionDate is null and actionID < 3)
  Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_Commission_Save
	   @iCommissionID int,
	   @iBrokerID int,
	   @dtCommissionDate smalldatetime,
	   @fCommissionValue float
	  
  as
	if exists (select * from Commission where brokerID = @iBrokerID and commissionDate = @dtCommissionDate and 
				actionID < 3 and (@iCommissionID <> commissionID or @iCommissionID is null))
		begin
			Raiserror ('Record with this commissionDate and Broker already exists!', 16, 1)
			Return (-1)
		end  
	   if (@iCommissionID is Null)
		  begin
			  insert into Commission (brokerID, commissionDate, commissionValue)
				 values (@iBrokerID, @dtCommissionDate, @fCommissionValue)
			 
			  if (@@error <> 0)  
				 return (-1)
			   else
				 return @@identity	
		  end  
		else
		  begin 	   
			  update Commission
				   set brokerID = @iBrokerID,
						 commissionDate = @dtCommissionDate,
						 commissionValue = @fCommissionValue 
			   where commissionID = @iCommissionID 	 
			  if (@@error <> 0)  
				 return (-1)
			   else
				 return 0
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

CREATE    Procedure dbo.usp_ContractByGroupID_Get 
	@iGroupID int =Null --the group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return information about 
				contract rule and group
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iGroupID Is Not Null 
		select	Contract.contractID as iContractID,
				Contract.symbol as vcSymbolName,
				GRG.groupID as iGroupID,
				Contract.ruleID as iRuleID 
			from ContractView Contract 
					join GenerationRuleGroup GRG on GRG.groupID=Contract.groupID
			where Contract.contractTypeID < 3 
					and GRG.groupID = @iGroupID
			order by Contract.symbol asc
	else
		select	Contract.contractID as iContractID,
				Contract.symbol as vcSymbolName,
				GRG.groupID as iGroupID,
				Contract.ruleID as iRuleID 
			from ContractView Contract 
					left join GenerationRuleGroup GRG on GRG.groupID=Contract.groupID
			where Contract.contractTypeID < 3 
			order by Contract.symbol asc
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

CREATE	proc usp_ContractByID_Get 
		@iContractID int,
		@vcExchangeIDs varchar(8000) = Null --the Exchange identifier
as
/*
	Changed by Zaur Nuraliev
	[Description]: 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @vcExchangeIDs Is Null or @vcExchangeIDs=''
		begin
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c 
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where c.contractID = @iContractID  
						and C.contractTypeID in (1, 2)
			union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c  
						join OptionView O on O.optionID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractID = @iContractID  
					and C.contractTypeID not in (1, 2)
		end
	else
		begin
		--===================================================================================
		-- decompose ID's from varchat string
		--===================================================================================
			declare @vcExchangeID varchar(100),
					@i int
			create table #Exchange(exchangeID int not Null)
			
			select @vcExchangeID='', @i=1
			while @i <= len(@vcExchangeIDs) 
				begin
					if substring(@vcExchangeIDs,@i,1)<>'|' 
						begin
							set @vcExchangeID=@vcExchangeID + substring(@vcExchangeIDs,@i,1)
							if len(@vcExchangeIDs)=@i
								insert into #Exchange values(@vcExchangeID)
						end
					else --if substring(@vcExchangeIDs,@i,1)='|' 
						begin
							insert into #Exchange values(@vcExchangeID)
							set @vcExchangeID=''
						end
					set @i=@i+1
				end
			if len(@vcExchangeIDs)=1 
				insert into #Exchange values(@vcExchangeID)
		
		--===================================================================================
		-- make output recordset
		--===================================================================================
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c  
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractID = @iContractID  
						and C.contractTypeID in (1, 2) and exchangeID in (select ExchangeID from #Exchange)
			union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID
				from ContractView c  
						join OptionView O on O.optionID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractID = @iContractID  
						and c.contractTypeID not in (1, 2)
						and exchangeID in (select ExchangeID from #Exchange)
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

CREATE	proc usp_ContractBySymbol_Get
		@vcSymbol varchar(8)
as
begin
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where C.symbol = @vcSymbol	
					and c.contractTypeID in (1, 2)
	union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					join OptionView O on O.optionID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID				
		   where c.symbol = @vcSymbol  
					and c.contractTypeID not in (1, 2)
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

CREATE proc usp_ContractBySymbol_Save
	  @iContractID int,
	  @vcSymbol varchar(8)
	  
  as
begin 
	update Contract
		 set symbol = @vcSymbol
	  where contractID = @iContractID
	return @@error
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

CREATE proc usp_ContractExchangeLink_Get
	@iContractID int = Null
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select contractID as iContractID,
			exchangeID as iExchangeID,
			exchangeLinkType as vcExchangeLinkType,
			ContractExchangeLinkID as iContractExchangeLinkID
		from ContractExchangeLink
		where contractID=@iContractID or (@iContractID is null)
		
	set @error = @@error
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

CREATE   proc usp_ContractForTrade_Get
	@iContractID int = null,
	@iUnderlyingID int = null,
	@tiNewOnly tinyint = 0
as
	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
	select
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize,
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
			c.symbol as vcUnderlyingSymbol,
			cp.priceBid as fUnderlyingPriceBid,
			cp.priceAsk as fUnderlyingPriceAsk,
			cp.lotSize as iUnderlyingLotSize
	from ContractView c  
			left join StockView S on S.stockID = C.contractID
			left join IndexView I on I.indexID = C.contractID
			left join ContractPrice CP on C.contractID = CP.contractID
			--left join (select Dividend.stockID, Dividend.DivDate, DivAmt from Dividend 
			--			inner join (select stockID, max(isnull(DivDate, '19000101')) DivDate
			--			from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
			--		d on s.stockID = d.stockID
	where	
			(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
			and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
			and c.contractTypeID in (1, 2)
			and CP.exchangeID is null
	union
	select	
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize,
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
			uc.symbol as vcUnderlyingSymbol,
			ucp.priceBid as fUnderlyingPriceBid,
			ucp.priceAsk as fUnderlyingPriceAsk,
			ucp.lotSize as iUnderlyingLotSize
	from ContractView c  
			left join OptionView O on O.optionID = C.contractID
			left join Contract uc on o.underlyingContractID = uc.contractID
			left join ContractPrice CP on C.contractID = CP.contractID
			left join ContractPrice ucp on uc.contractID = ucp.contractID
			left join StockView s on uc.contractID = s.stockID
			left join IndexView i on uc.contractID = i.indexID			
			--left join (select Dividend.stockID, Dividend.DivDate, DivAmt from Dividend 
			--			inner join (select stockID, max(isnull(DivDate, '19000101')) DivDate
			--			from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
			--		d on s.stockID = d.stockID
	where
			(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
			and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
			and (@tiNewOnly = 0 or @tiNewOnly = 1 and O.expiry >= @dtToday)
			and c.contractTypeID = 3
			and CP.exchangeID is null
			and ucp.exchangeID is null
	order by C.symbol
	Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE             Procedure dbo.usp_ContractGenerationRule_Get 
	@iContractID int --the contract identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return ContractGenerationRule information 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		select 
			GR.ruleID as iRuleID,
			GR.ruleType as iRuleType,
			GR.ruleName as vcRuleName, 
			GR.interpolationFactor as fInterpolationFactor, 
			GR.callShift as fCallShift , 
			GR.putShift as fPutShift,
			GR.requiredPoints as iRequiredPoints,
			GR.smileAccelerator as fSmileAccelerator , 
			GR.strikeIncrement as iStrikeIncrement, 
			GR.strikeMoneynessLo as fStrikeMoneynessLo,
			GR.strikeMoneynessHi as fStrikeMoneynessHi,
			GR.isDefaultRule as bIsDefaultRule,
			case when
					exists(select * from ContractView c  join GenerationRuleGroup GRG on C.groupID=GRG.groupID 
							where GRG.ruleID=GR.RuleID and C.ContractID=@iContractID) then 1
				else 0
			end as bIsGroupRule,
			case when exists(select * from Contract where Contract.ruleID=GR.RuleID and contractID=@iContractID) then 1
				else 0
			end as bIsContractRule
		from GenerationRule GR
		order by ruleName 
	if @@error <> 0 begin set @error = @@error goto finish end
/*
	if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[result_values]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
		drop table [dbo].[result_values]
	if @@error <> 0 begin set @error = @@error goto finish end
	Create table  dbo.result_values(iRuleID int not null,
									iRuleType int null, 	
									vcRuleName varchar(250) null,
									fInterpolationFactor float null,
									fCallShift float null,
 									fPutShift float null,
									iRequiredPoints int null, 
									fSmileAccelerator float null, 
									iStrikeIncrement int null, 
									fStrikeMoneynessLo float null,
									fStrikeMoneynessHi float null, 
									bIsDefaultRule bit not null, 
									bIsGroupRule bit not null, 
									bIsContractRule bit not null)
		if @@error <> 0 begin set @error = @@error goto finish end
	-- Declare and using a READ_ONLY cursor
	Declare GenerationRule_Cursor Cursor
		READ_ONLY
		For Select ruleID from GenerationRule order by ruleName 
	
	Declare @iRuleID int
		Open GenerationRule_Cursor
	
	Fetch Next From GenerationRule_Cursor Into @iRuleID
	While (@@fetch_status <> -1)
	begin
		if (@@fetch_status <> -2)
		begin
			insert into result_values
			select 
				GR.ruleID as iRuleID,
				GR.ruleType as iRuleType,
				GR.ruleName as vcRuleName, 
				GR.interpolationFactor as fInterpolationFactor, 
				GR.callShift as fCallShift , 
				GR.putShift as fPutShift,
				GR.requiredPoints as iRequiredPoints,
				GR.smileAccelerator as fSmileAccelerator , 
				GR.strikeIncrement as iStrikeIncrement, 
				GR.strikeMoneynessLo as fStrikeMoneynessLo,
				GR.strikeMoneynessHi as fStrikeMoneynessHi,
				GR.isDefaultRule as bIsDefaultRule,
				case when
						exists(select * from ContractView c  join GenerationRuleGroup GRG on C.groupID=GRG.groupID 
								where GRG.ruleID=@iRuleID and C.ContractID=@iContractID) then 1
					else 0
				end as bIsGroupRule,
				case when exists(select * from Contract where Contract.ruleID=@iRuleID and contractID=@iContractID) then 1
					else 0
				end as bIsContractRule
			from GenerationRule GR
			where GR.ruleID=@iRuleID
		end
		Fetch Next From GenerationRule_Cursor into @iRuleID
	end
	
	Close GenerationRule_Cursor
	Deallocate GenerationRule_Cursor
		if @@error <> 0 begin set @error = @@error goto finish end
	select * from result_values
		if @@error <> 0 begin set @error = @@error goto finish end
	if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[result_values]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
		drop table [dbo].[result_values]
	if @@error <> 0 begin set @error = @@error goto finish end
*/
/*
	select distinct G.ruleID as iRuleID,
			ruleType as iRuleType,
			ruleName as vcRuleName, 
			interpolationFactor as fInterpolationFactor, 
			callShift as fCallShift , 
			putShift as fPutShift,
			requiredPoints as iRequiredPoints,
			smileAccelerator as fSmileAccelerator , 
			strikeIncrement as iStrikeIncrement, 
			strikeMoneynessLo as fStrikeMoneynessLo,
			strikeMoneynessHi as fStrikeMoneynessHi,
			isDefaultRule as bIsDefaultRule,
			Case when (CC.groupID is not null) and (GRGC.ruleID=G.ruleID) and (CC.ContractID=@iContractID) then 1
				else 0
			end as bIsGroupRule,
			Case when ((C.ruleID is not null) and (C.ContractID=@iContractID)) then 1
				else 0
			end as bIsContractRule
		from  GenerationRule G 
				left join ContractView c  on G.ruleID=C.ruleID
				left join GenerationRuleGroup GRGC on G.ruleID=GRGC.ruleID
				left join ContractView c C on (GRGC.groupID=CC.groupID and CC.ContractID=@iContractID)
		order by ruleName 
*/
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

CREATE     Procedure dbo.usp_ContractGroupItem_Save
	@iContractID int,		--the stock identifier
	@iGroupID int=Null		--the group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save group identifier
			      
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iContractID is not null
	--if exists(select * from Contract where contractID=@iContractID)
		begin
			update Contract set groupID = @iGroupID
			where contractID=@iContractID
		end 
/*	else
		begin
			insert into VolaManagementData(contractID,basePoint,skewDelta,interpolationFactor,groupID)
				values(@iContractID,0,0,0,@iGroupID)
		end
*/
	set @error = @@error
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

CREATE  Procedure dbo.usp_ContractGroup_Del
	@iGroupID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update ContractGroup set actionID=3 where groupID = @iGroupID
	--delete ContractGroup where groupID = @iGroupID
	set @error = @@error
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

CREATE    Procedure dbo.usp_ContractGroup_Get
	@iGroupID int = null,
    @bShortFormat bit = 0,
	@dtActionDate datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if (@iGroupID is Null and @bShortFormat = 0)
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
			from ContractGroup
			where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
			order by groupName
	else if (@iGroupID is Null and @bShortFormat = 1)
		select groupID as iGroupID, 
			groupName as vcGroupName,
			actionDate as dtActionDate,
			actionID as tiActionID 
			from ContractGroup
			where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
			order by groupName
	else 		 
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
			from ContractGroupView
			where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				and groupID = @iGroupID
	set @error = @@error
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

CREATE  Procedure dbo.usp_ContractGroup_Save
	@iGroupID int out,
	@vcGroupName varchar(30),
	@vcDescription varchar(120)
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from ContractGroup where groupName = @vcGroupName and actionID < 3 and (@iGroupID <> groupID or @iGroupID is null))
		begin
			Raiserror ('Contract group with this name already exists!', 16, 1)
			Return (-1)
		end   
	if @iGroupID is Null 
		begin
			insert into ContractGroup (
					groupName,
					[Description]) 
				values (
					@vcGroupName,
					@vcDescription)
			select @iGroupID = @@identity
		end
	else
		begin
			update ContractGroup set
				groupName = isNull(@vcGroupName, groupName),
				[Description] = isNull(@vcDescription, [Description])
			where @iGroupID = groupID
		end
	set @error = @@error
	if (@error = 0) set @error = @iGroupID
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

create proc usp_ContractIDForTrade_Get
		@iContractID int,
		@iUnderlyingID int
  as
		
	  if (@iContractID is Null)
		  if (@iUnderlyingID is Null)
			  select c.contractID as iContractID
				 from ContractView c , IndexView i
			   where c.contractTypeID = 1
				   and i.indexID = c.contractID
				 union
			  select c.contractID as iContractID
				 from ContractView c , Stock s
			   where c.contractTypeID = 2
				   and s.stockID = c.contractID				
				 union
			  select c.contractID as iContractID
				 from ContractView c , OptionView o
			   where c.contractTypeID = 3
				   and o.optionID = c.contractID
		  else
			  select c.contractID as iContractID
				 from ContractView c , IndexView i
			   where c.contractID = @iUnderlyingID
				   and c.contractTypeID = 1
				   and i.indexID = c.contractID
				 union
			  select c.contractID as iContractID
				 from ContractView c , Stock s
			   where c.contractID = @iUnderlyingID
				  and c.contractTypeID = 2
				  and s.stockID = c.contractID			   
				 union
			  select c.contractID as iContractID
				 from ContractView c , OptionView o
			   where o.underlyingContractID = @iUnderlyingID
				  and o.optionID = c.contractID
				  and c.contractTypeID = 3
	  else
			  select c.contractID as iContractID
				 from ContractView c , IndexView i
			   where c.contractID = @iContractID
				   and c.contractTypeID = 1
				   and i.indexID = c.contractID
				 union
			  select c.contractID as iContractID
				 from ContractView c , Stock s
			   where c.contractID = @iContractID
				   and c.contractTypeID = 2
				   and s.stockID = c.contractID				
				 union
			  select c.contractID as iContractID
				 from ContractView c , OptionView o
			   where c.contractID = @iContractID
				   and c.contractTypeID = 3
				   and o.optionID = c.contractID
		
		   Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE     Procedure dbo.usp_ContractInExchange_Del 
	@iExchangeID int,        --the exchangeID
	@iContractID int   	 --the contractID
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure remove exchange link type data from ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		delete from ContractExchangeLink where  contractID=@iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
    set @error = @@Error
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

CREATE      procedure usp_ContractInExchange_Get
	@iContractID int,
	@bFullInfo tinyint = 1 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice=Exchange table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if @bFullInfo <> 0
		select E.exchangeID as iExchangeID,
				E.ExchangeName as vcExchangeName,
				isnull(convert(bit,left(CP.exchangeLinkType,1)),0) as iType1,
				isnull(convert(bit,right(CP.exchangeLinkType,1)),0) as iType2
			from ContractExchangeLink CP, ExchangeView E
			where CP.contractID = @iContractID and CP.exchangeID =* E.exchangeID
			order by E.ExchangeName
	else
		select cp.exchangeID as iExchangeID,
				isnull(convert(bit,left(CP.exchangeLinkType,1)),0) as iType1,
				isnull(convert(bit,right(CP.exchangeLinkType,1)),0) as iType2
			from ContractExchangeLink CP
					join ExchangeView on CP.exchangeID=ExchangeView.exchangeID
			where CP.contractID = @iContractID and CP.exchangeLinkType <> 0
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

CREATE     Procedure dbo.usp_ContractInExchange_Save 
	@iExchangeID int,        --the exchangeID
	@iContractID int,   	 --the contractID
	@itype1 bit=null, 	--the link type 1
	@itype2 bit=null 	--the link type 2
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @itype1 is null 
		select @itype1=left(exchangeLinkType,1) from ContractExchangeLink where contractID=@iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
	set @itype1=isnull(@itype1,0)
	if @itype2 is null 
		select @itype2=right(exchangeLinkType,1) from ContractExchangeLink where contractID=@iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
	set @itype2=isnull(@itype2,0)
	declare @vcExchangeLinkType varchar(2)
		set @vcExchangeLinkType = convert(varchar(1),@itype1) + convert(varchar(1),@itype2)
--select @vcExchangeLinkType
	Begin Transaction
		if not exists(select * from ContractExchangeLink where isnull(exchangeID,0)=isnull(@iExchangeID,0) and contractID=@iContractID) 
			begin
				insert into ContractExchangeLink (contractID,exchangeID,exchangeLinkType) 
					values (@iContractID,@iExchangeID,@vcExchangeLinkType)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update ContractExchangeLink set 
					exchangeLinkType = @vcExchangeLinkType
				where @iContractID = @iContractID and isnull(exchangeID,0) =isnull(@iExchangeID,0)
	
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

CREATE  proc usp_ContractInGroupByContract_Del
		  @iContractID int,
		  @iGroupID int = Null
 as
begin
   if (@iContractID is Null) return (-2)
   if (@iGroupID is Null)	 
		update ContractInGroup set actionID=3 where contractID = @iContractID
	  --delete ContractInGroup where contractID = @iContractID
	else
		update ContractInGroup set actionID=3 where contractID = @iContractID and groupID = @iGroupID
	  --delete ContractInGroup where contractID = @iContractID and groupID = @iGroupID
 
   if (@@error <> 0)
	  return (-1)
   else
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

CREATE proc usp_ContractInGroupByContract_Get
		  @iContractID int
as
  select g.groupID as iGroupID,
	g.groupName as vcGroupName,
	CASE WHEN cig.contractID is Null THEN 0 ELSE 1 END as iMark
	   from ContractGroupView g, ContractInGroupView cig
	 where cig.contractID = @iContractID
		and cig.groupID =* g.groupID
	  order by g.groupName
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

CREATE   proc usp_ContractInGroupByContract_Save
		  @iContractID int,
		  @iGroupID int
 as
	if exists (select * from ContractInGroup  where contractID = @iContractID and groupID = @iGroupID and actionID < 3)
		begin
			--Raiserror ('Row with same identifiers already exists!', 16, 1)
			Return (-1)
		end   
   insert into ContractInGroup (contractID, groupID) values (@iContractID, @iGroupID)
   if (@@error <> 0)
	  return (-1)
   else
	  return 0	 

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE   PROCEDURE usp_ContractInGroup_Get
	@iGroupID int = null,
	@iContractID int = null,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractInGroup table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 	groupID iGroupID, 
			contractID iContractID,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ContractInGroup
		where ((@iGroupID is null or @iGroupID is not null and groupID = @iGroupID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
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

CREATE Procedure dbo.usp_ContractMarketData_Del
	@iContractID int		-- Contract's identifier
as
/*
	Created by Andrey Sinkin
	It deletes data from ContractMarketData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	delete ContractMarketData where contractID = @iContractID
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

CREATE Procedure dbo.usp_ContractMarketData_Get
	@iContractID int		-- Contract's identifier
as
/*
	Created by Andrey Sinkin
	It returns data from ContractMarketData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	select contractID icontractID, [date] dtDate, priceMid fPriceMid
		from ContractMarketData
		where contractID = @iContractID
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

CREATE Procedure dbo.usp_ContractMarketData_Save
	@iContractID int,		-- Contract's identifier
	@dtDate datetime,	-- date
	@fPriceMid float	-- middle price
as
/*
	Created by Andrey Sinkin
	It deletes data from ContractMarketData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @rowcount int
	if @dtDate is null		set @dtDate = getdate()
	Begin Transaction
		update ContractMarketData set priceMid = @fPriceMid, date = @dtDate where @iContractID = contractID
		select @error = @@error, @rowcount = @@rowcount
		if @error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		if @rowcount = 0
			begin
				insert into ContractMarketData (contractID, date, priceMid) values (@iContractID, @dtDate, @fPriceMid)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end			
			end
		set @error = 0
	Commit Transaction
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
		SUM(t.quantity) as iQuantity
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
		SUM(t.quantity) as iQuantity
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
		SUM(t.quantity) as iQuantity
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
		SUM(t.quantity) as iQuantity
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   procedure dbo.usp_ContractPriceEOD_Save
	@vcSymbol varchar(8),
	@fPriceClose float,
	@iContractTypeID int, --2-stock 3-option
	@vcExchangeCode varchar(10) = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1
	if (select object_id('tempdb..##ContractPriceEOD')) is not null
		begin
			insert into ##ContractPriceEOD (symbol, contractTypeID, priceClose, exchangeCode)
				values (@vcSymbol, @iContractTypeID, @fPriceClose, @vcExchangeCode)
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

CREATE   proc usp_ContractPriceHistory_Save
	@iContractID int,
	@iExchangeID int = Null, 
	@fPriceTheo float = Null,
	@dtPriceDate smalldatetime = Null
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
if @dtPriceDate is null 
	set @dtPriceDate=GetDate()
	Begin Transaction
		if not exists (select * from ContractPriceHistory where contractID = @iContractID and exchangeID=@iExchangeID and exchangeID is not null and priceDate=@dtPriceDate)
			and not exists (select * from ContractPriceHistory where contractID=@iContractID and @iExchangeID is null and priceDate=@dtPriceDate)
		 begin
			if exists(select * from ContractPrice where contractID = @iContractID and exchangeID=@iExchangeID and exchangeID is not null)
				and exists (select * from ContractPrice where contractID=@iContractID and @iExchangeID is null)
			begin
				insert into ContractPriceHistory (contractID,
							exchangeID, 
							priceBid,
							priceAsk,
							lotSize,
							priceLast,
							priceOpen,
							priceClose,
							priceLow,
							priceHigh,
							volume,
							openInterest,
							priceDate,
							priceTheo)
					select	@iContractID,
							@iExchangeID, 
							priceBid,
							priceAsk,
							lotSize,
							priceLast,
							priceOpen,
							priceClose,
							priceLow,
							priceHigh,
							volume,
							openInterest,
							@dtPriceDate,
							@fPriceTheo 
					from ContractPrice where ContractID=@iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
			else 
			begin
				if exists(select * from Contract where contractID=@iContractID) and (@iExchangeID is null or exists(select * from Exchange where exchangeID=@iExchangeID))
					insert into ContractPriceHistory (
								contractID,
								exchangeID, 
								priceDate,
								priceTheo)
						values (@iContractID,
								@iExchangeID, 
								@dtPriceDate,
								@fPriceTheo) 
			end		
		 end
	   else
		 begin
			update ContractPriceHistory
				set priceBid = CP.priceBid,
					priceAsk = CP.priceAsk,
					lotSize = CP.lotSize,
					priceLast = CP.priceLast,
					priceOpen = CP.priceOpen,
					priceClose = CP.priceClose,
					priceLow = CP.priceLow,
					priceHigh = CP.priceHigh,
					volume = CP.volume,
					openInterest = CP.openInterest,
					priceTheo = priceTheo
				from ContractPriceHistory join ContractPrice CP on ContractPriceHistory.contractID=CP.contractID 
					and isnull(ContractPriceHistory.exchangeID,0)=isnull(CP.exchangeID,0) 
				where (ContractPriceHistory.contractID=@iContractID and 
						ContractPriceHistory.exchangeID=@iExchangeID and ContractPriceHistory.priceDate=@dtPriceDate)
					or (ContractPriceHistory.contractID=@iContractID and @iExchangeID is null and ContractPriceHistory.priceDate=@dtPriceDate)
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_ContractPrice_Get 
	@iContractPriceID int = Null output,
	@iContractID int = Null,
	@iExchangeID int = Null 
AS
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	contractID as iContractID,
			exchangeID as iExchangeID, 
			priceBid as fPriceBid,
			priceAsk as fPriceAsk,
			lotSize as iLotSize,
			priceLast as fPriceLast,
			priceOpen as fPriceOpen,
			priceClose as fPriceClose,
			priceLow as fPriceLow,
			priceHigh as fPriceHigh,
			volume as iVolume,
			openInterest as iOpenInterest
			from ContractPrice	
			where	(@iContractPriceID is not Null and contractPriceID = @iContractPriceID) or 
					(@iContractID is not Null and contractID=@iContractID and exchangeID=@iExchangeID)
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

CREATE    proc usp_ContractPrice_Save
	@iContractPriceID int = Null output,
	@iContractID int = Null,
	@iExchangeID int = Null, 
	@fPriceBid float = Null,
	@fPriceAsk float = Null,
	@iLotSize int = Null,
	@fPriceLast float = Null,
	@fPriceOpen float = Null,
	@fPriceClose float = Null,
	@fPriceLow float = Null,
	@fPriceHigh float = Null,
	@iVolume int = Null,
	@iOpenInterest int = Null,
	@vcSymbol varchar(8) = Null,
	@iContractTypeID int = Null, --2-stock 3-option
	@vcExchangeCode varchar(10) = Null
as
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	set ANSI_NULLS OFF --(exchangeID may be null)
	-------------------------------------------------
	declare @error int		set @error = -1
	if @vcSymbol is not null and @iContractTypeID is not null 
		begin
			Select @iContractID = contractID from Contract where symbol=@vcSymbol and contractTypeID=@iContractTypeID
			if @iContractID is null goto finish 
		end
	if @vcExchangeCode is not null
		begin
			Select @iExchangeID=exchangeID from Exchange where exchangeCode=@vcExchangeCode
		end
	Begin Transaction
		if (not exists (select * from ContractPrice where contractPriceID = @iContractPriceID) and @iContractPriceID is not Null)
		or (@iContractID is not Null and not exists(select * from ContractPrice where contractID=@iContractID and isnull(exchangeID,0)=isnull(@iExchangeID,0)))
		 begin
			insert into ContractPrice (contractID,
						exchangeID, 
						priceBid,
						priceAsk,
						lotSize,
						priceLast,
						priceOpen,
						priceClose,
						priceLow,
						priceHigh,
						volume,
						openInterest)
				  values (@iContractID,
						@iExchangeID, 
						@fPriceBid,
						@fPriceAsk,
						@iLotSize,
						@fPriceLast,
						@fPriceOpen,
						@fPriceClose,
						@fPriceLow,
						@fPriceHigh,
						@iVolume,
						@iOpenInterest)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			select @iContractPriceID = @@identity
		 end
	   else
		 begin
			update ContractPrice
				set contractID = isNull(@iContractID,contractID),
					exchangeID = isNull(@iExchangeID,exchangeID),
					priceBid = isNull(@fPriceBid,priceBid),
					priceAsk = isNull(@fPriceAsk,priceAsk),
					lotSize = isNull(@iLotSize,lotSize),
					priceLast = isNull(@fPriceLast,priceLast),
					priceOpen = isNull(@fPriceOpen,priceOpen),
					priceClose = isNull(@fPriceClose,priceClose),
					priceLow = isNull(@fPriceLow,priceLow),
					priceHigh = isNull(@fPriceHigh,priceHigh),
					volume = isNull(@iVolume,volume),
					openInterest = isNull(@iOpenInterest,openInterest),
					priceDate = GetDate()
					where (@iContractPriceID is not Null and contractPriceID = @iContractPriceID) or 
						(@iContractID is not Null and contractID=@iContractID and exchangeID=@iExchangeID)
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
		--===========================================================================================
		-- create record with Null ExchangeID
		--===========================================================================================
		if not exists(select * from ContractPrice where ContractID=@iContractID and exchangeID is null)
			begin
				insert into ContractPrice (contractID,
							exchangeID, 
							priceBid,
							priceAsk,
							lotSize,
							priceLast,
							priceOpen,
							priceClose,
							priceLow,
							priceHigh,
							volume,
							openInterest)
					  values (@iContractID,
							Null, 
							@fPriceBid,
							@fPriceAsk,
							@iLotSize,
							@fPriceLast,
							@fPriceOpen,
							@fPriceClose,
							@fPriceLow,
							@fPriceHigh,
							@iVolume,
							@iOpenInterest)
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE proc usp_ContractStockAndIndex_Get
	 @iGroupID int,
	 @bIsAll bit,
	 @iContractTypeID int	 
	   
  as
		 
   if (@iGroupID is Null)
	  if (@bIsAll is not Null)	and   (@bIsAll > 0)
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.defaultModelTypeID as iDefaultModelTypeID,
				S.defaultSurfaceTypeID as iDefaultSurfaceTypeID,
				S.divFreq as iDivFreq, 
				S.baseIndexID as iBaseIndexID,
				Null as fYield
					from ContractView c 
							join Stock S on S.stockID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 2) or (@iContractTypeID is Null))
							and C.contractTypeID = 2
			union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				I.defaultModelTypeID as iDefaultModelTypeID,
				Null as iDefaultSurfaceTypeID,
				Null as iDivFreq, 
				Null as iBaseIndexID,
				i.yield as fYield
					from ContractView c 
							join IndexView I on I.indexID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 1) or (@iContractTypeID is Null))
							and c.contractTypeID = 1
					order by c.symbol	
	   else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.defaultModelTypeID as iDefaultModelTypeID,
				S.defaultSurfaceTypeID as iDefaultSurfaceTypeID,
				S.beta as fBeta,
				S.divFreq as iDivFreq, 
				S.baseIndexID as iBaseIndexID,
				Null as fYield
					from ContractView c  
							join Stock S on S.stockID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 2) or (@iContractTypeID is Null))
						and c.contractTypeID = 2
						and not exists (select * from ContractsInGroup where contractID = c.contractID)
		  union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				I.defaultModelTypeID as iDefaultModelTypeID,
				Null as iDefaultSurfaceTypeID,
				Null as fBeta,
				Null as iDivFreq, 
				Null as iBaseIndexID,
				i.yield as fYield
					from ContractView c  
							join IndexView I on I.indexID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 1) or (@iContractTypeID is Null))
							and c.contractTypeID = 1
							and not exists (select * from ContractsInGroup where contractID = c.contractID) 
					order by c.symbol	
	else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.defaultModelTypeID as iDefaultModelTypeID,
				S.defaultSurfaceTypeID as iDefaultSurfaceTypeID,
				S.beta as fBeta,
				S.divFreq as iDivFreq, 
				S.baseIndexID as iBaseIndexID,
				Null as fYield
			 from ContractsInGroup CIG
					join ContractView c  on C.contractID = CIG.contractID
					join Stock S on S.stockID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
			 where CIG.groupID = @iGroupID
					   and c.contractTypeID = 2
	   union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				I.defaultModelTypeID as iDefaultModelTypeID,
				Null as iDefaultSurfaceTypeID,
				Null as fBeta,
				Null as iDivFreq, 
				Null as iBaseIndexID,
				i.yield as fYield
				from ContractsInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
				where CIG.groupID = @iGroupID
						and c.contractTypeID = 1
				order by c.symbol  
Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE proc usp_ContractSymbolForRisk_Get
		@iBookID int,
		@iStrategyID int,
		@iTraderID int,
		@iUnderlyingID int 
as
begin
  select c.contractID as iContractID,
	c.symbol as vcSymbol
	 from Trade t, ContractView c 
	where ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
	  and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
	  and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
	  and ((@iUnderlyingID is Null) or (@iUnderlyingID is not Null) and (t.contractID = @iUnderlyingID))
	  and c.contractID = t.contractID
	  and c.contractTypeID in (1, 2)
   union
  select c.contractID as iContractID,
	  c.symbol as vcSymbol
	 from Trade t, OptionView o, ContractView c 
	where ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
	  and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
	  and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
	  and o.optionID = t.contractID
	  and ((@iUnderlyingID is Null) or (@iUnderlyingID is not Null) and (o.underlyingContractID  = @iUnderlyingID))
	  and c.contractID = o.underlyingContractID 
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

CREATE	  proc usp_ContractSymbol_Get
	  @iGroupID int,
	  @bIsAll bit,
	  @bStockOnly bit,   
	  @bIndexesOnly bit,
	  @bOptionsOnly bit 
	   
  as
	 if (@iGroupID is Null)
		if (@bIsAll is not Null)  and	(@bIsAll > 0)
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, C.symbol as vcSymbol
				from ContractView c  
						join Stock S on S.stockID = C.contractID
				where ((@bStockOnly is not Null) and (@bStockOnly > 0))
				   and c.contractTypeID = 2
		  union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, C.symbol as vcSymbol
				 from ContractView c  
						join IndexView I on I.indexID = C.contractID
				 where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and c.contractTypeID = 1
		  union 
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, C.symbol as vcSymbol
				 from ContractView c 
						join OptionView O on O.optionID = C.contractID
				  where ((@bOptionsOnly is not Null) and (@bOptionsOnly > 0)) 
						  and c.contractTypeID in (3, 4)
			 order by c.symbol	 
	   else
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, C.symbol as vcSymbol
					from ContractView c  
							join Stock S on s.stockID = c.contractID
					where ((@bStockOnly is not Null) and (@bStockOnly > 0)) 
						   and c.contractTypeID = 2
						   and not exists (select * from ContractInGroup where contractID = c.contractID)
		  union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, C.symbol as vcSymbol
					from ContractView c  
							join IndexView I on I.indexID = C.contractID
					where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and C.contractTypeID = 1
						 and not exists (select * from ContractInGroup where contractID = c.contractID)
					 order by C.symbol 
	  else
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, C.symbol as vcSymbol
				from ContractInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
				where CIG.groupID = @iGroupID
					   and ((@bStockOnly is not Null) and (@bStockOnly > 0))	
					   and c.contractTypeID = 2
		 union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, C.symbol as vcSymbol
				 from ContractInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join IndexView I on I.indexID = C.contractID
				 where CIG.groupID = @iGroupID
						 and ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and c.contractTypeID = 1
				 order by c.symbol			 
Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE Procedure dbo.usp_ContractVM_Save 
	@iContractID int = null output,		-- contract's identifier
	@iContractTypeID int = null,		-- contract's type identifier
	@vcSymbolName varchar(12) = null,	-- index's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot size (not used)
					-- Stock's data:
	@iDivFrequency int = null,		-- dividend's frequency
	@fAmount float = null, 			-- amount
	@dDivDate datetime = null,		-- dividend's date
					-- Index's data:
	@fYield float = null,			-- yield
	@bIsAutoUpdateWeight bit = null,	-- flag for enable/disable index's weights updating
					-- ETFs data:
	@iIndexBaseID int = null,		-- index's base identifier
	@rDenominator real = null,		-- denominator
	@bIsActive bit = null,			-- flag for enable/disable
					-- Option's data:
	@iUnderlyingID int = null,		-- underlying identifier
	@iIsCall bit = null,			-- flag for call/put
	@fStrike float = null,			-- strike
	@dtExpDate datetime = null,		-- expiration's date
	@iGroupID int = null,
	@iRuleID int = null
as
/*
*/
	-----------------------------------------------------------
	set nocount on
	-----------------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int, @iStockID int, @iOptionID int
	-- checking for wrong parameters
	if @iContractTypeID is null or @iContractTypeID not in (1,2,3)		goto finish
	if @iContractTypeID = 1	-- index
		exec @error = usp_IndexVM_Save
				@iIndexID = @iContractID output,		-- index's identifier
				@iContractTypeID = @iContractTypeID,		-- contract's type identifier
				@iSymbolName = @vcSymbolName,			-- index's ticker
				@iExchangeID = @iExchangeID,			-- exchange's identifier
				@vcDescription = @vcDescription,		-- description
				@iLotSize = 0,					-- lot size (not used)
			
				@fYield = @fYield,				-- yield
				@bIsAutoUpdateWeight = @bIsAutoUpdateWeight,	-- flag for enable/disable index's weights updating
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
--				@iIndexBaseID = null,				-- index's base identifier
--				@rDenominator = null,				-- denominator
--				@bIsActive = null				-- flag for enable/disable
	else
	if @iContractTypeID = 2	-- stock
		exec @error = usp_StockVM_Save
				@iStockID = @iContractID output,		-- stock's identifier
			
				@iContractTypeID = @iContractTypeID,		-- contract's type identifier (not used)
				@vcSymbolName = @vcSymbolName,			-- stock's ticker
				@iExchangeID = @iExchangeID,			-- exchange's identifier
				@vcDescription = @vcDescription,		-- description
				@iLotSize = 0,					-- lot size (not used)
			
				@iDivFrequency = @iDivFrequency,		-- dividend's frequency
				@fAmount = @fAmount, 				-- amount
				@dDivDate = @dDivDate,				-- dividend's date
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
	else
	if @iContractTypeID = 3	-- option
		exec @error = usp_OptionVM_Save
				@iOptionID = @iContractID output,	-- option's identifier
			
				@iContractTypeID = @iContractTypeID,-- contract's type identifier
				@iSymbolName = @vcSymbolName,		-- option's ticker
				@iExchangeID = @iExchangeID,		-- exchange's identifier
				@vcDescription = @vcDescription,	-- description
				@iLotSize = @iLotSize,				-- lot's size
			
				@iUnderlyingID = @iUnderlyingID,	-- underlying identifier
				@iIsCall = @iIsCall,				-- flag for call/put
				@fStrike = @fStrike,				-- strike
				@dtExpDate = @dtExpDate,			-- expiration's date
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
--====================================================================
-- update ruleID, groupID fields
--====================================================================
	if @iContractID is not null
	--if exists(select * from Contract where contractID=@iContractID)
		begin
			if (@iGroupID is not null) or (@iRuleID is not null)
				update Contract 
					set groupID = isnull(@iGroupID,groupID), ruleID = isnull(@iRuleID,ruleID)
				where contractID = @iContractID
		end 
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

CREATE proc usp_ContractWithSpot_Get
as
begin
  
	select Contract.contractID as iContractID, symbol as vcSymbol, contractName as vcContractName, CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk
		from ContractView Contract
				left join ContractPrice CP on Contract.contractID=CP.contractID
		where contractTypeID in (1, 2)
		order by symbol 
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

CREATE   Procedure dbo.usp_Contract_Del
	@iContractID int	-- contract identifier
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
-- PLEASE CHECK THE LOGIC OF THIS PROCEDURE CAREFULLY !
	Begin Transaction
		delete StockDataHistory where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		delete Dividend where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Stock set actionID=3 where stockID=@iContractID
		--delete Stock where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		exec @error=usp_Option_Del @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Contract set actionID=0 where contractID = @iContractID
		--delete Contract where contractID = @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
finish:
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

CREATE     Procedure dbo.usp_Contract_Get
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
				Dividend.divAmt fAmount, 
				Dividend.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID
			from Contract
			left join StockView Stock on Contract.contractID = Stock.stockID
			left join IndexView on Contract.contractID = IndexView.indexID
			left join (select Dividend.stockID, Dividend.divDate, divAmt 
					from Dividend
					inner join (select stockID, max(isnull(divDate, getdate()-getdate())) date 
							from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.divDate = DivLast.date)
				Dividend on Stock.stockID = Dividend.stockID
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
				Dividend.divAmt fAmount, 
				Dividend.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
				left join (select Dividend.stockID, Dividend.divDate, divAmt 
							from Dividend
								inner join (select stockID, max(isnull(divDate, getdate()-getdate())) [date] 
								from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.divDate = DivLast.[date])
						Dividend on Stock.stockID = Dividend.stockID
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc usp_Contract_Save
	  @iContractID int,
	  @vcSymbol varchar(8),
	  @vcContractName varchar(255),
	  @iLotSize int,
	  @fPriceBid float,
	  @fPriceAsk float,
	  @iExchangeID int,
	  @fPriceLast float,
	  @fPriceOpen float,
	  @fPriceClose float,
	  @fPriceLow float,
	  @fPriceHigh float,
	  @iVolume int,
	  @iOpenInterest int
	   
  as
-------------------------------------------------
set nocount on
-------------------------------------------------
  begin tran
	update Contract
		 set symbol = @vcSymbol,
			   contractName = CASE WHEN contractTypeID in (1, 2) THEN @vcContractName ELSE contractName END
	  where contractID = @iContractID
   if (@@error <> 0)
	 begin
	   ROLLBACK TRAN
	   RETURN @@error
	 end  
	if exists(select * from ContractPrice where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID)	
		update ContractPrice 
			set lotSize = @iLotSize,
				priceBid = @fPriceBid,
				priceAsk = @fPriceAsk,
				priceLast = @fPriceLast,
				priceOpen = @fPriceOpen,
				priceClose = @fPriceClose,
				priceLow = @fPriceLow,
				priceHigh = @fPriceHigh,
				volume = @iVolume
		where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID
	else 
		insert into ContractPrice (contractID,
				exchangeID,
				lotSize,
				priceBid,
				priceAsk,
				priceLast,
				priceOpen,
				priceClose,
				priceLow,
				priceHigh,
				volume)
		values (@iContractID,
				@iExchangeID,
				@iLotSize,
				@fPriceBid,
				@fPriceAsk,
				@fPriceLast,
				@fPriceOpen,
				@fPriceClose,
				@fPriceLow,
				@fPriceHigh,
				@iVolume)
   if (@@error <> 0)
	 begin
	   ROLLBACK TRAN
	   RETURN @@error
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
			if (@@error <> 0)
			  begin
				ROLLBACK TRAN
				RETURN @@error
			  end  
		  end	
	  end
  commit tran
  return @@error

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
		
			select Top 1 @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPriceHistory 
					where contractID = @iContractID
					order by PriceDate desc
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  procedure usp_CurrentDate_Get
	@DayOffset int output
as 
	declare @Year int,
			@DayOfYear int, 
			@Now smalldatetime
	set @Now = getdate()
	set @Year = datename(year,@Now)
	set @DayOfYear = DATENAME (dayofyear, @Now)
	set @DayOffset = @Year * 366 + @DayOfYear

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_CustomStrikeSkewData_Del
	@iCustomStrikeSkewID int,
	@iCustomStrikeSkewPointID int
as
/*
	this procedure deletes data from CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		delete CustomStrikeSkewData where customStrikeSkewID = @iCustomStrikeSkewID and customStrikeSkewPointID = @iCustomStrikeSkewPointID
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = 0
	Commit Transaction
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

CREATE   Procedure dbo.usp_CustomStrikeSkewData_Get
	@iVolaSurfaceDataID int,
	@dtExpDate datetime = null,
	@iSetupMode int,
	@iCustomStrikeSkewID int = null,
	@iCustomStrikeSkewPointID int = null,
	@bType bit = 1
as
/*
	this procedure gets data from CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	-- truncate time information
	set @dtExpDate = convert(varchar(8), @dtExpDate, 112)
	if @bType is null	set @bType = 1
	if @dtExpDate is not null
		begin
			if not exists(select * 
							from CustomStrikeSkewPointView 
									left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
									left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
									inner join VolaSurfaceData  on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
							where setupMode = @iSetupMode and VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and expiry = @dtExpDate and type = @bType)
		
				set @dtExpDate = null
		
			select distinct CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, CustomStrikeSkewPoint.volaSurfaceDataID iVolaSurfaceDataID, 
				setupMode iSetupMode, strike fStrike, case when expiry=@dtExpDate then isnull(volatility, 0) else 0 end fVolatility, case when expiry=@dtExpDate then CustomStrikeSkew.customStrikeSkewID else null end iCustomStrikeSkewID, case when expiry=@dtExpDate then expiry else null end dtExpDate,
				type bType
		
				from CustomStrikeSkewPointView
						inner join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
						left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
						left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
				where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
					and (@dtExpDate is null or expiry = @dtExpDate)
					and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
		end
	else
		select  CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, 
			CustomStrikeSkewPoint.volaSurfaceDataID ivolaSurfaceDataID, 
			setupMode iSetupMode, 
			strike fStrike, 
			/*case when expiry=@dtExpDate then*/ volatility /*else 0 end*/ fVolatility, 
			/*case when expiry=@dtExpDate then*/ CustomStrikeSkew.customStrikeSkewID /*else null end*/ iCustomStrikeSkewID, 
			/*case when expiry=@dtExpDate then*/ expiry /*else null end*/ dtExpDate,
			type bType
			from CustomStrikeSkewPointView
					inner join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
					inner join CustomStrikeSkew on CustomStrikeSkewPoint.volaSurfaceDataID = CustomStrikeSkew.volaSurfaceDataID
					left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
							and CustomStrikeSkew.customStrikeSkewID = CustomStrikeSkewData.customStrikeSkewID
			where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
	--			and (@dtExpDate is null or expiry = @dtExpDate)
				and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
				and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
	set @error = @@error
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

CREATE  Procedure dbo.usp_CustomStrikeSkewData_Save
	@iCustomStrikeSkewID int = null,
	@iCustomStrikeSkewPointID int,
	@fVolatility float,
	@iVolaSurfaceDataID int = null,
	@vcContractTicker varchar(12) = null,
	@dtExpDate datetime = null
as
/*
	this procedure saves data to CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	--declare @iVolaSurfaceDataID int 
	-- wrong parameters
	--if @iCustomStrikeSkewID is null and (@iCustomStrikeSkewPointID is null and @vcContractTicker is null and @iContractID is null or @dtExpDate is null)	goto finish
	--if @iContractID is null and @vcContractTicker is not null
	--	select @iContractID = contractID from Contract where symbolName = @vcContractTicker and contractTypeID = 2
	--if @iContractID is not null
	--	select @iVolaSurfaceDataID = VolaSurfaceDataID from VolaSurfaceData where contractID = @iContractID
	--if @iContractID is null and @iCustomStrikeSkewPointID is not null
	--	select @iVolaSurfaceDataID = VolaSurfaceDataID from CustomStrikeSkewPoint where customStrikeSkewPointID = @iCustomStrikeSkewPointID
	if @iCustomStrikeSkewID is null and @iVolaSurfaceDataID is not null and @dtExpDate is not null
		select @iCustomStrikeSkewID = customStrikeSkewID from CustomStrikeSkew where volaSurfaceDataID = @iVolaSurfaceDataID and expiry = @dtExpDate
	Begin Transaction
		if @iCustomStrikeSkewID is null
			begin
				exec usp_CustomStrikeSkew_Save @iCustomStrikeSkewID = @iCustomStrikeSkewID output, @iVolaSurfaceDataID = @iVolaSurfaceDataID, @dtExpDate = @dtExpDate
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		if not exists (select * from CustomStrikeSkewData where customStrikeSkewID = @iCustomStrikeSkewID and customStrikeSkewPointID = @iCustomStrikeSkewPointID)
			begin
				insert into CustomStrikeSkewData (customStrikeSkewID, customStrikeSkewPointID, volatility) values (@iCustomStrikeSkewID, @iCustomStrikeSkewPointID, @fVolatility)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update CustomStrikeSkewData set
					customStrikeSkewPointID = isnull(@iCustomStrikeSkewPointID, customStrikeSkewPointID),
					volatility = isnull(@fVolatility, volatility)
				where customStrikeSkewID = @iCustomStrikeSkewID and customStrikeSkewPointID = @iCustomStrikeSkewPointID
				if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		set @error = 0
	Commit Transaction
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

CREATE  procedure usp_CustomStrikeSkewDefaultPoint_Save
	@iContractID int,
	@dtExpiry smalldatetime,
	@iType int,
	@fVolatility float
as
/*
	Created by Zaur Nuraliev
	Description: this procedure ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID
--select @dtExpiry as '@dtExpiry', @iVolaSurfaceDataID as '@iVolaSurfaceDataID'
	insert into CustomStrikeSkewPoint(VolaSurfaceDataID,strike,type,volatility,isBasePoint,expDate)
		select distinct @iVolaSurfaceDataID, strike as fStrike, @iType,@fVolatility,-1,convert(varchar(6),expiry,112) + '01' as expiry
			from OptionView 
			where expiry = @dtExpiry and underlyingContractID = @iContractID
				and not exists(select * 
									from CustomStrikeSkewPointView 
									where VolaSurfaceDataID = @iVolaSurfaceDataID and 
											strike = OptionView.strike and convert(varchar(8),expDate,112) = convert(varchar(6),expiry,112) + '01')

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_CustomStrikeSkewPoint_Copy 
	@iVolaSurfaceDataIDSrc int,
	@iVolaSurfaceDataIDDest int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is move information inside CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expDate,
				isBasePoint
			into #SourcePoints
			from CustomStrikeSkewPoint
			where volaSurfaceDataID=@iVolaSurfaceDataIDSrc
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		delete CustomStrikeSkewPoint where volaSurfaceDataID=@iVolaSurfaceDataIDDest
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		insert into CustomStrikeSkewPoint (volaSurfaceDataID,strike,type,volatility,expDate,isBasePoint)
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expDate,
				isBasePoint
				from #SourcePoints
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
	drop table #SourcePoints
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

CREATE    Procedure dbo.usp_CustomStrikeSkewPoint_Del
	@iCustomStrikeSkewPointID int = null
as
/*
	this procedure deletes data from CustomStrikeSkewPoint table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update CustomStrikeSkewPoint set actionID=3
--	delete CustomStrikeSkewPoint
		where customStrikeSkewPointID = @iCustomStrikeSkewPointID
	set @error = 0
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

CREATE      Procedure dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null,
	@dtActionDate datetime = null 
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 
		customStrikeSkewPointID as iCustomStrikeSkewPointID,
		volaSurfaceDataID as iVolaSurfaceDataID,
		strike as fStrike, 
		type as iType,
		volatility as fVolatility,
		expDate as dtExpDate,
		isBasePoint as iIsBasePoint,
		actionDate as dtActionDate,
		actionID as tiActionID
		from CustomStrikeSkewPoint
		where 
			(@iCustomStrikeSkewPointID is null or customStrikeSkewPointID=@iCustomStrikeSkewPointID)
		and (@iVolaSurfaceDataID is null or volaSurfaceDataID=@iVolaSurfaceDataID) 
		and (@iType is null or type=@iType) 
		and (@dtExpDate is null or expDate=@dtExpDate)
		and ((actionDate>=@dtActionDate or @dtActionDate is null) and (@dtActionDate is not null or @dtActionDate is null and actionID<3))
/*
(@iCustomStrikeSkewPointID is not null and customStrikeSkewPointID=@iCustomStrikeSkewPointID)
or ((@iVolaSurfaceDataID is not null and @iType is not null) and volaSurfaceDataID=@iVolaSurfaceDataID and type=@iType)
	or ((@iVolaSurfaceDataID is not null and @iType is not null and @dtExpDate is not null) and volaSurfaceDataID=@iVolaSurfaceDataID and type=@iType and expDate=@dtExpDate)
*/
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

CREATE    Procedure dbo.usp_CustomStrikeSkewPoint_Save 
	@iCustomStrikeSkewPointID int = Null output,
	@iVolaSurfaceDataID int = Null,
	@iType int = Null,
	@fStrike float = Null,
	@fVolatility float = Null,
	@dtExpDate datetime = Null,
	@iIsBasePoint int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into CustomStrikeSkewPoint table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from CustomStrikeSkewPoint where 
						volaSurfaceDataID = @iVolaSurfaceDataID and 
						strike = @fStrike and 
						type = @iType and 
						expDate = @dtExpDate
				and actionID < 3 and (@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null))
		begin
			Raiserror ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
			Return (-1)
		end   
	Begin Transaction
--		if not exists(select * From CustomStrikeSkewPoint where customStrikeSkewPointID=@iCustomStrikeSkewPointID) 
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					expDate,
					isBasePoint) 
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						@dtExpDate,
						@iIsBasePoint)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iCustomStrikeSkewPointID=@@IDENTITY
			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expDate = isnull(@dtExpDate,expDate),
					isBasePoint = isnull(@iIsBasePoint,isBasePoint)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
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

CREATE   Procedure dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(8) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	select distinct 
			CV.contractID as iContractID,
			CV.symbol as vcSymbolName,
			OV.expiry as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
			and Convert(varchar(6),OV.expiry,112) = Convert(varchar(6),CSSP.expDate,112)
		where 
			CSSP.volaSurfaceDataID is null and 
			(CV.symbol = @vcSymbolName or @vcSymbolName is null) 
		order by CV.contractID,OV.expiry
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

CREATE   Procedure dbo.usp_DataInfo_Get
	@iDataInfoID int = null,		-- DataInfo identifier
	@vcKeyName varchar(50) = null,		-- key's name
	@dtDateMin datetime = null,		-- the minimum date for seek
	@dtDateMax datetime = null,		-- the maximum date for seek
	@iTenorValue int = null,		-- tenor's value
	@vcSID varchar(32) = '',		-- session's identifier
	@iStatus int = null out,		-- -1 - unexpected error
						--  0 - success
						--  1 - user is not logged on. Access denied
	@bOnlyMaxDate bit = 0			-- flag for max date returning
as
/*
	Created by Andrey Sinkin
	Description:	this procedure returns information about databases parameters
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
/*	----------------------------------------------------------------------------------------
--	declare @iStatus int	set @iStatus = -1
	exec xsp_Permission_Check
		@vcSID = @vcSID,		-- session's identifier
		@iStatus = @iStatus output	-- -1 - unexpected error
						--  0 - success
						--  1 - user is not logged on. Access denied
	if @iStatus <> 0	begin set @error = @iStatus goto finish end
	----------------------------------------------------------------------------------------
*/
	-- Declaring variables
	declare @iTenorID int
	if @iDataInfoID is null and @vcKeyName is not null
		select @iDataInfoID = @iDataInfoID from DataInfo where keyName = @vcKeyName
	if @iDataInfoID in (2, 3) and @iTenorValue is not null
		begin
			select @iTenorID = tenorID from Tenor where value = @iTenorValue
		end
/*	if @iDataInfoID = 2
		begin
			if @bOnlyMaxDate <> 1
				begin
					if @dtDateMin is null		set @dtDateMin = getdate()-getdate()
					if @dtDateMax is null		set @dtDateMax = getdate()
					select @dtDateMin = convert(varchar(8), @dtDateMin, 112), @dtDateMax = convert(varchar(8), @dtDateMax, 112)
					select [date] dtDate 
						from IndexDataHistory 
						where tenorID = @iTenorID and [date] between @dtDateMin and @dtDateMax
						group by [date] 
						order by [date]
				end
			else
				select max([date]) dtDate from IndexDataHistory where tenorID = @iTenorID
		end */
	else
	if @iDataInfoID = 3
		begin
			if @bOnlyMaxDate <> 1
				begin
					if @dtDateMin is null		set @dtDateMin = getdate()-getdate()
					if @dtDateMax is null		set @dtDateMax = getdate()
					select @dtDateMin = convert(varchar(8), @dtDateMin, 112), @dtDateMax = convert(varchar(8), @dtDateMax, 112)
					select [date] dtDate 
						from StockDataHistory 
						where tenorID = @iTenorID and [date] between @dtDateMin and @dtDateMax
						group by [date] 
						order by [date]
				end
			else
				select max(date) dtDate from StockDataHistory where tenorID = @iTenorID
		end
	else
		select dataInfoID iDataInfoID, keyName vcKeyName, keyValue vcKeyValue
			from DataInfo
			where (@iDataInfoID is null or dataInfoID = @iDataInfoID)
	set @error = @@error
finish:
	if @iStatus is null 
		set @iStatus=0
Return(@iStatus)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE proc usp_Dividend_Del
	  @iStockID int,
	  @iDividendID int = Null
 as
	  if (@iStockID is Null) and (@iDividendID is Null)
		  return (-2)
	 if (@iDividendID is Null)
	   delete Dividend where stockID = @iStockID
	  else
	   delete Dividend where dividendID = @iDividendID 
   
	if (@@error <> 0) 
	   return (-1)
	 else 
	   return 0

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create proc usp_Dividend_Get
		@iStockID int,
		@iDividendID int = Null
 as
	 if (@iDividendID is Null)
	   select Dividend.dividendID as iDividendID,
		Dividend.stockID as iStockID,
		Dividend.divAmt as fDivAmt,
		Dividend.divDate as dtDivDate,
		Stock.divFreq as iDivFreq
		   from Dividend, Stock
		 where Dividend.stockID = @iStockID
		   and Stock.stockID = Dividend.stockID
		 order by Dividend.divDate
	 else
	   select Dividend.dividendID as iDividendID,
		Dividend.stockID as iStockID,
		Dividend.divAmt as fDivAmt,
		Dividend.divDate as dtDivDate,
		Stock.divFreq as iDivFreq
		   from Dividend, Stock
		 where Dividend.dividendID = @iDividendID 
		   and Stock.stockID = Dividend.stockID 
  Return(0)									  

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE proc usp_Dividend_Save
		@iDividendID int = null output,	-- dividend's identifier,
		@iStockID int,
		@fDivAmt float,
		@dtDivDate smalldatetime
 as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iDividendID is null and @iStockID is not null and @dtDivDate is not null
		select @iDividendID = dividendID from Dividend where stockID = @iStockID and divDate = @dtDivDate
Begin Transaction
   if (@iDividendID is Null)  
	 begin
		 insert into Dividend (stockID, divAmt, divDate)
				values (@iStockID, @fDivAmt, @dtDivDate)
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			
		select @iDividendID = @@identity
	 end
   else 
	 begin
		update Dividend 
			 set  	stockID = isnull(@iStockID, stockID),
					divAmt = isnull(@fDivAmt, divAmt),
					divDate =isnull(@dtDivDate, divDate)
			  where dividendID = @iDividendID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	 end				
	if @dtDivDate>=(select max(DivDate) from Dividend where stockID=@iStockID)
		update Stock 
			set divAmt=@fDivAmt,divDate=@dtDivDate 
		where stockID=@iStockID
		set @error = 0
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

CREATE  proc usp_ExchangeHoliday_Del
	  @iExchangeID int,
	  @iHolidayID int = Null
as
	  if ((@iExchangeID is Null) and (@iHolidayID is Null)) return (-2)
   
	  if (@iHolidayID is Null)
		update ExchangeHoliday set actionID=3 where exchangeID = @iExchangeID
		 --delete ExchangeHoliday where exchangeID = @iExchangeID
	   else
		update ExchangeHoliday set actionID=3 where exchangeID = @iExchangeID and holidayID = @iHolidayID
		 --delete ExchangeHoliday where exchangeID = @iExchangeID and holidayID = @iHolidayID
	
	  if (@@error <> 0)
		 return (-1)
	   else
		 return 0

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   Procedure dbo.usp_ExchangeHoliday_Get
	@iExchangeID int = null,		-- exchange's identifier
	@dtActionDate datetime = null
as
/*
	Created by Andrey Sinkin
	this procedure gets data from ExchangeHoliday table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	exchangeID iExchangeID, 
			holidayID iHolidayID,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ExchangeHoliday
		where (@iExchangeID is null or exchangeID = @iExchangeID)
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_ExchangeHoliday_Save
	  @iExchangeID int,
	  @iHolidayID int
as
	if exists (select * from ExchangeHoliday 
				where exchangeID = @iExchangeID and holidayID = @iHolidayID and actionID < 3)
		begin
			Raiserror ('Exchange holiday with this exchangeID and holidayID already exists!', 16, 1)
			Return (-1)
		end
	  insert into ExchangeHoliday (exchangeID, holidayID)
		   values (@iExchangeID, @iHolidayID)
	  if (@@error <> 0)
		 return (-1)
	   else
		 return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_Exchange_Del
		@iExchangeID int
  as
	update Exchange set actionID=3 where exchangeID = @iExchangeID
	--delete Exchange where exchangeID = @iExchangeID
	  if (@@error <> 0)
		 return (-1)
	   else  
		 return 0

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE     proc usp_Exchange_Get
	@iExchangeID int = null,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Exchange table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
  
	select
		exchangeID as iExchangeID,
		exchangeName as vcExchangeName,
		[Description] as vcDescription,
		ExchangeCode as vcExchangeCode,
		actionDate as dtActionDate,
		actionID as tiActionID
	from Exchange
	where (exchangeID=@iExchangeID or @iExchangeID is null)
		and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
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

CREATE   proc usp_Exchange_Save
		@iExchangeID int = null output,
		@vcExchangeName varchar(30) = null,
		@vcDescription varchar(120) = null,
		@vcExchangeCode varchar(10) = null
  as
	if exists (select * from Exchange where ExchangeName = @vcExchangeName and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this name already exists!', 16, 1)
			Return (-1)
		end   
	if exists (select * from Exchange where ExchangeCode = @vcExchangeCode and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this code already exists!', 16, 1)
			Return (-1)
		end   
   
	if @iExchangeID is null
	--if not exists(select * from Exchange where exchangeID = @iExchangeID)  
	 begin
		insert into Exchange (exchangeName, [Description],exchangeCode)
				values (@vcExchangeName, @vcDescription,@vcExchangeCode)
		if (@@error <> 0) return (-1)
		set @iExchangeID = @@identity
	 end
   else 
	 begin
		update Exchange
			 set  exchangeName = isnull(@vcExchangeName,exchangeName),
				[Description] = isnull(@vcDescription,[Description]),
				exchangeCode = isnull(@vcExchangeCode,exchangeCode)
		  where exchangeID = @iExchangeID
		if (@@error <> 0)
		   return (-1)
		 else  
		   return (0)
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

CREATE   Procedure dbo.usp_Expiry_Del
	@iExpiryID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int
	set @error = -1
		begin tran
			update expiry set actionID=3 where expiryID = @iExpiryID
			--delete expiry where expiryID = @iExpiryID
	
		  set @error = @@error
		  if (@error <> 0)
			begin
			  rollback tran
			  goto finish
			end 
		commit tran 
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

CREATE    Procedure dbo.usp_Expiry_Get
	@iExpiryID int = null,
	@vcExpiryPeriod varchar(30) = null,
	@dtActionDate datetime = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select expiryID as iExpiryID, 
		expiryPeriod as vcExpiryPeriod, 
		expiryDate as dtExpiryDate,
		vegaWeight as fVegaWeight,
		actionDate as dtActionDate,
		actionID as tiActionID
		from expiry
		where ((expiryPeriod = @vcExpiryPeriod or @vcExpiryPeriod is null) and 
				(expiryID = @iExpiryID or @iExpiryID is null))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
	set @error = @@error
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

CREATE Procedure dbo.usp_Expiry_Save
	@iExpiryID int=Null output,
	@vcExpiryPeriod varchar(30) = Null,
	@dtExpiryDate smalldatetime = Null,
	@fVegaWeight float = Null
as
/*
	Changed by Zaur Nuraliev 20011204
	[Description]:	this procedure save expiry parametrs for GU interface
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1
	if exists (select * from Expiry where expiryDate = @dtExpiryDate and actionID < 3 and (@iExpiryID <> expiryID or @iExpiryID is null))
		begin
			Raiserror ('Row in Expiry table with same expiry date already exists!', 16, 1)
			Return (-1)
		end   
	if @iExpiryID is Null 
		begin
			insert into expiry (expiryPeriod, expiryDate,vegaWeight) 
				values (@vcExpiryPeriod, @dtExpiryDate,@fVegaWeight)
			set @error = @@error
			if (@error <> 0)
				goto finish
			
			select @iExpiryID = @@identity
		end
	 else
		begin
		   declare @dt smalldatetime
		   select @dt = expiryDate from expiry where @iExpiryID = expiryID
--			 if ((@dtExpiryDate is not Null) and (@dtExpiryDate <> @dt))
		   if (@dtExpiryDate is not Null)
			   begin
				   begin tran 
						update expiry
							   	set expiryPeriod = isNull(@vcExpiryPeriod, expiryPeriod),
								   	expiryDate = isNull(@dtExpiryDate, expiryDate),
									vegaWeight = isNull(@fVegaWeight,vegaWeight)
								where @iExpiryID = expiryID
					  
						 set @error = @@error
						if (@error <> 0)
						  begin
							rollback tran
							goto finish
						  end
				   commit tran 
			  end
			else
			  begin
				update expiry
				   	set vegaWeight = isNull(@fVegaWeight,vegaWeight)
					where @iExpiryID = expiryID
				set @error = @@error
			end
		end
   if (@error = 0) set @error = @iExpiryID
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

CREATE Procedure dbo.usp_FilterData_Get
	@iFilterID int, --the row identifier
	@iTraderID int = null output,
	@iBookID int = null output,
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
	set @vcSelect1 = 'select distinct contractID = case isnull(O.underlyingContractID, 0)'
			 +' when isnull(null, 0) then T.contractID'
			 +' else O.underlyingContractID end'
			 +' from TradeView T left join OptionView O on T.contractID = O.optionID'
	set @vcSelect2 =''
	set @vcSelect3 = ' where 1=1'  
	declare @vcFiledValue varchar(50)
	-- TraderID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 1 )	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=1
			set @vcSelect3 = ' and T.traderID=' + @vcFiledValue
			select @iTraderID = convert(int,@vcFiledValue)
		end
	-- BookID
	if exists(select * from FilterContent where filterID=@iFilterID and fieldValue is not null and filterFieldID = 2)	
		begin
			select @vcFiledValue=fieldValue from FilterContent where filterID=@iFilterID and filterFieldID=2
			set @iBookID = convert(int,@vcFiledValue)
			set @vcSelect3 = @vcSelect3 + ' and T.bookID=' + @vcFiledValue
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
			set @vcSelect2 = @vcSelect2 + ' join ContractInGroup C1 on (O.underlyingContractID=C1.contractID or T.contractID = C1.contractID) and C1.groupID=' + @vcFiledValue 
		end
	--select @vcSelect1+@vcSelect2+@vcSelect3
	execute (@vcSelect1 + @vcSelect2 + @vcSelect3)
	set @error = @@error
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

CREATE  Procedure dbo.usp_FilterList_Get 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Filter table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select	Filter.filterID as iFilterID,
			Filter.filterName as vcFilterName
		from Filter	
	set @error = @@error
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

CREATE     Procedure dbo.usp_FilterTradeData_Get
	@iTraderID int = null,
	@iBookID int = null,
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
	-- BookID
	if @iBookID is not null
		begin
			if @vcSelect2 = ''
				set @vcSelect2 = ' bookID=' + Convert(varchar(15),@iBookID)
			else
				set @vcSelect2 = @vcSelect2 + ' and bookID=' + Convert(varchar(15),@iBookID)
		end	
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
	
/*
	if @vcSelect2 =''
		set @vcSelect2=' traderID is not null'
	else 
		set @vcSelect2= @vcSelect2 + ' and traderID is not null'
*/
	--====================================================================
	-- execute created script
	--====================================================================
	--select @vcSelect1 + @vcSelect2
	
	execute (@vcSelect1 + @vcSelect2)
	set @error = @@error
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

CREATE  Procedure dbo.usp_Filter_Del 
	@iFilterID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Filter table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete FilterContent where filterID = @iFilterID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		delete Filter where filterID = @iFilterID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
finish:
	set @error = @@error
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

CREATE    Procedure dbo.usp_Filter_Get 
	@iFilterID int = Null 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Filter table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if @iFilterID is not null
		select	Filter.filterID as iFilterID,
				Filter.filterName as vcFilterName,
				FF.fieldName as vcFieldName,
				FC.fieldValue as vcFieldValue
			from Filter	
				join FilterContent FC on Filter.filterID =FC.filterID
				join FilterField FF on FC.filterFieldID = FF.filterFieldID
			where Filter.filterID=@iFilterID
	else 
		select	filterID as iFilterID,
				filterName as vcFilterName
			from Filter	
			order by filterID
	set @error = @@error
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

CREATE     Procedure dbo.usp_Filter_Save 
	@iFilterID int = Null Output,		--the row identifier
	@vcFilterName varchar(80) = Null, 	--the filter name
	@iTraderID int = Null,  		
	@iBookID int = Null,  		
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
--	if @iBookID is not null
--		begin
			if exists(select * from FilterContent where filterID = @iFilterID and filterFieldID = 2)
				begin
					update FilterContent 
						set fieldValue = @iBookID
						where filterID = @iFilterID and filterFieldID = 2
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				begin
					insert into FilterContent 
						(filterID,filterFieldID,fieldValue)
						values (@iFilterID,2,@iBookID) 
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_GenerationGroupItemsClear_Save
	@iGroupID int --the  group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure break relation between StockGroup and Stock table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update Contract set groupID=Null where groupID = @iGroupID
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

CREATE  Procedure dbo.usp_GenerationRuleGroup_Del
	@iGroupID int --the  row identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure remove information 
			 from GenerationRuleGroup table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    Begin Transaction
		update Contract set groupID=Null where groupID = @iGroupID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		delete GenerationRuleGroup where groupID = @iGroupID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
    Commit Transaction
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

CREATE   Procedure dbo.usp_GenerationRuleGroup_Get
	@iGroupID int =Null --the group identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure return information 
			     from GenerationRuleGroup table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	groupID as iGroupID,
			groupName as vcGroupName,
			ruleID as iRuleID, 
			isActive as bIsActive
		from GenerationRuleGroup
		where (groupID = @iGroupID and @iGroupID Is Not Null) or (@iGroupID Is Null)
		order by groupName
	set @error = @@error
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

CREATE    Procedure dbo.usp_GenerationRuleGroup_Save
	@iGroupID int=Null, 			--the group identifier
	@vcGroupName varchar(250)=Null, --the group name
	@iRuleID int=Null, 				--the rule identifier
	@bIsActive bit=Null 			--the row 'isActive' atribute 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save information 
			 into GenerationRuleGroup table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
--	declare @error int		set @error = -1
	if not exists(select * from GenerationRuleGroup where groupID=@iGroupID)
		begin
			insert into GenerationRuleGroup(
					groupName,
					ruleID,
					isActive) 
				values (
					@vcGroupName,
					@iRuleID,
					@bIsActive)
			set @iGroupID = @@identity
		end
	else
		begin
			update GenerationRuleGroup set
				groupName = isnull(@vcGroupName, groupName),
				ruleID = isnull(@iRuleID, ruleID),
				isActive = isnull(@bIsActive, isActive)
			where @iGroupID = groupID
		end
--	set @error = @@error
Return(@iGroupID)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   Procedure dbo.usp_GenerationRule_Del
	@iRuleID int  --the rule identifier
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure remove inf. 
					from GenerationRule table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
	update Contract set ruleID=Null where ruleID=@iRuleID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end	
	delete GenerationRule Where ruleID = @iRuleID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
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

CREATE       Procedure dbo.usp_GenerationRule_Get
	@iRuleID int=Null --the rule identifier
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get inf. 
					from GenerationRule table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	ruleID as iRuleID,
			ruleType as iRuleType,
			ruleName as vcRuleName, 
			interpolationFactor as fInterpolationFactor, 
			callShift as fCallShift , 
			putShift as fPutShift,
			smileAccelerator as fSmileAccelerator , 
			strikeIncrement as iStrikeIncrement, 
			strikeMoneynessLo as fStrikeMoneynessLo,
			strikeMoneynessHi as fStrikeMoneynessHi,
			isDefaultRule as bIsDefaultRule,
			requiredPoints as iRequiredPoints
		from GenerationRule
		where (ruleID = @iRuleID and @iRuleID Is Not Null) or (@iRuleID Is Null)
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

CREATE         Procedure dbo.usp_GenerationRule_Save
	@iRuleID int=Null,		 	--the row identifier
	@iRuleType int=Null,			--the rule type	
	@vcRuleName varchar(250)=Null, 	--the rule name
	@fInterpolationFactor float=Null,--the Factor of Interpolation
	@fCallShift float = Null,			--the CallRange
	@fPutShift float = Null,			--the PutRange
	@fSmileAccelerator float=Null,		--the Smile Accelerator
	@iStrikeIncrement int=Null,		--the Strike Increment
	@fStrikeMoneynessHi float=Null,		--the Amount Of Points Required
	@fStrikeMoneynessLo float=Null,		--the Amount Of Points low value
	@bIsDefaultRule bit=Null,		--the IsDefault atribute
	@iRequiredPoints int=Null		--the number of Points Required
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if not exists (select * from GenerationRule where ruleID = @iRuleID) 
		begin
			insert into GenerationRule (
					ruleType,
					ruleName,
					interpolationFactor,
					callShift,
					putShift,
					smileAccelerator,
					strikeIncrement,
					strikeMoneynessHi,
					strikeMoneynessLo,
					isDefaultRule,
					requiredPoints
					) 
				values (
					@iRuleType,
					@vcRuleName,
					@fInterpolationFactor,
					@fCallShift,
					@fPutShift,
					@fSmileAccelerator,
					@iStrikeIncrement,
					@fStrikeMoneynessHi,
					@fStrikeMoneynessLo,
					@bIsDefaultRule,
					@iRequiredPoints)
	
			set @iRuleID=@@IDENTITY	
		end
	else
		begin
			update GenerationRule set
				ruleName = isnull(@vcRuleName, ruleName),
				ruleType = isnull(	@iRuleType, ruleType),
				interpolationFactor = isnull(@fInterpolationFactor, interpolationFactor),
				callShift = isnull(@fCallShift, callShift),
				putShift = isnull(@fPutShift, putShift),
				smileAccelerator = isnull(@fSmileAccelerator, smileAccelerator),
				strikeIncrement = isnull(@iStrikeIncrement, strikeIncrement),
				strikeMoneynessHi = isnull(@fstrikeMoneynessHi, strikeMoneynessHi),
				strikeMoneynessLo = isnull(@fStrikeMoneynessLo, strikeMoneynessLo),
				isDefaultRule=isnull(@bIsDefaultRule,IsDefaultRule),
				requiredPoints = isnull(@iRequiredPoints,requiredPoints)
			where @iRuleID = ruleID
		end
	set @error = @@error
if @error<>0 
	Return(@error)
else 
	Return(@iRuleID)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure usp_HistRangesCache_Del
	@iContractID int	-- contract's identifier
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure deletes data from HistRangesCache table
			using unic key fields combination 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	Delete HistRangesCache 
	Where @iContractID = contractID
Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    PROCEDURE usp_HistRangesCache_Get 
	@iContractID int 	-- the contract identifier
AS
/*
	Created by Zaur Nuraliev
	Description:	this procedure return rows from  HistRangesCache
					table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Select 
		contractID as iContractID,
		optionExpDate as dtOptionExpDate,
		strike as fStrike,
		HVLow as fHVLow,
		HVHigh as fHVHigh,
		isATM as bIsATM
	From HistRangesCache
	Where contractID = @iContractID
	Order by iContractID,dtOptionExpDate,fStrike asc
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

CREATE  PROCEDURE usp_HistRangesCache_Save 
	@iContractID int, 			--the ContractID value	
	@dtOptionExpDate datetime,	--the OptionExpDate value
	@fStrike float = Null, 		--the Strike value
	@fHVLow float  = Null, 		--the HVLow value
	@fHVHigh float  = Null, 	--the HVHigh value
	@bIsATM bit  = Null   	--the IsATM value
AS
/*
	Created by Zaur Nuraliev
	Description:	this procedure save information to 
					HistRangesCache table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    Begin Transaction
		if not exists(select * From HistRangesCache where @iContractID = contractID and @dtOptionExpDate = optionExpDate and @fStrike = strike) 
			begin
				insert into HistRangesCache( 
					ContractID,
					OptionExpDate,
					Strike,
					HVLow,
					HVHigh,
					isATM) 
				values (
						@iContractID,
						@dtOptionExpDate,
						@fStrike,
						@fHVLow,
						@fHVHigh,
						@bIsATM)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update HistRangesCache
					set HVLow = isnull(@fHVLow,HVLow),
						HVHigh = isnull(@fHVHigh,HVHigh),
						isATM = isnull(@bIsATM,isATM)
				where @iContractID = contractID and @dtOptionExpDate = optionExpDate and @fStrike = strike
	
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_Holiday_Del
	  @iHolidayID int
 as
	
	update Holiday set actionID=3 where holidayID = @iHolidayID 
	--delete Holiday where holidayID = @iHolidayID 
   
	if (@@error <> 0) 
	   return (-1)
	 else 
	   return 0

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc usp_Holiday_Get
		@iExchangeID int = null,
		@iHolidayID int = null,
		@dtActionDate datetime = null
 as
	 if (@iHolidayID is Null)
	   begin
		   if (@iExchangeID is Null)
			 select holidayID as iHolidayID, 
					holidayName as vcHolidayName, 
					holidayDate as dtHolidayDate,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Holiday
				where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				order by holidayDate 
			else
			 select h.holidayID as iHolidayID, 
					h.holidayName as vcHolidayName, 
					h.holidayDate as dtHolidayDate,
					h.actionDate as dtActionDate,
					h.actionID as tiActionID
				from ExchangeHolidayView eh join Holiday h on h.holidayID = eh.holidayID
				where eh.exchangeID = @iExchangeID
					and (h.actionDate>=@dtActionDate or @dtActionDate is null and h.actionID<3)
			  order by h.holidayDate
	   end
	 else
		 select holidayID as iHolidayID, 
				holidayName as vcHolidayName, 
				holidayDate as dtHolidayDate,
				actionDate as dtActionDate,
				actionID as tiActionID
		   from Holiday
		 where holidayID = @iHolidayID 
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
Return(0)	  

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_Holiday_Save
		@iHolidayID int,
		@dtHolidayDate smalldatetime,
		@vcHolidayName varchar(30)
 as
  
	if exists (select * from Holiday where holidayDate = @dtHolidayDate and actionID < 3 and (@iHolidayID <> holidayID or @iHolidayID is null))
		begin
			Raiserror ('Row in Holiday table with same holiday date already exists!', 16, 1)
			Return (-1)
		end   
	if exists (select * from Holiday where holidayName = @vcHolidayName and actionID < 3 and (@iHolidayID <> holidayID or @iHolidayID is null))
		begin
			Raiserror ('Row in Holiday table with same holiday name already exists!', 16, 1)
			Return (-1)
		end  
   if (@iHolidayID is Null)  
	 begin
		 insert into Holiday (holidayName, holidayDate)
				values (@vcHolidayName, @dtHolidayDate)
		 if (@@error <> 0)
			return (-1)
		  else	
			return @@identity
	 end
   else 
	 begin
		update Holiday 
			 set holidayName = @vcHolidayName,
				  holidayDate = @dtHolidayDate		  
		  where holidayID = @iHolidayID
		if (@@error <> 0)
		   return (-1)
		 else  
		   return 0
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

CREATE Procedure dbo.usp_IRCurve_Del
	@iCurveID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	begin tran
		update IRPoint set actionID=3 where curveID = @iCurveID
	--delete IRPoint Where curveID = @iCurveID
		update IRCurve set actionID=3 where curveID = @iCurveID
	--delete IRCurve Where curveID = @iCurveID
	if @@error = 0
		commit tran
	else
		rollback tran
	
	set @error = @@error
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

CREATE  Procedure dbo.usp_IRCurve_Get
	@iCurveID int = null,
	@dtActionDate datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		select curveID as iCurveID,
			curveName as vcCurveName, 
			curveTypeID as iCurveTypeID, 
			ruleTypeID as iRuleTypeID, 
			positionThreshold as mPositionThreshold,
			actionDate as dtActionDate,
			actionID as tiActionID
	from IRCurve
	where
		@iCurveID is Null or @iCurveID is not Null and curveID = @iCurveID and
		(actionDate>=@dtActionDate or @dtActionDate is null)
		and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
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

CREATE Procedure dbo.usp_IRCurve_Save
	@iCurveID int = Null,
	@vcCurveName varchar(30) = Null,
	@iRuleTypeID int = Null,
	@iCurveTypeID int = Null,
	@mPositionThreshold money = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iCurveID is Null 
		begin
			insert into IRCurve (
					curveName,
					ruleTypeID,
					curveTypeID,
					positionThreshold
					) 
				values (
					@vcCurveName,
					@iRuleTypeID,
					@iCurveTypeID,
					@mPositionThreshold
										)
			select @iCurveID = @@identity
		end
	else
		begin
			update IRCurve set
				curveName = isNull(@vcCurveName, curveName),
				ruleTypeID = isNull(@iRuleTypeID, ruleTypeID),
				curveTypeID = isNull(@iCurveTypeID, curveTypeID),
				positionThreshold = isNull(@mPositionThreshold, positionThreshold)
			where curveID = @iCurveID
		end
	set @error = @@error
	if (@error = 0) set @error = @iCurveID
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

CREATE Procedure dbo.usp_IRPoint_Del
	@iIRpointID int = Null,
	@iCurveID int = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	if @iIRpointID is not Null or @iCurveID is not Null
	begin
		begin tran
	
		update IRPoint set actionID=3 
		where (@iCurveID is Null or @iCurveID is not Null and curveID = @iCurveID)
			and (@iIRpointID is Null or @iIRpointID is not Null and IRpointID = @iIRpointID)
/*
		delete IRPoint
		where (@iCurveID is Null or @iCurveID is not Null and curveID = @iCurveID)
		and (@iIRpointID is Null or @iIRpointID is not Null and IRpointID = @iIRpointID)
*/
		if @@error = 0
			commit tran
		else
			rollback tran
	end
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

CREATE  Procedure dbo.usp_IRPoint_Get
	@iCurveID int,
	@dtActionDate datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select
		IRpointID as iIRpointID, 
		curveID as iCurveID, 
		periodTypeID as iPeriodTypeID, 
		num as iNum, 
		shortRate as fShortRate, 
		HTBRate as fHTBRate, 
		longRate as fLongRate,
		actionDate as dtActionDate,
		actionID as tiActionID
	from
		IRPoint
	where
		(@iCurveID is Null or not @iCurveID is Null and curveID = @iCurveID) and 
			(actionDate>=@dtActionDate or @dtActionDate is null)
			and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
	order by periodTypeID, num 
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

CREATE  Procedure dbo.usp_IRPoint_Save
	@iIRpointID int = Null,
	@iCurveID int = Null,
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fHTBRate float = Null,
	@fLongRate float = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID and actionID < 3 and (@iIRpointID <> IRpointID or @iIRpointID is null))
		begin
			Raiserror ('IRPoint with this period already exists!', 16, 1)
			Return (-1)
		end   
	if @iCurveID is not Null and @iIRpointID is Null
		begin
			insert into IRPoint
					(
					curveID,
					periodTypeID,
					num,
					shortRate,
					HTBRate,
					longRate
					) 
				values (
					@iCurveID,
					@iPeriodTypeID,
					@iNum,
					@fShortRate,
					@fHTBRate,
					@fLongRate
										)
			select @iIRpointID = @@identity
		end
	else if @iIRpointID is not Null
		begin
			update IRPoint set
				curveID = isNull(@iCurveID, curveID),
				periodTypeID = isNull(@iPeriodTypeID, periodTypeID),
				num = isNull(@iNum, num),
				shortRate = isNull(@fShortRate, shortRate),
				HTBRate = isNull(@fHTBRate, HTBRate),
				longRate = isNull(@fLongRate, longRate)
			where IRpointID = @iIRpointID
		end
	set @error = @@error
	if (@error = 0) set @error = @iCurveID
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

CREATE    Procedure dbo.usp_IndexCorrelationDH_Get
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null Output,
	@fCorrelationVola  float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID1 int, 
			@iIndexID2 int,
			@iIndexID3 int
	select @iIndexID1=contractID from Contract where symbol=@vcSymbol1 and contractTypeID=1
	select @iIndexID2=contractID from Contract where symbol=@vcSymbol2 and contractTypeID=1
	if 	@iIndexID1 > @iIndexID2
		select @iIndexID3=@iIndexID1,@iIndexID1=@iIndexID2,@iIndexID2=@iIndexID3
	select	@fCorrelationPrice=correlationPrice,
			@fCorrelationVola=correlationVola
		from IndexCorrelation	
		where indexID1 = @iIndexID1 and indexID2 = @iIndexID2
	set @error = @@error
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

CREATE  Procedure dbo.usp_IndexCorrelationDH_Save
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null,
	@fCorrelationVola  float = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID1 int, 
			@iIndexID2 int,
			@iIndexID3 int  
	select @iIndexID1=contractID from ContractView where symbol=@vcSymbol1 and contractTypeID=1
	select @iIndexID2=contractID from ContractView where symbol=@vcSymbol2 and contractTypeID=1
	if @iIndexID1>@iIndexID2
		select @iIndexID3=@iIndexID1,@iIndexID1=@iIndexID2,@iIndexID2=@iIndexID3
	Begin Transaction
		if not exists(select * from IndexCorrelation where IndexID1=@iIndexID1 and IndexID2=@iIndexID2) 
			begin
				insert into IndexCorrelation(IndexID1,IndexID2,correlationPrice,correlationVola)
					values(@iIndexID1,@iIndexID2,@fCorrelationPrice,@fCorrelationVola)
			end 
		else
			begin
				update IndexCorrelation
					set IndexID1=@iIndexID1,
						IndexID2=@iIndexID2,
						correlationPrice=isnull(@fCorrelationPrice,CorrelationPrice),
						correlationVola=isnull(@fCorrelationVola,CorrelationVola)
				where IndexID1=@iIndexID1 and IndexID2=@iIndexID2	
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    Procedure dbo.usp_IndexDataHistoryDH_Get
	@vcSymbol varchar(8),	-- index ticker
	@fCurrentIV float = null Output,
	@fCurrentHV float = null Output,
	@fCurrentVolOfVol float = null Output,
	@fCurrentVega float = null Output
as
/*
	Created by Zaur Nuraliev
	this procedure returns historical data for index
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int
	select @iIndexID=contractID from ContractView where symbol=@vcSymbol and contractTypeID = 1
	select 	@fCurrentIV=currentIV, 
			@fCurrentHV=currentHV,
			@fCurrentVolOfVol=currentVolOfVol,
			@fCurrentVega=currentVega
		from IndexDataHistory
		where tenorID=8 
			and [date]=(select max([date]) from IndexDataHistory where indexID=@iIndexID and tenorID=8)
			and indexID=@iIndexID
	set @error = @@error
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

CREATE Procedure dbo.usp_IndexDataHistory_Save
	@vcIndexTicker varchar(12) = null,	-- index ticker
	@iIndexID int = null,			-- index identifier
	@dtDate datetime,			-- date
	@iTenorID int = null,			-- tenor identifier
	@iTenorValue int = null,		-- tenor value
	@fDelta1 float = null,			-- delta 1 index's parameter
	@fDelta3 float = null,			-- delta 3 index's parameter 
	@fImpliedCorrelation float = null,	-- implied correlation index's parameter
	@fCurrentIV float = null,		-- current implied volatility index's parameter
	@fIVMean float = null,			-- implied volatility mean index's parameter
	@fIVCall float = null,			-- implied volatility call index's parameter
	@fIVPut float = null,			-- implied volatility put index's parameter
	@fCurrentHV float = null,		-- current historical volatility index's parameter
	@fPriceClose float = null,		-- index's close price
	@fCurrentVega float = null,
	@fCurrentVolOfVol float = null
as
/*
	Created by Andrey Sinkin
	this procedure saves historical data for index(s)
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iMarketDataID int
	-- if wrong parameters -> exit
	if @vcIndexTicker is null and @iIndexID is null or @iTenorID is null and @iTenorValue is null	goto finish
	if @iIndexID is null	select @iIndexID = contractID from ContractView where symbol = @vcIndexTicker and contractTypeID = 1
	if @iTenorID is null	select @iTenorID = tenorID from Tenor where value = @iTenorValue
	Begin Transaction
		if not exists(select * from IndexDataHistory where indexID = @iIndexID and date = @dtDate and tenorID = @iTenorID)
			begin
				insert into IndexDataHistory (indexID, date, tenorID, delta1, delta3, impliedCorrelation, currentIV, IVMean, IVCall, IVPut, currentHV,currentVega,currentVolOfVol) 
					values (@iIndexID, @dtDate, @iTenorID, @fDelta1, @fDelta3, @fImpliedCorrelation, @fCurrentIV, @fIVMean, @fIVCall, @fIVPut, @fCurrentHV,@fCurrentVega,@fCurrentVolOfVol)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexDataHistory set 
					delta1 = isnull(@fDelta1, delta1),
					delta3 = isnull(@fDelta3, delta3),
					impliedCorrelation = isnull(@fImpliedCorrelation, impliedCorrelation),
					currentIV = isnull(@fCurrentIV, currentIV),
					IVMean = isnull(@fIVMean, IVMean),
					IVCall = isnull(@fIVCall, IVCall),
					IVPut = isnull(@fIVPut, IVPut),
					currentHV = isnull(@fCurrentHV, currentHV),
					currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol),
					currentVega = isnull(@fCurrentVega,currentVega)
				where indexID = @iIndexID and date = @dtDate and tenorID = @iTenorID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
/*
		if @fPriceClose is not null
			begin
				exec usp_MarketData_Save 
					@iMarketDataID = @iMarketDataID output,	-- market data identifier
					@iContractID = @iIndexID,		-- contract's identifier
					@dDate = @dtDate,			-- date
					@fPriceClose = @fPriceClose		-- close price
				if @error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
			end
*/
		set @error = 0
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

CREATE  Procedure dbo.usp_IndexDefinition_Del 
	@iIndexID int = null, 
	@iStockID int = null 
AS
/*
	Created by ...
	Description: this procedure is delete information from IndexDefinition table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		if @iIndexID is null and @iStockID is null
			begin
				update IndexDefinition set actionID=3
				--delete from IndexDefinition
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else if @iIndexID is not null and @iStockID is null
			begin
				update IndexDefinition set actionID=3 where indexID = @iIndexID 
				--delete from IndexDefinition where indexID = @iIndexID 
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else if @iIndexID is null and @iStockID is not null
			begin
				update IndexDefinition set actionID=3 where stockID = @iStockID
				--delete from IndexDefinition where stockID = @iStockID
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else if @iIndexID is not null and @iStockID is not null
			begin
				update IndexDefinition set actionID=3 where stockID = @iStockID and indexID = @iIndexID 
				--delete from IndexDefinition where stockID = @iStockID and indexID = @iIndexID 
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
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

CREATE    Procedure dbo.usp_IndexDefinition_Get 
	@iIndexID int = Null, 
	@iStockID int = Null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexDefinition table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select indexID as iIndexID,
			stockID as iStockID,
			weight as fWeight,
			beta as fBeta,
			actionDate as dtActionDate,
			actionID as tiActionID
		from IndexDefinition
		where ((@iIndexID is null or indexID = @iIndexID) and (@iStockID is null or stockID = @iStockID))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
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

CREATE    Procedure dbo.usp_IndexDefinition_Save 
	@iIndexID int, --the index identifier
	@iStockID int, --the stock identifier
	@fWeight int = null,  --the weight
	@fBeta float = null
as
/*
	Created by ...
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if 	@iIndexID is null and @iStockID is null 
		Return(-2)
	if exists (select * from IndexDefinition where indexID = @iIndexID and stockID=@iStockID and actionID < 3)
		begin
			Raiserror ('Row in IndexDefinition table with same indexID and StockID already exists!', 16, 1)
			Return (-1)
		end   
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

CREATE  procedure usp_IndexID_Get 
	   @vcSymbol varchar(8) 
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting index identifiers
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select C.contractID as iIndexID
		from ContractView c 	
				join IndexView I on I.indexID = C.contractID
		where C.contractTypeID = 1 and C.symbol = @vcSymbol
	set @error = @@error
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

CREATE Procedure dbo.usp_IndexStockCorrelationDH_Get
	@vcIndexSymbol varchar(8), 
	@vcStockSymbol varchar(8),
	@fCorrelationPrice float = Null Output,
	@fCorrelationVola  float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexStockCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int, 
			@iStockID int
	select @iIndexID=contractID from ContractView where symbol=@vcIndexSymbol and contractTypeID=1
	select @iStockID=contractID from ContractView where symbol=@vcStockSymbol and contractTypeID=2
	select	@fCorrelationPrice=correlationPrice,
			@fCorrelationVola=correlationVola
		from IndexDefinitionView
		where indexID = @iIndexID and stockID = @iStockID
	set @error = @@error
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

CREATE   Procedure dbo.usp_IndexStockCorrelationDH_Save
	@vcIndexSymbol varchar(8), 
	@vcStockSymbol varchar(8),
	@fCorrelationPrice float = Null,
	@fCorrelationVola  float = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save Index-Stock correlation information
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int, 
			@iStockID int
	select @iIndexID=contractID from ContractView where symbol=@vcIndexSymbol and contractTypeID=1
	select @iStockID=contractID from ContractView where symbol=@vcStockSymbol and contractTypeID=2
	if (@iIndexID is null or @iStockID is null)
		goto finish
	Begin Transaction
	if not exists(select * from IndexDefinitionView where indexID=@iIndexID and stockID=@iStockID)
		begin
			insert into IndexDefinition(indexID,stockID,correlationPrice,correlationVola,weight) 
				values(@iIndexID,@iStockID,@fCorrelationPrice,@fCorrelationVola,0)
   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		end
	else
		begin
			update IndexDefinition
				set indexID=@iIndexID,
					stockID=@iStockID,
					correlationPrice=isnull(@fCorrelationPrice,correlationPrice),
					correlationVola=isnull(@fCorrelationVola,correlationVola)
			where indexID=@iIndexID and stockID=@iStockID
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

Create Procedure dbo.usp_IndexStockCorrelation_Get 
	@iIndexID int,
	@iStockID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting  correlation information from IndexDefinition table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 
		correlationPrice as fCorrelationPrice,
		correlationVola as fCorrelationVola
		from IndexDefinitionView
		where indexID = @iIndexID and stockID = @iStockID
	set @error = @@error
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

CREATE  Procedure dbo.usp_IndexVM_Save
	@iIndexID int = null output,		-- index's identifier
	@iContractTypeID int = null,		-- contract's type identifier
	@iSymbolName varchar(12) = null,	-- index's ticker
	@iExchangeID int = null,			-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,				-- lot size (not used)
	@fYield float = null,				-- yield
	@bIsAutoUpdateWeight bit = null,	-- flag for enable/disable index's weights updating
	@iTenorID int = null,				-- tenor's identifier
	@dtDate datetime = null,			-- date
	@fDelta1 float = null,				-- delta 1
	@fDelta3 float = null,				-- delta 3
	@fImpliedCorrelation float = null,	-- implied correlation
	@fCurrentIV float = null,			-- current implied volatility
	@fIVMean float = null,				-- implied volatility mean
	@fCurrentHV float = null,			-- current historical volatility
	@iTenorValue int = null,			-- tenor's value
	@fIVCall float = null,				-- iv call value
	@fIVPut float = null,				-- iv put value
	@iIndexBaseID int = null,			-- index's base identifier
	@rDenominator real = null,			-- denominator
	@bIsActive bit = null,				-- flag for enable/disable
	@fPriceClose float = null,			-- index's close price
	@iGroupID int = null,
	@iRuleID int = null
as
/*
	Created by Andrey Sinkin
	This procedure saves index data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	if @iIndexID is null and @iSymbolName is null 		goto finish
	set @iContractTypeID = 1	-- index
	set @iLotSize = 0
	if @iIndexID is null and @iSymbolName is not null
		select @iIndexID = contractID from ContractView where symbol = @iSymbolName and contractTypeID = 1
	if @iTenorID is null	select @iTenorID = tenorID from Tenor where value = @iTenorValue
	Begin Transaction
		if @iIndexID is null
			begin
				insert into Contract (contractTypeID, symbol, contractName, groupID,ruleID)
					values (@iContractTypeID, @iSymbolName, @vcDescription,@iGroupID,@iRuleID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iIndexID = @@identity
--				insert into [Index] (indexID, yield, isAutoUpdateWeight) values (@iIndexID, @fYield, @bIsAutoUpdateWeight)
				insert into [Index] (indexID, yield) values (@iIndexID, @fYield)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				if @iContractTypeID is not null or @iSymbolName is not null or @iExchangeID is not null or @vcDescription is not null or @iLotSize is not null
					begin
						update Contract set 
							contractTypeID = isnull(@iContractTypeID, contractTypeID),
							symbol = isnull(@iSymbolName, symbol),
							contractName = isnull(@vcDescription, contractName),
							groupID = isnull(@iGroupID, groupID),
							ruleID = isnull(@iRuleID, ruleID)
						where contractID = @iIndexID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
					end
				
				--if @fYield is not null or @bIsAutoUpdateWeight is not null
				if @fYield is not null 
					begin
						update [Index] set 
							yield = isnull(@fYield, yield)
							--isAutoUpdateWeight = isnull(@bIsAutoUpdateWeight, isAutoUpdateWeight)
						where indexID = @iIndexID
						if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
					end
			end
		--save lotSize value
		if @iLotSize is not null
			begin
				if exists(select * from ContractPrice where contractID = @iIndexID and (exchangeID = @iExchangeID or @iExchangeID is null))
					update ContractPrice 
						set lotSize = isnull(@iLotSize,lotSize),
							exchangeID = @iExchangeID,
							contractID = isnull(@iIndexID,contractID)
						where contractID = @iIndexID and (exchangeID = @iExchangeID or @iExchangeID is null)
					if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
				else
					insert into ContractPrice (contractID,exchangeID,lotSize)
						values(@iIndexID,@iExchangeID,@iLotSize)
					if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
			end
--=================================================================
-- related data update
--=================================================================
		if @dtDate is not null or @fDelta1 is not null or @fDelta3 is not null or @fImpliedCorrelation is not null or @fCurrentIV is not null or @fIVMean is not null or @fPriceClose is not null
			begin
			   RAISERROR ('Error. Can''t save index data history (attempt of a call usp_IndexDataHistory_Save)! Please send this error message to DB developer !', 16, 1)
/*				exec @error = usp_IndexDataHistory_Save 
					@iIndexID = @iIndexID, 
					@dtDate = @dtDate, 
					@iTenorID = @iTenorID, 
					@fDelta1 = @fDelta1, 
					@fDelta3 = @fDelta3, 
					@fImpliedCorrelation = @fImpliedCorrelation, 
					@fCurrentIV = @fCurrentIV, 
					@fIVMean = @fIVMean, 
					@fCurrentHV = @fCurrentHV, 
					@fIVCall = @fIVCall, 
					@fIVPut = @fIVPut,
					@fPriceClose = @fPriceClose
				if @error <> 0 begin set @error = 5 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
		if @fCurrentHV is not null
			begin
			   RAISERROR ('Error. Can''t save index data snapshot (attempt of a call usp_IndexDataSnapshot_Save)! Please send this error message to DB developer !', 16, 1)
/*				exec @error = usp_IndexDataSnapshot_Save 
					@iIndexID = @iIndexID, 
					@iTenorID = @iTenorID, 
					@fCurrentHV = @fCurrentHV, 
					@dtDateUpdate = @dtDate
				if @error <> 0 begin set @error = 6 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
		if @iIndexBaseID is not null
			begin
			   RAISERROR ('Error. Can''t save ETFLink data (attempt of a call usp_ETFLink_Save)! Please send this error message to DB developer !', 16, 1)
/*				exec @error = usp_ETFLink_Save 
					@iIndexID = @iIndexID, 
					@iIndexBaseID = @iIndexBaseID, 
					@rDenominator = @rDenominator, 
					@bIsActive = @bIsActive
				if @error <> 0 begin set @error = 7 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
/*		if @fPriceClose is not null
			begin
				exec usp_MarketData_Save 
					@iMarketDataID = @iMarketDataID output,	-- market data identifier
					@iContractID = @iIndexID,		-- contract's identifier
					@dDate = @dtDate,			-- date
					@fPriceClose = @fPriceClose		-- close price
				if @error <> 0 begin set @error = 8 if @@TranCount = 1 Rollback Transaction goto finish end
			end
*/
		set @error = 0
	Commit Transaction
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

CREATE	  proc usp_IndexWithPrice_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@bIsBestPriceOnly bit = 0
  as
	if (@iGroupID is Null)
		if @bIsBestPriceOnly=0
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				I.defaultModelTypeID as iDefaultModelTypeID,
				I.yield as fYield,
				CP.priceLast as fPriceLast,
				CP.priceClose as fPriceClose,
				CP.priceOpen as fPriceOpen,
				CP.priceLow as fPriceLow,
				CP.priceHigh as fPriceHigh,
				CP.volume as iVolume,
				E.exchangeID as iExchangeID,
				E.exchangeCode as vcExchangeCode
				from ContractView c 	
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractTypeID=1
					and (C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null)
				order by C.symbol,E.exchangeCode
		else 
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				I.defaultModelTypeID as iDefaultModelTypeID,
				I.yield as fYield,
				CP.priceLast as fPriceLast,
				CP.priceClose as fPriceClose,
				CP.priceOpen as fPriceOpen,
				CP.priceLow as fPriceLow,
				CP.priceHigh as fPriceHigh,
				CP.volume as iVolume,
				E.exchangeID as iExchangeID,
				E.exchangeName as vcExchangeCode
				from ContractView c 	
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractTypeID=1
					and (C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null)
					and CP.exchangeID is null
				order by C.symbol,E.exchangeCode
	else
		select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize,
			I.defaultModelTypeID as iDefaultModelTypeID,
			I.yield as fYield,
			CP.priceLast as fPriceLast,
			CP.priceClose as fPriceClose,
			CP.priceOpen as fPriceOpen,
			CP.priceLow as fPriceLow,
			CP.priceHigh as fPriceHigh,
			CP.volume as iVolume,
			E.exchangeID as iExchangeID,
			E.exchangeCode as vcExchangeCode
			from ContractViewInGroup CIG
					join ContractView c  on C.contractID = CIG.contractID
					join IndexView I on I.indexID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where CIG.groupID = @iGroupID
					 and C.contractTypeID=1
			order by C.symbol,E.exchangeCode
if (@@error <> 0)
	Return (-1)
else  
	Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_IndexYield_Save 
	@iIndexID int,
	@fYield float =null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if @iIndexID is null
		--if not exists(select * From [Index] where indexID=@iIndexID) 
			begin
				insert into [Index] (indexID,yield,isHTB) 
					values (@iIndexID,@fYield,0)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update [Index] set 
					yield = isnull(@fYield, yield)
				where indexID = @iIndexID
	
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

CREATE    proc usp_Index_Get
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
					I.actionID as tiActionID
				from ContractView c 	
						join [Index] I on I.indexID = C.contractID
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
					I.actionID as tiActionID
				from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
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
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 	
						join [Index] I on I.indexID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID
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
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID
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

CREATE	proc usp_Index_Save
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_InterestRateCurve_Del
	@iCurvePointID int	-- InterestRateCurve's identifier
as
/*
	Created by Andrey Sinkin
	this procedure deletes data from InterestRateCurve table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	-- declare @error int	set @error = -1
	--delete InterestRateCurve where curvePointID = @iCurvePointID
	exec usp_IRPoint_Del @iIRpointID = @iCurvePointID
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

CREATE Procedure dbo.usp_InterestRateCurve_Get
	@iCurvePointID int = null,	-- InterestRateCurve identifier
	@bAdvancedMode bit = 0		-- flag for advanced mode
as
/*
	Created by Andrey Sinkin
	this procedure returns data from InterestRateCurve table (now from IRPoint table)
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @date datetime		set @date = getdate()
	if @bAdvancedMode = 0
		select 	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iPeriodTypeID, iNumber
	else
		select	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid,
			case periodTypeID
				when 1 then datediff(day, getdate(), dateadd(day, num, 
@date))
				when 2 then datediff(day, getdate(), dateadd(week, num, 
@date))
				when 3 then datediff(day, getdate(), dateadd(month, num, 
@date))
				when 4 then datediff(day, getdate(), dateadd(year, num, 
@date))
			end iDaysFromToday
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iDaysFromToday
/*	if @bAdvancedMode = 0
		select 	curvePointID iCurvePointID,	periodTypeID iPeriodTypeID, number 
iNumber, rateAsk fRateAsk, rateBid fRateBid
			from InterestRateCurve
			where @iCurvePointID is null or curvePointID = @iCurvePointID
			order by iPeriodTypeID, iNumber
	else
		select curvePointID iCurvePointID, periodTypeID iPeriodTypeID, number 
iNumber, rateAsk fRateAsk, rateBid fRateBid,
			case periodTypeID
				when 1 then datediff(day, getdate(), dateadd(day, number, 
@date))
				when 2 then datediff(day, getdate(), dateadd(week, number, 
@date))
				when 3 then datediff(day, getdate(), dateadd(month, number, 
@date))
				when 4 then datediff(day, getdate(), dateadd(year, number, 
@date))
			end iDaysFromToday
			from InterestRateCurve
			where @iCurvePointID is null or curvePointID = @iCurvePointID
			order by iDaysFromToday */
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

CREATE Procedure dbo.usp_InterestRateCurve_Save
	@iCurvePointID int = null output,	-- InterestRateCurve's identifier
	@iPeriodTypeID int = null,		-- type of periods identifier
	@iNumber int = null,			-- number
	@fRateAsk float = null,			-- ask
	@fRateBid float = null			-- bid
as
/*
	Created by Andrey Sinkin
	this procedure saves data to IRPoint (InterestRateCurve) table 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iCurveID int
		select @iCurveID=curveID from IRCurve
	if @iCurvePointID is null and @iPeriodTypeID is not null and @iNumber is not null
		select @iCurvePointID = IRPointID from IRPointView where periodTypeID = 
@iPeriodTypeID and num = @iNumber
	Begin Transaction
		if @iCurvePointID is null 
			begin
				insert into IRPoint (periodTypeID, num, longRate, ShortRate, 
HTBRate,CurveID) 
				values (@iPeriodTypeID, @iNumber, @fRateAsk, 
@fRateBid,@fRateAsk,@iCurveID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 
Rollback Transaction goto finish end
				select @iCurvePointID = @@identity
			end
		else
			begin
				update IRPoint set
					periodTypeID = isnull(@iPeriodTypeID, periodTypeID),
					num = isnull(@iNumber, num),
					longRate = isnull(@fRateAsk, longRate),
					shortRate = isnull(@fRateBid, shortRate)
				where IRPointID = @iCurvePointID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 
Rollback Transaction goto finish end
			end
/*	if @iCurvePointID is null and @iPeriodTypeID is not null and @iNumber is not null
		select @iCurvePointID = curvePointID from InterestRateCurve where 
periodTypeID = @iPeriodTypeID and number = @iNumber
	Begin Transaction
		if @iCurvePointID is null 
			begin
				insert into InterestRateCurve (periodTypeID, number, 
rateAsk, rateBid) 
				values (@iPeriodTypeID, @iNumber, @fRateAsk, @fRateBid)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 
Rollback Transaction goto finish end
				select @iCurvePointID = @@identity
			end
		else
			begin
				update InterestRateCurve set
					periodTypeID = isnull(@iPeriodTypeID, periodTypeID),
					number = isnull(@iNumber, number),
					rateAsk = isnull(@fRateAsk, rateAsk),
					rateBid = isnull(@fRateBid, rateBid)
				where @iCurvePointID = curvePointID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 
Rollback Transaction goto finish end
			end */
		set @error = 0
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

CREATE  Procedure dbo.usp_IsSymbolExists_Get 
	@vcSymbol varchar(8)
AS
/*
	Created by ...
	Description: this procedure is getting information from Contract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select contractID as iContractID
		from ContractView	
		where contractTypeID < 3 and symbol=@vcSymbol
	set @error = @@error
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

Create procedure usp_LastCacheID_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now integer
		exec usp_CurrentDate_Get @Now output
	select count(*) from ClearingHouseTrade where iDateOfTrade = @Now
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

CREATE PROC usp_LastTradeSeqNum_Get
AS
	select max(seqNum) as iSeqNum
	from Trade 

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    procedure usp_LastTradeTime_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select convert(int,keyValue) as iLastTradeTime from DataInfo
		where keyName='lastTradeTime'
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

Create  procedure usp_LastTradeTime_Save
	@iLastTradeTime int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	update DataInfo set keyValue = @iLastTradeTime
		where keyName='lastTradeTime'
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

/********************************************************
	name: usp_MLState_Del
	Author: SP Generator V1.0.6
	Date: 26.03.2001
*********************************************************/
CREATE Procedure dbo.usp_MLState_Del
	@iMlStateID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	delete MLState Where mlStateID = @iMlStateID
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

/********************************************************
	name: usp_MLState_Get
	Author: SP Generator V1.0.6
	Date: 26.03.2001
*********************************************************/
CREATE Procedure dbo.usp_MLState_Get
	@iMlStateID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		if (@iMlStateID is Null) 
		   select mlStateID as iMlStateID, mlType as iMlType, name as vcName, columns as vcColumns, groups as vcGroups, data as vcData
		from MLState
		 else
		   select mlStateID as iMlStateID, mlType as iMlType, name as vcName, columns as vcColumns, groups as vcGroups, data as vcData
		from MLState
		where mlStateID = @iMlStateID
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

/********************************************************
	name: usp_MLState_Save
	Author: SP Generator V1.0.6
	Date: 26.03.2001
*********************************************************/
CREATE Procedure dbo.usp_MLState_Save
	@iMlStateID int out,
	@iMLType int,
	@vcName varchar,
	@vcColumns varchar,
	@vcGroups varchar,
	@vcData varchar
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iMlStateID is Null 
		begin
			insert into MLState (
					mlType,
					name,
					columns,
					groups,
					data) 
				values (
					@iMLType,
					@vcName,
					@vcColumns,
					@vcGroups,
					@vcData)
			select @iMlStateID = @@identity
		end
	else
		begin
			update MLState set
				mlType = isNull(@iMLType, mlType),
				name = isNull(@vcName, name),
				columns = isNull(@vcColumns, columns),
				groups = isNull(@vcGroups, groups),
				data = isNull(@vcData, data)
			where @iMlStateID = mlStateID
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

/********************************************************
	name: usp_ModelParameter_Del
	Author: SP Generator V1.0.6
	Date: 25.01.2001
*********************************************************/
CREATE Procedure dbo.usp_ModelParameter_Del
	@iParameterID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	delete ModelParameter Where parameterID = @iParameterID
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

CREATE Procedure dbo.usp_ModelParameter_Get
		@iContractID int,
		@bDefaultOnly bit,
		@iParameterID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		if (@iParameterID is Null) 
		   if (@iContractID is Null)			
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
				 from ModelParameter
				order by contractID, modelTypeID
			 else
			  if ((@bDefaultOnly is NOT Null) and (@bDefaultOnly = 1))		  
				  select mp.parameterID as iParameterID, mp.contractID as iContractID, mp.modelTypeID as iModelTypeID, mp.param1 as fParam1, mp.param2 as fParam2, mp.param3 as fParam3
					from StockView s, ModelParameter mp
				   where s.stockID = @iContractID
					 and mp.contractID = s.stockID
					 and mp.modelTypeID = s.defaultModelTypeID 
				  union 
				  select mp.parameterID as iParameterID, mp.contractID as iContractID, mp.modelTypeID as iModelTypeID, mp.param1 as fParam1, mp.param2 as fParam2, mp.param3 as fParam3
					from IndexView i, ModelParameter mp
				   where i.indexID = @iContractID
					 and mp.contractID = i.indexID
					 and mp.modelTypeID = i.defaultModelTypeID
				else
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
					from ModelParameter
				   where contractID = @iContractID
				  order by modelTypeID
		 else
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
		from ModelParameter
		where parameterID = @iParameterID
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

/********************************************************
	name: usp_ModelParameter_Save
	Author: SP Generator V1.0.6
	Date: 25.01.2001
*********************************************************/
CREATE Procedure dbo.usp_ModelParameter_Save
	@iParameterID int out,
	@iContractID int,
	@iModelTypeID int,
	@fParam1 float,
	@fParam2 float,
	@fParam3 float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iParameterID is Null 
		begin
			insert into ModelParameter (
					contractID,
					modelTypeID,
					param1,
					param2,
					param3) 
				values (
					@iContractID,
					@iModelTypeID,
					@fParam1,
					@fParam2,
					@fParam3)
			select @iParameterID = @@identity
		end
	else
		begin
			update ModelParameter set
				contractID = isNull(@iContractID, contractID),
				modelTypeID = isNull(@iModelTypeID, modelTypeID),
				param1 = isNull(@fParam1, param1),
				param2 = isNull(@fParam2, param2),
				param3 = isNull(@fParam3, param3)
			where @iParameterID = parameterID
		end
	set @error = @@error
if (@error = 0) set @error = @iParameterID
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

CREATE  Procedure dbo.usp_NextDaySurface_Del 
	@iNextDaySurfaceID int = null, 	-- the row identifier
	@iContractID int = null, 		-- the contract identifier
	@iVolasurfaceDataID int = null  -- the VolasurfaceData table identifier
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from NextDaySurface table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete NextDaySurface	
			where (nextDaySurfaceID = @iNextDaySurfaceID 
					and @iNextDaySurfaceID is not null and @iContractID is null and @iVolasurfaceDataID is null)
				or
				(@iNextDaySurfaceID is null and @iContractID is not null and @iVolasurfaceDataID is not null and ContractID=@iContractID and volasurfaceDataID=@iVolasurfaceDataID)
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
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

CREATE Procedure dbo.usp_NextDaySurface_Get 
	@iNextDaySurfaceID int = Null 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from NextDaySurface table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select
		nextDaySurfaceID as iNextDaySurfaceID,
		Contract.contractID as iContractID,
		VolaSurfaceData.volaSurfaceDataID as iVolaSurfaceDataID,
		dueDate as dtDueDate,
		Contract.symbol as vcSymbolName,
		VolaSurfaceData.surfaceName as SurfaceName
		from NextDaySurface	
				join ContractView Contract on NextDaySurface.contractID=Contract.contractID
				join VolaSurfaceData on NextDaySurface.volaSurfaceDataID=VolaSurfaceData.volaSurfaceDataID
		where (@iNextDaySurfaceID is null or nextDaySurfaceID=@iNextDaySurfaceID)
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

CREATE   Procedure dbo.usp_NextDaySurface_Save 
	@iNextDaySurfaceID int = Null output,	--the row identifier
	@iContractID int = Null, 				--the Contract identifier
	@iVolaSurfaceDataID int = Null,			--the surface data ID
	@dtDueDate datetime = Null				--the due date value
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into NextDaySurface table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if 	@iNextDaySurfaceID is null and @iVolaSurfaceDataID is null and @iContractID is null
		goto finish	
	if @dtDueDate is null
		set @dtDueDate=GetDate()+1
	Begin Transaction
		if not exists(select * From NextDaySurface where nextDaySurfaceID=@iNextDaySurfaceID and @iNextDaySurfaceID is not null) 
			and not exists (select * From NextDaySurface where contractID=@iContractID and volaSurfaceDataID=@iVolaSurfaceDataID and @iNextDaySurfaceID is null)
			begin
				insert into NextDaySurface (contractID,volaSurfaceDataID,dueDate) 
					values (@iContractID,@iVolaSurfaceDataID,@dtDueDate)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			
				set @iNextDaySurfaceID=@@identity
			end
		else
			begin
				update NextDaySurface 
					set 
						--contractID=isnull(@iContractID, contractID),
						--volaSurfaceDataID=isnull(@iVolaSurfaceDataID, volaSurfaceDataID),
						dueDate=isnull(@dtDueDate, dueDate)
				where (nextDaySurfaceID=@iNextDaySurfaceID and @iNextDaySurfaceID is not null) or (contractID=@iContractID and volaSurfaceDataID=@iVolaSurfaceDataID and @iVolaSurfaceDataID is not null and @iContractID is not null)
	
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

CREATE   Procedure dbo.usp_OptionDataCache_Del
	@iOptionID int = Null,	-- option's identifier
	@iContractID int = Null	-- contract's identifier
as
/*
	Create by Andrey Sinkin
	It deletes data from OptionDataCache table
	--Changed by Zaur Nuraliev at 19 oct. 2001 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	if @iOptionID Is Not Null and @iContractID Is Null
		Begin
			Delete OptionDataCache where optionID = @iOptionID
			set @error = @@error
		End
	Else if @iOptionID Is Null and @iContractID Is Not Null
		Begin
			Delete OptionDataCache 
				Where optionId in 
					(Select optionId 
					From OptionView O join ContractView c  on C.contractID = O.underlyingContractID 
					Where C.contractID =@iContractID)
			set @error = @@error
		End
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

CREATE  Procedure dbo.usp_OptionDataCache_Get
	@iContractID int,			-- Contract's identifier
	@vcOptionTicker varchar(12),	-- option's ticker
	@bIsCall bit = 1		-- flag for call/put option status
as
/*
	Create by Andrey Sinkin
	It gets data from OptionDataCache table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iOptionID int
	if @iContractID is not null
		begin
			if @bIsCall is not null
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV,priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where underlyingContractID = @iContractID and isCall = @bIsCall
			else
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV, priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where underlyingContractID = @iContractID
		end
	else
		begin
			if @bIsCall is not null
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV, priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where symbol = @vcOptionTicker and contractTYpeID = 3 and isCall = @bIsCall
			else
				select Contract.symbol vcOptionTicker, date dtDate, IV fIV, priceMid fPriceMid, isCall bIsCall
					from OptionDataCache
					inner join OptionView on OptionDataCache.optionID = OptionView.optionID
					inner join ContractView Contract on OptionDataCache.optionID = Contract.contractID
					where symbol = @vcOptionTicker and contractTYpeID = 3	-- option
		end
	set @error = @@error
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

CREATE   Procedure dbo.usp_OptionDataCache_Save
	@vcOptionTicker varchar(12),	-- option's ticker
	@dtDate datetime,				-- date
	@fIV float,						-- implied volatility
	@fPriceMid float				-- middle price
as
/*
	Create by Andrey Sinkin
	It deletes data from OptionDataCache table
	-- changed by Zaur Nuraliev 
	-- 19 oct. 2001
	-- removed  @fHVLow & @fHVHigh parametrs 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @rowcount int
	declare @iOptionID int		-- contract's identifier
	select @iOptionID = contractID from ContractView where symbol = @vcOptionTicker and contractTypeID = 3	-- option
		if @iOptionID is null 
			goto finish
	if @dtDate is null	set @dtDate = getdate()
	Begin Transaction
		update OptionDataCache set
			IV = isnull(@fIV, IV),
			priceMid = isnull(@fPriceMid, priceMid),
			date = @dtDate
		where optionID = @iOptionID
		select @error = @@error, @rowcount = @@rowcount
		if @error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		if @rowcount = 0
			begin
				insert into OptionDataCache (optionID, date, IV, priceMid) values (@iOptionID, @dtDate, @fIV, @fPriceMid)
				if @error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end			
			end
		set @error = 0
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

CREATE   procedure dbo.usp_OptionPair_Del
	@iOptionIDCall int,
	@iOptionIDPut int
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Option table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Create table #Options_for_delete(OptionID int)
	if  @iOptionIDCall is not null
		begin
			insert into #Options_for_delete values(@iOptionIDCall)
				if @@error > 0 begin set @error = 1 goto finish end
		end
	if  @iOptionIDPut is not null
		begin
			insert into #Options_for_delete values(@iOptionIDPut)
			if @@error > 0 begin set @error = 1 goto finish end
		end
	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Error. Can''t delete Option because of the existing Trade', 16, 1)
			Return (-1)
		end 
	if object_ID('tempdb..#Options_for_delete') is not null
		begin
			exec usp_Option_Del
			if @@error <> 0 set @error = 1 goto finish
		end
	set @error = @@error
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

Create Procedure dbo.usp_OptionPermanent_Del
	@iOptionID int = null 	-- contract identifier
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
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete ContractPriceHistory where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	Commit Transaction DelOpt
/*
		update [Option] set actionID=3 where underlyingContractID = @iOptionID
--		delete [Option] where underlyingContractID = @iOptionID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where optionID = @iOptionID
--		delete [Option] where optionID = @iOptionID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select optionID from #OptionGarbage)
--		delete Contract where contractID in (select optionID from #OptionGarbage)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where ContractID = @iOptionID
--		delete Contract where ContractID = @iOptionID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
*/
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

create proc usp_OptionUnderlying_Get
		@iOptionID int
  as
	 select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk
	   from OptionView O
				join ContractView c  on C.contractID = O.underlyingContractID
				left join ContractPrice CP on C.contractID=CP.contractID
	   where O.optionID = @iOptionID
		 
	 Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_OptionVM_Get
	@iOptionID int = null,			-- option's identifier
	@iUnderlyingID int,				-- underlying's identifier
	@dtExpDateMin datetime = null,	-- expiration's date
	@bIsCall bit = null,
	@iSortOrder int = 1	--			-- 1, 2 or 3. see comments below
as
/*
	Created by Andrey Sinkin
	This procedure returns option's data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iSortOrder = 1	-- sort by symbol
		select	OptionView.OptionID iOptionID, 
				OptionView.underlyingContractID iUnderlyingID, 
				isCall bIsCall, 
				strike fStrike, 
				expiry dtExpDate,
				Contract.symbol vcSymbolName, 
				null iExchangeID,
				Contract.contractName vcDescription,
				ContractPrice.lotSize iLotSize
				from OptionView
						join ContractView Contract on OptionView.OptionID = Contract.contractID
						join ContractPrice on ContractPrice.contractID = Contract.contractID and exchangeID is null
				where OptionView.underlyingContractID = @iUnderlyingID
						and (@iOptionID is null or OptionView.OptionID = @iOptionID)
						and (@dtExpDateMin is null or OptionView.expiry >= @dtExpDateMin)
						and (@bIsCall is null or OptionView.isCall = @bIsCall)
				order by Contract.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike
		select	OptionView.OptionID iOptionID, 
				OptionView.underlyingContractID iUnderlyingID, 
				isCall bIsCall, 
				strike fStrike, 
				expiry dtExpDate,
				Contract.symbol vcSymbolName, 
				null iExchangeID,
				Contract.contractName vcDescription, 
				ContractPrice.lotSize iLotSize
			from OptionView
					join ContractView Contract on OptionView.OptionID = Contract.contractID
					join ContractPrice on ContractPrice.contractID = Contract.contractID and exchangeID is null
			where OptionView.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or OptionView.OptionID = @iOptionID)
					and (@dtExpDateMin is null or OptionView.expiry >= @dtExpDateMin)
					and (@bIsCall is null or OptionView.isCall = @bIsCall)
			order by isCall, year(expiry),  month(expiry), strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	OptionView.OptionID iOptionID,
				OptionView.underlyingContractID iUnderlyingID, 
				isCall bIsCall, 
				strike fStrike, 
				expiry dtExpDate,
				Contract.symbol vcSymbolName, 
				null iExchangeID,
				Contract.contractName vcDescription,	
				ContractPrice.lotSize iLotSize
			from OptionView
					join ContractView Contract on OptionView.OptionID = Contract.contractID
					join ContractPrice on ContractPrice.contractID = Contract.contractID and exchangeID is null
			where OptionView.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or OptionView.OptionID = @iOptionID)
					and (@dtExpDateMin is null or OptionView.expiry >= @dtExpDateMin)
					and (@bIsCall is null or OptionView.isCall = @bIsCall)
			order by expiry, strike, left(Contract.symbol, len(Contract.symbol) - 2), isCall desc
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

CREATE       Procedure dbo.usp_OptionVM_Save
	@iOptionID int = null output,		-- option's identifier
	@iContractTypeID int = null,		-- contract's type identifier
	@iSymbolName varchar(12) = null,	-- option's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot's size
	@iUnderlyingID int = null,		-- underlying identifier
	@iIsCall bit = null,			-- flag for call/put
	@fStrike float = null,			-- strike
	@dtExpDate datetime = null,		-- expiration's date
	@iGroupID int = null,
	@iRuleID int = null
as
/*
	Created by Andrey Sinkin
	It saves option data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @iOptionID is null and @iUnderlyingID is not null and @iIsCall is not null and @fStrike is not null and @dtExpDate is not null 
		select @iOptionID = optionID from OptionView where underlyingContractID = @iUnderlyingID and isCall = @iIsCall and strike = @fStrike and convert(varchar(6), expiry, 112) = convert(varchar(6), @dtExpDate, 112)
	if @iOptionID is null and @iSymbolName is not null
		select @iOptionID = contractID from ContractView where symbol = @iSymbolName and contractTypeID = 3	-- option
	set @iContractTypeID = 3
	if @iLotSize is null		set @iLotSize = 100
	if exists (select * from Contract where symbol = @iSymbolName and contractTypeID=3 and actionID < 3 and (@iOptionID <> contractID or @iOptionID is null))
		begin
			Raiserror ('Option with this symbol already exists!', 16, 1)
			Return (-1)
		end   
	Begin Transaction
		if @iOptionID is null
			begin
				if @iLotSize is null	set @iLotSize = 100
				insert into Contract (contractTypeID, symbol, contractName, groupID, ruleID) 
					values (@iContractTypeID, @iSymbolName, @vcDescription, @iGroupID, @iRuleID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iOptionID = @@identity
				insert into [Option] (OptionID, underlyingContractID, isCall, strike, expiry) 
					values (@iOptionID, @iUnderlyingID, @iIsCall, @fStrike, @dtExpDate)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				if @iContractTypeID is not null or @iSymbolName is not null or @iExchangeID is not null or @vcDescription is not null or @iLotSize is not null
					begin
						update Contract set 
							contractTypeID = isnull(@iContractTypeID, contractTypeID),
							symbol = isnull(@iSymbolName, symbol),
							contractName = isnull(@vcDescription, contractName),
							groupID = isnull(@iGroupID, groupID),
							ruleID = isnull(@iRuleID, ruleID)
						where contractID = @iOptionID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
					end
				
				if @iUnderlyingID is not null or @iIsCall is not null or @fStrike is not null or @dtExpDate is not null
					begin
--select @iUnderlyingID as '@iUnderlyingID',@iIsCall as '@iIsCall',@fStrike as '@fStrike',@dtExpDate as '@dtExpDate'
						update [Option] set 
							underlyingContractID = isnull(@iUnderlyingID, underlyingContractID),
							isCall = isnull(@iIsCall, isCall),
							strike = isnull(@fStrike, strike),
							expiry = isnull(@dtExpDate, expiry)
						where OptionID = @iOptionID
						if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
						
					end
				if not exists (select * from OptionView where  optionID=@iOptionID)
					begin
						insert into [Option] (OptionID, underlyingContractID, isCall, strike, expiry) values (@iOptionID, @iUnderlyingID, @iIsCall, @fStrike, @dtExpDate)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
					end 
			end
		if not exists(select * from ContractPrice where contractID = @iOptionID and (exchangeID = @iExchangeID or @iExchangeID is null))
			insert into ContractPrice (contractID,exchangeID,lotSize) 
				values (@iOptionID, @iExchangeID, @iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		else
			update ContractPrice
				set lotSize = isnull(@iLotSize,lotSize),
					exchangeID = isnull(@iExchangeID, exchangeID),
					contractID = isnull(@iOptionID, contractID)
				where contractID = @iOptionID and (exchangeID = @iExchangeID or @iExchangeID is null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		-- now we are synchronizing expirations
		exec @error = usp_Option_expDate_Sync
			@iUnderlyingID = @iUnderlyingID,	-- underlying identifier
			@dtExpDate = @dtExpDate			-- expiration
		set @error = 0
	Commit Transaction
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

CREATE	proc usp_OptionWithPrice_Get
	  @iUnderlyingID int = Null,		 
	  @dtMinExpiry datetime = Null,
	  @iOptionID int = Null,
	  @bIsBestPriceOnly bit = 0
  as
   select	C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			O.underlyingContractID as iUnderlyingContractID,
			C2.contractTypeID as iUnderlyingContractTypeID,
			O.strike as fStrike, 
			O.expiry as dtExpiry,
			O.isCall as bIsCall,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize, 
			CP.priceLast as fPriceLast,
			CP.priceClose as fPriceClose,
			CP.priceOpen as fPriceOpen,
			CP.priceLow as fPriceLow, 
			CP.priceHigh as fPriceHigh,
			CP.volume as iVolume,
			CP.openInterest as iOpenInterest, 
			E.exchangeID as iExchangeID,
			E.exchangeCode as vcExchangeCode
			  from ContractView c  
					join OptionView O on O.optionID = C.contractID
					join ContractView c2 on C2.contractID = O.underlyingContractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where c.contractTypeID=3
					and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
					and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
					and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
					and (@bIsBestPriceOnly = 0 or @bIsBestPriceOnly = 1 and CP.exchangeID is null)
			order by o.underlyingContractID, year(o.expiry), month(o.expiry), o.strike,E.exchangeCode, o.isCall desc
 
  if (@@error <> 0)
	   return (-1)
	 else  
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

CREATE        Procedure dbo.usp_Option_Del
	@iOptionID int = null 	-- contract identifier
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
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete ContractPriceHistory where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
		--delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
		--delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
		--delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	Commit Transaction DelOpt
/*
		update [Option] set actionID=3 where underlyingContractID = @iOptionID
--		delete [Option] where underlyingContractID = @iOptionID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where optionID = @iOptionID
--		delete [Option] where optionID = @iOptionID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select optionID from #OptionGarbage)
--		delete Contract where contractID in (select optionID from #OptionGarbage)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where ContractID = @iOptionID
--		delete Contract where ContractID = @iOptionID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
*/
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

CREATE   proc usp_Option_Get
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
				O.actionID as tiActionID
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
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
				O.actionID as tiActionID
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
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
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID
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
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID
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

CREATE	proc usp_Option_Save
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
			update OptionView
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

Create  Procedure dbo.usp_Option_expDate_Sync
	@iUnderlyingID int = null,	-- underlying identifier
	@dtExpDate datetime = null	-- expiration
as
/*
	Created by Andrey Sinkin
	this procedure synchronizes expirations.
*/
	--------------------------------------------------
	set nocount on
	--------------------------------------------------
	declare @error int		set @error = -1
	if @iUnderlyingID is not null and @dtExpDate is not null
		update OptionView set expiry = maxExpDate
			from OptionView 
			inner join (select underlyingContractID, max(expiry)
					from OptionView
					where OptionView.underlyingContractID = @iUnderlyingID and year(expiry) = year(@dtExpDate) and month(expiry) = month(@dtExpDate)
					group by underlyingContractID, year(expiry), month(expiry)) MaxExpDateData (underlyingID, maxExpDate) on OptionView.underlyingContractID = MaxExpDateData.underlyingID and year(expiry) = year(maxExpDate) and month(expiry) = month(maxExpDate)
			where OptionView.underlyingContractID = @iUnderlyingID and year(expiry) = year(@dtExpDate) and month(expiry) = month(@dtExpDate) and expiry <> maxExpDate
	else
	if @iUnderlyingID is not null and @dtExpDate is null
		update OptionView set expiry = maxExpDate
			from OptionView 
			inner join (select underlyingContractID, max(expiry)
					from OptionView
					where OptionView.underlyingContractID = @iUnderlyingID
					group by underlyingContractID, year(expiry), month(expiry)) MaxExpDateData (underlyingID, maxExpDate) on OptionView.underlyingContractID = MaxExpDateData.underlyingID and year(expiry) = year(maxExpDate) and month(expiry) = month(maxExpDate)
			where OptionView.underlyingContractID = @iUnderlyingID and expiry <> maxExpDate
	else
	if @iUnderlyingID is null and @dtExpDate is not null
		update OptionView set expiry = maxExpDate
			from OptionView 
			inner join (select underlyingContractID, max(expiry)
					from OptionView
					where year(expiry) = year(@dtExpDate) and month(expiry) = month(@dtExpDate)
					group by underlyingContractID, year(expiry), month(expiry)) MaxExpDateData (underlyingContractID, maxExpDate) on OptionView.underlyingContractID = MaxExpDateData.underlyingContractID and year(expiry) = year(maxExpDate) and month(expiry) = month(maxExpDate)
			where year(expiry) = year(@dtExpDate) and month(expiry) = month(@dtExpDate) and expiry <> maxExpDate
	else
		update OptionView set expiry = maxExpDate
			from OptionView 
			inner join (select underlyingContractID, max(expiry)
					from OptionView
					group by underlyingContractID, year(expiry), month(expiry)) MaxExpDateData (underlyingContractID, maxExpDate) on OptionView.underlyingContractID = MaxExpDateData.underlyingContractID and year(expiry) = year(maxExpDate) and month(expiry) = month(maxExpDate)
			where expiry <> maxExpDate
finish:
Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure usp_Position_ByUnderlyingForRisk_Get
        @iContractID int,
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @dtTradeDate datetime = Null,
        @tiIsClose tinyint = Null
        
as
begin
     set nocount on
     select t.contractID, 'S' 'IsStock', SUM(t.Price * t.quantity) 'Total Price', SUM(t.quantity) 'Total quantity'
       from TradeView t
      where t.contractID = @iContractID
        and (@iBookID is Null or t.bookID = @iBookID)
        and (@iStrategyID is Null or t.strategyID = @iStrategyID)
        and (@iTraderID is Null or t.traderID = @iTraderID)
        and (@dtTradeDate is Null or t.TradeDate = @dtTradeDate)
        and (@tiIsClose is Null or t.IsClose = @tiIsClose)
      group by t.contractID
     union all
    select t.contractID, 'O' 'IsStock', SUM(t.Price * t.quantity) 'Total Price', SUM(t.quantity) 'Total quantity'
       from OptionView O, Trade T
      where o.underlyingContractID = @iContractID
        and t.contractID = o.optionID
        and (@iBookID is Null or t.bookID = @iBookID)
        and (@iStrategyID is Null or t.strategyID = @iStrategyID)
        and (@iTraderID is Null or t.traderID = @iTraderID)
        and (@dtTradeDate is Null or t.TradeDate = @dtTradeDate)
        and (@tiIsClose is Null or t.IsClose = @tiIsClose)
      group by t.contractID
    set nocount off
end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE  PROCEDURE dbo.usp_Position_Get
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
		--cp.priceBid as fPriceBid,
		--cp.priceAsk as fPriceAsk,
		--cp.lotSize as iLotSize,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		--d.divDate as dtDivDate,
		--d.divAmt as fDivAmt,
	
		-- index
		i.yield as fYield,
	
		-- option
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		--cp.priceBid as fUnderlyingPriceBid,
		--cp.priceAsk as fUnderlyingPriceAsk,
		--cp.lotSize as iUnderlyingLotSize,
		
		-- trade
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
		--sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then isnull(cp.priceClose, 0) else tr.price end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPositionMtmDaily,
		--null as fPositionTheoDaily
	
	from TradeView tr 
		left join ContractView c  on tr.contractID = c.contractID
		--left join ContractPrice cp on c.contractID = cp.contractID
		left join StockView s on c.contractID = s.stockID
		--left join (select Dividend.stockID, Dividend.DivDate, DivAmt from Dividend 
		--			inner join (select stockID, max(isnull(DivDate, '19000101')) DivDate
		--			from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
		--		d on s.stockID = d.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		--and cp.exchangeID is null
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	--group by c.contractID, c.contractTypeID, c.Symbol, cp.priceBid, cp.priceAsk, cp.lotSize, s.divFreq, d.divDate, d.divAmt, i.yield
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield/*,
			tr.tradeID,tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,tr.strategyID,
			tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommission,tr.clearingBrokerCommission,tr.tradedIV,tr.spotReference
	having min(tr.actionID) = 1*/
	
	union
	
	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		--cp.priceBid as fPriceBid,
		--cp.priceAsk as fPriceAsk,
		--cp.lotSize as iLotSize,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		--d.divDate as dtDivDate,
		--d.divAmt as fDivAmt,
	
		-- index
		i.yield as fYield,
	
		-- option
		o.expiry as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		--ucp.priceBid as fUnderlyingPriceBid,
		--ucp.priceAsk as fUnderlyingPriceAsk,
		--ucp.lotSize as iUnderlyingLotSize,
		
		-- trade
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when tr.tradeDate < @dtToday then isnull(cph.priceTheo, 0) else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cph.priceTheo as fPriceTheoClose
		--sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then isnull(cp.priceClose, 0) else tr.price end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPositionMtmDaily,
		--sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then isnull(cph.priceTheo, 0) else tr.price end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPositionTheoDaily
	
	from TradeView tr 
		left join ContractView c  on tr.contractID = c.contractID
		left join OptionView o on c.contractID = o.optionID
		left join Contract uc on o.underlyingContractID = uc.contractID
		--left join ContractPrice cp on c.contractID = cp.contractID
		--left join ContractPrice ucp on uc.contractID = ucp.contractID
		left join StockView s on uc.contractID = s.stockID
		--left join (select Dividend.stockID, Dividend.DivDate, DivAmt from Dividend 
		--			inner join (select stockID, max(isnull(DivDate, '19000101')) DivDate
		--			from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
		--		d on s.stockID = d.stockID
		left join IndexView i on uc.contractID = i.indexID
		--left join ContractPriceHistory cph on c.contractID = cph.contractID and datediff(d, tr.tradeDate, cph.priceDate) = 0
		left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
					from ContractPriceHistory
					inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
						on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
					cph on c.contractID = cph.contractID
	where c.contractTypeID = 3
		--and cp.exchangeID is null
		--and ucp.exchangeID is null
		--and cph.exchangeID is null
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		o.expiry, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cph.priceTheo/*,
		tr.tradeID,tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,tr.strategyID,
		tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommission,tr.clearingBrokerCommission,tr.tradedIV,tr.spotReference
	having min(tr.actionID) = 1*/
	--group by c.contractID, c.contractTypeID, c.Symbol, cp.priceBid, cp.priceAsk, cp.lotSize, s.divFreq, d.divDate, d.divAmt, i.yield,
	--	o.expiry, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, ucp.priceBid, ucp.priceAsk, ucp.lotSize, cph.priceTheo
	
	order by vcSymbol
/*	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		--cp.priceBid as fPriceBid,
		--cp.priceAsk as fPriceAsk,
		--cp.lotSize as iLotSize,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		--d.divDate as dtDivDate,
		--d.divAmt as fDivAmt,
	
		-- index
		i.yield as fYield,
	
		-- option
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		--cp.priceBid as fUnderlyingPriceBid,
		--cp.priceAsk as fUnderlyingPriceAsk,
		--cp.lotSize as iUnderlyingLotSize,
		
		-- trade
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then 1 else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when datediff(d, getdate(), tr.tradeDate) >= 0 then tr.price else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
		--sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then isnull(cp.priceClose, 0) else tr.price end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPositionMtmDaily,
		--null as fPositionTheoDaily
	
	from TradeView tr 
		left join ContractView c  on tr.contractID = c.contractID
		--left join ContractPrice cp on c.contractID = cp.contractID
		left join StockView s on c.contractID = s.stockID
		--left join (select Dividend.stockID, Dividend.DivDate, DivAmt from Dividend 
		--			inner join (select stockID, max(isnull(DivDate, '19000101')) DivDate
		--			from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
		--		d on s.stockID = d.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		--and cp.exchangeID is null
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	--group by c.contractID, c.contractTypeID, c.Symbol, cp.priceBid, cp.priceAsk, cp.lotSize, s.divFreq, d.divDate, d.divAmt, i.yield
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield
	
	union
	
	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		--cp.priceBid as fPriceBid,
		--cp.priceAsk as fPriceAsk,
		--cp.lotSize as iLotSize,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		--d.divDate as dtDivDate,
		--d.divAmt as fDivAmt,
	
		-- index
		i.yield as fYield,
	
		-- option
		o.expiry as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		--ucp.priceBid as fUnderlyingPriceBid,
		--ucp.priceAsk as fUnderlyingPriceAsk,
		--ucp.lotSize as iUnderlyingLotSize,
		
		-- trade
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then 1 else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when datediff(d, getdate(), tr.tradeDate) >= 0 then tr.price else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then isnull(cph.priceTheo, 0) else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cph.priceTheo as fPriceTheoClose
		--sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then isnull(cp.priceClose, 0) else tr.price end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPositionMtmDaily,
		--sum((case when datediff(d, getdate(), tr.tradeDate) < 0 then isnull(cph.priceTheo, 0) else tr.price end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPositionTheoDaily
	
	from TradeView tr 
		left join ContractView c  on tr.contractID = c.contractID
		left join OptionView o on c.contractID = o.optionID
		left join Contract uc on o.underlyingContractID = uc.contractID
		--left join ContractPrice cp on c.contractID = cp.contractID
		left join ContractPrice ucp on uc.contractID = ucp.contractID
		left join StockView s on uc.contractID = s.stockID
		--left join (select Dividend.stockID, Dividend.DivDate, DivAmt from Dividend 
		--			inner join (select stockID, max(isnull(DivDate, '19000101')) DivDate
		--			from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
		--		d on s.stockID = d.stockID
		left join IndexView i on uc.contractID = i.indexID
		--left join ContractPriceHistory cph on c.contractID = cph.contractID and datediff(d, tr.tradeDate, cph.priceDate) = 0
		left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
					from ContractPriceHistory
					inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
						on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
					cph on c.contractID = cph.contractID
	where c.contractTypeID = 3
		--and cp.exchangeID is null
		and ucp.exchangeID is null
		--and cph.exchangeID is null
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		o.expiry, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cph.priceTheo
	--group by c.contractID, c.contractTypeID, c.Symbol, cp.priceBid, cp.priceAsk, cp.lotSize, s.divFreq, d.divDate, d.divAmt, i.yield,
	--	o.expiry, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, ucp.priceBid, ucp.priceAsk, ucp.lotSize, cph.priceTheo
	
	order by vcSymbol
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

CREATE  Procedure dbo.usp_PriceCloseDH_Get
	@vcSymbol varchar(8),	--ticker of stock  
	@fPriceClose float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice table for DeltaHedger
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select @fPriceClose = priceClose
		from ContractPrice	
			join ContractView Contract on Contract.contractID=ContractPrice.contractID
		where contractTypeID in (1,2) 
			and exchangeID is null
			and Contract.symbol=@vcSymbol
	set @error = @@error
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

CREATE   Procedure dbo.usp_PriceCloseDH_Save 
	@vcSymbol varchar(8),	--ticker of stock(index)  
	@fPriceClose float
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice table for DeltaHedger
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iSymbolID int
	select @iSymbolID=contractID from Contract where symbol=@vcSymbol and ContractTypeID < 3
	Begin Transaction
		if not exists(select * From ContractPrice where contractID=@iSymbolID and exchangeID is null) 
			begin
				insert into ContractPrice (contractID,exchangeID,priceClose) 
				values (@iSymbolID,null,@fPriceClose)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update ContractPrice set 
					priceClose = @fPriceClose
				where contractID=@iSymbolID and exchangeID is null
	
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

CREATE  Procedure dbo.usp_RMScenario_Del
	@iRMScenarioID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	update RMScenario set actionID=0 where RMScenarioID = @iRMScenarioID
	--delete RMScenario Where RMScenarioID = @iRMScenarioID
	set @error = @@error
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

CREATE     Procedure dbo.usp_RMScenario_Get
	@iRMScenarioID int = null,
	@dtActionDate datetime = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    select rs.RMScenarioID as iRMScenarioID, 
			rs.ScenarioName as vcScenarioName, 
			rs.[Description] as vcDescription,
            points1 as iPoints1,
			step1 as fStep1,
			isAbs1 as tiIsAbs1,
			type1 as tiType1,
			points2 as iPoints2,
			step2 as fStep2,
			isAbs2 as tiIsAbs2,
			type2 as tiType2,
			actionDate as dtActionDate,
			actionID as tiActionID
		from RMScenario rs
		where ((@iRMScenarioID is Null) 
			or (@iRMScenarioID is Not Null and rs.RMScenarioID = @iRMScenarioID))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
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

CREATE     Procedure dbo.usp_RMScenario_Save
	@iRMScenarioID int = Null output,
	@vcScenarioName varchar(30) = Null,
	@vcDescription varchar(120) = Null,
    @iPoints1 int = Null,
	@fStep1 int = Null,
	@tiIsAbs1 tinyint = Null,
	@tiType1 tinyint = Null,
	@iPoints2 int = Null,
	@fStep2 float= Null,
	@tiIsAbs2 tinyint = Null,
	@tiType2 tinyint = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from RMScenario where scenarioName = @vcScenarioName and actionID < 3 and (@iRMScenarioID <> RMScenarioID or @iRMScenarioID is null))
		begin
			Raiserror ('Row in RMScenario table with same scenario name already exists!', 16, 1)
			Return (-1)
		end   
	if @iRMScenarioID is null
	--if not exists(select * from RMScenario where RMScenarioID = @iRMScenarioID)
		begin
			insert into RMScenario(
					ScenarioName,
					[Description],
					points1,
					step1,
					isAbs1,
					type1,
					points2,
					step2,
					isAbs2,
					type2) 
				values (
					@vcScenarioName,
					@vcDescription,
				    @iPoints1,
					@fStep1,
					@tiIsAbs1,
					@tiType1,
					@iPoints2,
					@fStep2,
					@tiIsAbs2,
					@tiType2)
			select @iRMScenarioID = @@identity
		end
	else
		begin
			update RMScenario set
				ScenarioName = isNull(@vcScenarioName, ScenarioName),
				[Description] = isNull(@vcDescription, [Description]),
				points1 = isNull(@iPoints1, points1),
				step1 = isNull(@fStep1, step1),
				isAbs1 = isNull(@tiIsAbs1, isAbs1),
				type1 = isNull(@tiType1, type1),
				points2 = isNull(@iPoints2, points2),
				step2 = isNull(@fStep2, step2),
				isAbs2 = isNull(@tiIsAbs2, isAbs2),
				type2 = isNull(@tiType2, type2)
				where RMScenarioID = @iRMScenarioID
		end
	set @error = @@error
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

CREATE              Procedure dbo.usp_ReportMarketStructure_Get
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
	create table #FilteredStocks(stockID int)
	insert into #FilteredStocks
	       exec usp_FilterData_Get @iFilterID
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		O.expiry as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join ContractPrice CP on 
						CP.contractID=O.optionID and 
						CP.contractID=Contract.contractID 
						and CP.exchangeID is null
		where 	O.expiry > GetDate() and
	(
		(@iStockID is not null and @iFilterID is null and underlyingContractID=@iStockID) or 
		(@iStockID is null and @iFilterID is not null and underlyingContractID in (Select stockID from #FilteredStocks)) or 
		(@iStockID is not null  and @iFilterID is not null and (underlyingContractID=@iStockID and O.underlyingContractID in (Select stockID from #FilteredStocks))) or 
		(@iStockID is null and @iFilterID is null)
	)
	drop table #FilteredStocks
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
		case T.isBuy
			when 0 then T.quantity * -1
			else T.quantity 
		end as iQuantity,
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

CREATE Procedure dbo.usp_ReportStock_Get
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
			Stock.divfreq as iDivFreq,
			Stock.divAmt as fDivAmt,
	  		Stock.DivDate as dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		Stock.isHTB as bIsHTB
	
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join StockView Stock on Contract.contractID = Stock.StockID
					left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory join
									(select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
								on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
						on S2.stockID=Stock.stockID
	
			where (
				(@iStockID is null and (Stock.stockID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and (Contract.ContractID=@iStockID and (Stock.stockID in (select stockID from #FilteredStocks))))
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
	  		I.isHTB as bIsHTB	
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

CREATE           Procedure dbo.usp_ReportTrade_Get
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
		case T.isBuy
			when 0 then T.quantity * -1
			else T.quantity 
		end as iQuantity,
		T.tradeDate as dtTradeDate,
		T.price as fPriceTrade,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceLast as fPriceLast,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.lotSize as iLotSize,
		cph.priceTheo as fPriceTheo,
		T.traderID as iTraderID,
		T.bookID as iBookID,
		T.strategyID as iStrategyID,
		CP.priceClose as fPriceClose,
		Contract.symbol as vcSymbol
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join TradeView T on O.optionID = T.contractID 
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
				left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
							from ContractPriceHistory
							inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
								on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
							cph on contract.contractID = cph.contractID
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

CREATE Procedure dbo.usp_RuleItemType_Del 
	@iRuleItemTypeID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from RuleItemType table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
		delete RuleItemType	where ruleItemTypeID = @iRuleItemTypeID 
		
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
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

CREATE Procedure dbo.usp_RuleItemType_Get 
	@iRuleItemTypeID int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from RuleItemType table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select ruleItemTypeID as iRuleItemTypeID,
			itemTypeName as vcItemTypeName
		from RuleItemType	
		where (ruleItemTypeID=@iRuleItemTypeID or @iRuleItemTypeID is null)
	set @error = @@error
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

CREATE Procedure dbo.usp_RuleItemType_Save 
	@iRuleItemTypeID int = Null, --the row ID
	@vcItemTypeName varchar(255)
AS
/*
	Created by ...
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		if not exists(select * From RuleItemType where ruleItemTypeID=@iRuleItemTypeID) 
			begin
				insert into RuleItemType (itemTypeName) values (@vcItemTypeName)
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iRuleItemTypeID=@@identity
			end
		else
			begin
				update RuleItemType set 
					itemTypeName = isnull(@vcItemTypeName, itemTypeName)
				where ruleItemTypeID = @iRuleItemTypeID
	
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

CREATE    Procedure dbo.usp_StockCorrelationDH_Get 
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null Output,
	@fCorrelationVola  float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from StockCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iStockID1 int, 
			@iStockID2 int,
			@iStockID3 int
	select @iStockID1=contractID from ContractView where symbol=@vcSymbol1 and contractTypeID=2
	select @iStockID2=contractID from ContractView where symbol=@vcSymbol2 and contractTypeID=2
	if 	@iStockID1 > @iStockID2
		select @iStockID3=@iStockID1,@iStockID1=@iStockID2,@iStockID2=@iStockID3
	select	@fCorrelationPrice=correlationPrice,
			@fCorrelationVola=correlationVola
		from StockCorrelation	
		where stockID1 = @iStockID1
			and stockID2 = @iStockID2
	set @error = @@error
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

CREATE   Procedure dbo.usp_StockCorrelationDH_Save
	@vcSymbol1 varchar(8), 
	@vcSymbol2 varchar(8),
	@fCorrelationPrice float = Null,
	@fCorrelationVola  float = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from StockCorrelation table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iStockID1 int, 
			@iStockID2 int,
			@iStockID3 int
	select @iStockID1=contractID from Contract where symbol=@vcSymbol1 and contractTypeID=2
	select @iStockID2=contractID from Contract where symbol=@vcSymbol2 and contractTypeID=2
	if @iStockID1>@iStockID2
		select @iStockID3=@iStockID1,@iStockID1=@iStockID2,@iStockID2=@iStockID3
	Begin Transaction
		if not exists(select * from StockCorrelation where StockID1=@iStockID1 and StockID2=@iStockID2) 
			begin
				insert into StockCorrelation(stockID1,stockID2,correlationPrice,correlationVola)
					values(@iStockID1,@iStockID2,@fCorrelationPrice,@fCorrelationVola)
			end 
		else
			begin
				update StockCorrelation
					set stockID1=@iStockID1,
						stockID2=@iStockID2,
						correlationPrice=isnull(@fCorrelationPrice,CorrelationPrice),
						correlationVola=isnull(@fCorrelationVola,CorrelationVola)
				where StockID1=@iStockID1 and StockID2=@iStockID2	
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   Procedure dbo.usp_StockDataHistoryDH_Get
	@vcSymbol varchar(8),	--ticker of stock 
	@fCurrentIV float = null Output, 
	@fCurrentHV float = null Output, 
	@fCurrentVolOfVol float = null Output 
as
/*
	Created by Zaur Nuraliev
	this procedure returns historical data for stock by ticker
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iStockID int 
	select @iStockID=contractID from ContractView where symbol=@vcSymbol and contractTypeID = 2
	select 	@fCurrentIV = IVMean, 
			@fCurrentHV = currentHV,
			@fCurrentVolOfVol = currentVolOfVol
		from StockDataHistory
		where tenorID=8 
			and [date]=(select max([date]) from StockDataHistory where stockID=@iStockID and tenorID=8)
			and stockID=@iStockID
	set @error = @@error
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

CREATE    Procedure dbo.usp_StockDataHistory_Save
	@iStockID int = null,
	@dtDate datetime = null,
	@iTenorID int = null,
	@fIVMean float = null,
	@vcStockTicker varchar(12) = null,
	@iTenorValue int = null,
	@fIVCall float = null,
	@fIVPut float = null,
	@fCurrentHV float = null,
	@fCurrentVolOfVol float = null
as
/*
	Created by Andrey Sinkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	-- if wrong parameters -> exit
	if @vcStockTicker is null and @iStockID is null or @iTenorID is null and @iTenorValue is null	goto finish
	if @iStockID is null	
		select @iStockID = contractID from Contract where symbol = @vcStockTicker and contractTypeID = 2
	if @iTenorID is null	
		select @iTenorID = tenorID from Tenor where value = @iTenorValue
	if @dtDate is null	
		set @dtDate = GetDate()
	Begin Transaction
		if not exists(select * from StockDataHistory where StockID = @iStockID and date = @dtDate and tenorID = @iTenorID)
			begin
				insert into StockDataHistory (StockID, [date], tenorID, IVMean, IVCall, IVPut, currentHV,currentVolOfVol) values (@iStockID, @dtDate, @iTenorID, @fIVMean, @fIVCall, @fIVPut, @fCurrentHV,@fCurrentVolOfVol)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update StockDataHistory set 
					IVMean = isnull(@fIVMean, IVMean),
					IVCall = isnull(@fIVCall, IVCall),
					IVPut = isnull(@fIVPut, IVPut),
					currentHV = isnull(@fCurrentHV, currentHV),
					currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol)
				where StockID = @iStockID and [date] = @dtDate and tenorID = @iTenorID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		set @error = 0
	Commit Transaction
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

CREATE procedure usp_StockID_Get
	   @vcSymbol varchar(8) 
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting stock identifiers
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select C.contractID as iStockID
		from ContractView c 	
				join StockView S on S.stockID = C.contractID
		where C.contractTypeID = 2 and C.symbol = @vcSymbol
	set @error = @@error
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

CREATE     Procedure usp_StockReportDH_Get
	@vcSymbolList varchar(8000) = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from <table_name, sysname,table_name> table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Declare @iSPX_ID int,
			@iNDX_ID int,
			@iOEX_ID int,
			@iDJX_ID int
	select @iSPX_ID=ContractID from Contract where symbol='SPX' and contractTypeID=1
	select @iNDX_ID=ContractID from Contract where symbol='NDX' and contractTypeID=1
	select @iOEX_ID=ContractID from Contract where symbol='OEX' and contractTypeID=1
	select @iDJX_ID=ContractID from Contract where symbol='DJX' and contractTypeID=1
	declare @SDH_Dt datetime
		select @SDH_Dt=max([date]) from StockDataHistory where tenorID=8
	-- check if data not exists in StockDataHistory
	select  @SDH_Dt= isnull(@SDH_Dt, 0)
	select @iSPX_ID = isnull(@iSPX_ID, 0)
	select @iNDX_ID = isnull(@iNDX_ID, 0)
	select @iOEX_ID = isnull(@iOEX_ID, 0)
	select @iDJX_ID = isnull(@iDJX_ID, 0)
if @vcSymbolList is null 
begin
	select
		Contract.Symbol,
		CP.priceClose,
		SDH.currentHV as HV,
		SDH.IVMean as IV,
		sum(case when IndexD.indexID=@iSPX_ID then correlationPrice else null end) as CorrSPX,
		sum(case when IndexD.indexID=@iSPX_ID then correlationVola else null end) as VCorrSPX,
		sum(case when IndexD.indexID=@iNDX_ID then correlationPrice else null end) as CorrNDX,
		sum(case when IndexD.indexID=@iNDX_ID then correlationVola else null end) as VCorrNDX,
		sum(case when IndexD.indexID=@iOEX_ID then correlationPrice else null end) as CorrOEX,
		sum(case when IndexD.indexID=@iOEX_ID then correlationVola else null end) as VCorrOEX,
		sum(case when IndexD.indexID=@iDJX_ID then correlationPrice else null end) as CorrDJX,
		sum(case when IndexD.indexID=@iDJX_ID then correlationVola else null end) as VCorrDJX
		from ContractView Contract
			left join StockDataHistory SDH on Contract.contractID=SDH.stockID and SDH.[date]=@SDH_Dt and SDH.tenorID=8
			left join IndexDefinition IndexD on Contract.contractID=IndexD.stockID
			left join ContractPrice CP on contract.contractID=CP.contractID and CP.exchangeID is null 
		where contract.contractTypeID=2
		group by Contract.Symbol,CP.priceClose,SDH.currentHV,SDH.IVMean
end 
else 
begin
	declare @ExecString varchar(8000)
	select @ExecString='select
				Contract.Symbol,
				CP.priceClose,
				SDH.currentHV as HV,
				SDH.IVMean as IV,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iSPX_ID) + ' then correlationPrice else null end) as CorrSPX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iSPX_ID) + ' then correlationVola else null end) as VCorrSPX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iNDX_ID) + ' then correlationPrice else null end) as CorrNDX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iNDX_ID) + ' then correlationVola else null end) as VCorrNDX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iOEX_ID) + ' then correlationPrice else null end) as CorrOEX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iOEX_ID) + ' then correlationVola else null end) as VCorrOEX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iDJX_ID) + ' then correlationPrice else null end) as CorrDJX,
				sum(case when IndexD.indexID=' + convert(varchar(15),@iDJX_ID) + ' then correlationVola else null end) as VCorrDJX '
				+'from ContractView Contract
							left join StockDataHistory SDH on Contract.contractID=SDH.stockID and convert(varchar(15),SDH.[date],112)=' + convert(varchar(15),@SDH_Dt,112) + ' and SDH.tenorID=8 
							left join IndexDefinition IndexD on Contract.contractID=IndexD.stockID 
							left join ContractPrice CP on contract.contractID=CP.contractID and CP.exchangeID is null  
							where Symbol in ('	+ @vcSymbolList + ') and contract.contractTypeID=2' 
				+ ' group by Contract.Symbol,CP.priceClose,SDH.currentHV,SDH.IVMean'
	exec (@ExecString)
end
	set @error = @@error
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

CREATE      Procedure dbo.usp_StockVM_Save
	@iStockID int = null output,		-- stock's identifier
	@iContractTypeID int = null,		-- contract's type identifier (not used)
	@vcSymbolName varchar(12) = null,	-- stock's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot size (not used)
	@iDivFrequency int = null,		-- dividend's frequency
	@dtDate datetime = null,		-- date
	@fIVMean float = null,			-- iv mean
	@fCurrentHV float = null,		-- current hv
	@fCurrentIV float = null,		-- current iv
	@fAmount float = null, 			-- amount
	@dDivDate datetime = null,		-- dividend's date
	@iTenorID int = null,			-- tenor's identifier
	@iTenorValue int = null,		-- tenor's value
	@fIVCall float = null,			-- iv call
	@fIVPut float = null,			-- iv put
	@fPriceClose float = null,		-- stock's close price
	@iGroupID int = null,
	@iRuleID int = null
as
/*
	Created by Andrey Sinkin
	Can to save data to Contract, Stock, StockDataHistory, StockDatasnapshot tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iDividendID int, @iMarketDataID int
	set @iContractTypeID = 2	-- stock
	set @iLotSize = 0
	-- if wrong parameters -> exit
	if @vcSymbolName is null and @iStockID is null	goto finish
	if @iTenorID is null and @iTenorValue is not null
		select @iTenorID = tenorID from Tenor where value = @iTenorValue
	if @iStockID is null and @vcSymbolName is not null
		select @iStockID = contractID from ContractView where symbol = @vcSymbolName and contractTypeID = 2	-- stock
	Begin Transaction
		if @iStockID is null
			begin
				insert into Contract (contractTypeID, symbol,contractName,groupID,ruleID) 
					values (@iContractTypeID, @vcSymbolName,@vcDescription, @iGroupID,@iRuleID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iStockID = @@identity
				insert into Stock (StockID, divFreq)
					values (@iStockID, @iDivFrequency)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				if @iContractTypeID is not null or @vcSymbolName is not null or @iExchangeID is not null or @vcDescription is not null or @iLotSize is not null
					begin
						update Contract set 
							contractTypeID = isnull(@iContractTypeID, contractTypeID),
							symbol = isnull(@vcSymbolName, symbol),
							contractName = isnull(@vcDescription, contractName),
							groupID = isnull(@iGroupID, groupID),
							ruleID = isnull(@iRuleID, ruleID)
						where contractID = @iStockID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
					end
				
				if @iDivFrequency is not null
					begin
						update Stock set 
							divFreq = isnull(@iDivFrequency, divFreq)
						where StockID = @iStockID
						if @@error <> 0 begin set @error = 4 if @@TranCount = 1 Rollback Transaction goto finish end
					end
			end
		--============================================================================================================
		if exists(select * from ContractPrice where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null))
			update ContractPrice 
				set exchangeID = @iExchangeID,
					lotSize = isnull(@iLotSize, lotSize),
					contractID = isnull(@iStockID, contractID)
				where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null)
		else
			insert into ContractPrice
				(exchangeID,contractID,lotSize)
			values (@iExchangeID,@iLotSize,@iStockID)
		--============================================================================================================
		if @dtDate is not null and @fIVMean is not null and @iTenorID is not null
			begin
				exec @error = usp_StockDataHistory_Save @iStockID = @iStockID, @dtDate = @dtDate, @fIVMean = @fIVMean, @iTenorID = @iTenorID, @fIVCall = @fIVCall, @fIVPut = @fIVPut, @fCurrentHV = @fCurrentHV
				if @error <> 0 begin set @error = 5 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		if (@fCurrentHV is not null or @fCurrentIV is not null) and @iTenorID is not null
			begin
			   Raiserror ('Error. Can''t save CurrentIV for Stock (usp_StockDataSnapshot_Save stored procedure), please send this error message to DB developer !', 16, 1)
/*				exec @error = usp_StockDataSnapshot_Save @iStockID = @iStockID, @fCurrentHV = @fCurrentHV, @fCurrentIV = @fCurrentIV, @iTenorID = @iTenorID, @dtDateUpdate = @dtDate
					if @error <> 0 begin set @error = 6 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
		if @fAmount is not null and @dDivDate is not null
			begin
				exec @error = usp_Dividend_Save 
					@iDividendID = @iDividendID output, 
					@iStockID = @iStockID, 
					@fDivAmt = @fAmount, 
					@dtDivDate = @dDivDate
				if @error <> 0 begin set @error = 7 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		if @fPriceClose is not null
			begin
			   Raiserror ('Error. Can''t save close price for Stock (usp_StockVM_Save stored procedure), please send this error message to DB developer !', 16, 1)
/*				exec usp_MarketData_Save 
					@iMarketDataID = @iMarketDataID output,	-- market data identifier
					@iContractID = @iStockID,		-- contract's identifier
					@dDate = @dtDate,			-- date
					@fPriceClose = @fPriceClose		-- close price 
				if @error <> 0 begin set @error = 8 if @@TranCount = 1 Rollback Transaction goto finish end */
			end
		set @error = 0
	Commit Transaction
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

CREATE    proc usp_StockWithPrice_Get
	@iGroupID int = Null,
	@iStockID int = Null,
	@bIsBestPriceOnly bit = 0
  as
	if (@iGroupID is Null)
		if @bIsBestPriceOnly=0
			select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
					S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
				    CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
					E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
					  --left join (select Dividend.stockID, Dividend.DivDate, DivAmt
					  --       from Dividend 
					  --       inner join (select stockID, max(isnull(DivDate, getdate()-getdate())) DivDate
					  --         from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
					  --      Dividend on S.stockID = Dividend.stockID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				   order by c.Symbol,E.ExchangeCode
		else
				select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
					S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
					CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
					E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
					  --left join (select Dividend.stockID, Dividend.DivDate, DivAmt
					  --       from Dividend 
					  --       inner join (select stockID, max(isnull(DivDate, getdate()-getdate())) DivDate
					  --         from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
					  --      Dividend on S.stockID = Dividend.stockID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and CP.exchangeID is null
				   order by c.Symbol,E.ExchangeCode
	else
		select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
				S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
				S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
			    CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
			    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
				E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
			    from ContractInGroupView cig
					join ContractView c  on C.contractID = CIG.contractID
					join StockView S on S.StockID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
					  --left join (select Dividend.stockID, Dividend.DivDate, DivAmt
					  --       from Dividend 
					  --       inner join (select stockID, max(isnull(DivDate, getdate()-getdate())) DivDate
					  --         from Dividend group by stockID) DivLast on Dividend.stockID = DivLast.stockID and Dividend.DivDate = DivLast.DivDate)
					  --      Dividend on S.stockID = Dividend.stockID
		        where CIG.groupID = @iGroupID
		          and c.contractTypeID = 2
			   order by c.Symbol,E.ExchangeCode
if (@@error <> 0)
	Return (-1)
else  
   Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE      proc usp_Stock_Del
        @iStockID int
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from TradeView(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Error. Can''t delete Stock because of the existing Trade', 16, 1)
		Return (-1)
	end 
	if exists(select * from [Option](nolock) where underlyingContractID=@iStockID)
		begin
			Create table #Options_for_delete(OptionID int)
		
			insert into #Options_for_delete
				select optionID from [Option](nolock) where underlyingContractID=@iStockID
		
			if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
				begin
					Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
					Return (-1)
				end 
		end
	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del
			end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	    delete ModelParameter where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
	    delete Dividend where stockID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
 
        update ContractInGroup set actionID=3 where contractID = @iStockID 
        --delete ContractInGroup where contractID = @iStockID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update IndexDefinition set actionID=3 where stockID = @iStockID  
        --delete IndexDefinition where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update TraderContract set actionID=3 where contractID = @iStockID  
		--delete TraderContract where contractID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Stock set actionID=3 where stockID = @iStockID  
        --delete Stock where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPriceHistory where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPrice where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Contract set actionID=3 where contractID = @iStockID
        --delete Contract where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
  
		set @error = @@error
	Commit Transaction
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

CREATE         proc usp_Stock_Get
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
						S.actionID as tiActionID
				    from ContractView c  
						join Stock S on S.stockID = C.contractID
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
						S.actionID as tiActionID
				    from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
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
						sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				    from ContractView c  
						join Stock S on S.stockID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID
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
						sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				    from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID
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

CREATE     proc usp_Stock_Import
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
		where contractID = @iContractID and exchangeID=@iExchangeID
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
					set divAmt=@fDivAmt,divDate=@dtDivDate 
				where stockID=@iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
	
				if not exists(select * from Dividend where stockID=@iContractID and divDate=@dtDivDate)
					insert into Dividend (stockID, DivAmt, DivDate)
						values (@iContractID, @fDivAmt, @dtDivDate)
					if (@@error <> 0)
						begin
							Raiserror ('Error. Can''t insert row into Dividend table!', 16, 1)
							set @error = -1 
							if @@TranCount = 1 Rollback Transaction goto finish 
						end
				else
					update Dividend
						set DivAmt = isnull(@fDivAmt,DivAmt)
						where stockID=@iContractID and DivDate=@dtDivDate
					if (@@error <> 0)
						begin
							Raiserror ('Error. Can''t update row in Dividend table!', 16, 1)
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_Strategy_Del
        @iStrategyID int
  as
	update Strategy set actionID=3 where strategyID = @iStrategyID
--    delete Strategy where strategyID = @iStrategyID
        if (@@error <> 0)
           return (-1)
         else  
           return 0

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE      proc usp_Strategy_Get
	 @iStrategyID int = Null,
	 @bInTrade bit = 0,
	 @bShortFormat bit = 0,
	@dtActionDate datetime = null
  as
/*
	Edited by Zaur Nuraliev
	Description: this procedure is getting information from Strategy table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
     if (@iStrategyID is Null)
        if (@bInTrade = 0 and @bShortFormat = 0) 
           select 	strategyID as iStrategyID, 
					strategyName as vcStrategyName, 
					[description] as vcDescription,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Strategy
				where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				order by StrategyName
		else if (@bInTrade = 0 and @bShortFormat = 1) 
           select 	strategyID as iStrategyID, 
					strategyName as vcStrategyName,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Strategy 
				where (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
				order by StrategyName
        else
           select strategyID as iStrategyID, 
					strategyName as vcStrategyName, 
					[description] as vcDescription,
					actionDate as dtActionDate,
					actionID as tiActionID
				from Strategy
	            where exists (select tr.strategyID from TradeView tr  where tr.strategyID = Strategy.strategyID)
					and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
    	        order by StrategyName
	else 
        select	strategyID as iStrategyID, 
				strategyName as vcStrategyName, 
				[description] as vcDescription,
				actionDate as dtActionDate,
				actionID as tiActionID
			from Strategy 
			where strategyID = @iStrategyID
				and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
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

CREATE    proc usp_Strategy_Save
        @iStrategyID int,
        @vcStrategyName varchar(12),
        @vcDescription varchar(120)
  as
   
	if exists (select * from Strategy where @vcStrategyName = strategyName and actionID < 3 and (@iStrategyID <> strategyID or @iStrategyID is null))
		begin
			Raiserror ('Row in Strategy table with same strategy name already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStrategyID is Null)  
     begin
         insert into Strategy (StrategyName, [Description])
                values (@vcStrategyName, @vcDescription)
         if (@@error <> 0)
            return (-1)
          else  
            return @@identity
     end
   else 
     begin
        update Strategy 
             set  StrategyName = @vcStrategyName,
                    [Description] = @vcDescription
          where strategyID = @iStrategyID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
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

CREATE    proc usp_Symbol_Get
      @iGroupID int,
      @bIsAll bit,
      @bStockOnly bit,   
      @bIndexesOnly bit,
      @bOptionOnly bit 
  as
     if (@iGroupID is Null)
        if (@bIsAll is not Null)  and   (@bIsAll > 0)
           select	C.contractTypeID, C.Symbol
	            from ContractView C 
						join StockView S on S.StockID = C.contractID
	            where ((@bStockOnly is not Null) and (@bStockOnly > 0))
	               and c.contractTypeID = 2
          union
    	     select C.contractTypeID, C.Symbol
        	     from ContractView C 
						join Indexes I on I.Index_ID = C.contractID
	        	 where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
		    	         and c.contractTypeID = 1
          union 
	         select C.contractTypeID, C.Symbol
    	         from ContractView C
						join OptionView O on O.optionID = C.contractID
        		  where ((@bOptionOnly is not Null) and (@bOptionOnly > 0)) 
				          and c.contractTypeID in (3, 4)
	         order by c.Symbol   
       else
			select	C.contractTypeID, C.Symbol
           		from ContractView C 
						join StockView S on s.StockID = c.contractID
	            where ((@bStockOnly is not Null) and (@bStockOnly > 0)) 
		               and c.contractTypeID = 2
        		       and not exists (select * from ContractViewInGroups where contractID = c.contractID)
          union
	         select C.contractTypeID, C.Symbol
	            from ContractView C 
							join Indexes I on I.Index_ID = C.contractID
	    			where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
			             and C.contractTypeID = 1
	            		 and not exists (select * from ContractViewInGroups where contractID = c.contractID)
			         order by C.Symbol 
      else
         select C.contractTypeID, C.Symbol
			from ContractViewInGroups CIG
					join ContractView C on C.contractID = CIG.contractID
					join StockView S on S.StockID = C.contractID
	            where CIG.Group_ID = @iGroupID
            		   and ((@bStockOnly is not Null) and (@bStockOnly > 0))  
		               and c.contractTypeID = 2
         union
	         select C.contractTypeID, C.Symbol
	             from ContractViewInGroups CIG
						join ContractView C on C.contractID = CIG.contractID
						join Indexes I on I.Index_ID = C.contractID
		         where CIG.Group_ID = @iGroupID
			             and ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
			             and c.contractTypeID = 1
		         order by c.Symbol           
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

CREATE  proc usp_TradeBySymbol_Get
        @bIsAllContracts bit
    
  as
      if ((@bIsAllContracts is Null) or (@bIsAllContracts = 1))
		select distinct TR.contractID as iContractID, C.Symbol as vcSymbol, C.ContractName as vcContractName, C.contractTypeID as iContractTypeID,
			case when (O.Strike is not Null) then O.Strike
				when (O.Strike is Null) and ((CP.priceBid <> 0) and (CP.priceAsk <> 0)) then (CP.priceBid + CP.priceAsk)/2 
				else (CP.priceBid + CP.priceAsk) end as fPriceValue,
			O.Expiry as dtExpiry
            from TradeView tr , ContractView c ,ContractPrice CP,OptionView O
            where c.contractID = tr.contractID
					and C.contractID=CP.contractID
                   	and c.contractID *= o.OptionID
             order by c.Symbol
           else
              select distinct TR.contractID as iContractID, C.Symbol as vcSymbol, C.ContractName as vcContractName, C.contractTypeID as iContractTypeID, 
				case when ((CP.priceBid <> 0) and (CP.priceAsk <> 0)) then (CP.priceBid + CP.priceAsk)/2 
                     else (CP.priceBid + CP.priceAsk) 
				end as fPriceValue,
			Null as dtExpiry
                from TradeView tr  
						join ContractView c  on TR.contractID=C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
                where C.contractTypeID in (1, 2)
             union
              select distinct C2.contractID as iContractID, C2.Symbol as vcSymbol, C2.ContractName as vcContractName, C2.contractTypeID as iContractTypeID,
			case when ((CP.priceBid <> 0) and (CP.priceAsk <> 0)) then (CP.priceBid + CP.priceAsk)/2 
                  else (CP.priceBid + CP.priceAsk) end as fPriceValue,
			Null as dtExpiry 
                  from TradeView tr 
						join ContractView c1 on C1.contractID = TR.contractID
						join OptionView O on O.optionID = C1.contractID
						join ContractView c2 on C2.contractID = O.UnderlyingcontractID
						left join ContractPrice CP on C2.contractID=CP.contractID
	              where C1.contractTypeID = 3
                order by c.Symbol
           
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

CREATE Procedure usp_TradeForRisk_Get
        @iContractID int,
        @iBookID int,
        @iStrategyID int,
        @iTraderID int,
        @dtTradeDate datetime = Null,
        @tiIsClose tinyint = Null
as
     select t.tradeID as iTradeID,
	t.contractID as iContractID,
	t.isClose as tiIsClose,
	t.tradeDate as dtTradeDate,
	t.quantity as iQuantity,
	t.price as fPrice,
	t.isBuy as bIsBuy,
	t.bookID as iBookID,
	t.strategyID as iStrategyID,
	t.traderID as iTraderID,
	t.brokerID as iBrokerID,
	t.clearingBrokerID as iClearingBrokerID,
	cv1.CommissionValue as fBrokerCommission,
	cv2.CommissionValue as fClearingBrokerCommission
       from ContractView c, TradeView t, CommissionView cv1, CommissionView cv2
      where t.contractID = c.contractID
        and t.clearingBrokerCommissionID = cv2.commissionID
        and t.BrokerCommissionID = cv1.commissionID
		and c.contractID = @iContractID
        and ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
        and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
        and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
        and ((@dtTradeDate is Null) or ((@dtTradeDate is not Null) and (t.TradeDate = @dtTradeDate)))
        and ((@tiIsClose is Null) or ((@tiIsClose is not Null) and (t.IsClose = @tiIsClose)))

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROC usp_TradeSeq_Get
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
			isPosition as tiIsPosition
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
			tr.isPosition as tiIsPosition
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
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
			cph.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition
		from Trade tr inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join ContractView uc on o.underlyingContractID = uc.contractID
			left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
						from ContractPriceHistory
						inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
							on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
						cph on c.contractID = cph.contractID
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

CREATE proc usp_TradeTotalForCID_Get
        @iContractID int
    
  as
      declare @tprc_b float, @tpos_b float
      declare @tprc_s float, @tpos_s float
 
      select @tprc_b = SUM(quantity * Price), @tpos_b = SUM(quantity)
         from TradeView
        where contractID = @iContractID  
          and IsBuy = 1
      select @tprc_s = SUM(quantity * Price), @tpos_s = SUM(quantity)
         from TradeView
        where contractID = @iContractID  
          and IsBuy = 0
      select isNull(@tprc_b, 0) - isNull(@tprc_s, 0) 'Total_price',
             isNull(@tpos_b, 0) - isNull(@tpos_s, 0) 'Total_position'  
                     
      Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE usp_Trade_Clear
AS
	begin tran
	
	delete from Trade
	
	if @@error = 0
		commit tran
	else
		rollback tran
	
	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_Trade_Del
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
				isPosition)
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
				isPosition
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

CREATE  proc usp_Trade_Get
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
			isPosition as tiIsPosition
		from Trade 
		where 
			(@iTradeID is null or @iTradeID is not null and tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tradeDate >= @dtMinTradeDate)
		group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition
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
				tr.isPosition as tiIsPosition
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
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
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
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
				cph.priceTheo as fPriceTheoClose,
	
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
				tr.isPosition as tiIsPosition
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				inner join Contract uc on o.underlyingContractID = uc.contractID
				left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
							from ContractPriceHistory
							inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
								on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
							cph on c.contractID = cph.contractID
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
				o.expiry,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cph.priceTheo,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
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
				tr.isPosition as tiIsPosition
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
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
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
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
				cph.priceTheo as fPriceTheoClose,
	
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
				tr.isPosition as tiIsPosition
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				inner join Contract uc on o.underlyingContractID = uc.contractID
				inner join ContractInGroup cig on o.underlyingContractID = cig.contractID
				left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
							from ContractPriceHistory
							inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
								on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
							cph on c.contractID = cph.contractID
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
				o.expiry,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cph.priceTheo,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
			having min(tr.actionID) = 1
			order by TradeDate
/*     if (@iTradeID is Null)
        if (@iContractID is Null)
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
			brokerCommission as fBrokerCommission,
			clearingBrokerCommission as fClearingBrokerCommission,
			brokerCommission + clearingBrokerCommission as fTotalCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference
            from Trade 
			order by TradeDate
      else 
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
			brokerCommission as fBrokerCommission,
			clearingBrokerCommission as fClearingBrokerCommission,
			brokerCommission + clearingBrokerCommission as fTotalCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference
            from Trade 
			where contractID = @iContractID order by TradeDate
      else
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
			brokerCommission as fBrokerCommission,
			clearingBrokerCommission as fClearingBrokerCommission,
			brokerCommission + clearingBrokerCommission as fTotalCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference
		    from Trade 
			where tradeID = @iTradeID
*/  
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

CREATE  proc usp_Trade_New
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
        @dtNewTradeDate datetime = Null out
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
		isPosition)
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
		@tiIsPosition)
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  proc usp_Trade_Split
        @iContractID int,
        @fK float,
        @fF float,
        @iC int,
        @iW int,
		@iExchangeID int = Null -- the exchangeID special for update ContractPrice table
  as
begin   
  if (@fK = 0) or (@fF = 0)
     return(-2)
  begin tran
	update ContractPrice 
		set priceBid = priceBid / @fK,
           priceAsk = priceAsk / @fK,
           priceLast = priceLast / @fK,
           priceOpen = priceOpen / @fK,
           priceClose = priceClose / @fK,
           priceLow = priceLow / @fK,
           priceHigh = priceHigh / @fK
		where contractID = @iContractID and exchangeID=@iExchangeID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update ContractPrice for Underlying!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end 
   update Trade
      set Price = Price / @fk,
          quantity = round(quantity * @fK, 0)
    where contractID = @iContractID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update Trade for Underlying!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end 
   update Options
      set Strike = Strike / @fF
    where UnderlyingcontractID = @iContractID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update Options for Underlying!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end
   update ContractPrice
      set priceBid = priceBid / @fF,
          priceAsk = priceAsk / @fF,
          priceLast = priceLast / @fF,
          priceOpen = priceOpen / @fF,
          priceClose = priceClose / @fF,
          priceLow = priceLow / @fF,
          priceHigh = priceHigh / @fF,
          LotSize = @iW
     from Options
    where Options.UnderlyingcontractID = @iContractID
      and ContractPrice.contractID = Options.Option_ID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update ContractPrice for Options!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end
   update Trade
      set Price = Price / @fF,
          quantity = quantity * @iC
     from Options
    where Options.UnderlyingcontractID = @iContractID
      and Trade.contractID = Options.Option_ID
   if (@@error <> 0)
     begin
       RAISERROR ('Error. Can''t update Trade for Options!', 16, 1)
       ROLLBACK TRAN
       RETURN (-1)
     end
  
  commit tran
  return 0
  
end  

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE  proc usp_Trade_Update
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
        @dtNewTradeDate datetime out
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
				isPosition)
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
				isPosition
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
			isPosition)
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
			@tiIsPosition)
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

CREATE  Procedure dbo.usp_TraderContractForHH_Get 
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
					left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID
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
					left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID
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
					s.actionDate as dtActionDate
	
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
					i.actionDate as dtActionDate
	
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
					left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID
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
					left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
								from ContractPriceHistory
								inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
									on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
								cph on c.contractID = cph.contractID
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
					s.actionDate as dtActionDate
	
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
					i.actionDate as dtActionDate
	
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
			left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
						from ContractPriceHistory
						inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
							on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
						cph on c.contractID = cph.contractID
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
			left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceClose, ContractPriceHistory.priceTheo
						from ContractPriceHistory
						inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
							on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
						cph on c.contractID = cph.contractID
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

Create   Procedure dbo.usp_TraderContract_Del 
	@iContractID int = null,
	@iTraderID int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
if 	@iTraderID is not null and @iContractID is not null
	update TraderContract set actionID=3
	--delete TraderContract 
		where traderID = @iTraderID  and ContractID=@iContractID
else if @iTraderID is not null and @iContractID is null
	update TraderContract set actionID=3
	--delete TraderContract 
		where traderID = @iTraderID
else if @iTraderID is null and @iContractID is not null
	update TraderContract set actionID=3
	--delete TraderContract 
		where ContractID=@iContractID
else if @iTraderID is null and @iContractID is null
	update TraderContract set actionID=3
	--delete TraderContract 
	set @error = @@error
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

CREATE    Procedure dbo.usp_TraderContract_Get
	@iContractID int = null,
	@iTraderID int = null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is get information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	select traderID as iTraderID,contractID as iContractID
		from TraderContract
		where 
			(
			(traderID=@iTraderID and ContractID=@iContractID and @iTraderID is not null and @iContractID is not null)
			or
			(traderID=@iTraderID and @iTraderID is not null and @iContractID is null)
			or
			(@iTraderID is null and @iContractID is not null and contractID=@iContractID)
			or 
			(@iTraderID is null and @iContractID is null)
			)
			 and (actionDate>=@dtActionDate or @dtActionDate is null and actionID < 3)
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

CREATE      Procedure dbo.usp_TraderContract_Save
	@iContractID int,
	@iTraderID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is get information from TraderRole table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from TraderContract where 
				contractID=@iContractID and 
				traderID=@iTraderID and 
				actionID < 3)
		begin
			Raiserror ('Row in TraderContract table with same atributes already exists!', 16, 1)
			Return (-1)
		end   
	--if @iContractID is null and @iTraderID is null 
	if not exists(select * from TraderContract where traderID=@iTraderID and contractID=@iContractID and actionID < 3)
		insert into TraderContract(traderID,contractID) 
			values(@iTraderID,@iContractID)
	set @error = @@error
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

CREATE proc usp_TraderRM_Get
     @iTraderRefID int = Null
  as
     if (@iTraderRefID is Null)
		select 	traderID as iTraderID, 
				traderName as vcTraderName 
			from TraderView 
			where traderRefID is null and traderRoleID = 4
			order by TraderName
     else
		select 	traderID as iTraderID, 
				traderName as vcTraderName 
			from TraderView
			where traderRefID = @iTraderRefID  and traderRoleID = 4
            order by TraderName
     if (@@error <> 0) 
			Return (-1)
         else 
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

CREATE  proc usp_TraderRM_Save
     @iTraderID int,
     @iTraderRefID int = Null
  as
	update Trader set traderRefID=@iTraderRefID where traderID = @iTraderID
     if (@@error <> 0) 
			Return (-1)
         else 
			Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   Procedure dbo.usp_TraderRole_Get 
	@iTraderRoleID int = null, 
	@vcRoleName varchar(30) = null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from TraderRole table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 	traderRoleID as iTraderRoleID,
			RoleName as vcRoleName,
			actionDate as dtActionDate,
			actionID as tiActionID
		from TraderRole
		where ((traderRoleID=@iTraderRoleID) or (@iTraderRoleID is null))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)
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

CREATE   Procedure dbo.usp_TraderRole_Save 
	@iTraderRoleID int = Null Output,
	@vcRoleName varchar(30) = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save Roles
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from TraderRole where roleName = @vcRoleName and actionID < 3 and (@iTraderRoleID <> traderRoleID or @iTraderRoleID is null))
		begin
			Raiserror ('TraderRole with same name already exists!', 16, 1)
			Return (-1)
		end   
	Begin Transaction
		if @iTraderRoleID is null
		--if not exists(select * From TraderRole where traderRoleID=@iTraderRoleID) 
			begin
				insert into TraderRole (roleName) values (@vcRoleName)
				set @iTraderRoleID=@@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update TraderRole set 
					roleName = isnull(@vcRoleName, roleName)
				where traderRoleID = @iTraderRoleID
	
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    proc usp_Trader_Del
        @iTraderID int
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Begin Transaction
        update Trader set actionID=3 where traderRefID = @iTraderID		
        --delete Trader where traderRefID = @iTraderID		
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Trader set actionID=3 where traderID = @iTraderID
        --delete Trader where traderID = @iTraderID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction
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

CREATE          proc usp_Trader_Get
	@iTraderID int = Null,
	@bInTrade bit = 0,
	@bShortFormat bit = 0,
	@dtActionDate datetime = null
  as
/*
	Edited by Zaur Nuraliev
	Description: this procedure is getting information from TraderView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
     if (@iTraderID is Null)
        if (@bInTrade = 0 and @bShortFormat = 0) 
			select Trader.traderID as iTraderID, 
					Trader.acronym as vcAcronym, 
					Trader.traderName as vcTraderName, 
					Trader.clearingTraderCode as vcClearingTraderCode,
					Trader.traderRoleID as iTraderRoleID,
					Trader.traderRefID as iTraderRefID,
					TraderRole.roleName as vcRoleName,
					T.traderName as vcTraderRefName,
					Trader.actionDate as dtActionDate,
					Trader.actionID as tiActionID 
				from Trader
					left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
					left join TraderView t on Trader.traderRefID=T.traderID
				where	(Trader.actionDate>=@dtActionDate or @dtActionDate is null) and 
						(@dtActionDate is not null or @dtActionDate is null and Trader.actionID<3)
				order by Trader.TraderName
		else if (@bInTrade = 0 and @bShortFormat = 1) 
				select 	traderID as iTraderID, 
						traderName as vcTraderName 
				from Trader
				where	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
				order by TraderName
        else
			select Trader.traderID as iTraderID, 
					Trader.acronym as vcAcronym, 
					Trader.traderName as vcTraderName, 
					Trader.clearingTraderCode as vcClearingTraderCode,
					Trader.traderRoleID as iTraderRoleID,
					Trader.traderRefID as iTraderRefID,
					TraderRole.roleName as vcRoleName,
					T.traderName as vcTraderRefName,
					Trader.actionDate as dtActionDate,
					Trader.actionID as tiActionID 
				from Trader 
					left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
					left join TraderView t on Trader.traderRefID=T.traderID
				where exists (select tr.traderID from TradeView tr  where tr.traderID = Trader.traderID)
					and	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
	            order by Trader.TraderName
      else 
		select Trader.traderID as iTraderID, 
				Trader.acronym as vcAcronym, 
				Trader.traderName as vcTraderName, 
				Trader.clearingTraderCode as vcClearingTraderCode,
				Trader.traderRoleID as iTraderRoleID,
				Trader.traderRefID as iTraderRefID,
				TraderRole.roleName as vcRoleName,
				T.traderName as vcTraderRefName,
				Trader.actionDate as dtActionDate,
				Trader.actionID as tiActionID  
			from Trader 
				left join TraderRoleView TraderRole on Trader.traderRoleID=TraderRole.traderRoleID
				left join TraderView T on Trader.traderRefID=T.traderID
			where Trader.traderID = @iTraderID
					and	(Trader.actionDate >= @dtActionDate or (@dtActionDate is null and Trader.actionID < 3))
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

CREATE        proc usp_Trader_Save
        @iTraderID int = null,
        @vcAcronym varchar(12) = null,
        @vcTraderName varchar(120) = null,
        @vcClearingTraderCode varchar(11) = null,
		@iTraderRoleID int = null,
		@iTraderRefID int = null
  as
	if exists (select * from Trader where TraderName = @vcTraderName and actionID < 3 and (@iTraderID <> traderID or @iTraderID is null))
		begin
			Raiserror ('Trader with this name already exists!', 16, 1)
			Return (-1)
		end   
	if exists (select * from Trader where Acronym = @vcAcronym and actionID < 3 and (@vcAcronym <> Acronym or @iTraderID is null))
		begin
			Raiserror ('Trader with this acronym already exists!', 16, 1)
			Return (-1)
		end   
   if @iTraderID is null
   --if not exists(select * from Trader where traderID=@iTraderID)  
     begin
         insert into Trader (Acronym, TraderName,clearingTraderCode,traderRoleID,traderRefID)
                values (@vcAcronym, @vcTraderName,@vcClearingTraderCode,@iTraderRoleID,@iTraderRefID)
         if (@@error <> 0) 
			return (-1)
          else  
            return @@identity
     end
   else 
     begin
		if @iTraderRoleID<>3
			update Trader set traderRefID=Null where traderRefID=@iTraderID
		if @iTraderRoleID is null
			select @iTraderRoleID = traderRoleID from Trader where traderID=@iTraderID
		else if @iTraderRoleID=0 
			set @iTraderRoleID = null
	
		if @iTraderRefID is null
			select @iTraderRefID = traderRefID from Trader where traderID=@iTraderID
		else if @iTraderRefID=0 
			set @iTraderRefID = null
		if exists (select * from Trader where acronym = @vcAcronym and actionID < 3 and (@iTraderID <> traderID or @iTraderID is null))
			begin
				Raiserror ('Row in Trader table with same acronym already exists!', 16, 1)
				Return (-1)
			end   
		if exists (select * from Trader where clearingTraderCode = @vcClearingTraderCode and actionID < 3 and (@iTraderID <> traderID or @iTraderID is null))
			begin
				Raiserror ('Row in Trader table with same clearing trader code already exists!', 16, 1)
				Return (-1)
			end   
        update Trader 
             set acronym = isnull(@vcAcronym,acronym),
                traderName = isnull(@vcTraderName,traderName),
				clearingTraderCode = isnull(@vcClearingTraderCode,clearingTraderCode),
				traderRoleID = @iTraderRoleID, 	--isnull(@iTraderRoleID,traderRoleID),
				traderRefID = @iTraderRefID 	--isnull(@iTraderRefID,traderRefID)
          where traderID = @iTraderID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
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

CREATE  Procedure usp_UnderlyingForRisk_Get
        @iContractID int,
        @iBookID int,
        @iStrategyID int,
        @iTraderID int,
        @dtTradeDate datetime = Null,
        @tiIsClose tinyint = Null
as
     set nocount on
     select t.contractID, SUM(t.Price * t.quantity) 'Price'
        into #c 
       from ContractView c, TradeView t
      where c.contractTypeID in (1, 2)
        and ((@iContractID is Null) or ((@iContractID is Null) and (c.contractID = @iContractID)))
        and t.contractID = c.contractID
        and ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
        and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
        and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
        and ((@dtTradeDate is Null) or ((@dtTradeDate is not Null) and (t.TradeDate = @dtTradeDate)))
        and ((@tiIsClose is Null) or ((@tiIsClose is not Null) and (t.IsClose = @tiIsClose)))
      group by t.contractID
      
     select o.UnderlyingcontractID 'contractID', SUM(t.Price * t.quantity) 'Price'
        into #o 
       from OptionView o, TradeView t
      where ((@iContractID is Null) or ((@iContractID is Null) and (o.UnderlyingcontractID = @iContractID))) 
        and t.contractID = o.optionID
        and ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
        and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
        and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
        and ((@dtTradeDate is Null) or ((@dtTradeDate is not Null) and (t.TradeDate = @dtTradeDate)))
        and ((@tiIsClose is Null) or ((@tiIsClose is not Null) and (t.IsClose = @tiIsClose)))
      group by o.UnderlyingcontractID
    select #c.contractID, ContractView.Symbol, #c.Price + isNull(#o.Price, 0) 'Total Price', 1 'bUnd'
      from #c, ContractView, #o
     where ContractView.contractID = #c.contractID
       and #c.contractID *= #o.contractID
     union
    select #o.contractID, ContractView.Symbol, #o.Price 'Total Price', 0 'bUnd'
      from #o, ContractView
     where not exists (select #c.contractID from #c where #c.contractID = #o.contractID)
       and ContractView.contractID = #o.contractID
    order by ContractView.Symbol
    drop table #c
    drop table #o 
    set nocount off

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE  PROCEDURE dbo.usp_VarData_Get
	@iIndexID as integer,
    @iBookID as integer = Null,
    @iStrategyID as integer = Null,
    @iTraderID as integer = Null
AS
	set nocount on
	-- get data for VaR report
		select		-- Stock
			CR.contractID as iContractID,
			0 as iUnderlyingID,
			count(CR.contractID) as iCount,
			CR.Symbol as vcSymbol,
			CR.contractTypeID as iContractType,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceBid as fSpotBid,
			CP.priceAsk as fSpotAsk,
			sum(case TR.IsBuy when 1 then TR.quantity else -TR.quantity end) as iPosition,
			0 as fStrike,
			0 as dtExpiry,
			0 as bIsCall
		from TradeView tr 
			join ContractView cR on CR.contractID = TR.contractID
			join StockView ST on ST.StockID = TR.contractID
			join IndexView IX on ST.BaseIndexID = IX.IndexID
			left join ContractPrice CP on CR.contractID=CP.contractID
		where
			CR.contractTypeID = 2 -- stock
			and IX.IndexID = @iIndexID
			and ((@iBookID is Null) or ((@iBookID is not Null) and (TR.bookID = @iBookID)))
			and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (TR.strategyID = @iStrategyID)))
			and ((@iTraderID is Null) or ((@iTraderID is not Null) and (TR.traderID = @iTraderID)))
		group by
			CR.contractID,
			CR.Symbol,
			CR.contractTypeID,
			CP.priceBid,
			CP.priceAsk
	union
		select		-- Option
			CR.contractID as iContractID,
			OP.UnderlyingcontractID as iUnderlyingID,
			count(CR.contractID) as iCount,
			CR.Symbol as vcSymbol,
			CR.contractTypeID as iContractType,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceBid as fSpotBid,
			CP.priceAsk as fSpotAsk,
			sum(case TR.IsBuy when 1 then TR.quantity else -TR.quantity end) as iPosition,
			OP.Strike as fStrike,
			OP.Expiry as dtExpiry,
			OP.isCall as bIsCall
		from TradeView tr ,
			ContractView cR, 
			ContractPrice CP,
			OptionView OP,
			ContractView cR2,
			Stock ST, 
			IndexView IX
		where
			CR.contractID=TR.contractID and
			OP.OptionID=TR.contractID and
			CR2.contractID = OP.UnderlyingcontractID and 
			(OP.UnderlyingcontractID = ST.StockID and ST.BaseIndexID=IX.IndexID OR OP.UnderlyingcontractID = IX.IndexID) and 
			CR2.contractID=CP.contractID and 
			CR.contractTypeID=3
			and ix.IndexID = @iIndexID
			and ((@iBookID is Null) or ((@iBookID is not Null) and (tr.bookID = @iBookID)))
			and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (tr.strategyID = @iStrategyID)))
			and ((@iTraderID is Null) or ((@iTraderID is not Null) and (tr.traderID = @iTraderID)))
		group by
			cr.contractID,
			op.UnderlyingcontractID,
			cr.Symbol,
			cr.contractTypeID,
			CP.priceBid,
			CP.priceAsk,
			CP.priceBid,
			CP.priceAsk,
			OP.Strike,
			OP.Expiry,
			OP.isCall

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE  PROCEDURE dbo.usp_VarIndex_Get
	@vcIndexSymbol as varchar(8)
AS
	select top 1
		ix.IndexID as iIndexID,
		null as fBeta,
		null as fVolatility,
		null as fCorrelation
	from
		IndexView ix,
		ContractView cr
	where
		ix.indexID = cr.contractID
		and cr.Symbol = @vcIndexSymbol

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.usp_VarIndex_Set
	@vcIndexName as varchar(8),
	@fBeta as float,
	@fVolatility as float,
	@fCorrelation as float
AS
	declare @iIndexID as integer
	set nocount on
	set @iIndexID = 0
	select top 1
		@iIndexID = contractID
	from
		ContractView
	where
		Symbol = @vcIndexName
		and contractTypeID = 1
	if @iIndexID <> 0
	begin
		update
			Indexes
		set
			fBeta = @fBeta,
			fVolatility = @fVolatility,
			fCorrelation = @fCorrelation
		where
			Index_ID = @iIndexID
	end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.usp_VarStockIndex_Set
	@vcStockName as varchar(8),
	@vcIndexName as varchar(8)
AS
	declare @iIndexID as integer
	declare @iStockID as integer
	set nocount on
	set @iIndexID = 0
	set @iStockID = 0
	select top 1
		@iStockID = contractID
	from
		ContractView
	where
		Symbol = @vcStockName
		and contractTypeID = 2
	select top 1
		@iIndexID = contractID
	from
		ContractView
	where
		Symbol = @vcIndexName
		and contractTypeID = 1
	if @iStockID <> 0 and @iIndexID <> 0
	begin
		update
			Stock
		set
			BaseIndexID = @iIndexID
		where
			StockID = @iStockID
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

CREATE        Procedure dbo.usp_VolaSurfaceData_Del
	@iVolaSurfaceDataID int
as
/*
	this procedure deletes data from VolaManagementData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update CustomStrikeSkewPoint set actionID=3
--		delete from CustomStrikeSkewPoint 
			where volaSurfaceDataID = @iVolaSurfaceDataID
			if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
		delete from NextDaySurface 
			where volaSurfaceDataID = @iVolaSurfaceDataID
			if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
		update VolaSurfaceData set actionID=3
--		delete VolaSurfaceData 
			where volaSurfaceDataID = @iVolaSurfaceDataID
			if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
		set @error = 0
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

CREATE         Procedure dbo.usp_VolaSurfaceData_Get
	@iContractID int = null,
	@iVolaSurfaceDataID int = null,
	@dtActionDate datetime = null 
as
/*
	this procedure gets data from VolaManagementData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	select	VolaSurfaceData.volaSurfaceDataID as iVolaSurfaceDataID, 
			VolaSurfaceData.contractID as icontractID, 
			baseUnderlinePrice as fBaseUnderlinePrice, 
			smileAccelerator as fSmileAccelerator,
			interpolationFactor as fInterpolationFactor,
			strikeMoneynessHi as fStrikeMoneynessHi,
			strikeMoneynessLo as fStrikeMoneynessLo,
			surfaceName as vcSurfaceName,
			isPriceOverride as bIsPriceOverride,
			isDiscreteAcceleration as bIsDiscreteAcceleration,
			isDefaultSurface as bIsDefaultSurface,
			ATMVolatility as fATMVolatility,
			Contract.symbol as vcSymbolName,
			case when NextDaySurface.volaSurfaceDataID is not null then 1
				else 0 end bIsNextDaySurface,
			contract.contractTypeID as iContractTypeID,
			VolaSurfaceData.actionDate as dtActionDate,
			VolaSurfaceData.actionID as tiActionID
			from VolaSurfaceData
					join ContractView Contract on VolaSurfaceData.contractID=Contract.contractID
					left join NextDaySurface on VolaSurfaceData.volaSurfaceDataID=NextDaySurface.volaSurfaceDataID
			where	((@iContractID is Null or VolaSurfaceData.contractID = @iContractID) and 
					(@ivolaSurfaceDataID Is Null or VolaSurfaceData.volaSurfaceDataID = @ivolaSurfaceDataID))
				and 
					(VolaSurfaceData.actionDate>=@dtActionDate or @dtActionDate is null)
						and (@dtActionDate is not null or @dtActionDate is null and VolaSurfaceData.actionID<3)
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

CREATE       Procedure dbo.usp_VolaSurfaceData_Save
	@iVolaSurfaceDataID int = null output,
	@iContractID int = null,
	@fBaseUnderlinePrice float,
	@fSmileAccelerator float,
	@fInterpolationFactor float = null,
	@fStrikeMoneynessHi float = null,
	@fStrikeMoneynessLo float = null,
	@vcSurfaceName varchar(250) = null,
	@bIsPriceOverride bit = null,
	@bIsDiscreteAcceleration bit = null,
	@bIsDefaultSurface bit = null,
	@fATMVolatility float = null
as
/*
	this procedure saves data to VolaSurfaceData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @identity int
--	if @iVolaSurfaceDataID is null and @iContractID is not null
--		select @iVolaSurfaceDataID = volaSurfaceDataID from VolaSurfaceData where contractID = @iContractID
		if @iVolaSurfaceDataID is null
--		if not exists(select * from VolaSurfaceData where  VolaSurfaceDataID=@iVolaSurfaceDataID)
			begin
				insert into VolaSurfaceData (
							contractID, 
							baseUnderlinePrice, 
							smileAccelerator,
							interpolationFactor,
							strikeMoneynessHi,
							strikeMoneynessLo,
							surfaceName,
							isPriceOverride,
							isDiscreteAcceleration,
							isDefaultSurface,
							ATMVolatility) 
				values (@iContractID, 
						@fBaseUnderlinePrice,
						@fSmileAccelerator,
						@fInterpolationFactor,
						@fStrikeMoneynessHi,
						@fStrikeMoneynessLo,
						@vcSurfaceName,
						@bIsPriceOverride,
						@bIsDiscreteAcceleration,
						@bIsDefaultSurface,
						@fATMVolatility)
				if @@error <> 0 begin set @error = 1 Rollback Transaction goto finish end
				select @iVolaSurfaceDataID = @@identity
			end
		else
			begin
				update VolaSurfaceData set
					contractID = isnull(@iContractID, contractID),
					baseUnderlinePrice = isnull(@fBaseUnderlinePrice, baseUnderlinePrice),
					smileAccelerator = isnull(@fSmileAccelerator, smileAccelerator),
					interpolationFactor = isnull(@fInterpolationFactor, interpolationFactor),
					strikeMoneynessHi = IsNull(@fStrikeMoneynessHi,strikeMoneynessHi),
					strikeMoneynessLo = IsNull(@fStrikeMoneynessLo,strikeMoneynessLo),
					surfaceName = IsNull(@vcSurfaceName,surfaceName),
					isPriceOverride = IsNull(@bIsPriceOverride,isPriceOverride),
					isDiscreteAcceleration = IsNull(@bIsDiscreteAcceleration,isDiscreteAcceleration),
					isDefaultSurface = IsNull(@bIsDefaultSurface,isDefaultSurface),
					ATMVolatility = IsNull(@fATMVolatility,ATMVolatility)
				where volaSurfaceDataID = @iVolaSurfaceDataID
				if @@error <> 0 begin set @error = 2 Rollback Transaction goto finish end
			end
		set @error = 0
--==================================================================
-- set 'IsDefaultSurface'
--==================================================================
	if (select count(*) 
			from VolaSurfaceDataView 
			where contractID=(select contractID from VolaSurfaceData where VolaSurfaceDataID=@iVolaSurfaceDataID))=1
		begin
			update VolaSurfaceData set isDefaultSurface=1 
			where volaSurfaceDataID = @iVolaSurfaceDataID
		end
	
	else
		begin
			if @bIsDefaultSurface<>0
				begin
					update VolaSurfaceData set isDefaultSurface=0
					where contractID=(select contractID from VolaSurfaceDataView where VolaSurfaceDataID=@iVolaSurfaceDataID)
	
					update VolaSurfaceData set isDefaultSurface=@bIsDefaultSurface 
					where volaSurfaceDataID = @iVolaSurfaceDataID
				end
		end
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

CREATE Procedure dbo.usp_WtdVegaMaturity_Get 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from DataInfo table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select Convert(int,KeyValue) as iWtdVegaMaturity
		from DataInfo	
		where dataInfoID = 4
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

CREATE Procedure dbo.usp_WtdVegaMaturity_Save 
	@iWtdVegaMaturity int		--the stored value of WtdVega
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure update WtdVega value
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		update DataInfo set keyValue = @iWtdVegaMaturity
			where dataInfoID = 4
	
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

CREATE  proc usp_allStockEOD_Get
	@iFormat int = 1	-- 1 - list only
				   		-- 2 - list with prices by exchange
  as
	if @iFormat = 1
		select	iContractID, 
				iContractTypeID, 
				vcSymbol
			from vALLStockEOD A
	else
		-- use NULL exchange only
		select iContractID, 
				iContractTypeID, 
				vcSymbol, 
				null as vcExchangeCode,  
				iDivFreq,  
				fDivAmt, 
				dtDivDate, 
				CP.priceClose as fPriceClose,
				A.bIsHTB as bIsHTB 
			from vALLStockEOD A, ContractPrice CP (nolock)
			where A.iContractID *= CP.contractID and CP.exchangeID is null
/*		select iContractID, iContractTypeID, vcSymbol, E.exchangeCode as vcExchangeCode,  iDivFreq,  fDivAmt, dtDivDate, CP.priceClose as fPriceClose
		from vALLStockEOD A, ContractPrice CP (nolock), Exchange E (nolock)
		where A.iContractID = CP.contractID and CP.exchangeID *= E.exchangeID */
				
if (@@error <> 0)
	Return (-1)
else  
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

CREATE     proc usp_allStockOptionEOD_Get
	@iStockID int = null
as
	if @iStockID is null 
		-- all options
	 	 select	C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				O.expiry as dtExpiry
         	from Contract C (nolock), [Option] O (nolock)
	   		where expiry >= GetDate() 
				and O.optionID = C.contractID
	else
	-- get list of NULL exchanges
	 	select C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				O.expiry as dtExpiry
            from Contract C (nolock)join [Option] O (nolock) on O.optionID = C.contractID
	   		where O.underlyingContractID = @iStockID 
				and expiry >= GetDate()
/*	union
	-- get list of traded exchanges
	 	 select	C.contractID as iContractID , C.Symbol as vcSymbol,  E.exchangeCode as vcExchangeCode, E.exchangeID as iExchangeID, O.isCall as bIsCall, O.strike as fStrike, O.expiry as dtExpiry
                 	from Contract C (nolock), [Option] O (nolock), ContractExchangeLink CEL (nolock), Exchange E (nolock)
   		where O.underlyingContractID = @iStockID  and expiry >= GetDate() and O.optionID = C.contractID and O.underlyingContractID = CEL.contractID and right(CEL.exchangeLinkType, 1) = '1' and  CEL.exchangeID = E.exchangeID */
				
	if (@@error <> 0)
		Return (-1)
	else  
	   Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   procedure dbo.usp_completeContractPriceEOD_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
Begin tran 
   if object_id('tempdb..##ContractPriceEOD') is null
   begin
       raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
       return (-1)
   end
        select C.contractID, E.exchangeID, A.priceClose into #temp
           from ##ContractPriceEOD A, Contract C (nolock), Exchange E (nolock)
       where A.symbol = C.symbol and A.exchangeCode *= E.exchangeCode
     
       
      update ContractPrice
            set contractID = S.contractID, exchangeID = S.exchangeID, priceClose = S.priceClose, priceDate = getdate()
         from ContractPrice Z join #temp S on Z.contractID = S.contractID and isnull(Z.exchangeID, 0) = isnull(S.exchangeID, 0)
       where Z.priceDate between convert(datetime, convert(varchar(15), GetDate(), 112), 112) and
	                   	convert(datetime, convert(varchar(15), GetDate()+1, 112), 112)
     insert into ContractPrice (contractID, exchangeID, priceClose, priceDate)
         select *, getdate()
           from #temp B
          where not exists ( select * from ContractPrice A where A.contractID = B.contractID and isnull(A.exchangeID, 0) = isnull(B.exchangeID, 0))
Commit tran
	if object_id('tempdb..##ContractPriceEOD') is not null
		drop table ##ContractPriceEOD
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

CREATE   procedure dbo.usp_initContractPriceEOD_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if object_id('tempdb..##ContractPriceEOD') is not null
	begin
		raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
		return (-1) 
		-- drop table ##ContractPriceEOD
	end
	create table ##ContractPriceEOD(
		symbol varchar (8) null ,
		contractTypeID int not null,
		priceClose float not null,
		exchangeCode char(10) null) 
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE     proc usp_Index_Del
		@iIndexID int
  as
/*
	Created by ...
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	 if exists (select tradeID from Trade
					 where Trade.contractID = @iIndexID)
	 begin
	   RAISERROR ('Error. Can''t delete [Index] because of the existing Trade', 16, 1)
	   RETURN (-1)
	 end

	Begin Transaction

		delete ModelParameter where contractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update [Option] set actionID=3 where underlyingContractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 
		where contractID in (select OptionID from [Option] where underlyingContractID = @iIndexID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 
		where contractID in (select underlyingContractID from [Option] where optionID= @iIndexID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		
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



CREATE   Procedure usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(8),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255)
	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from Contract 
		where symbol = @vcUnderlyingSymbol and contractTypeID=2
	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3
	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where underlyingContractID = @iUnderlyingContractID and 
				isCall = @iIsCall and
				abs(strike - @fStrike) < 0.009 and --round the strike value
				month(expiry) = month(@dtExpiry) and
				year(expiry) = year(@dtExpiry)

	--===================================================================			
	-- try to find Option with same ticker in DB
	--===================================================================			
	declare @iIncorrectOptionID int

	select @iIncorrectOptionID = contractID 
		from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
		where symbol = @vcSymbol and contractTypeID = 3 and 
			[Option].actionID < 3 and (@OptionID <> contractID or @OptionID is null)

	if @iIncorrectOptionID is not null
		begin
			-- mark as delete

			update [Option] set [Option].actionID = 3 
				where optionID = @iIncorrectOptionID and expiry < GetDate()

			if @@rowcount = 0 
				begin
					--Raiserror ('Option with this symbol already exists!', 16, 1)
					--Return (-1)
					update [Option] set [Option].actionID = 3 
						where optionID = @iIncorrectOptionID
					update Contract set actionID = 3 
						where ContractID = @iIncorrectOptionID
				end   
			else
				update Contract set actionID = 3 
					where ContractID = @iIncorrectOptionID
		end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

  if (@OptionID is Null) 
	begin
		Begin Transaction
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiry)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @dtExpiry)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest,lotSize)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest,@iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID,lotSize)
						values(@OptionID,null,@iLotSize)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
		Commit Transaction
	end 
   else
	begin
		  update Contract
			   set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 
		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest,
					lotSize = isnull(@iLotSize,lotSize)
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 
	end 
	 
-- return @OptionID 
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



CREATE        proc usp_Stock_Save
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
		@dtDivDateCustom smalldatetime = Null
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
				divDateCustom)
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
				@dtDivDateCustom)
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
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom)
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
--================================================================================================
-- change DB version to 3.07
--================================================================================================
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.07')
	begin
		update DataInfo set keyValue = '3.07' where DataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.07'
	end
GO