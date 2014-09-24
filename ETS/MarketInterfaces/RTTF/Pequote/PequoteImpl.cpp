#include "stdafx.h"

#include "Pequote.h"

HANDLE CPequote::m_hStopApplication = NULL;

#define ENABLE_QUICK_EDIT				0x0040L
#define ENABLE_INSERT_MODE				0x0020L

#define TRANSPORT_REGISTRY_KEY		    _T("Software\\Egar\\ETS\\Transport")
#define DB_REGISTRY_KEY			        _T("Software\\Egar\\ETS\\Database")

#define ROUTER_HOST					    _T("RouterHost")
#define DB_INFO					        _T("")

DWORD CPequote::Start()
{	
	//CEgRegKey key;
	CModuleVersion ver;
	TCHAR pModName [1024] = { 0 };
	TCHAR pVer[100] = { 0 };
    
    //detecting the module version
	::GetModuleFileName( NULL, pModName, sizeof(pModName)/sizeof(TCHAR));
	ver.GetModuleVersionInfo(pModName);		
	ver.GetValue(_T("ProductVersion"), pVer);

	DWORD dwFlags;
	if(GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwFlags))
	{
		dwFlags &= ~(ENABLE_QUICK_EDIT | ENABLE_INSERT_MODE);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), dwFlags);
	}

	m_hStopApplication = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!m_hStopApplication)
	{
		return TraceError();
	}
	
	if (!::SetConsoleCtrlHandler(HandlerRoutine, TRUE))
		return TraceError();

	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Starting Pequote connector..."));

	DWORD dwRes = m_PequoteConnector.Start(NULL);	
	if (dwRes)
	{
		CTracer::TraceMessage(CTracer::enMtError, NULL, _T("Failed to start Pequote connector."));
		return dwRes;
	}

	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Pequote connector started."));
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Server started."));
    
	return 0;
}

DWORD CPequote::Stop()
{
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Stopping Pequote connector..."));

	DWORD dwRes = m_PequoteConnector.Stop();
	
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Pequote connector stopped."));

	if (m_hStopApplication) 
	{
		::CloseHandle(m_hStopApplication);
		InterlockedExchange((LPLONG)&m_hStopApplication, NULL);
	}
	
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Server stopped."));
		
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Results of the last session:"));
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Received: %lu"), m_PequoteConnector.m_Publisher.m_dwReceived);
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("DB stored: %lu"), m_PequoteConnector.m_Publisher.m_dwDBStored);
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("DB deleted: %lu"), m_PequoteConnector.m_Publisher.m_dwDBDeleted);
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Published: %lu"), m_PequoteConnector.m_Publisher.m_dwPublished);
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Not published: %lu"), 
		(m_PequoteConnector.m_Publisher.m_dwReceived + m_PequoteConnector.m_Publisher.m_dwDBDeleted) - m_PequoteConnector.m_Publisher.m_dwPublished);

	return dwRes;
}

BOOL CPequote::HandlerRoutine(DWORD dwCtrlType)
{
	CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Server stopping..."));
	SetEvent(m_hStopApplication);
	return TRUE;
}

DWORD CPequote::Run()
{
	MSG		msg;
	while(TRUE)
	{
		switch (::MsgWaitForMultipleObjects(1, &m_hStopApplication, FALSE, INFINITE, QS_ALLEVENTS))
		{
			case WAIT_FAILED:
				return TraceError();
	
			case WAIT_OBJECT_0:		// m_hStopApplication
				return 0;

			default:
				while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
					DispatchMessage(&msg);
		}
	}

	return 0;
}
