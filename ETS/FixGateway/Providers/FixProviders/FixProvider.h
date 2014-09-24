// FixProvider.h : Declaration of the CFixProvider

#ifndef __PROVIDER_H_
#define __PROVIDER_H_

#include "resource.h"       // main symbols
#include "FixProvidersCP.h"

class CFixProvider;

class CFixProviderNotify:
public IDispEventImpl<1, CFixProviderNotify, &__uuidof(_OrderProviderEvents), (const GUID*)&LIBID_FixProvidersLibrary>{
public:
BEGIN_SINK_MAP (CFixProviderNotify)
	SINK_ENTRY_INFO(1, __uuidof(_OrderProviderEvents), DISPID_FpOnCancelReject, OnCancelReject, &m_OnCancelReject)
	SINK_ENTRY_INFO(1, __uuidof(_OrderProviderEvents), DISPID_FpOnExecReport, OnExecutionReport, &m_OnExecutionReport)
	SINK_ENTRY_INFO(1, __uuidof(_OrderProviderEvents), DISPID_FpOnOrderError, OnOrderError, &m_OnOrderError)
	SINK_ENTRY_INFO(1, __uuidof(_OrderProviderEvents), DISPID_FpOnDisconnect, OnDisconnect, &m_OnDisconnect)
	SINK_ENTRY_INFO(1, __uuidof(_OrderProviderEvents), DISPID_FpOnReLogon, OnReLogon, &m_OnReLogon)
	SINK_ENTRY_INFO(1, __uuidof(_OrderProviderEvents), DISPID_FpOnLogonFailed, OnLogonFailed, &m_OnLogonFailed)
END_SINK_MAP()

	CFixProviderNotify(CFixProvider* pClient): m_pClient(pClient)
	{
	};

	virtual ~CFixProviderNotify()
	{
		Destroy();
	}

	HRESULT Initialize(IOrderProvider* pEvent, CFixProvider* pClient)
	{
		HRESULT hr = E_FAIL;
		if(pClient)
		{
			m_pClient = pClient;

			m_spEvent = pEvent;
			hr = DispEventAdvise(m_spEvent);
		}
		return hr;
	}

	void Destroy()
	{
		if (m_spEvent)
		{
			DispEventUnadvise (m_spEvent);
			m_spEvent = NULL;
		}
	}

	STDMETHOD(OnCancelReject)(BSTR Reason, FoOrderCancelReject* Report);
	STDMETHOD(OnExecutionReport)(FoExecutionReport* Report);
	STDMETHOD(OnOrderError)(BSTR ClOrdID, BSTR Description);
	STDMETHOD(OnDisconnect)(long ProviderType);
	STDMETHOD(OnReLogon)(long ProviderType);
	STDMETHOD(OnLogonFailed)(long ProviderType, BSTR Description);

private:
	CFixProvider*	              m_pClient;
	CComPtr<IOrderProvider>   m_spEvent;

	static _ATL_FUNC_INFO m_OnCancelReject;
	static _ATL_FUNC_INFO m_OnExecutionReport;
	static _ATL_FUNC_INFO m_OnOrderError;
	static _ATL_FUNC_INFO m_OnDisconnect;
	static _ATL_FUNC_INFO m_OnReLogon;
	static _ATL_FUNC_INFO m_OnLogonFailed;
};


/////////////////////////////////////////////////////////////////////////////
// CFixProvider
class ATL_NO_VTABLE CFixProvider :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFixProvider, &CLSID_FixProvider>,
	public ISupportErrorInfo,
	public IDispatchImpl<IOrderProvider, &IID_IOrderProvider, &LIBID_FixProvidersLibrary>,
	public IDispatchImpl<IFixProviderInitializer, &IID_IFixProviderInitializer, &LIBID_FixProvidersLibrary>,
	public CProxy_FixProviderEvents<CFixProvider>,
	public IConnectionPointContainerImpl<CFixProvider>
{
public:
	CFixProvider(): m_Notify (0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFixProvider)
	COM_INTERFACE_ENTRY(IOrderProvider)
	COM_INTERFACE_ENTRY2(IDispatch, IOrderProvider)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IFixProviderInitializer)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IFixProvider
public:
	void FinalRelease();
	HRESULT FinalConstruct();
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	STDMETHOD(SubmitNewOrder)(FoOrder* Order, BSTR* OrderID);
	STDMETHOD(AlterOrder)(BSTR OrderID, FoOrderCancelReplaceRequest* Order, BSTR* AlteredOrderID);
	STDMETHOD(CancelOrder)(BSTR OrderID, FoOrderCancelRequest* Order, BSTR* CanceledOrderID);
	STDMETHOD(RequestOrderStatus)(BSTR OrderID, FoOrderStatusRequest* Order);

// IFixProviderInitializer
	STDMETHOD(SetType)(long NewId);
	STDMETHOD(GetType)(long * CurrId);

//

private:
	bool						m_bSessionEstablished;

protected:
	bool						m_bConnected;
	long						m_FixProviderId;
	IOrderProvider*			    m_pOrderProvider;
	CFixProviderNotify				m_Notify;
	bool						m_bRecursive;
public :

BEGIN_CONNECTION_POINT_MAP(CFixProvider)
	CONNECTION_POINT_ENTRY(DIID__OrderProviderEvents)
END_CONNECTION_POINT_MAP()

};

#endif //__PROVIDER_H_
