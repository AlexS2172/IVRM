// OptionCalc.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <OptionCalc\OptionCalc.h>
#include "utils.h"
#include "optex.h"
#include <malloc.h>
#include <OptionCalc\DateConverter.h>

#include "boost_month_iterator.h"

#include "Boost/date_time/posix_time/posix_time_types.hpp"
#include "Boost/date_time/posix_time/posix_time.hpp"
#include "Boost/date_time/gregorian/gregorian.hpp"
#include "Boost/date_time/gregorian/greg_month.hpp"
#include "Boost/date_time/c_local_time_adjustor.hpp"
#include "boost/date_time/local_time_adjustor.hpp"
#include "boost/date_time/compiler_config.hpp"

typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;

#pragma warning(disable:4800)

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}





/////////////////////////////////////////////////////////////////////////////////////////////////
double OPTIONCALC_API GetDateDiff(time_t begDate, time_t endDate)
{
	return (endDate - begDate)/OPM::cdSecondsPerYear365;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
static time_t ConvertToTimeT(long msTime)
{
	time_t uxDate = -1;
	OleDateToUnixDate(msTime, &uxDate);
	return uxDate;
}

bool OPTIONCALC_API GetNYDateTimeAsDATE(double *pdtDate)
{
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	if (pdtDate)
	{

		typedef boost::date_time::local_adjustor<ptime, -5, us_dst> us_eastern;
		ptime ptUTCNow = second_clock::universal_time(); 
		ptime ptNYNow = us_eastern::utc_to_local(ptUTCNow);
		tm	  tmNYNow = to_tm(ptNYNow);
		TmToDateEx(&tmNYNow, pdtDate);
	}
	return false;
}

#define ONE_DAY_IN_YEAR 0.002739726027397260273972602739726 // 1/365.0
bool OPTIONCALC_API GetCalculationParams(double dtNow, double dtExpiryOV, double dtCloseTime, bool bUseTime, double* dtDateCalc, double* dtExpiryCalc, double* dtCloseTimeCalc, double* dtYte)
{
	if (dtDateCalc && dtYte && dtCloseTimeCalc && dtExpiryCalc)
	{
		if (bUseTime) 
		{
			 *dtYte			= (dtExpiryOV - dtNow) * ONE_DAY_IN_YEAR;
			 *dtDateCalc	= dtNow;
			 *dtExpiryCalc	= dtExpiryOV;
			 *dtCloseTimeCalc = dtCloseTime;
		}
		else
		{
			*dtExpiryCalc		= floor(dtExpiryOV);

			if (dtExpiryOV - *dtExpiryCalc > 0.5)
				*dtExpiryCalc = *dtExpiryCalc + 1;
			
			*dtCloseTimeCalc	= dtCloseTime;
			*dtDateCalc			= dtNow;
			*dtYte				= (*dtExpiryCalc - floor(dtNow)) * ONE_DAY_IN_YEAR;
		}
		return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
static double InterpolateRatesOld( long nCount, const RATE *pRates, long nDTE )
{
	// Check parameters
	if( nCount <= 0 )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return 0;
	}
	if( ::IsBadReadPtr(pRates, sizeof(RATE) * nCount) )
	{
		::SetLastError( ERROR_NOACCESS );
		return 0;
	}
	if( nDTE < 0 )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return 0;
	}

	const RATE *pPrevRate = NULL;
	long  nPrevDif  = 0;
	const RATE *pNextRate = NULL;
	long  nNextDif  = 0;

	// Find previous and next rates
	for( int i = 0; i < nCount; i++ )
	{
		if( pPrevRate == NULL && pRates[i].nDTE <= nDTE )
		{
			pPrevRate = pRates + i;
			nPrevDif  = nDTE - pRates[i].nDTE;
		}
		else 
			if( pRates[i].nDTE <= nDTE && nDTE - pRates[i].nDTE < nPrevDif )
			{
				pPrevRate = pRates + i;
				nPrevDif  = nDTE - pRates[i].nDTE;
			}

			if( pNextRate == NULL && pRates[i].nDTE > nDTE )
			{
				pNextRate = pRates + i;
				nNextDif  = pRates[i].nDTE - nDTE;
			}
			else 
				if( pRates[i].nDTE > nDTE && pRates[i].nDTE - nDTE < nNextDif )
				{
					pNextRate = pRates + i;
					nNextDif  = pRates[i].nDTE - nDTE;
				}
	}
	_ASSERT( pPrevRate || pNextRate );

	// Return appropriate value
	if( pPrevRate == NULL )	
		return pNextRate->dRate;
	else if( pNextRate == NULL )
		return pPrevRate->dRate;
	else
	{
		long lMDP = nDTE - pPrevRate->nDTE;
		long lMDN = pNextRate->nDTE - pPrevRate->nDTE; 

		double dPNChange      = pNextRate->dRate - pPrevRate->dRate;

		double dK = dPNChange / double(lMDN);

		double dRate = dK * double(lMDP) +	pPrevRate->dRate;
		return dRate;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
static double InterpolateRates2Old( long nCount, double* pRates, long* pnDTEs, long nDTE )
{
	// Check parameters
	if ( nCount <= 0 )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return 0;
	}
	if ( ::IsBadReadPtr(pRates, sizeof(*pRates) * nCount) ||
		::IsBadReadPtr(pnDTEs, sizeof(*pnDTEs) * nCount) )
	{
		::SetLastError( ERROR_NOACCESS );
		return 0;
	}

	std::vector<RATE> vec(nCount);
	for (int i = 0; i < nCount; i++)
	{
		RATE& r = vec[i];
		r.dRate = pRates[i];
		r.nDTE = pnDTEs[i];
	}

	return InterpolateRatesOld(nCount, &vec.front(), nDTE);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
static long get_div_count_etm(boost::posix_time::ptime ptimeToday, 
							  double dYte, 
							  boost::posix_time::ptime ptimeLastDivDate, 
							  long nFrequency, 
							  boost::posix_time::ptime* firstDivDate = NULL
							  )
{

	long nCount = 0;
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	ptime ptimeCurDivDate	= ptimeLastDivDate;

	long lTTEInMinutes	= static_cast<long>(OPM::cdMinutesPerYear365 * dYte);
	ptime expiryDate	= ptimeToday + time_duration(0, lTTEInMinutes, 0);

	month_adder	real_month_adder(12 / nFrequency);
	
	while(ptimeCurDivDate < expiryDate)
	{	
		if (ptimeCurDivDate >= ptimeToday){
			++nCount;
			if (nCount == 1 && firstDivDate){
				*firstDivDate = ptimeCurDivDate;
			}
		}
		ptimeCurDivDate = ptimeCurDivDate + real_month_adder.get_offset(ptimeCurDivDate.date());
	}

	return nCount;
}



/////////////////////////////////////////////////////////////////////////////////////////////////
double OPTIONCALC_API InterpolateRates(	long nCount, 
										const RATE *pRates, 
										double dYte)
{
	return ::InterpolateRatesOld(nCount, pRates, static_cast<long>(ceil(dYte * OPM::cdDaysPerYear365)));
}
/////////////////////////////////////////////////////////////////////////////////////////////////
double OPTIONCALC_API InterpolateRates2( long nCount, 
										 double* pRates, 
										 double* pdYte, 
										 double dYte)
{
	long *pDTEs = (long*)_alloca(sizeof(long) * nCount);

	for (long i = 0; i < nCount; ++i)
	{
		pDTEs[i] = static_cast<long>(pdYte[i] * OPM::cdDaysPerYear365);
	}

	return InterpolateRates2Old(nCount, pRates, pDTEs, static_cast<long>(ceil(dYte * OPM::cdDaysPerYear365)));
}


/////////////////////////////////////////////////////////////////////////////////////////////////
long OPTIONCALC_API  GetDividendsCount(	 time_t nToday, 
										 double	dYte, 
										 time_t	nLastDivDate, 
										 long nFrequency )
{
	// Check parameters
	if( nFrequency != FREQUENCY_MONTHLY		&&  nFrequency != FREQUENCY_QUATERLY &&
		nFrequency != FREQUENCY_SEMIANNUALY &&  nFrequency != FREQUENCY_ANNUALY    )
	{
		return -1;
	}

	using namespace boost::posix_time;
	using namespace boost::gregorian;

	ptime ptimeToday				= local_adj::utc_to_local(from_time_t(nToday));
	ptime ptimeCurDivDate		= local_adj::utc_to_local(from_time_t(nLastDivDate));

	// Calculate dividends count
	return get_div_count_etm( ptimeToday, dYte, ptimeCurDivDate, nFrequency );

}

/////////////////////////////////////////////////////////////////////////////////////////////////
long OPTIONCALC_API  GetBasketDividendsCount(	time_t	nToday, 
												double	dYte, 
												REGULAR_DIVIDENDS*	pDividends,
												long	nCount)
{
	if (nCount < 0)
		return -1;

	if (nCount > 0 && ::IsBadWritePtr(pDividends, sizeof(double) * nCount))
	{
		::SetLastError( ERROR_NOACCESS );
		return -1;
	}

	std::map<double, double>	DivMap;

	for (long n = 0; n < nCount; n++)
	{
		long _nCount = GetDividendsCount(nToday, dYte, 
			pDividends[n].nLastDivDate, pDividends[n].nFrequency);

		if (_nCount <= 0)
			continue;

		double* pDivAmnt = (double*)_alloca(_nCount*sizeof(double));
		double* pDivDays = (double*)_alloca(_nCount*sizeof(double));

		long __nCount;

		long nRes = GetDividends2(	nToday,
			dYte,
			pDividends[n].nLastDivDate,
			pDividends[n].nFrequency,
			pDividends[n].dAmount,
			_nCount, 
			pDivAmnt,
			pDivDays,
			&__nCount);		

		if (nRes != 1 || __nCount != _nCount)
			return -1;

		for (long i = 0; i < _nCount; i++)
		{
			if (DivMap.find(pDivDays[i]) != DivMap.end())
				DivMap[pDivDays[i]] = DivMap[pDivDays[i]] + pDivAmnt[i];
			else
				DivMap[pDivDays[i]] = pDivAmnt[i];
		}
	}

	return DivMap.size();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Fills array of dividends using given last dividend amount, last dividend day and 
// dividend frequency. Returns 0 if failed, or value greater than zero otherwise.
// pnCount contains number of dividends available prior expiry date.
/////////////////////////////////////////////////////////////////////////////////////////////////
long OPTIONCALC_API  GetDividends(	time_t nToday, 
									double dYte, 
									time_t nLastDivDate, 
									long nFrequency, 
									double dAmount, 
									long nCount, 
									DIVIDEND *pDividends, 
									long *pnCount )
{
	// Check parameters
	if (nCount==0)
	{
		*pnCount = 0;
		return 1;
	}

	using namespace boost::posix_time;
	using namespace boost::gregorian;

	ptime ptimeCurDivDate;
	ptime ptimeToday = local_adj::utc_to_local(from_time_t(nToday));
	long nDividends = get_div_count_etm(ptimeToday, dYte, local_adj::utc_to_local(from_time_t(nLastDivDate)), nFrequency, &ptimeCurDivDate);
	month_adder	real_month_adder( 12 / nFrequency );

	if( nCount < nDividends )
	{
		*pnCount = 0;
		return -1;
	}

	for( int i = 0; i < nDividends; i++ )
	{

		pDividends[i].dAmount				= dAmount;
		pDividends[i].dTimeFraction = (ptimeCurDivDate - ptimeToday).total_seconds() / OPM::cdSecondsPerYear365;

		ptimeCurDivDate = ptimeCurDivDate + real_month_adder.get_offset(ptimeCurDivDate.date());
	}

	return 1;

}


/////////////////////////////////////////////////////////////////////////////////////////////////
long OPTIONCALC_API  GetDividends2(	time_t nToday, 
											double	dYte, 
											time_t nLastDivDate, 
											long nFrequency, 
											double dAmount, 
											long nCount, 
											double* pDivAmnts, 
											double* pDivDays,
											long *pnCount )
{
	// Check parameters
	if (nCount==0)
	{
		*pnCount = 0;
		return 1;
	}

	using namespace boost::posix_time;
	using namespace boost::gregorian;

	ptime ptimeCurDivDate;
	ptime ptimeToday = local_adj::utc_to_local(from_time_t(nToday));
	long nDividends = get_div_count_etm(ptimeToday, dYte, local_adj::utc_to_local(from_time_t(nLastDivDate)), nFrequency, &ptimeCurDivDate);
	month_adder	real_month_adder( 12 / nFrequency );

	if( nCount < nDividends )
	{
		*pnCount = 0;
		return -1;
	}

	for( int i = 0; i < nDividends; i++ )
	{

		pDivAmnts[i]				= dAmount;
		pDivDays[i]					= (ptimeCurDivDate - ptimeToday).total_seconds() / OPM::cdSecondsPerYear365;

		ptimeCurDivDate = ptimeCurDivDate + real_month_adder.get_offset(ptimeCurDivDate.date());
	}

	return 1;

}

/////////////////////////////////////////////////////////////////////////////////////////////////
long OPTIONCALC_API  GetBasketDividends(		 time_t				nToday, 
													 double				dYte, 
													 REGULAR_DIVIDENDS*	pDividends,
													 unsigned			nCount,
													 double*			pDivAmnts,
													 double*			pDivDays,
													 long				nInCount,
													 long*				pnOutCount)
{
	if (nCount < 0)
		return 1;

	if (nCount > 0 && ::IsBadWritePtr(pDividends, sizeof(double) * nCount))
	{
		::SetLastError( ERROR_NOACCESS );
		return 1;
	}

	if (::IsBadWritePtr(pnOutCount, sizeof(long)))
	{
		::SetLastError( ERROR_NOACCESS );
		return 1;
	}

	std::map<double, double>	DivMap;

	for (unsigned n = 0; n < nCount; n++)
	{
		long _nCount = GetDividendsCount(nToday, dYte, 
			pDividends[n].nLastDivDate, pDividends[n].nFrequency);

		if (_nCount <= 0)
			continue;

		double* pDivAmnt = (double*)_alloca(_nCount*sizeof(double));
		double* pDivDays = (double*)_alloca(_nCount*sizeof(double));

		long __nCount;

		long nRes = GetDividends2(	nToday,
			dYte,
			pDividends[n].nLastDivDate,
			pDividends[n].nFrequency,
			pDividends[n].dAmount,
			_nCount, 
			pDivAmnt,
			pDivDays,
			&__nCount);		

		if (nRes != 1 || __nCount != _nCount)
			return -1;

		for (long i = 0; i < _nCount; i++)
		{

			if (DivMap.find(pDivDays[i]) != DivMap.end())
				DivMap[pDivDays[i]] = DivMap[pDivDays[i]] + pDivAmnt[i];
			else
				DivMap[pDivDays[i]] = pDivAmnt[i];
		}
	}

	*pnOutCount = DivMap.size();

	if (nInCount < (long)DivMap.size())
		return 0;

	std::map<double, double>::iterator	it = DivMap.begin();

	for (int i = 0; it != DivMap.end(); it++, i++)
	{
		pDivAmnts[i] = it->second;
		pDivDays[i] = it->first;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void PrepareDivData(const DIVIDEND *pDividends,std::vector<double> *divytes,std::vector<double> *divamts, int nCount)
{
	if (nCount!=0)
	{
		divytes->resize (nCount);
		divamts->resize (nCount);
		for( int i = 0; i < nCount; i++ )
		{
			(*divytes)[i] = pDividends[i].dTimeFraction;
			(*divamts)[i] = pDividends[i].dAmount;
		}
	}
}



////////////////////////////////////////////////////////////
// Rates
////////////////////////////////////////////////////////////

// Interpolates rates. Returns 0 if failed, or value greater than 0 if succeeded.
// nDTE must be greater that zero. Input rates array assumed to be unsorted.
//
//
//////////////////////////////////////////////////////////////
//// Dividends
//// 
//// All dates in functions and structures are 
//// number of days since 30 December 1899, midnight.
//////////////////////////////////////////////////////////////
//
//// 
//static void _addMonths( SYSTEMTIME& st, WORD wMonths )
//{
//	if( (st.wMonth + wMonths) <= 12 )
//		 st.wMonth += wMonths;
//	else
//	{
//		st.wYear++;
//		st.wMonth = st.wMonth + wMonths - 12;// % 12;
//	}
//}
//
//// Internal dates calculator
//static long _getDivCount(	long nToday, //Нолмер текущего дня от 30 декабря 1899 года
//							long nDTE,   //Количетсво дней до экспорации от сегоднешнего дня
//							long nLastDivDate, //Номер последнего дня когда выплачивались дивиденты от 30 декабря 1899 года
//							long nFrequency, //Частота выплат девидентов (количество раз вгод)
//						  SYSTEMTIME* pFirstDivDate = NULL ) //Первый день когда выплачиваются девиденты
//{
//	long nCount = 0;
//
//	// Convert times
//	SYSTEMTIME stToday, stDivDate, stExpiry;
//	if( !::VariantTimeToSystemTime( nToday, &stToday ) )
//	{
//		::SetLastError( ERROR_INVALID_PARAMETER );
//		return -1;
//	}
//	if( !::VariantTimeToSystemTime( nLastDivDate, &stDivDate ) )
//	{
//		::SetLastError( ERROR_INVALID_PARAMETER );
//		return -1;
//	}
//	if( !::VariantTimeToSystemTime( nToday + nDTE, &stExpiry ) )
//	{
//		::SetLastError( ERROR_INVALID_PARAMETER );
//		return -1;
//	}
//
//
//	// Count dividends
//	while( true )
//	{
//		//if dividends date more then today
//		if (stToday < stDivDate && nCount==0)
//		{	
//			if( stExpiry < stDivDate )
//				break;
//
//			nCount++;
//			if( pFirstDivDate != NULL && nCount == 1 )
//				memcpy( pFirstDivDate, &stDivDate, sizeof(SYSTEMTIME) );
//			continue;
//		}	
//		_addMonths( stDivDate, WORD(12/nFrequency) );
//
//		if( stDivDate < stToday )
//			continue;
//		else if( stDivDate < stExpiry )
//		{
//			nCount++;
//			if( pFirstDivDate != NULL && nCount == 1 )
//				memcpy( pFirstDivDate, &stDivDate, sizeof(SYSTEMTIME) );
//		}
//		else
//			break;
//	}
//
//	return nCount;
//}
//
//
//// Returns number of dividends or -1 if error occured.
//long OPTIONCALC_API GetDividendsCount( long nToday, long nDTE, 
//									   long nLastDivDate, long nFrequency )
//{
//	// Check parameters
//	if( nFrequency != FREQUENCY_MONTHLY		&&  nFrequency != FREQUENCY_QUATERLY &&
//		nFrequency != FREQUENCY_SEMIANNUALY &&  nFrequency != FREQUENCY_ANNUALY    )
//	{
//		::SetLastError( ERROR_INVALID_PARAMETER );
//		return -1;
//	}
//
//	// Calculate dividends count
//	return _getDivCount( nToday, nDTE, nLastDivDate, nFrequency );
//}
//
//
//// Fills array of dividends using given last dividend amount, last dividend day and 
//// dividend frequency. Returns 0 if failed, or value greater than zero otherwise.
//// pnCount contains number of dividends available prior expiry date.
//long OPTIONCALC_API GetDividends( long nToday, long nDTE, 
//								  long nLastDivDate, long nFrequency, double dAmount, 
//								  long nCount, DIVIDEND *pDividends, long *pnCount )
//{
//	// Check parameters
//	if (nCount==0)
//	{
//		*pnCount = 0;
//		return 1;
//	}
//		
//	if( ::IsBadWritePtr(pDividends, sizeof(DIVIDEND) * nCount) )
//	{
//		::SetLastError( ERROR_NOACCESS );
//		return 0;
//	}
//	if( pnCount == NULL )
//	{
//		::SetLastError( ERROR_INVALID_PARAMETER );
//		return 0;
//	}
//
//	SYSTEMTIME stDivDate;
//	DATE	   dDivDate;
//
//	long nDividends = _getDivCount( nToday, nDTE, nLastDivDate, nFrequency, &stDivDate );
//	if( nDividends == -1 )
//		return 0;	// Last error is still available
//	*pnCount = nDividends;
//
//	if( nCount < nDividends )
//	{
//		::SetLastError( ERROR_NOACCESS );
//		return 0;
//	}
//
//	// Fill array of dividends
//	for( int i = 0; i < nDividends; i++ )
//	{
//		::SystemTimeToVariantTime( &stDivDate, &dDivDate );
//
//		pDividends[i].dAmount		= dAmount;
//		pDividends[i].dTimeFraction = (floor(dDivDate) - nToday) / OPM::cdDaysPerYear365;
//
//		_addMonths( stDivDate, WORD(12/nFrequency) );
//	}
//
//	return 1;
//}
//
//long OPTIONCALC_API GetDividends2( long nToday, long nDTE, 
//								  long nLastDivDate, long nFrequency, double dAmount, 
//								  long nCount, double* pDivAmnts, double* pDivDays,
//								  long *pnCount )
//{
//	// Check parameters
//	if (nCount==0)
//	{
//		*pnCount = 0;
//		return 1;
//	}
//		
//	if( ::IsBadWritePtr(pDivAmnts, sizeof(double) * nCount) )
//	{
//		::SetLastError( ERROR_NOACCESS );
//		return 0;
//	}
//	if( ::IsBadWritePtr(pDivDays, sizeof(double) * nCount) )
//	{
//		::SetLastError( ERROR_NOACCESS );
//		return 0;
//	}
//
//	if( pnCount == NULL )
//	{
//		::SetLastError( ERROR_INVALID_PARAMETER );
//		return 0;
//	}
//
//	SYSTEMTIME stDivDate;
//	DATE	   dDivDate;
//
//	long nDividends = _getDivCount( nToday, nDTE, nLastDivDate, nFrequency, &stDivDate );
//	if( nDividends == -1 )
//		return 0;	// Last error is still available
//	*pnCount = nDividends;
//
//	if( nCount < nDividends )
//	{
//		::SetLastError( ERROR_NOACCESS );
//		return 0;
//	}
//
//	// Fill array of dividends
//	for( int i = 0; i < nDividends; i++ )
//	{
//		::SystemTimeToVariantTime( &stDivDate, &dDivDate );
//
//		pDivAmnts[i] = dAmount;
//		pDivDays[i] = (floor(dDivDate) - nToday) / OPM::cdDaysPerYear365;
//
////		ATLTRACE("DivArr = %u:, ", (ULONG)(floor(dDivDate) - nToday));
//
//
//		_addMonths( stDivDate, WORD(12/nFrequency) );
//	}
//
//	return 1;
//}
//
//long OPTIONCALC_API GetBasketDividendsCount(long				nToday, 
//											long				nDTE, 
//											REGULAR_DIVIDENDS*	pDividends,
//											long				nCount)
//{
//	if (nCount < 0)
//		return -1;
//
//	if (nCount > 0 && ::IsBadWritePtr(pDividends, sizeof(double) * nCount))
//	{
//		::SetLastError( ERROR_NOACCESS );
//		return -1;
//	}
//
//	std::map<double, double>	DivMap;
//
//	for (long n = 0; n < nCount; n++)
//	{
//		long _nCount = GetDividendsCount(nToday, nDTE, 
//										pDividends[n].nLastDivDate, pDividends[n].nFrequency);
//
//		if (_nCount <= 0)
//			continue;
//
//		double* pDivAmnt = (double*)_alloca(_nCount*sizeof(double));
//		double* pDivDays = (double*)_alloca(_nCount*sizeof(double));
//
//		long __nCount;
//
//		long nRes = GetDividends2(	nToday,
//									nDTE,
//									pDividends[n].nLastDivDate,
//									pDividends[n].nFrequency,
//									pDividends[n].dAmount,
//									_nCount, 
//									pDivAmnt,
//									pDivDays,
//									&__nCount);		
//
//		if (nRes != 1 || __nCount != _nCount)
//			return -1;
//
//		for (long i = 0; i < _nCount; i++)
//		{
//			if (DivMap.find(pDivDays[i]) != DivMap.end())
//				DivMap[pDivDays[i]] = DivMap[pDivDays[i]] + pDivAmnt[i];
//			else
//				DivMap[pDivDays[i]] = pDivAmnt[i];
//		}
//	}
//	
//	return DivMap.size();
//}
//
//long OPTIONCALC_API GetBasketDividends(	long				nToday, 
//										long				nDTE, 
//										REGULAR_DIVIDENDS*	pDividends,
//										unsigned			nCount,
//										double*				pDivAmnts,
//										double*				pDivDays,
//										long				nInCount,
//										long*				pnOutCount)
//{
	//if (nCount < 0)
	//	return 1;

	//if (nCount > 0 && ::IsBadWritePtr(pDividends, sizeof(double) * nCount))
	//{
	//	::SetLastError( ERROR_NOACCESS );
	//	return 1;
	//}

	//if (::IsBadWritePtr(pnOutCount, sizeof(long)))
	//{
	//	::SetLastError( ERROR_NOACCESS );
	//	return 1;
	//}

	//std::map<double, double>	DivMap;

	//for (unsigned n = 0; n < nCount; n++)
	//{
	//	long _nCount = GetDividendsCount(nToday, nDTE, 
	//									pDividends[n].nLastDivDate, pDividends[n].nFrequency);

	//	if (_nCount <= 0)
	//		continue;

	//	double* pDivAmnt = (double*)_alloca(_nCount*sizeof(double));
	//	double* pDivDays = (double*)_alloca(_nCount*sizeof(double));

	//	long __nCount;

	//	long nRes = GetDividends2(	nToday,
	//								nDTE,
	//								pDividends[n].nLastDivDate,
	//								pDividends[n].nFrequency,
	//								pDividends[n].dAmount,
	//								_nCount, 
	//								pDivAmnt,
	//								pDivDays,
	//								&__nCount);		

	//	if (nRes != 1 || __nCount != _nCount)
	//		return -1;

	//	for (long i = 0; i < _nCount; i++)
	//	{

	//		if (DivMap.find(pDivDays[i]) != DivMap.end())
	//			DivMap[pDivDays[i]] = DivMap[pDivDays[i]] + pDivAmnt[i];
	//		else
	//			DivMap[pDivDays[i]] = pDivAmnt[i];
	//	}
	//}
	//
	//*pnOutCount = DivMap.size();
	//
	//if (nInCount < (long)DivMap.size())
	//	return 0;

	//std::map<double, double>::iterator	it = DivMap.begin();

	//for (int i = 0; it != DivMap.end(); it++, i++)
	//{
	//	pDivAmnts[i] = it->second;
	//	pDivDays[i] = it->first;
	//}

	//return 0;
//}

////////////////////////////////////////////////////////////
// Implied volatility
////////////////////////////////////////////////////////////

// Calculates volatility. Returns -1 if error occured.
double OPTIONCALC_API CalcVolatility( double dDomesticRate, double dForeignRate,
									  double dSpotPrice, double dOptionPrice, double dStrike, 
	 								  double dYTE, long nIsCall, long nIsAmerican,
									  long nCount, const DIVIDEND *pDividends, long nSteps )
{
	// Check parameters
	
	if(nCount > 0)
	{
		if( ::IsBadReadPtr(pDividends, sizeof(DIVIDEND) * nCount))
		{
			::SetLastError( ERROR_NOACCESS );
			return -1;
		}
	}

	// Prepare dividends data
	
	std::vector<double> divytes;
	std::vector<double> divamts;
	PrepareDivData(pDividends,&divytes,&divamts,nCount);
	
	double VPrec    = 0.0001;
	double volleft  = 0.01;
	double volright = 4.0;	// 2.5;

	double ImpVol = -1;
	double Delta  = VPrec + 1;
	int    Count  = -30; // 30 iteration and ger out here;
	double volmed;
	double OptValmid;

	// Calculate boundary values
	double OptValmin = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, volleft,
										dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );
	if(OPM::IsBadValue(OptValmin))
		OptValmin = 0.;

	double OptValmax = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, volright,
										dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );
	if(OPM::IsBadValue(OptValmax))
		OptValmax = 0.;

	if( dOptionPrice <= OptValmin + 0.005 || dOptionPrice >= OptValmax )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return -1;
	}

	// Iterate
	while( fabs(Delta) >= VPrec && Count != 0 )
	{
		Count++;
	
		volmed    = 0.5 * ( volleft + volright );
		OptValmid = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, volmed,
									 dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );

		if(OPM::IsBadValue(OptValmid))
			OptValmid = 0.;

		Delta  = OptValmid - dOptionPrice; 
		ImpVol = volmed;
		if( Delta > 0.0 )
			volright = volmed;
		else
			volleft = volmed;
	}

	if( Count == 0 || fabs(Delta) > VPrec )
	{
		::SetLastError( ERROR_INVALID_DATA );
		return -1;
	}

	return ImpVol;
}





double OPTIONCALC_API CalcVolatilityCustDivs( double dDomesticRate, double dForeignRate,
									  double dSpotPrice, double dOptionPrice, double dStrike, 
	 								  double dYTE, long nIsCall, long nIsAmerican,
									  long nCount,double * pDivAmnts, double * pDivYears, long nSteps )
{
	// Check parameters
	
	if (nCount > 0)
	{
		if( ::IsBadReadPtr(pDivAmnts, sizeof(double) * nCount) || 
			::IsBadReadPtr(pDivYears, sizeof(double) * nCount))
		{
			::SetLastError(ERROR_NOACCESS);
			return 0;
		}
	}

	
	double VPrec    = 0.0001;
	double volleft  = 0.01;
	double volright = 4.0;	// 2.5;

	double ImpVol = -1;
	double Delta  = VPrec + 1;
	int    Count  = -30; // 30 iteration and ger out here;
	double volmed;
	double OptValmid;

	// Calculate boundary values
	double OptValmin = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, volleft,
										dYTE, nIsCall, nIsAmerican, pDivAmnts, pDivYears, nCount );
	if(OPM::IsBadValue(OptValmin))
		OptValmin = 0.;

	double OptValmax = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, volright,
										dYTE, nIsCall, nIsAmerican, pDivAmnts, pDivYears, nCount );
	if(OPM::IsBadValue(OptValmax))
		OptValmax = 0.;

	if( dOptionPrice <= OptValmin + 0.005 || dOptionPrice >= OptValmax )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return -1;
	}

	// Iterate
	while( fabs(Delta) >= VPrec && Count != 0 )
	{
		Count++;
	
		volmed    = 0.5 * ( volleft + volright );
		OptValmid = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, volmed,
									 dYTE, nIsCall, nIsAmerican, pDivAmnts, pDivYears, nCount );

		if(OPM::IsBadValue(OptValmid))
			OptValmid = 0.;

		Delta  = OptValmid - dOptionPrice; 
		ImpVol = volmed;
		if( Delta > 0.0 )
			volright = volmed;
		else
			volleft = volmed;
	}

	if( Count == 0 || fabs(Delta) > VPrec )
	{
		::SetLastError( ERROR_INVALID_DATA );
		return -1;
	}

	return ImpVol;
}




////////////////////////////////////////////////////////////
// Greeks
////////////////////////////////////////////////////////////
// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
long OPTIONCALC_API CalcGreeksCustDivs( double dDomesticRate, double dForeignRate, 
								double dSpotPrice, double dStrike, double dVolatility, 
								double dYTE, long nIsCall, long nIsAmerican, 
								long nCount, double * pDivAmts, double * pDivYtes, long nSteps, 
								/*out*/GREEKS *pGreeks )
{

	// Check parameters
	if (nCount > 0)
	{
		if( ::IsBadReadPtr(pDivAmts, sizeof(double) * nCount) || 
			::IsBadReadPtr(pDivYtes, sizeof(double) * nCount))
		{
			::SetLastError(ERROR_NOACCESS);
			return 0;
		}
	}

	if( ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) || ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) )
	{
		::SetLastError( ERROR_NOACCESS );
		return 0L;
	}
	
	if( pGreeks->nMask == GT_NOTHING )
		return 1L;	// Nothing to calculate

	if( pGreeks->nMask & GT_ALPHA )
	{
		pGreeks->nMask = pGreeks->nMask | GT_GAMMA;
		pGreeks->nMask = pGreeks->nMask | GT_THETA;
	}

	// Calculated greeks mask
	long cCalculatedGreeksMask = GT_NOTHING;
	
	// Prepare dividends data
//	std::vector<double> divytes;
//	std::vector<double> divamts;
//	PrepareDivData(pDividends,&divytes,&divamts,nCount);

	// Calculate option price
	pGreeks->dTheoPrice = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility,
										 dYTE, nIsCall, nIsAmerican, pDivAmts, pDivYtes, nCount);

	if(OPM::IsBadValue(pGreeks->dTheoPrice) || !OPM::ValueNEQZero(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		pGreeks->nMask = cCalculatedGreeksMask;
		return 0L;
	}

	cCalculatedGreeksMask = cCalculatedGreeksMask | GT_THEOPRICE;

	double dS = 0.0;
	if( pGreeks->nMask & GT_ALPHA || pGreeks->nMask & GT_DELTA || pGreeks->nMask & GT_GAMMA )
	{
		dS = dSpotPrice * ( (dDomesticRate - 0.5 * dVolatility * dVolatility)/OPM::cdDaysPerYear365 + OPM::cdDeltaSqrtTime * dVolatility );
		_ASSERT( dS );
	}

	if( pGreeks->nMask & GT_DELTA && OPM::ValueNEQZero(dS) )
	{
		double op = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice + dS, dStrike, dVolatility,
									 dYTE, nIsCall, nIsAmerican,  pDivAmts, pDivYtes, nCount );

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op))
		{
			pGreeks->dDelta = (op - pGreeks->dTheoPrice) / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_DELTA;
		}
	}

	if( pGreeks->nMask & GT_GAMMA && OPM::ValueNEQZero(dS)  )
	{
		double op1 = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice + dS, dStrike, dVolatility,
									  dYTE, nIsCall, nIsAmerican, pDivAmts, pDivYtes, nCount );
		double op2 = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice - dS, dStrike, dVolatility,
									  dYTE, nIsCall, nIsAmerican,  pDivAmts, pDivYtes, nCount );

		if(!OPM::IsBadValue(op1) && OPM::ValueNEQZero(op1) && !OPM::IsBadValue(op2) && OPM::ValueNEQZero(op2))
		{
			pGreeks->dGamma = (op1 + op2 - 2 * pGreeks->dTheoPrice) / dS / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_GAMMA;
		}
	}

	if( pGreeks->nMask & GT_THETA )
	{
		double op = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility,
			dYTE - (1.0 / OPM::cdDaysPerYear365), nIsCall, nIsAmerican,  pDivAmts, pDivYtes, nCount );

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op))
		{
			pGreeks->dTheta = op - pGreeks->dTheoPrice;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_THETA;
		}
	}

	if( pGreeks->nMask & GT_VEGA )
	{
		double op = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility + OPM::cdDeltaVolatility,
								     dYTE, nIsCall, nIsAmerican,  pDivAmts, pDivYtes, nCount  );

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op))
		{
			pGreeks->dVega = op - pGreeks->dTheoPrice;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_VEGA;
		}
	}

	if( pGreeks->nMask & GT_RHO )
	{
		double op = CO_BlackScholes( dDomesticRate + OPM::cdDeltaRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility,
								     dYTE, nIsCall, nIsAmerican,  pDivAmts, pDivYtes, nCount );


		// Calc continuous rate 
		double R = OPM::RateDiscToCont( dDomesticRate, dYTE * OPM::cdDaysPerYear365);

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op) && OPM::ValueNEQZero(R))
		{
			double rho = op - pGreeks->dTheoPrice;
			if(!nIsAmerican ||
			   (nIsAmerican && nIsCall && nCount == 0))
				pGreeks->dRho = rho * OPM::cdDaysPerYear365 / OPM::cdDaysPerYear360 * exp(-R * OPM::cdDaysPerYear360 / OPM::cdDaysPerYear365);
			else
				pGreeks->dRho = rho;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_RHO;
		}
	}

	if( pGreeks->nMask & GT_ALPHA && OPM::ValueNEQZero(pGreeks->dTheta))
	{
		pGreeks->dAlpha = pGreeks->dGamma / pGreeks->dTheta;
		cCalculatedGreeksMask = cCalculatedGreeksMask | GT_ALPHA;
	}

	// Set up greeks mask
	pGreeks->nMask = cCalculatedGreeksMask;

	return 1L;

}

// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
long OPTIONCALC_API CalcGreeks( double dDomesticRate, double dForeignRate, 
								double dSpotPrice, double dStrike, double dVolatility, 
								double dYTE, long nIsCall, long nIsAmerican, 
								long nCount, const DIVIDEND *pDividends, long nSteps, 
								/*out*/GREEKS *pGreeks )
{
	// Check parameters
	if (nCount != 0)
	{
		if( ::IsBadReadPtr(pDividends, sizeof(DIVIDEND) * nCount))
		{
			::SetLastError( ERROR_NOACCESS );
			return 0L;
		}
	}

	if( ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) || ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) )
	{
		::SetLastError( ERROR_NOACCESS );
		return 0L;
	}
	
	if( pGreeks->nMask == GT_NOTHING )
		return 1L;	// Nothing to calculate

	if( pGreeks->nMask & GT_ALPHA )
	{
		pGreeks->nMask = pGreeks->nMask | GT_GAMMA;
		pGreeks->nMask = pGreeks->nMask | GT_THETA;
	}

	// Calculated greeks mask
	long cCalculatedGreeksMask = GT_NOTHING;
	
	// Prepare dividends data
	std::vector<double> divytes;
	std::vector<double> divamts;
	PrepareDivData(pDividends,&divytes,&divamts,nCount);

	// Calculate option price
	pGreeks->dTheoPrice = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility,
										 dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount);

	if(OPM::IsBadValue(pGreeks->dTheoPrice) || !OPM::ValueNEQZero(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		pGreeks->nMask = cCalculatedGreeksMask;
		return 0L;
	}

	cCalculatedGreeksMask = cCalculatedGreeksMask | GT_THEOPRICE;

	double dS = 0.0;
	if( pGreeks->nMask & GT_ALPHA || pGreeks->nMask & GT_DELTA || pGreeks->nMask & GT_GAMMA )
	{
		dS = dSpotPrice * ( (dDomesticRate - 0.5 * dVolatility * dVolatility)/OPM::cdDaysPerYear365 + OPM::cdDeltaSqrtTime * dVolatility );
		_ASSERT( dS );
	}

	if( pGreeks->nMask & GT_DELTA && OPM::ValueNEQZero(dS) )
	{
		double op = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice + dS, dStrike, dVolatility,
									 dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op))
		{
			pGreeks->dDelta = (op - pGreeks->dTheoPrice) / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_DELTA;
		}
	}

	if( pGreeks->nMask & GT_GAMMA && OPM::ValueNEQZero(dS)  )
	{
		double op1 = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice + dS, dStrike, dVolatility,
									  dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );
		double op2 = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice - dS, dStrike, dVolatility,
									  dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );

		if(!OPM::IsBadValue(op1) && OPM::ValueNEQZero(op1) && !OPM::IsBadValue(op2) && OPM::ValueNEQZero(op2))
		{
			pGreeks->dGamma = (op1 + op2 - 2 * pGreeks->dTheoPrice) / dS / dS;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_GAMMA;
		}
	}

	if( pGreeks->nMask & GT_THETA )
	{
		double op = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility,
			dYTE - (1.0 / OPM::cdDaysPerYear365), nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op))
		{
			pGreeks->dTheta = op - pGreeks->dTheoPrice;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_THETA;
		}
	}

	if( pGreeks->nMask & GT_VEGA )
	{
		double op = CO_BlackScholes( dDomesticRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility + OPM::cdDeltaVolatility,
								     dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount  );

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op))
		{
			pGreeks->dVega = op - pGreeks->dTheoPrice;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_VEGA;
		}
	}

	if( pGreeks->nMask & GT_RHO )
	{
		double op = CO_BlackScholes( dDomesticRate + OPM::cdDeltaRate, dForeignRate, OPM::BadDoubleValue, dSpotPrice, dStrike, dVolatility,
								     dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount );


		// Calc continuous rate 
		double R = OPM::RateDiscToCont( dDomesticRate, dYTE * OPM::cdDaysPerYear365 );

		if(!OPM::IsBadValue(op) && OPM::ValueNEQZero(op) && OPM::ValueNEQZero(R))
		{
			double rho = op - pGreeks->dTheoPrice;
			if(!nIsAmerican ||
			   (nIsAmerican && nIsCall && nCount == 0))
				pGreeks->dRho = rho * OPM::cdDaysPerYear365 / OPM::cdDaysPerYear360 * exp(-R * OPM::cdDaysPerYear360 / OPM::cdDaysPerYear365);
			else
				pGreeks->dRho = rho;
			cCalculatedGreeksMask = cCalculatedGreeksMask | GT_RHO;
		}
	}

	if( pGreeks->nMask & GT_ALPHA && OPM::ValueNEQZero(pGreeks->dTheta))
	{
		pGreeks->dAlpha = pGreeks->dGamma / pGreeks->dTheta;
		cCalculatedGreeksMask = cCalculatedGreeksMask | GT_ALPHA;
	}

	// Set up greeks mask
	pGreeks->nMask = cCalculatedGreeksMask;

	return 1L;
}


// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
// Uses alternative algorithm to calculate Greeks.
long OPTIONCALC_API CalcGreeksEx( double dDomesticRate, double dForeignRate, 
								  double dSpotPrice, double dStrike, double dVolatility, 
								  double dYTE, long nIsCall, long nIsAmerican, 
								  long nCount, const DIVIDEND *pDividends, long nSteps, 
								  /*out*/GREEKS *pGreeks )
{

	// Check parameters
	if( ::IsBadReadPtr(pDividends, sizeof(DIVIDEND) * nCount) && nCount!=0)
	{
		::SetLastError( ERROR_NOACCESS );
		return 0;
	}
	if( ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) || ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) )
		{
		::SetLastError( ERROR_NOACCESS );
		return 0;
		}

	if( pGreeks->nMask == GT_NOTHING )
		return 1;	// Nothing to calculate

	if( pGreeks->nMask & GT_ALPHA )
		{
		pGreeks->nMask = pGreeks->nMask | GT_GAMMA;
		pGreeks->nMask = pGreeks->nMask | GT_THETA;
		}

	// Calculated greeks mask
	long cGreeksMask = pGreeks->nMask;
	pGreeks->nMask = GT_NOTHING;

	// Prepare dividends data
	std::vector<double> divytes;
	std::vector<double> divamts;
	PrepareDivData(pDividends,&divytes,&divamts,nCount);

	// Calc continuous rate 
	double R = OPM::RateDiscToCont( dDomesticRate, dYTE * OPM::cdDaysPerYear365);

	// Calculate option price
	pGreeks->dTheoPrice = CalculateOptionEx( dDomesticRate, dForeignRate, dSpotPrice, dStrike, dVolatility,
										     dYTE, nIsCall, nIsAmerican, &divamts.front(), &divytes.front(), nCount, nSteps, pGreeks );
	if(OPM::IsBadValue(pGreeks->dTheoPrice) || !OPM::ValueNEQZero(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}
	return 1;
}



// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
// Uses alternative algorithm to calculate Greeks.
long OPTIONCALC_API CalcGreeksExCustDivs( double dDomesticRate, double dForeignRate, 
								  double dSpotPrice, double dStrike, double dVolatility, 
								  double dYTE, long nIsCall, long nIsAmerican, 
								  long nCount,  double*	pDivAmts, double* pDivYtes, long nSteps, 
								  /*out*/GREEKS *pGreeks )
{

	// Check parameters
	if (nCount > 0)
	{
		if( ::IsBadReadPtr(pDivAmts, sizeof(double) * nCount) || 
			::IsBadReadPtr(pDivYtes, sizeof(double) * nCount))
		{
			::SetLastError(ERROR_NOACCESS);
			return 0;
		}
	}
	
	if( ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) || ::IsBadWritePtr(pGreeks, sizeof(GREEKS)) )
		{
		::SetLastError( ERROR_NOACCESS );
		return 0;
		}

	if( pGreeks->nMask == GT_NOTHING )
		return 1;	// Nothing to calculate

	if( pGreeks->nMask & GT_ALPHA )
		{
		pGreeks->nMask = pGreeks->nMask | GT_GAMMA;
		pGreeks->nMask = pGreeks->nMask | GT_THETA;
		}

	// Calculated greeks mask
	long cGreeksMask = pGreeks->nMask;
	pGreeks->nMask = GT_NOTHING;

	// Prepare dividends data
//	std::vector<double> divytes;
//	std::vector<double> divamts;
//	PrepareDivData(pDividends,&divytes,&divamts,nCount);

	// Calc continuous rate 
	double R = OPM::RateDiscToCont( dDomesticRate, dYTE * OPM::cdDaysPerYear365 );

	// Calculate option price
	pGreeks->dTheoPrice = CalculateOptionEx( dDomesticRate, dForeignRate, dSpotPrice, dStrike, dVolatility,
										     dYTE, nIsCall, nIsAmerican, pDivAmts, pDivYtes, nCount, nSteps, pGreeks );
	if(OPM::IsBadValue(pGreeks->dTheoPrice) || !OPM::ValueNEQZero(pGreeks->dTheoPrice))
	{
		pGreeks->dTheoPrice = 0.;
		return 0L;
	}
	return 1;
}


////////////////////////////////////////////////////////////
// Forward
////////////////////////////////////////////////////////////

double OPTIONCALC_API CalcForwardPrice( double dSpotPrice, 
	                                    double dYTE, /*date of option expiration*/
	                                    long nDivCount, /* size of dividends array*/
	                                    double* pDivAmnts, /* array of dividends amounts*/
	                                    double* pDivYears, /* array of time to dividends payments*/
	                                    double dForeignRate, /*yield for index option */
	                                    long nRateCount, /*size of rates array*/
	                                    const RATE *pRates /*array of rates*/ )
{
	// Check parameters
	if( nDivCount > 0 && ::IsBadReadPtr(pDivAmnts, sizeof(double) * nDivCount) )
	{
		::SetLastError( ERROR_NOACCESS );
		return -1;
	}

	if( nDivCount > 0 && IsBadReadPtr(pDivYears, sizeof(double) * nDivCount) )
	{
		::SetLastError( ERROR_NOACCESS );
		return -1;
	}

	if( nRateCount > 0 && ::IsBadReadPtr(pRates, sizeof(double) * nRateCount) )
	{
		::SetLastError( ERROR_NOACCESS );
		return -1;
	}

	if( dYTE < 0 )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return -1;
	}

    double dPrice = 0;
    double dRate = 0;
    double dDTEYears = dYTE;

    if (nDivCount > 0)
    {
        dPrice = dSpotPrice;

        for (int i = 0; i < nDivCount; i++)
        {
            if (pDivYears[i] >= dDTEYears || pDivAmnts[i] <= 0 || pDivYears[i] < 0 ) continue;

            if (nRateCount > 0)
            {
                dRate = InterpolateRates(nRateCount, pRates, pDivYears[i]);
                if (dRate == 0) return -1;
            }

            dPrice -= pDivAmnts[i] * exp(-dRate * pDivYears[i]);
            if (dPrice < 0) 
                return 0;
        }

        if (nRateCount > 0)
        {
			dRate = InterpolateRates(nRateCount, pRates, dYTE );
            if (dRate == 0) return -1;
        }
    }
    else
    {
        dPrice = dSpotPrice;
        if (nRateCount > 0)
        {
			dRate = InterpolateRates(nRateCount, pRates, dYTE);
            if (dRate == 0) return -1;
        }
        dRate -=  dForeignRate;
    }

    dPrice = dPrice * exp(dRate * dDTEYears);

    return dPrice;
}

double OPTIONCALC_API CalcForwardPrice2( double dSpotPrice, 
	                                     double dYTE, /*date of option expiration*/
	                                     long nDivCount, /* size of dividends array*/
	                                     double* pDivAmnts, /* array of dividends amounts*/
	                                     double* pDivYears, /* array of time to dividends payments*/
	                                     double dForeignRate, /*yield for index option */
	                                     long nRateCount, /*size of rates array*/
	                                     double* pRates, /*array of rates*/
                                         double* pdYTEs /*array of time to rates*/ )
{
	// Check parameters
    std::vector<RATE> vec(nRateCount);
    if (nRateCount > 0)
    {
    	if( ::IsBadReadPtr(pRates, sizeof(*pRates) * nRateCount) ||
            ::IsBadReadPtr(pdYTEs, sizeof(*pdYTEs) * nRateCount) )
	    {
		    ::SetLastError( ERROR_NOACCESS );
		    return 0;
	    }

        for (int i = 0; i < nRateCount; i++)
        {
            RATE& r = vec[i];
            r.dRate = pRates[i];
			r.nDTE =  static_cast<long>(pdYTEs[i] * OPM::cdDaysPerYear365);
        }
    }

    return CalcForwardPrice(dSpotPrice, dYTE, nDivCount, pDivAmnts,
        pDivYears, dForeignRate, nRateCount, nRateCount > 0 ? &vec.front() : NULL);
}

////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////

// Check is symbol a call. Returns -1 if error occurred and SetLast Error - ERROR_INVALID_PARAMETER
int OPTIONCALC_API IsCall( LPCTSTR cSymbol )
{
	TCHAR cCalls[] = CALL_CODES;
	TCHAR cPuts[]  = PUT_CODES;

	if( ::IsBadStringPtr( cSymbol, 6 ) )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return -1;
	}

	TCHAR cMonthCode = cSymbol[3];
	_tcsupr_s(&cMonthCode, 1);

	if( _tcsrchr( cCalls, cMonthCode ) )
		return 1;
	if( _tcsrchr( cPuts, cMonthCode ) )
		return 0;

	::SetLastError( ERROR_INVALID_PARAMETER );
	return -1;
}

// Check is two symbols make a pair. Returns -1 if error occurred and SetLast Error - ERROR_INVALID_PARAMETER
int OPTIONCALC_API IsSymbolsPair( LPCTSTR cSymbol1, LPCTSTR cSymbol2 )
{
	TCHAR cCallsPuts[][13] = {CALL_CODES, PUT_CODES};

	if( ::IsBadStringPtr( cSymbol1, 6 ) || ::IsBadStringPtr( cSymbol2, 6 ) )
	{
		::SetLastError( ERROR_INVALID_PARAMETER );
		return -1;
	}

	if( (_tcsnicmp( cSymbol1, cSymbol2, 3 ) != 0) || (cSymbol1[4] != cSymbol2[4]) )
		return 0;

	TCHAR tcSymbol1 = cSymbol1[3];
	TCHAR tcSymbol2 = cSymbol2[3];

	for( int i = 0; i < 13; i++ )
	{
		if( ((cCallsPuts[0][i] == tcSymbol1) && (cCallsPuts[1][i] == tcSymbol2)) ||
			((cCallsPuts[1][i] == tcSymbol1) && (cCallsPuts[0][i] == tcSymbol2)) )
			return 1;
	}
	
	return 0;
}
