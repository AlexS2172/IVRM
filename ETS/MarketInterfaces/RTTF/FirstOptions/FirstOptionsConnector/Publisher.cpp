#include "stdafx.h"

#include "Publisher.h"
#include "trace.h"
#include "crs.h"
#include "traceitem.h"

void throw_com_error_ex(const HRESULT hr, const _bstr_t& bsDescription)
{
	CComPtr<ICreateErrorInfo> spCEI;
	if(SUCCEEDED(::CreateErrorInfo(&spCEI)))
	{
		if(bsDescription.length())
			spCEI->SetDescription(bsDescription);
		
		CComQIPtr<IErrorInfo> spEI = spCEI;
		ATLASSERT(spEI);
		
		::SetErrorInfo(0, spEI);
		throw _com_error(hr, spEI);
	}
	throw _com_error(hr);
}


HRESULT GetConnectionString(_bstr_t& bstrConnectionString)
{
	ISettingsPtr		    pEtsSettings;
	try
	{
		HRESULT hr = pEtsSettings.CreateInstance(__uuidof (Settings));
		if(FAILED(hr))
			throw_com_error_ex(hr, "Failed to create Settings object.");

		BSTR bsConnectionString;

		hr = pEtsSettings->get_DatabaseConnection(&bsConnectionString);
		if(FAILED(hr))
			throw_com_error_ex(hr, "Failed to get Connection String.");

		bstrConnectionString = bsConnectionString;

		SysFreeString(bsConnectionString);
	}
	catch(_com_error& e)
	{
		Trace(e.Description());
		return e.Error();
	}
	return S_OK;
}


DWORD CPublisher::Start()
{
	HRESULT hr = GetConnectionString(m_bstrConnectionString);
	if (FAILED(hr))
	{
		TraceError(hr, _T("Fail to read connection string from the registry"));
		return hr;
	}

	if(!m_fStructFile) // ! stands for stream error
		return TraceError(ERROR_OPEN_FAILED, _T("Failed to open structure file."));

	if(!m_fTradesFile.is_open())
		return TraceError(ERROR_OPEN_FAILED, _T("Failed to open trades file."));
	
	try
	{
		Trace( _T("Connecting to database...") );

		m_Connection.Open(m_bstrConnectionString);

		Trace( _T("Connection to database is established.") );

		CClientRecordset rs;		
		CStoredProc<> spGetLastID(m_Connection, L"usp_LastCacheID_Get");

		rs.Open (spGetLastID);

		// get trade id from db - it is count of trades for today
		m_CacheId = rs[L""];

		rs.Close();
	} 
	catch(const _com_error& e)
	{
		Trace( _T("Failed to retreive last cache id from DB : %s "), (TCHAR*)m_Connection.GetErrorMessage() );
		return e.Error();
	}

	m_hShutdownEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if(!m_hShutdownEvent)
		return TraceError(GetLastError(), _T("Failed to create event."));

	// Transport...
	try
	{
		Trace(_T("Initializing publisher..."));

		HRESULT hr = m_pPubManager.CreateInstance(__uuidof (MsgManager));
		if(FAILED(hr))
			throw_com_error_ex(hr, "Failed to create MsgManager object.");

		hr = m_spTradeUpdate.CreateInstance(__uuidof (TradeUpdate));
		if(FAILED(hr))
			throw_com_error_ex(hr, "Failed to create TradeUpdate object.");

		if FAILED(hr = m_pBroadcast.CreateInstance(__uuidof(BroadcastMessage)))
			throw_com_error_ex(hr, "Unable to create BroadcastMessage object.");

		m_pPubManager->RegPublisher(enMtTradeUpdate, NULL);
		m_pPubManager->RegPublisher(enMtBroadcastMessage, NULL);

		Trace( _T("Publisher initialized.") );
	}
	catch(const _com_error& e)
	{
		return TraceError( e.Error(), e.Description());
	}

	UINT ID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void *) this, 0, &ID);
	if(!m_hThread)
	{	
		::CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = NULL;
		return TraceError( GetLastError(), _T("Failed to create thread."));
	}

	return 0;
}

DWORD CPublisher::Stop()
{
	if (m_hShutdownEvent == NULL)
		return ERROR_SERVICE_NOT_ACTIVE;

	SetEvent(m_hShutdownEvent);

	WaitForSingleObject(m_hThread, INFINITE);

	Flush();
	Clear();
	m_Connection.Close();

	CloseHandle(m_hShutdownEvent);
	CloseHandle(m_hThread);

	try
	{
		if(m_pPubManager)
		{
			m_pPubManager->UnregPublisher(enMtTradeUpdate, NULL);
			m_pPubManager->UnregPublisher(enMtBroadcastMessage, NULL);
		}

		// give time for transport to prepare for shutdown..
		Sleep(5000);

		m_spTradeUpdate = NULL;
		m_pBroadcast = NULL;
		m_pPubManager = NULL;
	}
	catch(_com_error&)
	{
	}

	m_hShutdownEvent = NULL;
	return 0;
}


unsigned __stdcall CPublisher::ThreadFunc(void *pThisClass)
{
	CPublisher* pThis = reinterpret_cast<CPublisher*>(pThisClass);
	
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	while(TRUE)
	{
		if(pThis->IsStopping())
			return 0;

		pThis->Flush();
	}

	::CoUninitialize();
	
	return 0;
}

DWORD CPublisher::Flush()
{
	CAutoLock Lock(this);

	Trace( _T("Flushing data to database...") );
	DWORD dwErr;

	try
	{
		dwErr = FlushTrades();
		if(dwErr)
			throw _com_error(dwErr);
		else
			NormalizeCacheSize();
	} 
	catch(const _com_error & e)
	{
		HRESULT hr = S_OK;
		Trace( m_Connection.GetErrorMessage() );
		
		if (e.Error() == ERROR_REMOTE_STORAGE_NOT_ACTIVE)
		{
			if (hr = Reconnect())
				Trace(_T("Unable to reconnect due to error #'%d'"), hr);
		}

		return e.Error();
	}

	Trace( _T("Flushed.") );
	return 0;
}

DWORD CPublisher::Reconnect()
{
	try
	{
		Trace("Reconnecting...");
		m_Connection.Close();
		m_Connection.Open(m_bstrConnectionString);;
		Trace("Reconnected.");
	} 
	catch(_com_error & err)
	{
		Trace("Reconnection error : %s.", (TCHAR*)m_Connection.GetErrorMessage());
		return err.Error();
	}

	return 0;
}


BOOL CPublisher::IsStopping()
{
	return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hShutdownEvent, FLUSH_TIME));
}

DWORD CPublisher::GetLastTradeTime(FOC_DATETIME & Time)
{
	CAutoLock Lock(this);

	if(!m_pLastTradeTimeCache)
	{
		DWORD dwErr = LoadLastTradeTime();
		if(dwErr) 
			return dwErr;
	}

	Time = *m_pLastTradeTimeCache;
	return 0;
}

DWORD CPublisher::SetLastTradeTime(const FOC_DATETIME & Time)
{
	CAutoLock Lock(this);

	if(!m_pLastTradeTimeCache)
		m_pLastTradeTimeCache = new FOC_DATETIME;

	*m_pLastTradeTimeCache = Time;

	return 0;
}

DWORD CPublisher::InsertTrade(const foc_rttf_trade_ex & Trade, cache_id_t & Id)
{
	CAutoLock		Lock(this);
	trade_cache_t	TradeCache;

	Id = m_CacheId;

	TradeCache.m_Trade = Trade;
	TradeCache.m_AccessTime = time(NULL);
	TradeCache.m_bFlushed = false;
	m_TradesCache[m_CacheId++] = TradeCache;

	return 0;
}

DWORD CPublisher::GetTrade(const cache_id_t & Id, foc_rttf_trade_ex & Trade)
{
	CAutoLock		Lock(this);

	//first find in cache
	trades_cache_t::iterator it = m_TradesCache.find(Id);
	if(it != m_TradesCache.end())
	{
		it->second.m_AccessTime = time(NULL);
		Trade = it->second.m_Trade;
		return 0;
	}

	DWORD dwErr = LoadTradeById(Id, Trade);
	if(dwErr == 0)
	{
		trade_cache_t	TradeCache;
		TradeCache.m_Trade = Trade;
		TradeCache.m_AccessTime = time(NULL);
		TradeCache.m_bFlushed = true;
		m_TradesCache[Id] = TradeCache;
	}

	return dwErr;
}

void CPublisher::NormalizeCacheSize()
{
	trades_cache_t::iterator it;

	if(m_TradesCache.size() < CACHE_SIZE)
		return;

	typedef multimap<time_t, trades_cache_t::iterator>	cache_delete_candidates_t;
	cache_delete_candidates_t	Candidates;
	cache_delete_candidates_t::iterator it3;

	// getting all the trades that is not flushed yet
	for(it = m_TradesCache.begin(); it != m_TradesCache.end(); it++)
	{
			trade_cache_t & Trade = it->second;
			if(Trade.m_bFlushed)
			{
				Candidates.insert(cache_delete_candidates_t::value_type(Trade.m_AccessTime, it));
			}
	}

	// deleteting all the oldest trades before cache is 90% of max size
	cache_delete_candidates_t::reverse_iterator it2;
	for(it2 = Candidates.rbegin(); it2 != Candidates.rend(); it2++)
	{
		m_TradesCache.erase(it2->second);

		if(m_TradesCache.size() < CACHE_SIZE / 100 * 90)
		{
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
DWORD CPublisher::FlushTrades()
{
	trades_cache_t::iterator it = m_TradesCache.begin();
	while( it != m_TradesCache.end() )
	{
		trade_cache_t& Trade = it->second;
		const cache_id_t& CacheId = it->first;

		if(Trade.m_bFlushed == false)
		{
			// Allow reading thread to read trades from RTTF without queueing them on the letter
			::SwitchToThread();
			
			std::string		aStatus;
			DWORD			dwReturn = 0;
			
			DWORD	dwErr = FlushTrade(Trade.m_Trade);
			switch(dwErr)
			{
				case 0: 
				case -1: 
				case -2:
					aStatus = "OK";
					dwReturn = 0;
					break;

				case ERROR_ALREADY_EXISTS:
					aStatus = "ALREADYEXISTS";
					dwReturn = dwErr;
					break;

				case ERROR_FILE_NOT_FOUND:
					aStatus = "NOTFOUND";
					dwReturn = 0;
					break;

                case ERROR_BAD_QUERY_SYNTAX:
                    aStatus = "REPROCESSFAIL";
					dwReturn = dwErr;
                    break;

				default:
					aStatus = "ERROR";
					dwReturn = dwErr;
			}

			CTraceItem LogItem(Trade.m_Trade);
			LogItem.SetOpType(Trade.m_Trade.m_sType);
			LogItem.SetState(aStatus);

			m_fTradesFile << LogItem;

			if(dwReturn != 0)		// in case of any error we must not continue
				return dwReturn;

			Trade.m_bFlushed = true;
		}

		it++;
	}

	return 0;
}

#define __GET_CHAR(member)			Trade.##member = (BYTE)rs[L#member ];
#define __GET_STRING(member, len)	bstrBuf = rs[L#member]; strncpy(Trade.##member, bstrBuf, len);
#define __GET_DOUBLE(member)		Trade.##member = rs[L#member];
#define __GET_INT(member)			Trade.##member = rs[L#member];
#define __GET_TIME(member)			Trade.##member = (time_t)rs[L#member];
#define __GET_STL_STRING(field, member)	bstrBuf = rs[L#field]; Trade.##member = bstrBuf;


DWORD CPublisher::LoadTradeById(const cache_id_t & Id, foc_rttf_trade_ex & Trade)
{
	try
	{
		CStoredProc<> sp(m_Connection, L"usp_ClearingHouseTradeByCacheID_Get");

		sp	<< Id;
		CClientRecordset rs;
		rs.Open (sp);

		int nCnt = rs.GetRecordCount();

		if(nCnt != 1)
			throw _com_error(ERROR_NOT_FOUND);

		int		iBuf;
		_bstr_t bstrBuf;

		iBuf = rs[L"iDateOfTrade"];
		iBuf = rs[L"iCacheId"];
		__GET_CHAR(ins_upd_del_flag);
		__GET_CHAR(put_call);
		__GET_CHAR(buy_sell);
		__GET_CHAR(origin);
		__GET_CHAR(open_close);
		__GET_STRING(trade_id, TRADE_ID_LEN);
		__GET_STRING(user_id, USER_ID_LEN);
		__GET_STRING(trading_sym, SYMBOL_LEN);
		__GET_STRING(base_sym, SYMBOL_LEN);
		__GET_STRING(parent_account, ACCOUNTS_LEN);
		__GET_STRING(account, ACCOUNTS_LEN);
		__GET_STRING(q_account, ACCOUNTS_LEN);
		__GET_STRING(security_type, SECURITY_LEN);
		__GET_STRING(security_subtype, SECURITY_LEN);		
		__GET_STRING(clearing_firm, FIRM_LEN);
		__GET_STRING(cusip, FIRM_LEN);
		__GET_STRING(basket_id, BASKET_ID_LEN);
		__GET_STRING(giveup_firm, FIRM_LEN);
		__GET_STRING(contra_firm, FIRM_LEN);
		__GET_STRING(exec_broker, BROKER_LEN);
		__GET_STRING(contra_broker, BROKER_LEN);
		__GET_STRING(mmcs_sub_acct, MMCS_SUB_ACCT_LEN);
		__GET_DOUBLE(exercise_multiplier);
		__GET_DOUBLE(extended_premium);
		__GET_DOUBLE(premium);
		__GET_DOUBLE(strike);
		__GET_DOUBLE(price);
		__GET_INT(quantity);
		__GET_INT(tag);
		__GET_TIME(expiration_date);
		__GET_TIME(execution_date);
		__GET_TIME(last_upd_date);
		__GET_TIME(contract_date);
		__GET_INT(source_system);
		__GET_INT(trade_id_num);
		__GET_INT(trade_group_id);
		__GET_TIME(settlement_date);
		__GET_STRING(entry_firm, FIRM_LEN);
		__GET_CHAR(matched_flag);
		__GET_CHAR(spread_ind);
		__GET_DOUBLE(premium_multiplier);
		__GET_STRING(source_code, SOURCE_CODE_LEN);
		__GET_STRING(parent_acct_intrl_id, INTRL_ID_LEN);
		__GET_STRING(acct_intrl_id, INTRL_ID_LEN);
		__GET_STRING(q_acct_intrl_id, INTRL_ID_LEN);
		__GET_DOUBLE(cap_interval);
		__GET_INT(trade_exchange);
		__GET_CHAR(settled_unsettled_ind);
		__GET_CHAR(stock_origin);
		__GET_STRING(mm_card_number, MM_CARD_NUM_LEN);
		__GET_INT(basket_tag);
		__GET_DOUBLE(basket_price);
		__GET_CHAR(short_sale);
		__GET_STRING(commission_code, COMMISSION_CODE_LEN);
		__GET_DOUBLE(commission);
		__GET_CHAR(same_day_exer);
		__GET_STRING(terminal_name, TERMINAL_NAME_LEN);
		__GET_STL_STRING(portfolio_name, m_Portfolio);

	} 
	catch(_com_error & err)
	{
		Trace(m_Connection.GetErrorMessage());
		return err.Error();
	}

	return 0;
}

DWORD CPublisher::FlushLastTradeTime()
{
	if(!m_pLastTradeTimeCache)
		return 0;

	// optimization. if last time is not changed, then do not flush it
	// this in almost cases prevent sql server from begining tran
	static time_t tmLast = 0;

	if(tmLast == *m_pLastTradeTimeCache)
	{
		return 0;
	}

	try
	{
		CStoredProc<> sp(m_Connection, L"usp_LastTradeTime_Save");

		sp << *m_pLastTradeTimeCache;

		CClientRecordset rs;

		rs.Open (sp);

		tmLast = *m_pLastTradeTimeCache;
	}
	catch(_com_error & err)
	{
		Trace("Failed to flush last trade time : %s", (TCHAR*)m_Connection.GetErrorMessage());
		return err.Error();
	}

	return 0;
}

DWORD CPublisher::LoadLastTradeTime()
{
	try
	{
		CStoredProc<> sp(m_Connection, L"usp_LastTradeTime_Get");

		CClientRecordset rs;
		rs.Open (sp);

		int nCnt = rs.GetRecordCount();

		if(nCnt != 1)
			throw _com_error(ERROR_NOT_FOUND);

		FOC_DATETIME	dt = (time_t)rs[L"iLastTradeTime"];

		if(!m_pLastTradeTimeCache)
			m_pLastTradeTimeCache = new FOC_DATETIME;

		*m_pLastTradeTimeCache = dt;
	}
	catch(_com_error & err)
	{
		Trace(m_Connection.GetErrorMessage());
		return err.Error();
	}

	return 0;
}

HRESULT CPublisher::Publish(const foc_rttf_trade &Trade, bool bIsTradeUpdate)
{
	if (bIsTradeUpdate)
	{
		try
		{
			m_pPubManager->PubTradeUpdate(m_spTradeUpdate);

			Trace(_T("Trade '%s' published."), Trade.trade_id);
			m_dwPublished++;
		}
		catch(_com_error& e)
		{
			Trace(_T("WARNING. Failed to publish trade '%s' : 0x%x '%s'."), Trade.trade_id, e.Error(), e.Description());
		}
	}
	else
	{
		try
		{
			m_pPubManager->PubBroadcastMessage(m_pBroadcast);
			Trace(_T("Broadcast message with error for trade '%s' was sent"), Trade.trade_id);
		}
		catch(const _com_error& e)
		{
			Trace(_T("WARNING. Failed to broadcast the '%s' trade: 0x%x '%s'."), Trade.trade_id, e.Error(), e.Description());
		}
	}

	return 0;
}
