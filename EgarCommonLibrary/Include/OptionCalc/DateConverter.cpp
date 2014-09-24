#include "stdafx.h"
#include "dateconverter.h"
#include <time.h>

static const double DAYS_IN_ONE_YEAR = 365.0; /*365.2425;*/
#define isleap(y) (((y % 4) == 0) && (((y % 100) != 0) || ((y % 400) == 0)))

#include <ATLComTime.h>

// minimum represenative date: January 1, 1970
// maximum represenative date: January 19, 2038

static double MIN_UNIX_DATE_BOUNDARY = 25569;
static double MAX_UNIX_DATE_BOUNDARY = 50424;

bool OPTIONCALC_API OleDateToUnixDate(double dateIn, time_t* uxDate )
{

	if (dateIn < MIN_UNIX_DATE_BOUNDARY || dateIn > MAX_UNIX_DATE_BOUNDARY){
		return false;
	}

	double decimalPart = 0.0;
	double wholePart = 0.0;

	wholePart	= floor( dateIn );
	decimalPart = fmod( dateIn, wholePart );

	int t = static_cast<int>(wholePart - MIN_UNIX_DATE_BOUNDARY);

	*uxDate = t * 24 * 60 * 60;

	*uxDate += static_cast<int>(24 * 60 * 60 * decimalPart);

	return true;

}

bool OPTIONCALC_API TmToDATE( struct tm* pTm, double *pDateOut )

{

	int leapYear = 0;
	if( (pTm->tm_year - 1900) < 0 ) return false;

	/* Start at 1. This is the way DATE is defined.
	* January 1, 1900 at Midnight is 1.00.
	* January 1, 1900 at 6AM is 1.25.
	* and so on.
	*/

	*pDateOut = 1;
	/* Add the number of days corresponding to
	* tm_year.
	*/

	*pDateOut += (pTm->tm_year - 1900) * 365;
	/* Add the leap days in the previous years between now and 1900.
	* Note a leap year is one that is a multiple of 4
	* but not of a 100.  Except if it is a multiple of
	* 400 then it is a leap year.
	*/

	*pDateOut += ( (pTm->tm_year - 1) / 4 ) - ( 1900 / 4 );
	*pDateOut -= ( (pTm->tm_year - 1) / 100 ) - ( 1900 / 100 );
	*pDateOut += ( (pTm->tm_year - 1) / 400 ) - ( 1900 / 400 );

	/* Set the leap year flag if the
	* current year specified by tm_year is a
	* leap year. This will be used to add a day
	* to the day count.
	*/

	if( isleap( pTm->tm_year ) )
		leapYear = 1;

	/* Add the number of days corresponding to
	* the month.
	*/

	switch( pTm->tm_mon )
	{
	case 2:
		*pDateOut += 31;
		break;
	case 3:
		*pDateOut += ( 59 + leapYear );
		break;
	case 4:
		*pDateOut += ( 90 + leapYear );
		break;
	case 5:
		*pDateOut += ( 120 + leapYear );
		break;
	case 6:
		*pDateOut += ( 151 + leapYear );
		break;
	case 7:
		*pDateOut += ( 181 + leapYear );
		break;
	case 8:
		*pDateOut += ( 212 + leapYear );
		break;
	case 9:
		*pDateOut += ( 243 + leapYear );
		break;
	case 10:
		*pDateOut += ( 273 + leapYear );
		break;
	case 11:
		*pDateOut += ( 304 + leapYear );
		break;
	case 12:
		*pDateOut += ( 334 + leapYear );
		break;
	}

	/* Add the number of days in this month.
	*/

	*pDateOut += pTm->tm_mday;

	/* Add the number of seconds, minutes, and hours
	* to the DATE. Note these are the fracionnal part
	* of the DATE so seconds / number of seconds in a day.
	*/

	*pDateOut += pTm->tm_hour / 24.0;
	*pDateOut += pTm->tm_min / 1440.0;
	*pDateOut += pTm->tm_sec / 86400.0;

	return true;
}

bool OPTIONCALC_API TmToDateEx(struct tm* pTm, double *pDateOut)
{
	if (pTm && pDateOut)
	{
		SYSTEMTIME	st;
		st.wDayOfWeek = pTm->tm_wday;
		st.wDay = pTm->tm_mday;
		st.wHour = pTm->tm_hour;
		st.wMinute = pTm->tm_min;
		st.wMonth = pTm->tm_mon + 1;
		st.wSecond = 0;//pTm->tm_sec;
		st.wYear = pTm->tm_year + 1900;
		st.wMilliseconds = 0;

		::SystemTimeToVariantTime (&st, pDateOut);
	}
	return false;
}