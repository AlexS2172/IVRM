#ifndef __OPTION_CALC_MM2__
#define __OPTION_CALC_MM2__

#include "Models.h"
#include <OptionCalc\OptionCalc.h>

double OPTIONCALC_API CalcVolatilityMM2(double	dDomesticRate,	
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
										long	nModel);

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
									long 	nModel,
									GREEKS*	pGreeks);


#endif// __OPTION_CALC_MM2__

