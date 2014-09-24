// MmRvGroupingTotalColl.h : Declaration of the CMmRvGroupingTotalColl
#ifndef __MMRVGROUPINGTOTALCOLL_H__
#define __MMRVGROUPINGTOTALCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvGroupingTotalAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvGroupingTotalColl, IID_IMmRvGroupingTotalColl);

typedef IDispatchImpl<IMmRvGroupingTotalColl, &IID_IMmRvGroupingTotalColl, &LIBID_EtsMmRisksLib>					IMmRvGroupingTotalCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRvGroupingTotalCollDispImpl, LONG, IMmRvGroupingTotalAtom, LONG>	IMmRvGroupingTotalCollImpl;

// CMmRvGroupingTotalColl
class ATL_NO_VTABLE CMmRvGroupingTotalColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvGroupingTotalColl, &CLSID_MmRvGroupingTotalColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvGroupingTotalColl>,
	public IMmRvGroupingTotalCollImpl
{
public:
	CMmRvGroupingTotalColl()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVGROUPINGTOTALCOLL)


	BEGIN_COM_MAP(CMmRvGroupingTotalColl)
		COM_INTERFACE_ENTRY(IMmRvGroupingTotalColl)
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
		IMmRvGroupingTotalCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmRvGroupingTotalAtom* Value, IMmRvGroupingTotalAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvGroupingTotalColl), CMmRvGroupingTotalColl)

#endif //__MMGAEXPTOTALCOLL_H__
