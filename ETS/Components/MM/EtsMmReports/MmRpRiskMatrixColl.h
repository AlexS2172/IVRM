// MmRpRiskMatrixColl.h : Declaration of the CMmRpRiskMatrixColl
#ifndef __MM_RP_RISK_MATRIX_COLL__
#define __MM_RP_RISK_MATRIX_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpRiskMatrixColl, IID_IMmRpRiskMatrixColl);

typedef IDispatchImpl<IMmRpRiskMatrixColl, &IID_IMmRpRiskMatrixColl, &LIBID_EtsMmReportsLib>												IMmRpRiskMatrixCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRpRiskMatrixCollDispImpl, IMmRpRiskMatrixAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRpRiskMatrixCollImpl;

// CMmRpRiskMatrixColl

class ATL_NO_VTABLE CMmRpRiskMatrixColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpRiskMatrixColl, &CLSID_MmRpRiskMatrixColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpRiskMatrixColl>,
	public IMmRpRiskMatrixCollImpl
{
public:
	CMmRpRiskMatrixColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPRISKMATRIXCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpRiskMatrixColl)

BEGIN_COM_MAP(CMmRpRiskMatrixColl)
	COM_INTERFACE_ENTRY(IMmRpRiskMatrixColl)
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
		IMmRpRiskMatrixCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRpRiskMatrixAtom* Value, IMmRpRiskMatrixAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpRiskMatrixColl), CMmRpRiskMatrixColl)

#endif //__MM_RP_RISK_MATRIX_COLL__