// AssetColl.h : Declaration of the CAssetColl

#pragma once
#include "resource.h"       // main symbols
#include "CommonSPtr.h"

typedef IDispatchImpl<IAssetColl, &IID_IAssetColl, &LIBID_EtsGeneralLib>	IAssetCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IAssetCollDispImpl, IAsset, LONG, LONG, BSTR, _bstr_t  >	IAssetCollImpl;
// CAssetColl
class ATL_NO_VTABLE CAssetColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAssetColl, &CLSID_AssetColl>,
	public IAssetCollImpl
{
public:
	CAssetColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ASSETCOLL)


BEGIN_COM_MAP(CAssetColl)
	COM_INTERFACE_ENTRY(IAssetColl)
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
		IAssetCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IAsset* Value, IAsset** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(AssetColl), CAssetColl)
