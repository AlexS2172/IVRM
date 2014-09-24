// MmGaExpTotalColl.h : Declaration of the CMmGaExpTotalColl
#ifndef __MMGAEXPTOTALCOLL_H__
#define __MMGAEXPTOTALCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaExpTotalAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmGaExpTotalColl, IID_IMmGaExpTotalColl);

typedef IDispatchImpl<IMmGaExpTotalColl, &IID_IMmGaExpTotalColl, &LIBID_EtsMmGreeksLib>						IMmGaExpTotalCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmGaExpTotalCollDispImpl, LONG, IMmGaExpTotalAtom, LONG >	IMmGaExpTotalCollImpl;

// CMmGaExpTotalColl
class ATL_NO_VTABLE CMmGaExpTotalColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaExpTotalColl, &CLSID_MmGaExpTotalColl>,
	public ISupportErrorInfoImpl<&IID_IMmGaExpTotalColl>,
	public IMmGaExpTotalCollImpl
{
public:
	CMmGaExpTotalColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAEXPTOTALCOLL)


BEGIN_COM_MAP(CMmGaExpTotalColl)
	COM_INTERFACE_ENTRY(IMmGaExpTotalColl)
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
		IMmGaExpTotalCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmGaExpTotalAtom* Value, IMmGaExpTotalAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaExpTotalColl), CMmGaExpTotalColl)

#endif //__MMGAEXPTOTALCOLL_H__