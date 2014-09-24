// MmRpExercisedStockColl.h : Declaration of the CMmRpExercisedStockColl
#ifndef __MM_RP_EXERCISED_STOCK_COLL__
#define __MM_RP_EXERCISED_STOCK_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpExercisedStockColl, IID_IMmRpExercisedStockColl);

typedef IDispatchImpl<IMmRpExercisedStockColl, &IID_IMmRpExercisedStockColl, &LIBID_EtsMmReportsLib>						IMmRpExercisedStockCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpExercisedStockCollDispImpl, _bstr_t, IMmRpExercisedStockAtom, _bstr_t>	IMmRpExercisedStockCollImpl;

// CMmRpExercisedStockColl

class ATL_NO_VTABLE CMmRpExercisedStockColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpExercisedStockColl, &CLSID_MmRpExercisedStockColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpExercisedStockColl>,
	public IMmRpExercisedStockCollImpl
{
public:
	CMmRpExercisedStockColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPEXERCISEDSTOCKCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpExercisedStockColl)

BEGIN_COM_MAP(CMmRpExercisedStockColl)
	COM_INTERFACE_ENTRY(IMmRpExercisedStockColl)
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
		IMmRpExercisedStockCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(BSTR Key, IMmRpExercisedStockAtom* Value, IMmRpExercisedStockAtom** pRetVal);
	STDMETHOD(get_Item)(BSTR Key, IMmRpExercisedStockAtom** pValue);
	STDMETHOD(Remove)(BSTR Key);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpExercisedStockColl), CMmRpExercisedStockColl)

#endif //__MM_RP_EXERCISED_STOCK_COLL__