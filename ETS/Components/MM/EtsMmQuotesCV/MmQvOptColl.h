// MmQvOptColl.h : Declaration of the CMmQvOptColl
#ifndef __MMQVOPTCOLL_H__
#define __MMQVOPTCOLL_H__

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvOptAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvOptColl, IID_IMmQvOptColl);

typedef IDispatchImpl<IMmQvOptColl, &IID_IMmQvOptColl, &LIBID_EtsMmQuotesLib>						IMmQvOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmQvOptCollDispImpl, LONG, IMmQvOptAtom, LONG >		IMmQvOptCollImpl;

// CMmQvOptColl

class ATL_NO_VTABLE CMmQvOptColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvOptColl, &CLSID_MmQvOptColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvOptColl>,
	public IMmQvOptCollImpl
{
public:
	CMmQvOptColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVOPTCOLL)


BEGIN_COM_MAP(CMmQvOptColl)
	COM_INTERFACE_ENTRY(IMmQvOptColl)
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
		IMmQvOptCollImpl::Clear();
		m_pUnkMarshaler.Release();

	}

	size_t GetItemsCount(){return m_coll.size(); }

private:
	CComPtr<IUnknown> m_pUnkMarshaler;
public:

	STDMETHOD(Add)(LONG Key, IMmQvOptAtom* Value, IMmQvOptAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvOptColl), CMmQvOptColl)

#endif //__MMQVOPTCOLL_H__