#include "stdafx.h"
#include "statusthread.h"

UINT  CStatusThread::Thread(void* pParam)
{
	::CoInitializeEx(NULL,COINIT_MULTITHREADED);

	vector<HANDLE> vecHandles;
	vecHandles.push_back(GetStopEventHandle());

	DWORD dwRes = ::WaitForMultipleObjects(static_cast<DWORD>(vecHandles.size()),&vecHandles.front(),FALSE,INFINITE);

	::CoUninitialize();
	return 0;
}
