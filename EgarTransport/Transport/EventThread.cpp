// EventThread.cpp: implementation of the CEventThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "transport.h"

#include "Host.h"
#include "Router.h"
#include "Session.h"
#include "EventThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


HRESULT CHostEventThread::Fire_OnMessage(CTpEventPtr pEvent)
{
	switch(pEvent->m_enDispId)
	{
		case DISPID_IHostEventsOnLogoned:
		{
			ATLASSERT(pEvent->m_bsParams);

			if(pEvent->m_bsParams)
				return m_pHost->Fire_OnLogoned(pEvent->m_bsParams);
	
		}
		break;
		case DISPID_IHostEventsOnLogout:
		{
			ATLASSERT(pEvent->m_bsParams);

			if(pEvent->m_bsParams)
				return m_pHost->Fire_OnLogout(pEvent->m_bsParams);
		}
		break;
		case DISPID_IHostEventsOnDisabled:
		{
			ATLASSERT(!pEvent->m_bsParams);

			return m_pHost->Fire_OnDisabled();
		}
		break;
		case DISPID_IHostEventsOnTransportStopped:
		{
			ATLASSERT(!pEvent->m_bsParams);

			return m_pHost->Fire_OnTransportStopped();
		}
		break;
		default:
			ATLASSERT(false);
		break;						
	}

	return S_OK;
}

HRESULT CRouterEventThread::Fire_OnMessage(CTpEventPtr pEvent)
{
	switch(pEvent->m_enDispId)
	{
		case DISPID_IRouterEventsOnClientConnected:
		{

			ATLASSERT(pEvent->m_bsParams);

			if(pEvent->m_bsParams)
				return m_pRouter->Fire_OnClientConnected(pEvent->m_bsParams);
	
		}
		break;
		case DISPID_IRouterEventsOnClientDisconnected:
		{
			ATLASSERT(pEvent->m_bsParams);

			if(pEvent->m_bsParams)
				return m_pRouter->Fire_OnClientDisconnected(pEvent->m_bsParams);
		}
		break;
		case DISPID_IRouterEventsOnTransportStopped:
		{
			ATLASSERT(!pEvent->m_bsParams);

			return m_pRouter->Fire_OnTransportStopped();
		}
		break;
		case DISPID_IRouterEventsOnRouterStarted:
		case DISPID_IRouterEventsOnRouterStopped:
			break;
		default:
			ATLASSERT(false);
		break;						
	}
	return S_OK;
}

CTpSessionEvent::~CTpSessionEvent()
{
	if(m_pSession)
		m_pSession->Release();
	m_pSession = NULL;
};

HRESULT CSessionEventThread::Fire_OnMessage(CTpEventPtr pEvent)
{
	CTpSessionEventPtr pSessionEvent = boost::dynamic_pointer_cast<CTpSessionEvent>(pEvent);

	switch(pSessionEvent->m_enDispId)
	{
		case DISPID_ISessionEventsOnLogon:
		{

			ATLASSERT(pSessionEvent->m_bsParams);

			if(pSessionEvent->m_bsParams)
				return pSessionEvent->m_pSession->Fire_OnLogon(pSessionEvent->m_bsParams);
	
		}
		break;
		case DISPID_ISessionEventsOnLogoff:
		{

			ATLASSERT(pSessionEvent->m_bsParams);

			if(pSessionEvent->m_bsParams)
				return pSessionEvent->m_pSession->Fire_OnLogoff(pSessionEvent->m_bsParams);
		}
		break;
		default:
			ATLASSERT(false);
		break;						
	}
	return S_OK;
}
