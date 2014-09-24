// IVControl.cpp : Implementation of CIVControl
#include "stdafx.h"
#include "IVMCtrl.h"
#include "IVControl.h"
/////////////////////////////////////////////////////////////////////////////
// CIVControl


STDMETHODIMP CIVControl::SetActiveSymbol(BSTR SymbolName)
{
	Initialize();

	SMessage msg;
	msg.m_enType = enIVMSymbol;
	msg.m_vtMessage = _bstr_t(SymbolName);

	m_csMessages.Lock();
	m_Messages.push(msg);
	m_csMessages.Unlock();
	SetEvent(m_hEventMessage);
	return S_OK;
}

bool CIVControl::IsManagerStarted()
{
   HANDLE hStartMutex =  OpenMutex(MUTEX_ALL_ACCESS, FALSE,_T("{09F19211-F6A4-481f-B7DF-5F02FBAB56F9}"));
   bool bRet = hStartMutex==NULL?false:true;
   if(bRet)
	   CloseHandle(hStartMutex);
   
   return bRet;

}
void CIVControl::Initialize()
{
	if(!IsManagerStarted() && !m_lInitializing)
	{
		::ResetEvent(m_hEventStop);
		::InterlockedExchange((long*)&m_lInitializing, 1);

		UINT ID = 0;
		_beginthreadex(NULL, 0, StartManager, this,NULL, &ID);
	}
	
}

UINT CIVControl::StartManager(LPVOID pData)
{
	CIVControl* pThis = reinterpret_cast<CIVControl*>(pData);
	if(pThis)
	{
		TCHAR szPath[10240];
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		CEgRegKey key;
		_bstr_t bsPath = _T("");
		if(ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Egar\\MainLine"), KEY_READ))
		{
			key.QueryValue(bsPath, _T("Path"));
		}
		_bstr_t bsCommandLine;
		bsCommandLine += _T("\"");
		bsCommandLine += bsPath;
		bsCommandLine += _T("\\");
		bsCommandLine += _T("VolaManager.exe");
		bsCommandLine += _T("\"");

		USES_CONVERSION;
		LPTSTR pBuf = W2T((BSTR)bsCommandLine);

		PROCESS_INFORMATION  pi;
		STARTUPINFO si = {sizeof(si)};

		GetEnvironmentVariable(_T("Path"),szPath,10240);

		CEgRegKey regPath;
		regPath.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\VolaManager.exe"), KEY_READ);
		_bstr_t bsAddPath;
		regPath.QueryValue(bsAddPath, _T("Path"));
		
		_bstr_t bs = szPath;
		bs +=_T(";");
		bs +=bsAddPath;
		SetEnvironmentVariable(_T("Path"),(LPCTSTR)bs);

		BOOL bRet = ::CreateProcess(NULL, pBuf, NULL, NULL,FALSE,0,NULL,NULL/*"C:\\Program Files\\EGAR\\EMM.FW\\"*/,&si, &pi);
		if(!bRet)
		{
			DWORD dw = GetLastError();

		}
		else
		{
			while(1)
			{
				if(WAIT_TIMEOUT!= pThis->WaitWithEvents(1, &pThis->m_hEventStop,1000))
					break;
				if(!pThis->IsManagerStarted())
					continue;

//				CComVariant vt((BSTR)CComBSTR(L"MSFT"));

//				pThis->Fire_CommandEvent(enIVMSymbol, vt);
				SetEvent(pThis->m_hEventStarted);
				break;
			}
		}
		::InterlockedExchange((long*)&pThis->m_lInitializing, 0);
		CoUninitialize();
	}
	return 0;
}

UINT CIVControl::MessageManager(LPVOID pData)
{
	CIVControl* pThis = reinterpret_cast<CIVControl*>(pData);
	if(pThis)
	{
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		HANDLE hMessageEvents[] = 
		{
			pThis->m_hEventStop,
			pThis->m_hEventMessage
		};

		while(1)
		{
			if(WAIT_OBJECT_0==pThis->WaitWithEvents(2,hMessageEvents))
				break;
			while(1)
			{
				pThis->m_csMessages.Lock();
				long lSize = pThis->m_Messages.size() ;
				pThis->m_csMessages.Unlock();
				if(!lSize)
					break;

				pThis->m_csMessages.Lock();
				SMessage msg = pThis->m_Messages.front();
				pThis->m_Messages.pop();
				pThis->m_csMessages.Unlock();

				if(!pThis->IsManagerStarted())
				{
					HANDLE hStartEvents[] = 
					{
						pThis->m_hEventStop,
						pThis->m_hEventStarted
					};

					if(WAIT_OBJECT_0==pThis->WaitWithEvents(2,hStartEvents))
						break;
				}
				pThis->Fire_CommandEvent(msg.m_enType, msg.m_vtMessage);

			}
		}
		CoUninitialize();
	}
	return 0;
}
