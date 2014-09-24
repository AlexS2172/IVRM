#ifndef ISEMANAGER_BASE_H
#define ISEMANAGER_BASE_H

#include "ISEProvider.h"
#include "ISEMultiSession.h"

#include "ISEStructureProvider.h"
#include "ISEStdStructureProvider.h"
#include "ISEStdStructureInfo.h"
#include "ISEPriceProvider.h"
#include "ISEMMManager.h"
#include "ISETracer.h"
#include "OmniApiUtils.h"
#include "ISESettingsManager.h"


template<class _R>
struct CRequestContext
{
	
	CRequestContext(
		const _EventTypeEnum Type, 
		const _R& rData, 
		const DWORD dwObjectID = 0, 
		const bool bRemove = false)
		:	m_Type(Type),
			m_Data(rData),
			m_dwObjectID(dwObjectID),
			m_bRemove(bRemove)
	{
	}
	
	CRequestContext()
		:	m_Type(_enNoRequest),
			m_dwObjectID(0),
			m_bRemove(false)
	{
	}
	
	/*const*/ _EventTypeEnum	m_Type;
	/*const*/ _R				m_Data;

	DWORD					m_dwObjectID;
	bool					m_bRemove;

	bool operator< (const CRequestContext<_R>& Origin) const
	{
		if(m_Type == Origin.m_Type)
			return m_Data < Origin.m_Data;

		return m_Type < Origin.m_Type;
	}

	bool operator! () const
	{
		return !m_Data;
	}
};

struct CResponseData
{
	CComVariant		m_vtData;
	CComVariant		m_vtData2;

	// error
	ErrorNumberEnum m_Error;
	_bstr_t			m_bsText;

	// quote entry
	long			m_lReason; // also used in Heartbeat broadcast (BI9)
	long			m_lTickLevel;

	// information
	ISEInfoTypeEnum	m_InfoType;

	//series
	_bstr_t			m_bsSymbol;
	_bstr_t			m_bsExchange;

	//order
	_bstr_t			m_bsOrderID;
};

template<class _R>
struct CResponseContext
{
	
	CResponseContext(const _EventTypeEnum Type, const _R& rRequest)
	:	m_Type(Type),
		m_Request(rRequest),
		m_dwObjectID(0)
	{
	}
	

	/*const*/ _EventTypeEnum	m_Type;
	/*const*/ _R				m_Request;

	CResponseData			m_Data;
	DWORD					m_dwObjectID;

	bool operator< (const CResponseContext<_R>& Origin) const
	{
		if (m_Type == Origin.m_Type) 
			return m_Request < Origin.m_Request;

		return m_Type < Origin.m_Type;
	}
};

template<class _R>
struct CObjectContext
{
	set<CRequestContext<_R> >			m_setRequests;
	queue<CRequestContext<_R> >			m_queRequests;
	queue<CResponseContext<_R> >		m_queEvents;
	set<CResponseContext<_R> >			m_setEvents;

	bool								m_bZombie;

	CObjectContext<_R>()
	{
		m_bZombie = false;
	}
};

/*------------------------------------------------------------------------------------*/

class CISEManagerBase : public CISEMultiSession
{
private:

	HANDLE	m_hStopConnectEvent;
	HANDLE	m_hConnectEvent;
	HANDLE	m_hDisconnectEvent;
	HANDLE	m_hConnectThread;
	HANDLE	m_hConnectTimer;
	HANDLE	m_hReconnectTimer;
	HANDLE	m_hWorkTimeTimer;
	
	CLock	m_ConnectLock;

	LARGE_INTEGER m_liLogon;
	LARGE_INTEGER m_liLogoff;

	vector<CGateway>			m_vecGateways;
	map<int, vector<CAccount> > m_mapBinToAccounts;

	bool	m_bIseConnected;

private:

	HRESULT	RealConnect();

protected:

	bool    IsIseConnected() const {return m_bIseConnected;}

	CISEManagerBase()
	{
		m_hStopConnectEvent = NULL;
		m_hConnectEvent = NULL;
		m_hDisconnectEvent = NULL;
		m_hConnectThread = NULL;
		m_hConnectTimer = NULL;
		m_hReconnectTimer = NULL;
		m_hWorkTimeTimer = NULL;

		m_liLogon.QuadPart = 0;
		m_liLogoff.QuadPart = 0;

		m_bIseConnected = false;
		m_dwRefs = 0;
		
	}

	DWORD Start()
	{
		m_hStopConnectEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if(m_hStopConnectEvent == NULL)
			return GetLastError();

		unsigned int uiID;
		m_hConnectThread = (HANDLE)_beginthreadex(NULL, 0, ConnectThread, this, 0, &uiID);
		if(m_hConnectThread == (HANDLE)-1)
			return GetLastError();
			
		m_hConnectTimer = CreateWaitableTimer(NULL, FALSE, NULL);
		if(m_hConnectTimer == NULL)
			return GetLastError();

		m_hReconnectTimer = CreateWaitableTimer(NULL, FALSE, NULL);
		if(m_hReconnectTimer == NULL)
			return GetLastError();

		m_hWorkTimeTimer = CreateWaitableTimer(NULL, FALSE, NULL);
		if(m_hWorkTimeTimer == NULL)
			return GetLastError();

		return 0;
	}

	void Stop()
	{
		if(m_hStopConnectEvent)
		{
			SetEvent(m_hStopConnectEvent);
			WaitForSingleObject(m_hConnectThread, INFINITE);
			CloseHandle(m_hStopConnectEvent);
			CloseHandle(m_hConnectThread);
			CloseHandle(m_hConnectTimer);
			CloseHandle(m_hReconnectTimer);
			CloseHandle(m_hWorkTimeTimer);
			
			m_hStopConnectEvent = NULL;
			m_hConnectEvent = NULL;
			m_hDisconnectEvent = NULL;
			m_hConnectThread = NULL;
			m_hConnectTimer = NULL;
			m_hReconnectTimer = NULL;
			m_hWorkTimeTimer = NULL;
		}
	}

	void QueueConnectEvent(long lTimer)
	{
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
		liDueTime.QuadPart *= -1;

		SetWaitableTimer(m_hConnectTimer, &liDueTime, 0, NULL, NULL, 0);
	}

	void QueueReconnectEvent(long lTimer)
	{
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
		liDueTime.QuadPart *= -1;

		SetWaitableTimer(m_hReconnectTimer, &liDueTime, 0, NULL, NULL, 0);
	}

	void QueueWorkTimeEvent(PLARGE_INTEGER pliInterval = NULL)
	{
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = pliInterval ? pliInterval->QuadPart : (ULONGLONG)1000 * 10000;
		liDueTime.QuadPart *= -1;

		if(m_liLogon.QuadPart != m_liLogoff.QuadPart) //if same values then work time timer don`t need
		{
			IseTrace(enInfo, "Work time timer interval set at %d seconds", (liDueTime.QuadPart*-1)/1000/10000);
			SetWaitableTimer(m_hWorkTimeTimer, &liDueTime, 0, NULL, NULL, 0);
		}
	}

	bool IsWorkTime(PLARGE_INTEGER pliTimerInterval = NULL) const
	{
		if(m_liLogon.QuadPart == m_liLogoff.QuadPart)
		{
			return true; //if same values then work always
		}

		SYSTEMTIME st;
		::GetLocalTime(&st);
		LARGE_INTEGER liNow;
		liNow.QuadPart = (ULONGLONG)(st.wHour * 3600 + st.wMinute * 60 + st.wSecond) * 1000 * 10000;

		bool bYes;

		if(m_liLogon.QuadPart < m_liLogoff.QuadPart)
		{
			bYes = (liNow.QuadPart > m_liLogon.QuadPart) && (liNow.QuadPart < m_liLogoff.QuadPart);

			if(NULL == pliTimerInterval) return bYes;

			if(bYes)
			{
				pliTimerInterval->QuadPart = m_liLogoff.QuadPart - liNow.QuadPart;
			}
			else
			{
				pliTimerInterval->QuadPart = liNow.QuadPart > m_liLogoff.QuadPart ? 
					(ULONGLONG)24*3600*1000*10000 - liNow.QuadPart + m_liLogon.QuadPart :
					m_liLogon.QuadPart - liNow.QuadPart;
			}
		}
		else
		{
			bYes = (liNow.QuadPart > m_liLogon.QuadPart) || (liNow.QuadPart < m_liLogoff.QuadPart);

			if(NULL == pliTimerInterval) return bYes;

			if (bYes)
			{
				pliTimerInterval->QuadPart = liNow.QuadPart > m_liLogon.QuadPart ? 
					(ULONGLONG)24*3600*1000*10000 - liNow.QuadPart + m_liLogoff.QuadPart :
					m_liLogoff.QuadPart - liNow.QuadPart;
			}
			else
			{
				pliTimerInterval->QuadPart = m_liLogon.QuadPart - liNow.QuadPart;
			}
			
		}

		pliTimerInterval->QuadPart += 1000*10000;
		return bYes;
	}

	static UINT __stdcall ConnectThread(void* pThisClass)
	{
		CISEManagerBase* pThis = (CISEManagerBase*)pThisClass;

		const HANDLE hEvents[] = {pThis->m_hReconnectTimer,
								  pThis->m_hConnectTimer,
								  pThis->m_hWorkTimeTimer,
								  pThis->m_hStopConnectEvent};

		while(true)
		{
			DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE);

			switch(dwRes)
			{
				case WAIT_OBJECT_0://reconnection event
				{
					pThis->m_bIseConnected = false;

					try
					{
						pThis->Logoff();
					}
					catch(CISEException&)
					{
					}

					pThis->IseTrace(enWarning, "------------- DISCONNECTED ---------------");

					if(pThis->IsLogonEnabled() && pThis->IsWorkTime())
					{
						pThis->IseTrace(enInfo, "Full reconnection will be in 60 seconds...");

						pThis->QueueConnectEvent(RECONNECTION_TIME);
					}
				}
				break;

				case WAIT_OBJECT_0 + 1://connection event
				{
					if(pThis->IsLogonEnabled() && pThis->IsWorkTime())
					{
						HRESULT hr = pThis->RealConnect();

						if(pThis->IsLogonEnabled())
						{
							if(SUCCEEDED(hr))
							{
								pThis->m_bIseConnected = true;
								pThis->OnConnected(pThis->GetID());
							}
							else
							{
								pThis->OnDisconnected(pThis->GetID());
							}
						}
					}
				}
				break;

				case WAIT_OBJECT_0 + 2://worktime timer
				{
					LARGE_INTEGER liInterval;
					if(pThis->IsWorkTime(&liInterval))
					{
						pThis->IseTrace(enInfo, "It is the beginning of the trading day. Getting connected...");
						pThis->QueueConnectEvent(1);
					}
					else
					{
						pThis->IseTrace(enInfo, "It is the end of the trading day. Getting disconnected...");							
						pThis->QueueReconnectEvent(1);
					}
					pThis->QueueWorkTimeEvent(&liInterval);
				}
				break;

				default://exit
				{
					try
					{
						pThis->Logoff();
					}
					catch(CISEException&)
					{
					}
					return 0;
				}
			}
		}
	}


	HRESULT		Connect();
	HRESULT		Disconnect();

protected:
	
	DWORD		m_dwRefs;
	mutable CLock	m_RefsLock;

protected:

	HRESULT ComError(HRESULT hr, WCHAR* szError)
	{

		CComPtr<ICreateErrorInfo> spCEI;
		if(SUCCEEDED(::CreateErrorInfo(&spCEI)))
		{
			spCEI->SetDescription(szError);
			
			CComQIPtr<IErrorInfo> spEI = spCEI;
			ATLASSERT(spEI);
			
			::SetErrorInfo(0, spEI);
		}
		return hr;
	}
};

template<class _O, class _R>
class CISEManagerObject : virtual public CISEManagerBase
{
protected:

	map<_O, CObjectContext<_R> > m_mapObjCtx;
	
	HANDLE						 m_hRequestEvent;
	HANDLE						 m_hRequestThread;
	HANDLE						 m_hRequestStop;

	HANDLE						 m_hResponseEvent;
	HANDLE						 m_hResponseThread;
	HANDLE						 m_hResponseStop;

protected:

	CISEManagerObject<_O, _R>()
	{
		m_hRequestEvent = NULL;
		m_hRequestThread = NULL;
		m_hRequestStop = NULL;
		m_hResponseEvent = NULL;
		m_hResponseThread = NULL;
		m_hResponseStop = NULL;
	}

	virtual bool  OnRequest(_O pObject, CRequestContext<_R>& Request){return true;};
	virtual void  OnResponse(_O pObject, const CResponseContext<_R>& ResponseCtx){};
	virtual void  OnRemoveObject(_O pObject){};

	static UINT __stdcall RequestThread(void* pThisClass)
	{
		CISEManagerObject<_O, _R>* pThis = (CISEManagerObject<_O, _R>*)pThisClass;
		return pThis->RequestProc();
	}
	
	static UINT __stdcall ResponseThread(void* pThisClass)
	{
		CISEManagerObject<_O, _R>* pThis = (CISEManagerObject<_O, _R>*)pThisClass;
		return pThis->ResponseProc();
	}

	virtual UINT ResponseProc()
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		const HANDLE hEvents[] = {m_hResponseEvent, m_hResponseStop};

		while(true)
		{
			DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE);

			switch(dwRes)
			{
				case WAIT_OBJECT_0:
				{
					CAutoLock RefsLock(&m_RefsLock);

					for(map<_O, CObjectContext<_R> >::iterator itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end();)
					{
						CObjectContext<_R>& Ctx = itObj->second;
						_O pObject = itObj->first;

						if(IsConnected(pObject) == false)
						{
							itObj++;
							continue;
						}

						while(!Ctx.m_queEvents.empty())
						{
							// copy event data since it can be deleted
							CResponseContext<_R> ResponseCtx = Ctx.m_queEvents.front();
							Ctx.m_queEvents.pop();

							set<CResponseContext<_R> >::iterator itRes = Ctx.m_setEvents.find(ResponseCtx);
							if(itRes != Ctx.m_setEvents.end())
							{
								ResponseCtx.m_Data = itRes->m_Data; ///!!!///???
								Ctx.m_setEvents.erase(itRes);
							}

							if(ResponseCtx.m_dwObjectID == 0 || 
							   (pObject->GetID() == ResponseCtx.m_dwObjectID && 
							   IsRequestPresent(pObject, _EventTypeEnum(ResponseCtx.m_Type & ~_enRequestError), ResponseCtx.m_Request)))
							{
								pObject->AddRef();
								m_RefsLock.Unlock();

								OnResponse(pObject, ResponseCtx);

								m_RefsLock.Lock();
								pObject->Release();
							}							

							if(IsConnected(pObject) == false)
							{
								// the object was deleted - it was corrupted
								itObj = m_mapObjCtx.begin();
								break;
							}
						}

						itObj++;
					}
				}
				break;

				default:
				{
					CoUninitialize();
					return 0;
				}
			}
		}
	}

	virtual UINT RequestProc()
	{
		const HANDLE hEvents[] = {m_hRequestEvent, m_hRequestStop};
		while(true)
		{
			DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE);
			switch(dwRes)
			{
				case WAIT_OBJECT_0:
				{
					map<_O, CObjectContext<_R> >::iterator itObj;
					deque<CRequestContext<_R> >::iterator itReq;

					CAutoLock RefsLock(&m_RefsLock);

					for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); )
					{
						_O pObject = itObj->first;
						CObjectContext<_R>& Ctx = itObj->second;

						bool bConnected = true;

						while(!Ctx.m_queRequests.empty())
						{
							if(Ctx.m_queRequests.front().m_bRemove == true ||
							   IsRequestPresent(pObject, Ctx.m_queRequests.front()))
							{
								m_RefsLock.Unlock();

								bConnected = OnRequest(pObject, Ctx.m_queRequests.front());
								
								m_RefsLock.Lock();

								if(!bConnected)
									break;
							}

							Ctx.m_queRequests.pop();
						}

						if(!bConnected)
							break;

						if(Ctx.m_bZombie == true)
						{
							m_RefsLock.Unlock();

							OnRemoveObject(pObject);

							m_RefsLock.Lock();

							if(Ctx.m_bZombie == true)
							{
								itObj = m_mapObjCtx.erase(itObj);
								continue;
							}
						}

						itObj++;
					}
				}
				break;

				default:
				{
					return 0;
				}
			}
		}
	}

public:

	virtual void OnConnected(const DWORD dwSession)
	{
		SetEvent(m_hRequestEvent);
	}

	//
	// Request & responce queues
	//

	HRESULT		AddRequest(const _O pObject, _R& Request, 
		const _EventTypeEnum Type, const bool bSubscription = false)
	{
		CAutoLock	Lock(&m_RefsLock);

		if(IsConnected(pObject) == false)
			return ComError(E_FAIL, L"Object is not connected");

		map<_O, CObjectContext<_R> >::iterator itObj = m_mapObjCtx.find(pObject);

		CObjectContext<_R>& Ctx = itObj->second;
		CRequestContext<_R> Req(Type, Request);
		
		pair<set<CRequestContext<_R> >::iterator, bool> itInsReq = Ctx.m_setRequests.insert(Req);
		itInsReq.first->m_dwObjectID = pObject->GetID();

		//if(bSubscription == false || itInsReq.second)
		{
			itInsReq.first->m_bRemove = false;
			Ctx.m_queRequests.push(*(itInsReq.first));
		}

		SetEvent(m_hRequestEvent);

		return S_OK;
	}

	virtual bool IsRequestPresent(const _O pObject, const _EventTypeEnum Type, const _R& Request) const
	{
		CRequestContext<_R> Req(Type, Request);
		return IsRequestPresent(pObject, Req);
	}


	virtual bool IsRequestPresent(const _O pObject, const CRequestContext<_R>& Request) const
	{
		CAutoLock	Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::const_iterator itObj = m_mapObjCtx.find(pObject);
		if(itObj != m_mapObjCtx.end())
		{
			set<CRequestContext<_R> >::const_iterator itReq = itObj->second.m_setRequests.find(Request);
			if(itReq != itObj->second.m_setRequests.end())
				return true;
		}

		return false;
	}

	HRESULT		RemoveRequest(const _O pObject, _R& Request, 
		const _EventTypeEnum Type, const bool bSubscription = false)
	{
		CAutoLock	Lock(&m_RefsLock);

		if(IsConnected(pObject) == false)
			return ComError(E_FAIL, L"Object is not connected");

		map<_O, CObjectContext<_R> >::iterator itObj = m_mapObjCtx.find(pObject);
		CObjectContext<_R>& Ctx = itObj->second;

		if(Type == _enNoRequest || !Request)
		{
			// if request is empty then delete all the requests of its type
			for(set<CRequestContext<_R> >::iterator itReq  = Ctx.m_setRequests.begin();
				itReq != Ctx.m_setRequests.end(); )
			{
				if(Type == _enNoRequest || (itReq->m_Type & Type))
				{
					set<CRequestContext<_R> >::iterator itDelReq = itReq++;
					if(!*itDelReq)
					{
						Ctx.m_setRequests.erase(itDelReq);
					}
					else
					{
						RemoveRequest(pObject, itDelReq->m_Data, itDelReq->m_Type, bSubscription);
					}
				}
				else
				{
					itReq++;
				}
			}
		}
		else
		{
			// otherwise delete only this request
			CRequestContext<_R> Req(Type, Request);
			
			set<CRequestContext<_R> >::iterator itReq = Ctx.m_setRequests.find(Req);
			if(itReq != Ctx.m_setRequests.end())
			{
				Ctx.m_setRequests.erase(itReq);
			}

			if(bSubscription)
			{
				Req.m_bRemove = true;
				Ctx.m_queRequests.push(Req);
			}
		}

		SetEvent(m_hRequestEvent);
		
		return S_OK;
	}

	HRESULT AddResponse(const _O pObject, _EventTypeEnum Type, 
		const _R& Request, const CResponseData& Data, const dwObjectID,
		bool bCache = false) ///!!!
	{
		CAutoLock	Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::iterator it = m_mapObjCtx.find(pObject);
		
		if(it != m_mapObjCtx.end())
		{
			CObjectContext<_R>& Ctx = it->second;

			if(!Ctx.m_bZombie)
			{
				CResponseContext<_R> Event(Type, Request);
				Event.m_Data = Data;
				Event.m_dwObjectID = dwObjectID;

				if (bCache)
				{
					pair<set<CResponseContext<_R> >::iterator, bool> itInsRes = Ctx.m_setEvents.insert(Event);
					if (true == itInsRes.second)
					{
						Ctx.m_queEvents.push(Event);
					}
					else
					{
						itInsRes.first->m_Data = Event.m_Data;
					}
				}
				else
				{
					Ctx.m_queEvents.push(Event);
				}
				
				SetEvent(m_hResponseEvent);
			}
		}
		
		return S_OK;
	}

	//
	//	Start - stop
	//

	DWORD Start(bool bRequest = true, bool bResponse = true)
	{
		if(bRequest)
		{
			m_hRequestEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			if(m_hRequestEvent == NULL)
				return GetLastError();

			m_hRequestStop = CreateEvent(NULL, TRUE, FALSE, NULL);
			if(m_hRequestStop == NULL)
				return GetLastError();

			unsigned int uiID;
			m_hRequestThread = (HANDLE)_beginthreadex(NULL, 0, RequestThread, this, 0, &uiID);
			if(m_hRequestThread == (HANDLE)-1)
				return GetLastError();
		}

		if(bResponse)
		{
			m_hResponseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			if(m_hResponseEvent == NULL)
				return GetLastError();

			m_hResponseStop = CreateEvent(NULL, TRUE, FALSE, NULL);
			if(m_hResponseStop == NULL)
				return GetLastError();

			unsigned int uiID;
			m_hResponseThread = (HANDLE)_beginthreadex(NULL, 0, ResponseThread, this, 0, &uiID);
			if(m_hResponseThread == (HANDLE)-1)
				return GetLastError();
		}

		return 0;
	}

	void Stop()
	{
		if(m_hRequestStop)
		{
			SetEvent(m_hRequestStop);
			WaitForSingleObject(m_hRequestThread, INFINITE);
			CloseHandle(m_hRequestThread);
			CloseHandle(m_hRequestEvent);
			CloseHandle(m_hRequestStop);
		}

		if(m_hResponseStop)
		{
			SetEvent(m_hResponseStop);
			WaitForSingleObject(m_hResponseThread, INFINITE);
			CloseHandle(m_hResponseThread);
			CloseHandle(m_hResponseEvent);
			CloseHandle(m_hResponseStop);
		}

		m_hRequestEvent = NULL;
		m_hRequestThread = NULL;
		m_hRequestStop = NULL;
		m_hResponseEvent = NULL;
		m_hResponseThread = NULL;
		m_hResponseStop = NULL;
	}

	//
	// Connection & disconnection
	//

	HRESULT Connect(const _O pObject, bool bNotification = true)
	{
		{
			CAutoLock Lock(&m_RefsLock);

			if(m_hRequestThread == NULL)
			{
				DWORD dwRes = Start();
				if(dwRes)
					return ComError(E_FAIL, L"Failed to start object's thread.");
			}

			map<_O, CObjectContext<_R> >::iterator itCtx = m_mapObjCtx.find(pObject);
			if(itCtx == m_mapObjCtx.end())
			{
				HRESULT		hr = CISEManagerBase::Connect();
				if(FAILED(hr))
					return hr;

				m_mapObjCtx[pObject] = CObjectContext<_R>();
			}
			else
			{
				itCtx->second.m_bZombie = false;
			}
		}

		if(bNotification && IsIseConnected())
			pObject->OnEvent(enEvConnected, "Connected.");

		return S_OK;
	}

	bool IsConnected(const _O pObject)
	{
		CAutoLock Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::iterator it;

		it = m_mapObjCtx.find(pObject);

		if (it == m_mapObjCtx.end())
			return false;

		return (it->second.m_bZombie == false);
	}

	HRESULT	Disconnect(const _O pObject)
	{
		CAutoLock Lock(&m_RefsLock);

		

		map<_O, CObjectContext<_R> >::iterator itCtx = m_mapObjCtx.find(pObject);
		
		if(itCtx != m_mapObjCtx.end() && !itCtx->second.m_bZombie)
		{
			RemoveRequest(itCtx->first, _R(), _enNoRequest);

			itCtx->second.m_bZombie = true;
			
			while(!itCtx->second.m_queEvents.empty())
				itCtx->second.m_queEvents.pop();

			m_RefsLock.Unlock();

			CISEManagerBase::Disconnect();

			m_RefsLock.Lock();
		}
		return S_OK;
	}
};


#endif