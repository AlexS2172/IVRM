#include "stdafx.h"
#include "Server.h"

#include <iostream>

CServer* theApplication = NULL;

BOOL ConsoleApplicationHandler(DWORD ctrl_id)
{
	std::cout << "Application has been interrupted externaly.\n";

	if (theApplication != NULL)
	{
		try
		{
				std::cout << "Stopping application ...\t";
				theApplication->Stop(true, 10000);			
				delete theApplication;
	
				std::cout << "...[ OK ]\n";
		}
		catch(...)
		{	
			std::cout << "...[ FAILED ]\n";
		}	
	}

	return FALSE;
};
 
int _tmain(int argc, _TCHAR* argv[]) {

	std::cout << "======= IVolatility Analytical Data Server =======\n"; 
	
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleApplicationHandler, TRUE);

	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	theApplication = new CServer;
	
	if (theApplication == NULL)
	{
		std::cout << "Failed while creating application. Abnormal termination!\n";
		return -1;
	}
	
	theApplication->Start();

	std::cout << "Press Ctrl-C for exit.\n\n\n"; 
	
	Sleep(INFINITE);
	
	::CoUninitialize();
	
	std::cout << "======= Exit =======.\n"; 

	return 0;
}

