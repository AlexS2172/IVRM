// TestRttfServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rttfserver.h"
#include "data.h"

CRttfServer	g_Server;

int _tmain(int argc, TCHAR * argv[])
{
	if (argc != 2)
	{
		_tprintf(_T("usage: %s <port>\n"), argv[0]);
		exit(-1);
	}

	if(!LoadTrades())
	{
		_tprintf(_T("Could not load trades from file"));
		exit(-1);
	}

	DWORD dwErr;

	if(dwErr = g_Server.Start(argv[1]))
	{
		_tprintf(_T("OS Error : %d\n"), dwErr);
		exit(-1);
	}

	_tprintf(_T("Server running.\n"), dwErr);

	while(!_kbhit())
		Sleep(100);

	g_Server.Stop();

	_tprintf(_T("Server stopped.\n"), dwErr);

	printf("Ok!\n");
	return 0;
}

