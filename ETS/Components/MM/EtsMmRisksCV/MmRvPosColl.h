// MmRvPosColl.h : Declaration of the CMmRvPosColl
#ifndef __MMRVPOSCOLL_H__
#define __MMRVPOSCOLL_H__

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvPosAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvPosColl, IID_IMmRvPosColl);

typedef IDispatchImpl<IMmRvPosColl, &IID_IMmRvPosColl, &LIBID_EtsMmRisksLib>												IMmRvPosCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRvPosCollDispImpl, IMmRvPosAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRvPosCollImpl;

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvPosColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvPosColl, &CLSID_MmRvPosColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvPosColl>,
	public IMmRvPosCollImpl
{
public:
	CMmRvPosColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVPOSCOLL)


BEGIN_COM_MAP(CMmRvPosColl)
	COM_INTERFACE_ENTRY(IMmRvPosColl)
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
		IMmRvPosCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRvPosAtom* Value, IMmRvPosAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvPosColl), CMmRvPosColl)

#endif //__MMRVPOSCOLL_H__