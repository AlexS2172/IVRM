// SynthRootCompColl.h : Declaration of the CSynthRootCompColl

#ifndef __SYNTHROOTCOMPCOLL_H__
#define __SYNTHROOTCOMPCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "SynthRootCompAtom.h"


_COM_SMARTPTR_TYPEDEF(ISynthRootCompColl, IID_ISynthRootCompColl);

typedef IDispatchImpl<ISynthRootCompColl, &IID_ISynthRootCompColl, &LIBID_EtsGeneralLib>					ISynthRootCompCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<ISynthRootCompCollDispImpl, LONG, ISynthRootCompAtom, LONG >	ISynthRootCompCollImpl;

// CSynthRootCompColl

class ATL_NO_VTABLE CSynthRootCompColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSynthRootCompColl, &CLSID_SynthRootCompColl>,
	public ISupportErrorInfoImpl<&IID_ISynthRootCompColl>,
	public ISynthRootCompCollImpl
{
public:
	CSynthRootCompColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SYNTHROOTCOMPCOLL)


BEGIN_COM_MAP(CSynthRootCompColl)
	COM_INTERFACE_ENTRY(ISynthRootCompColl)
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
		ISynthRootCompCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, ISynthRootCompAtom* Value, ISynthRootCompAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(SynthRootCompColl), CSynthRootCompColl)

#endif //__SYNTHROOTCOMPCOLL_H__