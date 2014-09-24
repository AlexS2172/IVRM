// EgTpPublisher.cpp : Implementation of CEgTpPublisher
#include "stdafx.h"
#include "EgTransportAdapter.h"
#include "EgTpPublisher.h"

_ATL_FUNC_INFO CEgTpPublisher::m_OnMessageParams = {CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_BSTR}};
_ATL_FUNC_INFO CEgTpPublisher::m_OnLogonParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};
_ATL_FUNC_INFO CEgTpPublisher::m_OnLogoffParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};

/////////////////////////////////////////////////////////////////////////////
// CEgTpPublisher

STDMETHODIMP CEgTpPublisher::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEtxComponent,
		&IID_IEtxListener
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

// IEtxComponent
STDMETHODIMP CEgTpPublisher::raw_etxCreate(IEtxMessage* params)
{
	BEGIN_PARAMS
		IN_PARAM_REF(params)
	END_PARAMS
	
	ETX_TRY
	{
		CEtxMapMessage msg(params); // Initialize message
		
		m_bsPublishSubject = msg.Body(OLESTR("PublishSubject"));
		if(!m_bsPublishSubject.length())
			return LogError( OLESTR("Invalid PublishSubject"), E_INVALIDARG );
		
		BYTE bProp = msg.Body(OLESTR("MessageProperty"));
		m_enMessageProp = (MsgPropertiesEnum)bProp;
	}
	ETX_DUMP_COM_ERROR(OLESTR("Failed of create component"))

	return S_OK;
}

STDMETHODIMP CEgTpPublisher::raw_etxDestroy()
{
	return S_OK;
}
	
STDMETHODIMP CEgTpPublisher::raw_etxActivate()
{
	ETX_TRY
	{
		com_check(m_spHost.CreateInstance(OLESTR("Transport.Host")));
		com_check(m_spHost->CreateSession(&m_spSession));
		com_check(m_spHost->Logon(OLESTR("")));

		com_check(m_pReceiverPort->etxRegisterListener(static_cast<IEtxListener*>(this)));

		com_check(DispEventAdvise(m_spSession));
	}
	ETX_DUMP_COM_ERROR(OLESTR("Failed of activate component"))

	return S_OK;
}

STDMETHODIMP CEgTpPublisher::raw_etxDeactivate()
{
	ETX_TRY
	{
	    DispEventUnadvise(m_spSession, &DIID__ISessionEvents);
		m_spSession = LPUNKNOWN(0);
		m_spHost = LPUNKNOWN(0);
		m_pSendEventPort = LPUNKNOWN(0);
		m_pReceiverPort.Release();
		m_Ctxt = LPUNKNOWN(0);
	}
	ETX_DUMP_COM_ERROR(OLESTR("Failed of Deactivate component"))

	return S_OK;
}
	
STDMETHODIMP CEgTpPublisher::raw_etxSetContext(IEtxComponentContext* context)
{
	BEGIN_PARAMS
		IN_PARAM_REF(context)
	END_PARAMS

	ETX_TRY
	{
		m_Ctxt = context;
		IDispatchPtr pDisp;
		com_check(m_Ctxt->get_etxPortBinding( OLESTR("In") ,&pDisp));
		m_pReceiverPort = pDisp;
		pDisp = LPUNKNOWN(0);
		com_check(m_Ctxt->get_etxPortBinding( OLESTR("Out"),&pDisp) );
		m_pSendEventPort = pDisp;
	}
	ETX_DUMP_COM_ERROR(OLESTR("Failed of SetContext call"))
	return S_OK;
}

// IEtxListener
STDMETHODIMP CEgTpPublisher::raw_etxOnMessage(IEtxMessage* in_msg, IEtxMessage** ret_msg)
{
	BEGIN_PARAMS
		IN_PARAM_REF(in_msg)
		OUT_PARAM(ret_msg)
	END_PARAMS

	ETX_TRY
	{
		IStreamPtr pStream;
		IPersistStreamPtr pPersist;
		
		com_check(::CreateStreamOnHGlobal( NULL, TRUE, &pStream ) ) ;
		
		pPersist = in_msg;
		com_check( ::OleSaveToStream( pPersist, pStream ) );
		
		CAutoGlobal hData; ::GetHGlobalFromStream( pStream, &(HGLOBAL)hData );
		DWORD	dwSize = hData.Size();
				
		OLECHAR* pData = static_cast<OLECHAR*>(hData.Lock());
		BSTR bsMessage = SysAllocStringLen(pData, dwSize >> 1 );

		com_check(m_spSession->SendMessage(m_bsPublishSubject,m_enMessageProp,bsMessage));
		SysFreeString(bsMessage);
	}	
	ETX_DUMP_COM_ERROR(OLESTR("Failed of etxOnMessage call"))

	return S_OK;
}

HRESULT __stdcall CEgTpPublisher::OnMessage(BSTR Subject, BSTR Message)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CEgTpPublisher::OnLogon(BSTR Router)
{
	ETX_TRY
	{
		if(!m_pSendEventPort)
			return E_INVALIDARG;
		CComPtr<IEtxMessageFactory>	msg_factory;
		CComPtr<IEtxMessage>		send_msg;
			
		com_check(m_Ctxt->get_MessageFactory(&msg_factory));
		com_check(msg_factory->raw_etxCreateMessage(_bstr_t(MSG_TYPE_MAP), &send_msg));
		CComQIPtr<IEtxMapMessage> pSendMsg(send_msg);
	
		com_check(pSendMsg->put_Header(MSG_HEADER_MSGBUSINESS_TYPE, _variant_t(MSG_TYPE_ONLOGON)));
		com_check(pSendMsg->put_Body(_variant_t(OLESTR("Router")), _variant_t(Router)));
		com_check(m_pSendEventPort->etxSend(pSendMsg, etxSEND_NONE, -1));
	}
	ETX_DUMP_COM_ERROR(OLESTR("Failed of OnLogon call"))

	return S_OK;
}

HRESULT __stdcall CEgTpPublisher::OnLogoff(BSTR Router)
{
	ETX_TRY
	{
		if(!m_pSendEventPort)
			return E_INVALIDARG;
		CComPtr<IEtxMessageFactory>	msg_factory;
		CComPtr<IEtxMessage>		send_msg;
			
		com_check(m_Ctxt->get_MessageFactory(&msg_factory));
		com_check(msg_factory->raw_etxCreateMessage(_bstr_t(MSG_TYPE_MAP), &send_msg));
		CComQIPtr<IEtxMapMessage> pSendMsg(send_msg);
	
		com_check(pSendMsg->put_Header(MSG_HEADER_MSGBUSINESS_TYPE, _variant_t(MSG_TYPE_ONLOGOFF)));
		com_check(pSendMsg->put_Body(_variant_t(OLESTR("Router")), _variant_t(Router)));
		com_check(m_pSendEventPort->etxSend(pSendMsg, etxSEND_NONE, -1));
	}
	ETX_DUMP_COM_ERROR(OLESTR("Failed of OnLogoff call"))
	return S_OK;
}
