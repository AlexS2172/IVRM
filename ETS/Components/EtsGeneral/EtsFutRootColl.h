// EtsFutRootColl.h : Declaration of the CEtsFutRootColl
#ifndef __ETSFUTROOTCOLL_H__
#define __ETSFUTROOTCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsFutRootAtom.h"

//_COM_SMARTPTR_TYPEDEF(IEtsFutRootColl, IID_IEtsFutRootColl);

typedef IDispatchImpl<IEtsFutRootColl, &IID_IEtsFutRootColl, &LIBID_EtsGeneralLib>													IEtsFutRootCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsFutRootCollDispImpl, IEtsFutRootAtom, LONG, LONG, BSTR, _bstr_t >	IEtsFutRootCollImpl;

// CEtsFutRootColl
class ATL_NO_VTABLE CEtsFutRootColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsFutRootColl, &CLSID_EtsFutRootColl>,
	public ISupportErrorInfoImpl<&IID_IEtsFutRootColl>,
	public IEtsFutRootCollImpl
{
public:
	CEtsFutRootColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSFUTROOTCOLL)


BEGIN_COM_MAP(CEtsFutRootColl)
	COM_INTERFACE_ENTRY(IEtsFutRootColl)
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
		IEtsFutRootCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsFutRootAtom* Value, IEtsFutRootAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsFutRootColl), CEtsFutRootColl)

#endif //__ETSFUTROOTCOLL_H__
