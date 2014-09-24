// MmRpOptColl.h : Declaration of the CMmRpOptColl
#ifndef __MM_RP_OPT_COLL__
#define __MM_RP_OPT_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpOptColl, IID_IMmRpOptColl);

typedef IDispatchImpl<IMmRpOptColl, &IID_IMmRpOptColl, &LIBID_EtsMmReportsLib>					IMmRpOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpOptCollDispImpl, LONG, IMmRpOptAtom, LONG>	IMmRpOptCollImpl;

// CMmRpOptColl

class ATL_NO_VTABLE CMmRpOptColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpOptColl, &CLSID_MmRpOptColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpOptColl>,
	public IMmRpOptCollImpl
{
public:
	CMmRpOptColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPOPTCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpOptColl)

BEGIN_COM_MAP(CMmRpOptColl)
	COM_INTERFACE_ENTRY(IMmRpOptColl)
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
		IMmRpOptCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRpOptAtom* Value, IMmRpOptAtom** pRetVal);
	STDMETHOD(GetPriceProperty_)(BSTR Symbol , IMMRpPrice** PPrice) ;
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpOptColl), CMmRpOptColl)

#endif //__MM_RP_OPT_COLL__