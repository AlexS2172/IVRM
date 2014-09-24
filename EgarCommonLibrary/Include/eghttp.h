#ifndef __EG_HTTP_H_INCLUDE__
#define __EG_HTTP_H_INCLUDE__

#include "..\EgHttp\EgHttp.h"
#include "..\EgHttp\EgIvProvider.h"

#if defined(_UNICODE) || defined(UNICODE)
	#ifdef _DEBUG
		#pragma comment (lib, "EgHttpUD.lib")
		#pragma message ("Automatically likned with EgHttpUD.dll")
	#else
		#pragma comment (lib, "EgHttpU.lib")
		#pragma message ("Automatically likned with EgHttpU.dll")
	#endif // _DEBUG
#else
	#ifdef _DEBUG
		#pragma comment (lib, "EgHttpD.lib")
		#pragma message ("Automatically likned with EgHttpD.dll")
	#else
		#pragma comment (lib, "EgHttp.lib")
		#pragma message ("Automatically likned with EgHttp.dll")
	#endif   // _DEBUG
#endif // _UNICODE

#endif //__EG_HTTP_H_INCLUDE__
