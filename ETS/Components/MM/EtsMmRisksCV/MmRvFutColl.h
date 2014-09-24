// MmRvFutColl.h : Declaration of the CMmRvFutColl
#ifndef __MMRVFUTCOLL_H__
#define __MMRVFUTCOLL_H__

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvFutAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvFutColl, IID_IMmRvFutColl);

typedef IDispatchImpl<IMmRvFutColl, &IID_IMmRvFutColl, &LIBID_EtsMmRisksLib>												IMmRvFutCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRvFutCollDispImpl, IMmRvFutAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRvFutCollImpl;

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvFutColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvFutColl, &CLSID_MmRvFutColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvFutColl>,
	public IMmRvFutCollImpl
{
public:
	CMmRvFutColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVFUTCOLL)


BEGIN_COM_MAP(CMmRvFutColl)
	COM_INTERFACE_ENTRY(IMmRvFutColl)
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
		IMmRvFutCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRvFutAtom* Value, IMmRvFutAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvFutColl), CMmRvFutColl)

#endif //__MMRVFUTCOLL_H__