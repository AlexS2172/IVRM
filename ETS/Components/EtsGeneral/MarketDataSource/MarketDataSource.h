#pragma once
#include "MarketDataProvider.h"

class CMarketDataSource: public CMarketDataProvider
{
public:
	CMarketDataSource(void);
	virtual ~CMarketDataSource(void);

public:
	CQuoteTickerMapPtr	m_spSubscribed;
	CQuoteTickerMapPtr	m_spRequested;
	CQuoteTickerMapPtr	m_spRealTimeQuotes;

	CLock	m_csRisksLock;
	CLock	m_csQuotesLock;

	CLock	m_csRealTimeQuotesLock;
	CLock	m_csRealTimeRisksLock;

public:

	virtual void	Connect();
	virtual void	Disconnect();
public:
	void	OnLastQuote(const CTicker& ticker, const CQuote& quote);
	void	OnLastQuoteDone();
	void	OnLastQuoteError(ErrorNumberEnum enErrorNumber, BSTR bsDescription, const CTicker& ticker);

	void	OnError(ErrorNumberEnum enErrorNumber, BSTR bsDescription );
	void	OnDisconnect( ErrorNumberEnum enErrorNumber, BSTR bsDescription);
	
	void	OnUnsubscribed();
	
	virtual void	DoCalc() = 0;
	void			OnQuoteUpdate();
	bool			SubscribeQuotes(const CTicker& ticker);
	void			AcceptQuote(const CTicker& ticker, const CQuote& quote);
	void			OnSubscribed(const CTicker& ticker);
	
	CQuoteTickerMapPtr	GetRealTimeQuotes();
	
	CQuotePtr		GetQuote(const CTicker& ticker);

	void			ProcessRequests(bool wait);
	void			CheckRequestsProcesed();

public:
	virtual void	DoRisksUpdate() = 0;
	void			OnRisksUpdate();
	bool			SubscribeRisks(const CTicker& ticker);
	void			AcceptRisks(const CTicker& ticker, const CRisks& risks);
	void			OnSubscribedRisks(const CTicker& ticker);

	CRisksTickerMapPtr	GetRealTimeRisks();

	CRisksTickerMapPtr	m_spSubscribedRisks;
	CRisksTickerMapPtr	m_spRequestedRisks;
	CRisksTickerMapPtr	m_spRealTimeRisks;

public:
	HANDLE	m_hSubscribtionThread;
	static UINT WINAPI SubscribtionThread(LPVOID pData);
	HANDLE	m_hStopSubscribtionEvent;
	HANDLE	m_hProcessRequestsEvent;
	HANDLE	m_hRequestsProcessedEvent;
	void StartSubscribtionThread();
	void StopSubscribtionThread();
};

typedef	boost::shared_ptr<CMarketDataSource> CMarketDataSourcePtr;
