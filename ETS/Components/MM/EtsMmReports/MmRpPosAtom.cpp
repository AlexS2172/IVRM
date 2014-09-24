// MmRpPosAtom.cpp : Implementation of CMmRpPosAtom

#include "stdafx.h"
#include "MmRpPosAtom.h"


// CMmRpPosAtom

STDMETHODIMP CMmRpPosAtom::Clear()
{
	return __MmRpPosAtom::Clear();
}

STDMETHODIMP CMmRpPosAtom::Add(IMmTradeInfoAtom* Trade)
{
	__CHECK_POINTER(Trade);

	try
	{
		IMmTradeInfoAtomPtr	spTrade(Trade);

		LONG			nTradeDate = (LONG)spTrade->TradeDate;		
		LONG			nToday = (LONG)(DATE)COleDateTime::GetCurrentTime();
		LONG			nLotSize = spTrade->LotSize;		
		LONG			nQty = spTrade->Quantity;
		DOUBLE			dPrice = spTrade->Price;
		DOUBLE			dPriceClose = 0.;

		EtsContractTypeEnum enContractType = spTrade->ContractType;
		switch(enContractType)
		{
			case enCtIndex:
			case enCtStock:
				if(spTrade->Und != NULL)
					dPriceClose = spTrade->Und->PriceClose;
				break;

			case enCtOption:
				if(spTrade->Opt != NULL)
				{
					dPriceClose = spTrade->Opt->PriceClose;
				}
				break;

			case enCtFuture:
				if(spTrade->Fut != NULL)
					dPriceClose = spTrade->Fut->PriceClose;
				break;

			case enCtFutOption:
				if(spTrade->FutOpt != NULL)
				{
					dPriceClose = spTrade->FutOpt->PriceClose;
				}
				break;
		}

		VARIANT_BOOL	bIsBuy = spTrade->IsBuy;
		VARIANT_BOOL	bIsPosition = spTrade->IsPosition;

		DOUBLE*			pdQty = NULL;
		DOUBLE*			pdQtyInShares = NULL;
		DOUBLE*			pdPos = NULL;

#ifdef _DEBUG
		_bstr_t bsSym = spTrade->Symbol;
#endif

		if(bIsBuy == VARIANT_TRUE)
		{
			pdQty = &m_dQtyLTDBuy;
			pdQtyInShares = &m_dQtyInSharesLTDBuy;
			pdPos = &m_dPosLTDBuy;
		}
		else
		{
			pdQty = &m_dQtyLTDSell;
			pdQtyInShares = &m_dQtyInSharesLTDSell;
			pdPos = &m_dPosLTDSell;
		}

		*pdQty += nQty;
		*pdQtyInShares += nQty * nLotSize;

		if(bIsPosition == VARIANT_TRUE)
		{
			if(/*DoubleGTZero(dPriceClose)*/dPriceClose >= 0.0)
				*pdPos += dPriceClose * nQty * nLotSize;
		}
		else
		{
			if(/*DoubleGTZero(dPrice)*/dPrice >= 0.0)
			{
				*pdPos += dPrice * nQty * nLotSize;
			}
		}

		if(nTradeDate < nToday)
		{
			if(bIsBuy == VARIANT_TRUE)
			{
				pdQty = &m_dQtyDailyPrevDateBuy;
				pdQtyInShares = &m_dQtyInSharesDailyPrevDateBuy;
				pdPos = &m_dPosDailyPrevDateBuy;
			}
			else
			{
				pdQty = &m_dQtyDailyPrevDateSell;
				pdQtyInShares = &m_dQtyInSharesDailyPrevDateSell;
				pdPos = &m_dPosDailyPrevDateSell;
			}

			if(/*DoubleGTZero(dPriceClose)*/dPriceClose >= 0.0)
			{
				*pdPos += dPriceClose * nQty * nLotSize;
			}
			else if(bIsPosition == VARIANT_FALSE)
			{
				*pdPos += dPrice * nQty * nLotSize;
			}
		}
		else
		{
			if(bIsBuy == VARIANT_TRUE)
			{
				pdQty = &m_dQtyDailyTodayBuy;
				pdQtyInShares = &m_dQtyInSharesDailyTodayBuy;
				pdPos = &m_dPosDailyTodayBuy;
			}
			else
			{
				pdQty = &m_dQtyDailyTodaySell;
				pdQtyInShares = &m_dQtyInSharesDailyTodaySell;
				pdPos = &m_dPosDailyTodaySell;
			}

			if(bIsPosition == VARIANT_FALSE)
			{
				*pdPos += dPrice * nQty * nLotSize;
			}
			else
			{
				if(/*DoubleGTZero(dPriceClose)*/dPriceClose >= 0.0)
					*pdPos += dPriceClose * nQty * nLotSize;
			}
		}

		*pdQty += nQty;
		*pdQtyInShares += nQty * nLotSize;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPosAtom, e.Error());
	}

	return S_OK;
}