// MmQvExpAtom.cpp : Implementation of CMmQvExpAtom

#include "stdafx.h"
#include "MmQvExpAtom.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvExpAtom::get_NearAtmVola(DOUBLE* pVal)
{
	__CHECK_POINTER(pVal);

	*pVal = BAD_DOUBLE_VALUE;
	if(m_spAtmStrike != NULL)
	{
		ObjectLock lock(this);
		try
		{
			IMmQvOptPairCollPtr spOptPairs;
			_CHK(m_spAtmStrike->get_OptPair(&spOptPairs), _T("Fail to get ATM option pair collection."));

			IMmQvOptAtomPtr spOpt;
			IMmQvOptPairAtomPtr spOptPair;

			_variant_t varItem;
			ULONG nFetched = 0L;
			HRESULT hr = S_OK;

			IUnknownPtr spUnk;
			_CHK(spOptPairs->get__NewEnum(&spUnk), _T("Fail to get ATM option pair collection."));
			
			IEnumVARIANTPtr spEnum(spUnk);
			_CHK(spEnum->Reset(), _T("Fail to reset ATM option pair collection."));
			while((hr = spEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spOptPair = varItem;
				if(nFetched > 0 && spOptPair != NULL)
				{
					LONG nID = 0L;

					_CHK(spOptPair->get_Opt(enOtCall, &spOpt), _T("Fail to get ATM Option."));
					if(spOpt != NULL)
					{
						_CHK(spOpt->get_ID(&nID), _T("Fail to get ATM Option."));
					}

					if(nID == 0L)
					{
						_CHK(spOptPair->get_Opt(enOtPut, &spOpt), _T("Fail to get ATM Option."));
						if(spOpt != NULL)
						{
							_CHK(spOpt->get_ID(&nID), _T("Fail to get ATM Option."));
						}
					}

					if(nID != 0L)
					{
						_CHK(spOpt->get_Vola(pVal), _T("Fail to get ATM Vola."));
						break;
					}

					spOpt = NULL;
				}
				spOptPair = NULL;
				varItem.Clear();
			}
			_CHK(hr, _T("Fail to get next option pair."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvExpAtom, e.Error());
		}
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvExpAtom::FindAtmStrike(DOUBLE UnderlyingSpot)
{
	m_spAtmStrike = NULL;

	if(UnderlyingSpot > 0.)
	{
		ObjectLock lock(this);
		try
		{
			m_spAtmStrike = m_pStrike->FindATMStrike(UnderlyingSpot);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvExpAtom, e.Error());
		}
	}

	return S_OK;
}
