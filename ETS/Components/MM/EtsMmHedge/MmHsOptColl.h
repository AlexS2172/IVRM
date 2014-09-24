// MmHsOptColl.h : Declaration of the CMmHsOptColl

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmHedge.h"
#include "MmHsOptAtom.h"


_COM_SMARTPTR_TYPEDEF(IMmHsOptColl, IID_IMmHsOptColl);

typedef IDispatchImpl<IMmHsOptColl, &IID_IMmHsOptColl, &LIBID_EtsMmHedgeLib>	IMmHsOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmHsOptCollDispImpl, LONG, IMmHsOptAtom, LONG>	IMmHsOptCollImpl;

// CMmHsOptColl

class ATL_NO_VTABLE CMmHsOptColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmHsOptColl, &CLSID_MmHsOptColl>,
	public ISupportErrorInfoImpl<&IID_IMmHsOptColl>,
	public IMmHsOptCollImpl
{
public:
	CMmHsOptColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMHSOPTCOLL)


BEGIN_COM_MAP(CMmHsOptColl)
	COM_INTERFACE_ENTRY(IMmHsOptColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmHsOptCollImpl::Clear();
	}

public:

    STDMETHOD(Add)(LONG Key, IMmHsOptAtom* Value, IMmHsOptAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmHsOptColl), CMmHsOptColl)
