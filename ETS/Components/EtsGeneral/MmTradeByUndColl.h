// MmTradeByUndColl.h : Declaration of the CMmTradeByUndColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"


_COM_SMARTPTR_TYPEDEF(IMmTradeByUndColl, IID_IMmTradeByUndColl);

class CMmTradeByUndColl;
typedef IDispatchImpl<IMmTradeByUndColl, &IID_IMmTradeByUndColl, &LIBID_EtsGeneralLib>						IMmTradeByUndCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl< IMmTradeByUndCollDispImpl, BSTR, IMmTradeInfoColl, _bstr_t >	IMmTradeByUndCollImpl;
// CMmTradeByUndColl

class ATL_NO_VTABLE CMmTradeByUndColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTradeByUndColl, &CLSID_MmTradeByUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmTradeByUndColl>,
	public IMmTradeByUndCollImpl
{
public:
	CMmTradeByUndColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMTRADEBYUNDCOLL)
	//DECLARE_ONLY_AGGREGATABLE(CMmTradeByUndColl)

	BEGIN_COM_MAP(CMmTradeByUndColl)
		COM_INTERFACE_ENTRY(IMmTradeByUndColl)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	BEGIN_PROP_MAP(CMmTradeByUndColl)
	END_PROP_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		IMmTradeByUndCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(BSTR Key, IMmTradeInfoColl* Value, IMmTradeInfoColl** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTradeByUndColl), CMmTradeByUndColl)
