#include "stdafx.h"
#include "resource.h"
#include "EgHttpError.h"


/////////////////////////////////////////////////////////////////////////////
// Get error from WinInet.dll only
CEgString CEgHttpError::GetInetErrorMessage(DWORD dwError)
{
	CEgString strError;

	// this implementation from CInternetException::GetErrorMessage
	LPTSTR lpBuffer;
	bool bRet = true;

	HINSTANCE hWinINetLibrary;
	hWinINetLibrary = ::LoadLibraryA("WININET.DLL");

	if (hWinINetLibrary == NULL ||
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
			hWinINetLibrary, dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL) == 0)
	{
		// it failed! try Windows...

		bRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,  dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL) != 0;
	}

	if (!bRet)
		strError.Empty();
	else
	{
		if (dwError == ERROR_INTERNET_EXTENDED_ERROR)
		{
			LPTSTR lpExtended;
			DWORD dwLength = 0;
			DWORD dwError;

			// find the length of the error
			if (!InternetGetLastResponseInfo(&dwError, NULL, &dwLength) &&
				GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				lpExtended = (LPTSTR) LocalAlloc(LPTR, dwLength);
				InternetGetLastResponseInfo(&dwError, lpExtended, &dwLength);
				strError = lpExtended;
				LocalFree(lpExtended);
			}
			else
				ATLTRACE(_T("Warning: Extended error reported with no response info\n"));

			bRet = true;
		}
		else
		{
			strError = lpBuffer;
			bRet = true;
		}

		LocalFree(lpBuffer);
	}

	::FreeLibrary(hWinINetLibrary);
	
	strError.TrimRight();
	return strError;
}

/////////////////////////////////////////////////////////////////////////////
// Get last WinInet error description and attach it to additional description. 
// Then throw exception.
void CEgHttpError::ThrowHttpError(CEgString strDescription)
{
	DWORD dwError = ::GetLastError();
	if (strDescription.IsEmpty())
		throw CEgHttpError(dwError, GetInetErrorMessage(dwError));
	else
	{
		strDescription.AppendFormat(_T(" (%s)"), GetInetErrorMessage(dwError));
		throw CEgHttpError(dwError, strDescription);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Throw exception with given code. Get description from resources by error 
// code and attach custom error description. if bLoadDescription is false then
// don't load description correspond to dwError
void CEgHttpError::ThrowError(DWORD dwError, CEgString strDescription, 
							  bool bLoadDescription)
{
	if (!bLoadDescription)
	{
		ATLASSERT (!strDescription.IsEmpty());
		throw CEgHttpError(dwError, strDescription);
	}
	else
	{
		CEgString strMessage;
		strMessage.LoadString(dwError - EGHTTP_E_BASE);

		if (strDescription.IsEmpty())
			throw CEgHttpError(dwError, strMessage);
		else
		{
			strDescription.AppendFormat(_T(" (%s)"), strMessage);
			throw CEgHttpError(dwError, strDescription);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Throw exception with given code. Get description from resources by error 
// code and attach error description from _com_error patameter.
void CEgHttpError::ThrowError(DWORD dwError, _com_error& error)
{
	CEgString strDescription;
	strDescription.LoadString(dwError - EGHTTP_E_BASE);
	strDescription.AppendFormat(_T(" (%s)"), error.ErrorMessage());
	throw CEgHttpError(dwError, strDescription);
}
