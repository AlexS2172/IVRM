// HostProxy.cpp : Implementation of CHostProxy
#include "stdafx.h"
#include "EgTpPrx.h"
#include "HostProxy.h"
#include "SessionProxy.h"
#include "ServiceMonitor.h"
#include "RouterProxy.h"

/////////////////////////////////////////////////////////////////////////////
// CHostProxy

set<CHostProxy*>  CHostProxy::s_pHosts;
CLock		 CHostProxy::s_Lock;


_ATL_FUNC_INFO CHostProxy::m_OnLogonedParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };
_ATL_FUNC_INFO CHostProxy::m_OnLogoutParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR} };
_ATL_FUNC_INFO CHostProxy::m_OnDisabledParams = {CC_STDCALL, VT_EMPTY, 0 };
_ATL_FUNC_INFO CHostProxy::m_OnTransportStoppedParams = {CC_STDCALL, VT_EMPTY, 0 };

STDMETHODIMP CHostProxy::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IHostProxy,
		&IID_IHost
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CHostProxy::Start()
{
	DWORD dwErr = g_ServiceMonitor.StartTransport();
	if(ERROR_SUCCESS != dwErr)
		return Error(_T("Can't start transport service"), IID_IHostProxy,HRESULT_FROM_WIN32(dwErr));		

	return S_OK;
}

STDMETHODIMP CHostProxy::Stop()
{
	DWORD dwErr = g_ServiceMonitor.StopTransport();
	if(ERROR_SUCCESS != dwErr)
		return Error(_T("Can't stop transport service"), IID_IHostProxy,HRESULT_FROM_WIN32(dwErr));		

	return S_OK;
}

STDMETHODIMP CHostProxy::Restart()
{
	DWORD dwErr = g_ServiceMonitor.RestartTransport();
	if(ERROR_SUCCESS != dwErr)
		return Error(_T("Can't restart transport service"), IID_IHostProxy,HRESULT_FROM_WIN32(dwErr));

	return S_OK;
}

HRESULT CHostProxy::FinalConstruct()
{
	HRESULT hRes;

    if (FAILED(hRes = m_spGIT.CreateInstance(CLSID_StdGlobalInterfaceTable)))
    {
	    return Error(L"Can't create GIT object", IID_IHostProxy, hRes);
    }

	// access to this global variable is not synchronized,
	// so create one object of CHostProxy in module
	_AddHost(this);
	
	try
	{
		g_ServiceMonitor.Start(NULL);
	}
	catch(HRESULT& hr)
	{
		return Error(_T("Transport service is not properly installed"), IID_IHostProxy ,hr);
	}

 	hRes = _AttachToTransport();

	return hRes;
}

void CHostProxy::FinalRelease()
{
	try
	{
		g_ServiceMonitor.Stop();
		_DettachFromTransport();
		int iSessions = 0;

		if(!m_mapSessions.empty())
		{
			iSessions = m_mapSessions.size();
			std::vector<CSessionProxy*> sessionCashe;
			sessionCashe.reserve(iSessions);


			CSessionMap::iterator iterSessions = m_mapSessions.begin();
			for(;iterSessions != m_mapSessions.end(); iterSessions++)
				sessionCashe.insert(sessionCashe.end(), iterSessions->first);


			std::vector<CSessionProxy*>::iterator ite = sessionCashe.begin();
			for(;ite != sessionCashe.end(); ++ite)
				(*ite)->SetHost(NULL);

			m_mapSessions.clear();
		}

		_DeleteHost(this);
	}
	catch (...){}
}

HRESULT	CHostProxy::_AttachToTransport()
{
	HRESULT	hRes = S_OK;
	ObjectLock lock(this);

	if(m_dwHostCoreCookie)
		return S_OK;

	DWORD dwTransportStatus = g_ServiceMonitor.GetTransportStatus();

	if( dwTransportStatus != SERVICE_STOPPED && dwTransportStatus != SERVICE_RUNNING )
	    return Error(L"Can't create Transport.Host object. Egar Transport Service is busy.", IID_IHostProxy, E_FAIL);


  	IHostPtr spHostCore;
    if (FAILED(hRes = spHostCore.CreateInstance(CLSID_Host)))
    {
	    return Error(L"Can't create Transport.Host object", IID_IHostProxy, hRes);
    }

    if( m_spGIT != NULL )
    {
        hRes = m_spGIT->RegisterInterfaceInGlobal(
          spHostCore,                                     
          __uuidof(IHost),
          &m_dwHostCoreCookie 
        );
    }

    // Advise for events
    if (FAILED(hRes = m_HostCoreEvents.DispEventAdvise(spHostCore)))
    {
	    return Error(L"Advise to host failed", IID_IHostProxy, hRes);
    }

	if(!m_mapSessions.empty())
	{
		BSTR bsEmpty = ::SysAllocString(L"");

		if(FAILED(hRes = spHostCore->Logon(bsEmpty)))
		{
		    return Error(L"Host logon failed", IID_IHostProxy, hRes);
			::SysFreeString(bsEmpty);
		}
		::SysFreeString(bsEmpty);
	}
	
	CSessionMap::iterator iterSessions = m_mapSessions.begin();
	for(;iterSessions != m_mapSessions.end(); iterSessions++)
	{
		CSessionProxy* pSession = iterSessions->first; 
		if(FAILED(hRes = pSession->AttachToTransport()))
		    return Error(L"Can't attach to Transport.Session object", IID_IHostProxy, hRes);
		
		CSubjectsSet& setSubj = iterSessions->second;
		CSubjectsSet::iterator iterSubject = setSubj.begin();
		for(; iterSubject!=setSubj.end(); iterSubject++)
		{
			pSession->Subscribe(*iterSubject);
		}		
	}

	return hRes;
}

void	CHostProxy::_DettachFromTransport()
{
	ObjectLock lock(this);

	CSessionMap::iterator iterSessions = m_mapSessions.begin();
	for(;iterSessions != m_mapSessions.end(); iterSessions++)
	{
		iterSessions->first->DettachFromTransport();
	}

    if (m_dwHostCoreCookie)
	{
  		IHostPtr spHostCore;

		if( m_spGIT != NULL )
		{             
			HRESULT hRes = m_spGIT->GetInterfaceFromGlobal(                 
				m_dwHostCoreCookie,
				__uuidof(IHost),                  
				reinterpret_cast< void** >(&spHostCore));
				
			if( hRes == S_OK )
			{
				Unlock();
			    m_HostCoreEvents.DispEventUnadvise(spHostCore);
				Lock();
    			hRes = m_spGIT->RevokeInterfaceFromGlobal(m_dwHostCoreCookie);
			}
		}	

		m_dwHostCoreCookie = 0;
	}
}


STDMETHODIMP CHostProxy::CreateSession(ISession **ppSession)
{
	HRESULT	hRes = S_OK;
 	
	ObjectLock lock(this);

    if ( !m_dwHostCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
		    return Error(L"Transport service stopped.", IID_IHostProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}

	}

	if(!m_dwHostCoreCookie)
	    return Error(L"Can't access Transport service.", IID_IHostProxy, E_FAIL);

	CComObject<CSessionProxy>* pSessionPrx;
	hRes = CComObject<CSessionProxy>::CreateInstance(&pSessionPrx);

	if(FAILED(hRes))
		return Error(L"Transport proxy internal error.", IID_IHostProxy, hRes);

	pSessionPrx->AddRef();
	pSessionPrx->SetHost(this);

	hRes = pSessionPrx->AttachToTransport();
	if(FAILED(hRes))
		return Error(L"Transport proxy internal error.", IID_IHostProxy, hRes);

	*ppSession = pSessionPrx;

	m_mapSessions.insert(CSessionMap::value_type(pSessionPrx,CSubjectsSet()));

	return hRes;

}

STDMETHODIMP CHostProxy::Logout()
{
	HRESULT	hRes = S_OK;
 	
	ObjectLock lock(this);

    if ( !m_dwHostCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
		    return Error(L"Transport service stopped.", IID_IHostProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}

	}

	if( !m_dwHostCoreCookie )
	    return Error(L"Can't access Transport service.", IID_IHostProxy, E_FAIL);

	if( m_spGIT != NULL )
	{        
		IHostPtr spHostCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwHostCoreCookie,
					__uuidof(IHost),                  
					reinterpret_cast< void** >(&spHostCore));

		if(SUCCEEDED(hRes))
			hRes = spHostCore->Logout();
	}
				
	return hRes;
}

STDMETHODIMP CHostProxy::Logon(BSTR bsServerName)
{
	HRESULT	hRes = S_OK;
 	
	ObjectLock lock(this);

    if ( !m_dwHostCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
		    return Error(L"Transport service stopped.", IID_IHostProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}

	}

	if( !m_dwHostCoreCookie )
	    return Error(L"Can't access Transport service.", IID_IHostProxy, E_FAIL);

	if( m_spGIT != NULL )
	{        
		IHostPtr spHostCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwHostCoreCookie,
					__uuidof(IHost),                  
					reinterpret_cast< void** >(&spHostCore));

		if(SUCCEEDED(hRes))
			hRes = spHostCore->Logon(bsServerName);
	}
				
	return hRes;
}

STDMETHODIMP CHostProxy::get_Status(/*[out, retval]*/ HostStatusEnum* pVal)
{
	HRESULT	hRes = S_OK;
 	
	ObjectLock lock(this);

    if ( !m_dwHostCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
		    return Error(L"Transport service stopped.", IID_IHostProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}

	}

	if( !m_dwHostCoreCookie )
	    return Error(L"Can't access Transport service.", IID_IHostProxy, E_FAIL);

	if( m_spGIT != NULL )
	{        
		IHostPtr spHostCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwHostCoreCookie,
					__uuidof(IHost),                  
					reinterpret_cast< void** >(&spHostCore));

		if(SUCCEEDED(hRes))
			hRes = spHostCore->get_Status(pVal);
	}
				
	return hRes;
}

STDMETHODIMP CHostProxy::get_Statistic(/*[out, retval]*/ IHostStatisticData** ppStatistic)
{
	HRESULT	hRes = S_OK;
 	
	ObjectLock lock(this);

    if ( !m_dwHostCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
		    return Error(L"Transport service stopped.", IID_IHostProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}

	}

	if( !m_dwHostCoreCookie )
	    return Error(L"Can't access Transport service.", IID_IHostProxy, E_FAIL);

	if( m_spGIT != NULL )
	{        
		IHostPtr spHostCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwHostCoreCookie,
					__uuidof(IHost),                  
					reinterpret_cast< void** >(&spHostCore));

		if(SUCCEEDED(hRes))
			hRes = spHostCore->get_Statistic(ppStatistic);
	}
				
	return hRes;
}

STDMETHODIMP CHostProxy::get_RouterName(/*[out, retval]*/ BSTR* pRouterName)
{
	HRESULT	hRes = S_OK;
 	
	ObjectLock lock(this);

    if ( !m_dwHostCoreCookie )
	{
		if(m_bIsStopManually && g_ServiceMonitor.GetTransportStatus() != SERVICE_RUNNING)
		    return Error(L"Transport service stopped.", IID_IHostProxy, E_FAIL);
		else
		{
			Unlock();
			hRes = _AttachToTransport();
			Lock();
			if(FAILED(hRes))
				return hRes;
		}

	}

	if( !m_dwHostCoreCookie )
	    return Error(L"Can't access Transport service.", IID_IHostProxy, E_FAIL);

	if( m_spGIT != NULL )
	{        
		IHostPtr spHostCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwHostCoreCookie,
					__uuidof(IHost),                  
					reinterpret_cast< void** >(&spHostCore));

		if(SUCCEEDED(hRes))
			hRes = spHostCore->get_RouterName(pRouterName);
	}
				
	return hRes;
}

STDMETHODIMP CHostProxy::OnLogoned(BSTR bsRouterName)
{
	return Fire_OnLogoned(bsRouterName);
}

STDMETHODIMP CHostProxy::OnLogout(BSTR bsRouterName)
{
	return Fire_OnLogout(bsRouterName);
}

STDMETHODIMP CHostProxy::OnDisabled()
{
	return Fire_OnDisabled();
}

STDMETHODIMP CHostProxy::OnTransportStopped()
{
	ObjectLock lock(this);
	m_bIsStopManually = true;
	Unlock();
	_DettachFromTransport();
	Lock();

	return S_OK;
}

void CHostProxy::ServiceState(DWORD dwState)
{
	if(SERVICE_RUNNING == dwState)
	{
		ObjectLock lock(this);
		m_bIsStopManually = false;
		
		if( !m_dwHostCoreCookie )
		{
			Unlock();
			_AttachToTransport();
			Lock();
		}
		else
		{
			HostStatusEnum enStatus;
			if(IsRPCError(get_Status(&enStatus)))
			{
				Unlock();
				_DettachFromTransport();
				_AttachToTransport();
				Lock();
			}
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
	Fire_OnServiceStatus(dwState);
}

void CHostProxy::ServiceError(LPCTSTR pAction, DWORD dwError)
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

	_bstr_t bsError(strError.c_str());
	Fire_OnServiceError(dwError,bsError);
}