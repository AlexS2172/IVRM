// MmRpPosWithEarlyExerciseOptColl.h : Declaration of the CMmRpPosWithEarlyExerciseOptColl
#ifndef __MM_RP_POS_WITH_EARLY_EXERCISE_OPT_COLL__
#define __MM_RP_POS_WITH_EARLY_EXERCISE_OPT_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPosWithEarlyExerciseOptColl, IID_IMmRpPosWithEarlyExerciseOptColl);

typedef IDispatchImpl<IMmRpPosWithEarlyExerciseOptColl, &IID_IMmRpPosWithEarlyExerciseOptColl, &LIBID_EtsMmReportsLib>					IMmRpPosWithEarlyExOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpPosWithEarlyExOptCollDispImpl, _bstr_t, IMmRpPosWithEarlyExerciseOptAtom, _bstr_t>	IMmRpPosWithEarlyExOptCollImpl;

// CMmRpPosWithEarlyExerciseOptColl

class ATL_NO_VTABLE CMmRpPosWithEarlyExerciseOptColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPosWithEarlyExerciseOptColl, &CLSID_MmRpPosWithEarlyExerciseOptColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpPosWithEarlyExerciseOptColl>,
	public IMmRpPosWithEarlyExOptCollImpl
{
public:
	CMmRpPosWithEarlyExerciseOptColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPOSWITHEARLYEXERCISEOPTCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpPosWithEarlyExerciseOptColl)

BEGIN_COM_MAP(CMmRpPosWithEarlyExerciseOptColl)
	COM_INTERFACE_ENTRY(IMmRpPosWithEarlyExerciseOptColl)
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
		IMmRpPosWithEarlyExOptCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG nRootID, DOUBLE dStrike, DATE dtExpiry, IMmRpPosWithEarlyExerciseOptAtom* Value, IMmRpPosWithEarlyExerciseOptAtom** pRetVal);
	STDMETHOD(get_Item)(LONG nRootID, DOUBLE dStrike, DATE dtExpiry, IMmRpPosWithEarlyExerciseOptAtom** pValue);
	STDMETHOD(Remove)(LONG nRootID, DOUBLE dStrike, DATE dtExpiry);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPosWithEarlyExerciseOptColl), CMmRpPosWithEarlyExerciseOptColl)

#endif //__MM_RP_POS_WITH_EARLY_EXERCISE_OPT_COLL__