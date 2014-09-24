#include "stdafx.h"
#include "oledberr.h"
#include "SgConst.h"
#include "Publisher.h"
#include "Trace.h"
#include "FixTradeConst.h"
#include "resource_GUI.h"
#include "Win32Error.h"

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
    USES_CONVERSION;

	if(sSecurityType == T2A(ST_OPTION) && sSymbol.length() > 0)
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

/////////////////////////////////////////////////////////////////////////////////////
// CPublisher class implementation
_bstr_t	CPublisher::m_bsConnectionString;
_bstr_t	CPublisher::m_bsConnStringLabel;

/////////////////////////////////////////////////////////////////////////////////////
CPublisher::CPublisher() 
{ 		
	m_dwDBStored = 0;
	m_dwDBDeleted = 0;
	m_dwPublished = 0;
	m_dwReceived = 0;

	USES_CONVERSION;
		
	SYSTEMTIME st;
	::GetLocalTime(&st);

	TCHAR pTradesFile[100] = { 0 };
    TCHAR pFailesFile[100] = { 0 };
    TCHAR pFileDate[100] = { 0 };

    _stprintf(pFileDate, _T("_%2.2i_%2.2i_%4.4i"), st.wDay, st.wMonth, st.wYear);
				
	_tcscpy(pTradesFile, TRADES_FILENAME_BEGIN);
	_tcscat(pTradesFile, pFileDate);			
	_tcscat(pTradesFile, TRADES_FILENAME_END);	

    _tcscpy(pFailesFile, FAILES_FILENAME_BEGIN);
	_tcscat(pFailesFile, pFileDate);			
	_tcscat(pFailesFile, FAILES_FILENAME_END);	

	m_fTradesFile.open( T2A(pTradesFile), ios::app | ios::out );
    m_fFailesFile.open( T2A(pFailesFile), ios::app | ios::out );
	m_fStructFile.open( T2A(STRUCTURE_FILENAME), ios::app | ios::out);
}

CPublisher::~CPublisher() 
{ 
    m_fTradesFile.close();
    m_fFailesFile.close();
    m_fStructFile.close();
}

UINT  __stdcall CPublisher::Thread(void* pParam)
{
	const HANDLE hNewDataNotification = GetNotificationEvent();
	const HANDLE hStopEvent = GetStopEventHandle();
	const HANDLE hEvents[] = {hNewDataNotification, hStopEvent};

	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE);

		switch(dwRes)
		{
			case WAIT_OBJECT_0:
			{
				CTrade	Trade;
				BOOL	bNewTrades; 

				do
				{
					bNewTrades = Pop(Trade);

					if (bNewTrades)
					{
						//FlushTrade(Trade);
						
						DWORD dwRes = StoreInDatabase(Trade);
						if(dwRes == 0)
						{
							Publish(Trade);
						}
						else
						{
							CTracer::Trace(_T("Trade with ExecID = '%s' was not published."), 
								Trade.sExecID.c_str());
						}

						switch(dwRes)
						{
							case 0:
								Trade.sStatus = "OK";
								break;

							case ERROR_ALREADY_EXISTS:
								Trade.sStatus = "ALREADYEXISTS";
								break;

							case ERROR_FILE_NOT_FOUND:
								Trade.sStatus = "NOTFOUND";
								break;

                            case ERROR_DATABASE_FAILURE:
                                Trade.sStatus = "REPROCESSFAIL";
                                break;

							default:
								Trade.sStatus = "ERROR";
								break;
						}

						StoreInFile(Trade);
					}

                } while(bNewTrades && (WaitForSingleObject(hStopEvent, 0) != WAIT_OBJECT_0));
			}
			break;

			case (WAIT_OBJECT_0 + 1):
			{
				CoUninitialize();
				return 0;
			}

			default:
				CoUninitialize();
				return CTracer::TraceError();
		}
	}
};

DWORD __stdcall CPublisher::Start(void* pParam)
{
	USES_CONVERSION;

	LONG lRes = GetConnectionString();
	if(lRes)
		return CTracer::TraceError(lRes, _T("Fail to read connection string from the registry"));

	if(!m_fStructFile)
		return CTracer::TraceError(ERROR_OPEN_FAILED, _T("Fail to open structure file."));

	if(!m_fTradesFile)
		return CTracer::TraceError(ERROR_OPEN_FAILED, _T("Fail to open trades file."));

	// Database...
	CTracer::Trace(_T("Connecting to database..."));

	try
	{
        m_Connection.Open(CPublisher::m_bsConnectionString);
	}
	catch(_com_error& e)
	{
		m_Connection.Close();

		_bstr_t bsError = "Failed to connect to database : " + m_Connection.GetErrorMessage();		
		CTracer::Trace(bsError);

		return e.Error();
	}
	
	CTracer::Trace(_T("Connection to database is established."));

	try
    {
        _ConnectionPtr spConnection;
        HRESULT hr = spConnection.CreateInstance(__uuidof(Connection));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create ADO connection object.");

        spConnection->ConnectionString = m_Connection.GetConnectionString();
        
        _bstr_t bsInitialCatalog = spConnection->Properties->Item[L"Initial Catalog"]->Value;
        _bstr_t bsDataSource = spConnection->Properties->Item[L"Data Source"]->Value;
	 
		CTracer::Trace(_T("------------------------------------"));
		CTracer::Trace(_T(" - DB host: %s"), (LPCTSTR)bsDataSource );	
		CTracer::Trace(_T(" - DB name: %s"), (LPCTSTR)bsInitialCatalog );	
		CTracer::Trace(_T("------------------------------------"));
    }
    catch (const _com_error& e)
    {
        CTracer::TraceError(e.Error(), W2T(e.Description()) );
    }

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

		m_spPubManager->RegPublisher(enMtTradeUpdate, NULL);

		CTracer::Trace(_T("Publisher initialized."));
	}
	catch(_com_error& e)
	{
		return CTracer::TraceError(e.Error(), W2T(e.Description()) );
	}

	return CThreadWrapper::Start(pParam);
};

DWORD __stdcall CPublisher::Stop()
{
	while(IsEmpty() == false)
		::Sleep(500);

	CThreadWrapper::Stop();

	try
	{
		m_Connection.Close();
	}
	catch(_com_error&)
	{
	}

	try
	{
		if(m_spPubManager)
			m_spPubManager->UnregPublisher(enMtTradeUpdate, NULL);

		// give time for transport to prepare for shutdown..
//		Sleep(5000);

		m_spTradeUpdate = NULL;
		m_spPubManager = NULL;
	}
	catch(_com_error&)
	{
	}

	return 0;
}

HRESULT CPublisher::FillTradeUpdate(CClientRecordset& rs, const CTrade& Trade)
{
	USES_CONVERSION;

	try
	{
		m_spTradeUpdate->SeqNum = rs[L"iSeqNum"];
		if(m_spTradeUpdate->SeqNum == 0)
			_com_issue_error(S_FALSE);

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
		m_spTradeUpdate->IsCall = rs[L"tiIsCall"];
		m_spTradeUpdate->Strike = rs[L"fStrike"];
		m_spTradeUpdate->PriceClose = rs[L"fPriceClose"];
		m_spTradeUpdate->PriceTheoClose = rs[L"fPriceTheoClose"];
		m_spTradeUpdate->LotSize = rs[L"iLotSize"];
        m_spTradeUpdate->OptionRootID = rs[L"iOptionRootID"];
 //  not existed      m_spTradeUpdate->FutID =  rs[L"iFutureID"];
 //  not existed      m_spTradeUpdate->FutRootID =  rs[L"iFutureRootID"];

		if(Trade.cExecTransType == ETT_NEW)
		{
			long lStructureMissed = rs[L"iStructureMissed"];
			m_spTradeUpdate->MsState = (TRADE_MS_STATE)lStructureMissed;

			if(lStructureMissed)
			{
				_TCHAR szBuf[1024];
				
				switch(lStructureMissed)
				{
					case enTmssNewStock:
						_stprintf(szBuf, _T("New stock '%s' added.\n"), Trade.sUnderlying.c_str());
						break;

					case enTmssNewOption:
						_stprintf(szBuf, _T("New option '%s' for stock '%s' added.\n"),
							Trade.sSymbol.c_str(), Trade.sUnderlying.c_str());
						break;

					case enTmssNewStock | enTmssNewOption:
						_stprintf(szBuf, _T("New stock '%s' and new option '%s' for this stock are added.\n"),
							Trade.sUnderlying.c_str(), Trade.sSymbol.c_str());
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
		return e.Error();
	}

	return S_OK;
}

DWORD CPublisher::StoreInDatabase(const CTrade& Trade)
{
	USES_CONVERSION;

	DWORD	dwTries = MAX_DBRECONNECTION_TRIES;
	bool	bOk = false;
	DWORD	dwRes = ERROR_SUCCESS;
    long    lMaxSeqNum;
    long    lMinSeqNum;
    long    lStructureMissed;
    TDBFailsStore::iterator it;

	do
	{
		try
		{
			switch(Trade.cExecTransType)
			{
				case ETT_NEW:
				{                    
                    CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_New");

					sp << _bstr_t(Trade.sAccount.c_str()) << _bstr_t(Trade.sExecID.c_str());
					sp << ((Trade.cSide == SIDE_BUY) ? (int)1 : int(0));
					sp << Trade.iLastShares;
					sp << Trade.fLastPrice;

					DATE dtTradeDate;
					OleDateFromTm(Trade.ftTransactTime.tm_year + 1900, 
								  Trade.ftTransactTime.tm_mon + 1, 
								  Trade.ftTransactTime.tm_mday, 
								  Trade.ftTransactTime.tm_hour, 
								  Trade.ftTransactTime.tm_min, 
								  Trade.ftTransactTime.tm_sec, 
								  dtTradeDate );

					sp << (vt_date)dtTradeDate;
					sp << _bstr_t(Trade.sSymbol.c_str());

					if(Trade.sSecurityType != T2A(ST_OPTION))
						sp << (int)2;
					else
					{
						sp << (int)3;
						sp << _bstr_t(Trade.sUnderlying.c_str());
						sp << (Trade.iPutOrCall == PC_CALL);
						sp << Trade.fStrikePrice;

						DATE dtExpiryDate;
						OleDateFromTm(Trade.fmyMaturityMonthYear.tm_year + 1900, 
									  Trade.fmyMaturityMonthYear.tm_mon + 1, 
									  Trade.fmdMaturityDay.tm_mday, 
									  0, 0, 0, 
								      dtExpiryDate);
						sp << (vt_date)dtExpiryDate;
                        sp << dbNull;
                        sp << dbNull;
                        sp << dbNull;
                        sp << _bstr_t(Trade.sRoot.c_str());
					}

					rs.Open (sp);
                    if(rs.IsEOF())
                    {
                        rs.Close();
                        _com_issue_error( E_FAIL );
                    }                        

                    lMaxSeqNum = rs[L"iMaxSeqNum"];
                    lMinSeqNum = rs[L"iMinSeqNum"];
                    lStructureMissed = rs[L"iStructureMissed"];
                    rs.Close();
                    
                    if ( lMaxSeqNum == -1 
                        && lMinSeqNum == -1 
                        && lStructureMissed == -1) // unknown error
                        _com_issue_error( E_FAIL );
                    else
                        if ( lMaxSeqNum == -1 && lStructureMissed == -1 ) //trade already exists
                        {
						    dwRes = ERROR_ALREADY_EXISTS;                            
                            if ( ReProcessTrade(Trade, lMinSeqNum) == ERROR_SUCCESS )
                                dwRes = ERROR_SUCCESS;                            
                            break;
                        }
                    
                    CTracer::Trace(_T("Trade '%s' stored in database."), Trade.sExecID.c_str());
				    InterlockedIncrement((LPLONG)&m_dwDBStored);
                    
                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
                    sp_get << (int)lMaxSeqNum;
                    sp_get << (int)lMaxSeqNum;
                    sp_get << dbNull;
                    sp_get << (unsigned char)1;
					
                    rs.Open (sp_get);
					
                    if(rs.IsEOF())
					{
						CTracer::Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
							Trade.sExecID.c_str());
						dwRes = ERROR_NO_DATA_DETECTED;
					}
					else
					{
						HRESULT hRes;
						if(FAILED(hRes = FillTradeUpdate(rs, Trade)))
						{
							CTracer::Trace(_T("Failed to fill TradeUpdate message."));
							dwRes = ERROR_INVALID_PARAMETER;
						}
						else if(hRes == S_FALSE)
						{
							CTracer::Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
								Trade.sExecID.c_str());
							dwRes = ERROR_NO_DATA_DETECTED;
						}
					}
                    
					break;
				}

				case ETT_CANCEL:
				{
					CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_Del");
					sp << _bstr_t(Trade.sExecID.c_str());
					DATE dtTradeDate;
					OleDateFromTm(Trade.fdTradeDate.tm_year + 1900, 
								  Trade.fdTradeDate.tm_mon + 1, 
								  Trade.fdTradeDate.tm_mday, 
								  0, 0, 0, 
								  dtTradeDate);

					sp << (vt_date)dtTradeDate;
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
                            if ( ReProcessTrade(Trade, lMaxSeqNum) == ERROR_SUCCESS )
                                dwRes = ERROR_SUCCESS;
                            break;
                        }
                    else
                        if (lMaxSeqNum == -1)
                        {
                            CTracer::Trace(_T("Failed to store trade in database - trade with the same ExecID '%s' either does not exist."),
							     Trade.sExecID.c_str());
						    dwRes = ERROR_FILE_NOT_FOUND;
                            break;
                        }

                    CTracer::Trace(_T("Trade '%s' deleted from database."), Trade.sExecID.c_str());
				    InterlockedIncrement((LPLONG)&m_dwDBDeleted);
                    
                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
                    sp_get << (int)lMaxSeqNum;
					sp_get << (int)lMaxSeqNum;
                    sp_get << dbNull;
                    sp_get << (unsigned char)1;

                    rs.Open (sp_get);
					
                    if(rs.IsEOF())
					{
						CTracer::Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
							Trade.sExecID.c_str());
						dwRes = ERROR_NO_DATA_DETECTED;
					}
					else
					{
                        HRESULT hRes;
					    if(FAILED(hRes = FillTradeUpdate(rs, Trade)))
                        {
   					        CTracer::Trace(_T("Failed to fill TradeUpdate message."));
					        dwRes = ERROR_INVALID_PARAMETER;
                        }
					    else if(hRes == S_FALSE)
                        {
   					        CTracer::Trace(_T("Failed to retrieve trade from database - trade with the same ExecID '%s' does not exist."),
					 	        Trade.sExecID.c_str());
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
			_bstr_t bsError = "Failed to store trade in database : " + 
				m_Connection.GetErrorMessage();

			CTracer::Trace(bsError);

            pair<TDBFailsStore::iterator, bool> pairResult = 
                  m_DBFails.insert(Trade.sExecID);

            if(--dwTries)
			{				
                Reconnect();
			}
			else
			{
				CTracer::Trace(_T("CRITICAL ERROR : FAILED TO STORE TRADE IN DATABASE."));

				if(/*Uncomment this Oleg StoreInFile(Trade) != 0*/false)
				{
					CTracer::Trace(_T("CRITICAL ERROR : FAILED TO STORE TRADE IN FILE."));
					CTracer::Trace(_T("MANUAL INTERVENTION IS REQUIRED. THE APPLICATION IS STOPPED."));
					ExitProcess(-1);
				}
				else
				{
					FlushTrade(Trade);
                    BroadcastError(e.Error(), bsError);
                    dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
					bOk = true;
				}
			}
		}

	} while(!bOk);

	return dwRes;
};

DWORD CPublisher::StoreInFile(const CTrade& Trade)
{
	TCHAR	szBuf[8192];
	string	sExecTransType;
	string	sOrderStatus;
	string	sSide;

	USES_CONVERSION;

	switch(Trade.cExecTransType)
	{
		case ETT_NEW: sExecTransType = "NEW"; break;
		case ETT_CANCEL: sExecTransType = "CANCEL"; break;
		default: sExecTransType = "UNEXPECTED"; break;
	}

	switch(Trade.cOrderStatus)
	{
		case OS_PARTIALLY_FILLED: sOrderStatus = "PARTIALLY FILLED"; break;
		case OS_FILLED: sOrderStatus = "FILLED"; break;
		default: sOrderStatus = "UNEXPECTED"; break;
	}

	switch(Trade.cSide)
	{
		case SIDE_BUY: sSide = "BUY"; break;
		case SIDE_SELL: sSide = "SELL"; break;
		default: sSide = "UNEXPECTED"; break;
	}

	CFixTagValue	tagTransactTime = Trade.ftTransactTime;
	string			sTransactTime((char*)tagTransactTime.GetData(), tagTransactTime.GetLength());

	CFixTagValue	tagTradeDate = Trade.fdTradeDate;
	string			sTradeDate((char*)tagTradeDate.GetData(), tagTradeDate.GetLength());

	SYSTEMTIME st;
	::GetLocalTime(&st);
	

	_stprintf(szBuf, _T("%-2.2i/%-2.2i/%-4.4i %-2.2i:%-2.2i:%-2.2i\t")\
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

				   Trade.sStatus.c_str(),
				   
				   sExecTransType.c_str(),
				   Trade.fLastPrice,
				   Trade.iLastShares,
				   sOrderStatus.c_str(),
				   Trade.sExecID.c_str(),
				   Trade.sAccount.c_str(),
				   sSide.c_str(),
				   Trade.sSymbol.c_str(),
				   Trade.sSecurityType.c_str(),
				   sTransactTime.c_str(),
				   sTradeDate.c_str(),
				   Trade.sText.c_str() == NULL ? "" : Trade.sText.c_str()
				   );

	if(Trade.sSecurityType == T2A(ST_OPTION))
	{
		TCHAR szOptBuf[1024];

		CFixTagValue	tagMaturityMonthYear = Trade.fmyMaturityMonthYear;
		string			sMaturityMonthYear((char*)tagMaturityMonthYear.GetData(), tagMaturityMonthYear.GetLength());

		CFixTagValue	tagMaturityDay = Trade.fmdMaturityDay;
		string			sMaturityDay((char*)tagMaturityDay.GetData(), tagMaturityDay.GetLength());

		string sPutOrCall;

		switch(Trade.iPutOrCall)
		{
			case PC_PUT :  sPutOrCall = "PUT"; break;
			case PC_CALL : sPutOrCall = "CALL"; break;
			default : sPutOrCall = "UNEXPECTED"; break;
		}

		_stprintf(szOptBuf, _T("%s%s\t%s\t%f\t%s\n"),

						  sMaturityMonthYear.c_str(),
						  sMaturityDay.c_str(),
						  sPutOrCall.c_str(),
						  Trade.fStrikePrice,
						  Trade.sUnderlying.c_str()
						  );

		_tcscat(szBuf, szOptBuf);
	}
	else
	{
		_tcscat(szBuf, _T("\t\t\t\n"));
	}
	

	m_fTradesFile << T2A(szBuf) << flush;

	if(!m_fTradesFile)
		return ERROR_WRITE_FAULT;

	return 0;
}

DWORD CPublisher::Publish(const CTrade& Trade)
{
	try
	{
		m_spPubManager->PubTradeUpdate(m_spTradeUpdate);

		CTracer::Trace(_T("Trade '%s' published."), Trade.sExecID.c_str());
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
	USES_CONVERSION;

	try
	{
        ISettingsPtr spSettings;
		HRESULT hr = spSettings.CreateInstance(__uuidof (Settings));
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");

        CPublisher::m_bsConnectionString = spSettings->DatabaseConnection;

		_ConnectionPtr spConn;
		hr = spConn.CreateInstance( __uuidof(Connection) );
        if ( FAILED(hr) )
	 	  	  utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Connection object");

        spConn->ConnectionString = m_bsConnectionString;
	    PropertiesPtr spParams = spConn->Properties;
 		
	    _bstr_t bs = spParams->Item[L"Data Source"]->Value;
	    bs += L"\\";
	    bs += (_bstr_t)spParams->Item[L"Initial Catalog"]->Value;
	    CPublisher::m_bsConnStringLabel = bs;
	}
	catch(_com_error& e)
	{
		return CTracer::TraceError(e.Error(), W2T(e.Description()) );
	}

	return 0;
}


bool  CPublisher::Reconnect()
{
	CTracer::Trace(_T("Trying to reconnect..."));

	try
	{
		m_Connection.Close();
	}
	catch(_com_error&)
	{
	}

	Sleep(DBRECONNECTION_DELAY);

	try
	{
        m_Connection.Open(CPublisher::m_bsConnectionString);
	}
	catch(_com_error&)
	{
		CTracer::Trace(m_Connection.GetErrorMessage());
	}

	CTracer::Trace(_T("Reconnection done."));

	return true;
}

DWORD CPublisher::ReProcessTrade(const CTrade& Trade, long lSeqNum)
{
    DWORD dwRes = ERROR_SUCCESS;

    TDBFailsStore::iterator it = m_DBFails.find(Trade.sExecID);
    if ( it == m_DBFails.end() )
        return ERROR_NO_DATA;

    m_DBFails.erase(it);
    
    CClientRecordset rs;
	CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
    try
    {        
        sp_get << (int)lSeqNum;
        sp_get << (int)lSeqNum;
        sp_get << dbNull;
        sp_get << (unsigned char)1;        
        
		rs.Open (sp_get);

        if(rs.IsEOF())
        {
            rs.Close();
            _com_issue_error( E_FAIL );
        }             
    }
    catch (_com_error&)
    {        
        CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : %s"), Trade.sExecID.c_str(), (char*)m_Connection.GetErrorMessage() );	
        dwRes = ERROR_DATABASE_FAILURE;
    }
        
    HRESULT hRes = FillTradeUpdate(rs, Trade);
		
    if( FAILED(hRes) )
    {	 
        CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to fill TradeUpdate message"), Trade.sExecID.c_str() );
        dwRes = ERROR_INVALID_PARAMETER;
    }
	else if(hRes == S_FALSE)
	{
	    CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to retrieve trade from database."), Trade.sExecID.c_str());
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

HRESULT CPublisher::FlushTrade(const CTrade &Trade)
{   
	//time_t cur_time;
    //time(&cur_time);    

    _variant_t  vtVal;

    string sFlush = "|";
    
    sFlush += Trade.sAccount;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += Trade.sExecID;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += Trade.cExecTransType;
    sFlush += "|";        
    
    ///////////////////////////////////////////////////////////////////
    vtVal = Trade.fLastPrice;

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
		vtVal = (long)Trade.iLastShares;
        sFlush += (_bstr_t)vtVal;
	}
	catch(_com_error& e)
	{
		return e.Error();
	}
	
    sFlush += "|";
    
    ///////////////////////////////////////////////////////////////////
    sFlush += Trade.cOrderStatus;
    sFlush += "|";
    
    ///////////////////////////////////////////////////////////////////
    sFlush += Trade.cSide;
    sFlush += "|"; 

    ///////////////////////////////////////////////////////////////////
    sFlush += Trade.sSymbol;
    sFlush += "|"; 
    
    ///////////////////////////////////////////////////////////////////
    sFlush += Trade.sRoot;
    sFlush += "|"; 

    ///////////////////////////////////////////////////////////////////        
	double dTransactTime = ConvertDateTime(&Trade.ftTransactTime);
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
	double dTradeDate = ConvertDateTime(&Trade.fdTradeDate);
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
    sFlush += Trade.sSecurityType;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += Trade.sText;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    if ( Trade.sSecurityType == T2A(ST_OPTION) )
    {
		double dMaturityDate = ConvertDateTime(&Trade.fmyMaturityMonthYear, &Trade.fmdMaturityDay);
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
    if ( Trade.sSecurityType == T2A(ST_OPTION) )
	{	
	    vtVal = (long)Trade.iPutOrCall;
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
	if ( Trade.sSecurityType == T2A(ST_OPTION) )
	{	
        vtVal = Trade.fStrikePrice;
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
    sFlush += Trade.sUnderlying;

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

    m_fFailesFile << sFlush <<endl;
    
    return S_OK;
}

HRESULT CPublisher::BroadcastError(HRESULT hrErr, _bstr_t& bsDesc)
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
