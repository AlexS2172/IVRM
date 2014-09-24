// RouterProxy.h : Declaration of the CRouterProxy

#ifndef __ROUTERPROXY_H_
#define __ROUTERPROXY_H_

#include "resource.h"       // main symbols
#include "EgTpPrxCP.h"

class CRouterProxy;
typedef IDispEventSimpleImpl2<CRouterProxy, &__uuidof(_IRouterEvents)>  _CRouterCoreEventsBase;

class CRouterCoreEvents : public _CRouterCoreEventsBase
{
public:
    CRouterCoreEvents(CRouterProxy* pInvoke) : 
        _CRouterCoreEventsBase(pInvoke)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////
// CRouterProxy
class ATL_NO_VTABLE CRouterProxy : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRouterProxy, &CLSID_RouterProxy>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CRouterProxy>,
	public IDispatchImpl<IRouterProxy, &IID_IRouterProxy, &LIBID_EGTPPRXLib>,
	public CProxy_IRouterEvents< CRouterProxy >
{
public:
	CRouterProxy() : m_RouterCoreEvents(this), m_bIsStopManually(false),m_dwRouterCoreCookie(0)
	{
	}

	HRESULT FinalConstruct();
	void FinalRelease();

//DECLARE_CLASSFACTORY_SINGLETON(CRouterProxy)

DECLARE_REGISTRY_RESOURCEID(IDR_ROUTERPROXY)
DECLARE_NOT_AGGREGATABLE(CRouterProxy)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRouterProxy)
	COM_INTERFACE_ENTRY(IRouterProxy)
	COM_INTERFACE_ENTRY(IRouter)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CRouterProxy)
CONNECTION_POINT_ENTRY(DIID__IRouterProxyEvents)
END_CONNECTION_POINT_MAP()

	static _ATL_FUNC_INFO	m_OnClientConnectedParams;
	static _ATL_FUNC_INFO	m_OnClientDisconnectedParams;
	static _ATL_FUNC_INFO	m_OnTransportStoppedParams;

BEGIN_SINK_MAP(CRouterProxy)
	SINK_ENTRY_INFO2(m_RouterCoreEvents, __uuidof(_IRouterEvents), DISPID_IRouterEventsOnClientConnected, OnClientConnected, &m_OnClientConnectedParams)
	SINK_ENTRY_INFO2(m_RouterCoreEvents, __uuidof(_IRouterEvents), DISPID_IRouterEventsOnClientDisconnected, OnClientDisconnected, &m_OnClientDisconnectedParams)
	SINK_ENTRY_INFO2(m_RouterCoreEvents, __uuidof(_IRouterEvents), DISPID_IRouterEventsOnTransportStopped, OnTransportStopped, &m_OnTransportStoppedParams)
END_SINK_MAP()

	STDMETHOD(OnClientConnected)(BSTR bsHostIP);
	STDMETHOD(OnClientDisconnected)(BSTR bsHostIP);
	STDMETHOD(OnTransportStopped)();

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IRouterProxy
public:
	STDMETHOD(get_HostStatistic)(/*in*/ BSTR bsHostIP, /*[out, retval]*/ IHostStatisticData** ppStatistic);
	STDMETHOD(get_ActiveHosts)(/*[out, retval]*/ LPSAFEARRAY* ppHostsIPList);
	STDMETHOD(Stop)();
	STDMETHOD(Start)();

public:
	void	ServiceError(LPCTSTR pAction, DWORD dwError);
	void	ServiceState(DWORD dwState);
	void	TransportServiceState(DWORD dwState);

	HRESULT	_AttachToTransport();
	void	_DettachFromTransport();

private:
	IGlobalInterfaceTablePtr	m_spGIT;
	DWORD						m_dwRouterCoreCookie;

	CRouterCoreEvents			m_RouterCoreEvents;

	bool						m_bIsStopManually;
	
	static set<CRouterProxy*>	s_pRouters;
	static CLock				s_Lock;

	static	void _AddRouter(CRouterProxy* pProxy)
	{
		CAutoLock lock(s_Lock);
		s_pRouters.insert(pProxy);
	}
	static	void _DeleteRouter(CRouterProxy* pProxy)
	{
		CAutoLock lock(s_Lock);
		s_pRouters.erase(pProxy);
	}
public:
	static	CRouterProxy* GetNextRouter(CRouterProxy* pPrevRouter = NULL)  
	{
		CAutoLock lock(s_Lock);
		
		CRouterProxy* pRouter = NULL;

		set<CRouterProxy*>::iterator iter = s_pRouters.upper_bound(pPrevRouter);
		if(iter != s_pRouters.end())
		{
			pRouter = *iter;
			pRouter->AddRef();
		}
		return pRouter;
	}
};


#endif //__ROUTERPROXY_H_
