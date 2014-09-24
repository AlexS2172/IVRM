// EgTpRequesterProxy.h : Declaration of the CEgTpRequesterProxy

#ifndef __EGTPREQUESTERPROXY_H_
#define __EGTPREQUESTERPROXY_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEgTpRequesterProxy
class ATL_NO_VTABLE CEgTpRequesterProxy : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEgTpRequesterProxy, &CLSID_EgTpRequesterProxy>,
	public ISupportErrorInfo,
	public IDispatchImpl<IEtxComponent, &IID_IEtxComponent, &LIBID_ETXRTCLib>,
	public IDispatchImpl<IEtxListener, &IID_IEtxListener, &LIBID_ETXRTCLib>,
	public IDispEventImpl <1, CEgTpRequesterProxy, &DIID__ISessionEvents>,
	CETXDumpSupport
{
public:
	CEgTpRequesterProxy() : CETXDumpSupport(OLESTR("ETX.System.Component.EgTpRequesterProxy"), CLSID_EgTpRequesterProxy, IID_IEtxComponent)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EGTPREQUESTERPROXY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEgTpRequesterProxy)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IEtxComponent)
	COM_INTERFACE_ENTRY2(IDispatch, IEtxComponent)
	COM_INTERFACE_ENTRY(IEtxListener)
END_COM_MAP()

BEGIN_SINK_MAP(CEgTpRequesterProxy)
	SINK_ENTRY_INFO(1, DIID__ISessionEvents, 1, OnMessage, &m_OnMessageParams)
	SINK_ENTRY_INFO(1, DIID__ISessionEvents, 2, OnLogoff, &m_OnLogoffParams)
	SINK_ENTRY_INFO(1, DIID__ISessionEvents, 3, OnLogon, &m_OnLogonParams)
END_SINK_MAP()
// Transport source interface implementation
public:
	HRESULT __stdcall OnMessage(BSTR Subject, BSTR Message);
	HRESULT __stdcall OnLogon(BSTR Router);
	HRESULT __stdcall OnLogoff(BSTR Router);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEtxComponent
	STDMETHOD(raw_etxCreate)(IEtxMessage* params);
	STDMETHOD(raw_etxDestroy)();
	STDMETHOD(raw_etxActivate)();
	STDMETHOD(raw_etxDeactivate)();
	STDMETHOD(raw_etxSetContext)(IEtxComponentContext* context);

// IEtxListener
	STDMETHOD(raw_etxOnMessage) (IEtxMessage* in_msg, IEtxMessage** ret_msg);

private:
	static _ATL_FUNC_INFO				m_OnMessageParams;
	static _ATL_FUNC_INFO				m_OnLogonParams;
	static _ATL_FUNC_INFO				m_OnLogoffParams;

	IEtxComponentContextPtr				m_Ctxt;
	CComQIPtr<IetxSenderPortBinding>	m_pSendPort;
	IetxSyncReceiverPortBindingPtr		m_pReceiverPort;
		
	_bstr_t								m_bsServerSubject; // Subject of the transport messages
	MsgPropertiesEnum					m_enMessageProp; // Transport Type of sending messages (guaratee, buffered, ...)

	ISessionPtr							m_spSession; // Transport session
	IHostPtr							m_spHost; // Transport host

};

#endif //__EGTPREQUESTERPROXY_H_
