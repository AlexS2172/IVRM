// HFServer.cpp: implementation of the CHFServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HFServer.h"
#include "Price.h"
#include <sstream>
#include <psapi.h>



ULONG  CHFServer::m_lClients     = 0;
ULONG  CHFServer::m_lRTClients   = 0;
volatile LONG  CHFServer::m_lRTResponse  = 0;

#ifdef __PERFMON
LARGE_INTEGER CHFServer::m_lPerformanceFrequeny;
PerfMon::IPPBatchInfoPtr CHFServer::m_spBatchPerfMon;
PerfMon::IPPBatchInfoPtr CHFServer::GetGlobalPerfMonitor()
{
	return  m_spBatchPerfMon;
}
#endif // __PERFMON


void CEngineQuoteMessage::DoTask()
{
	try
	{
		m_pAdaptor->OnQuoteMessage(m_Key, m_record);
	}
	catch (_com_error& /*e*/)
	{
	}
	catch (...)
	{
	}
}

void CEngineNotificationMessage::DoTask()
{
	try
	{
		m_pAdaptor->OnNotificationMessage(m_enErrorType, m_strText);
	}
	catch (_com_error& /*e*/)
	{
	}
	catch (...)
	{
	}
}

void CEngineError::DoTask()
{
	try
	{
		m_pAdaptor->OnError(m_lStatus);
	}
	catch (_com_error& /*e*/)
	{
	}
	catch (...)
	{
	}
}


UINT CHFServerService::Run()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	bool bTerminate = false;
	DWORD lNoUpdates = 0;
	unsigned long lTotalReceived = 0;
	unsigned long lTotalProcessed = 0;
	bool bEnableChecking = false;

	while(!bTerminate)
	{
		HANDLE hEvents[] = 
		{
			m_pServer->m_hHasSubscriptions,
			CHFServerService::GetStopEventHandle()
		};
		DWORD dwWaitResult = WaitForMultipleObjects(2, hEvents, FALSE, 15000)  ;

		switch(dwWaitResult)
		{
		case WAIT_TIMEOUT:
			{
				//			if(m_pServer->IsTerminate())
				//				break;

				long lCompositeUpdates   = ::InterlockedExchange(&m_pServer->m_lCompositeUpdates,	0L);
				long lRegionalUpdates    = ::InterlockedExchange(&m_pServer->m_lRegionalUpdates,	0L);
				long lProcessed          = ::InterlockedExchange(&m_pServer->m_lProcessedQuotes,	0L);
				long lHeartbeat          = ::InterlockedExchange(&m_pServer->m_lHeartbeat,			0L); 
				long lPeriodic           = ::InterlockedExchange(&m_pServer->m_lPeriodic,			0L); 
				long lPriceUpdates =  lCompositeUpdates + lRegionalUpdates;

				lTotalReceived  +=  lPriceUpdates;
				lTotalProcessed +=  lProcessed;
				DWORD lQueuesize = lTotalReceived -lTotalProcessed;

				CString strInfo("{");
				try
				{
					CStrategyPoolStatistics stat; 
					m_pServer->m_Pool.GetPoolStatistics(stat);
					for(CStrategyPoolStatistics::iterator itr = stat.begin(); itr != stat.end(); ++itr)
					{
						CString strForm;;
						strForm.Format(" %-6d ",*itr);
						strInfo+=strForm;
					}

//					for(std::vector<DWORD>::iterator itr = m_pServer->m_vecThreadPoolStatistics.begin();itr != m_pServer->m_vecThreadPoolStatistics.end();++itr)
//					{
//						CString strForm;;
//						strForm.Format(" %-6d ",*itr);
//						*itr = 0;
//						strInfo+=strForm;
//					}
				}catch(...){}
				strInfo+="}";

				EgLib::CEgLibTraceManager::Trace(LogInfoExt, _T("") , _T("ETI Realtime\t T% -6d [C% -6d] Pr% -6d(% -6d) HB % -3d\t Per %-3d %s"), lRegionalUpdates+lCompositeUpdates, lCompositeUpdates,  lProcessed, lQueuesize, lHeartbeat, lPeriodic, strInfo.GetString());	

				if(bEnableChecking && _Module.IsAutorestartEnabled())
				{
					vt_date dtNow(vt_date::GetCurrentDate());
					if(dtNow.get_day_of_week() != 0 && dtNow.get_day_of_week()!=6)
					{
						if(_Module.GetAutorestartBeginTime() && _Module.GetAutorestartEndTime() &&  _Module.GetAutorestartBeginTime() < _Module.GetAutorestartEndTime())
						{
							long lBeginHour = (long)_Module.GetAutorestartBeginTime()/100;
							long lEndHour   = (long)_Module.GetAutorestartEndTime()/100;
							long lBeginMinute = _Module.GetAutorestartBeginTime() - lBeginHour*100;
							long lEndMinute   = _Module.GetAutorestartEndTime() - lEndHour*100;

							if((dtNow.get_hour()>lBeginHour || (dtNow.get_hour() == lBeginHour && dtNow.get_minute() >= lBeginMinute)) && 
								(dtNow.get_hour()<lEndHour   || (dtNow.get_hour() == lEndHour   && dtNow.get_minute() <= lEndMinute) ))
							{
								if(lProcessed == 0 && lPriceUpdates != 0)
								{
									EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("No HF activity was processed in 15 seconds interval."));	
									m_pServer->OnIdle(true);
									//m_pServer->m_Pool.QueueRequest(reinterpret_cast<CHFServerThreadPool::RequestType>(new CEngineNoActivityMessage(m_pServer, false)));

								}
								else
									if(0==(lPriceUpdates+lHeartbeat+lPeriodic))
									{
										// No updates found in 15 seconds interval
										EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("No HF activity found in 15 seconds interval."));
										m_pServer->OnIdle(true);
									}
									else
										if(lPriceUpdates == 0)
										{
											if(++lNoUpdates > (_Module.AutorestartInterval()-1))
											{
												EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("No price updates found in %d seconds interval."),_Module.AutorestartInterval()*15 );	
												m_pServer->OnIdle(false);
												lNoUpdates = 0;
											}
										}
										else
										{
											if(lQueuesize > _Module.GetMaxQueueSize())
											{
												EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("The Processing queue exides maximum allowed qieie limit %d"), _Module.GetMaxQueueSize());
												m_pServer->OnIdle(true);
											}
											lNoUpdates = 0;
										}
							}
							else
								if(lPriceUpdates == 0 )
									EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("No price updates found in %d seconds interval but not working hours."),_Module.AutorestartInterval()*20 );	

						}
						else
							if(lPriceUpdates == 0 )
								EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("No price updates found in %d seconds interval but no autorestart enabled."),_Module.AutorestartInterval()*20 );	
					}
					else
						if(lPriceUpdates == 0 )
							EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("No price updates found in %d seconds interval but weekend."),_Module.AutorestartInterval()*20 );	
				}
				else
					if(bEnableChecking)
					{
						if(lPriceUpdates == 0 )
							EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("No price updates found in %d seconds interval but autorestart is disabled."),_Module.AutorestartInterval()*20 );	
					}
			}break;
		case WAIT_OBJECT_0:
			{
				EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("First subscription is detected. Do Reset Checking timer."));	
				bEnableChecking = true;
			}break;
		default:
			bTerminate = true;
			break;
		}
	}
	CoUninitialize();
	try
	{
		EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Checking Thread Exit "));	
	}catch(...){}
	return 0 ;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHFServer::CHFServer() 
	:m_nStatus(DBA_ERR_NO_ATTACH)
	,m_Service(this)
	,m_bTerminate(true)
	,m_nCount(0)
	,m_etiPtr(0)
	,m_lHeartbeat(0)
	,m_lProcessedQuotes(0)
	,m_lCompositeUpdates(0)
	,m_lRegionalUpdates(0)
	,m_lPeriodic(0)
	,m_bConnected(false)
	//,m_hEventStop(CreateEvent(NULL, TRUE, FALSE, NULL))
	,m_hHasSubscriptions(CreateEvent(NULL, FALSE, FALSE, NULL))
	,m_Pool(-2)
#ifdef __PERFTEST
	,m_DataSimulator(this)
#endif

{
#ifdef __PERFMON
	QueryPerformanceFrequency(&m_lPerformanceFrequeny);
	m_lPerformanceFrequeny.QuadPart /=1000;
#endif // __PERFMON
	m_Service.Start();
	//m_vecThreadPoolStatistics.reserve(m_Pool.GetSize());
	//m_Pool   .Initialize(m_hEventStop,	-2);
//	m_Pool.Initialize(m_hEventStop,	1);

}

CHFServer::~CHFServer()
{
	if (m_hThread != NULL)
		Terminate();
	m_Service.Stop();
	_ASSERTE(m_setSubs.size() == 0);
}

//////////////////////////////////////////////////////////////////////
// CHFServer member functions
 long CHFServer::Initialize()
{
	EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("HF Server Initialize"));	
	{
		CAutoLock lockInitialize(m_csInitialize);
		if (m_hThread != NULL)
		{
			if (!m_bTerminate)
			{
				::InterlockedIncrement(&m_nCount);
				return m_nStatus = DBA_ERR_NO_ERROR;
			}
			else
			{
				EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("Waiting at HFServer.Initialize Start"));	
				if(WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 20000))
				{
					EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Unable to Initialize PriceFeed thread"));	
					OnIdle(true);
				}
				EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("Waiting at HFServer.Initialize End"));	
			}
		}
		else
		{
			m_hThread.Attach(::CreateEvent(0, FALSE, FALSE, 0));
			m_bTerminate = false;
			m_Pool.SetSize(_Module.GetThreadPoolCapacity());
			m_Pool.Start();
		}
	}

#ifdef __PERFMON
	try
	{
		HRESULT __hr = S_OK;
		if(FAILED(__hr = m_spBatchPerfMon.CreateInstance(__uuidof(PerfMon::CPPBatchInfo))) || m_spBatchPerfMon == NULL)
		{
			EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__ , _T("Unable to create PerfMon Object %d"),__hr);	
			m_spBatchPerfMon = NULL;
		}
		else
		{
			if(FAILED(__hr = m_spBatchPerfMon->Initialize(_bstr_t(L"__Total"))))
			{
				m_spBatchPerfMon = NULL;
				EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__ , _T("Unable to Initialize PerfMon Object %d"),__hr);	
			}
			else
			{
				EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T(" PerfMon Object is Initialized"));	
			}
		}

	}
	catch (_com_error& e)
	{
		EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__ , _T("Unable to create PerfMon Object. Error %d, %s"), e.Error(), (LPCSTR)e.Description());	
		m_spBatchPerfMon = NULL;
	}
	catch (...)
	{
		EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__ , _T("Unable to create PerfMon Object. Unknown Error"));	
		m_spBatchPerfMon = NULL;
	}
#endif // __PERFMON

	//m_Strategy.Start();
	 EgLib::CThread::Stop(true, 100);
	DWORD dwResult = EgLib::CThread::Start();

	if(dwResult != ERROR_SUCCESS)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Unable to create Message thread. Error %d"), dwResult);	
		return dwResult;
	}

#ifdef __PERFTEST
	if(_Module.SimulatedDataflow())
		m_DataSimulator.Start();
#endif

	HANDLE hThread = EgLib::CThread::GetThreadHandle();
	if(g_hMultilock!=NULL)
		g_hMultilock.Close();

	if (hThread != 0)
	{
		EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("Waiting at HFServer.Initialize 2 Start"));	
		if(WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 20000))
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Unable to Initialize PriceFeed thread"));	
			OnIdle(true);
		}
		EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("Waiting at HFServer.Initialize 2 End"));	

		m_hThread.Close();
		m_hThread.Attach(hThread);
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
	EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("HF Server Uninitialize"));	
	if (::InterlockedDecrement(&m_nCount) == 0 && m_hThread)
	{
		//SetEvent(m_hEventStop);
		m_Pool.Stop();
		//m_Strategy.Stop();
#ifdef __PERFMON
		m_spBatchPerfMon = NULL;
#endif // __PERFMON
		Terminate();
		return m_nStatus;
	}
	return DBA_ERR_NO_ERROR;
}

void CHFServer::Terminate()
{
	EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("HF Server Terminating"));	
#ifdef __PERFTEST
	if(_Module.SimulatedDataflow())
		m_DataSimulator.Stop();
#endif
	m_Pool.Stop();
	EgLib::CThread::Stop(true, HFC_TIMEOUT);
	m_hThread.Close();
}

UINT CHFServer::Run()
{
	EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("HF Server Run Enter"));	
	try
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		m_nStatus = eti(m_etiPblk, ETI_ATTACH_ASYNC, &m_etiPtr);
		::SetEvent(m_hThread);


		if (m_nStatus != DBA_ERR_NO_ERROR)
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("HF Server Exit Run func with an error"));	
			return 0;
		}
        
		long lCountError = 0;
		while (!m_bTerminate)
		{
			short status = 0;
			try
			{
				status = eti(m_etiPblk, ETI_READ, &m_etiPtr);
			}
			catch(...)
			{
				EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("ETI read unknown error"));	
			}

			try
			{
				if(EgLib::CThread::IsStopping() || !EgLib::CThread::IsRunning())
				{
					EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("HF Server Stopping"));	
					m_bTerminate = true;
					break;
				}		
				else if(status == RC_ETI_PERIODIC)
				{
					m_bConnected = true;
					InterlockedIncrement(&m_lPeriodic);
					continue;
				}
				else if (status != RC_ETI_NOERROR)
				{
					EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Messaging - Error"));	
					m_nStatus = status;
					m_Pool.PushBack(CHFServerThreadPool::CStrategyThread::PoolDataTypePtr(static_cast<CHFServerThreadPool::CStrategyThread::PoolDataType*>(new CEngineError(this, status))));

					if (status == RC_ETI_UNLOAD)
					{
						EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Messaging - UNLOAD message received"));	
						SwitchToThread();
						OnIdle(true);
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
									InterlockedIncrement(&m_lHeartbeat);
									m_bConnected = true;
								}break;
							case FT_CTRL_NONE:				// 0 - no state implied (Sharky: simple message while reconnecting)
								{
									EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("Messaging - Reconnecting"));	
									std::string strText;
									strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
									strText.insert(strText.end(),(CHAR)0);
									m_Pool.PushBack(CHFServerThreadPool::CStrategyThread::PoolDataTypePtr(static_cast<CHFServerThreadPool::CStrategyThread::PoolDataType*>(new CEngineNotificationMessage(this, enProviderConnecting, strText))));

									m_bConnected = false;
								}break;
							case FT_CTRL_ETI_SESS_RESET:	// 8 - Eti Session reset or restored
								{
									EgLib::CEgLibTraceManager::Trace(LogInfoExt, __FUNCTION__ , _T("Messaging - Session restored"));	
									std::string strText;
									strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
									strText.insert(strText.end(),(CHAR)0);
									m_Pool.PushBack(CHFServerThreadPool::CStrategyThread::PoolDataTypePtr(static_cast<CHFServerThreadPool::CStrategyThread::PoolDataType*>(new CEngineNotificationMessage(this, enProviderConnected, strText))));
									m_bConnected = true;
								}break;
							case FT_CTRL_ETI_SESS_DOWN:		// 4 - Eti Session down
								{
									EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Messaging -  Session is down"));	
									std::string strText;
									strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
									strText.insert(strText.end(),(CHAR)0);
									m_Pool.PushBack(CHFServerThreadPool::CStrategyThread::PoolDataTypePtr(static_cast<CHFServerThreadPool::CStrategyThread::PoolDataType*>(new CEngineNotificationMessage(this, enConnectionWasDisconnected, strText))));
									m_bConnected = false;
								}break;
							}
						}break;
					case MT_TRADE:
					case MT_QUOTE:
						{
							m_bConnected = true;
							try
							{
#ifdef __PERFMON
								if(m_spBatchPerfMon)
									m_spBatchPerfMon->AddRealTimeUpdate();
#endif // __PERFMON

								if (m_etiPtr->regionalPtr != 0 && m_etiPtr->regionalPtr->extended.item.type != 0xffff)
								{
									CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_etiPtr->regionalPtr->extended.item));

									if(m_sFilter.length())
									{
										if(*m_sFilter.c_str() == '*' || std::string::npos != pKey->GetKeyString().find(m_sFilter))
										{
											int iHourBid    = (int)m_etiPtr->regionalPtr->extended.bid.hour;
											int iMinuteBid  = (int)m_etiPtr->regionalPtr->extended.bid.minute;
											int iHourAsk    = (int)m_etiPtr->regionalPtr->extended.ask.hour;
											int iMinuteAsk  = (int)m_etiPtr->regionalPtr->extended.ask.minute;
											long lTick      = (long)m_etiPtr->regionalPtr->extended.tick;

											EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("R %-8s\t |Bid %02d:%02d %s|Ask %02d:%02d %s|%s|\t"), pKey->GetKeyString().c_str(), iHourBid, iMinuteBid, DNTICK_BID&lTick?"Dn":(UPTICK_BID&lTick?"Up":"  "), iHourAsk, iMinuteAsk, DNTICK_ASK&lTick?"Dn":(UPTICK_ASK&lTick?"Up":"  "), DNTICK_LASTSALE&lTick?"Dn":(UPTICK_LASTSALE&lTick?"Up":"  "));	
										}
									}
									InterlockedIncrement(&m_lRegionalUpdates);
									m_Pool.PushBack(CHFServerThreadPool::CStrategyThread::PoolDataTypePtr(static_cast<CHFServerThreadPool::CStrategyThread::PoolDataType*>(new CEngineQuoteMessage(this, pKey, &m_etiPtr->regionalPtr->extended))));

								}
								if (m_etiPtr->compositePtr != 0 &&	m_etiPtr->compositePtr->extended.item.type != 0xffff)	
								{
									CHahedKeyStringPtr pKey = CHahedKeyStringPtr(new CHahedKeyString(&m_etiPtr->compositePtr->extended.item));
									if(m_sFilter.length())
									{
										if(*m_sFilter.c_str() == '*' || std::string::npos != pKey->GetKeyString().find(m_sFilter))
										{
											int iHourBid    = (int)m_etiPtr->compositePtr->extended.bid.hour;
											int iMinuteBid  = (int)m_etiPtr->compositePtr->extended.bid.minute;
											int iHourAsk    = (int)m_etiPtr->compositePtr->extended.ask.hour;
											int iMinuteAsk  = (int)m_etiPtr->compositePtr->extended.ask.minute;
											long lTick      = (long)m_etiPtr->regionalPtr->extended.tick;

											EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("C %-8s\t |Bid %02d:%02d %s|Ask %02d:%02d %s|%s|\t"), pKey->GetKeyString().c_str(), iHourBid, iMinuteBid, DNTICK_BID&lTick?"Dn":(UPTICK_BID&lTick?"Up":"  "), iHourAsk, iMinuteAsk, DNTICK_ASK&lTick?"Dn":(UPTICK_ASK&lTick?"Up":"  "), DNTICK_LASTSALE&lTick?"Dn":(UPTICK_LASTSALE&lTick?"Up":"  "));	
										}
									}
									InterlockedIncrement(&m_lCompositeUpdates);
									m_Pool.PushBack(CHFServerThreadPool::CStrategyThread::PoolDataTypePtr(static_cast<CHFServerThreadPool::CStrategyThread::PoolDataType*>(new CEngineQuoteMessage(this, pKey, &m_etiPtr->compositePtr->extended))));
}
								lCountError = 0;
							}
							catch (...)
							{
								EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__ , _T("Unknown error"));	
								++lCountError;
									
							}
						}break;
					default:
						{
							continue;
						}
					}
				}
			}
			catch(...)
			{
				EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("ETI message processing Unknown error"));	
			}
			if( lCountError > 3 )
			{
				EgLib::CEgLibTraceManager::Trace(LogSystem, __FUNCTION__ , _T("Too many Errors"));	
				OnIdle(true);
				break;
			}
		} 

		_ASSERTE(m_setSubs.size() == 0);

		m_nStatus = eti(m_etiPblk, ETI_DETACH, &m_etiPtr);

		EgLib::CEgLibTraceManager::Trace(LogWarning , __FUNCTION__ , _T("HF Server Run Exit"));	
	}
	catch(...)
	{
		try
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("HF Server got exception, exiting"));		
		}
		catch(...){}
	}
	m_hThread.Close();
	CoUninitialize();
	return 0;
}

void CHFServer::Subscribe(CPrice* pSub)
{
	bool bEmpty = false;
	CWriterAutoLock lock(m_WRcsSubscscription);
	bEmpty =m_setSubs.empty();
	m_setSubs.insert(pSub);
	if(bEmpty)
		SetEvent(m_hHasSubscriptions);
}

void CHFServer::Unsubscribe(CPrice* pSub)
{
	CWriterAutoLock lock(m_WRcsSubscscription);
	subset::iterator it = m_setSubs.find(pSub);
	if (it != m_setSubs.end())
		m_setSubs.erase(it);
}

void CHFServer::OnIdle(bool bForce)
{
	
	if(!bForce)
	{
		subset::iterator itr = std::find_if(m_setSubs.begin(), m_setSubs.end(), boost::bind(&CPrice::HasSubscriptions, _1 ));
		bForce = (m_setSubs.end() != itr);
		if(!bForce)
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("No Active subscriptions detected. Termination will be skipped"));	
			return;
		}
		
	}
	if(bForce)
	{
		// Idle Event and active subscriptions
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("No HyperFeed Realtime Updates. Forcing Restart Sequence"));	

		CloseHFProcess("etilog.exe");
		CloseHFProcess("dbview.exe");
		CloseHFProcess("pcqlogin.exe");
		CloseHFProcess("pcqetix.exe");
		//CloseHFProcess("HyperFeedProviders.exe");
		//CloseHFProcess("HyperFeedProvidersP4.exe");

		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Terminating the provider executable. See you next time"));	
		Kill(GetCurrentProcessId());
		ExitProcess(0);
	}

}
bool CHFServer::Kill(DWORD dwID)
{
	_bstr_t bsKill(" -f ");
	_bstr_t bsProcID(dwID);
	bsKill+=bsProcID;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	TCHAR buffPath[MAX_PATH*20];
	ZeroMemory(buffPath, sizeof(buffPath));
	GetSystemWindowsDirectory(buffPath, sizeof(buffPath));
	_bstr_t bsPath(buffPath);
	bsPath +="\\Kill.exe";

	return CreateProcess(bsPath, const_cast<LPSTR>((LPCSTR)bsKill), NULL,NULL,FALSE,0,NULL,NULL,&si, &pi)?true:false;
}
void CHFServer::CloseHFProcess(LPCSTR szProcessName)
{
	std::string sProcessName =  utils::upcase(std::string(szProcessName));
	DWORD dwID = 0;
	CHandle hProcess = GetProcessId(sProcessName, dwID);
	long iCount = 0;
	if(hProcess == NULL)
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("No %s module found"), sProcessName.c_str());	

	while( hProcess != NULL)
	{
		EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Terminating %s module"), sProcessName.c_str());	
		

		if(!TerminateProcess(hProcess, 0)  )
		{
			DWORD dwError = GetLastError();

			if(dwID)
			{
				if(!Kill(dwID))
					EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Kill Process %s Failed with error %d and terminate with error %d"), sProcessName.c_str(), GetLastError() , dwError);	
			}  
			else
				EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Unable to terminate %s module %d"), sProcessName.c_str(), dwError);	
		}

		if(++iCount > 10)
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Too many %s process to terminate %d"), sProcessName.c_str(), GetLastError());	
			break;
		}
		hProcess.Close();
		hProcess.Attach(GetProcessId(sProcessName, dwID));
	}
}
void CHFServer::OnNotificationMessage(ErrorNumberEnum enType, std::string& sText)
{
	CReaderAutoLock lock(m_WRcsSubscscription);
	std::for_each(m_setSubs.begin(), 
		m_setSubs.end(), 
		boost::bind(&CPrice::NotificationMessage, _1, enType, sText));
}

void CHFServer::OnQuoteMessage(CHahedKeyStringPtr pKey, CDBARecordPtr pRecord)
{
//	try
//	{
//		UINT uiThreadId = GetCurrentThreadId();
//		std :: map<UINT, DWORD>::iterator it = m_threadPoolStatistics.find(uiThreadId);
//		if(it != m_threadPoolStatistics.end())
//			m_vecThreadPoolStatistics[it->second]++;
//		else
//		{
//			m_threadPoolStatistics[uiThreadId] = m_vecThreadPoolStatistics.size();
//			m_vecThreadPoolStatistics.insert(m_vecThreadPoolStatistics.end(), 0);
//		}
//	}catch(...){}

	long lProcessed = ::InterlockedIncrement(&m_lProcessedQuotes);
	if(lProcessed&1)
	{
		CReaderAutoLock lock(m_WRcsSubscscription);
		std::for_each(	m_setSubs.begin(), 
			m_setSubs.end(), 
			boost::bind(&CPrice::RealtimeMessage, _1, pKey, pRecord));
	}
	else
	{
		CReaderAutoLock lock(m_WRcsSubscscription);
		std::for_each(	m_setSubs.rbegin(), 
			m_setSubs.rend(), 
			boost::bind(&CPrice::RealtimeMessage, _1, pKey, pRecord));
	}
}

void CHFServer::OnError(long lStatus)
{
	CReaderAutoLock lock(m_WRcsSubscscription);
	std::for_each(m_setSubs.begin(), 
		m_setSubs.end(), 
		boost::bind(&CPrice::ErrorMessage, _1, lStatus));
}

CHandle CHFServer::GetProcessId(std::string& sProcessName, DWORD& dwProcessID )
{
	DWORD aProcesses[2048], cbNeeded, cProcesses;
	if(!sProcessName.length())
		return CHandle();
	dwProcessID = 0;

	unsigned int i;
	std::string sUpperCaseProcessName = utils::upcase(sProcessName);

	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		return CHandle();

	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.
 	for ( i = 0; i < cProcesses; i++ )
	{
		char szProcessName[MAX_PATH] ={0};
		CHandle hProcess(OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,	FALSE, aProcesses[i] ));

		if (NULL != (HANDLE)hProcess )
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
			{
				GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName));

				if(utils::upcase(std::string(szProcessName)) ==	sProcessName && aProcesses[i] != ::GetCurrentProcessId())
				{
					dwProcessID = aProcesses[i];
					return hProcess;
				}
			}
		}
	}
	return CHandle();
}


#ifdef __PERFTEST

void CHFServer::OnPerfTest()
{

//	try
//	{
//		UINT uiThreadId = GetCurrentThreadId();
//		std :: map<UINT, DWORD>::iterator it = m_threadPoolStatistics.find(uiThreadId);
//		if(it != m_threadPoolStatistics.end())
//			m_vecThreadPoolStatistics[it->second]++;
//		else
//		{
//			m_threadPoolStatistics[uiThreadId] = m_vecThreadPoolStatistics.size();
//			m_vecThreadPoolStatistics.insert(m_vecThreadPoolStatistics.end(), 0);
//		}
//	}catch(...){}

	CReaderAutoLock lock(m_WRcsSubscscription);
	InterlockedIncrement(&m_lProcessedQuotes);

	std::for_each(	m_setSubs.begin(), 
		m_setSubs.end(), 
		boost::bind(&CPrice::PerfTestMessage, _1));
}


UINT CDataDlowEmulator::Run()
{
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	
	DWORD dwSize = _Module.SimulatedDataflow();
	DWORD dwTimeout = 1;
	DWORD dwSent =0;
	DWORD dwStart = GetTickCount();

	while(dwSize > 0)
	{
		if(WAIT_TIMEOUT!=WaitWithEvents(GetStopEventHandle(), dwTimeout))
			break;
		DWORD dw = GetTickCount();

		for(DWORD i = 0; i<(dwSize/200);i++)
		{
			::InterlockedIncrement(&m_pServer->m_lCompositeUpdates);

			boost::shared_ptr<CEnginePerfTestMessage> spMessage(new CEnginePerfTestMessage(m_pServer));
			m_pServer->m_Pool.PushBack(boost::shared_static_cast<CHFServerThreadPool::CStrategyThread::PoolDataType>(spMessage));
			dwSent++;
		}
		dwTimeout = 5 - (GetTickCount() - dw)/2;
		if(dwTimeout < 0 || dwTimeout > 1000)
			dwTimeout = 1;

		if(dwSent >= dwSize)
		{
			DWORD dwEnd = GetTickCount();

			dwSent = 0;
			if((dwEnd-dwStart)< 1000)
				dwTimeout =	 1000-(dwEnd-dwStart);
            dwStart = dwEnd;
		}
	}

	CoUninitialize();
	return 0;
}


void CEnginePerfTestMessage::DoTask()
{
	try
	{
        m_pAdaptor->OnPerfTest();
	}
	catch (...) {}
}

#endif // __PERFTEST