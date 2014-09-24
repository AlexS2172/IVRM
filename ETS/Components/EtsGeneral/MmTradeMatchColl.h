// MmTradeMatchColl.h : Declaration of the CMmTradeMatchColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"


// CMmTradeMatchColl

_COM_SMARTPTR_TYPEDEF(IMmTradeMatchColl, IID_IMmTradeMatchColl);

//class CMmTradeMatchColl;
typedef IDispatchImpl<IMmTradeMatchColl, &IID_IMmTradeMatchColl, &LIBID_EtsGeneralLib>								IMmTradeMatchCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmTradeMatchCollDispImpl, IMmTradeMatchAtom, LONG, LONG, DATE, DATE>	IMmTradeMatchCollImpl;

class ATL_NO_VTABLE CMmTradeMatchColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTradeMatchColl, &CLSID_MmTradeMatchColl>,
	public ISupportErrorInfo,
	public IMmTradeMatchCollImpl
{
public:
	CMmTradeMatchColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMTRADEMATCHCOLL)


	BEGIN_COM_MAP(CMmTradeMatchColl)
		COM_INTERFACE_ENTRY(IMmTradeMatchColl)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		IMmTradeMatchCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	STDMETHOD(Add)(LONG Key, DATE SortKey, IMmTradeMatchAtom* Value, IMmTradeMatchAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTradeMatchColl), CMmTradeMatchColl)
