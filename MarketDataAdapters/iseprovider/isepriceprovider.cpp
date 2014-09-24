// ISEPriceProvider.cpp : Implementation of CISEPriceProvider
#include "stdafx.h"
#include "ISEProvider.h"
#include "ISEPriceProvider.h"
#include "ISEManager.h"

/////////////////////////////////////////////////////////////////////////////
// CISEPriceProvider
/*
STDMETHODIMP CISEPriceProvider::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IPriceProvider
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
*/


STDMETHODIMP CISEPriceProvider::RequestLastQuote(QuoteUpdateParams* Params)
{
	_EventTypeEnum	Type;

	CSymbolParams	Request;
	if(Params->Symbol)
		Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
	if(Params->Exchange)
		Request.m_sExchange = (char*)_bstr_t(Params->Exchange);

	if(Request.m_sSymbol.length() == 0)
		return Error("Invalid symbol name.");

	switch(Params->Type)
	{
		case enOPT:
			Type = _enRequestLastOption;
			break;

		case enIDX:
			Type = _enRequestLastIndex;
			break;

		case enSTK:
			Type = _enRequestLastStock;
			break;

		default:
			return Error("Invalid instrument type");
	}

	return ((CPriceProviderManagerObject*)theISEManager)->AddRequest(this, Request, Type);
}

STDMETHODIMP CISEPriceProvider::CancelLastQuote(VARIANT vtParams)
{
	_EventTypeEnum	Type = _enRequestLastQuote;
	CSymbolParams	Request;

	if(CComVariant(vtParams) != vtMissing)
	{
		try
		{
			_QuoteUpdateParams Params(vtParams);
			Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
			if(Request.m_sSymbol.length() == 0)
				return Error("Invalid symbol name.");

			Request.m_sExchange = (char*)_bstr_t(Params->Exchange);

			switch(Params->Type)
			{
				case enOPT:
					Type = _enRequestLastOption;
					break;

				case enIDX:
					Type = _enRequestLastIndex;
					break;

				case enSTK:
					Type = _enRequestLastStock;
					break;

				default:
					return Error("Invalid instrument type");
			}
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
	}

	return ((CPriceProviderManagerObject*)theISEManager)->RemoveRequest(this, Request, Type);
}

STDMETHODIMP CISEPriceProvider::SubscribeQuote(QuoteUpdateParams* Params)
{
	_EventTypeEnum	Type;

	CSymbolParams	Request;
	if(Params->Symbol)
		Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
	if(Params->Exchange)
		Request.m_sExchange = (char*)_bstr_t(Params->Exchange);

	if(Request.m_sSymbol.length() == 0)
		return Error("Invalid symbol name.");

	switch(Params->Type)
	{
		case enOPT:
			Type = _enSubscribeOption;
			break;

		case enIDX:
			Type = _enSubscribeIndex;
			break;

		case enSTK:
			Type = _enSubscribeStock;
			break;

		default:
			return Error("Invalid instrument type");
	}

	return ((CPriceProviderManagerObject*)theISEManager)->AddRequest(this, Request, Type, true);
}

STDMETHODIMP CISEPriceProvider::UnSubscribeQuote(VARIANT vtParams)
{
	CSymbolParams	Request;
	_EventTypeEnum	Type = _enSubscribeQuote;

	if(CComVariant(vtParams) != vtMissing)
	{
		try
		{
			_QuoteUpdateParams Params(vtParams);
			Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
			if(Request.m_sSymbol.length() == 0)
				return Error("Invalid symbol name.");

			Request.m_sExchange = (char*)_bstr_t(Params->Exchange);
			switch(Params->Type)
			{
				case enOPT:
					Type = _enSubscribeOption;
					break;

				case enIDX:
					Type = _enSubscribeIndex;
					break;

				case enSTK:
					Type = _enSubscribeStock;
					break;

				default:
					return Error("Invalid instrument type");
			}
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
	}

	return ((CPriceProviderManagerObject*)theISEManager)->RemoveRequest(this, Request, Type, true);
}

STDMETHODIMP CISEPriceProvider::Connect()
{
	EgTrace("CISEPriceProvider::Connect");
	return ((CPriceProviderManagerObject*)theISEManager)->Connect(this);
}

STDMETHODIMP CISEPriceProvider::Disconnect()
{
	EgTrace("CISEPriceProvider::Disconnect");
	return ((CPriceProviderManagerObject*)theISEManager)->Disconnect(this);
}

//----------------------------------- Group price -----------------------------------------------//

STDMETHODIMP CISEPriceProvider::RequestGroup(QuoteUpdateParams* Params,/*[in]*/ GroupRequestType enOperationType)
{
	_EventTypeEnum	Type;

	CSymbolParams	Request;
	if(Params->Symbol)
		Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
	if(Params->Exchange)
		Request.m_sExchange = (char*)_bstr_t(Params->Exchange);

	if(Request.m_sSymbol.length() == 0)
		return Error("Invalid symbol name.");
	
	HRESULT hr = E_UNEXPECTED;

	if(enOperationType == enGroupRequestLastQuote || enOperationType == enGroupRequestAll)
	{
		switch(Params->Type)
		{
			case enGrSTK:
			case enSTK:
				Type = _enRequestGroupStock;
				break;

			case enGrIDX:
			case enIDX:
				Type = _enRequestGroupIndex;
				break;

			default:
				return Error("Invalid instrument type");
		}

		Request.m_InstrumentType = Params->Type;
		hr = ((CPriceProviderManagerObject*)theISEManager)->AddRequest(this, Request, Type, false);

		if(FAILED(hr)) return hr;
	}

	if(enOperationType == enGroupRequestSubscribe || enOperationType == enGroupRequestAll)
	{
		switch(Params->Type)
		{
			case enGrSTK:
			case enSTK:
				Type = _enSubscribeGroupStock;
				break;

			case enGrIDX:
			case enIDX:
				Type = _enSubscribeGroupIndex;
				break;

			default:
				return Error("Invalid instrument type");
		}

		Request.m_InstrumentType = Params->Type;
		hr = ((CPriceProviderManagerObject*)theISEManager)->AddRequest(this, Request, Type, true);
	}

	return hr;
}

STDMETHODIMP CISEPriceProvider::CancelGroup(VARIANT vtParams)
{
	CSymbolParams	Request;

	_EventTypeEnum	TypeLast = _enRequestGroup;
	_EventTypeEnum	TypeSubscr = _enSubscribeGroup;

	if(CComVariant(vtParams) != vtMissing)
	{
		try
		{
			_QuoteUpdateParams Params(vtParams);
			Request.m_sSymbol = (char*)_bstr_t(Params->Symbol);
			if(Request.m_sSymbol.length() == 0)
				return Error("Invalid symbol name.");

			Request.m_sExchange = (char*)_bstr_t(Params->Exchange);
			switch(Params->Type)
			{
				case enGrSTK:
				case enSTK:
					TypeLast = _enRequestGroupStock;
					TypeSubscr = _enSubscribeGroupStock;
					break;

				case enGrIDX:
				case enIDX:
					TypeLast = _enRequestGroupIndex;
					TypeSubscr = _enSubscribeGroupIndex;
					break;

				default:
					return Error("Invalid instrument type");
			}
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
	}

	HRESULT hr = ((CPriceProviderManagerObject*)theISEManager)->RemoveRequest(this, Request, TypeSubscr, true);
	if(FAILED(hr))
		return hr;

	return ((CPriceProviderManagerObject*)theISEManager)->RemoveRequest(this, Request, TypeLast);
}

//--------------------------------------- Fire Events -------------------------------------------//
/*

void	CISEPriceProvider::FireOnLastQuote(const _EventTypeEnum RequestType,
						const CSymbolParams& RequestParams,
						const CQuoteBBO& BBO)
{
	CComVariant varParams;
	CComVariant varResult;

	if(ConvertQuote(RequestType, RequestParams, BBO,
					varParams, varResult))
	{
		OnLastQuote(varParams, varResult);
	}
}


void	CISEPriceProvider::FireOnQuoteUpdate(	const _EventTypeEnum RequestType,
							const CSymbolParams& RequestParams,
							const CQuoteBBO& BBO)
{
	CComVariant varParams;
	CComVariant varResult;

	if(ConvertQuote(RequestType, RequestParams, BBO,
					varParams, varResult))
	{
		OnQuoteUpdate(varParams, varResult);
	}
}

void	CISEPriceProvider::FireOnError(const CSymbolParams& Request, const _EventTypeEnum RequestType,
					const ErrorNumberEnum enumError, const string& sDescription)
{
	USES_CONVERSION;

	_QuoteUpdateParams Req;
	Req.Init();
	Req->Symbol = SysAllocString(A2W(Request.m_sSymbol.c_str()));
	Req->Exchange = SysAllocString(A2W(Request.m_sExchange.c_str()));
	switch(RequestType)
	{
		case _enRequestLastStock:
			Req->Type = enSTK;
			break;

		case _enRequestLastOption:
			Req->Type = enOPT;
			break;

		case _enRequestLastIndex:
			Req->Type = enIDX;
			break;

		default:
			EgAssert(false);
			return;
	}
	

	CComVariant varParams;
	Req.CopyTo(varParams);

	OnError(enumError, sDescription.c_str(), enRequestLastQuote, varParams);
}
*/
//--------------------------------------- CP ----------------------------------------------------//
HRESULT CISEPriceProvider::OnLastQuote(const CComVariant &varParams,const CComVariant &varResults)
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

HRESULT CISEPriceProvider::OnQuoteUpdate(const CComVariant &varParams,const  CComVariant &varResults)
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

HRESULT CISEPriceProvider::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
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

HRESULT	CISEPriceProvider::OnEvent(const EventNumberEnum EventNumber, _bstr_t Description)
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
			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
