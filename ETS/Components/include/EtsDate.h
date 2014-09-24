#ifndef __ETSDATE_H__
#define __ETSDATE_H__

class CEtsDate;

/////////////////////////////////////////////////////////////////////////////
//
class CEtsDateSpan
{
	friend class CEtsDate;
private:
	static DOUBLE _GetHalfSecond()
	{
		return 1. / (2. * 60. * 60. * 24.);
	}
public:
		
	CEtsDateSpan(DATE dtSpan)
		: m_dtSpan(dtSpan)
	{
	}

	CEtsDateSpan(LONG nDays = 0L, LONG nHours = 0L, LONG nMinutes =0L, LONG nSeconds = 0L)
	{
		m_dtSpan = nDays + nHours / 24. + nMinutes / (24. * 60.) + nSeconds / (24. * 60. * 60.);
	}

	LONG Days() const
	{
		return static_cast<LONG>(m_dtSpan);
	}

	LONG Hours() const
	{
		LONG nHours = static_cast<LONG>((GetFract(m_dtSpan) + _GetHalfSecond()) * 24);
		if(nHours >= 24)
			nHours -= 24;
		return nHours;
	}
	LONG Minutes() const
	{
		LONG nMinutes = static_cast<LONG>((GetFract(m_dtSpan * 24) + _GetHalfSecond()) * 60);
		if(nMinutes >= 60)
			nMinutes -= 60;
		return nMinutes;
	}
	LONG Seconds() const
	{
		LONG nSeconds = static_cast<LONG>((GetFract(m_dtSpan * 24 * 60) + _GetHalfSecond()) * 60);
		if(nSeconds >= 60)
			nSeconds -= 60;
		return nSeconds;
	}

	LONG TotalDays() const
	{
		return static_cast<LONG>(m_dtSpan);
	}

	LONG TotalHours() const
	{
		return static_cast<LONG>(m_dtSpan * 24 + _GetHalfSecond());
	}

	LONG TotalMinutes() const 
	{
		return static_cast<LONG>(m_dtSpan * 24 * 60 + _GetHalfSecond());
	}

	LONG TotalSeconds() const
	{
		return static_cast<LONG>(m_dtSpan * 24 * 60 * 60 + _GetHalfSecond());
	}

	LONG TotalMilliseconds() const
	{
		return static_cast<LONG>(m_dtSpan * 24 * 60 * 60 * 1000 );
	}

protected:
	DOUBLE GetFract(DOUBLE dVal) const
	{
		return dVal - static_cast<LONG>(dVal);
	}

	DATE				m_dtSpan;
};

/////////////////////////////////////////////////////////////////////////////
//
class CEtsDate
{
public:
	CEtsDate(void)
		: m_dtDate(0.)
	{
	}

	CEtsDate(DATE dtDate)
		: m_dtDate(dtDate)
	{
	}

	CEtsDate(const CEtsDate& aDate)
		: m_dtDate(aDate.m_dtDate)
	{
	}

	CEtsDate(USHORT nYear, USHORT nMonth, USHORT nDay,   
			USHORT nHour = 0, USHORT nMinute = 0, USHORT nSecond = 0)
		: m_dtDate(0.)
	{
		SYSTEMTIME st = { 0 };

		st.wYear = nYear;
		st.wMonth = nMonth;
		st.wDay = nDay;
		st.wHour = nHour;
		st.wMinute = nMinute;
		st.wSecond = nSecond;

		INT nResult = ::SystemTimeToVariantTime(&st, &m_dtDate);
		ATLASSERT(nResult);
	}

	CEtsDate& operator = (const CEtsDate& aDate)
	{
		m_dtDate = aDate.m_dtDate;
		return *this;
	}

	CEtsDateSpan operator - (const CEtsDate& dt)
	{
		return CEtsDateSpan(m_dtDate - dt.m_dtDate);
	}

	CEtsDate& operator += (const CEtsDateSpan& dtSpan)
	{
		m_dtDate += dtSpan.m_dtSpan;
		return *this;
	}

	CEtsDate& operator -= (const CEtsDateSpan& dtSpan)
	{
		m_dtDate -= dtSpan.m_dtSpan;
		return *this;
	}

	operator DATE()
	{
		return m_dtDate;
	}

	operator DATE() const
	{
		return m_dtDate;
	}

	USHORT DayOfWeek() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);

		return st.wDayOfWeek;
	}

	USHORT Day() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);

		return st.wDay;
	}

	USHORT Month() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);

		return st.wMonth;
	}

	USHORT Year() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);

		return st.wYear;
	}

	USHORT Hour() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);

		return st.wHour;
	}

	USHORT Minute() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);
	}

	USHORT Second() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);

		return st.wSecond;
	}
	
	USHORT Millisecond() const
	{
		SYSTEMTIME st = { 0 };
		INT nResult = 0;

		nResult = ::VariantTimeToSystemTime(m_dtDate, &st);
		ATLASSERT(nResult);

		return st.wMilliseconds;
	}

	static LONG GetCurrentDate()
	{
		INT nResult = 0;
		SYSTEMTIME st = { 0 }; 
		GetLocalTime(&st);

		st.wHour = 0;
		st.wMinute = 0;
		st.wSecond = 0;
		st.wMilliseconds = 0;
		
		DATE dt = 0.;
		nResult = ::SystemTimeToVariantTime(&st, &dt);
		ATLASSERT(nResult);
		
		return static_cast<LONG>(dt);
	}

	static CEtsDate GetCurrentDateTime()
	{
		INT nResult = 0;
		SYSTEMTIME st = { 0 }; 
		GetLocalTime(&st);

		DATE dt = 0.;
		nResult = ::SystemTimeToVariantTime(&st, &dt);
		ATLASSERT(nResult);
		
		return dt;
	}

	static LONGLONG GetTickCount()
	{
		LARGE_INTEGER aCurTickCount;
		// On a multiprocessor machine, it should not matter which processor is called.
		// However, you can get different results on different processors due to bugs in the BIOS or the HAL.
		if(::QueryPerformanceCounter(&aCurTickCount))
			return aCurTickCount.QuadPart;
		else
			return ::GetTickCount();
	}

	static DATE FirstMonthDayDateTime(DATE dtDate)
	{
		INT nResult = 0;
		SYSTEMTIME st = {0};

		nResult = ::VariantTimeToSystemTime(dtDate, &st);
		ATLASSERT(nResult);

		st.wDay = 1;
		nResult = ::SystemTimeToVariantTime(&st, &dtDate);
		ATLASSERT(nResult);

		return dtDate;
	}

	static LONG FirstMonthDayDate(DATE dtDate)
	{
		INT nResult = 0;
		SYSTEMTIME st = {0};

		nResult = ::VariantTimeToSystemTime(dtDate, &st);
		ATLASSERT(nResult);

		st.wDay = 1;
		nResult = ::SystemTimeToVariantTime(&st, &dtDate);
		ATLASSERT(nResult);

		return static_cast<LONG>(dtDate);
	}

	CEtsDate FirstMonthDayDateTime()
	{
		return FirstMonthDayDateTime(m_dtDate);
	}

	LONG FirstMonthDayDate()
	{
		return FirstMonthDayDate(m_dtDate);
	}

protected:
	DATE		m_dtDate;
};

#endif //__ETSDATE_H__
