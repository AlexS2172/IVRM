// MmRvUndColl.h : Declaration of the CMmRvUndColl
#ifndef __MMRVUNDCOLL_H__
#define __MMRVUNDCOLL_H__

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvUndAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvUndColl, IID_IMmRvUndColl);

typedef IDispatchImpl<IMmRvUndColl, &IID_IMmRvUndColl, &LIBID_EtsMmRisksLib>												IMmRvUndCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRvUndCollDispImpl, IMmRvUndAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRvUndCollImpl;

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvUndColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvUndColl, &CLSID_MmRvUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvUndColl>,
	public IMmRvUndCollImpl
{
public:
	CMmRvUndColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVUNDCOLL)


BEGIN_COM_MAP(CMmRvUndColl)
	COM_INTERFACE_ENTRY(IMmRvUndColl)
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
		IMmRvUndCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRvUndAtom* Value, IMmRvUndAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvUndColl), CMmRvUndColl)

#endif //__MMRVUNDCOLL_H__