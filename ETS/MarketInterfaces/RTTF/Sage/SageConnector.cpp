#include "stdafx.h"

#include "SageConnector.h"
#include "Publisher.h"
#include "SgConst.h"
//#include "Sage.h"


CFixTimeValue Convert(FIX::TransactTime & value) 
{

	CFixTimeValue Result;
	ZeroMemory(&Result, sizeof(Result));

	if (((char*)value.getString().c_str())[8]  != '-' || 
		((char*)value.getString().c_str())[11] != ':' ||
		((char*)value.getString().c_str())[14] != ':' )
		throw::logic_error("Unable to parse transactTime field");

	string Temp;

	Temp.assign((char*)value.getString().c_str(), 4);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_year) != 1) throw::logic_error("Unable to parse TransactTime field");
	Temp.assign(((char*)value.getString().c_str()) + 4, 2);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_mon) != 1) throw::logic_error("Unable to parse TransactTime field");
	Temp.assign(((char*)value.getString().c_str()) + 6, 2);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_mday) != 1) throw::logic_error("Unable to parse TransactTime field");
	Temp.assign(((char*)value.getString().c_str()) + 9, 2);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_hour) != 1) throw::logic_error("Unable to parse TransactTime field");
	Temp.assign(((char*)value.getString().c_str()) + 12, 2);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_min) != 1) throw::logic_error("Unable to parse TransactTime field");
	Temp.assign(((char*)value.getString().c_str()) + 15, 2);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_sec) != 1) throw::logic_error("Unable to parse TransactTime field");

	Result.tm_year -= 1900;
	Result.tm_mon -= 1;

	return Result;
}


CFixMonthYearValue  Convert(FIX::MaturityMonthYear& value) 
{
	CFixMonthYearValue Result;

	string Temp;

	Temp.assign((char*)value.getString().c_str(), 4);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_year) != 1) throw::logic_error("Unable to parse MaturityMonthYear field");
	Temp.assign(((char*)value.getString().c_str()) + 4, 2);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_mon) != 1) throw::logic_error("Unable to parse MaturityMonthYear field");

	Result.tm_year -= 1900;
	Result.tm_mon -= 1;

	return Result;
}


CFixDateValue Convert(FIX::TradeDate& value)
{
	CFixDateValue Result;
	ZeroMemory(&Result, sizeof(Result));
	std::string Temp;

	int iYear = 0;
	int iMonth = 0;

	Temp.assign((char*)value.getString().c_str(), 4);
	
	if (sscanf(Temp.c_str(), "%u", &iYear) != 1) throw::logic_error("Unable to parse TradeDate field");

	Temp.assign(((char*)value.getString().c_str()) + 4, 2);
	if (sscanf(Temp.c_str(), "%u", &iMonth) != 1) throw::logic_error("Unable to parse TradeDate field");

	Temp.assign(((char*)value.getString().c_str()) + 6, 2);
	if (sscanf(Temp.c_str(), "%u", &Result.tm_mday) != 1) throw::logic_error("Unable to parse TradeDate field");

	Result.tm_year = iYear-1900;
	Result.tm_mon  = iMonth-1;

	return Result;
}

CFixDayOfMonthValue Convert(FIX::MaturityDay& value) 
{
	CFixDayOfMonthValue Result;
	string Temp;

	Temp.assign((char*)value.getString().c_str(), 2);
	sscanf(Temp.c_str(), "%u", &Result.tm_mday);
	return Result;
}


DWORD CSageConnector::Start()
{
	if(m_pInitiator != NULL || m_pAcceptor != NULL)
		return E_FAIL;

	DWORD dwResult = ERROR_SUCCESS;
	CTracer::Trace(_T("Starting publisher..."));
	
	for (DWORD i = 0; i < m_nGroupsCount; i++)
	{
		if(ERROR_SUCCESS != m_Publisher[i].Start()) 
			return CTracer::TraceError(E_FAIL, _T("Unable to start publisher"));
	}

	CTracer::Trace(_T("Publisher started."));

	try
	{
		m_bFilterOutFutureTrades = false;

		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		_bstr_t sbsUserGroup;
		XMLParams.GetUserGroup(sbsUserGroup.GetAddress());

		_bstr_t sbsValue;
		
		XMLParams.GetMainXMLString(SETTINGS_XML_KEY, sbsUserGroup, &sbsValue);

		_bstr_t sbsKeyValue;
		XMLParams.GetXMLString( SETTINGS_XML_SETTINGS_KEY, _T("FilterOutFutureTrades"), &sbsKeyValue );
		if ( sbsKeyValue == _bstr_t(L"y") || sbsKeyValue == _bstr_t(L"Y") ) 
			m_bFilterOutFutureTrades = true;

		CString ConfigurationFile = (LPTSTR)sbsValue;

		if(ConfigurationFile.GetLength())
		{

			 m_pSettings     = boost::shared_ptr<FIX::SessionSettings>(new FIX::SessionSettings(ConfigurationFile.GetString()));
			 m_pStoreFactory = boost::shared_ptr<FIX::FileStoreFactory>(new FIX::FileStoreFactory(*m_pSettings));
			 m_pLogFactory   = boost::shared_ptr<FIX::FileLogFactory>(new FIX::FileLogFactory(*m_pSettings));     


			 const FIX::Dictionary& defaultDict = m_pSettings->get();

			if(defaultDict.has("ConnectionType") && defaultDict.getString("ConnectionType") == "acceptor")
			{
				boost::shared_ptr<FIX::SocketAcceptor> pAcceptor = boost::shared_ptr<FIX::SocketAcceptor>(new FIX::SocketAcceptor(*this, *m_pStoreFactory, *m_pSettings, *m_pLogFactory));
				if( pAcceptor != NULL )
				{
					m_pAcceptor = pAcceptor;
					m_pAcceptor->start();
					dwResult = ERROR_SUCCESS;
				}
				else
					dwResult = GetLastError();

			}
			else
			{
				boost::shared_ptr<FIX::SocketInitiator> pInitiator = boost::shared_ptr<FIX::SocketInitiator>(new FIX::SocketInitiator(*this, *m_pStoreFactory, *m_pSettings, *m_pLogFactory));
				if( pInitiator != NULL )
				{
					m_pInitiator = pInitiator;
					m_pInitiator->start();
					dwResult = ERROR_SUCCESS;
				}
				else
					dwResult = GetLastError();
			}
		}
		else
		{
			CTracer::TraceError(E_FAIL, "The configuration file is not specified");
			dwResult = E_FAIL;
		}
	}
	catch(FIX::ConfigError& re)
	{
		std::string sMsg("FIX Configuration Error:");
		sMsg += re.what();
		CTracer::TraceError(E_FAIL, sMsg.c_str());

		AfxMessageBox(sMsg.c_str(), MB_ICONERROR);
		dwResult = E_FAIL;

	}
	catch(FIX::RuntimeError& re)
	{
		std::string sMsg("FIX Runtime Error:");
		sMsg += re.what();
		CTracer::TraceError(E_FAIL, sMsg.c_str());
		AfxMessageBox(sMsg.c_str(), MB_ICONERROR);
		dwResult = E_FAIL;
	}
	catch(...)
	{
		CTracer::TraceError(E_FAIL, "Fatal error in QuickFix");
		AfxMessageBox("Fatal error in QuickFix");
		dwResult = E_FAIL;
	}
	
	if(dwResult != ERROR_SUCCESS)
	{	
		for (DWORD i = 0; i < m_nGroupsCount; i++)
		{
			m_Publisher[i].Stop();
		}
	}

	return dwResult;

};

DWORD CSageConnector::Stop()
{
	DWORD dwResult =  ERROR_SUCCESS;
	try
	{
		if(m_pInitiator)
		{
			m_pInitiator->stop();
			m_pInitiator = boost :: shared_ptr<FIX::SocketInitiator>();

			m_pStoreFactory = boost::shared_ptr<FIX::FileStoreFactory>();
			m_pLogFactory   = boost::shared_ptr<FIX::FileLogFactory>();     
			m_pSettings     = boost::shared_ptr<FIX::SessionSettings>();

		}
		if(m_pAcceptor)
		{
			m_pAcceptor->stop();
			m_pAcceptor = boost :: shared_ptr<FIX::SocketAcceptor>();
			m_pStoreFactory = boost::shared_ptr<FIX::FileStoreFactory>();
			m_pLogFactory   = boost::shared_ptr<FIX::FileLogFactory>();     
			m_pSettings     = boost::shared_ptr<FIX::SessionSettings>();
		}
		CTracer::Trace(_T("Stopping publisher..."));

		for (DWORD i = 0; i < m_nGroupsCount; i++)
		{
			m_Publisher[i].Stop();
		}
		CTracer::Trace(_T("Publisher stopped."));

	}
	catch(FIX::ConfigError& re)
	{
		std::string sMsg("FIX Configuration Error:");
		sMsg += re.what();
		CTracer::TraceError(E_FAIL, sMsg.c_str());

		AfxMessageBox(sMsg.c_str(), MB_ICONERROR);
		dwResult = E_FAIL;

	}
	catch(FIX::RuntimeError& re)
	{
		std::string sMsg("FIX Runtime Error:");
		sMsg += re.what();
		CTracer::TraceError(E_FAIL, sMsg.c_str());
		AfxMessageBox(sMsg.c_str(), MB_ICONERROR);
		dwResult = E_FAIL;
	}
	catch(...)
	{
		CTracer::TraceError(E_FAIL, "Fatal error in QuickFix");
		AfxMessageBox("Fatal error in QuickFix");
		dwResult = E_FAIL;
	}


	return dwResult;
}

void CSageConnector::onMessage(const FIX41::ExecutionReport& message, const FIX::SessionID& sessionID)
{
	char szBuf[2048];
	ZeroMemory(szBuf, sizeof(szBuf));
	size_t sizeBuf = sizeof(szBuf)-1;

	bool bCorrection = false;
	string sCorrectionID;

	try
	{
		CTradePtr pTrade = CTradePtr(new CTrade());

		FIX::ExecTransType execTransType;
		message.get(execTransType);
		pTrade->cExecTransType = execTransType;

		FIX::ExecID execID;
		message.get(execID);
		pTrade->sExecID = execID;

		if(pTrade->cExecTransType != FIX::ExecTransType_NEW && pTrade->cExecTransType != FIX::ExecTransType_CANCEL && pTrade->cExecTransType != FIX::ExecTransType_CORRECT /*&& Trade->cExecTransType != ETT_CORRECT*/)
		{
			_sntprintf(szBuf,sizeBuf, _T("ExecTransType = '%c'. Message ignored."), pTrade->cExecTransType);
			throw std::logic_error(szBuf);
		}
		if(pTrade->cExecTransType == FIX::ExecTransType_CANCEL)
		{
			FIX::ExecRefID execRefID;
			if(!message.isSetField(execRefID))
				throw std::logic_error("ExecRefID field missing");

			message.get(execRefID);
			pTrade->sExecID =  execRefID;
		}
		if(pTrade->cExecTransType == FIX::ExecTransType_CORRECT)
		{
			bCorrection = true;
			FIX::ExecRefID execRefID;
			if(!message.isSetField(execRefID))
				throw std::logic_error("ExecRefID field missing");

			message.get(execRefID);
			pTrade->sExecID = execRefID;
			sCorrectionID =	  execID;
		}

		FIX::OrdStatus ordStatus;
		message.get(ordStatus);
		pTrade->cOrderStatus = ordStatus;

//		if(pTrade->cExecTransType == FIX::ExecTransType_NEW && pTrade->cOrderStatus != FIX::OrdStatus_PARTIALLY_FILLED && pTrade->cOrderStatus != FIX::OrdStatus_FILLED )
//		{
//			_sntprintf(szBuf,sizeBuf, _T("OrdStatus = '%c'. Message ignored."), pTrade->cOrderStatus);
//			throw std::logic_error(szBuf);
//		}

		FIX::Account account;
		if(!message.isSetField(account))
			throw std::logic_error("Failed to get value of Account field.");
		message.get(account);

		if(m_pSettings->get(sessionID).has(_T("TraderOverride")))
		{
			  pTrade->sAccount = m_pSettings->get(sessionID).getString(_T("TraderOverride"));
			  if(m_pSettings->get(sessionID).has(account))
				  pTrade->sBroker = m_pSettings->get(sessionID).getString(account);
		}
		else
			pTrade->sAccount = account;

		CTracer::Trace(_T("Trade received ExecID = '%s'"), pTrade->sExecID.c_str());
		for (DWORD i = 0; i < m_nGroupsCount; i++)
		{
			InterlockedIncrement((LPLONG)m_Publisher[i].GetCounterReceived());
		}

		FIX::Symbol symbol;
		message.get(symbol);
		pTrade->sSymbol = symbol;
		FIX::SymbolSfx symbolSfx;
		if(message.isSetField(symbolSfx))
		{
			message.get(symbolSfx);
			pTrade->sSymbol += "/";
			pTrade->sSymbol += symbolSfx;
		}

		FIX::Side side;
		message.get(side);
		pTrade->cSide = side;
		if(FIX::Side_SELL_SHORT == pTrade->cSide)
			pTrade->cSide = FIX::Side_SELL;
		else
			if(FIX::Side_BUY != pTrade->cSide && FIX::Side_SELL != pTrade->cSide)
			{
				_stprintf(szBuf, _T("Side = '%c'. Message ignored."), pTrade->cSide);
				throw  std::logic_error(szBuf);
			}
		
		FIX::LastShares lastShares;
		message.get(lastShares);
		pTrade->iLastShares =(int)lastShares;

		FIX::LastPx lastPx;
		message.get(lastPx);
		pTrade->fLastPrice = (float)lastPx;

		FIX::TransactTime transactTime;
		if(!message.isSetField(transactTime))
			throw std::logic_error("Failed to get value of TransactTime field.");
		message.get(transactTime);
		pTrade->ftTransactTime = Convert(transactTime);


		FIX::TradeDate tradeDate;
		if(!message.isSetField(tradeDate))
		{
			time_t tNow = time(NULL);
			struct tm* tmNow = gmtime(&tNow);
			pTrade->fdTradeDate.tm_year = tmNow->tm_year;
			pTrade->fdTradeDate.tm_mon = tmNow->tm_mon;
			pTrade->fdTradeDate.tm_mday = tmNow->tm_mday;
			pTrade->fdTradeDate.tm_hour = 0;
			pTrade->fdTradeDate.tm_min = 0;
			pTrade->fdTradeDate.tm_sec = 0;
		}
		else
		{
			message.get(tradeDate);
			pTrade->fdTradeDate = Convert(tradeDate);
		}

		FIX::SecurityType securityType;
		if(message.isSetField(securityType))
		{
			message.get(securityType);
			pTrade->sSecurityType = securityType;
		}
		else
			pTrade->sSecurityType = FIX::SecurityType_COMMON_STOCK;

		if(pTrade->sSecurityType == FIX::SecurityType_OPTION  ) 
		{
			if ( message.isSetField( 9001 ) ) {
				FIX::DoubleField fld( 9001 );
				bool bExist = true;
				try{ message.getField( fld ); }
				catch(...) { bExist = false; }
				if(bExist) pTrade->fStockRefPrice = static_cast<float>(fld);
			}
			FIX::MaturityMonthYear maturityMonthYear;
			if(!message.isSetField(maturityMonthYear))
				throw std::logic_error("Failed to get value of MaturityMonthYear field.");
			message.get(maturityMonthYear);
			pTrade->fmyMaturityMonthYear = Convert(maturityMonthYear);

			FIX::PutOrCall putOrCall;
			if(!message.isSetField(putOrCall))
				throw std::logic_error("Failed to get value of PutOrCall field.");
			message.get(putOrCall);
			pTrade->iPutOrCall = putOrCall;

			FIX::StrikePrice strikePrice;
			if(!message.isSetField(strikePrice))
				throw std::logic_error("Failed to get value of StrikePrice field.");
			message.get(strikePrice);
			pTrade->fStrikePrice = (float)strikePrice;

			FIX::MaturityDay maturityDay;
			if(message.isSetField(maturityDay))
			{
				message.get(maturityDay);
				pTrade->fmdMaturityDay = Convert(maturityDay);
			}
			else
				pTrade->fmdMaturityDay.tm_mday  = 15;

			try
			{
				pTrade->sUnderlying = "";
				FIX::UnderlyingSymbol undSymbol;
				if(message.isSetField(undSymbol))
				{
					pTrade->sUnderlying = message.getField(undSymbol).getString();
					FIX::UnderlyingSymbolSfx undSymbolSfx;
					if(message.isSetField(undSymbolSfx))
					{
						pTrade->sUnderlying += "/";
						pTrade->sUnderlying += message.getField(undSymbolSfx).getString();
					}
				}
			}
			catch (...)
			{
				pTrade->sUnderlying = "";
				/*throw std::logic_error("UnderlyingSymbol field missing");*/
			}
		
			pTrade->sRoot = pTrade->sSymbol;
			pTrade->MakeOptionSymbol();
		}
		else if (pTrade->sSecurityType == FIX::SecurityType_FUTURE && !m_bFilterOutFutureTrades)
		{
			FIX::MaturityMonthYear maturityMonthYear;
			if(!message.isSetField(maturityMonthYear))
				throw std::logic_error("Failed to get value of MaturityMonthYear field.");
			message.get(maturityMonthYear);
			pTrade->fmyMaturityMonthYear = Convert(maturityMonthYear);

			FIX::MaturityDay maturityDay;
			if(message.isSetField(maturityDay))
			{
				message.get(maturityDay);
				pTrade->fmdMaturityDay = Convert(maturityDay);
			}
			else
				pTrade->fmdMaturityDay.tm_mday  = 15;

			try
			{
				pTrade->sUnderlying = "";
				FIX::UnderlyingSymbol undSymbol;
				if(message.isSetField(undSymbol))
				{
					pTrade->sUnderlying = message.getField(undSymbol).getString();
					FIX::UnderlyingSymbolSfx undSymbolSfx;
					if(message.isSetField(undSymbolSfx))
					{
						pTrade->sUnderlying += "/";
						pTrade->sUnderlying += message.getField(undSymbolSfx).getString();
					}
				}
			}
			catch (...)
			{
				pTrade->sUnderlying = "";
			}
			pTrade->MakeFuturesSymbolRoot();
		}
		else if(pTrade->sSecurityType == FIX::SecurityType_COMMON_STOCK || 
			pTrade->sSecurityType == FIX::SecurityType_MUTUAL_FUND || 
			pTrade->sSecurityType == FIX::SecurityType_PREFERRED_STOCK)
		{
			pTrade->sUnderlying = pTrade->sSymbol;
		}
		else
		{
			_stprintf(szBuf, _T("Security type = '%s'. Message ignored."), pTrade->sSecurityType.c_str());
			throw std::logic_error(szBuf);
		}

		FIX::Dictionary dicSettings = m_pSettings->get(sessionID);
		if(dicSettings.has("StrategyField"))
		{
			// Read Strategy Custom Field
			long lStrategyField = dicSettings.getLong("StrategyField");
			if(lStrategyField > 0 && message.isSetField(lStrategyField))
			{
				std::string txtStrategy;
				txtStrategy = message.getField(lStrategyField);
				pTrade->sStrategy = txtStrategy;				
			}
		}


		// Text
		FIX::Text text;
		if(message.isSetField(text))
		{
			message.get(text);
			pTrade->sText = text;
			CTracer::Trace(_T("!!! enFtText = '%s'"), pTrade->sText.c_str());

		}
		for (DWORD i = 0; i < m_nGroupsCount; i++)
		{
			if(!bCorrection)
				m_Publisher[i].PushData(pTrade);
			else
			{
				pTrade->cExecTransType = FIX::ExecTransType_CANCEL;
				m_Publisher[i].PushData(pTrade);

				SwitchToThread();
				CTradePtr pNewTrade = CTradePtr(new CTrade(pTrade.get()));
				pNewTrade->cExecTransType = FIX::ExecTransType_NEW;
				pNewTrade->sExecID = sCorrectionID;
				m_Publisher[i].PushData(pNewTrade);
			}
		}
	}
	catch (std::logic_error& err)
	{
		FIX41::Reject     rejectMessage(message);
		rejectMessage.set(FIX::Text(err.what()));
		rejectMessage.getHeader().set(FIX41::Reject::MsgType());
		rejectMessage.reverseRoute(message);
		FIX::Session::sendToTarget(rejectMessage, sessionID);
	}
}

void CSageConnector::onMessage(const FIX42::ExecutionReport& message, const FIX::SessionID& sessionID)
{
	char szBuf[2048];
	ZeroMemory(szBuf, sizeof(szBuf));
	size_t sizeBuf = sizeof(szBuf)-1;
	bool bCorrection = false;
	string sCorrectionID;
	try
	{
		CTradePtr pTrade = CTradePtr(new CTrade());

		FIX::ExecTransType execTransType;
		message.get(execTransType);
		pTrade->cExecTransType = execTransType;

		FIX::ExecID execID;
		message.get(execID);
		pTrade->sExecID = execID;

		if(pTrade->cExecTransType != FIX::ExecTransType_NEW && pTrade->cExecTransType != FIX::ExecTransType_CANCEL && pTrade->cExecTransType != FIX::ExecTransType_CORRECT /*&& Trade->cExecTransType != ETT_CORRECT*/)
		{
			_sntprintf(szBuf,sizeBuf, _T("ExecTransType = '%c'. Message ignored."), pTrade->cExecTransType);
			throw std::logic_error(szBuf);
		}
		if(pTrade->cExecTransType == FIX::ExecTransType_CANCEL)
		{
			FIX::ExecRefID execRefID;
			if(!message.isSetField(execRefID))
				throw std::logic_error("ExecRefID field missing");

			message.get(execRefID);
			pTrade->sExecID =  execRefID;
		}
		if(pTrade->cExecTransType == FIX::ExecTransType_CORRECT)
		{
			bCorrection = true;
			FIX::ExecRefID execRefID;
			if(!message.isSetField(execRefID))
				throw std::logic_error("ExecRefID field missing");

			message.get(execRefID);
			pTrade->sExecID = execRefID;
			sCorrectionID =	  execID;
		}


		FIX::OrdStatus ordStatus;
		message.get(ordStatus);
		pTrade->cOrderStatus = ordStatus;

//		if(pTrade->cOrderStatus != FIX::OrdStatus_PARTIALLY_FILLED && pTrade->cOrderStatus != FIX::OrdStatus_FILLED)
//		{
//			_sntprintf(szBuf,sizeBuf, _T("OrdStatus = '%c'. Message ignored."), pTrade->cOrderStatus);
//			throw std::logic_error(szBuf);
//		}

		FIX::Account account;
		if(!message.isSetField(account))
			throw std::logic_error("Failed to get value of Account field.");
		message.get(account);
		if(m_pSettings->get(sessionID).has(_T("TraderOverride")))
		{
			pTrade->sAccount = m_pSettings->get(sessionID).getString(_T("TraderOverride"));
			if(m_pSettings->get(sessionID).has(account))
				pTrade->sBroker = m_pSettings->get(sessionID).getString(account);
		}
		else
			pTrade->sAccount = account;


		CTracer::Trace(_T("Trade received ExecID = '%s'"), pTrade->sExecID.c_str());

		for (DWORD i = 0; i < m_nGroupsCount; i++)
		{
			InterlockedIncrement((LPLONG)m_Publisher[i].GetCounterReceived());
		}

		FIX::Symbol symbol;
		message.get(symbol);
		pTrade->sSymbol = symbol;
		FIX::SymbolSfx symbolSfx;
		if(message.isSetField(symbolSfx))
		{
			message.get(symbolSfx);
			pTrade->sSymbol += "/";
			pTrade->sSymbol += symbolSfx;
		}
		else
		{
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// Here is a hack due some FIX servers do not send underlying asset class 
			// in tag 312 but add it into tag 311 using '.' symbol
			checkAndCorrectEquityAssetSymbol(pTrade->sUnderlying);
		}		


		FIX::Side side;
		message.get(side);
		pTrade->cSide = side;
		if(FIX::Side_SELL_SHORT == pTrade->cSide)
			pTrade->cSide = FIX::Side_SELL;
		else
			if(FIX::Side_BUY != pTrade->cSide && FIX::Side_SELL != pTrade->cSide)
			{
				_stprintf(szBuf, _T("Side = '%c'. Message ignored."), pTrade->cSide);
				throw  std::logic_error(szBuf);
			}

		FIX::LastShares lastShares;
		message.get(lastShares);
		pTrade->iLastShares =(int)lastShares;

		FIX::LastPx lastPx;
		message.get(lastPx);
		pTrade->fLastPrice = (float)lastPx;

		FIX::TransactTime transactTime;
		if(!message.isSetField(transactTime))
			throw std::logic_error("Failed to get value of TransactTime field.");
		message.get(transactTime);
		pTrade->ftTransactTime = Convert(transactTime);


		FIX::TradeDate tradeDate;
		if(!message.isSetField(tradeDate))
		{
			time_t tNow = time(NULL);
			struct tm* tmNow = gmtime(&tNow);
			pTrade->fdTradeDate.tm_year = tmNow->tm_year;
			pTrade->fdTradeDate.tm_mon = tmNow->tm_mon;
			pTrade->fdTradeDate.tm_mday = tmNow->tm_mday;
			pTrade->fdTradeDate.tm_hour = 0;
			pTrade->fdTradeDate.tm_min = 0;
			pTrade->fdTradeDate.tm_sec = 0;
		}
		else
		{
			message.get(tradeDate);
			pTrade->fdTradeDate = Convert(tradeDate);
		}

		FIX::SecurityType securityType;
		if(message.isSetField(securityType))
		{
			message.get(securityType);
			pTrade->sSecurityType = securityType;
		}
		else
			pTrade->sSecurityType = FIX::SecurityType_COMMON_STOCK;

		if(pTrade->sSecurityType == FIX::SecurityType_OPTION)
		{
			if ( message.isSetField( 9001 ) ) {
				FIX::DoubleField fld( 9001 );
				bool bExist = true;
				try{ message.getField( fld ); }
				catch(...) { bExist = false; }
				if(bExist) pTrade->fStockRefPrice = static_cast<float>(fld);
			}

			FIX::MaturityMonthYear maturityMonthYear;
			if(!message.isSetField(maturityMonthYear))
				throw std::logic_error("Failed to get value of MaturityMonthYear field.");
			message.get(maturityMonthYear);
			pTrade->fmyMaturityMonthYear = Convert(maturityMonthYear);


			FIX::PutOrCall putOrCall;
			if(!message.isSetField(putOrCall))
				throw std::logic_error("Failed to get value of PutOrCall field.");
			message.get(putOrCall);
			pTrade->iPutOrCall = putOrCall;

			FIX::StrikePrice strikePrice;
			if(!message.isSetField(strikePrice))
				throw std::logic_error("Failed to get value of StrikePrice field.");
			message.get(strikePrice);
			pTrade->fStrikePrice = (float)strikePrice;

			FIX::MaturityDay maturityDay;
			if(message.isSetField(maturityDay))
			{
				message.get(maturityDay);
				pTrade->fmdMaturityDay = Convert(maturityDay);
			}
			else
				pTrade->fmdMaturityDay.tm_mday  = 15;

			try
			{
				pTrade->sUnderlying = "";
				FIX::UnderlyingSymbol undSymbol;
				if(message.isSetField(undSymbol))
				{
					pTrade->sUnderlying = message.getField(undSymbol).getString();
					
										
					FIX::UnderlyingSymbolSfx undSymbolSfx;					
					if(message.isSetField(undSymbolSfx))
					{
						pTrade->sUnderlying += "/";
						pTrade->sUnderlying += message.getField(undSymbolSfx).getString();
					}
					else
					{
						// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						// Here is a hack due some FIX servers do not send underlying asset class 
						// in tag 312 but add it into tag 311 using '.' symbol
						checkAndCorrectEquityAssetSymbol(pTrade->sUnderlying);
					}
				}
			}
			catch (...)
			{
				pTrade->sUnderlying = "";
				/*throw std::logic_error("UnderlyingSymbol field missing");*/
			}

			pTrade->sRoot = pTrade->sSymbol;
			pTrade->MakeOptionSymbol();
		}
		else if (pTrade->sSecurityType == FIX::SecurityType_FUTURE && !m_bFilterOutFutureTrades)
		{
			FIX::MaturityMonthYear maturityMonthYear;
			if(!message.isSetField(maturityMonthYear))
				throw std::logic_error("Failed to get value of MaturityMonthYear field.");
			message.get(maturityMonthYear);
			pTrade->fmyMaturityMonthYear = Convert(maturityMonthYear);

			FIX::MaturityDay maturityDay;
			if(message.isSetField(maturityDay))
			{
				message.get(maturityDay);
				pTrade->fmdMaturityDay = Convert(maturityDay);
			}
			else
				pTrade->fmdMaturityDay.tm_mday  = 15;

			try
			{
				pTrade->sUnderlying = "";
				FIX::UnderlyingSymbol undSymbol;
				if(message.isSetField(undSymbol))
				{
					pTrade->sUnderlying = message.getField(undSymbol).getString();
					FIX::UnderlyingSymbolSfx undSymbolSfx;
					if(message.isSetField(undSymbolSfx))
					{
						pTrade->sUnderlying += "/";
						pTrade->sUnderlying += message.getField(undSymbolSfx).getString();
					}
				}
			}
			catch (...)
			{
				pTrade->sUnderlying = "";
			}
			pTrade->MakeFuturesSymbolRoot();
		}
		else if(pTrade->sSecurityType == FIX::SecurityType_COMMON_STOCK || 
			pTrade->sSecurityType == FIX::SecurityType_MUTUAL_FUND || 
			pTrade->sSecurityType == FIX::SecurityType_PREFERRED_STOCK)
		{
			pTrade->sUnderlying = pTrade->sSymbol;
		}
		else
		{
			_stprintf(szBuf, _T("Security type = '%s'. Message ignored."), pTrade->sSecurityType.c_str());
			throw std::logic_error(szBuf);	
		}


		FIX::Dictionary dicSettings = m_pSettings->get(sessionID);
		if(dicSettings.has("StrategyField"))
		{
			// Read Strategy Custom Field
			long lStrategyField = dicSettings.getLong("StrategyField");
			if(lStrategyField > 0 && message.isSetField(lStrategyField))
			{
				std::string txtStrategy;
				txtStrategy = message.getField(lStrategyField);
				pTrade->sStrategy = txtStrategy;				
			}
		}


		// Text
		FIX::Text text;
		if(message.isSetField(text))
		{
			message.get(text);
			pTrade->sText = text;
			CTracer::Trace(_T("!!! enFtText = '%s'"), pTrade->sText.c_str());

		}

		for (DWORD i = 0; i < m_nGroupsCount; i++)
		{
			if(!bCorrection)
				m_Publisher[i].PushData(pTrade);
			else
			{
				pTrade->cExecTransType = FIX::ExecTransType_CANCEL;
				m_Publisher[i].PushData(pTrade);

				SwitchToThread();
				CTradePtr pNewTrade = CTradePtr(new CTrade(pTrade.get()));
				pNewTrade->cExecTransType = FIX::ExecTransType_NEW;
				pNewTrade->sExecID = sCorrectionID;
				m_Publisher[i].PushData(pNewTrade);

			}
		}
	}
	catch (std::logic_error& err)
	{
		FIX42::Reject     rejectMessage(message);
		rejectMessage.set(FIX::Text(err.what()));
		rejectMessage.getHeader().set(FIX42::Reject::MsgType());
		rejectMessage.reverseRoute(message);
		FIX::Session::sendToTarget(rejectMessage, sessionID);
	}
}

void CSageConnector::checkAndCorrectEquityAssetSymbol(std::string &symbol)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Here is a hack due some FIX servers do not send underlying asset class 
	// in tag 65 but add it into tag 55 using '.' or ' ' symbols
	size_t dotPosition = symbol.find('.');					
	size_t spacePosition = symbol.find(' ');

	if (dotPosition != string::npos && spacePosition != string::npos)			
	{
		CTracer::Trace(_T("ERROR!!! Base asset symbol '%s' has absolutely unaccepteble format. "
							"Dot and space letters are both in place. Contact broker why it happened"), 
							symbol.c_str());		
	}
	else
	{	
		// either dot or space will be replaced for slash
		size_t replacePosition = (dotPosition != string::npos) ? dotPosition : spacePosition;

		CTracer::Trace(_T("WARNING!!! Base asset symbol '%s' has formated worng. "
			"Hack code will be applied. Dot/space will be replaced for slash"),
			symbol.c_str());

		symbol[replacePosition] = '/';
		
	}
}
