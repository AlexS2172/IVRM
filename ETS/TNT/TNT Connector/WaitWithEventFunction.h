#pragma once
class WWE
{
public:
	static DWORD WaitWithEvents(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE, BOOL bMode = FALSE)
	{
		DWORD dwStatus = -1;
		ATLASSERT(nCount > 0);
		DWORD dwStart = GetTickCount();
		for(;;)
		{
			dwStatus = ::MsgWaitForMultipleObjects(nCount,pHandles,bMode,dwMilliseconds,QS_ALLEVENTS);
			
			if(dwStatus != WAIT_OBJECT_0+nCount) break;
			
			MSG msg;
			while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if(dwMilliseconds!=INFINITE)
			{
				dwMilliseconds -= (GetTickCount() - dwStart);
				dwStart = GetTickCount();
				if(dwMilliseconds<0)
					dwMilliseconds = 0;
			}

		}
		return dwStatus;	
	}
};