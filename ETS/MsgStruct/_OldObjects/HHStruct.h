#ifndef HHSTRUCT_H
#define HHSTRUCT_H

#include "MsgPacking.h"

struct __CommonData
{
	TStaticField<RECORD_TYPE>	m_enRecordType;
	LongF						m_nSequenceNumber;
	DateF						m_dtUpdateDate;
	LongF						m_nTotalRecords;

	TStaticField<RECORD_STATUS>	m_enStatus;
	LongF						m_nErrorCode;
	StringF						m_strErrorDescription;
};

#define FIELD_COMMON_ENTRIES					\
		FIELD_ENTRY(m_enRecordType)				\
		FIELD_ENTRY(m_nSequenceNumber)			\
		FIELD_ENTRY(m_dtUpdateDate)				\
		FIELD_ENTRY(m_nTotalRecords)			\
		FIELD_ENTRY(m_enStatus)					\
		FIELD_ENTRY(m_nErrorCode)				\
		FIELD_ENTRY(m_strErrorDescription)

struct __BrokerData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_Name)
		FIELD_ENTRY(m_Description)
		FIELD_ENTRY(m_IsClearing)
	END_PACKING

	LongF		m_nID;
	StringF		m_Name;
	StringF		m_Description;
	LongF		m_IsClearing;
};

struct __TraderData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_Name)
		FIELD_ENTRY(m_Acronym)
	END_PACKING

	LongF		m_nID;
	StringF		m_Name;
	StringF		m_Acronym;
};

struct __ExchangeData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_Name)
		FIELD_ENTRY(m_Description)
		FIELD_ENTRY(m_Code)
		FIELD_ENTRY(m_nIsUnderlying)
		FIELD_ENTRY(m_nIsOption)
	END_PACKING

	LongF		m_nID;
	StringF		m_Name;
	StringF		m_Description;
	StringF		m_Code;
	LongF		m_nIsUnderlying;
	LongF		m_nIsOption;
};

struct __HolidayData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_Name)
		FIELD_ENTRY(m_Date)
	END_PACKING

	LongF		m_nID;
	StringF		m_Name;
	DateF		m_Date;
};

struct __ExchangeHolidayData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nHolidayID)
		FIELD_ENTRY(m_nExchangeID)
	END_PACKING

	LongF	m_nHolidayID;
	LongF	m_nExchangeID;
};

struct __BookData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_Name)
		FIELD_ENTRY(m_Description)
		FIELD_ENTRY(m_nPriority)
	END_PACKING

	LongF		m_nID;
	StringF		m_Name;
	StringF		m_Description;
	LongF		m_nPriority;
};

struct __StrategyData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_Name)
		FIELD_ENTRY(m_Description)
	END_PACKING

	LongF		m_nID;
	StringF		m_Name;
	StringF		m_Description;
};

struct __CommissionData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_nBrokerID)
		FIELD_ENTRY(m_dValue)
		FIELD_ENTRY(m_Date)
	END_PACKING

	LongF		m_nID;
	LongF		m_nBrokerID;
	DoubleF		m_dValue;
	DateF		m_Date;
};

struct __IndexData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_dYield)
		FIELD_ENTRY(m_IsHTB)
		FIELD_ENTRY(m_nCalcModelType)
		FIELD_ENTRY(m_nCalcOptionType)
		FIELD_ENTRY(m_nIsBasket)
		FIELD_ENTRY(m_nIsActive)
		FIELD_ENTRY(m_dSkew)
		FIELD_ENTRY(m_dKurt)
	END_PACKING

	LongF		m_nID;
	DoubleF		m_dYield;
	LongF		m_IsHTB;
	LongF		m_nCalcModelType;
	LongF		m_nCalcOptionType;
	LongF		m_nIsBasket;
	LongF		m_nIsActive;
	DoubleF		m_dSkew;
	DoubleF		m_dKurt;
};

struct __IndexDefinitionData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_nStockID)
		FIELD_ENTRY(m_dWeight)
		FIELD_ENTRY(m_dBeta)
	END_PACKING

	LongF		m_nID;
	LongF		m_nStockID;
	DoubleF		m_dWeight;
	DoubleF		m_dBeta;
};

struct __StockData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nID)
		FIELD_ENTRY(m_IsHTB)
		FIELD_ENTRY(m_nPrimaryExchangeID)
		FIELD_ENTRY(m_nDivFreq)
		FIELD_ENTRY(m_dtDivDate)
		FIELD_ENTRY(m_dDivAmt)
		FIELD_ENTRY(m_nIsDivCustom)
		FIELD_ENTRY(m_nDivFreqCustom)
		FIELD_ENTRY(m_dtDivDateCustom)
		FIELD_ENTRY(m_dDivAmtCustom)
		FIELD_ENTRY(m_nCalcModelType)
		FIELD_ENTRY(m_nCalcOptionType)
		FIELD_ENTRY(m_nIsActive)
		FIELD_ENTRY(m_dSkew)
		FIELD_ENTRY(m_dKurt)
	END_PACKING

	LongF		m_nID;
	LongF		m_IsHTB;
	LongF		m_nPrimaryExchangeID;
	LongF		m_nDivFreq;
	DateF		m_dtDivDate;
	DoubleF		m_dDivAmt;
	LongF		m_nIsDivCustom;
	LongF		m_nDivFreqCustom;
	DateF		m_dtDivDateCustom;
	DoubleF		m_dDivAmtCustom;
	LongF		m_nCalcModelType;
	LongF		m_nCalcOptionType;
	LongF		m_nIsActive;
	DoubleF		m_dSkew;
	DoubleF		m_dKurt;
};

struct __ContractInGroupData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nGroupID)
		FIELD_ENTRY(m_nContractID)
	END_PACKING

	LongF		m_nGroupID;
	LongF		m_nContractID;
};

struct __ContractGroupData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nGroupID)
		FIELD_ENTRY(m_GroupName)
		FIELD_ENTRY(m_Description)
	END_PACKING

	LongF		m_nGroupID;
	StringF		m_GroupName;
	StringF		m_Description;
};

struct __ExpiryData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nExpiryID)
		FIELD_ENTRY(m_ExpiryPeriod)
	END_PACKING

	LongF		m_nExpiryID;
	StringF		m_ExpiryPeriod;
};

struct __OptionData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_nOptionRootID)
		FIELD_ENTRY(m_dStrike)
		FIELD_ENTRY(m_nExpiryID)
		FIELD_ENTRY(m_IsCall)
	END_PACKING

	LongF		m_nContractID;
	LongF		m_nOptionRootID;
	DoubleF		m_dStrike;
	LongF		m_nExpiryID;
	LongF		m_IsCall;
};

struct __RMScenarioData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nRMScenarioID)
		FIELD_ENTRY(m_ScenarioName)
		FIELD_ENTRY(m_Description)
		FIELD_ENTRY(m_nPoints1)
		FIELD_ENTRY(m_dStep1)
		FIELD_ENTRY(m_nIsAbs1)
		FIELD_ENTRY(m_nType1)
		FIELD_ENTRY(m_nPoints2)
		FIELD_ENTRY(m_dStep2)
		FIELD_ENTRY(m_nIsAbs2)
		FIELD_ENTRY(m_nType2)
	END_PACKING

	LongF		m_nRMScenarioID;
	StringF		m_ScenarioName;
	StringF		m_Description;
	LongF		m_nPoints1;
	DoubleF		m_dStep1;
	LongF		m_nIsAbs1;
	LongF		m_nType1;
	LongF		m_nPoints2;
	DoubleF		m_dStep2;
	LongF		m_nIsAbs2;
	LongF		m_nType2;
};

struct __ContractData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_nContractTypeID)
		FIELD_ENTRY(m_Symbol)
		FIELD_ENTRY(m_ContractName)
		FIELD_ENTRY(m_nExpCalendarID)
		FIELD_ENTRY(m_dPriceClose)
		FIELD_ENTRY(m_dPriceTheoClose)
	END_PACKING

	LongF		m_nContractID;
	LongF		m_nContractTypeID;
	StringF		m_Symbol;
	StringF		m_ContractName;
	LongF		m_nExpCalendarID;
	DoubleF		m_dPriceClose;
	DoubleF		m_dPriceTheoClose;
};

struct __TraderContractData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES
		
		FIELD_ENTRY(m_nContractID)
	END_PACKING

	LongF		m_nContractID;
};

struct __TradeData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nTradeSeqNum)
		FIELD_ENTRY(m_nTradeID)
		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_IsClose)
		FIELD_ENTRY(m_dtTradeDate)
		FIELD_ENTRY(m_nQuantity)
		FIELD_ENTRY(m_dPrice)
		FIELD_ENTRY(m_IsBuy)
		FIELD_ENTRY(m_nBookID)
		FIELD_ENTRY(m_nStrategyID)
		FIELD_ENTRY(m_nTraderID)
		FIELD_ENTRY(m_nBrokerID)
		FIELD_ENTRY(m_nClearingBrokerID)
		FIELD_ENTRY(m_nBrokerCommissionID)
		FIELD_ENTRY(m_nClearingBrokerCommissionID)
		FIELD_ENTRY(m_dTradedIV)
		FIELD_ENTRY(m_dSpotReference)
		FIELD_ENTRY(m_nIsPosition)
		FIELD_ENTRY(m_enTradeStatus)
	END_PACKING

	LongF		m_nTradeSeqNum;
	LongF		m_nTradeID;
	LongF		m_nContractID;
	LongF		m_IsClose;
	DateF		m_dtTradeDate;
	LongF		m_nQuantity;
	DoubleF		m_dPrice;
	LongF		m_IsBuy;
	LongF		m_nBookID;
	LongF		m_nStrategyID;
	LongF		m_nTraderID;
	LongF		m_nBrokerID;
	LongF		m_nClearingBrokerID;
	LongF		m_nBrokerCommissionID;
	LongF		m_nClearingBrokerCommissionID;
	DoubleF		m_dTradedIV;
	DoubleF		m_dSpotReference;
	LongF		m_nIsPosition;
	TStaticField<TRADE_STATUS>	m_enTradeStatus;
};

struct __IrPointData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nPointID)
		FIELD_ENTRY(m_nCurveID)
		FIELD_ENTRY(m_nPeriodTypeID)
		FIELD_ENTRY(m_nNum)
		FIELD_ENTRY(m_dShortRate)
		FIELD_ENTRY(m_dHTBRate)
		FIELD_ENTRY(m_dLongRate)
	END_PACKING

	LongF	m_nPointID;
	LongF	m_nCurveID;
	LongF	m_nPeriodTypeID;
	LongF	m_nNum;
	DoubleF	m_dShortRate;
	DoubleF	m_dHTBRate;
	DoubleF	m_dLongRate;
};

struct __IrCurveData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nCurveID)
		FIELD_ENTRY(m_CurveName)
		FIELD_ENTRY(m_nCurveTypeID)
		FIELD_ENTRY(m_nRuleTypeID)
		FIELD_ENTRY(m_cPositionThreshold)
	END_PACKING

	LongF		m_nCurveID;
	StringF		m_CurveName;
	LongF		m_nCurveTypeID;
	LongF		m_nRuleTypeID;
	CurrencyF	m_cPositionThreshold;	
};

struct __QuoteSubscriptionData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES
	END_PACKING
};

struct __DataInfoData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nDataInfoID)
		FIELD_ENTRY(m_KeyName)
		FIELD_ENTRY(m_KeyValue)
	END_PACKING

	LongF		m_nDataInfoID;
	StringF		m_KeyName;
	StringF		m_KeyValue;
};
	
struct __VolaSurfaceData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nVolaSurfaceDataID)
		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_dBaseUnderlinePrice)
		FIELD_ENTRY(m_dSmileAccelerator)
		FIELD_ENTRY(m_dInterpolationFactor)
		FIELD_ENTRY(m_dStrikeMoneynessHi)
		FIELD_ENTRY(m_dStrikeMoneynessLo)
		FIELD_ENTRY(m_SurfaceName)
		FIELD_ENTRY(m_nIsPriceOverride)
		FIELD_ENTRY(m_nIsDiscreteAcceleration)
		FIELD_ENTRY(m_nIsDefaultSurface)
		FIELD_ENTRY(m_dATMVolatility)
	END_PACKING

	LongF	m_nVolaSurfaceDataID;
	LongF	m_nContractID;
	DoubleF	m_dBaseUnderlinePrice;
	DoubleF	m_dSmileAccelerator;
	DoubleF	m_dInterpolationFactor;
	DoubleF	m_dStrikeMoneynessHi;
	DoubleF	m_dStrikeMoneynessLo;
	StringF	m_SurfaceName;
	LongF	m_nIsPriceOverride;
	LongF	m_nIsDiscreteAcceleration;
	LongF	m_nIsDefaultSurface;
	DoubleF	m_dATMVolatility;
};

struct __CustomStrikeSkewPointData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nCustomStrikeSkewPointID)
		FIELD_ENTRY(m_nVolaSurfaceDataID)
		FIELD_ENTRY(m_dStrike)
		FIELD_ENTRY(m_nType)
		FIELD_ENTRY(m_dVolatility)
		FIELD_ENTRY(m_nExpiryID)
		FIELD_ENTRY(m_nIsBasePoint)
	END_PACKING

	LongF	m_nCustomStrikeSkewPointID;
	LongF	m_nVolaSurfaceDataID;
	DoubleF	m_dStrike;
	LongF	m_nType;
	DoubleF	m_dVolatility;
	LongF	m_nExpiryID;
	LongF	m_nIsBasePoint;
};

struct __LastQuoteData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES
		
		FIELD_ENTRY(m_Symbol)
		FIELD_ENTRY(m_Type)
		FIELD_ENTRY(m_Exchange)
		FIELD_ENTRY(m_dBidPrice)
		FIELD_ENTRY(m_dAskPrice)
		FIELD_ENTRY(m_dLastPrice)
		FIELD_ENTRY(m_nBidSize)
		FIELD_ENTRY(m_nAskSize)
		FIELD_ENTRY(m_dClosePrice)
		FIELD_ENTRY(m_dLowPrice)
		FIELD_ENTRY(m_dHighPrice)
		FIELD_ENTRY(m_dOpenPrice)
		FIELD_ENTRY(m_dNetChange)
		FIELD_ENTRY(m_nVolume)
		FIELD_ENTRY(m_nOpenInterest)
		FIELD_ENTRY(m_nLotSize)
		FIELD_ENTRY(m_nErrorID);
	END_PACKING

	StringF						m_Symbol;
	TStaticField<SYMBOL_TYPE>	m_Type;
	StringF						m_Exchange;
	DoubleF						m_dBidPrice;
	DoubleF						m_dAskPrice;
	DoubleF						m_dLastPrice;
	LongF						m_nBidSize;
	LongF						m_nAskSize;
	DoubleF						m_dClosePrice;
	DoubleF						m_dLowPrice;
	DoubleF						m_dHighPrice;
	DoubleF						m_dOpenPrice;
	DoubleF						m_dNetChange;
	LongF						m_nVolume;
	LongF						m_nOpenInterest;
	LongF						m_nLotSize;
	LongF						m_nErrorID;
};

struct __HHRequestData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_chUID)
		FIELD_ENTRY(m_chTraderName)
		FIELD_ENTRY(m_nTraderID)
		FIELD_ENTRY(m_dtLastUpdateDate)
		FIELD_ENTRY(m_dtLastUpdateDate2)
		FIELD_ENTRY(m_nType)
		FIELD_ENTRY(m_nParam1)
		FIELD_ENTRY(m_nParam2)
		FIELD_ENTRY(m_StrParam1)
		FIELD_ENTRY(m_StrParam2)
	END_PACKING

	StringF							m_chUID;
	StringF							m_chTraderName;
	LongF							m_nTraderID;
	DateF							m_dtLastUpdateDate;
	DateF							m_dtLastUpdateDate2;
	TStaticField<HH_REQUEST_TYPE>	m_nType;
	LongF							m_nParam1;
	LongF							m_nParam2;
	StringF							m_StrParam1;
	StringF							m_StrParam2;
};

struct __DynamicMessageData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_Value)
	END_PACKING

    MapF<WStringF, VariantF>  m_Value;
};

struct __ExpiryCalendarData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nCalendarID)
		FIELD_ENTRY(m_CalendarName)
		FIELD_ENTRY(m_Description)
	END_PACKING

	LongF		m_nCalendarID;
	StringF		m_CalendarName;
	StringF		m_Description;
};

struct __ExpiryInCalendarData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nExpiryID)
		FIELD_ENTRY(m_nCalendarID)
		FIELD_ENTRY(m_dtExpiryDate)
		FIELD_ENTRY(m_dVegaWeight)
	END_PACKING

	LongF		m_nExpiryID;
	LongF		m_nCalendarID;
	DateF		m_dtExpiryDate;
	DoubleF		m_dVegaWeight;
};

struct __ContractExchangeData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES

		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_nExchangeID)
		FIELD_ENTRY(m_nIsUnderlying)
		FIELD_ENTRY(m_nIsOption)
	END_PACKING

	LongF		m_nContractID;
	LongF		m_nExchangeID;
	LongF		m_nIsUnderlying;
	LongF		m_nIsOption;
};

struct __OptionRootData : public __CommonData
{
	BEGIN_PACKING
		FIELD_COMMON_ENTRIES
		
		FIELD_ENTRY(m_nOptionRootID)
		FIELD_ENTRY(m_nUnderlyingContractID)
		FIELD_ENTRY(m_sSymbol)
		FIELD_ENTRY(m_nLotSize)
		FIELD_ENTRY(m_nIsSynthetic)
	END_PACKING

	LongF							m_nOptionRootID;
	LongF							m_nUnderlyingContractID;
	StringF							m_sSymbol;
	LongF							m_nLotSize;
	LongF							m_nIsSynthetic;
};

#endif