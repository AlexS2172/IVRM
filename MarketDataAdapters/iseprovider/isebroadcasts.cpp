#include "stdafx.h"

#include "ISESession.h"
#include "OmniApiUtils.h"
#include "Trace.h"

/*-------------------------[ Brx Thread ]--------------------------*/

UINT  __stdcall CISESession::Thread(void* pParam)
{
	struct _timeb tmNow;
	unsigned __int64 qStart, qEnd = 0;

	while(true)
	{
		DWORD dwRes = WaitForSingleObject(GetStopEventHandle(), 0);
		switch(dwRes)
		{
			case WAIT_OBJECT_0:
				return 0;

			case WAIT_TIMEOUT:
			{
				_ftime(&tmNow);
				qStart = tmNow.time * 1000 + tmNow.millitm;

				if(IsLogonEnabled() == false)
					return 0;

				bool bRes = ReadIseEvents();
				if(bRes == false)
					return 0;

				_ftime(&tmNow);
				qEnd = tmNow.time * 1000 + tmNow.millitm;
				unsigned __int64 qInterval = qEnd - qStart ;

				if(qInterval > TIME_DELAY_BROADCASTS)
					/*IseTrace(enWarning, 
					"Too long delay between omniapi_read_event_ext calls : %I64u ms !", 
					qInterval)*/;
				else
					Sleep(TIME_DELAY_BROADCASTS - qInterval);
				break;
			}

			default:
				return -1;

		}
	}

	return 0;
};

/*-------------------------[ Read brx ]--------------------------*/

bool CISESession::ReadIseEvents()
{
	char	pBrxBuf[65536];
	uint32	uiBrxBufLen = sizeof(pBrxBuf);
	
	int32	iStatus = 0;

	{
		CAutoLock	OmniApiLock(&m_OmniApiLock);

		iStatus = omniapi_read_event_ext_ex(m_hSession, OMNI_EVTTYP_ALL,
			pBrxBuf, &uiBrxBufLen, NULL, READEVENT_OPTMASK_MANY);
	}


	switch(iStatus)
	{
		case OMNIAPI_NOT_FOUND:
			return true;
			break;

		case OMNIAPI_OVERFLOW:
			IseTrace(enWarning, "Some ISE broadcasts are lost.");
			break;

		default:
			if(CSTATUS_FAILED(iStatus))
			{
				OnDisconnected(GetID());
				return false;
			}
			break;
	}

	char*	pBuf = pBrxBuf;

	// loop through all events
	while(true)
	{
		EgTrace("New Brx");
		subscribed_event_t* pEvent = (subscribed_event_t*)pBuf;

		switch(pEvent->handle_u)
		{
			case 0xffffffff:
				EgTrace("brx 0xffffffff handle");
				// it is subscription handle, which is always -1
				if(pEvent->evelen_n > 0)
				{
					OnBroadcast((broadcast_type_t*)(pBuf + sizeof(subscribed_event_t)),
						pEvent->evelen_n);

					pBuf += sizeof(subscribed_event_t) + pEvent->evelen_n;
				}
				break;

			default:
				EgTrace("brx other handle");
				// broadcast did NOT have a subscription handle.. on the assumption noted above
				// fudge.. point pEvent four bytes less.. horrible hack
				pEvent = (subscribed_event_t*)((char*)pEvent - sizeof(pEvent->handle_u));
				
				if(pEvent->evelen_n > 0)
				{
					OnBroadcast((broadcast_type_t*)(pBuf + sizeof(subscribed_event_t)),
						pEvent->evelen_n);
				}

				pBuf += sizeof(subscribed_event_t) + pEvent->evelen_n;
				break;
		}

		if(uiBrxBufLen - (pBuf - pBrxBuf) <= sizeof(subscribed_event_t))
			return true;
	}
};

void CISESession::OnBroadcast(const broadcast_type_t* const pEvent, const uint16 uiLen)
{
	EgTrace("brx %c %c %d", pEvent->central_module_c, pEvent->server_type_c, pEvent->transaction_number_n);
	if(pEvent->central_module_c != 'B')
	{
		IseTrace(enWarning, "Unknown broadcast type '%c%c%d' length=%d",
			pEvent->central_module_c, pEvent->server_type_c, (int)pEvent->transaction_number_n, (int)uiLen);

		return;
	}

	switch(pEvent->server_type_c)
	{
		case 'D':
			OnBroadcastBD(pEvent, uiLen);
			break;

		case 'I':
			OnBroadcastBI(pEvent, uiLen);
			break;

		case 'N':
			OnBroadcastBN(pEvent, uiLen);
			break;

		case 'O':
			OnBroadcastBO(pEvent, uiLen);
			break;

		default:
			IseTrace(enWarning, "Unknown broadcast type '%c%c%d' length=%d",
				pEvent->central_module_c, pEvent->server_type_c, (int)pEvent->transaction_number_n, (int)uiLen);
			break;
	}

	EgTrace("brx %c %c %d processed", pEvent->central_module_c, pEvent->server_type_c, pEvent->transaction_number_n);

	return;
};

void CISESession::OnBroadcastBD(const broadcast_type_t* const pEvent, const uint16 uiLen)
{
	switch(pEvent->transaction_number_n)
	{
		case 5:   // BD5 - Dedicated trade info
			OnBroadcastBD5((directed_trade_t*)pEvent, uiLen);
			break;

		case 101: // BD101 - Trade Information
			//OnBroadcastBD101((trade_info_bd101_t*)pEvent, uiLen);
			break;

		case 102:  // BD102 - OPRA Trade info
			//OnBroadcastBD102((opra_trade_info_bd102_t*)pEvent, uiLen);
			break;

		case 103:  // BD103 - Underlying Trade info
			//OnBroadcastBD103((underlying_trade_info_bd103_t*)pEvent, uiLen);
			break;

		case 111:   // BD111 - Deal
			OnBroadcastBD111((deal_user_bd111_t*)pEvent, uiLen);
			break;

		default:
			IseTrace(enWarning, "Unknown broadcast type '%c%c%d' length=%d",
				pEvent->central_module_c, pEvent->server_type_c, (int)pEvent->transaction_number_n, (int)uiLen);
			break;
	}
}

void CISESession::OnBroadcastBI(const broadcast_type_t* const pEvent, const uint16 uiLen)
{
	switch(pEvent->transaction_number_n)
	{
		case 8:// BI8 - General Message i.e. text
			OnBroadcastBI8((general_message_t*)pEvent, uiLen);
			break;

		case 9:  // BI9 - price information heartbeat
			OnBroadcastBI9((info_heartbeat_t*)pEvent, uiLen);
			break;

		case 41: // BI41 Instrument Status 
			OnBroadcastBI41((instrument_status_info_t*)pEvent, uiLen);
			break;

		case 103:// BI103 - Series Status Change
			OnBroadcastBI103((series_status_change_bi103_t*)pEvent, uiLen);
			break;

		case 114:// BI114 - Away Market Check
			break;

		case 132: // BI132 - Underlying Trading Status
			OnBroadcastBI132((underlying_trading_status_bi132_t*)pEvent, uiLen);
			break;

		case 202:// BI202 - Series Update - new or altered series info
			OnBroadcastBI202((series_update_bi202_t*)pEvent, uiLen);
			break;

		case 204:// BI204 - Underlying Update - new or altered underlying info
			OnBroadcastBI204((underlying_update_bi204_t*)pEvent, uiLen);
			break;

		case 210:// BI210 - Instrument Class Update - new or altered instrument class
			OnBroadcastBI210((instrument_class_update_bi210_t*)pEvent, uiLen);
			break;

		default:
			IseTrace(enWarning, "Unknown broadcast type '%c%c%d' length=%d",
				pEvent->central_module_c, pEvent->server_type_c, (int)pEvent->transaction_number_n, (int)uiLen);
			break;
	}
}

void CISESession::OnBroadcastBN(const broadcast_type_t* const pEvent, const uint16 uiLen)
{
	switch(pEvent->transaction_number_n)
	{
		case 1: // BN1 - Network event
			OnBroadcastBN1((omni_broadcast*)pEvent, uiLen);
			break;
		
		default:
			IseTrace(enWarning, "Unknown broadcast type '%c%c%d' length=%d",
				pEvent->central_module_c, pEvent->server_type_c, (int)pEvent->transaction_number_n, (int)uiLen);
			break;
	}
}

void CISESession::OnBroadcastBO(const broadcast_type_t* const pEvent, const uint16 uiLen)
{
	switch(pEvent->transaction_number_n)
	{
		case 101: // BO101 - Best Bid Offer
			OnBroadcastBO101((best_bid_offer_bo101_t*)pEvent, uiLen);
			break;

		case 102:  // BO102 - Extended Best Bid Offer
			OnBroadcastBO102((best_bid_offer_extended_bo102_t*)pEvent, uiLen);
			break;

		case 103:  // BO103 - OPRA Best Bid Offer
			//OnBroadcastBO103((opra_best_bid_offer_bo103_t*)pEvent, uiLen);
			break;

		case 104:  // BO104 - Each Exchange OPRA Best Bid Offer
			//OnBroadcastBO104((opra_all_best_bid_offer_bo104_t*)pEvent, uiLen);
			break;

		case 105:  // BO105 - Underlying Best Bid Offer
			OnBroadcastBO105((underlying_best_bid_offer_bo105_t*)pEvent, uiLen);
			break;

		case 106:  // BO106 - Block/Facilitation Order
			break;

		case 107:  // BO107 - Order/Quote Deleted
			break;

		case 108:  // BO108 - Quote Rejection
			OnBroadcastBO108((quote_rejection_reason_bo108_t*)pEvent, uiLen);
			break;

		case 109:  // BO109 - Quote Step-up
			OnBroadcastBO109((quote_stepped_up_bo109_t*)pEvent, uiLen);
			break;

		case 110:  // BO110 - Quote Tick-Worse
			OnBroadcastBO110((quote_tick_worse_bo110_t*)pEvent, uiLen);
			break;

		case 112:  // BO112 - Block/Facilitation Order Ended
			break;

		case 115:  // BO115 - PMM Locked Order locked_order_bo115
			break;

		case 116:  // BO116 - PMM Derived Order
			break;

		case 130:  // BO130 - orderbook changes, user to maintain local copy of user's/member's OB
			// TO DO?
			break;

		case 131:  // BO131 - public combo OB broadcast
			break;

		case 132:  // BO132 - combo OB broadcast for own user's/member's.. like bo130 for combos
			break;

		default:
			IseTrace(enWarning, "Unknown broadcast type '%c%c%d' length=%d",
				pEvent->central_module_c, pEvent->server_type_c, (int)pEvent->transaction_number_n, (int)uiLen);
			break;
	}
}

/*-------------------------[ Net status ]--------------------------*/

void CISESession::OnBroadcastBN1(const omni_broadcast* pData, const uint16 uiLen)
{
	EgAssert(false);

	if(pData->network_status_x.status_u == OMNI_DU_DATALOST || 
		pData->network_status_x.status_u == OMNI_DU_LINKLOST)
	{
		OnDisconnected(GetID());
	}
}

/*-------------------------[ Text data ]--------------------------*/

void CISESession::OnBroadcastBI8(const general_message_t* pData, const uint16 uiLen)
{
	const uint16 nItems = pData->items_n;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const general_message_item_t& Item = pData->item[nIndex];

		string sText;
		CS2S(Item.text_line_s, sText);
		AddGeneralMessage(pData->country_c, pData->market_c, sText);
	}	
}

/*-------------------------[ Price information heartbeat ]--------------------------*/

void CISESession::OnBroadcastBI9(const info_heartbeat_t* pData, const uint16 uiLen)
{
	OnHeartbeat(pData->instance_c);
}

/*-------------------------[ Trading status ]--------------------------*/

void CISESession::OnBroadcastBI41(const instrument_status_info_t* pData, const uint16 uiLen)
{
	const uint16 nItems = pData->items_n;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const instrument_status_info_item_t& Item = pData->item[nIndex];

		if(Item.minutes_to_state_change_n == 0)
		{
			ModifyInstrumentStatus(Item.series, Item.state_number_n);
		}
		else
		{
			string sText;
			CS2S(Item.warning_msg_s, sText);

			ModifyInstrumentStatus(Item.series, Item.state_number_n,
				Item.minutes_to_state_change_n, sText.c_str());
		}
	}
}

void CISESession::OnBroadcastBI103(const series_status_change_bi103_t* pData, const uint16 uiLen)
{
	const uint16 nItems = pData->items_n;

	for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
	{
		const series_status_change_bi103_item_t& Item = pData->item[nIndex];

		ModifySeriesStatus(Item);
	}
}

void CISESession::OnBroadcastBI132(const underlying_trading_status_bi132_t* pData, const uint16 uiLen)
{
	ModifyUnderlyingStatus(*pData);
}

/*-------------------------[ MS : Series ]--------------------------*/

void CISESession::OnBroadcastBI202(const series_update_bi202_t* pData, const uint16 uiLen)
{
	// update be in transaction
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);
	CAutoLock	SeriesLock(&m_ISE.m_Series);

	switch(pData->chg_type_n)
	{
		case CHANGE_TYPE_DELETE:
			DeleteSeries(pData->da202);
			break;

		case CHANGE_TYPE_MODIFY:
			ModifySeries(pData->da202);
			break;

		case CHANGE_TYPE_ADD:
			AddSeries(pData->da202);
			break;

		default:
			IseTrace(enWarning, "Unknown change type for BI202 = %d.", pData->chg_type_n);
			break;
	}

	m_ISE.UpdateMarketStructure(this);
	m_ISE.UpdateInstrumentClasses(this);
};

/*-------------------------[ MS : Underyling ]--------------------------*/

void CISESession::OnBroadcastBI204(const underlying_update_bi204_t* pData, const uint16 uiLen)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);
	CAutoLock	SeriesLock(&m_ISE.m_Series);

	switch(pData->chg_type_n)
	{
		case CHANGE_TYPE_DELETE:
			DeleteUnderlying(pData->da204);
			break;

		case CHANGE_TYPE_MODIFY:
			ModifyUnderlying(pData->da204);
			break;

		case CHANGE_TYPE_ADD:
			AddUnderlying(pData->da204);
			break;

		default:
			IseTrace(enWarning, "Unknown change type for BI204 = %d.", pData->chg_type_n);
			break;
	}

	m_ISE.UpdateMarketStructure(this);
	m_ISE.UpdateInstrumentClasses(this);
};

/*-------------------------[ MS : classes ]--------------------------*/

void CISESession::OnBroadcastBI210(const instrument_class_update_bi210_t* pData, const uint16 uiLen)
{
	CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
	CAutoLock	UndLock(&m_ISE.m_Underlyings);
	CAutoLock	SeriesLock(&m_ISE.m_Series);

	switch(pData->chg_type_n)
	{
		case CHANGE_TYPE_DELETE:
			DeleteInstrumentClass(pData->da210);
			break;

		case CHANGE_TYPE_MODIFY:
			ModifyInstrumentClass(pData->da210);
			break;

		case CHANGE_TYPE_ADD:
			AddInstrumentClass(pData->da210);
			break;

		default:
			IseTrace(enWarning, "Unknown change type for BI210 = %d.", pData->chg_type_n);
			break;
	}

	m_ISE.UpdateInstrumentClasses(this);
};

//------------------------------[ BBO ]---------------------------------

void CISESession::OnBroadcastBO101(const best_bid_offer_bo101_t* pData, const uint16 uiLen)
{
	ModifySeriesBBO(*pData);
}

void CISESession::OnBroadcastBO102(const best_bid_offer_extended_bo102_t* pData, const uint16 uiLen)
{
	ModifySeriesExBBO(*pData);
};

void CISESession::OnBroadcastBO105(const underlying_best_bid_offer_bo105_t* pData, const uint16 uiLen)
{
	ModifyUnderlyingBBO(*pData);
};

//------------------------------[ Trades ]---------------------------------

void CISESession::OnBroadcastBD5(const directed_trade_t* pData, const uint16 uiLen)
{
//	The Dedicated Trade Information Transaction is sent to the Trader in real time (or to one
//place for each member – system dependent). As soon as a deal has been controlled and
//accepted without errors, the Deal Capture System will send this dedicated Trade Information
//as a confirmation of each trade. Note that the trade has not been guaranteed by an OCC.

	CISETrade Trade;

	{
		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(pData->cl_trade_api.series);
		if(!pSeries || !pSeries->m_pInstrumentClass)
			return;

		Trade.m_sSeries = pSeries->m_sSymbol.c_str();
		Trade.m_lTradeNumber = pData->cl_trade_api.trade_number_i;
		Trade.m_Type = (TradeTypeEnum)pData->cl_trade_api.trade_type_c;

		if(TS_ACTIVE == pData->cl_trade_api.trade_state_c)
		{
			Trade.m_bIsNew = true;
			Trade.m_lOldTradeNumber = 0;
			Trade.m_OldType = enTTUnknown;
			Trade.m_sOldSeries = "";
		}
		else
		{
			CSeries* pOldSeries = m_ISE.m_Series.FindBySeries(*(series_t*)&pData->cl_trade_api.orig_series);
			if(!pSeries)
			{
				return;
			}

			Trade.m_bIsNew = false;
			Trade.m_lOldTradeNumber = pData->cl_trade_api.orig_trade_number_i;
			Trade.m_OldType = (TradeTypeEnum)pData->cl_trade_api.orig_trade_type_c;
			Trade.m_sOldSeries = pOldSeries->m_sSymbol.c_str();
		}

		Trade.m_dPrice = DEC2DBL(pData->cl_trade_api.deal_price_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);;
		Trade.m_lQuantity = pData->cl_trade_api.trade_quantity_i;
		SDT2OD(pData->cl_trade_api.modified_date_s, pData->cl_trade_api.modified_time_s, Trade.m_dtDateTime);
		QW2S(pData->cl_trade_api.order_number_u, Trade.m_sOrderID);
		Trade.m_bIsBought = pData->cl_trade_api.bought_or_sold_c == BS_BOUGHT;
	}

	OnTrade(Trade);
};

/*-------------------------[ Deal ]--------------------------*/

void CISESession::OnBroadcastBD111(const deal_user_bd111_t* pData, const uint16 uiLen)
{
//The Deal In The Market broadcast is sent for each deal in the Marketplace. This broadcast is
//sent immediately after a deal and may be used by the customers to check if they have
//participated in the deal.

	vector<CISEDeal>	vecDeals;

	{
		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	UndLock(&m_ISE.m_Underlyings);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(pData->series);
		if(!pSeries || !pSeries->m_pInstrumentClass)
			return;

		for(uint8_t nItem = 0; nItem < pData->items_c; nItem++)
		{
			CISEDeal Deal;

			pSeries->m_dLastPrice = Deal.m_dPrice = 
				DEC2DBL(pData->deal_price_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);

			FireBBO(pSeries);

			Deal.m_sSeries = pSeries->m_sSymbol;

			const deal_user_bd111_item_t& Item = pData->item[nItem];
			Deal.m_lQuantity = Item.quantity_u;
			QW2S(Item.order_number_u, Deal.m_sOrderID);
			Deal.m_BidOrAsk = (BidAskEnum)Item.bid_or_ask_c;

			vecDeals.push_back(Deal);
		}
	}

	for(vector<CISEDeal>::iterator it = vecDeals.begin(); it != vecDeals.end(); it++)
	{
		OnDeal(*it);
	}
};

//--------------------------------[ Quotes changes ]----------------------------------

void CISESession::OnBroadcastBO108(const quote_rejection_reason_bo108_t* pData, const uint16 uiLen)
{
	string sReason = GetTextError(this, pData->reason_quote_rejected_u);
	
	CQuoteParams Quote;

	{
		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(pData->series);

		if(!pSeries || !pSeries->m_pInstrumentClass)
			return;

		Quote.m_BidOrAsk = (BidAskEnum)pData->bid_or_ask_c;
		Quote.m_dPremium = DEC2DBL(pData->premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
		Quote.m_lOpenQuantity = pData->open_quantity_n;
		Quote.m_lQuantity = pData->quantity_n;
		if(pSeries->IsCached(Quote.m_BidOrAsk))
			Quote.m_lStepUpBuffer = (BidAskEnum)Quote.m_BidOrAsk == enBid ? 
				pSeries->m_lBidStepUpBufferCache : pSeries->m_lAskStepUpBufferCache;
		else
			Quote.m_lStepUpBuffer = 0;
		Quote.m_sSeries = pSeries->m_sSymbol.c_str();

		pSeries->UpdateQuoteCache(Quote);
	}

	OnQuoteRejection(Quote, sReason);
}

void CISESession::OnBroadcastBO109(const quote_stepped_up_bo109_t* pData, const uint16 uiLen)
{
	CQuoteParams Quote;

	{
		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(pData->series);

		if(!pSeries || !pSeries->m_pInstrumentClass)
			return;
		
		Quote.m_BidOrAsk = (BidAskEnum)pData->bid_or_ask_c;
		Quote.m_lStepUpBuffer = pData->step_up_buffer_n;
		Quote.m_lQuantity = pData->quantity_u;
		Quote.m_dPremium = DEC2DBL(pData->premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
		
		if(pSeries->IsCached(Quote.m_BidOrAsk))
			Quote.m_lOpenQuantity = (BidAskEnum)Quote.m_BidOrAsk == enBid ? 
				pSeries->m_lBidOpenQuantityCache : pSeries->m_lAskOpenQuantityCache;
		else
			Quote.m_lOpenQuantity = 0;

		Quote.m_sSeries = pSeries->m_sSymbol.c_str();

		pSeries->UpdateQuoteCache(Quote);
	}

	OnQuoteStepUp(Quote);
};

char* GetTextReason(const long lReason)
{
	switch(lReason)
	{
		case 1: return "Tick-Worse, All quantity traded.";break;
		case 2: return "Tick-Worse, Tick-Worse, Refused to trade with FARMM or Firm.";break;
		case 3: return "Tick-Worse, Too small quantity at BBO.";break;
		case 4: return "Exposure Limit Curtailment.";break;
		default: return "Tick-Worse, Unknown reason.";break;
	}
}

void CISESession::OnBroadcastBO110(const quote_tick_worse_bo110_t* pData, const uint16 uiLen)
{
	CQuoteParams Quote;

	{
		CAutoLock	ClassesLock(&m_ISE.m_InstrumentClasses);
		CAutoLock	SeriesLock(&m_ISE.m_Series);

		CSeries* pSeries = m_ISE.m_Series.FindBySeries(pData->series);

		if(!pSeries || !pSeries->m_pInstrumentClass)
			return;

		Quote.m_BidOrAsk = (BidAskEnum)pData->bid_or_ask_c;
		Quote.m_lStepUpBuffer = pData->step_up_buffer_n;
		Quote.m_lQuantity = pData->quantity_u;
		Quote.m_dPremium = DEC2DBL(pData->premium_i, pSeries->m_pInstrumentClass->m_uiDecInPremium);
		
		if(pSeries->IsCached(Quote.m_BidOrAsk))
			Quote.m_lOpenQuantity = (BidAskEnum)Quote.m_BidOrAsk == enBid ? 
				pSeries->m_lBidOpenQuantityCache : pSeries->m_lAskOpenQuantityCache;
		else
			Quote.m_lOpenQuantity = 0;

		Quote.m_sSeries = pSeries->m_sSymbol.c_str();

		pSeries->UpdateQuoteCache(Quote, pData->tick_worse_level_n);
	}

	OnQuoteTickWorse(Quote, pData->tick_worse_level_n, GetTextReason(pData->reason_tick_worse_c));
}