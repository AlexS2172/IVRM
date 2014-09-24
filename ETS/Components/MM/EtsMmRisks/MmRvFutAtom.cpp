// MmRvFutAtom.cpp : Implementation of CMmRvFutAtom

#include "stdafx.h"
#include "MmRvFutAtom.h"


// CMmRvFutAtom

//----------------------------------------------------------------------------------------------//
STDMETHODIMP CMmRvFutAtom::UpdateQuotes()
{
	try
	{
		if (m_spContract != NULL)
		{
			QuoteUpdateInfo retQuotes;
			_CHK(m_spContract->raw_GetQuotes(&retQuotes), _T("Fail to get contract quotes."));

			if (retQuotes.BidPrice > BAD_DOUBLE_VALUE)
				m_pPrice->put_Bid(retQuotes.BidPrice);

			if (retQuotes.AskPrice > BAD_DOUBLE_VALUE)
				m_pPrice->put_Ask(retQuotes.AskPrice);

			if (retQuotes.LastPrice > BAD_DOUBLE_VALUE)
				m_pPrice->put_Last(retQuotes.LastPrice);

			if (retQuotes.NetChange > BAD_DOUBLE_VALUE)
				m_pPrice->put_NetChange(retQuotes.NetChange);

			SysFreeString(retQuotes.BidExchange);
			SysFreeString(retQuotes.AskExchange);
			SysFreeString(retQuotes.Currency);
			SysFreeString(retQuotes.Exchange);
		}
	}
	catch(_com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosAtom, e.Error());
	}

	return S_OK;
};
//----------------------------------------------------------------------------------------------//

STDMETHODIMP CMmRvFutAtom::get_Underlying(IMmRvUndAtom **pVal)
{
	if(!pVal)	return E_POINTER;	

	if(*pVal) {
		(*pVal)->Release();
		*pVal = NULL;
	}

	ObjectLock lock(this);

	*pVal = m_spUnd;

	if(*pVal)	(*pVal)->AddRef();

	return S_OK;		
}


STDMETHODIMP CMmRvFutAtom::putref_Underlying(IMmRvUndAtom* NewVal)
{	
	ObjectLock lock(this);
	m_spUnd = NewVal;
	return S_OK;
}	

/*
This function returns future "mid" price, according to active future or price profile.
Sets bFutureUsed to VARIANT_TRUE if active future was used.
*/
STDMETHODIMP CMmRvFutAtom::GetFuturePrice( DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound, EtsReplacePriceStatusEnum * penPriceStatus , VARIANT_BOOL *bFutureUsed, DOUBLE *pPrice){
	IEtsPriceProfileAtomPtr	spActiveFuturePriceProfile;
	HRESULT hr = S_OK;
	if (!pPrice || !bFutureUsed)	return E_POINTER;

	*pPrice = BAD_DOUBLE_VALUE;
	*bFutureUsed = VARIANT_FALSE;
	IMmRvFutAtomPtr	spActiveFuture;

	if (m_spUnd )
		_CHK(m_spUnd->get_ActiveFuture(&spActiveFuture));

	try
	{
		if ( spActiveFuture )	
		{
			double priceBid, priceAsk, priceLast;
			double	activeFutureClosePrice = 0., activeFutureCurrentPrice = 0.;

			IMmRvPricePtr spPrice;
			_CHK(spActiveFuture->get_Price(&spPrice));

			_CHK(spActiveFuture->get_UndPriceProfile(&spActiveFuturePriceProfile));
			_CHK(spPrice->get_Ask(&priceAsk));
			_CHK(spPrice->get_Bid(&priceBid));
			_CHK(spPrice->get_Last(&priceLast));

			VARIANT_BOOL	bCalcByManual = VARIANT_FALSE;
			spPrice->get_IsUseManualActive(&bCalcByManual);

			if (bCalcByManual == VARIANT_TRUE)
				spPrice->get_Active(&activeFutureCurrentPrice);
			else
				activeFutureCurrentPrice = spActiveFuturePriceProfile->GetUndPriceMid(	priceBid, priceAsk,	priceLast, dTolerance, 
																						enPriceRound, NULL,  VARIANT_FALSE );

			if (activeFutureCurrentPrice > 0)
			{
				// we have price of active future
				if ( m_dRatio <= 0.)
				{
					_CHK(spPrice->get_Close(&activeFutureClosePrice));
					// we have no ratio, try to compute it on the fly
					if ( m_pPrice && m_pPrice->m_dPriceClose > 0. && activeFutureClosePrice > 0)
					{
						m_dRatio = m_pPrice->m_dPriceClose / activeFutureClosePrice;
						*pPrice = activeFutureCurrentPrice * m_dRatio;
					}
					else
						*pPrice = activeFutureCurrentPrice;	// couldn't compute ratio
				}
				else
					*pPrice = activeFutureCurrentPrice * m_dRatio;

				m_pPrice->m_dActivePrice = *pPrice;
				*bFutureUsed = VARIANT_TRUE;
			}
			else  //no active future price (market and manual) return 0.0
			{
				*pPrice = 0.;
				m_pPrice->m_dActivePrice = *pPrice;
				*bFutureUsed = VARIANT_FALSE;
			}
		}
		else if (m_spUndPriceProfile)
		{
			if (m_pPrice->m_bManualActive == VARIANT_FALSE)
			{
				*pPrice = m_spUndPriceProfile->GetUndPriceMid(	m_pPrice->m_dPriceBid, m_pPrice->m_dPriceAsk, m_pPrice->m_dPriceLast,
																dTolerance,	enPriceRound, penPriceStatus, VARIANT_FALSE );

				m_pPrice->m_dActivePrice = *pPrice;
			}
			else{
				*pPrice = m_pPrice->m_dActivePrice;
			}
		}
	}
	catch (_com_error& e) {
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmRvFutAtom), e.Error());
	}
	return hr;
}