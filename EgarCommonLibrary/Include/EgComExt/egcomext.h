#ifndef __EG_COM_EXT_H__
#define __EG_COM_EXT_H__

//=============================================================================
// Export/Import substitution
//=============================================================================
#ifdef EGCOM_EXPORT
	#define EGCOM_EXT_CLASS __declspec(dllexport)
	#define EGCOM_EXT_FUNC	__declspec(dllexport)
	#define EGCOM_EXT_DATA	__declspec(dllexport)
#else
	#define EGCOM_EXT_CLASS __declspec(dllimport)
	#define EGCOM_EXT_FUNC	__declspec(dllimport)
	#define EGCOM_EXT_DATA	__declspec(dllimport)
#endif

//=============================================================================
// Autolinking
//=============================================================================
#ifndef EGCOM_EXPORT
	#ifdef _DEBUG
		#pragma message ("Automatically likned with EgComExtD.dll")
		#pragma comment (lib, "EgComExtD.lib")
	#else
		#pragma message ("Automatically likned with EgComExt.dll")
		#pragma comment (lib, "EgComExt.lib")
	#endif
#endif

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <process.h>
#include <string.h>
#include <comdef.h>
#include <shlwapi.h>
#include <tchar.h>
#include <limits.h>
#include <math.h>

#include <atlbase.h>

#pragma warning (disable : 4251)

#endif //__EG_STD_LIB_H__