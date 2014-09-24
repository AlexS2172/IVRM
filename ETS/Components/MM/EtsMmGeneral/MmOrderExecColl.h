// MmOrderExecColl.h : Declaration of the CMmOrderExecColl
#ifndef __MMORDEREXECCOLL_H__
#define __MMORDEREXECCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmOrderExecAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmOrderExecColl, IID_IMmOrderExecColl);

typedef IDispatchImpl<IMmOrderExecColl, &IID_IMmOrderExecColl, &LIBID_EtsMmGeneralLib>								IMmOrderExecCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmOrderExecCollDispImpl, IMmOrderExecAtom, LONG, LONG, DATE, DATE>	IMmOrderExecCollImpl;

// CMmOrderExecColl

class ATL_NO_VTABLE CMmOrderExecColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmOrderExecColl, &CLSID_MmOrderExecColl>,
	public ISupportErrorInfoImpl<&IID_IMmOrderExecColl>,
	public IMmOrderExecCollImpl
{
public:
	CMmOrderExecColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMORDEREXECCOLL)

DECLARE_NOT_AGGREGATABLE(CMmOrderExecColl)

BEGIN_COM_MAP(CMmOrderExecColl)
	COM_INTERFACE_ENTRY(IMmOrderExecColl)
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
		IMmOrderExecCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, DATE SortKey, IMmOrderExecAtom* Value, IMmOrderExecAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmOrderExecColl), CMmOrderExecColl)

#endif //__MMORDEREXECCOLL_H__