#include "stdafx.h"

#include "Sage.h"
#include "ModulVer.h"

HANDLE CSage::m_hStopApplication = NULL;

#define ENABLE_QUICK_EDIT				0x0040L
#define ENABLE_INSERT_MODE				0x0020L

#define TRANSPORT_REGISTRY_KEY		    _T("Software\\Egar\\ETS\\Transport")
#define DB_REGISTRY_KEY			        _T("Software\\Egar\\ETS\\Database")

#define ROUTER_HOST					    _T("RouterHost")
#define DB_INFO					        _T("")

DWORD CSage::Start()
{	
	USES_CONVERSION;
	
	//CEgRegKey key;	
	TCHAR pModName [1024] = { 0 };
	
    //detecting the module version
	::GetModuleFileName( NULL, pModName, sizeof(pModName) );
	
	CModuleVersion ver;
	ver.GetModuleVersionInfo(pModName);

	std::string sMsg("Fail to Setup user groups.");
	UserGroups clParams;
	m_nGroupsCount = GetCommandLineParams(clParams);
	if (m_SageConnector.SetGroups(clParams) == E_FAIL)
	{
		CTracer::TraceError(E_FAIL, sMsg.c_str());
		return E_FAIL;
	}

	
	TCHAR pVer[100] = { 0 };
	ver.GetValue(_T("ProductVersion"), pVer);

	CTracer::Trace(_T("------------------------------------"));
	CTracer::Trace(_T("FIX Interface Connector v.%s"), pVer );	
	CTracer::Trace(_T("Press Ctrl + C to stop the server..."));
	CTracer::Trace(_T("------------------------------------"));

	DWORD dwFlags;
	if(GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwFlags))
	{
		dwFlags &= ~(ENABLE_QUICK_EDIT | ENABLE_INSERT_MODE);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), dwFlags);
	}

	m_hStopApplication = CreateEvent(NULL, TRUE, FALSE, _T("E53B916B-95BA-4e1f-917A-338580A74D91"));
	if (m_hStopApplication == NULL)
	{
		return CTracer::TraceError();
	}

	BOOL bRes = SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	
	if (bRes == FALSE)
		return CTracer::TraceError();

	CTracer::Trace(_T("Starting Sage connector..."));

	DWORD dwRes = m_SageConnector.Start();
	
	if (dwRes)
	{
		CTracer::Trace(_T("Failed to start Sage connector."));
		return dwRes;
	}

	CTracer::Trace(_T("FIX Interface connector started."));
	CTracer::Trace(_T("Server started."));

	return 0;
}

DWORD CSage::Stop()
{
	CTracer::Trace(_T("Stopping FIX Interface connector..."));

	DWORD dwRes = m_SageConnector.Stop();
	
	CTracer::Trace(_T("FIX Interface connector stopped."));

	if (m_hStopApplication)
    {
		CloseHandle(m_hStopApplication);
        m_hStopApplication = 0;
    }

	CTracer::Trace(_T("Server stopped."));
	
	for (int i = 0; i < m_nGroupsCount; i++)
	{
		CTracer::Trace(_T("************************************"));
		CTracer::Trace(_T("Results of the last session:"));	
		CTracer::Trace(_T("UserGroup:\t%s"), (LPCTSTR)m_SageConnector.m_Publisher[i].m_bsUserGroup);
		CTracer::Trace(_T("DB:	\t%s"), (LPCTSTR)m_SageConnector.m_Publisher[i].m_bsConnStringLabel);
		CTracer::Trace(_T("Received:      \t%lu"), m_SageConnector.m_Publisher[i].m_dwReceived);
		CTracer::Trace(_T("DB stored:     \t%lu"), m_SageConnector.m_Publisher[i].m_dwDBStored);
		CTracer::Trace(_T("DB deleted:    \t%lu"), m_SageConnector.m_Publisher[i].m_dwDBDeleted);
		CTracer::Trace(_T("Published:     \t%lu"), m_SageConnector.m_Publisher[i].m_dwPublished);
		CTracer::Trace(_T("Not published: \t%lu"), m_SageConnector.m_Publisher[i].m_dwReceived - 
													m_SageConnector.m_Publisher[i].m_dwPublished );
	}

	return dwRes;
}

BOOL CSage::HandlerRoutine(DWORD dwCtrlType)
{
	//if (dwCtrlType == CTRL_C_EVENT)
	//{
		CTracer::Trace(_T("Server stopping..."));
		SetEvent(m_hStopApplication);
		return TRUE;
	//}
	
	//return FALSE;
}

DWORD CSage::Run()
{
    MSG		msg = {0};

	while (true)
	{
		DWORD dwRes = MsgWaitForMultipleObjects(1, &m_hStopApplication, FALSE, INFINITE, QS_ALLEVENTS);
		switch (dwRes)
		{
			case WAIT_FAILED:	
				return CTracer::TraceError();
			case WAIT_OBJECT_0:		// m_hStopApplication
				return 0;
			default:
				while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
		}
	}

	return 0;
}
