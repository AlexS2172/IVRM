// MmVaOptRootColl.h : Declaration of the CMmVaOptRootColl
#ifndef __MMVAOPTROOTCOLL_H_
#define __MMVAOPTROOTCOLL_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaOptRootAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmVaOptRootColl, IID_IMmVaOptRootColl);

typedef IDispatchImpl<IMmVaOptRootColl, &IID_IMmVaOptRootColl, &LIBID_EtsMmVolaAnalysisLib>													IMmVaOptRootCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmVaOptRootCollDispImpl, IMmVaOptRootAtom, LONG, LONG, BSTR, _bstr_t  >	IMmVaOptRootCollImpl;

// CMmVaOptRootColl
class ATL_NO_VTABLE CMmVaOptRootColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaOptRootColl, &CLSID_MmVaOptRootColl>,
	public ISupportErrorInfoImpl<&IID_IMmVaOptRootColl>,
	public IMmVaOptRootCollImpl
{
public:
	CMmVaOptRootColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAOPTROOTCOLL)


BEGIN_COM_MAP(CMmVaOptRootColl)
	COM_INTERFACE_ENTRY(IMmVaOptRootColl)
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
		IMmVaOptRootCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmVaOptRootAtom* Value, IMmVaOptRootAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaOptRootColl), CMmVaOptRootColl)

#endif //__MMVAOPTROOTCOLL_H_