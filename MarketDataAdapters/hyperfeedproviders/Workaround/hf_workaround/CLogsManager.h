// CLogsManager.h : Declaration of the CCLogsManager

#pragma once
#include "resource.h"       // main symbols

#include "HyperFeedProviders.h"
#include "Price.h"

// CCLogsManager

class ATL_NO_VTABLE CSettingsManager : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSettingsManager, &CLSID_SettingsManager>,
	public ISupportErrorInfoImpl<&IID_ISettingsManager>,
	public IDispatchImpl<ISettingsManager, &IID_ISettingsManager, &LIBID_HYPERFEEDPROVIDERSLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSettingsManager()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CLOGSMANAGER)
DECLARE_CLASSFACTORY_SINGLETON(CSettingsManager)

DECLARE_NOT_AGGREGATABLE(CSettingsManager)

BEGIN_COM_MAP(CSettingsManager)
	COM_INTERFACE_ENTRY(ISettingsManager)
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
	}

public:

	STDMETHOD(get_MinLogLevel)(long* pVal);
	STDMETHOD(put_MinLogLevel)(long newVal);
	STDMETHOD(get_SymbolFilter)(BSTR* pVal);
	STDMETHOD(put_SymbolFilter)(BSTR newVal);
};

OBJECT_ENTRY_AUTO(__uuidof(SettingsManager), CSettingsManager)
