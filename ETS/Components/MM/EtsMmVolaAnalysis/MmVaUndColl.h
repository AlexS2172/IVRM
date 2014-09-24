// MmVaUndColl.h : Declaration of the CMmVaUndColl
#ifndef __MMVAUNDCOLL_H_
#define __MMVAUNDCOLL_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaUndAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmVaUndColl, IID_IMmVaUndColl);

typedef IDispatchImpl<IMmVaUndColl, &IID_IMmVaUndColl, &LIBID_EtsMmVolaAnalysisLib>													IMmVaUndCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmVaUndCollDispImpl, IMmVaUndAtom, LONG, LONG, BSTR, _bstr_t  >	IMmVaUndCollImpl;

// CMmVaUndColl
class ATL_NO_VTABLE CMmVaUndColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaUndColl, &CLSID_MmVaUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmVaUndColl>,
	public IMmVaUndCollImpl
{
public:
	CMmVaUndColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAUNDCOLL)


BEGIN_COM_MAP(CMmVaUndColl)
	COM_INTERFACE_ENTRY(IMmVaUndColl)
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
		IMmVaUndCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmVaUndAtom* Value, IMmVaUndAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaUndColl), CMmVaUndColl)

#endif //__MMVAUNDCOLL_H_