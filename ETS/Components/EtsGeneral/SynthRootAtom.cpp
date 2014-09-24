// SynthRootAtom.cpp : Implementation of CSynthRootAtom

#include "stdafx.h"
#include "SynthRootAtom.h"
#include "UndColl.h"
#include "IndexAtom.h"


// CSynthRootAtom


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CSynthRootAtom::get_BasketDivsCount(LONG* pVal)
{
	__CHECK_POINTER(pVal);

	ObjectLock lock(this);
	*pVal = (LONG)m_Divs.size();
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CSynthRootAtom::get_BasketDiv(LONG Index, EtsRegularDividend* pVal)
{
	__CHECK_POINTER(pVal);

	ObjectLock lock(this);
	if(Index >= 0L && Index < (LONG)m_Divs.size())
	{
		*pVal = m_Divs[Index];
		return S_OK;
	}
	else
	{
		return Error(L"Invalid synthetic root basket dividend index.", IID_ISynthRootAtom, E_INVALIDARG);
	}
}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CSynthRootAtom::InitBasketDivs(IUndColl* pUndColl, IIndexColl* pIndexColl)
{
	ObjectLock lock(this);
	m_spDivs->Clear();
	if(VARIANT_FALSE != m_bBasket)
	{
		if(pUndColl)
		{
			try
			{
				IUndCollPtr spUndColl(pUndColl);
				IUndAtomPtr spUnd;
				ISynthRootCompAtomPtr spSRComp;
				_variant_t varItem;
				ULONG nFetched = 0L;
				LONG nID = 0L; DOUBLE dWeight = 0.; EtsContractTypeEnum enUndType;
				EtsDivTypeEnum enDivType(enDivMarket);
				VARIANT_BOOL bValidDivs(VARIANT_FALSE);
				DOUBLE	dDivDate = 0.0;
				DOUBLE	dDivAmount = 0.0 ;
				LONG	lDivFreq = 0;

				HRESULT hr = S_OK;

				IUnknownPtr spUnk;
				_CHK(m_spSynthRootComponents->get__NewEnum(&spUnk), _T("Fail to get synthetic root components collection."));

				IEnumVARIANTPtr spEnum(spUnk);
				_CHK(spEnum->Reset(), _T("Fail to reset synthetic root components collection."));
				while((hr = spEnum->Next(1L, &varItem, &nFetched)) == S_OK)
				{
					ATLASSERT(varItem.vt == VT_DISPATCH);
					spSRComp = varItem;
					if(nFetched > 0L && spSRComp != NULL)
					{
						_CHK(spSRComp->get_Weight(&dWeight), _T("Fail to get synthetic root component weight."));
						_CHK(spSRComp->get_UndID(&nID), _T("Fail to get synthetic root component ID."));
						if(dWeight > 0.)
						{
							_CHK(spUndColl->get_Item(nID, &spUnd), _T("Fail to get underlying."));
							if(spUnd != NULL)
							{
								_CHK(spUnd->get_UndType(&enUndType), _T("Fail to get underlying type."));
								if(enCtStock == enUndType)
								{
									IEtsIndexDivAtomPtr spNewDividend;
									IEtsIndexDivAtomPtr spUndDividend;
									IEtsIndexDivAtomPtr spRetDividend;

									spUnd->get_Dividend(&spUndDividend);
									if (spUndDividend != NULL)
									{
										LONG lIndex = 0;
										m_spDivs->get_Count(&lIndex);
										_CHK(m_spDivs->Add(lIndex, lIndex, spUndDividend, &spNewDividend), _T("Fail add dividends to collection"));
										_CHK(m_spDivs->SetWeight(lIndex, dWeight));
									}

								}
								else if(enCtIndex == enUndType )
								{
									IIndexAtomPtr spBasketIndex;
									LONG nBaskDivCount = 0L; VARIANT_BOOL bIsBasket = VARIANT_FALSE;

									_CHK(pIndexColl->get_Item(nID, &spBasketIndex), _T("Fail to get basket index object."));									

									if(spBasketIndex != NULL)
									{
										IEtsIndexDivCollPtr spBasketIndexDiv = NULL;
										IEtsIndexDivCollPtr spRetDiv = NULL;
										spBasketIndex->get_IsBasket(&bIsBasket);
										spBasketIndex->get_BasketDivs(&spBasketIndexDiv);
										if (spBasketIndexDiv != NULL)
											spBasketIndexDiv->CopyToWithWeight(dWeight, m_spDivs, &spRetDiv );
									}
								}
								spUnd = NULL;
							}
						}
					}
					spSRComp = NULL;
					varItem.Clear();
				}
				_CHK(hr, _T("Fail to get next synthetic root component."));
			}
			catch(const _com_error& e)
			{
				m_spDivs->Clear();
				return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_ISynthRootAtom, e.Error());
			}
		}
		else
		{
			return Error(L"Invalid underlying collection.", IID_ISynthRootAtom, E_INVALIDARG);
		}
	}
	return S_OK;
}
