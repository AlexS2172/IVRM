// MmVaOptPairColl.h : Declaration of the CMmVaOptPairColl
#ifndef __MMVAOPTPAIRCOLL_H__
#define __MMVAOPTPAIRCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaOptPairAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmVaOptPairColl, IID_IMmVaOptPairColl);

typedef IDispatchImpl<IMmVaOptPairColl, &IID_IMmVaOptPairColl, &LIBID_EtsMmVolaAnalysisLib>													IMmVaOptPairCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmVaOptPairCollDispImpl, IMmVaOptPairAtom, LONG, LONG, BSTR, _bstr_t  >	IMmVaOptPairCollImpl;

// CMmVaOptPairColl
class ATL_NO_VTABLE CMmVaOptPairColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaOptPairColl, &CLSID_MmVaOptPairColl>,
	public ISupportErrorInfoImpl<&IID_IMmVaOptPairColl>,
	public IMmVaOptPairCollImpl
{
public:
	CMmVaOptPairColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAOPTPAIRCOLL)


BEGIN_COM_MAP(CMmVaOptPairColl)
	COM_INTERFACE_ENTRY(IMmVaOptPairColl)
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
		IMmVaOptPairCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmVaOptPairAtom* Value, IMmVaOptPairAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaOptPairColl), CMmVaOptPairColl)

#endif //__MMVAOPTPAIRCOLL_H__