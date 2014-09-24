// MmQvOptRootColl.h : Declaration of the CMmQvOptRootColl
#ifndef __MMQVOPTROOTCOLL_H_
#define __MMQVOPTROOTCOLL_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvOptRootAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvOptRootColl, IID_IMmQvOptRootColl);

typedef IDispatchImpl<IMmQvOptRootColl, &IID_IMmQvOptRootColl, &LIBID_EtsMmQuotesLib>													IMmQvOptRootCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmQvOptRootCollDispImpl, IMmQvOptRootAtom, LONG, LONG, BSTR, _bstr_t  >	IMmQvOptRootCollImpl;

// CMmQvOptRootColl
class ATL_NO_VTABLE CMmQvOptRootColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvOptRootColl, &CLSID_MmQvOptRootColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvOptRootColl>,
	public IMmQvOptRootCollImpl
{
public:
	CMmQvOptRootColl()
	{
		m_pUnkMarshaler = NULL;
	}


DECLARE_REGISTRY_RESOURCEID(IDR_MMQVOPTROOTCOLL)


BEGIN_COM_MAP(CMmQvOptRootColl)
	COM_INTERFACE_ENTRY(IMmQvOptRootColl)
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
		IMmQvOptRootCollImpl::Clear();
		m_pUnkMarshaler.Release();

	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IMmQvOptRootAtomPtr GetItem(long lItemKey);
	CMmQvOptRootAtom*   GetOptionRoot(long nOptRootID);
	IMmQvOptRootAtomPtr AddNew(long lOptRootID, _bstr_t bsRootName, CComObject<CMmQvOptRootAtom>** pRoot = NULL);

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmQvOptRootAtom* Value, IMmQvOptRootAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvOptRootColl), CMmQvOptRootColl)

#endif //__MMQVOPTROOTCOLL_H_