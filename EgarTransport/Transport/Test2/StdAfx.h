// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9362893A_25A1_42AC_A7EF_6B848330D0FC__INCLUDED_)
#define AFX_STDAFX_H__9362893A_25A1_42AC_A7EF_6B848330D0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <atlbase.h>

#include <process.h>

#include "objbase.h"

#include "Lock.h"
#include "ThreadClass.h"

#include "fstream"

#include "trace.h"

#import "..\..\tlb\MMW.tlb" no_namespace named_guids
#import "..\..\tlb\MsgStruct.tlb" no_namespace named_guids

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9362893A_25A1_42AC_A7EF_6B848330D0FC__INCLUDED_)
