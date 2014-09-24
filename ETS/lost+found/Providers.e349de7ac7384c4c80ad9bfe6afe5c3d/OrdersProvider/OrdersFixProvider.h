// OrdersFixProvider.h : Declaration of the COrdersFixProvider

#pragma once
#include "resource.h"       // main symbols

#include "OrdersProvider.h"
#include "_IOrdersFixProviderEvents_CP.h"
#include "OrderID.h"

// {98C1C057-1812-4069-B79E-7D6CFA1CCB60}
static const GUID CATID_FixProviders = 
{ 0x98c1c057, 0x1812, 0x4069, { 0xb7, 0x9e, 0x7d, 0x6c, 0xfa, 0x1c, 0xcb, 0x60 } };



// COrdersFixProvider

class ATL_NO_VTABLE COrdersFixProvider :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COrdersFixProvider, &CLSID_OrdersFixProvider>,
	public ISupportErrorInfoImpl<&IID_IOrderProvider>,
	public IConnectionPointContainerImpl<COrdersFixProvider>,
	public CProxy_IOrdersFixProviderEvents<COrdersFixProvider>,
	public IDispatchImpl<IOrderProvider, &IID_IOrderProvider, &LIBID_OrdersProviderLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	private FIX::Application,
	private FIX::MessageCracker
{
private:

	void	_ReadSettings();
	void	_SaveSettings();

	template<class OrderMessage>
	void	_FillOrder(FoOrder* Order, OrderMessage& Message);
	template<class OrderMessage>
	void	_FillOrderCancelReplaceRequest(FoOrderCancelReplaceRequest* Request, OrderMessage& Message);
	template<class OrderMessage>
	void	_FillOrderCancelRequest(FoOrderCancelRequest* Request, OrderMessage& Message);

	template<class OrderMessage>
	void	_FillOrderStatusRequest(FoOrderStatusRequest* Request, OrderMessage& Message);

	void	_ExtractExecutionReport  (const FIX41::ExecutionReport& Message, FoExecutionReport* Report)/* throw(_com_error)*/;
	void	_ExtractOrderCancelReject(const FIX41::OrderCancelReject& Message, FoOrderCancelReject* Reject)/* throw(_com_error)*/;

	virtual void onMessage(const FIX41::ExecutionReport& message, const FIX::SessionID& sessionID);
	virtual void onMessage(const FIX41::OrderCancelReject& message, const FIX::SessionID& sessionID);
	
public:
	virtual void onCreate(const FIX::SessionID& sessionID)
	{
		//		CString sSession;
		//		sSession.Format("FIX Session is created: %s", sessionID.toString().c_str());
		//		CTracer::Trace(sSession);
	}

	virtual void onLogon(const FIX::SessionID& sessionID)
	{
		m_cussrentSession = sessionID;
		//		CString sSession;
		//		sSession.Format("OnLogOn: %s", sessionID.toString().c_str());
	}
	virtual void onLogout(const FIX::SessionID& sessionID)
	{
		m_cussrentSession = FIX::SessionID();
		//		CString sSession;
		//		sSession.Format("OnLogOut: %s", sessionID.toString().c_str());

	}

	virtual void toAdmin(FIX::Message& message, const FIX::SessionID& sessionID)
		/*throw(FIX::DoNotSend&)*/{}

	virtual void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
	{
		crack(message, sessionID);
	}
	virtual void fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID)
		/*throw(FIX::FieldNotFound&, FIX::IncorrectDataFormat&, FIX::IncorrectTagValue&, FIX::RejectLogon&)*/
	{
	}
	virtual void toApp(FIX::Message& message, const FIX::SessionID& sessionID)
		/*throw(FIX::DoNotSend&)*/
	{
	}


private:
	boost::shared_ptr<FIX::SocketInitiator> m_pInitiator;

	boost::shared_ptr<FIX::SessionSettings>  m_pSettings;
	boost::shared_ptr<FIX::FileStoreFactory> m_pStoreFactory;
	boost::shared_ptr<FIX::FileLogFactory>   m_pLogFactory;

	FIX::SessionID							 m_cussrentSession;
	COrderID								 m_LastOrderID;
	
	_bstr_t									 m_bsClientID;
	_bstr_t									 m_bsExecBroker;

public:
	COrdersFixProvider() :
	  m_bConnected(false) {}

	  virtual ~COrdersFixProvider() {}

	  DECLARE_REGISTRY_RESOURCEID(IDR_ORDERSFIXPROVIDER)

	  DECLARE_PROTECT_FINAL_CONSTRUCT()

	  BEGIN_COM_MAP(COrdersFixProvider)
		  COM_INTERFACE_ENTRY(IOrderProvider)
		  COM_INTERFACE_ENTRY(IDispatch)
		  COM_INTERFACE_ENTRY(ISupportErrorInfo)
		  COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	  END_COM_MAP()

	  BEGIN_CATEGORY_MAP(COrdersFixProvider)
		  IMPLEMENTED_CATEGORY(CATID_FixProviders)
	  END_CATEGORY_MAP()

	  // IOrderProvider
public:

	STDMETHOD(raw_Disconnect)();
	STDMETHOD(raw_Connect)();

	STDMETHOD(raw_SubmitNewOrder)(FoOrder* Order, BSTR* OrderID);
	STDMETHOD(raw_AlterOrder)(BSTR OrderID, FoOrderCancelReplaceRequest* Order, BSTR* AlteredOrderID);
	STDMETHOD(raw_CancelOrder)(BSTR OrderID, FoOrderCancelRequest* Order, BSTR* CanceledOrderID);
	STDMETHOD(raw_RequestOrderStatus)(BSTR OrderID, FoOrderStatusRequest* Order);

private:
	bool m_bConnected;

public :
	void FinalRelease();

	BEGIN_CONNECTION_POINT_MAP(COrdersFixProvider)
		CONNECTION_POINT_ENTRY(__uuidof(_OrderProviderEvents))
	END_CONNECTION_POINT_MAP()

	BEGIN_SINK_MAP(COrdersFixProvider)
		//		SINK_ENTRY_INFO(1, __uuidof(_FixSessionEvents), DISPID_FiOnError, OnError, &m_OnErrorInfo)
		//		SINK_ENTRY_INFO(1, __uuidof(_FixSessionEvents), DISPID_FiOnPackedFixMessage, OnPackedFixMessage, &m_OnPackedFixMessageInfo)
		//		SINK_ENTRY_INFO(1, __uuidof(_FixSessionEvents), DISPID_FiOnReLogon, OnReLogon, &m_OnReLogonInfo)
	END_SINK_MAP()

	//	static _ATL_FUNC_INFO     m_OnErrorInfo;
	//	static _ATL_FUNC_INFO     m_OnPackedFixMessageInfo;
	//	static _ATL_FUNC_INFO     m_OnReLogonInfo;

	//protected:
	//    STDMETHOD(OnError)(FixErrorStates ErrorState,
	//                       BSTR Description,
	//                       VARIANT ExpandedErrorInfo);
	//	STDMETHOD(OnPackedFixMessage)(BSTR FIXString);
	//	STDMETHOD(OnReLogon)();


};
OBJECT_ENTRY_AUTO(__uuidof(OrdersFixProvider), COrdersFixProvider)
