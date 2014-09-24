// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

//#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
//#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
//#endif

#define WIN32_LEAN_AND_MEAN

//#include <conio.h>
//#include <stdio.h>
//#include <tchar.h>

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include "ActivMiddleware/ActivFramework/ActivFramework.h"
#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"
#include "ActivMiddleware/ActivAgentApplication/AgentApplication.h"
#include "ActivContentPlatform/ActivFeedApi/ActivFeedApi.h"
#include "ActivContentPlatform/ActivFeedCommon/UsEquityOptionHelper.h"


#include <atlbase.h>
#include <atlcom.h>
#include <atlutil.h>

using namespace ATL;

// Please add "EgCommon\include" path to "Tools->Options->VC++ Directories->Include Files" section
#define EG_DB_BACKWARD_COMPATIBLE
#include <EgLib/egLibDbg.h>
#include <EgLib/egLibSync.h>
#include <EgLib/egLibThread.h>
#include <EgLib/egLibMisc.h>
#include <EgLib\EgLibComError.h>
#include <EgLib/egLibDB.h>
using namespace EgLib;

#include <ComponentsCommon.h>
#include <EtsDate.h>
#include <Boost/shared_array.hpp>
#include <Boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <deque>
#include <hash_map>
#include <hash_set>
using namespace std;
using namespace stdext;

#pragma warning( disable : 4267 4312 4311)
#include <ivdataservice410.h>
#pragma warning( default : 4267 4312 4311)

#include <VolatilityServerTypes/datetimetype.h>
#include <VolatilityServerCore/Logger.h>

#include <ORBInit.h>
#include "CSwrapper.h"
#include "Settings.h"
#include "IniReader.h"
#include "ProviderCommon.h"

#include <eh.h>
#include "SeException.h"

//logger
extern log4cplus::Logger ProviderLog;
extern log4cplus::Logger CorbaLog;

//logging configurations
#define LOG_ALL_EXEPTIONS

#ifdef LOG_ALL_EXEPTIONS
#define LOG_EXCEPTION LOG4CPLUS_DEBUG(ProviderLog, _T("Exception: ") << __FILE__ << _T(":") << __LINE__);
#else
#define LOG_EXCEPTION ;
#endif

#import "../../tlb/MsgStruct.tlb" no_namespace named_guids
#import "../../tlb/MMW.tlb"		no_namespace named_guids

#include "Database.h"
