// HostStatisticData.cpp : Implementation of CHostStatisticData
#include "stdafx.h"
#include "Transport.h"
#include "HostStatisticData.h"

/////////////////////////////////////////////////////////////////////////////
// CHostStatisticData

STDMETHODIMP CHostStatisticData::get_ReceivedMessageNum(long *pVal)
{
	if (!pVal)
	{
		CEgLibTraceManager::TraceText(LogError,_T("HostStatisticData"),_T("get_ReceivedMessageNum failed. Bad Pointer"));
		return E_POINTER;
	}						
	ObjectLock lock(this);

	*pVal = m_ulReceivedMessage;

	return S_OK;
}

STDMETHODIMP CHostStatisticData::put_ReceivedMessageNum(long newVal)
{
	ObjectLock lock(this);
						
	m_ulReceivedMessage = newVal;

	return S_OK;
}

STDMETHODIMP CHostStatisticData::get_SentMessageNum(long *pVal)
{
	if (!pVal)
	{
		CEgLibTraceManager::TraceText(LogError, _T("HostStatisticData"),_T("get_SentMessageNum failed. Bad Pointer"));
		return E_POINTER;
	}						
	ObjectLock lock(this);

	*pVal = m_ulSentMessage;

	return S_OK;
}

STDMETHODIMP CHostStatisticData::put_SentMessageNum(long newVal)
{
	ObjectLock lock(this);
						
	m_ulSentMessage = newVal;

	return S_OK;
}

STDMETHODIMP CHostStatisticData::get_UnsentMessageNum(long *pVal)
{
	if (!pVal)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("HostStatisticData"),_T("get_UnsentMessageNum failed. Bad Pointer"));
		return E_POINTER;
	}						
	ObjectLock lock(this);

	*pVal = m_ulUnsentMessage;
	return S_OK;
}

STDMETHODIMP CHostStatisticData::put_UnsentMessageNum(long newVal)
{
	ObjectLock lock(this);
						
	m_ulUnsentMessage = newVal;

	return S_OK;
}

STDMETHODIMP CHostStatisticData::get_ConfirmedMessageNum(long *pVal)
{
	if (!pVal)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("HostStatisticData"),_T("get_ConfirmedMessageNum failed. Bad Pointer"));
		return E_POINTER;
	}					
	ObjectLock lock(this);
	*pVal = m_ulConfirmedMessage;

	return S_OK;
}

STDMETHODIMP CHostStatisticData::put_ConfirmedMessageNum(long newVal)
{
	ObjectLock lock(this);
						
	m_ulConfirmedMessage = newVal;
	return S_OK;
}

STDMETHODIMP CHostStatisticData::get_LogonTime(DATE *pVal)
{
	if (!pVal)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("HostStatisticData"),_T("get_LogonTime failed. Bad Pointer"));
		return E_POINTER;
	}						
	ObjectLock lock(this);
	*pVal = m_dtLogonTime;
	return S_OK;
}

STDMETHODIMP CHostStatisticData::put_LogonTime(DATE newVal)
{
	ObjectLock lock(this);
	m_dtLogonTime = newVal;
	return S_OK;
}

STDMETHODIMP CHostStatisticData::get_HostName( BSTR *pVal)
{
	if (!pVal)
	{
		CEgLibTraceManager::TraceText(LogInfo,_T("HostStatisticData"),_T("get_HostName failed. Bad Pointer"));
		return E_POINTER;
	}						
	ObjectLock lock(this);
	*pVal =m_bsHostData.copy();
	return S_OK;

}
STDMETHODIMP CHostStatisticData::put_HostName( BSTR newVal)
{
	ObjectLock lock(this);
	m_bsHostData = newVal;
	return S_OK;
}
