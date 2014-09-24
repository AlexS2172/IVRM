// MmRpSyntheticOptColl.h : Declaration of the CMmRpSyntheticOptColl
#ifndef __MM_RP_SYNTHETICOPT_COLL__
#define __MM_RP_SYNTHETICOPT_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpSyntheticOptColl, IID_IMmRpSyntheticOptColl);

typedef IDispatchImpl<IMmRpSyntheticOptColl, &IID_IMmRpSyntheticOptColl, &LIBID_EtsMmReportsLib>						IMmRpSyntheticOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpSyntheticOptCollDispImpl, LONG, IMmRpSyntheticOptAtom, LONG>	IMmRpSyntheticOptCollImpl;

// CMmRpSyntheticOptColl

class ATL_NO_VTABLE CMmRpSyntheticOptColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpSyntheticOptColl, &CLSID_MmRpSyntheticOptColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpSyntheticOptColl>,
	public IMmRpSyntheticOptCollImpl
{
public:
	CMmRpSyntheticOptColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPSYNTHETICOPTCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpSyntheticOptColl)

BEGIN_COM_MAP(CMmRpSyntheticOptColl)
	COM_INTERFACE_ENTRY(IMmRpSyntheticOptColl)
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
		IMmRpSyntheticOptCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRpSyntheticOptAtom* Value, IMmRpSyntheticOptAtom** pRetVal);
	STDMETHOD(Append)(IMmRpSyntheticOptColl* pValue, LONG RootID);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpSyntheticOptColl), CMmRpSyntheticOptColl)

#endif //__MM_RP_SYNTHETICOPT_COLL__