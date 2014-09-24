// SessionProxy.h : Declaration of the CSessionProxy

#ifndef __SESSIONPROXY_H_
#define __SESSIONPROXY_H_

#include "resource.h"       // main symbols
#include "EgTpPrxCP.h"

class CSessionProxy;
typedef IDispEventSimpleImpl2<CSessionProxy, &__uuidof(_ISessionEvents)>  _CSessionCoreEventsBase;

class CSessionCoreEvents : public _CSessionCoreEventsBase
{
public:
    CSessionCoreEvents(CSessionProxy* pInvoke) : 
        _CSessionCoreEventsBase(pInvoke)
    {
    }
};

class CMessageData
{
public:
	CMessageData(BSTR Subject, BSTR Message)
		:m_bstrSubject(NULL)
		,m_bstrMessage(NULL)
	{
		if(Subject!=NULL)
			m_bstrSubject = ::SysAllocStringLen(Subject, SysStringLen(Subject));

		if(Message!=NULL)
			m_bstrMessage = ::SysAllocStringLen(Message, SysStringLen(Message));
	}
	~CMessageData()
	{
		try
		{
			if(m_bstrSubject)
			{
				::SysFreeString(m_bstrSubject);
				m_bstrSubject = NULL;
			}
			if(m_bstrMessage)
			{
				::SysFreeString(m_bstrMessage);
				m_bstrMessage = NULL;
			}
		}
		catch (...){}
	}

	BSTR GetSubject(){return m_bstrSubject;}
	BSTR GetMessage(){return m_bstrMessage;}

private:

	BSTR m_bstrSubject;
	BSTR m_bstrMessage;

private:
	CMessageData(){};
	CMessageData(const CMessageData&){};
};
typedef boost::shared_ptr<CMessageData> CMessageDataPtr;
/////////////////////////////////////////////////////////////////////////////
// CSessionProxy
class ATL_NO_VTABLE CSessionProxy : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSessionProxy, &CLSID_SessionProxy>,
	public ISupportErrorInfoImpl<&IID_ISession>,
	public IConnectionPointContainerImpl<CSessionProxy>,
	public IDispatchImpl<ISession, &IID_ISession, &LIBID_TRANSPORTLib>,
	public CProxy_ISessionEvents< CSessionProxy >,
	public EgLib::CStrategyT<CSessionProxy, CMessageDataPtr>
{
public:

	CSessionProxy() : m_SessionCoreEvents(this),m_pHostProxy(NULL), m_dwSessionCoreCookie(0)
	{
	}

	HRESULT FinalConstruct();
	void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_SESSIONPROXY)
DECLARE_NOT_AGGREGATABLE(CSessionProxy)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSessionProxy)
	COM_INTERFACE_ENTRY(ISession)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CSessionProxy)
CONNECTION_POINT_ENTRY(DIID__ISessionEvents)
END_CONNECTION_POINT_MAP()

	static _ATL_FUNC_INFO	m_OnMessageParams;
	static _ATL_FUNC_INFO	m_OnLogonParams;
	static _ATL_FUNC_INFO	m_OnLogoffParams;
	static _ATL_FUNC_INFO	m_OnSubjectStatusParams;

BEGIN_SINK_MAP(CSessionProxy)
	SINK_ENTRY_INFO2(m_SessionCoreEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnMessage, OnMessage, &m_OnMessageParams)
	SINK_ENTRY_INFO2(m_SessionCoreEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnLogon, OnLogon, &m_OnLogonParams)
	SINK_ENTRY_INFO2(m_SessionCoreEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnLogoff, OnLogoff, &m_OnLogoffParams)
	SINK_ENTRY_INFO2(m_SessionCoreEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnSubjectStatus, OnSubjectStatus, &m_OnSubjectStatusParams)
END_SINK_MAP()

	STDMETHOD(OnMessage)(BSTR Subject, BSTR Message);
	STDMETHOD(OnLogon)(BSTR Router);
	STDMETHOD(OnLogoff)(BSTR Router);
	STDMETHOD(OnSubjectStatus)(BSTR Subject, unsigned long ulNumSubscribers);

// ISession
public:
	STDMETHOD(get_IsLogoned)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(SendMessageX)(/*[in]*/BSTR bsSubject,/*[in]*/MsgPropertiesEnum enMsgProp,/*[in]*/BSTR bsMessage);
	STDMETHOD(Unsubscribe)(/*[in]*/BSTR bsSubject);
	STDMETHOD(Subscribe)(/*[in]*/BSTR bsSubject);
	STDMETHOD(CheckSubjectStatus)(/*[in]*/BSTR bsSubject);

	HRESULT	AttachToTransport();
	void	DettachFromTransport();
	void	SetHost(CHostProxy* pHostProxy)
	{
		ObjectLock lock(this);
		if(m_pHostProxy != pHostProxy && pHostProxy == NULL)
			CloseHostConnection();
		m_pHostProxy = pHostProxy;
	}
	CHostProxy* GetHost()
	{
		ObjectLock lock(this);
		return m_pHostProxy;
	}
public:
	HRESULT OnStrategyInitialize(){return CoInitializeEx(NULL, COINIT_MULTITHREADED);}
	void    OnStrategyUninitialize(){CoUninitialize();}
	void	OnData(CMessageDataPtr& spData);
private:
	void CloseHostConnection();

private:
	IGlobalInterfaceTablePtr	m_spGIT;
	DWORD						m_dwSessionCoreCookie;

	CSessionCoreEvents			m_SessionCoreEvents;

	CHostProxy*					m_pHostProxy;
};

#endif //__SESSIONPROXY_H_
