#ifndef __ESDEFS_H__
#define __ESDEFS_H__

#include <float.h>
#include <math.h>

namespace EODCO
{

#define BAD_DOUBLE_VALUE				-1E+308
#define BAD_LONG_VALUE					-2147483647L

/////////////////////////////////////////////////////////////////////////////
//
static __inline bool IsBadValue(long nValue)
{
	return (BAD_LONG_VALUE == nValue);
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline bool IsBadValue(double dValue)
{
	return !(_finite(dValue) && dValue > BAD_DOUBLE_VALUE);
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline double PriceMidEx(double dPriceBid, double dPriceAsk, double dPriceLast, bool bUseLast)
{
	if(dPriceBid > 0. && dPriceAsk > 0.)
	{
		return (dPriceBid + dPriceAsk) / 2.;
	}

	if(bUseLast && dPriceLast > 0.)
	{
		return dPriceLast;
	}

	if(dPriceBid > 0. || dPriceAsk > 0.)
	{
		if(dPriceBid > 0. && dPriceAsk <= 0.)
		{
			if(bUseLast)
			{
				return dPriceBid;
			}

			return dPriceBid / 2.;
		}

		if(bUseLast)
		{
			return dPriceAsk;
		}
		return dPriceAsk / 2.;
	}

	return BAD_DOUBLE_VALUE;
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline bool DoubleEQZero(double dVal)
{
	return fabs(dVal) < DBL_EPSILON;		
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline bool DoubleNEQZero(double dVal)
{
	return fabs(dVal) > DBL_EPSILON;		
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline long ExpiryIdFromDate(DATE dtExpiration){
	return static_cast<long>(dtExpiration);
}

};

#endif // __ESDEFS_H__
