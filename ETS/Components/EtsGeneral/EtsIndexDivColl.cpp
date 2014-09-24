// EtsIndexDivColl.cpp : Implementation of CEtsIndexDivColl

#include "stdafx.h"
#include "EtsIndexDivAtom.h"
#include "EtsIndexDivColl.h"
#include "Atlsafe.h"


// CEtsIndexDivColl

STDMETHODIMP CEtsIndexDivColl::Add(LONG Key, LONG SortKey, IEtsIndexDivAtom* Value, IEtsIndexDivAtom** pRetVal)
{
	try
	{
		//m_bRequiresSave = true;
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Index dividend atom with the same key is already exists."));
		}

		if(m_collWeights.find(Key) == m_collWeights.end())
			m_collWeights[Key] = 1.0;

		if(!Value)
		{
			CComObject<CEtsIndexDivAtom>* pNewVal;

			_CHK(CComObject<CEtsIndexDivAtom>::CreateInstance(&pNewVal), _T("Fail to add index dividend."));
			pNewVal->AddRef();
			if(FAILED(IEtsIndexDivCollImpl::Add(Key, SortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add index dividend."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsIndexDivCollImpl::Add(Key, SortKey, Value), _T("Fail to add index dividend."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CEtsIndexDivColl::CopyToWithWeight( DOUBLE dWeight, IEtsIndexDivColl* pDest , IEtsIndexDivColl** ppVal)
{
	HRESULT hr = S_OK;
	__CHECK_POINTER(ppVal);
	__CHECK_POINTER(pDest);

	try
	{
		EnumIterType iter = m_coll.begin();
		EnumIterType iterEnd = m_coll.end();
		IEtsIndexDivAtomPtr spRetVal;
		
		IEtsIndexDivAtomPtr spTmpAtom;
		IEtsIndexDivAtomPtr spNewAtom;
		for (;iter!= iterEnd; ++iter)
		{
			spNewAtom = NULL;
			spRetVal = NULL;
			spTmpAtom =  iter->second;

			if (spTmpAtom != NULL)
			{
				pDest->Add(iter->first, iter->first, spTmpAtom, &spRetVal);
				pDest->SetWeight(iter->first, dWeight);
			}
		}
		if(ppVal)
		{
			*ppVal = pDest;
			if(pDest)
				pDest->AddRef();
		}
	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivColl, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at CopyToWithWeight"), IID_IEtsIndexDivColl, E_FAIL);
	}
	return hr;

}


STDMETHODIMP CEtsIndexDivColl::SetWeight(LONG nKey, DOUBLE dWeight)
{
	HRESULT hr = S_OK;
	try
	{
		m_collWeights[nKey] = dWeight;
	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivAtom, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at GetDividends"), IID_IEtsIndexDivAtom, E_FAIL);
	}
	return hr;
}

STDMETHODIMP CEtsIndexDivColl::GetDividends( DATE nToday,  DATE nExpiry,  LONG nCount, SAFEARRAY ** psaDivAmounts,  SAFEARRAY ** psaDivDates,  LONG* pnCount)
{

	__CHECK_POINTER(psaDivAmounts);
	__CHECK_POINTER(psaDivDates);
	__CHECK_POINTER(pnCount);
	HRESULT hr = S_OK;
	try
	{
		LONG nDivCount = 0;
		IEtsDivCollPtr spTmpDivColl = GetCachedDividends(nToday, nExpiry);

		spTmpDivColl->GetDividendCount(nToday, nExpiry, &nDivCount);
		if ((nCount == 0)||(nDivCount <= nCount))
			spTmpDivColl->GetDividends(nToday, nExpiry, nDivCount, psaDivAmounts, psaDivDates, pnCount);
		else
			hr = E_FAIL;
	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivAtom, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at GetDividends"), IID_IEtsIndexDivAtom, E_FAIL);
	}
	return hr;
}


STDMETHODIMP CEtsIndexDivColl::GetNearest( DATE nToday,  DATE nExpiry,  DOUBLE* pdDivAmount,  DOUBLE* pdDivDate)
{
	HRESULT hr = S_OK;
	try
	{
		IEtsDivCollPtr spTmpDivColl = GetCachedDividends(nToday, nExpiry);
		if(spTmpDivColl!=NULL)
			spTmpDivColl->GetNearest(nToday, nExpiry, pdDivAmount, pdDivDate);

	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivAtom, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at GetNearest"), IID_IEtsIndexDivAtom, E_FAIL);
	}
	return hr;


	return E_NOTIMPL;
}


IEtsDivCollPtr CEtsIndexDivColl::GetCachedDividends(DATE nToday, DATE nExpiry)
{
	if(m_spCustomDivsCache == NULL || nToday != m_nTodayCache || nExpiry != m_nExpiryCache)
	{
		m_spCustomDivsCache = NULL;
		m_nTodayCache = 0;
		m_nTodayCache = 0;

		IEtsIndexDivAtomPtr spDivAtom;
		EnumIterType iter = m_coll.begin();
		EnumIterType iterEnd = m_coll.end();

		LONG lRawTotal = 0;
		LONG lCurrent = 0;
		LONG lTotal = 0;
		DOUBLE dDivAmount = 0;
		DOUBLE dDivDate = 0;
		LONG   lDivFreq = 0;

		IEtsDivCollPtr spTmpDivColl;

		IEtsDivCollPtr spCustomDivs;

		CComObject<CEtsDivColl>* pTmpDivColl; 
		_CHK(CComObject<CEtsDivColl>::CreateInstance(&pTmpDivColl),  _T("Fail to create temp dividend coll object."));
		spTmpDivColl.Attach(pTmpDivColl, TRUE);

		for (; iter != iterEnd; ++iter)
		{
			LONG nDivCount = 0;
			LONG nRetCount = 0;

			spDivAtom = iter->second;
			if (spDivAtom != NULL)
			{

				spDivAtom->GetDividendCount(nToday, nExpiry, &nDivCount);

				if(nDivCount > 0L) 
				{
					CSafeArrayWrapper<double> saTmpDates;
					CSafeArrayWrapper<double> saTmpAmounts;
					LPSAFEARRAY psaTmpDates = NULL;
					LPSAFEARRAY psaTmpAmounts = NULL;

					spDivAtom->GetDividends(nToday, nExpiry, nDivCount, &psaTmpAmounts, &psaTmpDates, &nRetCount);
					saTmpDates.Attach(psaTmpDates);
					saTmpAmounts.Attach(psaTmpAmounts);

					for (long i =0;  i<nDivCount; i++)
						pTmpDivColl->AddNonUnique(floor(double(saTmpDates.GetPlainData()[i]*365.0)+nToday), saTmpAmounts.GetPlainData()[i]*m_collWeights[iter->first]);
				}
			}
		}
		m_spCustomDivsCache = pTmpDivColl;
		m_nTodayCache  = nToday;
		m_nExpiryCache = nExpiry;
	}
	return m_spCustomDivsCache;
}

STDMETHODIMP CEtsIndexDivColl::GetDividendCount( DATE nToday,  DATE nExpiry,  LONG* pnCount)
{
	__CHECK_POINTER(pnCount);
	HRESULT hr = S_OK;
	try
	{
		*pnCount = 0;

		IEtsDivCollPtr spTmpDivColl = GetCachedDividends(nToday, nExpiry);
		if(spTmpDivColl!=NULL)
			spTmpDivColl->GetDividendCount(nToday, nExpiry, pnCount);

	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivAtom, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at GetDividends"), IID_IEtsIndexDivAtom, E_FAIL);
	}
	return hr;

}



//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CEtsIndexDivColl::GetDividends2(DATE dtNow,  DATE dtExpiryOV, DATE tmCloseTime, LONG nCount, SAFEARRAY ** psaDivAmounts, SAFEARRAY ** psaDivDates,  LONG* pnCount)
{
	__CHECK_POINTER(psaDivAmounts);
	__CHECK_POINTER(psaDivDates);
	__CHECK_POINTER(pnCount);
	HRESULT hr = S_OK;
	try
	{
		LONG nDivCount = 0;
		IEtsDivCollPtr spTmpDivColl = GetCachedDividends2(dtNow, dtExpiryOV, tmCloseTime);

		spTmpDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);
		if ((nCount == 0)||(nDivCount <= nCount))
			spTmpDivColl->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, psaDivAmounts, psaDivDates, pnCount);
		else
			hr = E_FAIL;
	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivAtom, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at GetDividends2"), IID_IEtsIndexDivAtom, E_FAIL);
	}
	return hr;
}
STDMETHODIMP CEtsIndexDivColl::GetNearest2(DATE dtNow,  DATE dtExpiryOV, DATE tmCloseTime,  DOUBLE* pdDivAmount,  DOUBLE* pdDivDate)
{
	HRESULT hr = S_OK;
	try
	{
		IEtsDivCollPtr spTmpDivColl = GetCachedDividends2(dtNow, dtExpiryOV, tmCloseTime);
		if(spTmpDivColl!=NULL)
			spTmpDivColl->GetNearest2(dtNow, dtExpiryOV, tmCloseTime, pdDivAmount, pdDivDate);

	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivAtom, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at GetNearest2"), IID_IEtsIndexDivAtom, E_FAIL);
	}
	return hr;

}
STDMETHODIMP CEtsIndexDivColl::GetDividendCount2(DATE dtNow, DATE dtExpiryOV, DATE tmCloseTime,  LONG* pnCount)
{
	__CHECK_POINTER(pnCount);
	HRESULT hr = S_OK;
	try
	{
		*pnCount = 0;

		IEtsDivCollPtr spTmpDivColl = GetCachedDividends2(dtNow, dtExpiryOV, tmCloseTime);
		if(spTmpDivColl!=NULL)
			spTmpDivColl->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, pnCount);

	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsIndexDivAtom, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhanded exception handled at GetDividends2"), IID_IEtsIndexDivAtom, E_FAIL);
	}
	return hr;
}

IEtsDivCollPtr CEtsIndexDivColl::GetCachedDividends2(DATE dtNow, DATE dtExpiryOV, DATE tmCloseTime)
{
	if(m_spCustomDivsCache == NULL || fabs(dtNow - m_nTodayCache) > OneMinute  || fabs(dtExpiryOV - m_nExpiryCache) > OneMinute )
	{
		m_spCustomDivsCache = NULL;
		m_nTodayCache = 0;
		m_nTodayCache = 0;

		IEtsIndexDivAtomPtr spDivAtom;
		EnumIterType iter = m_coll.begin();
		EnumIterType iterEnd = m_coll.end();

		LONG lRawTotal = 0;
		LONG lCurrent = 0;
		LONG lTotal = 0;
		DOUBLE dDivAmount = 0;
		DOUBLE dDivDate = 0;
		LONG   lDivFreq = 0;

		IEtsDivCollPtr spTmpDivColl;

		IEtsDivCollPtr spCustomDivs;

		CComObject<CEtsDivColl>* pTmpDivColl; 
		_CHK(CComObject<CEtsDivColl>::CreateInstance(&pTmpDivColl),  _T("Fail to create temp dividend coll object."));
		spTmpDivColl.Attach(pTmpDivColl, TRUE);

		for (; iter != iterEnd; ++iter)
		{
			LONG nDivCount = 0;
			LONG nRetCount = 0;

			spDivAtom = iter->second;
			if (spDivAtom != NULL)
			{

				spDivAtom->GetDividendCount2(dtNow, dtExpiryOV, tmCloseTime, &nDivCount);

				if(nDivCount > 0L) 
				{
					CSafeArrayWrapper<double> saTmpDates;
					CSafeArrayWrapper<double> saTmpAmounts;
					LPSAFEARRAY psaTmpDates = NULL;
					LPSAFEARRAY psaTmpAmounts = NULL;

					spDivAtom->GetDividends2(dtNow, dtExpiryOV, tmCloseTime, nDivCount, &psaTmpAmounts, &psaTmpDates, &nRetCount);
					saTmpDates.Attach(psaTmpDates);
					saTmpAmounts.Attach(psaTmpAmounts);

					for (long i =0;  i<nDivCount; i++)
						pTmpDivColl->AddNonUnique(floor(double(saTmpDates.GetPlainData()[i]*365.0)+dtNow), saTmpAmounts.GetPlainData()[i]*m_collWeights[iter->first]);
				}
			}
		}
		m_spCustomDivsCache = pTmpDivColl;
		m_nTodayCache  = dtNow;
		m_nExpiryCache = dtExpiryOV;
	}
	return m_spCustomDivsCache;
}
//------------------------------------------------------------------------------------------------------
STDMETHODIMP CEtsIndexDivColl::Reload(void)
{
	IEtsIndexDivAtomPtr spDivAtom = NULL;
	EnumIterType iter = m_coll.begin();
	EnumIterType iterEnd = m_coll.end();

	for (; iter != iterEnd; ++iter)
	{
		spDivAtom = iter->second;
		if (static_cast<bool>(spDivAtom))
		{
			spDivAtom->Reload();
		}
	}
	return S_OK;
}
//------------------------------------------------------------------------------------------------------