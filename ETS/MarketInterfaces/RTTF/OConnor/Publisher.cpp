#include "stdafx.h"
#include "oledberr.h"

#include "Publisher.h"

//DWORD CPublisher::m_dwSent = 0;
//CHandle CPublisher::m_hFinishHandle(CreateEvent(NULL, FALSE, FALSE, NULL));

CPublisher::CPublisher() : 		
	m_dwDBStored(0),
	m_dwDBDeleted(0),
	m_dwPublished(0),
	m_dwReceived(0)
{
	
	SYSTEMTIME st;
	::GetLocalTime(&st);

	TCHAR pTradesFile[256] = { 0 };
    TCHAR pFailesFile[256] = { 0 };
	TCHAR pFileDate[100] = { 0 };
	CString sStructureFile;

	_bstr_t sbsUserGroup;
	_bstr_t sbsKey = SETTINGS_XML_KEY;
	_bstr_t sbsVal;

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();
	XMLParams.GetUserGroup(sbsUserGroup.GetAddress());

	sbsKey += "\\";
	sbsKey += sbsUserGroup;

	//get prefix for log file
	XMLParams.GetMainXMLString(sbsKey, "LogNameID", &sbsVal);
	CString strTradesPrefix = (LPTSTR)sbsUserGroup;
	if (sbsVal.length() > 0)
		strTradesPrefix = (LPTSTR)sbsVal;

    _stprintf_s(pFileDate, sizeof(pFileDate), _T("_%s_%2.2i_%2.2i_%4.4i"), strTradesPrefix.GetString(), st.wDay, st.wMonth, st.wYear);
				
    _tcscpy_s(pTradesFile, sizeof(pTradesFile), TRADES_FILENAME_BEGIN);
	_tcscat_s(pTradesFile, sizeof(pTradesFile), pFileDate);			
	_tcscat_s(pTradesFile, sizeof(pTradesFile), TRADES_FILENAME_END);	

    _tcscpy_s(pFailesFile, sizeof(pFailesFile), FAILES_FILENAME_BEGIN);
	_tcscat_s(pFailesFile, sizeof(pFailesFile), pFileDate);			
	_tcscat_s(pFailesFile, sizeof(pFailesFile), FAILES_FILENAME_END);

	sStructureFile.Format("%s_%s%s", STRUCTURE_FILENAME, strTradesPrefix.GetString(), FAILES_FILENAME_END);

	m_fTradesFile.open( CT2A(pTradesFile), ios::app | ios::out );
    m_fFailesFile.open( CT2A(pFailesFile), ios::app | ios::out );
	m_fStructFile.open( sStructureFile.GetString(), ios::app | ios::out);
}

CPublisher::~CPublisher() 
{ 
    m_fTradesFile.close();
    m_fFailesFile.close();
    m_fStructFile.close();
}


void CPublisher::OnData(CTradePtr& pTrade)
{
	BOOL	bNewTrades(FALSE); 

	InterlockedIncrement((LPLONG)&m_dwReceived);

	DWORD dwRes = StoreInDatabase(pTrade);
	if(dwRes == 0)
	{
		Publish(pTrade);
	}
	else
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Trade with ExecID = '%s' was not published."), 
			pTrade->trade_id.c_str());
	}

	switch(dwRes)
	{
	case 0:
		pTrade->status = "OK";
		break;

	case ERROR_ALREADY_EXISTS:
		pTrade->status = "ALREADYEXISTS";
		break;

	case ERROR_FILE_NOT_FOUND:
		pTrade->status = "NOTFOUND";
		break;

	case ERROR_DATABASE_FAILURE:
		pTrade->status = "REPROCESSFAIL";
		break;

	default:
		pTrade->status = "ERROR";
	}
	StoreInFile(pTrade);

//	if(!InterlockedDecrement((LPLONG) &CPublisher::m_dwSent))
//		SetEvent(CPublisher::m_hFinishHandle);
}


DWORD __stdcall CPublisher::Start(void* pParam)
{
	LONG lRes = GetConnectionString();
	if(lRes)
		return TraceError(lRes, _T("Fail to read connection string from the registry"));

	if(!m_fStructFile)
		return TraceError(ERROR_OPEN_FAILED, _T("Fail to open structure file."));

	if(!m_fTradesFile)
		return TraceError(ERROR_OPEN_FAILED, _T("Fail to open trades file."));

	m_dwReceived = 0;
	// Database...
	try
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Connecting to database..."));

		m_Connection.Open(m_bsConnectionString);

		_bstr_t bsInitialCatalog = m_Connection.GetAdoConnectionPtr()->Properties->Item[L"Initial Catalog"]->Value;
		_bstr_t bsDataSource = m_Connection.GetAdoConnectionPtr()->Properties->Item[L"Data Source"]->Value;

		//Trace(_T("------------------------------------"));
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T(" - DB host: %s"), (LPCTSTR)bsDataSource );	
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T(" - DB name: %s"), (LPCTSTR)bsInitialCatalog );	
		//Trace(_T("------------------------------------"));

		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Connection to database is established."));
	}
	catch(_com_error& e)
	{
		m_Connection.Close();

		_bstr_t bsError = "Failed to connect to database : " + m_Connection.GetErrorMessage();

		CTracer::TraceMessage(CTracer::enMtError, NULL, bsError);

		return e.Error();
	}

	// Transport...
	try
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Initializing publisher..."));

		HRESULT hr = m_spPubManager.CreateInstance(__uuidof (MsgManager));
		if(FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create MsgManager object.");

		hr = m_spTradeUpdate.CreateInstance(__uuidof (TradeUpdate));
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create TradeUpdate object.");
        
        hr = m_spBroadcastMessage.CreateInstance(__uuidof (BroadcastMessage));
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create BroadcastMessage object.");

		m_spPubManager->RegPublisher(enMtTradeUpdate, NULL);

		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Publisher initialized."));
	}
	catch(_com_error& e)
	{
		return TraceError(e.Error(), e.Description());
	}

	// Loading acronyms of accounts
	try
	{
		m_Accounts.clear();

		CClientRecordset rs;
		CStoredProc<> sp(m_Connection, L"usp_Trader_Get");
		rs.Open(sp);
		if (rs.IsEOF())
		{
			CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Fail. Where are no traders in database."));
			return ERROR_NO_DATA_DETECTED;
		}
		else
		{
			USES_CONVERSION;

			rs.MoveFirst();
			while (!rs.IsEOF())
			{
				_bstr_t bsAccount;
				bsAccount = rs["vcAcronym"];
				m_Accounts.insert(bsAccount);
				CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Trader '%s' loaded."), W2T(bsAccount));
				rs.MoveNext();
			}
		}
	}
	catch(_com_error& e)
	{
		return TraceError(e.Error(), e.Description());
	}
	
	return ERROR_SUCCESS;// CStrategyT<CPublisher, CTradePtr>::Start();
};

void __stdcall CPublisher::Stop()
{
	/*while(IsEmpty() == false)
		Sleep(1000);*/

	//CStrategyT<CPublisher, CTradePtr>::Stop();

	try
	{
		m_Connection.Close();
	}
	catch(_com_error&) { }

	try
	{
		if (m_spPubManager)
			m_spPubManager->UnregPublisher(enMtTradeUpdate, NULL);

		// give time for transport to prepare for shutdown..
		Sleep(2000);

		m_spTradeUpdate = 0;
		m_spPubManager = 0;
	}
	catch(_com_error&) { }
}

HRESULT CPublisher::FillTradeUpdate(CClientRecordset& rs, CTradePtr& pTrade)
{
	try
	{
		m_spTradeUpdate->SeqNum = rs[L"iSeqNum"];
		if(m_spTradeUpdate->SeqNum == 0)
		{
			CTracer::TraceMessage(CTracer::enMtError,NULL,"Duplicated trade %d", m_spTradeUpdate->SeqNum);
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
				CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Error. Unknown SymbolType for ContractTypeID = %d."), iContractTypeID);
				_com_issue_error(E_FAIL);
				break;
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

		if(pTrade->ins_upd_del_flag == NEW_FLAG || pTrade->ins_upd_del_flag == UPD_FLAG)
		{
			long lStructureMissed = rs[L"iStructureMissed"];
			m_spTradeUpdate->MsState = (TRADE_MS_STATE)lStructureMissed;

			if(lStructureMissed)
			{
                TCHAR szBuf[1024] = {0};
				
				switch(lStructureMissed)
				{
					case enTmssNewStock:
						_stprintf_s(szBuf, sizeof(szBuf), _T("New stock '%s' added.\n"), CA2CT(pTrade->base_sym.c_str()) );
						break;

					case enTmssNewOption:
						_stprintf_s(szBuf, sizeof(szBuf), _T("New option '%s' for stock '%s' added.\n"),
							CA2CT(pTrade->MakeTradingSymbol().c_str()), CA2CT(pTrade->base_sym.c_str()) );
						break;

					case enTmssNewStock | enTmssNewOption:
						_stprintf_s(szBuf, sizeof(szBuf),  _T("New stock '%s' and new option '%s' for this stock are added.\n"),
							CA2CT(pTrade->base_sym.c_str()), CA2CT(pTrade->MakeTradingSymbol().c_str()) );
						break;

					default:
						_tcscpy_s(szBuf, sizeof(szBuf), _T("Unexpected structure error."));
				}

				SYSTEMTIME st;
				::GetLocalTime(&st);
				_TCHAR szTime[32];
				_stprintf_s(szTime, sizeof(szTime), ("%d/%d/%d-%d:%d:%d "), st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

				m_fStructFile << CT2A(szTime) << CT2A(szBuf) << flush;
			}
		}
	}
	catch(_com_error& e)
	{
		CTracer::TraceMessage(CTracer::enMtError,NULL,"Trade %d Processing: COM Exception: %d %s",  m_spTradeUpdate->SeqNum, e.Error(), (LPCSTR)e.Description());
		return e.Error();
	}

	return S_OK;
}
DWORD CPublisher::StoreInDatabase(CTradePtr& pTrade)
{
	DWORD	dwTries = MAX_DBRECONNECTION_TRIES;
	bool	bOk = false;
	DWORD	dwRes = ERROR_SUCCESS;
    long    lMaxSeqNum;
    long    lMinSeqNum;
    long    lStructureMissed;
	long	lInfoId;
    TDBFailsStore::iterator it;

	do
	{
		try
		{
			switch(pTrade->ins_upd_del_flag)
			{
            case NEW_FLAG:
            case UPD_FLAG:
			    {                    
                    CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_New");

					sp << _bstr_t(pTrade->account.c_str());
					sp << _bstr_t(pTrade->trade_id.c_str());
					sp << (int)( pTrade->buy_sell == BUY_FLAG ? 1 : 0 );
					sp << pTrade->quantity;
					sp << pTrade->price;
					sp << (vt_date)pTrade->execution_date;
					
					sp << _bstr_t(pTrade->MakeTradingSymbol().c_str());

                    const char  option_flag[] = {OPTION_FLAG, 0};
					const char  future_flag[] = {FUTURE_FLAG, 0};

					//equity options
                    if (pTrade->security_type.compare(option_flag) == 0)
                    {
						if (pTrade->exec_broker_id == OCONNOR)
						{
							sp << (int)3;
							sp << CDBNull();// _bstr_t(pTrade->base_sym.c_str());
							sp << (bool)(pTrade->put_call == CALL_FLAG);
							sp << pTrade->strike;
							sp << (vt_date)pTrade->expiration_date;
							sp << CDBNull();
							sp << (int)1; //update enabled
							sp << CDBNull();
							sp << _bstr_t(pTrade->trading_sym.c_str()); // option root
						}
						else if(pTrade->exec_broker_id == EZECASTLE)	
						{
							//in this format we will try to find option by name
							//where is name like: 'LMTIQ'
							sp << (int)3;
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << (int)1; //update enabled
							sp << CDBNull();
							sp << _bstr_t(pTrade->trading_sym.c_str()); // option root
						}
                    }
					//futures contracts
                    else if(pTrade->security_type.compare(future_flag) == 0)
					{
						if (pTrade->exec_broker_id == OCONNOR)
						{
							sp << (int)4;								//contract type id #4 - futures
							sp << _bstr_t(pTrade->base_sym.c_str());	//vcUnderlyingSymbol for example 'SPX'
							sp << CDBNull();
							sp << CDBNull();  
							sp << (vt_date)pTrade->maturity_date;		//maturity date
							sp << CDBNull();
							sp << (int)1;								//update enabled
							sp << CDBNull();
							sp << _bstr_t(pTrade->trading_sym.c_str()); // future root, 'ES' for example
						}
						else if (pTrade->exec_broker_id == EZECASTLE)
						{
							
						}
					}
                    else
                    {
						sp << (int)2;
                        sp << CDBNull();
                        sp << CDBNull();
                        sp << CDBNull();
                        sp << CDBNull();
                        sp << CDBNull();
                        sp << (int)1; //update enabled
                        sp << CDBNull();
						sp << CDBNull(); // option root
                    }
		
					sp << CDBNull(); // Mark
					if(pTrade->strategy.length()>0) // strategy
						sp << _bstr_t(pTrade->strategy.c_str());
					else
						sp << CDBNull();

					rs.Open (sp);

                    if (rs.IsEOF())
                    {
                        rs.Close();
                        _com_issue_error(E_FAIL);
                    }                        

                    lMaxSeqNum = rs[L"iMaxSeqNum"];
                    lMinSeqNum = rs[L"iMinSeqNum"];
                    lStructureMissed = rs[L"iStructureMissed"];
					lInfoId = rs[L"iInfoId"];

                    rs.Close();
                    
                    if ( lMaxSeqNum == -1 && lStructureMissed == -1 )
                    {
                        if (lMinSeqNum == -1) // unknown error
                        {
							char szSym[100];
							switch(lInfoId)
							{
								case -4: //unknown underlying
									_snprintf_s(szSym, sizeof(szSym), sizeof(szSym), "Unknown underlying '%s' in trade '%s'.", pTrade->base_sym.c_str(), pTrade->trade_id.c_str());
									BroadcastInformation(_bstr_t(szSym));
									CTracer::TraceMessage(CTracer::enMtInformation, NULL, szSym);
									dwRes = ERROR_INVALID_DATA;
									break;
								case -3: //unable to find option
									_snprintf_s(szSym, sizeof(szSym), sizeof(szSym), "Unable to find option '%s' with root '%s' in DB.", pTrade->base_sym.c_str(), pTrade->trading_sym.c_str());
									BroadcastInformation(_bstr_t(szSym));
									CTracer::TraceMessage(CTracer::enMtInformation, NULL, szSym);
									dwRes = ERROR_INVALID_DATA;
									break;
								case -6:
									_snprintf_s(szSym, sizeof(szSym), sizeof(szSym), "Unknown underlying '%s' for future contract '%s'.", pTrade->base_sym.c_str(), pTrade->MakeTradingSymbol().c_str());
									BroadcastInformation(_bstr_t(szSym));
									CTracer::TraceMessage(CTracer::enMtInformation, NULL, szSym);
									dwRes = ERROR_INVALID_DATA;
									break;
								case -7:
									_snprintf_s(szSym, sizeof(szSym), sizeof(szSym), "Unknown future root '%s' for underlying '%s'.", pTrade->trading_sym.c_str(), pTrade->base_sym.c_str());
									BroadcastInformation(_bstr_t(szSym));
									CTracer::TraceMessage(CTracer::enMtInformation, NULL, szSym);
									dwRes = ERROR_INVALID_DATA;
									break;
								case -8:
									_snprintf_s(szSym, sizeof(szSym), sizeof(szSym),"Unable to find future '%s' in DB.",pTrade->MakeTradingSymbol().c_str());
									BroadcastInformation(_bstr_t(szSym));
									CTracer::TraceMessage(CTracer::enMtInformation, NULL, szSym);
									dwRes = ERROR_INVALID_DATA;
									break;
								default:
									_com_issue_error( E_FAIL );
									break;
							}break;
                        }
                        else
                        {
						    dwRes = ERROR_ALREADY_EXISTS;                            
                            if ( ReProcessTrade(pTrade, lMinSeqNum) == ERROR_SUCCESS )
                                dwRes = ERROR_SUCCESS;                            
                            break;
                        }
                    }
                    
                    CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Trade '%s' stored in database."), pTrade->trade_id.c_str());
				    InterlockedIncrement((LPLONG)&m_dwDBStored);
                    
                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
                    sp_get << (int)lMaxSeqNum;
                    sp_get << (int)lMaxSeqNum;
                    sp_get << CDBNull();
                    sp_get << (unsigned char)1;
					
                    rs.Open (sp_get);
					
                    if(rs.IsEOF())
					{
						CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
							pTrade->trade_id.c_str());
						dwRes = ERROR_NO_DATA_DETECTED;
					}
					else
					{
						HRESULT hRes;
						if ( FAILED(hRes = FillTradeUpdate(rs, pTrade)) )
						{
							CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to fill TradeUpdate message."));
							dwRes = ERROR_INVALID_PARAMETER;
						}
						else if(hRes == S_FALSE)
						{
							CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
								pTrade->trade_id.c_str());
							dwRes = ERROR_NO_DATA_DETECTED;
						}
					}
				}
				break;
			case DEL_FLAG:
				{
					CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_Del");
					sp << _bstr_t(pTrade->trade_id.c_str());

					sp << (vt_date)pTrade->execution_date;
					rs.Open (sp);

                    if(rs.IsEOF())
                    {
                        rs.Close();
                        _com_issue_error(E_FAIL);
                    }
                    
                    lMaxSeqNum = rs[L"iMaxSeqNum"];
                    lMinSeqNum = rs[L"iMinSeqNum"];
                    rs.Close();
                    
                    if (lMaxSeqNum == -1 && lMinSeqNum == -1) // unknown error
                        _com_issue_error(E_FAIL);

					if (lMinSeqNum == -1) // already exists
                    {                            
					    dwRes = ERROR_ALREADY_EXISTS;
                        if ( ReProcessTrade(pTrade, lMaxSeqNum) == ERROR_SUCCESS )
                            dwRes = ERROR_SUCCESS;
                        break;
                    }
					
                    if (lMaxSeqNum == -1)
                    {
                        CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to store trade in database - trade with the same ExecID '%s' either does not exist or was already deleted."),
    	    		        pTrade->trade_id.c_str());
						dwRes = ERROR_FILE_NOT_FOUND;
                        break;
                    }

                    CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Void Trade '%s'"), pTrade->trade_id.c_str());
					InterlockedIncrement((LPLONG)&m_dwDBDeleted);
                    
                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
                    sp_get << (int)lMaxSeqNum;
					sp_get << (int)lMaxSeqNum;
                    sp_get << CDBNull();
                    sp_get << (unsigned char)1;

                    rs.Open (sp_get);
					
                    if(rs.IsEOF())
					{
						CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
							pTrade->trade_id.c_str());
						dwRes = ERROR_NO_DATA_DETECTED;
					}
					else
					{
                        HRESULT hRes;
					    if(FAILED(hRes = FillTradeUpdate(rs, pTrade)))
                        {
   					        CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to fill TradeUpdate message."));
					        dwRes = ERROR_INVALID_PARAMETER;
                        }
					    else if(hRes == S_FALSE)
                        {
   					        CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to retrieve trade from database - trade with the same ExecID '%s' does not exist."),
					 	        pTrade->trade_id.c_str());
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

			CTracer::TraceMessage(CTracer::enMtError, NULL, bsError);

            pair<TDBFailsStore::iterator, bool> pairResult = 
                  m_DBFails.insert(pTrade->trade_id);

            if (--dwTries)
			{				
                Reconnect();
			}
			else
			{
				CTracer::TraceMessage(CTracer::enMtError, NULL, _T("CRITICAL ERROR : FAILED TO STORE TRADE IN DATABASE."));

				if(/*Uncomment this Oleg StoreInFile(Trade) != 0*/false)
				{
					CTracer::TraceMessage(CTracer::enMtError, NULL, _T("CRITICAL ERROR : FAILED TO STORE TRADE IN FILE."));
					CTracer::TraceMessage(CTracer::enMtError, NULL, _T("MANUAL INTERVENTION IS REQUIRED. THE APPLICATION IS STOPPED."));
					ExitProcess(-1);
				}
				else
				{
					FlushTrade(pTrade);
                    BroadcastError(e.Error(), bsError);
                    dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
					bOk = true;
				}
			}
		}

	} while(!bOk);

	return dwRes;
};

DWORD CPublisher::StoreInFile(const CTradePtr& Trade)
{
/*	TCHAR	szBuf[8192];
	string	sExecTransType;
	string	sOrderStatus;
	string	sSide;

	USES_CONVERSION;

	switch(pTrade->cExecTransType)
	{
		case ETT_NEW: sExecTransType = "NEW"; break;
		case ETT_CANCEL: sExecTransType = "CANCEL"; break;
		default: sExecTransType = "UNEXPECTED"; break;
	}

	switch(pTrade->cOrderStatus)
	{
		case OS_PARTIALLY_FILLED: sOrderStatus = "PARTIALLY FILLED"; break;
		case OS_FILLED: sOrderStatus = "FILLED"; break;
		default: sOrderStatus = "UNEXPECTED"; break;
	}

	switch(pTrade->cSide)
	{
		case SIDE_BUY: sSide = "BUY"; break;
		case SIDE_SELL: sSide = "SELL"; break;
		default: sSide = "UNEXPECTED"; break;
	}

//	CFixTagValue	tagTransactTime = pTrade->ftTransactTime;
	string			sTransactTime = "time";//((char*)tagTransactTime.GetData(), tagTransactTime.GetLength());

//	CFixTagValue	tagTradeDate = pTrade->fdTradeDate;
	string			sTradeDate = "date";//((char*)tagTradeDate.GetData(), tagTradeDate.GetLength());

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

	if(pTrade->sSecurityType == T2A(ST_OPTION))
	{
        TCHAR szOptBuf[1024] = {0};

//		CFixTagValue	tagMaturityMonthYear = pTrade->fmyMaturityMonthYear;
		string			sMaturityMonthYear = "year";//((char*)tagMaturityMonthYear.GetData(), tagMaturityMonthYear.GetLength());

//		CFixTagValue	tagMaturityDay = pTrade->fmdMaturityDay;
		string			sMaturityDay = "day";//((char*)tagMaturityDay.GetData(), tagMaturityDay.GetLength());

		string sPutOrCall;

		switch(pTrade->iPutOrCall)
		{
			case PC_PUT :
                sPutOrCall = "PUT";
                break;
			case PC_CALL :
                sPutOrCall = "CALL";
                break;
			default :
                sPutOrCall = "UNEXPECTED";
		}

		_stprintf(szOptBuf, _T("%s%s\t%s\t%f\t%s\n"),

						  sMaturityMonthYear.c_str(),
						  sMaturityDay.c_str(),
						  sPutOrCall.c_str(),
						  pTrade->fStrikePrice,
						  pTrade->sUnderlying.c_str()
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
*/
	return 0;
}

DWORD CPublisher::Publish(const CTradePtr& pTrade)
{
	try
	{
		m_spPubManager->PubTradeUpdate(m_spTradeUpdate);

		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Trade '%s' published."), pTrade->trade_id.c_str());		
		InterlockedIncrement((LPLONG)&m_dwPublished);
	}
	catch(_com_error& e)
	{
		CTracer::TraceMessage(CTracer::enMtWarning, NULL, _T("WARNING. Failed to publish trade : 0x%x '%s'."), e.Error(), (LPCSTR)e.Description());
	}
	return 0;
};

LONG CPublisher::GetConnectionString()
{
	try
	{
		HRESULT hr = m_pEtsSettings.CreateInstance(__uuidof (Settings));
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");

		BSTR bsConnectionString;

		hr = m_pEtsSettings->get_DatabaseConnection(&bsConnectionString);
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to get Connection String.");

		m_bsConnectionString = bsConnectionString;

		SysFreeString(bsConnectionString);
	}
	catch(_com_error& e)
	{
		m_pEtsSettings = NULL;
		return TraceError(e.Error(), e.Description());
	}

	m_pEtsSettings = NULL;

	return 0;
}


bool  CPublisher::Reconnect()
{
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Trying to reconnect..."));

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
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Reconnection done."));
		m_Connection.Open(m_bsConnectionString);

		_bstr_t bsInitialCatalog = m_Connection.GetAdoConnectionPtr()->Properties->Item[L"Initial Catalog"]->Value;
		_bstr_t bsDataSource = m_Connection.GetAdoConnectionPtr()->Properties->Item[L"Data Source"]->Value;
		//Trace(_T("------------------------------------"));
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T(" - DB host: %s"), (LPCTSTR)bsDataSource );	
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T(" - DB name: %s"), (LPCTSTR)bsInitialCatalog );	
		//Trace(_T("------------------------------------"));

		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Reconnection done."));
	}
	catch(_com_error&)
	{
		CTracer::TraceMessage(CTracer::enMtError, NULL, m_Connection.GetErrorMessage());
	} 
	return true;
}

DWORD CPublisher::ReProcessTrade(CTradePtr& pTrade, long lSeqNum)
{
    DWORD dwRes = ERROR_SUCCESS;

    TDBFailsStore::iterator it = m_DBFails.find(pTrade->trade_id);
    if ( it == m_DBFails.end() )
        return ERROR_NO_DATA;

    m_DBFails.erase(it);
    
    CClientRecordset rs;
	EgLib::CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
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
    catch (const _com_error& e)
    {        
        CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to reprocess trade with ExecID = '%s': %s"), pTrade->trade_id.c_str(),
            (char*)e.Description() );	
        dwRes = ERROR_DATABASE_FAILURE;
    }
        
    HRESULT hRes = FillTradeUpdate(rs, pTrade);
		
    if( FAILED(hRes) )
    {	 
        CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to reprocess trade with ExecID = '%s': unable to fill TradeUpdate message"),
            pTrade->trade_id.c_str() );
        dwRes = ERROR_INVALID_PARAMETER;
    }
	else if(hRes == S_FALSE)
	{
	    CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to reprocess trade with ExecID = '%s': unable to retrieve trade from database."),
            pTrade->trade_id.c_str());
        dwRes = ERROR_NO_DATA_DETECTED;
	}                   
    
    return dwRes;
}

HRESULT CPublisher::FlushTrade(const CTradePtr &pTrade)
{
/*    time_t cur_time;
    time(&cur_time);
    tm *ptm = gmtime(&cur_time);

    BSTR        bsOut;
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
    vtVal = _variant_t(pTrade->fLastPrice);
    HRESULT hr = VarFormatNumber( &vtVal, 2, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;
    
    ::SysFreeString(bsOut);

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    vtVal = _variant_t((long)pTrade->iLastShares);
    hr = VarFormatNumber( &vtVal, 0, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;

    ::SysFreeString(bsOut);

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
    vtVal = pTrade->dtTransactTime;
    hr = VarFormatNumber( &vtVal, 0, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;

    ::SysFreeString(bsOut);

    sFlush += "|";    
    
    ///////////////////////////////////////////////////////////////////
    ptm->tm_year = 0;//pTrade->fdTradeDate.tm_year;
    ptm->tm_mon = 0;//rade.fdTradeDate.tm_mon;
    ptm->tm_mday = 0;//pTrade->fdTradeDate.tm_mday;    
        
    vtVal = _variant_t( mktime(ptm) );
    hr = VarFormatNumber( &vtVal, 0, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;    

    ::SysFreeString(bsOut);

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sSecurityType;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sText;
    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    ptm = gmtime(&cur_time);

    if ( pTrade->sSecurityType == "OPT" )
    {
        ptm->tm_year = 0;//pTrade->fmyMaturityMonthYear.tm_year;
        ptm->tm_mon = 0;//pTrade->fmyMaturityMonthYear.tm_mon;
    }        
    
    vtVal = _variant_t( mktime(ptm) );
    hr = VarFormatNumber( &vtVal, 0, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;    

    ::SysFreeString(bsOut);

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    vtVal = _variant_t((long)pTrade->iPutOrCall);
    hr = VarFormatNumber( &vtVal, 0, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;    

    ::SysFreeString(bsOut);

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////    
    vtVal = _variant_t(pTrade->fStrikePrice);
    hr = VarFormatNumber( &vtVal, 2, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;
    
    ::SysFreeString(bsOut);

    sFlush += "|";    

    ///////////////////////////////////////////////////////////////////
    ptm = gmtime(&cur_time);

    if ( pTrade->sSecurityType == "OPT" )
    {
        ptm->tm_mday = 0;//pTrade->fmdMaturityDay.tm_mday;
    }

    vtVal = _variant_t( mktime(ptm) );
    hr = VarFormatNumber( &vtVal, 0, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        sFlush += (char*)_bstr_t(bsOut);
    }
    else
        return E_INVALIDARG;

    ::SysFreeString(bsOut);

    sFlush += "|";

    ///////////////////////////////////////////////////////////////////
    sFlush += pTrade->sUnderlying;

    ///////////////////////////////////////////////////////////////////
    
    vtVal = _variant_t((long)sFlush.size());
    hr = VarFormatNumber( &vtVal, 0, -2, -2, 0, 0, &bsOut);
    if ( SUCCEEDED(hr) )
    {        
        string sString = sFlush;
        sFlush = (char*)_bstr_t(bsOut);
        sFlush += sString;
    }
    else
        return E_INVALIDARG;

    m_fFailesFile << sFlush <<endl;
*/    
    return S_OK;
}

HRESULT CPublisher::BroadcastInformation(_bstr_t& bsDesc)
{    
	try
	{
		m_spBroadcastMessage->PutCode(S_OK);
		m_spBroadcastMessage->PutDescription(bsDesc);

		m_spBroadcastMessage->PutPriority(enPrHigh);
		m_spBroadcastMessage->PutSeverity(enSevInfo);
		m_spBroadcastMessage->PutFacility(enFacRTTF);

		PublishBroadcastMessage();        
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

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

void CPublisher::PublishBroadcastMessage() throw(...)
{
    if (m_spPubManager == NULL)
        utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"PubManager object wasn't created.");
    if (m_spBroadcastMessage == NULL)
        utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"BroadcastMessage object wasn't created.");

    m_spPubManager->PubBroadcastMessage(m_spBroadcastMessage);

    CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("BroadcastMessage successfully published."));
}
