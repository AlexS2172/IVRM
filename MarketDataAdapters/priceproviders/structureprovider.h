// StructureProvider.h : Declaration of the CStructureProvider

#ifndef __STRUCTUREPROVIDER_H_
#define __STRUCTUREPROVIDER_H_

#include "resource.h"       // main symbols

class CStructureProvider;

class CStructureProviderNotify:
public IDispEventImpl<1, CStructureProviderNotify, &DIID__IStructureProviderEvents, (const GUID*)&LIBID_PRICEPROVIDERSLib>
{
public:

BEGIN_SINK_MAP (CStructureProviderNotify)
SINK_ENTRY_INFO(1, DIID__IStructureProviderEvents, 1, OnStock, &m_ArrivedStock)
SINK_ENTRY_INFO(1, DIID__IStructureProviderEvents, 2, OnOption,&m_ArrivedOption)
SINK_ENTRY_INFO(1, DIID__IStructureProviderEvents, 3, OnError, &m_ArrivedError)
SINK_ENTRY_INFO(1, DIID__IStructureProviderEvents, 4, OnFuture, &m_ArrivedFuture)
SINK_ENTRY_INFO(1, DIID__IStructureProviderEvents, 5, OnFutureByRoot, &m_ArrivedFutureByRoot)
SINK_ENTRY_INFO(1, DIID__IStructureProviderEvents, 6, OnFuturesOption, &m_ArrivedFuturesOption)
END_SINK_MAP()

	CStructureProviderNotify(CStructureProvider* pClient):m_pClient(pClient){};
	virtual ~CStructureProviderNotify()
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
	STDMETHOD(OnStock)(StockParams *Params, StockResults *Results);
	STDMETHOD(OnOption)(OptionParams *Params, OptionResults *Results, VARIANT_BOOL bLast);
	STDMETHOD(OnFuture)(FutureParams *Params, FutureResults *Results);
	STDMETHOD(OnFutureByRoot)(FutureRootParams *Params, FutureResults *Results, VARIANT_BOOL bLast);
	STDMETHOD(OnFuturesOption)(FuturesOptionParams *Params, FuturesOptionResults *Results, VARIANT_BOOL bLast);

private:
	CStructureProvider*	         m_pClient;	

	CComPtr<IStructureProvider>  m_spEvent;

	static _ATL_FUNC_INFO m_ArrivedError;
	static _ATL_FUNC_INFO m_ArrivedStock;
	static _ATL_FUNC_INFO m_ArrivedOption;
	static _ATL_FUNC_INFO m_ArrivedFuture;
	static _ATL_FUNC_INFO m_ArrivedFutureByRoot;
	static _ATL_FUNC_INFO m_ArrivedFuturesOption;
};

/////////////////////////////////////////////////////////////////////////////
// CStructureProvider
class ATL_NO_VTABLE CStructureProvider : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CStructureProvider, &CLSID_StructureProvider>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CStructureProvider>,
	public IDispatchImpl<IStructureProvider2, &IID_IStructureProvider2, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_PRICEPROVIDERSLib>,
	IConnectionPointImpl<CStructureProvider, &DIID__IStructureProviderEvents, CComDynamicUnkArray>,
	public IDispatchImpl<IProvider, &IID_IProvider, &LIBID_PRICEPROVIDERSLib>
{
public:
	CStructureProvider():
	  m_ProviderType(-1),
	  m_bConnected(false),
	  m_Notify(this),
  	  m_bRecursive(true)
	{
	}

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	void FinalRelease()
	{
		if(m_spStructureProvider!=NULL)
		{
			Disconnect();
			m_Notify.Destroy();
		}
	}
DECLARE_REGISTRY_RESOURCEID(IDR_STRUCTUREPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStructureProvider)
	COM_INTERFACE_ENTRY(IStructureProvider2)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY2(IStructureProvider, IStructureProvider2)
	COM_INTERFACE_ENTRY2(IDispatch, IStructureProvider2)
	COM_INTERFACE_ENTRY(IProvider)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CStructureProvider)
	CONNECTION_POINT_ENTRY(DIID__IStructureProviderEvents)
END_CONNECTION_POINT_MAP()

// IStructureProvider2
public:
	STDMETHOD(get_ProgID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ProviderID)(/*[out, retval]*/ long *pVal);

	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	STDMETHOD(RequestFuture)(FutureParams *Params);
	STDMETHOD(CancelFuture)(VARIANT Params);
	STDMETHOD(RequestFutures)(FutureRootParams *Params);
	STDMETHOD(CancelFutures)(VARIANT Params);
	STDMETHOD(RequestFuturesOptions)(FuturesOptionParams *Params);
	STDMETHOD(CancelFuturesOptions)(VARIANT Params);	

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
					m_spStructureProvider2 = NULL;
					m_spStructureProvider = NULL;
					m_bConnected = false;
					m_ProviderType = Val;
				
					if(m_ProviderType<0)
						m_spStructureProvider = NULL;
					else
					{
						_bstr_t bsProgId;
						if(g_Providers.GetProgID(m_ProviderType, enStructureProvider, bsProgId))
						{
							try
							{
								CLSID cls;
								CLSIDFromProgID(bsProgId, &cls);

								hr = m_spStructureProvider.CreateInstance(cls);								
								m_spStructureProvider2 = m_spStructureProvider;
							}catch(...){}
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
			hr = utils::ComError2ErrInfo (e, this);	
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
							m_bConnected = true;
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
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	bool IsHandledError(HRESULT hr)
	{
		return  HRESULT_FROM_WIN32(RPC_S_CALL_FAILED_DNE) == hr || 
				HRESULT_FROM_WIN32(RPC_S_CALL_FAILED) == hr || 
				HRESULT_FROM_WIN32(RPC_S_SERVER_UNAVAILABLE) == hr;
	}

	HRESULT OnStock(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	HRESULT OnFuture(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnFutureByRoot(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastFutures);
	HRESULT OnFuturesOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);

private:
	long					m_ProviderType;
	IStructureProviderPtr	m_spStructureProvider;
	IStructureProvider2Ptr	m_spStructureProvider2;
	bool					m_bConnected;
	bool					m_bRecursive;

	CStructureProviderNotify   m_Notify;
};

#endif //__STRUCTUREPROVIDER_H_
