// MmTntCounterPartyAtom.h : Declaration of the CMmTntCounterPartyAtom
#ifndef __MMTNTCOUNTERPARTYATOM_H__
#define __MMTNTCOUNTERPARTYATOM_H
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"

_COM_SMARTPTR_TYPEDEF(IMmTntCounterPartyAtom, IID_IMmTntCounterPartyAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmTntCounterPartyAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrName;
	CComBSTR					m_bstrDesc;

	__MmTntCounterPartyAtom() :
		m_nID(0L)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmTntCounterPartyAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTntCounterPartyAtom, &CLSID_MmTntCounterPartyAtom>,
	public ISupportErrorInfoImpl<&IID_IMmTntCounterPartyAtom>,
	public IDispatchImpl<IMmTntCounterPartyAtom, &IID_IMmTntCounterPartyAtom, &LIBID_EtsMmGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmTntCounterPartyAtom
{
public:
	CMmTntCounterPartyAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTNTCOUNTERPARTYATOM)

DECLARE_NOT_AGGREGATABLE(CMmTntCounterPartyAtom)

BEGIN_COM_MAP(CMmTntCounterPartyAtom)
	COM_INTERFACE_ENTRY(IMmTntCounterPartyAtom)
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
	
	// core info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
	IMPLEMENT_BSTR_PROPERTY(Desc, m_bstrDesc)
};

OBJECT_ENTRY_AUTO(__uuidof(MmTntCounterPartyAtom), CMmTntCounterPartyAtom)

#endif //__MMTNTCOUNTERPARTYATOM_H