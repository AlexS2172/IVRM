// ISEStructureProvider.cpp : Implementation of CISEStdStructureProvider
#include "stdafx.h"
#include "ISEProvider.h"
#include "ISEStdStructureProvider.h"
#include "ISEManager.h"
#include "OmniApiUtils.h"

/////////////////////////////////////////////////////////////////////////////
// CISEStdStructureProvider
/*
STDMETHODIMP CISEStdStructureProvider::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IStructureProvider
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
*/

STDMETHODIMP CISEStdStructureProvider::RequestStock(StockParams* Params)
{
	CSymbolParams	Request;
	if(Params->Stock)
		Request.m_sSymbol = (char*)_bstr_t(Params->Stock);
	if(Params->Exchange)
		Request.m_sExchange = (char*)_bstr_t(Params->Exchange);

	if(Request.m_sSymbol.length() == 0)
		return Error("Invalid symbol name.");

	return ((CStructureProviderManagerObject*)theISEManager)->AddRequest(this, Request, _enRequestStock);
}

STDMETHODIMP CISEStdStructureProvider::CancelStock(VARIANT vtParams)
{
	CSymbolParams	Request;

	if(CComVariant(vtParams) != vtMissing)
	{
		try
		{
			_StockParams Params(vtParams);
			Request.m_sSymbol = (char*)_bstr_t(Params->Stock);
			if(Request.m_sSymbol.length() == 0)
				return Error("Invalid symbol name.");

			Request.m_sExchange = (char*)_bstr_t(Params->Exchange);
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
	}

	return ((CStructureProviderManagerObject*)theISEManager)->RemoveRequest(this, Request, _enRequestStock);
}

STDMETHODIMP CISEStdStructureProvider::RequestOptions(OptionParams* Params)
{
	CSymbolParams	Request;
	if(Params->UnderlyingSymbol)
		Request.m_sSymbol = (char*)_bstr_t(Params->UnderlyingSymbol);
	if(Params->Exchange)
		Request.m_sExchange = (char*)_bstr_t(Params->Exchange);

	if(Request.m_sSymbol.length() == 0)
		return Error("Invalid symbol name.");

	return ((CStructureProviderManagerObject*)theISEManager)->AddRequest(this, Request, _enRequestOption);
}

STDMETHODIMP CISEStdStructureProvider::CancelOptions(VARIANT vtParams)
{
	CSymbolParams	Request;

	if(CComVariant(vtParams) != vtMissing)
	{
		try
		{
			_OptionParams Params(vtParams);
			Request.m_sSymbol = (char*)_bstr_t(Params->UnderlyingSymbol);
			if(Request.m_sSymbol.length() == 0)
				return Error("Invalid symbol name.");

			Request.m_sExchange = (char*)_bstr_t(Params->Exchange);	
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
	}

	return ((CStructureProviderManagerObject*)theISEManager)->RemoveRequest(this, Request, _enRequestOption);
}

STDMETHODIMP CISEStdStructureProvider::Connect()
{
	EgTrace("CISEStdStructureProvider::Connect");
	return ((CStructureProviderManagerObject*)theISEManager)->Connect(this);
}

STDMETHODIMP CISEStdStructureProvider::Disconnect()
{
	EgTrace("CISEStdStructureProvider::Disconnect");
	return ((CStructureProviderManagerObject*)theISEManager)->Disconnect(this);
}

//-----------------------------------------------------------------------------------------------//

HRESULT CISEStdStructureProvider::OnStock(const CComVariant &varParams,const CComVariant &varResults, const CComVariant &varIsLastStock)
{
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

    CComVariant _varResults;

    _ISEStockResults ResultSrc(varResults);
    _StockResults ResultDest;
	ResultDest.Init();

    ResultDest->StockType = ResultSrc->StockType;
    ResultDest->DivFreq = 0;
    ResultDest->DivLastDate = 0;
    ResultDest->DivAmount = 0;
    ResultDest->Currency = SysAllocString(ResultSrc->Currency);
    ResultDest->Description = SysAllocString(ResultSrc->Description);
    ResultDest->LotSize = ResultSrc->LotSize;
    ResultDest->Exchange = SysAllocString(ResultSrc->Exchange);

    ResultDest.CopyTo(_varResults);

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
			pvars[0] = _varResults;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CISEStdStructureProvider::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
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

HRESULT CISEStdStructureProvider::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
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

HRESULT	CISEStdStructureProvider::OnEvent(const EventNumberEnum EventNumber, _bstr_t Description)
{
    return S_OK;
}
