// MmGaExpTotalAtom.h : Declaration of the CMmGaExpTotalAtom
#ifndef __MMGAEXPTOTALATOM_H__
#define __MMGAEXPTOTALATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaExpColl.h"

_COM_SMARTPTR_TYPEDEF(IMmGaExpTotalAtom, IID_IMmGaExpTotalAtom);

struct __MmGaExpTotalAtom
{
	LONG					m_nExpiryMonth;
	IMmGaExpCollPtr			m_spExpiry;
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

	__MmGaExpTotalAtom()
		: m_nExpiryMonth(0L),
		m_dDeltaInShares(BAD_DOUBLE_VALUE), m_dDeltaEq(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
		m_dVegaInShares(BAD_DOUBLE_VALUE), m_dTimeValueInShares(BAD_DOUBLE_VALUE),
		m_bBadDeltaInShares(VARIANT_FALSE), m_bBadDeltaEq(VARIANT_FALSE),
		m_bBadGammaInShares(VARIANT_FALSE), m_bBadGammaEq(VARIANT_FALSE),
		m_bBadVegaInShares(VARIANT_FALSE), m_bBadTimeValueInShares(VARIANT_FALSE),
		m_dRhoInShares(BAD_DOUBLE_VALUE), m_bBadRhoInShares(VARIANT_FALSE)
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
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaInShares = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadVegaInShares = VARIANT_FALSE;
		m_bBadTimeValueInShares = VARIANT_FALSE;
		m_bBadRhoInShares = VARIANT_FALSE ;

		m_dRhoInShares = BAD_DOUBLE_VALUE;
	}
};

// CMmGaExpTotalAtom
class ATL_NO_VTABLE CMmGaExpTotalAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaExpTotalAtom, &CLSID_MmGaExpTotalAtom>,
	public ISupportErrorInfoImpl<&IID_IMmGaExpTotalAtom>,
	public IDispatchImpl<IMmGaExpTotalAtom, &IID_IMmGaExpTotalAtom, &LIBID_EtsMmGreeksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmGaExpTotalAtom
{
public:
	CMmGaExpTotalAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAEXPTOTALATOM)


BEGIN_COM_MAP(CMmGaExpTotalAtom)
	COM_INTERFACE_ENTRY(IMmGaExpTotalAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CMmGaExpColl>* pObject = NULL;
			_CHK(CComObject<CMmGaExpColl>::CreateInstance(&pObject),  _T("Fail to create expiries."));
			m_spExpiry.Attach(pObject, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaExpTotalAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spExpiry = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpiryMonth, m_nExpiryMonth)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmGaExpColl*, Expiry, m_spExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaInShares, m_dVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValueInShares, m_dTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInShares, m_bBadDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRhoInShares, m_bBadRhoInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaInShares, m_bBadGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaInShares, m_bBadVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValueInShares, m_bBadTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares,m_dRhoInShares)

	STDMETHODIMP ClearValues()
	{
		__MmGaExpTotalAtom::ClearValues();
		return S_OK;
	}

	STDMETHOD(CalcTotals)();
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaExpTotalAtom), CMmGaExpTotalAtom)

#endif //__MMGAEXPTOTALATOM_H__