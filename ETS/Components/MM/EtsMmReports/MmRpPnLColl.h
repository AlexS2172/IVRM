// MmRpPnLColl.h : Declaration of the CMmRpPnLColl
#ifndef __MM_RP_PNL_COLL__
#define __MM_RP_PNL_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPnLColl, IID_IMmRpPnLColl);

typedef IDispatchImpl<IMmRpPnLColl, &IID_IMmRpPnLColl, &LIBID_EtsMmReportsLib>					IMmRpPnLCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpPnLCollDispImpl, LONG, IMmRpPnLAtom, LONG>	IMmRpPnLCollImpl;

// CMmRpPnLColl

class ATL_NO_VTABLE CMmRpPnLColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPnLColl, &CLSID_MmRpPnLColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpPnLColl>,
	public IMmRpPnLCollImpl
{
public:
	CMmRpPnLColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPNLCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpPnLColl)

BEGIN_COM_MAP(CMmRpPnLColl)
	COM_INTERFACE_ENTRY(IMmRpPnLColl)
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
		IMmRpPnLCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRpPnLAtom* Value, IMmRpPnLAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPnLColl), CMmRpPnLColl)

#endif //__MM_RP_PNL_COLL__