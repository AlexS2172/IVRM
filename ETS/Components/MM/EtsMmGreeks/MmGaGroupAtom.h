// MmGaGroupAtom.h : Declaration of the CMmGaGroupAtom
#ifndef __MMGAGROUPATOM_H__
#define __MMGAGROUPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaExpTotalColl.h"
#include "MmGaUndColl.h"

_COM_SMARTPTR_TYPEDEF(IMmGaGroupAtom, IID_IMmGaGroupAtom);

struct __MmGaGroupAtom
{
	LONG					m_nID;
	LONG					m_nType;
	CComBSTR				m_bstrName;
	IMmGaExpTotalCollPtr	m_spExpiryTotal;
	IMmGaUndCollPtr			m_spUnd;
	DOUBLE					m_dDeltaInShares;
	DOUBLE					m_dDeltaEq;
	DOUBLE					m_dGammaInShares;
	DOUBLE					m_dGammaEq;
	DOUBLE					m_dVegaInShares;
	DOUBLE					m_dTimeValueInShares;
	VARIANT_BOOL			m_bBadDeltaInShares;
	VARIANT_BOOL			m_bBadRhoInShares;
	VARIANT_BOOL			m_bBadDeltaEq;
	VARIANT_BOOL			m_bBadGammaInShares;
	VARIANT_BOOL			m_bBadGammaEq;
	VARIANT_BOOL			m_bBadVegaInShares;
	VARIANT_BOOL			m_bBadTimeValueInShares;
	DOUBLE					m_dRhoInShares;

	__MmGaGroupAtom()
		: m_nID(0L),
		m_dDeltaInShares(BAD_DOUBLE_VALUE), m_dDeltaEq(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
		m_dVegaInShares(BAD_DOUBLE_VALUE), m_dTimeValueInShares(BAD_DOUBLE_VALUE),
		m_bBadDeltaInShares(VARIANT_FALSE), m_bBadDeltaEq(VARIANT_FALSE),
		m_bBadGammaInShares(VARIANT_FALSE), m_bBadGammaEq(VARIANT_FALSE),
		m_bBadVegaInShares(VARIANT_FALSE), m_bBadTimeValueInShares(VARIANT_FALSE),
		m_dRhoInShares(BAD_DOUBLE_VALUE),m_bBadRhoInShares(VARIANT_FALSE)
	{
	}

	void ClearValues()
	{
		m_dDeltaInShares = BAD_DOUBLE_VALUE;
		m_dDeltaEq = BAD_DOUBLE_VALUE;
		m_dGammaInShares = BAD_DOUBLE_VALUE;
		m_dGammaEq = BAD_DOUBLE_VALUE;
		m_dVegaInShares = BAD_DOUBLE_VALUE;
		m_dTimeValueInShares = BAD_DOUBLE_VALUE;

		m_bBadDeltaInShares = VARIANT_FALSE;
		m_bBadRhoInShares = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaInShares = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadVegaInShares = VARIANT_FALSE;
		m_bBadTimeValueInShares = VARIANT_FALSE;

		m_dRhoInShares = BAD_DOUBLE_VALUE;
	}
};

// CMmGaGroupAtom
class ATL_NO_VTABLE CMmGaGroupAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaGroupAtom, &CLSID_MmGaGroupAtom>,
	public ISupportErrorInfoImpl<&IID_IMmGaGroupAtom>,
	public IDispatchImpl<IMmGaGroupAtom, &IID_IMmGaGroupAtom, &LIBID_EtsMmGreeksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmGaGroupAtom
{
public:
	CMmGaGroupAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAGROUPATOM)


BEGIN_COM_MAP(CMmGaGroupAtom)
	COM_INTERFACE_ENTRY(IMmGaGroupAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			{
				CComObject<CMmGaExpTotalColl>* pObject = NULL;
				_CHK(CComObject<CMmGaExpTotalColl>::CreateInstance(&pObject), _T("Fail to create expiries."));
				m_spExpiryTotal.Attach(pObject, TRUE);
			}
			{
				CComObject<CMmGaUndColl>* pObject = NULL;
				_CHK(CComObject<CMmGaUndColl>::CreateInstance(&pObject), _T("Fail to create underlyings."));
				m_spUnd.Attach(pObject, TRUE);
			}
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaGroupAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spExpiryTotal = NULL;
		m_spUnd = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Type, m_nType)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmGaExpTotalColl*, ExpiryTotal, m_spExpiryTotal)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmGaUndColl*, Und, m_spUnd)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaInShares, m_dVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValueInShares, m_dTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInShares, m_bBadDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaInShares, m_bBadGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaInShares, m_bBadVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValueInShares, m_bBadTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRhoInShares, m_bBadRhoInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares,m_dRhoInShares)

	STDMETHODIMP ClearValues()
	{
		__MmGaGroupAtom::ClearValues();
		return S_OK;
	}

	STDMETHOD(CalcUnderlying)(IMmGaUndAtom* aUnd, EtsCalcModelTypeEnum enCalcModel,
								VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
								VARIANT_BOOL bUseTheoVolaBadMarket, DOUBLE dUndPriceTolerance, 
								EtsPriceRoundingRuleEnum enPriceRoundingRule,
								ICalculationParametrs* pParams);

	STDMETHOD(CalcTotals)();
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaGroupAtom), CMmGaGroupAtom)

#endif //__MMGAGROUPATOM_H__