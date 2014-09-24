// MmRvExpTotalColl.h : Declaration of the CMmRvExpTotalColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvExpTotalAtom.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IMmRvExpTotalColl, __uuidof(IMmRvExpTotalColl));

typedef IDispatchImpl<IMmRvExpTotalColl, &__uuidof(IMmRvExpTotalColl), &LIBID_EtsMmRisksLib>				IMmRvExpTotalCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRvExpTotalCollDispImpl, DATE, IMmRvExpTotalAtom, DATE >	IMmRvExpTotalCollImpl;


// CMmRvExpTotalColl

class ATL_NO_VTABLE CMmRvExpTotalColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvExpTotalColl, &CLSID_MmRvExpTotalColl>,
	public ISupportErrorInfoImpl<&__uuidof(IMmRvExpTotalColl)>,
	public IMmRvExpTotalCollImpl
{
public:
	CMmRvExpTotalColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVEXPTOTALCOLL)


BEGIN_COM_MAP(CMmRvExpTotalColl)
	COM_INTERFACE_ENTRY(IMmRvExpTotalColl)
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
		IMmRvExpTotalCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;
public:
	IMmRvExpTotalAtomPtr AddNew(DATE dtExpiry, CComObject<CMmRvExpTotalAtom>** pObject = NULL);
	void Calc();

	IMmRvExpTotalAtomPtr GetExpiration(DATE dtExpiry)
	{
		IMmRvExpTotalAtomPtr spResult;
		CollType::iterator itr = m_coll.find(dtExpiry);
		if(itr != m_coll.end())
			spResult = itr->second;
		return spResult;
	}

public:
	STDMETHOD(Add)(DATE Key, IMmRvExpTotalAtom* Value, IMmRvExpTotalAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvExpTotalColl), CMmRvExpTotalColl)
