// BatchPriceInfo.h : Declaration of the CBatchPriceInfo

#ifndef __BATCHPRICEINFO_H_
#define __BATCHPRICEINFO_H_

#include "resource.h"       // main symbols

struct SBatchPriceInfoRequest
{
	CComVariant	m_vtRequest;
	bool		m_bIsSubscribe;

	SBatchPriceInfoRequest()
	{
		m_dtRequest = vt_date::GetCurrentDate();
		m_bIsSubscribe   = false;
	}
	SBatchPriceInfoRequest(const SBatchPriceInfoRequest& c){
		m_dtRequest = c.m_dtRequest;
		m_vtRequest = c.m_vtRequest;
		m_bIsSubscribe    = c.m_bIsSubscribe;
	}
	SBatchPriceInfoRequest& operator=(const SBatchPriceInfoRequest& c){
		m_dtRequest = c.m_dtRequest;
		m_vtRequest = c.m_vtRequest;
		m_bIsSubscribe    = c.m_bIsSubscribe;
		return *this;
	}
	vt_date GetDtRequest(){
		return m_dtRequest;
	}
	void    SetDtRequest(vt_date dt)
	{
		m_dtRequest = dt;
	}
private:
	vt_date		m_dtRequest;

};

class CBatchPriceInfoRequestsHolder
{
public:
	typedef std::map<_bstr_t, SBatchPriceInfoRequest> REQUEST;

	void AddRequest(QuoteUpdateParams* pParam, bool IsSubscribe)
	{
		_bstr_t bsKey = CreateName(pParam, IsSubscribe );

		_QuoteUpdateParams rec;
		rec.Init();
		rec->Symbol    = SysAllocString(pParam->Symbol);
		rec->Exchange  = SysAllocString(pParam->Exchange);
		rec->Type      = pParam->Type;
		
		SBatchPriceInfoRequest data;
		rec.CopyTo(data.m_vtRequest);

		data.m_bIsSubscribe = IsSubscribe;

		CAutoLock cs(m_csRequest);
		m_Request[bsKey] = data;
	}


	void Clear()
	{
		CAutoLock cs(m_csRequest);
		m_Request.clear();
	}

	void Remove(const CComVariant& vtParam, bool IsSubscribe)
	{
		CAutoLock cs(m_csRequest);
		_QuoteUpdateParams param;
		if(FAILED(param.Init(vtParam))) 	{
			REQUEST::iterator iter = m_Request.begin();
			for(;iter!=m_Request.end(); )	{
				if(iter->second.m_bIsSubscribe == IsSubscribe)
					iter = m_Request.erase(iter);
				else
					iter++;
			}
		}
		else {
			_bstr_t bsKey = CreateName(param, IsSubscribe);
			m_Request.erase(bsKey);
		}
	}


	_bstr_t CreateName(QuoteUpdateParams* param, bool IsSubscribe)
	{
		_bstr_t bsKey = param->Symbol;

		switch(param->Type) 
		{
			case enSTK:
				bsKey+=L"_STK_";
				break;
			case enFUT:
				bsKey+=L"_FUT_";
				break;
			case enOST:
				bsKey+=L"_OST_";
				break;
			case enOFT:
				bsKey+=L"_OFT_";
				break;
			case enIDX:
				bsKey+=L"_IDX_";
				break;
			case enOPT:
				bsKey+=L"_OPT_";
				break;
			case enGrSTK:
				bsKey+=L"_GrSTK_";
				break;
			case enGrIDX:
				bsKey+=L"_GrIDX_";
				break;
		}

		bsKey+= param->Exchange;

		if(IsSubscribe)
			bsKey += L"_1";
		else
			bsKey += L"_0";
	
		return bsKey;
	}

	REQUEST                  m_Request;
	CLock					 m_csRequest;
};

class CBatchPriceInfo;

class CBatchPriceInfoEvent:
	public IDispEventImpl<1, CBatchPriceInfoEvent, &DIID__IBatchPriceInfoEvents, (const GUID*)&LIBID_PRICEPROVIDERSLib>
{
public:

	BEGIN_SINK_MAP (CBatchPriceInfoEvent)
		SINK_ENTRY_INFO(1, DIID__IBatchPriceInfoEvents, 1, OnLastQuote, &m_ArrivedLastQuote)
		SINK_ENTRY_INFO(1, DIID__IBatchPriceInfoEvents, 2, OnQuoteUpdate, &m_ArrivedQuoteUpdate)
		SINK_ENTRY_INFO(1, DIID__IBatchPriceInfoEvents, 3, OnError, &m_ArrivedError)
		SINK_ENTRY_INFO(1, DIID__IBatchPriceInfoEvents, 4, OnSubscribed, &m_ArrivedSubscribed)
		SINK_ENTRY_INFO(1, DIID__IBatchPriceInfoEvents, 5, OnUnsubscribed, &m_ArrivedUnsubscribed)
	END_SINK_MAP()

	CBatchPriceInfoEvent(CBatchPriceInfo* pClient):m_pClient(pClient){};
	virtual ~CBatchPriceInfoEvent()
	{
		Destroy();
	}

	HRESULT Initialize(IPriceProvider* pClient)
	{
		HRESULT hr = E_FAIL;
		if(pClient)
		{
			m_spEvent = pClient;
			hr = DispEventAdvise(m_spEvent);
		}
		return hr;
	}
	void    Destroy()
	{
		if (m_spEvent)
		{
			DispEventUnadvise (m_spEvent);
			m_spEvent = NULL;
		}

	}

	STDMETHOD(OnError)(ErrorNumberEnum ErrorNumber, BSTR Description, RequestsTypeEnum ReqType, VARIANT Request);
	STDMETHOD(OnLastQuote)(QuoteUpdateParams *Params, QuoteUpdateInfo *Results);
	STDMETHOD(OnQuoteUpdate)();
	STDMETHOD(OnSubscribed)(QuoteUpdateParams *Params);
	STDMETHOD(OnUnsubscribed)();

private:
	CBatchPriceInfo*			m_pClient;	

	CComPtr<IPriceProvider>		m_spEvent;

	static _ATL_FUNC_INFO m_ArrivedError;
	static _ATL_FUNC_INFO m_ArrivedLastQuote;
	static _ATL_FUNC_INFO m_ArrivedQuoteUpdate;
	static _ATL_FUNC_INFO m_ArrivedSubscribed;
	static _ATL_FUNC_INFO m_ArrivedUnsubscribed;
};

/////////////////////////////////////////////////////////////////////////////
// CBatchPriceInfo
class ATL_NO_VTABLE CBatchPriceInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBatchPriceInfo, &CLSID_BatchPriceInfo>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CBatchPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IBatchPriceProvider, &IID_IBatchPriceProvider, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IGroupPriceWithNotify, &IID_IGroupPriceWithNotify, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IProvider, &IID_IProvider, &LIBID_PRICEPROVIDERSLib>,
	public IConnectionPointImpl<CBatchPriceInfo, &DIID__IBatchPriceInfoEvents, CComDynamicUnkArray>
{
public:
	CBatchPriceInfo()
	  :m_ProviderType(-1)
	  ,m_bConnected(false)
	  ,m_Notify(this)
	  ,m_bRecursive(true)
	  ,m_dtLastActivity(vt_date::GetCurrentDate())
	  ,m_bInProcClient(false)
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_BATCHPRICEINFO)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CBatchPriceInfo)
		COM_INTERFACE_ENTRY(IProvider)
		COM_INTERFACE_ENTRY(IPriceProvider)
		COM_INTERFACE_ENTRY(IBatchPriceProvider)
		COM_INTERFACE_ENTRY2(IDispatch, IBatchPriceProvider)
		COM_INTERFACE_ENTRY(IGroupPriceWithNotify)
		COM_INTERFACE_ENTRY2(IDispatch, IGroupPriceWithNotify)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CBatchPriceInfo)
		CONNECTION_POINT_ENTRY(DIID__IBatchPriceInfoEvents)
	END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct()
	{
		m_hStop.Attach(::CreateEvent(NULL, TRUE, FALSE, NULL));
		return S_OK;
	}

	void FinalRelease()
	{
		SetEvent(m_hStop);
		if(m_hThread != INVALID_HANDLE_VALUE && (HANDLE)m_hThread != NULL)
		{
			WaitWithEvents(m_hThread);
			m_hThread.Close();
		}
		if(m_spPriceProvider!=NULL)
		{
			Disconnect();
			m_Notify.Destroy();
		}
		m_hStop.Close();
	}

	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate();
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
		  RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	HRESULT OnSubscribed(const CComVariant &varRequest);
	HRESULT OnUnsubscribed();

	// IPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	// IBatchPriceProvider
public:
	STDMETHOD(get_IsQuotesUpdated)(VARIANT_BOOL *pVal);
	STDMETHOD(GetQuotesUpdates)(SAFEARRAY** pRetVal);
	STDMETHOD(RequestMultipleQuotes)  (SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams);
	STDMETHOD(SubscribeMultipleQuotes)(SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams);
	STDMETHOD(UnNotifiedSubscribtionQuantity)(VARIANT_BOOL bSkipNotification,  LONG* pVal);
	STDMETHOD(SetRegionRate)(/*[in]*/ BSTR bstrRegion, /*[in]*/ DOUBLE dRate);

public:
	// IGroupPriceWithNotify
	STDMETHOD(RequestLastGroupQuotes)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastGroupQuotes)(VARIANT Params);
	STDMETHOD(SubscribeGroupQuotes)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeGroupQuotes)(VARIANT Params);

	// IProvider
	STDMETHOD(get_Type)(long * pVal)
	{
		if (pVal == NULL)
			return E_POINTER;

		*pVal = m_ProviderType;
		return S_OK;
	}
	STDMETHOD(put_Type)(long Val)
	{
		HRESULT hr = S_OK;
		try
		{
			if(Val != m_ProviderType)
			{
				if(m_bConnected)
				{
					hr = Error(_T("Existing provider is active. Do disconnection procedure first"), IID_IPriceProvider, E_ACCESSDENIED);
				}
				else
				{
					m_Notify.Destroy();
					m_spPriceProvider = NULL;
					m_bConnected = false;
					m_ProviderType = Val;

					if(m_ProviderType<0)
						m_spPriceProvider = NULL;
					else
					{
						_bstr_t bsProgId;
						if(g_Providers.GetProgID(m_ProviderType, enBatchPriceInfo, bsProgId))
						{
							try{

								CLSID cls;
								CLSIDFromProgID(bsProgId, &cls);
								hr = m_spPriceProvider.CreateInstance(cls);
								if(SUCCEEDED(hr))
								{
									CComQIPtr<IUserInformation> spUserInfo(m_spPriceProvider);

									if(spUserInfo)
									{
										TCHAR buffer[1024];
										ULONG  lBuffer = sizeof(buffer)/sizeof(TCHAR);
										if(GetUserName(buffer, &lBuffer))
											spUserInfo->put_UserName(_bstr_t(buffer));

										VARIANT_BOOL vbInProc = VARIANT_FALSE;
										spUserInfo->get_InProc(&vbInProc);
										Sleep(10);
										m_bInProcClient = vbInProc!=VARIANT_FALSE;
									}
								}
							}catch (...) {}
						}
						else
							hr = Error(_T("Unknown Provider type"), IID_IPriceProvider, E_INVALIDARG);
					}

					if(SUCCEEDED(hr) && m_spPriceProvider!=NULL)
						m_Notify.Initialize(m_spPriceProvider);
				}
			}

		}
		catch (_com_error& e)
		{
			hr = utils::ComError2ErrInfo (e,this);	
		}
		catch (...)
		{
			hr = Error(L"Unknown Error",__uuidof(IPriceProvider), E_FAIL);
		}
		return hr;
	}

	HRESULT Reconnect();

private:
	bool							m_bConnected;
	long							m_ProviderType;
	IPriceProviderPtr				m_spPriceProvider;
	CBatchPriceInfoEvent			m_Notify;
	bool							m_bRecursive;
	CBatchPriceInfoRequestsHolder	m_Requests;

	CHandle							m_hConnected;
	CHandle							m_hStop;
	CHandle							m_hThread;
	CHandle							m_PriceProviderCreationMutex;	
	vt_date							m_dtLastActivity;
	bool							m_bInProcClient;


	static CLock					m_scReconnect;

	CLock                           m_csActivityLock;
public:
	vt_date_span GetActivity()
	{
		//CAutoLock lock(m_csActivityLock);
		return vt_date::GetCurrentDate() - m_dtLastActivity;
	}
	void SetActivity()
	{
		//CAutoLock lock(m_csActivityLock);
		m_dtLastActivity = vt_date::GetCurrentDate();
	}
private:
	DWORD WaitWithEvents(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE, BOOL bMode = FALSE)
	{
		DWORD dwStatus = -1;
		ATLASSERT(nCount > 0);
		for(;;)
		{
			dwStatus = ::MsgWaitForMultipleObjects(nCount,pHandles,bMode,dwMilliseconds,QS_ALLEVENTS);

			if(dwStatus != WAIT_OBJECT_0+nCount) break;

			MSG msg;
			while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return dwStatus;		
	}

	DWORD WaitWithEvents(HANDLE hObject, long timeout = INFINITE)
	{
		DWORD dwRet = WAIT_FAILED;
		if (hObject==INVALID_HANDLE_VALUE)
			return dwRet;
		MSG msg;	
		while(1)
		{
			dwRet = MsgWaitForMultipleObjects( 1,&hObject,FALSE,timeout,QS_ALLEVENTS);
			if(dwRet == WAIT_OBJECT_0)
				return dwRet;
			else 
				if(dwRet == WAIT_OBJECT_0 + 1)
				{
					while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				} 
				else
					return dwRet;
		}
	};

	bool IsHandledError(HRESULT hr)
	{
		return  0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr;
	}

	static UINT WINAPI         _ThreadReconnection(LPVOID pData);

};

#endif //__BATCHPRICEINFO_H_
