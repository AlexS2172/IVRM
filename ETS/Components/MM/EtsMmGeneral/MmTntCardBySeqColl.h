// MmTntCardBySeqColl.h : Declaration of the CMmTntCardBySeqColl
#ifndef __MMTNTCARDBYSEQCOLL_H__
#define __MMTNTCARDBYSEQCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmTntCardAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmTntCardBySeqColl, IID_IMmTntCardBySeqColl);

typedef IDispatchImpl<IMmTntCardBySeqColl, &IID_IMmTntCardBySeqColl, &LIBID_EtsMmGeneralLib>		IMmTntCardBySeqCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmTntCardBySeqCollDispImpl, IMmTntCardAtom, 
												LONG, LONG, DATE, DATE>		IMmTntCardBySeqCollImpl;

// CMmTntCardBySeqColl

class ATL_NO_VTABLE CMmTntCardBySeqColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTntCardBySeqColl, &CLSID_MmTntCardBySeqColl>,
	public ISupportErrorInfoImpl<&IID_IMmTntCardBySeqColl>,
	public IMmTntCardBySeqCollImpl
{
public:
	CMmTntCardBySeqColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTNTCARDBYSEQCOLL)

DECLARE_NOT_AGGREGATABLE(CMmTntCardBySeqColl)

BEGIN_COM_MAP(CMmTntCardBySeqColl)
	COM_INTERFACE_ENTRY(IMmTntCardBySeqColl)
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
		IMmTntCardBySeqCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(LONG Key, DATE SortKey, IMmTntCardAtom* Value, IMmTntCardAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmTntCardBySeqColl), CMmTntCardBySeqColl)

#endif //__MMTNTCARDBYSEQCOLL_H__