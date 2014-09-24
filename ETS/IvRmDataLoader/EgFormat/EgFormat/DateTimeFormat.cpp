// DateTimeFormat.cpp : Implementation of CDateTimeFormat

#include "stdafx.h"
#include "DateTimeFormat.h"
#include <time.h>
#include <comutil.h>



// CDateTimeFormat


STDMETHODIMP CDateTimeFormat::egStrftime(BSTR sFormat, BSTR* sRet)
{
	// TODO: Add your implementation code here
	HRESULT hr = S_OK;

	struct tm cur_time;
	struct _SYSTEMTIME st;

	GetLocalTime(&st);

	cur_time.tm_hour	= st.wHour;
	cur_time.tm_min		= st.wMinute;
	cur_time.tm_sec		= st.wSecond;
	cur_time.tm_year	= st.wYear;
	cur_time.tm_mon		= st.wMonth - 1;
	cur_time.tm_mday	= st.wDay;
	cur_time.tm_wday	= st.wDayOfWeek;
	cur_time.tm_year	= st.wYear - 1900;
	cur_time.tm_isdst	= 0;

	TCHAR *pVal		= new TCHAR[128];
	size_t lResult	= _tcsftime(pVal, 128, sFormat, &cur_time);

	*sRet = SysAllocString(pVal);
	if (*sRet == NULL) 
		 hr = E_OUTOFMEMORY;

	delete [] pVal;

	return hr;
}
