#include "stdafx.h"
#include "FutureModels.h"
#include "Models.h"

double CFO_BlackScholes(	
						double	R,
						double	S,
						double	K,
						double	V,
						double	Yte, 
                        bool	Call,
						bool	American,
						double*	pDivAmnts,
						double*	pDivYears,
						long	nCount,
						GREEKS *pGreeks
						)
{
	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_BlackScholes(R, R, OPM::BadDoubleValue, S, K, V, Yte, Call, American, pDivAmnts, pDivYears, nCount, pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{
		pGreeks->dRho = - Yte *  dTheoPrice * OPM::cdDeltaRate;
		pGreeks->nMask |= GT_RHO;
	}

	if(!OPM::SpotGreeksFromForward(R, Yte, pGreeks) && pGreeks)
		pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;

	return dTheoPrice;
} 

double CFO_VskLog(
						double	R,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double	Skew,
						double	Kurtosis,
						double*	pDivAmnts,
						double*	pDivYears,
						long	nCount,
						GREEKS *pGreeks)

{	

	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_VskLog(R, R, S, K, V, Yte, Call, American, pDivAmnts, pDivYears, nCount, Skew, Kurtosis, pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{
	
		double dTheoPricePlusDr = OPM::VSKLog( 
						S,
						K,
						R + OPM::cdDeltaRate,
						R + OPM::cdDeltaRate,
						V,
						Yte,
						Call,
						Skew,
						Kurtosis, 
						NULL, 
						NULL, 
						0);

		if(!OPM::IsBadValue(dTheoPricePlusDr))
		{
			pGreeks->dRho = dTheoPricePlusDr - dTheoPrice;
			pGreeks->nMask |= GT_RHO;
		}
	}

	if(!OPM::SpotGreeksFromForward(R, Yte, pGreeks) && pGreeks)
		pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;

	return dTheoPrice;
}


double CFO_CoxRossWithBlackScholes(	
							double	R,
						    double	S,
						    double	K,
						    double	V,
						    double	Yte, 
                            bool	Call,
						    bool	American,
						    long	Steps,
							double*	pDivAmnts,
							double*	pDivYears,
							long	nCount,
							GREEKS *pGreeks
                            )
{
	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_CoxRossWithBlackScholes(	R, 
													R, 
													OPM::BadDoubleValue,
													S, 
													K, 
													V, 
													Yte, 
													Call, 
													American, 
													pDivAmnts,
													pDivYears,
													nCount,
													Steps, 
													pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{
	
		GREEKS plGreeks = *pGreeks;
		double dTheoPricePlusDr = CO_CoxRossWithBlackScholes(	R + OPM::cdDeltaRate, 
																R + OPM::cdDeltaRate, 
																OPM::BadDoubleValue,
																S, 
																K, 
																V, 
																Yte, 
																Call, 
																American, 
																pDivAmnts,
																pDivYears,
																nCount,
																Steps, 
																&plGreeks);

		if(!OPM::IsBadValue(dTheoPricePlusDr))
		{
			pGreeks->dRho = dTheoPricePlusDr - dTheoPrice;
			pGreeks->nMask |= GT_RHO;
		}
	}

	if(!OPM::SpotGreeksFromForward(R, Yte, pGreeks) && pGreeks)
		pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;

	return dTheoPrice;

} 

double CFO_BlackScholes2(	
						double	R,
						double	S,
						bool	bSpotPrice,
						double	K,
						double	V,
						double	Yte, 
						bool	Call,
						bool	American,
						double*	pDivAmnts,
						double*	pDivYears,
						long	nCount,
						GREEKS *pGreeks
						)
{
	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_BlackScholes(R, R, OPM::BadDoubleValue, S, K, V, Yte, Call, American, pDivAmnts, pDivYears, nCount, pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{
		pGreeks->dRho = - Yte *  dTheoPrice * OPM::cdDeltaRate;
		pGreeks->nMask |= GT_RHO;
	}

	if (bSpotPrice)
	{
		if(!OPM::SpotGreeksFromForward(R, Yte, pGreeks) && pGreeks)
			pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;
	
	}

	return dTheoPrice;
} 

double CFO_VskLog2(
				  double	R,
				  double	S,
				  bool		bSpotPrice,
				  double	K,
				  double	V,
				  double	Yte, 
				  bool	Call,
				  bool	American,						 
				  double	Skew,
				  double	Kurtosis,
				  double*	pDivAmnts,
				  double*	pDivYears,
				  long	nCount,
				  GREEKS *pGreeks)

{	

	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_VskLog(R, R, S, K, V, Yte, Call, American, pDivAmnts, pDivYears, nCount, Skew, Kurtosis, pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{

		double dTheoPricePlusDr = OPM::VSKLog( 
			S,
			K,
			R + OPM::cdDeltaRate,
			R + OPM::cdDeltaRate,
			V,
			Yte,
			Call,
			Skew,
			Kurtosis, 
			NULL, 
			NULL, 
			0);

		if(!OPM::IsBadValue(dTheoPricePlusDr))
		{
			pGreeks->dRho = dTheoPricePlusDr - dTheoPrice;
			pGreeks->nMask |= GT_RHO;
		}
	}

	if (bSpotPrice)
	{
		if(!OPM::SpotGreeksFromForward(R, Yte, pGreeks) && pGreeks)
			pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;
	}

	return dTheoPrice;
}


double CFO_CoxRossWithBlackScholes2(	
								   double	R,
								   double	S,
								   bool		bSpotPrice,
								   double	K,
								   double	V,
								   double	Yte, 
								   bool	Call,
								   bool	American,
								   long	Steps,
								   double*	pDivAmnts,
								   double*	pDivYears,
								   long	nCount,
								   GREEKS *pGreeks
								   )
{
	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_CoxRossWithBlackScholes(	R, 
		R, 
		OPM::BadDoubleValue,
		S, 
		K, 
		V, 
		Yte, 
		Call, 
		American, 
		pDivAmnts,
		pDivYears,
		nCount,
		Steps, 
		pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{

		GREEKS plGreeks = *pGreeks;
		double dTheoPricePlusDr = CO_CoxRossWithBlackScholes(	R + OPM::cdDeltaRate, 
			R + OPM::cdDeltaRate, 
			OPM::BadDoubleValue,
			S, 
			K, 
			V, 
			Yte, 
			Call, 
			American, 
			pDivAmnts,
			pDivYears,
			nCount,
			Steps, 
			&plGreeks);

		if(!OPM::IsBadValue(dTheoPricePlusDr))
		{
			pGreeks->dRho = dTheoPricePlusDr - dTheoPrice;
			pGreeks->nMask |= GT_RHO;
		}
	}

	if (bSpotPrice)
	{
		if(!OPM::SpotGreeksFromForward(R, Yte, pGreeks) && pGreeks)
			pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;
	}

	return dTheoPrice;

} 




//////////////////////////////////////////////////////////
// versions with dYield/bSpotPrice switch and with divs

double CFO_BlackScholes3(	
						 double	R,
						 double	dYield,
						 bool		bSpotPrice,
						 double	S,
						 double	K,
						 double	V,
						 double Yte, 
						 bool	Call,
						 bool	American,
						 double*	pDivAmnts,
						 double*	pDivYears,
						 long	nCount,
						 GREEKS *pGreeks
						 )
{
	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_BlackScholes(R, R, OPM::BadDoubleValue, S, K, V, Yte, Call, American, NULL, NULL, 0, pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{
		pGreeks->dRho = - Yte *  dTheoPrice * OPM::cdDeltaRate;
		pGreeks->nMask |= GT_RHO;
	}

	if (bSpotPrice)
	{
		if(!OPM::SpotGreeksFromForward2(S, R, dYield, Yte, pDivAmnts, pDivYears, nCount, pGreeks) && pGreeks)
			pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;
	}


	return dTheoPrice;
} 

double CFO_VskLog3(
				   double	R,
				   double	dYield,
				   bool		bSpotPrice,
				   double	S,
				   double	K,
				   double	V,
				   double	Yte, 
				   bool	Call,
				   bool	American,						 
				   double	Skew,
				   double	Kurtosis,
				   double*	pDivAmnts,
				   double*	pDivYears,
				   long	nCount,
				   GREEKS *pGreeks)

{	

	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_VskLog(R, R, S, K, V, Yte, Call, American,  NULL, NULL, 0, Skew, Kurtosis, pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{

		double dTheoPricePlusDr = OPM::VSKLog( 
			S,
			K,
			R + OPM::cdDeltaRate,
			R + OPM::cdDeltaRate,
			V,
			Yte,
			Call,
			Skew,
			Kurtosis, 
			NULL, 
			NULL, 
			0);

		if(!OPM::IsBadValue(dTheoPricePlusDr))
		{
			pGreeks->dRho = dTheoPricePlusDr - dTheoPrice;
			pGreeks->nMask |= GT_RHO;
		}
	}

	if (bSpotPrice)
	{
		if(!OPM::SpotGreeksFromForward2(S, R, dYield, Yte, pDivAmnts, pDivYears, nCount, pGreeks) && pGreeks)
			pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;
	}

	return dTheoPrice;
}


double CFO_CoxRossWithBlackScholes3(	
									double	R,
									double	dYield,
									bool	bSpotPrice,
									double	S,
									double	K,
									double	V,
									double  Yte, 
									bool	Call,
									bool	American,
									long	Steps,
									double*	pDivAmnts,
									double*	pDivYears,
									long	nCount,
									GREEKS *pGreeks
									)
{
	bool bCalcRho = false;
	if(pGreeks && pGreeks->nMask & GT_RHO)
	{
		bCalcRho = true;
		pGreeks->nMask |=  ~GT_RHO;
	}

	double dTheoPrice = CO_CoxRossWithBlackScholes(	R, 
		R,
		OPM::BadDoubleValue,
		S, 
		K, 
		V, 
		Yte, 
		Call, 
		American, 
		NULL,
		NULL,
		0,
		Steps, 
		pGreeks);

	if(!OPM::IsBadValue(dTheoPrice) && bCalcRho && pGreeks)
	{

		GREEKS plGreeks = *pGreeks;
		double dTheoPricePlusDr = CO_CoxRossWithBlackScholes(	R + OPM::cdDeltaRate, 
			R + OPM::cdDeltaRate, 
			OPM::BadDoubleValue,
			S, 
			K, 
			V, 
			Yte, 
			Call, 
			American, 
			NULL,
			NULL,
			0,
			Steps, 
			&plGreeks);

		if(!OPM::IsBadValue(dTheoPricePlusDr))
		{
			pGreeks->dRho = dTheoPricePlusDr - dTheoPrice;
			pGreeks->nMask |= GT_RHO;
		}
	}

	if (bSpotPrice)
	{
		if(!OPM::SpotGreeksFromForward2(S, R, dYield, Yte, pDivAmnts, pDivYears, nCount, pGreeks) && pGreeks)
			pGreeks->nMask = OPM::IsBadValue(dTheoPrice) ? GT_NOTHING : GT_THEOPRICE;
	}

	return dTheoPrice;

} 



// versions with dYield/bSpotPrice switch and with divs
//////////////////////////////////////////////////////////

