// ActivFeedBatchPriceInfo.h : Declaration of the CActivFeedBatchPriceInfo

#pragma once
#include "resource.h"       // main symbols

#include "ActivFeedProviders.h"
#include "EgTracingClass.h"
#include "CustomerInfo.h"
#include "ActivFeedAppication.h"


class CActivFeedBatchPriceInfo;

class CActivTimer : public EgLib::CThread
{
	CActivFeedBatchPriceInfo	*m_pAFBPI;
	DWORD						 m_Delay;
	HANDLE						 m_hTimer;
public:
	CActivTimer( CActivFeedBatchPriceInfo* p, DWORD d) : m_Delay(d) {
		ATLASSERT(p);
		m_pAFBPI = p;
	}
	UINT	Run();
};
typedef boost::shared_ptr<CActivTimer>	ActivTimerPtr;

// CActivFeedBatchPriceInfo
typedef CComRecPtr<QuoteUpdateFullInfo, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateFullInfo;

class ATL_NO_VTABLE CActivFeedBatchPriceInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CActivFeedBatchPriceInfo, &CLSID_ActivFeedBatchPriceInfo>,
	public ISupportErrorInfoImpl<&__uuidof(IPriceProvider)>,
	public IConnectionPointContainerImpl<CActivFeedBatchPriceInfo>,
	public IDispatchImpl<IPriceProvider,					&__uuidof(IPriceProvider),		&LIBID_ActivFeedProvidersLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IBatchPriceProvider,				&__uuidof(IBatchPriceProvider), &LIBID_ActivFeedProvidersLib>,
	public IDispatchImpl<IUserInformation,					&__uuidof(IUserInformation),	&LIBID_ActivFeedProvidersLib>,
	public IConnectionPointImpl<CActivFeedBatchPriceInfo,	&__uuidof(_IBatchPriceInfoEvents)/*, EgLib::CComDynamicUnkGITArray*/>,
	public CActivProviderBase
{
public:
	CActivFeedBatchPriceInfo()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_ACTIVFEEDBATCHPRICEINFO)


	BEGIN_COM_MAP(CActivFeedBatchPriceInfo)
		COM_INTERFACE_ENTRY(IPriceProvider)
		COM_INTERFACE_ENTRY(IBatchPriceProvider)
		COM_INTERFACE_ENTRY2(IDispatch, IBatchPriceProvider)
		COM_INTERFACE_ENTRY(IUserInformation)
		COM_INTERFACE_ENTRY2(IDispatch, IUserInformation)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()


	BEGIN_CONNECTION_POINT_MAP(CActivFeedBatchPriceInfo)
		CONNECTION_POINT_ENTRY(__uuidof(_IBatchPriceInfoEvents))
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

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
		//m_UpdateTimer = 0;
		long ll = atol( spSettingsManager->Settings["LogLevel"].c_str() );
		EgLib::CEgLibTraceManager::SetMinLoggingLevel( ll );
		EgLib::CEgLibTraceManager::Trace( LogSystem, __FUNCTION__ , 
			_T("Log level: %d"), ll , __FILE__, __LINE__);
		return S_OK;
	}
	
	void FinalRelease() 
	{
		if(m_spActiv){
			try{
				m_spActiv.reset();
			}
			catch (...) {
				int i= 0;
				i++;
			}
		}
			
	}

public:
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate();
	HRESULT OnSubscribed(const CComVariant &varParams);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	void	OnRequstLast( QuoteUpdateParams& m_spRequest, QuoteUpdateInfo& m_spResponse );

public:

	// IPriceProvider
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	// IBatchPriceProvider
	STDMETHOD(get_IsQuotesUpdated)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(GetQuotesUpdates)(/*[out, retval]*/ SAFEARRAY** pRetVal);
	STDMETHOD(RequestMultipleQuotes)  (SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams);
	STDMETHOD(SubscribeMultipleQuotes)(SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams);
	STDMETHOD(UnNotifiedSubscribtionQuantity)(VARIANT_BOOL bSkipNotification,  LONG* pVal);
	STDMETHOD(SetRegionRate)(BSTR bstrRegion, DOUBLE dRate);
	//IUserInformation
	STDMETHOD(put_UserName)(BSTR userName);
    STDMETHOD(get_InProc)(VARIANT_BOOL* bMode)
	{
		if(bMode)
			*bMode = VARIANT_TRUE;
		return S_OK;
	}
private:
	//UINT	m_UpdateTimer;
	AgentApplication::Settings		m_Settings;
	CActivFeedAppicationPtr			m_spActiv; 
	ActivTimerPtr					m_pTimer;	
	friend class CActivTimer;  
};


OBJECT_ENTRY_AUTO(__uuidof(ActivFeedBatchPriceInfo), CActivFeedBatchPriceInfo)
