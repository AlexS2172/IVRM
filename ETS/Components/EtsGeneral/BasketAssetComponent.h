// BasketAssetComponent.h : Declaration of the CBasketAssetComponent

#pragma once
#include "resource.h"       // main symbols
#include "EtsGeneral.h"

// CBasketAssetComponent

class ATL_NO_VTABLE CBasketAssetComponent :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBasketAssetComponent, &CLSID_BasketAssetComponent>,
	public IDispatchImpl<IBasketAssetComponent, &IID_IBasketAssetComponent, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CBasketAssetComponent()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BASKETASSETCOMPONENT)


BEGIN_COM_MAP(CBasketAssetComponent)
	COM_INTERFACE_ENTRY(IBasketAssetComponent)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

};

OBJECT_ENTRY_AUTO(__uuidof(BasketAssetComponent), CBasketAssetComponent)
