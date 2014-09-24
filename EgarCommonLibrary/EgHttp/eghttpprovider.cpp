#include "stdafx.h"
#include "EgHttpProvider.h"
#include "EgHttpError.h"
#include "resource.h"

#define BUFFER_INITIAL_SIZE   1024
#define BUFFER_GROWING_SIZE   1024
#define BUFFER_MAXIMUM_SIZE   (1024*20)

//#define VIEW_INTERNET_OPTION

/////////////////////////////////////////////////////////////////////////////
// Initialize http session.
void CEgHttpProvider::InitSession(const CEgHttpConnectionParam& crcpParams)
{
#ifdef _DEBUG
	if (m_dwThreadID != 0 && m_dwThreadID != GetCurrentThreadId())
		ATLASSERT (!EGHTTP_E_USED_IN_ANOTHER_THREAD);
#endif

	if (m_hSession != NULL)
	{
		ATLTRACE(_T("Warning: implicit calling CloseSession()\n"));
		CloseSession();
	}

	m_cpParams = crcpParams;
	m_cpParams.m_strServerAddress.Trim();
	if (_tcsnicmp(m_cpParams.m_strServerAddress, _T("http://"), 7) != 0)
		m_cpParams.m_strServerAddress = _T("http://") + m_cpParams.m_strServerAddress;

	URL_COMPONENTS ucBuffer = { sizeof(ucBuffer), NULL, 0, (INTERNET_SCHEME)0, 
		NULL, 10, 0, NULL, 10, NULL, 10, NULL, 10, NULL, 10 };

	if (!InternetCrackUrl(m_cpParams.m_strServerAddress,0, 0, &ucBuffer) ||
		(ucBuffer.nScheme != INTERNET_SCHEME_DEFAULT && // used not HTTP protocol
			(ucBuffer.nScheme != INTERNET_SCHEME_HTTP || ucBuffer.nPort != 80)) || // Port present
		ucBuffer.lpszHostName  == NULL || // HostName not present
		ucBuffer.lpszHostName[ucBuffer.dwHostNameLength] == _T(':') || 
		ucBuffer.lpszUserName  != NULL || // UserName present
		ucBuffer.lpszPassword  != NULL || // Password present
		(ucBuffer.lpszUrlPath  != NULL && ucBuffer.dwUrlPathLength > 0) || // URL-path present
		ucBuffer.lpszExtraInfo != NULL)   // extra information present
	{
		::SetLastError(ERROR_INTERNET_INVALID_URL);

		CEgString strMsg;
		strMsg.Format(_T("The format of the URL \'%s\' is invalid"), (LPCSTR) m_cpParams.m_strServerAddress);
		CEgHttpError::ThrowHttpError(strMsg);
	}

	CEgString strAgent;
	strAgent.LoadString((UINT)IDS_AGENT_NAME);

	// default enEgPsDefault case
	DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;

	CEgString strProxy;
	LPCTSTR ptszProxy = NULL;

	switch (m_cpParams.m_enProxySettings)
	{
	case enEgPsDisabled:
		dwAccessType = INTERNET_OPEN_TYPE_DIRECT;
		break;

	case enEgPsCustom:
		{
		dwAccessType = INTERNET_OPEN_TYPE_PROXY;
		strProxy.Format(_T("http=%s:%d"), m_cpParams.m_strProxyAddress, m_cpParams.m_lProxyPort);
		ptszProxy = strProxy;
		}
		break;
	}

	m_hSession = ::InternetOpen(strAgent, dwAccessType, ptszProxy, NULL, 0);
		
	if (m_hSession == NULL)
	{
		CEgString strMsg((LPCSTR)IDS_EGHTTP_E_CANNOT_OPEN_SESSION);
		CEgHttpError::ThrowHttpError(strMsg);
	}
#ifdef _DEBUG
	m_dwThreadID = GetCurrentThreadId();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Open url relative to server address.
void CEgHttpProvider::OpenURL(CEgString strURL)
{
#ifdef _DEBUG
	if (m_dwThreadID != 0 && m_dwThreadID != GetCurrentThreadId())
		ATLASSERT (!EGHTTP_E_USED_IN_ANOTHER_THREAD);
#endif

	if (m_hSession == NULL)
	{
		CEgString strMsg((LPCSTR)IDS_EGHTTP_E_NOT_OPENED_SESSION);
		CEgHttpError::ThrowError(EGHTTP_E_NOT_OPENED_SESSION, strMsg);
	}

	if (m_hFile != NULL)
	{
		ATLTRACE(_T("Warning: URL already opened!\n"));
		CloseURL();
	}

	CEgString strAddress;
	if (m_cpParams.m_lServerPort != 0)
		strAddress.Format(_T("%s:%d/%s"), m_cpParams.m_strServerAddress, m_cpParams.m_lServerPort, strURL);
	else
		strAddress.Format(_T("%s/%s"), m_cpParams.m_strServerAddress, strURL);

	m_hFile = ::InternetOpenUrl(m_hSession, strAddress, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	if (m_hFile == NULL)
	{
		CEgString strMsg((LPCSTR)IDS_EGHTTP_E_CANNOT_OPEN_URL);
		CEgHttpError::ThrowHttpError(strMsg);
	}

	if (m_cpParams.m_enProxySettings == enEgPsCustom)
	{
		if (!InternetSetOption(m_hFile, INTERNET_OPTION_PROXY_USERNAME, 
				(PVOID)(LPCTSTR)m_cpParams.m_ProxyAcount.m_strLogin, 
				(DWORD)m_cpParams.m_ProxyAcount.m_strLogin.GetLength() + 1) ||
			!InternetSetOption(m_hFile, INTERNET_OPTION_PROXY_PASSWORD, 
				(PVOID)(LPCTSTR)m_cpParams.m_ProxyAcount.m_strPassword, 
				(DWORD)m_cpParams.m_ProxyAcount.m_strPassword.GetLength() + 1))
		{
			CEgString strMsg((LPCSTR)IDS_EGHTTP_E_CANNOT_OPEN_URL);
			CEgHttpError::ThrowHttpError(strMsg);
		}
	}
#if defined(_DEBUG) && defined(VIEW_INTERNET_OPTION)
	{
		ULONG ulData = 0;
		DWORD  dwSize = sizeof(ulData);
		BOOL bRet = InternetQueryOption(m_hFile, INTERNET_OPTION_CONNECT_TIMEOUT, &ulData, &dwSize);
		if (bRet)
			ATLTRACE(_T("!!! Info: INTERNET_OPTION_CONNECT_TIMEOUT = %u\n"), ulData);

		bRet = InternetQueryOption(m_hFile, INTERNET_OPTION_CONNECT_RETRIES, &ulData, &dwSize);
		if (bRet)
			ATLTRACE(_T("!!! Info: INTERNET_OPTION_CONNECT_RETRIES = %u\n"), ulData);

		dwSize = sizeof(ulData);
		bRet = InternetQueryOption(m_hFile, INTERNET_OPTION_READ_BUFFER_SIZE, &ulData, &dwSize);
		if (bRet)
			ATLTRACE(_T("!!! Info: INTERNET_OPTION_READ_BUFFER_SIZE = %u\n"), ulData);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Read a portion of data. Returns false if no more data available. 
// Control buffer size inside.
bool CEgHttpProvider::ReadData(CEgString& rstrData)
{
	rstrData.Empty();

	ATLASSERT (BUFFER_INITIAL_SIZE > 1 && BUFFER_INITIAL_SIZE < BUFFER_MAXIMUM_SIZE);
	ATLASSERT (BUFFER_GROWING_SIZE > 0 && BUFFER_GROWING_SIZE < BUFFER_MAXIMUM_SIZE);

	if (m_hSession == NULL)
	{
		CEgString strMsg((LPCSTR)IDS_EGHTTP_E_NOT_OPENED_SESSION);
		CEgHttpError::ThrowError(EGHTTP_E_NOT_OPENED_SESSION, strMsg);
	}

	if (m_hFile == NULL)
	{
		CEgString strMsg((LPCSTR)IDS_EGHTTP_E_NOT_OPENED_URL);
		CEgHttpError::ThrowError(EGHTTP_E_NOT_OPENED_URL, strMsg);
	}

#ifdef _DEBUG
	if (m_dwThreadID != GetCurrentThreadId())
		ATLASSERT (!EGHTTP_E_USED_IN_ANOTHER_THREAD);
#endif

	DWORD dwRead;
	while (true)
	{
		if (m_spbyBuffer == NULL)
		{
			if (!m_spbyBuffer.Allocate(BUFFER_INITIAL_SIZE))
			{
				void *pMsgBuf;
				::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, ERROR_NOT_ENOUGH_MEMORY, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
					(LPTSTR)&pMsgBuf, 0, NULL );

				CEgString strMsg((LPCSTR) pMsgBuf);
				LocalFree( pMsgBuf );
				CEgHttpError::ThrowError(ERROR_NOT_ENOUGH_MEMORY, strMsg);
			}
			m_dwBufferSize = BUFFER_INITIAL_SIZE;
		}

		BOOL bResult = ::InternetReadFile(m_hFile, m_spbyBuffer, m_dwBufferSize - 1, &dwRead);

		if (!bResult)
		{
			if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (!m_spbyBuffer.Reallocate(m_dwBufferSize + BUFFER_GROWING_SIZE))
				{
					m_spbyBuffer.Free();
					m_dwBufferSize = 0;

					void *pMsgBuf;
					::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
						NULL, ERROR_NOT_ENOUGH_MEMORY, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
						(LPTSTR)&pMsgBuf, 0, NULL );

					CEgString strMsg((LPCSTR) pMsgBuf);
					LocalFree( pMsgBuf );
					CEgHttpError::ThrowError(ERROR_NOT_ENOUGH_MEMORY, strMsg);
				}
				ATLASSERT ((m_dwBufferSize + BUFFER_GROWING_SIZE) < BUFFER_MAXIMUM_SIZE);
				m_dwBufferSize += BUFFER_GROWING_SIZE;
				continue;
			}
			else
			{
				CEgString strMsg((LPCSTR)IDS_EGHTTP_E_CANNOT_READ_DATA);
				CEgHttpError::ThrowHttpError(strMsg);
			}
		}
		else
			break;
	}

	if (dwRead != 0)
	{
		m_spbyBuffer[dwRead] = 0;
		rstrData = reinterpret_cast<LPCSTR>((LPBYTE)m_spbyBuffer);
	}

	return (dwRead == (m_dwBufferSize - 1));
}

/////////////////////////////////////////////////////////////////////////////
// Read all data. Use sequental calls of ReadData inside.
void CEgHttpProvider::ReadAllData(CEgString& rstrData)
{
#ifdef _DEBUG
	if (m_dwThreadID != GetCurrentThreadId())
		ATLASSERT (!EGHTTP_E_USED_IN_ANOTHER_THREAD);
#endif
	rstrData.Empty();

	CEgString strPiece;
	bool bDataPresent = false;
	do 
	{
		bDataPresent = ReadData(strPiece);
		rstrData += strPiece;
	}
	while (bDataPresent);
}

/////////////////////////////////////////////////////////////////////////////
// Close url
void CEgHttpProvider::CloseURL()
{
	if (m_hFile != NULL)
	{
		::InternetCloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Close http sesstion and url
void CEgHttpProvider::CloseSession()
{
#ifdef _DEBUG
	m_dwThreadID = 0;
#endif

	CloseURL();

	if (m_hSession != NULL)
	{
		::InternetCloseHandle(m_hSession);
		m_hSession = NULL;
	}
	m_spbyBuffer.Free();
	m_dwBufferSize = 0;
}


