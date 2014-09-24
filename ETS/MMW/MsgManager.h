#ifndef __MSGMANAGER_H_
#define __MSGMANAGER_H_

#include "resource.h"       
#include "MMW_CP.h"

#define DECLARE_REQUEST_METHODS_EX(_object)												\
		STDMETHOD(Unsub##_object)(BSTR UID);											\
		STDMETHOD(Sub##_object)(BSTR UID);												\
		STDMETHOD(Pub##_object)(BSTR UID, I##_object* Object, long* Result);

#define REQUEST_METHODS_IMPL_EX(_object)												\
																						\
	STDMETHODIMP CMsgManager::Pub##_object(BSTR UID, I##_object* Object, long* Result)	\
	{																					\
		if (Object == NULL || UID == NULL)												\
			return E_INVALIDARG;														\
																						\
		ISymbolObjectPtr	spSymbolObject;												\
		if (FAILED(spSymbolObject.CreateInstance(L"MsgStruct.SymbolObject")))			\
			return S_OK;																\
																						\
		spSymbolObject->put_Name(UID);													\
																						\
		_bstr_t bstrSubject = GenerateSubject(enMt##_object, spSymbolObject);			\
																						\
		if (!bstrSubject.length())														\
			return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);					\
																						\
		INetPackingPtr Packer = Object;													\
																						\
		return SendMessageToTransport(bstrSubject,enMt##_object, Packer, *Result);					\
	}																					\
																						\
	STDMETHODIMP CMsgManager::Sub##_object(BSTR UID)									\
	{																					\
		if (UID == NULL)																\
			return E_INVALIDARG;														\
																						\
		ISymbolObjectPtr	spSymbolObject;												\
		if (FAILED(spSymbolObject.CreateInstance(L"MsgStruct.SymbolObject")))			\
			return S_OK;																\
																						\
		spSymbolObject->put_Name(UID);													\
																						\
		return Subscribe(enMt##_object, spSymbolObject);					\
	}																					\
																						\
	STDMETHODIMP CMsgManager::Unsub##_object(BSTR UID)									\
	{																					\
		if (UID == NULL)																\
			return E_INVALIDARG;														\
																						\
		ISymbolObjectPtr	spSymbolObject;												\
		if (FAILED(spSymbolObject.CreateInstance(L"MsgStruct.SymbolObject")))			\
			return S_OK;																\
																						\
		spSymbolObject->put_Name(UID);													\
																						\
		return Unsubscribe(enMt##_object, spSymbolObject);								\
	}

//////////////////////////////////////////////////////////////////////////////

#define DECLARE_REQUEST_METHODS(_object)												\
		STDMETHOD(Unsub##_object)();    											    \
		STDMETHOD(Sub##_object)();												        \
		STDMETHOD(Pub##_object)(I##_object* Object, long* Result);

#define DECLARE_PUBLISH_METHOD(_method, _object)										\
		STDMETHOD(Pub##_method)(I##_object* Object, long* Result);

#define REQUEST_METHODS_IMPL(_object)													\
																						\
	STDMETHODIMP CMsgManager::Pub##_object(I##_object* Object, long* Result)	        \
	{																					\
		if (Object == NULL)             												\
			return E_INVALIDARG;														\
																						\
		_bstr_t bstrSubject = GenerateSubject(enMt##_object);               			\
																						\
		if (!bstrSubject.length())														\
			return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);					\
																						\
		INetPackingPtr Packer = Object;													\
																						\
		return SendMessageToTransport(bstrSubject,enMt##_object, Packer, *Result);		\
	}																					\
																						\
	STDMETHODIMP CMsgManager::Sub##_object()        									\
	{																					\
		return Subscribe(enMt##_object, 0);             					            \
	}																					\
																						\
	STDMETHODIMP CMsgManager::Unsub##_object()      									\
	{																					\
		return Unsubscribe(enMt##_object, 0);           								\
	}

#define PUBLISH_METHOD_IMPL(_method, _object)											\
																						\
	STDMETHODIMP CMsgManager::Pub##_method(I##_object* Object, long* Result)	        \
	{																					\
		if (Object == NULL)             												\
			return E_INVALIDARG;														\
																						\
		_bstr_t bstrSubject = GenerateSubject(enMt##_method);               			\
																						\
		if (!bstrSubject.length())														\
			return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);					\
																						\
		INetPackingPtr Packer = Object;													\
																						\
		return SendMessageToTransport(bstrSubject,enMt##_method, Packer, *Result);		\
	}																					
																						

#define SUBSCRIBE_METHODS_IMPL(_method)													\
	STDMETHODIMP CMsgManager::Sub##_method()        									\
	{																					\
		return Subscribe(enMt##_method, 0);             					            \
	}																					\
																						\
	STDMETHODIMP CMsgManager::Unsub##_method()      									\
	{																					\
		return Unsubscribe(enMt##_method, 0);           								\
	}
//////////////////////////////////////////////////////////////////////////////
class CMsgManager;
typedef IDispEventSimpleImpl2<CMsgManager, &__uuidof(_ISessionEvents)>
    _CSessionEventsBase;

class CSessionEvents : public _CSessionEventsBase
{
public:
    CSessionEvents(CMsgManager* pInvoke) : 
        _CSessionEventsBase(pInvoke)
    {
    }
};

////////////////////////////////////////////////////////////////////////////
// CMsgManager
class ATL_NO_VTABLE CMsgManager : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMsgManager, &CLSID_MsgManager>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CMsgManager>,
	public IDispatchImpl<IPubManager, &IID_IPubManager, &LIBID_MMWLib>,
	public IDispatchImpl<ISubManager, &IID_ISubManager, &LIBID_MMWLib>,
	public CProxy_IOnMessages< CMsgManager >
{

public:
	CMsgManager();

	HRESULT FinalConstruct();
	void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_MSGMANAGER)
DECLARE_NOT_AGGREGATABLE(CMsgManager)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMsgManager)
	COM_INTERFACE_ENTRY2(IDispatch, IPubManager)
	COM_INTERFACE_ENTRY(ISubManager)
	COM_INTERFACE_ENTRY(IPubManager)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMsgManager)
CONNECTION_POINT_ENTRY(DIID__IOnMessages)
END_CONNECTION_POINT_MAP()

	static _ATL_FUNC_INFO	m_OnMessageParams; 
	static _ATL_FUNC_INFO	m_OnLogonParams; 
	static _ATL_FUNC_INFO	m_OnLogoffParams; 
	static _ATL_FUNC_INFO	m_OnSubjectStatus; 

BEGIN_SINK_MAP(CMsgManager)
	SINK_ENTRY_INFO2(m_SessionEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnMessage, OnMessage, &m_OnMessageParams)
	SINK_ENTRY_INFO2(m_SessionEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnLogoff, OnLogoff, &m_OnLogoffParams)
	SINK_ENTRY_INFO2(m_SessionEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnLogon, OnLogon, &m_OnLogonParams)
	SINK_ENTRY_INFO2(m_SessionEvents, __uuidof(_ISessionEvents), DISPID_ISessionEventsOnSubjectStatus, OnSubjectStatus, &m_OnSubjectStatus)	
END_SINK_MAP()

// Transport source interface implementation
public:
	HRESULT __stdcall OnMessage(BSTR Subject, BSTR Message);
	HRESULT __stdcall OnLogon(BSTR Router);
	HRESULT __stdcall OnLogoff(BSTR Router);
	HRESULT __stdcall OnSubjectStatus(BSTR Subject, unsigned long ulNumSubscribers);

// ISupportErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPubManager
public:
	STDMETHOD(RegPublisher)(MESSAGE_TYPE Type, ISymbolObject* Symbol);
	STDMETHOD(UnregPublisher)(MESSAGE_TYPE Type, ISymbolObject* Symbol);
	STDMETHOD(PubVola)(ISymbolObject* Symbol, IVMESurface* Data, long* Result);
	STDMETHOD(PubTradeUpdate)(ITradeUpdate* Data, long* Result);
	STDMETHOD(PubPriceUpdate)(IPriceUpdate* Data, long* Result);
	STDMETHOD(PubUnderlyingUpdate)(IUnderlyingUpdate* Data, long* Result);
	STDMETHOD(PubManualPriceUpdate)(IManualPriceUpdate* Data, long* Result);
	STDMETHOD(PubSettingsUpdate)(ISettingsUpdate* Data, long* Result);
	STDMETHOD(get_IsLogoned)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_UserGroup)(/*[in]*/BSTR bsValue);

// ISubManager
public:
	DECLARE_REQUEST_METHODS_EX(DynamicMessage)
	
	STDMETHOD(SubTradeUpdate)();
	STDMETHOD(UnsubTradeUpdate)();
	STDMETHOD(SubVola)(ISymbolObject* pSymbolObject);
	STDMETHOD(UnsubVola)(ISymbolObject* pSymbolObject);
	STDMETHOD(SubPriceUpdate)();
	STDMETHOD(UnsubPriceUpdate)();
	STDMETHOD(SubUnderlyingUpdate)();
	STDMETHOD(UnsubUnderlyingUpdate)();
	STDMETHOD(SubExecutionReport)();
	STDMETHOD(UnsubExecutionReport)();
	
    DECLARE_REQUEST_METHODS(BroadcastMessage)

	STDMETHOD(UnsubOrderMessages)();
	STDMETHOD(SubOrderMessages)();
	STDMETHOD(UnsubOrdersNotificationMessages)();
	STDMETHOD(SubOrdersNotificationMessages)();

	STDMETHOD(UnsubActiveFuturesChange)();
	STDMETHOD(SubActiveFuturesChange)();

	STDMETHOD(SubFlexOption)();
	STDMETHOD(UnsubFlexOption)();

	STDMETHOD(SubManualPriceUpdate)();
	STDMETHOD(UnsubManualPriceUpdate)();
	STDMETHOD(SubSettingsUpdate)();
	STDMETHOD(UnsubSettingsUpdate)();

	DECLARE_PUBLISH_METHOD(RequestNewOrder,		Order)
	DECLARE_PUBLISH_METHOD(AlterOrder,			Order)
	DECLARE_PUBLISH_METHOD(CancelOrder,			Order)
	DECLARE_PUBLISH_METHOD(RequestOrderStatus,	Order)
	DECLARE_PUBLISH_METHOD(Order,				Order)
	DECLARE_PUBLISH_METHOD(ExecutionReport,		ExecutionReport)
	DECLARE_PUBLISH_METHOD(ActiveFuturesChange,	ActiveFuturesChange)
	DECLARE_PUBLISH_METHOD(FlexOption,	FlexOption)

	DECLARE_REQUEST_METHODS(FixGatewayStatusRequest)
	DECLARE_REQUEST_METHODS(FixGatewayStatus)

// Internal methods
	_bstr_t		GenerateSubject(MESSAGE_TYPE Type, ISymbolObject* pSymbolObject = NULL);
	bool		DecodeSubject(BSTR Subject, MESSAGE_TYPE& Type, ISymbolObject* pSymbolObject);

	HRESULT	SendMessageToTransport(BSTR Subject, MESSAGE_TYPE enType, INetPacking* pPacker, HRESULT& Result);

	_bstr_t g_bstrSubjectPrefix;

private:
	ISessionPtr			m_Session;
	IHostProxyPtr		m_Transport;
    CSessionEvents		m_SessionEvents;

protected:
    HRESULT Subscribe(	MESSAGE_TYPE			enType, 
						ISymbolObject*			pSymbol );

    HRESULT Unsubscribe(MESSAGE_TYPE enType, ISymbolObject* pSymbol);

private:
	static const wstring m_subjMtVola;
	static const wstring m_subjMtTradeUpdate;
	static const wstring m_subjMtPriceUpdate;
	static const wstring m_subjMtUnderlyingUpdate;
	static const wstring m_subjMtBroadcastMessage;
	static const wstring m_subjMtRequestNewOrder;
	static const wstring m_subjMtAlterOrder;
	static const wstring m_subjMtCancelOrder;
	static const wstring m_subjMtRequestOrderStatus;
	static const wstring m_subjMtOrder;
	static const wstring m_subjMtExecutionReport;
	static const wstring m_subjMtFixGatewayStatusRequest;
	static const wstring m_subjMtFixGatewayStatus;
	static const wstring m_subjMtDynamicMessage;
	static const wstring m_subjMtActiveFuturesChange;
	static const wstring m_subjMtFlexOption;
	static const wstring m_subjMtManualPriceUpdate;
	static const wstring m_subjMtSettingsUpdate;

};

#endif //__MSGMANAGER_H_
