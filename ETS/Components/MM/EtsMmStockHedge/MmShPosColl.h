// MmShPosColl.h : Declaration of the CMmShPosColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShPosAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShPosColl, IID_IMmShPosColl);

typedef IDispatchImpl<IMmShPosColl, &IID_IMmShPosColl, &LIBID_EtsMmStockHedgeLib>				IMmShPosCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmShPosCollDispImpl, LONG, IMmShPosAtom, LONG >	IMmShPosCollImpl;

// CMmShPosColl

class ATL_NO_VTABLE CMmShPosColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShPosColl, &CLSID_MmShPosColl>,
	public IMmShPosCollImpl
{
public:
	CMmShPosColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHPOSCOLL)


BEGIN_COM_MAP(CMmShPosColl)
	COM_INTERFACE_ENTRY(IMmShPosColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmShPosCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, IMmShPosAtom* Value, IMmShPosAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShPosColl), CMmShPosColl)
