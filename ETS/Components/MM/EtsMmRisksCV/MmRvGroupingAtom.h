// MmRvGroupingAtom.h : Declaration of the CMmRvGroupingAtom
#ifndef __MMRVGROUPINGATOM_H__
#define __MMRVGROUPINGATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvPosColl.h"

#include "../cv/cvCV.h"

_COM_SMARTPTR_TYPEDEF(IMmRvGroupingAtom, IID_IMmRvGroupingAtom);

struct __MmRvGroupingAtom
{
	LONG					m_nExpiryMonth;
	DATE					m_dtExpiry;
	DOUBLE					m_dRate;
	IMmRvPosCollPtr			m_spPos;
	DOUBLE					m_dDeltaInShares;
	DOUBLE					m_dDeltaEq;
	DOUBLE					m_dNetDeltaInShares;
	DOUBLE					m_dNetDeltaEq;
	DOUBLE					m_dGammaInShares;
	DOUBLE					m_dGammaEq;
	DOUBLE					m_dVegaInShares;
	DOUBLE					m_dTimeValueInShares;
	VARIANT_BOOL			m_bBadDeltaInShares;
	VARIANT_BOOL			m_bBadDeltaEq;
	VARIANT_BOOL			m_bBadNetDeltaInShares;
	VARIANT_BOOL			m_bBadNetDeltaEq;
	VARIANT_BOOL			m_bBadGammaInShares;
	VARIANT_BOOL			m_bBadGammaEq;
	VARIANT_BOOL			m_bBadVegaInShares;
	VARIANT_BOOL			m_bBadTimeValueInShares;
	VARIANT_BOOL			m_bHasSynth;
	IMmRvSynthGreeksCollPtr	m_spSynthGreek;
	DOUBLE					m_dHTBRate;
	LONG					m_nRoundMoneyness;

	// custom variables
	LONG					m_nPoolID;
	IcvRTContextPtr			m_spRTContext;

	__MmRvGroupingAtom()
		: m_nExpiryMonth(0L), m_dtExpiry(0.), m_dRate(0.),
		m_dDeltaInShares(BAD_DOUBLE_VALUE), m_dDeltaEq(BAD_DOUBLE_VALUE),
		m_dNetDeltaInShares(BAD_DOUBLE_VALUE), m_dNetDeltaEq(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
		m_dVegaInShares(BAD_DOUBLE_VALUE), m_dTimeValueInShares(BAD_DOUBLE_VALUE),
		m_bBadDeltaInShares(VARIANT_FALSE), m_bBadDeltaEq(VARIANT_FALSE),
		m_bBadNetDeltaInShares(VARIANT_FALSE), m_bBadNetDeltaEq(VARIANT_FALSE),
		m_bBadGammaInShares(VARIANT_FALSE), m_bBadGammaEq(VARIANT_FALSE),
		m_bBadVegaInShares(VARIANT_FALSE), m_bBadTimeValueInShares(VARIANT_FALSE),
		m_bHasSynth(VARIANT_FALSE), m_dHTBRate(BAD_DOUBLE_VALUE), m_nRoundMoneyness(BAD_LONG_VALUE),
		m_nPoolID(BAD_LONG_VALUE)
	{
	}

	void ClearValues()
	{
		m_dDeltaInShares = BAD_DOUBLE_VALUE;
		m_dDeltaEq = BAD_DOUBLE_VALUE;
		m_dNetDeltaInShares = BAD_DOUBLE_VALUE;
		m_dNetDeltaEq = BAD_DOUBLE_VALUE;
		m_dGammaInShares = BAD_DOUBLE_VALUE;
		m_dGammaEq = BAD_DOUBLE_VALUE;
		m_dVegaInShares = BAD_DOUBLE_VALUE;
		m_dTimeValueInShares = BAD_DOUBLE_VALUE;

		m_dHTBRate = BAD_DOUBLE_VALUE;

		m_bBadDeltaInShares = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadNetDeltaInShares = VARIANT_FALSE;
		m_bBadNetDeltaEq = VARIANT_FALSE;
		m_bBadGammaInShares = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadVegaInShares = VARIANT_FALSE;
		m_bBadTimeValueInShares = VARIANT_FALSE;
	}
};

// CMmRvGroupingAtom
class ATL_NO_VTABLE CMmRvGroupingAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvGroupingAtom, &CLSID_MmRvGroupingAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvGroupingAtom>,
	public IDispatchImpl<IMmRvGroupingAtom, &IID_IMmRvGroupingAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IcvDataProvider, &IID_IMmRvUndAtom, &__uuidof(__CVLib), /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvGroupingAtom
{
public:
	CMmRvGroupingAtom()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVGROUPINGATOM)


	BEGIN_COM_MAP(CMmRvGroupingAtom)
		COM_INTERFACE_ENTRY(IMmRvGroupingAtom)
		COM_INTERFACE_ENTRY(IcvDataProvider)
		COM_INTERFACE_ENTRY2(IDispatch,IMmRvGroupingAtom)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CMmRvPosColl>* pObject = NULL;
			_CHK(CComObject<CMmRvPosColl>::CreateInstance(&pObject),  _T("Fail to create positions."));
			m_spPos.Attach(pObject, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvGroupingAtom, e.Error());
		}
		return S_OK;
	}

	void FinalRelease() 
	{
		m_spPos = NULL;
		m_spSynthGreek = NULL;
		m_spRTContext = NULL;
	}

public:

		IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpiryMonth, m_nExpiryMonth)
		IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, HTBRate, m_dHTBRate)
		IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPosColl*, Pos, m_spPos)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDeltaInShares, m_dNetDeltaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDeltaEq, m_dNetDeltaEq)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaInShares, m_dVegaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValueInShares, m_dTimeValueInShares)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInShares, m_bBadDeltaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetDeltaInShares, m_bBadNetDeltaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetDeltaEq, m_bBadNetDeltaEq)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaInShares, m_bBadGammaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaInShares, m_bBadVegaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValueInShares, m_bBadTimeValueInShares)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HasSynth, m_bHasSynth)
		IMPLEMENT_OBJECT_PROPERTY(IMmRvSynthGreeksColl*, SynthGreek, m_spSynthGreek)

		IMPLEMENT_SIMPLE_PROPERTY(LONG, Moneyness, m_nRoundMoneyness)

		IMPLEMENT_SIMPLE_PROPERTY(LONG, cvPoolID, m_nPoolID)
		IMPLEMENT_OBJECT_PROPERTY(IcvRTContext*, cvRTContext, m_spRTContext)

		STDMETHODIMP ClearValues()
	{
		__MmRvGroupingAtom::ClearValues();
		return S_OK;
	}


		STDMETHOD(CalcCV)();

		// IcvDataProvider
		STDMETHOD(raw_GetData)(LONG lDataID, VARIANT* Value);
		STDMETHOD(raw_CallFunction)(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value);
		STDMETHOD(raw_Check)(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal); 
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvGroupingAtom), CMmRvGroupingAtom)

#endif //__MMRVGROUPINGATOM_H__

