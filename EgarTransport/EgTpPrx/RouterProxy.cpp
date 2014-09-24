// RouterProxy.cpp : Implementation of CRouterProxy
#include "stdafx.h"
#include "EgTpPrx.h"
#include "RouterProxy.h"
#include "ServiceMonitor.h"

/////////////////////////////////////////////////////////////////////////////
// CRouterProxy
set<CRouterProxy*>	CRouterProxy::s_pRouters;
CLock			CRouterProxy::s_Lock;

_ATL_FUNC_INFO CRouterProxy::m_OnClientConnectedParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };
_ATL_FUNC_INFO CRouterProxy::m_OnClientDisconnectedParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };
_ATL_FUNC_INFO CRouterProxy::m_OnTransportStoppedParams = {CC_STDCALL, VT_EMPTY, 0 };


STDMETHODIMP CRouterProxy::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IRouterProxy,
		&IID_IRouter
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CRouterProxy::Start()
{
	DWORD dwErr = g_ServiceMonitor.StartRouter();
	if(ERROR_SUCCESS != dwErr)
		return Error(_T("Can't start router service"), IID_IRouterProxy,HRESULT_FROM_WIN32(dwErr));		

	return S_OK;
}

STDMETHODIMP CRouterProxy::Stop()
{
	DWORD dwErr = g_ServiceMonitor.StopRouter();
	if(ERROR_SUCCESS != dwErr)
		return Error(_T("Can't stop router service"), IID_IRouterProxy,HRESULT_FROM_WIN32(dwErr));

	return S_OK;
}

HRESULT CRouterProxy::FinalConstruct()
{
	HRESULT hRes;

    if (FAILED(hRes = m_spGIT.CreateInstance(CLSID_StdGlobalInterfaceTable)))
    {
	    return Error(L"Can't create GIT object", IID_IRouterProxy, hRes);
    }

	// access to this global variable is not synchronized,
	// so create one object of CRouterProxy in module
	//	
	hRes = _AttachToTransport();
	_AddRouter(this);

	return hRes;
}


void CRouterProxy::FinalRelease()
{	
    _DettachFromTransport();

	_DeleteRouter(this);
}

HRESULT	CRouterProxy::_AttachToTransport()
{
	HRESULT	hRes = S_OK;
	ObjectLock lock(this);
	
	if(m_dwRouterCoreCookie)
		return S_OK;

	DWORD dwTransportStatus = g_ServiceMonitor.GetTransportStatus();

	if( dwTransportStatus != SERVICE_STOPPED && dwTransportStatus != SERVICE_RUNNING )
	    return Error(L"Can't create Transport.Router object. Egar Transport Service is busy.", IID_IHostProxy, E_FAIL);
	

  	IRouterPtr spRouterCore;
    if (FAILED(hRes = spRouterCore.CreateInstance(CLSID_Router)))
    {
	    return Error(L"Can't create Transport.Router object", IID_IRouterProxy, hRes);
    }

    if( m_spGIT != NULL )
    {
        hRes = m_spGIT->RegisterInterfaceInGlobal(
          spRouterCore,                                     
          __uuidof(IRouter),
          &m_dwRouterCoreCookie 
        );
    }

    // Advise for events
    if (FAILED(hRes = m_RouterCoreEvents.DispEventAdvise(spRouterCore)))
    {
	    return Error(L"Advise to router failed", IID_IRouterProxy, hRes);
    }

	return hRes;
}

void	CRouterProxy::_DettachFromTransport()
{
	ObjectLock lock(this);

    if (m_dwRouterCoreCookie)
	{
  		IRouterPtr spRouterCore;

		if( m_spGIT != NULL )
		{             
			HRESULT hRes = m_spGIT->GetInterfaceFromGlobal(                 
				m_dwRouterCoreCookie,
				__uuidof(IRouter),                  
				reinterpret_cast< void** >(&spRouterCore));
				
			if( hRes == S_OK )
			{
				Unlock();
			    m_RouterCoreEvents.DispEventUnadvise(spRouterCore);
				Lock();
    			hRes = m_spGIT->RevokeInterfaceFromGlobal(m_dwRouterCoreCookie);
			}
		}	

		m_dwRouterCoreCookie = 0;
	}
}

STDMETHODIMP CRouterProxy::get_HostStatistic(/*in*/ BSTR bsHostIP, /*[out, retval]*/ IHostStatisticData** ppStatistic)
{
	HRESULT	hRes = S_OK;

	ObjectLock lock(this);

    if ( !m_dwRouterCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
			return Error(L"Transport service stopped", IID_IRouterProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}
	}

	if( m_spGIT != NULL )
	{        
		IRouterPtr spRouterCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwRouterCoreCookie,
					__uuidof(IRouter),                  
					reinterpret_cast< void** >(&spRouterCore));

		if(SUCCEEDED(hRes))
			hRes = spRouterCore->get_HostStatistic(bsHostIP,ppStatistic);
	}
				
	return hRes;
}

STDMETHODIMP CRouterProxy::get_ActiveHosts(/*[out, retval]*/LPSAFEARRAY* ppHostsIPList)
{
	HRESULT	hRes = S_OK;

	ObjectLock lock(this);

    if ( !m_dwRouterCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
			return Error(L"Transport service stopped", IID_IRouterProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}
	}

	if( m_spGIT != NULL )
	{        
		IRouterPtr spRouterCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwRouterCoreCookie,
					__uuidof(IRouter),                  
					reinterpret_cast< void** >(&spRouterCore));

		if(SUCCEEDED(hRes))
			hRes = spRouterCore->get_ActiveHosts(ppHostsIPList);
	}

	return hRes;
}

STDMETHODIMP CRouterProxy::OnClientConnected(BSTR bsHostIP)
{
	return Fire_OnClientConnected(bsHostIP);
}

STDMETHODIMP CRouterProxy::OnClientDisconnected(BSTR bsHostIP)
{
	return Fire_OnClientDisconnected(bsHostIP);
}

STDMETHODIMP CRouterProxy::OnTransportStopped()
{
	ObjectLock lock(this);
	m_bIsStopManually = true;
	Unlock();
	_DettachFromTransport();
	Lock();
	return S_OK;
}

void CRouterProxy::ServiceState(DWORD dwState)
{
	Fire_OnServiceStatus(dwState);
}

void CRouterProxy::TransportServiceState(DWORD dwState)
{
	if(SERVICE_RUNNING == dwState)
	{
		ObjectLock lock(this);
		m_bIsStopManually = false;
		
		if( !m_dwRouterCoreCookie )
		{
			Unlock();
			_AttachToTransport();
			Lock();
		}
	}
	else if(SERVICE_STOPPED == dwState)
	{
		ObjectLock lock(this);
		if(!m_bIsStopManually)
		{
			Unlock();
			_DettachFromTransport();
			Lock();
		}
	}
}

void CRouterProxy::ServiceError(LPCTSTR pAction, DWORD dwError)
{
	string strError = pAction;
	strError += _T('\n');
	LPVOID lpMsgBuf;
	if(	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL ))
	{
		strError += (LPCTSTR)lpMsgBuf;
		::LocalFree( lpMsgBuf );
	}
	else
	{
		DWORD dwErr = GetLastError();
	}
	USES_CONVERSION;
	
	LPWSTR pwchError =  T2W(strError.c_str());

	BSTR bsError = ::SysAllocString(pwchError);

	Fire_OnServiceError(dwError,bsError);
	
	::SysFreeString(bsError);
}