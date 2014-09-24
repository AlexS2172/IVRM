#include "stdafx.h"

#include "Sage.h"

CComModule _Module;
//HINSTANCE hInstance;

DWORD	CheckLogsFolder()
{
	_TCHAR szBuf[_MAX_PATH];
	DWORD dwErr = GetModuleFileName(NULL, szBuf, _MAX_PATH);
	if(dwErr == 0)
		return GetLastError();

	_TCHAR szDrive[_MAX_DRIVE];
	_TCHAR szDir[_MAX_DIR];
	_TCHAR szFname[_MAX_FNAME];
	_TCHAR szExt[_MAX_EXT];
	_tsplitpath( szBuf, szDrive, szDir, szFname, szExt );

	_TCHAR szLogsPath[_MAX_PATH];
	_TCHAR szCurrentPath[_MAX_PATH];
	_stprintf(szLogsPath, _T("%s%sLogs"), szDrive, szDir);
	_stprintf(szCurrentPath, _T("%s%s"), szDrive, szDir);

	BOOL bRes = CreateDirectory(szLogsPath, NULL);
	if(!bRes)
	{
		dwErr = GetLastError();
		if(ERROR_ALREADY_EXISTS != dwErr)
			return dwErr;
	}

	bRes = SetCurrentDirectory(szCurrentPath);
	if(!bRes)
		return GetLastError();

	return 0;
}

int main(int argc, char* argv[])
{
	DWORD dwRes;

	dwRes = CheckLogsFolder();
	if(dwRes)
	{
		CTracer::TraceError(dwRes, _T("Fail to check Logs folder"));
		return -1;
	}

	CoInitializeEx(NULL, COINIT_MULTITHREADED );

	CTracer::TraceInit();
	
	_Module.Init(NULL, (HINSTANCE )GetCurrentProcess());

    {
    	CSage	theApp;
		
	    dwRes = theApp.Start();
	    if (dwRes)
	    {
		    CTracer::Trace(_T("Failed to start application."));
	    }
	    else
	    {
			CTracer::SetPublisher(theApp.m_SageConnector.m_Publisher);
		    dwRes = theApp.Run();
		    if (dwRes)
		    {
			    CTracer::Trace(_T("Failed to run application."));
		    }
		    else
		    {
			    dwRes = theApp.Stop();
			    if (dwRes)
			    {
				    CTracer::Trace(_T("While stopping some errors occured."));
			    }
		    }
	    }
    }

	_Module.Term();
	CoUninitialize();

	return 0;
}

