#include "stdafx.h"

#include "trace.h"
#include "RttfClient.h"
#include "crs.h"

void CRttfClient::Process(const FocCsLoginResponseMsg* pMsg)
{
	switch (pMsg->reason)
	{
		case FOC_ERR_NONE:
			Trace(_T("User %s successfully logged on to FOC server."), m_strUserId.c_str());

			// Make sure user has an RTTF subscription
			if (pMsg->subscription & (1 << FOC_SRVC_RTTF))
			{
				if (pMsg->servers_up & (1 << FOC_SRVC_RTTF))
				{
					m_bRttfAvailable = TRUE;
					Trace(_T("RTTF server is available."));

				}
				else
				{
					m_bRttfAvailable = FALSE;
					Trace(_T("RTTF server is currently unavailable."));
				}
			} 
			else
			{
				Trace(_T("User %s is not subscribed to RTTF"), m_strUserId.c_str());
			}	
			break;
		
		case FOC_ERR_INVALID_USERID:
			Trace(_T("ERROR : User %s invalid."), m_strUserId.c_str());
			theApp.Shutdown();
			break;

		case FOC_ERR_INVALID_PASSWD:
			Trace(_T("ERROR : Invalid password for user %s."), m_strUserId.c_str());
			theApp.Shutdown();
			break;

		case FOC_ERR_INVALID_SECURE_ID:
			Trace(_T("ERROR : Invalid security ID for user %s."), m_strUserId.c_str());
			break;

		case FOC_ERR_NO_SUBSCRIPTION:
			theApp.Shutdown();
			Trace(_T("ERROR : User %s is not subscribed to RTTF."), m_strUserId.c_str());
			break;

		case FOC_ERR_RELOGON_ATTEMPTED:
			Trace(_T("ERROR : User %s is already connected to CS."), m_strUserId.c_str());
			theApp.Shutdown();
			break;

		default:
			Trace(_T("ERROR : Failed to connect to CS (error %ld)."), pMsg->reason);
			theApp.Shutdown();
	}
}

void CRttfClient::Process(const FocCsInfoMsg* pMsg)
{
	switch (pMsg->what)
	{
		case FOC_ERR_SERVER_NOT_RESPONDING:
			if (pMsg->value & (1 << FOC_SRVC_RTTF))
			{
				Trace(_T("RTTF server is no longer responding. Waiting it to up..."));

				// Update state variables
				m_bRttfAvailable = FALSE;
				m_bTradesAreInOrder = FALSE;
			}
			break;

		case FOC_SERVER_UP:
			if (pMsg->value & (1 << FOC_SRVC_RTTF))
			{
				Trace(_T("RTTF server is back up"));

				// Update available state
				m_bRttfAvailable = TRUE;

				// Request the trades that were missed
				if (m_LastTrade.IsFirstStartOfToday())
					Replay(RTTF_OPENING_POS_ONLY); 
					///!!!///Replay(RTTF_OPENING_POS_AND_TRADES);
				else
					Replay(RTTF_OPENING_POS_ONLY);
					///!!!///Replay(RTTF_TRADES_ONLY);
			}
			break;

		default:
			Trace(_T("Received CS Info Message: what %ld, value %ld, value_str %d"), 
				pMsg->what, pMsg->value, pMsg->value_str);
			break;
	}
}

void CRttfClient::Process(const FocRttfLoginResponseMsg* pMsg)
{
	if (pMsg->status == RTTF_NO_ERROR)
	{
		Trace(_T("Logged in to RTTF server"));

		// Set the available state
		m_bRttfAvailable = TRUE;
		
		// Request trades
		if (m_LastTrade.IsFirstStartOfToday())
			Replay(RTTF_OPENING_POS_ONLY); 
			///!!!///Replay(RTTF_OPENING_POS_AND_TRADES);
		else
			Replay(RTTF_OPENING_POS_ONLY);
			///!!!///Replay(RTTF_TRADES_ONLY);
	}
	else
	{
		Trace(_T("Failed to login to RTTF (error %ld)"), pMsg->status);
	}
}

void CRttfClient::Process(FocRttfTradeMsgEx* pMsg)
{
	time_t ThisTradeTime = pMsg->last_upd_date;

	Trace(_T("Trade with ID = %s is put into publisher's queue."), pMsg->trade_id);

#ifdef USE_SMART_LASTTRADETIME_ALGORITHM
	if(ThisTradeTime > m_SmartLastTime)
	{
		m_SmartLastTime = ThisTradeTime;
	}
#endif

	DWORD dwErr = theApp.m_Portfolio.Classify(*pMsg, pMsg->m_Portfolio);
	Trace(_T("Classified to : %s"), pMsg->m_Portfolio.c_str());

	cache_id_t Id;
	dwErr = theApp.m_Publisher.InsertTrade(*pMsg, Id);

	if(dwErr)
		Trace(_T("Cant insert trade into cache : 0x%x"), dwErr);
}

void CRttfClient::Process(FocRttfPositionMsgEx* pMsg)
{
	Trace(_T("Position %s received"), ((FocRttfTradeMsgEx*)pMsg)->trade_id);
	
	DWORD dwResult;
	dwResult = m_PostionFile.WriteFile(pMsg);
	if (ERROR_SUCCESS != dwResult)
	{
		TraceError(dwResult, "Failed to write position record into position file");
		return;
	}
}
