// EtsOptColl.h : Declaration of the CEtsOptColl
#ifndef __ETSOPTCOLL_H__
#define __ETSOPTCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsOptAtom.h"

typedef IDispatchImpl<IEtsOptColl, &IID_IEtsOptColl, &LIBID_EtsGeneralLib>													IEtsOptCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsOptCollDispImpl, IEtsOptAtom, LONG, LONG, BSTR, _bstr_t  >	IEtsOptCollImpl;

// CEtsOptColl

class ATL_NO_VTABLE CEtsOptColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsOptColl, &CLSID_EtsOptColl>,
	public ISupportErrorInfoImpl<&IID_IEtsOptColl>,
	public IEtsOptCollImpl
{
public:
	CEtsOptColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSOPTCOLL)


BEGIN_COM_MAP(CEtsOptColl)
	COM_INTERFACE_ENTRY(IEtsOptColl)
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
		IEtsOptCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsOptAtom* Value, IEtsOptAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsOptColl), CEtsOptColl)

#endif //__ETSOPTCOLL_H__
