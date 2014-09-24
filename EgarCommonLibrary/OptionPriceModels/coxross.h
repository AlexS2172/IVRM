//=============================================================================
// Copyright 2001, Eugene Karnygin
//
// Author	: Eugene Karnygin
// File		: CoxRoss.h
// Created	: 15 july 2001
// 
// The Cox Ross Rubenstein model (Binomial) of theoretical option price
//=============================================================================

#ifndef __COX_ROSS_H__
#define __COX_ROSS_H__

namespace OPM
{

//=============================================================================
// Max. and min. steps of binomial tree 
//=============================================================================
const long cnTreeStepsMax = 303;
const long cnTreeStepsMin = 5;

//============================================================================= 
// This function implement the binomial option pricing model (Cox-Ross-Rubinstein).
//
// Return value: Option price
// 
// if function failed it return BadDoubleValue	  
//=============================================================================
double CoxRoss(	double	S,		// Underlying spot price
				double	K,		// Strike price
				double	Rd,		// Domestic continuos risk free rate
				double	Rf,		// Foreign continuos risk free rate (or yield for index)
				double	Rb,		// Actual borrowing continuous rate
				double	V,		// Volatility
				double	T,		// Years amount till expiration
				bool	IsCall,	// true if it's 'Call option', false is 'Put option'
				long	Steps,	// Amount of binomial tree steps
				double*	DA,		// Array of dividend's amount
				double*	DT,		// Array of years till dividend payment
				long	DC,		// Count of dividends
				GREEKS*	pGreeks	= NULL /*out*/
				);

//============================================================================= 
// This function implement the Cox-Ross-Rubinstein with odd-even adjustments
// option pricing model .
//
// Return value: Option price
// 
// if function failed it return BadDoubleValue	  
//=============================================================================
double CoxRossOddEvenAdjust(
				double	dSpotPrice,
				double	dStrike,
				double	dRateDomestic,
				double	dRateForeign,
				double	dHTBRate,
				double	dVolatility,
				double	dYte,
				bool	bIsCall,
				long	nSteps,	// Amount of binomial tree steps
				double* pdDivAmnt,						   
				double* pdDivYte,						    
				int		nDivCount,
				GREEKS*	pGreeks = NULL /*out*/
				);

};
#endif __COX_ROSS_H__
