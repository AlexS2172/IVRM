// PriceInfo.cpp : Implementation of CPriceInfo
#include "stdafx.h"
#include "PriceProviders.h"
#include "PriceInfo.h"

//-------------------------------------------------------------------------------

_ATL_FUNC_INFO CPriceInfoNotify::m_ArrivedError = 
	{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };
 
_ATL_FUNC_INFO CPriceInfoNotify::m_ArrivedLastQuote =  
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };
 
_ATL_FUNC_INFO CPriceInfoNotify::m_ArrivedQuoteUpdate =  
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

STDMETHODIMP CPriceInfoNotify::OnError(ErrorNumberEnum ErrorNumber, BSTR Description, RequestsTypeEnum ReqType, VARIANT Request)
{
HRESULT hr = S_OK;
if(m_pClient)
{
	hr = m_pClient->OnError((ErrorNumberEnum)ErrorNumber, Description, (RequestsTypeEnum)ReqType, Request);
}
return hr;
}

STDMETHODIMP CPriceInfoNotify::OnLastQuote(QuoteUpdateParams *Params, QuoteUpdateInfo *Results)
{	
HRESULT hr = S_OK;
	if(m_pClient && Params && Results)
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
STDMETHODIMP CPriceInfoNotify::OnQuoteUpdate(QuoteUpdateParams *Params, QuoteUpdateInfo *Results)
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
	hr = m_pClient->OnQuoteUpdate(vtPar, vtResult);
}
return hr;
}


/////////////////////////////////////////////////////////////////////////////
// CPriceInfo
STDMETHODIMP CPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->RequestLastQuote(reinterpret_cast<QuoteUpdateParams*>(Params)));
			m_Requests.AddRequest(SPriceRequestData::enLast, Params);
			SetActivity();
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
STDMETHODIMP CPriceInfo::CancelLastQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
		
			__CHECK_HRESULT3(m_spPriceProvider->CancelLastQuote(Params));
			m_Requests.Remove(SPriceRequestData::enLast, Params);
			SetActivity();
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
STDMETHODIMP CPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->SubscribeQuote(reinterpret_cast<QuoteUpdateParams*>(Params)));
			m_Requests.AddRequest(SPriceRequestData::enSubscribe, Params);
			SetActivity();
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = m_spPriceProvider->SubscribeQuote(reinterpret_cast<QuoteUpdateParams*>(Params));
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->UnSubscribeQuote(Params));
			SetActivity();
			m_Requests.Remove(SPriceRequestData::enSubscribe, Params);
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
STDMETHODIMP CPriceInfo::Connect()
{
	HRESULT hr = S_OK;
	if(m_bConnected)
		return S_OK;

	try 
	{
		if( NULL == ( HANDLE )m_hConnected )
		{
			m_hConnected.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));

			unsigned  id = 0;
			EgLib::CThread::Stop(false);
			EgLib::CThread::Start();
		}

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
STDMETHODIMP CPriceInfo::Disconnect()
{
	HRESULT hr = S_OK;
	if(!m_bConnected)
		return S_OK;
	try 
	{
		EgLib::CThread::Stop(true, 1000);
		if(m_spPriceProvider!=NULL)		
		{
			__CHECK_HRESULT3(m_spPriceProvider->Disconnect());
			m_bConnected = false;
			m_spPriceProvider = NULL;
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) ) 
			hr = utils::ComError2ErrInfo (err,this);		
		m_spPriceProvider = NULL;
	}
	return hr;
}

STDMETHODIMP CPriceInfo::RequestGroup(QuoteUpdateParams *Params, GroupRequestType enTypeGroupRequest)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			CComQIPtr<IGroupPrice> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
			{
					__CHECK_HRESULT3(spGroup->RequestGroup(Params, enTypeGroupRequest));
					m_Requests.AddRequest(SPriceRequestData::enGroup, Params, enTypeGroupRequest);
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
			CComQIPtr<IGroupPrice> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
				hr = spGroup->RequestGroup(Params, enTypeGroupRequest);
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);

			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		

	}
	return hr;
}

STDMETHODIMP CPriceInfo::CancelGroup(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			CComQIPtr<IGroupPrice> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
			{
				__CHECK_HRESULT3(spGroup->CancelGroup(Params));
				m_Requests.Remove(SPriceRequestData::enGroup, Params);
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
			CComQIPtr<IGroupPrice> spGroup = m_spPriceProvider;
			if(spGroup!=NULL)
				hr = spGroup->CancelGroup(Params);
			else
				hr = Error(_T("Group interface is not supported"), IID_IPriceProvider, E_NOINTERFACE);

			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

HRESULT CPriceInfo::OnQuoteUpdate(const CComVariant &varParams,const  CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	SetActivity();
	ATLTRACE(_T("CPriceInfo::OnQuoteUpdate\n"));
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
HRESULT CPriceInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceInfo::OnError\n"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[4];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	SetActivity();


	switch(enumRequest)
	{
	case enRequestLastQuote:
		m_Requests.Remove(SPriceRequestData::enLast, varRequest);
		m_Requests.Remove(SPriceRequestData::enGroup, varRequest);
		break;
	case enSubscribeQuote:
		m_Requests.Remove(SPriceRequestData::enSubscribe, varRequest);
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

HRESULT CPriceInfo::OnLastQuote(const CComVariant &varParams,const CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceInfo::OnLastQuote\n"));
		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();
		HRESULT hr = S_OK;
		m_Requests.Remove(SPriceRequestData::enLast, varParams);
		SetActivity();

		_QuoteUpdateInfo res;
		_QuoteUpdateParams par;

		if( SUCCEEDED(par.Init(varParams)) )
		{
			if(par->Type == enSTK)
			{
				if(SUCCEEDED(res.Init(varResults)))  
				{
					_bstr_t bsCur = res->Currency;
					if(!bsCur.length())
						m_Requests.Remove(SPriceRequestData::enGroup, varParams);
				}
			}
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


HRESULT CPriceInfo::Reconnect()
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
					__CHECK_HRESULT3(m_spPriceProvider->Connect());
					m_bConnected = true;
					{
						CAutoLock lock(m_Requests.m_csRequest);
						CPriceRequestsHolder::REQUEST::iterator iter;
						for(iter = m_Requests.m_Request.begin(); iter!=m_Requests.m_Request.end(); iter++)
						{
							if(WaitForSingleObject(EgLib::CThread::GetStopEventHandle(),0)!=WAIT_TIMEOUT)
								return S_OK;

							_QuoteUpdateParams param(iter->second.m_vtRequest);
							QuoteUpdateParams* pParam = reinterpret_cast<QuoteUpdateParams*>((QuoteUpdateParams*)param);
							switch(iter->second.m_enType)
							{
							case SPriceRequestData::enLast:
								{
									m_spPriceProvider->RequestLastQuote(pParam);									
								}break;
							case SPriceRequestData::enSubscribe:
								{
									m_spPriceProvider->SubscribeQuote(pParam);									
								}break;
							case SPriceRequestData::enGroup:
								{
									CComPtr<IGroupPrice> spGroup;
									if(SUCCEEDED(m_spPriceProvider->QueryInterface(IID_IGroupPrice,(void**) &spGroup)))
										spGroup->RequestGroup(param, iter->second.m_enTypeGroupRequest);
								}break;
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

UINT CPriceInfo::Run()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	int iCount = 0;
	ATLTRACE(_T("Price Info: Reconnection thread started"));
	HANDLE hEvents[] =
	{
		m_hConnected,
		EgLib::CThread::GetStopEventHandle()
	};

	DWORD dwWait = WaitWithEvents(2, hEvents);
	if(WAIT_OBJECT_0 == dwWait )
	{
		while(1)
		{
			if(WaitWithEvents(EgLib::CThread::GetStopEventHandle(), 15000) != WAIT_TIMEOUT)
				break;

			if(m_bConnected)
			{
				try
				{
					HRESULT hr = S_OK;
					if(GetActivity().get_seconds() >= 10)
					{
						CAutoLock css(m_scReconnect);
						__CHECK_HRESULT3(m_spPriceProvider->Connect());					
						SetActivity();
					}
					iCount++;
					if(iCount > 10)
					{
						iCount = 0;
						CAutoLock lock(m_Requests.m_csRequest);
						CPriceRequestsHolder::REQUEST::iterator iter;
						for(iter = m_Requests.m_Request.begin(); iter!=m_Requests.m_Request.end(); iter++)
						{
							if(WaitForSingleObject(EgLib::CThread::GetStopEventHandle(), 0) != WAIT_TIMEOUT)
								break;

							vt_date_span span = vt_date::GetCurrentDate() - iter->second.m_dtRequest;
							if((span.get_days() || span.get_hours() || span.get_minutes()))
							{
								_QuoteUpdateParams param(iter->second.m_vtRequest);
								QuoteUpdateParams* pParam = reinterpret_cast<QuoteUpdateParams*>((QuoteUpdateParams*)param);
								switch(iter->second.m_enType)
								{
								case SPriceRequestData::enLast:
									{
										if(span.get_minutes()>3)
										{
											m_spPriceProvider->CancelLastQuote(iter->second.m_vtRequest);
											m_spPriceProvider->RequestLastQuote(pParam);									
											iter->second.m_dtRequest = vt_date::GetCurrentDate();
										}
									}break;
								case SPriceRequestData::enGroup:
									{
										if(span.get_minutes()>9)
										{
											CComPtr<IGroupPrice> spGroup;
											if(SUCCEEDED(m_spPriceProvider->QueryInterface(IID_IGroupPrice,(void**) &spGroup)))
											{
												spGroup->CancelGroup(iter->second.m_vtRequest);
												spGroup->RequestGroup(param, iter->second.m_enTypeGroupRequest);
												iter->second.m_dtRequest = vt_date::GetCurrentDate();
											}
										}
									}break;
								}
							}
						}
					}
				}
				catch(...)
				{
					Reconnect();
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
