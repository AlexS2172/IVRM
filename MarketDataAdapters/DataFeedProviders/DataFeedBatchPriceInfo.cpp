// DataFeedBatchPriceInfo.cpp : Implementation of CDataFeedBatchPriceInfo

#include "stdafx.h"
#include "DataFeedBatchPriceInfo.h"
#include "CommohFunctions.h"
#include <PriceProviders_i.c>

//------------------------------------------------------------------------------------------------//
// CDataFeedBatchPriceInfo
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&__uuidof(IPriceProvider)
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::RequestLastQuote(QuoteUpdateParams *Params)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::CancelLastQuote(VARIANT Params)
{
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	
	if (Params == NULL) 
		return E_POINTER;

	if (m_gateway){
		CRequestKeyPtr spRequest = m_gateway->ConvertRequest(Params);
		m_gateway->AddTask(new CSubscribeSingle(m_gateway, spRequest, enQuotesResponse));
	};
	
	return S_OK;

};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::SubscribeRisks(ITicker *Params)
{

	if (Params == NULL) 
		return E_POINTER;

	if (m_gateway){
		CRequestKeyPtr spRequest = m_gateway->ConvertRequest(Params);
		m_gateway->AddTask(new CSubscribeSingle(m_gateway, spRequest, enRisksResponse));
	};

	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::Connect()
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::Disconnect()
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
// IBatchPriceProvider
STDMETHODIMP CDataFeedBatchPriceInfo::get_IsQuotesUpdated(VARIANT_BOOL *pVal)
{

	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::GetRisksUpdates(SAFEARRAY** pRetVal)
{
	HRESULT hrStatus = S_OK;

	if (!pRetVal){
		return E_POINTER;
	}
	else{
		::SafeArrayDestroy(*pRetVal);
		*pRetVal = NULL;
	}
	//-----------------------------------------------------//
	//Assemble message
	if (m_gateway)
	{
		CResponseVector response;
		m_gateway->GetUpdates(enRisksResponse, response);

		_RisksUpdateFullInfo saResponse;
		saResponse.Init();

		long lResponseSize = static_cast<long>(response.size());
		if (lResponseSize)
		{

			SAFEARRAYBOUND saBound = {lResponseSize, 0L};
			LPSAFEARRAY lpSaResponse = ::SafeArrayCreateEx(VT_RECORD, 1, &saBound, (PVOID)saResponse.GetRecordInfo());

			if (lpSaResponse)
			{
				IRisksFullInfo* pvResponseAtom = NULL;
				if(SUCCEEDED(hrStatus = ::SafeArrayAccessData(lpSaResponse, (void**)&pvResponseAtom)))
				{
					memset(pvResponseAtom, 0L, lResponseSize * sizeof( IRisksFullInfo ));

					CResponseVector::iterator it = response.begin();
					CResponseVector::iterator itEnd = response.end();

					for (long lCounter = 0; it != itEnd; it++, lCounter++)
					{
						m_gateway->Copy((*it)->m_spRequestKey, pvResponseAtom[lCounter].Ticker);
						m_gateway->Copy((*it), pvResponseAtom[lCounter].Risks);												
					}
					::SafeArrayUnaccessData(lpSaResponse);
					*pRetVal = lpSaResponse;
				}
				else{
					hrStatus = HRESULT_FROM_WIN32(GetLastError());
				}
			}
			else{
				hrStatus = HRESULT_FROM_WIN32(GetLastError());
			}
		}
		response.clear();
	};

	return hrStatus;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::GetQuotesUpdates(SAFEARRAY** pRetVal)
{
	HRESULT hrStatus = S_OK;

	if (!pRetVal){
		return E_POINTER;
	}
	else{
		::SafeArrayDestroy(*pRetVal);
		*pRetVal = NULL;
	}
	//-----------------------------------------------------//
	//Assemble message
	if (m_gateway)
	{
		CResponseVector response;
		m_gateway->GetUpdates(enQuotesResponse, response);
		
		_QuoteUpdateFullInfo saResponse;
		saResponse.Init();

		long lResponseSize = static_cast<long>(response.size());
		if (lResponseSize)
		{

			SAFEARRAYBOUND saBound = {lResponseSize, 0L};
			LPSAFEARRAY lpSaResponse = ::SafeArrayCreateEx(VT_RECORD, 1, &saBound, (PVOID)saResponse.GetRecordInfo());

			if (lpSaResponse)
			{
				QuoteUpdateFullInfo* pvResponseAtom = NULL;
				if(SUCCEEDED(hrStatus = ::SafeArrayAccessData(lpSaResponse, (void**)&pvResponseAtom)))
				{
					memset(pvResponseAtom, 0L, lResponseSize * sizeof( QuoteUpdateFullInfo ));
					
					CResponseVector::iterator it = response.begin();
					CResponseVector::iterator itEnd = response.end();

					for (long lCounter = 0; it != itEnd; it++, lCounter++)
					{
						m_gateway->Copy((*it)->m_spRequestKey, pvResponseAtom[lCounter].Params);
						m_gateway->Copy((*it), pvResponseAtom[lCounter].Info);												
					}
					::SafeArrayUnaccessData(lpSaResponse);
					*pRetVal = lpSaResponse;
				}
				else{
					hrStatus = HRESULT_FROM_WIN32(GetLastError());
				}
			}
			else{
				hrStatus = HRESULT_FROM_WIN32(GetLastError());
			}
		}
		response.clear();
	};

	return hrStatus;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::RequestMultipleQuotes(SAFEARRAY** Params)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::SubscribeMultipleQuotes(SAFEARRAY** Params)
{
	HRESULT hrStatus = S_OK;

	if (Params == NULL) 
		return E_POINTER;
	
	try {
	
		if (m_gateway) {
			
			TRACE_INFO(_T("enter: [%s]"), _T("SubscribeMultipleQuotes"));
			
			QuoteUpdateParams* pvData = NULL;
			
			if( SUCCEEDED(hrStatus = ::SafeArrayAccessData(*Params, (void**)&pvData))) {
						
				long array_size = (*Params)->rgsabound[0].cElements;
				
				if(array_size) {
					
					CRequestCollPtr spRequest = CRequestCollPtr(new CRequestColl);
					
					for(int i = 0; i < array_size; i++) {
					
						CRequestKeyPtr spRequestAtom = m_gateway->ConvertRequest(&pvData[i]);
						
						spRequest->push_back(spRequestAtom);
						
						TRACE_INFO(_T("subscribtion request for: [%s]"), spRequestAtom->Symbol.c_str());
					}			
					
					m_gateway->AddTask(new CSubscribeMultiple(m_gateway, spRequest, enQuotesResponse));
				}
				::SafeArrayUnaccessData(*Params);
			}
		}
	}
	catch(_com_error& err) {
		TRACE_COM_ERROR(err);
	}
	catch (...) {
		TRACE_UNKNOWN_ERROR();
	}
	return hrStatus;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::UnNotifiedSubscribtionQuantity(VARIANT_BOOL bSkipNotification,  LONG* pVal)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::SetRegionRate(BSTR bstrRegion, DOUBLE dRate)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
//IUserInformation
STDMETHODIMP CDataFeedBatchPriceInfo::put_UserName(BSTR userName)
{
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::get_InProc(VARIANT_BOOL* bMode)
{
	if(bMode)
		*bMode = VARIANT_TRUE;
	return S_OK;
}
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::RequestDividends(ITicker *Params, DividendsFullInfo* dividends)
{
	//EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request dividends call");
	//if (!Params) return S_FALSE;
	//HRESULT hr = S_OK;
	//try
	//{
	//	try
	//	{
	//		CORBA::Any* anyMsg_base = m_dataManager->GetMarketData((char*)_bstr_t(Params->Symbol),
	//																DDS::FIELD_STOCK_SYMBOL,
	//																DDS::T_EQUITY_BASE,
	//																CFieldCollections::DividendsFields);
	//		DDS::IVData* ivData;
	//		if (anyMsg_base && (*anyMsg_base >>= ivData))
	//		{
	//			VS::CVSMessageParser	msgParser;
	//			std::auto_ptr<VS::CResponseBlockList> responseBlockList = msgParser.DeserializeMessage(ivData->get_buffer(), ivData->length());
	//			if (!responseBlockList->IsEmpty())
	//			{
	//				std::cout << "on parsed message " << std::endl;
	//				for (VS::CResponseBlockList::const_iterator iter = responseBlockList->Begin(); iter!= responseBlockList->End(); ++iter)
	//				{
	//					const VS::CResponseBlock* responseBlock = *iter;
	//					long messageType = static_cast<long>(responseBlock->GetMessageType());
	//					for (VS::CResponseBlock::const_iterator recordIter = responseBlock->Begin(); recordIter != responseBlock->End(); ++recordIter)
	//					{
	//						const VS::CResponseRecord& record = *recordIter;
	//						dividends->Ticker.Symbol = _bstr_t(Params->Symbol).copy();
	//						dividends->Ticker.Type = Params->Type;
	//						dividends->Ticker.Exchange = _bstr_t(Params->Exchange).copy();
	//						std::string symbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	//						dividends->DividendsInfo.Market.Freq		= record.GetField(DDS::FIELD_STOCK_DIV_FREQ).AsShort();
	//						dividends->DividendsInfo.Market.Amount	= record.GetField(DDS::FIELD_STOCK_DIV_AMT_YIELD).AsFloat();
	//						dividends->DividendsInfo.Market.Date		= record.GetField(DDS::FIELD_STOCK_DIV_DATE).AsDateTime();
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			if (anyMsg_base)
	//				delete anyMsg_base;
	//			return S_FALSE;
	//		}
	//		if (anyMsg_base)
	//			delete anyMsg_base;
	//	}
	//	catch(CORBA::SystemException& e)
	//	{
	//		std::string err_msg = "CORBA System Exception: ";
	//		GetCORBAError(e, err_msg);
	//		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//		return S_FALSE;
	//	}
	//	catch(CORBA::Exception& e)
	//	{
	//		std::string err_msg = "CORBA Exception: ";
	//		GetCORBAError(e, err_msg);
	//		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//		return S_FALSE;
	//	}
	//	catch(omniORB::fatalException& e)
	//	{
	//		std::string err_msg = "omniORB Fatal Exception: ";
	//		err_msg += e.errmsg();
	//		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//		return S_FALSE;
	//	}
	//	catch(...)
	//	{
	//		std::string err_msg = "Unknown error";
	//		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//		return S_FALSE;
	//	}
	//}
	//catch(CORBA::SystemException& e)
	//{
	//	hr = S_FALSE;
	//	std::string err_msg = "CORBA System Exception: ";
	//	GetCORBAError(e, err_msg);
	//	EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//	return S_FALSE;
	//}
	//catch(CORBA::Exception& e)
	//{
	//	hr = S_FALSE;
	//	std::string err_msg = "CORBA Exception: ";
	//	GetCORBAError(e, err_msg);
	//	EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//	return S_FALSE;
	//}
	//catch(omniORB::fatalException& e)
	//{
	//	hr = S_FALSE;
	//	std::string err_msg = "omniORB Fatal Exception: ";
	//	err_msg += e.errmsg();
	//	EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//	return S_FALSE;
	//}
	//catch(...)
	//{
	//	hr = S_FALSE;
	//	std::string err_msg = "Unknown error";
	//	EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str());
	//	return S_FALSE;
	//}
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::RequestRates(ITicker *Params, IRatesFullInfo* Rates)
{
	//EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request rates call");
	//if (!Params) return S_FALSE;
	//HRESULT hr = S_OK;
	////try
	//{
	//	MakeCopy(Rates->Ticker,*Params);
	//	//try
	//	{
	//		CORBA::Any* anyMsg_base = m_dataManager->GetMarketData((char*)_bstr_t(Params->Symbol),
	//																DDS::STOCK_SYMBOL,
	//																DDS::T_IR_DATA,
	//																CFieldCollections::RatesFields);
	//		DDS::IVData* ivData;
	//		if (anyMsg_base && (*anyMsg_base >>= ivData))
	//		{
	//			VS::CVSMessageParser	msgParser;
	//			std::auto_ptr<VS::CResponseBlockList> responseBlockList = msgParser.DeserializeMessage(ivData->get_buffer(), ivData->length());
	//			if (!responseBlockList->IsEmpty())
	//			{
	//				std::cout << "on parsed message " << std::endl;
	//				std::vector<IRatePtr> rates;
	//				for (VS::CResponseBlockList::const_iterator iter = responseBlockList->Begin(); iter!= responseBlockList->End(); ++iter)
	//				{
	//					const VS::CResponseBlock* responseBlock = *iter;
	//					long messageType = static_cast<long>(responseBlock->GetMessageType());
	//					for (VS::CResponseBlock::const_iterator recordIter = responseBlock->Begin(); recordIter != responseBlock->End(); ++recordIter)
	//					{
	//						const VS::CResponseRecord& record = *recordIter;
	//						IRatePtr rate(new IRate);
	//						rates.push_back(rate);
	//						rate->NeutralRate = rate->LongRate = rate->ShortRate = record.GetField(DDS::FIELD_INTEREST_RATE).AsFloat();
	//						rate->Period = 1;
	//					}
	//				}
	//				IRate* rateArr;
	//				_Rates fullinfoInit;
	//				fullinfoInit.Init();
	//				long size = static_cast<long>(rates.size());
	//				SAFEARRAYBOUND sab = { size, 0 };
	//				Rates->Curve = ::SafeArrayCreateEx( VT_RECORD, 1, &sab, (PVOID) fullinfoInit.GetRecordInfo() );
	//				if( SUCCEEDED(::SafeArrayAccessData( Rates->Curve, (void**)&rateArr) ))
	//				{
	//					try
	//					{
	//						memset(rateArr, 0, size * sizeof(IRate));
	//						int i =0;
	//						for (std::vector<IRatePtr>::const_iterator iter = rates.begin(); iter!= rates.end();++iter,i++)
	//						{
	//							rateArr[i].NeutralRate = rateArr[i].ShortRate = rateArr[i].LongRate = (*iter)->LongRate;
	//							rateArr[i].Period = (*iter)->Period;
	//						}
	//					}
	//					catch(...)
	//					{
	//						hr = S_FALSE;
	//					}
	//					::SafeArrayUnaccessData(Rates->Curve);
	//				}
	//				rates.clear();
	//			}
	//		}
	//		else
	//		{
	//			if (anyMsg_base)
	//				delete anyMsg_base;
	//			return S_FALSE;
	//		}
	//		if (anyMsg_base)
	//			delete anyMsg_base;
	//	}
	//}
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::RequestContracts(ITicker *Params, IContractsFullInfo* Contracts)
{
	//EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "Request rates call");
	//if (!Params) return S_FALSE;
	//HRESULT hr = S_OK;
	////try
	//{
	//	MakeCopy(Contracts->Ticker,*Params);
	//	//try
	//	{
	//		switch (Params->Type)
	//		{
	//		case (InstrumentTypeEnum)enOPT:
	//			{
	//				CORBA::Any* anyMsg_base = m_dataManager->GetMarketData((char*)_bstr_t(Params->Symbol),DDS::OPTION_SYMBOL,DFFields::T_OPTION_BASE,CFieldCollections::OptionBaseFields);
	//				DDS::IVData* ivData;
	//				if (anyMsg_base && (*anyMsg_base >>= ivData))
	//				{
	//					VS::CVSMessageParser	msgParser;
	//					std::auto_ptr<VS::CResponseBlockList> responseBlockList = msgParser.DeserializeMessage(ivData->get_buffer(), ivData->length());
	//					if (!responseBlockList->IsEmpty())
	//					{
	//						std::cout << "on parsed message " << std::endl;
	//						for (VS::CResponseBlockList::const_iterator iter = responseBlockList->Begin(); iter!= responseBlockList->End(); ++iter)
	//						{
	//							const VS::CResponseBlock* responseBlock = *iter;
	//							long messageType = static_cast<long>(responseBlock->GetMessageType());
	//							std::vector<IRatePtr> rates;
	//							for (VS::CResponseBlock::const_iterator recordIter = responseBlock->Begin(); recordIter != responseBlock->End(); ++recordIter)
	//							{
	//								const VS::CResponseRecord& record = *recordIter;
	//								Contracts->Data.BaseSymbol = _bstr_t(record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString().c_str()).Detach();
	//								Contracts->Data.BaseSymbolType = (InstrumentTypeEnum)enSTK;
	//								Contracts->Data.Symbol = _bstr_t(record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString().c_str()).Detach();
	//								Contracts->Data.SymbolType = (InstrumentTypeEnum)enOPT;
	//								Contracts->Data.OptionType = (record.GetField(DDS::FIELD_OPTION_TYPE).AsString().c_str()[0] == 'C')?0:1;
	//								Contracts->Data.OptionStyle = (record.GetField(DDS::FIELD_OPTION_STYLE).AsString().c_str()[0] == 'A')?0:1;
	//								Contracts->Data.Expiration = record.GetField(DDS::FIELD_EXPIRATION).AsDateTime();
	//								Contracts->Data.OpenInterest = record.GetField(DDS::FIELD_OPEN_INTEREST).AsFloat();
	//								Contracts->Data.OptionID = record.GetField(DDS::FIELD_OPTION_ID).AsInt();
	//								Contracts->Data.Strike = record.GetField(DDS::FIELD_STRIKE).AsFloat();
	//							}
	//						}
	//					}
	//				}
	//				else
	//				{
	//					if (anyMsg_base)
	//						delete anyMsg_base;
	//					return S_FALSE;
	//				}
	//				if (anyMsg_base)
	//					delete anyMsg_base;
	//				anyMsg_base = m_dataManager->GetMarketData((char*)_bstr_t(Contracts->Data.BaseSymbol),DDS::OPTION_SYMBOL,DFFields::T_EQUITY_BASE,CFieldCollections::EquityBaseFields);
	//				if (anyMsg_base && (*anyMsg_base >>= ivData))
	//				{
	//					VS::CVSMessageParser	msgParser;
	//					std::auto_ptr<VS::CResponseBlockList> responseBlockList = msgParser.DeserializeMessage(ivData->get_buffer(), ivData->length());
	//					if (!responseBlockList->IsEmpty())
	//					{
	//						std::cout << "on parsed message " << std::endl;
	//						for (VS::CResponseBlockList::const_iterator iter = responseBlockList->Begin(); iter!= responseBlockList->End(); ++iter)
	//						{
	//							const VS::CResponseBlock* responseBlock = *iter;
	//							long messageType = static_cast<long>(responseBlock->GetMessageType());
	//							std::vector<IRatePtr> rates;
	//							for (VS::CResponseBlock::const_iterator recordIter = responseBlock->Begin(); recordIter != responseBlock->End(); ++recordIter)
	//							{
	//								const VS::CResponseRecord& record = *recordIter;
	//								Contracts->Data.CompanyName = _bstr_t(record.GetField(DDS::FIELD_COMPANY_NAME).AsString().c_str()).Detach();
	//								Contracts->Data.StockID = record.GetField(DDS::FIELD_STOCK_ID).AsInt();
	//								Contracts->Data.StockType = (record.GetField(DDS::FIELD_STOCK_TYPE).AsString().c_str()[0] == 'S')?0:1;
	//								Contracts->Data.Currency = _bstr_t(record.GetField(DDS::FIELD_CURRENCY).AsString().c_str()).Detach();
	//							}
	//						}
	//					}
	//				}
	//				else
	//				{
	//					if (anyMsg_base)
	//						delete anyMsg_base;
	//					return S_FALSE;
	//				}
	//				if (anyMsg_base)
	//					delete anyMsg_base;
	//				break;
	//			}
	//		case (InstrumentTypeEnum)enSTK:
	//			{
	//				CORBA::Any* anyMsg_base = m_dataManager->GetMarketData((char*)_bstr_t(Params->Symbol),DDS::OPTION_SYMBOL,DFFields::T_EQUITY_BASE,CFieldCollections::EquityBaseFields);
	//				DDS::IVData* ivData;
	//				if (anyMsg_base && (*anyMsg_base >>= ivData))
	//				{
	//					VS::CVSMessageParser	msgParser;
	//					std::auto_ptr<VS::CResponseBlockList> responseBlockList = msgParser.DeserializeMessage(ivData->get_buffer(), ivData->length());

	//					if (!responseBlockList->IsEmpty())
	//					{
	//						std::cout << "on parsed message " << std::endl;
	//						for (VS::CResponseBlockList::const_iterator iter = responseBlockList->Begin(); iter!= responseBlockList->End(); ++iter)
	//						{
	//							const VS::CResponseBlock* responseBlock = *iter;
	//							long messageType = static_cast<long>(responseBlock->GetMessageType());
	//							std::vector<IRatePtr> rates;
	//							for (VS::CResponseBlock::const_iterator recordIter = responseBlock->Begin(); recordIter != responseBlock->End(); ++recordIter)
	//							{
	//								const VS::CResponseRecord& record = *recordIter;
	//								Contracts->Data.BaseSymbol = _bstr_t(Params->Symbol).copy();
	//								Contracts->Data.BaseSymbolType = (InstrumentTypeEnum)enSTK;
	//								Contracts->Data.CompanyName = _bstr_t(record.GetField(DDS::FIELD_COMPANY_NAME).AsString().c_str()).Detach();
	//								Contracts->Data.StockID = record.GetField(DDS::FIELD_STOCK_ID).AsInt();
	//								Contracts->Data.StockType = (record.GetField(DDS::FIELD_STOCK_TYPE).AsString().c_str()[0] == 'S')?0:1;
	//								Contracts->Data.Currency = _bstr_t(record.GetField(DDS::FIELD_CURRENCY).AsString().c_str()).Detach();
	//							}
	//						}
	//					}
	//				}
	//				else
	//				{
	//					if (anyMsg_base)
	//						delete anyMsg_base;
	//					return S_FALSE;
	//				}
	//				if (anyMsg_base)
	//					delete anyMsg_base;
	//				break;
	//			}
	//		}
	//	}
	//}
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
STDMETHODIMP CDataFeedBatchPriceInfo::GetContractsUpdate(SAFEARRAY** pRetVal)
{
	return S_OK;
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnQuoteUpdate()
{
	HRESULT hr = Fire_OnQuoteUpdate();
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnLastQuote()
{
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnSubscribed(CRequestKeyPtr& request)
{
	QuoteUpdateParams params;

	m_gateway->Copy(request, params);
	HRESULT hr = Fire_OnSubscribed(&params);

	::SysFreeString(params.Symbol);
	::SysFreeString(params.Exchange);
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnUnsubscribed(CRequestKeyPtr& request)
{
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnSubscribedRisks(CRequestKeyPtr& request)
{
	ITicker params;

	m_gateway->Copy(request, params);
	HRESULT hr = Fire_OnRisksSubscribed(&params);

	::SysFreeString(params.Symbol);
	::SysFreeString(params.Exchange);
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnUnsubscribedRisks(CRequestKeyPtr& request)
{
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnRiskUpdate()
{
	HRESULT hr = Fire_OnRisksUpdate();
};
//------------------------------------------------------------------------------------------------//
void	
CDataFeedBatchPriceInfo::OnError(CRequestKeyPtr& request, std::string& error, long code, long requestType)
{
	CComVariant varRequest;
	_bstr_t bsDescription(error.c_str());

	if (m_gateway)
		m_gateway->CopyRequestToVariant(request, varRequest);

	HRESULT hr = Fire_OnError(code, bsDescription, requestType, varRequest);
		
};
//------------------------------------------------------------------------------------------------//
