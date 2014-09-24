// MmGaExpColl.h : Declaration of the CMmGaExpColl
#ifndef __MMGAEXPCOLL_H__
#define __MMGAEXPCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaExpAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmGaExpColl, IID_IMmGaExpColl);

typedef IDispatchImpl<IMmGaExpColl, &IID_IMmGaExpColl, &LIBID_EtsMmGreeksLib>						IMmGaExpCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmGaExpCollDispImpl, LONG, IMmGaExpAtom, LONG >		IMmGaExpCollImpl;


// CMmGaExpColl
class ATL_NO_VTABLE CMmGaExpColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaExpColl, &CLSID_MmGaExpColl>,
	public ISupportErrorInfoImpl<&IID_IMmGaExpColl>,
	public IMmGaExpCollImpl
{
public:
	CMmGaExpColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAEXPCOLL)


BEGIN_COM_MAP(CMmGaExpColl)
	COM_INTERFACE_ENTRY(IMmGaExpColl)
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
		IMmGaExpCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmGaExpAtom* Value, IMmGaExpAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaExpColl), CMmGaExpColl)

#endif //__MMGAEXPCOLL_H__