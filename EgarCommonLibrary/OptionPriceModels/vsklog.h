#ifndef __VSK_LOG_H__
#define __VSK_LOG_H__

namespace OPM
{

double VSKLog(	
				double	dSpotPrice,
				double	dStrike,
				double	dRateDomestic,
				double	dRateForeign,
				double	dVolatility,
				double	dYte,
				bool	bIsCall,
				double	dSkew,
				double	dKurtosis,
				double* pdDivAmnt,						   
				double* pdDivYte,						    
				int		nDivCount
				);

}

#endif // __VSK_LOG_H__
