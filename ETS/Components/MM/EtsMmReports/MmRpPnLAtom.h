// MmRpPnLAtom.h : Declaration of the CMmRpPnLAtom
#ifndef __MM_RP_PNL_ATOM__
#define __MM_RP_PNL_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPnLAtom, IID_IMmRpPnLAtom);

struct __MmRpPnLAtom
{
	LONG					m_nID;
	DOUBLE					m_dCallMarketValue;
	DOUBLE					m_dCallPoP;	
	DOUBLE					m_dNetMarketValue;
	DOUBLE					m_dNetPoP;
	DOUBLE					m_dOptionsMTMLTDPnL;
	DOUBLE					m_dOptionsMTMPnLDaily;
	DOUBLE					m_dOptionsTheoLTDPnL;
	DOUBLE					m_dOptionsTheoPnLDaily;
	DOUBLE					m_dPutMarketValue;
	DOUBLE					m_dPutPoP;
	CComBSTR				m_bstrSymbol;
	DOUBLE					m_dUndMTMLTDPnL;
	DOUBLE					m_dUndMTMPnLDaily;
	EtsContractTypeEnum		m_enUndType;

	__MmRpPnLAtom() :
		m_dCallMarketValue(0.), m_dCallPoP(0.),
		m_nID(0L), m_dNetMarketValue(0.), m_dNetPoP(0.),
		m_dOptionsMTMLTDPnL(0.), m_dOptionsMTMPnLDaily(0.),
		m_dOptionsTheoLTDPnL(0.), m_dOptionsTheoPnLDaily(0.),
        m_dPutMarketValue(0.), m_dPutPoP(0.),
		m_dUndMTMLTDPnL(0.), m_dUndMTMPnLDaily(0.),
		m_enUndType(enCtIndex)
	{
	}
};

// CMmRpPnLAtom

class ATL_NO_VTABLE CMmRpPnLAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPnLAtom, &CLSID_MmRpPnLAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpPnLAtom>,
	public IDispatchImpl<IMmRpPnLAtom, &IID_IMmRpPnLAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpPnLAtom
{
public:
	CMmRpPnLAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPNLATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpPnLAtom)

BEGIN_COM_MAP(CMmRpPnLAtom)
	COM_INTERFACE_ENTRY(IMmRpPnLAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,						m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				CallMarketValue,		m_dCallMarketValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				CallPoP,				m_dCallPoP)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				NetMarketValue,			m_dNetMarketValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				NetPoP,					m_dNetPoP)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				OptionsMTMLTDPnL,		m_dOptionsMTMLTDPnL)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				OptionsMTMPnLDaily,		m_dOptionsMTMPnLDaily)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				OptionsTheoLTDPnL,		m_dOptionsTheoLTDPnL)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				OptionsTheoPnLDaily,	m_dOptionsTheoPnLDaily)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PutMarketValue,			m_dPutMarketValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PutPoP,					m_dPutPoP)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				UndMTMLTDPnL,			m_dUndMTMLTDPnL)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				UndMTMPnLDaily,			m_dUndMTMPnLDaily)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,				m_enUndType)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPnLAtom), CMmRpPnLAtom)

#endif //__MM_RP_PNL_ATOM__