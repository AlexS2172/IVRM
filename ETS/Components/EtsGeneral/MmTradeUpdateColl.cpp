// MmTradeUpdateColl.cpp : Implementation of CMmTradeUpdateColl

#include "stdafx.h"
#include "MmTradeUpdateColl.h"


// CMmTradeUpdateColl

//STDMETHODIMP CMmTradeUpdateColl::InterfaceSupportsErrorInfo(REFIID riid)
//{
//	static const IID* arr[] = 
//	{
//		&IID_IMmTradeUpdateColl
//	};
//
//	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
//	{
//		if (InlineIsEqualGUID(*arr[i],riid))
//			return S_OK;
//	}
//	return S_FALSE;
//}

STDMETHODIMP CMmTradeUpdateColl::Add(LONG Key, ITradeUpdate* Value, ITradeUpdate** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Trade with the same key already exists."));
		}

		if(!Value)
		{
			ITradeUpdatePtr	spNewVal;
			__CHECK_HRESULT(spNewVal.CreateInstance(L"MsgStruct.TradeUpdate"), _T("Failed to add trade."));
			if(FAILED(IMmTradeUpdateCollImpl::Add(Key, spNewVal)))
			{
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add trade."));
			}
			*pRetVal = spNewVal.Detach();

			// copy trade data
			CopyTrade(Value, *pRetVal);
		}
		else
		{
			__CHECK_HRESULT(IMmTradeUpdateCollImpl::Add(Key, Value), _T("Failed to add trade."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeUpdateColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmTradeUpdateColl::Clone(IMmTradeUpdateColl** ppDstColl)
{
	__CHECK_POINTER(ppDstColl);
	if(*ppDstColl != NULL)
		return E_INVALIDARG;

	try
	{
		IMmTradeUpdateCollPtr spDstColl;
		_CHK(spDstColl.CreateInstance(CLSID_MmTradeUpdateColl));

		CollType::const_iterator itr_b = m_coll.begin();
		CollType::const_iterator itr_e = m_coll.end();

		for(CollType::const_iterator itr = itr_b; itr != itr_e; ++itr)
		{	
			LONG lSeqNum = 0;
			(itr->second)->get_SeqNum(&lSeqNum);

			// add new trade
			ITradeUpdatePtr	spDstTrade;
			if(FAILED(spDstColl->Add(lSeqNum, NULL, &spDstTrade)))
				continue;
			if(spDstTrade == NULL)
				continue;

			// copy trade data
			CopyTrade(itr->second, spDstTrade);
		}

		*ppDstColl = spDstColl.Detach();

	}
	catch (const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeUpdateColl, e.Error());
	}

	return S_OK;
}

HRESULT CMmTradeUpdateColl::CopyTrade(ITradeUpdate* pSrc, ITradeUpdate* pDst)
{
	__CHECK_POINTER(pSrc);
	__CHECK_POINTER(pDst);

	LONG		lTemp = 0;
	DATE		dTemp = 0.;
	double		dblTemp = 0.;
	CComBSTR	sbsTemp;

	pSrc->get_SeqNum(&lTemp);
	pDst->put_SeqNum(lTemp);

	pSrc->get_TradeID(&lTemp);
	pDst->put_TradeID(lTemp);

	TRADE_ACTION taTemp;
	pSrc->get_Action(&taTemp);
	pDst->put_Action(taTemp);

	TRADE_STATUS tsTemp;
	pSrc->get_TradeStatus(&tsTemp);
	pDst->put_TradeStatus(tsTemp);

	pSrc->get_Mark(&lTemp);
	pDst->put_Mark(lTemp);

	pSrc->get_TradeDate(&dTemp);
	pDst->put_TradeDate(dTemp);

	pSrc->get_TraderID(&lTemp);
	pDst->put_TraderID(lTemp);

	pSrc->get_SymbolID(&lTemp);
	pDst->put_SymbolID(lTemp);

	pSrc->get_Quantity(&lTemp);
	pDst->put_Quantity(lTemp);

	pSrc->get_Price(&dblTemp);
	pDst->put_Price(dblTemp);

	pSrc->get_IsBuy(&lTemp);
	pDst->put_IsBuy(lTemp);

	pSrc->get_StrategyID(&lTemp);
	pDst->put_StrategyID(lTemp);

	pSrc->get_BrokerID(&lTemp);
	pDst->put_BrokerID(lTemp);

	pSrc->get_BrokerCommission(&dblTemp);
	pDst->put_BrokerCommission(dblTemp);

	pSrc->get_ClearingBrokerID(&lTemp);
	pDst->put_ClearingBrokerID(lTemp);

	pSrc->get_ClearingBrokerCommission(&dblTemp);
	pDst->put_ClearingBrokerCommission(dblTemp);

	pSrc->get_SpotReference(&dblTemp);
	pDst->put_SpotReference(dblTemp);

	pSrc->get_TradedIV(&dblTemp);
	pDst->put_TradedIV(dblTemp);

	SYMBOL_TYPE stTemp;
	pSrc->get_SymbolType(&stTemp);
	pDst->put_SymbolType(stTemp);

	pSrc->get_Symbol(&sbsTemp);
	pDst->put_Symbol(sbsTemp);

	pSrc->get_UndSymbolID(&lTemp);
	pDst->put_UndSymbolID(lTemp);

	pSrc->get_UndSymbol(&sbsTemp);
	pDst->put_UndSymbol(sbsTemp);

	pSrc->get_Expiry(&dTemp);
	pDst->put_Expiry(dTemp);

	pSrc->get_ExpiryOV(&dTemp);
	pDst->put_ExpiryOV(dTemp);

	pSrc->get_TradingClose(&dTemp);
	pDst->put_TradingClose(dTemp);

	pSrc->get_IsCall(&lTemp);
	pDst->put_IsCall(lTemp);

	pSrc->get_Strike(&dblTemp);
	pDst->put_Strike(dblTemp);

	pSrc->get_PriceClose(&dblTemp);
	pDst->put_PriceClose(dblTemp);

	pSrc->get_PriceTheoClose(&dblTemp);
	pDst->put_PriceTheoClose(dblTemp);

	pSrc->get_LotSize(&lTemp);
	pDst->put_LotSize(lTemp);

	pSrc->get_OptionRootID(&lTemp);
	pDst->put_OptionRootID(lTemp);

	pSrc->get_FutID(&lTemp);
	pDst->put_FutID(lTemp);

	pSrc->get_FutRootID(&lTemp);
	pDst->put_FutRootID(lTemp);

	TRADE_MS_STATE tmsTemp;
	pSrc->get_MsState(&tmsTemp);
	pDst->put_MsState(tmsTemp);

	pSrc->get_ImportID(&sbsTemp);
	pDst->put_ImportID(sbsTemp);

	return S_OK;
}