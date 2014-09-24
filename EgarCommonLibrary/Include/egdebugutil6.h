#ifndef __EG_DEBUG_UTIL6_H_INCLUDE__
#define __EG_DEBUG_UTIL6_H_INCLUDE__

#ifndef __EG_DEBUG_UTIL_H_INCLUDE__

#include "..\EgDebugUtil\EgDebugUtil.h"

#if defined(_UNICODE) || defined(UNICODE)
	#ifdef _DEBUG
		#pragma comment (lib, "EgDebugUtil6UD.lib")
		#pragma message ("Automatically likned with EgDebugUtil6UD.dll")
	#else
		#pragma comment (lib, "EgDebugUtil6U.lib")
		#pragma message ("Automatically likned with EgDebugUtil6U.dll")
	#endif // _DEBUG
#else
	#ifdef _DEBUG
		#pragma comment (lib, "EgDebugUtil6D.lib")
		#pragma message ("Automatically likned with EgDebugUtil6D.dll")
	#else
		#pragma comment (lib, "EgDebugUtil6.lib")
		#pragma message ("Automatically likned with EgDebugUtil6.dll")
	#endif   // _DEBUG
#endif // _UNICODE

#else
	#pragma message ("Can't likned with EgDebugUtil6.dll & EgDebugUtil.dll simalteniously !!!")
#endif //

#endif //__EG_DEBUG_UTIL6_H_INCLUDE__
