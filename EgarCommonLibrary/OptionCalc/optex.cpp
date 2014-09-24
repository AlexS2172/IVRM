#include "stdafx.h"

#include <OptionCalc\OptionCalc.h>
#include "optex.h"

double CalculateOptionEx(	
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
						long	Steps,
						GREEKS *pGreeks
						)
{
	if (!American)
		return CO_BlackScholes(
						R,						
						RF,	
						OPM::BadDoubleValue,
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
						OPM::BadDoubleValue,
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
