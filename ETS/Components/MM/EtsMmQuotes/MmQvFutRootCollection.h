// MmQvFutRootCollection.h : Declaration of the CMmQvFutRootCollection

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"
#include "MmQvFutRootAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvFutRootCollection, IID_IMmQvFutRootCollection);

typedef IDispatchImpl<IMmQvFutRootCollection, &IID_IMmQvFutRootCollection, &LIBID_EtsMmQuotesLib>				IMmQvFutRootCollectionDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmQvFutRootCollectionDispImpl, LONG, IMmQvFutRootAtom, LONG >	IMmQvFutRootCollectionImpl;


// CMmQvFutRootCollection

class ATL_NO_VTABLE CMmQvFutRootCollection :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvFutRootCollection, &CLSID_MmQvFutRootCollection>,
	public ISupportErrorInfoImpl<&__uuidof(IMmQvFutRootCollection)>,
	public IMmQvFutRootCollectionImpl
{
public:
	CMmQvFutRootCollection()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVFUTROOTCOLLECTION)


BEGIN_COM_MAP(CMmQvFutRootCollection)
	COM_INTERFACE_ENTRY(IMmQvFutRootCollection)
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
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;
	size_t GetItemsCount(){return m_coll.size(); }

public:
	IMmQvFutRootAtomPtr  GetFuturesRoot(LONG nID)
	{
		IMmQvFutRootAtomPtr spResult;
		CollType::iterator itr =  m_coll.find(nID);
		if(itr != m_coll.end())
			spResult = itr->second;
		return spResult;
	}

	IMmQvFutRootAtomPtr AddNew(long lRootID,  CComObject<CMmQvFutRootAtom>** ppFutRoot = NULL);
	STDMETHOD(Add)(LONG Key, IMmQvFutRootAtom* Value, IMmQvFutRootAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmQvFutRootCollection), CMmQvFutRootCollection)
