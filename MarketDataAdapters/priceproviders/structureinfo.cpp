// StructureInfo.cpp : Implementation of CStructureInfo
#include "stdafx.h"
#include "PriceProviders.h"
#include "StructureInfo.h"

CLock g_csPriceProvider;
CLock CStructureInfo::m_csReconnect;
//-------------------------------------------------------------------------------

_ATL_FUNC_INFO CStructureInfoNotify::m_ArrivedError = 
	{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };
 
_ATL_FUNC_INFO CStructureInfoNotify::m_ArrivedStock =  
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

_ATL_FUNC_INFO CStructureInfoNotify::m_ArrivedOption =  
	{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD,VT_RECORD,VT_I2} };

STDMETHODIMP CStructureInfoNotify::OnError(ErrorNumberEnum ErrorNumber, BSTR Description, RequestsTypeEnum ReqType, VARIANT Request)
{
HRESULT hr = S_OK;
if(m_pClient)
{
	hr = m_pClient->OnError((ErrorNumberEnum)ErrorNumber, Description, (RequestsTypeEnum)ReqType, Request);
}
return hr;
}

STDMETHODIMP CStructureInfoNotify::OnStock(StockParams *Params, StockInfo *Results)
{	
HRESULT hr = S_OK;
if(m_pClient && Params && Results)
{
	CComVariant vtPar;
	if(Params)
	{
		_StockParams par;
		par.Init(Params);
		par.CopyTo(vtPar);
	}

	CComVariant vtResult;
	if(Results)
	{
		_StockInfo info;
		info.Init(Results);
		info.CopyTo(vtResult);
	}
	hr = m_pClient->OnStock(vtPar, vtResult);
}
return hr;
}
STDMETHODIMP CStructureInfoNotify::OnOption(OptionParams *Params, OptionInfo *Results, VARIANT_BOOL bLast)
{	
HRESULT hr = S_OK;
if(m_pClient)
{
	CComVariant vtVarIsLastOption;
	vtVarIsLastOption = bLast;

	CComVariant vtPar;
	if(Params)
	{
		_OptionParams par;
		par.Init(Params);
		par.CopyTo(vtPar);
	}

	CComVariant vtResult;
	if(Results)
	{
		_OptionInfo info;
		info.Init(Results);
		info.CopyTo(vtResult);   
	}
	hr = m_pClient->OnOption(vtPar, vtResult, vtVarIsLastOption);
}
return hr;
}


/////////////////////////////////////////////////////////////////////////////
// CStructureInfo
STDMETHODIMP CStructureInfo::RequestStock(StockParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spStructureProvider->RequestStock(reinterpret_cast<StockParams*>(Params)));
			m_Requests.Add(Params);
		}
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = RequestStock(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureInfo::CancelStock(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spStructureProvider->CancelStock(Params));
			m_Requests.Remove(Params);
		}
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = CancelStock(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureInfo::RequestOptions(OptionParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
		{
			__CHECK_HRESULT3( m_spStructureProvider->RequestOptions(reinterpret_cast<OptionParams*>(Params)));
			m_Requests.Add(Params);
		}
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = RequestOptions(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureInfo::CancelOptions(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spStructureProvider->CancelOptions(Params));
			m_Requests.Remove(Params);
		}
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = CancelOptions(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;

}
STDMETHODIMP CStructureInfo::Connect()
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_hConnected == INVALID_HANDLE_VALUE)
		{
			m_hConnected = CreateEvent(NULL, FALSE, FALSE,NULL);

			unsigned  id = 0;
			_beginthreadex(NULL, 0, _ThreadReconnection, this, 0, &id);
		}
		else
			ResetEvent(m_hStop);


		if(m_spStructureProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spStructureProvider->Connect());
			m_Requests.RemoveAll();
		}
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = m_spStructureProvider->Connect();
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	if(SUCCEEDED(hr))
		m_bConnected = true;
	return hr;
}
STDMETHODIMP CStructureInfo::Disconnect()
{
	HRESULT hr = S_OK;
	try 
	{
		SetEvent(m_hStop);
		if(m_spStructureProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spStructureProvider->Disconnect());
			m_Requests.RemoveAll();

		}
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error())) 
			return utils::ComError2ErrInfo (err,this);		
	}
	if(SUCCEEDED(hr))
		m_bConnected = false;
	return hr;
}


//-----------------------------------------------------------------------------------------------//

HRESULT CStructureInfo::OnStock(const CComVariant &varParams,const  CComVariant &varResults)
{
	ATLTRACE(_T("CStructureInfo::OnStock\n"));
	
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	m_Requests.Remove(varParams);	

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
			pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CStructureInfo::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CStructureInfo::OnOption\n"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
	int nConnections = m_vec.GetSize();
	
	VARIANT_BOOL bLast = V_BOOL(&varIsLastOption);

	if(bLast==VARIANT_TRUE)
		m_Requests.Remove(varParams);	
	
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[2] = varParams;
			pvars[1] = varResults;
			pvars[0] = varIsLastOption;
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CStructureInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest,const CComVariant &varRequest)
{
	ATLTRACE(_T("CStructureInfo::OnError\n"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[4];
	int nConnections = m_vec.GetSize();
	m_Requests.Remove(varRequest);	
	
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
			pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
	return varResult.scode;
}

UINT CStructureInfo::_ThreadReconnection(LPVOID pData)
{
	CStructureInfo* pThis = reinterpret_cast<CStructureInfo*>(pData);
	if(!pThis)
		return 0;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	int iCount = 0;
	HANDLE hEvents[] =
	{
		pThis->m_hConnected,
		pThis->m_hStop
	};
	
	if(WAIT_OBJECT_0 == WaitWithEvents(2, hEvents))
	{
		while(1)
		{
			if(WaitWithEvents(pThis->m_hStop, 30000)!=WAIT_TIMEOUT)
				break;

			if(pThis->m_bConnected)
			{
				try
				{
					pThis->m_spStructureProvider->Connect();
					iCount++;
					if(iCount>5)
					{
						iCount = 0;
						CAutoLock lock(pThis->m_Requests.m_csRequest);
						CStructureRequestsHolder::REQUEST::iterator iter;
						for(iter = pThis->m_Requests.m_Request.begin(); iter!=pThis->m_Requests.m_Request.end(); iter++)
						{
							vt_date_span span = vt_date::GetCurrentDate() - iter->second.m_dtRequest;
							if(span.get_minutes() || span.get_hours() || span.get_days() )
							{
								switch(iter->second.m_enType)
								{
								case SStructureRequestData::enStock:
									{
										if(span.get_minutes()>3)
										{
											_StockParams param(iter->second.m_vtRequest);
											StockParams* pParam = reinterpret_cast<StockParams*>((StockParams*)param);

											pThis->m_spStructureProvider->RequestStock(pParam);									
											iter->second.m_dtRequest = vt_date::GetCurrentDate();
										}
									}break;
								case SStructureRequestData::enOption:
									{
										if(span.get_minutes()>9)
										{
											_OptionParams param(iter->second.m_vtRequest);
											OptionParams* pParam = reinterpret_cast<OptionParams*>((OptionParams*)param);

											pThis->m_spStructureProvider->RequestOptions(pParam);									
											iter->second.m_dtRequest = vt_date::GetCurrentDate();
										}

									}break;
								}

							}
						}
					}
				}
				catch(...)
				{
					iCount = 0;
					pThis->Reconnect();
				}
			}
			else
				iCount = 0;
		}
	}
	
	CoUninitialize();

	CloseHandle(pThis->m_hStop);
	pThis->m_Requests.RemoveAll();
	pThis->m_hStop = INVALID_HANDLE_VALUE;

	return 0;
}
