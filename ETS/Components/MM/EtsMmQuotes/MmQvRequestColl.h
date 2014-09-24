// MmQvRequestColl.h : Declaration of the CMmQvRequestColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"
#include "MmQvRequestAtom.h"

typedef IDispatchImpl<IMmQvRequestColl, &IID_IMmQvRequestColl, &LIBID_EtsMmQuotesLib> IMmQvRequestCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmQvRequestCollDispImpl, BSTR, IMmQvRequestAtom, _bstr_t >	IMmQvRequestCollImpl;

_COM_SMARTPTR_TYPEDEF(IMmQvRequestColl, IID_IMmQvRequestColl);


// CMmQvRequestColl

class ATL_NO_VTABLE CMmQvRequestColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvRequestColl, &CLSID_MmQvRequestColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvRequestColl>,
	public IMmQvRequestCollImpl
{
	typedef::vector<IMmQvRequestAtomPtr> CRequests;
public:
	CMmQvRequestColl()
		:m_lVisibleRequests(0)
		,m_lInvisibleRequests(0)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVREQUESTCOLL)


BEGIN_COM_MAP(CMmQvRequestColl)
	COM_INTERFACE_ENTRY(IMmQvRequestColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		IMmQvRequestCollImpl::Clear();
		m_pUnkMarshaler.Release();

	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	HRESULT Request(bool bStockOrIndex, IBatchPriceProviderPtr spProvider, bool bSubscribe, long lActiveFuturesID);

	IMmQvRequestAtomPtr  GetQuote(_bstr_t bsKey)
	{
		ObjectLock (this);
		IMmQvRequestAtomPtr spReturn;
		CollType::iterator itrFind = m_coll.find(bsKey);
		if(itrFind != m_coll.end())
			spReturn  = itrFind->second;
		return spReturn;
	}

private:
	HRESULT RequestQuotes(CRequests& requests, IBatchPriceProviderPtr spProvider, bool bSubscribe);

	long m_lVisibleRequests;
	long m_lInvisibleRequests;

public:
	IMmQvRequestAtomPtr AddNew(_bstr_t bKey, CComObject<CMmQvRequestAtom>** ppAtom = NULL);
public:

	STDMETHOD(Add)(BSTR Key, IMmQvRequestAtom* Value, IMmQvRequestAtom** pRetVal);

	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG, VisibleRequests, m_lVisibleRequests);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG, InvisibleRequests, m_lInvisibleRequests);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvRequestColl), CMmQvRequestColl)
