// PPBatchInfo.cpp : Implementation of CPPBatchInfo

#include "stdafx.h"
#include "PPBatchInfo.h"
#include ".\ppbatchinfo.h"

// CPPBatchInfo

STDMETHODIMP CPPBatchInfo::Initialize(BSTR UserName)
{
	HRESULT hr = S_OK;
	if(SUCCEEDED(m_Counter.Initialize()) && m_pObject==NULL)
	{
		hr = m_Counter.CreateInstance(0, UserName, &m_pObject);
		if(m_pObject && SUCCEEDED(hr))
            m_pObject->Clear();
	}
	return hr;
}

STDMETHODIMP CPPBatchInfo::AddClientRealTimeUpdate(void)
{
	if(m_pObject)
		m_pObject->m_lUpdatesWorkloadBase = ++m_pObject->m_IncomingCounter;
	return S_OK;
}
STDMETHODIMP CPPBatchInfo::AddRealTimeUpdate(void)
{
	if(m_pObject)
		++m_pObject->m_IncomingDataFlow;
		
	return S_OK;
}

STDMETHODIMP CPPBatchInfo::put_RTClients(ULONG newVal)
{
	if(m_pObject)
		m_pObject->m_RealtimeClients = newVal;
	return S_OK;
}

STDMETHODIMP CPPBatchInfo::put_Clients(ULONG newVal)
{
	if(m_pObject)
		m_pObject->m_Clients = newVal;
	return S_OK;
}


STDMETHODIMP CPPBatchInfo::AddProcessedUpdate(void)
{
	if(m_pObject)
	{
		m_pObject->m_lUpdatesWorkload = ++m_pObject->m_ProcessedCounter;
	}
	return S_OK;
}

STDMETHODIMP CPPBatchInfo::put_QuoteUpdatesQueueSnapshot(ULONG newVal)
{
	if(m_pObject)
		m_pObject->m_lQuoteUpdatesQueueLength = newVal;
	return S_OK;
}

STDMETHODIMP CPPBatchInfo::AddGroupRequestResponce(void)
{
	if(m_pObject)
		m_pObject->m_lGroupRequest++;

	return S_OK;
}

 
STDMETHODIMP CPPBatchInfo::AddMultipleRequestResponce(void)
{
	if(m_pObject)
		m_pObject->m_lMultipleRequest++;

	return S_OK;
}

STDMETHODIMP CPPBatchInfo::AddMultipleUnsubscriptionResponce(void)
{
	if(m_pObject)
		m_pObject->m_lMultipleUnsubscription++;

	return S_OK;
}

STDMETHODIMP CPPBatchInfo::AddClientUpdate(void)
{
	if(m_pObject)
		m_pObject->m_lClientUpdates++;
	return S_OK;
}

STDMETHODIMP CPPBatchInfo::AddSubscription(void)
{
	if(m_pObject)
		++m_pObject->Subscriptions;

	return S_OK;
}

STDMETHODIMP CPPBatchInfo::DeleteSubscription(int iNumber)
{
	if(m_pObject)
		m_pObject->Subscriptions-=iNumber;

	return S_OK;
}
