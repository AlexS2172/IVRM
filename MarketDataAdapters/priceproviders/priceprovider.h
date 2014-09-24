// PriceProvider.h : Declaration of the CPriceProvider

#ifndef __PRICEPROVIDER_H_
#define __PRICEPROVIDER_H_

#include "resource.h"       // main symbols

class CPriceProvider;

class CPriceProviderNotify:
public IDispEventImpl<1, CPriceProviderNotify, &DIID__IPriceProviderEvents, (const GUID*)&LIBID_PRICEPROVIDERSLib>
{
public:

BEGIN_SINK_MAP (CPriceProviderNotify)
SINK_ENTRY_INFO(1, DIID__IPriceProviderEvents, 1, OnLastQuote, &m_ArrivedLastQuote)
SINK_ENTRY_INFO(1, DIID__IPriceProviderEvents, 2, OnQuoteUpdate,&m_ArrivedQuoteUpdate)
SINK_ENTRY_INFO(1, DIID__IPriceProviderEvents, 3, OnError, &m_ArrivedError)
END_SINK_MAP()

	CPriceProviderNotify(CPriceProvider* pClient):m_pClient(pClient){};
	virtual ~CPriceProviderNotify()
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
	STDMETHOD(OnLastQuote)(QuoteUpdateParams *Params, QuoteUpdateResult *Results);
	STDMETHOD(OnQuoteUpdate)(QuoteUpdateParams *Params, QuoteUpdateResult *Results);

private:
	CPriceProvider*	         m_pClient;	

	CComPtr<IPriceProvider>  m_spEvent;

	static _ATL_FUNC_INFO m_ArrivedError;
	static _ATL_FUNC_INFO m_ArrivedLastQuote;
	static _ATL_FUNC_INFO m_ArrivedQuoteUpdate;

};


/////////////////////////////////////////////////////////////////////////////
// CPriceProvider
class ATL_NO_VTABLE CPriceProvider : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPriceProvider, &CLSID_PriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CPriceProvider>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_PRICEPROVIDERSLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_PRICEPROVIDERSLib>,
	IConnectionPointImpl<CPriceProvider, &DIID__IPriceProviderEvents, CComDynamicUnkArray>,
	public IDispatchImpl<IProvider, &IID_IProvider, &LIBID_PRICEPROVIDERSLib>
{
public:
	CPriceProvider():
	  m_ProviderType(-1),
	  m_bConnected(false),
	  m_Notify(this),
	  m_bRecursive(true)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PRICEPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPriceProvider)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPrice)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvider)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPriceProvider)
	CONNECTION_POINT_ENTRY(DIID__IPriceProviderEvents)
END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
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

// ITrackDataPriceProvider
public:
	STDMETHOD(get_ProgID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ProviderID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

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
						if(g_Providers.GetProgID(m_ProviderType, enPriceProvider, bsProgId))
						{
							try{

								CLSID cls;
								CLSIDFromProgID(bsProgId, &cls);
								hr = m_spPriceProvider.CreateInstance(cls);
							}
							catch (...) {	}
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
						hr = m_spPriceProvider->Connect();
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

	bool IsHandledError(HRESULT hr)
	{
		return  0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr;
	}
	
private:
	long				   m_ProviderType;
	IPriceProviderPtr m_spPriceProvider;
	bool                   m_bConnected;
	CPriceProviderNotify   m_Notify;
	bool				   m_bRecursive;

};

#endif //__PRICEPROVIDER_H_
