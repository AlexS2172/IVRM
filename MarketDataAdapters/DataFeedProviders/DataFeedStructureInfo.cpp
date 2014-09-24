// DataFeedStructureInfo.cpp : Implementation of CDataFeedStructureInfo

#include "stdafx.h"
#include "DataFeedStructureInfo.h"
#include "CommohFunctions.h"


// CDataFeedStructureInfo

STDMETHODIMP CDataFeedStructureInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&__uuidof(IStructureProvider)
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CDataFeedStructureInfo::CancelFuture(VARIANT Params)
{
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::CancelFutures(VARIANT Params)
{
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::CancelFuturesOptions(VARIANT Params)
{
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::CancelOptions(VARIANT Params)
{
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::CancelStock(VARIANT Params)
{
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::Connect()
{
	//if (m_isConnected) 
	//	return S_OK;
	//else
	//	return InitializeCORBA();
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::Disconnect()
{
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::RequestFuture(FutureParams *Params)
{
	EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request Future call");
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::RequestFutures(FutureRootParams *Params)
{
	EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request Futures call");
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::RequestFuturesOptions(FuturesOptionParams *Params)
{
	EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request Future Options call");
	return S_OK;
}

STDMETHODIMP CDataFeedStructureInfo::RequestDividends(ITicker *Params, DividendsFullInfo* dividends)
{
	/*EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request dividends call");
	if (!Params) return S_FALSE;
	HRESULT hr = S_OK;
	try
	{
		try
		{
			CORBA::Any* anyMsg_base = m_dataManager->GetMarketData((char*)_bstr_t(Params->Symbol),DDS::STOCK_SYMBOL,DDS::T_EQUITY_BASE,CFieldCollections::DividendsFields);
			DDS::IVData* ivData;
			if (anyMsg_base && (*anyMsg_base >>= ivData))
			{
				VS::CVSMessageParser	msgParser;
				std::auto_ptr<VS::CResponseBlockList> responseBlockList = msgParser.DeserializeMessage(ivData->get_buffer(), ivData->length());
				if (!responseBlockList->IsEmpty())
				{
					std::cout << "on parsed message " << std::endl;
					for (VS::CResponseBlockList::const_iterator iter = responseBlockList->Begin(); iter!= responseBlockList->End(); ++iter)
					{
						const VS::CResponseBlock* responseBlock = *iter;
						long messageType = static_cast<long>(responseBlock->GetMessageType());
						for (VS::CResponseBlock::const_iterator recordIter = responseBlock->Begin(); recordIter != responseBlock->End(); ++recordIter)
						{
							const VS::CResponseRecord& record = *recordIter;
							dividends->Ticker.Symbol = _bstr_t(Params->Symbol).copy();
							dividends->Ticker.Type = Params->Type;
							dividends->Ticker.Exchange = _bstr_t(Params->Exchange).copy();
							dividends->DividendsInfo.Market.Freq		= record.GetField(DDS::FIELD_STOCK_DIV_FREQ).AsShort();
							dividends->DividendsInfo.Market.Amount	= record.GetField(DDS::FIELD_STOCK_DIV_AMT_YIELD).AsFloat();
							dividends->DividendsInfo.Market.Date		= record.GetField(DDS::FIELD_STOCK_DIV_DATE).AsDateTime();
						}
					}
				}
			}
			else
			{
				if (anyMsg_base)
					delete anyMsg_base;
				return S_FALSE;
			}
			if (anyMsg_base)
				delete anyMsg_base;
		}
		catch(CORBA::SystemException& e)
		{
			std::string err_msg = "CORBA System Exception: ";
			GetCORBAError(e, err_msg);
			EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
			return S_FALSE;
		}
		catch(CORBA::Exception& e)
		{
			std::string err_msg = "CORBA Exception: ";
			GetCORBAError(e, err_msg);
			EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
			return S_FALSE;
		}
		catch(omniORB::fatalException& e)
		{
			std::string err_msg = "omniORB Fatal Exception: ";
			err_msg += e.errmsg();
			EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
			return S_FALSE;
		}
		catch(...)
		{
			std::string err_msg = "Unknown error";
			EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
			return S_FALSE;
		}
	}
	catch(CORBA::SystemException& e)
	{
		hr = S_FALSE;
		std::string err_msg = "CORBA System Exception: ";
		GetCORBAError(e, err_msg);
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		return S_FALSE;
	}
	catch(CORBA::Exception& e)
	{
		hr = S_FALSE;
		std::string err_msg = "CORBA Exception: ";
		GetCORBAError(e, err_msg);
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		return S_FALSE;
	}
	catch(omniORB::fatalException& e)
	{
		hr = S_FALSE;
		std::string err_msg = "omniORB Fatal Exception: ";
		err_msg += e.errmsg();
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		return S_FALSE;
	}
	catch(...)
	{
		hr = S_FALSE;
		std::string err_msg = "Unknown error";
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		return S_FALSE;
	}*/
	return S_OK;
}

STDMETHODIMP CDataFeedStructureInfo::RequestStock(StockParams *Params)
{
	/*EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request stock call");
	if (!Params) return S_FALSE;
	HRESULT hr = S_OK;
	try
	{
		vector<QuoteUpdateParams> reqParams;
		QuoteUpdateParams qt;
		qt.Symbol = _bstr_t(Params->Stock).copy();
		qt.Exchange = _bstr_t("").copy();
		qt.Type = (InstrumentTypeEnum)enSTK;
		reqParams.push_back(qt);
		CStockMsgWorker* pSender  = new CStockMsgWorker(m_dataManager, this, reqParams, (RequestType)enStockRequest);
		if(pSender)
		{
			if(!m_ThreadPool.QueueRequest((CPullThreadPool::RequestType)pSender))
			{
			}
		}
	}
	catch(CORBA::SystemException& e)
	{
		hr = S_FALSE;
		std::string err_msg = "CORBA System Exception: ";
		GetCORBAError(e, err_msg);
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());

		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->Stock).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestStock, vp);
	}
	catch(CORBA::Exception& e)
	{
		hr = S_FALSE;
		std::string err_msg = "CORBA Exception: ";
		GetCORBAError(e, err_msg);
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->Stock).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestStock, vp);
	}
	catch(omniORB::fatalException& e)
	{
		hr = S_FALSE;
		std::string err_msg = "omniORB Fatal Exception: ";
		err_msg += e.errmsg();
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->Stock).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestStock, vp);
	}
	catch(...)
	{
		hr = S_FALSE;
		std::string err_msg = "Unknown error";
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->Stock).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestStock, vp);
	}
	return hr;*/
	return S_OK;
}
STDMETHODIMP CDataFeedStructureInfo::RequestOptions(OptionParams *Params)
{
	/*EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request option call");
	if (!Params) return S_FALSE;
	HRESULT hr = S_OK;
	try
	{
		vector<QuoteUpdateParams> reqParams;
		QuoteUpdateParams qt;
		qt.Symbol = _bstr_t(Params->UnderlyingSymbol).copy();
		qt.Exchange = _bstr_t("").copy();
		qt.Type = (InstrumentTypeEnum)enOPT;
		reqParams.push_back(qt);
		COptionMsgWorker* pSender  = new COptionMsgWorker(m_dataManager, this, reqParams, (RequestType)enOptionRequest);
		if(pSender)
		{
			if(!m_ThreadPool.QueueRequest((CPullThreadPool::RequestType)pSender))
			{
			}
		}
	}
	catch(CORBA::SystemException& e)
	{
		hr = S_FALSE;
		std::string err_msg = "CORBA System Exception: ";
		GetCORBAError(e, err_msg);
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());

		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->UnderlyingSymbol).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestOptions, vp);
	}
	catch(CORBA::Exception& e)
	{
		hr = S_FALSE;
		std::string err_msg = "CORBA Exception: ";
		GetCORBAError(e, err_msg);
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->UnderlyingSymbol).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestOptions, vp);
	}
	catch(omniORB::fatalException& e)
	{
		hr = S_FALSE;
		std::string err_msg = "omniORB Fatal Exception: ";
		err_msg += e.errmsg();
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->UnderlyingSymbol).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestOptions, vp);
	}
	catch(...)
	{
		hr = S_FALSE;
		std::string err_msg = "Unknown error";
		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
		CComVariant vp;
		_QuoteUpdateParams rp;
		rp.Init();
		rp->Exchange = _bstr_t("").copy();
		rp->Symbol = _bstr_t(Params->UnderlyingSymbol).copy();
		rp->Type = (InstrumentTypeEnum)enSTK;
		rp.CopyTo(vp);
		Fire_OnError((ErrorNumberEnum)enInternalError,_bstr_t(err_msg.c_str()),(RequestsTypeEnum)enRequestOptions, vp);
	}*/
	return S_OK;
}

// IStructureProviderExEvents
//STDMETHODIMP CDataFeedStructureInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest)
//{
//	EgLib::CEgLibTraceManager::TraceText(LogDebug, __FUNCTION__ , _T("Enter"));
//	HRESULT hr = S_OK;
//	CComVariant varResult;
//
//	try
//	{
//
//		int nConnectionIndex;
//		CComVariant* pvars = new CComVariant[4];
//		int nConnections = m_vec.GetSize();
//
//
//		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
//		{
//			Lock();
//			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
//			Unlock();
//			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
//			if (pDispatch != NULL)
//			{
//				VariantClear(&varResult);
//				pvars[3] = enumError;
//				pvars[2] = (BSTR)bstrDescription;
//				pvars[1] = enumRequest;
//				pvars[0] = varRequest;
//				DISPPARAMS disp = { pvars, NULL, 4, 0 };
//				hr = pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
//			}
//		}
//		delete[] pvars;
//
//		if(FAILED(hr))
//		{
//			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Exit with error hr = %d, file: %s line: %d"),  hr, __FILE__, __LINE__);
//			hr = E_FAIL;
//		}
//
//	}
//	_CATCH_COM_EXCEPTION_RETURN_COM(L"On Error Failed")
//	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
//
//	EgLib::CEgLibTraceManager::TraceText(LogDebug, __FUNCTION__ , _T("Exit"));
//	if (hr == E_FAIL)
//		return E_FAIL;
//	else
//		return varResult.scode;
//}
//STDMETHODIMP CDataFeedStructureInfo::OnFuturesOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption)
//{
//	return S_OK;
//}
//STDMETHODIMP CDataFeedStructureInfo::OnFuture(const CComVariant &varParams, const CComVariant &varResults)
//{
//	CComVariant varResult;
//	int nConnectionIndex;
//	CComVariant* pvars = new CComVariant[2];
//	int nConnections = m_vec.GetSize();
//	HRESULT hr = S_OK;
//
//	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
//	{
//		Lock();
//		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
//		Unlock();
//		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
//		if (pDispatch != NULL)
//		{
//			VariantClear(&varResult);
//			pvars[1] = varParams;
//			pvars[0] = varResults;
//			DISPPARAMS disp = { pvars, NULL, 2, 0 };
//			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
//		}
//	}
//	delete[] pvars;
//
//	if(FAILED(hr)) return hr;
//	return varResult.scode;
//}
//STDMETHODIMP CDataFeedStructureInfo::OnFutureByRoot(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastFutures)
//{
//	CComVariant varResult;
//	int nConnectionIndex;
//	CComVariant* pvars = new CComVariant[3];
//	int nConnections = m_vec.GetSize();
//	HRESULT hr = S_OK;
//
//	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
//	{
//		Lock();
//		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
//		Unlock();
//		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
//		if (pDispatch != NULL)
//		{
//			VariantClear(&varResult);
//			pvars[2] = varParams;
//			pvars[1] = varResults;
//			pvars[0] = varIsLastFutures;			
//			DISPPARAMS disp = { pvars, NULL, 3, 0 };
//			hr = pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
//		}
//	}
//	delete[] pvars;
//
//	if(FAILED(hr)) return hr;
//	return varResult.scode;
//}
//STDMETHODIMP CDataFeedStructureInfo::OnStock(const CComVariant &varParams, const CComVariant &varResults)
//{
//	CComVariant varResult;
//	int nConnectionIndex;
//	CComVariant* pvars = new CComVariant[2];
//	int nConnections = m_vec.GetSize();
//	HRESULT hr = S_OK;
//
//	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
//	{
//		Lock();
//		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
//		Unlock();
//		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
//		if (pDispatch != NULL)
//		{
//			VariantClear(&varResult);
//			pvars[1] = varParams;
//			pvars[0] = varResults;
//			DISPPARAMS disp = { pvars, NULL, 2, 0 };
//			hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
//		}
//	}
//	delete[] pvars;
//
//	if(FAILED(hr)) return hr;
//	return varResult.scode;
//}
//STDMETHODIMP CDataFeedStructureInfo::OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption)
//{
//	HRESULT  hr = S_OK ;
//	CComVariant varResult;
//	int nConnectionIndex;
//	CComVariant* pvars = new CComVariant[3];
//	int nConnections = m_vec.GetSize();
//
//	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
//	{
//		Lock();
//		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
//		Unlock();
//		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
//		if (pDispatch != NULL)
//		{
//			VariantClear(&varResult);
//			pvars[2] = varParams;
//			pvars[1] = varResults;
//			pvars[0] = varIsLastOption;
//			DISPPARAMS disp = { pvars, NULL, 3, 0 };
//			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
//		}
//	}
//	delete[] pvars;
//
//	if(FAILED(hr)) return hr;
//	return varResult.scode;
//}

void CDataFeedStructureInfo::Call_OnOption()
{
	//if (!m_optionInfo.empty())
	//{
	//	OptionInfoMap::iterator iter = m_optionInfo.begin();
	//	CComVariant vp;
	//	CComVariant vi;
	//	_OptionUpdateInfo ri;
	//	_OptionUpdateParams rp;
	//	ri.Init();
	//	rp.Init();
	//	MakeCopy(ri,iter->second->Info);
	//	MakeCopy(rp,*((OptionParams*)&(iter->second->Params)));
	//	rp.CopyTo( vp );
	//	ri.CopyTo( vi );
	//	m_optionInfo.erase(iter);
	//	bool Last = m_optionInfo.size() == 0;
	//	Fire_OnOption((OptionParams*)&(iter->second->Params),&(iter->second->Info), Last);
	//}
};

void CDataFeedStructureInfo::Call_OnStock()
{
	//if (!m_stockInfo.empty())
	//{
	//	StockInfoMap::iterator iter = m_stockInfo.begin();
	//	CComVariant vp;
	//	CComVariant vi;
	//	_StockUpdateInfo ri;
	//	_StockUpdateParams rp;
	//	ri.Init();
	//	rp.Init();
	//	MakeCopy(ri,iter->second->Info);
	//	MakeCopy(rp,iter->second->Params);
	//	rp.CopyTo( vp );
	//	ri.CopyTo( vi );
	//	m_stockInfo.erase(iter);
	//	bool Last = m_stockInfo.size() == 0;
	//	Fire_OnStock(&(iter->second->Params),&(iter->second->Info));
	//}
};