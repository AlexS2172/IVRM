// IVConnection.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "IVConnection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This function performs case-insensitive search that makes unnecessary case conversions

TCHAR * _tcsstr_nocase ( const TCHAR * str1, const TCHAR * str2 )
{
    TCHAR *cp = (TCHAR *) str1;
    TCHAR *s1, *s2;

    if ( !*str2 )
        return((TCHAR *)str1);

    while (*cp)
    {
            s1 = cp;
            s2 = (TCHAR *) str2;

            while ( *s1 && *s2 )
			{
                TCHAR szCmp [3] = { *s1, *s2, _T('\0') };
				_tcslwr_s (szCmp, 3);

				if ( szCmp[0]-szCmp[1] ) 
					break;

				s1++, s2++;
			}	
            if (!*s2)
                    return(cp);

            cp++;
    }

    return(NULL);

}

/////////////////////////////////////////////////////////////////////////////
// CIVConnection

//#define WM_IVC_GET_HIVOLA	WM_USER+1

#define WM_POST_REQUEST	WM_USER+1



CIVConnection::CIVConnection(): 
	m_hSession (NULL),
	m_evtBusy (FALSE, TRUE ),
	m_evtReady (TRUE, TRUE),
	m_bStopped (false),
	m_bError (false)
{
	m_bAutoDelete = false;
}

CIVConnection::~CIVConnection()
{
}


BOOL CIVConnection::InitInstance()
{
	CoInitialize (NULL);

	return TRUE;
}

int CIVConnection::ExitInstance()
{
	m_spIMD5 = NULL;

	CoUninitialize ();

	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CIVConnection, CWinThread)
	//{{AFX_MSG_MAP(CIVConnection)
	ON_THREAD_MESSAGE (WM_POST_REQUEST, OnPostRequest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIVConnection message handlers

void CIVConnection::SetParams(const CIVOptions & opts)
{
	StopRequests();
	
	m_opts = opts;
}


void CIVConnection::OnPostRequest	(WPARAM wParam, LPARAM lParam)
{
	ASSERT (!m_strRequest.IsEmpty());
	try
	{
		InitSession();

		CString strSID = Login();

		CString strRequest;
		strRequest.Format (m_strRequest, (LPCTSTR) strSID );

		GetHTTPData (strRequest, m_strResult);
	}
	catch (CException * e )
	{
		e->Delete();
	}

	CloseSession();
	SetReady (true);	
}

bool CIVConnection::GetHIVola (LPCTSTR szSymbol, const dte_vec & dtes, hist_ranges_map & ranges)
{
	InitRequest();
	
	ASSERT (szSymbol);
	
	CString strURL;
	if(-1 == m_opts.m_strURL.Find(_T("http")))
	{
		strURL  = _T("http://");
		strURL += m_opts.m_strURL;
	}
	else
		strURL = m_opts.m_strURL;

	
	m_strRequest = strURL + _T("/ivanalyzer.asp?sid=%s&si=1&S=");
	m_strRequest += szSymbol;

	CString strDates;
	for (dte_vec::const_iterator it = dtes.begin() ; it != dtes.end(); it++ )
	{
		CString strFmt;
		strFmt.Format (_T("&D=%d"), *it );
		strDates += strFmt;
	}
	
	m_strRequest += strDates;

	if (! RequestData() )
		return false;

	ranges.clear();
	DATE dtCurrent = (long) (DATE)  COleDateTime::GetCurrentTime();
	
	LPTSTR szResult = m_strResult.GetBuffer (0);
	
	LPCTSTR szDelimit = _T("\r");
	char* szContext = NULL;
	LPTSTR szToken = _tcstok_s (szResult, szDelimit, &szContext);
	while (szToken)
	{
		LPCTSTR szKey = _T("HiVola");
		LPCTSTR szStart =  _tcsstr_nocase (szToken, szKey);

		if (szStart)
		{
			szStart += _tcslen (szKey);
			
			CHistRangeData hrData;
			long nDte = 0;
#ifndef _UNICODE
			char chSep;
			std::istringstream is;	
#else
			wchar_t chSep;		
			std::wistringstream is;
#endif
			is.str ( szStart);
			is 
				>> nDte >> chSep 
				>> hrData.m_dStrike	>> chSep  
				>> hrData.m_dHistVolaHigh >> chSep
				>> hrData.m_dHistVolaLow >> chSep
				>> hrData.m_bATM;


			long dtExp = static_cast<long>(dtCurrent) + nDte;
			ranges.insert ( std::make_pair (dtExp, hrData)  );
		}
		szToken = _tcstok_s (NULL, szDelimit, &szContext);

	}

	m_strResult.ReleaseBuffer();

	return true;
}

bool CIVConnection::GetYield (LPCTSTR szIndex, double & rdYield )
{
	InitRequest();
	
	ASSERT (szIndex);
	CString strURL;
	if(-1 == m_opts.m_strURL.Find(_T("http")))
	{
		strURL  = _T("http://");
		strURL += m_opts.m_strURL;
	}
	else
		strURL = m_opts.m_strURL;


	m_strRequest = strURL + _T("/ivanalyzer.asp?sid=%s&si=2&idx=");
	m_strRequest += szIndex;

	if (! RequestData() )
		return false;
	
	rdYield = 0;

	LPCTSTR szKey = _T("Yield");
	LPCTSTR szToken = _tcsstr_nocase (m_strResult, szKey);
	if (szToken)
	{
		szToken += _tcslen (szKey) ;
		
#ifndef _UNICODE
		std::istringstream is;	
#else
		std::wistringstream is;
#endif
		is.str ( szToken);
		is >> rdYield;
	}

	return true;
}


void CIVConnection::InitSession()
{
	CString str = (LPCSTR) IDS_INTERNET_AGENT;
	
	DWORD dwError = 0;
	bool bStopped = false;
	m_csSession.Lock();
	

	DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
	CString strProxy;

	switch (m_opts.m_nUseProxy)
	{
	case 0:
		dwAccessType = INTERNET_OPEN_TYPE_DIRECT;
		break;
	case 1:
		dwAccessType = INTERNET_OPEN_TYPE_PROXY;
		strProxy.Format (_T("http=%s:%d"), (LPCTSTR) m_opts.m_strProxy, m_opts.m_nPort );
		break;
	}

	if (! (bStopped = m_bStopped) )
	{
		m_hSession = ::InternetOpen (str, dwAccessType, strProxy, NULL, 0);
		if (! m_hSession)
			dwError = GetLastError();
	}

	m_csSession.Unlock();

	if (bStopped)
		AfxThrowUserException();
		
	if (dwError)
		AfxThrowInternetException (0, dwError);
}

CString CIVConnection::Login()
{
	
	CString strQuery;
	CString strURL;
	if(-1 == m_opts.m_strURL.Find(_T("http")))
	{
		strURL  = _T("http://");
		strURL += m_opts.m_strURL;
	}
	else
		strURL = m_opts.m_strURL;

	strQuery.Format (_T("%s/auth.asp?name=%s"), (LPCTSTR) strURL, (LPCTSTR) m_opts.m_strAccount);
	
	CString strResult;
	if(!m_opts.m_strAccount.GetLength())
	{
		CString strError;
		AfxFormatString1 (strError, ERR_IDS_IV_CANNOT_LOGIN, _T(""));
		SetError (strError);
		
		AfxThrowUserException();

	}

	GetHTTPData(strQuery, strResult);

	if (strResult.IsEmpty() || strResult.GetLength()<6 || *(LPCSTR)strResult < '0' || *(LPCSTR)strResult >'9')
	{
		CString strError;
		AfxFormatString1 (strError, ERR_IDS_IV_CANNOT_LOGIN, m_opts.m_strAccount);
		SetError (strError);
		
		AfxThrowUserException();
	}
	
	if (m_spIMD5 == NULL)
	{
		HRESULT hr = m_spIMD5.CreateInstance (__uuidof(IC::MD5));
		if (FAILED (hr))
		{
			SetError (ERR_IDS_IV_MD5);
			AfxThrowUserException();
		}
	}		
	

	CString strSID;
	try
	{
		CString strInput  = strResult;
		strInput += m_opts.m_strPassword;

		BSTR bstrInput = strInput.AllocSysString();
		_bstr_t bstrTemp (bstrInput, false);

		strSID = (BSTR)  m_spIMD5->GetMDString (&bstrInput);
	}
	catch (_com_error & )
	{
		CString strErr ( (LPCSTR) ERR_IDS_UNEXPECTED   ) ;
		AfxThrowUserException();
	}


	return strSID;
}

void CIVConnection::GetHTTPData(LPCTSTR szQuery, CString &rstr)
{
	TRACE (_T("HTTP Request: %s\n"), szQuery);

	DWORD dwServiceType = 0;
	CString strServer;
	CString strObject;		
	INTERNET_PORT nPort=0;
	bool    bFirst = false;
	
	HINTERNET hFile = NULL;

	bool bFailed = false;
	
	try
	{
		while(1)
		{
			if(!m_strHeader.GetLength())
				hFile = ::InternetOpenUrl (m_hSession, szQuery, NULL, 0,  INTERNET_FLAG_DONT_CACHE, 0 );
			else
				hFile = ::InternetOpenUrl (m_hSession, szQuery, (LPCSTR)m_strHeader, m_strHeader.GetLength(),  INTERNET_FLAG_DONT_CACHE, 0 );

			if (! hFile)
				AfxThrowInternetException(0);

			rstr.Empty();

			const int nSizeBuffer = 1024;
			BYTE pBuffer [nSizeBuffer];
			DWORD dwTotalRead = 0;

			while (true)
			{
				DWORD dwRead = 0;
				BOOL bResult = ::InternetReadFile (hFile, pBuffer, nSizeBuffer, &dwRead);
				if (! bResult)
					AfxThrowInternetException(0);
				
				if (dwRead ==0)
					break;

				DWORD dwOldTotal = dwTotalRead;
				dwTotalRead += dwRead;		

				LPTSTR szTarget = rstr.GetBuffer(dwTotalRead);
				szTarget += dwOldTotal;
	#ifndef _UNICODE				
				memcpy (szTarget, pBuffer, dwRead);
	#else
				MultiByteToWideChar (CP_ACP, 0, (LPCSTR) pBuffer, dwRead, szTarget, dwRead);			
	#endif //_UNICODE
				rstr.ReleaseBuffer(dwTotalRead);
			}

			if ( CheckErrorMessage(rstr) ) 
				bFailed = true;
			else
			{
				DWORD dwStatus = QueryInfoStatusCode(hFile);
				if(HTTP_STATUS_PROXY_AUTH_REQ == dwStatus && !bFirst && m_opts.m_iProxyAuth)
				{
					bFirst = true;

					CString strIn;
					CString strOut;
					unsigned nLen = 0;
					
					strIn.Format(_T("%s:%s"), (LPCTSTR)m_opts.m_strProxyLogin, (LPCTSTR)m_opts.m_strProxyPassword);
					encode64((LPCSTR)strIn, strIn.GetLength(), strOut.GetBufferSetLength(200), 190, &nLen);
					strOut.ReleaseBuffer();
					m_strHeader.Format(_T("Proxy-Authorization: Basic %s\r\n"),(LPCTSTR)strOut);
					InternetCloseHandle(hFile);
					continue;

				}
				if(!m_opts.m_iProxyAuth)
					m_strHeader = _T("");

				if ( HttpError (hFile, dwStatus) )
					bFailed = true;
			}
			break;
		}
	}
	catch (CInternetException * e )
	{
		bFailed = true;
		
		if ( e->m_dwError  != ERROR_INTERNET_OPERATION_CANCELLED  &&
			e->m_dwError  != ERROR_INVALID_HANDLE )
		{
			TCHAR szError [512] = _T("") ;
			e->GetErrorMessage(szError, sizeof (szError) / sizeof (TCHAR) );
			SetError (GetCommonIVError(szError));
		}
		else
			TRACE (_T("CIVConnection - canceled\n"));

		e->Delete();
	}

	TRACE (_T("End of HTTP request\n"));

	if (hFile)
		::InternetCloseHandle (hFile);

	if (bFailed)
		AfxThrowUserException();
}

void CIVConnection::CloseSession()
{
	m_csSession.Lock();

	if (m_hSession)
	{
		::InternetCloseHandle(m_hSession);
		m_hSession = NULL;
	}

	m_csSession.Unlock();
}

bool CIVConnection::HttpError(HINTERNET hFile, DWORD dwStatus)
{
	if (dwStatus == HTTP_STATUS_OK)
		return false;
	
	CString strError;
	CString strStatus;
	QueryStatusText (hFile, strStatus);

	switch (dwStatus)
	{
	case HTTP_STATUS_SERVER_ERROR:
		strError.LoadString (ERR_IDS_HTTP_INTERNAL);
		break;
	case HTTP_STATUS_NOT_FOUND:
		strError.LoadString (ERR_IDS_HTTP_NOT_FOUND);
		break;
	default:
		strError.Format (ERR_IDS_HTTP_ERROR, dwStatus, (LPCTSTR) strStatus);
	}

	SetError (  GetCommonIVError(strError) );

	return true;
}

bool CIVConnection::CheckErrorMessage(LPCTSTR szSource)
{
	LPCTSTR szFound = _tcsstr_nocase(szSource, _T("<font") );
	if(! szFound)
		return false;

	szFound = _tcsstr_nocase(szSource, _T("error") );
	if(! szFound)
		return false;

	bool bError = true;
	szFound = _tcsstr_nocase(szSource, _T("Access denied") );
	
	UINT nError =0;
	if(szFound)
		nError =  ERR_IDS_IV_ACCESS_DENIED;
	else
	{
		LPCTSTR szTimeout = _tcsstr_nocase(szSource, _T("Timeout expired"));
		LPCTSTR szODBC = _tcsstr_nocase(szSource, _T("Microsoft OLE DB Provider"));

		if(szTimeout)
		{
			if(szODBC)
				nError = ERR_IDS_IV_OLEDB_TIMEOUT;
			else
				nError = ERR_IDS_IV_TIMEOUT;

		}
		else
		if(szODBC)
			nError = ERR_IDS_IV_OLEDB;
		else
			nError = ERR_IDS_IV_ERROR;
	}
	

	if (bError)
	{
		SetError (nError);
		return true;
	}
	else
		return false;

}

CString CIVConnection::GetCommonIVError (LPCTSTR szError)
{
	CString strError;

	CString strPrefix = (LPCSTR) ERR_IDS_IV_COMMON_PREFIX;
	strError.Format (_T("%s %s"), (LPCTSTR) strPrefix, szError);
	
	return strError;
}


void CIVConnection::SetError(LPCTSTR szError)
{
	m_bError = true;
	m_strError = szError;
}

void CIVConnection::SetError(UINT nID)
{
	CString strError;
	strError.LoadString (nID);
	SetError (strError);
}


void CIVConnection::StopRequests()
{
	m_csSession.Lock();

	m_bStopped = true;

	if (m_hSession)
		::InternetCloseHandle(m_hSession);

	m_csSession.Unlock();
	m_evtReady.Lock();
}

void CIVConnection::InitRequest()
{
	StopRequests();
	m_bStopped = false;
	
	m_strRequest.Empty();
	m_strResult.Empty();
	m_strError.Empty();

	m_bError = false;
}

void CIVConnection::SetReady(bool bReady)
{
	if (bReady)
		m_evtReady.SetEvent();
	else
		m_evtReady.ResetEvent();
}

DWORD CIVConnection::QueryInfoStatusCode(HINTERNET hFile)
{
	DWORD  dwStatusCode = HTTP_STATUS_OK;

	ASSERT(hFile);

	TCHAR szBuffer[80];
	DWORD dwLen = sizeof (szBuffer);
	BOOL bRet = HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE,
				szBuffer, &dwLen, NULL);

	if (bRet)
		dwStatusCode = (DWORD) _ttol(szBuffer);
	else
		AfxThrowInternetException(0);

	return dwStatusCode;
}

void CIVConnection::QueryStatusText(HINTERNET hFile, CString & rstr)
{
	rstr.Empty();
	
	DWORD dwLen = 0;
	::HttpQueryInfo(hFile, HTTP_QUERY_STATUS_TEXT, NULL, &dwLen, 0);
		
	if (dwLen)	
	{
		DWORD dwChars = dwLen / sizeof (TCHAR);

		LPTSTR pstr = rstr.GetBufferSetLength(dwChars);
		BOOL bRet = HttpQueryInfo(hFile, HTTP_QUERY_STATUS_TEXT, pstr, &dwLen, NULL);
		if (bRet)
			rstr.ReleaseBuffer(dwChars);
		else
			rstr.ReleaseBuffer(0);
	}

	return;
}

bool CIVConnection::RequestData()
{
	SetReady (false);	
	PostThreadMessage (WM_POST_REQUEST, 0, 0);

	HANDLE hEvtReady = m_evtReady;
	
	while ( MsgWaitForMultipleObjects (1, & hEvtReady, FALSE, INFINITE, QS_ALLEVENTS) != WAIT_OBJECT_0 )
	{
		MSG msg;
		while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
			AfxGetThread()->PumpMessage();
	}

	if (m_bStopped  ||  m_bError )
		return false;
	else
		return true;
}
