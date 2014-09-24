#ifndef __EXCEPTION_CATCH_WRAPPER_H__
#define __EXCEPTION_CATCH_WRAPPER_H__

//=============================================================================================
// MACRO DEFINITION
//=============================================================================================

//---------------------------------------------------------------------------------------------
// MethodName - constant string
// Description - constant string

//==========================================================================
// COM Exception catching and handling
//==========================================================================
#define _HANDLE_COM_EXCEPTION(Err, MethodName)								\
			OutputDebugStringW(L"COM EXCEPTION HANDLED!!! " MethodName);	\
			_ERRTRACE(Err);													\
			ATLASSERT(false);													

#define _CATCH_COM_EXCEPTION_THROW(MethodName, Description)			\
		catch(_com_error& err)										\
		{															\
			_HANDLE_COM_EXCEPTION(err, MethodName);					\
			_ERRTRW(err, MethodName, Description);					\
		}

#define _CATCH_COM_EXCEPTION_RETURN_COM(MethodName, Description)	\
		catch(_com_error& err)										\
		{															\
			_HANDLE_COM_EXCEPTION(err, MethodName);					\
			return _ERRSET(err, MethodName, Description);			\
		}
//==========================================================================

//==========================================================================
// Unhandled exception catching and handling
//==========================================================================
#define _HANDLE_EXCEPTION(MethodName)							\
			AtlTrace(L"UNHANDLED EXCEPTION!!! " MethodName);	\
			ATLASSERT(false);									

#define _CATCH_UNHANDLED_EXCEPTION_THROW(MethodName)			\
		catch(...)												\
		{														\
			_HANDLE_EXCEPTION(MethodName);						\
			throw;												\
		}

#define _CATCH_UNHANDLED_EXCEPTION_THROW_COM(MethodName)						\
		catch(...)																\
		{																		\
			_HANDLE_EXCEPTION(MethodName);										\
			_ERRTRW(E_FAIL, MethodName, L"UNHANDLED EXCEPTION!!!");				\
		}


#define _CATCH_UNHANDLED_EXCEPTION_RETURN_COM(MethodName)						\
		catch(...)																\
		{																		\
			_HANDLE_EXCEPTION(MethodName);										\
			return _ERRSET(E_FAIL, MethodName, L"UNHANDLED EXCEPTION!!!");		\
		}
//==========================================================================

//=============================================================================================
// MACRO DEFINITION
//=============================================================================================

#endif __EXCEPTION_CATCH_WRAPPER_H__