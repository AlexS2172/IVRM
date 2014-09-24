// MmRpGreeksByMonthColl.h : Declaration of the CMmRpGreeksByMonthColl
#ifndef __MM_RP_GREEKS_BY_MONTH_COLL__
#define __MM_RP_GREEKS_BY_MONTH_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpGreeksByMonthColl, IID_IMmRpGreeksByMonthColl);

typedef IDispatchImpl<IMmRpGreeksByMonthColl, &IID_IMmRpGreeksByMonthColl, &LIBID_EtsMmReportsLib>						IMmRpGreeksByMonthCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpGreeksByMonthCollDispImpl, _bstr_t, IMmRpGreeksByMonthAtom, _bstr_t>	IMmRpGreeksByMonthCollImpl;

// CMmRpGreeksByMonthColl

class ATL_NO_VTABLE CMmRpGreeksByMonthColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpGreeksByMonthColl, &CLSID_MmRpGreeksByMonthColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpGreeksByMonthColl>,
	public IMmRpGreeksByMonthCollImpl
{
public:
	CMmRpGreeksByMonthColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPGREEKSBYMONTHCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpGreeksByMonthColl)

BEGIN_COM_MAP(CMmRpGreeksByMonthColl)
	COM_INTERFACE_ENTRY(IMmRpGreeksByMonthColl)
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
		IMmRpGreeksByMonthCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(BSTR Key, IMmRpGreeksByMonthAtom* Value, IMmRpGreeksByMonthAtom** pRetVal);
	STDMETHOD(get_Item)(BSTR Key, IMmRpGreeksByMonthAtom** pValue);
	STDMETHOD(Remove)(BSTR Key);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpGreeksByMonthColl), CMmRpGreeksByMonthColl)

#endif //__MM_RP_GREEKS_BY_MONTH_COLL__