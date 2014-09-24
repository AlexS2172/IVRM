#include "stdafx.h"

#include "ISESession.h"
#include "OmniApiUtils.h"

/*--------------------------------[ Ready - to - trade ]--------------------------------------------*/

void CISESession::ReadyToTrade()
{
	CISEReadyToTrade	Msg;

	Msg.application_status_i = 1;

	uint64_t uiTransactionID;
	uint64_t uiOrderID;

	SendTx(Msg, m_uiBaseFacility, uiTransactionID, uiOrderID);
}

/*--------------------------------[ MM parameters ]--------------------------------------------*/

void CISESession::SetMMParameters(	// for underlying or class
									const unsigned short uiCommodity,
									const unsigned char  uiInstrumentGroup,

									const unsigned char  uiExpirationGroup,
									const unsigned char  uiStrikePriceGroup,

									// CMM parameters
									const unsigned short *puiTickWorseVolume,

									const unsigned short uiStepUpAfterRegenBuffer,

									const unsigned short uiIseMMTradeLimitAbsolute,
 									const unsigned short uiFirmTradeLimitAbsolute,
									const unsigned short uiFarMMTradeLimitAbsolute,
									
									const unsigned char  uiIseMmTradeLimitFraction,
									const unsigned char  uiFirmTradeLimitFraction,
									const unsigned char  uiFarMmTradeLimitFraction,

									// PMM parameters
									const unsigned short *puiDerivedOrderMaxSize,
									const unsigned short *puiMatchAwayMarketMaxSize,

									//array
									vector<set_pmm_parameters_ui102_item_t>& vecParams
								) 
{
	if(uiExpirationGroup == 0)
	{
		for(unsigned char uiExpGroupIt = 1; uiExpGroupIt <= 3; uiExpGroupIt++)
		{
			SetMMParameters(uiCommodity,
							uiInstrumentGroup,
							uiExpGroupIt,
							uiStrikePriceGroup,
							puiTickWorseVolume,
							uiStepUpAfterRegenBuffer,
							uiIseMMTradeLimitAbsolute,
							uiFirmTradeLimitAbsolute,
							uiFarMMTradeLimitAbsolute,
							uiIseMmTradeLimitFraction,
							uiFirmTradeLimitFraction,
							uiFarMmTradeLimitFraction,
							puiDerivedOrderMaxSize,
							puiMatchAwayMarketMaxSize,
							vecParams);
		}
		return;
	}

	if(uiStrikePriceGroup == 0)
	{
		for(unsigned char uiStrikeGroupIt = 1; uiStrikeGroupIt <= 5; uiStrikeGroupIt++)
		{
			SetMMParameters(uiCommodity,
							uiInstrumentGroup,
							uiExpirationGroup,
							uiStrikeGroupIt,
							puiTickWorseVolume,
							uiStepUpAfterRegenBuffer,
							uiIseMMTradeLimitAbsolute,
							uiFirmTradeLimitAbsolute,
							uiFarMMTradeLimitAbsolute,
							uiIseMmTradeLimitFraction,
							uiFirmTradeLimitFraction,
							uiFarMmTradeLimitFraction,
							puiDerivedOrderMaxSize,
							puiMatchAwayMarketMaxSize,
							vecParams);
		}
		return;
	}

	set_pmm_parameters_ui102_item_t	Item;

	Item.series.country_c = m_ISE.m_uiCountry;
	Item.series.market_c = m_ISE.m_uiMarket;
	Item.series.instrument_group_c = uiInstrumentGroup;
	Item.series.commodity_n = uiCommodity;
	Item.series.expiration_date_n = 0;
	Item.series.modifier_c = 0;
	Item.series.strike_price_i = 0;

	Item.expiration_group_c = uiExpirationGroup;
	Item.strike_price_group_c = uiStrikePriceGroup;

	memcpy(&Item.mm_parameters.tick_worse_volume_an, puiTickWorseVolume,
			sizeof(Item.mm_parameters.tick_worse_volume_an));

	Item.mm_parameters.isemm_trade_limit_absolute_n = uiIseMMTradeLimitAbsolute;
	Item.mm_parameters.firm_trade_limit_absolute_n  = uiFirmTradeLimitAbsolute;
	Item.mm_parameters.farmm_trade_limit_absolute_n = uiFarMMTradeLimitAbsolute;
	Item.mm_parameters.step_up_after_regen_buffer_n = uiStepUpAfterRegenBuffer;
	Item.mm_parameters.isemm_trade_limit_fraction_c = uiIseMmTradeLimitFraction;
	Item.mm_parameters.firm_trade_limit_fraction_c  = uiFirmTradeLimitFraction;
	Item.mm_parameters.farmm_trade_limit_fraction_c = uiFarMmTradeLimitFraction;

	memcpy(&Item.pmm_parameters.derived_order_max_size_an, puiDerivedOrderMaxSize, 
		sizeof(Item.pmm_parameters.derived_order_max_size_an));

	memcpy(&Item.pmm_parameters.match_away_market_max_size_an, puiMatchAwayMarketMaxSize, 
		sizeof(Item.pmm_parameters.match_away_market_max_size_an));

	vecParams.push_back(Item);
}


void CISESession::SetMMParameters(	// role
									bool bPmm,
									// for underlying or class
									const char* const	 szUnd,
									const unsigned char  uiInstrumentGroup,

									const unsigned char  uiExpirationGroup,
									const unsigned char  uiStrikePriceGroup,

									// CMM parameters
									const unsigned short *puiTickWorseVolume,

									const unsigned short uiStepUpAfterRegenBuffer,

									const unsigned short uiIseMMTradeLimitAbsolute,
 									const unsigned short uiFirmTradeLimitAbsolute,
									const unsigned short uiFarMMTradeLimitAbsolute,
									
									const unsigned char  uiIseMmTradeLimitFraction,
									const unsigned char  uiFirmTradeLimitFraction,
									const unsigned char  uiFarMmTradeLimitFraction,

									// PMM parameters
									const unsigned short *puiDerivedOrderMaxSize,
									const unsigned short *puiMatchAwayMarketMaxSize
								) 
{
	vector<set_pmm_parameters_ui102_item_t>	vecParams;

	CISESetPMMParameters	PmmRequest;
	CISESetCMMParameters	CmmRequest;

	{
		CAutoLock DataLock(this);

		if(IsLoaded() == false)
			throw CISEException(0, 0, this, "Statis data is not ready to use.");

		CAutoLock UndLock(&m_ISE.m_Underlyings);

		if(szUnd == NULL || *szUnd == 0)
		{
			// for all underlyings in MM bin
			for(set<CUnderlying>::iterator itUnd = m_ISE.m_Underlyings.m_setData.begin();
				itUnd != m_ISE.m_Underlyings.m_setData.end(); itUnd++)
			{
				const CUnderlying& Und = *itUnd;
				if(Und.m_uiBin == m_ISE.m_uiDefaultBin)
				{
					const unsigned short uiCommodity = Und.m_uiCommodity;
					
					m_ISE.m_Underlyings.Unlock();

					SetMMParameters(uiCommodity,
									uiInstrumentGroup,
									uiExpirationGroup,
									uiStrikePriceGroup,
									puiTickWorseVolume,
									uiStepUpAfterRegenBuffer,
									uiIseMMTradeLimitAbsolute,
									uiFirmTradeLimitAbsolute,
									uiFarMMTradeLimitAbsolute,
									uiIseMmTradeLimitFraction,
									uiFirmTradeLimitFraction,
									uiFarMmTradeLimitFraction,
									puiDerivedOrderMaxSize,
									puiMatchAwayMarketMaxSize,
									vecParams);

					m_ISE.m_Underlyings.Lock();
				}
			}
		}
		else
		{
			CUnderlying* pUnd = m_ISE.m_Underlyings.FindBySymbol(szUnd);
			if(pUnd)
			{
				const unsigned short uiCommodity = pUnd->m_uiCommodity;

				m_ISE.m_Underlyings.Unlock();

				SetMMParameters(uiCommodity,
								uiInstrumentGroup,
								uiExpirationGroup,
								uiStrikePriceGroup,
								puiTickWorseVolume,
								uiStepUpAfterRegenBuffer,
								uiIseMMTradeLimitAbsolute,
								uiFirmTradeLimitAbsolute,
								uiFarMMTradeLimitAbsolute,
								uiIseMmTradeLimitFraction,
								uiFirmTradeLimitFraction,
								uiFarMmTradeLimitFraction,
								puiDerivedOrderMaxSize,
								puiMatchAwayMarketMaxSize,
								vecParams);

				m_ISE.m_Underlyings.Lock();
			}
		}

		if(vecParams.empty())
		{
			throw CISEException(OMNIAPI_INVARG, 0, this, "Failed to find underlying symbol to set parameters for.");
		}
	}

	uint64_t uiTransactionID;
	uint64_t uiOrderID;
	int iRequestNum = 0;

	if(bPmm)
	{
		const int iMaxParams = sizeof(PmmRequest.item) / sizeof(set_pmm_parameters_ui102_item_t);
		for(int iIndex = 0; iIndex < vecParams.size(); iIndex++)
		{
			if(iIndex && (iIndex % iMaxParams == 0))
			{
				PmmRequest.items_n = iMaxParams;
				PmmRequest.series = PmmRequest.item[0].series;
				SendTx(PmmRequest, m_uiBaseFacility + FACILITY_EP0, uiTransactionID, uiOrderID);
				iRequestNum++;
			}

			PmmRequest.item[iIndex - iMaxParams * iRequestNum] = vecParams[iIndex];
		}

		PmmRequest.items_n = iIndex - iMaxParams * iRequestNum;
		PmmRequest.series = PmmRequest.item[0].series;
		SendTx(PmmRequest, m_uiBaseFacility + FACILITY_EP0, uiTransactionID, uiOrderID);
	}
	else
	{
		const int iMaxParams = sizeof(CmmRequest.item) / sizeof(set_mm_parameters_ui101_item_t);
		for(int iIndex = 0; iIndex < vecParams.size(); iIndex++)
		{
			if(iIndex && (iIndex % iMaxParams == 0))
			{
				CmmRequest.items_n = iMaxParams;
				CmmRequest.series = CmmRequest.item[0].series;
				SendTx(CmmRequest, m_uiBaseFacility + FACILITY_EP0, uiTransactionID, uiOrderID);
				iRequestNum++;
			}

			CmmRequest.item[iIndex - iMaxParams * iRequestNum].series = vecParams[iIndex].series;
			CmmRequest.item[iIndex - iMaxParams * iRequestNum].mm_parameters = vecParams[iIndex].mm_parameters;
			CmmRequest.item[iIndex - iMaxParams * iRequestNum].expiration_group_c = vecParams[iIndex].expiration_group_c;
			CmmRequest.item[iIndex - iMaxParams * iRequestNum].strike_price_group_c = vecParams[iIndex].strike_price_group_c;
		}

		CmmRequest.items_n = iIndex - iMaxParams * iRequestNum;
		CmmRequest.series = CmmRequest.item[0].series;
		SendTx(CmmRequest, m_uiBaseFacility + FACILITY_EP0, uiTransactionID, uiOrderID);
	}
}

/*--------------------------------[ Submit & Delete quotes ]--------------------------------------------*/

void CISESession::SubmitQuotes(CISESubmitQuote& Request, string& sOrderID)
{
	uint64_t uiTransactionID;
	uint64_t uiOrderID;

	//IseTrace(enInfo, "[%x] Submitting %d quotes...", m_hSession, Request.items_n);

	Request.m_uiLen -= sizeof ( quote_trans_mo106_item_t ) * ( 192 - Request.items_n);
	SendTx(Request, m_uiBaseFacility + FACILITY_EP7, uiTransactionID, uiOrderID);
	QW2S(uiOrderID, sOrderID);
};

void CISESession::DeleteQuote(CISEDeleteQuote& Request, string& sOrderID)
{
	uint64_t uiTransactionID;
	uint64_t uiOrderID;

	//IseTrace(enInfo, "[%x] Deleting quote...", m_hSession);

	SendTx(Request, m_uiBaseFacility + FACILITY_EP7, uiTransactionID, uiOrderID);
	QW2S(uiOrderID, sOrderID);
};

/*--------------------------------[ Order ]--------------------------------------------*/

void CISESession::EnterOrder(const string& sSeries, 
							 const unsigned int uiQuantity, const double dPremium,
							 const BidAskEnum BidOrAsk,
							 string& sOrderID)
{
	CISEEnterOrder	Request;

	{
		CAutoLock	DataLock(this);

		if(IsLoaded() == false)
			throw CISEException(0, 0, this, "Statis data is not ready to use.");

		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CSymbolParams Req;
		Req.m_sSymbol = sSeries.c_str();

		CSeries* pSeries = GetSeriesByRequest(Req);
		if(pSeries == NULL || pSeries->m_pUnderlying == NULL || pSeries->m_pInstrumentClass == NULL)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}


		Request.series = pSeries->m_Series;
		Request.orderbook_c = pSeries->m_pUnderlying->m_uiOrderbook;
		Request.quantity_u = uiQuantity;
		Request.premium_i = DBL2DEC(dPremium, pSeries->m_pInstrumentClass->m_uiDecInPremium);
		Request.time_validity_n = VALIDITY_UNIT_IOC << 8 + 0;// IOC
		Request.bid_or_ask_c = BidOrAsk;
		Request.order_type_c = ORDER_TYPE_LIMIT;
		Request.ignore_away_market_c = ISEBOOL_YES;
		Request.client_category_c = CUST_CATEGORY_MM;
		Request.open_close_req_c = OPEN_CLOSE_REQUEST_DEFAULT;
		StringToClickString("", MAX_CMTA_MEMBER, Request.ex_client_s);
		StringToClickString("", MAX_SUB_ACCOUNT_ID, (char*)Request.ex_client_s + MAX_CMTA_MEMBER);
		StringToClickString("", sizeof(Request.broker_id_s), Request.broker_id_s);
		StringToClickString("Egar", sizeof(Request.free_text_s), Request.free_text_s);
	
	}

	uint64_t uiTransactionID;
	uint64_t uiOrderID;

	SendTx(Request, m_uiBaseFacility + FACILITY_EP7, uiTransactionID, uiOrderID);

	QW2S(uiOrderID, sOrderID);
}
