#include "stdafx.h"

#include "Publisher.h"
#include "trace.h"
#include "crs.h"
#include "traceitem.h"

// time utils
int MonthDays[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

DATE ChicagoTimeToGMT(SYSTEMTIME& ChicagoTime)
{
	TIME_ZONE_INFORMATION TZI;

	TZI.Bias = 360;
	TZI.StandardBias = 0;
	TZI.DaylightBias = -60;
	TZI.StandardName[0] = 0;
	TZI.DaylightName[0] = 0;

	memset(&TZI.DaylightDate, 0, sizeof(SYSTEMTIME));
	memset(&TZI.StandardDate, 0, sizeof(SYSTEMTIME));

	TZI.DaylightDate.wYear = 0;
	TZI.DaylightDate.wDayOfWeek = 1;
	TZI.DaylightDate.wMonth = 4;
	TZI.DaylightDate.wDay = 1;

	TZI.StandardDate.wYear = 0;
	TZI.StandardDate.wDayOfWeek = 1;
	TZI.StandardDate.wMonth = 10;
	TZI.StandardDate.wDay = 5;

	SYSTEMTIME dtGMT;

	BOOL bRetn = SystemTimeToTzSpecificLocalTime(&TZI, &ChicagoTime, &dtGMT);
	
	if (FALSE == bRetn)
		return 0;

	DATE dtToReturn = 0;

	if (0 == SystemTimeToVariantTime(&dtGMT, &dtToReturn))
		return 0;

	return dtToReturn;
}



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

// Return values:
//		ERROR_SEEK_ON_DEVICE - unable to delete/insert/update something. 
//			This error indicates logical error in the code if it was occured during DEL_ processing
//			This error indicates sql error if it was occured during NEW_ & UPD_ processing
//
//		ERROR_BAD_QUERY_SYNTAX - error occured while executing sql sp
//
//		ERROR_FILE_NOT_FOUND - nothing to delete. Procedure was executed against error parameters.
DWORD CPublisher::FlushTrade(foc_rttf_trade_ex& Trade)
{
	USES_CONVERSION;

	bool	bOut = false;
	DWORD	dwRes = 0;
    TDBFailsMap::iterator fIt;
	
	do
	{
		try
		{
			switch(Trade.ins_upd_del_flag)
			{
				case NEW_FLAG:
				case UPD_FLAG:
				{
					Trade.m_sType = "NEW/UPDATE";
					m_dwNewTotal ++;

					CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_New");

					sp << _bstr_t(Trade.account) << _bstr_t(Trade.trade_id);
					sp << (int)( Trade.buy_sell == BUY_FLAG ? 1 : 0);
					sp << Trade.quantity;
					sp << Trade.price;//premium?
					
					DATE dtExecutionDate = ChicagoTimeToGMT( (SYSTEMTIME)Trade.execution_date );					
					if (!dtExecutionDate)
					{
						Trace(_T("Failed to convert execution_date from CST to GMT for trade '%s'"), Trade.trade_id);
						bOut = true;
						utils::ThrowErrorNoSetErrorInfo(E_INVALIDARG, L"Failed to convert expiry date from CST to GMT");
					}

					sp << (vt_date)dtExecutionDate;
					sp << _bstr_t(Trade.MakeOptionSymbol().c_str());

					// if option
					time_t LastTradeTime = *m_pLastTradeTimeCache;
					
					if(strlen(Trade.security_type) >= 2 && Trade.security_type[0] == '0')
					{ 
						sp << (int)3;//type
						sp << _bstr_t(Trade.base_sym);
						
						if ( _bstr_t(Trade.base_sym).length() == 0 )
							LogError(m_fFailsFile, Trade, "Base symbol is undefined");

						sp << (bool)(Trade.put_call == CALL_FLAG);
						sp << Trade.strike;

						SYSTEMTIME stExpiryDate = 
						{	
							Trade.expiration_date.year,
							Trade.expiration_date.month + 1,
							0, 
							Trade.expiration_date.day, 
							0, 
							0, 
							0, 
							0 
						};

						DATE dtExpiryDate = ChicagoTimeToGMT(stExpiryDate);						
						if (!dtExpiryDate)
						{
							Trace(_T("Failed to convert expiration_date from CST to GMT for trade '%s'"), Trade.trade_id);
							bOut = true;							
							utils::ThrowErrorNoSetErrorInfo(E_INVALIDARG, L"Failed to convert expiry date from CST to GMT");
						}						
						
						sp << vt_date(dtExpiryDate);
						sp << dbNull;
						sp << (int)1; //update enabled
						sp << LastTradeTime;
						sp << _bstr_t(Trade.trading_sym); // option root
					}
					else
					{
						sp << (int)2; //type
						sp << dbNull;
						sp << dbNull;
						sp << dbNull;
						sp << dbNull;
						sp << dbNull;
						sp << (int)1; //update enabled
						sp << LastTradeTime;//update enabled
					}

					rs.Open (sp);

                    if(rs.IsEOF())
                    {
						Trace(_T("Failed to store 'NEW' or 'UPDATE' trade '%s' in database. Unexpected error."), Trade.trade_id);
                        bOut = true;
						utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"Failed to store 'NEW' or 'UPDATE' trade in database");
                    }                        

                    long lMax = rs[L"iMaxSeqNum"];
					long lMin = rs[L"iMinSeqNum"];
					long lMissed = rs[L"iStructureMissed"];

					// non-fatal errors processing
					if(lMissed == -1 && lMax == -1)
						if (lMin != -1)
						{
							Trace(_T("Attempt to insert trade with the '%s' ExecID twice. This error is a result of serious DB error"), Trade.trade_id);
							bOut = true;
							utils::ThrowErrorNoSetErrorInfo(dwRes = ERROR_SEEK_ON_DEVICE, 
									L"Attempt to insert trade with the same ExecID twice");
						}
						else
						{
							Trace(_T("Error in a stored procedure"));
							bOut = true;
							utils::ThrowErrorNoSetErrorInfo(dwRes = ERROR_BAD_QUERY_SYNTAX, L"Error in a stored procedure");
						}

                    rs.Close();
                   
                    Trace(_T("Trade '%s' stored in database."), Trade.trade_id);

					if(lMin == 0)
					{
						dwRes = 0; // NEW TRADE
						Trace(_T("Trade '%s' is NEW."), Trade.trade_id);
						lMin = lMax;
					}
					else
					{
						dwRes = -1; // UPDATE
						Trace(_T("Trade '%s' is UPDATE."), Trade.trade_id);
					}

                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
					sp_get << lMin << lMax << CDBNull() << 1 << CDBNull();
					
                    rs.Open (sp_get);
					
                    if(rs.IsEOF())
					{
						Trace(_T("Failed to retrieve trade from database - trade with ID '%s' does not exist."), Trade.trade_id);
						bOut = true;
						utils::ThrowErrorNoSetErrorInfo(E_FAIL, 
							L"Failed to retrieve trade from database - trade does not exist");
					}
					else
					{
						HRESULT hRes;
						long lRowsReturned = 0;

						while (!rs.IsEOF())
						{
							if ( FAILED(hRes = FillTradeUpdate(rs, Trade)) )
							{
								Trace(_T("Failed to fill TradeUpdate message."));
								
							}
							else if (S_FALSE == hRes)
							{
								Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."), Trade.trade_id);
								bOut = true;
								utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"Failed to retrieve trade from database - trade does not exist");
							}
							else
							{
								Trace(_T("Trade update filled, SeqNum: %d"), m_spTradeUpdate->SeqNum);
								Publish(Trade);
							}
							
							++rs;
							lRowsReturned++;
						}

						// Try to detect error output from sp
						// Normally, store proc MUST NOT return more that 2 rows 
						ATLASSERT(lRowsReturned <= 2);
					}
                    
					break;
				}

				case DEL_FLAG:
				{
					m_dwDeleteTotal += 1;
					Trade.m_sType = "DELETE";

					CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_Del");
					sp << _bstr_t(Trade.trade_id);
					
					DATE dtTradeDate = ChicagoTimeToGMT((SYSTEMTIME)Trade.execution_date);					
					if (!dtTradeDate)
					{
						Trace(_T("Failed to convert execution_date from CST to GMT for trade '%s'"), Trade.trade_id);
						bOut = true;
						utils::ThrowErrorNoSetErrorInfo(E_INVALIDARG, L"Failed to convert expiry date from CST to GMT");
					}

					sp << vt_date(dtTradeDate) << (BYTE)0;

					time_t LastTradeTime = *m_pLastTradeTimeCache;
					sp << LastTradeTime;

					rs.Open (sp);                    
                    if(rs.IsEOF())
                    {
                        rs.Close();
						bOut = true;
                        _com_issue_error(E_FAIL);
                    }
                    
                    long lMax = rs[L"iMaxSeqNum"];
					long lMin = rs[L"iMinSeqNum"];

                    rs.Close();
                    
					if (lMax == -1)
						if (lMin == 0)
						{
							Trace(_T("Failed to store trade in database - trade with the ExecID '%s' does not exist."), 
									Trade.trade_id);

							bOut = true;
							_com_issue_error(dwRes = ERROR_FILE_NOT_FOUND);
						}
						else
						{
							Trace(_T("Error in a stored procedure"));
							bOut = true;
							utils::ThrowErrorNoSetErrorInfo(dwRes = ERROR_BAD_QUERY_SYNTAX, L"Error in a stored procedure");
						}

//					assert(lMax != -1);

                    dwRes = -2; // TRADE HAS BEEN DELETED

					Trace(_T("Trade '%s' deleted from database."), Trade.trade_id);
                    
                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
					sp_get << lMax << lMax << CDBNull() << 1 << CDBNull();

                    rs.Open (sp_get);
                    if(rs.IsEOF())
					{
						Trace(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
							Trade.trade_id);
						bOut = true;
						utils::ThrowErrorNoSetErrorInfo(dwRes = ERROR_NO_DATA_DETECTED, 
							L"Failed to retrieve trade from database - trade does not exist");
					}
					else
					{
                        HRESULT hRes;
					    if(FAILED(hRes = FillTradeUpdate(rs, Trade)))
                        {
   					        Trace(_T("Failed to fill TradeUpdate message."));
							bOut = true;
							utils::ThrowErrorNoSetErrorInfo(dwRes = ERROR_INVALID_PARAMETER, L"Failed to fill TradeUpdate message");
                        }
					    else 
							if(hRes == S_FALSE)
							{
   								Trace(_T("Failed to retrieve trade from database - trade with the same ExecID '%s' does not exist."),
					 				Trade.trade_id);
								bOut = true;
								utils::ThrowErrorNoSetErrorInfo(dwRes = ERROR_NO_DATA_DETECTED, 
									L"Failed to retrieve trade from database - trade does not exist");
							}                        
							else
							{
								// OK
								Publish(Trade);
							}
                    }

					break;
				}

				default:
					Trade.m_sType = "UNKNOWN";
					dwRes = ERROR_INVALID_PARAMETER;
					break;
			}

			bOut = true;		
		}
		catch(_com_error& e)
		{
			if(!bOut)
			{
				_bstr_t bsError = "Unable to store trade in database :" + m_Connection.GetErrorMessage();
				Trace(_T("CRITICAL ERROR : UNABLE TO STORE TRADE IN DATABASE."));

				dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
				bOut = true;
			}

			_bstr_t bsDescr = e.Description();

			LogError(m_fFailsFile, Trade, bsDescr.length()!=0 ? string("Unable to store trade in database.\nError: ") + string((char*)bsDescr) : "Unable to store trade in database" );

			if ( e.Error() != ERROR_FILE_NOT_FOUND )
			{
				// neither operation didn't complete successfully nor trade was published,
				// so needed to inform these who're upstairs
				if (bsDescr.length())
				{
					bsDescr = L"Unable to store trade in database.\nError: ";
					bsDescr += e.Description();
				}
				else
				{
					bsDescr = L"Unable to store trade in database";
				}

				HRESULT hr = FillBroadcast( e.Error(), bsDescr );
				if ( SUCCEEDED(hr))
				{
					Publish(Trade, false);
				}
				else
				{
					Trace(_T("Failed to fill Broadcase message"));
					LogError(m_fFailsFile, Trade);
				}
			}
		}
	} while(!bOut);

	return dwRes;
};

HRESULT CPublisher::FillTradeUpdate(CClientRecordset& rs, const foc_rttf_trade_ex& Trade)
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
				Trace(_T("Error. Unknown SymbolType for ContractTypeID = %d."), iContractTypeID);
				_com_issue_error(E_FAIL);
				break;
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
        m_spTradeUpdate->FutID =  rs[L"iFutureID"];
        m_spTradeUpdate->FutRootID =  rs[L"iFutureRootID"];

		if(Trade.ins_upd_del_flag == NEW_FLAG || Trade.ins_upd_del_flag == UPD_FLAG)
		{
			long lStructureMissed = rs[L"iStructureMissed"];
			m_spTradeUpdate->MsState = (TRADE_MS_STATE)lStructureMissed;

			if(lStructureMissed)
			{
                TCHAR szBuf[1024] = {0};
				
				switch(lStructureMissed)
				{
					case enTmssNewStock:
						_stprintf(szBuf, _T("New stock '%s' added.\n"), A2CT(Trade.base_sym) );
						break;

					case enTmssNewOption:
						_stprintf(szBuf, _T("New option '%s' for stock '%s' added.\n"),
							A2CT(Trade.trading_sym), A2CT(Trade.base_sym) );
						break;

					case enTmssNewStock | enTmssNewOption:
						_stprintf(szBuf, _T("New stock '%s' and new option '%s' for this stock are added.\n"),
							A2CT(Trade.base_sym), A2CT(Trade.trading_sym) );
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

HRESULT CPublisher::FillBroadcast(HRESULT hrErrorCode, _bstr_t& bsDescription)
{
	try
	{
		m_pBroadcast->Code = hrErrorCode;
		m_pBroadcast->Facility = enFacRTTF;
		m_pBroadcast->Severity = enSevError;
		m_pBroadcast->Priority = enPrHigh;

		if (bsDescription.length() == 0)
		{
			TCHAR szBuffer[1024] = {0};
			
			if ( ::LoadString( GetModuleHandle(0), 
							   IDS_BROADCASTMESSAGE, 
							   szBuffer, 
							   sizeof(szBuffer)/sizeof(TCHAR) ) )
			{
				m_pBroadcast->Description = szBuffer;
			}
			else
			{
				m_pBroadcast->Description = L"Unavailable";
			}
		}
		else
		{
			m_pBroadcast->Description = bsDescription;
		}
	}
	catch(const _com_error& e)
	{
		return e.Error();
	}

	return S_OK;
}


DWORD CPublisher::ReProcessTrade(const foc_rttf_trade_ex& Trade)
{
    DWORD dwRes = ERROR_SUCCESS;

    TDBFailsStore::iterator it = m_DBFails.find(string(Trade.trade_id));
    if ( it == m_DBFails.end() )
        return ERROR_NO_DATA;

    m_DBFails.erase(it);
    
    CClientRecordset rs;
	CStoredProc<> sp_get(m_Connection, L"usp_TradeFeed_Get");
    try
    {
        sp_get << _bstr_t(Trade.trade_id);
        DATE dtTradeDate;
	    OleDateFromTm(Trade.execution_date.year + 1900, 
					  Trade.execution_date.month + 1,
					  Trade.execution_date.day, 
		     		  0, 0, 0,
			    	  dtTradeDate);
        sp_get << (vt_date)dtTradeDate;
        
        if ( Trade.ins_upd_del_flag == NEW_FLAG || Trade.ins_upd_del_flag == UPD_FLAG)
            sp_get << (int)1;
        if ( Trade.ins_upd_del_flag == DEL_FLAG )
            sp_get << (int)0;
        
		rs.Open (sp_get);

        if(rs.IsEOF())
        {
            rs.Close();
            _com_issue_error( E_FAIL );
        }             
    }
    catch (_com_error&)
    {        
        Trace(_T("Failed to reprocess trade with ExecID = '%s' : %s"), Trade.trade_id, (char*)m_Connection.GetErrorMessage() );	
        dwRes = ERROR_DATABASE_FAILURE;
    }
        
    HRESULT hRes = FillTradeUpdate(rs, Trade);
		
    if( FAILED(hRes) )
    {	 
        Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to fill TradeUpdate message"), Trade.trade_id );
        dwRes = ERROR_INVALID_PARAMETER;
    }
	else if(hRes == S_FALSE)
	{
	    Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to retrieve trade from database."), Trade.trade_id);
        dwRes = ERROR_NO_DATA_DETECTED;
	}                   
    
    return dwRes;
}
