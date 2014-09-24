// MmRpRiskMatrixShiftColl.h : Declaration of the CMmRpRiskMatrixShiftColl
#ifndef __MM_RP_RISK_MATRIX_SHIFT_COLL__
#define __MM_RP_RISK_MATRIX_SHIFT_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpRiskMatrixShiftColl, IID_IMmRpRiskMatrixShiftColl);

typedef IDispatchImpl<IMmRpRiskMatrixShiftColl, &IID_IMmRpRiskMatrixShiftColl, &LIBID_EtsMmReportsLib>						IMmRpRiskMatrixShiftCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpRiskMatrixShiftCollDispImpl, _bstr_t, IMmRpRiskMatrixShiftAtom, _bstr_t>	IMmRpRiskMatrixShiftCollImpl;

// CMmRpRiskMatrixShiftColl

class ATL_NO_VTABLE CMmRpRiskMatrixShiftColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpRiskMatrixShiftColl, &CLSID_MmRpRiskMatrixShiftColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpRiskMatrixShiftColl>,
	public IMmRpRiskMatrixShiftCollImpl
{
public:
	CMmRpRiskMatrixShiftColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPRISKMATRIXSHIFTCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpRiskMatrixShiftColl)

BEGIN_COM_MAP(CMmRpRiskMatrixShiftColl)
	COM_INTERFACE_ENTRY(IMmRpRiskMatrixShiftColl)
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
		IMmRpRiskMatrixShiftCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(BSTR Key, IMmRpRiskMatrixShiftAtom* Value, IMmRpRiskMatrixShiftAtom** pRetVal);
	STDMETHOD(get_Item)(BSTR Key, IMmRpRiskMatrixShiftAtom** pValue);
	STDMETHOD(Remove)(BSTR Key);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpRiskMatrixShiftColl), CMmRpRiskMatrixShiftColl)

#endif //__MM_RP_RISK_MATRIX_SHIFT_COLL__