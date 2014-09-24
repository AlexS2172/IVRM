#include "stdafx.h"
#include "DataFeedBaseClass.h"

std::string 
ToString(const CORBA::SystemException& e){
	CORBA::Any tmp;
	tmp <<= e;
	CORBA::TypeCode_var tc = tmp.type();
	return std::string(tc->name()) + " Description: " + e.NP_minorString();
};

std::string 
ToString(const CORBA::Exception & e){
	CORBA::Any tmp;
	tmp <<= e;
	CORBA::TypeCode_var tc = tmp.type();
	return std::string(tc->name());
};

std::string 
ToString(const omniORB::fatalException & e){
	return std::string(e.errmsg());
};

void CDataFeedBaseClass::GetCORBAError(const CORBA::Exception & e, std::string& msg)
{
	CORBA::Any tmp;
	tmp <<= e;
	CORBA::TypeCode_var tc = tmp.type();
	msg += std::string(tc->name());
	msg += _T(" code: ");
	msg += boost::lexical_cast<std::string>(tc->id());
}

void CDataFeedBaseClass::GetCORBAError(const CORBA::SystemException & e, std::string& msg)
{
	CORBA::Any tmp;
	tmp <<= e;
	CORBA::TypeCode_var tc = tmp.type();
	msg += std::string(tc->name()) + _T(" Description: ") + e.NP_minorString();
	msg += _T(" Minor code: ");
	msg += boost::lexical_cast<std::string>(e.minor());
}


void CDataFeedBaseClass::InsertEquityTypedUpdate(const VS::CResponseRecord& record, RequestMap* req_map, long type)
{
	switch (type)
	{
	case DDS::T_EQUITY_BASE:
		{
			CParseHelper::ParseEquityBaseRecord(record, req_map, &m_stockInfo);
			break;
		}
	case DDS::T_EQUITY_QUOTE:
		{
			CParseHelper::ParseEquityQuoteRecord(record, req_map, &m_stockInfo);
			break;
		}
	}
}
void CDataFeedBaseClass::InsertOptionTypedUpdate(const VS::CResponseRecord& record, RequestMap* req_map, long type)
{
	switch (type)
	{
	case DDS::T_OPTION_BASE:
		{
			CParseHelper::ParseOptionBaseRecord(record, req_map, &m_optionInfo);
			break;
		}
	case DDS::T_OPTION_QUOTE:
		{
			CParseHelper::ParseOptionQuoteRecord(record, req_map, &m_optionInfo);			
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------//
void CDataFeedBaseClass::CopyRequestToVariant(QuoteUpdateParams* request, CComVariant& varRequest)
{
	if (request == NULL) return;
	
	_QuoteUpdateParams __request;

	__request.Init();

	__request->Exchange = _bstr_t(request->Exchange, true).Detach();
	__request->Symbol = _bstr_t(request->Symbol, true).Detach();
	__request->Type = request->Type;

	__request.CopyTo(varRequest);
};
//-------------------------------------------------------------------------------------------------------//
bool CDataFeedBaseClass::IsBadRequest(QuoteUpdateParams* request)
{
	//now, exchange defined requests not supported
	std::string	error_message;
	bool	bBadRequestType = false;
	bool	bBadRequest = false;

	switch (request->Type)
	{
		case (InstrumentTypeEnum)enOPT:
		case (InstrumentTypeEnum)enIDX:
		case (InstrumentTypeEnum)enSTK:
		{
			bBadRequestType = false;
			break;
		}
		default:
			bBadRequestType = true;
	}
	
	if (bBadRequestType)
	{
		error_message = std::string("Contract of this type is not supported.");
		bBadRequest = true;
	}
	else if (request->Exchange && ::SysStringLen(request->Exchange) > 0)
	{
		error_message = std::string("Exchange defined request's not supported.");
		bBadRequest = true;
	}

	
	if (bBadRequest)
	{
		OnError(request, error_message, enRequestLastQuote);
	}
	return bBadRequest;
};
//-------------------------------------------------------------------------------------------------------//
void CDataFeedBaseClass::OnError(QuoteUpdateParams *request, std::string& error_message, RequestsTypeEnum requestType)
{
	CComVariant	varRequest;
	CDataFeedBaseClass::CopyRequestToVariant(request, varRequest);

	EgLib::CEgLibTraceManager::Trace(EgLib::LogError, __FUNCTION__, error_message.c_str());

	Call_OnError((ErrorNumberEnum)enSymbolNotSupported,
				_bstr_t(error_message.c_str()),
				(RequestsTypeEnum)requestType,
				varRequest);
};
//-------------------------------------------------------------------------------------------------------//

HRESULT CDataFeedBaseClass::InitializeCORBA()
{
	HRESULT hr = S_OK;
	int my_argc = 7;
	char** my_argv = new char*[my_argc];
	std::ostringstream appName;
	appName << CDataFeedSettings::GetProcessFilename();
	appName << _T(".exe");
	my_argv[0] = new char[appName.str().size()+1];
	my_argv[0][appName.str().size()] = '\0';
	strcpy(my_argv[0],appName.str().c_str());
	
	TRACEINFO(__FUNCTION__, "AppName: ");
	TRACEINFO(__FUNCTION__, my_argv[0]);
	my_argv[1] = "-ORBInitRef";
	my_argv[2] = const_cast<char*>(CDataFeedSettings::m_provider.c_str());
	my_argv[3] = "-ORBendPoint";
	my_argv[4] = const_cast<char*>(CDataFeedSettings::m_endpoint.c_str());
	my_argv[5] = "-ORBtraceLevel";
	my_argv[6] = const_cast<char*>(CDataFeedSettings::m_ORBTraceLevel.c_str());
	
	std::string err_msg;
	int initCalls = 0;
	std::string conn_info = "Connection info: ";
	conn_info +="Provider: " + CDataFeedSettings::m_provider;
	conn_info +="; RemoteObject: " + CDataFeedSettings::m_remoteObj;
	conn_info +="; UserName: " + CDataFeedSettings::m_userName;
	conn_info +="; Password: " + CDataFeedSettings::m_password;
	TRACEINFO(__FUNCTION__, "Initializing ORB");
	TRACEINFO(__FUNCTION__, conn_info.c_str());
	CORBA::ORB_var orb;
	
	hr = ORBInit::Initialize(	my_argc,
								my_argv,
								&orb,
								&m_SessionManager,
								CDataFeedSettings::m_serverTimeout,
								CDataFeedSettings::m_remoteObj,
								CDataFeedSettings::m_userName,
								CDataFeedSettings::m_password,
								&initCalls,
								CDataFeedSettings::m_attempts,
								err_msg);

	std::string SessionID = boost::lexical_cast<std::string>(static_cast<long>( GetTickCount() ));
	m_dataManager = m_SessionManager->ResolveSession(SessionID.c_str());

	std::ostringstream str;
	str << "Initcalls = ";
	str << initCalls;
	TRACEINFO(__FUNCTION__, str.str().c_str());
	if (hr==S_OK)
		TRACEINFO(__FUNCTION__, "ORB initizlized");
	else
		TRACEINFO(__FUNCTION__, err_msg.c_str());
	
	delete[] my_argv[0];
	delete[] my_argv;
	
	if (hr == S_OK)
	{
		m_dataManager->SetCompressionMode(CDataFeedSettings::m_compression);
		TRACEINFO(__FUNCTION__, "Compression mode was set");
		m_dataManager->SetProtocolVersion(1);
		TRACEINFO(__FUNCTION__, "Protocol version was set");
		m_dataManager->RegisterAllFields(DDS::T_EQUITY_QUOTE);
		TRACEINFO(__FUNCTION__, "Registered fields for Stocks");
		m_dataManager->RegisterAllFields(DDS::T_OPTION_QUOTE);
		TRACEINFO(__FUNCTION__, "Registered fields for Options");
		m_dataManager->RegisterAllFields(DFFields::T_IVRM_RISKS);
		TRACEINFO(__FUNCTION__, "Registered fields for Risks");
		TRACEINFO(__FUNCTION__, "Connection completed successfully");
		m_isConnected = true;
	} 
	else 
		m_isConnected = false;

	return hr;
}
//----------------------------------------------------------------------------------------//
CDataFeedBaseClass::CDataFeedBaseClass():CDataFeedGateway(NULL)
{
	//m_ID = ++ID;
};
//----------------------------------------------------------------------------------------//
int CDataFeedBaseClass::m_OptionsSubs;
int CDataFeedBaseClass::m_StocksSubs;
int CDataFeedBaseClass::m_RisksSubs;
int CDataFeedBaseClass::m_OnErrorCalled;
int CDataFeedBaseClass::m_OnLastQuoteCalled;
int CDataFeedBaseClass::ID = 0;
//----------------------------------------------------------------------------------------//
//CDataFeedGateway impl
//----------------------------------------------------------------------------------------//
HANDLE CStopEvent::hEvent = NULL;
//----------------------------------------------------------------------------------------//
CDataFeedGateway::CDataFeedGateway():CGateway(NULL)
{
};
//----------------------------------------------------------------------------------------//
CDataFeedGateway::CDataFeedGateway(CGatewayNotify *client):
CGateway(client)
{
	m_ThreadPool.Initialize(NULL, 1);
	m_hPullingThread = NULL;
	__isConnected = false;
};
//----------------------------------------------------------------------------------------//
CDataFeedGateway::~CDataFeedGateway()
{
	m_ThreadPool.Shutdown(5000L);
};
//----------------------------------------------------------------------------------------//
ResponseTypeEnum	
CDataFeedGateway::MessageTypeToResponse(long messageType)
{
	if (DFFields::T_IVRM_RISKS == messageType)
		return enRisksResponse;
	else if (DFFields::T_EQUITY_QUOTE == messageType)
		return enQuotesResponse;
	else if (DFFields::T_OPTION_QUOTE == messageType)
		return enQuotesResponse;
	else if (DFFields::T_FUTURE_QUOTE == messageType)
		return enQuotesResponse;
	else if (DFFields::T_FUTURE_OPTION_QUOTE == messageType)
		return enQuotesResponse;

	return enNoneResponse;
};
//----------------------------------------------------------------------------------------//
void CDataFeedGateway::ProcessDataFeedResponse(const VS::CResponseRecord &response, long messageType)
{
	ResponseTypeEnum enResponseType = MessageTypeToResponse(messageType);
	
	CRequestKeyPtr spRequest = CResponseParser::GetRequestKey(response, messageType);

	CResponsePtr spResponse;

	if (spRequest->Type == enSTK && !FindResponse(spRequest, enResponseType)){
		spRequest->Type = enIDX;
		if (!FindResponse(spRequest, enResponseType))
			spRequest->Type = enSTK;
	};

	if (GetResponse(spRequest, enResponseType, spResponse)){

		CResponseParser::ParseResponse(response, messageType, spResponse);

		spResponse->SetUpdated();
		
		CString strInfo;
		if (enRisksResponse == enResponseType){
			SetUpdateStatus(enRisksUpdates);

			CRisksResponse* risks = dynamic_cast<CRisksResponse*>(spResponse.get());
			strInfo.Format(_T("Symbol: %s IVol= %.2f TVol= %.2f Thv= %.2f"),
							spRequest->Symbol.c_str(),
							risks->IVola,
							risks->TheoVola,
							risks->TheoPrice);

			TRACE_INFO(_TEXT("Update Risks: [%s]"), strInfo);
		}
		else if (enQuotesResponse == enResponseType){
			SetUpdateStatus(enQuotesUpdates);

			CQuoteResponse* quote = dynamic_cast<CQuoteResponse*>(spResponse.get());
			strInfo.Format(_T("Symbol: %s Bid= %.2f Ask= %.2f Last= %.2f"),
							spRequest->Symbol.c_str(),
							quote->BidPrice,
							quote->AskPrice,
							quote->LastPrice);

			TRACE_INFO(_TEXT("Update Quotes: [%s]"), strInfo);
		}

		if (m_client){
			if (spResponse->GetUpdatesCount() == 1){
				if (spResponse->GetType() == enQuotesResponse)
					m_client->OnSubscribed(spRequest);
				else if (spResponse->GetType() == enRisksResponse)
					m_client->OnSubscribedRisks(spRequest);
			}
		};
	}
	else{
		TRACE_INFO(_TEXT("Source not found: [%s]"), spRequest->Symbol.c_str());
	};
};
//----------------------------------------------------------------------------------------//
bool	
CDataFeedGateway::IsBadRequest(CRequestKeyPtr& request)
{
	return false;
};
//----------------------------------------------------------------------------------------//
void 
CDataFeedGateway::CopyRequestToVariant(CRequestKeyPtr& spRequest, CComVariant& varRequest)
{
	if (spRequest) return;

	_QuoteUpdateParams __request;

	__request.Init();

	__request->Exchange = _com_util::ConvertStringToBSTR(spRequest->Exchange.c_str());
	__request->Symbol = _com_util::ConvertStringToBSTR(spRequest->Symbol.c_str());
	__request->Type = spRequest->Type;

	__request.CopyTo(varRequest);
};
//----------------------------------------------------------------------------------------//
void	
CDataFeedGateway::Copy(CRequestKeyPtr& spRequest, QuoteUpdateParams& request)
{
	request.Symbol		= _com_util::ConvertStringToBSTR(spRequest->Symbol.c_str());
	request.Type		= spRequest->Type;															
	request.Exchange	= _com_util::ConvertStringToBSTR(spRequest->Exchange.c_str());
};
//----------------------------------------------------------------------------------------//
void	
CDataFeedGateway::Copy(CRequestKeyPtr& spRequest, ITicker& request)
{
	request.Symbol		= _com_util::ConvertStringToBSTR(spRequest->Symbol.c_str());
	request.Type		= spRequest->Type;
	request.Exchange	= _com_util::ConvertStringToBSTR(spRequest->Exchange.c_str());
};
//----------------------------------------------------------------------------------------//
void	
CDataFeedGateway::Copy(CResponsePtr& spResponse, QuoteUpdateInfo& response)
{
	CQuoteResponse* ptrQuote = dynamic_cast<CQuoteResponse*>(spResponse.get());

	if (ptrQuote)
	{
		response.UpdateDateTime =		ptrQuote->UpdateDateTime;
		response.BidDateTime	=		ptrQuote->BidDateTime;
		response.AskDateTime	=		ptrQuote->AskDateTime;
		response.LastDateTime	=		ptrQuote->LastDateTime;
		response.SeqNum			=		ptrQuote->SeqNum;
		response.BidPrice		=		ptrQuote->BidPrice;
		response.AskPrice		=		ptrQuote->AskPrice;
		response.BidSize		=		ptrQuote->BidSize;
		response.AskSize		=		ptrQuote->AskSize;
		response.LotSize		=		ptrQuote->LotSize;
		response.LastPrice		=		ptrQuote->LastPrice;
		response.OpenInterest	=		ptrQuote->OpenInterest;
		response.Volume			=		ptrQuote->Volume;
		response.OpenPrice		=		ptrQuote->OpenPrice;
		response.ClosePrice		=		ptrQuote->ClosePrice;
		response.HighPrice		=		ptrQuote->HighPrice;
		response.LowPrice		=		ptrQuote->LowPrice;
		response.Skipped		=		ptrQuote->Skipped;
		response.TotalRequests	=		ptrQuote->TotalRequests;
		response.WaitTime		=		ptrQuote->WaitTime;
		response.NetChange		=		ptrQuote->NetChange;
		response.Status			=		ptrQuote->Status;
		response.PriceTick		=		ptrQuote->PriceTick;

		response.BidExchange	=		_com_util::ConvertStringToBSTR(ptrQuote->BidExchange.c_str());
		response.AskExchange	=		_com_util::ConvertStringToBSTR(ptrQuote->AskExchange.c_str());
		response.Exchange		=		_com_util::ConvertStringToBSTR(ptrQuote->Exchange.c_str());
		response.Currency		=		_com_util::ConvertStringToBSTR(ptrQuote->Currency.c_str());
	};
};
//----------------------------------------------------------------------------------------//
void	
CDataFeedGateway::Copy(CResponsePtr& spResponse, IRisks& response)
{
	CRisksResponse* ptrRisks = dynamic_cast<CRisksResponse*>(spResponse.get());

	if (ptrRisks)
	{
		response.TheoPrice	=	ptrRisks->TheoPrice;
		response.TheoPriceBid	=	ptrRisks->TheoPriceBid;
		response.TheoPriceAsk	=	ptrRisks->TheoPriceAsk;
		response.Alpha		=	ptrRisks->Alpha;
		response.Delta		=	ptrRisks->Delta;
		response.Gamma		=	ptrRisks->Gamma;
		response.Vega		=	ptrRisks->Vega;
		response.Theta		=	ptrRisks->Theta;
		response.Rho		=	ptrRisks->Rho;
		response.Volga		=	ptrRisks->Volga;
		response.DeltaVega	=	ptrRisks->DeltaVega;
		response.DeltaTheta	=	ptrRisks->DeltaTheta;
		response.GammaVega	=	ptrRisks->GammaVega;
		response.GammaTheta	=	ptrRisks->GammaTheta;
		response.IVola		=	ptrRisks->IVola;
		response.IVolaBid	=	ptrRisks->IVolaBid;
		response.IVolaAsk	=	ptrRisks->IVolaAsk;
		response.TheoVola	=	ptrRisks->TheoVola;
		response.BaseContractPrice	=	ptrRisks->BaseContractPrice;
		response.BaseContractPriceBid	=	ptrRisks->BaseContractPriceBid;
		response.BaseContractPriceAsk	=	ptrRisks->BaseContractPriceAsk;
		response.ContractPrice		=	ptrRisks->ContractPrice;
		response.ContractPriceBid	=	ptrRisks->ContractPriceBid;
		response.ContractPriceAsk	=	ptrRisks->ContractPriceAsk;
		response.IntrinsicValue		=	ptrRisks->IntrinsicValue;
		response.BidPrice			=	ptrRisks->BidPrice;
		response.BidDate			=	ptrRisks->BidDate;
		response.BidSize			=	ptrRisks->BidSize;
		response.AskPrice			=	ptrRisks->AskSize;
		response.AskDate			=	ptrRisks->AskDate;
		response.AskSize			=	ptrRisks->AskSize;
		response.LastPrice			=	ptrRisks->LastPrice;
		response.LastDate			=	ptrRisks->LastDate;
		response.Volume				=	ptrRisks->Volume;
		response.UpdateDate			=	ptrRisks->UpdateDate;
	};
};
//----------------------------------------------------------------------------------------//
CRequestKeyPtr 
CDataFeedGateway::ConvertRequest(QuoteUpdateParams *request)
{
	if (request){
		CRequestKeyPtr spRequest = CRequestKeyPtr(new CRequestKey);

		if (request->Symbol){
			char* __symbol = _com_util::ConvertBSTRToString(request->Symbol);
			spRequest->Symbol = __symbol;
			delete [] __symbol;
		}
		if (request->Exchange){
			char* __exchange = _com_util::ConvertBSTRToString(request->Exchange);
			spRequest->Exchange = __exchange;
			delete [] __exchange;
		}
		spRequest->Type = request->Type;
		return spRequest;
	}
	return CRequestKeyPtr();
};
//----------------------------------------------------------------------------------------//
CRequestKeyPtr 
CDataFeedGateway::ConvertRequest(ITicker *request)
{
	if (request){
		CRequestKeyPtr spRequest = CRequestKeyPtr(new CRequestKey);

		if (request->Symbol){
			char* __symbol = _com_util::ConvertBSTRToString(request->Symbol);
			spRequest->Symbol = __symbol;
			delete [] __symbol;
		}
		if (request->Exchange){
			char* __exchange = _com_util::ConvertBSTRToString(request->Exchange);
			spRequest->Exchange = __exchange;
			delete [] __exchange;
		}
		spRequest->Type = request->Type;
		return spRequest;
	}
	return CRequestKeyPtr();
};
//----------------------------------------------------------------------------------------//
long
CDataFeedGateway::InitCorba()
{
	try
	{

		long my_argc = 7L;
		char** my_argv = new char*[my_argc];

		std::ostringstream applicationName;
		applicationName << CDataFeedSettings::GetProcessFilename();
		applicationName << _T(".exe");

		my_argv[0] = new char[applicationName.str().size() + 1];
		my_argv[0][applicationName.str().size()] = '\0';
		strcpy(my_argv[0], applicationName.str().c_str());

		my_argv[1] = "-ORBInitRef";
		my_argv[2] = const_cast<char*>(CDataFeedSettings::m_provider.c_str());
		my_argv[3] = "-ORBendPoint";
		my_argv[4] = const_cast<char*>(CDataFeedSettings::m_endpoint.c_str());
		my_argv[5] = "-ORBtraceLevel";
		my_argv[6] = const_cast<char*>(CDataFeedSettings::m_ORBTraceLevel.c_str());

		std::string errorMessage;
		int iInitializeCount = 0L;

		std::string conn_info = "Connection info: ";
		conn_info +="  Provider: " + CDataFeedSettings::m_provider;
		conn_info +="; RemoteObject: " + CDataFeedSettings::m_remoteObj;
		conn_info +="; UserName: " + CDataFeedSettings::m_userName;
		conn_info +="; Password: " + CDataFeedSettings::m_password;

		CORBA::ORB_var my_Orb;

		HRESULT hr = ORBInit::Initialize(	my_argc,
											my_argv,
											&my_Orb,
											&__sessionManager,
											CDataFeedSettings::m_serverTimeout,
											CDataFeedSettings::m_remoteObj,
											CDataFeedSettings::m_userName,
											CDataFeedSettings::m_password,
											&iInitializeCount,
											CDataFeedSettings::m_attempts,
											errorMessage );

		std::string SessionID = boost::lexical_cast<std::string>(static_cast<long>( GetTickCount() ));
		__dataManager = __sessionManager->ResolveSession( SessionID.c_str() );

		std::ostringstream str;
		str << "InitializeCount = " << iInitializeCount;
		
		delete[] my_argv[0];
		delete[] my_argv;

		if (hr == S_OK)
		{
			__dataManager->SetCompressionMode( CDataFeedSettings::m_compression );
			__dataManager->SetProtocolVersion( 1 );
			__dataManager->RegisterAllFields( DFFields::T_EQUITY_QUOTE );
			__dataManager->RegisterAllFields( DFFields::T_OPTION_QUOTE );
			__dataManager->RegisterAllFields( DFFields::T_IVRM_RISKS );
			__dataManager->RegisterAllFields( DFFields::T_FUTURE_QUOTE );
			__dataManager->RegisterAllFields( DFFields::T_FUTURE_OPTION_QUOTE );
			__isConnected = true;
		} 
		else 
			__isConnected = false;
	}
	catch (...) {TRACE_UNKNOWN_ERROR();}
	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
long
CDataFeedGateway::DeinitCorba()
{
	try {
		int iInitializeCount = 0;

		CORBA::Short wResult = __sessionManager->DropSession( __dataManager->GetSessionID() );

		if (wResult != DDS::S_ERR_NOERROR){
			std::ostringstream error_mesaage;
			error_mesaage << "Fail to DropSession return: " << static_cast<short>(wResult);
		};

		CORBA::release(__sessionManager);
		ORBInit::ORBShutdown(&iInitializeCount);
	}
	catch (...){ TRACE_UNKNOWN_ERROR();}
	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
void	
CDataFeedGateway::StartPulling()
{
	CStopEvent::CreateEvent();

	unsigned int threadID = 0;
	if (!m_hPullingThread)
		m_hPullingThread  = (HANDLE)_beginthreadex(NULL, 0, PullingThread, this, 0, &threadID);
};
//----------------------------------------------------------------------------------------//
void	
CDataFeedGateway::StopPulling()
{
	if (m_hPullingThread)
	{
		CStopEvent::SetEvent();
		if(::WaitForSingleObject(m_hPullingThread, 1000) != WAIT_OBJECT_0)
		{
			::TerminateThread(m_hPullingThread, -1);
		}
		::CloseHandle(m_hPullingThread);
		m_hPullingThread = NULL;
	}
};
//----------------------------------------------------------------------------------------//
UINT WINAPI CDataFeedGateway::PullingThread(LPVOID pData)
{
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	bool isCorbaError = false;
	CDataFeedGateway* gatewayPtr = reinterpret_cast< CDataFeedGateway* >(pData);

	long code_step = 0;

	while (!CStopEvent::HasSignaledState())
	{
		code_step = 0;
		try{

			DDS::IVData* ivData = NULL;
			CORBA::Any* anyMsg = NULL;

			++code_step;
			gatewayPtr->__dataManager->SetProtocolVersion(1);

			++code_step;
			anyMsg = gatewayPtr->__dataManager->pull(CDataFeedSettings::pull_timeout);
			
			if ((anyMsg) && (*anyMsg >>= ivData))
			{
				try
				{
					//lock scope
					{
						CAutoLock lock(gatewayPtr->__pullLock);
						VS::CVSMessageParser	msgParser;

						std::auto_ptr<VS::CResponseBlockList> responseBlockList = 
							msgParser.DeserializeMessage(ivData->get_buffer(), ivData->length());
						
						if (!responseBlockList->IsEmpty())
						{
							VS::CResponseBlockList::const_iterator iter = responseBlockList->Begin();
							VS::CResponseBlockList::const_iterator itEnd = responseBlockList->End();

							for (; iter!= itEnd; ++iter)
							{
								const VS::CResponseBlock* responseBlock = *iter;
								long messageType = static_cast<long>(responseBlock->GetMessageType());
						
								VS::CResponseBlock::const_iterator recordIter = responseBlock->Begin();
								VS::CResponseBlock::const_iterator recordEnd = responseBlock->End();

								for (; recordIter != recordEnd; ++recordIter)
								{
									const VS::CResponseRecord& record = *recordIter;
									gatewayPtr->ProcessDataFeedResponse(record, messageType);
								}
							}
						}
					}
				}
				catch(VS::CVSParseException ex){
					TRACE_ERROR(_T("Exception occured CVSParseException: [%s]"), _T(ex.what()));
				}

				//Notify client OnQuoteUpdate
				if (gatewayPtr->GetUpdateStatus()|enQuotesUpdates){
					gatewayPtr->m_client->OnQuoteUpdate();
				};

				//Notify client OnRisksUpdate
				if (gatewayPtr->GetUpdateStatus()|enRisksUpdates){
					gatewayPtr->m_client->OnRiskUpdate();
				};
			} 
			++code_step;
			if (anyMsg)
				delete anyMsg;
		}
		catch(CORBA::SystemException& err) {
			std::ostringstream info;
			info << "code_step: " << code_step << " Additional: " <<  ToString(err);
			std::string ex_desc = std::string(info.str());
			TRACE_ERROR(_T("CORBA::SystemException: [%s]"), _T(ex_desc.c_str()));
			isCorbaError = true;
			Sleep(10000);
		}catch(CORBA::Exception& err){
			std::ostringstream info;
			info << "code_step: " << code_step << " Additional: " <<  ToString(err);
			std::string ex_desc = std::string(info.str());
			TRACE_ERROR(_T("CORBA::Exception: [%s]"), _T(ex_desc.c_str()));
			isCorbaError = true;
			Sleep(10000);
		}catch(omniORB::fatalException& err) {
			std::ostringstream info;
			info << "code_step: " << code_step << " Additional: " <<  ToString(err);
			std::string ex_desc = std::string(info.str());
			TRACE_ERROR(_T("omniORB::fatalException: [%s]"), _T(ex_desc.c_str()));
			isCorbaError = true;
			Sleep(10000);
		}catch(...){
			TRACE_UNKNOWN_ERROR();
			isCorbaError = true;
			Sleep(10000);
		}
	};

	::CoUninitialize();
	return 0;
};
//----------------------------------------------------------------------------------------//
//------------------------------------//
//CGateway virtual impl
//----------------------------------------------------------------------------------------//
bool
CDataFeedGateway::AddTask(CTaskInterface::pointer newTask)
{
	if (newTask) return (0 != m_ThreadPool.QueueRequest(newTask));
	
	TRACE_ERROR(_T("invalid pointer of task [%s]"), _T("NULL"));
	
	return false;
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::Connect()
{
	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::Disconnect()
{
	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::Subscribe(CRequestKeyPtr& spRequest, ResponseTypeEnum type)
{
	CResponsePtr spResponse;
	if (!GetResponse(spRequest, type, spResponse)){
		if (enRisksResponse == type)
			return SubscribeRisks(spRequest);
		else if (enQuotesResponse == type)
			return SubscribeQuote(spRequest);
	}
	else {
		if (m_client){
			if (enQuotesResponse == type)
				m_client->OnSubscribed(spRequest);
			else if (enRisksResponse == type)
				m_client->OnSubscribedRisks(spRequest);
		}
	}

	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::SubscribeMultiple(CRequestColl& request, ResponseTypeEnum type)
{
	
	try {
		
		CRequestColl newRequest;
		CResponsePtr spResponse;
		CRequestColl::iterator it = request.begin();
		CRequestColl::iterator itEnd = request.end();

		for (; it != itEnd; it++){
			if (!GetResponse(*it, type, spResponse))
				newRequest.push_back(*it);
			else{
				if (m_client){
					if (enQuotesResponse == type)
						m_client->OnSubscribed(*it);
					else if (enRisksResponse == type)
						m_client->OnSubscribedRisks(*it);
				}
			}
		}
		
		try {
			if (enRisksResponse == type) {
				return SubscribeRisksMultiple(newRequest);
			}
			else if (enQuotesResponse == type) {
				return SubscribeQuoteMultiple(newRequest);
			}
		}
		catch (...) {
			TRACE_ERROR(_T("unknown exception occured while: [%s]"), _T("calling subscribe"));
			return -1L;
		}
	}
	catch (...) {
		TRACE_ERROR(_T("unknown exception occured while: [%s]"), _T("subscribe multiple"));
		return -1L;
	}		
	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::Request(CRequestKeyPtr& request, ResponseTypeEnum type, CResponsePtr& response)
{
	if (enRisksResponse == type)
		return RequestRisks(request, response);
	else if (enQuotesResponse == type)
		return RequestQuote(request, response);

	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestMultiple(CRequestColl& request, ResponseTypeEnum type, CResponseVector& response)
{
	if (enRisksResponse == type)
		return RequestRisksMultiple(request, response);
	else if (enQuotesResponse == type)
		return RequestQuoteMultiple(request, response);

	return static_cast<long>(1);
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::SubscribeQuote(CRequestKeyPtr& request)
{
	CORBA::Short retResult = DDS::S_ERR_NOERROR;

	if (IsBadRequest(request)){
		m_client->OnError(request, std::string("Request not supported."), 500, enSubscribeQuote);
		return static_cast<long>(retResult);
	};

	if (request->Type == enOPT){
		//subscribe to data feed
		try {
			retResult = __dataManager->Subscribe(request->Symbol.c_str(), DDS::OPTION_SYMBOL, DFFields::T_OPTION_QUOTE);
		}
		catch (...){TRACE_UNKNOWN_ERROR();}
	}
	else if (request->Type == enSTK || request->Type == enIDX){
		//subscribe to data feed
		try {
			retResult = __dataManager->Subscribe(request->Symbol.c_str(), DDS::STOCK_SYMBOL, DFFields::T_EQUITY_QUOTE);
		}
		catch (...){TRACE_UNKNOWN_ERROR();}
	}
	else if (request->Type == enFUT){
		//subscribe to data feed
		try {
			retResult = __dataManager->Subscribe(request->Symbol.c_str(), DDS::OPTION_SYMBOL, DFFields::T_FUTURE_QUOTE);
		}
		catch (...){TRACE_UNKNOWN_ERROR();}
	}
	else if (request->Type == enFOP){
		//subscribe to data feed
		try {
			retResult = __dataManager->Subscribe(request->Symbol.c_str(), DDS::OPTION_SYMBOL, DFFields::T_FUTURE_OPTION_QUOTE);
		}
		catch (...){TRACE_UNKNOWN_ERROR();}
	}

	return static_cast<long>(retResult);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestQuote(CRequestKeyPtr& request, CResponsePtr& response)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::SubscribeQuoteMultiple(CRequestColl& request)
{

	CORBA::Short retResult = DDS::S_ERR_NOERROR;
	
	CRequestColl::iterator it = request.begin();
	CRequestColl::iterator itEnd = request.end();

	CRequestColl options;
	CRequestColl equitys;
	CRequestColl futures;
	CRequestColl future_options;

	try {
	
		for ( ;it != itEnd; it++) {
		
			if ((*it)->Type == enOPT) {
				options.push_back(*it);
			}
			else if ((*it)->Type == enSTK || (*it)->Type == enIDX) {
				equitys.push_back(*it);
			}
			else if ((*it)->Type == enFUT) {
				futures.push_back(*it);
			}
			else if ((*it)->Type == enFOP) {
				future_options.push_back(*it);
			}
			else {
			
				TRACE_ERROR(_T("request not supported"), _T("unknown type"));
				
				m_client->OnError(*it, std::string("Request not supported."), 501, enSubscribeQuote);
			}
		}
	}
	catch (...) {
		TRACE_ERROR(_T("unknown error occurred while fill request containers[%s]"), _T("requests not processed"));
		return -1L;
	}
	
	if (!equitys.empty()) {
	
		CORBA::Any providerAnySymbols;

		//pack request collection to IVDataFeed structures
		
		PackRequestToAny(providerAnySymbols, equitys);

		//subscribe group of symbols from DataFeed
		try {
			retResult = __dataManager->SubscribeGroup(providerAnySymbols, DDS::STOCK_SYMBOL, DFFields::T_EQUITY_QUOTE);
		}
		catch (...) {
			TRACE_UNKNOWN_ERROR();
		}
	}

	if (!options.empty()) {
		
		unsigned long request_size = static_cast<unsigned long>(options.size());
		
		CRequestColl request_part;
		
		request_part.reserve(10000);
		
		bool send_part = false;
		
		for (unsigned long i = 0; i < request_size; i++) {
			
			request_part.push_back(options[i]);
			
			if ((i + 1) % 3000) {
				send_part = true;	
			}
			else if (i + 1 == request_size) {
				send_part = true;
			}
			else {
				send_part = false;
			}
			
			if (send_part) {
					
				CORBA::Any providerAnySymbols;

				//pack request collection to IVDataFeed structures
				PackRequestToAny(providerAnySymbols, request_part);

				//subscribe group of symbols from DataFeed
				try {
					retResult = __dataManager->SubscribeGroup(providerAnySymbols, DDS::OPTION_SYMBOL, DFFields::T_OPTION_QUOTE);
				}
				catch (...) {
					TRACE_UNKNOWN_ERROR();
				}
				
				request_part.clear();
			}
		}
	}

	if (!futures.empty()) {
	
		CORBA::Any providerAnySymbols;

		//pack request collection to IVDataFeed structures
		PackRequestToAny(providerAnySymbols, futures);

		//subscribe group of symbols from DataFeed
		try {
			retResult = __dataManager->SubscribeGroup(providerAnySymbols, DDS::OPTION_SYMBOL, DFFields::T_FUTURE_QUOTE);
		}
		catch (...) {
			TRACE_UNKNOWN_ERROR();
		}
	}

	if (!future_options.empty()) {
	
		CORBA::Any providerAnySymbols;

		//pack request collection to IVDataFeed structures
		PackRequestToAny(providerAnySymbols, future_options);

		//subscribe group of symbols from DataFeed
		try {
			retResult = __dataManager->SubscribeGroup(providerAnySymbols, DDS::OPTION_SYMBOL, DFFields::T_FUTURE_OPTION_QUOTE);
		}
		catch (...) {
			TRACE_UNKNOWN_ERROR();
		}
	}

	return static_cast<long>(retResult);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestQuoteMultiple(CRequestColl& request, CResponseVector& response)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::CancelLastQuote()
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
bool	
CDataFeedGateway::IsQuotesUpdated()
{
	return static_cast<bool>(true);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::GetUpdates(ResponseTypeEnum type, CResponseVector& response)
{
	if (type == enQuotesResponse)
		GetQuoteUpdates(response);
	else if (type == enRisksUpdates)
		GetRiskUpdates(response);

	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::GetQuoteUpdates(CResponseVector& response)
{
	CAutoLock lock(m_dataLock);

	if (GetUpdateStatus()|enQuotesUpdates){
		ResetUpdateStatus(enQuotesUpdates);
	};

	CDataColl::iterator it = m_spData->begin();
	CDataColl::iterator itEnd = m_spData->end();

	CResponsePtr spResponse;
	for (;it != itEnd; it++){
		if (it->second->end() != it->second->find(enQuotesResponse)){
			spResponse = it->second->operator [](enQuotesResponse);
			if (spResponse->IsUpdated()){
				response.push_back(spResponse);
				spResponse->ResetUpdated();
			}
		}
	};
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
bool	
CDataFeedGateway::SetRegionRate(double rate)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::UnNotifiedSubscribtionQuantity(bool skipNotification)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestRisks(CRequestKeyPtr& request, CResponsePtr& response)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestRisksMultiple(CRequestColl& request, CResponseVector& response)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::SubscribeRisks(CRequestKeyPtr& request)
{
	CORBA::Short retResult = DDS::S_ERR_NOERROR;

	if (IsBadRequest(request)){
		m_client->OnError(request, std::string("Request not supported."), 500, enSubscribeRisks);
		return static_cast<long>(retResult);
	};

	std::ostringstream key;
	key << request->Symbol << ":" << static_cast<int>(request->Type);

	//subscribe to data feed
	try {
		retResult = __dataManager->Subscribe(key.str().c_str(), DDS::OPTION_SYMBOL, DFFields::T_IVRM_RISKS);
	}
	catch (...){TRACE_UNKNOWN_ERROR();}


	return static_cast<long>(retResult);		
};
//----------------------------------------------------------------------------------------//
void	
CDataFeedGateway::PackRequestToAny(CORBA::Any& anyMsg, const CRequestColl& request)
{
	try {
		if (!request.empty()) {	
		
			const CORBA::ULong listSize = static_cast<CORBA::ULong>(request.size());

			char** corbaSymbolList	= new char*[listSize];
			char** pCurrentSymbol	= corbaSymbolList;

			for (CRequestColl::const_iterator it = request.begin(); it != request.end(); it++)
			{
				const CRequestKeyPtr& spRequest = *it;
				*pCurrentSymbol++ = const_cast<char*>(spRequest->Symbol.c_str());
			}

			DDS::SymbolsGroup symbolsGroup(listSize, corbaSymbolList, 0);
			anyMsg <<= symbolsGroup;

			delete[] corbaSymbolList;
		}
	}
	catch (...) {
		TRACE_ERROR(_T("unknown exception occurred while packing request to type ANY"), _T("critical error"));
	}
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::SubscribeRisksMultiple(CRequestColl& request)
{
	
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::GetRiskUpdates(CResponseVector& response)
{
	CAutoLock lock(m_dataLock);

	if (GetUpdateStatus()|enRisksUpdates){
		ResetUpdateStatus(enRisksUpdates);	
	};

	CDataColl::iterator it = m_spData->begin();
	CDataColl::iterator itEnd = m_spData->end();

	CResponsePtr spResponse;
	for (;it != itEnd; it++){
		if (it->second->end() != it->second->find(enRisksResponse)){
			spResponse = it->second->operator [](enRisksResponse);
			if (spResponse->IsUpdated()){
				response.push_back(spResponse);
				spResponse->ResetUpdated();
			}
		}
	};

	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestDividends(CRequestKeyPtr& request, CResponsePtr& response)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestRates(CRequestKeyPtr& request, CResponsePtr& response)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
long	
CDataFeedGateway::RequestContracts(CRequestKeyPtr& request, CResponsePtr& response)
{
	return static_cast<long>(1);	
};
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
void CSubscribeSingle::DoTask()
{
	if (m_gateway)
		m_gateway->Subscribe(m_spRequest, m_enType);
};
//----------------------------------------------------------------------------------------//
void CSubscribeMultiple::DoTask()
{
	if (m_gateway) {
		m_gateway->SubscribeMultiple(*m_spRequest, m_enType);
	}
};
//----------------------------------------------------------------------------------------//
void CRequestSingle::DoTask()
{
	if (m_gateway){
		CResponsePtr response;
		m_gateway->Request(m_spRequest, m_enType, response);
	}
};
//----------------------------------------------------------------------------------------//
void CRequestMultiple::DoTask()
{
	if (m_gateway){
		CResponseVector response;
		m_gateway->RequestMultiple(*m_spRequest, m_enType, response);
	}
};
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
CRequestKeyPtr	
CResponseParser::GetRequestKey(const VS::CResponseRecord& record, long messageType)
{
	CRequestKeyPtr spRequest = CRequestKeyPtr(new CRequestKey);
	if (messageType == DFFields::T_EQUITY_QUOTE){	
		spRequest->Symbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
		spRequest->Type = enSTK;
		return spRequest;
	}
	else if (messageType == DFFields::T_OPTION_QUOTE){
		spRequest->Symbol = record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
		spRequest->Type = enOPT;
		return spRequest;
	}
	else if (messageType == DFFields::T_IVRM_RISKS){
		spRequest->Symbol = record.GetField(DFFields::FIELD_SYMBOL).AsString();
		spRequest->Type = static_cast<InstrumentTypeEnum>(record.GetField(DFFields::FIELD_SYMBOL_TYPE).AsLong());
		return spRequest;
	}
	else if (messageType == DFFields::T_FUTURE_QUOTE){
		spRequest->Symbol = record.GetField(DDS::FIELD_FUTURE_SYMBOL).AsString();
		spRequest->Type = enFUT;
		return spRequest;
	}
	else if (messageType == DFFields::T_FUTURE_OPTION_QUOTE){
		spRequest->Symbol = record.GetField(DDS::FIELD_FUTURE_OPTION_SYMBOL).AsString();
		spRequest->Type = enFOP;
		return spRequest;
	}
	else {
		return CRequestKeyPtr();
	}
	return CRequestKeyPtr();
};
//----------------------------------------------------------------------------------------//
double
CResponseParser::round(float value){
	
	float ip = 0.;
	float fp = modf(value, &ip);
	
	float fip = 0.;
	modf(floor(fp * float(10000.) + float(0.5)), &fip);

	return double(ip) + double(fip) * 0.0001;
};
//----------------------------------------------------------------------------------------//
void	
CResponseParser::ParseOptionQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse)
{
	CQuoteResponse* quote = dynamic_cast<CQuoteResponse*>(spResponse.get());

	if (quote){
		quote->BidPrice			= CResponseParser::round(record.GetField(DDS::FIELD_BID).AsFloat());
		quote->AskPrice			= CResponseParser::round(record.GetField(DDS::FIELD_ASK).AsFloat());
		quote->LastPrice		= CResponseParser::round(record.GetField(DDS::FIELD_LAST).AsFloat());

		quote->BidExchange		= record.GetField(DDS::FIELD_BID_EXCHANGE).AsString();
		quote->AskExchange		= record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString();
		quote->Volume			= static_cast<long>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
		quote->BidSize			= record.GetField(DDS::FIELD_BID_SIZE).AsInt();
		quote->AskSize			= record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
		quote->Exchange			= record.GetField(DDS::FIELD_LAST_EXCHANGE).AsString();
		quote->UpdateDateTime	= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();
		quote->BidDateTime		= record.GetField(DDS::FIELD_BID_DATE).AsDouble();
		quote->AskDateTime		= record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
		quote->LastDateTime		= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();

		quote->OpenInterest		= record.GetField(DFFields::FIELD_OPEN_INTEREST).AsDouble();
		quote->NetChange		= record.GetField(DFFields::FIELD_NET_CHANGE_1).AsDouble();
		quote->PriceTick		= record.GetField(DFFields::FIELD_PRICE_TICK).AsLong();
		quote->OpenPrice		= CResponseParser::round(record.GetField(DFFields::FIELD_OPEN_PRICE).AsFloat());
		quote->HighPrice		= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_HIGH).AsFloat());
		quote->LowPrice			= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_LOW).AsFloat());
	};
};
//----------------------------------------------------------------------------------------//
void	
CResponseParser::ParseEquityQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse)
{
	CQuoteResponse* quote = dynamic_cast<CQuoteResponse*>(spResponse.get());

	if (quote){
		quote->BidPrice				= CResponseParser::round(record.GetField(DDS::FIELD_BID).AsFloat());
		quote->BidExchange			= record.GetField(DDS::FIELD_BID_EXCHANGE).AsString();
		quote->UpdateDateTime		= record.GetField(DDS::FIELD_BID_DATE).AsDouble();
		quote->BidSize				= record.GetField(DDS::FIELD_BID_SIZE).AsInt();
		quote->AskPrice				= CResponseParser::round(record.GetField(DDS::FIELD_ASK).AsFloat());
		quote->AskExchange			= record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString();
		quote->UpdateDateTime		= record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
		quote->AskSize				= record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
		quote->LastPrice			= CResponseParser::round(record.GetField(DDS::FIELD_LAST).AsFloat());
		quote->Exchange				= record.GetField(DDS::FIELD_LAST_EXCHANGE).AsString();
		quote->UpdateDateTime		= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();
		quote->Volume				= static_cast<long>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
		quote->BidDateTime			= record.GetField(DDS::FIELD_BID_DATE).AsDouble();
		quote->AskDateTime			= record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
		quote->LastDateTime			= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();

		quote->OpenInterest			= record.GetField(DFFields::FIELD_OPEN_INTEREST).AsDouble();
		quote->NetChange			= record.GetField(DFFields::FIELD_NET_CHANGE_1).AsDouble();
		quote->PriceTick			= record.GetField(DFFields::FIELD_PRICE_TICK).AsLong();
		quote->OpenPrice			= CResponseParser::round(record.GetField(DFFields::FIELD_OPEN_PRICE).AsFloat());
		quote->HighPrice			= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_HIGH).AsFloat());
		quote->LowPrice				= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_LOW).AsFloat());
	};
};
//----------------------------------------------------------------------------------------//
void	
CResponseParser::ParseFutureOptionQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse)
{
	CQuoteResponse* quote = dynamic_cast<CQuoteResponse*>(spResponse.get());

	if (quote){
		quote->BidPrice			= CResponseParser::round(record.GetField(DDS::FIELD_BID).AsFloat());
		quote->AskPrice			= CResponseParser::round(record.GetField(DDS::FIELD_ASK).AsFloat());
		quote->BidExchange		= record.GetField(DDS::FIELD_BID_EXCHANGE).AsString();
		quote->AskExchange		= record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString();
		quote->LastPrice		= CResponseParser::round(record.GetField(DDS::FIELD_LAST).AsFloat());
		quote->Volume			= static_cast<long>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
		quote->BidSize			= record.GetField(DDS::FIELD_BID_SIZE).AsInt();
		quote->AskSize			= record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
		quote->Exchange			= record.GetField(DDS::FIELD_LAST_EXCHANGE).AsString();
		quote->UpdateDateTime	= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();
		quote->BidDateTime		= record.GetField(DDS::FIELD_BID_DATE).AsDouble();
		quote->AskDateTime		= record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
		quote->LastDateTime		= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();

		quote->OpenInterest		= record.GetField(DFFields::FIELD_OPEN_INTEREST).AsDouble();
		quote->NetChange		= record.GetField(DFFields::FIELD_NET_CHANGE_1).AsDouble();
		quote->PriceTick		= record.GetField(DFFields::FIELD_PRICE_TICK).AsLong();
		quote->OpenPrice		= CResponseParser::round(record.GetField(DFFields::FIELD_OPEN_PRICE).AsFloat());
		quote->HighPrice		= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_HIGH).AsFloat());
		quote->LowPrice			= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_LOW).AsFloat());
	};
};
//----------------------------------------------------------------------------------------//
void	
CResponseParser::ParseFutureQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse)
{
	CQuoteResponse* quote = dynamic_cast<CQuoteResponse*>(spResponse.get());

	if (quote){
		quote->BidPrice				= CResponseParser::round(record.GetField(DDS::FIELD_BID).AsFloat());
		quote->BidExchange			= record.GetField(DDS::FIELD_BID_EXCHANGE).AsString();
		quote->UpdateDateTime		= record.GetField(DDS::FIELD_BID_DATE).AsDouble();
		quote->BidSize				= record.GetField(DDS::FIELD_BID_SIZE).AsInt();
		quote->AskPrice				= CResponseParser::round(record.GetField(DDS::FIELD_ASK).AsFloat());
		quote->AskExchange			= record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString();
		quote->UpdateDateTime		= record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
		quote->AskSize				= record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
		quote->LastPrice			= CResponseParser::round(record.GetField(DDS::FIELD_LAST).AsFloat());
		quote->Exchange				= record.GetField(DDS::FIELD_LAST_EXCHANGE).AsString();
		quote->UpdateDateTime		= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();
		quote->Volume				= static_cast<long>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
		quote->BidDateTime			= record.GetField(DDS::FIELD_BID_DATE).AsDouble();
		quote->AskDateTime			= record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
		quote->LastDateTime			= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();

		quote->OpenInterest			= record.GetField(DFFields::FIELD_OPEN_INTEREST).AsDouble();
		quote->NetChange			= record.GetField(DFFields::FIELD_NET_CHANGE_1).AsDouble();
		quote->PriceTick			= record.GetField(DFFields::FIELD_PRICE_TICK).AsLong();
		quote->OpenPrice			= CResponseParser::round(record.GetField(DFFields::FIELD_OPEN_PRICE).AsFloat());
		quote->HighPrice			= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_HIGH).AsFloat());
		quote->LowPrice				= CResponseParser::round(record.GetField(DFFields::FIELD_PRICE_LOW).AsFloat());
	};
};
//----------------------------------------------------------------------------------------//
void 
CResponseParser::ParseRisks(const VS::CResponseRecord& record, CResponsePtr& spResponse)
{
	CRisksResponse* risks = dynamic_cast<CRisksResponse*>(spResponse.get());

	if (risks){
		risks->TheoPrice =			record.GetField(DFFields::FIELD_THEO_PRICE).AsFloat();
		risks->TheoPriceBid =		record.GetField(DFFields::FIELD_THEO_PRICE_BID).AsFloat();
		risks->TheoPriceAsk =		record.GetField(DFFields::FIELD_THEO_PRICE_ASK).AsFloat();
		risks->Alpha =				0.;
		risks->Delta =				record.GetField(DFFields::FIELD_DELTA).AsFloat();
		risks->Gamma =				record.GetField(DFFields::FIELD_GAMMA).AsFloat();
		risks->Vega =				record.GetField(DFFields::FIELD_VEGA).AsFloat();
		risks->Theta =				record.GetField(DFFields::FIELD_THETA).AsFloat();
		risks->Rho =				record.GetField(DFFields::FIELD_RHO).AsFloat();
		risks->Volga =				record.GetField(DFFields::FIELD_VOLGA).AsFloat();

		risks->DeltaVega =			record.GetField(DFFields::FIELD_DELTA_VEGA).AsFloat();
		risks->DeltaTheta =			record.GetField(DFFields::FIELD_DELTA_THETA).AsFloat();
		risks->GammaVega =			record.GetField(DFFields::FIELD_GAMMA_VEGA).AsFloat();
		risks->GammaTheta =			record.GetField(DFFields::FIELD_GAMMA_THETA).AsFloat();

		risks->IVola =				record.GetField(DFFields::FIELD_IV).AsFloat();
		risks->IVolaBid =			0.;
		risks->IVolaAsk =			0.;
		risks->TheoVola =			record.GetField(DFFields::FIELD_THEO_VOLA).AsFloat();

		risks->BaseContractPrice =	record.GetField(DFFields::FIELD_BASECONTRACT_PRICE).AsFloat();
		risks->ContractPrice =		record.GetField(DFFields::FIELD_CONTRACT_PRICE).AsFloat();

		risks->BaseContractPriceBid =	record.GetField(DFFields::FIELD_BASECONTRACT_PRICE_BID).AsFloat();
		risks->ContractPriceBid =	record.GetField(DFFields::FIELD_CONTRACT_PRICE_BID).AsFloat();

		risks->BaseContractPriceAsk =	record.GetField(DFFields::FIELD_BASECONTRACT_PRICE_ASK).AsFloat();
		risks->ContractPriceAsk =	record.GetField(DFFields::FIELD_CONTRACT_PRICE_ASK).AsFloat();

		risks->IntrinsicValue	=	record.GetField(DFFields::FIELD_INTRINSIC_VALUE).AsFloat();

		risks->BidPrice	=			CResponseParser::round(record.GetField(DDS::FIELD_BID).AsFloat());
		risks->BidDate =			record.GetField(DDS::FIELD_BID_DATE).AsDouble();
		risks->BidSize =			record.GetField(DDS::FIELD_BID_SIZE).AsInt();
		risks->AskPrice =			CResponseParser::round(record.GetField(DDS::FIELD_ASK).AsFloat());
		risks->AskDate =			record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
		risks->AskSize =			record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
		risks->LastPrice =			CResponseParser::round(record.GetField(DDS::FIELD_LAST).AsFloat());
		risks->LastDate =			record.GetField(DDS::FIELD_LAST_DATE).AsDouble();
		risks->Volume =				static_cast<long>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
		risks->UpdateDate =			record.GetField(DFFields::FIELD_UPDATE_DATE).AsDouble();
	};
};
//----------------------------------------------------------------------------------------//
void 
CResponseParser::ParseResponse(const VS::CResponseRecord& record, long messageType, CResponsePtr& spResponse)
{
	if (messageType == DFFields::T_IVRM_RISKS){
		ParseRisks(record, spResponse);
	}
	else if (messageType == DFFields::T_EQUITY_QUOTE){
		ParseEquityQuote(record, spResponse);
	}
	else if (messageType == DFFields::T_OPTION_QUOTE){
		ParseOptionQuote(record, spResponse);
	}
	else if (messageType == DFFields::T_FUTURE_QUOTE){
		ParseFutureQuote(record, spResponse);
	}
	else if (messageType == DFFields::T_FUTURE_OPTION_QUOTE){
		ParseFutureOptionQuote(record, spResponse);
	}
};
//----------------------------------------------------------------------------------------//