#include "Common.h"

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
				int		nDivCount)
{
	if (dYte < 0.) 
		return max(bIsCall ? dSpotPrice - dStrike : dStrike - dSpotPrice, 0.);

	double dContRd = RateDiscToCont(dRateDomestic , dYte * cdDaysPerYear365);
	double dContRf = RateDiscToCont(dRateForeign, dYte * cdDaysPerYear365);
	
	double dDiscountedSpot = DiscountForDividends(dSpotPrice,
												dContRd,
												0,			// borrowing rate
												pdDivAmnt,
												pdDivYte,
												nDivCount,
												dYte);

	double X0 = log(dStrike / dDiscountedSpot);
	double Mu = (dContRd - dContRf - dVolatility*dVolatility/2) * dYte;
	
	double Sigma	= dVolatility * sqrt(dYte);
	double Sigma2	= Sigma*Sigma;
	
	double Lambda3	= - dSkew * Sigma2*Sigma/6;
	double Lambda4	= dKurtosis * Sigma2*Sigma2/24;
	
	double X	= (X0 - Mu) / Sigma;
	double P_	= (Mu - X0) / Sigma2;
	double P0	= exp(-X*X/2) / (sqrt(2*Pi)*Sigma);
	double P1	= P_ * P0;
	double P2	= (P_*P_ - 1/Sigma2) * P0;
	
	// Calculate price by Black and Scholes model
	double PriceBS	= BlackAndScholes(dRateDomestic, dRateForeign, BadDoubleValue, dSpotPrice, dStrike, dYte, dVolatility, true, pdDivAmnt, pdDivYte, nDivCount);

	if(IsBadValue(PriceBS))
		return BadDoubleValue;

	// Adjust BS price by VSK suggestions
	double Price = PriceBS	+	dStrike * exp(-dContRd*dYte) * (Lambda4*P2 + (Lambda4 - Lambda3) * (P0 - P1))
							+	dDiscountedSpot * exp(-dContRf*dYte) * (Lambda4 - Lambda3) * NormalC((Mu + Sigma2 -X0)/Sigma);

	if(!IsValidTheoPrice(Price))
		return BadDoubleValue;


	// Additional adjustment if there is a put option
	if (!bIsCall) 
	{
		Price = Price - dDiscountedSpot*exp(-dContRf*dYte)*(1 - Lambda3 + Lambda4) + dStrike*exp(-dContRd*dYte);

		if(!IsValidTheoPrice(Price))
			return BadDoubleValue;
	}

	return Price;
}

}
