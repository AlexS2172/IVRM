// ISEPriceInfo.cpp : Implementation of CISEPriceInfo
#include "stdafx.h"
#include "ISEProvider.h"
#include "ISEPriceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CISEPriceInfo

STDMETHODIMP CISEPriceInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IPriceProvider
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (ATL::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CISEPriceInfo::RequestLastQuote(QuoteUpdateParams* Params)
{
	return E_NOTIMPL;
}

STDMETHODIMP CISEPriceInfo::CancelLastQuote(VARIANT Params)
{
	return E_NOTIMPL;

}
STDMETHODIMP CISEPriceInfo::SubscribeQuote(QuoteUpdateParams* Params)
{
	return E_NOTIMPL;
}

STDMETHODIMP CISEPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	return E_NOTIMPL;
}

STDMETHODIMP CISEPriceInfo::Connect()
{
	return E_NOTIMPL;
}

STDMETHODIMP CISEPriceInfo::Disconnect()
{
	return E_NOTIMPL;
}

//----------------------------------- Group price -----------------------------------------------//

STDMETHODIMP CISEPriceInfo::RequestGroup(QuoteUpdateParams* Params,/*[in]*/ VARIANT_BOOL bSubscribe)
{
	return E_NOTIMPL;
}

STDMETHODIMP CISEPriceInfo::CancelGroup(VARIANT Params)
{
	return E_NOTIMPL;
}

//--------------------------------------- CP ----------------------------------------------------//

HRESULT CISEPriceInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
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

HRESULT CISEPriceInfo::OnLastQuote(const CComVariant &varParams, const CComVariant &varResults)
{
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

HRESULT CISEPriceInfo::OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults)
{
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