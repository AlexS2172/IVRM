// StructureProviderEx.cpp : Implementation of CStructureProviderEx

#include "stdafx.h"
#include "StructureProviderEx.h"

//-------------------------------------------------------------------------------

_ATL_FUNC_INFO CStructureProviderExNotify::m_ArrivedError = 
	{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };
 
_ATL_FUNC_INFO CStructureProviderExNotify::m_ArrivedStock =  
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

_ATL_FUNC_INFO CStructureProviderExNotify::m_ArrivedOption =  
	{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD,VT_RECORD,VT_BOOL} };

_ATL_FUNC_INFO CStructureProviderExNotify::m_ArrivedFuture =  
{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

_ATL_FUNC_INFO CStructureProviderExNotify::m_ArrivedFutureByRoot =  
{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD,VT_RECORD,VT_BOOL} };

_ATL_FUNC_INFO CStructureProviderExNotify::m_ArrivedFuturesOption =  
{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD,VT_RECORD,VT_BOOL} };

STDMETHODIMP CStructureProviderExNotify::OnError(ErrorNumberEnum ErrorNumber, BSTR Description, RequestsTypeEnum ReqType, VARIANT Request)
{
HRESULT hr = S_OK;
if(m_pClient)
{
	hr = m_pClient->OnError((ErrorNumberEnum)ErrorNumber, Description, (RequestsTypeEnum)ReqType, Request);
}
return hr;
}

STDMETHODIMP CStructureProviderExNotify::OnStock(StockParams *Params, StockResultsEx *Results)
{	
HRESULT hr = S_OK;
if(m_pClient && Params && Results)
{
	CComVariant vtPar;
	_StockParams par;
	par.Init(Params);
	par.CopyTo(vtPar);

	CComVariant vtResult;
	_StockResultsEx info;
	info.Init(Results);
	info.CopyTo(vtResult);

	hr = m_pClient->OnStock(vtPar, vtResult);
}
return hr;
}
STDMETHODIMP CStructureProviderExNotify::OnOption(OptionParams *Params, OptionResultsEx *Results, VARIANT_BOOL bLast)
{	
HRESULT hr = S_OK;
if(m_pClient)
{
	CComVariant vtVarIsLastOption;
	vtVarIsLastOption = bLast;

	CComVariant vtPar;
	_OptionParams par;
	par.Init(Params);
	par.CopyTo(vtPar);

	CComVariant vtResult;
	_OptionResultsEx info;
	info.Init(Results);
	info.CopyTo(vtResult);   
	
	hr = m_pClient->OnOption(vtPar, vtResult, vtVarIsLastOption);
}
return hr;
}

STDMETHODIMP CStructureProviderExNotify::OnFuture(FutureParams *Params, FutureResultsEx *Results)
{	
	HRESULT hr = S_OK;
	if(m_pClient && Params && Results)
	{
		CComVariant vtPar;
		_FutureParams par;
		par.Init(Params);
		par.CopyTo(vtPar);

		CComVariant vtResult;
		_FutureResultsEx info;
		info.Init(Results);
		info.CopyTo(vtResult);

		hr = m_pClient->OnFuture(vtPar, vtResult);
	}
	return hr;
}
STDMETHODIMP CStructureProviderExNotify::OnFutureByRoot(FutureRootParams *Params, FutureResultsEx *Results, VARIANT_BOOL bLast)
{	
	HRESULT hr = S_OK;
	if(m_pClient && Params && Results)
	{
		CComVariant vtVarIsLastOption;
		vtVarIsLastOption = bLast;

		CComVariant vtPar;
		_FutureRootParams par;
		par.Init(Params);
		par.CopyTo(vtPar);

		CComVariant vtResult;
		_FutureResultsEx info;
		info.Init(Results);
		info.CopyTo(vtResult);

		hr = m_pClient->OnFutureByRoot(vtPar, vtResult, vtVarIsLastOption);
	}
	return hr;
}
STDMETHODIMP CStructureProviderExNotify::OnFuturesOption(FuturesOptionParams *Params, FuturesOptionResultsEx *Results, VARIANT_BOOL bLast)
{	
	HRESULT hr = S_OK;
	if(m_pClient)
	{
		CComVariant vtVarIsLastOption;
		vtVarIsLastOption = bLast;

		CComVariant vtPar;
		_FuturesOptionParams par;
		par.Init(Params);
		par.CopyTo(vtPar);

		CComVariant vtResult;
		_FuturesOptionResultsEx info;
		info.Init(Results);
		info.CopyTo(vtResult);   

		hr = m_pClient->OnFuturesOption(vtPar, vtResult, vtVarIsLastOption);
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CStructureProviderEx

STDMETHODIMP CStructureProviderEx::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IStructureProvider,
		&IID_IStructureProvider2
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CStructureProviderEx::RequestStock(StockParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
			hr = m_spStructureProvider->RequestStock(reinterpret_cast<StockParams*>(Params));
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
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::CancelStock(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
			hr = m_spStructureProvider->CancelStock(Params);
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
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::RequestOptions(OptionParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
			hr = m_spStructureProvider->RequestOptions(reinterpret_cast<OptionParams*>(Params));
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
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::CancelOptions(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
			hr = m_spStructureProvider->CancelOptions(Params);
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
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;

}
STDMETHODIMP CStructureProviderEx::Connect()
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
			hr = m_spStructureProvider->Connect();
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
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	
	if(SUCCEEDED(hr))
		m_bConnected = true;
	return hr;
}
STDMETHODIMP CStructureProviderEx::Disconnect()
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider!=NULL)
			hr = m_spStructureProvider->Disconnect();
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error())) 
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	if(SUCCEEDED(hr))
		m_bConnected = false;
	return hr;
}

STDMETHODIMP CStructureProviderEx::RequestFuture(FutureParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider2!=NULL)
			hr = m_spStructureProvider2->RequestFuture(reinterpret_cast<FutureParams*>(Params));
		else 
			hr = Error(_T("Structure Provider is not initialized or doesn't support futures"), IID_IStructureProvider2, E_NOTIMPL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = RequestFuture(Params);
			m_bRecursive = true;
		}
		else
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::CancelFuture(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider2!=NULL)
			hr = m_spStructureProvider2->CancelFuture(Params);
		else 
			hr = Error(_T("Structure Provider is not initialized or doesn't support futures"), IID_IStructureProvider2, E_NOTIMPL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = CancelFuture(Params);
			m_bRecursive = true;
		}
		else
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::RequestFutures(FutureRootParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider2!=NULL)
			hr = m_spStructureProvider2->RequestFutures(reinterpret_cast<FutureRootParams*>(Params));
		else 
			hr = Error(_T("Structure Provider is not initialized or doesn't support futures"), IID_IStructureProvider2, E_NOTIMPL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = RequestFutures(Params);
			m_bRecursive = true;
		}
		else
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::CancelFutures(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider2!=NULL)
			hr = m_spStructureProvider2->CancelFutures(Params);
		else 
			hr = Error(_T("Structure Provider is not initialized or doesn't support futures"), IID_IStructureProvider2, E_NOTIMPL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = CancelFutures(Params);
			m_bRecursive = true;
		}
		else
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::RequestFuturesOptions(FuturesOptionParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider2!=NULL)
			hr = m_spStructureProvider2->RequestFuturesOptions(reinterpret_cast<FuturesOptionParams*>(Params));
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider2, E_NOTIMPL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = RequestFuturesOptions(Params);
			m_bRecursive = true;
		}
		else
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CStructureProviderEx::CancelFuturesOptions(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spStructureProvider2!=NULL)
			hr = m_spStructureProvider2->CancelFuturesOptions(Params);
		else 
			hr = Error(_T("Structure Provider is not initialized"), IID_IStructureProvider2, E_NOTIMPL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = CancelFuturesOptions(Params);
			m_bRecursive = true;
		}
		else
			return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return hr;

}

//-----------------------------------------------------------------------------------------------//
HRESULT CStructureProviderEx::OnStock(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CStructureProviderEx::OnStock\n"));
	
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
			hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CStructureProviderEx::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CStructureProviderEx::OnOption\n"));
	HRESULT  hr = S_OK ;
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
	int nConnections = m_vec.GetSize();
	
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
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CStructureProviderEx::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
	ATLTRACE(_T("CStructureProviderEx::OnError\n"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[4];
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
//-----------------------------------------------------------------------------------------------//
HRESULT CStructureProviderEx::OnFuture(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CStructureProviderEx::OnFuture\n"));

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
			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CStructureProviderEx::OnFutureByRoot(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastFutures)
{
	ATLTRACE(_T("CStructureProviderEx::OnFutureByRoot\n"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
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
			pvars[2] = varParams;
			pvars[1] = varResults;
			pvars[0] = varIsLastFutures;
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			hr = pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CStructureProviderEx::OnFuturesOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CStructureProviderEx::OnFuturesOption\n"));
	HRESULT  hr = S_OK ;
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
	int nConnections = m_vec.GetSize();

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
			hr = pDispatch->Invoke(0x6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

STDMETHODIMP CStructureProviderEx::get_ProviderID(long *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CStructureProviderEx::get_Description(BSTR *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CStructureProviderEx::put_Description(BSTR newVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CStructureProviderEx::get_ProgID(BSTR *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
