#include "stdafx.h"

#include "OmniApiUtils.h"

void ClickStringToString(const char* const pSource, int iSize, string& sResult)
{
	EgAssert(iSize != 0);

	while(pSource[iSize - 1] == ' ' || pSource[iSize - 1] == 0)
	{
		if(--iSize == 0)
			break;
	}

	if(iSize == 0)
		sResult = "";
	else
		sResult.assign(pSource, iSize);
}

void StringToClickString(const string& sSource, const int iResultSize, char* pResult)
{
	strncpy(pResult, sSource.c_str(), iResultSize);
	int iLen = sSource.length();
	if(iLen < iResultSize)
	{
		memset(pResult + iLen, ' ', iResultSize - iLen);
	}
}

static int iDividers[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 
							10000000, 100000000, 1000000000 };

double DecimalToDouble(const int32 iValue, const uint16 uiDecimals)
{
    return ((double) iValue / iDividers[uiDecimals]);
}

int32 DoubleToDecimal(const double& dValue, const uint16 uiDecimals)
{
	float d = (float)dValue * iDividers[uiDecimals];
	printf("");
	int32 i = (int32)d;

	return i;
}

void   DecimalToString(const int32 iValue, const uint16 uiDecimals, string& sValue)
{
	double dValue = DecimalToDouble(iValue, uiDecimals);
	char szBuf[256];

	sprintf(szBuf, "%f", dValue);
	sValue = szBuf;
}

void   Series2String(const series_t& Series, string& sValue)
{
	uint16 uiYear;
	uint8  uiMonth;
	uint8  uiDay;

	SplitIntDate(Series.expiration_date_n, &uiYear, &uiMonth, &uiDay);
	char   szBuf[1024];
	sprintf(szBuf, "Country='%c' Market='%c' Instrument='%c' Commodity='%d' Date='%d/%d/%d' Strike='%d'",
		Series.country_c, Series.market_c, Series.instrument_group_c,
		Series.commodity_n, uiMonth, uiDay, uiYear, Series.strike_price_i);
}

double IntDateToDoubleDaysFromNow(uint16 uiDate)
{
    time_t  tFrom, tTo;
    struct tm	tmTo;

	uint16 uiYear;
	uint8  uiMonth;
	uint8  uiDay;

	SplitIntDate(uiDate, &uiYear, &uiMonth, &uiDay);

    tFrom = time(NULL);
    memcpy (&tmTo, localtime(&tFrom), sizeof(tmTo));

    tmTo.tm_mday =  uiDay;
    tmTo.tm_mon =   uiMonth - 1;
    tmTo.tm_year =  uiYear - 1900;

    tTo = mktime(&tmTo);

    return ((double)(difftime (tTo, tFrom) / 86400));
} 

void SplitIntDate(uint16 uiDate, uint16* puiYear, uint8* puiMonth, uint8* puiDay)
{
    if (!uiDate)
    {
		*puiYear = 0;
		*puiMonth = 0;
		*puiDay = 0;
    }
    else
    {
		*puiDay = uiDate & 0x001F;
		*puiMonth = ( uiDate >> 5 ) &  0x000F;
		*puiYear = (( uiDate >> 9 ) & 0x007F ) + 1989;
    }
}

int _afxMonthDays[13] =
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

BOOL _AfxOleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
	WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
	// Validate year and month (ignore day of week and milliseconds)
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return FALSE;

	//  Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((wYear & 3) == 0) &&
		((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth =
		_afxMonthDays[wMonth] - _afxMonthDays[wMonth-1] +
		((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

	// Finish validating the date
	if (wDay < 1 || wDay > nDaysInMonth ||
		wHour > 23 || wMinute > 59 ||
		wSecond > 59)
	{
		return FALSE;
	}

	// Cache the date in days and time in fractional days
	long nDate;
	double dblTime;

	//It is a valid date; make Jan 1, 1AD be 1
	nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 +
		_afxMonthDays[wMonth-1] + wDay;

	//  If leap year and it's before March, subtract 1:
	if (wMonth <= 2 && bLeapYear)
		--nDate;

	//  Offset so that 12/30/1899 is 0
	nDate -= 693959L;

	dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		((long)wMinute * 60L) +  // mins in seconds
		((long)wSecond)) / 86400.;

	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return TRUE;
}

void   IntDateToOleDate(const uint16 uiDate, DATE& dtOleDate)
{
	uint16 uiYear;
	uint8  uiMonth;
	uint8  uiDay;

	SplitIntDate(uiDate, &uiYear, &uiMonth, &uiDay);

	_AfxOleDateFromTm(uiYear, uiMonth, uiDay, 0, 0, 0, dtOleDate);
}

void  StringsDateTimeToOleDate(const char* pDate, const char* pTime, DATE& dtOleDate)
{
	DWORD dwYear, dwMonth, dwDay;
	DWORD dwHour, dwMinute, dwSecond;

	char szBuf[9];
	strncpy(szBuf, pDate, 8);
	sscanf(szBuf, "%04d%02d%02d", &dwYear, &dwMonth, &dwDay);
	strncpy(szBuf, pTime, 8);
	sscanf(szBuf, "%02d%02d%02d", &dwHour, &dwMinute, &dwSecond);

	_AfxOleDateFromTm(dwYear, dwMonth, dwDay, dwHour, dwMinute, dwSecond, dtOleDate);
}

void QuardWordToString(const quad_word& qValue, string& sValue)
{
	char szBuf[8 * 2 + 1];
	
	const unsigned int* pdw1 = (const unsigned int*)(const char*)(&qValue);
	const unsigned int* pdw2 = (const unsigned int*)((const char*)&qValue + sizeof(unsigned int));

	sprintf(szBuf, "%04x%04x", *pdw1, *pdw2);
	sValue = szBuf;
}

void QuardWordToString(const __int64& qValue, string& sValue)
{
	char szBuf[8 * 2 + 1];
	
	const unsigned int* pdw1 = (const unsigned int*)(const char*)(&qValue);
	const unsigned int* pdw2 = (const unsigned int*)((const char*)&qValue + sizeof(unsigned int));

	sprintf(szBuf, "%04x%04x", *pdw1, *pdw2);
	sValue = szBuf;
}