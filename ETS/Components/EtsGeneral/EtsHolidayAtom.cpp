// EtsHolidayAtom.cpp : Implementation of CEtsHolidayAtom

#include "stdafx.h"
#include "EtsHolidayAtom.h"

STDMETHODIMP CEtsHolidayAtom::Add(DATE dtDate)
{
	long		lDate		=	static_cast<long>(floor(dtDate));
	LVIterator	searchItr	=	find(m_vHoliday.begin(), m_vHoliday.end(), lDate);
	
	if(searchItr == m_vHoliday.end()) m_vHoliday.push_back(lDate);

	sort(m_vHoliday.begin(), m_vHoliday.end());

	return S_OK;
}

STDMETHODIMP CEtsHolidayAtom::Remove(DATE dtDate)
{
	long		lDate		=	static_cast<long>(floor(dtDate));
	LVIterator	searchItr	=	find(m_vHoliday.begin(), m_vHoliday.end(), lDate);	

	if (searchItr != m_vHoliday.end())
	{
		m_vHoliday.erase(searchItr);
		sort(m_vHoliday.begin(), m_vHoliday.end());
	}

	return S_OK;
}

//return previous working day
STDMETHODIMP CEtsHolidayAtom::GetPreviousWorkingDate(DATE dtDate, DATE *dtPWDate)
{
	if (!dtPWDate)	return	E_POINTER;
	
	long		lDate		= static_cast<long>(floor(dtDate - 1.));
	
	SYSTEMTIME	stDate;
	::VariantTimeToSystemTime(dtDate - 1., &stDate);
	WORD		wDayOfWeek	= stDate.wDayOfWeek;
	
	while(true)
	{
		if (wDayOfWeek != 0 && wDayOfWeek != 6){
			if ( !binary_search( m_vHoliday.begin(), m_vHoliday.end(), lDate) )	break;
		}

		if (wDayOfWeek == 0) wDayOfWeek = 6;
		else --wDayOfWeek;

		--lDate;
	}

	*dtPWDate = static_cast<double>(lDate);

	return S_OK;
}
//return holidays in array
STDMETHODIMP CEtsHolidayAtom::GetHolidays(DATE dtToday, SAFEARRAY ** psaDates,  LONG* pnCount)
{

	if (!pnCount) return E_POINTER;

	*pnCount = static_cast<LONG>( m_vHoliday.size() );

	if (*pnCount > 0)
	{
		LPSAFEARRAY pDates = SafeArrayCreateVector(VT_R8, 0, *pnCount);
		::SafeArrayLock(pDates);
		
		LPVOID lpDateData = NULL;
		::SafeArrayAccessData(pDates, &lpDateData);
		

		DOUBLE * pdDates =	 reinterpret_cast<DOUBLE *>(lpDateData);
		for(long i = 0; i< *pnCount; i++)
		{
			pdDates[i] = m_vHoliday[i];
		}

		::SafeArrayUnaccessData(pDates);
		::SafeArrayUnlock(pDates);

		*psaDates = pDates;
	}

	return S_OK;
}

//clear holiday coll
STDMETHODIMP CEtsHolidayAtom::Clear(void)
{
	m_vHoliday.clear();
	return	S_OK;
}

//transform dividends
STDMETHODIMP CEtsHolidayAtom::TransformDividends(SAFEARRAY **psaDates, LONG nCount, SAFEARRAY **psaTDates, LONG *pnTCount)
{
	if (!pnTCount || !psaDates || !psaTDates) return E_POINTER;
	
	HRESULT	hr = S_OK;

	try
	{
		*pnTCount = 0;
		if (nCount > 0)
		{
			DOUBLE	*pdDates = NULL;

			LongVector	lvDates;

			if( *psaTDates )
			{
				::SafeArrayDestroy(*psaTDates);
				*psaTDates = NULL;
			}

			if( SUCCEEDED(hr = ::SafeArrayAccessData(*psaDates, reinterpret_cast<void**>(&pdDates))) )
			{
				DATE	dtRealDate;
				for(int i = 0; i < nCount; i++)
				{
					GetPreviousWorkingDate(DATE(pdDates[i] * 365.0), &dtRealDate);
					lvDates.push_back( static_cast<LONG>(floor(dtRealDate)) );
				}
				::SafeArrayUnaccessData(*psaDates);
				
				*pnTCount = static_cast<LONG>(lvDates.size());
				if (*pnTCount > 0)
				{
					LPSAFEARRAY pDates = SafeArrayCreateVector(VT_R8, 0, *pnTCount);
					::SafeArrayLock(pDates);
					LPVOID lpDateData = NULL;
					::SafeArrayAccessData(pDates, &lpDateData);

					DOUBLE * pdDates =	 reinterpret_cast<DOUBLE *>(lpDateData);

					for(long i = 0; i< *pnTCount; i++)
					{
						pdDates[i] = lvDates[i]/365.0;
					}

					::SafeArrayUnaccessData(pDates);
					::SafeArrayUnlock(pDates);
					
					*psaTDates = pDates;
				}
			}
		}
	}
	catch (...)
	{
		return E_FAIL;
	}
	return S_OK;
}
// CEtsHolidayAtom