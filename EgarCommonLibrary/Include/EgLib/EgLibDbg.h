//**************************************************************************************************************//
// abstract:	interface & implementation of debug routines and classes
// copyright:	(c) 2003 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//

//**************************************************************************************************************//
// header identifier
//**************************************************************************************************************//
#define __EGLIBDBG_H__

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#if !defined(_EGLIB_NO_DEBUG_CRT)
//
// Warning: if you define the above symbol, you will have
// to provide your own definition of the EGLIBASSERT(x) macro
// in order to compile EGLIB
//
#include <crtdbg.h>

#define EGLIBASSERT _ASSERTE
#define EGLIBCHECK_EXIT(f) {HRESULT _hr = f; if(FAILED(_hr)) return _hr;}

#if defined(_DEBUG)
#define EGLIBVERIFY(f) EGLIBASSERT(f)
#else
#define EGLIBVERIFY(f) (void)(f)
#endif

#define EGLIBVERIFY_EXIT(f,result) if(!(f)){EGLIBASSERT(FALSE); return result;}

#endif //!defined(_EGLIB_NO_DEBUG_CRT)

#if defined(_DEBUG)
#define EGLIBDEBUGONLY(f)				(f)
#define EGLIBDEBUGBREAK					DebugBreak()
#define EGLIBDEBUGMSG(c, s)				if(c)::MessageBox(NULL, s, NULL, MB_ICONERROR);
#else
#define EGLIBDEBUGONLY(f)				((void)0)
#define EGLIBDEBUGBREAK					__noop
#define EGLIBDEBUGMSG(c, s)				((void)0)
#endif //_DEBUG

//**************************************************************************************************************//
// namespace OmLib
//**************************************************************************************************************//
namespace OmLib {

} //namespace OmLib