// EtsFilterData.h : Declaration of the CEtsFilterData

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CEtsFilterData

class ATL_NO_VTABLE CEtsFilterData :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsFilterData, &CLSID_EtsFilterData>,
	public ISupportErrorInfoImpl<&IID_IEtsFilterData>,
	public IDispatchImpl<IEtsFilterData, &IID_IEtsFilterData, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEtsFilterData()
		:m_lLowerBound(0L)
		,m_lUpperBound(0L)
		,m_szDataSize(0)
		,m_bInitialized(false)
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_ETSFILTERDATA)


	BEGIN_COM_MAP(CEtsFilterData)
		COM_INTERFACE_ENTRY(IEtsFilterData)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
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

private:
	long   m_lLowerBound;
	long   m_lUpperBound;
	size_t m_szDataSize;
	bool   m_bInitialized;
	boost::shared_array<long> m_spArrayData;

public:
	STDMETHOD(get_Data)(LONG DataId, LONG* pVal);
	STDMETHOD(put_Data)(LONG DataId, LONG newVal);
	STDMETHOD(Initialize)(LONG LowBound, LONG HighBound);
};
_COM_SMARTPTR_TYPEDEF(IEtsFilterData, IID_IEtsFilterData);

OBJECT_ENTRY_AUTO(__uuidof(EtsFilterData), CEtsFilterData)
