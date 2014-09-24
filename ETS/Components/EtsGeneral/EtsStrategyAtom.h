// EtsStrategyAtom.h : Declaration of the CEtsStrategyAtom
#ifndef __ETSSTRATEGYATOM_H__
#define __ETSSTRATEGYATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IEtsStrategyAtom, IID_IEtsStrategyAtom);

struct __EtsStrategyAtom
{
	LONG					m_nID;
	_bstr_t					m_bstrName;
	_bstr_t					m_bstrDesc;

	__EtsStrategyAtom()
		: m_nID(0L)
	{
	}
};

// CEtsStrategyAtom

class ATL_NO_VTABLE CEtsStrategyAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsStrategyAtom, &CLSID_EtsStrategyAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsStrategyAtom>,
	public IDispatchImpl<IEtsStrategyAtom, &IID_IEtsStrategyAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsStrategyAtom
{
public:
	CEtsStrategyAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSSTRATEGYATOM)


BEGIN_COM_MAP(CEtsStrategyAtom)
	COM_INTERFACE_ENTRY(IEtsStrategyAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTRT_PROPERTY(Name, m_bstrName)
	IMPLEMENT_BSTRT_PROPERTY(Desc, m_bstrDesc)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsStrategyAtom), CEtsStrategyAtom)

#endif //__ETSSTRATEGYATOM_H__
