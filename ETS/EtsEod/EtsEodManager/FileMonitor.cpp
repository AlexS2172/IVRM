#include "StdAfx.h"
#include "filemonitor.h"

CFileMonitor::CFileMonitor()
	: m_hFile(INVALID_HANDLE_VALUE),
	m_hDir(INVALID_HANDLE_VALUE),
	m_hCompPort(0),
	m_hThread(0),
	m_dwBuffer(0),
	m_bNotifyEnabled(FALSE)
{
	memset(&m_ovDir, 0, sizeof(m_ovDir));
	memset(m_pBuffer, 0, sizeof(m_pBuffer));
}

CFileMonitor::~CFileMonitor()
{
	FileClose();
}

DWORD CFileMonitor::FileOpen(LPCTSTR lpszFileName)
{
	FileClose();

	if(!lpszFileName)
		return ERROR_INVALID_PARAMETER;

	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);

	if(INVALID_HANDLE_VALUE == hFile)
		return GetLastError();

	m_hFile = hFile;

	TCHAR szDrive[_MAX_PATH] = {0};
	TCHAR szDir[_MAX_DIR] = {0};
	TCHAR szName[_MAX_FNAME] = {0};
	TCHAR szExt[_MAX_EXT] = {0};

	_tsplitpath_s(lpszFileName, szDrive,sizeof(szDrive), szDir, sizeof(szDir), szName,sizeof(szName), szExt, sizeof(szExt));
	// copy to szDrive buffer directory name
	_tcscat_s(szDrive, sizeof(szDrive), szDir);
	
	hFile = CreateFile(szDrive, FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return GetLastError();
	}

	m_hDir = hFile;

	hFile = CreateIoCompletionPort(m_hDir, NULL, (ULONG_PTR)this, 0);

	if(!hFile)
	{
		CloseHandle(m_hFile);
		CloseHandle(m_hDir);
		
		m_hFile = INVALID_HANDLE_VALUE;
		m_hDir = INVALID_HANDLE_VALUE;

		return GetLastError();
	}

	m_hCompPort = hFile;

	BOOL bRes = ReadDirectoryChangesW(m_hDir, &m_pBuffer, sizeof(m_pBuffer), FALSE,
		FILE_NOTIFY_CHANGE_LAST_WRITE, &m_dwBuffer, &m_ovDir, NULL);

	_tcscpy_s(m_szFileName, sizeof(m_szFileName), szName);
	_tcscat_s(m_szFileName, sizeof(m_szFileName), szExt);
	
	USES_CONVERSION;
	wcscpy_s(m_szFileNameW, sizeof(m_szFileNameW)/sizeof(wchar_t), T2W(m_szFileName));
	

	unsigned int nThreadID;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, WaitAppendThread, m_hCompPort, 0, &nThreadID);

	return ERROR_SUCCESS;
}

UINT APIENTRY CFileMonitor::WaitAppendThread(void* pParam)
{
	//USES_CONVERSION;

	DWORD numBytes;
	DWORD cbOffset;
	LPOVERLAPPED lpOverlapped;
	PFILE_NOTIFY_INFORMATION fni;
	CFileMonitor* pThis;
	DWORD dwRes;
	DWORD dwSize;

	do
	{
		// Retrieve the directory info for this directory
		// through the completion key
		BOOL bRet =	GetQueuedCompletionStatus((HANDLE)pParam, &numBytes,
								(PULONG_PTR)&pThis, &lpOverlapped, INFINITE);

		if(pThis)
		{
			fni = (PFILE_NOTIFY_INFORMATION)pThis->m_pBuffer;

			do
			{
				if(fni->Action== FILE_ACTION_MODIFIED &&
					0 == wcsncmp(fni->FileName, pThis->m_szFileNameW,
					fni->FileNameLength / sizeof(WCHAR)))
				{
					do
					{
						if(!pThis->m_bNotifyEnabled)
							break;

						CHAR pBuffer[MAX_BUFFER] = {0};
						dwSize = sizeof(pBuffer);

						dwRes = pThis->Read(pBuffer, dwSize);
						if(ERROR_SUCCESS == dwRes && dwSize > 0)
							pThis->OnAppend(pBuffer, dwSize);
					}
					while(ERROR_SUCCESS == dwRes && dwSize > 0);
				}

				cbOffset = fni->NextEntryOffset;
				fni = (PFILE_NOTIFY_INFORMATION)((LPBYTE)fni + cbOffset);

			}
			while(cbOffset);

			memset(pThis->m_pBuffer, 0, sizeof(pThis->m_pBuffer));

			// Reissue the watch command
			BOOL bRes = ReadDirectoryChangesW(pThis->m_hDir, &pThis->m_pBuffer,
				sizeof(pThis->m_pBuffer), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE,
				&pThis->m_dwBuffer, &pThis->m_ovDir, NULL);
		}

	}
	while(pThis);

	return 0;
}

DWORD CFileMonitor::Read(LPSTR lpBuf, DWORD& nSize)
{
	if(!nSize)
		return ERROR_SUCCESS;

	if(!lpBuf)
		return ERROR_INVALID_PARAMETER;

	if(INVALID_HANDLE_VALUE == m_hFile)
		return ERROR_INVALID_HANDLE;

	DWORD dwRead = 0;

	if(!ReadFile(m_hFile, lpBuf, nSize, &dwRead, NULL))
		return GetLastError();

	nSize = dwRead;

	return ERROR_SUCCESS;
}

void CFileMonitor::FileClose()
{
	m_bNotifyEnabled = FALSE;
	
	if(m_hCompPort)
	{
		PostQueuedCompletionStatus(m_hCompPort, 0, 0, NULL);

		if(m_hThread)
		{
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
			m_hThread = 0;
		}

		CloseHandle(m_hCompPort);
		m_hCompPort = 0;
	}

	if(INVALID_HANDLE_VALUE != m_hDir)
	{
		CloseHandle(m_hDir);
		m_hDir = INVALID_HANDLE_VALUE;
	}

	if(INVALID_HANDLE_VALUE != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	memset(&m_ovDir, 0, sizeof(m_ovDir));
	memset(m_pBuffer, 0, sizeof(m_pBuffer));
}

void CFileMonitor::EnableNotify(BOOL bNotifyState)
{
	m_bNotifyEnabled = bNotifyState;
}
