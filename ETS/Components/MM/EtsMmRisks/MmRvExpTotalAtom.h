// MmRvExpTotalAtom.h : Declaration of the CMmRvExpTotalAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvExpColl.h"

#include "../cv/cvCV.h"

_COM_SMARTPTR_TYPEDEF(IMmRvExpTotalAtom, IID_IMmRvExpTotalAtom);

struct __MmRvExpTotalAtom
{
	LONG					m_nExpiryMonth;
	IMmRvExpCollPtr			m_spExpiry;
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
	DOUBLE					m_dThetaInShares;
	VARIANT_BOOL			m_bBadThetaInShares;

	// custom variables
	LONG					m_nPoolID;
	IcvRTContextPtr			m_spRTContext;

	__MmRvExpTotalAtom()
	: m_nExpiryMonth(0L),
	m_dDeltaInShares(BAD_DOUBLE_VALUE), m_dDeltaEq(BAD_DOUBLE_VALUE),
	m_dGammaInShares(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
	m_dVegaInShares(BAD_DOUBLE_VALUE), m_dTimeValueInShares(BAD_DOUBLE_VALUE),
	m_bBadDeltaInShares(VARIANT_FALSE), m_bBadDeltaEq(VARIANT_FALSE),
	m_bBadGammaInShares(VARIANT_FALSE), m_bBadGammaEq(VARIANT_FALSE),
	m_bBadVegaInShares(VARIANT_FALSE), m_bBadTimeValueInShares(VARIANT_FALSE),
	m_dRhoInShares(BAD_DOUBLE_VALUE), m_bBadRhoInShares(VARIANT_FALSE),
	m_dThetaInShares(BAD_DOUBLE_VALUE), m_bBadThetaInShares(VARIANT_FALSE),
	m_nPoolID(BAD_LONG_VALUE){
		m_spRTContext = NULL;
	};

	void ClearValues()
	{
		m_dDeltaInShares = BAD_DOUBLE_VALUE;
		m_dDeltaEq = BAD_DOUBLE_VALUE;
		m_dGammaInShares = BAD_DOUBLE_VALUE;
		m_dGammaEq = BAD_DOUBLE_VALUE;
		m_dVegaInShares = BAD_DOUBLE_VALUE;
		m_dTimeValueInShares = BAD_DOUBLE_VALUE;
		m_dThetaInShares = BAD_DOUBLE_VALUE;

		m_bBadDeltaInShares = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaInShares = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadVegaInShares = VARIANT_FALSE;
		m_bBadTimeValueInShares = VARIANT_FALSE;
		m_bBadRhoInShares = VARIANT_FALSE ;
		m_bBadThetaInShares = VARIANT_FALSE;

		m_dRhoInShares = BAD_DOUBLE_VALUE;
	}
};



// CMmRvExpTotalAtom
class ATL_NO_VTABLE CMmRvExpTotalAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvExpTotalAtom, &CLSID_MmRvExpTotalAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvExpTotalAtom>,
	public IDispatchImpl<IMmRvExpTotalAtom, &IID_IMmRvExpTotalAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IcvDataProvider, &IID_IMmRvUndAtom, &__uuidof(__CVLib), /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvExpTotalAtom
{
public:
	CMmRvExpTotalAtom():m_pExpiry(NULL){
		m_pUnkMarshaler = NULL;
	};

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVEXPTOTALATOM)

	BEGIN_COM_MAP(CMmRvExpTotalAtom)
		COM_INTERFACE_ENTRY(IMmRvExpTotalAtom)
		COM_INTERFACE_ENTRY(IcvDataProvider)
		COM_INTERFACE_ENTRY2(IDispatch, IMmRvExpTotalAtom)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct(){
		_CHK(CComObject<CMmRvExpColl>::CreateInstance(&m_pExpiry),  _T("Fail to create expiries."));
		m_spExpiry.Attach(m_pExpiry, TRUE);

		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	};

	void FinalRelease(){
		m_spRTContext = NULL;
		m_pUnkMarshaler.Release();
	};

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	CComObject<CMmRvExpColl>* m_pExpiry;

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpiryMonth, m_nExpiryMonth)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvExpColl*, Expiry, m_spExpiry)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaInShares, m_dVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValueInShares, m_dTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaInShares, m_dThetaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares,m_dRhoInShares)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInShares, m_bBadDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRhoInShares, m_bBadRhoInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaInShares, m_bBadGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaInShares, m_bBadVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValueInShares, m_bBadTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadThetaInShares, m_bBadThetaInShares)

	STDMETHODIMP ClearValues()
	{
		__MmRvExpTotalAtom::ClearValues();
		return S_OK;
	}

	STDMETHOD(CalcTotals)();

	IMPLEMENT_SIMPLE_PROPERTY(LONG, cvPoolID, m_nPoolID)
	IMPLEMENT_OBJECT_PROPERTY(IcvRTContext*, cvRTContext, m_spRTContext)

	STDMETHOD(CalcCV)();

	// IcvDataProvider
	STDMETHOD(raw_GetData)(LONG lDataID, VARIANT* Value);
	STDMETHOD(raw_CallFunction)(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value);
	STDMETHOD(raw_Check)(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvExpTotalAtom), CMmRvExpTotalAtom)
