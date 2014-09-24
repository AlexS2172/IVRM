/*

ComErrorWrapper class

*/

#include <stdio.h>
#include <comdef.h>
#include <crtdbg.h>

#ifdef NEWCATCH
	class TYPE_CATCH_ALL_TYPE{};
	#define CATCH_ALL_TYPE TYPE_CATCH_ALL_TYPE
#else
	#define CATCH_ALL_TYPE ...
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

/////////////////////////////////////////////////////////////////////////////
//
class CComErrorWrapper
{
private:
	// Throws _com_error exception
	static IErrorInfoPtr _SetError(HRESULT hr,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		ICreateErrorInfoPtr spCEI;
		IErrorInfoPtr spEI;

		WCHAR szSource[1024] = { 0 };
		_snwprintf(szSource, sizeof(szSource) / sizeof(szSource[0]), L"%s (%S(%d)): ",
							pszSource ? pszSource : L"<Unknown source>",
							pszFile ? pszFile : "<Unknown file>", nLine);

		// Try to get description from previous error (if any)
		_bstr_t strFullDescr;
		if(S_OK == ::GetErrorInfo(NULL, &spEI))
		{
			BSTR strPrevDescr = NULL, strSource = NULL;

			spEI->GetDescription(&strPrevDescr);
			spEI->GetSource(&strSource);

			strFullDescr = strSource;
			strFullDescr += strPrevDescr;

			::SysFreeString(strPrevDescr);
			::SysFreeString(strSource);
		}
		else
		{
			LPWSTR pszMsgBuf = NULL;

			::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
							 NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							 (LPWSTR)&pszMsgBuf, 0, NULL);

			strFullDescr = pszMsgBuf ? pszMsgBuf : L"<No previous description>";
			
			::LocalFree(pszMsgBuf);
		}

		strFullDescr = L"\n" + strFullDescr;

		if(pszDescription && wcslen(pszDescription) > 0)
			strFullDescr = pszDescription + strFullDescr;
		else
			strFullDescr = L"<No previous description>" + strFullDescr;

		if(SUCCEEDED(::CreateErrorInfo(&spCEI)))
		{
			if(wcslen(szSource) > 0)
				spCEI->SetSource(_bstr_t(szSource));

			spCEI->SetDescription(strFullDescr);

			spEI = spCEI;
			_ASSERT(spEI.GetInterfacePtr());
			
			::SetErrorInfo(0, spEI);
		}

		return spEI;
	}

	static IErrorInfoPtr _SetError(const _com_error& e,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		ICreateErrorInfoPtr spCEI;
		IErrorInfoPtr spEI;

		WCHAR szSource[1024] = { 0 };
		_snwprintf(szSource, sizeof(szSource) / sizeof(szSource[0]), L"%s (%S(%d)): ",
							pszSource ? pszSource : L"<Unknown source>",
							pszFile ? pszFile : "<Unknown file>", nLine);

		// Try to get description from previous error (if any)
		_bstr_t strFullDescr;
		if(pszDescription && wcslen(pszDescription) > 0)
			strFullDescr = pszDescription;
		else
			strFullDescr = L"<No description>";

		strFullDescr += L"\n";
		strFullDescr += ((LPCWSTR)e.Source()) ? (LPCWSTR)e.Source() : L"<No previuos source>: ";
		strFullDescr += ((LPCWSTR)e.Description()) ? (LPCWSTR)e.Description()
							: (e.ErrorMessage() ? (LPCWSTR)_bstr_t(e.ErrorMessage())
								: L"<No previous description>");

		if(SUCCEEDED(::CreateErrorInfo(&spCEI)))
		{
			if(wcslen(szSource) > 0)
				spCEI->SetSource(_bstr_t(szSource));

			spCEI->SetDescription(strFullDescr);

			spEI = spCEI;
			_ASSERT(spEI.GetInterfacePtr());
			
			::SetErrorInfo(0, spEI);
		}

		return spEI;
	}

public:
	static HRESULT SetError(HRESULT hr,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		_SetError(hr, pszSource, pszDescription, pszFile, nLine);
		return hr;
	}

	static HRESULT SetError(const _com_error& e,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		_SetError(e, pszSource, pszDescription, pszFile, nLine);
		return e.Error();
	}

	static IErrorInfoPtr SetErrorEx(HRESULT hr,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		return _SetError(hr, pszSource, pszDescription, pszFile, nLine);
	}

	static IErrorInfoPtr SetErrorEx(const _com_error& e,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		return _SetError(e, pszSource, pszDescription, pszFile, nLine);
	}

	// Throws _com_error exception
	static void ThrowError(HRESULT hr,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		IErrorInfoPtr spEI(_SetError(hr, pszSource, pszDescription, pszFile, nLine));

		if(spEI.GetInterfacePtr())
			throw _com_error(hr, spEI, true);
		else
			throw _com_error(hr);
	}

	// Throws _com_error exception
	static void ThrowError(const _com_error& e,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, int nLine)
	{
		IErrorInfoPtr spEI(_SetError(e, pszSource, pszDescription, pszFile, nLine));

		if(spEI.GetInterfacePtr())
			throw _com_error(e.Error(), spEI, true);
		else
			throw _com_error(e.Error());
	}

	// Extracts the most apropriate error description
	static _bstr_t ErrorDescription(const _com_error& e)
	{
		WCHAR szError[32] = { 0 };
		_snwprintf(szError, sizeof(szError) / sizeof(szError[0]), L"Error 0x%X:\n", e.Error());

		_bstr_t strFullDescr = szError;
		strFullDescr += ((LPCWSTR)e.Source()) ? (LPCWSTR)e.Source() : L"<No source>: ";
		strFullDescr += ((LPCWSTR)e.Description()) ? (LPCWSTR)e.Description()
							: (e.ErrorMessage() ? (LPCWSTR)_bstr_t(e.ErrorMessage())
								: L"<No description>");
		return strFullDescr;
	}
};

/////////////////////////////////////////////////////////////////////////////
// check result and throw exception if error
__inline void __ErrCheck(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, int nLine)
{
	if(FAILED(hr))
		CComErrorWrapper::ThrowError(hr, pszSource, pszDesc, pszFile, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// throw exception
__inline void __ErrThrow(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, int nLine)
{
	CComErrorWrapper::ThrowError(hr, pszSource, pszDesc, pszFile, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// throw exception
__inline void __ErrThrow(const _com_error& e, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, int nLine)
{
	CComErrorWrapper::ThrowError(e, pszSource, pszDesc, pszFile, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// set error info and return error code
__inline HRESULT __ErrSet(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, int nLine)
{
	return CComErrorWrapper::SetError(hr, pszSource, pszDesc, pszFile, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// set error info and return error code
__inline HRESULT __ErrSet(const _com_error& e, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, int nLine)
{
	return CComErrorWrapper::SetError(e, pszSource, pszDesc, pszFile, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// set error info and return error info object
__inline IErrorInfoPtr __ErrSetEx(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, int nLine)
{
	return CComErrorWrapper::SetErrorEx(hr, pszSource, pszDesc, pszFile, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// set error info and return error info object
__inline IErrorInfoPtr __ErrSetEx(const _com_error& e, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, int nLine)
{
	return CComErrorWrapper::SetErrorEx(e, pszSource, pszDesc, pszFile, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// get error description
static _bstr_t __ErrDesc(const _com_error& e)
{
	return CComErrorWrapper::ErrorDescription(e);
}

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRCHK(error, pszSource, pszDesc)	\
	__ErrCheck(error, pszSource, pszDesc, __FILE__, __LINE__)

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRTRW(error, pszSource, pszDesc)	\
	__ErrThrow(error, pszSource, pszDesc, __FILE__, __LINE__)

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRSET(error, pszSource, pszDesc)	\
	__ErrSet(error, pszSource, pszDesc, __FILE__, __LINE__)

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRSETEX(error, pszSource, pszDesc)	\
	__ErrSetEx(error, pszSource, pszDesc, __FILE__, __LINE__)

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRDESC(error)	\
	__ErrDesc(error)

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRDESCW(error)	\
	((LPCWSTR)__ErrDesc(error))

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRTRACE(error)	\
	{	\
		::OutputDebugStringW((LPCWSTR)__ErrDesc(error));	\
		::OutputDebugStringW(L"\n");	\
	}

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRCHKNULL(expression, pszSource, pszDesc)	\
	{	\
		if(NULL == expression)	\
			__ErrThrow(E_FAIL, pszSource, pszDesc, __FILE__, __LINE__);	\
	}

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRCHKEX(expression, pszSource, pszDesc)	\
	{	\
		try	\
		{	\
			expression;	\
		}	\
		catch(const _com_error& e)	\
		{	\
			__ErrThrow(e, pszSource, pszDesc, __FILE__, __LINE__);	\
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
			__ErrThrow(E_UNEXPECTED, pszSource, pszDesc, __FILE__, __LINE__);	\
		}	\
	}

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRCHKNULLEX(expression, expressionNull, pszSource, pszDesc)	\
	{	\
		try	\
		{	\
			expression;	\
		}	\
		catch(const _com_error& e)	\
		{	\
			__ErrThrow(e, pszSource, pszDesc, __FILE__, __LINE__);	\
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
			__ErrThrow(E_UNEXPECTED, pszSource, pszDesc, __FILE__, __LINE__);	\
		}	\
		if(NULL == expressionNull)	\
			__ErrThrow(E_FAIL, pszSource, pszDesc, __FILE__, __LINE__);	\
	}

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRIGNORE(expression)	\
	{	\
		try	\
		{	\
			expression;	\
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
		}	\
	}
