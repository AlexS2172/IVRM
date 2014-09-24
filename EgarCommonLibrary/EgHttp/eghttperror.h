#ifndef __EGHTTP_ERROR_H__
#define __EGHTTP_ERROR_H__

#include <comdef.h> // for _com_error
#include "EgHttpCommon.h"


/////////////////////////////////////////////////////////////////////////////
// EgHttp custom errors
#define EGHTTP_E_BASE                    INTERNET_ERROR_LAST

#define EGHTTP_E_ALREADY_OPENED_SESSION  (EGHTTP_E_BASE + IDS_EGHTTP_E_ALREADY_OPENED_SESSION)
#define EGHTTP_E_NOT_OPENED_SESSION      (EGHTTP_E_BASE + IDS_EGHTTP_E_NOT_OPENED_SESSION)
#define EGHTTP_E_ALREADY_OPENED_URL      (EGHTTP_E_BASE + IDS_EGHTTP_E_ALREADY_OPENED_URL)
#define EGHTTP_E_NOT_OPENED_URL          (EGHTTP_E_BASE + IDS_EGHTTP_E_NOT_OPENED_URL)
#define EGHTTP_E_IV_ACCESS_DENIED        (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_ACCESS_DENIED)
#define EGHTTP_E_IV_OLEDB_TIMEOUT        (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_OLEDB_TIMEOUT)
#define EGHTTP_E_IV_TIMEOUT              (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_TIMEOUT)
#define EGHTTP_E_IV_OLEDB                (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_OLEDB)
#define EGHTTP_E_IV_UNKNOWN              (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_UNKNOWN)
#define EGHTTP_E_IV_CANNOT_LOGIN         (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_CANNOT_LOGIN)
#define EGHTTP_E_IV_LOGIN_FAILED         (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_LOGIN_FAILED)
#define EGHTTP_E_IV_MD5                  (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_MD5)
#define EGHTTP_E_UNEXPECTED              (EGHTTP_E_BASE + IDS_EGHTTP_E_UNEXPECTED)
#define EGHTTP_E_NOT_OPENED_SESSION      (EGHTTP_E_BASE + IDS_EGHTTP_E_NOT_OPENED_SESSION)
#define EGHTTP_E_IV_BUSY				 (EGHTTP_E_BASE + IDS_EGHTTP_E_IV_BUSY)
#ifdef _DEBUG
#define EGHTTP_E_USED_IN_ANOTHER_THREAD   _T("object is used by another thread!")
#endif

/////////////////////////////////////////////////////////////////////////////
// General http exception.
class EGHTTP_API CEgHttpError
{
public:
	// Get error from WinInet.dll
	static CEgString GetInetErrorMessage(DWORD dwError = ::GetLastError());
	
	// Get last WinInet error description and attach it to additional description. 
	// Then throw exception.
	static void ThrowHttpError(CEgString strDescription = CEgString());

	// Throw exception with given code. Get description from resources by error 
	// code (if bLoadDescription is true) and attach custom error description.
	static void ThrowError(DWORD dwError, CEgString strDescription = CEgString(), bool bLoadDescription = true);

	// Throw exception with given code. Get description from resources by error code and 
	// attach error description from _com_error patameter.
	static void ThrowError(DWORD dwError, _com_error& error);

public:
	
	DWORD Error() const
	{
		return m_dwError;
	}

	CEgString Description() const
	{
		return m_strDescription;
	}

protected:
	DWORD m_dwError;
	CEgString m_strDescription;

	CEgHttpError(DWORD dwError, CEgString strDescription):
		m_dwError(dwError), m_strDescription(strDescription){};
};

#endif // __EGHTTP_ERROR_H__

