// SynthRootColl.h : Declaration of the CSynthRootColl
#ifndef __SYNTHROOTCOLL_H__
#define __SYNTHROOTCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "SynthRootAtom.h"


_COM_SMARTPTR_TYPEDEF(ISynthRootColl, IID_ISynthRootColl);

typedef IDispatchImpl<ISynthRootColl, &IID_ISynthRootColl, &LIBID_EtsGeneralLib>					ISynthRootCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<ISynthRootCollDispImpl, LONG, ISynthRootAtom, LONG >	ISynthRootCollImpl;

// CSynthRootColl

class ATL_NO_VTABLE CSynthRootColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSynthRootColl, &CLSID_SynthRootColl>,
	public ISupportErrorInfoImpl<&IID_ISynthRootColl>,
	public ISynthRootCollImpl
{
public:
	CSynthRootColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SYNTHROOTCOLL)


BEGIN_COM_MAP(CSynthRootColl)
	COM_INTERFACE_ENTRY(ISynthRootColl)
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
		ISynthRootCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, ISynthRootAtom* Value, ISynthRootAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(SynthRootColl), CSynthRootColl)

#endif //__SYNTHROOTCOLL_H__
