// MmOrderByReqColl.h : Declaration of the CMmOrderByReqColl
#ifndef __MMORDERBYREQCOLL_H__
#define __MMORDERBYREQCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmOrderAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmOrderByReqColl, IID_IMmOrderByReqColl);

typedef IDispatchImpl<IMmOrderByReqColl, &IID_IMmOrderByReqColl, &LIBID_EtsMmGeneralLib>							IMmOrderByReqCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmOrderByReqCollDispImpl, IMmOrderAtom, 
												BSTR, _bstr_t, DATE, DATE>	IMmOrderByReqCollImpl;

// CMmOrderByReqColl

class ATL_NO_VTABLE CMmOrderByReqColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmOrderByReqColl, &CLSID_MmOrderByReqColl>,
	public ISupportErrorInfoImpl<&IID_IMmOrderByReqColl>,
	public IMmOrderByReqCollImpl
{
public:
	CMmOrderByReqColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMORDERBYREQCOLL)

DECLARE_NOT_AGGREGATABLE(CMmOrderByReqColl)

BEGIN_COM_MAP(CMmOrderByReqColl)
	COM_INTERFACE_ENTRY(IMmOrderByReqColl)
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
		IMmOrderByReqCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(BSTR Key, DATE SortKey, IMmOrderAtom* Value, IMmOrderAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmOrderByReqColl), CMmOrderByReqColl)

#endif //__MMORDERBYREQCOLL_H__