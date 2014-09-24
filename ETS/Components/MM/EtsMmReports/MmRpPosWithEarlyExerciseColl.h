// MmRpPosWithEarlyExerciseColl.h : Declaration of the CMmRpPosWithEarlyExerciseColl
#ifndef __MM_RP_POS_WITH_EARLY_EXERCISE_COLL__
#define __MM_RP_POS_WITH_EARLY_EXERCISE_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPosWithEarlyExerciseColl, IID_IMmRpPosWithEarlyExerciseColl);

typedef IDispatchImpl<IMmRpPosWithEarlyExerciseColl, &IID_IMmRpPosWithEarlyExerciseColl, &LIBID_EtsMmReportsLib>											IMmRpPosWithEarlyExCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRpPosWithEarlyExCollDispImpl, IMmRpPosWithEarlyExerciseAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRpPosWithEarlyExCollImpl;

// CMmRpPosWithEarlyExerciseColl

class ATL_NO_VTABLE CMmRpPosWithEarlyExerciseColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPosWithEarlyExerciseColl, &CLSID_MmRpPosWithEarlyExerciseColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpPosWithEarlyExerciseColl>,
	public IMmRpPosWithEarlyExCollImpl
{
public:
	CMmRpPosWithEarlyExerciseColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPOSWITHEARLYEXERCISECOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpPosWithEarlyExerciseColl)

BEGIN_COM_MAP(CMmRpPosWithEarlyExerciseColl)
	COM_INTERFACE_ENTRY(IMmRpPosWithEarlyExerciseColl)
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
		IMmRpPosWithEarlyExCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SotKey, IMmRpPosWithEarlyExerciseAtom* Value, IMmRpPosWithEarlyExerciseAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPosWithEarlyExerciseColl), CMmRpPosWithEarlyExerciseColl)

#endif //__MM_RP_POS_WITH_EARLY_EXERCISE_COLL__