// SessionProxy.cpp : Implementation of CSessionProxy
#include "stdafx.h"
#include "EgTpPrx.h"
#include "SessionProxy.h"
#include "HostProxy.h"

/////////////////////////////////////////////////////////////////////////////
// CSessionProxy
_ATL_FUNC_INFO CSessionProxy::m_OnMessageParams = {CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_BSTR}};
_ATL_FUNC_INFO CSessionProxy::m_OnLogonParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};
_ATL_FUNC_INFO CSessionProxy::m_OnLogoffParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};
_ATL_FUNC_INFO CSessionProxy::m_OnSubjectStatusParams = {CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_UI4}};


HRESULT CSessionProxy::FinalConstruct()
{
	HRESULT hRes = S_OK;
    if (FAILED(hRes = m_spGIT.CreateInstance(CLSID_StdGlobalInterfaceTable)))
    {
	    return Error(L"Can't create GIT object", IID_ISession, hRes);
    }
	EgLib::CStrategyT<CSessionProxy, CMessageDataPtr>::Start();

	return hRes;
}

void CSessionProxy::CloseHostConnection()
{
	try
	{
		if(m_pHostProxy)
		{
			CHostProxy::CSubjectsVector leftSubjects;
			m_pHostProxy->GetSessionSubjects(this, leftSubjects);
			if(!leftSubjects.empty())
			{
				for(CHostProxy::CSubjectsVector::iterator itrDetach = leftSubjects.begin(); itrDetach!=leftSubjects.end(); ++itrDetach)
					m_pHostProxy->RemoveSessionSubject(this, *itrDetach);
			}
			DettachFromTransport();
			m_pHostProxy->DeleteSession(this);
		}
	}
	catch (_com_error& /*e*/)
	{

	}
}
void CSessionProxy::FinalRelease()
{
	try
	{
		CloseHostConnection();
		DettachFromTransport();
		if(m_pHostProxy)
			m_pHostProxy->DeleteSession(this);
	}
	catch(...){}
	EgLib::CStrategyT<CSessionProxy, CMessageDataPtr>::Stop(true, 500);

}

HRESULT	CSessionProxy::AttachToTransport()
{
	HRESULT	hRes = S_OK;
	//ObjectLock lock(this);

	if(m_dwSessionCoreCookie)
		return S_OK;
	
	if(!m_pHostProxy)
		return E_FAIL;
	
	//Unlock();
	IHostPtr spHost = m_pHostProxy->GetHostCore();
	//Lock();
	
	if(spHost == NULL)
		return Error(L"Empty Transport.Host object", IID_ISession, hRes);

	ISessionPtr spSessionCore;
	//Unlock();
    if (FAILED(hRes = spHost->CreateSession(&spSessionCore)))
    {
		//Lock();
	    return Error(L"Can't create Transport.Session object", IID_ISession, hRes);
    }
	//Lock();

    if( m_spGIT != NULL )
    {
        hRes = m_spGIT->RegisterInterfaceInGlobal(
          spSessionCore,                                     
          __uuidof(ISession),
          &m_dwSessionCoreCookie 
        );
    }
 
    // Advise for events
    if (FAILED(hRes = m_SessionCoreEvents.DispEventAdvise(spSessionCore)))
    {
	    return Error(L"Advise to session failed", IID_ISession, hRes);
    }

	return hRes;
}

void CSessionProxy::DettachFromTransport()
{
	//ObjectLock lock(this);

	BSTR bsEmpty = ::SysAllocString(L"");
	Fire_OnLogoff(bsEmpty);
	::SysFreeString(bsEmpty);

    if (m_dwSessionCoreCookie)
	{
  		ISessionPtr spSessionCore;

		if( m_spGIT != NULL )
		{             
			HRESULT hRes = m_spGIT->GetInterfaceFromGlobal(                 
				m_dwSessionCoreCookie,
				__uuidof(ISession),                  
				reinterpret_cast< void** >(&spSessionCore));
				
			if( hRes == S_OK )
			{
				//Unlock();
			    m_SessionCoreEvents.DispEventUnadvise(spSessionCore);
				//Lock();
    			hRes = m_spGIT->RevokeInterfaceFromGlobal(m_dwSessionCoreCookie);
			}
		}	

		m_dwSessionCoreCookie = 0;
	}
}

STDMETHODIMP CSessionProxy::Subscribe(BSTR bsSubject)
{
	HRESULT	hRes = S_OK;

	//ObjectLock lock(this);

    if ( !m_dwSessionCoreCookie )
		return Error(L"Transport service stopped", IID_ISession, E_FAIL);

	try
	{
		if(m_pHostProxy)
		{
			//Unlock();
			m_pHostProxy->AddSessionSubject(this,bsSubject);
			//Lock();
		}
	}
	catch(_com_error& er)
	{
		//Lock();
		return er.Error();
	}

	if( m_spGIT != NULL )
	{        
		ISessionPtr spSessionCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwSessionCoreCookie,
					__uuidof(ISession),                  
					reinterpret_cast< void** >(&spSessionCore));

		if(SUCCEEDED(hRes))
			hRes = spSessionCore->Subscribe(bsSubject);
	}

	return hRes;
}

STDMETHODIMP CSessionProxy::Unsubscribe(BSTR bsSubject)
{
	HRESULT	hRes = S_OK;

	//ObjectLock lock(this);

    if ( !m_dwSessionCoreCookie )
		return Error(L"Transport service stopped", IID_ISession, E_FAIL);

	try
	{
		if(m_pHostProxy)
		{
			//Unlock();
			m_pHostProxy->RemoveSessionSubject(this, bsSubject);
			//Lock();
		}
	}
	catch(_com_error& er)
	{
		//Lock();
		return er.Error();
	}

	if( m_spGIT != NULL )
	{        
		ISessionPtr spSessionCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwSessionCoreCookie,
					__uuidof(ISession),                  
					reinterpret_cast< void** >(&spSessionCore));

		if(SUCCEEDED(hRes))
			hRes = spSessionCore->Unsubscribe(bsSubject);
	}

	return hRes;
}

STDMETHODIMP CSessionProxy::SendMessageX(BSTR bsSubject, MsgPropertiesEnum enMsgProp, BSTR bsMessage)
{
	HRESULT	hRes = S_OK;

	//ObjectLock lock(this);

    if ( !m_dwSessionCoreCookie )
		return Error(L"Transport service stopped", IID_ISession, E_FAIL);
	
	if( m_spGIT != NULL )
	{        
		ISessionPtr spSessionCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwSessionCoreCookie,
					__uuidof(ISession),                  
					reinterpret_cast< void** >(&spSessionCore));

		if(SUCCEEDED(hRes))
			hRes = spSessionCore->SendMessageX(bsSubject,enMsgProp,bsMessage);
	}

	return hRes;
}

STDMETHODIMP CSessionProxy::get_IsLogoned(VARIANT_BOOL *pVal)
{
	HRESULT	hRes = S_OK;

	//ObjectLock lock(this);

    if ( !m_dwSessionCoreCookie )
		return Error(L"Transport service stopped", IID_ISession, E_FAIL);

	if( m_spGIT != NULL )
	{        
		ISessionPtr spSessionCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwSessionCoreCookie,
					__uuidof(ISession),                  
					reinterpret_cast< void** >(&spSessionCore));

		if(SUCCEEDED(hRes))
			hRes = spSessionCore->get_IsLogoned(pVal);
	}

	return hRes;
}

STDMETHODIMP CSessionProxy::CheckSubjectStatus(BSTR bsSubject)
{
	HRESULT	hRes = S_OK;

	//ObjectLock lock(this);

    if ( !m_dwSessionCoreCookie)
		return Error(L"Transport service stopped", IID_ISession, E_FAIL);

	if( m_spGIT != NULL )
	{        
		ISessionPtr spSessionCore;
		hRes = m_spGIT->GetInterfaceFromGlobal(                 
					m_dwSessionCoreCookie,
					__uuidof(ISession),                  
					reinterpret_cast< void** >(&spSessionCore));

		if(SUCCEEDED(hRes))
			hRes = spSessionCore->CheckSubjectStatus(bsSubject);
	}

	return hRes;
}

STDMETHODIMP CSessionProxy::OnMessage(BSTR Subject, BSTR Message)
{
	//Fire_OnMessage(Subject, Message);
	PushData(CMessageDataPtr(new CMessageData(Subject, Message)));
	return S_OK;
}

STDMETHODIMP CSessionProxy::OnLogon(BSTR Router)
{
	return Fire_OnLogon(Router);
}

STDMETHODIMP CSessionProxy::OnLogoff(BSTR Router)
{
	return Fire_OnLogoff(Router);
}

STDMETHODIMP CSessionProxy::OnSubjectStatus(BSTR Subject, unsigned long ulNumSubscribers)
{
	return Fire_OnSubjectStatus(Subject,ulNumSubscribers);
}

void CSessionProxy::OnData(CMessageDataPtr& spData)
{
	try
	{
		if(spData)
			Fire_OnMessage(spData->GetSubject(), spData->GetMessage());
	}
	catch (...)
	{
	}
}
