#include "StdAfx.h"

#include "Trace.h"
#include "OmniApiUtils.h"
#include "ISESession.h"
#include "ISEData.h"

/*------------------------------[ Series ]---------------------------------------*/

void CISEData::ModifySeriesBBO(const best_bid_offer_bo101_t& Data)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);
	CAutoLock	SeriesLock(&m_ISE.m_Series);

	const uint16 nItems = Data.items_n;

	for(uint16 nIndex = 0; nIndex < nItems; nIndex++)
	{
		const best_bid_offer_bo101_item_t& Item = Data.item[nIndex];
		
		CSeries* pSeries = m_ISE.m_Series.FindBySeries(Item.series);
		if(pSeries && pSeries->m_pInstrumentClass)
		{
		   pSeries->m_uiBestBidSize = Item.best_bid_size_u;
		   pSeries->m_uiBestAskSize = Item.best_ask_size_u;
		   pSeries->m_dBestBidPremium = DEC2DBL(Item.best_bid_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
		   pSeries->m_dBestAskPremium = DEC2DBL(Item.best_ask_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);

		   FireBBO(pSeries);
		}
	}
};

void CISEData::ModifySeriesExBBO(const best_bid_offer_extended_bo102_t& Data)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);
	CAutoLock	SeriesLock(&m_ISE.m_Series);

	const uint16 nItems = Data.items_n;

	for(uint16 nIndex = 0; nIndex < nItems; nIndex++)
	{
		const best_bid_offer_extended_bo102_item_t& Item = Data.item[nIndex];
		
		CSeries* pSeries = m_ISE.m_Series.FindBySeries(Item.series);
		if(pSeries && pSeries->m_pInstrumentClass)
		{
		   pSeries->m_uiBestBidSize = Item.best_bid_size_u;
		   pSeries->m_uiBestAskSize = Item.best_ask_size_u;
		   pSeries->m_dBestBidPremium = DEC2DBL(Item.best_bid_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
		   pSeries->m_dBestAskPremium = DEC2DBL(Item.best_ask_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
		   pSeries->m_uiBidMarketOrderSize = Item.bid_market_order_size_u;
   		   pSeries->m_uiAskMarketOrderSize = Item.ask_market_order_size_u;
           pSeries->m_nMmNumber = Item.mm_number_n;

		   FireBBO(pSeries);
		}
	}
}

/*------------------------------[ Underlying ]---------------------------------------*/

void CISEData::ModifyUnderlyingBBO(const underlying_best_bid_offer_bo105_t& Data)
{
	CAutoLock	Lock(&m_ISE.m_Underlyings);

	CUnderlying* pUnd = m_ISE.m_Underlyings.FindByCommodity(Data.series.commodity_n);
	if(pUnd)
	{
		pUnd->m_dBestAskPremium = DEC2DBL(Data.best_ask_premium_i, pUnd->m_uiDecInPrice);
		pUnd->m_dBestBidPremium = DEC2DBL(Data.best_bid_premium_i, pUnd->m_uiDecInPrice);
		pUnd->m_uiCountry = Data.originator_id_c;//@
		pUnd->m_uiBestBidSize = pUnd->m_uiBestAskSize = 0;

		FireBBO(pUnd);		
	}
}

/*------------------------------[ OPRA ]---------------------------------------*/

void CISEData::ModifyOpraSeriesBBO(const opra_best_bid_offer_bo103_t& Data)
{
	CAutoLock	Lock(&m_ISE.m_Series);

	const uint16 nItems = Data.items_n;

	for(uint16 nIndex = 0; nIndex < nItems; nIndex++)
	{
		const opra_best_bid_offer_bo103_item_t& Item = Data.item[nIndex];
	}
};

void  CISEData::ModifyOpraEachExchangeSeriesBBO(const opra_all_best_bid_offer_bo104_t& Data)
{
	CAutoLock	Lock(&m_ISE.m_Series);

	const uint16 nItems = Data.items_n;

	for(uint16 nIndex = 0; nIndex < nItems; nIndex++)
	{
		const opra_all_best_bid_offer_bo104_item_t& Item = Data.item[nIndex];
	}
};


/*------------------------------[ Utils ]---------------------------------------*/

bool CISEData::PrepareBBO(const CSeries* pSeries, CSymbolParams& Symbol, CQuoteBBO& BBO)
{
	if(pSeries->m_uiTradingStatus != STS_UNKNOWN)
	{
		if(pSeries->m_pUnderlying)
		{
		   Symbol.m_sSymbol = pSeries->m_sSymbol.c_str();
		   Symbol.m_sParentSymbol = pSeries->m_pUnderlying->m_sSymbol.c_str();
		   Symbol.m_sExchange = ISE_EXCHANGE_CODE;

		   BBO.m_dBid = pSeries->m_dBestBidPremium;
		   BBO.m_dAsk = pSeries->m_dBestAskPremium;
		   BBO.m_dLast = pSeries->m_dLastPrice;
		   BBO.m_dClose = pSeries->m_dClosePrice;
		   BBO.m_lBidSize = pSeries->m_uiBestBidSize;
		   BBO.m_lAskSize = pSeries->m_uiBestAskSize;
		   BBO.m_lStatus = pSeries->m_uiTradingStatus;
		   if(pSeries->m_pInstrumentClass)
			   BBO.m_sCurrency = pSeries->m_pInstrumentClass->m_sCurrency;
		   else
			   BBO.m_sCurrency = "USD";
		   return true;
		}
	}

	return false;
}

bool CISEData::PrepareBBO(const CUnderlying* pUnd, CSymbolParams& Symbol, CQuoteBBO& BBO)
{
	if(pUnd->m_uiTradingStatus != UTS_UNKNOWN)
	{
		Symbol.m_sSymbol = pUnd->m_sSymbol.c_str();
		Symbol.m_sExchange = ISE_EXCHANGE_CODE;

		BBO.m_dBid = pUnd->m_dBestBidPremium;
		BBO.m_dAsk = pUnd->m_dBestAskPremium;
		BBO.m_dLast = 0/*pUnd->m_dLastPrice*/;
		BBO.m_dClose = pUnd->m_dClosePrice;
		BBO.m_lStatus = pUnd->m_uiTradingStatus;
		BBO.m_lBidSize = pUnd->m_uiBestBidSize;
		BBO.m_lAskSize = pUnd->m_uiBestAskSize;
		BBO.m_sCurrency = pUnd->m_sCurrency;

		return true;
	}

	return false;
}

void CISEData::FireBBO(const CSeries* pSeries)
{
   CSymbolParams Symbol;
   CQuoteBBO	 BBO;

   if(PrepareBBO(pSeries, Symbol, BBO))
   {
	   m_ISE.m_Series.Unlock();
	   m_ISE.m_Underlyings.Unlock();
	   m_ISE.m_InstrumentClasses.Unlock();

	   OnQuoteBBO(Symbol, _enSubscribeOption, BBO);

	   m_ISE.m_InstrumentClasses.Lock();
	   m_ISE.m_Underlyings.Lock();
	   m_ISE.m_Series.Lock();
   }
}

void CISEData::FireBBO(const CUnderlying* pUnd)
{
	CSymbolParams Symbol;
	CQuoteBBO	  BBO;

	if(PrepareBBO(pUnd, Symbol, BBO))
	{
		_EventTypeEnum Type;
		switch(pUnd->m_uiType)
		{
			case UT_STOCK_INDEX:
			case UT_CURRENCY_INDEX:
			case UT_INTEREST_RATE_INDEX:
			case UT_ENERGY_INDEX:
			case UT_SOFT_AND_AGRICS_INDEX:
			case UT_METAL_INDEX:
				Type = _enSubscribeIndex;
				break;

			default:
				Type = _enSubscribeStock;
				break;
		}

		m_ISE.m_Underlyings.Unlock();

		OnQuoteBBO(Symbol, Type, BBO);

		m_ISE.m_Underlyings.Lock();
	}
}