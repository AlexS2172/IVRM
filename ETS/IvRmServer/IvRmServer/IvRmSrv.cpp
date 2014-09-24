// IvRmServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CXmlSettings settings;
	BSTR bsSymbol;
	settings.GetDBConnection(&bsSymbol);

	CServer server;
	server.Start();

	_getch();	
	return 0;
}

