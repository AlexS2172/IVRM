// MmGaPosColl.h : Declaration of the CMmGaPosColl
#ifndef __MMGAPOSCOLL_H__
#define __MMGAPOSCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaPosAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmGaPosColl, IID_IMmGaPosColl);

typedef IDispatchImpl<IMmGaPosColl, &IID_IMmGaPosColl, &LIBID_EtsMmGreeksLib>						IMmGaPosCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmGaPosCollDispImpl, LONG, IMmGaPosAtom, LONG >		IMmGaPosCollImpl;

// CMmGaPosColl
class ATL_NO_VTABLE CMmGaPosColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaPosColl, &CLSID_MmGaPosColl>,
	public ISupportErrorInfoImpl<&IID_IMmGaPosColl>,
	public IMmGaPosCollImpl
{
public:
	CMmGaPosColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAPOSCOLL)


BEGIN_COM_MAP(CMmGaPosColl)
	COM_INTERFACE_ENTRY(IMmGaPosColl)
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
		IMmGaPosCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmGaPosAtom* Value, IMmGaPosAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaPosColl), CMmGaPosColl)

#endif //__MMGAPOSCOLL_H__