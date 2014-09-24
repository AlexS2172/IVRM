//=============================================================================
//
// Author	: Michael Malyshkin
// File		: FutureGreeks.cpp
// Created	: 3 nov 2003
// 
// Future options "spot" greeks from "forward" greeks
//=============================================================================

#include "Common.h"

namespace OPM
{

bool SpotGreeksFromForward(
				double	dRateDomestic,	// Domestic continuos risk free rate
				double	dYte,			// Days till expiration
				GREEKS*	pGreeks			// in, out params
				)				//returns true if success
{
	if(!pGreeks)
		return false;

	double	dContRd	= RateDiscToCont(dRateDomestic, dYte * cdDaysPerYear365);

	const double dExpRT = exp(dContRd * dYte);
	const double dExp2RT = exp(2 * dContRd * dYte);

	if(pGreeks->nMask & GT_DELTA)
		pGreeks->dDelta *= dExpRT;

	if(pGreeks->nMask & GT_GAMMA)
		pGreeks->dGamma *= dExp2RT;

	if(pGreeks->nMask & GT_ALPHA)
		pGreeks->dAlpha *= dExp2RT;

	if(pGreeks->nMask & GT_DELTA_THETA)
		pGreeks->dDeltaTheta *= dExpRT;

	if(pGreeks->nMask & GT_GAMMA_THETA)
		pGreeks->dGammaTheta *= dExp2RT;

	if(pGreeks->nMask & GT_DELTA_VEGA)
		pGreeks->dDeltaVega *= dExpRT;

	if(pGreeks->nMask & GT_GAMMA_VEGA)
		pGreeks->dGammaVega *= dExp2RT;

	return true;
}

bool SpotGreeksFromForward2(
							double	dSpotPrice,
							double	dRateDomestic,	// Domestic continuos risk free rate
							double	dYield,			// Domestic continuos risk free rate
							double	dYte,			// Days till expiration
							double*	pDivAmnts,		// Dividend amounts
							double*	pDivYears,		// Dividend dates
							long	nCount,			// Dividend quantity
							GREEKS*	pGreeks			// in, out params
							)						//returns true if success
{
	// SpotGreeksFromForward version with index dividend yield and with discrete dividends (for baskets)

	if(!pGreeks)
		return false;

	double dExpRT = 0;
	double dExp2RT = 0;
	double	dContRd	= RateDiscToCont(dRateDomestic, dYte * cdDaysPerYear365);

	/// legacy ones
	//const double dExpRT = exp(dContRd * dYte);
	//const double dExp2RT = exp(2 * dContRd * dYte);
	/// legacy ones

	if ((nCount > 0)&&((pDivAmnts != NULL)&&(pDivYears != NULL)))
	{
		// now with dividend (market/custom periodical or custom stream...)
		if (dSpotPrice == 0)
			return false;
		double dNPV = 0;
		dNPV = DividendNPV(dSpotPrice, dContRd, 0, pDivAmnts, pDivYears, nCount, dYte);
		dExpRT = exp(dContRd*dYte)*(1 - dNPV/dSpotPrice);
		dExp2RT = exp(2*dContRd*dYte)*(1 - dNPV/dSpotPrice);

	}
	else
	{
		// now with index dividend Yield (dYield)
		dExpRT = exp((dContRd - dYield)*dYte);
		dExp2RT = exp(2*(dContRd - dYield)*dYte);
	}


	if(pGreeks->nMask & GT_DELTA)
		pGreeks->dDelta *= dExpRT;

	if(pGreeks->nMask & GT_GAMMA)
		pGreeks->dGamma *= dExp2RT;

	if(pGreeks->nMask & GT_ALPHA)
		pGreeks->dAlpha *= dExp2RT;

	if(pGreeks->nMask & GT_DELTA_THETA)
		pGreeks->dDeltaTheta *= dExpRT;

	if(pGreeks->nMask & GT_GAMMA_THETA)
		pGreeks->dGammaTheta *= dExp2RT;

	if(pGreeks->nMask & GT_DELTA_VEGA)
		pGreeks->dDeltaVega *= dExpRT;

	if(pGreeks->nMask & GT_GAMMA_VEGA)
		pGreeks->dGammaVega *= dExp2RT;

	return true;
}


}
