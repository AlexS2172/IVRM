// EtsTraderColl.h : Declaration of the CEtsTraderColl
#ifndef __ETSTRADERCOLL_H__
#define __ETSTRADERCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsTraderAtom.h"

_COM_SMARTPTR_TYPEDEF(IEtsTraderColl, IID_IEtsTraderColl);

typedef IDispatchImpl<IEtsTraderColl, &IID_IEtsTraderColl, &LIBID_EtsGeneralLib>													IEtsTraderCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsTraderCollDispImpl, IEtsTraderAtom, LONG, LONG, BSTR, _bstr_t  >	IEtsTraderCollImpl;

// CEtsTraderColl
class ATL_NO_VTABLE CEtsTraderColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsTraderColl, &CLSID_EtsTraderColl>,
	public ISupportErrorInfoImpl<&IID_IEtsTraderColl>,
	public IEtsTraderCollImpl
{
public:
	CEtsTraderColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSTRADERCOLL)


BEGIN_COM_MAP(CEtsTraderColl)
	COM_INTERFACE_ENTRY(IEtsTraderColl)
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
		IEtsTraderCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IEtsTraderAtomPtr AddNew(long lKey, const _bstr_t& bsName, CComObject<CEtsTraderAtom>** pAtom = NULL);

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsTraderAtom* Value, IEtsTraderAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsTraderColl), CEtsTraderColl)

#endif //__ETSTRADERCOLL_H__
