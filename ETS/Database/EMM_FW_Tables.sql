if exists (select * from dbo.sysobjects where id = object_id(N'[_stock]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [_stock]
GO

CREATE TABLE [_stock] (
	[f1] [varchar] (30)   NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Book]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Book]
GO

CREATE TABLE [Book] (
	[bookID] [int] IDENTITY (1, 1) NOT NULL ,
	[bookName] [varchar] (12)   NOT NULL ,
	[description] [varchar] (120)   NULL ,
	[priority] [int] NULL ,
	[actionID] [tinyint] NULL ,
	[actionDate] [datetime] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[BookRuleItem]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [BookRuleItem]
GO

CREATE TABLE [BookRuleItem] (
	[bookRuleItemID] [int] IDENTITY (1, 1) NOT NULL ,
	[itemName] [varchar] (255)   NOT NULL ,
	[ruleItemTypeID] [int] NULL ,
	[bookID] [int] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[BookRuleItemMask]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [BookRuleItemMask]
GO

CREATE TABLE [BookRuleItemMask] (
	[bookRuleItemMaskID] [int] IDENTITY (1, 1) NOT NULL ,
	[maskName] [varchar] (255)   NOT NULL ,
	[bookRuleItemID] [int] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Broker]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Broker]
GO

CREATE TABLE [Broker] (
	[brokerID] [int] IDENTITY (1, 1) NOT NULL ,
	[brokerName] [varchar] (12)   NOT NULL ,
	[description] [varchar] (120)   NULL ,
	[isClearing] [tinyint] NULL ,
	[actionDate] [datetime] NULL ,
	[actionID] [tinyint] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ClearingHouseTrade]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ClearingHouseTrade]
GO

CREATE TABLE [ClearingHouseTrade] (
	[iDateOfTrade] [int] NOT NULL ,
	[iCacheID] [int] NOT NULL ,
	[ins_upd_del_flag] [tinyint] NULL ,
	[put_call] [tinyint] NULL ,
	[buy_sell] [tinyint] NULL ,
	[origin] [tinyint] NULL ,
	[open_close] [tinyint] NULL ,
	[trade_id] [varchar] (10)   NULL ,
	[user_id] [varchar] (10)   NULL ,
	[trading_sym] [varchar] (7)   NULL ,
	[base_sym] [varchar] (7)   NULL ,
	[parent_account] [varchar] (11)   NULL ,
	[account] [varchar] (11)   NULL ,
	[q_account] [varchar] (11)   NULL ,
	[security_type] [varchar] (3)   NULL ,
	[security_subtype] [varchar] (3)   NULL ,
	[clearing_firm] [varchar] (5)   NULL ,
	[cusip] [varchar] (10)   NULL ,
	[basket_id] [varchar] (6)   NULL ,
	[giveup_firm] [varchar] (5)   NULL ,
	[contra_firm] [varchar] (5)   NULL ,
	[exec_broker] [varchar] (4)   NULL ,
	[contra_broker] [varchar] (4)   NULL ,
	[mmcs_sub_acct] [varchar] (3)   NULL ,
	[exercise_multiplier] [float] NULL ,
	[extended_premium] [float] NULL ,
	[premium] [float] NULL ,
	[strike] [float] NULL ,
	[price] [float] NULL ,
	[quantity] [int] NULL ,
	[tag] [int] NULL ,
	[expiration_date] [int] NULL ,
	[execution_date] [int] NULL ,
	[last_upd_date] [int] NULL ,
	[contract_date] [int] NULL ,
	[source_system] [int] NULL ,
	[trade_id_num] [int] NULL ,
	[trade_group_id] [int] NULL ,
	[settlement_date] [int] NULL ,
	[entry_firm] [varchar] (5)   NULL ,
	[matched_flag] [tinyint] NULL ,
	[spread_ind] [tinyint] NULL ,
	[premium_multiplier] [float] NULL ,
	[source_code] [varchar] (3)   NULL ,
	[parent_acct_intrl_id] [varchar] (14)   NULL ,
	[acct_intrl_id] [varchar] (14)   NULL ,
	[q_acct_intrl_id] [varchar] (14)   NULL ,
	[cap_interval] [float] NULL ,
	[trade_exchange] [int] NULL ,
	[settled_unsettled_ind] [tinyint] NULL ,
	[stock_origin] [tinyint] NULL ,
	[mm_card_number] [varchar] (8)   NULL ,
	[basket_tag] [int] NULL ,
	[basket_price] [float] NULL ,
	[short_sale] [tinyint] NULL ,
	[commission_code] [varchar] (3)   NULL ,
	[commission] [float] NULL ,
	[same_day_exer] [tinyint] NULL ,
	[terminal_name] [varchar] (11)   NULL ,
	[portfolio_name] [varchar] (255)   NULL ,
	[ClearingHouseTradeID] [int] IDENTITY (1, 1) NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Commission]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Commission]
GO

CREATE TABLE [Commission] (
	[commissionID] [int] IDENTITY (1, 1) NOT NULL ,
	[brokerID] [int] NOT NULL ,
	[commissionDate] [smalldatetime] NOT NULL ,
	[commissionValue] [float] NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[actionID] [tinyint] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Contract]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Contract]
GO

CREATE TABLE [Contract] (
	[contractID] [int] IDENTITY (1, 1) NOT NULL ,
	[contractTypeID] [int] NOT NULL ,
	[symbol] [varchar] (8)   NOT NULL ,
	[contractName] [varchar] (255)   NULL ,
	[ruleID] [int] NULL ,
	[groupID] [int] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[MSupdateDate] [datetime] NULL ,
	[expCalendarID] [int] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ContractExchangeLink]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ContractExchangeLink]
GO

CREATE TABLE [ContractExchangeLink] (
	[contractExchangeLinkID] [int] IDENTITY (1, 1) NOT NULL ,
	[exchangeID] [int] NULL ,
	[contractID] [int] NOT NULL ,
	[exchangeLinkType] [varchar] (2)   NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ContractGroup]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ContractGroup]
GO

CREATE TABLE [ContractGroup] (
	[groupID] [int] IDENTITY (1, 1) NOT NULL ,
	[groupName] [varchar] (30)   NOT NULL ,
	[description] [varchar] (120)   NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ContractInGroup]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ContractInGroup]
GO

CREATE TABLE [ContractInGroup] (
	[contractInGroupID] [int] IDENTITY (1, 1) NOT NULL ,
	[contractID] [int] NOT NULL ,
	[groupID] [int] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ContractMarketData]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ContractMarketData]
GO

CREATE TABLE [ContractMarketData] (
	[contractID] [int] NOT NULL ,
	[date] [datetime] NOT NULL ,
	[priceMid] [float] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ContractPrice]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ContractPrice]
GO

CREATE TABLE [ContractPrice] (
	[contractPriceID] [int] IDENTITY (1, 1) NOT NULL ,
	[contractID] [int] NOT NULL ,
	[exchangeID] [int] NULL ,
	[priceBid] [float] NULL ,
	[priceAsk] [float] NULL ,
	[priceLast] [float] NULL ,
	[priceOpen] [float] NULL ,
	[priceClose] [float] NULL ,
	[priceLow] [float] NULL ,
	[priceHigh] [float] NULL ,
	[lotSize] [int] NULL ,
	[volume] [int] NULL ,
	[openInterest] [int] NULL ,
	[priceDate] [datetime] NOT NULL ,
	[priceTheo] [float] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[CustomStrikeSkewPoint]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [CustomStrikeSkewPoint]
GO

CREATE TABLE [CustomStrikeSkewPoint] (
	[customStrikeSkewPointID] [int] IDENTITY (1, 1) NOT NULL ,
	[volaSurfaceDataID] [int] NOT NULL ,
	[strike] [float] NOT NULL ,
	[type] [int] NOT NULL ,
	[volatility] [float] NOT NULL ,
	[expiryID] [int] NOT NULL ,
	[isBasePoint] [int] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[DataInfo]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [DataInfo]
GO

CREATE TABLE [DataInfo] (
	[dataInfoID] [int] NOT NULL ,
	[keyName] [varchar] (50)   NOT NULL ,
	[keyValue] [varchar] (150)   NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Exchange]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Exchange]
GO

CREATE TABLE [Exchange] (
	[exchangeID] [int] IDENTITY (1, 1) NOT NULL ,
	[exchangeName] [varchar] (30)   NOT NULL ,
	[description] [varchar] (120)   NULL ,
	[exchangeCode] [varchar] (10)   NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ExchangeHoliday]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ExchangeHoliday]
GO

CREATE TABLE [ExchangeHoliday] (
	[exchangeHolidayID] [int] IDENTITY (1, 1) NOT NULL ,
	[exchangeID] [int] NOT NULL ,
	[holidayID] [int] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Expiry]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Expiry]
GO

CREATE TABLE [Expiry] (
	[expiryID] [int] NOT NULL ,
	[expiryPeriod] [varchar] (30)   NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ExpiryCalendar]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ExpiryCalendar]
GO

CREATE TABLE [ExpiryCalendar] (
	[expCalendarID] [int] IDENTITY (1, 1) NOT NULL ,
	[expCalendarName] [varchar] (50)   NOT NULL ,
	[expCalendarDesc] [varchar] (250)   NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ExpiryInCalendar]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ExpiryInCalendar]
GO

CREATE TABLE [ExpiryInCalendar] (
	[expCalendarID] [int] NOT NULL ,
	[expiryID] [int] NOT NULL ,
	[expiryDate] [smalldatetime] NOT NULL ,
	[vegaWeight] [float] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Filter]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Filter]
GO

CREATE TABLE [Filter] (
	[filterID] [int] IDENTITY (1, 1) NOT NULL ,
	[filterName] [varchar] (80)   NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[FilterContent]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [FilterContent]
GO

CREATE TABLE [FilterContent] (
	[filterID] [int] NOT NULL ,
	[filterFieldID] [tinyint] NOT NULL ,
	[fieldValue] [varchar] (15)   NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[FilterField]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [FilterField]
GO

CREATE TABLE [FilterField] (
	[filterFieldID] [tinyint] IDENTITY (1, 1) NOT NULL ,
	[fieldName] [varchar] (50)   NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[GenerationRule]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [GenerationRule]
GO

CREATE TABLE [GenerationRule] (
	[ruleID] [int] IDENTITY (1, 1) NOT NULL ,
	[ruleName] [varchar] (250)   NOT NULL ,
	[interpolationFactor] [float] NULL ,
	[callShift] [float] NULL ,
	[putShift] [float] NULL ,
	[smileAccelerator] [float] NULL ,
	[strikeIncrement] [int] NULL ,
	[strikeMoneynessLo] [float] NULL ,
	[strikeMoneynessHi] [float] NULL ,
	[isDefaultRule] [bit] NOT NULL ,
	[requiredPoints] [int] NULL ,
	[ruleType] [int] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[GenerationRuleGroup]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [GenerationRuleGroup]
GO

CREATE TABLE [GenerationRuleGroup] (
	[groupID] [int] IDENTITY (1, 1) NOT NULL ,
	[groupName] [varchar] (250)   NOT NULL ,
	[ruleID] [int] NULL ,
	[isActive] [bit] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[HistRangesCache]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [HistRangesCache]
GO

CREATE TABLE [HistRangesCache] (
	[contractID] [int] NOT NULL ,
	[optionExpDate] [datetime] NOT NULL ,
	[strike] [float] NOT NULL ,
	[HVLow] [float] NOT NULL ,
	[HVHigh] [float] NOT NULL ,
	[isATM] [bit] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Holiday]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Holiday]
GO

CREATE TABLE [Holiday] (
	[holidayID] [int] IDENTITY (1, 1) NOT NULL ,
	[holidayName] [varchar] (30)   NOT NULL ,
	[holidayDate] [smalldatetime] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Index]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Index]
GO

CREATE TABLE [Index] (
	[indexID] [int] NOT NULL ,
	[defaultModelTypeID] [int] NULL ,
	[yield] [float] NULL ,
	[isHTB] [tinyint] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[calcOptionType] [tinyint] NULL ,
	[calcModelType] [tinyint] NULL ,
	[isBasket] [tinyint] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[IndexCorrelation]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IndexCorrelation]
GO

CREATE TABLE [IndexCorrelation] (
	[indexID1] [int] NOT NULL ,
	[indexID2] [int] NOT NULL ,
	[correlationPrice] [float] NULL ,
	[correlationVola] [float] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[IndexDataHistory]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IndexDataHistory]
GO

CREATE TABLE [IndexDataHistory] (
	[indexID] [int] NOT NULL ,
	[date] [datetime] NOT NULL ,
	[tenorID] [int] NOT NULL ,
	[delta1] [float] NULL ,
	[delta3] [float] NULL ,
	[impliedCorrelation] [float] NULL ,
	[currentIV] [float] NULL ,
	[IVMean] [float] NULL ,
	[IVCall] [float] NULL ,
	[IVPut] [float] NULL ,
	[currentHV] [float] NULL ,
	[currentVega] [float] NULL ,
	[currentVolOfVol] [float] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[IndexDefinition]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IndexDefinition]
GO

CREATE TABLE [IndexDefinition] (
	[indexDefinitionID] [int] IDENTITY (1, 1) NOT NULL ,
	[indexID] [int] NOT NULL ,
	[stockID] [int] NOT NULL ,
	[weight] [float] NOT NULL ,
	[beta] [float] NULL ,
	[correlationPrice] [float] NULL ,
	[correlationVola] [float] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[IRCurve]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IRCurve]
GO

CREATE TABLE [IRCurve] (
	[curveID] [int] IDENTITY (1, 1) NOT NULL ,
	[curveName] [varchar] (30)   NOT NULL ,
	[ruleTypeID] [int] NULL ,
	[curveTypeID] [int] NULL ,
	[positionThreshold] [money] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[IRPoint]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IRPoint]
GO

CREATE TABLE [IRPoint] (
	[IRpointID] [int] IDENTITY (1, 1) NOT NULL ,
	[curveID] [int] NOT NULL ,
	[periodTypeID] [int] NOT NULL ,
	[num] [int] NOT NULL ,
	[shortRate] [float] NOT NULL ,
	[HTBRate] [float] NOT NULL ,
	[longRate] [float] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[JPMCOR]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [JPMCOR]
GO

CREATE TABLE [JPMCOR] (
	[currency1] [nvarchar] (3)   NULL ,
	[var_class_code1] [nvarchar] (4)   NULL ,
	[currency2] [nvarchar] (3)   NULL ,
	[var_class_code2] [nvarchar] (4)   NULL ,
	[correlation] [float] NULL ,
	[dataset] [nvarchar] (1)   NULL ,
	[update_flag] [nvarchar] (1)   NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[JPMVOL]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [JPMVOL]
GO

CREATE TABLE [JPMVOL] (
	[dataset] [nvarchar] (1)   NULL ,
	[currency] [nvarchar] (3)   NULL ,
	[var_class_code] [nvarchar] (4)   NULL ,
	[price_vol] [float] NULL ,
	[update_flag] [nvarchar] (1)   NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[MLState]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [MLState]
GO

CREATE TABLE [MLState] (
	[mlStateID] [int] IDENTITY (1, 1) NOT NULL ,
	[mlType] [int] NOT NULL ,
	[name] [varchar] (50)   NOT NULL ,
	[columns] [varchar] (255)   NOT NULL ,
	[groups] [varchar] (255)   NOT NULL ,
	[data] [varchar] (255)   NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[ModelParameter]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [ModelParameter]
GO

CREATE TABLE [ModelParameter] (
	[parameterID] [int] IDENTITY (1, 1) NOT NULL ,
	[contractID] [int] NULL ,
	[modelTypeID] [int] NULL ,
	[param1] [float] NULL ,
	[param2] [float] NULL ,
	[param3] [float] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[NextDaySurface]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [NextDaySurface]
GO

CREATE TABLE [NextDaySurface] (
	[nextDaySurfaceID] [int] IDENTITY (1, 1) NOT NULL ,
	[contractID] [int] NOT NULL ,
	[volaSurfaceDataID] [int] NOT NULL ,
	[dueDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Option]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Option]
GO

CREATE TABLE [Option] (
	[optionID] [int] NOT NULL ,
	[underlyingContractID] [int] NOT NULL ,
	[isCall] [tinyint] NOT NULL ,
	[strike] [float] NOT NULL ,
	[expiryID] [int] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[isManualyEntered] [tinyint] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[OptionDataCache]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [OptionDataCache]
GO

CREATE TABLE [OptionDataCache] (
	[optionID] [int] NOT NULL ,
	[date] [datetime] NOT NULL ,
	[IV] [float] NOT NULL ,
	[priceMid] [float] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[opts_price]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [opts_price]
GO

CREATE TABLE [opts_price] (
	[OptionName] [varchar] (15)   NOT NULL ,
	[isCall] [int] NOT NULL ,
	[price] [real] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[OptSymbol]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [OptSymbol]
GO

CREATE TABLE [OptSymbol] (
	[symbol] [varchar] (8)   NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[RMScenario]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [RMScenario]
GO

CREATE TABLE [RMScenario] (
	[RMScenarioID] [int] IDENTITY (1, 1) NOT NULL ,
	[scenarioName] [varchar] (30)   NOT NULL ,
	[description] [varchar] (120)   NULL ,
	[points1] [int] NOT NULL ,
	[step1] [float] NOT NULL ,
	[isAbs1] [tinyint] NOT NULL ,
	[type1] [tinyint] NOT NULL ,
	[points2] [int] NOT NULL ,
	[step2] [float] NOT NULL ,
	[isAbs2] [tinyint] NOT NULL ,
	[type2] [tinyint] NOT NULL ,
	[actionID] [int] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[RuleItemType]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [RuleItemType]
GO

CREATE TABLE [RuleItemType] (
	[ruleItemTypeID] [int] IDENTITY (1, 1) NOT NULL ,
	[itemTypeName] [varchar] (255)   NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Stock]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Stock]
GO

CREATE TABLE [Stock] (
	[stockID] [int] NOT NULL ,
	[defaultModelTypeID] [int] NULL ,
	[defaultSurfaceTypeID] [int] NULL ,
	[divFreq] [int] NULL ,
	[baseIndexID] [int] NULL ,
	[isHTB] [tinyint] NULL ,
	[primaryExchangeID] [int] NULL ,
	[divAmt] [float] NULL ,
	[divDate] [smalldatetime] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[isDivCustom] [tinyint] NOT NULL ,
	[divAmtCustom] [float] NULL ,
	[divDateCustom] [smalldatetime] NULL ,
	[divFreqCustom] [int] NULL ,
	[calcOptionType] [tinyint] NULL ,
	[calcModelType] [tinyint] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[StockCorrelation]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [StockCorrelation]
GO

CREATE TABLE [StockCorrelation] (
	[stockID1] [int] NOT NULL ,
	[stockID2] [int] NOT NULL ,
	[correlationPrice] [float] NULL ,
	[correlationVola] [float] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[StockDataHistory]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [StockDataHistory]
GO

CREATE TABLE [StockDataHistory] (
	[stockID] [int] NOT NULL ,
	[date] [datetime] NOT NULL ,
	[tenorID] [int] NOT NULL ,
	[IVMean] [float] NOT NULL ,
	[IVCall] [float] NULL ,
	[IVPut] [float] NULL ,
	[currentHV] [float] NULL ,
	[currentVolOfVol] [float] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Strategy]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Strategy]
GO

CREATE TABLE [Strategy] (
	[strategyID] [int] IDENTITY (1, 1) NOT NULL ,
	[strategyName] [varchar] (12)   NOT NULL ,
	[description] [varchar] (120)   NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Tenor]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Tenor]
GO

CREATE TABLE [Tenor] (
	[tenorID] [int] IDENTITY (1, 1) NOT NULL ,
	[value] [int] NOT NULL ,
	[valueIV] [int] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Trade]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Trade]
GO

CREATE TABLE [Trade] (
	[seqNum] [int] IDENTITY (1, 1) NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[tradeID] [int] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[contractID] [int] NOT NULL ,
	[isClose] [tinyint] NOT NULL ,
	[tradeDate] [datetime] NULL ,
	[quantity] [int] NOT NULL ,
	[price] [float] NOT NULL ,
	[isBuy] [tinyint] NOT NULL ,
	[bookID] [int] NULL ,
	[strategyID] [int] NULL ,
	[traderID] [int] NULL ,
	[brokerID] [int] NULL ,
	[clearingBrokerID] [int] NULL ,
	[brokerCommissionID] [int] NULL ,
	[clearingBrokerCommissionID] [int] NULL ,
	[tradedIV] [float] NULL ,
	[spotReference] [float] NULL ,
	[isPosition] [tinyint] NULL ,
	[lotSize] [int] NOT NULL ,
	[isSimulated] [tinyint] NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[Trader]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [Trader]
GO

CREATE TABLE [Trader] (
	[traderID] [int] IDENTITY (1, 1) NOT NULL ,
	[acronym] [varchar] (12)   NOT NULL ,
	[traderName] [varchar] (120)   NOT NULL ,
	[clearingTraderCode] [char] (11)   NULL ,
	[traderRoleID] [int] NULL ,
	[traderRefID] [int] NULL ,
	[actionDate] [datetime] NOT NULL ,
	[actionID] [tinyint] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[TraderContract]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [TraderContract]
GO

CREATE TABLE [TraderContract] (
	[traderContractID] [int] IDENTITY (1, 1) NOT NULL ,
	[contractID] [int] NOT NULL ,
	[traderID] [int] NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[actionID] [tinyint] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[TraderRole]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [TraderRole]
GO

CREATE TABLE [TraderRole] (
	[traderRoleID] [int] IDENTITY (1, 1) NOT NULL ,
	[roleName] [varchar] (150)   NOT NULL ,
	[actionDate] [datetime] NOT NULL ,
	[actionID] [tinyint] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[VarCorrelation]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [VarCorrelation]
GO

CREATE TABLE [VarCorrelation] (
	[dataSet] [varchar] (1)   NOT NULL ,
	[currency1] [varchar] (3)   NOT NULL ,
	[currency2] [varchar] (3)   NOT NULL ,
	[varCode1] [varchar] (4)   NOT NULL ,
	[varCode2] [varchar] (4)   NOT NULL ,
	[correlation] [float] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[VarReport]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [VarReport]
GO

CREATE TABLE [VarReport] (
	[currency] [varchar] (3)   NOT NULL,
	[varCode] [varchar] (4)   NOT NULL,
	[volume] [float] NOT NULL,
	[varPosition] [float] NOT NULL,
	[varValue] [float] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[VarVolatility]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [VarVolatility]
GO

CREATE TABLE [VarVolatility] (
	[dataSet] [varchar] (1)   NOT NULL,
	[currency] [varchar] (3)   NOT NULL,
	[varCode] [varchar] (4)   NOT NULL,
	[volatility] [float] NOT NULL 
) ON [PRIMARY]
GO


if exists (select * from dbo.sysobjects where id = object_id(N'[VolaSurfaceData]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [VolaSurfaceData]
GO

CREATE TABLE [VolaSurfaceData] (
	[volaSurfaceDataID] [int] IDENTITY (1, 1) NOT NULL ,
	[contractID] [int] NOT NULL ,
	[baseUnderlinePrice] [float] NULL ,
	[smileAccelerator] [float] NULL ,
	[interpolationFactor] [float] NULL ,
	[strikeMoneynessHi] [float] NULL ,
	[strikeMoneynessLo] [float] NULL ,
	[surfaceName] [varchar] (250)   NOT NULL ,
	[isPriceOverride] [bit] NULL ,
	[isDiscreteAcceleration] [bit] NULL ,
	[isDefaultSurface] [bit] NULL ,
	[ATMVolatility] [float] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO



