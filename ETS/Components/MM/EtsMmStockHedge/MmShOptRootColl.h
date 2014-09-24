// MmShOptRootColl.h : Declaration of the CMmShOptRootColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShOptRootAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShOptRootColl, IID_IMmShOptRootColl);

typedef IDispatchImpl<IMmShOptRootColl, &IID_IMmShOptRootColl, &LIBID_EtsMmStockHedgeLib>				IMmShOptRootCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmShOptRootCollDispImpl, LONG, IMmShOptRootAtom, LONG >	IMmShOptRootCollImpl;

// CMmShOptRootColl

class ATL_NO_VTABLE CMmShOptRootColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShOptRootColl, &CLSID_MmShOptRootColl>,
	public IMmShOptRootCollImpl
{
public:
	CMmShOptRootColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHOPTROOTCOLL)


BEGIN_COM_MAP(CMmShOptRootColl)
	COM_INTERFACE_ENTRY(IMmShOptRootColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmShOptRootCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmShOptRootAtom* Value, IMmShOptRootAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShOptRootColl), CMmShOptRootColl)
