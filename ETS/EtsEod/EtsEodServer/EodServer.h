// EodServer.h : Declaration of the CEodServer

#pragma once
#include "resource.h"       // main symbols

#include "EtsEodServer.h"
#include "_IEodServerEvents_CP.h"
#include "EodGeneral.h"

// CEodServer

class ATL_NO_VTABLE CEodServer : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEodServer, &CLSID_EodServer>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CEodServer>,
	public CProxy_IEodServerEvents<CEodServer>, 
	public IDispatchImpl<IEodServer, &IID_IEodServer, &LIBID_EtsEodServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CThread,
	public EODCO::CEgReportable,
	public EODCO::CEgTraceable
{
public:
	CEodServer() : m_hStartEvent(NULL), m_hTaskStartTimer(NULL), m_hStopEodEvent(NULL)
	{
	}

DECLARE_CLASSFACTORY_SINGLETON(CEodServer)

DECLARE_REGISTRY_RESOURCEID(IDR_EODSERVER)

DECLARE_NOT_AGGREGATABLE(CEodServer)

BEGIN_COM_MAP(CEodServer)
	COM_INTERFACE_ENTRY(IEodServer)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CEodServer)
	CONNECTION_POINT_ENTRY(__uuidof(_IEodServerEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease() ;

public:
	STDMETHOD(StartEod)(void);
	STDMETHOD(StopEod)(void);
	STDMETHOD(AddTask)(ITask* pTask);
	STDMETHOD(RemoveTask)();
	STDMETHOD(GetTask)(ITask** ppTask);

	STDMETHOD(get_EodMask)(ULONG* pVal);
	STDMETHOD(put_EodMask)(ULONG newVal);
	STDMETHOD(get_EodStatus)(EodStatusEnum* pVal);
	STDMETHOD(get_EodCalcModel)(EodCalcModelTypeEnum* pVal);
	STDMETHOD(put_EodCalcModel)(EodCalcModelTypeEnum newVal);
	STDMETHOD(get_EodLogLevel)(int* pVal);
	STDMETHOD(put_EodLogLevel)(int newVal);
	STDMETHOD(get_EodLogHistory)(int* pVal);
	STDMETHOD(put_EodLogHistory)(int newVal);

public:
	UINT  virtual Run();

	EODSM::CSmSettingsMgrPtr SettingsMgr()
	{
		return m_spSettingsMgr;
	}
	EOD::CEodMktStructureMgrPtr MktStructureMgr()
	{
		return m_spMktStrMgr;
	}
	EOD::CEodStepManagerPtr StepsMgr()
	{
		return m_spStepsMgr;
	}
	/*EOD::CEodStepLoadIndexFuturesPtr IndexFuturesMgr()
	{
		return m_spIndexFutures;
	}*/

	static CEodServer*	s_pEodServer;

private:
	bool _QueueSetTaskStartTimer(const COleDateTime& dtStartTime);

	EODSM::CSmSettingsMgrPtr	m_spSettingsMgr;
	EOD::CEodMktStructureMgrPtr	m_spMktStrMgr;
	EOD::CEodStepManagerPtr		m_spStepsMgr;
	//EOD::CEodStepLoadIndexFuturesPtr		m_spIndexFutures;
	HANDLE						m_hStartEvent;
	HANDLE						m_hStopEodEvent;
	HANDLE						m_hTaskStartTimer;

	CXMLParamsHelper			m_XMLParams;
};

OBJECT_ENTRY_AUTO(__uuidof(EodServer), CEodServer)
