if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BookView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[BookView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BrokerView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[BrokerView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CommissionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[CommissionView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractGroupView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractGroupView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractInGroupView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractInGroupView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  View dbo.ContractView
as
	Select 
		contractID,
		contractTypeID,
		symbol,
		contractName,
		ruleID,
		groupID,
		actionID,
		actionDate,
		expCalendarID 
	from Contract
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CustomStrikeSkewPointView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[CustomStrikeSkewPointView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE View dbo.CustomStrikeSkewPointView
as
	Select 
		customStrikeSkewPointID,
		volaSurfaceDataID,
		strike,
		type,
		volatility,
		expiryID,
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExchangeHolidayView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExchangeHolidayView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExchangeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExchangeView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExpiryView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExpiryView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  View dbo.ExpiryView
as
	Select 
		expiryID,
		expiryPeriod,
		actionID,
		actionDate
	from Expiry
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[HolidayView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[HolidayView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexDefinitionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexDefinitionView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    View dbo.IndexView
as
	Select 
		indexID,
		defaultModelTypeID,
		yield,
		isHTB,
		actionID,
		actionDate,
		calcOptionType,
		calcModelType,
		isBasket
	from [Index]
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IRCurveView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IRCurveView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IRPointView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IRPointView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[OptionView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  View dbo.OptionView
as
	Select 
		optionID,
		underlyingContractID,
		isCall,
		strike,
		expiryID,
		actionID,
		actionDate,
		ExpiryOV, 
		TradingClose
	from [Option]
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[RMScenarioView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[RMScenarioView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StockView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StockView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StrategyView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StrategyView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TraderContractView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderRoleView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TraderRoleView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TraderView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradeView]
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



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[vALLStockEOD]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[vALLStockEOD]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE          view dbo.vALLStockEOD
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
		S.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID,
		null as tiIsBasket
	--FROM ContractView C (nolock), StockView S (nolock)
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
		I.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID,
		I.isBasket as tiIsBasket
	--FROM ContractView C (nolock), IndexView I (nolock)
	FROM Contract C (nolock), IndexView I (nolock)
	WHERE I.indexID = C.contractID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[VolaSurfaceDataView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[VolaSurfaceDataView]
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
