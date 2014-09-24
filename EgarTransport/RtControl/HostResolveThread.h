// HostResolveThread.h: interface for the CHostResolveThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOSTRESOLVETHREAD_H__E48E43F9_523B_47DD_B3AD_668BB6753FD0__INCLUDED_)
#define AFX_HOSTRESOLVETHREAD_H__E48E43F9_523B_47DD_B3AD_668BB6753FD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"
#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibSync.h>
using namespace EgLib;

class CHostResolveThread  : public CThread
{
protected:

	typedef map< unsigned long,bool >			MAP_HOSTS_ADDR;
	typedef MAP_HOSTS_ADDR::iterator			MAP_HOSTS_ITERATOR;
		
public:
	CHostResolveThread(){};

	void ResolveHostName(unsigned long Addr, bool IsConnected)
	{
		CAutoLock	Lock(m_Lock);

		m_mapHostsAddr.insert(MAP_HOSTS_ADDR::value_type(Addr,IsConnected));		
		
		SetEvent(m_hNeedResolve);
	}

	DWORD Start()
	{
		CAutoLock	Lock(m_Lock);

		if (m_hNeedResolve == NULL)
			m_hNeedResolve.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));

		if (m_hNeedResolve == NULL)
			return GetLastError();

		m_mapHostsAddr.clear();

		return CThread::Start();
	}

	DWORD Stop()
	{
		CAutoLock	Lock(m_Lock);

		DWORD dwErr = CThread::Stop();

		m_hNeedResolve.Close();
		m_mapHostsAddr.clear();

		return dwErr;
	}
	UINT Run()
	{
		HANDLE hStopEvent = GetStopEventHandle();
		const HANDLE hEvents[] = {m_hNeedResolve, hStopEvent};

		while(true)
		{
			DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), 
				hEvents, FALSE, INFINITE);

			if (dwRes == WAIT_OBJECT_0 + 1) /* Stop event */
			{
				return 0;
			}

			while(true)
			{
				unsigned long ulHostAddr;
				bool bIsConnected;

				{
					CAutoLock	lock(m_Lock);
					
					if(m_mapHostsAddr.empty())
						break;

					MAP_HOSTS_ITERATOR iter = m_mapHostsAddr.begin();
					if(iter == m_mapHostsAddr.end())
						break;

					ulHostAddr = iter->first;
					bIsConnected = iter->second;

					m_mapHostsAddr.erase(iter);
				}
				
				const hostent* pAddr = ::gethostbyaddr((const char*)&ulHostAddr, 
					sizeof(ulHostAddr), PF_INET );

				CString strHostName;
				
				if(pAddr)
				{
					strHostName = pAddr->h_name;
					strHostName += _T(" (");

					strHostName += inet_ntoa(*(in_addr*)&ulHostAddr);

					strHostName += _T(")");
				}
				else
				{
					strHostName = inet_ntoa(*(in_addr*)&ulHostAddr);
				}


				CMainFrame* pWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();
				
				if(!pWnd)
					continue;

				if(bIsConnected)
					pWnd->AddClient(strHostName,ulHostAddr);
				else
					pWnd->RemoveClient(strHostName);

			}

		}		
	}

private:
	CHandle					m_hNeedResolve;
	
	MAP_HOSTS_ADDR			m_mapHostsAddr;

	CLock					m_Lock;
};

#endif // !defined(AFX_HOSTRESOLVETHREAD_H__E48E43F9_523B_47DD_B3AD_668BB6753FD0__INCLUDED_)
