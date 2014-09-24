// MmShOptColl.h : Declaration of the CMmShOptColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShOptAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShOptColl, IID_IMmShOptColl);

typedef IDispatchImpl<IMmShOptColl, &IID_IMmShOptColl, &LIBID_EtsMmStockHedgeLib>				IMmShOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmShOptCollDispImpl, LONG, IMmShOptAtom, LONG >	IMmShOptCollImpl;

// CMmShOptColl

class ATL_NO_VTABLE CMmShOptColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShOptColl, &CLSID_MmShOptColl>,
	public IMmShOptCollImpl
{
public:
	CMmShOptColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHOPTCOLL)


BEGIN_COM_MAP(CMmShOptColl)
	COM_INTERFACE_ENTRY(IMmShOptColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmShOptCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, IMmShOptAtom* Value, IMmShOptAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShOptColl), CMmShOptColl)
