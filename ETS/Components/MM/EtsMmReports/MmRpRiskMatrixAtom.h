// MmRpRiskMatrixAtom.h : Declaration of the CMmRpRiskMatrixAtom
#ifndef __MM_RP_RISK_MATRIX_ATOM__
#define __MM_RP_RISK_MATRIX_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"
#include "MmRpRiskMatrixShiftColl.h"

_COM_SMARTPTR_TYPEDEF(IMmRpRiskMatrixAtom, IID_IMmRpRiskMatrixAtom);

struct __MmRpRiskMatrixAtom
{
	LONG							m_nID;
	CComBSTR						m_bstrSymbol;
	DOUBLE							m_dPrice;
	EtsContractTypeEnum				m_enUndType;
	DOUBLE							m_dHV;
	DOUBLE							m_dSTD;
	IMmRpRiskMatrixShiftCollPtr		m_spShift;

	__MmRpRiskMatrixAtom() :
		m_nID(0L), m_dPrice(0.), m_enUndType(enCtIndex),
		m_dHV(0.), m_dSTD(0.)
	{
	}
};

// CMmRpRiskMatrixAtom

class ATL_NO_VTABLE CMmRpRiskMatrixAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpRiskMatrixAtom, &CLSID_MmRpRiskMatrixAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpRiskMatrixAtom>,
	public IDispatchImpl<IMmRpRiskMatrixAtom, &IID_IMmRpRiskMatrixAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpRiskMatrixAtom
{
public:
	CMmRpRiskMatrixAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPRISKMATRIXATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpRiskMatrixAtom)

BEGIN_COM_MAP(CMmRpRiskMatrixAtom)
	COM_INTERFACE_ENTRY(IMmRpRiskMatrixAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{	
			__CHECK_HRESULT(m_spShift.CreateInstance(CLSID_MmRpRiskMatrixShiftColl), _T("Failed to create MmRpRiskMatrixShiftColl object."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpRiskMatrixAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spShift = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,			m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Price,		m_dPrice)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,	m_enUndType)
    IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				HV,			m_dHV)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				STD,		m_dSTD)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpRiskMatrixShiftColl*,	Shift,	m_spShift)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpRiskMatrixAtom), CMmRpRiskMatrixAtom)

#endif //__MM_RP_RISK_MATRIX_ATOM__