#ifndef _SAGE_H
#define _SAGE_H

#include "SageConnector.h"

extern HANDLE m_hStopApplication;

class CSage
{
public:
	DWORD Start();
	DWORD Stop();
	DWORD Run();
public:
	static HANDLE	m_hStopApplication;
	CSageConnector	m_SageConnector;
	long			m_nGroupsCount;
	
private:	
	static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
};

#endif