// ActivFeedStructureInfo.h : Declaration of the CActivFeedStructureInfo

#pragma once
#include "resource.h"       // main symbols

#include "ActivFeedProviders.h"
#include "ActivFeedAppication.h"
#include "CustomerInfo.h"

// CActivFeedStructureInfo

class ATL_NO_VTABLE CActivFeedStructureInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CActivFeedStructureInfo, &CLSID_ActivFeedStructureInfo>,
	public ISupportErrorInfoImpl<&__uuidof(IStructureProvider)>,
	public IConnectionPointContainerImpl<CActivFeedStructureInfo>,
	public IDispatchImpl<IStructureProvider, &__uuidof(IStructureProvider), &LIBID_ActivFeedProvidersLib>,
	public IDispatchImpl<IStructureProvider2, &__uuidof(IStructureProvider2), &LIBID_ActivFeedProvidersLib>,
	public IConnectionPointImpl<CActivFeedStructureInfo, &__uuidof(_IStructureProviderExEvents)/*, EgLib::CComDynamicUnkGITArray*/>,
	public CActivProviderBase
{
	AgentApplication::Settings  m_Settings;
	CActivFeedAppicationPtr     m_spActiv; 

public:
	//construction/destruction
	CActivFeedStructureInfo() {}
	HRESULT FinalConstruct()
	{
		ActivFeedSettings::CCustomerInfo::Initialize();
		ActivFeedSettings::CSettingsManagerPtr spSettingsManager = ActivFeedSettings::g_spUserSettings->Settings;
		m_Settings.m_systemName  = spSettingsManager->Settings["SystemName"];
		m_Settings.m_serviceLocationIniFile= spSettingsManager->Settings["ActivServiceINIFilePath"];
		m_Settings.m_memoryModel = Application::Settings::MEMORY_MODEL_PRIVATE;
		m_Settings.m_enableUiService = false;
		m_Settings.m_createSystemLog = false;
		m_Settings.m_systemLogMode = AgentApplication::Settings::SYSTEM_LOG_MODE_OFF;
		return S_OK;
	}
	void FinalRelease() { if( m_spActiv ) m_spActiv.reset(); }

	// IStructureProvider2
	STDMETHOD(CancelFuture)(VARIANT Params);
	STDMETHOD(CancelFutures)(VARIANT Params);
	STDMETHOD(CancelFuturesOptions)(VARIANT Params);	
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	STDMETHOD(RequestFuture)(FutureParams *Params);
	STDMETHOD(RequestFutures)(FutureRootParams *Params);
	STDMETHOD(RequestFuturesOptions)(FuturesOptionParams *Params);
	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);

	// IStructureProviderExEvents
	HRESULT	OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	HRESULT	OnFuturesOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT	OnFuture(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT	OnFutureByRoot(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastFutures);
	HRESULT	OnStock(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT	OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);

	DECLARE_REGISTRY_RESOURCEID(IDR_ACTIVFEEDSTRUCTUREINFO)


	BEGIN_COM_MAP(CActivFeedStructureInfo)
		COM_INTERFACE_ENTRY(IStructureProvider2)
		COM_INTERFACE_ENTRY2(IStructureProvider, IStructureProvider2)
		COM_INTERFACE_ENTRY2(IDispatch, IStructureProvider2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CActivFeedStructureInfo)
		CONNECTION_POINT_ENTRY(__uuidof(_IStructureProviderExEvents))
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
};

OBJECT_ENTRY_AUTO(__uuidof(ActivFeedStructureInfo), CActivFeedStructureInfo)
