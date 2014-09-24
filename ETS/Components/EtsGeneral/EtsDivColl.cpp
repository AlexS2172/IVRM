// EtsDivColl.cpp : Implementation of CEtsDivColl

#include "stdafx.h"
#include "EtsDivColl.h"

#include "OptionCalc\OptionCalc.h"
#include "OptionPriceModels\common.h"
#include ".\etsdivcoll.h"
#include <atlsafe.h>
#include <atlsafe.h>

// CEtsDivColl
STDMETHODIMP CEtsDivColl::GetDividends(DATE nToday, DATE nExpiry, LONG nCount, SAFEARRAY ** psaDivAmounts,  SAFEARRAY ** psaDivDates, LONG* pnCount)
{
	__CHECK_POINTER(pnCount);
	*pnCount = 0;
	__CHECK_POINTER(psaDivAmounts);
	__CHECK_POINTER(psaDivDates);

	if (nExpiry <= nToday) return S_OK;

	HRESULT hr = S_OK;
	try
	{
		CComSafeArray<double> spDates(nCount);
		CComSafeArray<double> spAmount(nCount);

		EnumIterType iter;
		EnumIterType iterStart = m_DivColl.lower_bound(static_cast<LONG>(nToday));
		EnumIterType iterExp = m_DivColl.lower_bound(static_cast<LONG>(nExpiry));
		EnumIterType iterEnd = m_DivColl.end();

		if ( iterStart != iterEnd && iterStart != iterExp )
		{
			LONG lDivCount = 0;
			LONG lCurDiv = 0;
			DOUBLE dDivDate;
			DOUBLE dCurAmount;

			for( iter = iterStart; iter != iterExp; ++iter)
			{
				dCurAmount = iter->second;
				dDivDate = iter->first;
				dDivDate = (dDivDate - nToday) / OPM::cdDaysPerYear365;

				spDates[lCurDiv] = dDivDate;
				spAmount[lCurDiv] = dCurAmount;

				++lCurDiv;
			}
			*pnCount = lCurDiv;


			m_nTodayCache    = nToday;
			m_lExpryCache    = nExpiry;
			m_lQuantityCache = lCurDiv;
		}

		*psaDivAmounts = spAmount.Detach();
		*psaDivDates = spDates.Detach();

	}
	catch(const _com_error& e)
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error((PTCHAR)(_T("Unhandled exception at GetDividends")), IID_IEtsDivColl, E_FAIL);
	}
	return hr;
}

STDMETHODIMP CEtsDivColl::GetNearest( DATE nToday, DATE nExpiry, DOUBLE* pdDivAmount,  DOUBLE* pdDivDate)
{
	__CHECK_POINTER(pdDivAmount);
	__CHECK_POINTER(pdDivDate);

	if ( nExpiry > 0 && nExpiry <= nToday )	return E_FAIL;
	if ( m_nTodayCache != nToday ) ClearCache();

	HRESULT hr = S_OK;
	try
	{
		EnumIterType iterStart = m_DivColl.upper_bound(static_cast<LONG>(nToday));
		EnumIterType iterEnd = m_DivColl.end();
		EnumIterType iterExp;
		if (nExpiry > 0)
			iterExp = m_DivColl.lower_bound(static_cast<LONG>(nExpiry));
		else
			iterExp = iterEnd;

		

		if ((iterStart != iterEnd)&&(iterStart != iterExp))
		{
			*pdDivAmount = iterStart->second;	
			*pdDivDate = iterStart->first;
		}
	}
	catch(const _com_error& e)
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error((PTCHAR)(_T("Unhandled exception at GetNearest")), IID_IEtsDivColl, E_FAIL);
	}
	return hr;
}



STDMETHODIMP CEtsDivColl::GetDividendCount(DATE nToday, DATE nExpiry,  LONG* pnCount)
{
	__CHECK_POINTER(pnCount);
	*pnCount = 0;
	long lCount = 0;

	if (nExpiry <= nToday) return S_OK;


	HRESULT hr = S_OK;
	try
	{
		if(m_nTodayCache == nToday && m_lExpryCache == nExpiry)
			*pnCount = m_lQuantityCache;
		else
		{
			EnumIterType iter;
			EnumIterType iterStart = m_DivColl.upper_bound(static_cast<LONG>(nToday));
			EnumIterType iterEnd = m_DivColl.end();
			EnumIterType iterExp = m_DivColl.lower_bound(static_cast<LONG>(nExpiry));

			if( iterStart != iterEnd && iterStart != iterExp )
				for(iter = iterStart; iter != iterExp; ++iter)
					++lCount;

			*pnCount = lCount;
			m_nTodayCache    = nToday;
			m_lExpryCache    = nExpiry;
			m_lQuantityCache = lCount;
			m_bDivCacheInit  = false;

		}
	}
	catch(const _com_error& e)
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error((PTCHAR)(_T("Unhandled exception at GetDividendCount")), IID_IEtsDivColl, E_FAIL);
	}
	return hr;

}

STDMETHODIMP CEtsDivColl::Add(DATE Date, DOUBLE Amount)
{
	try
	{
		long lDate = static_cast<long>(Date);

		if(m_DivColl.find(lDate) != m_DivColl.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Div with the same date is already exists."));
		}

		ClearCache();
		m_DivColl.insert(std::pair<LONG, DOUBLE>(lDate, Amount));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}

	return S_OK;
}


STDMETHODIMP CEtsDivColl::AddNonUnique(DATE Date, DOUBLE Amount)
{
	try
	{
		long lDate = static_cast<long>(Date);
		EnumIterType iter = m_DivColl.find(lDate);

		if(iter != m_DivColl.end())
		{
			iter->second += Amount;
		}
		else
			m_DivColl.insert(std::pair<LONG, DOUBLE>(lDate, Amount));

		ClearCache();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}

	return S_OK;
}


STDMETHODIMP CEtsDivColl::Remove(DATE Date)
{
	HRESULT hr = S_OK;
	try
	{
		long lDate = static_cast<long>(Date);
		if(m_DivColl.find(lDate) == m_DivColl.end())
			hr = E_FAIL;
		else
		{
			m_DivColl.erase(lDate);
		}
		ClearCache();
	}
	catch(const _com_error& e)
	{
		hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	return hr;
}

STDMETHODIMP CEtsDivColl::Clear(void)
{
	m_DivColl.clear();
	ClearCache();
	return S_OK;
}

STDMETHODIMP CEtsDivColl::CopyToWithWeight( DOUBLE dWeight, IEtsDivColl* pDest , IEtsDivColl** ppVal)
{
	__CHECK_POINTER(ppVal);
	HRESULT hr = S_OK;
	try
	{
		if (pDest == NULL)
		{
			CComObject<CEtsDivColl>* pCustDivs; 
			_CHK(CComObject<CEtsDivColl>::CreateInstance(&pCustDivs),  _T("Fail to create dividend object."));
			pCustDivs->AddRef();
			pDest = pCustDivs;
		}
		EnumIterType iter = m_DivColl.begin();
		EnumIterType iterEnd = m_DivColl.end();
		DOUBLE dDate = 0;
		DOUBLE dAmount = 0;
		for(;iter != iterEnd; ++iter)
		{
			dDate = iter->first;
			dAmount = iter->second;
			pDest->Add(dDate ,  (dAmount * dWeight) );
		}
		if(ppVal)
		{
			*ppVal = pDest;
			pDest->AddRef();
		}

	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhandled exception handled at CopyToWithWeight"), IID_IEtsDivColl, E_FAIL);
	}
	return hr;
}


STDMETHODIMP CEtsDivColl::Clone(IEtsDivColl** ppVal)
{
	HRESULT hr = S_OK;
	__CHECK_POINTER(ppVal);

	try
	{
		ClearCache();
		IEtsDivCollPtr spColl;
		CComObject<CEtsDivColl>* pColl = NULL;
		_CHK(CComObject<CEtsDivColl>::CreateInstance(&pColl));
		spColl.Attach(pColl, TRUE);
		if(!m_DivColl.empty())
		{
			EnumIterType itr = m_DivColl.begin();
			while(itr!=m_DivColl.end())
			{
				pColl->m_DivColl.insert(*itr);
				++itr;
			}

		}   
		*ppVal = spColl.Detach();
	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	return hr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
////ETM MODEL//////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CEtsDivColl::GetDividendCount2(DATE dtNow, DATE dtExpiryOV, DATE dtCloseTime, LONG* pnCount)
{
	__CHECK_POINTER(pnCount);
	*pnCount = 0;
	long lCount = 0;

	if (dtExpiryOV < dtNow) return S_OK;

	HRESULT hr = S_OK;
	try
	{
		EnumIterType	iter;
		EnumIterType	iterStart	= m_DivColl.begin();
		EnumIterType	iterEnd		= m_DivColl.end();

		DATE			dtDate,	dtPWDate;
		
		for(iter = iterStart; iter != iterEnd; iter++)
		{
			dtPWDate =	dtDate	=	static_cast<DOUBLE>(iter->first);
			if (m_spHolidays)
				m_spHolidays->GetPreviousWorkingDate(dtDate, &dtPWDate);

			dtPWDate += dtCloseTime;
			if ( dtPWDate > dtNow && dtPWDate < dtExpiryOV )
				++lCount;
		}

		*pnCount = lCount;
	}
	catch(const _com_error& e)
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error((PTCHAR)(_T("Unhandled exception at GetDividendCount2")), IID_IEtsDivColl, E_FAIL);
	}
	return hr;
}
STDMETHODIMP CEtsDivColl::GetDividends2(DATE dtNow, DATE dtExpiryOV, DATE dtCloseTime, LONG nCount, SAFEARRAY ** psaDivAmounts,  SAFEARRAY ** psaDivDates, LONG* pnCount)
{
	__CHECK_POINTER(pnCount);
	__CHECK_POINTER(psaDivAmounts);
	__CHECK_POINTER(psaDivDates);

	*pnCount = 0;
	if (dtExpiryOV < dtNow) return S_OK;

	HRESULT hr = S_OK;
	try
	{
		CComSafeArray<double> spDates(nCount);
		CComSafeArray<double> spAmount(nCount);

		EnumIterType iter;
		EnumIterType iterStart = m_DivColl.begin();
		EnumIterType iterEnd = m_DivColl.end();
		
		LONG	lCurDiv		= 0;
		DOUBLE	dDivDate;
		DOUBLE	dCurAmount;
		DATE	dtDate, dtPWDate;

		for( iter = iterStart; iter != iterEnd; iter++)
		{
			dCurAmount	= iter->second;
			dtPWDate	= dtDate = iter->first;

			if (m_spHolidays)
				m_spHolidays->GetPreviousWorkingDate(dtDate, &dtPWDate);

			dtPWDate += dtCloseTime;

			if (dtPWDate > dtNow && dtPWDate < dtExpiryOV){

				dDivDate = (dtPWDate - dtNow) / OPM::cdDaysPerYear365;
				spDates[lCurDiv]	= dDivDate;
				spAmount[lCurDiv]	= dCurAmount;
				++lCurDiv;
			}
		}
		*pnCount = lCurDiv;

		*psaDivAmounts	= spAmount.Detach();
		*psaDivDates	= spDates.Detach();

	}
	catch(const _com_error& e)
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error((PTCHAR)(_T("Unhandled exception at GetDividends2")), IID_IEtsDivColl, E_FAIL);
	}
	return hr;
}


STDMETHODIMP CEtsDivColl::GetNearest2( DATE dtNow, DATE dtExpiryOV, DATE dtCloseTime,  DOUBLE* pdDivAmount,  DOUBLE* pdDivDate)
{
	__CHECK_POINTER(pdDivAmount);
	__CHECK_POINTER(pdDivDate);

	if ( dtExpiryOV <= dtNow )	return E_FAIL;
	
	HRESULT hr = S_OK;
	try
	{
		EnumIterType iter;
		EnumIterType iterStart = m_DivColl.begin();
		EnumIterType iterEnd = m_DivColl.end();
		
		DATE	dtPWDate, dtDate;
		DOUBLE	dCurAmount;


		for( iter = iterStart; iter != iterEnd; iter++)
		{
			dCurAmount	= iter->second;
			dtPWDate	= dtDate = iter->first;
			if (m_spHolidays)
				m_spHolidays->GetPreviousWorkingDate(dtDate, &dtPWDate);

			dtPWDate += dtCloseTime;

			if (dtPWDate > dtNow && dtPWDate < dtExpiryOV)
			{
				*pdDivAmount = dCurAmount;	
				*pdDivDate = dtPWDate;
				break;
			}

		}
	}
	catch(const _com_error& e)
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error((PTCHAR)(_T("Unhandled exception at GetNearest2")), IID_IEtsDivColl, E_FAIL);
	}
	return hr;
}
//------------------------------------------------------------------------------------------------------
STDMETHODIMP CEtsDivColl::Reload(void)
{
	try
	{
		CDBConnection	dbConnection;

		if(!dbConnection.IsOpened())
			dbConnection.Open(CGenegalSettings::GetDBConnectionString(), 10, 120, 300, 300);

		if (dbConnection.IsOpened())
		{
			Clear();
			CStoredProc<> db(dbConnection, L"usp_Dividends_Get");
			
			//push parameters
			db << m_nAssetID;

			CClientRecordset rs;
			rs.Open(db);

			DATE dtDividendDate;
			DOUBLE dDividendAmmount;
			for(rs.MoveFirst(); !rs.IsEOF(); ++rs) 
			{
				dtDividendDate = (vt_date)rs[L"DivYtes"];
				dDividendAmmount = rs[L"DivAmnt"];
				_CHK(AddNonUnique(dtDividendDate, dDividendAmmount), _T("Fail to Add Dividend payment date."));
			};

			dbConnection.Close();
		};
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
	return S_OK;
}
//------------------------------------------------------------------------------------------------------
