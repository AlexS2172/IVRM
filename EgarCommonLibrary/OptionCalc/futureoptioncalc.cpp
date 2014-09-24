/***************************************************************************************************
*	
*	(C) EGAR technology inc. 2003
*	
*	Special functions which calulates greeks and implied volatility 
*	of future options.
*	
*	
*	Author: Michael Malyshkin
*
***************************************************************************************************/

#include "stdafx.h"

#include "Models.h"
#include "FutureModels.h"
#include <OptionCalc/FutureOptionCalc.h>
//#include "OptionCalc.cpp"

// version with index dividend yield
static double CalculateFutureOption3(double	dDomesticRate,
									 double	dYield,
									 bool	bSpotPrice,
									 double	dFuturePrice,
									 double	dStrike,
									 double	dVolatility, 
									 double	dYTE,
									 bool	bIsCall,
									 bool	bIsAmerican,
									 long	nSteps,
									 double	dSkew,
									 double	dKurtosis,
									 long	nModel,
									 double* pDivAmnts,
									 double* pDivYears,
									 long	 nCount,
									 GREEKS* pGreeks = NULL)
{



	switch (nModel)
	{
	case MM_EGAR_BS			:
		return CFO_BlackScholes3(dDomesticRate,  dYield, bSpotPrice, dFuturePrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, pDivAmnts, pDivYears, nCount, pGreeks);
	case MM_EGAR_BINOMIAL	:	
		if (!bIsAmerican || bIsCall )
			return CFO_BlackScholes3(
			dDomesticRate,
			dYield, 
			bSpotPrice,
			dFuturePrice,						
			dStrike,						
			dVolatility,						
			dYTE,						
			bIsCall,						
			bIsAmerican,
			pDivAmnts,
			pDivYears,
			nCount,
			pGreeks);

		return CFO_CoxRossWithBlackScholes3(
			dDomesticRate,						 
			dYield, 
			bSpotPrice,
			dFuturePrice,
			dStrike,
			dVolatility,
			dYTE,
			bIsCall,						
			bIsAmerican,						
			nSteps,
			pDivAmnts, 
			pDivYears, 
			nCount,
			pGreeks);

	case MM_EGAR_OPTIMIZED	:	
		if (!bIsAmerican || bIsCall )
			return CFO_BlackScholes3(
			dDomesticRate,
			dYield, 
			bSpotPrice,
			dFuturePrice,						
			dStrike,						
			dVolatility,						
			dYTE,						
			bIsCall,						
			bIsAmerican,						
			pDivAmnts,
			pDivYears,
			nCount,
			pGreeks);

		return CFO_CoxRossWithBlackScholes3(
			dDomesticRate,						 
			dYield, 
			bSpotPrice,
			dFuturePrice,
			dStrike,
			dVolatility,
			dYTE,
			bIsCall,						
			bIsAmerican,						
			cnOptimizedBinomialSteps,
			pDivAmnts,
			pDivYears,
			nCount,
			pGreeks);
	case MM_EGAR_VSKLOG		:	
		return CFO_VskLog3(dDomesticRate, dYield, bSpotPrice, dFuturePrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, dSkew, dKurtosis, pDivAmnts, pDivYears, nCount, pGreeks);

	default: return OPM::BadDoubleValue;
	}
}


static double CalculateFutureOption(double	dDomesticRate,
								double	dFuturePrice,
								double	dStrike,
								double	dVolatility, 
								double	dYTE,
								bool	bIsCall,
								bool	bIsAmerican,
								long	nSteps,
								double	dSkew,
								double	dKurtosis,
								long	nModel,
								double*	pDivAmnts,
								double*	pDivYears,
								long	nCount,
								GREEKS*	pGreeks = NULL)
{



	switch (nModel)
	{
	case MM_EGAR_BS			:
        return CFO_BlackScholes(dDomesticRate, dFuturePrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, pDivAmnts, pDivYears, nCount, pGreeks);
	case MM_EGAR_BINOMIAL	:	
		if (!bIsAmerican || bIsCall )
			return CFO_BlackScholes(
							dDomesticRate,						
							dFuturePrice,						
							dStrike,						
							dVolatility,						
							dYTE,						
							bIsCall,						
							bIsAmerican,
							pDivAmnts,
							pDivYears,
							nCount,
							pGreeks);

		return CFO_CoxRossWithBlackScholes(
							dDomesticRate,						 
							dFuturePrice,
							dStrike,
							dVolatility,
							dYTE,
							bIsCall,						
							bIsAmerican,						
							nSteps,
							pDivAmnts, 
							pDivYears, 
							nCount,
							pGreeks);
		
	case MM_EGAR_OPTIMIZED	:	
		if (!bIsAmerican || bIsCall )
			return CFO_BlackScholes(
							dDomesticRate,						
							dFuturePrice,						
							dStrike,						
							dVolatility,						
							dYTE,						
							bIsCall,						
							bIsAmerican,						
							pDivAmnts,
							pDivYears,
							nCount,
							pGreeks);

		return CFO_CoxRossWithBlackScholes(
							dDomesticRate,						 
							dFuturePrice,
							dStrike,
							dVolatility,
							dYTE,
							bIsCall,						
							bIsAmerican,						
							cnOptimizedBinomialSteps,
							pDivAmnts,
							pDivYears,
							nCount,
							pGreeks);
	case MM_EGAR_VSKLOG		:	
        return CFO_VskLog(dDomesticRate, dFuturePrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, dSkew, dKurtosis, pDivAmnts, pDivYears, nCount, pGreeks);

	default: return OPM::BadDoubleValue;
	}
}

static double CalculateFutureOption2(double	dDomesticRate,
									double	dFuturePrice,
									bool	bSpotPrice,
									double	dStrike,
									double	dVolatility, 
									double	dYTE,
									bool	bIsCall,
									bool	bIsAmerican,
									long	nSteps,
									double	dSkew,
									double	dKurtosis,
									long	nModel,
									double*	pDivAmnts,
									double*	pDivYears,
									long	nCount,
									GREEKS*	pGreeks = NULL)
{



	switch (nModel)
	{
	case MM_EGAR_BS			:
		return CFO_BlackScholes2(dDomesticRate, dFuturePrice, bSpotPrice, dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, pDivAmnts, pDivYears, nCount, pGreeks);
	case MM_EGAR_BINOMIAL	:	
		if (!bIsAmerican || bIsCall )
			return CFO_BlackScholes2(
			dDomesticRate,						
			dFuturePrice, 
			bSpotPrice, 
			dStrike,						
			dVolatility,						
			dYTE,						
			bIsCall,						
			bIsAmerican,
			pDivAmnts,
			pDivYears,
			nCount,
			pGreeks);

		return CFO_CoxRossWithBlackScholes2(
			dDomesticRate,						 
			dFuturePrice,
		    bSpotPrice, 
			dStrike,
			dVolatility,
			dYTE,
			bIsCall,						
			bIsAmerican,						
			nSteps,
			pDivAmnts, 
			pDivYears, 
			nCount,
			pGreeks);

	case MM_EGAR_OPTIMIZED	:	
		if (!bIsAmerican || bIsCall )
			return CFO_BlackScholes2(
			dDomesticRate,						
			dFuturePrice,	
			bSpotPrice, 
			dStrike,						
			dVolatility,						
			dYTE,						
			bIsCall,						
			bIsAmerican,						
			pDivAmnts,
			pDivYears,
			nCount,
			pGreeks);

		return CFO_CoxRossWithBlackScholes2(
			dDomesticRate,						 
			dFuturePrice,
			bSpotPrice, 
			dStrike,
			dVolatility,
			dYTE,
			bIsCall,						
			bIsAmerican,						
			cnOptimizedBinomialSteps,
			pDivAmnts,
			pDivYears,
			nCount,
			pGreeks);
	case MM_EGAR_VSKLOG		:	
		return CFO_VskLog2(dDomesticRate, dFuturePrice,  bSpotPrice,  dStrike,
			dVolatility, dYTE, bIsCall, bIsAmerican, dSkew, dKurtosis, pDivAmnts, pDivYears, nCount, pGreeks);

	default: return OPM::BadDoubleValue;
	}
}


/***************************************************************************************************
	Calculates volatility. Returns -1 if error occured.
***************************************************************************************************/
double OPTIONCALC_API CalcFutureOptionVolatility(	double	dDomesticRate,	
											double	dFuturePrice,
											double	dOptionPrice,
											double	dStrike,
											double	dYTE,
											long	nIsCall,
											long	nIsAmerican,
											long	nSteps,
											double	dSkew,
											double	dKurtosis,
											long	nModel,
                                            long*   pnFlag)
{
	// Check parameters
    if (!pnFlag)
    {
		::SetLastError(ERROR_INVALID_PARAMETER);
		return -1;
    }

	double dVolaLeft	= 0.01;		// Left boundary of volatility value
	double dVolaRight	= 2.5;		// Right boundary of volatility value
	double dVolaMed;
	
	double ImpVol = -1;

	const double cdOptValPrec = 0.0001;			// Precision of volatility search (Epsilon for option price)
	
	double	dOptValDelta = cdOptValPrec + 1;	// 

	int	Count = -30;			// Amount of iterations to find volatility value

	// Calculate boundary values
	double dOptValMin = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
		dVolaLeft, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		max(nSteps, 5), dSkew, dKurtosis, nModel, NULL, NULL, 0);	
	if(OPM::IsBadValue(dOptValMin))
		dOptValMin = 0.;

	double dOptValMax = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
		dVolaRight, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		max(nSteps, 5), dSkew, dKurtosis, nModel, NULL, NULL, 0);
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
		dOptValMid = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
			dVolaMed, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
			max(nSteps, 5), dSkew, dKurtosis, nModel, NULL, NULL, 0, NULL);
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


/***************************************************************************************************
	Calculates volatility. Returns -1 if error occured.
***************************************************************************************************/
double OPTIONCALC_API CalcFutureOptionVolatilityCustDivs(	double	dDomesticRate,	
											double	dFuturePrice,
											double	dOptionPrice,
											double	dStrike,
											double	dYTE,
											long	nIsCall,
											long	nIsAmerican,
											long	nSteps,
											double	dSkew,
											double	dKurtosis,
											long	nCount,
											double *pDivAmnts,
											double *pDivYears,
											long	nModel,
                                            long*   pnFlag)
{
	// Check parameters
    if (!pnFlag)
    {
		::SetLastError(ERROR_INVALID_PARAMETER);
		return -1;
    }

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

	double dVolaLeft	= 0.01;		// Left boundary of volatility value
	double dVolaRight	= 2.5;		// Right boundary of volatility value
	double dVolaMed;
	
	double ImpVol = -1;

	const double cdOptValPrec = 0.0001;			// Precision of volatility search (Epsilon for option price)
	
	double	dOptValDelta = cdOptValPrec + 1;	// 

	int	Count = -30;			// Amount of iterations to find volatility value

	// Calculate boundary values
	double dOptValMin = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
		dVolaLeft, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		max(nSteps, 5), dSkew, dKurtosis,  nModel, pDivAmnts, pDivYears, nCount, NULL);
	if(OPM::IsBadValue(dOptValMin))
		dOptValMin = 0.;

	double dOptValMax = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
		dVolaRight, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		max(nSteps, 5), dSkew, dKurtosis,  nModel, pDivAmnts, pDivYears, nCount, NULL);
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
		dOptValMid = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
			dVolaMed, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
			max(nSteps, 5), dSkew, dKurtosis, nModel, pDivAmnts, pDivYears, nCount, NULL);
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



//long OPTIONCALC_API CalcGreeks( double dDomesticRate, double dForeignRate, 
//								double dSpotPrice, double dStrike, double dVolatility, 
//								long nDTE, long nIsCall, long nIsAmerican, 
//								long nCount, const DIVIDEND *pDividends, long nSteps, 
//								/*out*/GREEKS *pGreeks )
//{



// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.

long OPTIONCALC_API CalcFutureOptionGreeksCustDivs(	double	dDomesticRate,
									double	dFuturePrice,
									double	dStrike,
									double	dVolatility, 
									double	dYTE,
									long	nIsCall,
									long	nIsAmerican,
									long	nSteps,
									double	dSkew,
									double	dKurtosis,
									long	nModel,
									long	nCount,
									double *pDivAmnts,	
									double *pDivYears,
									GREEKS*	pGreeks) /*out*/
{
	if (dVolatility <= 0.0005 || dStrike <= 0.)
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

	gt |= (gt & GT_GAMMA_VEGA)	? GT_DELTA_VEGA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_GAMMA_THETA) ? GT_DELTA_THETA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_DELTA_VEGA)	? GT_DELTA | GT_VEGA | GT_GAMMA     : GT_NOTHING;
	gt |= (gt & GT_DELTA_THETA) ? GT_DELTA | GT_THETA | GT_GAMMA    : GT_NOTHING;
	gt |= (gt & GT_GAMMA)		? GT_DELTA					        : GT_NOTHING;
	gt |= (gt & GT_ALPHA)		? GT_GAMMA | GT_THETA		        : GT_NOTHING;

	pGreeks->nMask = gt;

	pGreeks->dTheoPrice = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
		dVolatility, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		min(max(nSteps, OPM::cnTreeStepsMin), OPM::cnTreeStepsMax), dSkew, dKurtosis, nModel, pDivYears, pDivAmnts, nCount, pGreeks);

	if(OPM::IsBadValue(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}

	
	return 1;
}



// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.

long OPTIONCALC_API CalcFutureOptionGreeks(	double	dDomesticRate,
									double	dFuturePrice,
									double	dStrike,
									double	dVolatility, 
									double	dYTE,
									long	nIsCall,
									long	nIsAmerican,
									long	nSteps,
									double	dSkew,
									double	dKurtosis,
									long	nModel,
									GREEKS*	pGreeks) /*out*/
{
	if (dVolatility <= 0.0005 || dStrike <= 0.)
		return 0;				// Check valid volatility & strike

	// Check parameters
	if(::IsBadWritePtr(pGreeks, sizeof(GREEKS)))
	{
		::SetLastError(ERROR_NOACCESS);
		return 0;
	}

	if( pGreeks->nMask == GT_NOTHING )
		return 1;	// Nothing to calculate


	long gt = pGreeks->nMask;

	gt |= (gt & GT_GAMMA_VEGA)	? GT_DELTA_VEGA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_GAMMA_THETA) ? GT_DELTA_THETA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_DELTA_VEGA)	? GT_DELTA | GT_VEGA | GT_GAMMA     : GT_NOTHING;
	gt |= (gt & GT_DELTA_THETA) ? GT_DELTA | GT_THETA | GT_GAMMA    : GT_NOTHING;
	gt |= (gt & GT_GAMMA)		? GT_DELTA					        : GT_NOTHING;
	gt |= (gt & GT_ALPHA)		? GT_GAMMA | GT_THETA		        : GT_NOTHING;

	pGreeks->nMask = gt;

	pGreeks->dTheoPrice = CalculateFutureOption(dDomesticRate, dFuturePrice, dStrike, 
		dVolatility, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		min(max(nSteps, OPM::cnTreeStepsMin), OPM::cnTreeStepsMax), dSkew, dKurtosis, nModel, NULL , NULL, 0, pGreeks);

	if(OPM::IsBadValue(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}

	
	return 1;
}


long OPTIONCALC_API CalcFutureOptionGreeksCustDivs2(	double	dDomesticRate,
												   double	dFuturePrice,
												   bool	bSpotGreeks,
												   double	dStrike,
												   double	dVolatility, 
												   double	dYTE,
												   long	nIsCall,
												   long	nIsAmerican,
												   long	nSteps,
												   double	dSkew,
												   double	dKurtosis,
												   long	nModel,
												   long	nCount,
												   double *pDivAmnts,	
												   double *pDivYears,
												   GREEKS*	pGreeks) /*out*/
{
	if (dVolatility <= 0.0005 || dStrike <= 0.)
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

	gt |= (gt & GT_GAMMA_VEGA)	? GT_DELTA_VEGA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_GAMMA_THETA) ? GT_DELTA_THETA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_DELTA_VEGA)	? GT_DELTA | GT_VEGA | GT_GAMMA     : GT_NOTHING;
	gt |= (gt & GT_DELTA_THETA) ? GT_DELTA | GT_THETA | GT_GAMMA    : GT_NOTHING;
	gt |= (gt & GT_GAMMA)		? GT_DELTA					        : GT_NOTHING;
	gt |= (gt & GT_ALPHA)		? GT_GAMMA | GT_THETA		        : GT_NOTHING;

	pGreeks->nMask = gt;

	pGreeks->dTheoPrice = CalculateFutureOption2(dDomesticRate, dFuturePrice, bSpotGreeks, dStrike, 
		dVolatility, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		min(max(nSteps, OPM::cnTreeStepsMin), OPM::cnTreeStepsMax), dSkew, dKurtosis, nModel, pDivYears, pDivAmnts, nCount, pGreeks);

	if(OPM::IsBadValue(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}


	return 1;
}



// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.

long OPTIONCALC_API CalcFutureOptionGreeks2(	double	dDomesticRate,
										   double	dFuturePrice,
										   bool	bSpotGreeks,
										   double	dStrike,
										   double	dVolatility, 
										   double	dYTE,
										   long	nIsCall,
										   long	nIsAmerican,
										   long	nSteps,
										   double	dSkew,
										   double	dKurtosis,
										   long	nModel,
										   GREEKS*	pGreeks) /*out*/
{
	if (dVolatility <= 0.0005 || dStrike <= 0.)
		return 0;				// Check valid volatility & strike

	// Check parameters
	if(::IsBadWritePtr(pGreeks, sizeof(GREEKS)))
	{
		::SetLastError(ERROR_NOACCESS);
		return 0;
	}

	if( pGreeks->nMask == GT_NOTHING )
		return 1;	// Nothing to calculate


	long gt = pGreeks->nMask;

	gt |= (gt & GT_GAMMA_VEGA)	? GT_DELTA_VEGA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_GAMMA_THETA) ? GT_DELTA_THETA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_DELTA_VEGA)	? GT_DELTA | GT_VEGA | GT_GAMMA     : GT_NOTHING;
	gt |= (gt & GT_DELTA_THETA) ? GT_DELTA | GT_THETA | GT_GAMMA    : GT_NOTHING;
	gt |= (gt & GT_GAMMA)		? GT_DELTA					        : GT_NOTHING;
	gt |= (gt & GT_ALPHA)		? GT_GAMMA | GT_THETA		        : GT_NOTHING;

	pGreeks->nMask = gt;

	pGreeks->dTheoPrice = CalculateFutureOption2(dDomesticRate, dFuturePrice, bSpotGreeks,  dStrike, 
		dVolatility, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		min(max(nSteps, OPM::cnTreeStepsMin), OPM::cnTreeStepsMax), dSkew, dKurtosis, nModel, NULL , NULL, 0, pGreeks);

	if(OPM::IsBadValue(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}


	return 1;
}


long OPTIONCALC_API CalcFutureOptionGreeks3(	double	dDomesticRate,
													double dYield,
													double	dFuturePrice,
													bool	bSpotGreeks,
													double	dStrike,
													double	dVolatility, 
													double	dYTE,
													long	nIsCall,
													long	nIsAmerican,
													long	nSteps,
													double	dSkew,
													double	dKurtosis,
													long	nModel,
													long	nCount,
													double *pDivAmnts,	
													double *pDivYears,
													GREEKS*	pGreeks) /*out*/
{
	if (dVolatility <= 0.0005 || dStrike <= 0.)
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

	gt |= (gt & GT_GAMMA_VEGA)	? GT_DELTA_VEGA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_GAMMA_THETA) ? GT_DELTA_THETA | GT_GAMMA	        : GT_NOTHING;
	gt |= (gt & GT_DELTA_VEGA)	? GT_DELTA | GT_VEGA | GT_GAMMA     : GT_NOTHING;
	gt |= (gt & GT_DELTA_THETA) ? GT_DELTA | GT_THETA | GT_GAMMA    : GT_NOTHING;
	gt |= (gt & GT_GAMMA)		? GT_DELTA					        : GT_NOTHING;
	gt |= (gt & GT_ALPHA)		? GT_GAMMA | GT_THETA		        : GT_NOTHING;

	pGreeks->nMask = gt;

	pGreeks->dTheoPrice = CalculateFutureOption3(dDomesticRate, dYield, bSpotGreeks, dFuturePrice, dStrike, 
		dVolatility, dYTE, (nIsCall != 0L), (nIsAmerican != 0L), 
		min(max(nSteps, OPM::cnTreeStepsMin), OPM::cnTreeStepsMax), dSkew, dKurtosis, nModel, pDivAmnts, pDivYears, nCount, pGreeks);

	if(OPM::IsBadValue(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}


	return 1;
}

