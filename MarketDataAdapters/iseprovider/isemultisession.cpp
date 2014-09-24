#include "StdAfx.h"
#include "ISEMultiSession.h"
#include "OmniApiUtils.h"

void CAsyncISESession::Logon(const bool bPrimary,
							 const char* szAddr, const char* szPort,
							 const char* szUser, const char* szPassword,
							 const char* szProgID,
							 const char* szNewPassword)
{
	CISESession::Logon( bPrimary, szAddr, szPort,
						szUser, szPassword, szProgID,
						szNewPassword);

	unsigned int uiID;
	m_hAsyncStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hAsyncEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hReadyEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_hAsyncThread = (HANDLE)_beginthreadex(NULL, 0, AsyncThread, this, 0, &uiID);
}

void CAsyncISESession::Logoff()
{
	if(m_hAsyncStopEvent)
	{
		SetEvent(m_hAsyncStopEvent);

		WaitForSingleObject(m_hAsyncThread, INFINITE);

		CloseHandle(m_hAsyncThread);
		CloseHandle(m_hAsyncStopEvent);
		CloseHandle(m_hAsyncEvent);
		CloseHandle(m_hReadyEvent);

		m_hAsyncThread = NULL;
		m_hAsyncStopEvent = NULL;
		m_hAsyncEvent = NULL;
		m_hReadyEvent = NULL;
	}

	CISESession::Logoff();
}

UINT __stdcall CAsyncISESession::AsyncThread( void *pThisClass)
{
	return ((CAsyncISESession*)pThisClass)->AsyncProc();
};

UINT  CAsyncISESession::AsyncProc()
{
const HANDLE hEvents[] = {m_hAsyncEvent, m_hAsyncStopEvent};

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE);

		switch(dwRes)
		{
			case WAIT_OBJECT_0:
			{
				ErrorNumberEnum Error = enNoError;
				string			sOrderID;

				switch(m_RequestType)
				{
					case enArtSumbit:
					{
						CSubmitQuoteRequest* pRequest = (CSubmitQuoteRequest*)m_pRequestData;

						try
						{
							SubmitQuotes(pRequest->m_Data, sOrderID);
						}
						catch(CISEException& e)
						{
							//EgAssert(e.IsConnectionError());
							Error = e.IsConnectionError() ? enNotConnected : enNoAccessForData;
						}

						vector<CQuoteParams*>::iterator it;
						for(it = pRequest->m_Refs.begin(); it != pRequest->m_Refs.end(); it++)
						{	
							(*it)->m_SendResult.m_Error = Error;
							(*it)->m_SendResult.m_sOrderID = sOrderID;
						}
					}
					break;

					case enArtDelete:
					{
						CDeleteQuoteRequest* pRequest = (CDeleteQuoteRequest*)m_pRequestData;

						try
						{
							DeleteQuote(pRequest->m_Data, sOrderID);
						}
						catch(CISEException& e)
						{
							//EgAssert(e.IsConnectionError());
							Error = e.IsConnectionError() ? enNotConnected : enNoAccessForData;
						}

						pRequest->m_Ref->m_SendResult.m_Error = Error;
						pRequest->m_Ref->m_SendResult.m_sOrderID = sOrderID;
					}
					break;
				}
				SetEvent(m_hReadyEvent);
			}
			break;

			default:
				return 0;
		}
	}
}

CAsyncISESession::CAsyncISESession()
{
	m_hAsyncThread = NULL;
	m_hAsyncStopEvent = NULL;
	m_hAsyncEvent = NULL;
	m_hReadyEvent = NULL;
};

void CISEMultiSession ::Logon(const char* szAddr, const char* szPort,
							  vector<CAccount>& vecAccounts,
							  const char* szProgID,
							  const char* szNewPassword)  throw (CISEException)
{
	CISESessionBase::SetID(++m_dwSessionID);

	try
	{
		m_nSessions = vecAccounts.size();

		CISESession0::Logon(true, szAddr, szPort, vecAccounts[0].m_sUser.c_str(), 
			vecAccounts[0].m_sPassword.c_str(), szProgID, szNewPassword);

		if(m_nSessions > 1)
		{
			// 5 sec should be between each logon
			IseTrace(enInfo, "Waiting 5 seconds...");
			if(LogonSleep(5000) == false)
				throw CISEException(0, 0, (CISESession1*)this, "Operation cancelled.");

			CISESession1::Logon(false, szAddr, szPort, vecAccounts[1].m_sUser.c_str(), 
				vecAccounts[1].m_sPassword.c_str(), szProgID, szNewPassword);
		}

		if(m_nSessions > 2)
		{
			IseTrace(enInfo, "Waiting 5 seconds...");
			if(LogonSleep(5000) == false)
				throw CISEException(0, 0, (CISESession2*)this, "Operation cancelled.");

			CISESession2::Logon(false, szAddr, szPort, vecAccounts[2].m_sUser.c_str(), 
				vecAccounts[2].m_sPassword.c_str(), szProgID, szNewPassword);
		}
	}
	catch(const CISEException& e)
	{
		Logoff();

		throw e;
	}

	CAutoLock	Lock(&m_MultiSessionLock);
	m_bMultiSessionLoggedOn = true;
};

void CISEMultiSession ::Logoff() throw (CISEException)
{
	CAutoLock	Lock(&m_MultiSessionLock);
	m_bMultiSessionLoggedOn = false;

	try
	{
		CISESession0::Logoff();
	}
	catch(CISEException&)
	{
	}
	try
	{
		CISESession1::Logoff();
	}
	catch(CISEException&)
	{
	}
	try
	{
		CISESession2::Logoff();
	}
	catch(CISEException&)
	{
	}
}

bool CISEMultiSession::SubscribeOption(const CSymbolParams& Req, bool bGroup)
{
	return CISESession0::SubscribeOption(Req, bGroup);
}

bool CISEMultiSession::SubscribeUnderlying(const CSymbolParams& Req)
{
	return CISESession0::SubscribeUnderlying(Req);
}

void CISEMultiSession::UnsubscribeOption(const CSymbolParams& Req, bool bGroup)
{
	CISESession0::UnsubscribeOption(Req, bGroup);
}

void CISEMultiSession::UnsubscribeUnderlying(const CSymbolParams& Req)
{
	CISESession0::UnsubscribeUnderlying(Req);
}

void CISEMultiSession::ReadyToTrade() throw (CISEException)
{
	CISESession0::ReadyToTrade();

}

/****************************************************************************************************/

bool CISEMultiSession::SendQuotes(vector<CQuoteParams>& vecQuotes)
{
	CAutoLock	Lock(&m_MultiSessionLock);

	map<uint8, CBookRequests>	mapBook2Requests;

	{
		// check for connection state
		CAutoLock DataLock(this);

		if(IsLoaded() == false || m_bMultiSessionLoggedOn == false)
		{
			for(vector<CQuoteParams>::iterator itQuotes = vecQuotes.begin(); 
				itQuotes != vecQuotes.end(); itQuotes++)
			{
				itQuotes->m_SendResult.m_Error = enNotConnected;
				itQuotes->m_SendResult.m_sOrderID = "";
			}
			return false;
		}

		static quote_trans_mo106_t Mo106;
		static const int nMaxItems = /*1;*/sizeof(Mo106.item) / sizeof(quote_trans_mo106_item_t);

		{
			CAutoLock ClassesLock(&m_ISE.m_InstrumentClasses);
			CAutoLock UndLock(&m_ISE.m_Underlyings);
			CAutoLock SeriesLock(&m_ISE.m_Series);

			for(vector<CQuoteParams>::iterator itQuotes = vecQuotes.begin(); 
				itQuotes != vecQuotes.end(); itQuotes++)
			{
				// find series
				CSeries* pSeries = GetSeriesByRequest(itQuotes->m_sSeries);
				if(pSeries && pSeries->m_pUnderlying && pSeries->m_pInstrumentClass)
				{
					// check for premium
					if(itQuotes->m_dPremium == 0)
					{
						CDeleteQuoteRequest	DeleteRequest;

						DeleteRequest.m_Data.series = pSeries->m_Series;
						DeleteRequest.m_Data.orderbook_c = pSeries->m_pUnderlying->m_uiOrderbook;
						DeleteRequest.m_Data.bid_or_ask_c = itQuotes->m_BidOrAsk;

						//ThEN/B/ DeleteRequest.m_Data.order_category_n = ORDER_CAT_QUOTE;
						DeleteRequest.m_Data.order_category_filter_n = ORDER_CAT_QUOTE;
						//ThEN/E/
						

						{
							CAutoLock UserLock(&(CISESession0::m_User));

							StringToClickString(CISESession0::m_User.m_sCountryID, 
								sizeof(DeleteRequest.m_Data.whose.trading_code.country_id_s),
								DeleteRequest.m_Data.whose.trading_code.country_id_s);
							StringToClickString(CISESession0::m_User.m_sCustomerID, 
								sizeof(DeleteRequest.m_Data.whose.trading_code.ex_customer_s),
								DeleteRequest.m_Data.whose.trading_code.ex_customer_s);
						}

						DeleteRequest.m_Ref = &(*itQuotes);

						mapBook2Requests[pSeries->m_pUnderlying->m_uiOrderbook].m_DeleteRequests.push_back(DeleteRequest);
					}
					else
					{
						vector<CSubmitQuoteRequest>& vecSubmitRequests = 
							mapBook2Requests[pSeries->m_pUnderlying->m_uiOrderbook].m_SubmitRequests;

						if(vecSubmitRequests.size() == 0 || vecSubmitRequests.back().m_Data.items_n == nMaxItems)
							vecSubmitRequests.push_back(CSubmitQuoteRequest());

						CSubmitQuoteRequest& SubmitRequest = vecSubmitRequests.back();
						uint16_t& nItems = SubmitRequest.m_Data.items_n;

						SubmitRequest.m_Data.item[nItems].series = pSeries->m_Series;
						SubmitRequest.m_Data.item[nItems].open_quantity_u = itQuotes->m_lOpenQuantity;
						SubmitRequest.m_Data.item[nItems].premium_i = DBL2DEC(itQuotes->m_dPremium, pSeries->m_pInstrumentClass->m_uiDecInPremium);

						SubmitRequest.m_Data.item[nItems].quantity_u = itQuotes->m_lQuantity;
						SubmitRequest.m_Data.item[nItems].step_up_buffer_n = itQuotes->m_lQuantity;
						SubmitRequest.m_Data.item[nItems].bid_or_ask_c = itQuotes->m_BidOrAsk;
						nItems++;
						SubmitRequest.m_Refs.push_back(&(*itQuotes));
					}
				}
				else
				{
					itQuotes->m_SendResult.m_Error = enSymbolNotSupported;
					itQuotes->m_SendResult.m_sOrderID = "";
				}
			}
		}
	}


	map<uint8, CBookRequests >::iterator itBook;
	ErrorNumberEnum Error = enNoError;

	InitAsync();

	for(itBook = mapBook2Requests.begin(); itBook != mapBook2Requests.end(); itBook++)
	{
		CBookRequests& BookRequests = itBook->second;

		vector<CSubmitQuoteRequest>::iterator itSubmReq;
		for(itSubmReq = BookRequests.m_SubmitRequests.begin(); 
			itSubmReq != BookRequests.m_SubmitRequests.end();itSubmReq++)
		{
			CSubmitQuoteRequest& Req = *itSubmReq;
			Req.m_Data.orderbook_c = itBook->first;
			CallAsync(enArtSumbit, &Req);
		}

		vector<CDeleteQuoteRequest>::iterator itDelReq;
		for(itDelReq = BookRequests.m_DeleteRequests.begin(); 
			itDelReq != BookRequests.m_DeleteRequests.end();itDelReq++)
		{	
			CallAsync(enArtDelete, &(*itDelReq));
		}	
	}

	WaitAsyncComplete();

	// update cache
	CAutoLock SeriesLock(&m_ISE.m_Series);

	for(vector<CQuoteParams>::iterator itQuotes = vecQuotes.begin(); 
		itQuotes != vecQuotes.end(); itQuotes++)
	{
		const CQuoteParams& Quote = *itQuotes;
		if(Quote.m_SendResult.m_Error == enNoError)
		{
			// find series
			CSeries* pSeries = GetSeriesByRequest(itQuotes->m_sSeries);
			if(pSeries)
			{
				pSeries->UpdateQuoteCache(Quote, 0);
			}
		}
	}

	return true;
}

void CISEMultiSession::CallAsync(EnumAsyncRequestType Type, void* pParam)
{
	HANDLE hReadyEvents[] = {CISESession0::m_hReadyEvent, 
							 CISESession1::m_hReadyEvent, 
							 CISESession2::m_hReadyEvent};

	DWORD dwRes = WaitForMultipleObjects(m_nSessions, hReadyEvents, FALSE, INFINITE);
	switch(dwRes)
	{
		case WAIT_OBJECT_0:
			CISESession0::m_RequestType = Type;
			CISESession0::m_pRequestData = pParam;
			SetEvent(CISESession0::m_hAsyncEvent);
			break;
		case WAIT_OBJECT_0 + 1:
			CISESession1::m_RequestType = Type;
			CISESession1::m_pRequestData = pParam;
			SetEvent(CISESession1::m_hAsyncEvent);
			break;
		case WAIT_OBJECT_0 + 2:
			CISESession2::m_RequestType = Type;
			CISESession2::m_pRequestData = pParam;
			SetEvent(CISESession2::m_hAsyncEvent);
			break;
		default:
			break;
	}
}

void CISEMultiSession::WaitAsyncComplete()
{
	HANDLE hReadyEvents[] = {CISESession0::m_hReadyEvent, 
							 CISESession1::m_hReadyEvent, 
							 CISESession2::m_hReadyEvent};

	WaitForMultipleObjects(m_nSessions, hReadyEvents, TRUE, INFINITE);
}

void CISEMultiSession::InitAsync()
{
	if(CISESession0::m_hReadyEvent)
		SetEvent(CISESession0::m_hReadyEvent);

	if(CISESession1::m_hReadyEvent)
		SetEvent(CISESession1::m_hReadyEvent);

	if(CISESession2::m_hReadyEvent)
		SetEvent(CISESession2::m_hReadyEvent);
}


/**************************************************************************************************/

void CISEMultiSession::SetMMParameters(	// role
							bool bPmm,
							// for underlying or class
							const char* const	 szUnd,
							const unsigned char  uiInstrumentGroup,

							const unsigned char  uiExpirationGroup,
							const unsigned char  uiStrikePriceGroup,

							// CMM parameters
							const unsigned short* puiTickWorseVolume,

							const unsigned short uiStepUpAfterRegenBuffer,

							const unsigned short uiIseMMTradeLimitAbsolute,
 						    const unsigned short uiFirmTradeLimitAbsolute,
						    const unsigned short uiFarMMTradeLimitAbsolute,
						    
							const unsigned char  uiIseMmTradeLimitFraction,
							const unsigned char  uiFirmTradeLimitFraction,
							const unsigned char  uiFarMmTradeLimitFraction,

							// PMM parameters
							const unsigned short* puiDerivedOrderMaxSize,
							const unsigned short* puiMatchAwayMarketMaxSize
						 )
{
	CISESession0::SetMMParameters(bPmm, szUnd,uiInstrumentGroup,
		uiExpirationGroup, uiStrikePriceGroup, puiTickWorseVolume,
		uiStepUpAfterRegenBuffer, uiIseMMTradeLimitAbsolute,
 		uiFirmTradeLimitAbsolute, uiFarMMTradeLimitAbsolute,
		uiIseMmTradeLimitFraction, uiFirmTradeLimitFraction,
		uiFarMmTradeLimitFraction, puiDerivedOrderMaxSize,
		puiMatchAwayMarketMaxSize);
}

void CISEMultiSession::EnterOrder( const string& sSeries, 
					 const unsigned int uiQuantity, 
					 const double dPremium,
					 const BidAskEnum BidOrAsk,
					 string& sOrderID)
{
	CISESession0::EnterOrder(sSeries, uiQuantity, dPremium, BidOrAsk, sOrderID);
};