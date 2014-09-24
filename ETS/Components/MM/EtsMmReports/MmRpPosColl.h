// MmRpPosColl.h : Declaration of the CMmRpPosColl
#ifndef __MM_RP_POS_COLL__
#define __MM_RP_POS_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPosColl, IID_IMmRpPosColl);

typedef IDispatchImpl<IMmRpPosColl, &IID_IMmRpPosColl, &LIBID_EtsMmReportsLib>					IMmRpPosCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpPosCollDispImpl, LONG, IMmRpPosAtom, LONG>	IMmRpPosCollImpl;

// CMmRpPosColl

class ATL_NO_VTABLE CMmRpPosColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPosColl, &CLSID_MmRpPosColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpPosColl>,
	public IMmRpPosCollImpl
{
public:
	CMmRpPosColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPOSCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpPosColl)

BEGIN_COM_MAP(CMmRpPosColl)
	COM_INTERFACE_ENTRY(IMmRpPosColl)
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
		IMmRpPosCollImpl::Clear();
	}

public:

    STDMETHOD(Add)(LONG Key, IMmRpPosAtom* Value, IMmRpPosAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPosColl), CMmRpPosColl)

#endif //__MM_RP_POS_COLL__