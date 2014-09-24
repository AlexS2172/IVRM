#include "stdafx.h"
//#include <atlutil.h>
#include "Models.h"

double CO_BlackScholes(	
						double	R,
						double	RF,
						double	dHTBRate,
						double	S,
						double	K,
						double	V,
						double	Yte, 
                        bool	Call,
						bool	American,
						double* DivAmnt,
						double* DivYte,
						int		DivCount,
						GREEKS *pGreeks
						)
{
	if (Yte < 0.) 
		return max(Call ? S - K : K - S, 0.);

	if (DivCount > 0)
		RF = 0.;
    
	return OPM::BlackAndScholes(R, RF, dHTBRate, S, K, Yte, V, Call, DivAmnt, DivYte, DivCount, pGreeks);
} 

double CO_StandardBinomial(
						double	R,
					    double	RF,
						double	dHTBRate,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double* DivAmnt,						   
						double* DivYte,						    
						int		DivCount,						    
						long	Steps,							
						GREEKS *pGreeks)
{
	if (!American ||
	   (Call && DivCount == 0) )
		return CO_BlackScholes(
						R,						
						RF,	
						dHTBRate,
						S,						
						K,						
						V,						
						Yte,						
						Call,						
						American,						
						DivAmnt,						
						DivYte,						
						DivCount, 														
						pGreeks);
	
	return CO_CoxRossWithBlackScholes(
						R,						 
						RF,
						dHTBRate,
						S,
						K,
						V,
						Yte,
						Call,
						American,
						DivAmnt,
						DivYte,
						DivCount,
						Steps,
						pGreeks
                        );

}

double CO_OptimizedBinomial(
						double	R,
					    double	RF,
						double	dHTBRate,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double* DivAmnt,						   
						double* DivYte,						    
						int		DivCount,						    
						GREEKS *pGreeks)
{
	if (!American || Call )
		return CO_BlackScholes(
						R,						
						RF,	
						dHTBRate,
						S,						
						K,						
						V,						
						Yte,						
						Call,						
						American,						
						DivAmnt,						
						DivYte,						
						DivCount, 														
						pGreeks);

	return CO_CoxRossWithBlackScholes(
						R,						 
						RF,
						dHTBRate,
						S,
						K,
						V,
						Yte,
						Call,
						American,
						DivAmnt,
						DivYte,
						DivCount,
						cnOptimizedBinomialSteps,
						pGreeks
                        );
}

double CO_VskLog(				
						double	R,
					    double	RF,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double* DivAmnt,						   
						double* DivYte,						    
						int		DivCount,
						double	Skew,
						double	Kurtosis,
						GREEKS *pGreeks)

{	
	// Calculated greeks mask
	long cCalculatedGreeksMask = GT_NOTHING;

	if (Yte < 0.) 
	{
		if(pGreeks)
			pGreeks->nMask = GT_THEOPRICE;
		return max(Call ? S - K : K - S, 0.);
	}
	if (DivCount > 0)
		RF = 0.;	

	double dTheoPrice = OPM::VSKLog(S, K, R, RF, V, Yte, Call, Skew, Kurtosis, DivAmnt, DivYte, DivCount);

	if(OPM::IsBadValue(dTheoPrice))
	{
		if(pGreeks)
			pGreeks->nMask = cCalculatedGreeksMask;
		return OPM::BadDoubleValue;
	}

	if(pGreeks)
	{
		cCalculatedGreeksMask = GT_THEOPRICE;

		double dS = S * ( (R - RF -  0.5 * V * V) / OPM::cdDaysPerYear365 + OPM::cdDeltaSqrtTime * V );

		double dTheoPricePlusDs = OPM::BadDoubleValue;
		double dTheoPriceMinusDs = OPM::BadDoubleValue;
		if(OPM::ValueNEQZero(dS))
		{
			dTheoPricePlusDs = OPM::VSKLog( S + dS,
						K,
						R,
						RF,
						V,
						Yte,
						Call,
						Skew,
						Kurtosis, 
						DivAmnt, 
						DivYte, 
						DivCount);

			dTheoPriceMinusDs = OPM::VSKLog(	
						S - dS,
						K,
						R,
						RF,
						V,
						Yte,
						Call,
						Skew,
						Kurtosis, 
						DivAmnt, 
						DivYte, 
						DivCount);
		}
		else
			dS = OPM::BadDoubleValue;

		double dTheoPriceMinusDt = OPM::VSKLog( 
						S,
						K,
						R,
						RF,
						V,
						Yte - (1.0/OPM::cdDaysPerYear365),
						Call,
						Skew,
						Kurtosis, 
						DivAmnt, 
						DivYte, 
						DivCount);

		double dTheoPricePlusDv = OPM::VSKLog( 
						S,
						K,
						R,
						RF,
						V + OPM::cdDeltaVolatility,
						Yte,
						Call,
						Skew,
						Kurtosis, 
						DivAmnt, 
						DivYte, 
						DivCount);

		if( pGreeks->nMask & GT_DELTA && !OPM::IsBadValue(dS) && !OPM::IsBadValue(dTheoPricePlusDs))
		{

			pGreeks->dDelta = (dTheoPricePlusDs - dTheoPrice) / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_DELTA;
		}

		if( pGreeks->nMask & GT_GAMMA && !OPM::IsBadValue(dS)
			&& !OPM::IsBadValue(dTheoPricePlusDs) && !OPM::IsBadValue(dTheoPriceMinusDs) )
		{
			pGreeks->dGamma = (dTheoPricePlusDs + dTheoPriceMinusDs - 2 * dTheoPrice) / dS / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_GAMMA;
		}

		if( pGreeks->nMask & GT_THETA  && !OPM::IsBadValue(dTheoPriceMinusDt))
		{
			pGreeks->dTheta = dTheoPriceMinusDt - dTheoPrice;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_THETA;
		}

		if( pGreeks->nMask & GT_ALPHA && OPM::ValueNEQZero(pGreeks->dTheta))
		{
			pGreeks->dAlpha = pGreeks->dGamma / pGreeks->dTheta;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_ALPHA;
		}

		if( pGreeks->nMask & GT_VEGA && !OPM::IsBadValue(dTheoPricePlusDv))
		{
			pGreeks->dVega = dTheoPricePlusDv - dTheoPrice;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_VEGA;
		}

		if( pGreeks->nMask & GT_RHO )
		{
		
			double dTheoPricePlusDr = OPM::VSKLog( 
							S,
							K,
							R + OPM::cdDeltaRate,
							RF,
							V,
							Yte,
							Call,
							Skew,
							Kurtosis, 
							DivAmnt, 
							DivYte, 
							DivCount);

			if(!OPM::IsBadValue(dTheoPricePlusDr))
			{
				pGreeks->dRho = dTheoPricePlusDr - dTheoPrice;
				cCalculatedGreeksMask = cCalculatedGreeksMask | GT_RHO;
			}
		}

		double dTheoPricePlusDsMinusDt = OPM::BadDoubleValue;
		if(pGreeks->nMask & GT_DELTA_THETA || pGreeks->nMask & GT_GAMMA_THETA)
		{
			dTheoPricePlusDsMinusDt = OPM::VSKLog( 
						S + dS,
						K,
						R,
						RF,
						V,
						Yte - (1.0 / OPM::cdDaysPerYear365),
						Call,
						Skew,
						Kurtosis, 
						DivAmnt, 
						DivYte, 
						DivCount);
		}

		if( pGreeks->nMask & GT_DELTA_THETA && !OPM::IsBadValue(dS) 
			&& !OPM::IsBadValue(dTheoPriceMinusDt) && !OPM::IsBadValue(dTheoPricePlusDs) 
			&& !OPM::IsBadValue(dTheoPricePlusDsMinusDt))
		{

			pGreeks->dDeltaTheta =  (dTheoPricePlusDsMinusDt - dTheoPriceMinusDt) / dS
				 - (dTheoPricePlusDs - dTheoPrice) / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_DELTA_THETA;
		}

		if( pGreeks->nMask & GT_GAMMA_THETA && !OPM::IsBadValue(dS) 
			&& !OPM::IsBadValue(dTheoPriceMinusDt) && !OPM::IsBadValue(dTheoPricePlusDs) 
			&& !OPM::IsBadValue(dTheoPricePlusDsMinusDt) && !OPM::IsBadValue(dTheoPriceMinusDs))
		{
			double dTheoPriceMinusDsMinusDt = OPM::VSKLog( 
						S - dS,
						K,
						R,
						RF,
						V,
						Yte - (1.0 / OPM::cdDaysPerYear365),
						Call,
						Skew,
						Kurtosis, 
						DivAmnt, 
						DivYte, 
						DivCount);

			if(!OPM::IsBadValue(dTheoPriceMinusDsMinusDt))
			{
				pGreeks->dGammaTheta = (dTheoPricePlusDsMinusDt - 2. * dTheoPriceMinusDt + dTheoPriceMinusDsMinusDt) / dS / dS
					 - (dTheoPricePlusDs - 2. * dTheoPrice + dTheoPriceMinusDs) / dS / dS;
				cCalculatedGreeksMask = cCalculatedGreeksMask | GT_GAMMA_THETA;
			}
		}

		double dS1 = OPM::BadDoubleValue;
		double dTheoPricePlusDs1PlusDv = OPM::BadDoubleValue;
		if( pGreeks->nMask & GT_DELTA_VEGA || pGreeks->nMask & GT_GAMMA_VEGA)
		{
			dS1 = S * ( ( R - RF -  0.5 * (V + OPM::cdDeltaVolatility) * (V + OPM::cdDeltaVolatility) ) / OPM::cdDaysPerYear365 + OPM::cdDeltaSqrtTime * (V + OPM::cdDeltaVolatility) );

			if(OPM::ValueNEQZero(dS1))
			{
			
				dTheoPricePlusDs1PlusDv = OPM::VSKLog( 
							S + dS1,
							K,
							R,
							RF,
							V + OPM::cdDeltaVolatility,
							Yte,
							Call,
							Skew,
							Kurtosis, 
							DivAmnt, 
							DivYte, 
							DivCount);
			}
			else
				dS1 = OPM::BadDoubleValue;
		}


		if( pGreeks->nMask & GT_DELTA_VEGA && !OPM::IsBadValue(dS) && !OPM::IsBadValue(dS1) 
			&& !OPM::IsBadValue(dTheoPricePlusDs1PlusDv) && !OPM::IsBadValue(dTheoPricePlusDv) 
			&& !OPM::IsBadValue(dTheoPricePlusDs))
		{

			pGreeks->dDeltaVega = (dTheoPricePlusDs1PlusDv - dTheoPricePlusDv) / dS1 -
				(dTheoPricePlusDs - dTheoPrice) / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_DELTA_VEGA;
		}

		if( pGreeks->nMask & GT_GAMMA_VEGA && !OPM::IsBadValue(dS) && !OPM::IsBadValue(dS1) 
			&& !OPM::IsBadValue(dTheoPricePlusDs1PlusDv) && !OPM::IsBadValue(dTheoPricePlusDv) 
			&& !OPM::IsBadValue(dTheoPricePlusDs) && !OPM::IsBadValue(dTheoPriceMinusDs) )
		{
			double dTheoPriceMinusDs1PlusDv = OPM::VSKLog( 
						S - dS1,
						K,
						R,
						RF,
						V + OPM::cdDeltaVolatility,
						Yte,
						Call,
						Skew,
						Kurtosis, 
						DivAmnt, 
						DivYte, 
						DivCount);

			if(!OPM::IsBadValue(dTheoPriceMinusDs1PlusDv))
			{
				pGreeks->dGammaVega = (dTheoPricePlusDs1PlusDv - 2. * dTheoPricePlusDv + dTheoPriceMinusDs1PlusDv) / dS1 / dS1 -
					(dTheoPricePlusDs - 2. * dTheoPrice + dTheoPriceMinusDs) / dS / dS;
				cCalculatedGreeksMask = cCalculatedGreeksMask | GT_GAMMA_VEGA;
			}
		}

		pGreeks->dTheoPrice = dTheoPrice;
		pGreeks->nMask = cCalculatedGreeksMask;
	}

	return dTheoPrice;
}


double CO_CoxRossWithBlackScholes(	
							double	R,
						    double	RF,
							double	dHTBrate,
						    double	S,
						    double	K,
						    double	V,
						    double	Yte, 
                            bool	Call,
						    bool	American,
						    double* DivAmnt,
						    double* DivYte,
						    int		DivCount,
						    long	Steps,
							GREEKS *pGreeks
                            )
{
	if (Yte < .0)
		return max(Call ? S - K : K - S, 0.);

	if (DivCount > 0)
		RF = 0.;
		    
	GREEKS GreeksBS, GreeksCR;
	GreeksBS.nMask = pGreeks ? pGreeks->nMask : GT_NOTHING;
	double	dResultBS = OPM::BlackAndScholes(R, RF, dHTBrate, S, K, Yte, V, Call, DivAmnt, DivYte, DivCount, pGreeks ? &GreeksBS : NULL);
	
	GreeksCR.nMask = pGreeks ? pGreeks->nMask : GT_NOTHING;
	double dResultCR = OPM::CoxRossOddEvenAdjust(S, K, R, RF, dHTBrate, V, Yte, Call, Steps, DivAmnt, DivYte, DivCount, pGreeks ? &GreeksCR : NULL);

//	ATLTRACE("BS/CR: %f/%f", dResultBS, dResultCR);

	if (dResultCR > dResultBS)
    {
  		long nMask = GT_NOTHING, nnMask = GT_NOTHING;
		if(pGreeks)	
		{
			nMask = pGreeks->nMask;
			memcpy(pGreeks, &GreeksCR, sizeof(GREEKS));

			GREEKS GreeksCRdVola, GreeksBSdVola;

			if(nMask & GT_VEGA)
				GreeksCRdVola.nMask |= GT_THEOPRICE;
			if(nMask & GT_DELTA_VEGA && GreeksCR.nMask & GT_DELTA)
				GreeksCRdVola.nMask |= GT_DELTA;
			if(nMask & GT_GAMMA_VEGA && GreeksCR.nMask & GT_GAMMA)
				GreeksCRdVola.nMask |= GT_GAMMA;
			if(nMask & GT_VOLGA)
				GreeksCRdVola.nMask |= GT_THEOPRICE;

			if(GreeksCRdVola.nMask != GT_NOTHING)
			{

				GreeksBSdVola.nMask = GreeksCRdVola.nMask;//nMask;

				//ATLTRACE("Mask for BS model: %u\n", GreeksBSdVola.nMask);

				double dResultBSdVola = OPM::BlackAndScholes(R, RF, dHTBrate, S, K, Yte, V + OPM::cdDeltaVolatility, Call, DivAmnt, DivYte, DivCount, &GreeksBSdVola);
				GreeksBSdVola.dTheoPrice = dResultBSdVola;

				//ATLTRACE("TheoPr for BS model: %e\n", GreeksBSdVola.dTheoPrice);

				double dResultCRdVola = OPM::CoxRossOddEvenAdjust(S, K, R, RF, dHTBrate, V + OPM::cdDeltaVolatility, Yte, Call, Steps, DivAmnt, DivYte, DivCount, &GreeksCRdVola);

				//ATLTRACE("TheoPr for CR model: %e\n", GreeksCRdVola.dTheoPrice);

				if(!OPM::IsBadValue(dResultCRdVola)) 
				{
					GREEKS& GreeksCRBSdVola = !OPM::IsBadValue(dResultBSdVola) && dResultBSdVola>dResultCRdVola?GreeksBSdVola:GreeksCRdVola;

					if(nMask & GT_VEGA)
					{
						pGreeks->dVega = max(GreeksCRBSdVola.dTheoPrice - GreeksCR.dTheoPrice, 0.);
						pGreeks->nMask |= GT_VEGA;
					}

					if(GreeksCRdVola.nMask & GT_DELTA)
					{	
						pGreeks->dDeltaVega = GreeksCRBSdVola.dDelta - GreeksCR.dDelta;
						pGreeks->nMask |= GT_DELTA_VEGA;
					}

					if(GreeksCRdVola.nMask & GT_GAMMA)
					{	
						pGreeks->dGammaVega = GreeksCRBSdVola.dGamma - GreeksCR.dGamma;
						pGreeks->nMask |= GT_GAMMA_VEGA;
					}

					if(nMask & GT_VOLGA)
					{
						double dResultBSdVolaMinus = OPM::BlackAndScholes(R, RF, dHTBrate, S, K, Yte, V - OPM::cdDeltaVolatility, Call, DivAmnt, DivYte, DivCount, NULL);
						double dResultCRdVolaMinus = OPM::CoxRossOddEvenAdjust(S, K, R, RF, dHTBrate, V - OPM::cdDeltaVolatility, Yte, Call, Steps, DivAmnt, DivYte, DivCount);

						if(!OPM::IsBadValue(dResultCRdVolaMinus)) 
						{
							double dResultCRBSdVolaMinus = (!OPM::IsBadValue(dResultBSdVolaMinus) && dResultBSdVolaMinus>dResultCRdVolaMinus) ? dResultBSdVolaMinus : dResultCRdVolaMinus;

							pGreeks->dVolga = (GreeksCRBSdVola.dTheoPrice - 2 * GreeksCR.dTheoPrice + dResultCRBSdVolaMinus) / (OPM::cdDeltaVolatility * OPM::cdDeltaVolatility);
							pGreeks->nMask |= GT_VOLGA;
						}
					}

				}
			}

			if(nMask & GT_RHO)
			{
				double dResultBSdRate = OPM::BlackAndScholes(R + OPM::cdDeltaRate, RF, dHTBrate, S, K, Yte, V, Call, DivAmnt, DivYte, DivCount, NULL);
				double dResultCRdRate = OPM::CoxRossOddEvenAdjust(S, K, R + OPM::cdDeltaRate, RF, dHTBrate, V, Yte, Call, Steps, DivAmnt, DivYte, DivCount);

				if(!OPM::IsBadValue(dResultCRdRate)) 
				{
					pGreeks->dRho = (!OPM::IsBadValue(dResultBSdRate) && dResultBSdRate>dResultCRdRate?dResultBSdRate:dResultCRdRate) - dResultCR;
					pGreeks->nMask |= GT_RHO;
				}
			}
		}	 
	}
    else
    {
		if(pGreeks)	
			memcpy(pGreeks, &GreeksBS, sizeof(GREEKS));
    }

	return max(dResultBS, dResultCR);
} 