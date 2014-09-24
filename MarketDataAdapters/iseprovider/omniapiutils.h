#ifndef OMNIAPIUTILS_H
#define OMNIAPIUTILS_H

/*
	Strings utils
*/

void ClickStringToString(const char* const pSource, const int iSize, string& sResult);
#define CS2S(click_string, cstring) \
	ClickStringToString(click_string, sizeof(click_string), cstring)

void StringToClickString(const string& sSource, const int iResultSize, char* pResult);
#define S2CS(cstring, click_string) \
	StringToClickString(cstring, sizeof(click_string), click_string)


double DecimalToDouble(const int32 iValue, const uint16 uiDecimals);
#define DEC2DBL(value, decimals)\
	   DecimalToDouble(value, decimals)

int32 DoubleToDecimal(const double& dValue, const uint16 uiDecimals);
#define DBL2DEC(value, decimals)\
	   DoubleToDecimal(value, decimals)

void QuardWordToString(const quad_word& qValue, string& sValue);
void QuardWordToString(const __int64& qValue, string& sValue);
#define QW2S(q, s) \
	QuardWordToString(q, s)


double IntDateToDoubleDaysFromNow(uint16 uiDate);
#define IDATE2DBL_FROMNOW(date)\
		IntDateToDoubleDaysFromNow(date)

void   IntDateToOleDate(const uint16 uiDate, DATE& dtOleDate);
#define ID2OD(i, o)	\
	IntDateToOleDate(i, o);


void   StringsDateTimeToOleDate(const char* pDate, const char* pTime, DATE& dtOleDate);
#define SDT2OD(d, t, o) \
	StringsDateTimeToOleDate(d, t, o);


void   DecimalToString(const int32 iValue, const uint16 uiDecimals, string& sValue);
#define	DEC2STR(value, decimals, string)\
		DecimalToString(value, decimals, string);

void   Series2String(const series_t& Series, string& sValue);
#define SERIES2STR(series, string)
		SeriesToString(series, string);

void SplitIntDate(uint16 uiDate, uint16* puiYear, uint8* puiMonth, uint8* puiDay);

#endif //OMNIAPIUTILS_H