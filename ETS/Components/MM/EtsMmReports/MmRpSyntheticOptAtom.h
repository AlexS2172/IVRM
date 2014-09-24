// MmRpSyntheticOptAtom.h : Declaration of the CMmRpSyntheticOptAtom
#ifndef __MM_RP_SYNTHETICOPT_ATOM__
#define __MM_RP_SYNTHETICOPT_ATOM__
#pragma once
#include "resource.h"       // main symbols

#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpSyntheticOptAtom, IID_IMmRpSyntheticOptAtom);

struct __MmRpSyntheticOptAtom
{
	LONG					m_nStockID;
	CComBSTR				m_bstrRootSymbol;
	LONG					m_nRootID;
	DATE					m_dtExpiry;
	DOUBLE					m_dStrike;
	DOUBLE					m_dNetPos;
	DOUBLE					m_dDelta;
	DOUBLE					m_dGamma;
	DOUBLE					m_dVega;
	DOUBLE					m_dTheta;
    DOUBLE					m_dOTMDelta;
	LONG					m_nID;
	DOUBLE					m_dNetGamma;
    
	void ClearValues(VARIANT_BOOL bDataOnly = VARIANT_TRUE)
	{
		if (bDataOnly != VARIANT_TRUE)
		{
			m_nID = 0L;
			m_nStockID = 0L;
			m_bstrRootSymbol.Empty();
			m_nRootID = 0L;
			m_dtExpiry =0.;
			m_dStrike = 0.;
		}

		m_dNetPos = 0.;
		m_dDelta = 0.;
		m_dGamma = 0.;
		m_dVega = 0.;
		m_dTheta = 0.;
		m_dOTMDelta = 0.;
	}

	__MmRpSyntheticOptAtom() :
		m_nStockID(0L), m_nRootID(0L), m_dtExpiry(0.), 
		m_dStrike(0.), m_dNetPos(0.),
		m_dDelta(0.), m_dGamma(0.),
		m_dVega(0.), m_dTheta(0.),
		m_dOTMDelta(0.), m_nID(0)
	{
	}
};

// CMmRpSyntheticOptAtom

class ATL_NO_VTABLE CMmRpSyntheticOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpSyntheticOptAtom, &CLSID_MmRpSyntheticOptAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpSyntheticOptAtom>,
	public IDispatchImpl<IMmRpSyntheticOptAtom, &IID_IMmRpSyntheticOptAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public __MmRpSyntheticOptAtom
{
public:
	CMmRpSyntheticOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPSYNTHETICOPTATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpSyntheticOptAtom)

BEGIN_COM_MAP(CMmRpSyntheticOptAtom)
	COM_INTERFACE_ENTRY(IMmRpSyntheticOptAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG,			StockID,	m_nStockID)
	IMPLEMENT_BSTR_PROPERTY(RootSymbol,		m_bstrRootSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,			RootID,		m_nRootID)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,			Expiry,		m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Strike,		m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		NetPos,		m_dNetPos)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Delta,		m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Gamma,		m_dGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Vega,		m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Theta,		m_dTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		OTMDelta,	m_dOTMDelta)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,			ID,			m_nID)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		NetGamma,	m_dNetGamma)

	STDMETHOD(Clone)(IMmRpSyntheticOptAtom** ppRetVal);
	STDMETHOD(ClearValues)(VARIANT_BOOL DataOnly);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpSyntheticOptAtom), CMmRpSyntheticOptAtom)

#endif //__MM_RP_SYNTHETICOPT_ATOM__