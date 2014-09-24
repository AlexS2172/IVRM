#ifndef __EG_STD_LIB_H__
#define __EG_STD_LIB_H__

#ifndef __EG_STD_LIB7_H__

#include <EgStdLib\EgStdLibCommon.h>
//=============================================================================
// Autolinking
//=============================================================================
#ifndef EGSTD_EXPORT
	#ifdef _DEBUG
		#pragma message ("Automatically likned with EgStdLibD.dll")
		#pragma comment (lib, "EgStdLibD.lib")
	#else
		#pragma message ("Automatically likned with EgStdLib.dll")
		#pragma comment (lib, "EgStdLib.lib")
	#endif
#endif

#else
	#pragma message ("Can't likned with EgStdLib.dll & EgStdLib7.dll simalteniously !!!")
#endif //

#endif //__EG_STD_LIB_H__