// MmShUndGroupColl.h : Declaration of the CMmShUndGroupColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShUndGroupAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShUndGroupColl, IID_IMmShUndGroupColl);

typedef IDispatchImpl<IMmShUndGroupColl, &IID_IMmShUndGroupColl, &LIBID_EtsMmStockHedgeLib>												IMmShUndGroupCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmShUndGroupCollDispImpl, IMmShUndGroupAtom, LONG, LONG,BSTR, _bstr_t >	IMmShUndGroupCollImpl;

// CMmShUndGroupColl

class ATL_NO_VTABLE CMmShUndGroupColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShUndGroupColl, &CLSID_MmShUndGroupColl>,
	public IMmShUndGroupCollImpl
{
public:
	CMmShUndGroupColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHUNDGROUPCOLL)


BEGIN_COM_MAP(CMmShUndGroupColl)
	COM_INTERFACE_ENTRY(IMmShUndGroupColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmShUndGroupCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmShUndGroupAtom* Value, IMmShUndGroupAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShUndGroupColl), CMmShUndGroupColl)
