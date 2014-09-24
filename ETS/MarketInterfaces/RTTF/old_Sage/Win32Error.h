#ifndef _WIN32ERROR_H
#define _WIN32ERROR_H

class CWin32Error
{
	TCHAR * m_lpMsgBuf;

public:

	CWin32Error(DWORD dwErr)
	{
		m_lpMsgBuf = NULL;

		FormatError(dwErr);
	};

	CWin32Error()
	{
		m_lpMsgBuf = NULL;

		FormatError(::GetLastError());
	};

	void Clear()
	{
		if (m_lpMsgBuf)
			LocalFree( m_lpMsgBuf );

		m_lpMsgBuf = NULL;
	}

	TCHAR * GetLastError(DWORD dwErr)
	{
		Clear();
		FormatError(dwErr);
		return m_lpMsgBuf;
	}

	TCHAR * GetLastError()
	{
		Clear();
		FormatError(::GetLastError());
		return m_lpMsgBuf;
	}


	void FormatError(DWORD dwErr)
	{

			FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &m_lpMsgBuf,
			0,
			NULL );

			if (m_lpMsgBuf == NULL)
			{
				m_lpMsgBuf = (TCHAR*)LocalAlloc(LMEM_FIXED, 16*sizeof(TCHAR));
				if (m_lpMsgBuf)
				{
					_stprintf(m_lpMsgBuf, _T("0x%x"), dwErr);
				}
			}
	};

	operator const TCHAR *() const
	{
		return m_lpMsgBuf;
	}


	~CWin32Error()
	{
		Clear();
	}
};


#endif _WIN32ERROR_H