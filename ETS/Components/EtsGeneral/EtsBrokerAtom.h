// EtsBrokerAtom.h : Declaration of the CEtsBrokerAtom
#ifndef __ETSBROKERATOM_H__
#define __ETSBROKERATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IEtsBrokerAtom, IID_IEtsBrokerAtom);

struct __EtsBrokerAtom
{
	LONG				m_nID;
	_bstr_t				m_bstrName;
	_bstr_t				m_bstrDesc;
	VARIANT_BOOL		m_bIsClearing;
	DOUBLE				m_dUndComm;
	DOUBLE				m_dOptRegComm;
	DOUBLE				m_dOptSpreadComm;
	DOUBLE				m_dLowPremComm;

	__EtsBrokerAtom()
		: m_nID(0L), m_bIsClearing(VARIANT_FALSE),
		m_dUndComm(0.), m_dOptRegComm(0.), m_dOptSpreadComm(0.), m_dLowPremComm(0.)
	{
	}
};

// CEtsBrokerAtom

class ATL_NO_VTABLE CEtsBrokerAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsBrokerAtom, &CLSID_EtsBrokerAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsBrokerAtom>,
	public IDispatchImpl<IEtsBrokerAtom, &IID_IEtsBrokerAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsBrokerAtom
{
public:
	CEtsBrokerAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSBROKERATOM)


BEGIN_COM_MAP(CEtsBrokerAtom)
	COM_INTERFACE_ENTRY(IEtsBrokerAtom)
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
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsClearing, m_bIsClearing)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, UndComm, m_dUndComm)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, OptRegComm, m_dOptRegComm)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, OptSpreadComm, m_dOptSpreadComm)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, LowPremComm, m_dLowPremComm)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsBrokerAtom), CEtsBrokerAtom)

#endif //__ETSBROKERATOM_H__
