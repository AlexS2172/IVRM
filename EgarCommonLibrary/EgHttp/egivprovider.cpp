#include "stdafx.h"
#include "resource.h"
#include "EgIvProvider.h"
#include "EgHttpError.h"


/////////////////////////////////////////////////////////////////////////////
// session must be already opened!
CEgString CEgIvProvider::Login()
{
#ifdef _DEBUG
	if (m_dwThreadID != GetCurrentThreadId())
		ATLASSERT (!EGHTTP_E_USED_IN_ANOTHER_THREAD);
#endif

	if (m_accIvLogin.m_strLogin.IsEmpty())
	{
		CEgString strMsg;
		strMsg.Format(IDS_EGHTTP_E_IV_CANNOT_LOGIN, m_accIvLogin.m_strLogin);
		CEgHttpError::ThrowError(EGHTTP_E_IV_CANNOT_LOGIN, strMsg, false);
	}
	
	// creating query string
	CEgString strQuery = _T("data_load/auth.asp?name=") + m_accIvLogin.m_strLogin;

	CEgString strResult;
	// getting ID from IVolatility.com
	OpenURL(strQuery);
	ReadAllData(strResult);
	CloseURL();

	if (strResult.IsEmpty() || strResult.GetLength() < 6 || _istxdigit(*strResult) == 0)
	{
		CEgString strMsg;
		strMsg.Format(IDS_EGHTTP_E_IV_CANNOT_LOGIN, m_accIvLogin.m_strLogin);
		CEgHttpError::ThrowError(EGHTTP_E_IV_CANNOT_LOGIN, strMsg, false);
	}
	
	IC::IMD5Ptr spIMD5;
	HRESULT hr = spIMD5.CreateInstance (__uuidof(IC::MD5));
	if (FAILED (hr))
	{
		_com_error error(hr);
		CEgHttpError::ThrowError(EGHTTP_E_IV_MD5, error);
	}

	CString strSID;
	try
	{
		CString strInput = strResult + m_accIvLogin.m_strPassword;
		_bstr_t bstrInput(strInput.AllocSysString(), false); // for auto free
		strSID = (BSTR)spIMD5->GetMDString(&bstrInput.GetBSTR());
	}
	catch (_com_error& e)
	{
		CEgHttpError::ThrowError(EGHTTP_E_UNEXPECTED, e);
	}

	return strSID;	    
}

/////////////////////////////////////////////////////////////////////////////
//
void CEgIvProvider::CheckErrorMessage(CEgString strData)
{
	LPCTSTR ptszFound = _tcsistr(strData, _T("Server too busy") );
	if (ptszFound)
		CEgHttpError::ThrowError(EGHTTP_E_IV_BUSY);

	ptszFound = _tcsistr(strData, _T("error"));
	if (!ptszFound)
		return;

	ptszFound = _tcsistr(strData, _T("The following error was encountered:"));
	if (ptszFound)
		CEgHttpError::ThrowError(EGHTTP_E_IV_UNKNOWN);

	ptszFound = _tcsistr(strData, _T("Access denied") );
	if (ptszFound)
		CEgHttpError::ThrowError(EGHTTP_E_IV_ACCESS_DENIED);

	LPCTSTR ptszTimeout = _tcsistr(strData, _T("Timeout expired"));
	LPCTSTR ptszODBC = _tcsistr(strData, _T("Microsoft OLE DB Provider"));

	if (ptszTimeout)
	{
		CEgHttpError::ThrowError((ptszODBC != NULL) ? EGHTTP_E_IV_OLEDB_TIMEOUT : 
		                         EGHTTP_E_IV_TIMEOUT);
	}
	else
	{
		CEgHttpError::ThrowError((ptszODBC != NULL) ? EGHTTP_E_IV_OLEDB : 
		                         EGHTTP_E_IV_UNKNOWN);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Additional check of data for IV errors
void CEgIvProvider::ReadAllData(CEgString& rstrData)
{
#ifdef _DEBUG
	if (m_dwThreadID != GetCurrentThreadId())
		ATLASSERT (!EGHTTP_E_USED_IN_ANOTHER_THREAD);
#endif

	CEgHttpProvider::ReadAllData(rstrData);
	CheckErrorMessage(rstrData);
}

/////////////////////////////////////////////////////////////////////////////
// Initialize http session for IVolatility.com server.
void CEgIvProvider::InitIvSession(const CEgHttpConnectionParam& crcpParams, const CEgAccount& crIvLogin)
{
	CEgHttpProvider::InitSession(crcpParams);
	m_accIvLogin = crIvLogin;
}


