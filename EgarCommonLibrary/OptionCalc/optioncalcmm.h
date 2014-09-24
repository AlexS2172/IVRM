#ifndef __OPTION_CALC_MM__
#define __OPTION_CALC_MM__

#include <OptionCalc\OptionCalc.h>

double OPTIONCALC_API CalcVolatilityMM(	double	dDomesticRate,	
										double	dForeignRate, 
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
										long	nModel);

long OPTIONCALC_API CalcGreeksMM(	double	dDomesticRate,
									double	dForeignRate,		
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
									long 	nModel,
									GREEKS*	pGreeks);
#endif //__OPTION_CALC_MM__

