#include "StdAfx.h"

#include "OmniApiUtils.h"
#include "ISESession.h"
#include "ISEData.h"

/*-------------------------[ Underlying info ]--------------------------*/

bool CISEData::QueryAllUnderlyingsInfo(vector<CUnderlyingInfo>& Infos)
{
	Infos.clear();

	CAutoLock DataLock(this);

	if(IsLoaded() == false)
		return false;

	CUnderlyingInfo	Info;

	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);

	for(set<CUnderlying>::iterator it = m_ISE.m_Underlyings.m_setData.begin();
			it != m_ISE.m_Underlyings.m_setData.end(); it++)
	{
		const CUnderlying& Und = *it;

		if(Und.m_uiBin == m_ISE.m_uiDefaultBin)
			Info.m_TraderRole = m_ISE.m_TraderRoleInDefaultBin;
		else
			Info.m_TraderRole = enTrEAM;

		Info.m_lBinNum = Und.m_uiBin;
		Info.m_sSymbol = Und.m_sSymbol.c_str();
		Info.m_sDescription = Und.m_sFullname;
		Info.m_sCurrency = Und.m_sCurrency.c_str();
		Info.m_uiType = Und.m_uiType;
		Info.m_bLast = false;

		if(Und.m_pInstrumentClass)
			Info.m_lLotSize = Und.m_pInstrumentClass->m_iContractSize;
		else
			Info.m_lLotSize = 0;

		{
			CAutoLock	ExchangesLock(&m_Exchanges);

			CExchange* pExchange = m_Exchanges.FindByCountry(Und.m_uiCountry);
			if(pExchange)
				Info.m_sExchange = pExchange->m_sShortName;
			else
				Info.m_sExchange = "";
		}


		Infos.push_back(Info);
	}

	return true;
}

bool CISEData::QueryUnderlyingInfo(const CSymbolParams& Req, CUnderlyingInfo& Info)
{
	CAutoLock DataLock(this);

	if(IsLoaded() == false)
		return false;

	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);

	CUnderlying* pUnd = GetUnderlyingByRequest(Req);
	if(pUnd == NULL)
	{
		throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
			"No such symbol found");
	}

	if(pUnd->m_uiBin == m_ISE.m_uiDefaultBin)
		Info.m_TraderRole = m_ISE.m_TraderRoleInDefaultBin;
	else
		Info.m_TraderRole = enTrEAM;

	Info.m_lBinNum = pUnd->m_uiBin;
	Info.m_sSymbol = pUnd->m_sSymbol.c_str();
	Info.m_sDescription = pUnd->m_sFullname;
	Info.m_sCurrency = pUnd->m_sCurrency.c_str();
	Info.m_uiType = pUnd->m_uiType;
	Info.m_bLast = false;
	Info.m_lBinNum = pUnd->m_uiBin;

	if(pUnd->m_pInstrumentClass)
		Info.m_lLotSize = pUnd->m_pInstrumentClass->m_iContractSize;
	else
		Info.m_lLotSize = 0;

	CAutoLock	ExchangesLock(&m_Exchanges);
	CExchange* pExchange = m_Exchanges.FindByCountry(pUnd->m_uiCountry);
	if(pExchange)
		Info.m_sExchange = pExchange->m_sShortName;
	else
		Info.m_sExchange = "";

	return true;
}

/*-------------------------[ Series info ]--------------------------*/

/*
	Query list of all options for specific underlying
*/
bool CISEData::QueryUnderlyingOptionsInfo(const CSymbolParams& Req, vector<COptionInfo>& Infos)
{
	Infos.clear();

	CAutoLock DataLock(this);

	if(IsLoaded() == false)
		return false;

	COptionInfo	Info;

	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);
	CAutoLock	SeriesLock(&m_ISE.m_Series);

	CUnderlying* pUnd = GetUnderlyingByRequest(Req);
	if(pUnd == NULL)
	{
		throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
			"No such symbol found");
	}

	const vector<CSeriesDatabase::itSeriesT>& SeriesIts = pUnd->m_Series;

	for(vector<CSeriesDatabase::itSeriesT>::const_iterator it = SeriesIts.begin();
		it != SeriesIts.end(); it++)
	{
		const CSeriesDatabase::itSeriesT & itSeries = *it;
		const CSeries& Series = *itSeries;

		Info.m_dStrikePrice = Series.m_dStrikePrice;
		Info.m_uiExpirationDate = Series.m_Series.expiration_date_n;
		Info.m_uiGroup = Series.m_Series.instrument_group_c;
		Info.m_lLotSize = Series.m_iContractSize;
		if(Series.m_pInstrumentClass)
		{
			if(Info.m_lLotSize == 0)
			{
				Info.m_lLotSize = Series.m_pInstrumentClass->m_iContractSize;
			}

			Info.m_sCurrency = Series.m_pInstrumentClass->m_sCurrency;
		}
		else
		{
			Info.m_sCurrency = "";
		}

		{
			CAutoLock	ExchangesLock(&m_Exchanges);
			CExchange* pExchange = m_Exchanges.FindByCountry(Series.m_Series.country_c);
 			if(pExchange)
			{
				Info.m_sExchange = pExchange->m_sShortName;
				//if(Info.m_sExchange != "K") __asm int 3;
			}
			else
				Info.m_sExchange = "";
		}

		Info.m_sSeries =  Series.m_sSymbol.c_str();
		Info.m_sUnderlying = Req.m_sSymbol.c_str();
		Info.m_bLast = false;

		Infos.push_back(Info);
	}

	return true;
}

/*-------------------------[ Underlying BBO ]--------------------------*/

bool CISEData::QueryUnderlyingBBO(const CSymbolParams& Req, CQuoteBBO& BBO)
{
	CISEQueryUnderlyingPrices	Query;
	CISEAnswer					Answer;
	uint64_t					uiTransactionID;
	uint64_t					uiOrderID;

	uint16 uiCommodity;
	

	{
		CAutoLock DataLock(this);

		if(IsLoaded() == false)
			return false;

		if(Req.m_sExchange != "" && _stricmp(Req.m_sExchange.c_str(), ISE_EXCHANGE_CODE) != 0)
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, "Only ISE exchange is supported.");

		CAutoLock	UndLock(&m_ISE.m_Underlyings);

		CUnderlying* pUnd = GetUnderlyingByRequest(Req);
		if(pUnd == NULL)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}

		Query.series.country_c = m_ISE.m_uiCountry;
		Query.series.market_c = m_ISE.m_uiMarket;
		Query.bin_c = pUnd->m_uiBin;
		uiCommodity = Query.series.commodity_n = pUnd->m_uiCommodity; 
	}

	do
	{
		Query.segment_number_n++;

		try
		{
			m_pSession->SendQuery(Query, m_uiBaseFacility + FACILITY_EP1, Answer, 
						uiTransactionID, uiOrderID);
		}
		catch(CISEException& e)
		{
			// disconnection
			if(e.IsConnectionError())
				return false;

			throw;
		}

		const answer_underlying_prices_ia121_t& Result  = Answer.m_Data.m_UnderlyingPrices;
		if(Result.items_n == 0)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}

		for(uint16 nIndex = 0; nIndex < Result.items_n; nIndex++)
		{
		   const answer_underlying_prices_ia121_item_t& Item = Result.item[nIndex];
		   if(Item.commodity_n == uiCommodity)
		   {
			   CAutoLock	UndLock(&m_ISE.m_Underlyings);
			   
			   CUnderlying* pUnd = m_ISE.m_Underlyings.FindByCommodity(uiCommodity);
			   
			   if(pUnd == NULL)
			   {
				   throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
						"No such symbol found");
			   }

			   pUnd->m_dBestBidPremium = BBO.m_dBid = DEC2DBL(Item.best_bid_premium_i, pUnd->m_uiDecInPrice);
			   pUnd->m_dBestAskPremium = BBO.m_dAsk = DEC2DBL(Item.best_ask_premium_i, pUnd->m_uiDecInPrice);
			   pUnd->m_uiBestBidSize = BBO.m_lBidSize = 0;
			   pUnd->m_uiBestAskSize = BBO.m_lAskSize = 0;
			   BBO.m_lStatus = pUnd->m_uiTradingStatus;
			   BBO.m_sCurrency = pUnd->m_sCurrency;
			   pUnd->m_dClosePrice = BBO.m_dClose = DEC2DBL(Item.closing_price_i, pUnd->m_uiDecInPrice);
			   pUnd->m_dLastPrice = BBO.m_dLast = DEC2DBL(Item.last_price_i, pUnd->m_uiDecInPrice);
			   return true;
		   }
		}

	}while(Answer.m_Data.m_UnderlyingPrices.segment_number_n != 0);

	// no items were found for the same commodity
	throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
		"No such symbol found");
}

/*-------------------------[ Options BBO ]--------------------------*/

bool CISEData::QueryOptionBBO(const CSymbolParams& Req, CQuoteBBO& BBO)
{
	CISEQuerySeriesPrices	Query;

	series_t	Series;

	{
		CAutoLock DataLock(this);

		if(IsLoaded() == false)
			return false;

		if(Req.m_sExchange != "" && _stricmp(Req.m_sExchange.c_str(), ISE_EXCHANGE_CODE) != 0)
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, "Only ISE exchange is supported.");

		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CSeries* pSeries = GetSeriesByRequest(Req);
		if(pSeries == NULL || pSeries->m_pUnderlying == NULL || pSeries->m_pInstrumentClass == NULL)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}

		Series = Query.series = pSeries->m_Series;
		Query.orderbook_c = pSeries->m_pUnderlying->m_uiOrderbook;
		Query.bin_c = pSeries->m_pUnderlying->m_uiBin;
	}

	CISEAnswer				Answer;
	uint64_t				uiTransactionID;
	uint64_t				uiOrderID;

	do
	{
		Query.segment_number_n++;

		try
		{
			m_pSession->SendQuery(Query, m_uiBaseFacility + FACILITY_EP7, Answer, 
						uiTransactionID, uiOrderID);
		}
		catch(CISEException& e)
		{
			// disconnection
			if(e.IsConnectionError())
				return false;

			throw;
		}

		const answer_prices_ia101& Result  = Answer.m_Data.m_Prices;

		if(Result.items_n == 0)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}

		CAutoLock DataLock(this);

		if(IsLoaded() == false)
			return false;

		CAutoLock	SeriesLock(&m_ISE.m_Series);

		for(uint16 nIndex = 0; nIndex < Result.items_n; nIndex++)
		{
			const answer_prices_ia101_item_t& Item = Result.item[nIndex];

			if(memcmp(&Item.series, &Series, sizeof(series_t)) == 0)
			{
				CSeries* pSeries = m_ISE.m_Series.FindBySeries(Item.series);
				if(pSeries == NULL || pSeries->m_pInstrumentClass == NULL)
				{
					throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
						"No such symbol found");
				}

				pSeries->m_dBestBidPremium = BBO.m_dBid = DEC2DBL(Item.best_bid_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
				pSeries->m_dBestAskPremium = BBO.m_dAsk = DEC2DBL(Item.best_ask_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
				pSeries->m_uiBestBidSize = BBO.m_lBidSize = Item.best_bid_size_u;
				pSeries->m_uiBestAskSize = BBO.m_lAskSize = Item.best_ask_size_u;
				pSeries->m_uiTradingStatus = BBO.m_lStatus = Item.series_status_c;
				pSeries->m_dClosePrice = BBO.m_dClose = DEC2DBL(Item.closing_price_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
				pSeries->m_dLastPrice = BBO.m_dLast = DEC2DBL(Item.last_price_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
				BBO.m_sCurrency = pSeries->m_pInstrumentClass->m_sCurrency;
				return true;
			}
		}

	}while(Answer.m_Data.m_Prices.segment_number_n != 0);

	throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
		"No such symbol found");
}

bool CISEData::QueryGroupOptionsBBO(const CSymbolParams& Req, map<CSymbolParams, CQuoteBBO>& mapSeriesBBO)
{
	CISEQuerySeriesPrices	Query;
	CSymbolParams			Series;

	{
		CAutoLock DataLock(this);

		if(IsLoaded() == false)
			return false;

		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CUnderlying* pUnd = GetUnderlyingByRequest(Req);
		if(pUnd == NULL)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}

		CQuoteBBO		BBO;
		BBO.m_dBid = 0;
		BBO.m_dAsk = 0;
		BBO.m_lBidSize = 0;
		BBO.m_lAskSize = 0;
		BBO.m_lStatus = 0;

		vector<CSeriesDatabase::itSeriesT>::iterator itSeries;
		for(itSeries = pUnd->m_Series.begin(); itSeries != pUnd->m_Series.end(); itSeries++)
		{
			Series.m_sSymbol = (*itSeries)->m_sSymbol.c_str();
			Series.m_sParentSymbol = Req.m_sSymbol.c_str();
			Series.m_sExchange = Req.m_sExchange.c_str();
			mapSeriesBBO[Series] = BBO;
		}

		Query.series.commodity_n = pUnd->m_uiCommodity;
		Query.series.country_c = m_ISE.m_uiCountry;
		Query.series.market_c = m_ISE.m_uiMarket;
		Query.orderbook_c = pUnd->m_uiOrderbook;
		Query.bin_c = pUnd->m_uiBin;
	}

	CISEAnswer				Answer;
	uint64_t				uiTransactionID;
	uint64_t				uiOrderID;


	do
	{
		Query.segment_number_n++;

		try
		{
			m_pSession->SendQuery(Query, m_uiBaseFacility + FACILITY_EP7, Answer, 
						uiTransactionID, uiOrderID);
		}
		catch(CISEException& e)
		{
			// disconnection
			if(e.IsConnectionError())
				return false;

			throw;
		}

		const answer_prices_ia101& Result  = Answer.m_Data.m_Prices;

		CAutoLock DataLock(this);

		if(IsLoaded() == false)
			return false;

		CAutoLock	SeriesLock(&m_ISE.m_Series);

		for(uint16 nIndex = 0; nIndex < Result.items_n; nIndex++)
		{
			const answer_prices_ia101_item_t& Item = Result.item[nIndex];

			CSeries* pSeries = m_ISE.m_Series.FindBySeries(Item.series);
			
			if(pSeries && pSeries->m_pInstrumentClass)
			{
				Series.m_sSymbol = pSeries->m_sSymbol.c_str();
				map<CSymbolParams, CQuoteBBO>::iterator it = mapSeriesBBO.find(Series);
				if(it != mapSeriesBBO.end())
				{
					CQuoteBBO& BBO = it->second;

					pSeries->m_dBestBidPremium = BBO.m_dBid = DEC2DBL(Item.best_bid_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
					pSeries->m_dBestAskPremium = BBO.m_dAsk = DEC2DBL(Item.best_ask_premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);;
					pSeries->m_uiBestBidSize = BBO.m_lBidSize = Item.best_bid_size_u;
					pSeries->m_uiBestAskSize = BBO.m_lAskSize = Item.best_ask_size_u;
					pSeries->m_uiTradingStatus = BBO.m_lStatus = Item.series_status_c;
					pSeries->m_dClosePrice = BBO.m_dClose = DEC2DBL(Item.closing_price_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
					pSeries->m_dLastPrice = BBO.m_dLast = DEC2DBL(Item.last_price_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
					BBO.m_sCurrency = pSeries->m_pInstrumentClass->m_sCurrency;
				}
			}
		}

	}while(Answer.m_Data.m_Prices.segment_number_n != 0);

	return true;
}

/*-------------------------[ MM Quotes ]--------------------------*/

bool CISEData::QueryMmQuotes(const CMMQuoteParams& Req, set<CQuoteParams>& setQuotes)
{
	setQuotes.clear();

	CISEQueryQuotes	Query;
	CQuoteParams	Quote;

	{
		CAutoLock DataLock(this);

		if(IsLoaded() == false)
			return false;

		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CUnderlying*			pUnd;
		CSeries*				pSeries;


		// fill query
		if(Req.m_Type == enSTK)
		{
			pUnd = GetUnderlyingByRequest(Req.m_sSymbol);
			if(pUnd == NULL)
			{
				throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession,  "No such symbol found");
			}

			Query.series.commodity_n = pUnd->m_uiCommodity;
			Query.orderbook_c = pUnd->m_uiOrderbook;
			Query.bin_c = pUnd->m_uiBin;
		}
		else
		{
			pSeries = GetSeriesByRequest(Req.m_sSymbol);
			if(pSeries == NULL || pSeries->m_pUnderlying == NULL || pSeries->m_pInstrumentClass == NULL)
			{
				throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession,  "No such symbol found");
			}

			Query.series = pSeries->m_Series;
			Query.orderbook_c = pSeries->m_pUnderlying->m_uiOrderbook;
			Query.bin_c = pSeries->m_pUnderlying->m_uiBin;
			memcpy(&Query.filter_series, &pSeries->m_Series, sizeof(series_t));
		}

		Query.bid_or_ask_c = Req.m_BidOrAsk;

		// check for cache
		bool	bCached = true;
	
		if(Req.m_Type == enSTK)
		{
			vector<CSeriesDatabase::itSeriesT>::iterator itSeries;
			for(itSeries = pUnd->m_Series.begin(); itSeries != pUnd->m_Series.end(); itSeries++)
			{
				Quote.m_sSeries = (*itSeries)->m_sSymbol.c_str();

				if(Req.m_BidOrAsk == enBoth)
				{
					if((*itSeries)->GetQuoteCache(enBid, Quote, Quote.m_lTickLevel) == false)
						bCached = false;
					setQuotes.insert(Quote);

					if((*itSeries)->GetQuoteCache(enAsk, Quote, Quote.m_lTickLevel) == false)
						bCached = false;
					setQuotes.insert(Quote);
				}
				else
				{
					if((*itSeries)->GetQuoteCache(Req.m_BidOrAsk, Quote, Quote.m_lTickLevel) == false)
						bCached = false;
					setQuotes.insert(Quote);
				}
			}		
		}
		else
		{
			Quote.m_sSeries = Req.m_sSymbol.c_str();

			if(Req.m_BidOrAsk == enBoth)
			{
				if(pSeries->GetQuoteCache(enBid, Quote, Quote.m_lTickLevel) == false)
					bCached = false;
				setQuotes.insert(Quote);

				if(pSeries->GetQuoteCache(enAsk, Quote, Quote.m_lTickLevel) == false)
					bCached = false;
				setQuotes.insert(Quote);
			}
			else
			{
				if(pSeries->GetQuoteCache(Req.m_BidOrAsk, Quote, Quote.m_lTickLevel) == false)
					bCached = false;

				setQuotes.insert(Quote);
			}
		}

		if(bCached)
			return true;
	}

	CISEAnswer				Answer;
	uint64_t				uiTransactionID;
	uint64_t				uiOrderID;

	{
		CAutoLock UserLock(&(m_pSession->m_User));

		StringToClickString(m_pSession->m_User.m_sCountryID, 
			sizeof(Query.whose.trading_code.country_id_s),
			Query.whose.trading_code.country_id_s);
		StringToClickString(m_pSession->m_User.m_sCustomerID, 
			sizeof(Query.whose.trading_code.ex_customer_s),
			Query.whose.trading_code.ex_customer_s);
	}

	try
	{
		m_pSession->SendQuery(Query, m_uiBaseFacility + FACILITY_EP7, Answer, 
					uiTransactionID, uiOrderID);
	}
	catch(CISEException& e)
	{
		// disconnection
		if(e.IsConnectionError())
			return false;

		throw;
	}

	CAutoLock DataLock(this);

	if(IsLoaded() == false)
		return false;

	CAutoLock	SeriesLock(&m_ISE.m_Series);

	const answer_quote_ma104& Result = Answer.m_Data.m_Quote;
	for(int iQuote = 0; iQuote < Result.items_n; iQuote++)
	{
		const answer_quote_ma104_item_t& Item = Result.item[iQuote];

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(Item.series);
		if(pSeries)
		{
			Quote.m_sSeries = pSeries->m_sSymbol.c_str();
			Quote.m_BidOrAsk = Item.bid_or_ask_c == BA_BID ? enBid : enAsk;

			set<CQuoteParams>::iterator it = setQuotes.find(Quote);
			if(it != setQuotes.end())
			{
				CQuoteParams& Quote = *it;
				Quote.m_dPremium = DEC2DBL(Item.premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
				Quote.m_lOpenQuantity = Item.open_quantity_u;
				Quote.m_lQuantity = Item.quantity_u;
				Quote.m_lStepUpBuffer = Item.step_up_buffer_n;
				Quote.m_lTickLevel = Item.number_of_tick_down_levels_n;

				pSeries->UpdateQuoteCache(Quote, Quote.m_lTickLevel);
			}
		}
	}

	return true;
}
