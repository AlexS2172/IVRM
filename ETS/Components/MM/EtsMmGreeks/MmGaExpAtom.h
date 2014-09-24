// MmGaExpAtom.h : Declaration of the CMmGaExpAtom
#ifndef __MMGAEXPATOM_H__
#define __MMGAEXPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaPosColl.h"

_COM_SMARTPTR_TYPEDEF(IMmGaExpAtom, IID_IMmGaExpAtom);

struct __MmGaExpAtom
{
	LONG					m_nExpiryMonth;
	DATE					m_dtExpiry;
	DOUBLE					m_dRate;
	IMmGaPosCollPtr			m_spPos;
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
	VARIANT_BOOL			m_bHasSynth;
	IMmGaSynthGreekCollPtr	m_spSynthGreek;
	DOUBLE					m_dRhoInShares;
	DOUBLE					m_dHTBRate;

	__MmGaExpAtom()
		: m_nExpiryMonth(0L), m_dtExpiry(0.), m_dRate(0.),
		m_dDeltaInShares(BAD_DOUBLE_VALUE), m_dDeltaEq(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
		m_dVegaInShares(BAD_DOUBLE_VALUE), m_dTimeValueInShares(BAD_DOUBLE_VALUE),
		m_bBadDeltaInShares(VARIANT_FALSE), m_bBadDeltaEq(VARIANT_FALSE),
		m_bBadGammaInShares(VARIANT_FALSE), m_bBadGammaEq(VARIANT_FALSE),
		m_bBadVegaInShares(VARIANT_FALSE), m_bBadTimeValueInShares(VARIANT_FALSE),
		m_bHasSynth(VARIANT_FALSE), m_dHTBRate(BAD_DOUBLE_VALUE), m_dRhoInShares(BAD_DOUBLE_VALUE),
		m_bBadRhoInShares(VARIANT_FALSE)
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

		m_dHTBRate = BAD_DOUBLE_VALUE;

		m_bBadDeltaInShares = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaInShares = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadVegaInShares = VARIANT_FALSE;
		m_bBadTimeValueInShares = VARIANT_FALSE;
		m_dRhoInShares = BAD_DOUBLE_VALUE;
		m_bBadRhoInShares = VARIANT_FALSE;
	}
};

// CMmGaExpAtom
class ATL_NO_VTABLE CMmGaExpAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaExpAtom, &CLSID_MmGaExpAtom>,
	public ISupportErrorInfoImpl<&IID_IMmGaExpAtom>,
	public IDispatchImpl<IMmGaExpAtom, &IID_IMmGaExpAtom, &LIBID_EtsMmGreeksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmGaExpAtom
{
public:
	CMmGaExpAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAEXPATOM)


BEGIN_COM_MAP(CMmGaExpAtom)
	COM_INTERFACE_ENTRY(IMmGaExpAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CMmGaPosColl>* pObject = NULL;
			_CHK(CComObject<CMmGaPosColl>::CreateInstance(&pObject),  _T("Fail to create positions."));
			m_spPos.Attach(pObject, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaExpAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spPos = NULL;
		m_spSynthGreek = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpiryMonth, m_nExpiryMonth)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, HTBRate, m_dHTBRate)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmGaPosColl*, Pos, m_spPos)
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
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HasSynth, m_bHasSynth)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRhoInShares, m_bBadRhoInShares)
	IMPLEMENT_OBJECT_PROPERTY(IMmGaSynthGreekColl*, SynthGreek, m_spSynthGreek)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares,m_dRhoInShares)

	STDMETHODIMP ClearValues()
	{
		__MmGaExpAtom::ClearValues();
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaExpAtom), CMmGaExpAtom)

#endif //__MMGAEXPATOM_H__