#ifndef __EG_STD_LIB7_H__
#define __EG_STD_LIB7_H__

#ifndef __EG_STD_LIB_H__

//=============================================================================
// Autolinking
//=============================================================================
#ifndef EGSTD_EXPORT
	#ifdef _DEBUG
		#pragma message ("Automatically linked with EgStdLib7D.dll")
		#pragma comment (lib, "EgStdLib7D.lib")
	#else
		#pragma message ("Automatically linked with EgStdLib7.dll")
		#pragma comment (lib, "EgStdLib7.lib")
	#endif
#endif


#else
	#pragma message ("Can't likned with EgStdLib.dll & EgStdLib7.dll simalteniously !!!")
#endif //


#endif //__EG_STD_LIB7_H__