// MmIhUndColl.h : Declaration of the CMmIhUndColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmIndexHedge.h"
#include "MmIhUndAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmIhUndColl, IID_IMmIhUndColl);

typedef IDispatchImpl<IMmIhUndColl, &IID_IMmIhUndColl, &LIBID_EtsMmIndexHedgeLib>												IMmIhUndCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmIhUndCollDispImpl, IMmIhUndAtom, LONG, LONG, BSTR, _bstr_t  >	IMmIhUndCollImpl;

// CMmIhUndColl

class ATL_NO_VTABLE CMmIhUndColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmIhUndColl, &CLSID_MmIhUndColl>,
	public IMmIhUndCollImpl
{
public:
	CMmIhUndColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMIHUNDCOLL)


BEGIN_COM_MAP(CMmIhUndColl)
	COM_INTERFACE_ENTRY(IMmIhUndColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmIhUndCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmIhUndAtom* Value, IMmIhUndAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmIhUndColl), CMmIhUndColl)
