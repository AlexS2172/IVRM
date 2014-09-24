// IVControl.h : Declaration of the CIVControl

#ifndef __IVCONTROL_H_
#define __IVCONTROL_H_

#include "resource.h"       // main symbols
#include "IVMCtrlCP.h"

#include <Process.h>
#include <queue>

struct    SMessage
{
	IVMCommandTypeEnum  m_enType;
	_variant_t          m_vtMessage;
	
	SMessage(){m_enType=enIVMNone;}
	SMessage(const SMessage& c){Copy(c);}
	SMessage& operator=(const SMessage& c){Copy(c); return *this;}
private:
	
	void Copy(const SMessage& c)
	{
		m_enType	= c.m_enType;
		m_vtMessage = c.m_vtMessage;
	}
};
/////////////////////////////////////////////////////////////////////////////
// CIVControl
class ATL_NO_VTABLE CIVControl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIVControl, &CLSID_IVControl>,
	public ISupportErrorInfoImpl<&IID_IIVControl>,
	public IConnectionPointContainerImpl<CIVControl>,
	public IDispatchImpl<IIVControl, &IID_IIVControl, &LIBID_IVMCTRLLib>,
	public CProxy_IIVControlEvents< CIVControl >
{
		typedef	std::queue<SMessage>  MESSAGE;
public:
	CIVControl()
	{
		m_hEventStop    = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_hEventMessage = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hEventStarted = CreateEvent(NULL, FALSE, FALSE, NULL);
		
		m_lInitializing = 0L;
		m_lInitialized  = 0L;

	}

	HRESULT FinalConstruct()
	{
		UINT ID = 0;
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, MessageManager, this,NULL, &ID);
		return S_OK;

	}
DECLARE_REGISTRY_RESOURCEID(IDR_IVCONTROL)
DECLARE_CLASSFACTORY_SINGLETON(CIVControl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIVControl)
	COM_INTERFACE_ENTRY(IIVControl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CIVControl)
CONNECTION_POINT_ENTRY(DIID__IIVControlEvents)
END_CONNECTION_POINT_MAP()

	void FinalRelease()
	{
		SetEvent(m_hEventStop);

		::CloseHandle(m_hEventMessage);
		::CloseHandle(m_hEventStarted);
		WaitWithEvents(1,&m_hThread,150000);
	}

// IIVControl
public:
	STDMETHOD(SetActiveSymbol)(/*[in]*/ BSTR SymbolName);

private:
	DWORD WaitWithEvents(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE, BOOL bMode = FALSE)
	{
		DWORD dwStatus = -1;
		ATLASSERT(nCount > 0);
		for(;;)
		{
			dwStatus = ::MsgWaitForMultipleObjects(nCount,pHandles,bMode,dwMilliseconds,QS_ALLEVENTS);
			
			if(dwStatus != WAIT_OBJECT_0+nCount) break;
			
			MSG msg;
			while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return dwStatus;		
	}

private:
	void    Initialize();

	static UINT WINAPI StartManager(LPVOID pData);
	static UINT WINAPI MessageManager(LPVOID pData);

	HANDLE  m_hEventStop;
	HANDLE  m_hEventMessage;
	HANDLE  m_hEventStarted;
	HANDLE  m_hThread;

	bool IsManagerStarted();
	
	volatile LONG    m_lInitializing;
	volatile LONG    m_lInitialized;

	MESSAGE						m_Messages;
	CComAutoCriticalSection		m_csMessages;
};

#endif //__IVCONTROL_H_
