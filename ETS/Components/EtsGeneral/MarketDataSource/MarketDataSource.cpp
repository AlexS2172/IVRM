#include "StdAfx.h"
#include "MarketDataSource.h"
#include <algorithm>
//-------------------------------------------------------------------------------------------------------------------//
CMarketDataSource::CMarketDataSource(void)
{
	m_spSubscribed = CQuoteTickerMapPtr(new CQuoteTickerMap());	
	m_spRequested = CQuoteTickerMapPtr(new CQuoteTickerMap());
	m_spRealTimeQuotes = CQuoteTickerMapPtr(new CQuoteTickerMap());

	m_spRequested->clear();
	m_spSubscribed->clear();
	m_spRealTimeQuotes->clear();

	m_spSubscribedRisks = CRisksTickerMapPtr(new CRisksTickerMap());
	m_spRequestedRisks = CRisksTickerMapPtr(new CRisksTickerMap());
	m_spRealTimeRisks = CRisksTickerMapPtr(new CRisksTickerMap());

	m_spSubscribedRisks->clear();
	m_spRequestedRisks->clear();
	m_spRealTimeRisks->clear();

	m_hSubscribtionThread =	NULL;
};
//-------------------------------------------------------------------------------------------------------------------//
CMarketDataSource::~CMarketDataSource(void)
{

};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnLastQuote(const CTicker& pReq, const CQuote& pResult)
{
	
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnLastQuoteDone()
{

};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnLastQuoteError(ErrorNumberEnum enErrorNumber, BSTR bsDescription, const CTicker& ticker)
{

};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnError(ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnDisconnect(ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnSubscribed(const CTicker& ticker)
{
	CAutoLock lLock(m_csQuotesLock);

	CQuoteTickerMap::iterator it = m_spRequested->find(ticker);

	if (it != m_spRequested->end())
	{
		CString strInfo;
		strInfo.Format(_T("Symbol: [%s] Type: [%i]"),
			ticker.m_sSymbol.c_str(),
			static_cast<long>(ticker.m_enType));

		TRACE_DEBUG(_TEXT("Subscribed: %s"), strInfo);

		m_spSubscribed->insert(QTPair(ticker, it->second));
		m_spRequested->erase(it);
	};

	CheckRequestsProcesed();
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnSubscribedRisks(const CTicker& ticker)
{
	CAutoLock lLock(m_csRisksLock);

	CRisksTickerMap::iterator it = m_spRequestedRisks->find(ticker);

	if (it != m_spRequestedRisks->end())
	{
		TRACE_DEBUG(_TEXT("Symbol: [%s]"), ticker.m_sSymbol.c_str());
		m_spSubscribedRisks->insert(RTPair(ticker, it->second));
		m_spRequestedRisks->erase(it);
	};

	CheckRequestsProcesed();
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnUnsubscribed()
{	

	{
		CAutoLock subLock(m_csRisksLock);
		m_spSubscribed->clear();
		m_spRequested->clear();
	}

	{
		CAutoLock lock(m_csRealTimeQuotesLock);
		m_spRealTimeQuotes->clear();
	}

	{
		CAutoLock lock(m_csRealTimeRisksLock);
		m_spRealTimeRisks->clear();
	}
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnQuoteUpdate()
{
	ATLTRACE("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	DoCalc();
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::OnRisksUpdate()
{
	ATLTRACE("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");	
	DoRisksUpdate();
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::AcceptQuote(const CTicker& ticker, const CQuote& quote)
{
	CAutoLock lLock(m_csQuotesLock);

	CString strInfo;
	strInfo.Format(_T("Symbol: %s Type: %i Bid= %.2f Ask= %.2f Last= %.2f"),
					ticker.m_sSymbol.c_str(),
					static_cast<long>(ticker.m_enType),
					quote.BidPrice,
					quote.AskPrice,
					quote.LastPrice);

	TRACE_DEBUG(_TEXT("QuoteUpdate for: [%s]"), strInfo);

	CQuoteTickerMap::iterator it = m_spSubscribed->find(ticker);
	if(it != m_spSubscribed->end())
	{
		if ((bool)(it->second))
		{

			quote.CopyTo(*(it->second));
			
			CString strInfo;
			strInfo.Format(_T("Symbol: %s Bid= %.2f Ask= %.2f Last= %.2f"),
							ticker.m_sSymbol.c_str(),
							quote.BidPrice,
							quote.AskPrice,
							quote.LastPrice);

			TRACE_DEBUG(_TEXT("Accept quotes for: [%s]"), strInfo);

			//update collection of fresh RealTime quotes
			m_spRealTimeQuotes->insert(QTPair(ticker, it->second));
		};
	};
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::AcceptRisks(const CTicker& ticker, const CRisks& risks)
{
	CAutoLock lLock(m_csRisksLock);

	CRisksTickerMap::iterator it = m_spSubscribedRisks->find(ticker);
	if(it != m_spSubscribedRisks->end())
	{
		if ((bool)(it->second))
		{

			risks.CopyTo(*(it->second));

			CString strInfo;
			strInfo.Format(_T("Symbol: %s IVol= %.2f TVol= %.2f Thv= %.2f"),
							ticker.m_sSymbol.c_str(),
							risks.dIVola,
							risks.dTheoVola,
							risks.dTheoPrice);

			TRACE_DEBUG(_TEXT("Accept risks for: [%s]"), strInfo);

			//update collection of fresh RealTime quotes
			m_spRealTimeRisks->insert(RTPair(ticker, it->second));
		};
	};
};
//-------------------------------------------------------------------------------------------------------------------//
CQuoteTickerMapPtr CMarketDataSource::GetRealTimeQuotes()
{
	CAutoLock lock(m_csRealTimeQuotesLock);

	m_spRealTimeQuotes->clear();
	CMarketDataProvider::GetQuoteUpdate();

	return m_spRealTimeQuotes;
};
//-------------------------------------------------------------------------------------------------------------------//
CRisksTickerMapPtr CMarketDataSource::GetRealTimeRisks()
{
	CAutoLock lock(m_csRealTimeRisksLock);

	m_spRealTimeRisks->clear();
	CMarketDataProvider::GetRisksUpdate();

	return m_spRealTimeRisks;
};
//-------------------------------------------------------------------------------------------------------------------//
CQuotePtr	CMarketDataSource::GetQuote(const CTicker& ticker)
{
	CAutoLock lLock(m_csQuotesLock);

	CQuoteTickerMap::iterator it = m_spSubscribed->find(ticker);
	
	if (it == m_spSubscribed->end())
		return	CQuotePtr();

	return	it->second;		
};
//-------------------------------------------------------------------------------------------------------------------//
bool	CMarketDataSource::SubscribeQuotes(const CTicker& ticker)
{
	CAutoLock lLock(m_csQuotesLock);
	
	if (m_spSubscribed->find(ticker) == m_spSubscribed->end())
	{
		if (m_spRequested->find(ticker) == m_spRequested->end())
		{
			m_spRequested->insert(QTPair(CTicker(ticker), CQuotePtr(new CQuote())));

			CString strInfo;
			strInfo.Format(_T("Symbol: [%s] Type: [%i]"),
							ticker.m_sSymbol.c_str(),
							static_cast<long>(ticker.m_enType));

			TRACE_DEBUG(_TEXT("Subscribe: [%s]"), strInfo);
		}
		return true;
	}	
	return false;
};
//-------------------------------------------------------------------------------------------------------------------//
bool	CMarketDataSource::SubscribeRisks(const CTicker& ticker)
{
	CAutoLock subLock(m_csRisksLock);

	if (m_spSubscribedRisks->find(ticker) == m_spSubscribedRisks->end())
	{
		if (m_spRequestedRisks->find(ticker) == m_spRequestedRisks->end())
		{
			m_spRequestedRisks->insert(RTPair(CTicker(ticker), CRisksPtr(new CRisks())));	
		}
		return true;
	}	
	return false;
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::StartSubscribtionThread()
{
	try
	{
		unsigned int lThreadID = 0L;
		m_hStopSubscribtionEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hProcessRequestsEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hRequestsProcessedEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hSubscribtionThread  = (HANDLE)_beginthreadex(NULL, 0, SubscribtionThread, this, 0, &lThreadID);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::StopSubscribtionThread()
{
	try
	{
		::SetEvent(m_hStopSubscribtionEvent);
		if (m_hSubscribtionThread)
		{
			if(::WaitForSingleObject(m_hSubscribtionThread, 5000) != WAIT_OBJECT_0)
			{
				::TerminateThread(m_hSubscribtionThread, -1);			
			}
			::CloseHandle(m_hSubscribtionThread);
			
			m_hSubscribtionThread = NULL;

			::CloseHandle(m_hStopSubscribtionEvent);
			::CloseHandle(m_hProcessRequestsEvent);
			::CloseHandle(m_hRequestsProcessedEvent);
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::ProcessRequests(bool wait){
	
	::SetEvent(m_hProcessRequestsEvent);

	if (wait){
		::ResetEvent(m_hRequestsProcessedEvent);
		::WaitForSingleObject(m_hRequestsProcessedEvent, 100);
	}
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::CheckRequestsProcesed()
{
	if (m_spRequestedRisks->empty() && m_spRequested->empty())
		::SetEvent(m_hRequestsProcessedEvent);
};
//-------------------------------------------------------------------------------------------------------------------//
UINT WINAPI CMarketDataSource::SubscribtionThread(LPVOID pData)
{
	if (pData == NULL) return 0;

	try
	{
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);

		CMarketDataSource* pMarketDataSource = reinterpret_cast<CMarketDataSource*>(pData);

		if (pMarketDataSource)
		{
			HANDLE events[2];
			events[0] = pMarketDataSource->m_hStopSubscribtionEvent;
			events[1] = pMarketDataSource->m_hProcessRequestsEvent;

			while (true)
			{

				if (WAIT_OBJECT_0 == ::WaitForSingleObject(pMarketDataSource->m_hStopSubscribtionEvent, 15000)) break;
				
				//subscribe requested quotes
				{	/*Auto lock scope*/
					CAutoLock subLock(pMarketDataSource->m_csQuotesLock);
					if (!pMarketDataSource->m_spRequested->empty())
					{
						CQuoteTickerMap::iterator it = pMarketDataSource->m_spRequested->begin();
						CQuoteTickerMap::iterator itEnd = pMarketDataSource->m_spRequested->end();
						
						RequestVector request;
						for( ; it != itEnd; it++)
						{
							request.push_back(it->first);
						}
						pMarketDataSource->Subscribe(request);
						request.clear();
					}
				}

				//subscribe requested risks
				{	/*Auto lock scope*/
					CAutoLock subLock(pMarketDataSource->m_csRisksLock);
					if (!pMarketDataSource->m_spRequestedRisks->empty())
					{
						CRisksTickerMap::iterator it = pMarketDataSource->m_spRequestedRisks->begin();
						CRisksTickerMap::iterator itEnd = pMarketDataSource->m_spRequestedRisks->end();

						for( ; it != itEnd; it++)
						{
							pMarketDataSource->SubscribeRisksSingle(it->first);
						}
					}
				}
			}
		}

		::CoUninitialize();
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch(...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	
	return 0;
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::Connect()
{
	try
	{
		CMarketDataProvider::Connect();
		StartSubscribtionThread();
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
};
//-------------------------------------------------------------------------------------------------------------------//
void CMarketDataSource::Disconnect()
{
	try
	{
		StopSubscribtionThread();
		CMarketDataProvider::Disconnect();
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
};
//-------------------------------------------------------------------------------------------------------------------//