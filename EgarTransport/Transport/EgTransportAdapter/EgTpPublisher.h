// EgTpPublisher.h : Declaration of the CEgTpPublisher

#ifndef __EGTPPUBLISHER_H_
#define __EGTPPUBLISHER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCEgTpPublisher
class ATL_NO_VTABLE CEgTpPublisher : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEgTpPublisher, &CLSID_EgTpPublisher>,
	public ISupportErrorInfo,
	public IDispatchImpl<IEtxComponent, &IID_IEtxComponent, &LIBID_ETXRTCLib>,
	public IDispatchImpl<IEtxListener, &IID_IEtxListener, &LIBID_ETXRTCLib>,
	public IDispEventImpl <1, CEgTpPublisher, &DIID__ISessionEvents>,
	CETXDumpSupport
{
public:
	
	CEgTpPublisher() : CETXDumpSupport(OLESTR("ETX.System.Component.EgTpPublisher"), CLSID_EgTpPublisher, IID_IEtxComponent)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EGTPPUBLISHER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEgTpPublisher)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IEtxComponent)
	COM_INTERFACE_ENTRY2(IDispatch, IEtxComponent)
	COM_INTERFACE_ENTRY(IEtxListener)
END_COM_MAP()

BEGIN_SINK_MAP(CEgTpPublisher)
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
	IetxSyncReceiverPortBindingPtr		m_pReceiverPort;
	CComQIPtr<IetxSenderPortBinding>	m_pSendEventPort; // send only OnLogon, OnLogoff events
		
	_bstr_t								m_bsPublishSubject; // Subject of the transport messages
	MsgPropertiesEnum					m_enMessageProp; // Transport Type of sending messages (guaratee, buffered, ...)

	ISessionPtr							m_spSession; // Transport session
	IHostPtr							m_spHost; // Transport host
};

#endif //__EGTPPUBLISHER_H_
