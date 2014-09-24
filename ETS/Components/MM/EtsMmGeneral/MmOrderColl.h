// MmOrderColl.h : Declaration of the CMmOrderColl
#ifndef __MMORDERCOLL_H__
#define __MMORDERCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmOrderAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmOrderColl, IID_IMmOrderColl);

typedef IDispatchImpl<IMmOrderColl, &IID_IMmOrderColl, &LIBID_EtsMmGeneralLib>								IMmOrderCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmOrderCollDispImpl, IMmOrderAtom, LONG, LONG, DATE, DATE>	IMmOrderCollImpl;

// CMmOrderColl

class ATL_NO_VTABLE CMmOrderColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmOrderColl, &CLSID_MmOrderColl>,
	public ISupportErrorInfoImpl<&IID_IMmOrderColl>,
	public IMmOrderCollImpl
{
public:
	CMmOrderColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMORDERCOLL)

DECLARE_NOT_AGGREGATABLE(CMmOrderColl)

BEGIN_COM_MAP(CMmOrderColl)
	COM_INTERFACE_ENTRY(IMmOrderColl)
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
		IMmOrderCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, DATE SortKey, IMmOrderAtom* Value, IMmOrderAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmOrderColl), CMmOrderColl)

#endif //__MMORDERCOLL_H__