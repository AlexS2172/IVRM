// EtsOptRootColl.h : Declaration of the CEtsOptRootColl
#ifndef __ETSOPTROOTCOLL_H__
#define __ETSOPTROOTCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsOptRootAtom.h"

//_COM_SMARTPTR_TYPEDEF(IEtsOptRootColl, IID_IEtsOptRootColl);

typedef IDispatchImpl<IEtsOptRootColl, &IID_IEtsOptRootColl, &LIBID_EtsGeneralLib>													IEtsOptRootCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsOptRootCollDispImpl, IEtsOptRootAtom, LONG, LONG, BSTR, _bstr_t >	IEtsOptRootCollImpl;

// CEtsOptRootColl
class ATL_NO_VTABLE CEtsOptRootColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsOptRootColl, &CLSID_EtsOptRootColl>,
	public ISupportErrorInfoImpl<&IID_IEtsOptRootColl>,
	public IEtsOptRootCollImpl
{
public:
	CEtsOptRootColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSOPTROOTCOLL)


BEGIN_COM_MAP(CEtsOptRootColl)
	COM_INTERFACE_ENTRY(IEtsOptRootColl)
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
		IEtsOptRootCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsOptRootAtom* Value, IEtsOptRootAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsOptRootColl), CEtsOptRootColl)

#endif //__ETSOPTROOTCOLL_H__
