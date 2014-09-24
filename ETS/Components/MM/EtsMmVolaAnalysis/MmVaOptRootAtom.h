// MmVaOptRootAtom.h : Declaration of the CMmVaOptRootAtom
#ifndef __MMVAOPTROOTATOM_H_
#define __MMVAOPTROOTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"

_COM_SMARTPTR_TYPEDEF(IMmVaOptAtom, IID_IMmVaOptAtom);

struct __MmVaOptRootAtom
{
	LONG				m_nID;
	CComBSTR			m_bstrName;

	__MmVaOptRootAtom()
		: m_nID(0L)
	{
	}
};

// CMmVaOptRootAtom
class ATL_NO_VTABLE CMmVaOptRootAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaOptRootAtom, &CLSID_MmVaOptRootAtom>,
	public ISupportErrorInfoImpl<&IID_IMmVaOptRootAtom>,
	public IDispatchImpl<IMmVaOptRootAtom, &IID_IMmVaOptRootAtom, &LIBID_EtsMmVolaAnalysisLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmVaOptRootAtom
{
public:
	CMmVaOptRootAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAOPTROOTATOM)


BEGIN_COM_MAP(CMmVaOptRootAtom)
	COM_INTERFACE_ENTRY(IMmVaOptRootAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaOptRootAtom), CMmVaOptRootAtom)

#endif //__MMVAOPTROOTATOM_H_