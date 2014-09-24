// HFServer.cpp: implementation of the CHFServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HFServer.h"
#include "Price.h"

LARGE_INTEGER CHFServer::m_lPerformanceFrequeny;
PerfMon::IPPBatchInfoPtr CHFServer::m_spBatchPerfMon;

ULONG  CHFServer::m_lClients     = 0;
ULONG  CHFServer::m_lRTClients   = 0;
volatile LONG  CHFServer::m_lRTResponse  = 0;


void CEngineQuoteMessage::DoTask(LPVOID, LPOVERLAPPED)
{
	try
	{
		m_pAdaptor->OnQuoteMessage(m_Key, &m_record);
	}
	catch (_com_error& e)
	{
	}
	catch (...)
	{
	}
}

void CEngineNotificationMessage::DoTask(LPVOID, LPOVERLAPPED)
{
	try
	{
		m_pAdaptor->OnNotificationMessage(m_enErrorType, m_strText);
	}
	catch (_com_error& e)
	{
	}
	catch (...)
	{
	}
}

void CEngineError::DoTask(LPVOID, LPOVERLAPPED)
{
	try
	{
		m_pAdaptor->OnError(m_lStatus);
	}
	catch (_com_error& e)
	{
	}
	catch (...)
	{
	}
}

void CEngineSubscribe::DoTask(LPVOID, LPOVERLAPPED)
{
	try
	{
		m_pAdaptor->OnSubscribe(m_pPrice);
	}
	catch (_com_error& e)
	{
	}
	catch (...)
	{
	}
}
void CEngineUnsubscribe::DoTask(LPVOID, LPOVERLAPPED)
{
	try
	{
		m_pAdaptor->OnUnsubscribe(m_pPrice);
	}
	catch (_com_error& e)
	{
	}
	catch (...)
	{
	}
}


PerfMon::IPPBatchInfoPtr CHFServer::GetGlobalPerfMonitor()
{
	return  m_spBatchPerfMon;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHFServer::CHFServer() 
:m_nStatus(DBA_ERR_NO_ATTACH)
,m_bTerminate(true)
,m_hThread(0)
,m_nCount(0)
,m_etiPtr(0)
,m_lProcessedQuotes(0)
//,m_Strategy(this)
{
	QueryPerformanceFrequency(&m_lPerformanceFrequeny);
	m_lPerformanceFrequeny.QuadPart /=1000;

	m_hEventStop.Attach(CreateEvent(NULL, TRUE, FALSE, NULL));
	m_Pool      .Initialize(m_hEventStop,	  1);

}

CHFServer::~CHFServer()
{
	if (m_hThread != 0)
		Terminate();
	_ASSERTE(m_setSubs.size() == 0);
}

//////////////////////////////////////////////////////////////////////
// CHFServer member functions

long CHFServer::Initialize()
{
	if (m_hThread != 0)
	{
		if (!m_bTerminate)
		{
			::InterlockedIncrement(&m_nCount);
			return m_nStatus = DBA_ERR_NO_ERROR;
		}
		else
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Waiting at HFServer.Initialize Start"));	
			::WaitForSingleObject(m_hThread, INFINITE);
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Waiting at HFServer.Initialize End"));	
		}
	}

	m_hThread = ::CreateEvent(0, FALSE, FALSE, 0);
	m_bTerminate = false;
	try
	{
		HRESULT __hr = S_OK;
		if(FAILED(__hr = m_spBatchPerfMon.CreateInstance(__uuidof(PerfMon::CPPBatchInfo))) || m_spBatchPerfMon == NULL)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSystem, __FUNCTION__ , _T("Unable to create PerfMon Object %d"),__hr);	
			m_spBatchPerfMon = NULL;
		}
		else
		{
			if(FAILED(__hr = m_spBatchPerfMon->Initialize(_bstr_t(L"__Total"))))
			{
				m_spBatchPerfMon = NULL;
				EgStd::CEgTracingClass::TraceStatic(enlogSystem, __FUNCTION__ , _T("Unable to Initialize PerfMon Object %d"),__hr);	
			}
			else
			{
				EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__ , _T(" PerfMon Object is Initialized"));	
			}
		}

	}
	catch (_com_error& e)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogSystem, __FUNCTION__ , _T("Unable to create PerfMon Object. Error %d, %s"), e.Error(), (LPCSTR)e.Description());	
		m_spBatchPerfMon = NULL;
	}
	catch (...)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogSystem, __FUNCTION__ , _T("Unable to create PerfMon Object. Unknown Error"));	
		m_spBatchPerfMon = NULL;
	}

	//m_Strategy.Start();

	EgLib::CThread::Start();
	HANDLE hThread = EgLib::CThread::GetThreadHandle();

	if (hThread != 0)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Waiting at HFServer.Initialize 2 Start"));	
		::WaitForSingleObject(m_hThread, INFINITE);
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Waiting at HFServer.Initialize 2 End"));	
		::CloseHandle(m_hThread);
		m_hThread = hThread;
		if (m_nStatus == DBA_ERR_NO_ERROR)
		{
			::InterlockedIncrement(&m_nCount);
			return DBA_ERR_NO_ERROR;
		}
	}
	return -1;
}

long CHFServer::Uninitialize()
{
	if (::InterlockedDecrement(&m_nCount) == 0 && m_hThread)
	{
		SetEvent(m_hEventStop);
		//m_Strategy.Stop();
		m_spBatchPerfMon = NULL;
		Terminate();
		return m_nStatus;
	}
	return DBA_ERR_NO_ERROR;
}

void CHFServer::Terminate()
{
	EgLib::CThread::Stop(true, HFC_TIMEOUT);
	m_hThread = 0;
}

UINT CHFServer::Run()
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("HF Server Run Enter"));	
	try
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		m_nStatus = eti(m_etiPblk, ETI_ATTACH_ASYNC, &m_etiPtr);
		::SetEvent(m_hThread);


		if (m_nStatus != DBA_ERR_NO_ERROR)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("HF Server Exit Run func with an error"));	
			return 0;
		}
		DWORD dwCounter = GetTickCount();
		long  lCompositeUpdates = 0;
		long  lRegionalUpdates = 0;
		long  lHeartbeat = 0;


		while (!m_bTerminate)
		{
			short status = 0;
			try
			{
				status = eti(m_etiPblk, ETI_READ, &m_etiPtr);
			}
			catch(...)
			{
				EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("ETI read unknown error"));	
			}

			if((GetTickCount() - dwCounter) > 20000)
			{
				long lProcessed =  ::InterlockedExchange(&m_lProcessedQuotes, 0);
				EgStd::CEgTracingClass::TraceStatic(enlogInfoExt, __FUNCTION__ , _T("ETI Updates\t Composite %d\t Regional %d\t Processed %d\t HeartBeat %d"),lCompositeUpdates, lRegionalUpdates, lProcessed, lHeartbeat);	

				dwCounter = GetTickCount();		
				lCompositeUpdates = 0;
				lRegionalUpdates = 0;
				lHeartbeat = 0;
			}

			if(EgLib::CThread::IsStopping() || !EgLib::CThread::IsRunning())
			{
				EgStd::CEgTracingClass::TraceStatic(enlogWarning, __FUNCTION__ , _T("HF Server Stopping"));	
				m_bTerminate = true;
				break;
			}		
			else if(status == RC_ETI_PERIODIC)
			{
				continue;
			}
			else if (status != RC_ETI_NOERROR)
			{
				EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("Messaging - Error"));	
				m_nStatus = status;
				m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineError(this, status)));

				if (status == RC_ETI_UNLOAD)
				{
					EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("Messaging - UNLOAD message is received"));	
					break;
				}
			}
			else
			{
				switch(m_etiPtr->packetPtr->undefined.header.type)
				{
				case MT_FILE_TRANSFER:
					if(FILE_CONTROL == m_etiPtr->packetPtr->fileTransfer.transferMask[0])
					{
						switch(m_etiPtr->packetPtr->fileTransfer.data.control.command)
						{
						case FT_CTRL_HEART_BEAT:		// 32 - heart beat
							{
								++lHeartbeat;

							}break;
						case FT_CTRL_NONE:				// 0 - no state implied (Sharky: simple message while reconnecting)
							{
								EgStd::CEgTracingClass::TraceStatic(enlogInfoExt, __FUNCTION__ , _T("Messaging - Reconnecting"));	
								std::string strText;
								strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
								strText.insert(strText.end(),(CHAR)0);

								m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineNotificationMessage(this, enProviderConnecting, strText)));

							}break;
						case FT_CTRL_ETI_SESS_RESET:	// 8 - Eti Session reset or restored
							{
								EgStd::CEgTracingClass::TraceStatic(enlogInfoExt, __FUNCTION__ , _T("Messaging - Session restored"));	
								std::string strText;
								strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
								strText.insert(strText.end(),(CHAR)0);
								m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineNotificationMessage(this, enProviderConnected, strText)));

							}break;
						case FT_CTRL_ETI_SESS_DOWN:		// 4 - Eti Session down
							{
								EgStd::CEgTracingClass::TraceStatic(enlogWarning, __FUNCTION__ , _T("Messaging -  Session is down"));	
								std::string strText;
								strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
								strText.insert(strText.end(),(CHAR)0);
								m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineNotificationMessage(this, enConnectionWasDisconnected, strText)));
							}break;
						}
					}break;
				case MT_TRADE:
				case MT_QUOTE:
					{
						try
						{
							if(m_spBatchPerfMon)
								m_spBatchPerfMon->AddRealTimeUpdate();

							DWORD dwCounter = GetTickCount();

							if (m_etiPtr->regionalPtr != 0 && m_etiPtr->regionalPtr->standard.item.type != 0xffff)
							{
								CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_etiPtr->regionalPtr->standard.item));

								if(m_sFilter.length())
								{
									if(*m_sFilter.c_str() == '*' || std::string::npos != pKey->GetKeyString().find(m_sFilter))
									{
										int iHourBid    = (int)m_etiPtr->regionalPtr->standard.bid.hour;
										int iMinuteBid  = (int)m_etiPtr->regionalPtr->standard.bid.minute;
										int iHourAsk    = (int)m_etiPtr->regionalPtr->standard.ask.hour;
										int iMinuteAsk  = (int)m_etiPtr->regionalPtr->standard.ask.minute;
										int iHourLast   = (int)m_etiPtr->regionalPtr->standard.lastSale.hour;
										int iMinuteLast = (int)m_etiPtr->regionalPtr->standard.lastSale.minute;
										long lTick      = (long)m_etiPtr->regionalPtr->standard.tick;

										EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("R %-8s\t |Bid %02d:%02d %s|Ask %02d:%02d %s|%s|\t"), pKey->GetKeyString().c_str(), iHourBid, iMinuteBid, DNTICK_BID&lTick?"Dn":(UPTICK_BID&lTick?"Up":"  "), iHourAsk, iMinuteAsk, DNTICK_ASK&lTick?"Dn":(UPTICK_ASK&lTick?"Up":"  "), DNTICK_LASTSALE&lTick?"Dn":(UPTICK_LASTSALE&lTick?"Up":"  "));	
									}
								}
								++lRegionalUpdates;
								m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineQuoteMessage(this, pKey, &m_etiPtr->regionalPtr->standard)));

							}
							if (m_etiPtr->compositePtr != 0 &&	m_etiPtr->compositePtr->standard.item.type != 0xffff)	
							{
								CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_etiPtr->compositePtr->standard.item));
								if(m_sFilter.length())
								{
									if(*m_sFilter.c_str() == '*' || std::string::npos != pKey->GetKeyString().find(m_sFilter))
									{
										int iHourBid    = (int)m_etiPtr->compositePtr->standard.bid.hour;
										int iMinuteBid  = (int)m_etiPtr->compositePtr->standard.bid.minute;
										int iHourAsk    = (int)m_etiPtr->compositePtr->standard.ask.hour;
										int iMinuteAsk  = (int)m_etiPtr->compositePtr->standard.ask.minute;
										int iHourLast   = (int)m_etiPtr->compositePtr->standard.lastSale.hour;
										int iMinuteLast = (int)m_etiPtr->compositePtr->standard.lastSale.minute;
										long lTick      = (long)m_etiPtr->regionalPtr->standard.tick;

										EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("C %-8s\t |Bid %02d:%02d %s|Ask %02d:%02d %s|%s|\t"), pKey->GetKeyString().c_str(), iHourBid, iMinuteBid, DNTICK_BID&lTick?"Dn":(UPTICK_BID&lTick?"Up":"  "), iHourAsk, iMinuteAsk, DNTICK_ASK&lTick?"Dn":(UPTICK_ASK&lTick?"Up":"  "), DNTICK_LASTSALE&lTick?"Dn":(UPTICK_LASTSALE&lTick?"Up":"  "));	
									}
								}
								++lCompositeUpdates;
								m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineQuoteMessage(this, pKey, &m_etiPtr->compositePtr->standard)));
							}

						}
						catch (...)
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSystem, __FUNCTION__ , _T("Unknown error"));	
						}
					}break;
				}
			}
		}

		_ASSERTE(m_setSubs.size() == 0);

		m_nStatus = eti(m_etiPblk, ETI_DETACH, &m_etiPtr);

		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("HF Server Run Exit"));	
	}
	catch(...)
	{
		EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("HF Server got exception, exiting"));		
	}
	CoUninitialize();
	return 0;
}

void CHFServer::Subscribe(CPrice* pSub)
{

	m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineSubscribe(this, pSub)));
	//m_Strategy.PushData(boost::tuples::make_tuple(pSub, true));
}

void CHFServer::Unsubscribe(CPrice* pSub)
{
	m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineUnsubscribe(this, pSub)));
	//m_Strategy.PushData(boost::tuples::make_tuple(pSub, false));
}

void CHFServer::OnNotificationMessage(ErrorNumberEnum enType, std::string& sText)
{
	std::for_each(m_setSubs.begin(), 
		m_setSubs.end(), 
		boost::bind(&CPrice::NotificationMessage, _1, enType, sText));
}

void CHFServer::OnQuoteMessage(CHahedKeyStringPtr pKey, DBA_RECORD_3* pRecord)
{
	::InterlockedIncrement(&m_lProcessedQuotes);
	CPrice::RealtimeUpdate(pKey, pRecord);
	std::for_each(	m_setSubs.begin(), 
		m_setSubs.end(), 
		boost::bind(&CPrice::RealtimeMessage, _1, pKey, pRecord));
}

void CHFServer::OnError(long lStatus)
{
	std::for_each(m_setSubs.begin(), 
		m_setSubs.end(), 
		boost::bind(&CPrice::ErrorMessage, _1, lStatus));
}

void CHFServer::OnSubscribe(CPrice* pPrice)
{
	m_setSubs.insert(pPrice);

}
void CHFServer::OnUnsubscribe(CPrice* pPrice)
{
	subset::iterator it = m_setSubs.find(pPrice);
	_ASSERTE(it != m_setSubs.end());
	if (it != m_setSubs.end())
		m_setSubs.erase(it);
}

//void CHFServer::OnData(CSubsData& data)
//{
//	EgLib::CAutoLock lock(m_csQueue);
//	if(!boost::tuples::get<1>(data))
//	{
//		subset::iterator it = m_setSubs.find(boost::tuples::get<0>(data));
//		_ASSERTE(it != m_setSubs.end());
//		if (it != m_setSubs.end())
//			m_setSubs.erase(it);
//	}
//	else
//	{
//		//subscribe
//		m_setSubs.insert(boost::tuples::get<0>(data));
//	}
//	if(m_spBatchPerfMon)
//		m_spBatchPerfMon->RTClients = m_setSubs.size();
//}


/*
LONG CALLBACK EtiHandler(LONG nStatus, ETI_TABLE* pTable, PVOID pUserData)
{
if(MT_FILE_TRANSFER == pTable->packetPtr->info.header.type
&& FILE_CONTROL == pTable->packetPtr->fileTransfer.transferMask[0])

{

//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_HEART_BEAT) // 32 - heart beat

//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_NONE) // 0 - no state implied (Sharky: simple message while reconnecting)

//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_ETI_SESS_RESET) // 8 - Eti Session reset or restored

//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_ETI_SESS_DOWN) // 4 - Eti Session down

/*

Sequence:

FT_CTRL_ETI_SESS_RESET - may not raised if connection already was established

...

FT_CTRL_HEART_BEAT - connected

...

FT_CTRL_ETI_SESS_DOWN - when disconnected

...

FT_CTRL_NONE - while disconnected

...

FT_CTRL_ETI_SESS_RESET - when reconnected

...

FT_CTRL_HEART_BEAT - connected

...



printf("MT_FILE_TRANSFER: %d\t%d\t%02d%02d\t%s\n",

pTable->packetPtr->fileTransfer.sequenceNumber,

pTable->packetPtr->fileTransfer.function,

pTable->packetPtr->fileTransfer.data.control.source,

pTable->packetPtr->fileTransfer.data.control.command,

(FT_CTRL_HEART_BEAT == pTable->packetPtr->fileTransfer.data.control.command ? ""

: (LPSTR)pTable->packetPtr->fileTransfer.data.control.text));

}

return 0L;

}
*/