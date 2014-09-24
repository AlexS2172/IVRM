// MmRvSynthGreeksColl.h : Declaration of the CMmRvSynthGreeksColl
#ifndef __MMRVSYNTHGREEKSCOLL_H__
#define __MMRVSYNTHGREEKSCOLL_H__

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvSynthGreeksAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvSynthGreeksColl, IID_IMmRvSynthGreeksColl);

typedef IDispatchImpl<IMmRvSynthGreeksColl, &IID_IMmRvSynthGreeksColl, &LIBID_EtsMmRisksLib>												IMmRvSynthGreeksCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRvSynthGreeksCollDispImpl, IMmRvSynthGreeksAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRvSynthGreeksCollImpl;

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvSynthGreeksColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvSynthGreeksColl, &CLSID_MmRvSynthGreeksColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvSynthGreeksColl>,
	public IMmRvSynthGreeksCollImpl
{
public:
	CMmRvSynthGreeksColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVSYNTHGREEKSCOLL)


BEGIN_COM_MAP(CMmRvSynthGreeksColl)
	COM_INTERFACE_ENTRY(IMmRvSynthGreeksColl)
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
		IMmRvSynthGreeksCollImpl::Clear();
	}

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRvSynthGreeksAtom* Value, IMmRvSynthGreeksAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvSynthGreeksColl), CMmRvSynthGreeksColl)

#endif //__MMRVSYNTHGREEKSCOLL_H__