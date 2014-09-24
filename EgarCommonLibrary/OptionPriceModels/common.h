#ifndef __OPTION_PRICE_MODELS_COMMON_H__
#define __OPTION_PRICE_MODELS_COMMON_H__

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <windef.h>

#include <math.h>
#include <stdlib.h>
#include <float.h>

namespace OPM
{

//=============================================================================
// Pi constant value
//=============================================================================
const double Pi = 3.1415926535897932384626433832795;

//=============================================================================
// Epsilon constant value
//=============================================================================
const double Epsilon = 0.000001;

//=============================================================================
// Bad double value
//=============================================================================
const double BadDoubleValue = -1E+308;

//=============================================================================
// Max theo price value
//=============================================================================
const double MaxTheoPriceValue = 10000.;

bool IsBadValue(double dVal);

bool ValueNEQZero(double dVal);

bool IsValidTheoPrice(double& dTheoPrice);


const double cdDaysPerYear360 = 360.;

const double cdDaysPerYear365 = 365.;

const double cdMinutesPerYear365 = 24*60*cdDaysPerYear365;

const double cdSecondsPerYear365 = 60*cdMinutesPerYear365;

const double cdDeltaVolatility = 0.01;

const double cdDeltaRate = 0.01;

const double cdDeltaTime = 1. / 365.;

const double cdDeltaSqrtTime = 0.0523;

}

#include <OptionCalc\DataTypes.h>

#include "NormalC.h"
#include "BlackAndScholes.h"
#include "CoxRoss.h"
#include "Dividends.h"
#include "Rates.h"
#include "VSKLog.h"
#include "FutureGreeks.h"

#endif //__OPTION_PRICE_MODELS_COMMON_H__