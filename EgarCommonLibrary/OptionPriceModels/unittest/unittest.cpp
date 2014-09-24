
#include "stdafx.h"

#include <iostream>
#include <stdio.h>

using namespace std;


using namespace OPM;

void TestBlackAndScholes()
{
	const double	Rd		= 0.05;
	const double	S		= 50;
	const double	K		= 50;
	const double	T		= 90./365.;
	const double	V		= 0.40;
	const bool		IsCall	= true;

	double dCntRd	= RateDiscToCont(Rd, T * 365);

	double dPrice = BlackAndScholes(dCntRd, 0, S, K, T, V, IsCall);

	printf("Black and Scholes model: Price = %.10f\n", dPrice);

}

void TestCoxRoss()
{
	const double	Rd		= 0.05;
	const double	Rf		= 0.0;
	double	S		= 50;
	const double	K		= 50;
	const double	T		= 90./365.;
	const double	V		= 0.40;
	const bool		IsCall	= true;
	const long		Steps	= 10;

	double	cpdDivAmnts[2] = {0.1, 0.1};
	double	cpdDivYears[2] = {10/365., 30/365.};

	double dCntRd	= RateDiscToCont(Rd, T * 365);
	double dCntRf	= RateDiscToCont(Rf, T * 365);

	S = DiscountForDividends(S, dCntRd, cpdDivAmnts, cpdDivYears, 2, T);// Stock price w/o dividends

	double dPriceA = CoxRoss(S, K, dCntRd, 0., V, T, IsCall, Steps, cpdDivAmnts, cpdDivYears, 2);
	double dPriceB = CoxRoss(S, K, dCntRd, 0., V, T, IsCall, Steps - 1, cpdDivAmnts, cpdDivYears, 2);

	double dPrice = (dPriceA + dPriceB) / 2;

	printf("Cox and Ross model: Price = %.10f\n", dPrice);
}

int main(int argc, char* argv[])
{
	TestBlackAndScholes();
	TestCoxRoss();

	return 0;
}

