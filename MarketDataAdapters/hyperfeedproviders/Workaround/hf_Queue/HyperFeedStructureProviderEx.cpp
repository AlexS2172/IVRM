// HyperFeedStructureProviderEx.cpp : Implementation of CHyperFeedStructureProviderEx

#include "stdafx.h"
#include "HyperFeedProviders.h"
#include "HyperFeedStructureProviderEx.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedStructureProviderEx
STDMETHODIMP CHyperFeedStructureProviderEx::InterfaceSupportsErrorInfo(REFIID riid)
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

STDMETHODIMP CHyperFeedStructureProviderEx::RequestStock(StockParams *Params)
{
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CStockRequest(Params));
		EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__ , CW2T(Params->Stock));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::CancelStock(VARIANT Params)
{
	try
	{
		Cancel();
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::RequestOptions(OptionParams *Params)
{
	try
	{ 
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new COptionRequest(Params));
		EgStd::CEgTracingClass::TraceStatic(enlogInfo, __FUNCTION__ , CW2T(Params->UnderlyingSymbol));

		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err, this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::CancelOptions(VARIANT Params)
{
	try
	{
		Cancel();
		CMasterOptions::CancelRequest();
		COptions::CancelRequest();
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err, this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::RequestFuture(FutureParams *Params)
{
	try
	{ 
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CFutureRequest(Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::CancelFuture(VARIANT Params)
{
	try
	{
		Cancel();
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::RequestFutures(FutureRootParams *Params)
{
	try
	{ 
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CFuturesRequest(Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::CancelFutures(VARIANT Params)
{
	try
	{
		Cancel();
		CFutures::CancelRequest(Params);
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::RequestFuturesOptions(FuturesOptionParams *Params)
{
	try
	{ 
		CRequestBasePtr pRequest = boost::shared_static_cast<CRequestBase>(CFuturesOptionRequestPtr(new CFuturesOptionRequest(Params)));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}

STDMETHODIMP CHyperFeedStructureProviderEx::CancelFuturesOptions(VARIANT Params)
{
	try
	{
		Cancel();
		CFuturesOptions::CancelRequest();
		CFutures::CancelRequest();
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::Connect()
{
	try 
	{
		ConnectProvider();
	}
	catch(_com_error& err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProviderEx::Disconnect()
{
	CFuturesOptions::CancelRequest();
	CFutures::CancelRequest();
	CMasterOptions::CancelRequest();
	COptions::CancelRequest();
	DisconnectProvider();
	return S_OK;
}
long CHyperFeedStructureProviderEx::Attach()
{
	long nRes = CMasterOptions::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to master options database."));
	}
	nRes = COptions::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price database."));
	}
	nRes = CUnderlyings::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to security profile database."));
	}
	nRes = CFutures::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to futures fundamental database."));
	}
	nRes = CFuturesOptions::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price database."));
	}	

	nRes = CPrice::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price database."));
	}
	return nRes ;
}
long CHyperFeedStructureProviderEx::Detach()
{
	long nRes = CMasterOptions::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from master options database."));
	}
	nRes = COptions::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price database."));
	}
	nRes = CUnderlyings::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from security profile database."));
	}
	nRes = CFutures::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from futures fundamental database."));
	}
	nRes = CFuturesOptions::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price database."));
	}	
	nRes = CPrice::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price database."));
	}	
	return nRes ;
}

void CHyperFeedStructureProviderEx::OnResponse(CResponseBasePtr pResponse)
{
	switch(pResponse->m_enType)
	{
	case _enResponseStock:
		{
			CStockResultExResponse* pStockResponse = static_cast<CStockResultExResponse*>(pResponse.get());
			if(pStockResponse)
				OnStock(pStockResponse->m_vtParams, pStockResponse->m_vtResult);
		}
		break;
	case _enResponseOption:
		{
			COptionResultExResponse* pOptionResponse = static_cast<COptionResultExResponse*>(pResponse.get());
			if(pOptionResponse)
				OnOption(pOptionResponse->m_vtParams, pOptionResponse->m_vtResult, CComVariant(pOptionResponse->m_bIsLast));
		}
		break;
	case _enResponseFuture:
		{
			CFutureResultExResponse* pFutureResponse = static_cast<CFutureResultExResponse*>(pResponse.get());
			if(pFutureResponse)
				OnFuture(pFutureResponse->m_vtParams, pFutureResponse->m_vtResult);
		}
		break;
	case _enResponseFutures:
		{
			CFuturesResultExResponse* pFuturesResponse = static_cast<CFuturesResultExResponse*>(pResponse.get());
			if(pFuturesResponse)
				OnFutureByRoot(pFuturesResponse->m_vtParams, pFuturesResponse->m_vtResult, CComVariant(pFuturesResponse->m_bIsLast));
		}
		break;
	case _enResponseFuturesOption:
		{
			CFutureOptionResultExResponse* pFutureOptionResponse = static_cast<CFutureOptionResultExResponse*>(pResponse.get());
			if(pFutureOptionResponse)
				OnFuturesOption(pFutureOptionResponse->m_vtParams, pFutureOptionResponse->m_vtResult, CComVariant(pFutureOptionResponse->m_bIsLast));
		}
		break;
	}
}

void CHyperFeedStructureProviderEx::OnRequest(CRequestBasePtr pRequest)
{
	switch(pRequest->m_enType)
	{
	case _enRequestStock:
		{
			CStockRequest* pStockRequest = static_cast<CStockRequest*>(pRequest.get());
			if(pStockRequest)
				GetStockResultsEx(pStockRequest->m_recParams);
		}
		break;
	case _enRequestOption:
		{
			COptionRequest* pOptionRequest = static_cast<COptionRequest*>(pRequest.get());
			if(pOptionRequest)
			{
				pOptionRequest->m_recParams.CopyTo(m_vtCurrentRequest);
				if(!IsCanceled())
					LoadOptionFundamentalInfo(pOptionRequest->m_recParams->UnderlyingSymbol);
				if(!IsCanceled())
					GetOptions(pOptionRequest->m_recParams->UnderlyingSymbol);
			}
		}
		break;
	case _enRequestFuture:
		{
			CFutureRequest* pFutureRequest = static_cast<CFutureRequest*>(pRequest.get());
			if(pFutureRequest)
				GetFutureResultsBySymbol(pFutureRequest->m_recParams, true);
		}
		break;
	case _enRequestFutures:
		{
			CFuturesRequest* pFuturesRequest = static_cast<CFuturesRequest*>(pRequest.get());
			if(pFuturesRequest)
				GetFutureResultsByRootSymbol(pFuturesRequest->m_recParams, true);
		}
		break;
	case _enRequestFuturesOption:
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Requst Futures Options Start"));
			CFuturesOptionRequestPtr pFuturesOptionRequest = boost::shared_dynamic_cast<CFuturesOptionRequest>(pRequest);
			if(pFuturesOptionRequest)
			{
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("pFuturesOptionRequest"));
				pFuturesOptionRequest->m_recParams.CopyTo(m_vtCurrentRequest);
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetFutureInfo Call"));
				long nResult = GetFutureInfo(pFuturesOptionRequest->m_recParams, false);
				if(nResult == DBA_ERR_NO_ERROR) // Sharky: try to resolve FT_CQ00001489
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetFutureInfo Call - No Error"));
					QuoteUpdateParams Params;
					Params.Symbol = ::SysAllocString(pFuturesOptionRequest->m_recParams->FutureSymbol);
					Params.Type = enFUT;
					Params.Exchange = NULL;

					CQuoteRequestPtr pQuoteRequest = CQuoteRequestPtr(new CQuoteRequest(_enRequestFuturesOption, &Params));
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetPrice for: Symbol - %s"), CW2T(Params.Symbol) );
					nResult = GetPrice(boost::shared_static_cast<CRequestBase>(pQuoteRequest), false);

					::SysFreeString(Params.Symbol);

					if(nResult == DBA_ERR_NO_ERROR) // Sharky: try to resolve FT_CQ00001489
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Got Price for: %s"), CW2T(Params.Symbol));
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Requesting future options for: %s"), CW2T(Params.Symbol));
						GetFutureOptionsResults(m_clCurrentFuture, false);
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Got future options for: %s"), CW2T(Params.Symbol));

					}
					else
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetPrice FAILED for: %s"),CW2T(Params.Symbol));
						/* Lets try to get with 'G' symbol */

						//Params.Exchange = ::SysAllocString(L"G");
						CComBSTR cbsTmp(pFuturesOptionRequest->m_recParams->FutureSymbol);
						cbsTmp.Append(L"G");
						Params.Symbol = cbsTmp.Detach();


						CQuoteRequestPtr pQuoteRequest = CQuoteRequestPtr(new CQuoteRequest(_enRequestFuturesOption, &Params));
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetPrice for: Symbol - %s"), CW2T(Params.Symbol) );
						nResult = GetPrice(boost::shared_dynamic_cast<CRequestBase>(pQuoteRequest), false);
						//::SysFreeString(Params.Symbol);
						//::SysFreeString(Params.Exchange );

						if(nResult == DBA_ERR_NO_ERROR) 
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Got Price for: %s"), CW2T(Params.Symbol));
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Requesting future options for: %s"), CW2T(Params.Symbol));
							GetFutureOptionsResults(m_clCurrentFuture, false);
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Got future options for: %s"), CW2T(Params.Symbol));
						}
						else
						{
							EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetPrice FAILED for: %s"),CW2T(Params.Symbol));

						}
						/* ---Lets try to get with 'G' symbol */
					}

				}
				else
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("GetFutureInfo Call - Failed"));
				}

				if(nResult != DBA_ERR_NO_ERROR) // Sharky: try to resolve FT_CQ00001489
				{
					EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Request future options - got error"));

					if(nResult == DBA_TERMINATED)
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Operation was canceled by user"));
						PublicGetFutureOptionError(enInternalError, _bstr_t(L"Operation was canceled by user"));
					}
					if(nResult == DBA_ERR_KEY_NOT_FOUND)
					{
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("Couldn't get future options for future. Unknown symbol"));
						PublicGetFutureOptionError(enNoDataAvailableForSymbol, _bstr_t(L"Couldn't get future options for future. Unknown symbol"));
					}
					else
					{
						_bstr_t bs =  "Couldn't get options for future. Error: ";
						bs += EtGetMessage(DBA_ERROR,nResult);
						TCHAR buffer[0x100] = {0};				
						_ltot(nResult,buffer,10);
						bs += " (";
						bs += buffer;
						bs += ")";
						EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T(buffer));
						PublicGetFutureOptionError(enProviderInternalError, bs);
					}
				}
			}
		}
		break;
	}
}


//-----------------------------------------------------------------------------------------------//
HRESULT CHyperFeedStructureProviderEx::OnStock(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedStructureProviderEx::OnStock\n"));
	
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
HRESULT CHyperFeedStructureProviderEx::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CHyperFeedStructureProviderEx::OnOption\n"));
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
HRESULT CHyperFeedStructureProviderEx::OnFuture(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedStructureProviderEx::OnFuture\n"));

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
HRESULT CHyperFeedStructureProviderEx::OnFutureByRoot(const CComVariant &varParams,const CComVariant &varResults, const CComVariant &varIsLastFutures)
{
	ATLTRACE(_T("CHyperFeedStructureProviderEx::OnFutureByRoot\n"));

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
HRESULT CHyperFeedStructureProviderEx::OnFuturesOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
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

void CHyperFeedStructureProviderEx::PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription)
{
	CErrorResponse* pError = new CErrorResponse();
	CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);

	pError->m_enRequestType = enRequestOptions;
	pError->m_vtRequest = m_vtCurrentRequest;
	pError->m_bstrDescription = bsDescription;
	pError->m_Error = nError;
	PublicResponse(pErrorPtr);
}

void CHyperFeedStructureProviderEx::PublicLastOption()
{
	_OptionResultsEx results;
	results.Init();
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*) new COptionResultExResponse(m_vtCurrentRequest, results, true));
	PublicResponse(pOption);
}

void CHyperFeedStructureProviderEx::PublicOption(DBA_OPTIONS_FILTER_RECORD* pData)
{
	_OptionResultsEx results;
	results.Init();
	CopyOptionFilterToResult(*pData, results);
	CopyFundamentalInfoToResult(results);
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new COptionResultExResponse(m_vtCurrentRequest, results, false));
	PublicResponse(pOption);
	if(results->Series)
	{
		SysFreeString(results->Series);
		results->Series = NULL;
	}
	if(results->Currency)
	{
		SysFreeString(results->Currency);
		results->Currency = NULL;
	}
}

void CHyperFeedStructureProviderEx::PublicGetFutureOptionError(ErrorNumberEnum nError, BSTR bsDescription)
{
	CErrorResponse* pError = new CErrorResponse();
	pError->m_enRequestType = enRequestFuturesOptions;
	pError->m_vtRequest = m_vtCurrentRequest;
	pError->m_bstrDescription = bsDescription;
	pError->m_Error = nError;
	CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
	PublicResponse(pErrorPtr);
}

void CHyperFeedStructureProviderEx::PublicLastFutureOption()
{
	_FuturesOptionResultsEx results;
	results.Init();
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new CFutureOptionResultExResponse(m_vtCurrentRequest, results, true));
	PublicResponse(pOption);
}

void CHyperFeedStructureProviderEx::PublicFutureOption(_FuturesOptionResultsEx& results)
{
	CopyFutureFundamentalInfoToResult(results);
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new CFutureOptionResultExResponse(m_vtCurrentRequest, results, false));
	PublicResponse(pOption);
	if(results->Series)
	{
		SysFreeString(results->Series);
		results->Series = NULL;
	}
}

void CHyperFeedStructureProviderEx::PublicFuture(const _FutureRootParams& params, _FutureResultsEx& results)
{
	results->ClosePrice = GetClosePrice(results->Symbol, enFUT);
	CResponseBasePtr pFuture = CResponseBasePtr((CResponseBase*)new CFuturesResultExResponse(params, results));
	PublicResponse(pFuture);
}

void CHyperFeedStructureProviderEx::PublicFuture(const _FutureParams& params, _FutureResultsEx& results)
{
	results->ClosePrice = GetClosePrice(results->Symbol, enFUT);
	CResponseBasePtr pFuture = CResponseBasePtr((CResponseBase*)new CFutureResultExResponse(params, results));
	PublicResponse(pFuture);
}

double CHyperFeedStructureProviderEx::GetClosePrice(BSTR bsSymbol, InstrumentTypeEnum enType)
{
	double dClosePrice = BAD_DOUBLE_VALUE;

	QuoteUpdateParams Params;
	Params.Symbol = ::SysAllocString(bsSymbol);
	Params.Type = enType;
	Params.Exchange = NULL;

	CQuoteRequestPtr pQuoteRequest = CQuoteRequestPtr(new CQuoteRequest(_enRequestNone, &Params));
	GetOneClosePrice(boost::shared_static_cast<CRequestBase>(pQuoteRequest), dClosePrice);
	::SysFreeString(Params.Symbol);
	
	return dClosePrice;
}

void CHyperFeedStructureProviderEx::PublicStock(const _StockParams& params, _StockResultsEx& results)
{
	results->ClosePrice = GetClosePrice(params->Stock, (results->StockType == enStock ? enSTK : enIDX));
	CResponseBasePtr pStock = CResponseBasePtr((CResponseBase*)new CStockResultExResponse(params, results));
	PublicResponse(pStock);
}
