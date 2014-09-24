// MmRpOptRootAtom.h : Declaration of the CMmRpOptRootAtom
#ifndef __MM_RP_OPTROOT_ATOM__
#define __MM_RP_OPTROOT_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpOptRootAtom, IID_IMmRpOptRootAtom);

struct __MmRpOptRootAtom
{
	LONG				m_nID;
	CComBSTR			m_bstrName;
	LONG				m_nLotSize;
	VARIANT_BOOL		m_vbIsSynthetic;

	__MmRpOptRootAtom() :
		m_nID(0L), m_nLotSize(0L), m_vbIsSynthetic(VARIANT_FALSE)
	{
	}
};

// CMmRpOptRootAtom

class ATL_NO_VTABLE CMmRpOptRootAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpOptRootAtom, &CLSID_MmRpOptRootAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpOptRootAtom>,
	public IDispatchImpl<IMmRpOptRootAtom, &IID_IMmRpOptRootAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpOptRootAtom
{
public:
	CMmRpOptRootAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPOPTROOTATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpOptRootAtom)

BEGIN_COM_MAP(CMmRpOptRootAtom)
	COM_INTERFACE_ENTRY(IMmRpOptRootAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG,				ID,				m_nID)
	IMPLEMENT_BSTR_PROPERTY(Name,	m_bstrName)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,				LotSize,		m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,		IsSynthetic,	m_vbIsSynthetic)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpOptRootAtom), CMmRpOptRootAtom)

#endif //__MM_RP_OPTROOT_ATOM__