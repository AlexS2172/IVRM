// PriceProvider.cpp : Implementation of CPriceProvider
#include "stdafx.h"
#include "PriceProviders.h"
#include "PriceProvider.h"


//-------------------------------------------------------------------------------

_ATL_FUNC_INFO CPriceProviderNotify::m_ArrivedError = 
	{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };
 
_ATL_FUNC_INFO CPriceProviderNotify::m_ArrivedLastQuote =  
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

_ATL_FUNC_INFO CPriceProviderNotify::m_ArrivedQuoteUpdate =  
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

STDMETHODIMP CPriceProviderNotify::OnError(ErrorNumberEnum ErrorNumber, BSTR Description, RequestsTypeEnum ReqType, VARIANT Request)
{
HRESULT hr = S_OK;
if(m_pClient)
{
	hr = m_pClient->OnError((ErrorNumberEnum)ErrorNumber, Description, (RequestsTypeEnum)ReqType, Request);
}
return hr;
}

STDMETHODIMP CPriceProviderNotify::OnLastQuote(QuoteUpdateParams *Params, QuoteUpdateResult *Results)
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
		_QuoteUpdateResult info;
		info.Init(Results);
		info.CopyTo(vtResult);
	}

	hr = m_pClient->OnLastQuote(vtPar, vtResult);
}
return hr;
}
STDMETHODIMP CPriceProviderNotify::OnQuoteUpdate(QuoteUpdateParams *Params, QuoteUpdateResult *Results)
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
		_QuoteUpdateResult info;
		info.Init(Results);
		info.CopyTo(vtResult);
	}
	
	hr = m_pClient->OnQuoteUpdate(vtPar, vtResult);
}
return hr;
}


/////////////////////////////////////////////////////////////////////////////
// CPriceProvider
STDMETHODIMP CPriceProvider::RequestLastQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->RequestLastQuote(reinterpret_cast<QuoteUpdateParams*>(Params)));
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
STDMETHODIMP CPriceProvider::CancelLastQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->CancelLastQuote(Params));
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
STDMETHODIMP CPriceProvider::SubscribeQuote(QuoteUpdateParams *Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->SubscribeQuote(reinterpret_cast<QuoteUpdateParams*>(Params)));
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = SubscribeQuote(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CPriceProvider::UnSubscribeQuote(VARIANT Params)
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)
		{
			__CHECK_HRESULT3(m_spPriceProvider->UnSubscribeQuote(Params));
		}
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) && m_bRecursive && SUCCEEDED(hr = Reconnect())) 
		{
			m_bRecursive = false;
			hr = UnSubscribeQuote(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}
STDMETHODIMP CPriceProvider::Connect()
{
	HRESULT hr = S_OK;
	try 
	{
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
		m_bConnected = true;
	return hr;
}
STDMETHODIMP CPriceProvider::Disconnect()
{
	HRESULT hr = S_OK;
	try 
	{
		if(m_spPriceProvider!=NULL)		
			hr = m_spPriceProvider->Disconnect();
		else 
			hr = Error(_T("Price Provider is not initialized"), IID_IPriceProvider, E_FAIL);
	}
	catch(_com_error& err)
	{
		if(IsHandledError(err.Error()) ) 
			return utils::ComError2ErrInfo (err,this);		
	}
	if(SUCCEEDED(hr))
		m_bConnected = false;
	return hr;
}


STDMETHODIMP CPriceProvider::RequestGroup(QuoteUpdateParams *Params, GroupRequestType enTypeGroupRequest)
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
			hr = RequestGroup(Params, enTypeGroupRequest);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}

STDMETHODIMP CPriceProvider::CancelGroup(VARIANT Params)
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
			hr = CancelGroup(Params);
			m_bRecursive = true;
		}
		else
			return utils::ComError2ErrInfo (err,this);		
	}
	return hr;
}



HRESULT CPriceProvider::OnQuoteUpdate(const CComVariant &varParams,const  CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceProvider::OnQuoteUpdate\n"));
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
HRESULT CPriceProvider::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceProvider::OnError\n"));
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

HRESULT CPriceProvider::OnLastQuote(const CComVariant &varParams,const CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATLTRACE(_T("CPriceProvider::OnLastQuote\n"));
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

STDMETHODIMP CPriceProvider::get_ProviderID(long *pVal)
{
	return S_OK;
}

STDMETHODIMP CPriceProvider::get_Description(BSTR *pVal)
{
	return S_OK;
}

STDMETHODIMP CPriceProvider::put_Description(BSTR newVal)
{
	return S_OK;
}

STDMETHODIMP CPriceProvider::get_ProgID(BSTR *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
