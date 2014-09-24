// MmIhOptColl.h : Declaration of the CMmIhOptColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmIndexHedge.h"
#include "MmIhOptAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmIhOptColl, IID_IMmIhOptColl);

typedef IDispatchImpl<IMmIhOptColl, &IID_IMmIhOptColl, &LIBID_EtsMmIndexHedgeLib>				IMmIhOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmIhOptCollDispImpl, LONG, IMmIhOptAtom, LONG >	IMmIhOptCollImpl;

// CMmIhOptColl
class ATL_NO_VTABLE CMmIhOptColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmIhOptColl, &CLSID_MmIhOptColl>,
	public IMmIhOptCollImpl
{
public:
	CMmIhOptColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMIHOPTCOLL)


BEGIN_COM_MAP(CMmIhOptColl)
	COM_INTERFACE_ENTRY(IMmIhOptColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmIhOptCollImpl::Clear();
}

public:

	STDMETHOD(Add)(LONG Key, IMmIhOptAtom* Value, IMmIhOptAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmIhOptColl), CMmIhOptColl)
