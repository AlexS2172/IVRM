#include "Common.h"

namespace OPM
{

bool IsBadValue(double dVal)
{
	if (_isnan(dVal) || !_finite(dVal))
		return false;

	return !(dVal > BadDoubleValue);
}

bool ValueNEQZero(double dVal)
{
	return fabs(dVal) > DBL_EPSILON;
}

bool IsValidTheoPrice(double& dTheoPrice)
{
	if (_isnan(dTheoPrice) || !_finite(dTheoPrice))
		return false;

	if (dTheoPrice < -DBL_EPSILON || dTheoPrice > MaxTheoPriceValue)
		return false;

	// set -DBL_EPSILON to 0. (price cann't be negative)
	if(!ValueNEQZero(dTheoPrice))
		dTheoPrice = 0.;

	return true;	
}

}