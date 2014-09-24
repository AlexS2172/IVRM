//=============================================================================
// Copyright 2001, Eugene Karnygin
//
// Author	: Eugene Karnygin
// File		: CoxRoss.cpp
// Created	: 15 july 2001
// 
// The Cox Ross Rubenstein model of theoretical option price
//=============================================================================

#include "Common.h"

namespace OPM
{

double CoxRoss(	double	S,		// Underlying spot price
				double	K,		// Strike price
				double	Rd,		// Domestic continuos risk free rate
				double	Rf,		// Foreign continuos risk free rate (or yield value)
				double	Rb,		// Actual borrowing continuous rate
				double	V,		// Volatility
				double	T,		// Years amount till expiration
				bool	IsCall,	// true if it's 'Call option', false is 'Put option'
				long	Steps,	// Amount of binomial tree steps		
				double*	DA,		// Array of dividend's amount
				double*	DT,		// Array of years till dividend payment
				long	DC,		// Count of dividends
				GREEKS*	pGreeks/*out*/
				)
{
	// Validate the input parametrs
	if (Steps < cnTreeStepsMin || Steps > cnTreeStepsMax)
		return BadDoubleValue;

	if (S <= 0. || K <= 0. || V <= 0. || T <= 0. || Rd < 0. || Rf < 0. || Rb < 0.)
		return BadDoubleValue;

	if (DC > 0)	
		Rf = 0.;

	bool bCalculateGreeks = pGreeks ? ( (pGreeks->nMask & ~GT_THEOPRICE) != GT_NOTHING) : false;
	
	double C00, C10, C11, C20, C21, C22, C31, C32, C41, C42, C43;
	C00 = C10 = C11 = C20 = C21 = C22 = C31 = C32 = C41 = C42 = C43 = BadDoubleValue;
	long cCalculatedGreeksMask = GT_NOTHING;

	// Contains option prices of one step (for begin and end of the step)
	//
	//      Tree[1] Tree[3] Tree[5]      Begin of step i
	//      /     \ /     \ /    \
	//  Tree[0] Tree[2] Tree[4] Tree[6]  End of step i
	//
	// One tree step (i) has i points of price at the begin and
	// i + 1 points of price and the end, therefore one step has 
	// i + (i + 1) points
	double	Tree[cnTreeStepsMax + cnTreeStepsMax + 1];
	
	// This array contains all probably values 
	// of start up option price in the tree
	double	_Tree[cnTreeStepsMax + cnTreeStepsMax + 1];			
	
	// This array contains dividends' payments on each step
	double	Payment[cnTreeStepsMax] = {0.}; 

	double	dDividendPv = 0.;
	// Computes steps and cumulative dividend payment
	for (long k = 0; k < DC; k++) 
	{
		if (DT[k] < 0. || !ValueNEQZero(DT[k]) || DT[k] > T || DA[k] < 0. || !ValueNEQZero(DA[k]))
			continue;
		
		dDividendPv = DividendPv(DA[k], Rd, Rb, DT[k]);						// Discounts the underlying
		S -= dDividendPv;
			
		if (S < 0.)
			return BadDoubleValue;

		long	Step = long(DT[k] / T  * Steps);	// Calculate payment step
	
		// Sometimes it can has two dividend payments in one day
		if(Step < cnTreeStepsMax && Step >= 0L)
			Payment[Step] += dDividendPv;
	}
	const double	S0 = S;		// stock price without dividends
	const double	TimeStep = T / Steps;

	const double	R = Rd - (Rf + Rb);						// Summary rate of underlying
	const double	A = exp(R * TimeStep);					// Yield on one step 
	const double	U = V * sqrt(TimeStep);				
	const double	D = -U;
	const double	P = (A - exp(D)) / (exp(U) - exp(D)); 
	
	// Probabilities of changing spot price 
	double  StepDiscountFactor = 1. / exp(Rd * TimeStep);
	double	Pu = P * StepDiscountFactor;		// Upper
	double	Pd = StepDiscountFactor - Pu;		// Lower
	
	// It's the indexes of border point of step 
	long	Bottom	= 0;
	long	Top		= Steps * 2;

	// Set start values of the option prices at the last step
	double	SpotShiftFactor[607];
	for (long i = Bottom; i <= Top; i++)
	{
		SpotShiftFactor[i] = exp((IsCall ? U : -U) * (Steps - i));
		double _S	= S * SpotShiftFactor[i];
		double C	= (IsCall ? _S - K : K - _S);

		_Tree[i] = Tree[i] = max(C, 0);
	}

	// Flag is true when we have done all steps of the tree
	bool Finish = (Bottom == Top);		

	// This calculate tree from last step to first one
	while (!Finish)
	{
		long Step = (Top - Bottom) / 2; // Number of current step

		// If dividend payed on this step it should correct
		// option prices of the tree
		if (Payment[Step] > Epsilon)
		{
#ifndef NonPropDividend
			//ProportionalDividendCalculation
			for (register int i = Bottom; i <= Top; i++)
			{
				double _S = S * SpotShiftFactor[i] + Payment[Step];
				double C  = (IsCall ? _S - K : K - _S);	

				_Tree[i] = max(C, 0);

				if ((i - Bottom) % 2 == 0)
					Tree[i] = max(Tree[i], _Tree[i]);			
			}
			S += Payment[Step];
#else
			S += Payment[Step];
			for (int i = Bottom; i <= Top; i++)
			{
				double _S = S * SpotShiftFactor[i];
				double C  = (IsCall ? _S - K : K - _S);	

				_Tree[i] = max(C, 0);

				if ((i - Bottom) % 2 == 0)
					Tree[i] = max(Tree[i], _Tree[i]);			
			}
#endif
		}

		Finish = (Bottom++ == Top--); 

		// Pass by one step
		for (int i = Bottom; i <= Top; i += 2)
		{
			if (IsCall)		
				Tree[i] = max(Pu * Tree[i-1] + Pd * Tree[i+1], _Tree[i]);
			else
				Tree[i] = max(Pu * Tree[i+1] + Pd * Tree[i-1], _Tree[i]);

			// It break the step pass if option price at
			// next point is zero, because next calculated 
			// price will be zero as well and therefore
			// it will be less or equal then calculated
			// price before.
			if (Tree[i+1] < Epsilon)
				break;
		}

		if(bCalculateGreeks)
		{
			if(Step == 4)
			{
				if(IsCall)
				{
					C41 = Tree[Bottom + 5];
					C42 = Tree[Bottom + 3];
					C43 = Tree[Bottom + 1];
				}
				else
				{
					C41 = Tree[Bottom + 1];
					C42 = Tree[Bottom + 3];
					C43 = Tree[Bottom + 5];
				}
			}
			else if(Step == 3)
			{
				if(IsCall)
				{
					C31 = Tree[Bottom + 3];
					C32 = Tree[Bottom + 1];
				}
				else
				{
					C31 = Tree[Bottom + 1];
					C32 = Tree[Bottom + 3];
				}
			}
			else if(Step == 2)
			{
				if(IsCall)
				{
					C20 = Tree[Bottom + 3];
					C21 = Tree[Bottom + 1];
					C22 = Tree[Bottom - 1];
				}
				else
				{
					C20 = Tree[Bottom - 1];
					C21 = Tree[Bottom + 1];
					C22 = Tree[Bottom + 3];
				}
			}
			else if(Step == 1)
			{
				if(IsCall)
				{
					C10 = Tree[Bottom + 1];
					C11 = Tree[Bottom - 1];
				}
				else
				{
					C10 = Tree[Bottom - 1];
					C11 = Tree[Bottom + 1];
				}
			}
			else if(Step == 0)
			{
				C00 = Tree[Steps];
			}
		}

	}

	double dTheoPrice = Tree[Steps]; // return option value from top of the tree
	
	if(!IsValidTheoPrice(dTheoPrice))
	{
		dTheoPrice = BadDoubleValue;
	}
	else
	{
		cCalculatedGreeksMask |= GT_THEOPRICE;
	}

	if(pGreeks && bCalculateGreeks && !IsBadValue(dTheoPrice))
	{
		const double Tau = T / Steps;
		const double u = exp(U);
		const double d = exp(D);
		const double u_d = u - d;

		// DELTA
	    if ( (pGreeks->nMask & GT_DELTA) && ValueNEQZero(u_d) && ValueNEQZero(S0)
			&& !IsBadValue(C11) && !IsBadValue(C10))
	    {
			pGreeks->dDelta = (C11 - C10) / (S0 * u_d);
		    cCalculatedGreeksMask |= GT_DELTA;
	    }

    	// GAMMA
	    if ((pGreeks->nMask & GT_GAMMA)  && ValueNEQZero(u_d) && ValueNEQZero(S0) && ValueNEQZero(u) && ValueNEQZero(d)
			&& !IsBadValue(C20) && !IsBadValue(C21) && !IsBadValue(C22))
	    {
			double dDeltaU = (C22 - C21) / (S0 * u_d * u);
			double dDeltaD = (C21 - C20) / (S0 * u_d * d);

		    pGreeks->dGamma = (dDeltaU - dDeltaD) / (S0 * u_d);
		    cCalculatedGreeksMask |= GT_GAMMA;
	    }

	    // THETA
	    if (pGreeks->nMask & GT_THETA && ValueNEQZero(Tau)
			&& !IsBadValue(C21) && !IsBadValue(C00))
	    {
            pGreeks->dTheta = (C21 - C00) / (730.0 * Tau);
		    cCalculatedGreeksMask |= GT_THETA;
	    }

	    // DELTA THETA
	    if ((pGreeks->nMask & GT_DELTA_THETA) && ValueNEQZero(u_d) && ValueNEQZero(S0) 
			&& ValueNEQZero(u) && ValueNEQZero(d) && ValueNEQZero(Tau)
			&& !IsBadValue(C32) && !IsBadValue(C31))
	    {
			double dDeltaM = (C32 - C31) / (S0 * u_d * u * d);
		    pGreeks->dDeltaTheta = (dDeltaM - pGreeks->dDelta) / (730.0 * Tau);
		    cCalculatedGreeksMask |= GT_DELTA_THETA;
	    }

	    // GAMMA THETA
	    if ((pGreeks->nMask & GT_GAMMA_THETA) && ValueNEQZero(u_d) && ValueNEQZero(S0) 
			&& ValueNEQZero(u) && ValueNEQZero(d) && ValueNEQZero(Tau)
			&& !IsBadValue(C41) && !IsBadValue(C42) && !IsBadValue(C43) )
	    {
			double dDeltaUU = (C43 - C42) / (S0 * u_d * u * u * d);
			double dDeltaDD = (C42 - C41) / (S0 * u_d * u * d * d);
			double dGammaM = (dDeltaUU - dDeltaDD) / (S0 * u_d * u * d );

		    pGreeks->dGammaTheta = (dGammaM - pGreeks->dGamma) / (730.0 * Tau);
		    cCalculatedGreeksMask |= GT_GAMMA_THETA;
	    }

		// Check values
		if (cCalculatedGreeksMask & GT_DELTA)
			pGreeks->dDelta = min(max(pGreeks->dDelta, -1.), 1.);
				
		if (cCalculatedGreeksMask & GT_GAMMA)
			pGreeks->dGamma = max(pGreeks->dGamma, 0.);

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

	return dTheoPrice;
}

double CoxRossOddEvenAdjust(
				double	dSpotPrice,
				double	dStrike,
				double	dRateDomestic,
				double	dRateForeign,
				double	dHTBRate,
				double	dVolatility,
				double	dYte,
				bool	bIsCall,
				long	nSteps,	// Amount of binomial tree steps
				double* pdDivAmnt,						   
				double* pdDivYte,						    
				int		nDivCount,
				GREEKS*	pGreeks/*out*/
				)
{
	if (dYte < 0.) 
		return max(bIsCall ? dSpotPrice - dStrike : dStrike - dSpotPrice, 0.);

	double dContRd = RateDiscToCont(dRateDomestic , dYte * cdDaysPerYear365);
	double dContRf = RateDiscToCont(dRateForeign, dYte * cdDaysPerYear365);
	//double dContRb = IsBadValue(dHTBRate) ? 0 : (dContRd - RateDiscToCont(dHTBRate, nDte)); //actual stock borrowing rate (b in spec)
	double dContRb = IsBadValue(dHTBRate) ? 0 : (RateDiscToCont(dRateDomestic - dHTBRate, dYte * cdDaysPerYear365)); //actual stock borrowing rate (b in spec)
	
	GREEKS GreeksN, GreeksN_1;

	if(pGreeks)
	{
		GreeksN.nMask = pGreeks->nMask;
		GreeksN_1.nMask = pGreeks->nMask;
		pGreeks->nMask = GT_NOTHING;
	}

	double dPriceN = CoxRoss(dSpotPrice, dStrike, dContRd, dContRf, dContRb, dVolatility, dYte, bIsCall, nSteps, pdDivAmnt, pdDivYte, nDivCount, pGreeks ? &GreeksN : NULL);
	if(IsBadValue(dPriceN))
		return BadDoubleValue;
	double dPriceN_1 = CoxRoss(dSpotPrice, dStrike, dContRd, dContRf, dContRb, dVolatility, dYte, bIsCall, nSteps - 1, pdDivAmnt, pdDivYte, nDivCount, pGreeks ? &GreeksN_1 : NULL);
	if(IsBadValue(dPriceN_1))
		return BadDoubleValue;

	double dPrice = (dPriceN + dPriceN_1) / 2.;

	if(pGreeks)
	{
		pGreeks->nMask = GT_THEOPRICE;
		pGreeks->dTheoPrice = dPrice;
	}
	
	// DELTA
	if ((GreeksN.nMask & GT_DELTA) && (GreeksN_1.nMask & GT_DELTA))
	{
		pGreeks->dDelta = (GreeksN.dDelta + GreeksN_1.dDelta) / 2.;
		pGreeks->nMask |= GT_DELTA;
	}

	// GAMMA	
    if ((GreeksN.nMask & GT_GAMMA) && (GreeksN_1.nMask & GT_GAMMA))	   
	{			
		pGreeks->dGamma = (GreeksN.dGamma + GreeksN_1.dGamma) / 2.;
		pGreeks->nMask |= GT_GAMMA;
    }

	// THETA	
    if ((GreeksN.nMask & GT_THETA) && (GreeksN_1.nMask & GT_THETA))	   
	{			
		pGreeks->dTheta = (GreeksN.dTheta + GreeksN_1.dTheta) / 2.;
		pGreeks->nMask |= GT_THETA;
    }

	// DELTA THETA	
    if ((GreeksN.nMask & GT_DELTA_THETA) && (GreeksN_1.nMask & GT_DELTA_THETA))	   
	{			
		pGreeks->dDeltaTheta = (GreeksN.dDeltaTheta + GreeksN_1.dDeltaTheta) / 2.;
		pGreeks->nMask |= GT_DELTA_THETA;
    }
	
	// GAMMA THETA	
    if ((GreeksN.nMask & GT_GAMMA_THETA) && (GreeksN_1.nMask & GT_GAMMA_THETA))	   
	{			
		pGreeks->dGammaTheta = (GreeksN.dGammaTheta + GreeksN_1.dGammaTheta) / 2.;
		pGreeks->nMask |= GT_GAMMA_THETA;
    }

	// ALPHA
	if ( (GreeksN.nMask & GT_ALPHA) && (GreeksN_1.nMask & GT_ALPHA))
	{
		pGreeks->dAlpha = (GreeksN.dAlpha + GreeksN_1.dAlpha) / 2.;
		pGreeks->nMask |= GT_ALPHA;
	}

	return dPrice;
}

}