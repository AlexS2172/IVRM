// MmRpGreeksSummaryColl.h : Declaration of the CMmRpGreeksSummaryColl
#ifndef __MM_RP_GREEKS_SUMMARY_COLL__
#define __MM_RP_GREEKS_SUMMARY_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpGreeksSummaryColl, IID_IMmRpGreeksSummaryColl);

typedef IDispatchImpl<IMmRpGreeksSummaryColl, &IID_IMmRpGreeksSummaryColl, &LIBID_EtsMmReportsLib>					IMmRpGreeksSummaryCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpGreeksSummaryCollDispImpl, LONG, IMmRpGreeksSummaryAtom, LONG>	IMmRpGreeksSummaryCollImpl;

// CMmRpGreeksSummaryColl

class ATL_NO_VTABLE CMmRpGreeksSummaryColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpGreeksSummaryColl, &CLSID_MmRpGreeksSummaryColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpGreeksSummaryColl>,
	public IMmRpGreeksSummaryCollImpl
{
public:
	CMmRpGreeksSummaryColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPGREEKSSUMMARYCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpGreeksSummaryColl)

BEGIN_COM_MAP(CMmRpGreeksSummaryColl)
	COM_INTERFACE_ENTRY(IMmRpGreeksSummaryColl)
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
		IMmRpGreeksSummaryCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRpGreeksSummaryAtom* Value, IMmRpGreeksSummaryAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpGreeksSummaryColl), CMmRpGreeksSummaryColl)

#endif //__MM_RP_GREEKS_SUMMARY_COLL__