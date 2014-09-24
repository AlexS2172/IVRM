// MmRpOptRootColl.h : Declaration of the CMmRpOptRootColl
#ifndef __MM_RP_OPTROOT_COLL__
#define __MM_RP_OPTROOT_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpOptRootColl, IID_IMmRpOptRootColl);

typedef IDispatchImpl<IMmRpOptRootColl, &IID_IMmRpOptRootColl, &LIBID_EtsMmReportsLib>					IMmRpOptRootCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpOptRootCollDispImpl, LONG, IMmRpOptRootAtom, LONG>	IMmRpOptRootCollImpl;

// CMmRpOptRootColl

class ATL_NO_VTABLE CMmRpOptRootColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpOptRootColl, &CLSID_MmRpOptRootColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpOptRootColl>,
	public IMmRpOptRootCollImpl
{
public:
	CMmRpOptRootColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPOPTROOTCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpOptRootColl)

BEGIN_COM_MAP(CMmRpOptRootColl)
	COM_INTERFACE_ENTRY(IMmRpOptRootColl)
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
		IMmRpOptRootCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRpOptRootAtom* Value, IMmRpOptRootAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpOptRootColl), CMmRpOptRootColl)

#endif //__MM_RP_OPTROOT_COLL__