// EtsBrokerColl.h : Declaration of the CEtsBrokerColl
#ifndef __ETSBROKERCOLL_H__
#define __ETSBROKERCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsBrokerAtom.h"

_COM_SMARTPTR_TYPEDEF(IEtsBrokerColl, IID_IEtsBrokerColl);

typedef IDispatchImpl<IEtsBrokerColl, &IID_IEtsBrokerColl, &LIBID_EtsGeneralLib>													IEtsBrokerCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsBrokerCollDispImpl, IEtsBrokerAtom, LONG, LONG, BSTR, _bstr_t >	IEtsBrokerCollImpl;

// CEtsBrokerColl
class ATL_NO_VTABLE CEtsBrokerColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsBrokerColl, &CLSID_EtsBrokerColl>,
	public ISupportErrorInfoImpl<&IID_IEtsBrokerColl>,
	public IEtsBrokerCollImpl
{
public:
	CEtsBrokerColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSBROKERCOLL)


BEGIN_COM_MAP(CEtsBrokerColl)
	COM_INTERFACE_ENTRY(IEtsBrokerColl)
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
		IEtsBrokerCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IEtsBrokerAtomPtr AddNew(long lID, _bstr_t bsName, _bstr_t bsDescription = L"", CComObject<CEtsBrokerAtom>** pAtom = NULL);
public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsBrokerAtom* Value, IEtsBrokerAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsBrokerColl), CEtsBrokerColl)

#endif //__ETSBROKERCOLL_H__
