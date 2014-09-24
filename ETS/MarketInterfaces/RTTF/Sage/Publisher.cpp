#include "stdafx.h"
#include "oledberr.h"
#include "SgConst.h"
#include "Publisher.h"
#include "Trace.h"
#include "resource_GUI.h"
#include "Win32Error.h"
#include "Sage.h"

/////////////////////////////////////////////////////////////////////////////////////

#define LCID_ENGLISH_USA  MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT )

// time utils
int MonthDays[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

/////////////////////////////////////////////////////////////////////////////////////
BOOL OleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
	WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return FALSE;

	BOOL bLeapYear = ((wYear & 3) == 0) &&
		((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth =
		MonthDays[wMonth] - MonthDays[wMonth-1] +
		((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

	if (wDay < 1 || wDay > nDaysInMonth ||
		wHour > 23 || wMinute > 59 ||
		wSecond > 59)
	{
		return FALSE;
	}

	long nDate;
	double dblTime;

	nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 +
		MonthDays[wMonth-1] + wDay;

	if (wMonth <= 2 && bLeapYear)
		--nDate;

	nDate -= 693959L;

	dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		((long)wMinute * 60L) +  // mins in seconds
		((long)wSecond)) / 86400.;

	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
LPCSTR g_cMonthName[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

/////////////////////////////////////////////////////////////////////////////////////
void CTrade::MakeOptionSymbol()
{
    
	if(sSecurityType == FIX::SecurityType_OPTION && sSymbol.length() > 0)
	{
		char szSym[21] = { 0 };

		_snprintf(szSym, sizeof(szSym) - 1, "%s%s%d%s",
					sSymbol.c_str(),
					g_cMonthName[fmyMaturityMonthYear.tm_mon >= 0 && fmyMaturityMonthYear.tm_mon < 12 ? fmyMaturityMonthYear.tm_mon : 0],
					(long)fStrikePrice,
					(iPutOrCall ? "C" : "P"));
		
		if(strlen(szSym))
		{
			sSymbol = szSym;
		}
	}
}

void CTrade::MakeFuturesSymbolRoot()
{
	if(sSecurityType == FIX::SecurityType_FUTURE&& sSymbol.length() > 0)
	{
		char szSym [21] = { 0 };
		int year;
		char month;
	
		if ((sSymbol.c_str()[sSymbol.size()-2] >='0')&& (sSymbol.c_str()[sSymbol.size()-2] <='9'))
		{
			year = (sSymbol.c_str()[sSymbol.size()-2] - '0')*10 + sSymbol.c_str()[sSymbol.size()-1]- '0';
			sSymbol.resize(sSymbol.size()-2);
		}
		else
		{
			year = sSymbol.c_str()[sSymbol.size()-1]- '0';
			sSymbol.resize(sSymbol.size()-1);
		}
		month = sSymbol.c_str()[sSymbol.size()-1];
		sSymbol.resize(sSymbol.size()-1);
		_snprintf(szSym, sizeof(szSym) + 1, "%s%s%d%c",
			sSymbol.c_str(), 
			"/0",
			year,
			month);
		if (sSymbol.size())
			sRoot = sSymbol;
		if(strlen(szSym))
		{
			sSymbol = szSym;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// CPublisher class implementation
/////////////////////////////////////////////////////////////////////////////////////
CPublisher::CPublisher() 
{ 		
	m_dwDBStored = 0;
	m_dwDBDeleted = 0;
	m_dwPublished = 0;
	m_dwReceived = 0;
}

CPublisher::~CPublisher() 
{ 
    m_fTradesFile.close();
    m_fFailesFile.close();
    m_fStructFile.close();
}

HRESULT CPublisher::InitInfromationStream()
{

	CString sTradesFile;
	CString sFailesFile;
	CString sStructureFile;

	TCHAR szUserName[256] = { 0 };
	DWORD dwSize = 255;

	SYSTEMTIME st;
	::GetLocalTime(&st);

	TCHAR pTradesFile[100] = { 0 };
	TCHAR pFailesFile[100] = { 0 };
	TCHAR pFileDate[100] = { 0 };

	CString strSubjectPrefix = (LPTSTR)m_bsUserGroup;

	_stprintf(pFileDate, _T("%2.2i_%2.2i_%4.4i"), st.wDay, st.wMonth, st.wYear);

	sTradesFile.Format("%s_%s_%s%s", TRADES_FILENAME_BEGIN, strSubjectPrefix.GetString(), pFileDate, TRADES_FILENAME_END);
	sFailesFile.Format("%s_%s_%s%s", FAILES_FILENAME_BEGIN, strSubjectPrefix.GetString(), pFileDate, FAILES_FILENAME_END);
	sStructureFile.Format("%s_%s%s", STRUCTURE_FILENAME_BEGIN, strSubjectPrefix.GetString(), STRUCTURE_FILENAME_END);

	m_fTradesFile.open( sTradesFile.GetString(),    ios::app | ios::out );
	m_fFailesFile.open( sFailesFile.GetString(),    ios::app | ios::out );
	m_fStructFile.open( sStructureFile.GetString(), ios::app | ios::out );

	return S_OK;
}

HRESULT CPublisher::SetUserGroup(_bstr_t bsUserGroup)
{
	HRESULT dwResult = S_OK;
	
	if (bsUserGroup.length() > 0)
	{
		m_bsUserGroup = bsUserGroup;
	}
	else
	{
		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();
		XMLParams.GetUserGroup(m_bsUserGroup.GetAddress());
	}

	//init traces for incoming data
	dwResult = InitInfromationStream();

	return dwResult;
}

HRESULT CPublisher::OnStrategyInitialize()
{
	HRESULT hr =S_OK;
	hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	return hr;

}
void CPublisher::OnStrategyUninitialize()
{
	CoUninitialize();
}

void CPublisher::OnData(CTradePtr& pTrade)
{
	DWORD dwRes = StoreInDatabase(pTrade);
	if(dwRes == 0)
	{
		Publish(pTrade);
	}
	else
	{
		CTracer::Trace(_T("Trade with ExecID = '%s' was not published in '%s'."), 
						pTrade->sExecID.c_str(),
						(LPCTSTR)m_bsUserGroup);
	}

	switch(dwRes)
	{
	case 0:
		pTrade->sStatus = "OK";
		break;

	case ERROR_ALREADY_EXISTS:
		pTrade->sStatus = "ALREADYEXISTS";
		break;

	case ERROR_FILE_NOT_FOUND:
		pTrade->sStatus = "NOTFOUND";
		break;

	case ERROR_DATABASE_FAILURE:
		pTrade->sStatus = "REPROCESSFAIL";
		break;

	default:
		pTrade->sStatus = "ERROR";
		break;
	}

	StoreInFile(pTrade);
}



DWORD CPublisher::Start()
{
	LONG lRes = GetConnectionString();
	if(lRes != S_OK)
		return CTracer::TraceError(lRes, _T("Fail to read connection string from the registry"));

	if(!m_fStructFile)
		return CTracer::TraceError(ERROR_OPEN_FAILED, _T("Fail to open structure file."));

	if(!m_fTradesFile)
		return CTracer::TraceError(ERROR_OPEN_FAILED, _T("Fail to open trades file."));

	// Database...
	CTracer::Trace(_T("Connecting to database..."));

	try
	{
		m_pConnection = CDBConnectionPtr(new CDBConnection());
        m_pConnection->Open(m_bsConnectionString, 5, 10, 250, 250);

		_bstr_t bsInitialCatalog = m_pConnection->GetAdoConnectionPtr()->Properties->Item[L"Initial Catalog"]->Value;
		_bstr_t bsDataSource = m_pConnection->GetAdoConnectionPtr()->Properties->Item[L"Data Source"]->Value;

		CTracer::Trace(_T("------------------------------------"));
		CTracer::Trace(_T(" - DB host: %s"), (LPCTSTR)bsDataSource );	
		CTracer::Trace(_T(" - DB name: %s"), (LPCTSTR)bsInitialCatalog );	
		CTracer::Trace(_T(" - Group  : %s"), (LPCTSTR)m_bsUserGroup );
		CTracer::Trace(_T("------------------------------------"));

	}
	catch(_com_error& e)
	{
		m_pConnection->Close();
		m_pConnection.reset();

		_bstr_t bsError = L"Failed to connect to database : " + e.Description();		
		CTracer::Trace(bsError);

		return e.Error();
	}
	
	CTracer::Trace(_T("Connection to database is established."));

	// Transport...
	try
	{
		CTracer::Trace(_T("Initializing publisher..."));

		HRESULT hr = m_spPubManager.CreateInstance(__uuidof (MsgManager));
		if(FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create MsgManager object");

		hr = m_spTradeUpdate.CreateInstance(__uuidof (TradeUpdate));
		if(FAILED(hr))			
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create TradeUpdate object");
        
        hr = m_spBroadcastMessage.CreateInstance(__uuidof (BroadcastMessage));
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create BroadcastMessage object.");

		m_spPubManager->PutUserGroup(m_bsUserGroup); 
		m_spPubManager->RegPublisher(enMtTradeUpdate, NULL);

		CTracer::Trace(_T("Publisher initialized."));
	}
	catch(_com_error& e)
	{
		return CTracer::TraceError(e.Error(), (LPCSTR)e.Description() );
	}

	return EgLib::CStrategyT<CPublisher, CTradePtr>::Start();
};

DWORD CPublisher::Stop()
{
	EgLib::CStrategyT<CPublisher, CTradePtr>::Stop();
	try
	{
		if(m_pConnection != NULL)
		{
			m_pConnection->Close();
			m_pConnection.reset();
		}
	}
	catch(_com_error& /*e*/){}

	try
	{
		if(m_spPubManager)
			m_spPubManager->UnregPublisher(enMtTradeUpdate, NULL);

		m_spTradeUpdate = NULL;
		m_spPubManager = NULL;
	}
	catch(_com_error&)
	{
	}

	return 0;
}

HRESULT CPublisher::FillTradeUpdate(CClientRecordset& rs, const CTradePtr& pTrade)
{
	try
	{
		m_spTradeUpdate->SeqNum = rs[L"iSeqNum"];
		if(m_spTradeUpdate->SeqNum == 0)
		{
			CTracer::Trace("Duplicated trade");
			_com_issue_error(S_FALSE);
		}

		m_spTradeUpdate->TradeID = rs[L"iTradeID"];
		m_spTradeUpdate->Action = (TRADE_ACTION)(int)rs[L"tiActionID"];
		m_spTradeUpdate->TradeStatus = (TRADE_STATUS)(long)rs[L"tiStatus"];
		m_spTradeUpdate->Mark = rs[L"iMark"];
		m_spTradeUpdate->TradeDate = rs[L"dtTradeDate"];
		m_spTradeUpdate->TraderID = rs[L"iTraderID"];
		m_spTradeUpdate->SymbolID = rs[L"iContractID"];
		m_spTradeUpdate->Quantity = rs[L"iQuantity"];
		m_spTradeUpdate->Price = rs[L"fPrice"];
		m_spTradeUpdate->IsBuy = rs[L"tiIsBuy"];
		m_spTradeUpdate->StrategyID = rs[L"iStrategyID"];
		m_spTradeUpdate->BrokerID = rs[L"iBrokerID"];
		m_spTradeUpdate->BrokerCommission = rs[L"fBrokerCommission"];
		m_spTradeUpdate->ClearingBrokerID = rs[L"iClearingBrokerID"];
		m_spTradeUpdate->ClearingBrokerCommission = rs[L"fClearingBrokerCommission"];
		m_spTradeUpdate->SpotReference = rs[L"fSpotReference"];
		m_spTradeUpdate->TradedIV = rs[L"fTradedIV"];

		int iContractTypeID = rs[L"iContractTypeID"];
		switch(iContractTypeID)
		{
			case 1: 
				m_spTradeUpdate->SymbolType = enStIndex; 
				break;

			case 2: 
				m_spTradeUpdate->SymbolType = enStStock; 
				break;
			
			case 3: 
				m_spTradeUpdate->SymbolType = enStOption; 
				break;

			case 4: 
				m_spTradeUpdate->SymbolType = enStFuture; 
				break;

			case 5: 
				m_spTradeUpdate->SymbolType = enStFutureOption; 
				break;

			default:
                {
					CTracer::Trace(_T("Error. Unknown SymbolType for ContractTypeID = %d."), iContractTypeID);
					_com_issue_error(E_FAIL);
				    break;
                }
		}

		m_spTradeUpdate->Symbol = rs[L"vcSymbol"];
		m_spTradeUpdate->UndSymbolID = rs[L"iUnderlyingContractID"];
		m_spTradeUpdate->UndSymbol = rs[L"vcUnderlyingSymbol"];
		m_spTradeUpdate->Expiry = rs[L"dtExpiry"];
		m_spTradeUpdate->ExpiryOV = rs[L"dtExpiryOV"];
		double dtTradingClose = rs[L"dtTradingClose"];
		m_spTradeUpdate->TradingClose = dtTradingClose - floor(dtTradingClose);
		m_spTradeUpdate->IsCall = rs[L"tiIsCall"];
		m_spTradeUpdate->Strike = rs[L"fStrike"];
		m_spTradeUpdate->PriceClose = rs[L"fPriceClose"];
		m_spTradeUpdate->PriceTheoClose = rs[L"fPriceTheoClose"];
		m_spTradeUpdate->LotSize = rs[L"iLotSize"];
        m_spTradeUpdate->OptionRootID = rs[L"iOptionRootID"];
		m_spTradeUpdate->FutID =  rs[L"iFutureID"];
		m_spTradeUpdate->FutRootID =  rs[L"iFutureRootID"];

		if(pTrade->cExecTransType == FIX::ExecTransType_NEW)
		{
			long lStructureMissed = rs[L"iStructureMissed"];
			m_spTradeUpdate->MsState = (TRADE_MS_STATE)lStructureMissed;

			if(lStructureMissed)
			{
				_TCHAR szBuf[1024];
				
				switch(lStructureMissed)
				{
					case enTmssNewStock:
						_stprintf(szBuf, _T("New stock '%s' added.\n"), pTrade->sUnderlying.c_str());
						break;

					case enTmssNewOption:
						_stprintf(szBuf, _T("New option(future) '%s' for stock '%s' added.\n"),
							pTrade->sSymbol.c_str(), pTrade->sUnderlying.c_str());
						break;

					case enTmssNewStock | enTmssNewOption:
						_stprintf(szBuf, _T("New stock '%s' and new option '%s' for this stock are added.\n"),
							pTrade->sUnderlying.c_str(), pTrade->sSymbol.c_str());
						break;

					default:
						_tcscpy(szBuf, _T("Unexpected structure error."));
				}

				SYSTEMTIME st;
				::GetLocalTime(&st);
				_TCHAR szTime[32];
				_stprintf(szTime, _T("%d/%d/%d-%d:%d:%d "), st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

				m_fStructFile << T2A(szTime) << T2A(szBuf) << flush;
			}
		}
	}
	catch(_com_error& e)
	{
		CTracer::TraceError(e.Error(), (LPCSTR)e.Description());
		return e.Error();
	}

	return S_OK;
}

DWORD CPublisher::StoreInDatabase(const CTradePtr& pTrade)
{

	DWORD	dwTries = MAX_DBRECONNECTION_TRIES;
	bool	bOk = false;
	DWORD	dwRes = ERROR_SUCCESS;
    long    lMaxSeqNum;
    long    lMinSeqNum;
    long    lStructureMissed;
    TDBFailsStore::iterator it;

	do
	{
		if (m_pConnection==NULL)
		{
			Reconnect();
			
			if (m_pConnection==NULL)
			{
				if (!--dwTries)
				{
					FlushTrade(pTrade);
					BroadcastError(ERROR_REMOTE_STORAGE_NOT_ACTIVE, "Unable to open the database connection");
					dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
					bOk = true;
					break;
				}
			}
		}
		else
		{
			try
			{
				switch(pTrade->cExecTransType)
				{
				case FIX::ExecTransType_NEW:
					{                    
						CClientRecordset rs;
						CStoredProc<> sp(*m_pConnection.get(), L"usp_TradeFeed_1_New");

						sp << _bstr_t(pTrade->sAccount.c_str());						//@vcTraderAcronym
						sp << _bstr_t(pTrade->sExecID.c_str());							//@vcExecID
						sp << ((pTrade->cSide == FIX::Side_BUY) ? (int)1 : int(0));		//@tiIsBuy
						sp << pTrade->iLastShares;										//@iQuantity
						sp << pTrade->fLastPrice;										//@fPrice

						DATE dtTradeDate;

						OleDateFromTm(pTrade->ftTransactTime.tm_year + 1900, 
							pTrade->ftTransactTime.tm_mon + 1, 
							pTrade->ftTransactTime.tm_mday, 
							pTrade->ftTransactTime.tm_hour, 
							pTrade->ftTransactTime.tm_min, 
							pTrade->ftTransactTime.tm_sec, 
							dtTradeDate );

						sp << vt_date(dtTradeDate);								//@dtTradeDate
						sp << _bstr_t(pTrade->sSymbol.c_str());					//@vcContractSymbol

						if(pTrade->sSecurityType != FIX::SecurityType_OPTION && pTrade->sSecurityType != FIX::SecurityType_FUTURE)
						{
							sp << (int)2;
							sp << CDBNull();									//@vcUnderlyingSymbol
							sp << CDBNull();									//@tiIsCall	
							sp << CDBNull();									//@fStrike
							sp << CDBNull();									//@dtExpiry
							sp << CDBNull();									//@iTradeLotSize
							sp << CDBNull();									//@tiIsUpdateEnabled
							sp << CDBNull();									//@iLastTradeTime
							sp << CDBNull();									//@vcOptRootSymbol
						}
						else if (pTrade->sSecurityType == FIX::SecurityType_OPTION)
						{
							sp << (int)3;										//@iContractTypeID
							if(pTrade->sUnderlying.length())					//@vcUnderlyingSymbol
								sp << _bstr_t(pTrade->sUnderlying.c_str());
							else
								sp << CDBNull(); // No underlying supplied

							sp << (pTrade->iPutOrCall == FIX::PutOrCall_CALL);	//@tiIsCall	
							sp << pTrade->fStrikePrice;							//@fStrike
	
							DATE dtExpiryDate;
							OleDateFromTm(pTrade->fmyMaturityMonthYear.tm_year + 1900, 
								pTrade->fmyMaturityMonthYear.tm_mon + 1, 
								pTrade->fmdMaturityDay.tm_mday, 
								0, 0, 0, 
								dtExpiryDate);

							sp << vt_date(dtExpiryDate);						//@dtExpiry
							sp << CDBNull();									//@iTradeLotSize
							sp << CDBNull();									//@tiIsUpdateEnabled
							sp << CDBNull();									//@iLastTradeTime
							sp << _bstr_t(pTrade->sRoot.c_str());				//@vcOptRootSymbol
						}
						else 
						{
							sp << (int)4;										//@iContractTypeID
							if(pTrade->sUnderlying.length())					//@vcUnderlyingSymbol
								sp << _bstr_t(pTrade->sUnderlying.c_str());	
							else
								sp << CDBNull();							
							sp << CDBNull();									//@tiIsCall	
							sp << CDBNull();									//@fStrike

							DATE dtExpiryDate;
							OleDateFromTm(pTrade->fmyMaturityMonthYear.tm_year + 1900, 
								pTrade->fmyMaturityMonthYear.tm_mon + 1, 
								pTrade->fmdMaturityDay.tm_mday, 
								0, 0, 0, 
								dtExpiryDate);

							sp << vt_date(dtExpiryDate);						//@dtExpiry
							sp << CDBNull();									//@iTradeLotSize
							sp << CDBNull();									//@tiIsUpdateEnabled
							sp << CDBNull();									//@iLastTradeTime
							sp << _bstr_t(pTrade->sRoot.c_str());				//@vcOptRootSymbol
						}
						sp << CDBNull();										//@iMark

						if(pTrade->sStrategy.length()>0)						//@vcStrategyName
							sp << _bstr_t(pTrade->sStrategy.c_str());										
						else
							sp << CDBNull();									

						if ( pTrade->fStockRefPrice > 0.0 )						//@fStockRefPrice
							sp << pTrade->fStockRefPrice;
						else
							sp << CDBNull();									

						sp << CDBNull();										//@vcOrderID
						sp << CDBNull();										//@iExecStatus
						if(pTrade->sBroker.length())							//@vcBrokerName
							sp << _bstr_t(pTrade->sBroker.c_str());
						else 
							sp << CDBNull();

						// execute SQL command
						rs.Open (sp);
						
						// if nothing has been returned then fail operation
						if (rs.IsEOF())
						{
							rs.Close();
							_com_issue_error( E_FAIL );
						}                        

						lMaxSeqNum = rs[L"iMaxSeqNum"];
						lMinSeqNum = rs[L"iMinSeqNum"];
						lStructureMissed = rs[L"iStructureMissed"];
						rs.Close();

						if ( lMaxSeqNum == -1 && lMinSeqNum == -1 && lStructureMissed == -1) // unknown error
							_com_issue_error( E_FAIL );
							
						if ( lMaxSeqNum == -1 && lStructureMissed == -1 ) //trade already exists
						{
							dwRes = ERROR_ALREADY_EXISTS;                            
							
							if (ReProcessTrade(pTrade, lMinSeqNum) == ERROR_SUCCESS)
							{
								dwRes = ERROR_SUCCESS;
							}	                            
							
							break;
						}

						CTracer::Trace(_T("Trade '%s' stored in database '%s'."), 
										pTrade->sExecID.c_str(), 
										(LPCTSTR)m_bsConnStringLabel);

						InterlockedIncrement((LPLONG)&m_dwDBStored);

						CStoredProc<> sp_get(*m_pConnection.get(), L"usp_TradeSeq_Get");
						sp_get << (int)lMaxSeqNum;
						sp_get << (int)lMaxSeqNum;
						sp_get << CDBNull();
						sp_get << (unsigned char)1;

						rs.Open (sp_get);

						if(rs.IsEOF())
						{
							CTracer::Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist in '%s'."),
											pTrade->sExecID.c_str(),
											(LPCTSTR)m_bsConnStringLabel);
							dwRes = ERROR_NO_DATA_DETECTED;
						}
						else
						{
							HRESULT hRes;
							if(FAILED(hRes = FillTradeUpdate(rs, pTrade)))
							{
								CTracer::Trace(_T("Failed to fill TradeUpdate message."));
								dwRes = ERROR_INVALID_PARAMETER;
							}
							else if(hRes == S_FALSE)
							{
								CTracer::Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist in '%s'."),
												pTrade->sExecID.c_str(),
												(LPCTSTR)m_bsConnStringLabel);
								dwRes = ERROR_NO_DATA_DETECTED;
							}
						}

						break;
					}

				case FIX::ExecTransType_CANCEL:
					{
						CClientRecordset rs;
						CStoredProc<> sp(*m_pConnection.get(), L"usp_TradeFeed_1_Del");
						sp << _bstr_t(pTrade->sExecID.c_str());

						vt_date vtTradeDate(pTrade->fdTradeDate.tm_year + 1900, 
							pTrade->fdTradeDate.tm_mon + 1, 
							pTrade->fdTradeDate.tm_mday);

						sp << vtTradeDate;
						rs.Open (sp);
						if(rs.IsEOF())
						{
							rs.Close();
							_com_issue_error( E_FAIL );
						}

						lMaxSeqNum = rs[L"iMaxSeqNum"];
						lMinSeqNum = rs[L"iMinSeqNum"];
						rs.Close();

						if (lMaxSeqNum == -1 
							&& lMinSeqNum == -1) // unknown error
							_com_issue_error(E_FAIL);
						else                    
							if (lMinSeqNum == -1) // already exists
							{                            
								dwRes = ERROR_ALREADY_EXISTS;
								if ( ReProcessTrade(pTrade, lMaxSeqNum) == ERROR_SUCCESS )
									dwRes = ERROR_SUCCESS;
								break;
							}
							else
								if (lMaxSeqNum == -1)
								{
									CTracer::Trace(_T("Failed to store trade in database - trade with the same ExecID '%s' either does not exist in '%s'."),
													pTrade->sExecID.c_str(),
													(LPCTSTR)m_bsConnStringLabel);

									dwRes = ERROR_FILE_NOT_FOUND;
									break;
								}

								CTracer::Trace(_T("Trade '%s' deleted from database '%s'."), 
												pTrade->sExecID.c_str(),
												(LPCTSTR)m_bsConnStringLabel);

								InterlockedIncrement((LPLONG)&m_dwDBDeleted);

								CStoredProc<> sp_get(*m_pConnection.get(), L"usp_TradeSeq_Get");
								sp_get << (int)lMaxSeqNum;
								sp_get << (int)lMaxSeqNum;
								sp_get << CDBNull();
								sp_get << (unsigned char)1;

								rs.Open (sp_get);

								if(rs.IsEOF())
								{
									CTracer::Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
													pTrade->sExecID.c_str(),
													(LPCTSTR)m_bsConnStringLabel);

									dwRes = ERROR_NO_DATA_DETECTED;
								}
								else
								{
									HRESULT hRes;
									if(FAILED(hRes = FillTradeUpdate(rs, pTrade)))
									{
										CTracer::Trace(_T("Failed to fill TradeUpdate message."));
										dwRes = ERROR_INVALID_PARAMETER;
									}
									else if(hRes == S_FALSE)
									{
										CTracer::Trace(_T("Failed to retrieve trade from database - trade with the same ExecID '%s' does not exist."),
														pTrade->sExecID.c_str(),
														(LPCTSTR)m_bsConnStringLabel);

										dwRes = ERROR_NO_DATA_DETECTED;
									}                        
								}

								break;
					}

				default:
					dwRes = ERROR_INVALID_PARAMETER;
					break;
				}

				bOk = true;
			}
			catch(_com_error& e)
			{
				CString strError;
				strError.Format(_T("Failed to store trade in database Error: %X  %s %s"), 
								e.Error(), 
								(LPCTSTR)e.Description(),
								(LPCTSTR)m_bsConnStringLabel);

				CTracer::Trace(strError.GetString());

				pair<TDBFailsStore::iterator, bool> pairResult = 
					m_DBFails.insert(pTrade->sExecID);

				if(--dwTries)
				{				
					Reconnect();
				}
				else
				{
					CTracer::Trace(_T("CRITICAL ERROR : FAILED TO STORE TRADE IN DATABASE %s."), (LPCTSTR)m_bsConnStringLabel);

					if(/*Uncomment this Oleg StoreInFile(Trade) != 0*/false)
					{
						CTracer::Trace(_T("CRITICAL ERROR : FAILED TO STORE TRADE IN FILE."));
						CTracer::Trace(_T("MANUAL INTERVENTION IS REQUIRED. THE APPLICATION IS STOPPED."));
						ExitProcess(-1);
					}
					else
					{
						FlushTrade(pTrade);
						BroadcastError(e.Error(), strError.GetString());
						dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
						bOk = true;
					}
				}
			}
		}
	} while(!bOk);

	return dwRes;
};

DWORD CPublisher::StoreInFile(const CTradePtr& pTrade)
{
	TCHAR	szBuf[8192];
	string	sExecTransType;
	string	sOrderStatus;
	string	sSide;


	switch(pTrade->cExecTransType)
	{
	case FIX::ExecTransType_NEW: sExecTransType = "NEW"; break;
	case FIX::ExecTransType_CANCEL: sExecTransType = "CANCEL"; break;
	default: sExecTransType = "UNEXPECTED"; break;
	}

	switch(pTrade->cOrderStatus)
	{
	case FIX::OrdStatus_PARTIALLY_FILLED: sOrderStatus = "PARTIALLY FILLED"; break;
	case FIX::OrdStatus_FILLED: sOrderStatus = "FILLED"; break;
	default: sOrderStatus = "UNEXPECTED"; break;
	}

	switch(pTrade->cSide)
	{
	case FIX::Side_BUY: sSide = "BUY"; break;
	case FIX::Side_SELL: sSide = "SELL"; break;
	default: sSide = "UNEXPECTED"; break;
	}

	CFixTagValue	tagTransactTime = pTrade->ftTransactTime;
	string			sTransactTime((char*)tagTransactTime.GetData(), tagTransactTime.GetLength());

	CFixTagValue	tagTradeDate = pTrade->fdTradeDate;
	string			sTradeDate((char*)tagTradeDate.GetData(), tagTradeDate.GetLength());

	SYSTEMTIME st;
	::GetLocalTime(&st);
	

	_sntprintf(szBuf,sizeof(szBuf)-1, _T("%-2.2i/%-2.2i/%-4.4i %-2.2i:%-2.2i:%-2.2i\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%f\t")\
				   _T("%d\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%s\t")\
				   _T("%s\t"),

				   st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, 

				   pTrade->sStatus.c_str(),
				   
				   sExecTransType.c_str(),
				   pTrade->fLastPrice,
				   pTrade->iLastShares,
				   sOrderStatus.c_str(),
				   pTrade->sExecID.c_str(),
				   pTrade->sAccount.c_str(),
				   sSide.c_str(),
				   pTrade->sSymbol.c_str(),
				   pTrade->sSecurityType.c_str(),
				   sTransactTime.c_str(),
				   sTradeDate.c_str(),
				   pTrade->sText.c_str() == NULL ? "" : pTrade->sText.c_str()
				   );

	if(pTrade->sSecurityType == FIX::SecurityType_OPTION)
	{
		TCHAR szOptBuf[1024];

		CFixTagValue	tagMaturityMonthYear = pTrade->fmyMaturityMonthYear;
		string			sMaturityMonthYear((char*)tagMaturityMonthYear.GetData(), tagMaturityMonthYear.GetLength());

		CFixTagValue	tagMaturityDay = pTrade->fmdMaturityDay;
		string			sMaturityDay((char*)tagMaturityDay.GetData(), tagMaturityDay.GetLength());

		string sPutOrCall;

		switch(pTrade->iPutOrCall)
		{
		case FIX::PutOrCall_PUT :  sPutOrCall = "PUT"; break;
		case FIX::PutOrCall_CALL : sPutOrCall = "CALL"; break;
		default : sPutOrCall = "UNEXPECTED"; break;
		}

		_sntprintf(szOptBuf,sizeof(szOptBuf)-1, _T("%s%s\t%s\t%f\t%s\t%s\n"),

						  sMaturityMonthYear.c_str(),
						  sMaturityDay.c_str(),
						  sPutOrCall.c_str(),
						  pTrade->fStrikePrice,
						  pTrade->sUnderlying.c_str(),
						  pTrade->sBroker.length()?pTrade->sBroker.c_str():_T("")
						  );

		_tcscat(szBuf, szOptBuf);
	}
	else if (pTrade->sSecurityType == FIX::SecurityType_FUTURE)
	{
		TCHAR szFutBuf[1024];

		CFixTagValue	tagMaturityMonthYear = pTrade->fmyMaturityMonthYear;
		string			sMaturityMonthYear((char*)tagMaturityMonthYear.GetData(), tagMaturityMonthYear.GetLength());

		CFixTagValue	tagMaturityDay = pTrade->fmdMaturityDay;
		string			sMaturityDay((char*)tagMaturityDay.GetData(), tagMaturityDay.GetLength());
		
		_sntprintf(szFutBuf,sizeof(szFutBuf)-1, _T("%s%s\t%s\t%s\n"),

			sMaturityMonthYear.c_str(),
			sMaturityDay.c_str(),
			pTrade->sUnderlying.c_str(),
			pTrade->sBroker.length()?pTrade->sBroker.c_str():_T("")
			);

		_tcscat(szBuf, szFutBuf);
	}
	else
	{
		TCHAR szOptBuf[1024];
		_sntprintf(szOptBuf,sizeof(szOptBuf)-1, _T("%s\t\t\t\n"), pTrade->sBroker.length()?pTrade->sBroker.c_str():_T(""));
   		_tcscat(szBuf, szOptBuf);
	}
	

	m_fTradesFile << T2A(szBuf) << flush;

	if(!m_fTradesFile)
		return ERROR_WRITE_FAULT;

	return 0;
}

DWORD CPublisher::Publish(const CTradePtr& pTrade)
{
	try
	{
		m_spPubManager->PubTradeUpdate(m_spTradeUpdate);

		CTracer::Trace(_T("Trade '%s' published in group '%s'."), pTrade->sExecID.c_str(), (LPCTSTR)m_bsUserGroup);
		InterlockedIncrement((LPLONG)&m_dwPublished);
	}
	catch(_com_error& e)
	{
		CTracer::Trace(_T("WARNING. Failed to publish trade : 0x%x '%s'."), e.Error(), e.Description());
	}
	return 0;
};

LONG CPublisher::GetConnectionString()
{

	try
	{
		HRESULT hr = S_OK;

		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();
		if(FAILED(XMLParams.GetMainXMLString(DB_XML_KEY, (BSTR)m_bsUserGroup, &m_bsConnectionString)))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to load connection string");

		if (m_bsConnectionString.length() <= 0)
			utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"Connection string not defined for user group");				

		_ConnectionPtr spConn;
		hr = spConn.CreateInstance( __uuidof(Connection) );
		if ( FAILED(hr) )
		utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Connection object");

        spConn->ConnectionString = m_bsConnectionString;
	    PropertiesPtr spParams = spConn->Properties;
 		
	    _bstr_t bs = spParams->Item[L"Data Source"]->Value;
	    bs += L"\\";
	    bs += (_bstr_t)spParams->Item[L"Initial Catalog"]->Value;
	    m_bsConnStringLabel = bs;
	}
	catch(_com_error& e)
	{
		return CTracer::TraceError(e.Error(), (LPCSTR)e.Description());
	}

	return S_OK;
}


bool  CPublisher::Reconnect()
{
	CTracer::Trace(_T("Trying to reconnect..."));

	try
	{
		if(m_pConnection!=NULL)
		{
			m_pConnection->Close();
			m_pConnection.reset();
		}
	}
	catch(_com_error&)
	{
	}

	Sleep(DBRECONNECTION_DELAY);

	try
	{
		m_pConnection = CDBConnectionPtr(new CDBConnection);
        m_pConnection->Open(m_bsConnectionString,5, 10, 250, 250);

		_bstr_t bsInitialCatalog = m_pConnection->GetAdoConnectionPtr()->Properties->Item[L"Initial Catalog"]->Value;
		_bstr_t bsDataSource = m_pConnection->GetAdoConnectionPtr()->Properties->Item[L"Data Source"]->Value;

		CTracer::Trace(_T("------------------------------------"));
		CTracer::Trace(_T(" - DB host: %s"), (LPCTSTR)bsDataSource );	
		CTracer::Trace(_T(" - DB name: %s"), (LPCTSTR)bsInitialCatalog );	
		CTracer::Trace(_T("------------------------------------"));

	}
	catch(_com_error& e)
	{
		CTracer::Trace(e.Description());
	}

	CTracer::Trace(_T("Reconnection done."));

	return true;
}

DWORD CPublisher::ReProcessTrade(const CTradePtr& pTrade, long lSeqNum)
{
    DWORD dwRes = ERROR_SUCCESS;

    TDBFailsStore::iterator it = m_DBFails.find(pTrade->sExecID);
    if ( it == m_DBFails.end() )
        return ERROR_NO_DATA;

    m_DBFails.erase(it);
    
    CClientRecordset rs;
	CStoredProc<> sp_get(*m_pConnection.get(), L"usp_TradeSeq_Get");
    try
    {        
        sp_get << (int)lSeqNum;
        sp_get << (int)lSeqNum;
        sp_get << CDBNull();
        sp_get << (unsigned char)1;        
        
		rs.Open (sp_get);

        if(rs.IsEOF())
        {
            rs.Close();
            _com_issue_error( E_FAIL );
        }             
    }
    catch (_com_error& e)
    {        
        CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : %s"), pTrade->sExecID.c_str(), (char*)e.Description() );	
        dwRes = ERROR_DATABASE_FAILURE;
    }
        
    HRESULT hRes = FillTradeUpdate(rs, pTrade);
		
    if( FAILED(hRes) )
    {	 
        CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to fill TradeUpdate message"), pTrade->sExecID.c_str() );
        dwRes = ERROR_INVALID_PARAMETER;
    }
	else if(hRes == S_FALSE)
	{
	    CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to retrieve trade from database."), pTrade->sExecID.c_str());
        dwRes = ERROR_NO_DATA_DETECTED;
	}                   
    
    return dwRes;
}

double CPublisher::ConvertDateTime(const CFixTimeValue* tValue)
{
	SYSTEMTIME st = {0};
	::GetSystemTime(&st);
	
	st.wYear = tValue->tm_year + 1900;
	st.wMonth = tValue->tm_mon + 1;
	st.wDay = tValue->tm_mday;
	st.wHour = tValue->tm_hour;
	st.wMinute = tValue->tm_min;
	st.wSecond = tValue->tm_sec;
	st.wMilliseconds = 0;

	double dt = 0.;
	::SystemTimeToVariantTime(&st, &dt);
	
	return dt;
}

double CPublisher::ConvertDateTime(const CFixDateValue* tValue)
{
	SYSTEMTIME st = {0};
	::GetSystemTime(&st);
	
	st.wYear = tValue->tm_year + 1900;
	st.wMonth = tValue->tm_mon + 1;
	st.wDay = tValue->tm_mday;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	double dt = 0.;
	::SystemTimeToVariantTime(&st, &dt);
	
	return dt;
}

double CPublisher::ConvertDateTime(const CFixMonthYearValue* tValue, const CFixDayOfMonthValue* dValue)
{
	SYSTEMTIME st = {0};
	::GetSystemTime(&st);

	st.wYear = tValue->tm_year + 1900;
	st.wMonth = tValue->tm_mon + 1;
	st.wDay = dValue->tm_mday;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	double dt = 0.;
	::SystemTimeToVariantTime(&st, &dt);
	
	return dt;
}

HRESULT CPublisher::FlushTrade(const CTradePtr &pTrade)
{   
	//time_t cur_time;
    //time(&cur_time);    

    _variant_t  vtVal;

    string sFlush = "|";
    
    sFlush += pTrade->sAccount;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sExecID;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->cExecTransType;
    sFlush += "|";        
    
    ///////////////////////////////////////////////////////////////////
    vtVal = pTrade->fLastPrice;

	_variant_t vtDest;
	
	HRESULT hr = ::VariantChangeTypeEx( &vtDest, &vtVal, LCID_ENGLISH_USA, 
		                                VARIANT_NOVALUEPROP, VT_BSTR );
	if ( FAILED(hr) )
		return hr;

	try
	{	
	    sFlush += _bstr_t(V_BSTR(&vtDest));
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
	try
	{	
		vtVal = (long)pTrade->iLastShares;
        sFlush += (_bstr_t)vtVal;
	}
	catch(_com_error& e)
	{
		return e.Error();
	}
	
    sFlush += "|";
    
    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->cOrderStatus;
    sFlush += "|";
    
    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->cSide;
    sFlush += "|"; 

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sSymbol;
    sFlush += "|"; 
    
    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sRoot;
    sFlush += "|"; 

    ///////////////////////////////////////////////////////////////////        
	double dTransactTime = ConvertDateTime(&pTrade->ftTransactTime);
	if (dTransactTime == 0.)
		return E_FAIL;	

	vtDest.Clear();
	vtVal = dTransactTime;

	hr = ::VariantChangeTypeEx( &vtDest, &vtVal, LCID_ENGLISH_USA, 
		VARIANT_NOVALUEPROP, VT_BSTR );
	if (FAILED(hr))
		return hr;

	try
	{	
        sFlush += _bstr_t(V_BSTR(&vtDest));
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

    sFlush += "|";    
    
	
    ///////////////////////////////////////////////////////////////////
	double dTradeDate = ConvertDateTime(&pTrade->fdTradeDate);
	if (dTradeDate == 0.)
		return E_FAIL;	

	vtDest.Clear();
	vtVal = dTradeDate;

	hr = ::VariantChangeTypeEx( &vtDest, &vtVal, LCID_ENGLISH_USA, 
		VARIANT_NOVALUEPROP, VT_BSTR );
	if (FAILED(hr))
		return hr;

	try
	{	
        sFlush += _bstr_t(V_BSTR(&vtDest));
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sSecurityType;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sText;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    if ( pTrade->sSecurityType == FIX::SecurityType_OPTION )
    {
		double dMaturityDate = ConvertDateTime(&pTrade->fmyMaturityMonthYear, &pTrade->fmdMaturityDay);
		if (dMaturityDate == 0.)
			return E_FAIL;	

		vtDest.Clear();
		vtVal = dMaturityDate;

		hr = ::VariantChangeTypeEx( &vtDest, &vtVal, LCID_ENGLISH_USA, 
			VARIANT_NOVALUEPROP, VT_BSTR );
		if (FAILED(hr))
			return hr;

		try
		{	
			sFlush += _bstr_t(V_BSTR(&vtDest));
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
    }
	else
	{
        vtVal = (long)-1;

		try
		{	
			sFlush += (_bstr_t)vtVal;
		}
		catch(_com_error& e)
		{
			return e.Error();
		}
	}

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    if ( pTrade->sSecurityType == FIX::SecurityType_OPTION )
	{	
	    vtVal = (long)pTrade->iPutOrCall;
	}
	else
	{
		vtVal = (long)-1;
	}
    
	try
	{	
        sFlush += (_bstr_t)vtVal;
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////    
	if ( pTrade->sSecurityType == FIX::SecurityType_OPTION )
	{	
        vtVal = pTrade->fStrikePrice;
	}
	else
	{
		vtVal = (long)-1;
	}    

	try {  vtDest.Clear();  } catch(_com_error&) {}

	hr = ::VariantChangeTypeEx( &vtDest, &vtVal, LCID_ENGLISH_USA, 
		                        VARIANT_NOVALUEPROP, VT_BSTR );
	if ( FAILED(hr) )
		return hr;

	try
	{	
	    sFlush += _bstr_t(V_BSTR(&vtDest));
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sUnderlying;

    ///////////////////////////////////////////////////////////////////    
    vtVal = (long)sFlush.size();

	string sString = sFlush;
	try
	{
		sFlush = (_bstr_t)vtVal;
		sFlush += sString;
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

	if(pTrade->sBroker.length())
	{
		sFlush += "|";
		sFlush += pTrade->sBroker;
	}
    m_fFailesFile << sFlush <<endl;
    
    return S_OK;
}

HRESULT CPublisher::BroadcastError(HRESULT hrErr, _bstr_t bsDesc)
{    
    try
    {
        m_spBroadcastMessage->PutCode(hrErr);
        m_spBroadcastMessage->PutDescription(bsDesc);

        m_spBroadcastMessage->PutPriority(enPrHigh);
        m_spBroadcastMessage->PutSeverity(enSevError);
        m_spBroadcastMessage->PutFacility(enFacRTTF);

        PublishBroadcastMessage();        
    }
    catch(_com_error& e)
    {
        return e.Error();
    }

    return S_OK;
}

void CPublisher::PublishBroadcastMessage() throw(_com_error)
{
    if (m_spPubManager == NULL)
        utils::ThrowErrorNoSetErrorInfo(E_INVALIDARG, L"PubManager object wasn't created");
    if (m_spBroadcastMessage == NULL)
        utils::ThrowErrorNoSetErrorInfo(E_INVALIDARG, L"BroadcastMessage object wasn't created");

    m_spPubManager->PubBroadcastMessage(m_spBroadcastMessage);

    CTracer::Trace(_T("BroadcastMessage successfully published."));
}

PDWORD CPublisher::GetCounterReceived()
{
	return &m_dwReceived;
}
