// FixGServer.h : Declaration of the CFixGServer

#ifndef __FIXGSERVER_H_
#define __FIXGSERVER_H_

#include "resource.h"       // main symbols
#include "Storage.h"
//#include "FIXGateway.h"
#include "_IFixGServerEvents_CP.H"

class CFixGServer;
/////////////////////////////////////////////////////////////////////////////
typedef IDispEventImpl<1, CFixGServer, &__uuidof(_IOnMessages), &LIBID_MMWLib, 1, 0>
    CPubDispEvent;

typedef IDispEventImpl<2, CFixGServer, &__uuidof(FX::_OrderProviderEvents), &__uuidof(FX::__FixProvidersLibrary), 1, 0>
    CProvidersDispEvent;

/////////////////////////////////////////////////////////////////////////////
// CFixGServer
class ATL_NO_VTABLE CFixGServer :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFixGServer, &CLSID_FixGServer>,
	public ISupportErrorInfoImpl<&IID_IFixGServer>,
	public IDispatchImpl<IFixGServer, &IID_IFixGServer, &LIBID_FIXGATEWAYLib>,
	public CPubDispEvent,
	public CProvidersDispEvent,
	public CThread,
	public IConnectionPointContainerImpl<CFixGServer>,
	public CProxy_IFixGServerEvents<CFixGServer>
{
public:

	/////////////////////////////////////////////////////////////////////////////
	struct CFgProvider
	{
		CFgProvider() 
			: m_bsProviderName(L"")
			, m_spProvider(NULL)
			, m_bDisconnected(false)
		{}

		_bstr_t					m_bsProviderName;
		FX::IOrderProviderPtr	m_spProvider;
		bool					m_bDisconnected;
	};
	/////////////////////////////////////////////////////////////////////////////
	struct CFgThreadPoolParams
	{
		IOrderPtr			m_spOrder;
		DISP_IDS			m_dispID;
		MsFixProviderEnum	m_enLoc;
	};

    typedef map<DISP_IDS, HANDLE>				TEventsMap;
    typedef map<MsFixProviderEnum, CFgProvider> TProvidersMap;

public:
	CFixGServer() 
		: m_enStatus(enFgSsStopped)
		, m_hGatewaysCheckTimer(NULL)
		, m_hStartFixEvent(NULL)
		, m_hStopFixEvent(NULL)
		, m_hStartFixTimer(NULL)
		, m_hStopFixTimer(NULL){}

DECLARE_REGISTRY_RESOURCEID(IDR_FixGServer)
DECLARE_NOT_AGGREGATABLE(CFixGServer)
DECLARE_CLASSFACTORY_SINGLETON(CFixGServer)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFixGServer)
	COM_INTERFACE_ENTRY(IFixGServer)
	COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_SINK_MAP(CFixGServer)
    SINK_ENTRY_INFO(1, __uuidof(_IOnMessages), DISPID_OnRequestNewOrder, OnRequestNewOrder, &m_OnRequestNewOrder)
    SINK_ENTRY_INFO(1, __uuidof(_IOnMessages), DISPID_OnAlterOrder, OnAlterOrder, &m_OnAlterOrder)
    SINK_ENTRY_INFO(1, __uuidof(_IOnMessages), DISPID_OnCancelOrder, OnCancelOrder, &m_OnCancelOrder)
    SINK_ENTRY_INFO(1, __uuidof(_IOnMessages), DISPID_OnRequestOrderStatus, OnRequestOrderStatus, &m_OnRequestOrderStatus)
    SINK_ENTRY_INFO(1, __uuidof(_IOnMessages), DISPID_OnFixGatewayStatusRequest, OnFixGatewayStatusRequest, &m_OnFixGatewayStatusRequest)
	SINK_ENTRY_INFO(1, __uuidof(_IOnMessages), DISPID_OnFixGatewayStatus, OnFixGatewayStatus, &m_OnFixGatewayStatus)
	SINK_ENTRY_INFO(2, __uuidof(FX::_OrderProviderEvents), FX::DISPID_FpOnCancelReject, OnCancelReject,		&m_OnCancelReject)
	SINK_ENTRY_INFO(2, __uuidof(FX::_OrderProviderEvents), FX::DISPID_FpOnExecReport,	OnExecutionReport,	&m_OnExecutionReport)
	SINK_ENTRY_INFO(2, __uuidof(FX::_OrderProviderEvents), FX::DISPID_FpOnOrderError,	OnOrderError,		&m_OnOrderError)
	SINK_ENTRY_INFO(2, __uuidof(FX::_OrderProviderEvents), FX::DISPID_FpOnDisconnect,	OnDisconnect,		&m_OnDisconnect)
	SINK_ENTRY_INFO(2, __uuidof(FX::_OrderProviderEvents), FX::DISPID_FpOnReLogon,		OnReLogon,			&m_OnReLogon)
	SINK_ENTRY_INFO(2, __uuidof(FX::_OrderProviderEvents), FX::DISPID_FpOnLogonFailed,	OnLogonFailed,		&m_OnLogonFailed)
END_SINK_MAP()

BEGIN_CONNECTION_POINT_MAP(CFixGServer)
	CONNECTION_POINT_ENTRY(__uuidof(_IFixGServerEvents))
END_CONNECTION_POINT_MAP()

//IFixGServer
STDMETHOD(StartFix)(void);
STDMETHOD(StopFix)(void);
STDMETHOD(get_Status)(FgServerStatusEnum* pVal);
STDMETHOD(get_StartTime)(DATE* pVal);
STDMETHOD(put_StartTime)(DATE dtStart);
STDMETHOD(get_StopTime)(DATE* pVal);
STDMETHOD(put_StopTime)(DATE dtStop);
STDMETHOD(get_AutoStartStop)(VARIANT_BOOL* pVal);
STDMETHOD(put_AutoStartStop)(VARIANT_BOOL bVal);

//MMW
STDMETHOD(OnRequestNewOrder)(IOrder* Order);
STDMETHOD(OnAlterOrder)(IOrder* Order);
STDMETHOD(OnCancelOrder)(IOrder* Order);
STDMETHOD(OnRequestOrderStatus)(IOrder* Order);
STDMETHOD(OnFixGatewayStatusRequest)(IFixGatewayStatusRequest* Request);
STDMETHOD(OnFixGatewayStatus)(IFixGatewayStatus* Status);

//Providers
STDMETHOD(OnCancelReject)(BSTR Reason, FX::FoOrderCancelReject* Report);
STDMETHOD(OnExecutionReport)(FX::FoExecutionReport* Report);
STDMETHOD(OnOrderError)(BSTR ClOrdID,	BSTR Description);
STDMETHOD(OnDisconnect)(long ProviderType);
STDMETHOD(OnReLogon)(long ProviderType);
STDMETHOD(OnLogonFailed)(long ProviderType,	BSTR Description);

HRESULT FinalConstruct();
void FinalRelease();


public:
	UINT  virtual	Run();

private:
	void	_ConnectProvider(FX::IOrderProviderPtr& spProvider, MsFixProviderEnum enLoc) throw (_com_error);
	void	_DisconnectProvider(MsFixProviderEnum enLoc);
	void	_ValidateProvidersMap(MsFixProviderEnum enLoc) throw (_com_error);
	void	_GetProvider(FX::IOrderProviderPtr& spProvider, MsFixProviderEnum enLoc) throw(_com_error);
	void	_InitProvidersMap();
	void	_ClearProvidersMap();

    HANDLE	_GetEventHandle(DISP_IDS dispid);
    static DWORD WINAPI _Thread_Order(LPVOID lpParam);
	bool	_QueueSetGatewaysCheckTimer(DWORD dwMilliseconds);
	bool	_QueueSetFixStartTimer(const COleDateTime& dtStartTime);
	bool	_CheckIsWorkingTime();
	bool	_QueueSetFixStopTimer(const COleDateTime& dtStopTime);
	void	_GetComputerInfo();

	FgServerStatusEnum _Status();
	void	_Status(FgServerStatusEnum enStatus);

	void	_FillOrder(IOrder* pOrderSrc, FX::FoOrder* pOrderDst) throw(_com_error);
	void	_FillOrderCancelReplaceRequest(IOrder* pOrder, FX::FoOrderCancelReplaceRequest* Request) throw(_com_error);
	void	_FillOrderCancelRequest(IOrder* pOrder, FX::FoOrderCancelRequest* Request) throw(_com_error);
	void	_FillOrderStatusRequest(IOrder* pOrder, FX::FoOrderStatusRequest* Request) throw(_com_error);
	void	_ExtractExecutionReport(FX::FoExecutionReport* pReportSrc, IExecutionReport* pReportDest) throw(_com_error);
	void	_ExtractExecutionReportToOrder(FX::FoExecutionReport* pReportSrc, IOrder* pOrder) throw(_com_error);
	void	_ExtractOrderCancelReject(FX::FoOrderCancelReject* Reject, IOrder* pOrder) throw(_com_error);	
	LPCTSTR	_OrderStatusDescription(FX::FoOrderStatusEnum enStatus) throw(_com_error);
	void	_PubError(IOrder* pOrder, _com_error& e, LPCTSTR lpszCaption = NULL);

public:
	static CFixGServer*			s_pFixGServer;

protected:
	static _ATL_FUNC_INFO		m_OnRequestNewOrder;
	static _ATL_FUNC_INFO		m_OnAlterOrder;
	static _ATL_FUNC_INFO		m_OnCancelOrder;
	static _ATL_FUNC_INFO		m_OnRequestOrderStatus;
	static _ATL_FUNC_INFO		m_OnFixGatewayStatusRequest;
	static _ATL_FUNC_INFO		m_OnFixGatewayStatus;

	static _ATL_FUNC_INFO		m_OnCancelReject;
	static _ATL_FUNC_INFO		m_OnExecutionReport;
	static _ATL_FUNC_INFO		m_OnOrderError;
	static _ATL_FUNC_INFO		m_OnDisconnect;
	static _ATL_FUNC_INFO		m_OnReLogon;
	static _ATL_FUNC_INFO		m_OnLogonFailed;

private:
	IPubManagerPtr				m_spPubManager;
	TEventsMap					m_mapEvents;
	TProvidersMap				m_mapProviders;
	FXGSM::CSmSettingsMgrPtr	m_spSettingsMgr;
	FgServerStatusEnum			m_enStatus;
	FXG::CFgOrderStorage		m_OrderStorage;
	FXG::CFgExecutionStorage	m_ExecStorage;
	CString						m_strHost;
	CString						m_strIP;

	HANDLE						m_hStartFixEvent;
	HANDLE						m_hStopFixEvent;
	HANDLE						m_hGatewaysCheckTimer;
	HANDLE						m_hGatewaysRuns;
	HANDLE						m_hStartFixTimer;
	HANDLE						m_hStopFixTimer;
};
OBJECT_ENTRY_AUTO(__uuidof(FixGServer), CFixGServer)

#endif //__FIXGSERVER_H_
