// MmHsUndColl.h : Declaration of the CMmHsUndColl

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmHedge.h"
#include "MmHsUndAtom.h"


_COM_SMARTPTR_TYPEDEF(IMmHsUndColl, IID_IMmHsUndColl);

typedef IDispatchImpl<IMmHsUndColl, &IID_IMmHsUndColl, &LIBID_EtsMmHedgeLib>	IMmHsUndCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmHsUndCollDispImpl, LONG, IMmHsUndAtom, LONG > IMmHsUndCollImpl;

// CMmHsUndColl

class ATL_NO_VTABLE CMmHsUndColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmHsUndColl, &CLSID_MmHsUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmHsUndColl>,
	public IMmHsUndCollImpl
{
public:
	CMmHsUndColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMHSUNDCOLL)


BEGIN_COM_MAP(CMmHsUndColl)
	COM_INTERFACE_ENTRY(IMmHsUndColl)
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
		IMmHsUndCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmHsUndAtom* Value, IMmHsUndAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmHsUndColl), CMmHsUndColl)
