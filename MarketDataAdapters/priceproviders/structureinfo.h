// StructureInfo.h : Declaration of the CStructureInfo

#ifndef __STRUCTUREINFO_H_
#define __STRUCTUREINFO_H_

#include "resource.h"       // main symbols

class CStructureInfo;
struct SStructureRequestData
{
	CComVariant m_vtRequest;
	vt_date     m_dtRequest;
	bool        m_bValue;
	enum enReqType { enNone, enStock, enOption} m_enType;

	SStructureRequestData()
	{
		m_dtRequest = vt_date::GetCurrentDate();
		m_bValue    = false;
		m_enType   =  enNone;
	}
	SStructureRequestData(const SStructureRequestData& c){
		m_dtRequest = c.m_dtRequest;
		m_bValue    = c.m_bValue;
		m_vtRequest = c.m_vtRequest;
		m_enType    = c.m_enType;
	}
	SStructureRequestData& operator=(const SStructureRequestData& c){
		m_dtRequest = c.m_dtRequest;
		m_bValue    = c.m_bValue;
		m_vtRequest = c.m_vtRequest;
		m_enType    = c.m_enType;
		return *this;
	}
};


class CStructureRequestsHolder
{
public:
	typedef std::map<_bstr_t, SStructureRequestData> REQUEST;

	void Add(StockParams* pParam)
	{
		_bstr_t bsKey = pParam->Stock;
		bsKey+=L".";
		bsKey+= pParam->Exchange;
		bsKey+=L"_1";

		_StockParams rec;
		rec.Init();
		
		rec->Stock     =SysAllocString(pParam->Stock);
		
		rec->Exchange  =SysAllocString(pParam->Exchange);
	/*
		if (wcsncmp((wchar_t *)(rec->Exchange), (wchar_t *)("G"), 1) == 0)
		{
			
			CComBSTR cbsTmp;
			cbsTmp.Append(pParam->Stock);
			cbsTmp.Append(_T("G"));
			rec->Stock    = SysAllocString(cbsTmp.Detach());

		}
		else
		{
			rec->Stock    = SysAllocString(pParam->Stock);
		
		}
*/	
		SStructureRequestData data;
		rec.CopyTo(data.m_vtRequest);
		data.m_enType = SStructureRequestData::enStock;

		CAutoLock lock(m_csRequest);
		m_Request[bsKey] = data;
	}

	void Add(OptionParams* pParam)
	{
		_bstr_t bsKey = pParam->UnderlyingSymbol;
		bsKey+=L".";
		bsKey+= pParam->Exchange;
		bsKey+=L"_2";

		_OptionParams rec;
		CComVariant vt;
		rec.Init();
		rec->UnderlyingSymbol     =SysAllocString(pParam->UnderlyingSymbol);
		rec->Exchange  =SysAllocString(pParam->Exchange);
/*
		if (wcsncmp((wchar_t *)(rec->Exchange), (wchar_t *)("G"), 1) == 0)
		{
			CComBSTR cbsTmp;
			cbsTmp.Append(pParam->UnderlyingSymbol);
			cbsTmp.Append(_T("G"));
			rec->UnderlyingSymbol    = SysAllocString(cbsTmp.Detach());
		}
		else
		{
			rec->UnderlyingSymbol    = SysAllocString(pParam->UnderlyingSymbol);
		}
*/
		SStructureRequestData data;
		rec.CopyTo(data.m_vtRequest);
		data.m_enType = SStructureRequestData::enOption;

		CAutoLock lock(m_csRequest);
		m_Request[bsKey] = data;
	}

	void Remove(const CComVariant& vtParam)
	{
		RemoveStock(vtParam);
		RemoveOption(vtParam);
	}

	void RemoveStock(const CComVariant& vtParam)
	{
		_StockParams param;
		if(FAILED(param.Init(vtParam)))
			return;

		_bstr_t bsKey = param->Stock;
		bsKey+=L".";
		bsKey+= param->Exchange;
		bsKey+=L"_1";


		CAutoLock lock(m_csRequest);
		m_Request.erase(bsKey);

	}
	void RemoveOption(const CComVariant& vtParam)
	{
		_OptionParams param;
		if(FAILED(param.Init(vtParam)))
			return;

		_bstr_t bsKey = param->UnderlyingSymbol;
		bsKey+=L".";
		bsKey+= param->Exchange;
		bsKey+=L"_2";

		CAutoLock lock(m_csRequest);
		m_Request.erase(bsKey);

	}
	void RemoveAll()
	{
		CAutoLock lock(m_csRequest);
		m_Request.clear();
	}


	REQUEST                  m_Request;
	CLock					 m_csRequest;

};

class CStructureInfoNotify:
public IDispEventImpl<1, CStructureInfoNotify, &DIID__IMyTrackStructureInfoEvents, (const GUID*)&LIBID_PRICEPROVIDERSLib>
{
public:

BEGIN_SINK_MAP (CStructureInfoNotify)
SINK_ENTRY_INFO(1, DIID__IMyTrackStructureInfoEvents, 1, OnStock, &m_ArrivedStock)
SINK_ENTRY_INFO(1, DIID__IMyTrackStructureInfoEvents, 2, OnOption,&m_ArrivedOption)
SINK_ENTRY_INFO(1, DIID__IMyTrackStructureInfoEvents, 3, OnError, &m_ArrivedError)
END_SINK_MAP()

	CStructureInfoNotify(CStructureInfo* pClient):m_pClient(pClient){};
	virtual ~CStructureInfoNotify()
	{
		Destroy();
	}

	HRESULT Initialize(IStructureProvider* pClient)
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
	STDMETHOD(OnStock)(StockParams *Params, StockInfo *Results);
	STDMETHOD(OnOption)(OptionParams *Params, OptionInfo *Results, VARIANT_BOOL bLast);

private:
	CStructureInfo*	         m_pClient;	

	CComPtr<IStructureProvider>  m_spEvent;

	static _ATL_FUNC_INFO m_ArrivedError;
	static _ATL_FUNC_INFO m_ArrivedStock;
	static _ATL_FUNC_INFO m_ArrivedOption;

};


/////////////////////////////////////////////////////////////////////////////
// CStructureInfo
class ATL_NO_VTABLE CStructureInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CStructureInfo, &CLSID_StructureInfo>,
	public ISupportErrorInfoImpl<&IID_IStructureProvider>,
	public IConnectionPointContainerImpl<CStructureInfo>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_PRICEPROVIDERSLib>,
	IConnectionPointImpl<CStructureInfo, &DIID__IMyTrackStructureInfoEvents, CComDynamicUnkArray>,
	public IDispatchImpl<IProvider, &IID_IProvider, &LIBID_PRICEPROVIDERSLib>
{
public:
	CStructureInfo():
	  m_ProviderType(-1),
	  m_bConnected(false),
	  m_Notify(this),
	  m_bRecursive(true),
	  m_hStop(INVALID_HANDLE_VALUE),
	  m_hConnected(INVALID_HANDLE_VALUE)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STRUCTUREINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStructureInfo)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY2(IDispatch, IStructureProvider)
	COM_INTERFACE_ENTRY(IProvider)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CStructureInfo)
	CONNECTION_POINT_ENTRY(DIID__IMyTrackStructureInfoEvents)
END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct()
	{
		m_hStop = CreateEvent(NULL, TRUE, FALSE, NULL);
		return S_OK;
	}
	void FinalRelease()
	{
		SetEvent(m_hStop);
		Sleep(0);
		if(m_spStructureProvider!=NULL)
		{
			Disconnect();
			m_Notify.Destroy();
		}
	}

	HRESULT OnStock(const CComVariant &varParams,const  CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams,const  CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
						RequestsTypeEnum enumRequest,const CComVariant &varRequest);

// IATFStructureInfo
public:

	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	
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
					m_spStructureProvider = NULL;
					m_bConnected = false;
					m_ProviderType = Val;
				
					if(m_ProviderType<0)
						m_spStructureProvider = NULL;
					else
					{
						_bstr_t bsProgId;
						if(g_Providers.GetProgID(m_ProviderType, enStructureInfo, bsProgId))
						{

							try
							{
								CLSID cls;
								CLSIDFromProgID(bsProgId, &cls);
								hr = m_spStructureProvider.CreateInstance(cls);
							}
							catch (...) {	}
						}
						else
							hr = Error(_T("Unknown Provider type"), IID_IPriceProvider, E_INVALIDARG);
					}

					if(SUCCEEDED(hr) && m_spStructureProvider!=NULL)
						m_Notify.Initialize(m_spStructureProvider);
				}
			}

		}
		catch (_com_error& e)
		{
			hr = utils::ComError2ErrInfo (e,this);	
		}
		catch (...)
		{
			hr = Error(L"Unknown Error",__uuidof(IStructureProvider), E_FAIL);
		}
		return hr;
	}

	HRESULT Reconnect()
	{
		HRESULT hr = S_OK;
		CAutoLock r(m_csReconnect);
		try 
		{
			m_bConnected = false;
			if(m_bRecursive)
			{
				long enTmp = m_ProviderType;			
				hr = put_Type(-1);
				if(SUCCEEDED(hr))
				{
					hr = put_Type(enTmp);
					if(SUCCEEDED(hr))
					{
						hr = m_spStructureProvider->Connect();
						if(SUCCEEDED(hr))
						{
							m_bConnected = true;
							CAutoLock lock(m_Requests.m_csRequest);
							CStructureRequestsHolder::REQUEST::iterator iter;
							for(iter = m_Requests.m_Request.begin(); iter!=m_Requests.m_Request.end(); iter++)
							{
								switch(iter->second.m_enType)
								{
								case SStructureRequestData::enStock:
									{
										_StockParams param(iter->second.m_vtRequest);
										StockParams* pParam = reinterpret_cast<StockParams*>((StockParams*)param);

										m_spStructureProvider->RequestStock(pParam);									
									}break;
								case SStructureRequestData::enOption:
									{
										_OptionParams param(iter->second.m_vtRequest);
										OptionParams* pParam = reinterpret_cast<OptionParams*>((OptionParams*)param);

										m_spStructureProvider->RequestOptions(pParam);									


									}break;
								}
							}
						}
					}
				}
			}
		}
		catch(_com_error& err)
		{
			return utils::ComError2ErrInfo (err,this);		
		}
		return hr;
	}

private:
	static UINT WINAPI         _ThreadReconnection(LPVOID pData);
	static CLock			   m_csReconnect;
	HANDLE                     m_hStop;
	HANDLE					   m_hConnected;	

	long					   m_ProviderType;
	IStructureProviderPtr	   m_spStructureProvider;
	bool					   m_bConnected;
	CStructureInfoNotify	   m_Notify;
	bool					   m_bRecursive;
	CStructureRequestsHolder   m_Requests;

//	DWORD WaitWithEvents(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE, BOOL bMode = FALSE)
//	{
//		DWORD dwStatus = -1;
//		ATLASSERT(nCount > 0);
//		for(;;)
//		{
//			dwStatus = ::MsgWaitForMultipleObjects(nCount,pHandles,bMode,dwMilliseconds,QS_ALLEVENTS);
//			
//			if(dwStatus != WAIT_OBJECT_0+nCount) break;
//			
//			MSG msg;
//			while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
//			{
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//			}
//		}
//		return dwStatus;		
//	}
//	DWORD WaitWithEvents(HANDLE &hObject, long timeout = INFINITE)
//	{
//		DWORD dwRet = WAIT_FAILED;
//		if (hObject==INVALID_HANDLE_VALUE)
//			return dwRet;
//		MSG msg;	
//		while(1)
//		{
//			dwRet = MsgWaitForMultipleObjects( 1,&hObject,FALSE,timeout,QS_ALLEVENTS);
//			if(dwRet == WAIT_OBJECT_0)
//				return dwRet;
//			else 
//			if(dwRet == WAIT_OBJECT_0 + 1)
//			{
//				while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
//				{
//					TranslateMessage(&msg);
//					DispatchMessage(&msg);
//				}
//			} 
//			else
//				return dwRet;
//		}
//	};

	bool IsHandledError(HRESULT hr)
	{
		return  0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr;
	}
	
};

#endif //__STRUCTUREINFO_H_
