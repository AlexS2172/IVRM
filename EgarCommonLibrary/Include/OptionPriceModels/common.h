#ifndef __OPTION_PRICE_MODELS_COMMON_H__
#define __OPTION_PRICE_MODELS_COMMON_H__

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <windef.h>

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

const double cdDaysPerYear360 = 360.;

const double cdDaysPerYear365 = 365.;

const double cdDeltaVolatility = 0.01;

const double cdDeltaRate = 0.01;

const double cdDeltaTime = 1. / 365.;

const double cdDeltaSqrtTime = 0.0523;


//=============================================================================
// Lookup maximum and minimum value
//=============================================================================
//#define max(A,B) (A > B ? A : B)
//#define min(A,B) (A < B ? A : B)

}

//=============================================================================
// Autolinking
//=============================================================================
#ifdef OPTION_PRICE_MODELS_IMPORT
	#ifdef _DEBUG
		#pragma message ("Automatically likned with OptionPriceModelsD.lib")
		#pragma comment (lib, "..\\lib\\OptionPriceModelsD.lib")
	#else
		#pragma message ("Automatically likned with OptionPriceModels.lib")
		#pragma comment (lib, "..\\lib\\OptionPriceModels.lib")
	#endif
#endif

#endif //__OPTION_PRICE_MODELS_COMMON_H__