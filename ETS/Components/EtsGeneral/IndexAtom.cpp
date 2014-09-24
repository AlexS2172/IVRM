// IndexAtom.cpp : Implementation of CIndexAtom

#include "stdafx.h"
#include "IndexAtom.h"
#include "UndColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CIndexAtom::get_BasketDivsCount(LONG* pVal)
{
	__CHECK_POINTER(pVal);

	ObjectLock lock(this);
	*pVal = (LONG)m_Divs.size();
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CIndexAtom::InitBasketDivs(IUndColl* pUndColl)
{
	ObjectLock lock(this);
	m_spDivs->Clear();
	//if(VARIANT_FALSE != m_bIsBasket)
	{
		if(pUndColl != NULL)
		{
			try
			{
				IUndCollPtr spUndColl(pUndColl);
				IUndAtomPtr spUnd = NULL;
				IIndexCompAtomPtr spIdxComp = NULL;
				_variant_t varItem;
				ULONG nFetched = 0L;
				LONG nID = 0L; 
				DOUBLE dWeight = 0.0; 
				EtsContractTypeEnum enUndType;
				EtsDivTypeEnum enDivType(enDivMarket);
				VARIANT_BOOL bValidDivs(VARIANT_FALSE);

				IEtsDivCollPtr spCustDivs = NULL;

				IEtsIndexDivAtomPtr spNewDividend = NULL;
				IEtsIndexDivAtomPtr spUndDividend = NULL;
				IEtsIndexDivAtomPtr spRetDividend = NULL;


				HRESULT hr = S_OK;

				IUnknownPtr spUnk = NULL;
				_CHK(m_spComponents->get__NewEnum(&spUnk), _T("Fail to get index components collection."));

				IEnumVARIANTPtr spEnum(spUnk);
				if (spEnum != NULL)
				{
					_CHK(spEnum->Reset(), _T("Fail to reset index components collection."));
					while((hr = spEnum->Next(1L, &varItem, &nFetched)) == S_OK)
					{
						ATLASSERT(varItem.vt == VT_DISPATCH);
						spIdxComp = varItem;
						if(nFetched > 0L && spIdxComp != NULL)
						{
							nID = 0L; 
							dWeight = 0.0; 
							_CHK(spIdxComp->get_Weight(&dWeight), _T("Fail to get index component weight."));
							_CHK(spIdxComp->get_ID(&nID), _T("Fail to get index component ID."));
							if(dWeight > 0.)
							{
								spUnd = NULL;
								_CHK(spUndColl->get_Item(nID, &spUnd), _T("Fail to get component underlying."));
								if(spUnd != NULL)
								{
									enUndType = enCtStock;
									_CHK(spUnd->get_UndType(&enUndType), _T("Fail to get component underlying type."));
									if(enUndType = enCtStock)
									{
										spNewDividend = NULL;
										spUndDividend = NULL;
										spRetDividend = NULL;

										spUnd->get_Dividend(&spUndDividend);
										if (spUndDividend != NULL)
										{
											LONG lIndex = 0;
											m_spDivs->get_Count(&lIndex);
											_CHK(m_spDivs->Add(lIndex, lIndex, spUndDividend, &spNewDividend),  _T("Fail add dividends to collection"));
											_CHK(m_spDivs->SetWeight(lIndex, dWeight));
										}
									}
									spUnd = NULL;
								}
							}
						}
						spIdxComp = NULL;
						varItem.Clear();
					}

				}
				_CHK(hr, _T("Fail to get next index component."));
			}
			catch(const _com_error& e)
			{
				m_spDivs->Clear();
				return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IIndexAtom, e.Error());
			}
		}
		else
		{
			return Error(L"Invalid underlying collection.", IID_IIndexAtom, E_INVALIDARG);
		}
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CIndexAtom::get_BasketDiv(LONG Index, EtsRegularDividend* pVal)
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
		return Error(L"Invalid basket dividend index.", IID_IIndexAtom, E_INVALIDARG);
	}
}
