// CRS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RttfClient.h"
#include "trace.h"
#include "Publisher.h"
#include "portfolio.h"
#include "crs.h"
#include "versioninfo.h"

HANDLE CCrs::m_hStopApplication = NULL;

DWORD CCrs::Start()
{
	if ( FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)) )
		return ERROR_GEN_FAILURE;		

	DWORD		dwPort = 0;

	_bstr_t		bsHost;
	_bstr_t		bsUserId;
	_bstr_t		bsPassword;
	_bstr_t		bsSecurityId;	

	try
	{
		CEgRegKey key;
		
		LONG lResult = key.Open(HKEY_CURRENT_USER, SETTINGS_REGISTRY_KEY, KEY_READ);
		if (lResult != ERROR_SUCCESS )
			lResult = key.Open (HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_READ);

		if (lResult != ERROR_SUCCESS) 
			throw lResult;

		lResult = key.QueryValue (bsHost, SETTINGS_SERVERADDR);
		if (lResult != ERROR_SUCCESS) 
			throw lResult;

		lResult = key.QueryValue (dwPort, SETTINGS_SERVERPORT);
		if (lResult != ERROR_SUCCESS) 
			throw lResult;

		lResult = key.QueryValue (bsUserId, SETTINGS_USERID);
		if (lResult != ERROR_SUCCESS) 
			throw lResult;

		lResult = key.QueryValue (bsPassword, SETTINGS_PASSWORD);
		if (lResult != ERROR_SUCCESS) 
			throw lResult;

		lResult = key.QueryValue (bsSecurityId, SETTINGS_SECURITYID);
		if (lResult != ERROR_SUCCESS) 
			throw lResult;
	}
	catch(const LONG nError)
	{
		TraceError(nError, _T("Failed to read connection settings from the registry."));
		return nError;
	}

	DWORD dwErr = m_Portfolio.Init();
	if(dwErr)
		return dwErr;

	dwErr = m_Publisher.Start();
	if(dwErr)
		return dwErr;

	return m_RttfClient.Start(bsHost, dwPort, bsUserId, bsPassword, bsSecurityId);
};

BOOL __stdcall CCrs::HandlerRoutine(DWORD dwCtrlType)
{
	SetEvent(m_hStopApplication);
	return TRUE;
}

void CCrs::Shutdown()
{
	HandlerRoutine(0);
}


DWORD CCrs::Run()
{
	m_hStopApplication = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hStopApplication == NULL)
	{
		return TraceError(GetLastError(), "Failed to create event.");
	}

	BOOL bRes = SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	
	if (bRes == FALSE)
		return TraceError(GetLastError(), "Failed to set control handler.");

	Trace("Press Ctrl + C to stop application...");
		
	MSG		msg;
	HANDLE	hHandles[] = {m_hStopApplication};

	while (true)
	{
		DWORD dwRes = MsgWaitForMultipleObjects(sizeof(hHandles) / sizeof(HANDLE), 
													hHandles, FALSE, INFINITE, QS_ALLEVENTS);

		switch (dwRes)
		{
		case WAIT_FAILED:	
			return TraceError(GetLastError(), "MsgWaitForMultipleObjects error");
	
		case WAIT_OBJECT_0:		// m_hStopApplication
			return 0;

		default:
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				DispatchMessage(&msg);
			}
		}
	}

	return 0;
}

DWORD CCrs::Stop()
{
	m_RttfClient.Stop();

	m_Publisher.Stop();

	m_Portfolio.Exit();

	::CoUninitialize();

	return 0;
};

CCrs theApp;


#ifdef _DEBUG
	#include <crtdbg.h>
	_CrtMemState s1, s2, s3;
#endif

int main(int argc, char* argv[])
{
#ifdef _DEBUG
   _CrtMemCheckpoint( &s1 );
#endif

	InitMainLog();
	
	CVersionInfo aVerInfo;
	std::string sVerInfo = std::string("FOC Connector version ") + aVerInfo.GetVersionAsString();

	Trace( _T("******************************************************") );
	Trace( A2T((char*)sVerInfo.c_str()) );
	Trace( _T("******************************************************") );

	Trace(_T("Starting..."));

	DWORD dwErr = theApp.Start();
	if(!dwErr)
	{
		Trace( _T("Server started.") );
		dwErr = theApp.Run();
		if(dwErr)
			Trace( _T("Failed to run server.") );
	}
	else
	{
		Trace( _T("Failed to start server.") );
	}

	Trace( _T("Stopping server...") );
	theApp.Stop();

	Trace( _T("---- STATISTICS -----") );
	Trace( _T("NEW operations, total %d"), theApp.m_Publisher.m_dwNewTotal );
	Trace( _T("DELETE operations, total %d"), theApp.m_Publisher.m_dwDeleteTotal );
	Trace( _T("Published %d trade(s)"), theApp.m_Publisher.m_dwPublished );
	
	Trace( _T("Server stopped.") );

#ifdef _DEBUG
   _CrtMemCheckpoint( &s2 );

   if ( _CrtMemDifference( &s3, &s1, &s2 ) )
      _CrtMemDumpStatistics( &s3 );
   else
	   OutputDebugString( _T("NO MEMORY LEAKS!!") );
#endif

	return 0;
}

