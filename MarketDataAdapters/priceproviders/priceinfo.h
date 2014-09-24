// PriceInfo.h : Declaration of the CPriceInfo

#ifndef __PRICEINFO_H_
#define __PRICEINFO_H_

#include "resource.h"       // main symbols

struct SPriceRequestData
{
	CComVariant m_vtRequest;
	vt_date     m_dtRequest;
	GroupRequestType m_enTypeGroupRequest;
	enum enReqType { enNone, enLast, enSubscribe, enGroup} m_enType;

	SPriceRequestData()
	{
		m_dtRequest = vt_date::GetCurrentDate();
		m_enTypeGroupRequest = enGroupRequestLastQuote;
		m_enType   =  enNone;
	}
	SPriceRequestData(const SPriceRequestData& c){
		m_dtRequest = c.m_dtRequest;
		m_enTypeGroupRequest    = c.m_enTypeGroupRequest;
		m_vtRequest = c.m_vtRequest;
		m_enType    = c.m_enType;
	}
	SPriceRequestData& operator=(const SPriceRequestData& c){
		m_dtRequest = c.m_dtRequest;
		m_vtRequest = c.m_vtRequest;
		m_enType    = c.m_enType;
		m_enTypeGroupRequest = c.m_enTypeGroupRequest;
		return *this;
	}
};

class CPriceRequestsHolder
{
public:
	typedef std::map<_bstr_t, SPriceRequestData> REQUEST;

	void AddRequest(SPriceRequestData::enReqType type, QuoteUpdateParams* pParam, GroupRequestType enOperationType = enGroupRequestLastQuote)
	{
		_bstr_t bsKey = CreateName(type, pParam);

		_QuoteUpdateParams rec;
		rec.Init();

		rec->Symbol    = SysAllocString(pParam->Symbol);
		rec->Exchange  = SysAllocString(pParam->Exchange);
		rec->Type      = pParam->Type;
		

		SPriceRequestData data;
		rec.CopyTo(data.m_vtRequest);

		data.m_enTypeGroupRequest = enOperationType;
		data.m_enType = type;

		CAutoLock cs(m_csRequest);
		m_Request[bsKey] = data;
	}


	void Clear()
	{
		CAutoLock cs(m_csRequest);
		m_Request.clear();
	}

	void Remove(SPriceRequestData::enReqType type, const CComVariant& vtParam)
	{
		CAutoLock cs(m_csRequest);
		_QuoteUpdateParams param;
		if(FAILED(param.Init(vtParam)))
		{
			REQUEST::iterator iter = m_Request.begin();
			for(;iter!=m_Request.end(); iter++)
			{
				if(iter->second.m_enType==type)
				{
					m_Request.erase(iter);
					iter = m_Request.begin();
				}
			}
		}
		else
		{
			_bstr_t bsKey = CreateName(type, param);
			m_Request.erase(bsKey);
		}
	}

	
	_bstr_t CreateName(SPriceRequestData::enReqType type, QuoteUpdateParams* param)
	{
		_bstr_t bsKey = param->Symbol;
		if(param->Type != enOPT)
			bsKey+=L".";
		else
			bsKey+=L"&";

		bsKey+= param->Exchange;
		switch(type)
		{
		case SPriceRequestData::enLast:
			bsKey += L"_1";
			break;
		case SPriceRequestData::enSubscribe:
			bsKey += L"_2";
			break;
		case SPriceRequestData::enGroup:
			bsKey += L"_3";
			break;
		}
		return bsKey;
	}

	REQUEST           m_Request;
	CLock			  m_csRequest;

};

class CPriceInfo;

class CPriceInfoNotify:
public IDispEventImpl<1, CPriceInfoNotify, &DIID__IPriceInfoEvents, (const GUID*)&LIBID_PRICEPROVIDERSLib>
{
public:

BEGIN_SINK_MAP (CPriceInfoNotify)
SINK_ENTRY_INFO(1, DIID__IPriceInfoEvents, 1, OnLastQuote, &m_ArrivedLastQuote)
SINK_ENTRY_INFO(1, DIID__IPriceInfoEvents, 2, OnQuoteUpdate,&m_ArrivedQuoteUpdate)
SINK_ENTRY_INFO(1, DIID__IPriceInfoEvents, 3, OnError, &m_ArrivedError)
END_SINK_MAP()

	CPriceInfoNotify(CPriceInfo* pClient):m_pClient(pClient){};
	virtual ~CPriceInfoNotify()
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

private:
	CPriceInfo*	         m_pClient;	

	CComPtr<IPriceProvider>  m_spEvent;

	static _ATL_FUNC_INFO m_ArrivedError;
	static _ATL_FUNC_INFO m_ArrivedLastQuote;
	static _ATL_FUNC_INFO m_ArrivedQuoteUpdate;

};
 
/////////////////////////////////////////////////////////////////////////////
// CPriceInfo
class ATL_NO_VTABLE CPriceInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPriceInfo, &CLSID_PriceInfo>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IProvider, &IID_IProvider, &LIBID_PRICEPROVIDERSLib>,
	public IConnectionPointImpl<CPriceInfo, &DIID__IPriceInfoEvents, CComDynamicUnkArray>,
	public EgLib::CThread
{
public:
	CPriceInfo()
	  : m_ProviderType(-1)
	  ,m_bConnected(false)
	  ,m_Notify(this)
	  ,m_bRecursive(true)
	  ,m_dtLastActivity(vt_date::GetCurrentDate())
	{
//		m_f = fopen("C:\\TempLog.txt", "w");
	}

	~CPriceInfo()
	{
//		fprintf(m_f,"Destructor\n");
//		fflush(m_f);
//		fclose(m_f);
	}

	//FILE* m_f;

DECLARE_REGISTRY_RESOURCEID(IDR_PRICEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPriceInfo)
	COM_INTERFACE_ENTRY(IProvider)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPrice)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPriceInfo)
	CONNECTION_POINT_ENTRY(DIID__IPriceInfoEvents)
END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		EgLib::CThread::Stop(true, 1000);
		if(m_spPriceProvider!=NULL)
		{
			Disconnect();
			m_Notify.Destroy();
		}
	}

	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);

// IPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	
public:
	UINT Run();

public:
// IGroupPrice
	STDMETHOD(RequestGroup)(QuoteUpdateParams * Params, GroupRequestType enTypeGroupRequest);
	STDMETHOD(CancelGroup)(VARIANT Params);

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
						if(g_Providers.GetProgID(m_ProviderType, enPriceInfo, bsProgId))
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
										{
											_bstr_t bsName(L"*");
											bsName += buffer;
											spUserInfo->put_UserName(bsName);
										}
										Sleep(100);
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
	bool                   m_bConnected;
	long			       m_ProviderType;
	IPriceProviderPtr m_spPriceProvider;
	CPriceInfoNotify       m_Notify;
	bool			       m_bRecursive;
	CPriceRequestsHolder   m_Requests;

	CHandle                m_hConnected;

	CLock				   m_scReconnect;
	vt_date				   m_dtLastActivity;

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
	CLock                           m_csActivityLock;

public:
		vt_date_span GetActivity()
		{
			CAutoLock lock(m_csActivityLock);
			return vt_date::GetCurrentDate() - m_dtLastActivity;
		}
		void SetActivity()
		{
			CAutoLock lock(m_csActivityLock);
			m_dtLastActivity = vt_date::GetCurrentDate();
		}
private:

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

#endif //__PRICEINFO_H_
