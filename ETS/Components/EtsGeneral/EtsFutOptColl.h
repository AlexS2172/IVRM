// EtsFutOptColl.h : Declaration of the CEtsFutOptColl
#ifndef __ETSFUTOPTCOLL_H__
#define __ETSFUTOPTCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsFutOptAtom.h"

typedef IDispatchImpl<IEtsFutOptColl, &IID_IEtsFutOptColl, &LIBID_EtsGeneralLib>													IEtsFutOptCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsFutOptCollDispImpl, IEtsFutOptAtom, LONG, LONG, BSTR, _bstr_t  >	IEtsFutOptCollImpl;

// CEtsFutOptColl

class ATL_NO_VTABLE CEtsFutOptColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsFutOptColl, &CLSID_EtsFutOptColl>,
	public ISupportErrorInfoImpl<&IID_IEtsFutOptColl>,
	public IEtsFutOptCollImpl
{
public:
	CEtsFutOptColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSFUTOPTCOLL)


BEGIN_COM_MAP(CEtsFutOptColl)
	COM_INTERFACE_ENTRY(IEtsFutOptColl)
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
		IEtsFutOptCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsFutOptAtom* Value, IEtsFutOptAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsFutOptColl), CEtsFutOptColl)

#endif //__ETSFUTOPTCOLL_H__
