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

//////////////////////////////////////////////////////////////////////////////
DOUBLE	CMmQvExpAtom::GetStrikeVola(IMmQvStrikeAtomPtr& spStrike, DOUBLE dSpotPrice)
{
	DOUBLE	dVola = BAD_DOUBLE_VALUE;
	DOUBLE	dStrike = BAD_DOUBLE_VALUE;
	if (spStrike)
	{
		try
		{
			IMmQvOptPairCollPtr spOptPairs;
			_CHK(spStrike->get_OptPair(&spOptPairs), _T("Fail to get option pair collection."));

			_CHK(spStrike->get_Strike(&dStrike), _T("Fail to get Strike"));

			IMmQvOptAtomPtr spOpt;
			IMmQvOptPairAtomPtr spOptPair;

			_variant_t varItem;
			ULONG nFetched = 0L;
			HRESULT hr = S_OK;

			IUnknownPtr spUnk;
			_CHK(spOptPairs->get__NewEnum(&spUnk), _T("Fail to get option pair collection."));

			IEnumVARIANTPtr spEnum(spUnk);
			_CHK(spEnum->Reset(), _T("Fail to reset ATM option pair collection."));
			while((hr = spEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spOptPair = varItem;
				if(nFetched > 0 && spOptPair != NULL)
				{
					LONG nID = 0L;


					if (dStrike < dSpotPrice)
					{
						_CHK(spOptPair->get_Opt(enOtPut, &spOpt), _T("Fail to get Option."));
						//if(spOpt != NULL)_CHK(spOpt->get_ID(&nID), _T("Fail to get Option."));
					}
					else
					{
						_CHK(spOptPair->get_Opt(enOtCall, &spOpt), _T("Fail to get Option."));
						//if(spOpt != NULL)_CHK(spOpt->get_ID(&nID), _T("Fail to get Option."));
					}

					if(spOpt)
					{
						IMmQvQuoteAtomPtr	spDefQuote = NULL;
						_CHK(spOpt->get_DefQuote(&spDefQuote),_T("Fail to get def Quote"));
						if (spDefQuote != NULL){
							_CHK(spDefQuote->get_IV(&dVola), _T("Fail to get IV"));
							break;
						}
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
	return dVola;
}
//////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CMmQvExpAtom::get_Vola(DOUBLE	Strike, DOUBLE SpotPrice, DOUBLE *pVola)
{
	if (!pVola) return E_POINTER;

	if (Strike > 0.)
	{
		*pVola = 0.;
		IMmQvStrikeAtomPtr	spLowerStrike = NULL;
		IMmQvStrikeAtomPtr	spUpperStrike = NULL;

		ObjectLock lock(this);
		try
		{
			if (m_pStrike)
			{
				if(!m_pStrike->m_coll.empty())
				{
					//Need to find boundary strikes
					CMmQvStrikeColl::CollType::iterator itr = m_pStrike->m_coll.lower_bound(Strike);
					if(itr == m_pStrike->m_coll.end())
						spLowerStrike = m_pStrike->m_coll.rbegin()->second;
					else 
						if(itr == m_pStrike->m_coll.begin())
							spUpperStrike = itr->second;
						else
						{
							CMmQvStrikeColl::CollType::iterator greaterIt =   itr;
							CMmQvStrikeColl::CollType::iterator lowerIt   = --itr;
							spLowerStrike = lowerIt->second;
							spUpperStrike = greaterIt->second;
						}
					//next interpolate volatility between strikes
					if (spLowerStrike == NULL && spUpperStrike != NULL) //Left volatility used
							*pVola = GetStrikeVola(spUpperStrike, SpotPrice);
					else if (spUpperStrike == NULL && spLowerStrike != NULL) //Right volatility used
							*pVola = GetStrikeVola(spLowerStrike, SpotPrice);
					else if (spLowerStrike != NULL && spUpperStrike != NULL) //Need to interpolate volatility 
					{
						DOUBLE	dLeftVola	=	GetStrikeVola(spLowerStrike, SpotPrice);
						DOUBLE	dRightVola	= 	GetStrikeVola(spUpperStrike, SpotPrice);
						DOUBLE	dLeftStrike, dRightStrike;
						spLowerStrike->get_Strike(&dLeftStrike);
						spUpperStrike->get_Strike(&dRightStrike);

						*pVola = ((dRightVola - dLeftVola)/(dRightStrike - dLeftStrike))*(Strike - dLeftStrike) + dLeftVola;
					}
					return S_OK;
					
				}
			}
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvExpAtom, e.Error());
		}
	}
	return S_OK;
}
