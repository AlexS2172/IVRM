// BasketAssetComponentsColl.h : Declaration of the CBasketAssetComponentsColl

#pragma once
#include "resource.h"       // main symbols
#include "CommonSPtr.h"

typedef IDispatchImpl<IBasketAssetComponentsColl, &IID_IBasketAssetComponentsColl, &LIBID_EtsGeneralLib>	IBasketAssetComponentsCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IBasketAssetComponentsCollDispImpl, IBasketAssetComponent, LONG, LONG, BSTR, _bstr_t  >	IBasketAssetComponentsCollImpl;

// CBasketAssetComponentsColl
class ATL_NO_VTABLE CBasketAssetComponentsColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBasketAssetComponentsColl, &CLSID_BasketAssetComponentsColl>,
	public IBasketAssetComponentsCollImpl
{
public:
	CBasketAssetComponentsColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BASKETASSETCOMPONENTSCOLL)


BEGIN_COM_MAP(CBasketAssetComponentsColl)
	COM_INTERFACE_ENTRY(IBasketAssetComponentsColl)
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
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IBasketAssetComponent* Value, IBasketAssetComponent** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(BasketAssetComponentsColl), CBasketAssetComponentsColl)
