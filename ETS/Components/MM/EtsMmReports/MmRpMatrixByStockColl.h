// MmRpMatrixByStockColl.h : Declaration of the CMmRpMatrixByStockColl
#ifndef __MM_RP_MATRIX_BY_STOCK_COLL__
#define __MM_RP_MATRIX_BY_STOCK_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpMatrixByStockColl, IID_IMmRpMatrixByStockColl);

typedef IDispatchImpl<IMmRpMatrixByStockColl, &IID_IMmRpMatrixByStockColl, &LIBID_EtsMmReportsLib>													IMmRpMatrixByStockCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRpMatrixByStockCollDispImpl, IMmRpMatrixByStockAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRpMatrixByStockCollImpl;

// CMmRpMatrixByStockColl

class ATL_NO_VTABLE CMmRpMatrixByStockColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpMatrixByStockColl, &CLSID_MmRpMatrixByStockColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpMatrixByStockColl>,
	public IMmRpMatrixByStockCollImpl
{
public:
	CMmRpMatrixByStockColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPMATRIXBYSTOCKCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpMatrixByStockColl)

BEGIN_COM_MAP(CMmRpMatrixByStockColl)
	COM_INTERFACE_ENTRY(IMmRpMatrixByStockColl)
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
		IMmRpMatrixByStockCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRpMatrixByStockAtom* Value, IMmRpMatrixByStockAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpMatrixByStockColl), CMmRpMatrixByStockColl)

#endif //__MM_RP_MATRIX_BY_STOCK_COLL__