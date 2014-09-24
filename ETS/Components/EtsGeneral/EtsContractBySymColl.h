// EtsContractBySymColl.h : Declaration of the CEtsContractBySymColl
#ifndef __ETSCONTRACTBYSYMCOLL_H__
#define __ETSCONTRACTBYSYMCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsContractAtom.h"

_COM_SMARTPTR_TYPEDEF(IEtsContractBySymColl, IID_IEtsContractBySymColl);

typedef IDispatchImpl<IEtsContractBySymColl, &IID_IEtsContractBySymColl, &LIBID_EtsGeneralLib>								IEtsContractBySymCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IEtsContractBySymCollDispImpl, BSTR, IEtsContractAtom, CAdapt<CComBSTR> >		IEtsContractBySymCollImpl;

// CEtsContractBySymColl
class ATL_NO_VTABLE CEtsContractBySymColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsContractBySymColl, &CLSID_EtsContractBySymColl>,
	public ISupportErrorInfoImpl<&IID_IEtsContractBySymColl>,
	public IEtsContractBySymCollImpl
{
public:
	CEtsContractBySymColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSCONTRACTBYSYMCOLL)


BEGIN_COM_MAP(CEtsContractBySymColl)
	COM_INTERFACE_ENTRY(IEtsContractBySymColl)
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
		IEtsContractBySymCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(BSTR Key, IEtsContractAtom* Value, IEtsContractAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsContractBySymColl), CEtsContractBySymColl)

#endif //__ETSCONTRACTBYSYMCOLL_H__