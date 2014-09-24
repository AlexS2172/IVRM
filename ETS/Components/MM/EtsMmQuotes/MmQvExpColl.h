// MmQvExpColl.h : Declaration of the CMmQvExpColl
#ifndef __MMQVEXPCOLL_H__
#define __MMQVEXPCOLL_H__

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvExpAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvExpColl, IID_IMmQvExpColl);

typedef IDispatchImpl<IMmQvExpColl, &IID_IMmQvExpColl, &LIBID_EtsMmQuotesLib>						IMmQvExpCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmQvExpCollDispImpl, DATE, IMmQvExpAtom, DATE >		IMmQvExpCollImpl;

// CMmQvExpColl

class ATL_NO_VTABLE CMmQvExpColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvExpColl, &CLSID_MmQvExpColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvExpColl>,
	public IMmQvExpCollImpl
{
public:
	CMmQvExpColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVEXPCOLL)


BEGIN_COM_MAP(CMmQvExpColl)
	COM_INTERFACE_ENTRY(IMmQvExpColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		IMmQvExpCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	IMmQvExpAtomPtr GetExpiry(DATE dtExpiration);
private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(DATE Key, IMmQvExpAtom* Value, IMmQvExpAtom** pRetVal);
	
public:	
	IMmQvExpAtomPtr GetItem(DATE Key);

	IMmQvExpAtomPtr AddNew(DATE Key, CComObject<CMmQvExpAtom>** pAtom = NULL);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvExpColl), CMmQvExpColl)

#endif //__MMQVEXPCOLL_H__