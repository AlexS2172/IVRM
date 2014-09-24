// MmVaStrikeColl.h : Declaration of the CMmVaStrikeColl
#ifndef __MMVASTRIKECOLL_H__
#define __MMVASTRIKECOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaStrikeAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmVaStrikeColl, IID_IMmVaStrikeColl);

typedef IDispatchImpl<IMmVaStrikeColl, &IID_IMmVaStrikeColl, &LIBID_EtsMmVolaAnalysisLib>							IMmVaStrikeCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmVaStrikeCollDispImpl, DOUBLE, IMmVaStrikeAtom, DOUBLE >	IMmVaStrikeCollImpl;

// CMmVaStrikeColl

class ATL_NO_VTABLE CMmVaStrikeColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaStrikeColl, &CLSID_MmVaStrikeColl>,
	public ISupportErrorInfoImpl<&IID_IMmVaStrikeColl>,
	public IMmVaStrikeCollImpl
{
public:
	CMmVaStrikeColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVASTRIKECOLL)


BEGIN_COM_MAP(CMmVaStrikeColl)
	COM_INTERFACE_ENTRY(IMmVaStrikeColl)
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
		IMmVaStrikeCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(DOUBLE Key, IMmVaStrikeAtom* Value, IMmVaStrikeAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaStrikeColl), CMmVaStrikeColl)

#endif //__MMVASTRIKECOLL_H__