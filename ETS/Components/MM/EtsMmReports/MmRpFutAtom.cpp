// MmRpFutAtom.cpp : Implementation of CMmRpFutAtom

#include "stdafx.h"
#include "MmRpFutAtom.h"


// CMmRpFutAtom

STDMETHODIMP CMmRpFutAtom::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMmRpFutAtom
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CMmRpFutAtom::GetFuturePrice(DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,EtsReplacePriceStatusEnum * penPriceStatus, VARIANT_BOOL *bFutureUsed, DOUBLE *pPrice){
	IEtsPriceProfileAtomPtr	spActiveFuturePriceProfile;
	HRESULT hr = S_OK;

	if (!pPrice || !bFutureUsed)	return E_POINTER;

	*pPrice = BAD_DOUBLE_VALUE;
	*bFutureUsed = VARIANT_FALSE;
	double activeFutureBidPrice, activeFutureAskPrice, activeFutureLastPrice, activeFutureClosePrice;

	try{
		bool	useThis = true;

		if ( m_spActiveFuture )	{
			double	activeFutureCurrentPrice = 0.;
			IMMRpPricePtr	activeFuturePrice;

			_CHK(m_spActiveFuture->get_Price(&activeFuturePrice));
			_CHK(m_spActiveFuture->get_UndPriceProfile(&spActiveFuturePriceProfile));
			if (activeFuturePrice && spActiveFuturePriceProfile){
			
				_CHK(activeFuturePrice->get_Ask(&activeFutureAskPrice));
				_CHK(activeFuturePrice->get_Bid(&activeFutureBidPrice));
				_CHK(activeFuturePrice->get_Last(&activeFutureLastPrice));
				_CHK(activeFuturePrice->get_Close(&activeFutureClosePrice));
				activeFutureCurrentPrice = spActiveFuturePriceProfile->GetUndPriceMid(	activeFutureBidPrice,
																												activeFutureAskPrice,
																												activeFutureLastPrice,
																												dTolerance,
																												enPriceRound,
																												NULL, VARIANT_FALSE );
				if (activeFutureCurrentPrice > 0) {
					// we have price of active future
					if ( m_dRatio <= 0.&& m_spPrice){
						double	futureClosePrice = 0.;
						_CHK(m_spPrice->get_Close(&futureClosePrice));
						// we have no ratio, try to compute it on the fly
						if ( futureClosePrice > 0. && activeFutureClosePrice > 0)	
							m_dRatio = futureClosePrice / activeFutureClosePrice;
						else
							m_dRatio = 1.;	// couldn't compute ratio
					}
					*pPrice = activeFutureCurrentPrice * m_dRatio;
					useThis = false;
					*bFutureUsed = VARIANT_TRUE;
				}
			}
		}
		// if we were unable to compute future price based on price of active future
		// do it using this future quote 
		if( useThis && m_spUndPriceProfile && m_spPrice) {
			double	futureBidPrice = 0., futureAskPrice = 0., futureLastPrice = 0.;

			_CHK(m_spPrice->get_Ask(&futureAskPrice));
			_CHK(m_spPrice->get_Bid(&futureBidPrice));
			_CHK(m_spPrice->get_Last(&futureLastPrice));
			*pPrice = m_spUndPriceProfile->GetUndPriceMid(	futureBidPrice, futureAskPrice, 
																			futureLastPrice, dTolerance,
																			enPriceRound, penPriceStatus, VARIANT_FALSE );
		}
	}
	catch (_com_error& e) {
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmRpFutAtom), e.Error());
	}
	return hr;
}