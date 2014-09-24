// MmQvFutCollection.h : Declaration of the CMmQvFutCollection
#pragma once
#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvFutAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvFutCollection, IID_IMmQvFutCollection);

typedef IDispatchImpl<IMmQvFutCollection, &IID_IMmQvFutCollection, &LIBID_EtsMmQuotesLib>						IMmQvFutCollectionDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmQvFutCollectionDispImpl, LONG, IMmQvFutAtom, LONG >		    IMmQvFutCollectionImpl;

// CMmQvFutCollection

class ATL_NO_VTABLE CMmQvFutCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmQvFutCollection, &CLSID_MmQvFutCollection>,
	public ISupportErrorInfoImpl<&IID_IMmQvFutCollection>,
	public IMmQvFutCollectionImpl
{
public:
	CMmQvFutCollection()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVFUTCOLLECTION)


BEGIN_COM_MAP(CMmQvFutCollection)
	COM_INTERFACE_ENTRY(IMmQvFutCollection)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmQvFutCollectionImpl::Clear();
	}

	size_t GetItemsCount(){return m_coll.size(); }
	IMmQvFutAtomPtr AddNew(LONG lId, CComObject<CMmQvFutAtom>** ppAtom =NULL);

	IMmQvFutAtomPtr GetFutures(long lID)
	{
		IMmQvFutAtomPtr spResult;
		CollType::iterator itr = m_coll.find(lID);
		if(itr!=m_coll.end())
			spResult = itr->second;

		return spResult;
	}

public:

	STDMETHOD(Add)(LONG Key, IMmQvFutAtom* Value, IMmQvFutAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvFutCollection), CMmQvFutCollection)

