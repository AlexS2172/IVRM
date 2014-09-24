// MmOrderExecDestColl.h : Declaration of the CMmOrderExecDestColl
#ifndef __MMORDEREXECDESTCOLL_H__
#define __MMORDEREXECDESTCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmOrderExecDestAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmOrderExecDestColl, IID_IMmOrderExecDestColl);

typedef IDispatchImpl<IMmOrderExecDestColl, &IID_IMmOrderExecDestColl, &LIBID_EtsMmGeneralLib>								IMmOrderExecDestCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmOrderExecDestCollDispImpl, IMmOrderExecDestAtom,
												LONG, LONG, BSTR, _bstr_t  >	IMmOrderExecDestCollImpl;

// CMmOrderExecDestColl

class ATL_NO_VTABLE CMmOrderExecDestColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmOrderExecDestColl, &CLSID_MmOrderExecDestColl>,
	public ISupportErrorInfoImpl<&IID_IMmOrderExecDestColl>,
	public IMmOrderExecDestCollImpl
{
public:
	CMmOrderExecDestColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMORDEREXECDESTCOLL)

DECLARE_NOT_AGGREGATABLE(CMmOrderExecDestColl)

BEGIN_COM_MAP(CMmOrderExecDestColl)
	COM_INTERFACE_ENTRY(IMmOrderExecDestColl)
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
		IMmOrderExecDestCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmOrderExecDestAtom* Value, IMmOrderExecDestAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmOrderExecDestColl), CMmOrderExecDestColl)

#endif //__MMORDEREXECDESTCOLL_H__