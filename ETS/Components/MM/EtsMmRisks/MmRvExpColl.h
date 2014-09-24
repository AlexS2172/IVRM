// MmRvExpColl.h : Declaration of the CMmRvExpColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvExpAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvExpColl, IID_IMmRvExpColl);

typedef IDispatchImpl<IMmRvExpColl, &IID_IMmRvExpColl, &LIBID_EtsMmRisksLib>						IMmRvExpCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRvExpCollDispImpl, DATE, IMmRvExpAtom, DATE >		IMmRvExpCollImpl;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CMmRvExpColl

class ATL_NO_VTABLE CMmRvExpColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvExpColl, &CLSID_MmRvExpColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvExpColl>,
	public IMmRvExpCollImpl
{
public:
	CMmRvExpColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVEXPCOLL)


BEGIN_COM_MAP(CMmRvExpColl)
	COM_INTERFACE_ENTRY(IMmRvExpColl)
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
		IMmRvExpCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

	IMmRvExpAtomPtr GetExpiration(DATE dtExpiry)
	{
		IMmRvExpAtomPtr spResult;
		CollType::iterator itr = m_coll.find(dtExpiry);
		if(itr != m_coll.end())
			spResult = itr->second;
		return spResult;
	}

public:
	IMmRvExpAtomPtr AddNew(DATE dtExpiry, CComObject<CMmRvExpAtom>** pObject = NULL);

public:

	STDMETHOD(Add)(DATE Key, IMmRvExpAtom* Value, IMmRvExpAtom** pRetVal);


};

OBJECT_ENTRY_AUTO(__uuidof(MmRvExpColl), CMmRvExpColl)
