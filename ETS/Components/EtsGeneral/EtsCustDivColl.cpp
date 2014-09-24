// EtsCustDivColl.cpp : Implementation of CEtsCustDivColl

#include "stdafx.h"

#include "EtsCustDivAtom.h"

#include "EtsCustDivColl.h"

#include "OptionCalc\OptionCalc.h"
#include "OptionPriceModels\common.h"

#include "EgStdLib\Trace.h"

using namespace EgLib;


// CEtsCustDivColl

STDMETHODIMP CEtsCustDivColl::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEtsCustDivColl
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CEtsCustDivColl::Add (DATE Key, IEtsCustDivAtom	*Value, IEtsCustDivAtom	**pRetVal)
{
	__CHECK_POINTER (pRetVal);

	try
	{
		if (m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Custom divident pair with the same key is already exists."));
		}

		if(!Value)
			*pRetVal = _AddNew (Key).Detach ();
		else
		{
			_CHK(IEtsCustDivCollImpl::Add (Key, Value), _T("Fail to add custom divident pair."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsCustDivColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CEtsCustDivColl::Clear ()
{
	IEtsCustDivCollImpl::Clear();
	m_lStockID = 0;
	m_enDivType = enMarketDiv;
	m_nDivFreqMarket = 0;
	m_dtDivDateMarket = 0;
	m_dDivAmtMarket = 0;
	m_nDivFreqCust = 0;
	m_dtDivDateCust = 0;
	m_dDivAmtCust = 0;
	return S_OK;
}

// Add or Edit Dividend Amount for Specified Date
STDMETHODIMP CEtsCustDivColl::SetDividend(double dDivDate, double dDivAmount)
{
	try
	{
		CComPtr <IEtsCustDivAtom> spCustDiv;

		CollType::iterator it = m_coll.find (dDivDate);
		if (it != m_coll.end())
			spCustDiv = (*it).second;
		else
			spCustDiv = _AddNew (dDivDate);
			
		_ASSERT (spCustDiv);
		if (spCustDiv)
		{
			spCustDiv->put_DivAmount (dDivAmount);
			spCustDiv->put_DivDate (dDivDate);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsCustDivColl, e.Error());
	}

	return S_OK;
}


STDMETHODIMP CEtsCustDivColl::GetDividend(double dDivAmount, double* pdRetVal)
{
	return E_NOTIMPL;
}

STDMETHODIMP CEtsCustDivColl::GetNextDevidentsCount (/*[in]*/LONG nToday, /*[in]*/LONG nExpiryDay, /*[out, retval]*/LONG* pnDivCount)
{
	if (pnDivCount == 0)
		return S_FALSE;
	*pnDivCount = 0;

	if (m_enDivType == enMarketDiv || m_enDivType == enCustomDFDiv)
	{
		*pnDivCount = _GetNextDevidentsCountFromFrequency (nToday, nExpiryDay);
		if ((long)(*pnDivCount) < 0)
			*pnDivCount = 0;

	}
	else if (m_enDivType == enTablFormDiv)
	{
		*pnDivCount = _GetNextDevidentsFromTabl (nToday, nExpiryDay);
		if ((long)(*pnDivCount) < 0)
			*pnDivCount = 0;
	}
	else
		return S_FALSE;


	return S_OK;
}

STDMETHODIMP CEtsCustDivColl::GetNextDevidentsAsArray (/*[in]*/LONG nToday, /*[in]*/LONG nExpiryDay, /*[in]*/LONG nCount, /*[in,out]*/DATE* pdDivDte, /*[in,out]*/DOUBLE* pdDivAmt, /*[out,retval]*/LONG* pnCount)
{
	if (pdDivDte == NULL)
		return S_FALSE;

	if (pdDivAmt == NULL)
		return S_FALSE;

	if (pnCount)
		*pnCount = 0; 

	LONG lCount = 0;
	if (m_enDivType == enMarketDiv || m_enDivType == enCustomDFDiv)
		lCount = _GetNextDevidentsFromFrequency (nToday, nExpiryDay, nCount, pdDivDte, pdDivAmt);
	else
		lCount = _GetNextDevidentsFromTabl (nToday, nExpiryDay, nCount, pdDivDte, pdDivAmt);
	
	if (lCount < 0)
		lCount = 0;

	if (pnCount)
		*pnCount = lCount; 
	

	return S_OK;
}

STDMETHODIMP CEtsCustDivColl::DeleteDividen(double dDivDate)
{
	//m_coll.erase(dDivDate)
	return E_NOTIMPL;
}


#define _CREATE_COM_OBJECT(spObject, TypeOfObject, strErr) \
	{\
		CComObject <TypeOfObject> *pObj;\
		_CHK (CComObject<CEtsCustDivAtom>::CreateInstance (&pObj), strErr);\
		(spObject) = pObj;\
	}

CComPtr<IEtsCustDivAtom> CEtsCustDivColl::_AddNew (DATE Key)
{
	CComPtr<IEtsCustDivAtom> spCustDiv;
	_CREATE_COM_OBJECT(spCustDiv, CEtsCustDivAtom, _T("Fail to add custom divident pair."));
	if(FAILED (IEtsCustDivCollImpl::Add (Key, spCustDiv)))
		CComErrorWrapper::ThrowError (E_INVALIDARG, _T("Fail to add custom divident pair."));
	
	return spCustDiv;
}

LONG CEtsCustDivColl::_GetNextDevidentsCountFromFrequency (LONG nToday, LONG nExpiryDay)
{
	if (nExpiryDay < nToday)
		return 0;

	if (!_IsValidDivs (m_enDivType != enMarketDiv))
		return 0;

	return ::GetDividendsCount (nToday, nExpiryDay - nToday, (LONG)_DivDateRef (), NULL);
}

LONG CEtsCustDivColl::_GetNextDevidentsFromFrequency (LONG nToday, LONG nExpiryDay, LONG nCount, DATE* pdDivDte, DOUBLE* pdDivAmt)
{
	LONG lResult;
	if (GetDividends2(nToday, nExpiryDay - nToday,	
			(LONG)_DivDateRef (), _DivFreqRef (), _DivAmtRef (), 
								nCount, pdDivAmt, pdDivDte, &lResult))
		return lResult;
	else
		return 0;
}

LONG CEtsCustDivColl::_GetNextDevidentsFromTabl (LONG nToday, LONG nExpiryDay, LONG nCount, DATE* pdDivDte, DOUBLE* pdDivAmt)
{
	if (nExpiryDay < nToday)
		return 0;

	CollType::const_iterator citBeg = m_coll.upper_bound (nToday);

	if (citBeg == m_coll.end())
		return 0;

	CollType::const_iterator citEnd = m_coll.upper_bound (nExpiryDay);

	if (citBeg == citEnd)
		return 0;

	int iCount = 0;
	for (CollType::const_iterator cit = citBeg; cit != citEnd; cit++)
	{
		if (iCount >= nCount && nCount > 0)
			break;

		if (pdDivAmt)
			(*cit).second->get_DivAmount (pdDivAmt + iCount);

		if (pdDivDte)
		{
			double dDivDate = 0;
			(*cit).second->get_DivDate (&dDivDate);
			*(pdDivDte + iCount) = (dDivDate - nToday) / OPM::cdDaysPerYear365;
		}

		iCount++;
	}
	
	return iCount;	
}

STDMETHODIMP  CEtsCustDivColl::IsValidDivs(VARIANT_BOOL CheckCustom, VARIANT_BOOL* pVal)
{
	__CHECK_POINTER(pVal);
	ObjectLock lock(this);

	*pVal = (VARIANT_BOOL)_IsValidDivs (CheckCustom == VARIANT_TRUE);

	return S_OK;
}

bool CEtsCustDivColl::_IsValidDivs (bool bCheckCustom)
{
	if (bCheckCustom)
	{
		return (m_nDivFreqCust == 1L || m_nDivFreqCust == 2L || m_nDivFreqCust == 4L || m_nDivFreqCust == 12L)
			&& m_dDivAmtCust > 0. && m_dtDivDateCust > 0.;
	}
	else
	{
		return ((m_nDivFreqMarket == 1L || m_nDivFreqMarket == 2L || m_nDivFreqMarket == 4L || m_nDivFreqMarket == 12L)
			&& m_dDivAmtMarket > 0. && m_dtDivDateMarket > 0.);
	}
}

STDMETHODIMP  CEtsCustDivColl::CorrectDivs ()
{
	if (m_nDivFreqCust != 1L && m_nDivFreqCust != 2L && m_nDivFreqCust != 4L && m_nDivFreqCust != 12L)
		m_nDivFreqCust = 0;

	if (m_nDivFreqMarket != 1L && m_nDivFreqMarket != 2L && m_nDivFreqCust != 4L && m_nDivFreqCust != 12L)
		m_nDivFreqCust = 0;

	return S_OK;
}



/*
LONG _GetDevidents (DATE dToday, DATE dDivDate, DATE dExpiry,  DOUBLE* pdDivDte, DOUBLE* pdDivAmt, DATE* pFirstDivDate)
{
	static const COleDateTime t0 (1900, 1, 1, 0, 0, 0);

	LONG nCount = 0;
	COleDateTime dtToday ((DATE)nToday), dtDivDate (nDTE), dtExpiry (nLastDivDate);

	// Count dividends
	while( true )
	{
		//if dividends date more then today
		if (dtToday < dtDivDate && nCount==0)
		{	
			nCount++;
			if( pFirstDivDate != NULL && nCount == 1 )
				*pFirstDivDate = pdDivDte;

			continue;
		}

		dtDivDate.SetDate (dtDivDate.GetYear(), dtDivDate.GetMonth() + WORD(12/nFrequency), dtDivDate.GetDay());
		
		if( dtDivDate < dtToday )
			continue;
		else if( dtDivDate < dtExpiry )
		{
			nCount++;
			if( pFirstDivDate != NULL && nCount == 1 )
				*pFirstDivDate = dtDivDate;
		}
		else
			break;
	}

	return nCount;
}
*/
/*
get_Count (pnDivCount);

if (*pnDivCount == 0)
return S_FALSE;

*ppdDivDte = new DOUBLE [*pnDivCount];
ATLASSERT(*ppdDivDte);
*ppdDivAmt = new DOUBLE [*pnDivCount];
ATLASSERT(*ppdDivAmt);

int i = 0;
for (CollType::const_iterator cit =  m_coll.begin (); cit != m_coll.end (); cit++, i++)
{
(*cit).second->get_DivAmount ((*ppdDivAmt)+ i);
(*cit).second->get_DivDate ((*ppdDivDte)+ i);
}

return S_OK;
*/

