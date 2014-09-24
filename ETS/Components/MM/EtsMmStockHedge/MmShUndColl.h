// MmShUndColl.h : Declaration of the CMmShUndColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShUndAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShUndColl, IID_IMmShUndColl);

typedef IDispatchImpl<IMmShUndColl, &IID_IMmShUndColl, &LIBID_EtsMmStockHedgeLib>				IMmShUndCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmShUndCollDispImpl, LONG, IMmShUndAtom, LONG >	IMmShUndCollImpl;

// CMmShUndColl

class ATL_NO_VTABLE CMmShUndColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShUndColl, &CLSID_MmShUndColl>,
	public IMmShUndCollImpl
{
public:
	CMmShUndColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHUNDCOLL)


BEGIN_COM_MAP(CMmShUndColl)
	COM_INTERFACE_ENTRY(IMmShUndColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmShUndCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, IMmShUndAtom* Value, IMmShUndAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShUndColl), CMmShUndColl)
