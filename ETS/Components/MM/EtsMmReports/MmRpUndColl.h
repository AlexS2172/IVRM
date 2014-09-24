// MmRpUndColl.h : Declaration of the CMmRpUndColl
#ifndef __MM_RP_UND_COLL__
#define __MM_RP_UND_COLL__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpUndColl, IID_IMmRpUndColl);

typedef IDispatchImpl<IMmRpUndColl, &IID_IMmRpUndColl, &LIBID_EtsMmReportsLib>					IMmRpUndCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpUndCollDispImpl, BSTR, IMmRpUndAtom, _bstr_t>	IMmRpUndCollImpl;

// CMmRpUndColl

class ATL_NO_VTABLE CMmRpUndColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpUndColl, &CLSID_MmRpUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmRpUndColl>,
	public IMmRpUndCollImpl
{
public:
	CMmRpUndColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPUNDCOLL)

DECLARE_NOT_AGGREGATABLE(CMmRpUndColl)

BEGIN_COM_MAP(CMmRpUndColl)
	COM_INTERFACE_ENTRY(IMmRpUndColl)
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
		IMmRpUndCollImpl::Clear();
	}

public:

	STDMETHOD(Add)(BSTR Key, IMmRpUndAtom* Value, IMmRpUndAtom** pRetVal);
	STDMETHOD(GetPriceProperty)(BSTR Symbol , IMMRpPrice** PPrice) ;

	STDMETHOD(GetPricePropertyOpt)( BSTR Underlying, BSTR Symbol , IMMRpPrice** PPrice) ;
	STDMETHOD(GetPricePropertyFut)( BSTR Underlying, BSTR Symbol , IMMRpPrice** PPrice) ;
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpUndColl), CMmRpUndColl)

#endif //__MM_RP_UND_COLL__