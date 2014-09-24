// MmRpRiskMatrixShiftAtom.h : Declaration of the CMmRpRiskMatrixShiftAtom
#ifndef __MM_RP_RISK_MATRIX_SHIFT_ATOM__
#define __MM_RP_RISK_MATRIX_SHIFT_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"
#include "MmRpRiskMatrixShiftColl.h"

_COM_SMARTPTR_TYPEDEF(IMmRpRiskMatrixShiftAtom, IID_IMmRpRiskMatrixShiftAtom);

struct __MmRpRiskMatrixShiftAtom
{
	LONG							m_nID;
	CComBSTR						m_bstrSymbol;
	DOUBLE							m_dPrice;
	EtsContractTypeEnum				m_enUndType;
	DOUBLE							m_dShift;
	LONG							m_nGroupID;
	DOUBLE							m_dMarketMove;
	DOUBLE							m_dMarketValue;
	DOUBLE							m_dMarketValueChange;
	DOUBLE							m_dPoP;
	DOUBLE							m_dOptionsTheoPnLDaily;
	DOUBLE							m_dDelta;
	DOUBLE							m_dGamma;
	DOUBLE							m_dVega;
	DOUBLE							m_dTheta;
	DOUBLE							m_dNetGamma;

	__MmRpRiskMatrixShiftAtom() :
		m_nID(0L), m_dPrice(0.), m_enUndType(enCtIndex), m_dShift(0.), 
		m_nGroupID(0L), m_dMarketValue(0.), m_dMarketValueChange(0.),
		m_dMarketMove(0.), m_dPoP(0.), m_dOptionsTheoPnLDaily(0.), 
		m_dDelta(0.), m_dGamma(0.), m_dNetGamma(0.),  m_dVega(0.), m_dTheta(0.)
	{
	}
};

// CMmRpRiskMatrixShiftAtom

class ATL_NO_VTABLE CMmRpRiskMatrixShiftAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpRiskMatrixShiftAtom, &CLSID_MmRpRiskMatrixShiftAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpRiskMatrixShiftAtom>,
	public IDispatchImpl<IMmRpRiskMatrixShiftAtom, &IID_IMmRpRiskMatrixShiftAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpRiskMatrixShiftAtom
{
public:
	CMmRpRiskMatrixShiftAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPRISKMATRIXSHIFTATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpRiskMatrixShiftAtom)

BEGIN_COM_MAP(CMmRpRiskMatrixShiftAtom)
	COM_INTERFACE_ENTRY(IMmRpRiskMatrixShiftAtom)
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
	IMPLEMENT_BSTR_PROPERTY(Symbol,			m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Price,					m_dPrice)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,				m_enUndType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Shift,					m_dShift)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					GroupID,				m_nGroupID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				MarketMove,				m_dMarketMove)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				MarketValue,			m_dMarketValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				MarketValueChange,		m_dMarketValueChange)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PoP,					m_dPoP)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				OptionsTheoPnLDaily,	m_dOptionsTheoPnLDaily)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Delta,					m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Gamma,					m_dGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Vega,					m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Theta,					m_dTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				NetGamma,				m_dNetGamma)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpRiskMatrixShiftAtom), CMmRpRiskMatrixShiftAtom)

#endif //__MM_RP_RISK_MATRIX_SHIFT_ATOM__