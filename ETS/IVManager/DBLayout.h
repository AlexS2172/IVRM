// DBLayout.h: interface for the CDBLayout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBLAYOUT_H__4B6DE227_FA4A_41E7_BF66_F6D6E7BEF2A0__INCLUDED_)
#define AFX_DBLAYOUT_H__4B6DE227_FA4A_41E7_BF66_F6D6E7BEF2A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _IVAN_DB_ACCESS

#include "DBLayoutAccess.h"
typedef CDBLayoutAccess CDBLayout; 

#else

#include "DBLayoutSQL.h"
typedef CDBLayoutSQL CDBLayout; 

#endif


#endif // !defined(AFX_DBLAYOUT_H__4B6DE227_FA4A_41E7_BF66_F6D6E7BEF2A0__INCLUDED_)
