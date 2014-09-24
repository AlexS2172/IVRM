#ifndef __EG_DEBUG_UTIL_H_INCLUDE__
#define __EG_DEBUG_UTIL_H_INCLUDE__

#ifndef __EG_DEBUG_UTIL6_H_INCLUDE__

#include "..\EgDebugUtil\EgDebugUtil.h"

#if defined(_UNICODE) || defined(UNICODE)
	#ifdef _DEBUG
		#pragma comment (lib, "EgDebugUtilUD.lib")
		#pragma message ("Automatically likned with EgDebugUtilUD.dll")
	#else
		#pragma comment (lib, "EgDebugUtilU.lib")
		#pragma message ("Automatically likned with EgDebugUtilU.dll")
	#endif // _DEBUG
#else
	#ifdef _DEBUG
		#pragma comment (lib, "EgDebugUtilD.lib")
		#pragma message ("Automatically likned with EgDebugUtilD.dll")
	#else
		#pragma comment (lib, "EgDebugUtil.lib")
		#pragma message ("Automatically likned with EgDebugUtil.dll")
	#endif   // _DEBUG
#endif // _UNICODE

#else
	#pragma message ("Can't likned with EgDebugUtil6.dll & EgDebugUtil.dll simalteniously !!!")
#endif //

#endif //__EG_DEBUG_UTIL_H_INCLUDE__
