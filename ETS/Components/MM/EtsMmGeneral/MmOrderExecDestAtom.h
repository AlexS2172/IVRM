// MmOrderExecDestAtom.h : Declaration of the CMmOrderExecDestAtom
#ifndef __MMORDEREXECDESTATOM_H__
#define __MMORDEREXECDESTATOM_H
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"

_COM_SMARTPTR_TYPEDEF(IMmOrderExecDestAtom, IID_IMmOrderExecDestAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmOrderExecDestAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrCode;
	CComBSTR					m_bstrName;
	VARIANT_BOOL				m_bIsOption;
	VARIANT_BOOL				m_bIsStock;

	__MmOrderExecDestAtom() :
		m_nID(0L), m_bIsOption(VARIANT_FALSE), m_bIsStock(VARIANT_FALSE)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmOrderExecDestAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmOrderExecDestAtom, &CLSID_MmOrderExecDestAtom>,
	public ISupportErrorInfoImpl<&IID_IMmOrderExecDestAtom>,
	public IDispatchImpl<IMmOrderExecDestAtom, &IID_IMmOrderExecDestAtom, &LIBID_EtsMmGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmOrderExecDestAtom
{
public:
	CMmOrderExecDestAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMORDEREXECDESTATOM)

DECLARE_NOT_AGGREGATABLE(CMmOrderExecDestAtom)

BEGIN_COM_MAP(CMmOrderExecDestAtom)
	COM_INTERFACE_ENTRY(IMmOrderExecDestAtom)
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
	IMPLEMENT_BSTR_PROPERTY(Code, m_bstrCode)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsOption, m_bIsOption)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsStock, m_bIsStock)
};

OBJECT_ENTRY_AUTO(__uuidof(MmOrderExecDestAtom), CMmOrderExecDestAtom)

#endif //__MMORDEREXECDESTATOM_H