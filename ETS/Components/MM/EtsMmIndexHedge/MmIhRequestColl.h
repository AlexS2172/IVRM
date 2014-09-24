// MmIhRequestColl.h : Declaration of the CMmIhRequestColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmIndexHedge.h"


_COM_SMARTPTR_TYPEDEF(IMmIhRequestColl, __uuidof(IMmIhRequestColl));

typedef IDispatchImpl<IMmIhRequestColl, &IID_IMmIhRequestColl, &LIBID_EtsMmIndexHedgeLib>					IMmIhRequestCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmIhRequestCollDispImpl, BSTR, IMmIhRequestAtom, _bstr_t >	IMmIhRequestCollImpl;

// CMmIhRequestColl

class ATL_NO_VTABLE CMmIhRequestColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmIhRequestColl, &CLSID_MmIhRequestColl>,
	public ISupportErrorInfoImpl<&IID_IMmIhRequestColl>,
	public IMmIhRequestCollImpl
{
public:
	CMmIhRequestColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMIHREQUESTCOLL)


BEGIN_COM_MAP(CMmIhRequestColl)
	COM_INTERFACE_ENTRY(IMmIhRequestColl)
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

	STDMETHOD(Add) (BSTR,IMmIhRequestAtom *,IMmIhRequestAtom ** );
	STDMETHOD(AddRequest)(BSTR Key, IMmIhUndAtom* pUnd, IMmIhIndexAtom* pIdx, IMmIhOptAtom* pOpt, IMmIhRequestAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmIhRequestColl), CMmIhRequestColl)
