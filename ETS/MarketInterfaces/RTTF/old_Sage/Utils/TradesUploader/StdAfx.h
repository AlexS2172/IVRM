// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D6B60953_74BF_49CE_8A2C_29D63CA441EC__INCLUDED_)
#define AFX_STDAFX_H__D6B60953_74BF_49CE_8A2C_29D63CA441EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

// STL includes
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#include <process.h>

#define EG_DB_BACKWARD_COMPATIBLE
#include <eg_lib\egDB.h>
#include <eg_lib\EgRegistry.h>
#include <eg_lib\EgMisc.h>
#include <Lock.h>

using namespace eg_lib;
using namespace eg_reg;


#ifdef _DEBUG
#define EgDbgBreakPoint			__asm int 3;
#else
#define EgDbgBreakPoint	
#endif

#import <MsgStruct.tlb> no_namespace named_guids 
#import <mmw.tlb> no_namespace named_guids
#import <ETSManager.tlb> no_namespace named_guids 

//UDTs
/////////////////////////////////////////////////////////////////////////////////
struct CFixTimeValue : public tm {};
struct CFixDateValue : public tm {};
struct CFixMonthYearValue : public tm {};
struct CFixDayOfMonthValue : public tm {};

/////////////////////////////////////////////////////////////////////////////////

struct CTrade
{
	string			sAccount;
	string			sExecID;
	char			cExecTransType;
	float			fLastPrice;
	int				iLastShares;
	char			cOrderStatus; 
	char			cSide;
	string			sSymbol;
    string          sRoot;
	CFixTimeValue	ftTransactTime;
	CFixDateValue	fdTradeDate;
	string			sSecurityType;
	string			sText;

	// for options
	CFixMonthYearValue	fmyMaturityMonthYear;
	int					iPutOrCall;			//0=Put, 1=Call
	float				fStrikePrice;
	CFixDayOfMonthValue	fmdMaturityDay;
	string				sUnderlying;
	
	string				sStatus;
};

typedef vector<CTrade> TTradesVector;

// ExecTransType
#define		ETT_NEW					'0'
#define		ETT_CANCEL				'1'
#define		ETT_CORRECT				'2'
#define		ETT_STATUS				'3'

// Side
#define		SIDE_BUY				'1'
#define		SIDE_SELL				'2'

// Security type
#define		ST_OPTION				_T("OPT")
#define		ST_STOCK				_T("CS")

// Put or call
#define		PC_PUT					0
#define		PC_CALL					1

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D6B60953_74BF_49CE_8A2C_29D63CA441EC__INCLUDED_)
