// MmTntCounterPartyByNameColl.h : Declaration of the CMmTntCounterPartyByNameColl
#ifndef __MMTNTCOUNTERPARTYBYNAMECOLL_H__
#define __MMTNTCOUNTERPARTYBYNAMECOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmTntCounterPartyAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmTntCounterPartyByNameColl, IID_IMmTntCounterPartyByNameColl);

typedef IDispatchImpl<IMmTntCounterPartyByNameColl, &IID_IMmTntCounterPartyByNameColl, &LIBID_EtsMmGeneralLib>						IMmTntCounterPartyByNameCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmTntCounterPartyByNameCollDispImpl, BSTR, IMmTntCounterPartyAtom, CAdapt<CComBSTR> >	IMmTntCounterPartyByNameCollImpl;

// CMmTntCounterPartyByNameColl

class ATL_NO_VTABLE CMmTntCounterPartyByNameColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTntCounterPartyByNameColl, &CLSID_MmTntCounterPartyByNameColl>,
	public ISupportErrorInfoImpl<&IID_IMmTntCounterPartyByNameColl>,
	public IMmTntCounterPartyByNameCollImpl
{
public:
	CMmTntCounterPartyByNameColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTNTCOUNTERPARTYBYNAMECOLL)

DECLARE_NOT_AGGREGATABLE(CMmTntCounterPartyByNameColl)

BEGIN_COM_MAP(CMmTntCounterPartyByNameColl)
	COM_INTERFACE_ENTRY(IMmTntCounterPartyByNameColl)
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
		IMmTntCounterPartyByNameCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(BSTR Key, IMmTntCounterPartyAtom* Value, IMmTntCounterPartyAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTntCounterPartyByNameColl), CMmTntCounterPartyByNameColl)

#endif //__MMTNTCOUNTERPARTYBYNAMECOLL_H__