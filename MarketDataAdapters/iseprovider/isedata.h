#ifndef ISEDATA_H
#define ISEDATA_H

#include "ISEConst.h"
#include "ISEMessage.h"
#include "ISEUser.h"
#include "ISEDataTypes.h"
#include "ISEEvents.h"
#include "ExchangesMapping.h"

/*-------------------------Classes for static data retrieval-----------------------------*/

class CISESession;

class CSessionParameters	
{
public:
	CISESession*	m_pSession;
	uint32			m_uiBaseFacility;
};

template<class _QueryClass, class _AnswerType, uint32 _Facility>
class TSegmNonPartQuery : virtual public CSessionParameters, virtual public CISEEvents
{
protected:

	virtual bool	OnData(const _AnswerType* Answer, DWORD& dwCount) = 0;

	void	Load(const char* const szDesc, 
		const series_t* const pFilter = NULL,
		_QueryClass* pQuery = NULL) throw (CISEException);
};

// absence of specialization in Microsoft Visual C++
template<class _QueryClass, class _AnswerType, uint32 _Facility>
class TSegmNonPartQuery2 : virtual public CSessionParameters, virtual public CISEEvents
{
protected:

	virtual bool	OnData(const _AnswerType* Answer, DWORD& dwCount) = 0;

	void	Load(const char* const szDesc, 
		const series_t* const pFilter = NULL,
		_QueryClass* pQuery = NULL) throw (CISEException);
};

/*---------------------------Static data classes declarations-----------------------------*/

typedef TSegmNonPartQuery<CISEQueryInstrumentGroup, answer_instrument_group_da208_t, FACILITY_EP0>	CInstrumentGroupData;
typedef TSegmNonPartQuery<CISEQueryInstrument, answer_instrument_da203, FACILITY_EP0>	CInstrumentData;
typedef TSegmNonPartQuery<CISEQueryMarket, answer_market_da207_t, FACILITY_EP0>	CMarketData;
typedef TSegmNonPartQuery<CISEQueryExchange, answer_exchange_da24, FACILITY_EP0>	CExchangeData;
typedef TSegmNonPartQuery<CISEQueryTradingState, answer_trading_state_da245, FACILITY_EP0>	CTradingStateData;
typedef TSegmNonPartQuery<CISEQueryUnderlying, answer_underlying_da204, FACILITY_EP0>	CUnderlyingData;
typedef TSegmNonPartQuery<CISEQueryInstrumentClass, answer_instrument_class_da210, FACILITY_EP0>	CInstrumentClassData;
typedef TSegmNonPartQuery<CISEQuerySeries, answer_series_da202, FACILITY_EP0>	CSeriesData;
typedef TSegmNonPartQuery<CISEQueryBrokers, answer_broker_da206, FACILITY_EP0>	CBrokersData;
typedef TSegmNonPartQuery<CISEQueryMmBin, answer_bin_da211, FACILITY_EP0>	CMmBinData;
typedef TSegmNonPartQuery<CISEQueryUnderlyingTradingStatus, answer_underlying_status_us, FACILITY_EP1>	CUnderlyingTradingStatusData;
typedef TSegmNonPartQuery<CISEQueryInstrumentTradingStatus, answer_instrument_status, FACILITY_EP1>	CInstrumentTradingStatusData;
typedef TSegmNonPartQuery2<CISEQueryGeneralMessage, answer_bi8_broadcasts_sent, FACILITY_EP1>	CGeneralMessageData;


class CISEData :
	public CInstrumentGroupData,
	public CInstrumentData,
	public CMarketData,
	public CExchangeData,
	public CTradingStateData,
	public CUnderlyingData,
	public CInstrumentClassData,
	public CSeriesData,
	public CBrokersData,
	public CMmBinData,
	public CUnderlyingTradingStatusData,
	public CInstrumentTradingStatusData,
	public CGeneralMessageData,
	public CLock
{
// data
public:

	CCountryMarket			m_ISE;
	CExchangesDatabase		m_Exchanges;
	CTradingStatesDatabase	m_TradingStates;


private:
// overrides...
	bool	OnData(const answer_instrument_group_da208_t* pInstrumentGroup, DWORD& dwCount);
	bool	OnData(const answer_instrument_da203_t* pData, DWORD& dwCount);
	bool	OnData(const answer_market_da207_t* pData, DWORD& dwCount);
	bool	OnData(const answer_exchange_da24* pData, DWORD& dwCount);
	bool	OnData(const answer_trading_state_da245_t* pData, DWORD& dwCount);
	bool	OnData(const answer_underlying_da204* pData, DWORD& dwCount);
	bool	OnData(const answer_instrument_class_da210* pData, DWORD& dwCount);
	bool	OnData(const answer_series_da202* pData, DWORD& dwCount);
	bool	OnData(const answer_broker_da206* pData, DWORD& dwCount);
	bool	OnData(const answer_bin_da211* pData, DWORD& dwCount);
	bool	OnData(const answer_underlying_status_us* pData, DWORD& dwCount);
	bool	OnData(const answer_instrument_status* pData, DWORD& dwCount);
	bool	OnData(const answer_bi8_broadcasts_sent* pData, DWORD& dwCount);


// data modification helpers
	void	UpdateUnderlying(const da204_t& Data, CUnderlying& Underlying);
	void	UpdateInstrumentClass(const da210_t& Data, CInstrumentClass& InstrumentClass);
	void	UpdateSeries(const da202_t& Data, CSeries& Series);

private:

	bool				m_bLoaded;
	
	CExchangesMapping	m_mapExchangeCodes;

protected:
// helpers
	CUnderlying* GetUnderlyingByRequest(const CSymbolParams& Req);
	CUnderlying* GetUnderlyingByRequest(const string& sReq);
	CSeries*	 GetSeriesByRequest(const CSymbolParams& Req);
	CSeries*	 GetSeriesByRequest(const string& sReq);
	bool		 PrepareBBO(const CSeries* pSeries, CSymbolParams& Symbol, CQuoteBBO& BBO);
	bool		 PrepareBBO(const CUnderlying* pUnd, CSymbolParams& Symbol, CQuoteBBO& BBO);
	void		 FireBBO(const CSeries* pSeries);
	void		 FireBBO(const CUnderlying* pUnd);
	
	void	Clear()
	{
		CAutoLock	DataLock(this);

		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);
		CAutoLock   GroupLock(&m_ISE.m_Instruments);
		CAutoLock	ExchangesLock(&m_Exchanges);
		CAutoLock	BinsLock(&m_ISE.m_Bins);
		CAutoLock	TradingStatesLock(&m_TradingStates);

		m_bLoaded = false;

		m_ISE.Clear();
		m_Exchanges.Clear();
		m_TradingStates.Clear();
	};

// Market Structure

	// Underlying
	void	AddUnderlying(const da204_t& Data);
	void	DeleteUnderlying(const da204_t& Data);
	void	ModifyUnderlying(const da204_t& Data);

	// Class
	void	AddInstrumentClass(const da210_t& Data);
	void	DeleteInstrumentClass(const da210_t& Data);
	void	ModifyInstrumentClass(const da210_t& Data);
	
	// Series
	void	AddSeries(const da202_t& Data);
	void	DeleteSeries(const da202_t& Data);
	void	ModifySeries(const da202_t& Data);

	// Status
	void	ModifyInstrumentStatus(const series_t& Data, const uint16 uiState, const uint16 uiTimeout = 0, const char* const szText = NULL);
	void	ModifySeriesStatus(const series_status_change_bi103_item_t& Data);
	void	ModifyUnderlyingStatus(const underlying_trading_status_bi132_t& Data);

	// Text information
	void	AddGeneralMessage(const uint8 uiCountry, const uint8 uiMarket, const string& sText);

	// BBO

	void	ModifySeriesBBO(const best_bid_offer_bo101_t& Data);

	void	ModifySeriesExBBO(const best_bid_offer_extended_bo102_t& Data);
	
	void	ModifyOpraSeriesBBO(const opra_best_bid_offer_bo103_t& Data);
	
	void	ModifyOpraEachExchangeSeriesBBO(const opra_all_best_bid_offer_bo104_t& Data);
	
	void	ModifyUnderlyingBBO(const underlying_best_bid_offer_bo105_t& Data);


public:

	CISEData():m_bLoaded(false){}

	void	Load(CISESession*	pSession) throw (CISEException);
	bool	IsLoaded() const {return m_bLoaded;}

	bool	QueryAllUnderlyingsInfo(vector<CUnderlyingInfo>& Infos) throw (CISEException);
	bool	QueryUnderlyingInfo(const CSymbolParams& Req, CUnderlyingInfo& Info) throw (CISEException);
	bool	QueryUnderlyingOptionsInfo(const CSymbolParams& Req, vector<COptionInfo>& vecInfos) throw (CISEException);

	bool	QueryUnderlyingBBO(const CSymbolParams& Req, CQuoteBBO& BBO) throw (CISEException);
	bool	QueryOptionBBO(const CSymbolParams& Req, CQuoteBBO& BBO) throw (CISEException);
	bool	QueryGroupOptionsBBO(const CSymbolParams& Req, map<CSymbolParams, CQuoteBBO>& mapSeriesBBO) throw (CISEException);
	
	bool	QueryMmQuotes(const CMMQuoteParams& Req, set<CQuoteParams>& setQuotes) throw (CISEException);
};


#endif