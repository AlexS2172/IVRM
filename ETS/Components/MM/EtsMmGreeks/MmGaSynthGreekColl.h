// MmGaSynthGreekColl.h : Declaration of the CMmGaSynthGreekColl
#ifndef __MMGASYNTHGREEKCOLL_H__
#define __MMGASYNTHGREEKCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaSynthGreekAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmGaSynthGreekColl, IID_IMmGaSynthGreekColl);

typedef IDispatchImpl<IMmGaSynthGreekColl, &IID_IMmGaSynthGreekColl, &LIBID_EtsMmGreeksLib>						IMmGaSynthGreekCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmGaSynthGreekCollDispImpl, LONG, IMmGaSynthGreekAtom, LONG >	IMmGaSynthGreekCollImpl;

// CMmGaSynthGreekColl
class ATL_NO_VTABLE CMmGaSynthGreekColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaSynthGreekColl, &CLSID_MmGaSynthGreekColl>,
	public ISupportErrorInfoImpl<&IID_IMmGaSynthGreekColl>,
	public IMmGaSynthGreekCollImpl
{
public:
	CMmGaSynthGreekColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGASYNTHGREEKCOLL)


BEGIN_COM_MAP(CMmGaSynthGreekColl)
	COM_INTERFACE_ENTRY(IMmGaSynthGreekColl)
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
		IMmGaSynthGreekCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, IMmGaSynthGreekAtom* Value, IMmGaSynthGreekAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaSynthGreekColl), CMmGaSynthGreekColl)

#endif //__MMGASYNTHGREEKCOLL_H__