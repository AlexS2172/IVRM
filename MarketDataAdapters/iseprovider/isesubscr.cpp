#include "stdafx.h"

#include "ISESession.h"
#include "OmniApiUtils.h"
#include "Trace.h"

#pragma pack(push)
#pragma pack(1)

struct SBrxType
{
	char m_cCentralModule;
	char m_cServerType;
	unsigned short m_usTransactionNumber;
};

#pragma pack(pop)

SBrxType g_AutoBrxs[] = {	//{'B', 'O', 107}, //Quote deleted
							{'B', 'O', 108}, //Quote rejected
							{'B', 'O', 109}, //Quote stepped up
							{'B', 'O', 110}, //Quote tick worse

							{'B', 'D', 5},	 //Dedicated trade info
							{'B', 'D', 111}, //Deal

							{'B', 'I', 202},
							{'B', 'I', 204},  // updates MS
							{'B', 'I', 210},

							{'B', 'I', 103},  // trading status
							{'B', 'I', 132},
							{'B', 'I', 41},

							{'B', 'I', 9},	  // HB
							{'B', 'I', 8}	  // Text
						};

static SBrxType  g_HbBrx = {'B', 'I', 9};	  // HB

/*-------------------------[ Subscription utils ]--------------------------*/

void CISESession::Subscribe(set_event_list_t* pSubsList)
{
	if(IsLogonEnabled() == false)
		throw CISEException(0, 0, this, "Operation cancelled.");

	CAutoLock	OmniApiLock(&m_OmniApiLock);

	int32 iStatus = omniapi_set_event_ex(m_hSession, GENERAL_BROADCAST_EVENT_TYPE,
			(int8*) pSubsList);

	if(CSTATUS_FAILED(iStatus))
		throw CISEException(iStatus, 0, this, "Subscription error");
}

void CISESession::Unsubscribe(uint32* pSubsHandles)
{
	if(IsLogonEnabled() == false)
		throw CISEException(0, 0, this, "Operation cancelled.");

	CAutoLock	OmniApiLock(&m_OmniApiLock);

	int32 iStatus = omniapi_clear_event_ex(m_hSession, GENERAL_BROADCAST_EVENT_TYPE,
			(int8*) pSubsHandles);

	if(CSTATUS_FAILED(iStatus))
		throw CISEException(iStatus, 0, this, "Unsubscription error");

}

/*-------------------------[ Subscribe & Restore Broadcasts ]--------------------------*/

void CISESession::RestoreSubscriptions()
{
	CAutoLock SubLock(&m_SubscriptionsLock);
	m_Subscriptions.clear();

	for(mapSubscribedSymbols::iterator it = m_SubscribedSymbols.begin(); it != m_SubscribedSymbols.end(); it++)
	{
		for(DWORD dwCount = 0; dwCount < it->second; dwCount++)
		{
			try
			{
				if(it->first.m_Type == enOPT)
				{
					// first we should ask for trading states so as to properly handle BBO brxs
					if(dwCount == 0)
					{
						if(it->first.m_bGroup)
						{
							map<CSymbolParams, CQuoteBBO> mapSeriesBBO;
							QueryGroupOptionsBBO(it->first.m_SymbolParams, mapSeriesBBO);
						}
						else
						{
							CQuoteBBO BBO;
							QueryOptionBBO(it->first.m_SymbolParams, BBO);
						}
					}

					// then sub
					SubscribeOption(it->first.m_SymbolParams, it->first.m_bGroup, false);
				}
				else
				{
					SubscribeUnderlying(it->first.m_SymbolParams, false);
				}
			}
			catch(CISEException& e)
			{
				IseTrace(enError, "Failed to restore subscription for %s : %s", it->first.m_SymbolParams.m_sSymbol.c_str(),
					e.GetDescription());
			}
		}
	}
}

void CISESession::SubscribeGeneralBroadcasts()
{
	char	pBuf[4096];
	uint32	uiBufLen = sizeof(pBuf);
	int32	iStatus = 0;

	memset(pBuf, 0, uiBufLen);

	if(IsLogonEnabled() == false)
		throw CISEException(0, 0, this, "Operation cancelled.");

	{
		CAutoLock	OmniApiLock(&m_OmniApiLock);

		iStatus = omniapi_read_event_ext_ex(m_hSession, OMNI_EVTTYP_SHOW_SUBSCR,
			pBuf, &uiBufLen, NULL, 0);
	}

	if(CSTATUS_FAILED(iStatus))
		throw CISEException(iStatus, 0, this, "General broadcasts subscription error");

	if(iStatus == OMNIAPI_OVERFLOW)
		throw CISEException(iStatus, 0, this, "General broadcasts subscription error (Overflow)");

	const show_subscr_list_t* pAuthBroadcasts = (show_subscr_list_t*)pBuf;

	uint32 uiLen = sizeof(int32) + sizeof(subscr_item_t) * pAuthBroadcasts->objects_u;

	set_event_list_t* pSubsList = (set_event_list_t*)malloc(uiLen);
	uint32 uiSubCount = 0;

	for(uint32 uiIndex = 0; uiIndex < pAuthBroadcasts->objects_u; uiIndex++)
	{
		const auth_infobj_t& Object = pAuthBroadcasts->assobj_x[uiIndex];

		if(Object.force_u == 0)
		{
			for(int nBrx = 0; nBrx < sizeof(g_AutoBrxs) / sizeof(SBrxType); nBrx++)
			{
				if(memcmp(&g_AutoBrxs[nBrx],&Object.infobj_x.brdcst_x, sizeof(SBrxType)) == 0)
				{
					// if it is primary session then subscribe on everything
					// if it is secondary session than only HB
					if(m_bPrimary || memcmp(&g_HbBrx, &Object.infobj_x.brdcst_x, sizeof(SBrxType)) == 0)
					{
						pSubsList->subitm_x[uiSubCount].infobj_x = Object.infobj_x;
						uiSubCount++;
						break;
					}
				}
			}
		}
	}
	
	if(uiSubCount != 0)
	{
		pSubsList->buflen_i = sizeof(int32) + sizeof(subscr_item_t) * uiSubCount;

		if(IsLogonEnabled() == false)
			throw CISEException(0, 0, this, "Operation cancelled.");

		CAutoLock	OmniApiLock(&m_OmniApiLock);
		
		iStatus = omniapi_set_event_ex(m_hSession, GENERAL_BROADCAST_EVENT_TYPE,
			(int8*) pSubsList);
	}

	free(pSubsList);

	if(CSTATUS_FAILED(iStatus))
		throw CISEException(iStatus, 0, this, "General broadcasts subscription error");
}

void CISESession::SubscribeAll()
{
	char	pBuf[4096];
	uint32	uiBufLen = sizeof(pBuf);
	int32	iStatus;

	memset(pBuf, 0, uiBufLen);

	/* Ask the gateway for a list of known event types */

	if(IsLogonEnabled() == false)
		throw CISEException(0, 0, this, "Operation cancelled.");

	{
		CAutoLock	OmniApiLock(&m_OmniApiLock);

		iStatus = omniapi_read_event_ext_ex(m_hSession, OMNI_EVTTYP_SHOW,
			pBuf, &uiBufLen, NULL, 0);
	}

	// loop through all events
	char* pPos = pBuf; 
	while(CSTATUS_SUCCEEDED(iStatus))
	{

		if(IsLogonEnabled() == false)
			throw CISEException(0, 0, this, "Operation cancelled.");

		long lEventType = strtol(pPos, &pPos, 10);
		if(lEventType == 0)
			break;

		if(*pPos != 0)
			pPos++;

		if(lEventType == GENERAL_BROADCAST_EVENT_TYPE)
		{
			SubscribeGeneralBroadcasts();
		}
		else
		{
			CAutoLock	OmniApiLock(&m_OmniApiLock);

			iStatus = omniapi_set_event_ex (m_hSession, lEventType, NULL);
		}
	}

	if(CSTATUS_FAILED(iStatus))
		throw CISEException(iStatus, 0, this, "Subscription Error");
}

/*-------------------------[ Subscribe & unsubscribe symbols ]--------------------------*/

bool CISESession::SubscribeOption(const CSymbolParams& Req, bool bGroup, bool bAddSubscription) throw (CISEException)
{
	CAutoLock SubLock(&m_SubscriptionsLock);

	uint16	uiCommodity;
		
	{
		CAutoLock	DataLock(this);

		if(IsLoaded() == false)
			return false;

		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		if(bGroup)
		{
			CUnderlying* pUnd = GetUnderlyingByRequest(Req);
			if(pUnd == NULL)
			{
				throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
					"No such symbol found");
			}

			uiCommodity = pUnd->m_uiCommodity;
		}
		else
		{
			CSeries* pSeries = GetSeriesByRequest(Req);
			if(pSeries == NULL || pSeries->m_pUnderlying == NULL)
			{
				throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
					"No such symbol found");
			}

			uiCommodity = pSeries->m_pUnderlying->m_uiCommodity;
		}
	}

	CSubscriptionItem Item;
	Item.m_Data.infobj_x.infsrc_n = 0;
	Item.m_Data.infobj_x.inftyp_n = 3;
	Item.m_Data.infobj_x.brdcst_x.central_module_c = 'B';
	Item.m_Data.infobj_x.brdcst_x.server_type_c = 'O';
	Item.m_Data.infobj_x.brdcst_x.transaction_number_n = 101;
	Item.m_Data.infobj_x.attrib_x.underlying_x.commodity_u = uiCommodity;

	{
		set<CSubscriptionItem>::iterator it = m_Subscriptions.find(Item);
		
		// if item is inserted for the first time 
		if(it == m_Subscriptions.end())
		{
			uint32 uiLen = sizeof(int32) + sizeof(subscr_item_t)/* * 2*/;
			set_event_list_t* pSubsList = (set_event_list_t*)malloc(uiLen);
			pSubsList->subitm_x[0].infobj_x = Item.m_Data.infobj_x;
			pSubsList->buflen_i = uiLen;
			
			try
			{
				Subscribe(pSubsList);

				//IseTrace(enInfo, "Sub on series for und = %d.", uiCommodity);
			}
			catch(CISEException& e)
			{
				free(pSubsList);
				if(e.IsConnectionError())
					return false;

				throw;
			}

			Item.m_dwRefCount = 1;
			Item.m_Data = pSubsList->subitm_x[0];
			
			m_Subscriptions.insert(Item);

			free(pSubsList);
		}
		else
		{
			it->m_dwRefCount++;
		}
	}

	if(bAddSubscription)
	{
		CSubscribedSymbol	Symbol;
		Symbol.m_Type = enOPT;
		Symbol.m_SymbolParams = Req;
		Symbol.m_bGroup = bGroup;
		if(m_SubscribedSymbols.find(Symbol) == m_SubscribedSymbols.end())
			m_SubscribedSymbols[Symbol] = 1;
		else
			m_SubscribedSymbols[Symbol]++;
	}
	
	return true;
};

bool CISESession::SubscribeUnderlying(const CSymbolParams& Req, bool bAddSubscription) throw (CISEException)
{
	uint16	uiCommodity;

	CAutoLock SubLock(&m_SubscriptionsLock);

	{
		CAutoLock	DataLock(this);

		if(IsLoaded() == false)
			return false;
		
		CAutoLock	UndLock(&m_ISE.m_Underlyings);

		CUnderlying* pUnd = GetUnderlyingByRequest(Req);
		if(pUnd == NULL)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}

		uiCommodity = pUnd->m_uiCommodity;
	}

	CSubscriptionItem Item;
	Item.m_Data.infobj_x.infsrc_n = 0;
	Item.m_Data.infobj_x.inftyp_n = 3;
	Item.m_Data.infobj_x.brdcst_x.central_module_c = 'B';
	Item.m_Data.infobj_x.brdcst_x.server_type_c = 'O';
	Item.m_Data.infobj_x.brdcst_x.transaction_number_n = 105;
	Item.m_Data.infobj_x.attrib_x.derivative_x.commodity_n = uiCommodity;

	{
		set<CSubscriptionItem>::iterator it = m_Subscriptions.find(Item);
		
		// if item is inserted for the first time 
		if(it == m_Subscriptions.end())
		{
			uint32 uiLen = sizeof(int32) + sizeof(subscr_item_t);
			set_event_list_t* pSubsList = (set_event_list_t*)malloc(uiLen);

			pSubsList->subitm_x[0].infobj_x = Item.m_Data.infobj_x;
			pSubsList->buflen_i = uiLen;

			try
			{
				Subscribe(pSubsList);

				//IseTrace(enInfo, "Sub on und = %d.", uiCommodity);
			}
			catch(CISEException& e)
			{
				free(pSubsList);
				if(e.IsConnectionError())
					return false;

				throw;
			}

			Item.m_dwRefCount = 1;
			Item.m_Data = pSubsList->subitm_x[0];
			
			m_Subscriptions.insert(Item);
			free(pSubsList);
		}
		else
		{
			it->m_dwRefCount++;
		}
	}

	if(bAddSubscription)
	{
		CSubscribedSymbol	Symbol;
		Symbol.m_Type = enSTK;
		Symbol.m_SymbolParams = Req;
		Symbol.m_bGroup = false;
		if(m_SubscribedSymbols.find(Symbol) == m_SubscribedSymbols.end())
			m_SubscribedSymbols[Symbol] = 1;
		else
			m_SubscribedSymbols[Symbol]++;
	}
	
	return true;
};

void CISESession::UnsubscribeOption(const CSymbolParams& Req, bool bGroup) throw (CISEException)
{
	CAutoLock SubLock(&m_SubscriptionsLock);

	CSubscribedSymbol	Symbol;
	Symbol.m_Type = enOPT;
	Symbol.m_SymbolParams = Req;
	Symbol.m_bGroup = bGroup;
	mapSubscribedSymbols::iterator itSymb = m_SubscribedSymbols.find(Symbol);

	if(itSymb == m_SubscribedSymbols.end())
		return;

	if(--itSymb->second == 0)
		m_SubscribedSymbols.erase(itSymb);

	uint16	uiCommodity;

	{
		CAutoLock	DataLock(this);

		if(IsLoaded() == false)
			return;

		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		if(bGroup)
		{
			CUnderlying* pUnd = GetUnderlyingByRequest(Req);
			if(pUnd == NULL)
			{
				throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
					"No such symbol found");
			}

			uiCommodity = pUnd->m_uiCommodity;
		}
		else
		{
			CSeries* pSeries = GetSeriesByRequest(Req);
			if(pSeries == NULL || pSeries->m_pUnderlying == NULL)
			{
				throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
					"No such symbol found");
			}

			uiCommodity = pSeries->m_pUnderlying->m_uiCommodity;
		}
	}

	CSubscriptionItem Item;
	Item.m_Data.infobj_x.infsrc_n = 0;
	Item.m_Data.infobj_x.inftyp_n = 3;
	Item.m_Data.infobj_x.brdcst_x.central_module_c = 'B';
	Item.m_Data.infobj_x.brdcst_x.server_type_c = 'O';
	Item.m_Data.infobj_x.brdcst_x.transaction_number_n = 101;
	Item.m_Data.infobj_x.attrib_x.underlying_x.commodity_u = uiCommodity;

	{
		set<CSubscriptionItem>::iterator it = m_Subscriptions.find(Item);
		if(it != m_Subscriptions.end())
		{
			if(--it->m_dwRefCount == 0)
			{
				uint32 uiHandles[2];
				uiHandles[0] = sizeof(uiHandles);
				uiHandles[1] = it->m_Data.handle_u;
				m_Subscriptions.erase(it);

				try
				{
					Unsubscribe(uiHandles);

					//IseTrace(enInfo, "Unsub from series for und = %d.", uiCommodity);
				}
				catch(...)
				{
				}
			}
		}
	}
};

void CISESession::UnsubscribeUnderlying(const CSymbolParams& Req) throw (CISEException)
{
	CAutoLock SubLock(&m_SubscriptionsLock);

	CSubscribedSymbol	Symbol;
	Symbol.m_Type = enSTK;
	Symbol.m_SymbolParams = Req;
	Symbol.m_bGroup = false;
	mapSubscribedSymbols::iterator itSymb = m_SubscribedSymbols.find(Symbol);
	
	if(itSymb == m_SubscribedSymbols.end())
		return;
	
	if(--itSymb->second == 0)
		m_SubscribedSymbols.erase(itSymb);

	uint16	uiCommodity;

	{
		CAutoLock	DataLock(this);

		if(IsLoaded() == false)
			return;

		CAutoLock	UndLock(&m_ISE.m_Underlyings);

		CUnderlying* pUnd = GetUnderlyingByRequest(Req);
		if(pUnd == NULL)
		{
			throw CISEException(OMNIAPI_NOT_FOUND, 0, m_pSession, 
				"No such symbol found");
		}

		uiCommodity = pUnd->m_uiCommodity;
	}

	CSubscriptionItem Item;
	Item.m_Data.infobj_x.infsrc_n = 0;
	Item.m_Data.infobj_x.inftyp_n = 3;
	Item.m_Data.infobj_x.brdcst_x.central_module_c = 'B';
	Item.m_Data.infobj_x.brdcst_x.server_type_c = 'O';
	Item.m_Data.infobj_x.brdcst_x.transaction_number_n = 105;
	Item.m_Data.infobj_x.attrib_x.underlying_x.commodity_u = uiCommodity;

	
	{
		set<CSubscriptionItem>::iterator it = m_Subscriptions.find(Item);

		if(it != m_Subscriptions.end())
		{
			if(--it->m_dwRefCount == 0)
			{
				uint32 uiHandles[2];
				uiHandles[0] = sizeof(uiHandles);
				uiHandles[1] = it->m_Data.handle_u;
				m_Subscriptions.erase(it);

				try
				{
					Unsubscribe(uiHandles);

					//IseTrace(enInfo, "Unsub from und = %d.", uiCommodity);
				}
				catch(...)
				{
				}
			}
		}
	}
};


