#include "StdAfx.h"

#include "Trace.h"
#include "OmniApiUtils.h"
#include "ISESession.h"
#include "ISEData.h"

/*-------------------------[ Segmented Nonepartitioned Query ]--------------------------*/

template<class _QueryClass, class _AnswerType, uint32 _Facility>
void TSegmNonPartQuery<_QueryClass, _AnswerType, _Facility>::Load(const char* const szDesc,
																  const series_t* const pIseFilter,
																  _QueryClass* pQuery)
{
	_QueryClass					qDQ;
	CISEAnswer					aDQ;
	uint64_t					uiTransactionID;
	uint64_t					uiOrderID;

	IseTrace(enInfo, "[%x] Loading %s...", m_pSession->GetHandle(), szDesc);

	if(pQuery)
		pQuery->segment_number_n = 0;
	else
		qDQ.segment_number_n = 0;

	DWORD dwTotalCount = 0;
	DWORD dwCount;

	try
	{
		bool bContinue;

		do
		{
			if(pQuery)
			{
				pQuery->segment_number_n++;
				m_pSession->SendQuery(*pQuery, m_uiBaseFacility + _Facility, aDQ, 
					uiTransactionID, uiOrderID);
			}
			else
			{
				qDQ.segment_number_n++;
				if(pIseFilter)
					qDQ.series = *pIseFilter;

				m_pSession->SendQuery(qDQ, m_uiBaseFacility + _Facility, aDQ, 
					uiTransactionID, uiOrderID);
			}

			EgAssert(aDQ.m_uiLen <= sizeof(_AnswerType));

			bContinue = OnData((_AnswerType*)&aDQ.m_Data, dwCount);
			dwTotalCount += dwCount;
			IseTrace(enInfo, "[%x] Received %d records.", m_pSession->GetHandle(), dwTotalCount);

		}while(bContinue);
	}
	catch(CISEException& e)
	{
		char szBuf[256];
		sprintf(szBuf, "Failed to load %s", szDesc);
		e.AddDescription(szBuf);
		throw;
	}

	IseTrace(enInfo, "[%x] %s loaded. Total records count = %d.", m_pSession->GetHandle(), szDesc, dwTotalCount);
}

template<class _QueryClass, class _AnswerType, uint32 _Facility>
void TSegmNonPartQuery2<_QueryClass, _AnswerType, _Facility>::Load(const char* const szDesc,
																  const series_t* const pIseFilter,
																  _QueryClass* pQuery)
{
	_QueryClass					qDQ;
	CISEAnswer					aDQ;
	uint64_t					uiTransactionID;
	uint64_t					uiOrderID;

	IseTrace(enInfo, "[%x] Loading %s...", m_pSession->GetHandle(), szDesc);

	if(pQuery)
		pQuery->segment_number_n = 0;
	else
		qDQ.segment_number_n = 0;

	DWORD dwTotalCount = 0;
	DWORD dwCount;

	try
	{
		bool bContinue;

		do
		{
			if(pQuery)
			{
				pQuery->segment_number_n++;
				m_pSession->SendQuery(*pQuery, m_uiBaseFacility + _Facility, aDQ, 
					uiTransactionID, uiOrderID);
			}
			else
			{
				qDQ.segment_number_n++;
				m_pSession->SendQuery(qDQ, m_uiBaseFacility + _Facility, aDQ, 
					uiTransactionID, uiOrderID);
			}

			EgAssert(aDQ.m_uiLen <= sizeof(_AnswerType));

			bContinue = OnData((_AnswerType*)&aDQ.m_Data, dwCount);
			dwTotalCount += dwCount;
			IseTrace(enInfo, "[%x] Received %d records.", m_pSession->GetHandle(), dwTotalCount);

		}while(bContinue);
	}
	catch(CISEException& e)
	{
		char szBuf[256];
		sprintf(szBuf, "Failed to load %s", szDesc);
		e.AddDescription(szBuf);
		throw;
	}

	IseTrace(enInfo, "[%x] %s loaded. Total records count = %d.", m_pSession->GetHandle(), szDesc, dwTotalCount);
}

/*-------------------------[ Load static data ]--------------------------*/

void CISEData::Load(CISESession*	pSession)
{
	m_pSession = pSession;
	m_uiBaseFacility = pSession->m_uiBaseFacility;

	series_t	IseFilter;
	memset(&IseFilter, 0, sizeof(IseFilter));

	try
	{
		//The Query Instrument Group Transaction provides the existing 
		//instrument groups in binary format 
		//and their equivalent character representation.
		//SK. Load instruments groups and thier txt desc(option, futures... - groups)

		CInstrumentGroupData::Load("Instrument Group Data");

		//The Query Instrument Transaction will provide permanent information 
		//for each instrument type.
		//SK. Load market code for groups
		CInstrumentData::Load("Instrument Data");

		//The Query Market Transaction provides the valid markets in binary format 
		//and their equivalent character representation.
		//SK. Load ISE market code, country & desc - Only 1 market is expected
		CMarketData::Load("Market Data");

		//The Query Exchange Transaction provides information on all foreign exchanges, 
		//derivative as well as stock exchanges.
		//SK. Exchange counties & Names
		CExchangeData::Load("Exchange Data");

		//The Query Trading State Transaction provides the requestor with data 
		//regarding the trading states.
		//SK. 
		IseFilter.country_c = m_ISE.m_uiCountry;
		IseFilter.market_c = m_ISE.m_uiMarket;

		CTradingStateData::Load("Trading State Data", &IseFilter);

		//The Query Underlying Transaction provides the underlying 
		//identity character format for the specified series.
		//SK. Query list of underlyings for ISE
		CUnderlyingData::Load("Underlying Data", &IseFilter);

		// CSyntheticUnderlyingData - NOT_IMPL 

		//The Query Instrument Class Transaction provides all existing instrument 
		//classes.
		//SK. 
		CInstrumentClassData::Load("Instrument Class Data");

		// check if we have instrument classes for all underlying.
		// also check internal consistency of info we have
		m_ISE.UpdateInstrumentClasses(this);

		//The Query Series Transaction provides information on tradable series.
		//SK.
		CSeriesData::Load("Series Data", &IseFilter);

		// Update dependencies btw stocks & derivatives
		m_ISE.UpdateMarketStructure(this);

		//The Query Brokers Signature Transaction provides the trading identity of each 
		//user within your customer organization.
		//SK.

		CISEQueryBrokers	BrokersQuery;

		S2CS(m_pSession->m_User.m_sCustomerID, BrokersQuery.ex_customer_s);
		S2CS(m_pSession->m_User.m_sCountryID, BrokersQuery.country_id_s);

		CBrokersData::Load("Brokers Data", NULL, &BrokersQuery);

		//The Query Bin Transaction provides a customer with information on which 
		//market makers are responsible for which bins.
		//SK.
		CMmBinData::Load("MM/BIN Data");

		//The Query Underlying Trading Status Transaction provides the trading status of the
		//underlying market.
		//SK.
		CUnderlyingTradingStatusData::Load("Underlying Trading Status Data");

		//The Query Instrument Status Transaction is used to obtain the market status.
		//SK.
		CInstrumentTradingStatusData::Load("Instrument Trading Status Data");

		//The purpose of this query is to retrieve all General Message (BI8) broadcasts 
		//that have been sent on the current business date.
		//SK.
		CGeneralMessageData::Load("General Message Data", &IseFilter);

		// Find default bin
		m_ISE.UpdateDefaultBin(this);
	}
	catch(CISEException& e)
	{
		e.AddDescription("Failed to load static data");
		throw;
	}

	IseTrace(enInfo, "Static data loaded.");

	CAutoLock Lock(this);
	m_bLoaded = true;
}


bool CISEData::OnData(const answer_instrument_group_da208_t* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	CAutoLock   GroupLock(&m_ISE.m_Instruments);

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da208_t& Item = pData->item[nIndex].da208;
		
		CInstrument	Instrument(Item.instrument_group_c);
		CS2S(Item.name_s, Instrument.m_sName);

		m_ISE.m_Instruments.Add(Instrument);
	}

	return (nItems != 0) && (pData->segment_number_n != 0);
};

bool CISEData::OnData(const answer_instrument_da203_t* pData, DWORD& dwCount)
{
	const uint8 nItems = pData->items_c;
	dwCount = nItems;

	CAutoLock   GroupLock(&m_ISE.m_Instruments);

	for(uint8	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da203_t& Item = pData->item[nIndex].da203;
		
		CInstrument* Instrument = m_ISE.m_Instruments.FindByGroup(Item.series.instrument_group_c);

		if(Instrument)
		{
			Instrument->m_uiMarket = Item.series.market_c;
			Instrument->m_iBBOVolume = Item.bbo_volume_i;
			Instrument->m_iBlockOrderMinSize = Item.block_order_min_size_i;
			Instrument->m_iBlockOrderResponceTime = Item.block_order_rsptime_i;
		}
		else
		{
			IseTrace(enWarning, "[%x] No description found for instrument group = %d.",
				m_pSession->GetHandle(), Item.series.instrument_group_c);
		}
	}

	return (nItems != 0) && (pData->segment_number_n != 0);
}

bool CISEData::OnData(const answer_market_da207_t* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	if(nItems != 1)
		IseTrace(enWarning, "[%x] Expected 1 market, found : %d", m_pSession->GetHandle(), nItems);

	EgAssert(nItems != 0);

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da207_t& Item = pData->item[nIndex].da207;
		m_ISE.m_uiMarket = Item.market_c;
		m_ISE.m_uiCountry = Item.country_c;
		CS2S(Item.name_s, m_ISE.m_sName);

		IseTrace(enInfo, "[%x] Market found : %s.", m_pSession->GetHandle(), m_ISE.m_sName.c_str());
	}

	IseTrace(enInfo, "[%x] Our market is : %s.", m_pSession->GetHandle(), m_ISE.m_sName.c_str());

	return (nItems != 0) && (pData->segment_number_n != 0);
}

bool CISEData::OnData(const answer_exchange_da24* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da24_t& Item = pData->item[nIndex].da24;
		
		CExchange Exchange(Item.country_c);
		Exchange.m_cOpraIndicator = Item.opra_indicator_c;
		CS2S(Item.name_s, Exchange.m_sName);
		string sExhangeName;
		CS2S(Item.exchange_short_s, sExhangeName);
		//Exchange.m_sShortName = m_mapExchangeCodes.GetExchangeCode(sExhangeName);
		Exchange.m_sShortName = ISE_EXCHANGE_CODE;
		CS2S(Item.country_id_s, Exchange.m_sCountryID);

		m_Exchanges.Add(Exchange);
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
}

bool CISEData::OnData(const answer_trading_state_da245_t* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const answer_trading_state_da245_item_t& Item = pData->item[nIndex];

		CTradingState	TradingState(Item.state_number_n);
		CS2S(Item.state_name_s, TradingState.m_sName);
		TradingState.m_uiFastMarketLevel = Item.fast_market_level_n;
		TradingState.m_cMatchingType = Item.type_of_matching_c;
		TradingState.m_cEndTrading = Item.trading_end_c;
		TradingState.m_cPriceQuotationReq = Item.price_quotation_required_c;
		TradingState.m_cMarketOrdersAllowed = Item.market_orders_allowed_c;

#ifdef OMEXVER16_5
		TradingState.m_cFillKillAllowed = Item.fill_kill_allowed_c;
#else
		TradingState.m_cFillKillAllowed = Item.fill_or_kill_allowed_c;
#endif

		TradingState.m_cEditedOrderBookChangesAvail = Item.edited_ob_changes_avail_c;
		TradingState.m_cOrderBookChangesAvail = Item.ob_changes_avail_c;

		m_TradingStates.Add(TradingState);
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
}

bool CISEData::OnData(const answer_underlying_da204* pData, DWORD& dwCount)
{
	const uint8 nItems = pData->items_c;
	dwCount = nItems;

	for(uint8	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da204_t& Item = pData->item[nIndex].da204;

		AddUnderlying(Item);
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
};

bool CISEData::OnData(const answer_instrument_class_da210* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da210_t& Item = pData->item[nIndex].da210;

		AddInstrumentClass(Item);
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
}

bool CISEData::OnData(const answer_series_da202* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da202_t& Item = pData->item[nIndex].da202;

		AddSeries(Item);
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
}

bool CISEData::OnData(const answer_broker_da206* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da206_t& Item = pData->item[nIndex].da206;
		
		// TO DO. What to do here?
		
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
};

bool CISEData::OnData(const answer_bin_da211* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const da211_t& Item = pData->item[nIndex].da211;

		CBin *pBin = m_ISE.m_Bins.FindByID(Item.bin_c);
		if(pBin == NULL)
		{
			CBin NewBin;
			NewBin.m_uiID = Item.bin_c;
			m_ISE.m_Bins.Add(NewBin);
			pBin = m_ISE.m_Bins.FindByID(Item.bin_c);
		}

		if(Item.cst_type_c == MM_PRIMARY)
		{
			CS2S(Item.ex_customer_s, pBin->m_sPMM);
		}
		else
		{
			string sCMM;
			CS2S(Item.ex_customer_s, sCMM);
			pBin->m_vecCMMs.push_back(sCMM);
		}
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
};

bool CISEData::OnData(const answer_underlying_status_us* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const answer_underlying_status_us_item_t& Item = pData->item[nIndex];

		CUnderlying* pUnderlying = m_ISE.m_Underlyings.FindByCommodity(Item.commodity_n);
		if(pUnderlying)
		{
			pUnderlying->m_uiTradingStatus = Item.underlying_trading_status_c;
		}
		else
		{
			IseTrace(enWarning, "[%x] Failed to find underlying to set trading status, commodity = %u",
				m_pSession->GetHandle(), Item.commodity_n);
		}
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
};

bool CISEData::OnData(const answer_instrument_status* pData, DWORD& dwCount)
{
	const uint16 nItems = pData->items_n;
	dwCount = nItems;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const answer_instrument_status_item_t& Item = pData->item[nIndex];
		
		ModifyInstrumentStatus(Item.series, Item.state_number_n);
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
};

bool CISEData::OnData(const answer_bi8_broadcasts_sent* pData, DWORD& dwCount)
{
	const uint8 nItems = pData->items_c;
	dwCount = nItems;

	for(uint8	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const answer_bi8_broadcasts_sent_item1_t& Item = pData->item1[nIndex];

		for(uint16 i = 0; i < Item.items_n; i++)
		{
			string sText;
			CS2S(Item.item2[i].free_text_80_s, sText);
			AddGeneralMessage(Item.country_c, Item.market_c, sText);
		}
	}
	return (nItems != 0) && (pData->segment_number_n != 0);
};

//------------------------------------[ISE Message]----------------------------------------

void CISEData::AddGeneralMessage(const uint8 uiCountry, const uint8 uiMarket, const string& sText)
{
	IseTrace(enISE, sText.c_str());
}

//------------------------------------[Underlying]----------------------------------------

void CISEData::AddUnderlying(const da204_t& Data)
{
	CUnderlying	Und(Data.commodity_n);

	UpdateUnderlying(Data, Und);

	{
		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		m_ISE.m_Underlyings.Add(Und);
	}
};

void CISEData::ModifyUnderlying(const da204_t& Data)
{
	CAutoLock	UndLock(&m_ISE.m_Underlyings);

	CUnderlying* pUnd = m_ISE.m_Underlyings.FindByCommodity(Data.commodity_n);

	if(pUnd) UpdateUnderlying(Data, *pUnd);
};

void CISEData::DeleteUnderlying(const da204_t& Data)
{
	CAutoLock	UndLock(&m_ISE.m_Underlyings);

	m_ISE.m_Underlyings.DeleteByCommodity(Data.commodity_n);
};


//
// Updates underlyings data, not update m_uiCommodity
//
void CISEData::UpdateUnderlying(const da204_t& Data, CUnderlying& Underlying)
{
	CS2S(Data.com_id_s, Underlying.m_sSymbol);
	EgAssert(Underlying.m_sSymbol.length() != 0);

	Underlying.m_uqNominalValue = Data.nominal_value_q;
	Underlying.m_uqPositionLimit = Data.position_limit_q;
	Underlying.m_uiCouponInterest = Data.coupon_interest_i;
	
	/*
	Underlying.m_uiCommodity = Data.commodity_n;
	EgAssert(Underlying.m_uiCommodity != 0);
	*/

	Underlying.m_uiDaysCount = Data.day_count_n;
	Underlying.m_uiDaysInInterestYear = Data.days_in_interest_year_n;
	Underlying.m_uiSettlementDaysAtCoupon = Data.coupon_settlement_days_n;
	Underlying.m_uiDecInPrice = Data.dec_in_price_n;
	Underlying.m_uiCountry = Data.prm_country_c;
	
	Underlying.m_uiBin = Data.bin_c;
	EgAssert(Underlying.m_uiBin != 0);
	if(m_ISE.m_uiBinsCount < Underlying.m_uiBin)
		m_ISE.m_uiBinsCount = Underlying.m_uiBin;

	Underlying.m_uiOrderbook = Data.orderbook_c;
	EgAssert(Underlying.m_uiOrderbook != 0);
	if(m_ISE.m_uiOrderBookCount < Underlying.m_uiOrderbook)
		m_ISE.m_uiOrderBookCount = Underlying.m_uiOrderbook;

	Underlying.m_uiType = Data.underlying_type_c;
#ifdef _DEBUG
	if(Underlying.m_uiType != UT_STOCK)
	{
		OutputDebugString(Underlying.m_sSymbol.c_str());
		OutputDebugString(" ==== INDEX \n");
	}
#endif
	Underlying.m_uiPriceUnit = Data.price_unit_c;
	CS2S(Data.cusip_s, Underlying.m_sCusip);
	CS2S(Data.name_s, Underlying.m_sFullname);
	CS2S(Data.date_release_s, Underlying.m_sReleaseDate);
	CS2S(Data.date_termination_s, Underlying.m_sTerminationDate);
	CS2S(Data.base_cur_s, Underlying.m_sCurrency);
	CS2S(Data.prm_mm_customer_s, Underlying.m_sPrmMmCustomer);

	uint8 nIndex;

	Underlying.m_vecCoupons.clear();
	for(nIndex = 0; nIndex < Data.items_c; nIndex++)
	{
		CCoupon	Coupon;
		CS2S(Data.coupon[nIndex].date_coupdiv_s, Coupon.m_sDate);
		Coupon.m_uiDividend = Data.coupon[nIndex].dividend_i;
		Underlying.m_vecCoupons.push_back(Coupon);
	}

	Underlying.m_vecFastMarketLevels.clear();
	for(nIndex = 0; nIndex < Data.items2_c; nIndex++)
	{
		CFastMarketLevel	Level;
		Level.m_uiLevel = Data.fast_market_lvl[nIndex].level_n;
		Level.m_uiMatchInterval = Data.fast_market_lvl[nIndex].match_interval_i;
		Underlying.m_vecFastMarketLevels.push_back(Level);
	}
};

//------------------------------------[Class]----------------------------------------

void CISEData::AddInstrumentClass(const da210_t& Data)
{
	CInstrumentClass	InstrumentClass;

	UpdateInstrumentClass(Data, InstrumentClass);

	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);

	m_ISE.m_InstrumentClasses.Add(InstrumentClass);
};

void CISEData::DeleteInstrumentClass(const da210_t& Data)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);

	m_ISE.m_InstrumentClasses.DeleteBySeries(Data.series);
};

void CISEData::ModifyInstrumentClass(const da210_t& Data)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);

	CInstrumentClass* pInstrumentClass = m_ISE.m_InstrumentClasses.FindBySeries(Data.series);
	
	if(pInstrumentClass)
		UpdateInstrumentClass(Data, *pInstrumentClass);
};

void CISEData::UpdateInstrumentClass(const da210_t& Data, CInstrumentClass& InstrumentClass)
{
	InstrumentClass.m_Series = Data.series;
	InstrumentClass.m_ParentSeries = Data.upper_level_series;
	InstrumentClass.m_uiDecInStrikePrice = Data.dec_in_strike_price_n;
	CS2S(Data.base_cur_s, InstrumentClass.m_sCurrency);
	
	InstrumentClass.m_iContractSize = Data.contract_size_i;
	InstrumentClass.m_uiDecInContractSize = Data.dec_in_contr_size_n;


	InstrumentClass.m_iBlockOrderMinSize = Data.block_order_min_size_i;
	InstrumentClass.m_iBlockOrderResponceTime = Data.block_order_rsptime_i;

#ifdef OMEXVER16_5	
	InstrumentClass.m_iFacilitationOrderMinSize = Data.fac_order_min_size_i;
	InstrumentClass.m_iFacilitationOrderResponceTime = Data.fac_order_rsptime_i;
#endif

	InstrumentClass.m_iExercizeLimit = Data.exerc_limit_i;
	InstrumentClass.m_uiExercizeLimitUnit = Data.exerc_limit_unit_c;
	InstrumentClass.m_iBboVolume = Data.bbo_volume_i;

	InstrumentClass.m_iSpg1LowLimit = Data.spg1_low_limit_n;
	InstrumentClass.m_iSpg2LowLimit = Data.spg2_low_limit_n;
	InstrumentClass.m_iSpg3LowLimit = Data.spg3_low_limit_n;
	InstrumentClass.m_iSpg4LowLimit = Data.spg4_low_limit_n;
	InstrumentClass.m_iSpg5LowLimit = Data.spg5_low_limit_n;

	InstrumentClass.m_cTraded = Data.traded_c;
	InstrumentClass.m_uiDeriveLevel = Data.derivate_level_n;

	uint16	uiIndex;

	InstrumentClass.m_vecLowLimits.clear();
	for(uiIndex = 0; uiIndex < Data.items2_c; uiIndex++)
	{
		InstrumentClass.m_vecLowLimits.push_back(Data.exp_date_group[uiIndex].edg_low_limit_n);
	}

	InstrumentClass.m_uiRanking = Data.rnt_id_n;
	InstrumentClass.m_uiDecInPremium = Data.dec_in_premium_n;
	InstrumentClass.m_iPriceQuoteFactor = Data.price_quot_factor_i;

	InstrumentClass.m_TickSizes.clear();
	for(uiIndex = 0; uiIndex < Data.items_n; uiIndex++)
	{
		InstrumentClass.m_TickSizes.push_back(Data.item[uiIndex].tick_size);
	}

	InstrumentClass.m_MmTickSizes.clear();
	for(uiIndex = 0; uiIndex < Data.items_c; uiIndex++)
	{
		InstrumentClass.m_MmTickSizes.push_back(Data.tss_mm[uiIndex].tick_size);
	}

	InstrumentClass.m_bTradesInDecimals = TRUE;

	if(InstrumentClass.m_TickSizes.size() >= 2)
	{
		if(InstrumentClass.m_TickSizes[0].step_size_i == 625 &&
			InstrumentClass.m_TickSizes[0].step_size_i == 1250)
		{
			InstrumentClass.m_bTradesInDecimals = FALSE;
		}
	}
}

//------------------------------------[Series]----------------------------------------

void CISEData::AddSeries(const da202_t& Data)
{
	CAutoLock ClassesLock(&m_ISE.m_InstrumentClasses);

	CSeries	Series(Data.series);

	UpdateSeries(Data, Series);

	{
		CAutoLock	SeriesLock(&m_ISE.m_Series);
		m_ISE.m_Series.Add(Series);
	}
}

void CISEData::DeleteSeries(const da202_t& Data)
{
	CAutoLock	SeriesLock(&m_ISE.m_Series);
		
	m_ISE.m_Series.DeleteBySeries(Data.series);
}

void CISEData::ModifySeries(const da202_t& Data)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	SeriesLock(&m_ISE.m_Series);
		
	CSeries*	pSeries = m_ISE.m_Series.FindBySeries(Data.series);

	if(pSeries)
	{
		UpdateSeries(Data, *pSeries);
	}
	else
	{
		//ISE TECHNICAL BULLETIN 22 April 2002
        //When developing member applications, it is necessary to handle this case where the BI202 has 
		//a change-type of "modified" but the application has not yet seen that series and 
		//it must be treated as if it were a new series .
		AddSeries(Data);
	}
}

//
// Update series data, without series (key)
//
void CISEData::UpdateSeries(const da202_t& Data, CSeries& Series)
{
	/*
	Series.m_Series = Data.series;
	*/
	CS2S(Data.ins_id_s, Series.m_sFullname);
	CS2S(Data.symbol_s, Series.m_sRoot);
	Series.m_iContractSize = Data.contract_size_i;
	Series.m_cStrikeCode = Data.strike_code_c;
	Series.m_cMonthCode = Data.month_code_c;
	
	{
		CAutoLock  GroupLock(&m_ISE.m_Instruments);
		Series.m_pInstrument = m_ISE.m_Instruments.FindByGroup(Series.m_Series.instrument_group_c);
	}

	Series.m_pInstrumentClass = m_ISE.m_InstrumentClasses.FindBySeries(Series.m_Series);

	if(Series.m_pInstrumentClass == NULL)
	{
		IseTrace(enWarning, "[%x] No instrument classes found for series %s", 
			m_pSession->GetHandle(), Series.m_sSymbol.c_str());

		Series.m_dStrikePrice = 0;
	}
	else
	{
		Series.m_dStrikePrice = DEC2DBL(Series.m_Series.strike_price_i, 
			Series.m_pInstrumentClass->m_uiDecInPremium);
	}

	// build symbol name

	// 1 variant
	//Series.m_sSymbol = Series.m_sFullname;

	// 2 variant
	Series.m_sSymbol = Series.m_sRoot + Series.m_cMonthCode + Series.m_cStrikeCode;

/*
	// 3 variant
	Series.m_sSymbol = Series.m_sRoot;
	uint16 uiYear;
	uint8 uiMonth;
	uint8 uiDay;
	SplitIntDate(Data.series.expiration_date_n, &uiYear, &uiMonth, &uiDay);

	struct tm YearMonth;
	YearMonth.tm_year = uiYear - 1900;
	YearMonth.tm_mon = uiMonth - 1;

	char szBuf[256];
	strftime(szBuf, sizeof(szBuf), "%y%b", &YearMonth);
	strupr(szBuf);
	Series.m_sSymbol += szBuf;

	sprintf(szBuf, "%d", (int)Series.m_dStrikePrice);
	Series.m_sSymbol += szBuf;

	switch(Series.m_Series.instrument_group_c)
	{
		case IG_EUROPEAN_CALL_OPTION:
		case IG_AMERICAN_CALL_OPTION:
			Series.m_sSymbol += 'C';
			break;

		case IG_EUROPEAN_PUT_OPTION:
		case IG_AMERICAN_PUT_OPTION:
			Series.m_sSymbol += 'P';
			break;

		default:
			IseTrace(enWarning, "[%x] Invalid instrument group for series %s = %d", 
				m_pSession->GetHandle(), Series.m_sSymbol.c_str(), Series.m_Series.instrument_group_c);
			break;
	}
*/
}

//------------------------------[Status]---------------------------------------

void CISEData::ModifyInstrumentStatus(const series_t& Data, const uint16 uiState,
		const uint16 uiTimeout, const char* const szText)
{
	if(uiTimeout != 0)
	{
		CAutoLock  SeriesLock(&m_ISE.m_Series);

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(Data);
		if(pSeries)
		{
			IseTrace(enISE, "Trading status for '%s' will change in %d minutes, %s",
				pSeries->m_sSymbol.c_str(), uiTimeout, szText);
		}

		return;
	}

	if(Data.commodity_n == 0)
	{
		if(Data.instrument_group_c == 0)
		{
			if(	Data.country_c == m_ISE.m_uiCountry &&
				Data.market_c == m_ISE.m_uiMarket)
			{
				m_ISE.m_uiTradingState = uiState;
			}
		}
		else
		{
			CAutoLock  GroupLock(&m_ISE.m_Instruments);
			CInstrument* pInstrument = m_ISE.m_Instruments.FindByGroup(Data.instrument_group_c);
			if(pInstrument)
				pInstrument->m_uiTradingState = uiState;
		}
	}
	else
	{
		CAutoLock  ClassesLock(&m_ISE.m_InstrumentClasses);

		CInstrumentClass* pInstrumentClass = m_ISE.m_InstrumentClasses.FindBySeries(Data);
		if(pInstrumentClass)
		{
			pInstrumentClass->m_uiTradingState = uiState;
		}
	}
}

void  CISEData::ModifySeriesStatus(const series_status_change_bi103_item_t& Data)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);
	CAutoLock	SeriesLock(&m_ISE.m_Series);

	if(Data.series.strike_price_i == 0)
	{
		// change status for all series of underlying
		CUnderlying* pUnd = m_ISE.m_Underlyings.FindByCommodity(Data.series.commodity_n);
		if(pUnd)
		{
			for(vector<CSeriesDatabase::itSeriesT>::iterator itSeries = pUnd->m_Series.begin();
				itSeries != pUnd->m_Series.end(); itSeries++)
			{
				CSeriesDatabase::itSeriesT	itSeriesObject = *itSeries;
				CSeries& Series = *itSeriesObject;
				Series.m_uiTradingStatus = Data.series_status_c;
				Series.m_uiFastMarketLevel = Data.fast_market_level_n;
				
				FireBBO(&Series);
			}
		}
	}
	else
	{
		// change status of one series

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(Data.series);
		if(pSeries)
		{
			pSeries->m_uiTradingStatus = Data.series_status_c;
			pSeries->m_uiFastMarketLevel = Data.fast_market_level_n;
			pSeries->m_iOpeningPrice= Data.opening_price_i;
			pSeries->m_uiVolume= Data.volume_u;
			FireBBO(pSeries);
		}
	}
};

void  CISEData::ModifyUnderlyingStatus(const underlying_trading_status_bi132_t& Data)
{
	CAutoLock  UndLock(&m_ISE.m_Underlyings);

	CUnderlying* pUnd = m_ISE.m_Underlyings.FindByCommodity(Data.commodity_n);
	if(pUnd)
	{
		pUnd->m_uiTradingStatus = Data.underlying_trading_status_c;

		FireBBO(pUnd);
	}
}

//------------------------------------[Utils]----------------------------------------

CUnderlying* CISEData::GetUnderlyingByRequest(const CSymbolParams& Req)
{
	return m_ISE.m_Underlyings.FindBySymbol(Req.m_sSymbol);
}

CUnderlying* CISEData::GetUnderlyingByRequest(const string& sReq)
{
	return m_ISE.m_Underlyings.FindBySymbol(sReq);
}


CSeries* CISEData::GetSeriesByRequest(const CSymbolParams& Req)
{
	return m_ISE.m_Series.FindBySymbol(Req.m_sSymbol);
}

CSeries* CISEData::GetSeriesByRequest(const string& sReq)
{
	return m_ISE.m_Series.FindBySymbol(sReq);
}

