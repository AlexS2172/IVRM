// EtsTraderGroupColl.h : Declaration of the CEtsTraderGroupColl
#ifndef __ETSTRADERGROUPCOLL_H__
#define __ETSTRADERGROUPCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsTraderGroupAtom.h"

_COM_SMARTPTR_TYPEDEF(IEtsTraderGroupColl, IID_IEtsTraderGroupColl);

typedef IDispatchImpl<IEtsTraderGroupColl, &IID_IEtsTraderGroupColl, &LIBID_EtsGeneralLib>													IEtsTraderGroupCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsTraderGroupCollDispImpl, IEtsTraderGroupAtom, LONG, LONG, BSTR, _bstr_t  >	IEtsTraderGroupCollImpl;

// CEtsTraderGroupColl
class ATL_NO_VTABLE CEtsTraderGroupColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsTraderGroupColl, &CLSID_EtsTraderGroupColl>,
	public ISupportErrorInfoImpl<&IID_IEtsTraderGroupColl>,
	public IEtsTraderGroupCollImpl
{
public:
	CEtsTraderGroupColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSTRADERGROUPCOLL)


BEGIN_COM_MAP(CEtsTraderGroupColl)
	COM_INTERFACE_ENTRY(IEtsTraderGroupColl)
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
		IEtsTraderGroupCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IEtsTraderGroupAtomPtr AddNew(long lID, _bstr_t bsName, _bstr_t bsDescription = L"", CComObject<CEtsTraderGroupAtom>** pAtom = NULL);

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsTraderGroupAtom* Value, IEtsTraderGroupAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsTraderGroupColl), CEtsTraderGroupColl)

#endif //__ETSTRADERGROUPCOLL_H__
