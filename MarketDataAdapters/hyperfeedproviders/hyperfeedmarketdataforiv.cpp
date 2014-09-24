// HyperFeedMarketDataForIv.cpp : Implementation of CHyperFeedMarketDataForIv

#include "stdafx.h"
#include "HyperFeedMarketDataForIv.h"

STDMETHODIMP CHyperFeedMarketDataForIv::RequestData(short RequestDataTypes)
{
	ATLTRACE(_T("CHyperFeedMarketDataForIv::RequestData\n"));
	try
	{
		if (!RequestDataTypes)
		{
			HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, ERROR_INVALID_PARAMETER);
			EgLib::CComErrorWrapper::ThrowError(hr, _T("There are no one valid data request defined."));
		}
		m_RequestDataTypes = RequestDataTypes;
		
		CRequestBasePtr  pRequest = CRequestBasePtr((CRequestBase*)new CDataForIvRequest()); 
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedMarketDataForIv::CancelRequest()
{
	ATLTRACE(_T("CHyperFeedMarketDataForIv::CancelRequest\n"));
	try
	{
		Cancel();
		CMasterOptions::CancelRequest();
		//COptions::CancelRequest();
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

void CHyperFeedMarketDataForIv::OnResponse(CResponseBasePtr pResponse)
{
	switch(pResponse->m_enType)
	{
	case _enResponseOption:
		{
			COptionForIvResponse* pOptionResponse = dynamic_cast<COptionForIvResponse*>(pResponse.get());
			if(pOptionResponse)
				OnOption(pOptionResponse->m_vtResult, CComVariant(pOptionResponse->m_bIsLast));
		}
		break;	
	case _enResponseStock:
		{
			CStockForIvResponse* pStockResponse = dynamic_cast<CStockForIvResponse*>(pResponse.get());
			if(pStockResponse)
				OnStock(pStockResponse->m_vtResult);
		}
		break;	
	}
}

void CHyperFeedMarketDataForIv::OnRequest(CRequestBasePtr pRequest)
{
	switch(pRequest->m_enType)
	{
	case _enRequestOption:
		{
			CDataForIvRequest* pDataRequest = dynamic_cast<CDataForIvRequest*>(pRequest.get());
			if(pDataRequest)
				EnumOptionableStocks();
		}
		break;
	}
}


STDMETHODIMP CHyperFeedMarketDataForIv::Connect()
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

STDMETHODIMP CHyperFeedMarketDataForIv::Disconnect()
{
	Cancel();
	CMasterOptions::CancelRequest();
	//COptions::CancelRequest();
	DisconnectProvider();
	return S_OK;
}

long CHyperFeedMarketDataForIv::Attach()
{
	long nRes = CMasterOptions::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		Sleep(500);
		nRes = CMasterOptions::Attach();
		if(nRes != DBA_ERR_NO_ERROR)
		{
			HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
			EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to master options database."));
		}
	}
	/*nRes = COptions::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price database."));
	}*/

	nRes = CSecurityProfile::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to security profile database."));
	}

	nRes = CPrice::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price database."));
	}

	return nRes ;
}
long CHyperFeedMarketDataForIv::Detach()
{
	long nRes = CMasterOptions::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from master options database."));
	}
	/*nRes = COptions::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price database."));
	}*/
	nRes = CSecurityProfile::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from security profile database."));
	}
	nRes = CPrice::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price database."));
	}
	return nRes ;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CHyperFeedMarketDataForIv::OnOption(const CComVariant& varResults,const CComVariant& varIsLastOption)
{
	ATLTRACE(_T("CHyperFeedMarketDataForIv::OnOption\n"));
	HRESULT  hr = S_OK ;
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
			pvars[1] = varResults;
			pvars[0] = varIsLastOption;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(DISPID_HfMeOnOption, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CHyperFeedMarketDataForIv::OnStock(const CComVariant& varResults)
{
	ATLTRACE(_T("CHyperFeedMarketDataForIv::OnStock\n"));
	HRESULT  hr = S_OK ;
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
			pvars[0] = varResults;
			DISPPARAMS disp = { pvars, NULL, 1, 0 };
			hr = pDispatch->Invoke(DISPID_HfMeOnStock, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
void CHyperFeedMarketDataForIv::PublicGetMasterOptionError(ErrorNumberEnum nError, BSTR bsDescription)
{
	CErrorResponse* pError = new CErrorResponse();
	pError->m_enRequestType = enRequestStock;
	pError->m_bstrDescription = bsDescription;
	pError->m_Error = nError;

	CResponseBasePtr pErrorPtr = CResponseBasePtr(pError);
	PublicResponse(pErrorPtr);
}

void CHyperFeedMarketDataForIv::PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription)
{
	CErrorResponse* pError = new CErrorResponse();
	pError->m_enRequestType = enRequestOptions;
	pError->m_bstrDescription = bsDescription;
	pError->m_Error = nError;
	pError->m_vtRequest = m_vtCurrentRequest;
	CResponseBasePtr pErrorPtr = CResponseBasePtr(pError);
	PublicResponse(pErrorPtr);
}

void CHyperFeedMarketDataForIv::PublicLastMasterOption()
{
	_OptionForIvResult results;
	results.Init();
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*) new COptionForIvResponse(results, true));
	PublicResponse(pOption);
}

/*void CHyperFeedMarketDataForIv::PublicOption(DBA_OPTIONS_FILTER_RECORD* pData)
{
	_OptionForIvResult results;
	results.Init();
	CopyOptionFilterToIvResult(*pData, results);
	CopyFundamentalInfoToIvResult(results);

	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new COptionForIvResponse(results, false));
	PublicResponse(pOption);
	if(results->Ticker)
	{
		SysFreeString(results->Ticker);
		results->Ticker = NULL;
	}
	if(results->UnderlyingSymbol)
	{
		SysFreeString(results->UnderlyingSymbol);
		results->UnderlyingSymbol = NULL;
	}
	if(results->Exchanges)
	{
		SysFreeString(results->Exchanges);
		results->Exchanges = NULL;
	}
}*/

void CHyperFeedMarketDataForIv::OnSecurityInfo(const _QuoteUpdateParams& Params, const DBA_RECORD_52& rec)
{
	_bstr_t bsSymbol(Params->Symbol);
	string sSymbol(bsSymbol);

	switch(Params->Type)
	{
	case enOPT:
		{
			string sKey(sSymbol.substr(0, sSymbol.length() - 1));
			if (sKey[sKey.length() - 1] > 'L')
				sKey[sKey.length() - 1] -= 12;

			CSecurityRequestMap::iterator iter = m_SecurityRequestMap.find(sKey);
			if (iter != m_SecurityRequestMap.end())
			{
				iter->second.dtExpiration =  EgLib::vt_date(
					rec.vehicle.maturityDate.century*100 + 
					rec.vehicle.maturityDate.year, 
					rec.vehicle.maturityDate.month, 
					rec.vehicle.maturityDate.day);
			}
		} 
		break;
	}
}

void CHyperFeedMarketDataForIv::_RetrieveSecuritiesInfo()
{
	CSecurityRequestMap::iterator	iter;

	_QuoteUpdateParams Params;
	Params.Init();
	Params->Type = enMStart;
	CRequestBasePtr pRequestStart = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestOption, Params));
	RetrieveSecuritiesInfo(pRequestStart);

	for (iter = m_SecurityRequestMap.begin(); iter != m_SecurityRequestMap.end(); iter++)
	{
		const sec_info_t &info = iter->second;
	
		_QuoteUpdateParams ParamsOPT;
		ParamsOPT.Init();
		ParamsOPT->Symbol = ::SysAllocString(info.bsSymb);
		ParamsOPT->Type = enOPT;

		CRequestBasePtr pRequestOPT = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestOption, ParamsOPT));
		RetrieveSecuritiesInfo(pRequestOPT);

		::SysFreeString(ParamsOPT->Symbol);
		ParamsOPT->Symbol = NULL;
	}

	_QuoteUpdateParams ParamsStop;
	ParamsStop.Init();
	ParamsStop->Type = enMStop;
	CRequestBasePtr pRequestStop = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestOption, ParamsStop));
	RetrieveSecuritiesInfo(pRequestStop);
}

void CHyperFeedMarketDataForIv::_CopySecurityInfoToIvResult(_OptionForIvResult& Result)
{
	_bstr_t bsSymbol(Result->Ticker);
	string sSymbol(bsSymbol);
	string sRootPlusExpirationCode(sSymbol.substr(0, sSymbol.length() - 1));

	int  iLen   = sRootPlusExpirationCode.length();
	CHAR &chExp = sRootPlusExpirationCode[iLen - 1];
	if (chExp > 'L')
		chExp -= 12;

	CSecurityRequestMap::const_iterator iter = m_SecurityRequestMap.find(sRootPlusExpirationCode);
	if (iter != m_SecurityRequestMap.end())
	{
		const sec_info_t &info = iter->second;
		Result->ExpirationDate  = info.dtExpiration;		
	}
	else
	{
		Result->ExpirationDate  = 0; //_CreateExpiryDate		
	}
}

void CHyperFeedMarketDataForIv::PublishUnderlyingLoaded(BSTR bsUnderlying)
{
	if (NULL == bsUnderlying)
	{
        PublicLastMasterOption();
		return;
	}

	if(!IsCanceled() && m_RequestDataTypes)
	{
		QuoteUpdateParams Params;
		Params.Symbol = ::SysAllocString(bsUnderlying);

		if (m_RequestDataTypes & enIvReqEODSO)
		{
			Params.Type = enSTK;
			Params.Exchange = NULL;

			CQuoteRequestPtr pQuoteRequest = CQuoteRequestPtr(new CQuoteRequest(_enRequestStock, &Params));
			GetPrice(boost::shared_dynamic_cast<CRequestBase>(pQuoteRequest));
			::SysFreeString(Params.Symbol);
		}
		else
		{
			_PublicLastQuoteSymbolOnly(&Params);
		}

		if (m_RequestDataTypes & (enIvReqSODO | enIvReqEODO))
		{
			m_OptList.clear();
			RetrieveTheoOptListForCurUnderlying(m_OptList);

			_QuoteUpdateParams Params;
			Params.Init();
			Params->Type = enMStart;
			CRequestBasePtr pRequestStart = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestOption, Params));
			GetPrice(pRequestStart, true, false);

			for (unsigned int i = 0; i < m_OptList.size(); ++i)
			{
				_QuoteUpdateParams ParamsOPT;
				ParamsOPT.Init();
				ParamsOPT->Symbol = ::SysAllocString(m_OptList[i]);
				ParamsOPT->Type = enOPT;

				CRequestBasePtr pRequestOPT = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestOption, ParamsOPT));
				GetPrice(pRequestOPT, true, false);
				::SysFreeString(ParamsOPT->Symbol);
				ParamsOPT->Symbol = NULL;
			}

			m_OptList.clear();
			m_OptionResults.clear();
			m_SecurityRequestMap.clear();

			_QuoteUpdateParams ParamsStop;
			ParamsStop.Init();
			ParamsStop->Type = enMStop;
			CRequestBasePtr pRequestStop = CRequestBasePtr((CRequestBase*)new CQuoteRequest(_enRequestOption, ParamsStop));
			GetPrice(pRequestStop, false, false);

			if (m_RequestDataTypes & enIvReqSODO)
				_RetrieveSecuritiesInfo();

			for (unsigned int i = 0; i < m_OptionResults.size(); ++i)
			{
				_OptionForIvResult &results = m_OptionResults[i];

				if (m_RequestDataTypes & enIvReqSODO)
					_CopySecurityInfoToIvResult(results);

				CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new COptionForIvResponse(results, false));
				PublicResponse(pOption);

				if(results->Ticker)
				{
				SysFreeString(results->Ticker);
				results->Ticker = NULL;
				}
				if(results->UnderlyingSymbol)
				{
				SysFreeString(results->UnderlyingSymbol);
				results->UnderlyingSymbol = NULL;
				}
				if(results->Exchanges)
				{
				SysFreeString(results->Exchanges);
				results->Exchanges = NULL;
				}
			}
			
			m_OptionResults.clear();
			m_SecurityRequestMap.clear();
		}
	}
}

void CHyperFeedMarketDataForIv::PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData)
{

	switch((*pParams)->Type)
	{
	case enSTK:
		{
			_StockForIvResult results;
			results.Init();
			results->Symbol = ::SysAllocString((*pParams)->Symbol);
			CopyExtendedToIvResult(pData.get(), results);

			CResponseBasePtr pResponse = CResponseBasePtr((CResponseBase*)new CStockForIvResponse(results));
			PublicResponse(pResponse);
			::SysFreeString(results->Symbol);
			results->Symbol = NULL;

		}break;
	case enOPT:
		{
			_OptionForIvResult results;
			results.Init();
			results->Ticker = ::SysAllocString((*pParams)->Symbol);
			results->UnderlyingSymbol = _bstr_t(m_clCurrentUnderlying.m_sSymbol.c_str()).copy();

			if (m_RequestDataTypes & enIvReqEODO)
			{
				_CopyOptionExtendedToIvResult(*pData, results);
			}
			if (m_RequestDataTypes & enIvReqSODO)
			{
				CopyFundamentalInfoToIvResult(results);
			}


			_bstr_t bsSymb = results->Ticker;
			string sSymb = bsSymb;
			string sRootPlusExp = sSymb.substr(0, sSymb.length() - 1);
			char &chExpiration = sRootPlusExp[sRootPlusExp.length() - 1];
			if (chExpiration > 'L')
				chExpiration -= 12;

			CSecurityRequestMap::iterator iter = m_SecurityRequestMap.find(sRootPlusExp);
			if (iter == m_SecurityRequestMap.end())
			{
				sec_info_t info;
				info.bsSymb = bsSymb;
				m_SecurityRequestMap.insert(CSecurityRequestMap::value_type(sRootPlusExp, info));
			}

			m_OptionResults.push_back(results);

			/*CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new COptionForIvResponse(results, false));
			PublicResponse(pOption);
			if(results->Ticker)
			{
			SysFreeString(results->Ticker);
			results->Ticker = NULL;
			}
			if(results->UnderlyingSymbol)
			{
			SysFreeString(results->UnderlyingSymbol);
			results->UnderlyingSymbol = NULL;
			}
			if(results->Exchanges)
			{
			SysFreeString(results->Exchanges);
			results->Exchanges = NULL;
			}*/
		}break;
	}
}

void CHyperFeedMarketDataForIv::_PublicLastQuoteSymbolOnly(const QuoteUpdateParams* Params)
{
	_StockForIvResult results;
	results.Init();
	results->Symbol = Params->Symbol;

	CResponseBasePtr pResponse = CResponseBasePtr((CResponseBase*)new CStockForIvResponse(results));
	PublicResponse(pResponse);
	results->Symbol = NULL;
}

void CHyperFeedMarketDataForIv::_CopyOptionExtendedToIvResult(const DBA_RECORD_3& rec, _OptionForIvResult& Result)
{
	/*char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)rec.item.symbol, MAX_TICKER);
	RemoveSpaceFromName(szName);
	_bstr_t bsSymbol(szName);*/

	Result->BestBid = Price2Dbl(rec.priceType, rec.bid.price);
	Result->BestBidExchange = static_cast<HfUsaOptionsExchanges>(rec.bid.exchangeCode[0]);
	Result->BestBidSize = rec.bid.size;

	Result->BestAsk = Price2Dbl(rec.priceType, rec.ask.price);
	Result->BestAskExchange = static_cast<HfUsaOptionsExchanges>(rec.ask.exchangeCode[0]);
	Result->BestAskSize = rec.ask.size;

	Result->LastSale = Price2Dbl(rec.priceType, rec.lastSale.price);
	Result->LastSaleExchange = static_cast<HfUsaOptionsExchanges>(rec.lastSale.exchangeCode[0]);
	Result->LastSaleSize = rec.lastSale.size;

	Result->Volume = rec.volume;
	Result->OpenInterest = rec.openInterest;
}
