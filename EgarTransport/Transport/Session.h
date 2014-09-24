// Session.h : Declaration of the CSession

#ifndef __SESSION_H_
#define __SESSION_H_

#include "resource.h"       // main symbols
#include "CRtClasses.h"
#include "TransportCP.h"
#include "MessageProcessor.h"
#include "EventThread.h"


using namespace __dev__;

/////////////////////////////////////////////////////////////////////////////
// CSession
class ATL_NO_VTABLE CSession : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSession, &CLSID_Session>,
	public ISupportErrorInfoImpl<&IID_ISession>,
	public IConnectionPointContainerImpl<CSession>,
	public IDispatchImpl<ISession, &IID_ISession, &LIBID_TRANSPORTLib>,
	public CProxy_ISessionEvents< CSession >
{
private:	
	friend class CHost;
public:
	CSession()
	{
	}
	HRESULT FinalConstruct( );
	void FinalRelease( );

DECLARE_REGISTRY_RESOURCEID(IDR_SESSION)
DECLARE_NOT_AGGREGATABLE(CSession)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSession)
	COM_INTERFACE_ENTRY(ISession)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CSession)
CONNECTION_POINT_ENTRY(DIID__ISessionEvents)
END_CONNECTION_POINT_MAP()



// ISession
public:

public:
	STDMETHOD(get_IsLogoned)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(SendMessage)(/*[in]*/BSTR bsSubject,/*[in]*/MsgPropertiesEnum enMsgProp,/*[in]*/BSTR bsMessage);
	STDMETHOD(Unsubscribe)(/*[in]*/BSTR bsSubject);
	STDMETHOD(Subscribe)(/*[in]*/BSTR bsSubject);
	STDMETHOD(CheckSubjectStatus)(/*[in]*/BSTR bsSubject);

	const CEgGuid& getSessionKey() const
	{
		return m_guidKey;
	}

//Session Context
	bool GetNextMessage(MsgProcessorTypeEnum Type, CEgTpBaseMessagePtr* ppMessage, u_long keyMessage)
	{
		if(enMPTLocal == Type)
			return m_storLocal.GetMessage(ppMessage);
		
		return false;
	}
	void SetMessageSend(MsgProcessorTypeEnum Type, u_long){};
	bool PushMessage(CEgTpBaseMessagePtr pMessage)
	{
		m_storLocal.Push(pMessage);
		return true;
	}

	void ClearAllMessages()
	{
		m_storLocal.Clear();
	}

private:
	CEgGuid							m_guidKey;
	CEgTpBufferedMessageStorage		m_storLocal;
	CHost*							m_pHost;
};

class CEgTpSessionList : public CTEgTpList<CEgGuid, CSession*>
{
public:
	bool InsertSession(CSession* pSession)
	{
		CAutoLock	Lock(m_Lock);	
		TKeyDataMap::_Pairib pib = m_ListMap.insert(std::make_pair(pSession->getSessionKey(),pSession));
		return pib.second;
	}
};

#endif //__SESSION_H_
