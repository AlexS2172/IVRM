// Session.cpp : Implementation of CSession
#include "stdafx.h"
#include "Transport.h"
#include "Session.h"
#include "Host.h"

/////////////////////////////////////////////////////////////////////////////
// CSession

HRESULT CSession::FinalConstruct( )
{
	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("Start FinalConstruct"));
	HRESULT hr = CoCreateGuid(&m_guidKey);
	if(FAILED(hr))
	{
		CEgLibTraceManager::Trace(LogError,_T("Session"),_T("FinalConstruct failed. CoCreateGuid error %d"),hr);
		return hr;
	}

	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("FinalConstruct completed"));
	return S_OK;
} 

void CSession::FinalRelease( )
{
	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("Start FinalRelease"));

	if (m_pHost == NULL)
		return;

	m_pHost->DestroySession(this);
	
	m_storLocal.Clear();
	
	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("FinalRelease completed"));
}

STDMETHODIMP CSession::Subscribe(BSTR bsSubject)
{
	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("Start Subscribe"));
	if (m_pHost == NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Session"),_T("Subscribe failed. Empty Host."));
		return Error(L"Can't subscribe. Host doesn't exists.", IID_ISession, E_FAIL);
	}
	
	HRESULT hr = m_pHost->Subscribe(bsSubject,this);
	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("Subscribe completed"));
	return hr;
}

STDMETHODIMP CSession::Unsubscribe(BSTR bsSubject)
{
	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("Start Unsubscribe"));
	if (m_pHost == NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Session"),_T("Unsubscribe failed. Empty Host."));
		return Error(L"Can't unsubscribe. Host doesn't exists.", IID_ISession, E_FAIL);
	}
	HRESULT hr = m_pHost->Unsubscribe(bsSubject,this);
	CEgLibTraceManager::Trace(LogDebug,_T("Session"),_T("Unsubscribe completed"));
	return hr;
}

STDMETHODIMP CSession::SendMessage(BSTR bsSubject, MsgPropertiesEnum enMsgProp, BSTR bsMessage)
{
	CEgLibTraceManager::Trace(LogError,_T("Session"),_T("Start SendMessage"));
	if (m_pHost == NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Session"),_T("SendMessage failed. Empty Host."));
		return Error(L"Can't send message. Host doesn't exists.", IID_ISession, E_FAIL);
	}
	HRESULT hr = m_pHost->SendMessage(bsSubject,enMsgProp,bsMessage,this);
	CEgLibTraceManager::Trace(LogError,_T("Session"),_T("SendMessage completed"));
	return hr;
}

STDMETHODIMP CSession::get_IsLogoned(VARIANT_BOOL *pVal)
{
	CEgLibTraceManager::Trace(LogInfo,_T("Session"),_T("Start get_IsLogoned"));
	if (m_pHost == NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Session"),_T("get_IsLogoned failed. Empty Host."));
		return Error(L"Host doesn't exists.", IID_ISession, E_FAIL);
	}
	*pVal = (m_pHost->IsLogoned() ? VARIANT_TRUE : VARIANT_FALSE);
	CEgLibTraceManager::Trace(LogInfo,_T("Session"),_T("get_IsLogoned completed"));

	return S_OK;
}
STDMETHODIMP CSession::CheckSubjectStatus(BSTR bsSubject)
{
	CEgLibTraceManager::Trace(LogInfo,_T("Session"),_T("Start CheckSubjectStatus"));
	if (m_pHost == NULL)
	{
		CEgLibTraceManager::Trace(LogError,_T("Session"),_T("get_IsLogoned failed. Empty Host."));
		return Error(L"Can't check subject status. Host doesn't exists.", IID_ISession, E_FAIL);
	}
	HRESULT hr =  m_pHost->CheckSubjectStatus(bsSubject,this);
	CEgLibTraceManager::Trace(LogInfo,_T("Session"),_T("CheckSubjectStatus completed"));
	return hr;
}
