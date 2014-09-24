// UndBySymColl.h : Declaration of the CUndBySymColl
#ifndef __UNDBYSYMCOLL_H__
#define __UNDBYSYMCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "UndAtom.h"

_COM_SMARTPTR_TYPEDEF(IUndBySymColl, IID_IUndBySymColl);

typedef IDispatchImpl<IUndBySymColl, &IID_IUndBySymColl, &LIBID_EtsGeneralLib>								IUndBySymCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IUndBySymCollDispImpl, BSTR, IUndAtom, CAdapt<CComBSTR> >		IUndBySymCollImpl;

// CUndBySymColl
class ATL_NO_VTABLE CUndBySymColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CUndBySymColl, &CLSID_UndBySymColl>,
	public ISupportErrorInfoImpl<&IID_IUndBySymColl>,
	public IUndBySymCollImpl
{
public:
	CUndBySymColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_UNDBYSYMCOLL)


BEGIN_COM_MAP(CUndBySymColl)
	COM_INTERFACE_ENTRY(IUndBySymColl)
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
		IUndBySymCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(BSTR Key, IUndAtom* Value, IUndAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(UndBySymColl), CUndBySymColl)

#endif //__UNDBYSYMCOLL_H__