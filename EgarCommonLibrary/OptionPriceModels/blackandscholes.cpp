//=============================================================================
// Copyright 2002, Eugene Karnygin
//
// Author	: Eugene Karnygin
// File		: BlackAndScholes.cpp
// Created	: 25 june 2002
// 
// The Black and Scholes model of theoretical option price
//=============================================================================

//=============================================================================
// Standart headers including
//=============================================================================
#include "Common.h"

namespace OPM
{

double BlackAndScholes(	double	dRateDomestic,
						double	dRateForeign,
						double	dHTBRate,
						double	dSpotPrice,
						double	dStrike,
						double  dYte,
						double	dVolatility,
						bool	bIsCall,
						double* pdDivAmnt,						   
						double* pdDivYte,						    
						int		nDivCount,
						GREEKS*	pGreeks/*out*/)
{
	if (dYte < 0.) 
		return max(bIsCall ? dSpotPrice - dStrike : dStrike - dSpotPrice, 0.);

	double dContRd = RateDiscToCont(dRateDomestic , dYte * cdDaysPerYear365);
	double dContRf = RateDiscToCont(dRateForeign, dYte * cdDaysPerYear365);
	//double dContRb = IsBadValue(dHTBRate) ? 0 : (dContRd - RateDiscToCont(dHTBRate, nDte)); //actual stock borrowing rate (b in spec)
	double dContRb = IsBadValue(dHTBRate) ? 0 : (RateDiscToCont(dRateDomestic - dHTBRate, dYte * cdDaysPerYear365)); //actual stock borrowing rate (b in spec)
	
	double dDiscountedSpot = DiscountForDividends(dSpotPrice,
												dContRd,
												dContRb,
												pdDivAmnt,
												pdDivYte,
												nDivCount,
												dYte);
	if(IsBadValue(dDiscountedSpot))
		return BadDoubleValue;

	const double A = dDiscountedSpot * (1. / exp( (dContRf + dContRb) * dYte));
	const double B = dStrike * (1. / exp( dContRd * dYte));

	const double D = dVolatility * sqrt(dYte);

	const double d1	= log(dDiscountedSpot/dStrike) / D + D * (((dContRd - (dContRf + dContRb)) / (dVolatility * dVolatility)) + .5);
	const double d2	= d1 - D;

	const double N1	= NormalC(d1);
	const double N2	= NormalC(d2);

	long cCalculatedGreeksMask = GT_NOTHING;

	double dTheoPrice = bIsCall ? A * N1 - B * N2 : A * ( N1 - 1.) - B * ( N2 - 1.);
	
	if(!IsValidTheoPrice(dTheoPrice))
	{
		dTheoPrice = BadDoubleValue;
	}
	else
	{
		cCalculatedGreeksMask |= GT_THEOPRICE;
	}

	if(pGreeks && pGreeks->nMask & ~GT_THEOPRICE && !IsBadValue(dTheoPrice))
	{
        const double	dd1dS = 1. / (D * dDiscountedSpot);
        const double	dd1dT = -d2 / (2. * dYte) + (dContRd - (dContRf + dContRb)) / D;
        const double	dd2dT = -d1 / (2. * dYte) + (dContRd - (dContRf + dContRb)) / D;
        const double	dd1dVol = - d2 / dVolatility;
        const double	dd2dVol = - d1 / dVolatility;
        const double	dd12dR = dYte / D;
		const double	d2d1dVol2 = 2 * log(A/B) / (D * dVolatility * dVolatility);
        const double    n1 = NormalDensity(d1);
        const double    n2 = NormalDensity(d2);
		

    	// DELTA
	    if (pGreeks->nMask & GT_DELTA)
	    {
            pGreeks->dDelta = exp(-(dContRf + dContRb) * dYte) * (N1 - (bIsCall ? 0. : 1.));
		    cCalculatedGreeksMask |= GT_DELTA;
	    }

    	// GAMMA
	    if (pGreeks->nMask & GT_GAMMA)
	    {
		    pGreeks->dGamma = exp(-(dContRf + dContRb) * dYte) * dd1dS * n1;
		    cCalculatedGreeksMask |= GT_GAMMA;
	    }

	    // THETA
	    if (pGreeks->nMask & GT_THETA)
	    {
		    pGreeks->dTheta = ( A * ((dContRf + dContRb) * (N1 - (bIsCall ? 0. : 1.)) - dd1dT * n1) -
                B * (dContRd * (N2 - (bIsCall ? 0. : 1.)) - dd2dT * n2) ) * cdDeltaTime;            
		    cCalculatedGreeksMask |= GT_THETA;
	    }

	    // DELTA THETA
	    if (pGreeks->nMask & GT_DELTA_THETA)
	    {
		    pGreeks->dDeltaTheta = ( (dContRf + dContRb) * (pGreeks->dDelta - (bIsCall ? 0. : exp(-(dContRf + dContRb) * dYte) )) - 
				pGreeks->dGamma * dd1dT / dd1dS ) * cdDeltaTime;
		    cCalculatedGreeksMask |= GT_DELTA_THETA;
	    }

	    // GAMMA THETA
	    if (pGreeks->nMask & GT_GAMMA_THETA)
	    {
		    pGreeks->dGammaTheta = ( pGreeks->dGamma * ((dContRf + dContRb) + 1. / (2. * dYte) + d1 * dd1dT) ) * cdDeltaTime;
		    cCalculatedGreeksMask |= GT_GAMMA_THETA;
	    }

        // DELTA VEGA
        if (pGreeks->nMask & GT_DELTA_VEGA)
        {
	        pGreeks->dDeltaVega = ( pGreeks->dGamma * dd1dVol / dd1dS ) * cdDeltaVolatility;
	        cCalculatedGreeksMask |= GT_DELTA_VEGA;
        }

        // GAMMA VEGA
        if (pGreeks->nMask & GT_GAMMA_VEGA)
        {
	        pGreeks->dGammaVega = ( -pGreeks->dGamma * ( 1. / dVolatility + d1 * dd1dVol) ) * cdDeltaVolatility;
	        cCalculatedGreeksMask |= GT_GAMMA_VEGA;
        }

        // VEGA
        if (pGreeks->nMask & GT_VEGA)
        {
	        pGreeks->dVega = ( A * dd1dVol * n1 - B * dd2dVol * n2 ) * cdDeltaVolatility;
	        cCalculatedGreeksMask |= GT_VEGA;
        }

        // RHO
        if (pGreeks->nMask & GT_RHO)
   	    {
	        pGreeks->dRho = ( (A * n1 - B * n2) * dd12dR + dYte * B * (N2 - (bIsCall ? 0. : 1.)) ) * cdDeltaRate;
	        cCalculatedGreeksMask |= GT_RHO;
        }

		// VOLGA
		if (pGreeks->nMask & GT_VOLGA)
		{
			pGreeks->dVolga = (A * n1 * (-d1 * (dd1dVol * dd1dVol) + d2d1dVol2) - 
							   B * n2 * (-d2 * (dd2dVol * dd2dVol) + d2d1dVol2)) * cdDeltaVolatility * cdDeltaVolatility;
			cCalculatedGreeksMask |= GT_VOLGA;
		}

		// Check values
		if (cCalculatedGreeksMask & GT_DELTA)
			pGreeks->dDelta = min(max(pGreeks->dDelta, -1.), 1.);
				
		if (cCalculatedGreeksMask & GT_GAMMA)
			pGreeks->dGamma = max(pGreeks->dGamma, 0.);

		if (cCalculatedGreeksMask & GT_VEGA)
			pGreeks->dVega = max(pGreeks->dVega, 0.);

		if (cCalculatedGreeksMask & GT_THETA)
			pGreeks->dTheta = min(pGreeks->dTheta, 0.);

		// ALPHA
		if ((pGreeks->nMask & GT_ALPHA) && (ValueNEQZero(pGreeks->dTheta)))
		{
			pGreeks->dAlpha = pGreeks->dGamma / pGreeks->dTheta;
			cCalculatedGreeksMask |= GT_ALPHA;
		}
	}
	// Set up greeks mask
	if(pGreeks)
		pGreeks->nMask = cCalculatedGreeksMask;

	return  dTheoPrice;
}

}
