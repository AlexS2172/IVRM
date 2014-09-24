#ifndef __DIVIDENDS_H__
#define __DIVIDENDS_H__

namespace OPM
{

/************************************************************
	Discount asset price for the present value of 
	dividends.

	S			- Stock price
	R			- Domestic continuous rate
	pDivAmnts	- Array of dividends 
	pDivYte		- Array of time(amount of years) to payments
	nDivCount	- Size of dividends array
	T			- Time horizont for discount

	Return value - Discounted asset price
************************************************************/
double DiscountForDividends(double		S, 
							double		R,
							double*		pDivAmnt,
							double*		pDivYte,
							unsigned	nDivCount,
							double		T);

}

#endif //__DIVIDENDS_H__