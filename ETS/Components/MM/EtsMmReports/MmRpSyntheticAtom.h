// MmRpSyntheticAtom.h : Declaration of the CMmRpSyntheticAtom
#ifndef __MM_RP_SYNTHETIC_ATOM__
#define __MM_RP_SYNTHETIC_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"
#include "MmRpSyntheticOptColl.h"

_COM_SMARTPTR_TYPEDEF(IMmRpSyntheticAtom, IID_IMmRpSyntheticAtom);

struct __MmRpSyntheticAtom
{
	LONG									m_nID;
	DOUBLE									m_dPrice;
	CComBSTR								m_bstrSymbol;
	EtsContractTypeEnum						m_enUndType;
	IMmRpSyntheticOptCollPtr				m_spOpt;
	DOUBLE									m_dPos;
	DOUBLE									m_dNetDelta;
	DOUBLE									m_dSynthetic;
	DOUBLE									m_dNet;

	__MmRpSyntheticAtom() :
		m_nID(0L), m_dPrice(0.), m_enUndType(enCtIndex),
		m_dPos(0.), m_dNetDelta(0.),
		m_dSynthetic(0.), m_dNet(0.)
	{
	}
};

// CMmRpSyntheticAtom

class ATL_NO_VTABLE CMmRpSyntheticAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpSyntheticAtom, &CLSID_MmRpSyntheticAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpSyntheticAtom>,
	public IDispatchImpl<IMmRpSyntheticAtom, &IID_IMmRpSyntheticAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpSyntheticAtom
{
public:
	CMmRpSyntheticAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPSYNTHETICATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpSyntheticAtom)

BEGIN_COM_MAP(CMmRpSyntheticAtom)
	COM_INTERFACE_ENTRY(IMmRpSyntheticAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{	
			__CHECK_HRESULT(m_spOpt.CreateInstance(CLSID_MmRpSyntheticOptColl), _T("Failed to create options collection."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpSyntheticAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOpt = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,			m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Price,		m_dPrice)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,	m_enUndType)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpSyntheticOptColl*,	Opt,	m_spOpt)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Pos,		m_dPos)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				NetDelta,	m_dNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Synthetic,	m_dSynthetic)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Net,		m_dNet)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpSyntheticAtom), CMmRpSyntheticAtom)

#endif //__MM_RP_SYNTHETIC_ATOM__