#include "stdafx.h"
#include "oledberr.h"

#include "MainFrm.h"
#include "Publisher.h"
#include "Settings.h"
#include "tracer.h"

#define _ERROR_NEED_TO_STOP   (DWORD)-10

#define DBRECONNECTION_DELAY 10
#define MAX_DBRECONNECTION_TRIES 5

#define NEWTRADEERROR		-1
#define NEWTRADENOTFOUND	 0
#define NEWTRADEFOUND		 1
#define TRADE_ENDPART		 2


vt_date ConvertLocalToUTC(DATE fLocalDate)
{
    TIME_ZONE_INFORMATION tzInfo;
    LONG    nTimeZoneBias;
    vt_date dtLocal(fLocalDate);

    DWORD nResult = GetTimeZoneInformation(&tzInfo);
    if (nResult != TIME_ZONE_ID_INVALID)
    {
        nTimeZoneBias = tzInfo.Bias;
        
        if (nResult == TIME_ZONE_ID_DAYLIGHT)
            nTimeZoneBias += tzInfo.DaylightDate.wMinute;
    }
    else
        nTimeZoneBias = 0;
        
    return (dtLocal += vt_date_span(0,0, nTimeZoneBias));
}

void FillTrade(const CClientRecordset &rs, CTrade &trade)
{
    trade.iSeqID	= rs[L"iSeqID"];
    trade.vcAcronym = rs[L"vcAcronym"];
    trade.nTradeID	= rs[L"nTradeID"];
    trade.siStatus	= rs[L"siStatus"];
    trade.vcSymbol	= rs[L"vcSymbol"];
    trade.vcAction	= rs[L"vcAction"];
    trade.tiIsBuy   = rs[L"tiIsBuy"];
    trade.fUnits	= fabs((double)rs[L"fUnits"]);
    trade.fPrice	= rs[L"fPrice"];
    trade.dtTradeDate= ConvertLocalToUTC(rs[L"dtTradeDate"]);
    trade.vcHedgeGroup= rs[L"vcHedgeGroup"];
}

CPublisher::CPublisher() : 		
    m_nMode(-1)
{
    Clear();
}

CPublisher::~CPublisher() 
{ 
}

void CPublisher::Clear()
{
	m_dwDBStored  = 0;
	m_dwDBDeleted = 0;
	m_dwPublished = 0;
	m_dwNotPublished = 0;
	m_dwReceived  = 0;
}

void CPublisher::SetMode(UINT nMode) 
{ 
    m_nMode = nMode;
}

UINT  CPublisher::Run()
{
    if (MODE_INTRADAY == m_nMode)
        return ThreadFuncForIntradayMode(this);
    else if (MODE_ENDOFADAY == m_nMode)
        return ThreadFuncForEndofdayMode(this);

    return -1;
};

DWORD CPublisher::Start(void* pParam)
{
    Clear();

    try
    {
        if ( !CSettings::IsLoaded() )
            CSettings::LoadSettings();    
    }
	catch(_com_error& e)
    {
		CTracer::Trace(CTracer::enMtWarning, e.Description());
    }
    catch(...)
    {
        return TraceError(E_UNEXPECTED, _T("Unexpected error"));
    }
    
	// Database...
	// Source
	try
	{
        m_ConnectionSource.Open(CSettings::m_bsExchange_DB_Path);
        m_ConnectionDest.Open(CSettings::m_bsETS_DB_Path);


		CTracer::Trace(CTracer::enMtInformation, _T("Connection to databases is established."));
	}
	catch(_com_error& e)
	{
		m_ConnectionSource.Close();
		m_ConnectionDest.Close();

		_bstr_t bsError = m_ConnectionSource.GetErrorMessage();
		if( bsError.length() == 0 )
				bsError = m_ConnectionDest.GetErrorMessage();
		if( bsError.length() == 0 )
				bsError = L"Please check your settings";
		bsError = "Failed to connect to database : " + bsError;

		CTracer::Trace(CTracer::enMtError, bsError);
		return e.Error();
	}
	
	// Transport...
	try
	{
		CTracer::Trace(CTracer::enMtInformation, _T("Initializing publisher..."));

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

		CTracer::Trace(CTracer::enMtInformation, _T("Publisher initialized."));
	}
	catch(_com_error& e)
	{
		return TraceError(e.Error(), e.Description());
	}
    catch(...)
    {
        return TraceError(E_UNEXPECTED, _T("Unexpected error"));
    }

	CTracer::Trace(CTracer::enMtInformation, _T("Starting CIBC Trade Feed Adapter..."));
	
    DWORD dwRes = CThread::Start();
    if (ERROR_SUCCESS == dwRes)
	    CTracer::Trace(CTracer::enMtInformation, _T("CIBC Trade Feed Adapter successfully started."));
    else
        CTracer::Trace(CTracer::enMtError, _T("Failed to start CIBC Trade Feed Adapter."));
	
    return dwRes;
};

DWORD CPublisher::Stop(DWORD dwWait)
{
	TRACE0("Stop Event Came");

	CTracer::Trace(CTracer::enMtInformation, _T("Stopping CIBC Trade Feed Adapter..."));
	CThread::Stop(true, dwWait);
	CTracer::Trace(CTracer::enMtInformation, _T("CIBC Trade Feed Adapter successfully stopped."));
	
    CTracer::Trace(CTracer::enMtInformation, 
        _T("Session statistic: Received: %lu, Stored: %lu, Deleted: %lu, Published: %lu, Not published: %lu"),
	    m_dwReceived, m_dwDBStored, m_dwDBDeleted, m_dwPublished, m_dwNotPublished);
        /*CTracer::Trace(CTracer::enMtInformation, _T("Received: %lu"), m_dwReceived);
	CTracer::Trace(CTracer::enMtInformation, _T("Stored: %lu"), m_dwDBStored);
	CTracer::Trace(CTracer::enMtInformation, _T("Deleted: %lu"), m_dwDBDeleted);
	CTracer::Trace(CTracer::enMtInformation, _T("Published: %lu"), m_dwPublished);
	CTracer::Trace(CTracer::enMtInformation, _T("Not published: %lu"), m_dwNotPublished);*/
	
    try
	{
        TRACE0("m_ConnectionSource.Close()");
		m_ConnectionSource.Close();
		TRACE0("m_ConnectionDest.Close()");
		m_ConnectionDest.Close();
	}
	catch(_com_error&) { }

	try
	{
		TRACE0("m_spPubManager->UnregPublisher");
		if (m_spPubManager)
			m_spPubManager->UnregPublisher(enMtTradeUpdate, NULL);

		// give time for transport to prepare for shutdown..
		Sleep(2000);

		m_spTradeUpdate = 0;
		m_spPubManager = 0;
	}
	catch(_com_error&) { }
	return 0;
}

HRESULT CPublisher::FillTradeUpdate(CClientRecordset& rs, CTrade& Trade)
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
				CTracer::Trace(CTracer::enMtError, _T("Error. Unknown SymbolType for ContractTypeID = %d."), iContractTypeID);
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
//  not existed      m_spTradeUpdate-> FutID =  rs[L"iFutureID"];
//  not existed      m_spTradeUpdate->FutRootID =  rs[L"iFutureRootID"];

		if(Trade.siStatus == NEW_FLAG || Trade.siStatus == UPD_FLAG)
		{
			long lStructureMissed = rs[L"iStructureMissed"];
			m_spTradeUpdate->MsState = (TRADE_MS_STATE)lStructureMissed;
		}
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

	return S_OK;
}

DWORD CPublisher::StoreInDatabase(CTrade& Trade)
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
			switch(Trade.siStatus)
			{
            case NEW_FLAG:
            case UPD_FLAG:
			    {                    
                    CClientRecordset rs;
					CStoredProc<> sp(m_ConnectionDest, L"usp_TradeFeed_1_New");

					sp << Trade.vcAcronym;
					sp << _bstr_t(_variant_t((long)Trade.nTradeID));
					sp << (int)( Trade.vcAction == ACTION_BUY ? 1 : 0 );
					sp << (int)Trade.fUnits;
					sp << Trade.fPrice;
					sp << Trade.dtTradeDate;
					sp << Trade.vcSymbol;
					sp << int(0); // Contract Type s Stock
                    sp << CDBNull();
					sp << CDBNull();
					sp << CDBNull();
					sp << CDBNull();
					sp << CDBNull();
                    sp << (int)1; //update enabled
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
                    rs.Close();
                    
                    if ( lMaxSeqNum == -1 && lStructureMissed == -1 )
                    {
                        if (lMinSeqNum == -1) // unknown error
                        {
                            _com_issue_error( E_FAIL );
                        }
                        else
                        {
						    dwRes = ERROR_ALREADY_EXISTS;                            
                            if ( ReProcessTrade(Trade, lMinSeqNum) == ERROR_SUCCESS )
                                dwRes = ERROR_SUCCESS;                            
                            break;
                        }
                    }
                    
                    CTracer::Trace(CTracer::enMtInformation, _T("Trade '%d' stored in database."), Trade.nTradeID);
				    InterlockedIncrement((LPLONG)&m_dwDBStored);
                    
                    CStoredProc<> sp_get(m_ConnectionDest, L"usp_TradeSeq_Get");
                    sp_get << (int)lMinSeqNum;
                    sp_get << (int)lMaxSeqNum;
                    sp_get << CDBNull();
                    sp_get << (UCHAR)1;
					
                    rs.Open (sp_get);
					
                    if(rs.IsEOF())
					{
						CTracer::Trace(CTracer::enMtError, _T("Failed to retrieve trade from database - trade with ExecID '%d' does not exist."), Trade.nTradeID);
						dwRes = ERROR_NO_DATA_DETECTED;
					}
					else
					{
						HRESULT hRes;

                        while (!rs.IsEOF() && GetMode() == MODE_INTRADAY)
                        {
                            if ( FAILED(hRes = FillTradeUpdate(rs, Trade)) )
                            {
                                CTracer::Trace(CTracer::enMtError, _T("Failed to fill TradeUpdate message."));
                                dwRes = ERROR_INVALID_PARAMETER;
                            }
                            else if(hRes == S_FALSE)
                            {
                                CTracer::Trace(CTracer::enMtError, _T("Failed to retrieve trade from database - trade with ExecID '%d' does not exist."),
                                    Trade.nTradeID);
                                dwRes = ERROR_NO_DATA_DETECTED;
                            }

					        Publish(Trade);
							if (::WaitForSingleObject(GetStopEventHandle(), CSettings::m_nPublishDelayTime) == WAIT_OBJECT_0)
							{
								return _ERROR_NEED_TO_STOP;
							}

                            rs.MoveNext();

                        }
					}
				}
				break;
			case DEL_FLAG:
				{
					CClientRecordset rs;
					CStoredProc<> sp(m_ConnectionDest, L"usp_TradeFeed_1_Del");
					sp << _bstr_t(_variant_t((long)Trade.nTradeID));
					sp << Trade.dtTradeDate;
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
                        if ( ReProcessTrade(Trade, lMaxSeqNum) == ERROR_SUCCESS )
                            dwRes = ERROR_SUCCESS;
                        break;
                    }
					
                    if (lMaxSeqNum == -1)
                    {
                        CTracer::Trace(CTracer::enMtWarning, _T("Failed to delete trade in database - trade with the same ExecID '%d' either does not exist or was already deleted."),
    	    		        Trade.nTradeID);
						dwRes = ERROR_NOT_FOUND;
                        break;
                    }

                    CTracer::Trace(CTracer::enMtInformation, _T("Trade '%d' deleted from database."), Trade.nTradeID);
					InterlockedIncrement((LPLONG)&m_dwDBDeleted);
                    
                    CStoredProc<> sp_get(m_ConnectionDest, L"usp_TradeSeq_Get");
                    sp_get << (int)lMaxSeqNum;
					sp_get << (int)lMaxSeqNum;
                    sp_get << CDBNull();
                    sp_get << (unsigned char)1;

                    rs.Open (sp_get);
					
                    if(rs.IsEOF())
					{
						CTracer::Trace(CTracer::enMtError, _T("Failed to retrieve trade from database - trade with ExecID '%d' does not exist."),
							Trade.nTradeID);
						dwRes = ERROR_NO_DATA_DETECTED;
					}
					else if (GetMode() == MODE_INTRADAY)
                    {
                        HRESULT hRes;
					    if(FAILED(hRes = FillTradeUpdate(rs, Trade)))
                        {
   					        CTracer::Trace(CTracer::enMtError, _T("Failed to fill TradeUpdate message."));
					        dwRes = ERROR_INVALID_PARAMETER;
                        }
					    else if(hRes == S_FALSE)
                        {
   					        CTracer::Trace(CTracer::enMtError, _T("Failed to retrieve trade from database - trade with the same ExecID '%d' does not exist."),
					 	        Trade.nTradeID);
					        dwRes = ERROR_NO_DATA_DETECTED;
                        } 
                        
					    Publish(Trade);
						if (::WaitForSingleObject(GetStopEventHandle(), CSettings::m_nPublishDelayTime) == WAIT_OBJECT_0)
						{
							return _ERROR_NEED_TO_STOP;
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
				m_ConnectionDest.GetErrorMessage();

			CTracer::Trace(CTracer::enMtError, bsError);

            pair<TDBFailsStore::iterator, bool> pairResult = 
                  m_DBFails.insert(Trade.nTradeID);

            if (--dwTries)
			{				
                Reconnect();
			}
			else
			{
				CTracer::Trace(CTracer::enMtError, _T("CRITICAL ERROR : FAILED TO STORE TRADE IN DATABASE."));

				if(false)//Uncomment this Oleg StoreInFile(Trade) != 0
				{
					CTracer::Trace(CTracer::enMtError, _T("CRITICAL ERROR : FAILED TO STORE TRADE IN FILE."));
					CTracer::Trace(CTracer::enMtError, _T("MANUAL INTERVENTION IS REQUIRED. THE APPLICATION IS STOPPED."));
					ExitProcess(-1);
				}
				else
				{
                    BroadcastError(e.Error(), bsError);
                    dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
					bOk = true;
				}
			}
		}

	} while(!bOk);
	
	return dwRes;
};


DWORD CPublisher::Publish(const CTrade& trade)
{
	try
	{
		m_spPubManager->PubTradeUpdate(m_spTradeUpdate);
		CTracer::Trace(CTracer::enMtInformation, _T("Trade '%d' published."), trade.nTradeID);		
        InterlockedIncrement((LPLONG)&m_dwPublished);
	}
	catch(_com_error& e)
	{
		CTracer::Trace(CTracer::enMtWarning, _T("WARNING. Failed to publish trade : 0x%x '%s'."), e.Error(), e.Description());
		InterlockedIncrement((LPLONG)&m_dwNotPublished);
	}
	return 0;
};

//CString CPublisher::GetConnectionString(LPCTSTR key)
//{
	//CString str;

   	//CEgRegKey regkey;
	//LONG lResult = regkey.Open(HKEY_LOCAL_MACHINE /*HKEY_CURRENT_USER*/, SETTINGS_REGISTRY_KEY, KEY_READ);
    //if (lResult == ERROR_SUCCESS)
    //{
       	//lResult = regkey.QueryValue(str, key);
        //regkey.Close();
    //}
	//return str;
//}


bool  CPublisher::Reconnect()
{
	CTracer::Trace(CTracer::enMtInformation, _T("Trying to reconnect..."));

	try
	{
		m_ConnectionSource.Close();
	}
	catch(_com_error&)
	{
	}

	Sleep(DBRECONNECTION_DELAY);

	try
	{
		m_ConnectionSource.Open(m_bsConnectionStringSource);
	}
	catch(_com_error&)
	{
		CTracer::Trace(CTracer::enMtError, m_ConnectionSource.GetErrorMessage());
	}

	CTracer::Trace(CTracer::enMtInformation, _T("Reconnection done."));

	return true;
}

DWORD CPublisher::ReProcessTrade(CTrade& Trade, long lSeqNum)
{
   DWORD dwRes = ERROR_SUCCESS;

    TDBFailsStore::iterator it = m_DBFails.find(Trade.nTradeID);
    if ( it == m_DBFails.end() )
        return ERROR_NO_DATA;

    m_DBFails.erase(it);
    
    CClientRecordset rs;
	CStoredProc<> sp_get(m_ConnectionDest, L"usp_TradeSeq_Get");
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
        CTracer::Trace(CTracer::enMtError, _T("Failed to reprocess trade with ExecID = '%d': %s"), 
			Trade.nTradeID, (char*)e.Description() );	
        dwRes = ERROR_DATABASE_FAILURE;
    }
        
    HRESULT hRes = FillTradeUpdate(rs, Trade);
		
    if( FAILED(hRes) )
    {	 
        CTracer::Trace(CTracer::enMtError, _T("Failed to reprocess trade with ExecID = '%d': unable to fill TradeUpdate message"),
            Trade.nTradeID);
        dwRes = ERROR_INVALID_PARAMETER;
    }
	else if(hRes == S_FALSE)
	{
	    CTracer::Trace(CTracer::enMtError, _T("Failed to reprocess trade with ExecID = '%d': unable to retrieve trade from database."),
            Trade.nTradeID);
        dwRes = ERROR_NO_DATA_DETECTED;
	}                   
    
    return dwRes;
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

    CTracer::Trace(CTracer::enMtInformation, _T("BroadcastMessage successfully published."));
}


DWORD CPublisher::GetTrades(CClientRecordset &rs, CTrade& trade)
{
	try
	{
		if(!rs.IsOpened())
		{
			CStoredProc<> sp(m_ConnectionSource, L"sp_Trade_Select_UnProcessed");
			sp << CSettings::m_nTradesCount;
			rs.Open(sp);

			if(rs.IsEOF()) 
			{
				rs.Close();
				CTracer::Trace(CTracer::enMtInformation, _T("New Trade Not Found."));
				return NEWTRADENOTFOUND;
			}
		}

		if(rs.IsOpened())
		{
            FillTrade(rs, trade);

            rs.MoveNext();

			if(rs.IsEOF()) 
			{
				rs.Close();
				return TRADE_ENDPART | NEWTRADEFOUND;
			}
			else
				return NEWTRADEFOUND;
		}
	}
	catch(_com_error& err)
	{
        CTracer::Trace(CTracer::enMtError, _T("Failed to GetTrades - %s"), (char*)err.Description() );	
	}
	return NEWTRADEERROR;
}

void CPublisher::SetTradeStatus(const CTrade& trade, TradeStatusesEnum eStat)
{
	CStoredProc<> sp(m_ConnectionSource, "sp_Trade_ProcessedAs");
	sp << trade.iSeqID;
	sp << (int) eStat;
	sp.Execute();
	DWORD dw = sp.GetResult();
}

UINT  CPublisher::ThreadFuncForIntradayMode(void* pParam)
{

	const HANDLE    hStopEvent = GetStopEventHandle();
	const HANDLE    hEvents[]  = {hStopEvent};
    CPublisher*     pT = reinterpret_cast<CPublisher*>(pParam);
	CTrade trade;
	DWORD			dwRes = 0;
	
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CClientRecordset rs;

	while(::WaitForSingleObject(hStopEvent, 10) != WAIT_OBJECT_0)
	{
		while( (dwRes = pT->GetTrades(rs, trade)) != NEWTRADEERROR)
		{
			if(dwRes == NEWTRADENOTFOUND) 
			{
				::WaitForSingleObject(hStopEvent, CSettings::m_nWaitingTime);
				break;
			}

			if(dwRes & NEWTRADEFOUND)
			{
				InterlockedIncrement((LPLONG)&pT->m_dwReceived);
					
				DWORD dwRes = StoreInDatabase(trade);
				if(dwRes == ERROR_SUCCESS)
                {
				    SetTradeStatus(trade, tsProcessed);
                }
                else if (dwRes == ERROR_NOT_FOUND)
                {
				    SetTradeStatus(trade, tsIgnored);
                }
				else if (dwRes == _ERROR_NEED_TO_STOP)
				{
					break;
				}

                if(dwRes != ERROR_SUCCESS) 
				{
					CTracer::Trace(CTracer::enMtInformation, _T("Trade with ExecID = '%d' was not published."), trade.nTradeID );
				}
			}

			if(dwRes & TRADE_ENDPART)
			{
				break;
			}
		}
	} 
    
	CoUninitialize();
	return 0;
}

UINT  CPublisher::ThreadFuncForEndofdayMode(void* pParam)
{

	const HANDLE    hStopEvent = GetStopEventHandle();
	const HANDLE    hEvents[]  = {hStopEvent};
    CPublisher*     pT = reinterpret_cast<CPublisher*>(pParam);
	DWORD  dwRes = 0;
    CClientRecordset rs;
    
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if(::WaitForSingleObject(hStopEvent, 10) != WAIT_OBJECT_0)
	{
		if ( SelectSnapShotTrades(rs) != ERROR_NOT_FOUND)
        {
            PutSnapShotTrades(rs);
            rs.Close();
            
            ClearTradeFeed();
        }
	} 

    /* to ask main window to stop this thread */ 
    ::PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, WM_STOP_ENDOFDAY_PROCESS, 0, 0);

	while(::WaitForSingleObject(hStopEvent, 10) != WAIT_OBJECT_0)
	{
    }
    
	CoUninitialize();
	return 0;
}

DWORD CPublisher::PutSnapShotTrades(CClientRecordset &rsSnapShotTrades)
{
    /* to delete all processed trades from ETS DB */
	try
	{
        CClientRecordset rs;
        CStoredProc<> sp(m_ConnectionSource, L"sp_Trade_ActiveProcessed_Select");
        rs.Open(sp);
        
        
        while ( !rs.IsEOF() )
        {
			InterlockedIncrement((LPLONG)&m_dwReceived);

            CTrade trade;

            FillTrade(rs, trade);
            StoreInDatabase(trade);

            rs.MoveNext();

			if (::WaitForSingleObject(GetStopEventHandle(), 0) == WAIT_OBJECT_0)
			{
				return _ERROR_NEED_TO_STOP;
			}
			
        }
    }
	catch(_com_error& err)
	{
        CTracer::Trace(CTracer::enMtError, _T("Failed to Get All Processed Intraday Trades.%s"), (char*)err.Description() );	
	}

    
    /* to insert all End-of-Day trades into ETS DB */
	try
	{
        while ( !rsSnapShotTrades.IsEOF() )
        {
			InterlockedIncrement((LPLONG)&m_dwReceived);

            CTrade trade;

            FillTrade(rsSnapShotTrades, trade);
            StoreInDatabase(trade);
            
            rsSnapShotTrades.MoveNext();

			if (::WaitForSingleObject(GetStopEventHandle(), 0) == WAIT_OBJECT_0)
			{
				return _ERROR_NEED_TO_STOP;
			}
        }
    }
	catch(_com_error& err)
	{
        CTracer::Trace(CTracer::enMtError, _T("Failed to process End-of-Day Trades.%s"), (char*)err.Description() );	
	}
    
    return ERROR_SUCCESS;
}

UINT CPublisher::SelectSnapShotTrades(CClientRecordset &rs)
{
	try
	{
        if (rs.IsOpened())
            rs.Close();
        
        CStoredProc<> sp(m_ConnectionSource, L"sp_TradeSnapShot_Select");
        rs.Open(sp);
        
        if(rs.IsEOF()) 
        {
            rs.Close();
            CTracer::Trace(CTracer::enMtInformation, _T("End-of-Day Trades Not Found."));
            return ERROR_NOT_FOUND;
        }
    }
    catch(_com_error& err)
    {
        CTracer::Trace(CTracer::enMtError, _T("Failed to Get End-of-Day Trades.%s"), (char*)err.Description() );	
    }
    
    return ERROR_SUCCESS;
}

UINT  CPublisher::ClearTradeFeed()
{
    CClientRecordset rs;

    try
    {
        CStoredProc<> sp(m_ConnectionSource, L"sp_TradeSnapShot_Clear");
        rs.Open (sp);
        rs.Close();
        CTracer::Trace(CTracer::enMtInformation, _T("End-of-day trades were successfully deleted."));	
    }
    catch(_com_error& err)
    {
        CTracer::Trace(CTracer::enMtError, _T("Error occurred while end-of-day trades were being deleted.%s"), (char*)err.Description() );	
    }

    try
    {
        CStoredProc<> sp(m_ConnectionSource, L"sp_Trade_Clear");
        rs.Open (sp);
        rs.Close();
        CTracer::Trace(CTracer::enMtInformation, _T("Intraday trades were successfully deleted."));	
    }
    catch(_com_error& err)
    {
        CTracer::Trace(CTracer::enMtError, _T("Error occurred while intraday trades were being deleted.%s"), (char*)err.Description() );	
    }
    
    return ERROR_SUCCESS;
}

