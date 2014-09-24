#include "stdafx.h"
//=============================================================================
// Copyright 2000-2005
//
// Author	: Eugene Karnygin
// File		: VarianceSwap.cpp
// Created	: 6 march 2006
//
//=============================================================================

#include "OptionPriceModels\Common.h"

#include <windows.h>
#include <malloc.h>

#include <map>

namespace OPM
{

namespace VarianceSwap             
{

double WINAPI ImpliedVariance(	DATE	dtValuationDate,
  double	dInterestRate,
  double	dSpotPrice,
  double	dDividendYield,
  double*	pdDividendAmounts,
  DATE*	pdtDividendDates,
  long	nDividendCount,
  DATE	dtExpirationDate,
  double* pdStrikePrices,
  double*		pdCallVolatilities,
  double*		pdPutVolatilities,
  long	nSkewSize
  );
  
int		WINAPI VarianceSwapGreeks(	DATE		dtValuationDate,
				double		dInterestRate,
				double		dSpotPrice,
				double		dDividendYield,
				double*		pdDividendAmounts,
				DATE*		pdtDividendDates,
				unsigned	nDividendCount,
				DATE		dtExpirationDate,
				double*		pdStrikePrices,
				double*		pdCallVolatilities,
				double*		pdPutVolatilities,
				long		nSkewSize,
				VSGREEKS&		rReturnValue)
{
	
	double* pdCallVolatilitiesUp	= NULL;
	double* pdCallVolatilitiesDown	= NULL;
  double* pdPutVolatilitiesUp	= NULL;
  double* pdPutVolatilitiesDown	= NULL;
  double* pdCallVolatilities2Up = NULL;
  double* pdCallVolatilities2Down = NULL;
  double* pdPutVolatilities2Up = NULL;
  double* pdPutVolatilities2Down = NULL;
	__try
	{
		pdCallVolatilitiesUp	= (double*) _alloca(nSkewSize * sizeof(double));
		pdCallVolatilitiesDown	= (double*) _alloca(nSkewSize * sizeof(double));
    pdPutVolatilitiesUp	= (double*) _alloca(nSkewSize * sizeof(double));
    pdPutVolatilitiesDown	= (double*) _alloca(nSkewSize * sizeof(double));
    
    pdCallVolatilities2Up	= (double*) _alloca(nSkewSize * sizeof(double));
    pdCallVolatilities2Down	= (double*) _alloca(nSkewSize * sizeof(double));
    pdPutVolatilities2Up	= (double*) _alloca(nSkewSize * sizeof(double));
    pdPutVolatilities2Down	= (double*) _alloca(nSkewSize * sizeof(double));
	}
	__except(GetExceptionCode() == STATUS_STACK_OVERFLOW)
	{
		throw (__FUNCTION__ "Fatal error! Stack overflow");		
	}

	double dVolatilityShift = 0.01;

	for (int i = 0; i < nSkewSize; i++)
	{
		pdCallVolatilitiesUp[i]		= pdCallVolatilities[i] + dVolatilityShift; // Shift up 
		pdCallVolatilitiesDown[i]	= pdCallVolatilities[i] - dVolatilityShift; // Shift down
    pdPutVolatilitiesUp[i]		= pdPutVolatilities[i] + dVolatilityShift; // Shift up 
    pdPutVolatilitiesDown[i]	= pdPutVolatilities[i] - dVolatilityShift; // Shift down
    
    pdCallVolatilities2Up[i]		= pdCallVolatilitiesUp[i] + dVolatilityShift; // double shift up 
    pdCallVolatilities2Down[i]	= pdCallVolatilitiesDown[i] - dVolatilityShift; // double shift down
    pdPutVolatilities2Up[i]		= pdPutVolatilitiesUp[i] + dVolatilityShift; // double shift up 
    pdPutVolatilities2Down[i]	= pdPutVolatilitiesDown[i] - dVolatilityShift; // double shift down
	}

	double dValue	= ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice, 
										dDividendYield, pdDividendAmounts, pdtDividendDates, nDividendCount,
										dtExpirationDate, pdStrikePrices, pdCallVolatilities, pdPutVolatilities, nSkewSize);

	double dSpotPriceShift = dSpotPrice * 0.01; 

	double dValueSpotPriceUp	= ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice + dSpotPriceShift, 
													dDividendYield, pdDividendAmounts, pdtDividendDates, nDividendCount,
													dtExpirationDate, pdStrikePrices, pdCallVolatilities, pdPutVolatilities, nSkewSize);

  double dValueSpotUpVolaUp =  ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice+ dSpotPriceShift, dDividendYield, 
                                                pdDividendAmounts, pdtDividendDates, nDividendCount, dtExpirationDate, 
                                                pdStrikePrices, pdCallVolatilitiesUp, pdPutVolatilitiesUp, nSkewSize);
                                                
  double dValueSpotDownVolaUp =  ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice - dSpotPriceShift, dDividendYield,
                                                  pdDividendAmounts, pdtDividendDates, nDividendCount, dtExpirationDate,
                                                  pdStrikePrices, pdCallVolatilitiesUp, pdPutVolatilitiesUp, nSkewSize);
                     
  double dValueSpotUpVolaDown =  ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice+ dSpotPriceShift, dDividendYield, 
                                                  pdDividendAmounts, pdtDividendDates, nDividendCount, dtExpirationDate, 
                                                  pdStrikePrices, pdCallVolatilitiesDown, pdPutVolatilitiesDown, nSkewSize);

  double dValueSpotDownVolaDown =  ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice - dSpotPriceShift, dDividendYield,
                                                    pdDividendAmounts, pdtDividendDates, nDividendCount, dtExpirationDate,
                                                    pdStrikePrices, pdCallVolatilitiesDown, pdPutVolatilitiesDown, nSkewSize);
    
  double dValueVolatilities2Up  = ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice, dDividendYield, 
                                                  pdDividendAmounts, pdtDividendDates, nDividendCount, dtExpirationDate, 
                                                  pdStrikePrices, pdCallVolatilities2Up, pdPutVolatilities2Up, nSkewSize); 
  
  double dValueVolatilities2Down = ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice, dDividendYield, 
                                                  pdDividendAmounts, pdtDividendDates, nDividendCount, dtExpirationDate, 
                                                  pdStrikePrices, pdCallVolatilities2Down, pdPutVolatilities2Down, nSkewSize); 
                                                
	double dValueSpotPriceDown	= ImpliedVariance(	dtValuationDate, dInterestRate, dSpotPrice - dSpotPriceShift, 
													dDividendYield, pdDividendAmounts, pdtDividendDates, nDividendCount,
													dtExpirationDate, pdStrikePrices, pdCallVolatilities, pdPutVolatilities, nSkewSize);

	double dValueYesterdayValuationDate	= ImpliedVariance(	dtValuationDate - 1, dInterestRate, dSpotPrice, 
															dDividendYield, pdDividendAmounts, pdtDividendDates, nDividendCount,
															dtExpirationDate, pdStrikePrices, pdCallVolatilities, pdPutVolatilities, nSkewSize);
	
	double dValueInterestRateUp	=	ImpliedVariance(	dtValuationDate, dInterestRate + 0.01, dSpotPrice, 
															dDividendYield, pdDividendAmounts, pdtDividendDates, nDividendCount,
															dtExpirationDate, pdStrikePrices, pdCallVolatilities, pdPutVolatilities, nSkewSize);

	double dValueVolatilitiesUp		= ImpliedVariance(	dtValuationDate,
														dInterestRate,
														dSpotPrice,
														dDividendYield,
														pdDividendAmounts,
														pdtDividendDates,
														nDividendCount,
														dtExpirationDate,
														pdStrikePrices,
														pdCallVolatilitiesUp, pdPutVolatilitiesUp,
														nSkewSize);

	double dValueVolatilitiesDown	= ImpliedVariance(	dtValuationDate,
														dInterestRate,
														dSpotPrice,
														dDividendYield,
														pdDividendAmounts,
														pdtDividendDates,
														nDividendCount,
														dtExpirationDate,
														pdStrikePrices,
														pdCallVolatilitiesDown, pdPutVolatilitiesDown,
														nSkewSize);

	/*double dVegaUp		= (dValueVolatilitiesUp - dValue) / dVolatilityShift;
	double dVegaDown	= (dValue - dValueVolatilitiesDown) / dVolatilityShift;*/
	
  double dVegaUp		= dValueVolatilitiesUp - dValue;
  double dVegaDown	= dValue - dValueVolatilitiesDown;
  
	/*double dDeltaUp		= (dValueSpotPriceUp - dValue) / dSpotPriceShift;
	double dDeltaDown	= (dValue - dValueSpotPriceDown) / dSpotPriceShift;*/
	
  double dDeltaUp		= dValueSpotPriceUp - dValue;
  double dDeltaDown	= dValue - dValueSpotPriceDown;

	rReturnValue.dTheoPrice = dValue;
	rReturnValue.dDelta		= (dDeltaUp + dDeltaDown) / 2;
	rReturnValue.dDeltaDown = dDeltaDown;
	rReturnValue.dDeltaUp = dDeltaUp;
	rReturnValue.dGamma		= dDeltaUp - dDeltaDown;
	rReturnValue.dVega		= (dVegaUp + dVegaDown) / 2;
	rReturnValue.dVegaDown = dVegaDown;
	rReturnValue.dVegaUp = dVegaUp;
	rReturnValue.dTheta		= dValueYesterdayValuationDate - dValue;
	rReturnValue.dRho		= dValueInterestRateUp - dValue;
	//rReturnValue.dVomma		= dVegaUp - dVegaDown;
	rReturnValue.dVommaDown = (dValueVolatilities2Down + dValueVolatilitiesUp - dValueVolatilitiesDown - dValue)/2;
	rReturnValue.dVommaUp = (dValueVolatilities2Up+dValueVolatilitiesDown-dValueVolatilitiesUp-dValue)/2;
	rReturnValue.dVomma = (rReturnValue.dVommaUp + rReturnValue.dVommaDown )/2;
	
	rReturnValue.dVannaUp = (dValueSpotUpVolaUp - dValueVolatilitiesUp - dValueSpotUpVolaDown + dValueVolatilitiesDown)/2;
	rReturnValue.dVannaDown = (dValueVolatilitiesUp - dValueSpotDownVolaUp - dValueVolatilitiesDown + dValueSpotDownVolaDown)/2;
	rReturnValue.dVanna = ( rReturnValue.dVannaUp + rReturnValue.dVannaDown )/ 2;
	
	rReturnValue.dDeltaVega = 0;

	return 0;
}

double WINAPI RealizedVariance(DATE*		pDates,
						double*		pPrices,
						unsigned	nDaysCount,
						DATE		dtFrom,
						DATE		dtTo,
						double*		pdDividendAmounts,
						DATE*		pdtDividendDates,
						unsigned	nDividendsCount)
{
	// Validate input parameters
	if (pDates == NULL || pPrices == NULL)
		return 0;

	double	dAccumulator	= 0;
	int		nActualDays		= 0;

	std::map<DATE, double> mapData;

	for (unsigned i = 1; i < nDaysCount; i++)
	{
		DATE	dtDate = pDates[i];
		double	dPrice = pPrices[i];

		if (dtDate <= 0 || dPrice <= 0)			// Price data is invalid
			continue;

		if (dtDate < dtFrom || dtDate > dtTo)	// Date is out of specified range
			continue;
		
		mapData[dtDate] = dPrice;
	}

	if (mapData.size() <= 1)
		return 0;	// Variance is zero in this case

	for (unsigned i = 0; i < nDividendsCount; i++)
	{
		DATE	dtDividendDate = pdtDividendDates[i];
		double	dDividendAmount = pdDividendAmounts[i];

		if (dtDividendDate <= 0 || dDividendAmount <= 0)	// Dividend date is invalid
			continue;

		if (dtDividendDate < dtFrom || dtDividendDate > dtTo)	// Date is out of specified range
			continue;
		
		if (mapData.find(dtDividendDate) != mapData.end())
			mapData[dtDividendDate] -= dDividendAmount;
	}

	std::map<DATE, double>::iterator it = mapData.begin();

	double dPreviousPrice = it->second;
	
	for (it++; it != mapData.end(); it++)
	{

		double dPrice	= it->second;
		double dReturn	= dPrice / dPreviousPrice;

		dAccumulator	+= log(dReturn) * log(dReturn);
		dPreviousPrice	= dPrice;
	}

	double dBasis = 252. / (mapData.size() - 1); 
	
	return dBasis * dAccumulator;
}

double WINAPI ImpliedVariance(	DATE	dtValuationDate,
						double	dInterestRate,
						double	dSpotPrice,
						double	dDividendYield,
						double*	pdDividendAmounts,
						DATE*	pdtDividendDates,
						long	nDividendCount,
						DATE	dtExpirationDate,
						double* pdStrikePrices,
            double*		pdCallVolatilities,
            double*		pdPutVolatilities,
						long	nSkewSize
)
{
	if (pdStrikePrices == NULL || pdStrikePrices == NULL || NULL == pdPutVolatilities || NULL == pdCallVolatilities)
		throw (__FUNCTION__ "Invalid parameter(s). NULL pointers");

	double dForwardPrice = 0;

	double dYearsToExpiry = (dtExpirationDate - dtValuationDate) / cdDaysPerYear365;

	if (dYearsToExpiry < 0)
		return 0;

	if (pdDividendAmounts != NULL && pdtDividendDates != NULL && nDividendCount > 0)
	{
		double dDividendsPresentValue = OPM::DividendNPV(	dtValuationDate,	
																dInterestRate,
																BadDoubleValue,
																pdDividendAmounts,
																pdtDividendDates,
																nDividendCount,
																dtExpirationDate);

		dForwardPrice = (dSpotPrice - dDividendsPresentValue) * exp(dInterestRate * dYearsToExpiry);
	}
	else
	{
		dForwardPrice = dSpotPrice * exp((dInterestRate - dDividendYield) * dYearsToExpiry);
	}

	double* pdX = NULL; 
	double* pdY = NULL; 
	
	__try
	{
		pdX = (double*) _alloca(nSkewSize * sizeof(double));
		pdY = (double*) _alloca(nSkewSize * sizeof(double));
	}
	__except(GetExceptionCode() == STATUS_STACK_OVERFLOW)
	{
		throw (__FUNCTION__ "Fatal error! Stack overflow");		
	}

	double dSumm = 0;
  double *pdVolatilities;
  
	for (int i = 0; i < nSkewSize; i++)
	{
	  if ( pdStrikePrices[i] > dForwardPrice ) 
	    pdVolatilities = pdCallVolatilities;
	  else
	    pdVolatilities = pdPutVolatilities;
	    
		pdX[i] = log(pdStrikePrices[i]/dForwardPrice) / ( pdVolatilities[i] * sqrt(dYearsToExpiry) ) +
				0.5 * pdVolatilities[i] * sqrt(dYearsToExpiry);
		
		pdY[i] = exp(-pdX[i]*pdX[i] * 0.5) * pdVolatilities[i]*pdVolatilities[i];
			
		if (i > 0)
		{
			dSumm += abs(pdX[i] - pdX[i-1]) * (pdY[i-1] + pdY[i]) * 0.5;
		}
	}

	return 10000. / sqrt(2*Pi) * dSumm; 
}

} // namespace VarianceSwap

}