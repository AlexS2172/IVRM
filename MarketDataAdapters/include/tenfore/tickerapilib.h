#ifndef __TICKERAPI_LIB_H___
#define __TICKERAPI_LIB_H___

// Ticker API header file
// Includes all headers and lib 

#include "tapidef.h"
#include "TickerApi.h"

// Comment this statement to switch to realworld version
//#define _TAPI_SIMULATION_

#if defined( _TAPI_SIMULATION_ )
	#if defined(UNICODE) || defined(_UNICODE)
		#define _TICKERAPI_COMMENT "..\\..\\lib\\Tenfore\\TickerApiSim.lib"
		#define _TICKERAPI_MESSAGE "Automatically linking with TickerApiSim.dll"
	#else
		#define _TICKERAPI_COMMENT "..\\..\\lib\\Tenfore\\TickerApiSim.lib"
		#define _TICKERAPI_MESSAGE "Automatically linking with TickerApiSim.dll"
	#endif // UNICODE
#else
	#if defined(UNICODE) || defined(_UNICODE)
		#define _TICKERAPI_COMMENT "..\\..\\lib\\Tenfore\\TickerApi.lib"
		#define _TICKERAPI_MESSAGE "Automatically linking with TickerApi.dll"
	#else
		#define _TICKERAPI_COMMENT "..\\..\\lib\\Tenfore\\TickerApi.lib"
		#define _TICKERAPI_MESSAGE "Automatically linking with TickerApi.dll"
	#endif // UNICODE
#endif	// _TAPI_SIMULATION

#pragma comment( lib, _TICKERAPI_COMMENT )
#pragma message( _TICKERAPI_MESSAGE )

#endif // __TICKERAPI_LIB_H___