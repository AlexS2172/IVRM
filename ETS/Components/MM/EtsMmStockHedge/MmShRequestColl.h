// MmShRequestColl.h : Declaration of the CMmShRequestColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"


_COM_SMARTPTR_TYPEDEF(IMmShRequestColl, __uuidof(IMmShRequestColl));

typedef IDispatchImpl<IMmShRequestColl, &IID_IMmShRequestColl, &LIBID_EtsMmStockHedgeLib>					IMmShRequestCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmShRequestCollDispImpl, BSTR, IMmShRequestAtom, _bstr_t >	IMmShRequestCollImpl;

// CMmShRequestColl

class ATL_NO_VTABLE CMmShRequestColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShRequestColl, &CLSID_MmShRequestColl>,
	public ISupportErrorInfoImpl<&IID_IMmShRequestColl>,
	public IMmShRequestCollImpl
{
public:
	CMmShRequestColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHREQUESTCOLL)


BEGIN_COM_MAP(CMmShRequestColl)
	COM_INTERFACE_ENTRY(IMmShRequestColl)
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
	}

public:

	STDMETHOD(Add)(BSTR,IMmShRequestAtom *,IMmShRequestAtom **);
	STDMETHOD(AddRequest)(BSTR Key, IMmShUndAtom* pUnd, IMmShOptAtom* pOpt, IMmShRequestAtom** pRetVal);


};

OBJECT_ENTRY_AUTO(__uuidof(MmShRequestColl), CMmShRequestColl)
