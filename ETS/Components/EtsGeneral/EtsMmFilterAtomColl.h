// EtsMmFilterAtomColl.h : Declaration of the CEtsMmFilterAtomColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IEtsMmFilterAtomColl, IID_IEtsMmFilterAtomColl);

typedef IDispatchImpl<IEtsMmFilterAtomColl, &IID_IEtsMmFilterAtomColl, &LIBID_EtsGeneralLib> IEtsMmFilterAtomCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IEtsMmFilterAtomCollDispImpl, BSTR, IEtsMmFilterAtom, _bstr_t >	IEtsMmFilterAtomCollImpl;



// CEtsMmFilterAtomColl

class ATL_NO_VTABLE CEtsMmFilterAtomColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsMmFilterAtomColl, &CLSID_EtsMmFilterAtomColl>,
	public ISupportErrorInfoImpl<&IID_IEtsMmFilterAtomColl>,
	public IEtsMmFilterAtomCollImpl
{
public:
	CEtsMmFilterAtomColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSMMFILTERATOMCOLL)


BEGIN_COM_MAP(CEtsMmFilterAtomColl)
	COM_INTERFACE_ENTRY(IEtsMmFilterAtomColl)
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

	_bstr_t     m_bsName;

public:

	STDMETHOD(Add)(BSTR Key, IEtsMmFilterAtom* Value, IEtsMmFilterAtom** pRetVal);

	IMPLEMENT_BSTRT_PROPERTY(Name, m_bsName);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsMmFilterAtomColl), CEtsMmFilterAtomColl)
