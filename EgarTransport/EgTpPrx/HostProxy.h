// HostProxy.h : Declaration of the CHostProxy

#ifndef __HOSTPROXY_H_
#define __HOSTPROXY_H_

#include "resource.h"       // main symbols
#include "EgTpPrxCP.h"     

class CHostProxy;
typedef IDispEventSimpleImpl2 < CHostProxy, &__uuidof(_IHostEvents) >  _CHostCoreEventsBase;

class CHostCoreEvents : public _CHostCoreEventsBase
{
public:
    CHostCoreEvents(CHostProxy* pInvoke) : 
        _CHostCoreEventsBase(pInvoke)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////
// CHostProxy
class ATL_NO_VTABLE CHostProxy : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHostProxy, &CLSID_HostProxy>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CHostProxy>,
	public IDispatchImpl<IHostProxy, &IID_IHostProxy, &LIBID_EGTPPRXLib>,
	public CProxy_IHostEvents< CHostProxy >
{
public:

	typedef set<_bstr_t>	CSubjectsSet;
	typedef vector<_bstr_t>	CSubjectsVector;
	typedef map<CSessionProxy*,CSubjectsSet> CSessionMap;

	CHostProxy() : m_HostCoreEvents(this), m_bIsStopManually(false),m_dwHostCoreCookie(0)
	{
	}
	HRESULT FinalConstruct();
	void FinalRelease();

//DECLARE_CLASSFACTORY_SINGLETON(CHostProxy)

DECLARE_REGISTRY_RESOURCEID(IDR_HOSTPROXY)
DECLARE_NOT_AGGREGATABLE(CHostProxy)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHostProxy)
	COM_INTERFACE_ENTRY(IHostProxy)
	COM_INTERFACE_ENTRY(IHost)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHostProxy)
CONNECTION_POINT_ENTRY(DIID__IHostProxyEvents)
END_CONNECTION_POINT_MAP()
	
	static _ATL_FUNC_INFO	m_OnLogonedParams;
	static _ATL_FUNC_INFO	m_OnLogoutParams;
	static _ATL_FUNC_INFO	m_OnDisabledParams;
	static _ATL_FUNC_INFO	m_OnTransportStoppedParams;

BEGIN_SINK_MAP(CHostProxy)
	SINK_ENTRY_INFO2(m_HostCoreEvents, __uuidof(_IHostEvents), DISPID_IHostEventsOnLogoned, OnLogoned, &m_OnLogonedParams)
	SINK_ENTRY_INFO2(m_HostCoreEvents, __uuidof(_IHostEvents), DISPID_IHostEventsOnLogout, OnLogout, &m_OnLogoutParams)
	SINK_ENTRY_INFO2(m_HostCoreEvents, __uuidof(_IHostEvents), DISPID_IHostEventsOnDisabled, OnDisabled, &m_OnDisabledParams)
	SINK_ENTRY_INFO2(m_HostCoreEvents, __uuidof(_IHostEvents), DISPID_IHostEventsOnTransportStopped, OnTransportStopped, &m_OnTransportStoppedParams)
END_SINK_MAP()

	STDMETHOD(OnLogoned)(BSTR bsRouterName);
	STDMETHOD(OnLogout)(BSTR bsRouterName);
	STDMETHOD(OnDisabled)();
	STDMETHOD(OnTransportStopped)();

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IHostProxy
public:
	STDMETHOD(CreateSession)(/*[out]*/ISession** ppSession);
	STDMETHOD(Logout)();
	STDMETHOD(Logon)(BSTR bsServerName);
	STDMETHOD(get_Status)(/*[out, retval]*/ HostStatusEnum* pVal);
	STDMETHOD(get_Statistic)(/*[out, retval]*/ IHostStatisticData** ppStatistic);
	STDMETHOD(get_RouterName)(/*[out, retval]*/ BSTR* pRouterName);

	STDMETHOD(Restart)();
	STDMETHOD(Stop)();
	STDMETHOD(Start)();

	IHostPtr GetHostCore()
	{
		ObjectLock lock(this);

		IHostPtr spHostCore;
		if( m_spGIT != NULL && m_dwHostCoreCookie)
		{        
			m_spGIT->GetInterfaceFromGlobal(                 
				m_dwHostCoreCookie,
				__uuidof(IHost),                  
				reinterpret_cast< void** >(&spHostCore));
		}
		return spHostCore;		
	}

	void DeleteSession(CSessionProxy* pSession)
	{
		ObjectLock lock(this);
		
		m_mapSessions.erase(pSession);
	}

	void AddSessionSubject(CSessionProxy* pSession, const _bstr_t& bsSubject)
	{
		ObjectLock lock(this);
		
		CSessionMap::iterator iterSessions = m_mapSessions.find(pSession);
		
		if(iterSessions != m_mapSessions.end())	
			iterSessions->second.insert(bsSubject);
	}
	void RemoveSessionSubject(CSessionProxy* pSession, const _bstr_t& bsSubject)
	{
		ObjectLock lock(this);
		
		CSessionMap::iterator iterSessions = m_mapSessions.find(pSession);
		
		if(iterSessions != m_mapSessions.end())	
			iterSessions->second.erase(bsSubject);
	}

	void  GetSessionSubjects(CSessionProxy* pSession, CSubjectsVector& subjects)
	{
		subjects.clear();
		ObjectLock lock(this);

		CSessionMap::iterator iterSessions = m_mapSessions.find(pSession);

		if(iterSessions != m_mapSessions.end())	
		{
			CSubjectsSet& setSubj = iterSessions->second;
			if(!setSubj.empty())
			{
				subjects.reserve(setSubj.size());
				for(CSubjectsSet::iterator itr = setSubj.begin(); itr!=setSubj.end(); ++itr)
					subjects.insert(subjects.end(), *itr);
			}
		}
	}

	void	ServiceError(LPCTSTR pAction, DWORD dwError);
	void	ServiceState(DWORD dwState);
		
private:
	HRESULT	_AttachToTransport();
	void	_DettachFromTransport();
			
private:
	IGlobalInterfaceTablePtr	m_spGIT;
	DWORD						m_dwHostCoreCookie;

	CHostCoreEvents				m_HostCoreEvents;
	
	bool						m_bIsStopManually;

	CSessionMap					m_mapSessions;


	static set<CHostProxy*>		s_pHosts;
	static CLock				s_Lock;

	static bool IsRPCError(HRESULT hr)
	{
		return FAILED(hr) && (0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr);
	}

	static	void _AddHost(CHostProxy* pProxy)
	{
		CAutoLock lock(s_Lock);
		s_pHosts.insert(pProxy);
	}
	static	void _DeleteHost(CHostProxy* pProxy)
	{
		CAutoLock lock(s_Lock);
		s_pHosts.erase(pProxy);
	}
public:
	static	CHostProxy* GetNextHost(CHostProxy* pPrevHost) 
	{
		CAutoLock lock(s_Lock);
		
		CHostProxy* pHost = NULL;

		set<CHostProxy*>::iterator iter = s_pHosts.upper_bound(pPrevHost);
		if(iter != s_pHosts.end())
		{
			pHost = *iter;
			pHost->AddRef();
		}
		return pHost;
	}
};

#endif //__HOSTPROXY_H_
