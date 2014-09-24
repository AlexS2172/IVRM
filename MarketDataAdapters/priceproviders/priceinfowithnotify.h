// PriceInfoWithNotify.h : Declaration of the CPriceInfoWithNotify

#ifndef __PRICEINFOWITHNOTIFY_H_
#define __PRICEINFOWITHNOTIFY_H_

#include "resource.h"       // main symbols

struct SPriceRequestWithNotifyData
{
	CComVariant	m_vtRequest;
	vt_date		m_dtRequest;
	bool		m_bIsSubscribe;

	SPriceRequestWithNotifyData()
	{
		m_dtRequest = vt_date::GetCurrentDate();
		m_bIsSubscribe   = false;
	}
	SPriceRequestWithNotifyData(const SPriceRequestWithNotifyData& c){
		m_dtRequest = c.m_dtRequest;
		m_vtRequest = c.m_vtRequest;
		m_bIsSubscribe    = c.m_bIsSubscribe;
	}
	SPriceRequestWithNotifyData& operator=(const SPriceRequestWithNotifyData& c){
		m_dtRequest = c.m_dtRequest;
		m_vtRequest = c.m_vtRequest;
		m_bIsSubscribe    = c.m_bIsSubscribe;
		return *this;
	}
};

class CPriceRequestsWithNotifyHolder
{
public:
	typedef std::map<_bstr_t, SPriceRequestWithNotifyData> REQUEST;

	void AddRequest(QuoteUpdateParams* pParam, bool IsSubscribe)
	{
		_bstr_t bsKey = CreateName(pParam, IsSubscribe );

		_QuoteUpdateParams rec;
		rec.Init();
		rec->Symbol    = SysAllocString(pParam->Symbol);
		rec->Exchange  = SysAllocString(pParam->Exchange);
		rec->Type      = pParam->Type;
		

		SPriceRequestWithNotifyData data;
		rec.CopyTo(data.m_vtRequest);

		data.m_bIsSubscribe = IsSubscribe;

		CAutoLock lock(m_csRequest);
		m_Request[bsKey] = data;
	}


	void Clear()
	{
		CAutoLock lock(m_csRequest);
		m_Request.clear();
	}

	void Remove(const CComVariant& vtParam, bool IsSubscribe)
	{
		CAutoLock lock(m_csRequest);
		_QuoteUpdateParams param;
		if(FAILED(param.Init(vtParam)))
		{
			REQUEST::iterator iter = m_Request.begin();
			for(;iter!=m_Request.end(); iter++)
			{
				if(iter->second.m_bIsSubscribe == IsSubscribe)
				{
					m_Request.erase(iter);
					iter = m_Request.begin();
				}
			}
		}
		else
		{
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

	REQUEST               m_Request;
	CLock		      m_csRequest;
};

class CPriceInfoWithNotify;

class CPriceInfoWithNotifyEvent:
	public IDispEventImpl<1, CPriceInfoWithNotifyEvent, &DIID__IPriceInfoWithNotifyEvents, (const GUID*)&LIBID_PRICEPROVIDERSLib>
{
public:

	BEGIN_SINK_MAP (CPriceInfoWithNotifyEvent)
		SINK_ENTRY_INFO(1, DIID__IPriceInfoWithNotifyEvents, 1, OnLastQuote, &m_ArrivedLastQuote)
		SINK_ENTRY_INFO(1, DIID__IPriceInfoWithNotifyEvents, 2, OnQuoteUpdate,&m_ArrivedQuoteUpdate)
		SINK_ENTRY_INFO(1, DIID__IPriceInfoWithNotifyEvents, 3, OnError, &m_ArrivedError)
		SINK_ENTRY_INFO(1, DIID__IPriceInfoWithNotifyEvents, 4, OnSubscribed, &m_ArrivedSubscribed)
		SINK_ENTRY_INFO(1, DIID__IPriceInfoWithNotifyEvents, 5, OnUnsubscribed, &m_ArrivedUnsubscribed)
	END_SINK_MAP()

	CPriceInfoWithNotifyEvent(CPriceInfoWithNotify* pClient):m_pClient(pClient){};
	virtual ~CPriceInfoWithNotifyEvent()
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
	STDMETHOD(OnQuoteUpdate)(QuoteUpdateParams *Params, QuoteUpdateInfo *Results);
	STDMETHOD(OnSubscribed)(QuoteUpdateParams *Params);
	STDMETHOD(OnUnsubscribed)();


private:
	CPriceInfoWithNotify*	         m_pClient;	

	CComPtr<IPriceProvider>  m_spEvent;

	static _ATL_FUNC_INFO m_ArrivedError;
	static _ATL_FUNC_INFO m_ArrivedLastQuote;
	static _ATL_FUNC_INFO m_ArrivedQuoteUpdate;
	static _ATL_FUNC_INFO m_ArrivedSubscribed;
	static _ATL_FUNC_INFO m_ArrivedUnsubscribed;
};

/////////////////////////////////////////////////////////////////////////////
// CPriceInfoWithNotify
class ATL_NO_VTABLE CPriceInfoWithNotify : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPriceInfoWithNotify, &CLSID_PriceInfoWithNotify>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CPriceInfoWithNotify>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IGroupPriceWithNotify, &IID_IGroupPriceWithNotify, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IProvider, &IID_IProvider, &LIBID_PRICEPROVIDERSLib>,
	public IConnectionPointImpl<CPriceInfoWithNotify, &DIID__IPriceInfoWithNotifyEvents, CComDynamicUnkArray>
{
public:
	CPriceInfoWithNotify():
		m_ProviderType(-1),
		m_bConnected(false),
		m_Notify(this),
		m_bRecursive(true)
	  {
	  }

	  DECLARE_REGISTRY_RESOURCEID(IDR_PRICEINFOWITHNOTIFY)

	  DECLARE_PROTECT_FINAL_CONSTRUCT()

	  BEGIN_COM_MAP(CPriceInfoWithNotify)
		  COM_INTERFACE_ENTRY(IProvider)
		  COM_INTERFACE_ENTRY(IPriceProvider)
		  COM_INTERFACE_ENTRY(IGroupPriceWithNotify)
		  COM_INTERFACE_ENTRY2(IDispatch, IGroupPriceWithNotify)
		  COM_INTERFACE_ENTRY(ISupportErrorInfo)
		  COM_INTERFACE_ENTRY(IConnectionPointContainer)
		  COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	  END_COM_MAP()

	  BEGIN_CONNECTION_POINT_MAP(CPriceInfoWithNotify)
		  CONNECTION_POINT_ENTRY(DIID__IPriceInfoWithNotifyEvents)
	  END_CONNECTION_POINT_MAP()
	  HRESULT FinalConstruct()
	  {
		  m_hStop.Attach(CreateEvent(NULL, TRUE, FALSE, NULL));
		  return S_OK;
	  }

	  void FinalRelease()
	  {
		  SetEvent(m_hStop);
		  if((HANDLE)m_hThread != NULL)
		  {
			  WaitWithEvents(m_hThread);
			  m_hThread.Close();
		  }
		  if(m_spPriceProvider!=NULL)
		  {
			  Disconnect();
			  m_Notify.Destroy();
		  }
		  CloseHandle(m_hStop);
	  }

	  HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	  HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);
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
						if(g_Providers.GetProgID(m_ProviderType, enPriceInfoWithNotify, bsProgId))
						{
							CHandle hPriceProviderCreationMutex(::CreateMutex(NULL,FALSE, "Global\\C5E2FA75-83F2-476c-8158-166B884B0175"));
							try
							{
								HANDLE hEvents[] = {hPriceProviderCreationMutex, m_hStop};
								if(WAIT_OBJECT_0 == WaitWithEvents(2, hEvents))
								{
									CLSID cls;
									CLSIDFromProgID(bsProgId, &cls);
									hr = m_spPriceProvider.CreateInstance(cls);
								}
							}
							catch (...) {}
							if(hPriceProviderCreationMutex)
								ReleaseMutex(hPriceProviderCreationMutex);
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
			hr =utils::ComError2ErrInfo (e,this);	
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
	CPriceInfoWithNotifyEvent		m_Notify;
	bool							m_bRecursive;
	CPriceRequestsWithNotifyHolder	m_Requests;

	CHandle							m_hConnected;
	CHandle							m_hStop;
	CHandle							m_hThread;
	CHandle							m_PriceProviderCreationMutex;	

	static CLock				    m_scReconnect;

	bool IsHandledError(HRESULT hr)
	{
		return  0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr;
	}

	static UINT WINAPI         _ThreadReconnection(LPVOID pData);

};

#endif //__PRICEINFOWITHNOTIFY_H_
