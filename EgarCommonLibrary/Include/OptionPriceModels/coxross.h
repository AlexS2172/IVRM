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

#include "Common.h"
#include "Exception.h"

//=============================================================================
// Max. and min. steps of binomial tree 
//=============================================================================
const long cnTreeStepsMax = 300;
const long cnTreeStepsMin = 5;

//============================================================================= 
// This function implement the binomial option pricing model.
// There is just year payment (Yield for index option or foreign rate 
// for FX option).
//
// Return value: Option price
// 
// if function failed it instanciates an exception	  
//=============================================================================
double CoxRoss(	double	S,		// Underlying spot price
				double	K,		// Strike price
				double	Rd,		// Domestic continuos risk free rate
				double	Rf,		// Foreign continuos risk free rate
				double	V,		// Volatility
				double	T,		// Years amount till expiration
				bool	IsCall,	// true if it's 'Call option', false is 'Put option'
				long	Steps	// Amount of binomial tree steps
				) throw();

//============================================================================= 
// This function implements the binomial option pricing model.
// There are discrete dividends (Equity options).
//
// Return value: Option price
// 
// if the function failed it instanciates an exception. if DC (Dividends count) = 0
// when it calls simple CoxRoss version of model.
//
// Comments:
// The function implementation contains small perfomance issues (it's not 
// meaning feature optimized by compiler but algoriphmical one. 
// It's esspecially to make the code clear.
//=============================================================================
double CoxRossDiscDividends(	
				double	S,		// Underlying spot price
				double	K,		// Strike price
				double	R,		// Continuos risk free rate
				double	V,		// Volatility
				double	T,		// Years amount till expiration
				bool	IsCall,	// true if it's 'Call option', false is 'Put option'
				long	Steps,	// Amount of binomial tree steps
				double*	DA,		// Array of dividend's amount
				double*	DT,		// Array of years till dividend payment
				long	DC		// Count of dividends
				) throw();

#endif __COX_ROSS_H__
