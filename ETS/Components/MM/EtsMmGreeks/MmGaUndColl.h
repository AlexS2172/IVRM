// MmGaUndColl.h : Declaration of the CMmGaUndColl
#ifndef  __MMGAUNDCOLL_H__
#define  __MMGAUNDCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaUndAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmGaUndColl, IID_IMmGaUndColl);

typedef IDispatchImpl<IMmGaUndColl, &IID_IMmGaUndColl, &LIBID_EtsMmGreeksLib>													IMmGaUndCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmGaUndCollDispImpl, IMmGaUndAtom, LONG, LONG, BSTR, _bstr_t  >	IMmGaUndCollImpl;

// CMmGaUndColl

class ATL_NO_VTABLE CMmGaUndColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaUndColl, &CLSID_MmGaUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmGaUndColl>,
	public IMmGaUndCollImpl
{
public:
	CMmGaUndColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAUNDCOLL)


BEGIN_COM_MAP(CMmGaUndColl)
	COM_INTERFACE_ENTRY(IMmGaUndColl)
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
		IMmGaUndCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmGaUndAtom* Value, IMmGaUndAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmGaUndColl), CMmGaUndColl)

#endif // __MMGAUNDCOLL_H__
