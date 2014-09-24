// MmRpSyntheticColl.h : Declaration of the CMmRpSyntheticColl
#ifndef __MM_RP_SYNTHETIC_COLL__
#define __MM_RP_SYNTHETIC_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpSyntheticColl, IID_IMmRpSyntheticColl);
#define MAKELONGLONG(a, b)	((LONGLONG)a << 32 | b)

typedef IDispatchImpl<IMmRpSyntheticColl, &IID_IMmRpSyntheticColl, &LIBID_EtsMmReportsLib>	IMmRpSyntheticCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRpSyntheticCollDispImpl, IMmRpSyntheticAtom, LONGLONG, LONGLONG, BSTR, _bstr_t  >	IMmRpSyntheticCollImpl;

// CMmRpSyntheticColl

class ATL_NO_VTABLE CMmRpSyntheticColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpSyntheticColl, &CLSID_MmRpSyntheticColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpSyntheticColl>,
	public IMmRpSyntheticCollImpl
{
public:
	CMmRpSyntheticColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPSYNTHETICCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpSyntheticColl)

BEGIN_COM_MAP(CMmRpSyntheticColl)
	COM_INTERFACE_ENTRY(IMmRpSyntheticColl)
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
		IMmRpSyntheticCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, LONG SubKey, BSTR SortKey, IMmRpSyntheticAtom* pValue, IMmRpSyntheticAtom** ppRetVal);
	STDMETHOD(get_Item)(LONG Key,LONG SubKey, IMmRpSyntheticAtom** ppVal);
	STDMETHOD(Remove)(LONG Key,LONG SubKey);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpSyntheticColl), CMmRpSyntheticColl)

#endif //__MM_RP_SYNTHETIC_COLL__