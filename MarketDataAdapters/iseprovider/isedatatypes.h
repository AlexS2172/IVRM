#ifndef ISE_DATA_TYPES_H
#define ISE_DATA_TYPES_H

#include "ISEProvider.h"
#include "ISEEvents.h"

// forward declarations
class CInstrument;
class CInstrumentsDatabase;
class CUnderlying;
class CUnderlyingDatabase;
class CInstrumentClass;
class CInstrumentClassDatabase;
class CCountryMarket;
class CSeries;
class CSeriesDatabase;
class CBin;
class CBinDatabase;
class CISEData;

//----------------------------------[Instrument]--------------------------------------------

class CInstrument
{
public:

	CInstrument(uint8 uiGroup)
	:	m_uiGroup(uiGroup),
		m_uiMarket(0),
		m_uiTradingState(0), 
		m_iBBOVolume(0),
		m_iBlockOrderMinSize(0),
		m_iBlockOrderResponceTime(0)
	{
	}

	const uint8	m_uiGroup;					// IG_* constants
	string		m_sName;					// "American Put Option"
	uint8		m_uiMarket;					// MC_* constants
	int32		m_iBBOVolume;				// minimum size at BBO
	int32		m_iBlockOrderMinSize;
	int32		m_iBlockOrderResponceTime;
	uint16		m_uiTradingState;

	bool operator < (const CInstrument& Origin) const
	{
		return m_uiGroup < Origin.m_uiGroup;
	}
};

class CInstrumentsDatabase : public CLock
{
	friend class CCountryMarket;

private:

	set<CInstrument> m_setData;

public:

	CInstrument* FindByGroup(const uint8 uiGroup) 
	{
		CInstrument	Instrument(uiGroup);
		
		set<CInstrument>::iterator it = m_setData.find(Instrument);
		if(it != m_setData.end())
			return &(*it);
		else
			return NULL;
	}

	void Add(const CInstrument& Instrument)
	{
		EgAssert(m_setData.find(Instrument) == m_setData.end());
		m_setData.insert(Instrument);
	}

	void Clear()
	{
		m_setData.clear();
	}
};

//----------------------------------[Exchange]--------------------------------------------

class CExchange
{
public:
		
	CExchange(uint8 uiCountry)
	:	m_uiCountry(uiCountry),
		m_cOpraIndicator(0)
	{
	}

	const uint8	m_uiCountry;	
	int8		m_cOpraIndicator;	// The character by which OPRA identifies an exchange.
	string		m_sName;
	string		m_sShortName;
	string		m_sCountryID;

	bool operator < (const CExchange& Origin) const
	{
		return m_uiCountry < Origin.m_uiCountry;
	}
};

class CExchangesDatabase : public CLock
{
private:

	set<CExchange>	m_setData;

public:
	
	void Add(const CExchange& Exchange)
	{
		EgAssert(m_setData.find(Exchange) == m_setData.end());
		m_setData.insert(Exchange);
	}

	CExchange* FindByCountry(const uint8 uiCountry) 
	{
		CExchange Exchange(uiCountry);
		
		set<CExchange>::iterator it = m_setData.find(Exchange);

		if(it != m_setData.end())
			return &(*it);
		else
			return NULL;
	}

	CExchange* FindByName(const string& sName) 
	{
		for(set<CExchange>::iterator it = m_setData.begin(); it != m_setData.end(); it++)
		{
			if(it->m_sName == sName)
				return &(*it);
		}
		return NULL;
	}

	void Clear()
	{
		m_setData.clear();
	}
};

//----------------------------------[Trading state]--------------------------------------------

class CTradingState
{
public:

	CTradingState(uint16 uiID)
	:	m_uiID(uiID),
		m_uiFastMarketLevel(0),
		m_cMatchingType(0),
		m_cEndTrading(0),
		m_cPriceQuotationReq(0),
		m_cMarketOrdersAllowed(0),
		m_cFillKillAllowed(0),
		m_cEditedOrderBookChangesAvail(0),
		m_cOrderBookChangesAvail(0)
	{
	}

	string		m_sName;					// ASCII
	const uint16 m_uiID;					// Binary
	uint16		m_uiFastMarketLevel;
	uint8		m_cMatchingType;			//Indicates how matching is performed for a trading state:MATCH_*
	uint8		m_cEndTrading;				//This state means the end of trading for a day. ENDTRADE_*
	uint8		m_cPriceQuotationReq;		//Is price quotation required during this state? ISEBOOL_*
	uint8		m_cMarketOrdersAllowed;		//Are market orders allowed during the state?ISEBOOL_*
	uint8		m_cFillKillAllowed;			//whether fill and/or kill orders are allowed during the state? ISEBOOL_*
	uint8		m_cEditedOrderBookChangesAvail;//Are edited order book changes available during the state? ISEBOOL_*
	uint8		m_cOrderBookChangesAvail;	//Are order book changes available during the state? ISEBOOL_*

   bool operator < (const CTradingState& Origin) const
   {
	   return m_uiID < Origin.m_uiID;
   }
};

class CTradingStatesDatabase : public CLock
{
	set<CTradingState>		m_setData;

public:

	void Add(const CTradingState& TradingState)
	{
		EgAssert(m_setData.find(TradingState) == m_setData.end());
		m_setData.insert(TradingState);
	}

	CTradingState* FindByID(const uint16 uiID) 
	{
		CTradingState TradingState(uiID);
		
		set<CTradingState>::iterator it = m_setData.find(TradingState);

		if(it != m_setData.end())
			return &(*it);
		else
			return NULL;
	}

	void Clear()
	{
		m_setData.clear();
	}
};

struct CCoupon
{
	string	m_sDate;
	uint32	m_uiDividend;
};

typedef vector<CCoupon>		TCouponsVector;

struct CFastMarketLevel
{
	uint16	m_uiLevel;
	int32	m_uiMatchInterval;
};

typedef vector<CFastMarketLevel>	TFastMarketLevelsVector;

//----------------------------------[Series]--------------------------------------------

class CSeries
{
public:
	/*
	CSeries()
	:	m_dStrikePrice(0.0), 
		m_pInstrument(NULL), 
		m_pInstrumentClass(NULL), 
		m_pUnderlying(NULL), 
		m_uiTradingStatus(STS_UNKNOWN),
		m_lBidTickLevel(-1),
		m_lAskTickLevel(-1),
		m_uiBestBidSize(0),
		m_uiBestAskSize(0),
		m_dBestBidPremium(0.0),
		m_dBestAskPremium(0.0),
		m_dClosePrice(0.0),
		m_dLastPrice(0.0)
	{
	}
	*/

	CSeries(const series_t& rSeries)
	:	m_dStrikePrice(0.0), 
		m_pInstrument(NULL), 
		m_pInstrumentClass(NULL), 
		m_pUnderlying(NULL), 
		m_uiTradingStatus(STS_UNKNOWN),
		m_lBidTickLevel(-1),
		m_lAskTickLevel(-1),
		m_uiBestBidSize(0),
		m_uiBestAskSize(0),
		m_dBestBidPremium(0.0),
		m_dBestAskPremium(0.0),
		m_dClosePrice(0.0),
		m_dLastPrice(0.0),
		m_Series(rSeries)
	{
	}

	// MM quotes cache
	double		m_dMmBidCache;
	double		m_dMmAskCache;
	long		m_lMmBidSizeCache;
	long		m_lMmAskSizeCache;
	long		m_lBidOpenQuantityCache;
	long		m_lAskOpenQuantityCache;
	long		m_lBidStepUpBufferCache;
	long		m_lAskStepUpBufferCache;
	long		m_lBidTickLevel;
	long		m_lAskTickLevel;

	void	UpdateQuoteCache(const CQuoteParams& Quote)
	{
		UpdateQuoteCache(Quote.m_BidOrAsk,
						 Quote.m_dPremium,
						 Quote.m_lQuantity,
						 Quote.m_lOpenQuantity,
						 Quote.m_lStepUpBuffer);
	}

	void	UpdateQuoteCache(const CQuoteParams& Quote, const long lTickLevel)
	{
		UpdateQuoteCache(Quote.m_BidOrAsk,
						 Quote.m_dPremium,
						 Quote.m_lQuantity,
						 Quote.m_lOpenQuantity,
						 Quote.m_lStepUpBuffer,
						 lTickLevel);
	}


	void	UpdateQuoteCache(const BidAskEnum BidOrAsk, 
							 const double&  dPrice,
							 const long		lSize,
							 const long		lOpenQuantity,
							 const long		lStepUpBuffer)
	{
		if(BidOrAsk == enBid)
		{
			m_dMmBidCache = dPrice;
			m_lMmBidSizeCache = lSize;
			m_lBidOpenQuantityCache = lOpenQuantity;
			m_lBidStepUpBufferCache = lStepUpBuffer;
		}
		else 
		{
			if(BidOrAsk == enBoth)
			{
				m_dMmBidCache = dPrice;
				m_lMmBidSizeCache = lSize;
				m_lBidOpenQuantityCache = lOpenQuantity;
				m_lBidStepUpBufferCache = lStepUpBuffer;
			}

			m_dMmAskCache = dPrice;
			m_lMmAskSizeCache = lSize;
			m_lAskOpenQuantityCache = lOpenQuantity;
			m_lAskStepUpBufferCache = lStepUpBuffer;
		}
	}

	void	UpdateQuoteCache(const BidAskEnum BidOrAsk, 
							 const double&  dPrice,
							 const long		lSize,
							 const long		lOpenQuantity,
							 const long		lStepUpBuffer,
							 const long		lTickLevel)
	{
		if(BidOrAsk == enBid)
		{
			m_dMmBidCache = dPrice;
			m_lMmBidSizeCache = lSize;
			m_lBidOpenQuantityCache = lOpenQuantity;
			m_lBidStepUpBufferCache = lStepUpBuffer;
			m_lBidTickLevel = lTickLevel;
		}
		else 
		{
			if(BidOrAsk == enBoth)
			{
				m_dMmBidCache = dPrice;
				m_lMmBidSizeCache = lSize;
				m_lBidOpenQuantityCache = lOpenQuantity;
				m_lBidStepUpBufferCache = lStepUpBuffer;
				m_lBidTickLevel = lTickLevel;
			}

			m_dMmAskCache = dPrice;
			m_lMmAskSizeCache = lSize;
			m_lAskOpenQuantityCache = lOpenQuantity;
			m_lAskStepUpBufferCache = lStepUpBuffer;
			m_lAskTickLevel = lTickLevel;
		}
	}


	bool	IsCached(const BidAskEnum BidOrAsk)
	{
		if(BidOrAsk == enBid)
		{
			if(m_lBidTickLevel != -1)
				return true;
		}
		else if(BidOrAsk == enAsk)
		{
			if(m_lAskTickLevel != -1)
				return true;
		}
		else
			EgAssert(false);

		return false;
	}

	bool	GetQuoteCache(const BidAskEnum BidOrAsk, CQuoteParams& Quote)
	{
		Quote.m_BidOrAsk = BidOrAsk;

		return GetQuoteCache(BidOrAsk, Quote.m_dPremium, Quote.m_lQuantity, 
			Quote.m_lOpenQuantity, Quote.m_lStepUpBuffer);
	}

	bool	GetQuoteCache(const BidAskEnum BidOrAsk, CQuoteParams& Quote, long& lTickLevel)
	{
		Quote.m_BidOrAsk = BidOrAsk;

		return GetQuoteCache(BidOrAsk, Quote.m_dPremium, Quote.m_lQuantity, 
			Quote.m_lOpenQuantity, Quote.m_lStepUpBuffer, lTickLevel);
	}

	bool	GetQuoteCache(const BidAskEnum BidOrAsk, 
						  double&  dPrice,
						  long&	   lSize,
						  long&	   lOpenQuantity,
						  long&	   lStepUpBuffer) const
	{
		if(BidOrAsk == enBid)
		{
			if(m_lBidTickLevel != -1)
			{
				dPrice = m_dMmBidCache;
				lSize = m_lMmBidSizeCache;
				lOpenQuantity = m_lBidOpenQuantityCache;
				lStepUpBuffer = m_lBidStepUpBufferCache;
				return true;
			}
		}
		else if(BidOrAsk == enAsk)
		{
			if(m_lAskTickLevel != -1)
			{
				dPrice = m_dMmAskCache;
				lSize = m_lMmAskSizeCache;
				lOpenQuantity = m_lAskOpenQuantityCache;
				lStepUpBuffer = m_lAskStepUpBufferCache;
				return true;
			}
		}
		else
			EgAssert(false);

		dPrice = 0;
		lSize = 0;
		lOpenQuantity = 0;
		lStepUpBuffer = 0;

		return false;
	}

	bool	GetQuoteCache(const BidAskEnum BidOrAsk, 
						  double&  dPrice,
						  long&	   lSize,
						  long&	   lOpenQuantity,
						  long&	   lStepUpBuffer,
						  long&	   lTickLevel) const
	{
		if(BidOrAsk == enBid)
		{
			if(m_lBidTickLevel != -1)
			{
				dPrice = m_dMmBidCache;
				lSize = m_lMmBidSizeCache;
				lOpenQuantity = m_lBidOpenQuantityCache;
				lStepUpBuffer = m_lBidStepUpBufferCache;
				lTickLevel = m_lBidTickLevel;
				return true;
			}
		}
		else if(BidOrAsk == enAsk)
		{
			if(m_lAskTickLevel != -1)
			{
				dPrice = m_dMmAskCache;
				lSize = m_lMmAskSizeCache;
				lOpenQuantity = m_lAskOpenQuantityCache;
				lStepUpBuffer = m_lAskStepUpBufferCache;
				lTickLevel = m_lAskTickLevel;
				return true;
			}
		}
		else
			EgAssert(false);

		dPrice = 0;
		lSize = 0;
		lOpenQuantity = 0;
		lStepUpBuffer = 0;
		lTickLevel = 0;

		return false;
	}



public:

	const series_t	m_Series;
	string		m_sFullname;
	string		m_sSymbol;
	string		m_sRoot;
	int32		m_iContractSize;
	char		m_cMonthCode;
	char		m_cStrikeCode;

	double		m_dStrikePrice;		// normal form

	CInstrument*				m_pInstrument;
	CInstrumentClass*			m_pInstrumentClass;
	CUnderlying*				m_pUnderlying;

	double		m_dClosePrice;
	double		m_dLastPrice;


	// price cache	
	uint32		m_uiBestBidSize;
	uint32		m_uiBestAskSize;
	double		m_dBestBidPremium;
	double		m_dBestAskPremium;

	// price information from IQ101
	uint32		m_uiTimeReceivedBid;
	uint32		m_uiTimeReceivedAsk;
	uint32		m_uiVolume;
	uint32		m_uiTurnover;
	uint32		m_uiOpenBalance;
	int32		m_iClosingPrice;
	int32		m_iOpeningPrice;
	int32		m_iHighPrice;
	int32		m_iLowPrice;
	int32		m_iLastPrice;
	uint16		m_uiFastMarketLevel;
	uint8		m_uiTradingStatus;
	uint8		m_uiCountryBid;
	uint8		m_uiCountryAsk;

	// additional MM info from BO102
    uint32		m_uiBidMarketOrderSize; // # of contracts, with "at Market" flavour, buy side
	uint32		m_uiAskMarketOrderSize;
	uint16		m_nMmNumber;			// preopen, # of MMs quoting in side with most quotes

	// OPRA - BO103
	uint32		m_uiOpraBestBidSize;
	uint32		m_uiOpraBestAskSize;
	int32		m_iOpraBestBidPremium;
	int32		m_iOpraBestAskPremium;
	
public:

	bool operator < (const CSeries& Origin) const
	{
		return memcmp(&m_Series, &Origin.m_Series, sizeof(series_t)) < 0;//@
	}
};

class CSeriesDatabase : public CLock
{
	friend class CCountryMarket;
	friend class CISEData;

private:

	set<CSeries>							m_setData;
	map<string, set<CSeries>::iterator>		m_mapIndex;
	
public:

	typedef set<CSeries>::iterator itSeriesT;

	void	Add(const CSeries& Data)
	{
		EgAssert(m_setData.find(Data) == m_setData.end());
		pair<set<CSeries>::iterator, bool> it = m_setData.insert(Data);
		m_mapIndex[Data.m_sSymbol] = it.first;
	}

	CSeries* FindBySeries(const series_t& IseSeries)
	{
		CSeries Series(IseSeries);
		
		itSeriesT it = m_setData.find(Series);
		if(it != m_setData.end())
			return &(*it);
		else
			return NULL;
	}

	bool    DeleteBySeries(const series_t& IseSeries)
	{
		CSeries Series(IseSeries);
		
		itSeriesT it = m_setData.find(Series);
		if(it != m_setData.end())
		{
			map<string, set<CSeries>::iterator>::iterator itIndex;
			for(itIndex = m_mapIndex.begin(); itIndex != m_mapIndex.end(); itIndex++)
			{
				if(itIndex->second == it)
				{
					m_mapIndex.erase(itIndex);
					break;
				}
			}

			m_setData.erase(it);
			return true;
		}
		
		return false;
	}

	CSeries* FindBySymbol(const string& sSymbol)
	{
		map<string, set<CSeries>::iterator>::iterator itIndex = m_mapIndex.find(sSymbol);

		if(itIndex != m_mapIndex.end())
			return &(*(itIndex->second));

		return NULL;
	}

	void Clear()
	{
		m_setData.clear();
		m_mapIndex.clear();
	}
};

//----------------------------------[Underlying]--------------------------------------------

class CUnderlying
{
public:

	CUnderlying(uint16 uiCommodity)
	:	m_pInstrumentClass(NULL),
		m_uiTradingStatus(UTS_UNKNOWN),
		m_uiBestBidSize(0),
		m_uiBestAskSize(0),
		m_dBestBidPremium(0),
		m_dBestAskPremium(0),
		m_dClosePrice(0),
		m_dLastPrice(0),
		m_uiCommodity(uiCommodity)
	{
	}

public:

	string	m_sSymbol;			//The ASCII representation of the UNDERLYING CODE.

#ifdef OMEXVER16_5	
	uint64	m_uqNominalValue;	
	uint64	m_uqPositionLimit;	//The position limit is across the whole underlying.
#else
	int64_t	m_uqNominalValue;	
	int64_t	m_uqPositionLimit;	//The position limit is across the whole underlying.
#endif

	uint32	m_uiCouponInterest; //A decimal value stored with 6 decimals, e.g. 11% is stored as 110000.
	const uint16	m_uiCommodity;
	uint16	m_uiDaysCount;		//The number of days in a year used for interest rate calculation.
	uint16  m_uiDaysInInterestYear;//The number of days in a coupon period used for interest rate calculation.
	uint16	m_uiSettlementDaysAtCoupon;//Number of settlement days at coupon.
	uint8	m_uiBin;			//Collection of Underlyings. All underlyings belonging 
								//to the same bin will have the same unique bin number. 
								//Market Makers are associated with bins.
	uint8	m_uiOrderbook;		//The identity of a Marketplace partition. Used
								//for routing transactions and queries to the Marketplace.

	uint8	m_uiType;			//UT_*
	uint8	m_uiPriceUnit;		//UPU_*
	string	m_sCusip;
	string	m_sFullname;		
	string	m_sReleaseDate;		//YYYYMMDD.
	string	m_sTerminationDate;	//YYYYMMDD.
	string	m_sCurrency;		//S.W.I.F.T. handbook and the ISO 3166 standard, 
								//e.g. SEK, GBP, USD and ATS.
	string	m_sPrmMmCustomer;	//This is a combined field containing the following fields:
								//- COUNTRY NAME
								//- CUSTOMER IDENTITY
	TCouponsVector			m_vecCoupons;
	TFastMarketLevelsVector	m_vecFastMarketLevels;

	CInstrumentClass*		m_pInstrumentClass;
	vector<CSeriesDatabase::itSeriesT>		m_Series;		// Derivatives

	uint8	m_uiTradingStatus;	// UTS_* constants
	

	uint16	m_uiDecInPrice;		// implied decimals in field


	// price information from IQ121

	// price cache	
	uint32		m_uiBestBidSize;
	uint32		m_uiBestAskSize;
	double		m_dBestBidPremium;
	double		m_dBestAskPremium;
	double		m_dClosePrice;
	double		m_dLastPrice;

	int32		m_iClosingPrice;
	int32		m_iOpeningPrice;
	int32		m_iHighPrice;
	int32		m_iLowPrice;
	int32		m_iLastPrice;
	uint32		m_uiVolume;
	uint32		m_uiTurnover;
	uint8		m_uiCountry;

public:

	bool operator < (const CUnderlying& Origin) const
	{
		return m_uiCommodity < Origin.m_uiCommodity;
	}
};

class CUnderlyingDatabase : public CLock
{
	friend class CCountryMarket;
	friend class CISEData;
	friend class CISESession;

private:

	set<CUnderlying>	m_setData;
	map<string, set<CUnderlying>::iterator>		m_mapIndex;

public:

	void Add(const CUnderlying& Data)
	{
		if(m_setData.find(Data) != m_setData.end())
		{
			EgTrace("Underlying with commodity = %d already exists", (int)Data.m_uiCommodity);
		};

		pair<set<CUnderlying>::iterator, bool> it = m_setData.insert(Data);
		m_mapIndex[Data.m_sSymbol] = it.first;
	}

	CUnderlying* FindByCommodity(const uint16 uiCommodity)
	{
		CUnderlying Underlying(uiCommodity);
		
		set<CUnderlying>::iterator it = m_setData.find(Underlying);
		if(it != m_setData.end())
			return &(*it);
		else
			return NULL;
	}

	bool DeleteByCommodity(const uint16 uiCommodity)
	{
		EgTrace("Deleting by commodity = %d", (int )uiCommodity);

		CUnderlying Underlying(uiCommodity);
		
		set<CUnderlying>::iterator it = m_setData.find(Underlying);
		if(it != m_setData.end())
		{
			map<string, set<CUnderlying>::iterator>::iterator itIndex;
			for(itIndex = m_mapIndex.begin(); itIndex != m_mapIndex.end(); itIndex++)
			{
				if(itIndex->second == it)
				{
					m_mapIndex.erase(itIndex);
					break;
				}
			}

			m_setData.erase(it);
			return true;
		}

		return false;
	}

	CUnderlying* FindBySymbol(const string& sSymbol)
	{
		map<string, set<CUnderlying>::iterator>::iterator itIndex = m_mapIndex.find(sSymbol);

		if(itIndex != m_mapIndex.end())
			return &(*(itIndex->second));

		return NULL;
	}

	void Clear()
	{
		m_setData.clear();
		m_mapIndex.clear();
	}
};

//----------------------------------[Instrument class]--------------------------------------------

class CInstrumentClass
{
public:

	bool operator < (const CInstrumentClass& Origin) const
	{
		/*
		Keeping in mind that the defining properties of an instrument class are
		country, market, underlying, and instrument group,
		we just sort on those. We should NEVER have duplicates.
		*/
		int iCompare;

		iCompare = m_Series.country_c - Origin.m_Series.country_c;
		if(iCompare)
			return iCompare < 0;

		iCompare = m_Series.market_c - Origin.m_Series.market_c;
		if(iCompare)
			return iCompare < 0;

		iCompare = m_Series.commodity_n - Origin.m_Series.commodity_n;
		if(iCompare)
			return iCompare < 0;

		return (m_Series.instrument_group_c < Origin.m_Series.instrument_group_c);
	}

public:

	series_t	m_Series;
	upper_level_series_t	m_ParentSeries;		//The Series (normally the equity) that the
												//derivative is based on. Filled with null, if
												//derivative level = 0.
	string		m_sCurrency;
	int32		m_iContractSize;

	int32		m_iBlockOrderMinSize;
	int32		m_iBlockOrderResponceTime;

#ifdef OMEXVER16_5	
	int32		m_iFacilitationOrderMinSize;
	int32		m_iFacilitationOrderResponceTime;
#endif

	int32		m_iExercizeLimit;				//Not used by ISE.
	uint8		m_uiExercizeLimitUnit;			//Not used by ISE.
	int32		m_iBboVolume;					//This is the minimum volume required at the
												//best bid or offer.

	//The lower limit of the different strike price groups. 
	int16		m_iSpg1LowLimit;				//Group 1 is "Deep in the money".
	int16		m_iSpg2LowLimit;				//Group 2 is "In the money".
	int16		m_iSpg3LowLimit;				//Group 3 is "At the money".
	int16		m_iSpg4LowLimit;				//Group 4 is "Out of the money".
	int16		m_iSpg5LowLimit;				//Group 5 is "Deep out of the money".
	uint8		m_cTraded;						//whether corresponding Series for the Instrument Class are traded://ISEBOOL_
	uint16		m_uiDeriveLevel;				//0=underlying, 1 == derivative on underlying, 2 == derivative on derivative

	vector<uint16>	m_vecLowLimits;				//Defines the lower limit for the expiration group.
	uint16		m_uiRanking;					//Identifies what is ranked as being RANK_*
	int32		m_iPriceQuoteFactor;			//The factor is used to calculate the total price for a trade.
	
	vector<tick_size_t>	m_TickSizes;
	vector<tick_size_t>	m_MmTickSizes;

	// as per OM/ISE advice.. traded in fractions if first entry is 625, second is 1250
    // hopefully US industry gets on with it, and we don't run hybrid market for long
	bool		m_bTradesInDecimals;			// Computed by ourself

	uint16		m_uiTradingState;


	uint16		m_uiDecInStrikePrice;
	uint16		m_uiDecInContractSize;
	uint16		m_uiDecInPremium;

public:

	CInstrumentClass():m_uiTradingState(0){}
};

class CInstrumentClassDatabase : public CLock
{
	
	friend class CCountryMarket;

private:

	set<CInstrumentClass>	m_setData;

public:

	void	Add(CInstrumentClass& InstrumentClass)
	{
		EgAssert(m_setData.find(InstrumentClass) == m_setData.end());
		m_setData.insert(InstrumentClass);
	}

	CInstrumentClass* FindBySeries(const series_t& Series) 
	{
		CInstrumentClass	InstrumentClass;
		
		InstrumentClass.m_Series = Series;
		set<CInstrumentClass>::iterator it = m_setData.find(InstrumentClass);
		if(it != m_setData.end())
			return &(*it);
		else
			return NULL;
	}

	bool DeleteBySeries(const series_t& Series) 
	{
		CInstrumentClass	InstrumentClass;
		
		InstrumentClass.m_Series = Series;
		set<CInstrumentClass>::iterator it = m_setData.find(InstrumentClass);
		if(it != m_setData.end())
		{
			m_setData.erase(it);
		}
		
		return false;
	}

	void Clear()
	{
		m_setData.clear();
	}
};

//----------------------------------[BIN]--------------------------------------------

class CBin
{
	friend class CCountryMarket;

public:

	uint8			m_uiID;
	string			m_sPMM;
	vector<string>	m_vecCMMs;

public:

	bool operator < (const CBin& Origin) const
	{
		return m_uiID < Origin.m_uiID;
	}
};

class CBinDatabase : public CLock
{
	friend class CCountryMarket;
	friend class CISEData;
	friend class CISESession;

private:

	set<CBin>		m_setData;

public:

	void Add(const CBin& Bin)
	{
		m_setData.insert(Bin);
	}

	CBin* FindByID(const uint8 uiBinID)
	{
		CBin	Bin;
		Bin.m_uiID = uiBinID;

		set<CBin>::iterator it = m_setData.find(Bin);
		if(it != m_setData.end())
			return &(*it);
		else
			return NULL;
	}

	void Clear()
	{
		m_setData.clear();
	}
};

//----------------------------------[Instrument]--------------------------------------------

/* country code. In much of the world, there is only one
equities exchange for each country, and hence a single code
allocated to each exchange was 1:1 with country...
That model fails in the US, so the name of this field is
somewhat misleading...the field would, perhaps (now) be more
accurate if it was called exchange, rather than country...
Some examples include :-
	10 == AMEX, American Stock Exchange/Nasdaq
	15 == ASX, Australian Stock Exchange (national)
	20 == ISE, International Securities Exchange
*/
class CCountryMarket 
{
public:
	
	uint8	m_uiCountry;
	uint8	m_uiMarket;
	string	m_sName;
	uint32	m_uiBinsCount;
	uint32	m_uiOrderBookCount;
	uint16	m_uiTradingState;
	
	uint8			m_uiDefaultBin;
	TraderRoleEnum	m_TraderRoleInDefaultBin;

	// All underlyings	
	CUnderlyingDatabase			m_Underlyings;
	CInstrumentClassDatabase	m_InstrumentClasses;
	CInstrumentsDatabase		m_Instruments;
	CSeriesDatabase				m_Series;
	CBinDatabase				m_Bins;

	bool	UpdateInstrumentClasses(CISEData* pAllData);
	bool	UpdateMarketStructure(CISEData* pAllData);
	bool	UpdateDefaultBin(CISEData* pAllData);

	CCountryMarket():m_uiBinsCount(0), m_uiOrderBookCount(0), m_uiTradingState(0),
		m_uiDefaultBin(0){}

	void Clear()
	{
		m_uiDefaultBin = 0;
		m_Underlyings.Clear();
		m_InstrumentClasses.Clear();
		m_Instruments.Clear();
		m_Series.Clear();
		m_Bins.Clear();
	}
};

#endif