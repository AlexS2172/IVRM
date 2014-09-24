// EtsRateColl.cpp : Implementation of CEtsRateColl

#include "stdafx.h"
#include "EtsRateColl.h"


// CEtsRateColl
STDMETHODIMP CEtsRateColl::Add(LONG Key, IEtsRateAtom* Value, IEtsRateAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Rate component with the same key is already exists."));


		if(!Value)
			*pRetVal = AddNew(Key).Detach();
		else
		{
			_CHK(IEtsRateCollImpl::Add(Key, Value), _T("Fail to add rate."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsRateColl, e.Error());
	}

	return S_OK;
}

IEtsRateAtomPtr CEtsRateColl::AddNew(LONG Key, CComObject<CEtsRateAtom>** pAtom)
{
	CComObject<CEtsRateAtom>* pNewVal;
	IEtsRateAtomPtr spRet;

	_CHK(CComObject<CEtsRateAtom>::CreateInstance(&pNewVal), _T("Fail to create rate."));
	spRet.Attach(pNewVal, TRUE);

	pNewVal->m_nPeriod = Key;

	if(FAILED(IEtsRateCollImpl::Add(Key, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add rate."));

	if(pAtom)
		*pAtom = pNewVal;
	return spRet;
}


STDMETHODIMP CEtsRateColl::GetShortRate(DATE dtToday, DATE dtExpiry, DOUBLE* pdRate)
{
	__CHECK_POINTER(pdRate);

	*pdRate = 0;

	ULONG nCount = static_cast<ULONG>(m_coll.size());

	if(nCount == 0 || dtToday > dtExpiry)
		return E_FAIL;

	try
	{
		boost::scoped_array<DOUBLE>	spDates(new DOUBLE[nCount]);
		boost::scoped_array<DOUBLE> spRates(new DOUBLE[nCount]);

		long idx = 0;
		for(CollType::const_iterator itr = m_coll.begin(); itr != m_coll.end(); ++itr, ++idx)
		{
			LONG	date = 0;
			DOUBLE	rate = 0;
			(itr->second)->get_Period(&date);
			(itr->second)->get_ShortRate(&rate);

			spDates[idx] = static_cast<DOUBLE>(date) / DAYS_IN_YEAR;
			spRates[idx] = rate;
		}

		DOUBLE dYTE = ceil(dtExpiry - dtToday) / DAYS_IN_YEAR;
		*pdRate = ::InterpolateRates2(nCount, spRates.get(), spDates.get(), dYTE);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsRateColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CEtsRateColl::GetLongRate(DATE dtToday, DATE dtExpiry, DOUBLE* pdRate)
{
	__CHECK_POINTER(pdRate);

	*pdRate = 0;

	ULONG nCount = static_cast<ULONG>(m_coll.size());

	if(nCount == 0 || dtToday > dtExpiry)
		return E_FAIL;

	try
	{
		boost::scoped_array<DOUBLE>	spDates(new DOUBLE[nCount]);
		boost::scoped_array<DOUBLE> spRates(new DOUBLE[nCount]);

		long idx = 0;
		for(CollType::const_iterator itr = m_coll.begin(); itr != m_coll.end(); ++itr, ++idx)
		{
			LONG	date = 0;
			DOUBLE	rate = 0;
			(itr->second)->get_Period(&date);
			(itr->second)->get_LongRate(&rate);

			spDates[idx] = static_cast<DOUBLE>(date) / DAYS_IN_YEAR;
			spRates[idx] = rate;
		}

		DOUBLE dYTE = ceil(dtExpiry - dtToday) / DAYS_IN_YEAR;
		*pdRate = ::InterpolateRates2(nCount, spRates.get(), spDates.get(), dYTE);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsRateColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CEtsRateColl::GetNeutralRate(DATE dtToday, DATE dtExpiry, DOUBLE* pdRate)
{
	__CHECK_POINTER(pdRate);

	*pdRate = BAD_DOUBLE_VALUE;

	ULONG nCount = static_cast<ULONG>(m_coll.size());

	if(nCount == 0 || dtToday > dtExpiry)
		return S_OK;

	try
	{
		boost::scoped_array<DOUBLE>	spDates(new DOUBLE[nCount]);
		boost::scoped_array<DOUBLE> spRates(new DOUBLE[nCount]);
		
		long idx = 0;
		for(CollType::const_iterator itr = m_coll.begin(); itr != m_coll.end(); ++itr, ++idx)
		{
			LONG	date = 0;
			DOUBLE	rate = 0;
			(itr->second)->get_Period(&date);
			(itr->second)->get_NeutralRate(&rate);

			spDates[idx] = static_cast<DOUBLE>(date) / DAYS_IN_YEAR;
			spRates[idx] = rate;
		}

		DOUBLE dYTE = ceil(dtExpiry - dtToday) / DAYS_IN_YEAR;
		*pdRate = ::InterpolateRates2(nCount, spRates.get(), spDates.get(), dYTE);
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsRateColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CEtsRateColl::CopyTo(IEtsRateColl** ppRetVal)
{
	__CHECK_POINTER(ppRetVal);
	if(*ppRetVal != NULL)
		return E_INVALIDARG;
	
	try
	{
		
		CComObject<CEtsRateColl>* pRateColl; 
		_CHK(CComObject<CEtsRateColl>::CreateInstance(&pRateColl),  _T("Fail to copy rates."));
		pRateColl->AddRef();
		*ppRetVal = pRateColl;
		
		CollType::const_iterator itr_b = m_coll.begin();
		CollType::const_iterator itr_e = m_coll.end();
		
		for(CollType::const_iterator itr = itr_b; itr != itr_e; ++itr)
		{
			LONG	date = 0;
			(itr->second)->get_Period(&date);

			IEtsRateAtomPtr spRateAtom = pRateColl->AddNew(date);

			LONG ID = BAD_LONG_VALUE;
			(itr->second)->get_ID(&ID);
			spRateAtom->put_ID(ID);

			DOUBLE	rate = BAD_DOUBLE_VALUE;
			(itr->second)->get_LongRate(&rate);
			spRateAtom->put_LongRate(rate);

			(itr->second)->get_ShortRate(&rate);
			spRateAtom->put_ShortRate(rate);

			(itr->second)->get_NeutralRate(&rate);
			spRateAtom->put_NeutralRate(rate);

			EtsPeriodTypeEnum enPeriodType = enPeriodDays;
			(itr->second)->get_PeriodType(&enPeriodType);
			spRateAtom->put_PeriodType(enPeriodType);

			(itr->second)->get_PeriodAmount(&date);
			spRateAtom->put_PeriodAmount(date);
		}
		
	}
	catch (const _com_error& e) 
	{
		return  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsRateColl, e.Error());
	}
	
	return S_OK;
}

STDMETHODIMP CEtsRateColl::GetPeriodInDays(DATE dtToday, 
										   EtsPeriodTypeEnum enPeriodType, 
										   LONG nPeriodAmount, 
										   LONG* pnPeriod)
{
	__CHECK_POINTER(pnPeriod);

	vt_date dtStrtDate(dtToday);
	vt_date dtDestDate(dtToday);

	switch(enPeriodType)
	{
	case enPeriodDays:
		dtDestDate += nPeriodAmount;
		break;

	case enPeriodWeeks:
		dtDestDate += (nPeriodAmount * 7);
		break;

	case enPeriodMonths:
		{
			WORD nYear		= dtStrtDate.get_year();
			WORD nMonth		= dtStrtDate.get_month() + static_cast<WORD>(nPeriodAmount);

			while(nMonth > 12)
			{
				nYear++;
				nMonth -= 12;
			}

			dtDestDate = vt_date(nYear, nMonth, dtStrtDate.get_day());
		}
		break;

	case enPeriodYears:
		dtDestDate = vt_date(dtStrtDate.get_year() + static_cast<WORD>(nPeriodAmount), 
							 dtStrtDate.get_month(), 
							 dtStrtDate.get_day());
		break;

	default:
		break;
	}

	*pnPeriod = (dtDestDate - dtStrtDate).get_days();

	return S_OK;
}