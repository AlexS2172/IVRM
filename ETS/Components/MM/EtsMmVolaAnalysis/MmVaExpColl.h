// MmVaExpColl.h : Declaration of the CMmVaExpColl
#ifndef __MMVAEXPCOLL_H__
#define __MMVAEXPCOLL_H__

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaExpAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmVaExpColl, IID_IMmVaExpColl);

typedef IDispatchImpl<IMmVaExpColl, &IID_IMmVaExpColl, &LIBID_EtsMmVolaAnalysisLib>						IMmVaExpCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmVaExpCollDispImpl, DATE, IMmVaExpAtom, DATE >		IMmVaExpCollImpl;

// CMmVaExpColl

class ATL_NO_VTABLE CMmVaExpColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaExpColl, &CLSID_MmVaExpColl>,
	public ISupportErrorInfoImpl<&IID_IMmVaExpColl>,
	public IMmVaExpCollImpl
{
public:
	CMmVaExpColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAEXPCOLL)


BEGIN_COM_MAP(CMmVaExpColl)
	COM_INTERFACE_ENTRY(IMmVaExpColl)
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
		IMmVaExpCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(DATE Key, IMmVaExpAtom* Value, IMmVaExpAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaExpColl), CMmVaExpColl)

#endif //__MMVAEXPCOLL_H__