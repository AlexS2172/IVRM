// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <conio.h>
#include <stdio.h>
#include <tchar.h>

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

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
#include <EgLib/EgLibStrategy.h>
#include <EgLib/egLibReg.h>
#include <EgLib/egLibDB.h>
using namespace EgLib;

#include <EgLib\EgLibComError.h>
#include <vcue\collections.h>
#include <ComponentsCommon.h>
#include <EtsDate.h>
#include <Boost/shared_array.hpp>
#include <Boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#import "../../tlb/MsgStruct.tlb" no_namespace named_guids
#import "../../tlb/VADBLayout.tlb"		no_namespace named_guids
#import "../../tlb/VolatilitySources.tlb"	no_namespace named_guids
#import "../../../MarketDataAdapters/Tlb/PriceProviders.tlb" rename("SettlementTypeEnum", "PpSettlementTypeEnum") no_namespace named_guids

#import "../../Components/tlb/EtsGeneral.tlb"	no_namespace named_guids
#import "../../Components/tlb/EtsMMGeneral.tlb"	no_namespace named_guids

#import "../../tlb/MMW.tlb"		no_namespace named_guids

#import "../../tlb/ETSXMLParams.tlb" no_namespace named_guids
#include "XMLParamsHelper.h"

#include <OptionCalc\OptionCalc.h>
#include <MarketCommon.h>
using namespace IvRmCore;

#include "Settings.h"
#include "Common.h"
#include "Cache.h"
#include "Messenger.h"
#include <iostream>

#include <hash_map>
#include <hash_set>
#include <vector>
#pragma warning( disable : 4267 4312 4311)
#include <ivdataservice410.h>
#pragma warning( default : 4267 4312 4311)
#include <VolatilityServerTypes/datetimetype.h>
#include <VolatilityServerCore/CORBAServerApplication.h>
#include <VolatilityServerCore/WaitableTimer.h>
#include <VolatilityServerCore\appsettingsbase.h>
#include <VolatilityServerCore/Logger.h>
#include <MsgParser/StreamFactory.h>
#include <MsgParser/VSMessageParser.h>
#include <MsgParser\VSParseException.h>
#include <MsgParser\responseblocklist.h>
#include <MsgAssembler/MessageGroup.h>
#include <MsgAssembler/MessageTrails.h>
#include <MsgAssembler/MessageFactory.h>
#include <MsgAssembler/StreamWriterFactory.h>
#include <VolatilityServerTypes/TableRow.h>

#include <log4cplus/fileappender.h>
#include <log4cplus/tstring.h>
#include <log4cplus/layout.h>

#define MAIN_SECTION	_T("MAIN")
#define OBJECT_NAME_KEY	_T("ObjectName")

#include <eh.h>
#include "SeException.h"

#include <ATLComTime.h>

#define BAD_DOUBLE_VALUE	-1E+308
#define BAD_LONG_VALUE		-2147483647

//------------------------------------------------------------//
#define TRACE_COM_ERROR(TYPE, LOG, ERR, DEV_TEXT)	\
{													\
	_bstr_t		bsSource(ERR.Source());				\
	_bstr_t		bsDescription(ERR.Description());	\
													\
	std::string	errMessage;							\
	char		buffer[2048];						\
	memset(	buffer,	0, size_t(2048) * sizeof(char));\
									\
									\
	sprintf_s(buffer,				\
	2048,							\
	"COM ERROR\n\tInfo = %s\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",\
	DEV_TEXT.c_str(),				\
	err.Error(),					\
	err.ErrorMessage(),				\
	(LPCSTR) bsSource,				\
	(LPCSTR) bsDescription);		\
									\
	errMessage += buffer;			\
									\
	TYPE(LOG, errMessage.c_str());	\
}	\
//------------------------------------------------------------//
#define TRACE_EXECUTION(TYPE, LOG, DEV_TEXT)	\
{												\
	TYPE(LOG, DEV_TEXT.c_str());				\
}												\
//------------------------------------------------------------//
#define TRACE_UNKNOWN_ERROR(TYPE, LOG, DEV_TEXT)	\
	TRACE_EXECUTION(TYPE, LOG, DEV_TEXT)			\
//------------------------------------------------------------//
long get_configuration_file_name(std::string& file_name);
//------------------------------------------------------------//
std::string 
ToString(const CORBA::SystemException& e);
//------------------------------------------------------------//
std::string 
ToString(const CORBA::Exception & e);
//------------------------------------------------------------//
std::string 
ToString(const omniORB::fatalException & e);
//------------------------------------------------------------//
// TODO: reference additional headers your program requires here
