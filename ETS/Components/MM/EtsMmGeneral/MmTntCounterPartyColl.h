// MmTntCounterPartyColl.h : Declaration of the CMmTntCounterPartyColl
#ifndef __MMTNTCOUNTERPARTYCOLL_H__
#define __MMTNTCOUNTERPARTYCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmTntCounterPartyAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmTntCounterPartyColl, IID_IMmTntCounterPartyColl);

typedef IDispatchImpl<IMmTntCounterPartyColl, &IID_IMmTntCounterPartyColl, &LIBID_EtsMmGeneralLib>								IMmTntCounterPartyCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmTntCounterPartyCollDispImpl, IMmTntCounterPartyAtom,
												LONG, LONG, BSTR, _bstr_t  >	IMmTntCounterPartyCollImpl;

// CMmTntCounterPartyColl

class ATL_NO_VTABLE CMmTntCounterPartyColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTntCounterPartyColl, &CLSID_MmTntCounterPartyColl>,
	public ISupportErrorInfoImpl<&IID_IMmTntCounterPartyColl>,
	public IMmTntCounterPartyCollImpl
{
public:
	CMmTntCounterPartyColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTNTCOUNTERPARTYCOLL)

DECLARE_NOT_AGGREGATABLE(CMmTntCounterPartyColl)

BEGIN_COM_MAP(CMmTntCounterPartyColl)
	COM_INTERFACE_ENTRY(IMmTntCounterPartyColl)
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
		IMmTntCounterPartyCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmTntCounterPartyAtom* Value, IMmTntCounterPartyAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTntCounterPartyColl), CMmTntCounterPartyColl)

#endif //__MMTNTCOUNTERPARTYCOLL_H__