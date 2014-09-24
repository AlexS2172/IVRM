// MmGaSynthGreekAtom.h : Declaration of the CMmGaSynthGreekAtom
#ifndef __MMGASYNTHGREEKATOM_H__
#define __MMGASYNTHGREEKATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"

_COM_SMARTPTR_TYPEDEF(IMmGaSynthGreekAtom, IID_IMmGaSynthGreekAtom);

struct __MmGaSynthGreekAtom
{
	LONG					m_nUndID;
	CComBSTR				m_bstrUndSymbol;
	DOUBLE					m_dDeltaInShares;
	DOUBLE					m_dDeltaEq;
	DOUBLE					m_dGammaInShares;
	DOUBLE					m_dGammaEq;
	VARIANT_BOOL			m_bBadDeltaInShares;
	VARIANT_BOOL			m_bBadDeltaEq;
	VARIANT_BOOL			m_bBadGammaInShares;
	VARIANT_BOOL			m_bBadGammaEq;
	DOUBLE					m_dRhoInShares;
	VARIANT_BOOL			m_bBadRhoInShares;

	__MmGaSynthGreekAtom()
		: m_nUndID(0L),
		m_dDeltaInShares(BAD_DOUBLE_VALUE), m_dDeltaEq(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
		m_bBadDeltaInShares(VARIANT_FALSE), m_bBadDeltaEq(VARIANT_FALSE),
		m_bBadGammaInShares(VARIANT_FALSE), m_bBadGammaEq(VARIANT_FALSE),
		m_dRhoInShares(BAD_DOUBLE_VALUE),m_bBadRhoInShares(VARIANT_FALSE)
	{
	}

	void ClearValues()
	{
		m_dDeltaInShares = BAD_DOUBLE_VALUE;
		m_dDeltaEq = BAD_DOUBLE_VALUE;
		m_dGammaInShares = BAD_DOUBLE_VALUE;
		m_dGammaEq = BAD_DOUBLE_VALUE;

		m_bBadRhoInShares = VARIANT_FALSE ;
		m_bBadDeltaInShares = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaInShares = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;

		m_dRhoInShares = BAD_DOUBLE_VALUE;
	}
};

// CMmGaSynthGreekAtom
class ATL_NO_VTABLE CMmGaSynthGreekAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaSynthGreekAtom, &CLSID_MmGaSynthGreekAtom>,
	public ISupportErrorInfoImpl<&IID_IMmGaSynthGreekAtom>,
	public IDispatchImpl<IMmGaSynthGreekAtom, &IID_IMmGaSynthGreekAtom, &LIBID_EtsMmGreeksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmGaSynthGreekAtom
{
public:
	CMmGaSynthGreekAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGASYNTHGREEKATOM)


BEGIN_COM_MAP(CMmGaSynthGreekAtom)
	COM_INTERFACE_ENTRY(IMmGaSynthGreekAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, UndID, m_nUndID)
	IMPLEMENT_BSTR_PROPERTY(UndSymbol, m_bstrUndSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInShares, m_bBadDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaInShares, m_bBadGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares,m_dRhoInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRhoInShares, m_bBadRhoInShares)

	STDMETHODIMP ClearValues()
	{
		__MmGaSynthGreekAtom::ClearValues();
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaSynthGreekAtom), CMmGaSynthGreekAtom)

#endif //__MMGASYNTHGREEKATOM_H__