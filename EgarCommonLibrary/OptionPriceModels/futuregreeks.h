//=============================================================================
//
// Author	: Michael Malyshkin
// File		: FutureGreeks.h
// Created	: 3 nov 2003
// 
// Future options "spot" greeks from "forward" greeks
//=============================================================================

#ifndef __FUTURE_GREEKS_H__
#define __FUTURE_GREEKS_H__

namespace OPM
{

bool SpotGreeksFromForward(
				double	dRateDomestic,	// Domestic continuos risk free rate
				double	dYte,			// Days till expiration
				GREEKS*	pGreeks			// in, out params
				);						//returns true if success



// SpotGreeksFromForward2 is version of SpotGreeksFromForward with
// index dividend yiels/ index discrete dividend support
bool SpotGreeksFromForward2(
							double	dSpotPrice,
							double	dRateDomestic,	// Domestic continuos risk free rate
							double	dYield,			// Domestic continuos risk free rate
							double	dYte,			// Days till expiration
							double*	pDivAmnts,		// Dividend amounts
							double*	pDivYears,		// Dividend dates
							long	nCount,			// Dividend quantity
							GREEKS*	pGreeks			// in, out params
							);	

};
#endif //__FUTURE_GREEKS_H__
