// MmTntCardColl.h : Declaration of the CMmTntCardColl
#ifndef __MMTNTCARDCOLL_H__
#define __MMTNTCARDCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmTntCardAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmTntCardColl, IID_IMmTntCardColl);

typedef IDispatchImpl<IMmTntCardColl, &IID_IMmTntCardColl, &LIBID_EtsMmGeneralLib>		IMmTntCardCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmTntCardCollDispImpl, IMmTntCardAtom, 
												BSTR, _bstr_t, DATE, DATE>		IMmTntCardCollImpl;

// CMmTntCardColl

class ATL_NO_VTABLE CMmTntCardColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTntCardColl, &CLSID_MmTntCardColl>,
	public ISupportErrorInfoImpl<&IID_IMmTntCardColl>,
	public IMmTntCardCollImpl
{
public:
	CMmTntCardColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTNTCARDCOLL)

DECLARE_NOT_AGGREGATABLE(CMmTntCardColl)

BEGIN_COM_MAP(CMmTntCardColl)
	COM_INTERFACE_ENTRY(IMmTntCardColl)
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
		IMmTntCardCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(BSTR Key, DATE SortKey, IMmTntCardAtom* Value, IMmTntCardAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmTntCardColl), CMmTntCardColl)

#endif //__MMTNTCARDCOLL_H__