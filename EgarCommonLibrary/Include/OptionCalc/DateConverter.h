#pragma once

#include <OptionCalc\OptionCalc.h>

bool OPTIONCALC_API  OleDateToUnixDate(double dateIn, time_t* uxDate);

bool OPTIONCALC_API  TmToDATE( struct tm* pTm, double *pDateOut );

bool OPTIONCALC_API	 TmToDateEx( struct tm* pTm, double *pDateOut);