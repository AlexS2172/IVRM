#ifndef __EG_STD_LIB_COMMON_H__
#define __EG_STD_LIB_COMMON_H__

//=============================================================================
// Export/Import substitution
//=============================================================================
#ifdef EGSTD_EXPORT
	#define EGSTD_EXT_CLASS __declspec(dllexport)
	#define EGSTD_EXT_FUNC	__declspec(dllexport)
	#define EGSTD_EXT_DATA	__declspec(dllexport)
#else
	#define EGSTD_EXT_CLASS __declspec(dllimport)
	#define EGSTD_EXT_FUNC	__declspec(dllimport)
	#define EGSTD_EXT_DATA	__declspec(dllimport)
#endif

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <conio.h>
#include "time.h"

#include <windows.h>
#include <process.h>
#include <string.h>

#include <comdef.h>

#include <shlwapi.h>
#include <tchar.h>

#include <atlbase.h>

#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

#include <EgLib\EgLibDbg.h>
#include <EgLib\EgLibReg.h>

using namespace EgLib;

typedef basic_string<TCHAR>		tstring;
typedef basic_fstream < TCHAR > tfstream;
typedef unsigned long u_long;

#pragma warning (disable : 4251)

#endif //__EG_STD_LIB_COMMON_H__