// MmShTraderGroupColl.h : Declaration of the CMmShTraderGroupColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShTraderGroupAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShTraderGroupColl, IID_IMmShTraderGroupColl);

typedef IDispatchImpl<IMmShTraderGroupColl, &IID_IMmShTraderGroupColl, &LIBID_EtsMmStockHedgeLib>												IMmShTraderGroupCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmShTraderGroupCollDispImpl, IMmShTraderGroupAtom, LONG, LONG, BSTR, _bstr_t >	IMmShTraderGroupCollImpl;

// CMmShTraderGroupColl

class ATL_NO_VTABLE CMmShTraderGroupColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShTraderGroupColl, &CLSID_MmShTraderGroupColl>,
	public IMmShTraderGroupCollImpl
{
public:
	CMmShTraderGroupColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHTRADERGROUPCOLL)


BEGIN_COM_MAP(CMmShTraderGroupColl)
	COM_INTERFACE_ENTRY(IMmShTraderGroupColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmShTraderGroupCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmShTraderGroupAtom* Value, IMmShTraderGroupAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShTraderGroupColl), CMmShTraderGroupColl)
