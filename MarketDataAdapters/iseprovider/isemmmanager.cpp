// ISEPMMManager.cpp : Implementation of ISEMMManager
#include "stdafx.h"
#include "ISEProvider.h"
#include "ISEMMManager.h"
#include "ISEManager.h"

/////////////////////////////////////////////////////////////////////////////
// ISEMMManager
/*
STDMETHODIMP CISEMMManager::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IISEMMManager
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
*/

STDMETHODIMP CISEMMManager::SetPmmParameters(IMMParameters* Parameters)
{
	if(Parameters == NULL)
		return E_POINTER;

	return theISEManager->SetMMParameters(Parameters, true);
}

STDMETHODIMP CISEMMManager::SetCmmParameters(IMMParameters* Parameters)
{
	if(Parameters == NULL)
		return E_POINTER;

	return theISEManager->SetMMParameters(Parameters, false);
}

STDMETHODIMP CISEMMManager::Connect()
{
	EgTrace("CISEMMManager::Connect");

	HRESULT hr = ((CMMManagerObject*)theISEManager)->Connect(this, false);
	if(FAILED(hr))
		return hr;

	return ((CMMManagerPriceObject*)theISEManager)->Connect(this);
}

STDMETHODIMP CISEMMManager::Disconnect()
{
	EgTrace("CISEMMManager::Disconnect");

	HRESULT hr = ((CMMManagerObject*)theISEManager)->Disconnect(this);
	if(FAILED(hr))
		return hr;

	return ((CMMManagerPriceObject*)theISEManager)->Disconnect(this);
}

STDMETHODIMP CISEMMManager::SubmitQuotes(/*[in]*/ LPSAFEARRAY* saQuotes)
{
	if(saQuotes == NULL || *saQuotes == NULL)
		return E_POINTER;

	ISEQuote* pQuotes;

	HRESULT hr = SafeArrayAccessData(*saQuotes, (PVOID*)&pQuotes);
	if(FAILED(hr))
		return hr;

	UINT uiDim = SafeArrayGetDim(*saQuotes);
	if(uiDim == 1)
	{
		for (int iIndex = 0; iIndex < (*saQuotes)->rgsabound[0].cElements; iIndex++)
		{
			const ISEQuote& Quote = pQuotes[iIndex];

			if(Quote.Series == NULL)
			{
				hr = E_INVALIDARG;
				break;
			}

			CQuoteParams Params;
			Params.m_sSeries = (char*)_bstr_t(Quote.Series);
			if(Params.m_sSeries.length() == 0)
			{
				hr = Error(L"Invalid series name.");
				break;
			}

			Params.m_BidOrAsk = Quote.BidOrAsk;
			Params.m_dPremium = Quote.Premium;
			if(Params.m_dPremium < 0)
			{
				hr = Error(L"Invalid premium.");
				break;
			}

			Params.m_lQuantity = Quote.Quantity;
			if(Params.m_lQuantity < 0)
			{
				hr = Error(L"Invalid quantity.");
				break;
			}

			Params.m_lOpenQuantity = Quote.OpenQuantity;
			if(Params.m_lOpenQuantity < 0)
			{
				hr = Error(L"Invalid open quantity.");
				break;
			}

			Params.m_lStepUpBuffer = Quote.StepUpBuffer;
			if(Params.m_lStepUpBuffer < 0)
			{
				hr = Error(L"Invalid step up buffer.");
				break;
			}

			hr = ((CMMManagerObject*)theISEManager)->AddRequest(this, Params, _enSubmitQuote, true);
			if(FAILED(hr))
				break;
		}
	}
	else
	{
		hr = E_INVALIDARG;
	}

	SafeArrayUnaccessData(*saQuotes);

	return hr;
}

STDMETHODIMP CISEMMManager::RequestLastMmQuote(ISEMMQuoteParams* Params)
{
	CMMQuoteParams	Request;
	Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
	if(Request.m_sSymbol.length() == 0)
		return Error(L"Invalid symbol name.");

	Request.m_Type = Params->Type;
	Request.m_BidOrAsk = Params->BidOrAsk;

	return ((CMMManagerPriceObject*)theISEManager)->AddRequest(this, Request, _enRequestLastOption);
}

STDMETHODIMP CISEMMManager::CancelLastMmQuote(VARIANT vtParams)
{
	_EventTypeEnum	Type = _enRequestLastQuote;
	CMMQuoteParams	Request;

	if(CComVariant(vtParams) != vtMissing)
	{
		try
		{
			_ISEMMQuoteParams Params(vtParams);
			Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
			if(Request.m_sSymbol.length() == 0)
				return Error(L"Invalid symbol name.");

			Request.m_Type = Params->Type;
			Request.m_BidOrAsk = Params->BidOrAsk;
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
	}

	return ((CMMManagerPriceObject*)theISEManager)->RemoveRequest(this, Request, _enRequestLastOption);
}

STDMETHODIMP CISEMMManager::EnterOrder(ISEOrder* Order, BSTR* OrderID)
{
	USES_CONVERSION;

	if(!Order || !OrderID || !Order->Series)
		return E_POINTER;

	string sSeries = (char*)_bstr_t(Order->Series);
	if(sSeries.length() == 0)
		return Error(L"Invalid series name.");

	if(Order->Quantity <= 0)
		return Error(L"Invalid order quantity.");

	if(Order->Price <= 0)
		return Error(L"Invalid order price.");

	if(Order->BidOrAsk == enBoth)
		return Error(L"Order may only be of Ask or Bid type.");

	string sOrderID;

	HRESULT hr = theISEManager->EnterOrder(sSeries, Order->Quantity, 
		Order->Price, Order->BidOrAsk, sOrderID);

	if(SUCCEEDED(hr))
		*OrderID = SysAllocString(A2W(sOrderID.c_str()));

	return hr;
}



HRESULT CISEMMManager::OnQuoteRejection(const CComVariant& varParams, _bstr_t bsReason)
{
		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
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
				pvars[1] = varParams;
				pvars[0] = (BSTR)bsReason;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
}

HRESULT CISEMMManager::OnQuoteStepUp(const CComVariant& varParams)
{
		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
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
				pvars[0] = varParams;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
}

HRESULT CISEMMManager::OnQuoteTickWorse(const CComVariant& varParams, 
		long lTickWorseLevel, _bstr_t bsReason)
{
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
				pvars[1] = lTickWorseLevel;
				pvars[0] = (BSTR)bsReason;
				DISPPARAMS disp = { pvars, NULL, 3, 0 };
				pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
}


HRESULT CISEMMManager::OnQuotesSent(const CComVariant& varParams, ErrorNumberEnum Error, bstr_t bsOrderID)
{
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
				pvars[1] = Error;
				pvars[0] = (BSTR)bsOrderID;
				DISPPARAMS disp = { pvars, NULL, 3, 0 };
				HRESULT hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
}

HRESULT CISEMMManager::OnLastMmQuote(const CComVariant &varParams,const CComVariant &varResults, const long TickWorseLevel)
{
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
			pvars[0] = TickWorseLevel;
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			hr = pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CISEMMManager::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
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
			hr = pDispatch->Invoke(0x6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT	CISEMMManager::OnEvent(const EventNumberEnum EventNumber, _bstr_t Description)
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
			pvars[1] = EventNumber;
			pvars[0] = (BSTR)Description;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(0x9, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}



HRESULT CISEMMManager::OnDeal(const CComVariant& varParams)
{
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
			hr = pDispatch->Invoke(0x7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CISEMMManager::OnTrade(const CComVariant& varParams)
{
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
			hr = pDispatch->Invoke(0x8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
