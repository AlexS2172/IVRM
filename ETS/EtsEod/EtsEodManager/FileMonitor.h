#pragma	once

#define  MAX_BUFFER     4096

class CFileMonitor
{
public:
	CFileMonitor();
	virtual ~CFileMonitor();

	DWORD FileOpen(LPCTSTR lpszFileName);
	DWORD Read(LPSTR lpBuf, DWORD& nSize);
	void FileClose();

	virtual void OnAppend(LPCSTR lpBuf, long nSize) = 0;
	void EnableNotify(BOOL bNotifyState);

protected:
	static UINT APIENTRY WaitAppendThread(void* pParam);

protected:
	HANDLE			m_hFile;
	HANDLE			m_hDir;
	HANDLE			m_hCompPort;
	HANDLE			m_hThread;
	TCHAR			m_szFileName[_MAX_PATH];
	WCHAR			m_szFileNameW[_MAX_PATH];
	CHAR			m_pBuffer[4096];
	DWORD			m_dwBuffer;
	OVERLAPPED		m_ovDir;
	BOOL			m_bNotifyEnabled;
};
