// MmRvGroupingColl.h : Declaration of the CMmRvGroupingColl
#ifndef __MMRVGROUPINGCOLL_H__
#define __MMRVGROUPINGCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvGroupingAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvGroupingColl, IID_IMmRvGroupingColl);

typedef IDispatchImpl<IMmRvGroupingColl, &IID_IMmRvGroupingColl, &LIBID_EtsMmRisksLib>					IMmRvGroupingCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRvGroupingCollDispImpl, LONG, IMmRvGroupingAtom, LONG>	IMmRvGroupingCollImpl;


// CMmRvGroupingColl
class ATL_NO_VTABLE CMmRvGroupingColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvGroupingColl, &CLSID_MmRvGroupingColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvGroupingColl>,
	public IMmRvGroupingCollImpl
{
public:
	CMmRvGroupingColl()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVGROUPINGCOLL)


	BEGIN_COM_MAP(CMmRvGroupingColl)
		COM_INTERFACE_ENTRY(IMmRvGroupingColl)
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
		IMmRvGroupingCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRvGroupingAtom* Value, IMmRvGroupingAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvGroupingColl), CMmRvGroupingColl)

#endif //__MMRVGROUPINGCOLL_H__
