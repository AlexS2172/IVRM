#include "Common.h"

namespace OPM
{

inline double DividendPv(double dDivAmnt, double dCntRate, double dCntBorrowingRate, double dTime)
{
	return dDivAmnt * exp(-(dCntRate - dCntBorrowingRate) * dTime);
}


double DiscountForDividends(double		S, 
							double		R,
							double		B,			//actual stock borrowing rate
							double*		pdDivAmnt,
							double*		pdDivYears,
							unsigned	nDivCount,
							double		T)
{
	return (S - DividendNPV(S, R, B, pdDivAmnt, pdDivYears, nDivCount, T));
}

double			DividendNPV(double		S, 
							double		R,
							double		B,			//actual stock borrowing rate
							double*		pdDivAmnt,
							double*		pdDivYears,
							unsigned	nDivCount,
							double		T)
{
	
	double SumPV = 0;

	for (unsigned n = 0; n < nDivCount; n++) 
	{
		bool IsGood = (pdDivYears[n] >= 0 && pdDivYears[n] <= T && pdDivAmnt[n]>0);
		
		if (IsGood) 
		{
			SumPV += DividendPv(pdDivAmnt[n], R, B, pdDivYears[n]);
			
			if (SumPV > S)
				return BadDoubleValue;
       	}
	}

	return SumPV;
}

}