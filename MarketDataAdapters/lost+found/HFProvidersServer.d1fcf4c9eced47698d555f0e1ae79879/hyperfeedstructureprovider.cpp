// HyperFeedStructureProvider.cpp : Implementation of CHyperFeedStructureProvider
#include "stdafx.h"
#include "HyperFeedProviders.h"
#include "HyperFeedStructureProvider.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedStructureProvider
STDMETHODIMP CHyperFeedStructureProvider::InterfaceSupportsErrorInfo(REFIID riid)
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

STDMETHODIMP CHyperFeedStructureProvider::RequestStock(StockParams *Params)
{
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CStockRequest(Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProvider::CancelStock(VARIANT Params)
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
STDMETHODIMP CHyperFeedStructureProvider::RequestOptions(OptionParams *Params)
{
	try
	{ 
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new COptionRequest(Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err, this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProvider::CancelOptions(VARIANT Params)
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
STDMETHODIMP CHyperFeedStructureProvider::RequestFuture(FutureParams *Params)
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
STDMETHODIMP CHyperFeedStructureProvider::CancelFuture(VARIANT Params)
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
STDMETHODIMP CHyperFeedStructureProvider::RequestFutures(FutureRootParams *Params)
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
STDMETHODIMP CHyperFeedStructureProvider::CancelFutures(VARIANT Params)
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
STDMETHODIMP CHyperFeedStructureProvider::RequestFuturesOptions(FuturesOptionParams *Params)
{
	try
	{ 
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CFuturesOptionRequest(Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureProvider::CancelFuturesOptions(VARIANT Params)
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
STDMETHODIMP CHyperFeedStructureProvider::Connect()
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
STDMETHODIMP CHyperFeedStructureProvider::Disconnect()
{
	CFuturesOptions::CancelRequest();
	CFutures::CancelRequest();
	CMasterOptions::CancelRequest();
	COptions::CancelRequest();
	DisconnectProvider();
	return S_OK;
}
long CHyperFeedStructureProvider::Attach()
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
long CHyperFeedStructureProvider::Detach()
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

void CHyperFeedStructureProvider::OnResponse(CResponseBasePtr pResponse)
{
	switch(pResponse->m_enType)
	{
	case _enResponseStock:
		{
			CStockResultResponse* pStockResponse = dynamic_cast<CStockResultResponse*>(pResponse.get());
			if(pStockResponse)
				OnStock(pStockResponse->m_vtParams, pStockResponse->m_vtResult);
		}
		break;
	case _enResponseOption:
		{
			COptionResultResponse* pOptionResponse = dynamic_cast<COptionResultResponse*>(pResponse.get());
			if(pOptionResponse)
				OnOption(pOptionResponse->m_vtParams, pOptionResponse->m_vtResult, CComVariant(pOptionResponse->m_bIsLast));
		}
		break;
	case _enResponseFuture:
		{
			CFutureResultResponse* pFutureResponse = dynamic_cast<CFutureResultResponse*>(pResponse.get());
			if(pFutureResponse)
				OnFuture(pFutureResponse->m_vtParams, pFutureResponse->m_vtResult);
		}
		break;
	case _enResponseFutures:
		{
			CFuturesResultResponse* pFuturesResponse = dynamic_cast<CFuturesResultResponse*>(pResponse.get());
			if(pFuturesResponse)
				OnFutureByRoot(pFuturesResponse->m_vtParams, pFuturesResponse->m_vtResult, CComVariant(pFuturesResponse->m_bIsLast));
		}
		break;
	case _enResponseFuturesOption:
		{
			CFutureOptionResultResponse* pFutureOptionResponse = dynamic_cast<CFutureOptionResultResponse*>(pResponse.get());
			if(pFutureOptionResponse)
				OnFuturesOption(pFutureOptionResponse->m_vtParams, pFutureOptionResponse->m_vtResult, CComVariant(pFutureOptionResponse->m_bIsLast));
		}
		break;
	}
}

void CHyperFeedStructureProvider::OnRequest(CRequestBasePtr pRequest)
{
	switch(pRequest->m_enType)
	{
	case _enRequestStock:
		{
			CStockRequest* pStockRequest = dynamic_cast<CStockRequest*>(pRequest.get());
			if(pStockRequest)
				GetStockResults(pStockRequest->m_recParams);
		}
		break;
	case _enRequestOption:
		{
			COptionRequest* pOptionRequest = dynamic_cast<COptionRequest*>(pRequest.get());
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
			CFutureRequest* pFutureRequest = dynamic_cast<CFutureRequest*>(pRequest.get());
			if(pFutureRequest)
				GetFutureResultsBySymbol(pFutureRequest->m_recParams, false);
		}
		break;
	case _enRequestFutures:
		{
			CFuturesRequest* pFuturesRequest = dynamic_cast<CFuturesRequest*>(pRequest.get());
			if(pFuturesRequest)
				GetFutureResultsByRootSymbol(pFuturesRequest->m_recParams, false);
		}
		break;
	case _enRequestFuturesOption:
		{
			CFuturesOptionRequest* pFuturesOptionRequest = dynamic_cast<CFuturesOptionRequest*>(pRequest.get());
			if(pFuturesOptionRequest)
			{
				pFuturesOptionRequest->m_recParams.CopyTo(m_vtCurrentRequest);
				long nResult = GetFutureInfo(pFuturesOptionRequest->m_recParams, false);
				if(nResult == DBA_ERR_NO_ERROR) // Sharky: try to resolve FT_CQ00001489
				{
					QuoteUpdateParams Params;
					Params.Symbol = ::SysAllocString(pFuturesOptionRequest->m_recParams->FutureSymbol);
					Params.Type = enFUT;
					Params.Exchange = NULL;

					CQuoteRequestPtr pQuoteRequest = CQuoteRequestPtr(new CQuoteRequest(_enRequestFuturesOption, &Params));
					nResult = GetPrice(boost::shared_dynamic_cast<CRequestBase>(pQuoteRequest), false);
					::SysFreeString(Params.Symbol);
					
					if(nResult == DBA_ERR_NO_ERROR) // Sharky: try to resolve FT_CQ00001489
					{
						GetFutureOptionsResults(m_clCurrentFuture, false);
					}
				}
				
				if(nResult != DBA_ERR_NO_ERROR) // Sharky: try to resolve FT_CQ00001489
				{
					if(nResult == DBA_TERMINATED)
					{
						PublicGetFutureOptionError(enInternalError, _bstr_t(L"Operation was canceled by user"));
					}
					if(nResult == DBA_ERR_KEY_NOT_FOUND)
					{
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
						PublicGetFutureOptionError(enProviderInternalError, bs);
					}
				}
			}
		}
		break;
	}
}


//-----------------------------------------------------------------------------------------------//
HRESULT CHyperFeedStructureProvider::OnStock(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedStructureProvider::OnStock\n"));
	
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
HRESULT CHyperFeedStructureProvider::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CHyperFeedStructureProvider::OnOption\n"));
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
HRESULT CHyperFeedStructureProvider::OnFuture(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedStructureProvider::OnFuture\n"));

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
HRESULT CHyperFeedStructureProvider::OnFutureByRoot(const CComVariant &varParams,const CComVariant &varResults, const CComVariant &varIsLastFutures)
{
	ATLTRACE(_T("CHyperFeedStructureProvider::OnFutureByRoot\n"));

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
HRESULT CHyperFeedStructureProvider::OnFuturesOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CStructureProvider::OnFuturesOption\n"));
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

void CHyperFeedStructureProvider::PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription)
{
	CErrorResponse* pError = new CErrorResponse();
	CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);

	pError->m_enRequestType = enRequestOptions;
	pError->m_vtRequest = m_vtCurrentRequest;
	pError->m_bstrDescription = bsDescription;
	pError->m_Error = nError;
	PublicResponse(pErrorPtr);
}

void CHyperFeedStructureProvider::PublicLastOption()
{
	_OptionResults results;
	results.Init();
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*) new COptionResultResponse(m_vtCurrentRequest, results, true));
	PublicResponse(pOption);
}

void CHyperFeedStructureProvider::PublicOption(DBA_OPTIONS_FILTER_RECORD* pData)
{
	_OptionResults results;
	results.Init();
	CopyOptionFilterToResult(*pData, results);
	CopyFundamentalInfoToResult(results);
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new COptionResultResponse(m_vtCurrentRequest, results, false));
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

void CHyperFeedStructureProvider::PublicLastQuote(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec)
{
	try
	{
		_bstr_t bsFutSymbol(Params->Symbol);
		if(Params->Type == enFUT && !strncmp(bsFutSymbol, m_clCurrentFuture.m_sSymbol.c_str(), m_clCurrentFuture.m_sSymbol.length()))
		{
			m_clCurrentFuture.m_dFutureClosePrice = Price2Dbl(rec.priceType, rec.close.price);
		}
	}
	catch(_com_error)
	{
	}
}

void CHyperFeedStructureProvider::PublicGetFutureOptionError(ErrorNumberEnum nError, BSTR bsDescription)
{
	CErrorResponse* pError = new CErrorResponse();
	pError->m_enRequestType = enRequestFuturesOptions;
	pError->m_vtRequest = m_vtCurrentRequest;
	pError->m_bstrDescription = bsDescription;
	pError->m_Error = nError;
	CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
	PublicResponse(pErrorPtr);
}

void CHyperFeedStructureProvider::PublicLastFutureOption()
{
	_FuturesOptionResults results;
	results.Init();
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new CFutureOptionResultResponse(m_vtCurrentRequest, results, true));
	PublicResponse(pOption);
}

void CHyperFeedStructureProvider::PublicFutureOption(_FuturesOptionResults& results)
{
	CopyFutureFundamentalInfoToResult(results);
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new CFutureOptionResultResponse(m_vtCurrentRequest, results, false));
	PublicResponse(pOption);
	if(results->Series)
	{
		SysFreeString(results->Series);
		results->Series = NULL;
	}
}
