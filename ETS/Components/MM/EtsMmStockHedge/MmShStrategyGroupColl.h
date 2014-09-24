// MmShStrategyGroupColl.h : Declaration of the CMmShStrategyGroupColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShStrategyGroupAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShStrategyGroupColl, IID_IMmShStrategyGroupColl);

typedef IDispatchImpl<IMmShStrategyGroupColl, &IID_IMmShStrategyGroupColl, &LIBID_EtsMmStockHedgeLib>												IMmShStrategyGroupCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmShStrategyGroupCollDispImpl, IMmShStrategyGroupAtom, LONG, LONG, BSTR, _bstr_t  >	IMmShStrategyGroupCollImpl;

// CMmShStrategyGroupColl

class ATL_NO_VTABLE CMmShStrategyGroupColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShStrategyGroupColl, &CLSID_MmShStrategyGroupColl>,
	public IMmShStrategyGroupCollImpl
{
public:
	CMmShStrategyGroupColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHSTRATEGYGROUPCOLL)


BEGIN_COM_MAP(CMmShStrategyGroupColl)
	COM_INTERFACE_ENTRY(IMmShStrategyGroupColl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmShStrategyGroupCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmShStrategyGroupAtom* Value, IMmShStrategyGroupAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShStrategyGroupColl), CMmShStrategyGroupColl)
