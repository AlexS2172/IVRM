// PriceInfo.cpp : Implementation of CPriceInfoWithNotify
#include "stdafx.h"
#include "PriceProviders.h"
#include "PriceInfoWithNotify.h"
CLock   CPriceInfoWithNotify::m_scReconnect;


//-------------------------------------------------------------------------------

_ATL_FUNC_INFO CPriceInfoWithNotifyEvent::m_ArrivedError = 
{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };

_ATL_FUNC_INFO CPriceInfoWithNotifyEvent::m_ArrivedLastQuote =  
{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

_ATL_FUNC_INFO CPriceInfoWithNotifyEvent::m_ArrivedQuoteUpdate =  
{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

_ATL_FUNC_INFO CPriceInfoWithNotifyEvent::m_ArrivedSubscribed =  
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };

_ATL_FUNC_INFO CPriceInfoWithNotifyEvent::m_ArrivedUnsubscribed =  
{CC_STDCALL, VT_EMPTY, 0 };

STDMETHODIMP CPriceInfoWithNotifyEvent::OnError(ErrorNumberEnum ErrorNumber, BSTR Description, RequestsTypeEnum ReqType, VARIANT Request)
{
	HRESULT hr = S_OK;
	if(m_pClient)
	{
		hr = m_pClient->OnError((ErrorNumberEnum)ErrorNumber, Description, (RequestsTypeEnum)ReqType, Request);
	}
	return hr;
}

STDMETHODIMP CPriceInfoWithNotifyEvent::OnLastQuote(QuoteUpdateParams *Params, QuoteUpdateInfo *Results)
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

		hr = m_pClient->OnLastQuote(vtPar, vtResult);
	}
	return hr;
}
STDMETHODIMP CPriceInfoWithNotifyEvent::OnQuoteUpdate(QuoteUpdateParams *Params, QuoteUpdateInfo *Results)
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


		hr = m_pClient->OnQuoteUpdate(vtPar, vtResult);
	}
	return hr;
}
STDMETHODIMP CPriceInfoWithNotifyEvent::OnUnsubscribed()
{	
	HRESULT hr = S_OK;
	if(m_pClient)
		hr = m_pClient->OnUnsubscribed();

	return hr;
}


STDMETHODIMP CPriceInfoWithNotifyEvent::OnSubscribed(QuoteUpdateParams *Params)
{	
	HRESULT hr = S_OK;
	if(m_pClient && Params)
	{
		CComVariant vtPar;
		_QuoteUpdateParams par;
		par.Init(Params);
		par.CopyTo(vtPar);

		hr = m_pClient->OnSubscribed(vtPar);
	}
	return hr;
}


/////////////////////////////////////////////////////////////////////////////
// CPriceInfoWithNotify
STDMETHODIMP CPriceInfoWithNotify::RequestLastQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->RequestLastQuote(Params));
			m_Requests.AddRequest(Params, false);
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
STDMETHODIMP CPriceInfoWithNotify::CancelLastQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{

			__CHECK_HRESULT3(m_spPriceProvider->CancelLastQuote(Params));
			m_Requests.Remove(Params, false);
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
STDMETHODIMP CPriceInfoWithNotify::SubscribeQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->SubscribeQuote(Params));
			m_Requests.AddRequest(Params, true);
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
STDMETHODIMP CPriceInfoWithNotify::UnSubscribeQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->UnSubscribeQuote(Params));
			m_Requests.Remove(Params, true);
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
STDMETHODIMP CPriceInfoWithNotify::Connect()
{
	HRESULT hr = S_OK;
	if(m_bConnected)
		return S_OK;

	try 
	{
		if((HANDLE)m_hConnected == NULL)
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
STDMETHODIMP CPriceInfoWithNotify::Disconnect()
{
	HRESULT hr = S_OK;
	if(!m_bConnected)
		return S_OK;
	try 
	{
		SetEvent(m_hStop);
		if(m_spPriceProvider!=NULL)		
		{
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

	if((HANDLE)m_hThread != NULL)
	{
		WaitWithEvents(m_hThread);
		m_hThread.Close();
	}

	return hr;
}

STDMETHODIMP CPriceInfoWithNotify::RequestLastGroupQuotes(QuoteUpdateParams *Params)
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
				m_Requests.AddRequest(Params, false);

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

STDMETHODIMP CPriceInfoWithNotify::CancelLastGroupQuotes(VARIANT Params)
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
				m_Requests.Remove(Params, false);
			}
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

STDMETHODIMP CPriceInfoWithNotify::SubscribeGroupQuotes(QuoteUpdateParams *Params)
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
				m_Requests.AddRequest(Params, true);

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

STDMETHODIMP CPriceInfoWithNotify::UnSubscribeGroupQuotes(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			CComQIPtr<IGroupPriceWithNotify> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
			{
				hr = spGroup->UnSubscribeGroupQuotes(Params);
				if(SUCCEEDED(hr))
					m_Requests.Remove(Params, true);
			}
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

HRESULT CPriceInfoWithNotify::OnLastQuote(const CComVariant &varParams,const CComVariant &varResults)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceInfoWithNotify::OnLastQuote\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	m_Requests.Remove(varParams, false);

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

HRESULT CPriceInfoWithNotify::OnQuoteUpdate(const CComVariant &varParams,const  CComVariant &varResults)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceInfoWithNotify::OnQuoteUpdate\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
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
			pvars[1] = varParams;
			pvars[0] = varResults;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CPriceInfoWithNotify::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
							RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceInfoWithNotify::OnError\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[4];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;


	switch(enumRequest)
	{
	case enRequestLastQuote:
		m_Requests.Remove(varRequest, false);
		break;
	case enSubscribeQuote:
		m_Requests.Remove(varRequest, true);
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

HRESULT CPriceInfoWithNotify::OnUnsubscribed()
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceInfoWithNotify::OnUnsubscribed\n"));

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
HRESULT CPriceInfoWithNotify::OnSubscribed(const CComVariant &varParams)
{
	//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceInfoWithNotify::OnSubscribed\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[1];
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
			pvars[0] = varParams;
			DISPPARAMS disp = { pvars, NULL, 1, 0 };
			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CPriceInfoWithNotify::Reconnect()
{
	HRESULT hr = S_OK;
	CAutoLock css(m_scReconnect);
	try 
	{
		m_bConnected = false;
		if(m_bRecursive)
		{
			long enTmp = m_ProviderType;			
			hr = put_Type(-1);
			if(SUCCEEDED(hr))
			{
				hr = put_Type(enTmp);
				if(SUCCEEDED(hr))
				{
					hr = m_spPriceProvider->Connect();
					if(SUCCEEDED(hr))
					{
						m_bConnected = true;

						{
							CAutoLock lock(m_Requests.m_csRequest);
							CPriceRequestsWithNotifyHolder::REQUEST::iterator iter;
							for(iter = m_Requests.m_Request.begin(); iter!=m_Requests.m_Request.end(); iter++)
							{
								if(WaitForSingleObject(m_hStop,0)!=WAIT_TIMEOUT)
									return S_OK;

								_QuoteUpdateParams param(iter->second.m_vtRequest);
								QuoteUpdateParams* pParam = reinterpret_cast<QuoteUpdateParams*>((QuoteUpdateParams*)param);
								
								if(pParam->Type == enGrSTK || pParam->Type == enGrIDX)
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
								else
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

UINT CPriceInfoWithNotify::_ThreadReconnection(LPVOID pData)
{
	CPriceInfoWithNotify* pThis = reinterpret_cast<CPriceInfoWithNotify*>(pData);
	if(!pThis)
		return 0;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	int iCount = 0;
	ATLTRACE(_T("Price Info: Reconnection thread started"));
	HANDLE hEvents[] =
	{
		pThis->m_hConnected,
			pThis->m_hStop
	};

	if(WAIT_OBJECT_0 == WaitWithEvents(2, hEvents))
	{
		while(1)
		{
			if(WaitWithEvents(pThis->m_hStop, 10000)!=WAIT_TIMEOUT)
				break;

			if(pThis->m_bConnected)
			{
				try
				{
					pThis->m_spPriceProvider->Connect();					
					iCount++;
					if(iCount>5)
					{
						iCount = 0;
						CAutoLock lock(pThis->m_Requests.m_csRequest);
						CPriceRequestsWithNotifyHolder::REQUEST::iterator iter;
						for(iter = pThis->m_Requests.m_Request.begin(); iter!=pThis->m_Requests.m_Request.end(); iter++)
						{
							if(WaitForSingleObject(pThis->m_hStop,0)!=WAIT_TIMEOUT)
								break;

							vt_date_span span = vt_date::GetCurrentDate() - iter->second.m_dtRequest;
							if((span.get_days() || span.get_hours() || span.get_minutes()))
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
											if(iter->second.m_bIsSubscribe)
											{
												spGroup->UnSubscribeGroupQuotes(iter->second.m_vtRequest);
												spGroup->SubscribeGroupQuotes(pParam);
												iter->second.m_dtRequest = vt_date::GetCurrentDate();
											}
											else
											{
												spGroup->CancelLastGroupQuotes(iter->second.m_vtRequest);
												spGroup->RequestLastGroupQuotes(pParam);
												iter->second.m_dtRequest = vt_date::GetCurrentDate();
											}
										}
									}
								}
								else if(span.get_minutes()>3)
								{
									if(iter->second.m_bIsSubscribe)
									{
										pThis->m_spPriceProvider->UnSubscribeQuote(iter->second.m_vtRequest);
										pThis->m_spPriceProvider->SubscribeQuote(pParam);
										iter->second.m_dtRequest = vt_date::GetCurrentDate();
									}
									else
									{
										pThis->m_spPriceProvider->CancelLastQuote(iter->second.m_vtRequest);
										pThis->m_spPriceProvider->RequestLastQuote(pParam);
										iter->second.m_dtRequest = vt_date::GetCurrentDate();
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
	ATLTRACE(_T("Price Info: Reconnection thread stopped"));
	return 0;
}
