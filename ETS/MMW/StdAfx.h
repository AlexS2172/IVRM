#ifndef __STD_AFX_H__
#define __STD_AFX_H__

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_FREE_THREADED

#include <atlbase.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

extern CComModule _Module;

#include <atlcom.h>
#include <comutil.h>

#include <string>
#include <algorithm>
#include <map>

using namespace std;

#include <egatlcom.h>

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibReg.h>
using namespace EgLib;
//extern _bstr_t g_bstrSubjectPrefix;

#import "..\tlb\MsgStruct.tlb" no_namespace raw_interfaces_only
#import "..\tlb\Transport.tlb" no_namespace named_guids raw_interfaces_only , rename("SendMessage", "SendTransportMessage")
#import "..\tlb\EgTpPrx.tlb" no_namespace named_guids raw_interfaces_only

// Please add "C:\Program Files\Common Files\System\ado" path to "Tools->Options->VC++ Directories->Executable Files" section
#import "msado25.tlb" no_namespace, named_guids, rename("EOF", "Eof")
//#include <EgStdLib\EgStdLib.h>

#import "../tlb/ETSXMLParams.tlb" no_namespace named_guids
	
#endif //__STD_AFX_H__
