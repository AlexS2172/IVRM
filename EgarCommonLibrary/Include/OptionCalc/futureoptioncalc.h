#ifndef __FUTURE_OPTION_CALC__
#define __FUTURE_OPTION_CALC__

//#include "Models.h"
#include <OptionCalc\OptionCalc.h>
#include <OptionCalc\DataTypes.h>

double OPTIONCALC_API CalcFutureOptionVolatility(double	dDomesticRate,	
										double	dFuturePrice,
										double	dOptionPrice,
										double	dStrike,
										long	nDTE,
										long	nIsCall,
										long	nIsAmerican,
										long	nSteps,
										double	dSkew,
										double	dKurtosis,
										long	nModel, 
										long*   pnFlag);


double OPTIONCALC_API CalcFutureOptionVolatilityCustDivs(	double	dDomesticRate,	
											double	dFuturePrice,
											double	dOptionPrice,
											double	dStrike,
											long	nDTE,
											long	nIsCall,
											long	nIsAmerican,
											long	nSteps,
											double	dSkew,
											double	dKurtosis,
											long	nCount,
											double *pDivAmnts,
											double *pDivYears,
											long	nModel,
                                            long*   pnFlag);

/*

	// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
	long OPTIONCALC_API CalcGreeks( double dDomesticRate, double dForeignRate, 
									double dSpotPrice, double dStrike, double dVolatility, 
									long nDTE, long nIsCall, long nIsAmerican, 
									long nCount, const DIVIDEND *pDividends, long nSteps, 
									GREEKS *pGreeks );

*/								

long OPTIONCALC_API CalcFutureOptionGreeks(	double	dDomesticRate,
									double	dFuturePrice,
									double	dStrike,
									double	dVolatility, 
									long	nDTE,
									long	nIsCall,
									long	nIsAmerican,
									long	nSteps,				
									double	dSkew,
									double	dKurtosis,
									long 	nModel,
									GREEKS*	pGreeks);


long OPTIONCALC_API CalcFutureOptionGreeksCustDivs(	double	dDomesticRate,
									double	dFuturePrice,
									double	dStrike,
									double	dVolatility, 
									long	nDTE,
									long	nIsCall,
									long	nIsAmerican,
									long	nSteps,				
									double	dSkew,
									double	dKurtosis,
									long 	nModel,
									long	nCount,
									double  *pDivAmnts,
									double  *pDivYears,
									GREEKS*	pGreeks);


long OPTIONCALC_API CalcFutureOptionGreeks2(	double	dDomesticRate,
										   double	dFuturePrice,
										   bool bSpotGreeks,
										   double	dStrike,
										   double	dVolatility, 
										   long	nDTE,
										   long	nIsCall,
										   long	nIsAmerican,
										   long	nSteps,				
										   double	dSkew,
										   double	dKurtosis,
										   long 	nModel,
										   GREEKS*	pGreeks);


long OPTIONCALC_API CalcFutureOptionGreeksCustDivs2(	double	dDomesticRate,
												   double	dFuturePrice,
												   bool bSpotGreeks,
												   double	dStrike,
												   double	dVolatility, 
												   long	nDTE,
												   long	nIsCall,
												   long	nIsAmerican,
												   long	nSteps,				
												   double	dSkew,
												   double	dKurtosis,
												   long 	nModel,
												   long	nCount,
												   double  *pDivAmnts,
												   double  *pDivYears,
												   GREEKS*	pGreeks);


// a version with [custom] dividends, with Spot/Forward switching 
// for commodities and with Yield using...

long OPTIONCALC_API CalcFutureOptionGreeks3(	double	dDomesticRate,
											double	dYield,
											double	dFuturePrice,
											bool		bSpotPrice,
											double	dStrike,
											double	dVolatility, 
											long	nDTE,
											long	nIsCall,
											long	nIsAmerican,
											long	nSteps,				
											double	dSkew,
											double	dKurtosis,
											long 	nModel,
											long	nCount,
											double *pDivAmnts,
											double *pDivYears,
											GREEKS*	pGreeks);


#endif //__FUTURE_OPTION_CALC__

