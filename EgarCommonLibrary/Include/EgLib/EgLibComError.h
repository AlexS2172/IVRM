/*

ComErrorWrapper class

*/
#pragma once
// Prerequisites
#ifndef  _INC_COMDEF
	#error comdef.h must be included prior ComErrorWrapper.h
#endif //_INC_COMDEF
#ifndef  _INC_TCHAR
	#error tchar.h must be included prior ComErrorWrapper.h
#endif //_INC_TCHAR
#ifndef  _INC_CRTDBG
	#error crtdbg.h must be included prior ComErrorWrapper.h
#endif //_INC_CRTDBG


#ifdef NEWCATCH
	class TYPE_CATCH_ALL_TYPE{};
	#define CATCH_ALL_TYPE TYPE_CATCH_ALL_TYPE
#else
	#define CATCH_ALL_TYPE ...
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__      WIDEN(__FILE__)
#define __WFUNCDNAME__ WIDEN(__FUNCDNAME__)
wchar_t *pwsz = __WFILE__;

#include <stdio.h>
namespace EgLib
{
/////////////////////////////////////////////////////////////////////////////
//
class CComErrorWrapper
{
private:
	// Throws _com_error exception
	static IErrorInfoPtr _SetError(HRESULT hr,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, LPCSTR pszFunc,  int nLine)
	{
		ICreateErrorInfoPtr spCEI;
		IErrorInfoPtr spEI;

		WCHAR szSource[1024] = { 0 };
#if (_MSC_VER >= 1400)
		_snwprintf_s(szSource, sizeof(szSource) / sizeof(szSource[0]), L"%s (%S(%S: %d)): ",
							pszSource ? pszSource : L"<Unknown source>",
							pszFile ? pszFile : "<Unknown file>", pszFunc ? pszFunc : "<Unknown function>", nLine);
#else
		_snwprintf(szSource, sizeof(szSource) / sizeof(szSource[0]), L"%s (%S(%S: %d)): ",
			pszSource ? pszSource : L"<Unknown source>",
			pszFile ? pszFile : "<Unknown file>", pszFunc ? pszFunc : "<Unknown function>", nLine);
#endif
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
							LPCSTR pszFile, LPCSTR pszFunc, int nLine)
	{
		ICreateErrorInfoPtr spCEI;
		IErrorInfoPtr spEI;

		WCHAR szSource[1024] = { 0 };
#if (_MSC_VER >= 1400)
		_snwprintf_s(szSource, sizeof(szSource) / sizeof(szSource[0]), L"%s (%S(%S:%d)): ",
							pszSource ? pszSource : L"<Unknown source>",
							pszFile ? pszFile : "<Unknown file>", pszFunc ? pszFunc : "<Unknown function>", nLine);

#else
		_snwprintf(szSource, sizeof(szSource) / sizeof(szSource[0]), L"%s (%S(%S:%d)): ",
			pszSource ? pszSource : L"<Unknown source>",
			pszFile ? pszFile : "<Unknown file>", pszFunc ? pszFunc : "<Unknown function>", nLine);
#endif
		// Try to get description from previous error (if any)
		_bstr_t strFullDescr;
		if(pszDescription && wcslen(pszDescription) > 0)
			strFullDescr = pszDescription;
		else
			strFullDescr = L"<No description>";

		strFullDescr += L"\n";
		strFullDescr += e.Source().length() ? e.Source() : L"<No previous source>: ";
		strFullDescr += e.Description().length() ? e.Description()
							: (e.ErrorMessage() ? _bstr_t(e.ErrorMessage())	: L"<No previous description>");

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
							LPCSTR pszFile, LPCSTR pszFunc, int nLine)
	{
		_SetError(hr, pszSource, pszDescription, pszFile, pszFunc, nLine);
		return hr;
	}

	static HRESULT SetError(const _com_error& e,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, LPCSTR pszFunc, int nLine)
	{
		_SetError(e, pszSource, pszDescription, pszFile, pszFunc, nLine);
		return e.Error();
	}

	static IErrorInfoPtr SetErrorEx(HRESULT hr,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, LPCSTR pszFunc, int nLine)
	{
		return _SetError(hr, pszSource, pszDescription, pszFile, pszFunc, nLine);
	}

	static IErrorInfoPtr SetErrorEx(const _com_error& e,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, LPCSTR pszFunc, int nLine)
	{
		return _SetError(e, pszSource, pszDescription, pszFile, pszFunc, nLine);
	}

	// Throws _com_error exception
	static void ThrowError(HRESULT hr,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, LPCSTR pszFunc, int nLine)
	{
		IErrorInfoPtr spEI(_SetError(hr, pszSource, pszDescription, pszFile, pszFunc, nLine));

		if(spEI.GetInterfacePtr())
			throw _com_error(hr, spEI, true);
		else
			throw _com_error(hr);
	}

	// Throws _com_error exception
	static void ThrowError(const _com_error& e,
							LPCWSTR pszSource, LPCWSTR pszDescription,
							LPCSTR pszFile, LPCSTR pszFunc, int nLine)
	{
		IErrorInfoPtr spEI(_SetError(e, pszSource, pszDescription, pszFile, pszFunc, nLine));

		if(spEI.GetInterfacePtr())
			throw _com_error(e.Error(), spEI, true);
		else
			throw _com_error(e.Error());
	}

	// Extracts the most appropriate error description
	static _bstr_t ErrorDescription(const _com_error& e)
	{
		WCHAR szError[32] = { 0 };

#if (_MSC_VER >= 1400)
		_snwprintf_s(szError, sizeof(szError) / sizeof(szError[0]), L"Error 0x%X: ", e.Error());
#else
		_snwprintf(szError, sizeof(szError) / sizeof(szError[0]), L"Error 0x%X: ", e.Error());
#endif

		_bstr_t strFullDescr = szError;
		strFullDescr += ((LPCWSTR)e.Source()) ? (LPCWSTR)e.Source() : L"<No source>: ";
		strFullDescr += ((LPCWSTR)e.Description()) ? (LPCWSTR)e.Description()
							: (e.ErrorMessage() ? (LPCWSTR)_bstr_t(e.ErrorMessage())
								: L"<No description>");
		return strFullDescr;
	}

	//////////// Legacy version methods
	// Throws _com_error exception
	static void ThrowError( HRESULT hr, LPCTSTR description = NULL )
		{
		ICreateErrorInfoPtr spCEI;
		IErrorInfoPtr		spEI;

		// Try to get description from previous error (if any)
		_bstr_t bsFullDescr;
		if( S_OK == ::GetErrorInfo( NULL, &spEI ) )
			{
			BSTR PrevDescr, Source;

			spEI->GetDescription( &PrevDescr );
			spEI->GetSource( &Source );

			bsFullDescr += Source;
			bsFullDescr += ": ";
			bsFullDescr += PrevDescr;

			::SysFreeString( PrevDescr );
			::SysFreeString( Source );
			}
		else
			{
			void *pMsgBuf;

			::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
							 NULL, hr, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
							 (LPTSTR)&pMsgBuf, 0, NULL );

			bsFullDescr += (LPTSTR)pMsgBuf;
			
			LocalFree( pMsgBuf );
			}

		if( NULL != description && _tcslen(description) > 0 )
			{
			bsFullDescr = "\n" + bsFullDescr;
			bsFullDescr = description + bsFullDescr;
			}

		if(SUCCEEDED( ::CreateErrorInfo(&spCEI) ))
			{
			spCEI->SetDescription( bsFullDescr );

			spEI = spCEI;
			_ASSERT( spEI.GetInterfacePtr() );
			
			::SetErrorInfo(0, spEI);
			}

		if( spEI.GetInterfacePtr() )
			throw _com_error( hr, spEI, true );
		else
			throw _com_error( hr );
		}

/*
	// Extracts the most apropriate error description
	static _bstr_t ErrorDescription( const _com_error& err )
		{
		_bstr_t descr = err.Description();
		return descr.length() != 0 ? descr : err.ErrorMessage();
		}
*/
};

/////////////////////////////////////////////////////////////////////////////
// check result and throw exception if error
__inline void __ErrCheck(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, LPCSTR pszFunc, int nLine)
{
	if(FAILED(hr))
		CComErrorWrapper::ThrowError(hr, pszSource, pszDesc, pszFile, pszFunc, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// throw exception
__inline void __ErrThrow(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, LPCSTR pszFunc, int nLine)
{
	CComErrorWrapper::ThrowError(hr, pszSource, pszDesc, pszFile, pszFunc, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// throw exception
__inline void __ErrThrow(const _com_error& e, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, LPCSTR pszFunc, int nLine)
{
	CComErrorWrapper::ThrowError(e, pszSource, pszDesc, pszFile, pszFunc, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// set error info and return error code
__inline HRESULT __ErrSet(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile,  LPCSTR pszFunc, int nLine)
{
	return CComErrorWrapper::SetError(hr, pszSource, pszDesc, pszFile, pszFunc, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// set error info and return error code
__inline HRESULT __ErrSet(const _com_error& e, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile,  LPCSTR pszFunc, int nLine)
{
	return CComErrorWrapper::SetError(e, pszSource, pszDesc, pszFile, pszFunc, nLine);
}



/////////////////////////////////////////////////////////////////////////////
// set error info and return error info object
__inline IErrorInfoPtr __ErrSetEx(HRESULT hr, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, LPCSTR pszFunc, int nLine)
{
	return CComErrorWrapper::SetErrorEx(hr, pszSource, pszDesc, pszFile, pszFunc, nLine);
}


/////////////////////////////////////////////////////////////////////////////
// set error info and return error info object
__inline IErrorInfoPtr __ErrSetEx(const _com_error& e, LPCWSTR pszSource, LPCWSTR pszDesc, LPCSTR pszFile, LPCSTR pszFunc, int nLine)
{
	return CComErrorWrapper::SetErrorEx(e, pszSource, pszDesc, pszFile, pszFunc, nLine);
}

/////////////////////////////////////////////////////////////////////////////
// get error description
static _bstr_t __ErrDesc(const _com_error& e)
{
	return CComErrorWrapper::ErrorDescription(e);
}

}; // EgLib

/////////////////////////////////////////////////////////////////////////////
//

#if _MSC_VER > 1300

	/////////////////////////////////////////////////////////////////////////////
	//
	#define _ERRTRW(error, pszSource, pszDesc)	\
		EgLib::__ErrThrow(error, pszSource, pszDesc, __FILE__, __FUNCTION__ , __LINE__ )

	/////////////////////////////////////////////////////////////////////////////
	//
	#define _ERRSET(error, pszSource, pszDesc)	\
		EgLib::__ErrSet(error, pszSource, pszDesc, __FILE__, __FUNCTION__ ,__LINE__)

	/////////////////////////////////////////////////////////////////////////////
	//
	#define _ERRSETEX(error, pszSource, pszDesc)	\
		EgLib::__ErrSetEx(error, pszSource, pszDesc, __FILE__, __FUNCTION__ ,__LINE__)


#else

	/////////////////////////////////////////////////////////////////////////////
	//
	#define _ERRCHK(error, pszSource, pszDesc)	\
		EgLib::__ErrCheck(error, pszSource, pszDesc, __FILE__, "", __LINE__)

	/////////////////////////////////////////////////////////////////////////////
	//
	#define _ERRTRW(error, pszSource, pszDesc)	\
		EgLib::__ErrThrow(error, pszSource, pszDesc, __FILE__, "", __LINE__)

	/////////////////////////////////////////////////////////////////////////////
	//
	#define _ERRSET(error, pszSource, pszDesc)	\
		EgLib::__ErrSet(error, pszSource, pszDesc, __FILE__, "", __LINE__ )

	/////////////////////////////////////////////////////////////////////////////
	//
	#define _ERRSETEX(error, pszSource, pszDesc)	\
		EgLib::__ErrSetEx(error, pszSource, pszDesc, __FILE__, "", __LINE__)


#endif // _MSC_VER > 1300



/////////////////////////////////////////////////////////////////////////////
//
#define _ERRDESC(error)	\
	EgLib::__ErrDesc(error)

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRDESCW(error)	\
	((LPCWSTR)EgLib::__ErrDesc(error))

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRTRACE(error)	\
	{	\
		::OutputDebugStringW((LPCWSTR)EgLib::__ErrDesc(error));	\
		::OutputDebugStringW(L"\n");	\
	}





#if _MSC_VER > 1300

/////////////////////////////////////////////////////////////////////////////
//
#define _ERRCHKNULL(expression, pszSource, pszDesc)	\
	{	\
		if(NULL == expression)	\
		EgLib::__ErrThrow(E_FAIL, pszSource, pszDesc, __FILE__, __FUNCTION__ ,__LINE__) \
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
			EgLib::__ErrThrow(e, pszSource, pszDesc, __FILE__, __FUNCTION__ ,__LINE__) \
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
			EgLib::__ErrThrow(E_UNEXPECTED, pszSource, pszDesc, __FILE__, __FUNCTION__ ,__LINE__) \
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
			EgLib::__ErrThrow(e, pszSource, pszDesc, __FILE__, __LINE__);	\
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
			EgLib::__ErrThrow(E_UNEXPECTED, pszSource, pszDesc, __FILE__, __LINE__);	\
		}	\
		if(NULL == expressionNull)	\
			EgLib::__ErrThrow(E_FAIL, pszSource, pszDesc, __FILE__, __LINE__);	\
	}



#else


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



#endif

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



/// Legacy version macro definitions


// Check result and throw exception if error
#define __CHECK_HRESULT(expression, error_description)				\
	{																\
		HRESULT _hr = expression;									\
		if(FAILED(_hr))												\
		EgLib::CComErrorWrapper::ThrowError(_hr, error_description);	\
	}

// Check result and throw exception if error
#define __CHECK_HRESULT2(_hr, _expression, _error_description)				\
	{																		\
		if(FAILED(_hr = _expression))										\
			EgLib::CComErrorWrapper::ThrowError(_hr, _error_description);			\
	}

// Check result and throw exception if error
#define __CHECK_HRESULT4(_expression, _error_description)	\
	{														\
		HRESULT _hr = _expression;							\
		if(FAILED(_hr))										\
		{ \
			_bstr_t bsDescription(_error_description);		\
			bsDescription +=L": ";							\
			bsDescription += _bstr_t(__FILE__);				\
			bsDescription +=L"(";\
			bsDescription += _bstr_t(__LINE__);\
			bsDescription +=L")";\
			EgLib::CComErrorWrapper::ThrowError(_hr, bsDescription);			\
		} \
	}


// Check result and throw exception if error
#define __CHECK_HRESULT3(_expression)										\
	{																		\
		HRESULT _hr = _expression;\
		if(FAILED(_hr))										\
			EgLib::CComErrorWrapper::ThrowError(_hr, _T(""));						\
	}


// Check result and throw exception if error
#define __CHECK_HRESULT_STRICT(expression, error_description)		\
	{																\
		HRESULT _hr = expression;									\
		if(_hr != S_OK)												\
			EgLib::CComErrorWrapper::ThrowError(_hr, error_description);	\
	}

// Check interface pointer
#define __CHECK_INTERFACE_POINTER(sp, error_description)					\
	{																		\
		if(!sp.GetInterfacePtr())											\
			EgLib::CComErrorWrapper::ThrowError(E_NOINTERFACE, error_description);	\
	}

// Check pointer
#define		__CHECK_POINTER( ptr )	if (!ptr) return E_POINTER;


//=============================================================================================
// MACRO DEFINITION
//=============================================================================================

//---------------------------------------------------------------------------------------------
// MethodName - constant string - obsolete
// Description - constant string

//==========================================================================
// COM Exception catching and handling
//==========================================================================
#define _HANDLE_COM_EXCEPTION(Err)								\
			OutputDebugStringW(L"COM EXCEPTION HANDLED!!! ");	\
			_ERRTRACE(Err);													\
			ATLASSERT(false);													

#define _CATCH_COM_EXCEPTION_THROW( Source  )			\
		catch(_com_error& err)										\
		{															\
			_HANDLE_COM_EXCEPTION(err);					\
			_ERRTRW(err, Source, L"");					\
		}

#define _CATCH_COM_EXCEPTION_RETURN_COM( Description)	\
		catch(_com_error& err)										\
		{															\
			_HANDLE_COM_EXCEPTION(err);					\
			return _ERRSET(err, L"", Description);			\
		}
//==========================================================================

//==========================================================================
// Unhandled exception catching and handling
//==========================================================================


#if _MSC_VER > 1300


#define _HANDLE_EXCEPTION							\
			AtlTrace(L"UNHANDLED EXCEPTION!!! ", __FUNCTION__ );	\
			ATLASSERT(false);									


#else

#define _HANDLE_EXCEPTION							\
			AtlTrace(L"UNHANDLED EXCEPTION!!! ");	\
			ATLASSERT(false);									


#endif


#define _CATCH_UNHANDLED_EXCEPTION			\
		catch(...)												\
		{														\
			_HANDLE_EXCEPTION;						\
		}

#define _CATCH_UNHANDLED_EXCEPTION_THROW			\
		catch(...)												\
		{														\
			_HANDLE_EXCEPTION;						\
			throw;												\
		}

#define _CATCH_UNHANDLED_EXCEPTION_THROW_COM						\
		catch(...)																\
		{																		\
			_HANDLE_EXCEPTION;										\
			_ERRTRW((HRESULT)E_UNEXPECTED, L"", L"UNHANDLED EXCEPTION!!!");				\
		}


#define _CATCH_UNHANDLED_EXCEPTION_RETURN_COM					\
		catch(...)																\
		{																		\
			_HANDLE_EXCEPTION										\
			return _ERRSET((HRESULT)E_UNEXPECTED, L"", L"UNHANDLED EXCEPTION!!!");		\
		}

