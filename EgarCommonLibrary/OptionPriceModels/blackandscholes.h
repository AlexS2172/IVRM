#ifndef __BLACK_AND_SCHOLES_H__
#define __BLACK_AND_SCHOLES_H__

namespace OPM
{

double BlackAndScholes(	double	dRateDomestic,
						double	dRateForeign,
						double	dHTBRate,
						double	dSpotPrice,
						double	dStrike,
						double	dYte,
						double	dVolatility,
						bool	bIsCall,
						double* pdDivAmnt,						   
						double* pdDivYte,						    
						int		nDivCount,
						GREEKS*	pGreeks = NULL/*out*/
						);

}

#endif	__BLACK_AND_SCHOLES_H__