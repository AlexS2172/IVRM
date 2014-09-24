#include "stdafx.h"

#include "trace.h"
#include "RttfClient.h"
#include "crs.h"


unsigned __stdcall CRttfClient::ReadThread(void* arg)
{
	CRttfClient* pThis = reinterpret_cast<CRttfClient*>(arg);
	FocCsHeader header;
	FocCsMsg* pMsg = 0;

	while (TRUE)
	{
		if(pThis->IsStopping())
		{
			Trace( _T("Read thread stopped.") );
			return 0;
		}

		if(!pThis->m_bFocAvailable)
		{
			continue;
		}

		// Initialize read set
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(pThis->m_Socket, &readfds);

		fd_set errorfds;
		FD_ZERO(&errorfds);
		FD_SET(pThis->m_Socket, &errorfds);


		TIMEVAL tm;
		tm.tv_sec = tm.tv_usec = 0;
		DWORD  dwRes = ::select(0, &readfds, NULL, &errorfds, &tm);

		if(dwRes == SOCKET_ERROR || errorfds.fd_count)
		{
			pThis->m_bFocAvailable = false;
			pThis->m_bTradesAreInOrder = false;
		}

		if (dwRes <= 0)
		{
			continue;
		}

		// Process records while there is data available
		do	
		{
			// Skip any fragmented records
			if (!xdrrec_skiprecord(&pThis->m_xdrRead))
			{
				Trace(_T("xdrrec_skiprecord failed!"));
				break;
			}

			// Get the header of the message 
			if (!header.xdr_convert(&pThis->m_xdrRead, XDR_DECODE))
			{
				continue;
			}

			// Initialize message pointer to NULL
			pMsg = NULL;

			// Decode the rest of the message using the header
			switch (header.msg_class)
			{
				// FOC messages
				case FOC_CLASS_SYSTEM:
					switch (header.service_id)
					{
						case FOC_SRVC_LOGIN_RESPONSE:
							pMsg = new FocCsLoginResponseMsg(header);
							break;

						case FOC_SRVC_INFO:
							pMsg = new FocCsInfoMsg(header);
							break;

						default:
							Trace(_T("Unsupported system message: service_id <%ld> msg_type <%ld>"), 
								header.service_id, header.msg_type);
							break;
					}
					break;

				// RTTF messages
				case FOC_CLASS_APPLICATION:
				/* -- THIS COMMENT IS OBSOLETE
				When the client application logs back into the Comm Server, the Comm
				Server may send some messages from the interrupted session before
				sending the Login Response message. It is recommended that the client
				application discard any messages received before it receives a Login
				Response message for the new session. Once a Login Response message is
				received, the client application can send a request to the specialized server to
				determine where it left off and what messages it requires.
				*/
				if (true) // (pThis->m_bRttfAvailable)//
				{
					switch (header.service_id)
					{
						case FOC_SRVC_RTTF:
							switch (header.msg_type)
							{
								case FOC_RTTF_TRADE_MSG:
									pMsg = new FocRttfTradeMsgEx(header);
									break;	

								case FOC_RTTF_TRADEREQ_MSG:
									pMsg = new FocRttfTradeRequestMsg(header);
									break;

								case FOC_RTTF_LOGINRESP_MSG:
									pMsg = new FocRttfLoginResponseMsg(header);
									Trace(_T("RTTF login response received"));
									break;

								default:
									Trace(_T("Unsupported application message: msg_type <%ld>"), 
										header.msg_type);
									break;
							}
							break;

						default:
							Trace(_T("Ignoring application message from service_id <%ld>"), 
								header.service_id);
							break;
					}
				}
			}

			if (pMsg)
			{
#if PROFILE_ON
				static int trades_n  = 0;
				static time_t	start_time = time(NULL);
				time_t now_time = time(NULL);
				if(now_time == start_time)
					now_time++;
#endif

				// Decode the body of the message
				if (pMsg->xdr_convert(&pThis->m_xdrRead, XDR_DECODE))
				{
					CAutoLock Lock(&pThis->m_InQueLock);
#if PROFILE_ON
					printf("sizeof InQueue : %d speed = %f t/sec\n", 
						pThis->m_InQue.size(), (double)++trades_n / ((double)now_time - (double)start_time));
#endif
					pThis->m_InQue.push(pMsg);
				}
				else
					Trace(_T("Failed to decode message body"));

			}
		} while (!xdrrec_eof(&pThis->m_xdrRead));
	}

	return 0;
}

unsigned __stdcall CRttfClient::WriteThread(void* arg)
{
	CRttfClient* pThis = (CRttfClient *) arg;
	FocCsMsg* pMsg = NULL;

	while (true)
	{
		if(pThis->IsStopping())
		{
			Trace( _T("Write thread stopped.") );
			return 0;
		}

		if(!pThis->m_bFocAvailable)
			continue;

		// Initialize read set
		fd_set writefds;
		FD_ZERO(&writefds);
		FD_SET(pThis->m_Socket, &writefds);

		fd_set errorfds;
		FD_ZERO(&errorfds);
		FD_SET(pThis->m_Socket, &errorfds);


		TIMEVAL tm;
		tm.tv_sec = tm.tv_usec = 0;
		DWORD  dwRes = ::select(0, NULL, &writefds, &errorfds, &tm);

		if(dwRes == SOCKET_ERROR || errorfds.fd_count)
		{
			pThis->m_bFocAvailable = false;
			pThis->m_bTradesAreInOrder = false;
		}


		if (dwRes <= 0)
			continue;

		// Get a message from the outbound queue
		pThis->m_OutQueLock.Lock();
		if(pThis->m_OutQue.empty())
		{
			pThis->m_OutQueLock.Unlock();
			continue;
		}
		pMsg = pThis->m_OutQue.front();
		pThis->m_OutQue.pop();
		pThis->m_OutQueLock.Unlock();

		// Encode the message
		if (!pMsg->xdr_convert(&pThis->m_xdrWrite, XDR_ENCODE))
			Trace( _T("Failed to encode queued message!") );


		if (!xdrrec_endofrecord(&pThis->m_xdrWrite, TRUE))
			fprintf(stderr, "Failed to send queued message!"); 

		// Remove the message
		delete pMsg;
	}
	
	return 0;
}

unsigned __stdcall CRttfClient::InputThread(void* arg)
{
	CRttfClient* pThis = reinterpret_cast<CRttfClient*>(arg);
	FocCsMsg* pMsg = 0;

	while (TRUE)
	{
		if(pThis->IsStopping())
		{
			Trace("Input thread stopped.");
			return 0;
		}
/*
		if(!pThis->m_bFocAvailable)
			continue;
*/
		// Get a message from the outbound queue
		{
			CAutoLock Lock(&pThis->m_InQueLock);

			if(pThis->m_InQue.empty())
				continue;

			pMsg = pThis->m_InQue.front();
			pThis->m_InQue.pop();
		}

		FocRttfTradeMsgEx* pTradeMsgEx = (FocRttfTradeMsgEx*)pMsg;
		
		switch (pMsg->header.msg_class)
		{
			case FOC_CLASS_SYSTEM:
				switch (pMsg->header.service_id)
				{
					case FOC_SRVC_LOGIN_RESPONSE:
						pThis->Process((FocCsLoginResponseMsg *) pMsg);
						break;

					case FOC_SRVC_INFO:
						pThis->Process((FocCsInfoMsg *) pMsg);
						break;	
				}
				break;
		
			case FOC_CLASS_APPLICATION:
				if (pMsg->header.service_id == FOC_SRVC_RTTF)
				{
					switch (pMsg->header.msg_type)
					{
						case FOC_RTTF_TRADE_MSG:
							if (!strchr( pTradeMsgEx->trade_id, '-'))
							{	
   								if (pThis->m_bTradesAreInOrder)
								{
									// Prevent from internal cache flushing 
									CAutoLock Lock(&theApp.m_Publisher);

									time_t temp = pTradeMsgEx->last_upd_date;
									Trace(_T("Set last update date to %s"), ctime(&temp));

									pThis->m_LastTrade.SetDate(pTradeMsgEx->last_upd_date);
									pThis->Process(pTradeMsgEx);
								}
								else
								{
									//serialize trades by timestamp
									CAutoLock Lock(&pThis->m_TradesMapLock);

									pThis->m_mmapTrades.insert( make_pair( pTradeMsgEx->last_upd_date, *pTradeMsgEx) );
										/*multimap<FOC_DATETIME, FocRttfTradeMsgEx>::value_type(
										((FocRttfTradeMsgEx *) pMsg)->last_upd_date, *(FocRttfTradeMsgEx *) pMsg));*/
								}
							}
							else
							{
								pThis->Process((FocRttfPositionMsgEx *) pMsg);
							}
							break;	

						case FOC_RTTF_TRADEREQ_MSG:
							{
								Trace(_T("TradeReq received. Start processing old trades..."));

								pThis->m_TradesMapLock.Lock();
								for(multimap<FOC_DATETIME, FocRttfTradeMsgEx>::iterator it = 
									pThis->m_mmapTrades.begin(); it != pThis->m_mmapTrades.end(); it++)
								{
									pThis->Process(&(it->second));
								}
								pThis->m_mmapTrades.clear();
								pThis->m_TradesMapLock.Unlock();

								Trace(_T("All old trades received."));
							}
					
#ifdef USE_SMART_LASTTRADETIME_ALGORITHM
							if(pThis->m_SmartLastTime)
							{
								Trace(_T("Set last trade time using smart time : %s"), ctime(&pThis->m_SmartLastTime));
								pThis->m_LastTrade.SetDate(pThis->m_SmartLastTime);
							}
#endif
							pThis->m_bTradesAreInOrder = TRUE;
							break;

						case FOC_RTTF_LOGINRESP_MSG:
							pThis->Process((FocRttfLoginResponseMsg *) pMsg);
							break;
					}
				}
				break;
		}
			
		delete pMsg;
	}
	
	return 0;
}

unsigned __stdcall CRttfClient::OutputThread(void* arg)
{
	CRttfClient* pThis = (CRttfClient *) arg;
	FocCsMsg* pMsg = 0;

	while(true)
	{
		if(pThis->IsStopping())
		{
			Trace("Output thread stopped.");
			return 0;
		}

		if(!pThis->m_bFocAvailable)
		{
			// nothing to send and receive for a while
			pThis->Clear();

			DWORD dwErr = pThis->Reconnect();
			if(!dwErr)
			{
				Trace( _T("Reconnect done."));
			}
			else
			{
				CWin32Error Err;
				if((const TCHAR*)Err)
					Trace( _T("Failed to reconnect : %s"), (const TCHAR*)Err);
				else
					Trace( _T("Failed to reconnect") );
			}
		}
	}

	return 0;
}