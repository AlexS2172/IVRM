// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
#pragma warning( disable:4311 )

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlutil.h>
#include <comutil.h>

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>

#include <PriceProviders.h>

using namespace ATL;

#include <string>
#import "C:\Program Files\Common Files\System\ado\msado27.tlb" rename("EOF", "adoEOF"), named_guids

#pragma warning( disable : 4267 4312 4311)
#include "ivdataservice410.h"
#pragma warning( default : 4267 4312 4311)

#include "omniORB4/wstringtypes.h"
#include <MsgAssembler/MessageGroup.h>
#include <MsgAssembler/MessageFactory.h>
#include <MsgAssembler/MessageTrails.h>
#include <MsgParser/VSMessageParser.h>
#include <MsgParser/VSParseException.h>
#include <MsgParser/StreamFactory.h>
#include <VolatilityServerTypes/datetimetype.h>
#include <VolatilityServerCore/VolatilityServerCoreExport.h>
#include <VolatilityServerCore/IniFileParser.h>
#include <limits>
#include <iostream>
#import <msxml4.dll> 

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <hash_map>
#include <hash_set>
using namespace std;
using namespace stdext;

#include <atlbase.h>
#include <atlcom.h>
#include <crtdbg.h>
#include <atlutil.h>
#include <boost/shared_ptr.hpp>
using namespace ATL;
#include <stdlib.h>
#include <iostream>
#include <string>
#include <comutil.h>

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibException.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/EgLibComError.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibMath.h>
#include <EgLib/EgLibTimeZone.h>
#include <EgLib/EgLibTrace.h>
#include <EgLib/EgLibDynamicGITArray.h>
using namespace EgLib;

#include <boost/smart_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <winbase.h>
#include <windows.h>

#include "DataTypes.h"
#include <ORBInit.h>


#define	CATCHALL		catch(CORBA::SystemException& e) \
						{ \
							hr = S_FALSE; \
							std::string err_msg = "CORBA System Exception: "; \
							GetCORBAError(e, err_msg); \
							EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
						} \
							catch(CORBA::Exception& e) \
						{ \
							hr = S_FALSE; \
							std::string err_msg = "CORBA Exception: "; \
							GetCORBAError(e, err_msg); \
							EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
						} \
						catch(omniORB::fatalException& e) \
						{ \
							hr = S_FALSE; \
							std::string err_msg = "omniORB Fatal Exception: "; \
							err_msg += e.errmsg(); \
							EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
						} \
						catch(std::exception&) \
						{ \
							EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__, "STD exception"); \
						} \
						catch(...) \
						{ \
							hr = S_FALSE; \
							std::string err_msg = "Unknown error"; \
							EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
						}


#define	CATCHALL_WITH_ONERROR	catch(CORBA::SystemException& e) \
								{ \
									hr = S_FALSE; \
									std::string err_msg = "CORBA System Exception: "; \
									GetCORBAError(e, err_msg); \
									EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
									OnError(); \
								} \
									catch(CORBA::Exception& e) \
								{ \
									hr = S_FALSE; \
									std::string err_msg = "CORBA Exception: "; \
									GetCORBAError(e, err_msg); \
									EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
									OnError(); \
								} \
								catch(omniORB::fatalException& e) \
								{ \
									hr = S_FALSE; \
									std::string err_msg = "omniORB Fatal Exception: "; \
									err_msg += e.errmsg(); \
									EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
									OnError(); \
								} \
								catch(...) \
								{ \
									hr = S_FALSE; \
									std::string err_msg = "Unknown error"; \
									EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  err_msg.c_str()); \
									OnError(); \
								}
void trim(string& str);


//-------------------------------------------------------------------------------------------------------------//
#define TRACE_COM_ERROR(__error)																			\
	EgLib::CEgLibTraceManager::Trace(LogError,																\
	__FUNCTION__,															\
	_TEXT("ComError: %s[%d] FileName: %s Line: %d"),						\
	(LPCSTR)__error.Description(), __error.Error(), __FILE__, __LINE__)		\

#define TRACE_INFO(__format, __description)																	\
	EgLib::CEgLibTraceManager::Trace(LogInfo,																\
	__FUNCTION__,															\
	__format,																\
	__description)															\

#define TRACE_ERROR(__format, __description) 																\
	EgLib::CEgLibTraceManager::Trace(LogError,																\
	__FUNCTION__,															\
	__format,																\
	__description)															\

#define TRACE_UNKNOWN_ERROR() 																				\
	EgLib::CEgLibTraceManager::Trace(LogInfo,																\
	__FUNCTION__,																			\
	_TEXT("Unknown error! %s[%d] WinError: [%d]"),											\
	__FILE__, __LINE__, ::GetLastError())														\

#define TRACE_COM_ERROR_EX(__error, __info, __description)												\
	EgLib::CEgLibTraceManager::Trace(LogError,															\
	__FUNCTION__,																						\
	__info _TEXT("ComError: %s[%d] FileName: %s Line: %d"),												\
	__description,(LPCSTR)__error.Description(), __error.Error(), __FILE__, __LINE__)					\
//-------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//