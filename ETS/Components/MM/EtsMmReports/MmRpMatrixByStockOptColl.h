// MmRpMatrixByStockOptColl.h : Declaration of the CMmRpMatrixByStockOptColl
#ifndef __MM_RP_MATRIX_BY_STOCKOPT_COLL__
#define __MM_RP_MATRIX_BY_STOCKOPT_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpMatrixByStockOptColl, IID_IMmRpMatrixByStockOptColl);

typedef IDispatchImpl<IMmRpMatrixByStockOptColl, &IID_IMmRpMatrixByStockOptColl, &LIBID_EtsMmReportsLib>				IMmRpMatrixByStockOptCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpMatrixByStockOptCollDispImpl, LONG, IMmRpMatrixByStockOptAtom, LONG>	IMmRpMatrixByStockOptCollImpl;

// CMmRpMatrixByStockOptColl

class ATL_NO_VTABLE CMmRpMatrixByStockOptColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpMatrixByStockOptColl, &CLSID_MmRpMatrixByStockOptColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpMatrixByStockOptColl>,
	public IMmRpMatrixByStockOptCollImpl
{
public:
	CMmRpMatrixByStockOptColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPMATRIXBYSTOCKOPTCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpMatrixByStockOptColl)

BEGIN_COM_MAP(CMmRpMatrixByStockOptColl)
	COM_INTERFACE_ENTRY(IMmRpMatrixByStockOptColl)
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
		IMmRpMatrixByStockOptCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRpMatrixByStockOptAtom* Value, IMmRpMatrixByStockOptAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpMatrixByStockOptColl), CMmRpMatrixByStockOptColl)

#endif //__MM_RP_MATRIX_BY_STOCKOPT_COLL__