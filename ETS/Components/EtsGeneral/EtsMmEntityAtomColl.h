// EtsMmEntityAtomColl.h : Declaration of the CEtsMmEntityAtomColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"


// CEtsMmEntityAtomColl
_COM_SMARTPTR_TYPEDEF(IEtsMmEntityAtomColl, IID_IEtsMmEntityAtomColl);

typedef IDispatchImpl<IEtsMmEntityAtomColl, &IID_IEtsMmEntityAtomColl, &LIBID_EtsGeneralLib> IEtsMmEntityAtomCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IEtsMmEntityAtomCollDispImpl, BSTR, IEtsMmEntityAtom, _bstr_t >	IEtsMmEntityAtomCollImpl;



class ATL_NO_VTABLE CEtsMmEntityAtomColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsMmEntityAtomColl, &CLSID_EtsMmEntityAtomColl>,
	public ISupportErrorInfoImpl<&IID_IEtsMmEntityAtomColl>,
	public IEtsMmEntityAtomCollImpl
{
public:
	CEtsMmEntityAtomColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSMMENTITYATOMCOLL)


BEGIN_COM_MAP(CEtsMmEntityAtomColl)
	COM_INTERFACE_ENTRY(IEtsMmEntityAtomColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

// ISupportsErrorInfo

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

public:

	STDMETHOD(Add)(BSTR Key, IEtsMmEntityAtom* Value, IEtsMmEntityAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(EtsMmEntityAtomColl), CEtsMmEntityAtomColl)
