// BatchPriceInfo.cpp : Implementation of CBatchPriceInfo
#include "stdafx.h"
#include "PriceProviders.h"
#include "BatchPriceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CBatchPriceInfo
CLock   CBatchPriceInfo::m_scReconnect;

//-------------------------------------------------------------------------------

_ATL_FUNC_INFO CBatchPriceInfoEvent::m_ArrivedError = 
{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };

_ATL_FUNC_INFO CBatchPriceInfoEvent::m_ArrivedLastQuote =  
{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

_ATL_FUNC_INFO CBatchPriceInfoEvent::m_ArrivedQuoteUpdate =  
{CC_STDCALL, VT_EMPTY, 0 };

_ATL_FUNC_INFO CBatchPriceInfoEvent::m_ArrivedSubscribed =  
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };

_ATL_FUNC_INFO CBatchPriceInfoEvent::m_ArrivedUnsubscribed =  
{CC_STDCALL, VT_EMPTY, 0 };


STDMETHODIMP CBatchPriceInfoEvent::OnError(ErrorNumberEnum ErrorNumber, BSTR Description, RequestsTypeEnum ReqType, VARIANT Request)
{
	HRESULT hr = S_OK;
	if(m_pClient)
	{
		m_pClient->SetActivity();
		hr = m_pClient->OnError((ErrorNumberEnum)ErrorNumber, Description, (RequestsTypeEnum)ReqType, Request);
	}
	return hr;
}

STDMETHODIMP CBatchPriceInfoEvent::OnLastQuote(QuoteUpdateParams *Params, QuoteUpdateInfo *Results)
{	
	HRESULT hr = S_OK;
	if(m_pClient)
	{
		CComVariant vtPar;
		if(Params)
		{
			_QuoteUpdateParams par;
			par.Init(Params);
			par.CopyTo(vtPar);
		}

		CComVariant vtResult;
		if(Results)
		{
			_QuoteUpdateInfo info;
			info.Init(Results);
			info.CopyTo(vtResult);
		}
		m_pClient->SetActivity();
		hr = m_pClient->OnLastQuote(vtPar, vtResult);
	}
	return hr;
}
STDMETHODIMP CBatchPriceInfoEvent::OnQuoteUpdate()
{	
	HRESULT hr = S_OK;
	if(m_pClient)
	{
		m_pClient->SetActivity();
		hr = m_pClient->OnQuoteUpdate();
	}
	return hr;
}
STDMETHODIMP CBatchPriceInfoEvent::OnUnsubscribed()
{	
	HRESULT hr = S_OK;
	if(m_pClient)
	{
		m_pClient->SetActivity();
		hr = m_pClient->OnUnsubscribed();
	}
	return hr;
}
STDMETHODIMP CBatchPriceInfoEvent::OnSubscribed(QuoteUpdateParams *Params)
{	
	HRESULT hr = S_OK;
	if(m_pClient && Params)
	{
		CComVariant vtPar;
		_QuoteUpdateParams par;
		par.Init(Params);
		par.CopyTo(vtPar);
		m_pClient->SetActivity();
		hr = m_pClient->OnSubscribed(vtPar);
	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::SetRegionRate(BSTR bstrRegion, DOUBLE dRate)
{
	
	HRESULT hr = S_OK;
	try {
		if(m_spPriceProvider!=NULL) {
			
			CComQIPtr<IBatchPriceProvider> spBatchPriceProvider(m_spPriceProvider);
			if(spBatchPriceProvider!=NULL){
				__CHECK_HRESULT3(spBatchPriceProvider->SetRegionRate(bstrRegion, dRate));
			}
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(...) {
		
	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::RequestMultipleQuotes  (SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams)
{
	HRESULT hr = S_OK;
	try {
		if(m_spPriceProvider!=NULL) {
			CComQIPtr<IBatchPriceProvider> spBatchPriceProvider(m_spPriceProvider);
			if(spBatchPriceProvider!=NULL){
				QuoteUpdateParams* pvData = NULL;
				if( SUCCEEDED( hr = ::SafeArrayAccessData( *pParams, (void**)&pvData) ) ) 	{
					long lLBound = 0, lUBound = 0;
					HRESULT hRes = SafeArrayGetLBound(*pParams, 1L, &lLBound);
					hRes = SafeArrayGetUBound(*pParams, 1L, &lUBound);
					long lArraySize = lUBound - lLBound +1 ;
					if(lArraySize) {	
						try	{
							for(int i = 0; i< lArraySize; ++i)	{	
								m_Requests.AddRequest(&pvData[i], false);
								SetActivity();
							}
						}
						catch( _com_error& e)	{ 	hr = e.Error();  	}
						catch(... )	 { ATLTRACE(_T("CBatchPriceInfo::RequestMultipleQuotes Unhandled exception\n"));	}
					}
					::SafeArrayUnaccessData(*pParams);
				}
				__CHECK_HRESULT3(spBatchPriceProvider->RequestMultipleQuotes(pParams));
			}
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err) {
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) {
			m_bRecursive = false;
			hr = RequestMultipleQuotes(pParams);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;

}
//***********************************************************************************************************
STDMETHODIMP CBatchPriceInfo::SubscribeMultipleQuotes(SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL) {
			CComQIPtr<IBatchPriceProvider> spBatchPriceProvider(m_spPriceProvider);
			if(spBatchPriceProvider!=NULL){
				CComQIPtr<IBatchPriceProvider> spBatchPriceProvider(m_spPriceProvider);
				if(spBatchPriceProvider!=NULL){
					QuoteUpdateParams* pvData = NULL;
					if( SUCCEEDED( hr = ::SafeArrayAccessData( *pParams, (void**)&pvData) ) ) 	{
						long lLBound = 0, lUBound = 0;
						HRESULT hRes = SafeArrayGetLBound(*pParams, 1L, &lLBound);
						hRes = SafeArrayGetUBound(*pParams, 1L, &lUBound);
						long lArraySize = lUBound - lLBound +1 ;
						if(lArraySize) {	
							try	{
								for(int i = 0; i< lArraySize; ++i)	{	
									m_Requests.AddRequest(&pvData[i], true);
									SetActivity();
								}
							}
							catch( _com_error& e)	{ 	hr = e.Error();  	}
							catch(... )	 { ATLTRACE(_T("CBatchPriceInfo::RequestMultipleQuotes Unhandled exception\n"));	}
						}
						::SafeArrayUnaccessData(*pParams);
					}
					__CHECK_HRESULT3(spBatchPriceProvider->SubscribeMultipleQuotes(pParams));
				}
			}
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = SubscribeMultipleQuotes(pParams);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CBatchPriceInfo
STDMETHODIMP CBatchPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->RequestLastQuote(Params));
			//if(!m_bInProcClient)
			{
				m_Requests.AddRequest(Params, false);
				SetActivity();
			}
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = RequestLastQuote(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CBatchPriceInfo::CancelLastQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->CancelLastQuote(Params));
			//if(!m_bInProcClient)
			{
				m_Requests.Remove(Params, false);
				SetActivity();
			}
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = CancelLastQuote(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CBatchPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->SubscribeQuote(Params));
			//if(!m_bInProcClient)
			{
				m_Requests.AddRequest(Params, true);
				SetActivity();
			}
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = m_spPriceProvider->SubscribeQuote(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CBatchPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->UnSubscribeQuote(Params));

			//if(!m_bInProcClient)
			{
				if(SUCCEEDED(hr))
					m_Requests.Remove(Params, true);
				SetActivity();
			}
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = UnSubscribeQuote(Params);;
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		

	}
	return hr;

}
STDMETHODIMP CBatchPriceInfo::Connect()
{
	HRESULT hr = S_OK;
	if(m_bConnected)
		return S_OK;

	try 
	{
		if(!m_hConnected)
		{
			m_hConnected.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));

			unsigned  id = 0;
			m_hThread.Attach((HANDLE) _beginthreadex(NULL, 0, _ThreadReconnection, this, 0, &id));
		}
		else
			ResetEvent(m_hStop);


		if(m_spPriceProvider!=NULL)		
		{
			__CHECK_HRESULT3(m_spPriceProvider->Connect());
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = m_spPriceProvider->Connect();
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	if(SUCCEEDED(hr))
	{
		SetEvent(m_hConnected);
		m_bConnected = true;
	}

	return hr;
}
STDMETHODIMP CBatchPriceInfo::Disconnect()
{
	HRESULT hr = S_OK;
	if(!m_bConnected)
		return S_OK;
	try 
	{
		SetEvent(m_hStop);
		if(m_spPriceProvider!=NULL)		
		{
			UnSubscribeQuote(vtMissing);
			__CHECK_HRESULT3(m_spPriceProvider->Disconnect());
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) ) 
			hr = utils::ComError2ErrInfo (err,this);		
	}
	if(SUCCEEDED(hr))
		m_bConnected = false;

	if(NULL != (HANDLE)m_hThread)
	{
		WaitWithEvents(m_hThread);
		m_hThread.Close();
	}

	return hr;
}

STDMETHODIMP CBatchPriceInfo::UnNotifiedSubscribtionQuantity(VARIANT_BOOL bSkipNotification,  LONG* pVal)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider != NULL)
		{
			CComQIPtr<IBatchPriceProvider> spBatch = m_spPriceProvider;
			if(spBatch != NULL)
			{
				__CHECK_HRESULT3(spBatch->UnNotifiedSubscribtionQuantity(bSkipNotification, pVal));
				SetActivity();
			}
			else
				hr = Error(_T("Batch price interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = UnNotifiedSubscribtionQuantity(bSkipNotification, pVal);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::get_IsQuotesUpdated(VARIANT_BOOL *pVal)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider != NULL)
		{
			CComQIPtr<IBatchPriceProvider> spBatch = m_spPriceProvider;
			if(spBatch != NULL)
			{
				__CHECK_HRESULT3(spBatch->get_IsQuotesUpdated(pVal));
				SetActivity();
			}
			else
				hr = Error(_T("Batch price interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = get_IsQuotesUpdated(pVal);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::GetQuotesUpdates(SAFEARRAY** pRetVal)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider != NULL)
		{
			CComQIPtr<IBatchPriceProvider> spBatch = m_spPriceProvider;
			if(spBatch != NULL)
			{
				__CHECK_HRESULT3(spBatch->GetQuotesUpdates(pRetVal));
				SetActivity();
			}
			else
				hr = Error(_T("Batch price interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = GetQuotesUpdates(pRetVal);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::RequestLastGroupQuotes(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
			{
				__CHECK_HRESULT3(spGroup->RequestLastGroupQuotes(Params));
				if(!m_bInProcClient)
				{
					m_Requests.AddRequest(Params, false);
				}
			}
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
				hr = spGroup->RequestLastGroupQuotes(Params);
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);

			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		

	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::CancelLastGroupQuotes(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
			{
				__CHECK_HRESULT3(spGroup->CancelLastGroupQuotes(Params));
				if(!m_bInProcClient)
				{
					m_Requests.Remove(Params, false);
				}
				SetActivity();
			}
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
				hr = spGroup->CancelLastGroupQuotes(Params);
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);

			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::SubscribeGroupQuotes(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
			{
				__CHECK_HRESULT3(spGroup->SubscribeGroupQuotes(Params));
				if(!m_bInProcClient)
				{
					m_Requests.AddRequest(Params, true);
					SetActivity();
				}										  
				
			}
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
				hr = spGroup->SubscribeGroupQuotes(Params);
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);

			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		

	}
	return hr;
}

STDMETHODIMP CBatchPriceInfo::UnSubscribeGroupQuotes(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
			{
				__CHECK_HRESULT3( spGroup->UnSubscribeGroupQuotes(Params));
				if(!m_bInProcClient)
				{
					m_Requests.Remove(Params, true);
					SetActivity();
				}
				
			}
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
				hr = spGroup->UnSubscribeGroupQuotes(Params);
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);

			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

HRESULT CBatchPriceInfo::OnLastQuote(const CComVariant &varParams,const CComVariant &varResults)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CBatchPriceInfo::OnLastQuote\n"));
	SetActivity();
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

	//if(!m_bInProcClient)
	{
		m_Requests.Remove(varParams, false);
	}

	_QuoteUpdateInfo res;
	_QuoteUpdateParams par;

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[1] = varParams;
			pvars[0] = varResults;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CBatchPriceInfo::OnQuoteUpdate()
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CBatchPriceInfo::OnQuoteUpdate\n"));
	SetActivity();
	CComVariant varResult;
	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			DISPPARAMS disp = { NULL, NULL, 0, 0 };
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CBatchPriceInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
							RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CBatchPriceInfo::OnError\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[4];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	SetActivity();


	switch(enumRequest)	{
		case enRequestLastQuote:
			//if(!m_bInProcClient)
				m_Requests.Remove(varRequest, false);
			break;
		case enSubscribeQuote:
			//if(!m_bInProcClient)
				m_Requests.Remove(varRequest, true);
			break;
		case enNoRequest:
			/*if (!m_bInProcClient )*/ {
				if(enumError == enProviderConnected )
					Reconnect();
			}
			break;
	}

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[3] = enumError;
			pvars[2] = (BSTR)bstrDescription;
			pvars[1] = enumRequest;
			pvars[0] = varRequest;
			DISPPARAMS disp = { pvars, NULL, 4, 0 };
			hr = pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CBatchPriceInfo::OnSubscribed(const CComVariant &varParams)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CBatchPriceInfo::OnSubscribed\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[1];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	SetActivity();

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[0] = varParams;
			DISPPARAMS disp = { pvars, NULL, 1, 0 };
			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
HRESULT CBatchPriceInfo::OnUnsubscribed()
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CBatchPriceInfo::OnUnsubscribed\n"));
	SetActivity();
	CComVariant varResult;
	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			DISPPARAMS disp = { NULL, NULL, 0, 0 };
			hr = pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CBatchPriceInfo::Reconnect()
{
	HRESULT hr = S_OK;
	CAutoLock css(m_scReconnect);
	try 
	{
		m_bConnected = false;
		if(m_bRecursive)
		{
			long enTmp = m_ProviderType;			
			//hr = put_Type(-1);
			if(SUCCEEDED(hr))
			{
				//hr = put_Type(enTmp);
				if(SUCCEEDED(hr))
				{
					__CHECK_HRESULT3(m_spPriceProvider->Connect());
					if(SUCCEEDED(hr))
					{
						m_bConnected = true;
						//if(!m_bInProcClient)
						{ 
							Sleep(500);
							CAutoLock lock(m_Requests.m_csRequest);
							CBatchPriceInfoRequestsHolder::REQUEST::iterator iter;
							for(iter = m_Requests.m_Request.begin(); iter!=m_Requests.m_Request.end(); iter++)
							{
								if(WaitForSingleObject(m_hStop,0)!=WAIT_TIMEOUT)
									return S_OK;

								_QuoteUpdateParams param(iter->second.m_vtRequest);
								QuoteUpdateParams* pParam = reinterpret_cast<QuoteUpdateParams*>((QuoteUpdateParams*)param);
								
								/*if(pParam->Type == enGrSTK || pParam->Type == enGrIDX)
								{
									CComPtr<IGroupPriceWithNotify> spGroup;
									if(SUCCEEDED(m_spPriceProvider->QueryInterface(IID_IGroupPriceWithNotify,(void**) &spGroup)))
									{
										if(iter->second.m_bIsSubscribe)
											spGroup->SubscribeGroupQuotes(pParam);
										else
											spGroup->RequestLastGroupQuotes(pParam);
									}
								}
								else*/
								{
									if(iter->second.m_bIsSubscribe)
										m_spPriceProvider->SubscribeQuote(pParam);
									else
										m_spPriceProvider->RequestLastQuote(pParam);

								}
							}
						}
					}
				}
			}
		}
	}
	catch(_com_error& err)
	{
		return utils::ComError2ErrInfo (err,this);		
	}
	return hr;

}

UINT CBatchPriceInfo::_ThreadReconnection(LPVOID pData)
{
	HRESULT hr = ERROR_SUCCESS;
	CBatchPriceInfo* pThis = reinterpret_cast<CBatchPriceInfo*>(pData);
	if(!pThis)
		return 0;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	int iCount = 0;
	ATLTRACE(_T("CBatchPriceInfo: Reconnection thread started\n"));
	HANDLE hEvents[] =
	{
		pThis->m_hConnected,
			pThis->m_hStop
	};

	if(WAIT_OBJECT_0==pThis->WaitWithEvents(2, hEvents))
	{
		DWORD dwTimeout = 15000;
		if(!pThis->m_bInProcClient)
			dwTimeout = INFINITE;


		while(1)
		{
			if(pThis->WaitWithEvents(pThis->m_hStop, dwTimeout)!=WAIT_TIMEOUT)
				break;

			if(pThis->m_bConnected && !pThis->m_bInProcClient)
			{
				try
				{
					if(pThis->GetActivity().get_seconds() >= 10)
					{
						CAutoLock css(m_scReconnect);
						__CHECK_HRESULT3(pThis->m_spPriceProvider->Connect());					
						pThis->SetActivity();
					}
					
					if(++iCount > 10)
					{
						iCount = 0;
						CAutoLock lock(pThis->m_Requests.m_csRequest);
						CBatchPriceInfoRequestsHolder::REQUEST::iterator iter;
						for(iter = pThis->m_Requests.m_Request.begin(); iter!=pThis->m_Requests.m_Request.end(); iter++)
						{
							if(WaitForSingleObject(pThis->m_hStop,0)!=WAIT_TIMEOUT)
								break;
							vt_date_span span = vt_date::GetCurrentDate() - iter->second.GetDtRequest();
							if(span.get_total_minutes())
							{
								_QuoteUpdateParams param(iter->second.m_vtRequest);
								QuoteUpdateParams* pParam = reinterpret_cast<QuoteUpdateParams*>((QuoteUpdateParams*)param);

								if(pParam->Type == enGrSTK || pParam->Type == enGrIDX)
								{
									CComPtr<IGroupPriceWithNotify> spGroup;
									if(SUCCEEDED(pThis->m_spPriceProvider->QueryInterface(IID_IGroupPriceWithNotify,(void**) &spGroup)))
									{
										if(span.get_minutes()>9)
										{
											if(!iter->second.m_bIsSubscribe)
											{
												spGroup->CancelLastGroupQuotes(iter->second.m_vtRequest);
												spGroup->RequestLastGroupQuotes(pParam);
												iter->second.SetDtRequest( vt_date::GetCurrentDate());
											}
										}
									}
								}
								else if(span.get_minutes()>3)
								{
									if(!iter->second.m_bIsSubscribe)
									{
										pThis->m_spPriceProvider->CancelLastQuote(iter->second.m_vtRequest);
										pThis->m_spPriceProvider->RequestLastQuote(pParam);
										iter->second.SetDtRequest( vt_date::GetCurrentDate());
									}
								}
							}
						}
					}
				}
				catch(...)
				{
					pThis->Reconnect();
					iCount = 0;
				}
			}
			else
				iCount = 0;
		}
	}

	CoUninitialize();
	ATLTRACE(_T("CBatchPriceInfo: Reconnection thread stopped\n"));
	return 0;
}

