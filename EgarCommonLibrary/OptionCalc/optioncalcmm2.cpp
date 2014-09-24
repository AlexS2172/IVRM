/***************************************************************************************************
*	
*	(C) EGAR technology inc. 2002
*	
*	Special functions which calulates greeks and implied volatility 
*	calculation model.
*	
*	
*	Author: Eugene Karnygin (EVK)
*
***************************************************************************************************/

#include "stdafx.h"

#include "Models.h"
#include "OptionCalcMM2.h"

static double CalculateOptionMM(double	dDomesticRate,
								double	dForeignRate, 
								double	dHTBRate,
								double	dSpotPrice,
								double	dStrike,
								double	dVolatility, 
								double	dYTE,
								bool	bIsCall,
								bool	bIsAmerican,
								long	nCount, 
								double*	pDivAmnts,
								double*	pDivYears,
								long	nSteps,
								double	dSkew,
								double	dKurtosis,
								long	nModel,
								GREEKS*	pGreeks = NULL)
{
	switch (nModel)
	{
	case MM_EGAR_BS			:
        return CO_BlackScholes(dDomesticRate, dForeignRate, dHTBRate, dSpotPrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, pDivAmnts, pDivYears, nCount, pGreeks);
	case MM_EGAR_BINOMIAL	:	
        return CO_StandardBinomial(dDomesticRate, dForeignRate, dHTBRate, dSpotPrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, pDivAmnts, pDivYears, nCount, nSteps, pGreeks);
	case MM_EGAR_OPTIMIZED	:	
        return CO_OptimizedBinomial(dDomesticRate, dForeignRate, dHTBRate, dSpotPrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, pDivAmnts, pDivYears, nCount, pGreeks);
	case MM_EGAR_VSKLOG		:	
        return CO_VskLog(dDomesticRate, dForeignRate, dSpotPrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, pDivAmnts, pDivYears, nCount, dSkew, dKurtosis, pGreeks);

	default: return 0.;
	}
}

/***************************************************************************************************
	Calculates volatility. Returns -1 if error occured.
***************************************************************************************************/
double OPTIONCALC_API CalcVolatilityMM2(	double	dDomesticRate,	
											double	dForeignRate,
											double	dHTBRate,
											double	dSpotPrice,
											double	dOptionPrice,
											double	dStrike,
											double	dYTE,
											long	nIsCall,
											long	nIsAmerican,
											long	nCount, 
											double*	pDivAmnts,
											double*	pDivYears,
											long	nSteps,
											double	dSkew,
											double	dKurtosis,
											long	nModel)
{
    long nFlag = VF_OK;
    double dRet = CalcVolatilityMM3(dDomesticRate,
                                    dForeignRate,
									dHTBRate,
                                    dSpotPrice,
                                    dOptionPrice,
                                    dStrike,
                                    dYTE,
                                    nIsCall,
                                    nIsAmerican,
                                    nCount,
                                    pDivAmnts,
                                    pDivYears,
                                    nSteps,
                                    dSkew,
                                    dKurtosis,
                                    nModel,
                                    &nFlag);

    return nFlag == VF_OK ? dRet : -1;
}

/***************************************************************************************************
	Calculates volatility. Returns -1 if error occured.
***************************************************************************************************/
double OPTIONCALC_API CalcVolatilityMM3(	double	dDomesticRate,	
											double	dForeignRate, 
											double	dHTBRate,
											double	dSpotPrice,
											double	dOptionPrice,
											double	dStrike,
											double	dYTE,
											long	nIsCall,
											long	nIsAmerican,
											long	nCount, 
											double*	pDivAmnts,
											double*	pDivYears,
											long	nSteps,
											double	dSkew,
											double	dKurtosis,
											long	nModel,
                                            long*   pnFlag)
{
	// Check parameters
	if (nCount != 0)
	{
		if( ::IsBadReadPtr(pDivAmnts, sizeof(double) * nCount) || 
			::IsBadReadPtr(pDivYears, sizeof(double) * nCount))
		{
			::SetLastError(ERROR_NOACCESS);
			return -1;
		}
	}

    if (!pnFlag)
    {
		::SetLastError(ERROR_INVALID_PARAMETER);
		return -1;
    }

	double dVolaLeft	= 0.01;		// Left boundary of volatility value
	double dVolaRight	= 4.0;	// 2.5;		// Right boundary of volatility value
	double dVolaMed;
	
	double ImpVol = -1;

	const double cdOptValPrec = 0.0001;			// Precision of volatility search (Epsilon for option price)
	
	double	dOptValDelta = cdOptValPrec + 1;	// 

	int	Count = -30;			// Amount of iterations to find volatility value

	// Calculate boundary values
	double dOptValMin = CalculateOptionMM(dDomesticRate, dForeignRate, dHTBRate, dSpotPrice, dStrike, 
		dVolaLeft, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), nCount, pDivAmnts, pDivYears,
		max(nSteps, 5), dSkew, dKurtosis, nModel);	
	if(OPM::IsBadValue(dOptValMin))
		dOptValMin = 0.;

	double dOptValMax = CalculateOptionMM(dDomesticRate, dForeignRate, dHTBRate, dSpotPrice, dStrike, 
		dVolaRight, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), nCount, pDivAmnts, pDivYears,
		max(nSteps, 5), dSkew, dKurtosis, nModel);
	if(OPM::IsBadValue(dOptValMax))
		dOptValMax = 0.;
	
	double dOptValMid;
	
	if (dOptionPrice > dOptValMax)
	{
        *pnFlag = VF_TOOHIGH;
		return dVolaRight;
	}
    else if (dOptionPrice < (dOptValMin + .005))
    {
        *pnFlag = VF_TOOLOW;
        dOptionPrice = dOptValMin + .005;
    }
    else
    {
        *pnFlag = VF_OK;
    }

	// Iterate
	while (fabs(dOptValDelta) >= cdOptValPrec && Count != 0)
	{
		Count++;
	
		dVolaMed = 0.5 * (dVolaLeft + dVolaRight);	
		dOptValMid = CalculateOptionMM(dDomesticRate, dForeignRate, dHTBRate, dSpotPrice, dStrike, 
			dVolaMed, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), nCount, pDivAmnts, pDivYears,
			max(nSteps, 5), dSkew, dKurtosis, nModel);
		if(OPM::IsBadValue(dOptValMid))
			dOptValMid = 0.;

		dOptValDelta = dOptValMid - dOptionPrice; 
		
		ImpVol = dVolaMed;
		
		if( dOptValDelta > 0.0 )
			dVolaRight = dVolaMed;
		else
			dVolaLeft = dVolaMed;
	}

	if( Count == 0 || fabs(dOptValDelta) > cdOptValPrec)
	{
        *pnFlag = VF_UNRELIABLE;
	}

	return ImpVol;
}

// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
long OPTIONCALC_API CalcGreeksMM2(	double	dDomesticRate,
									double	dForeignRate, 
									double	dHTBRate,
									double	dSpotPrice,
									double	dStrike,
									double	dVolatility, 
									double	dYTE,
									long	nIsCall,
									long	nIsAmerican,
									long	nCount, 
									double*	pDivAmnts,
									double*	pDivYears,
									long	nSteps,
									double	dSkew,
									double	dKurtosis,
									long	nModel,
									GREEKS*	pGreeks) /*out*/
{
	if (dVolatility < 0. || dStrike <= 0.)
		return 0;				// Check valid volatility & strike

	// Check parameters
	if (nCount > 0)
	{
		if( ::IsBadReadPtr(pDivAmnts, sizeof(double) * nCount) || 
			::IsBadReadPtr(pDivYears, sizeof(double) * nCount))
		{
			::SetLastError(ERROR_NOACCESS);
			return 0;
		}
	}

	if(::IsBadWritePtr(pGreeks, sizeof(GREEKS)))
	{
		::SetLastError(ERROR_NOACCESS);
		return 0;
	}

	if( pGreeks->nMask == GT_NOTHING )
		return 1;	// Nothing to calculate

	long gt = pGreeks->nMask;

	if (dVolatility <= 0.0005){
		memset(pGreeks, 0, sizeof(GREEKS));
		pGreeks->nMask = gt;
		return 1;
	}



	gt |= (gt & GT_GAMMA_VEGA)	? GT_DELTA_VEGA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_GAMMA_THETA) ? GT_DELTA_THETA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_DELTA_VEGA)	? GT_DELTA | GT_VEGA | GT_GAMMA     : GT_NOTHING;
	gt |= (gt & GT_DELTA_THETA) ? GT_DELTA | GT_THETA | GT_GAMMA    : GT_NOTHING;
	gt |= (gt & GT_GAMMA)		? GT_DELTA					        : GT_NOTHING;
	gt |= (gt & GT_ALPHA)		? GT_GAMMA | GT_THETA		        : GT_NOTHING;

	pGreeks->nMask = gt;

	pGreeks->dTheoPrice = CalculateOptionMM(dDomesticRate, dForeignRate, dHTBRate, dSpotPrice, dStrike, 
		dVolatility, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), nCount, pDivAmnts, pDivYears,
		min(max(nSteps, OPM::cnTreeStepsMin), OPM::cnTreeStepsMax), dSkew, dKurtosis, nModel, pGreeks);

	if(OPM::IsBadValue(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}

	return 1;
}

