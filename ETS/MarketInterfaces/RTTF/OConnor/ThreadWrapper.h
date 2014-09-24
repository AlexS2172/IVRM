#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#include "Tracer.h"

class CThreadWrapper
{

public:

	DWORD __stdcall Start(void* pParam);

	DWORD __stdcall Stop();

	UINT  virtual __stdcall Thread(void* pParam) = 0;

private:

	static UINT __stdcall ThreadProc( void *pThisClass);

protected:

	void* m_pParam;

	const HANDLE __stdcall GetThreadHandle() const {return m_hThread;}

	const HANDLE __stdcall GetStopEventHandle() const {return m_hStopEvent;}
	
	BOOL   __stdcall IsStopping();

private:

	HANDLE m_hThread;
	HANDLE m_hStopEvent;
};

#endif