// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_FREE_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS
//#import "..\tlb\PriceProviders.tlb" no_namespace
#include "ActivMiddleware/ActivFramework/ActivFramework.h"
#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"
#include "ActivMiddleware/ActivAgentApplication/AgentApplication.h"
#include "ActivContentPlatform/ActivFeedCommon/UsEquityOptionHelper.h"


#define DEF_PROVIDER_ID    6L
#define DEF_PROVIDER_NAME  L"Active Price Provider" 
#define PROVIDER_SETTINGS_ROOT	_T("SOFTWARE\\eGAR\\Providers\\{687E392C-C0BB-44C8-8DB4-034E4FF10927}")
#define ASP_SETTINGS_ROOT	    _T("SOFTWARE\\eGAR\\ETS\\ASP")

#import <msxml4.dll> 
//using namespace MSXML2;

#include "resource.h"
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <hash_map>
#include <hash_set>
using namespace std;
using namespace stdext;

#include <atlbase.h>
#include <atlcom.h>
#include <crtdbg.h>
#include <atlutil.h>
#include <boost/shared_ptr.hpp>
using namespace ATL;

#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibReg.h>
#include <EgLib/EgLibException.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/EgLibComError.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibMath.h>
#include <EgLib/EgLibTimeZone.h>
#include <EgLib/EgLibTrace.h>
#include <EgLib/EgLibDynamicGITArray.h>
using namespace EgLib;

#include <boost/smart_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>

#include <PriceProviders.h>

#define BAD_DOUBLE_VALUE -1E+308
#define BAD_LONG_VALUE   -2147483647

typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateParams;
typedef CComRecPtr<QuoteUpdateInfo, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateInfo;

#ifndef _VK_QU
#define _VK_QU
/*
_QuoteRequest - QuoteUpdateParams packer
*/
struct	_QuoteRequest
{
	std::string			Symbol;
	InstrumentTypeEnum	Type;
	std::string			Exchange;
	int					TryCount;
	double				Rate;
	_QuoteRequest( QuoteUpdateParams& m_spRequest )
	{
		Symbol = "";
		wchar_t	*p = ( wchar_t *) m_spRequest.Symbol;
		while( p && *p )
			Symbol += *( char *) p++;
		p = ( wchar_t *) m_spRequest.Exchange;
		while( p && *p )
			Exchange += *( char *) p++;
		Type = m_spRequest.Type;
		TryCount = 0;
		Rate	=	1.0;
	}

	_QuoteRequest() : Symbol(""), Exchange(""), Type(enSTK), TryCount(0), Rate(1.0) { }
	void	AssignToQuoteUpdateParams( QuoteUpdateParams* pQuoteUpdateParams )
	{
		pQuoteUpdateParams->Type = Type;

		if( pQuoteUpdateParams->Symbol )
			::SysFreeString( pQuoteUpdateParams->Symbol );
		pQuoteUpdateParams->Symbol  = ::SysAllocString( ::CA2W( Symbol.c_str() ) );			
		if( pQuoteUpdateParams->Exchange ) 
			::SysFreeString( pQuoteUpdateParams->Exchange );
		pQuoteUpdateParams->Exchange  = ::SysAllocString( ::CA2W( Exchange.c_str() ) );	
		TryCount = 0;
	}
};

/*
_QuoteInfo - QuoteUpdateInfo packer.
*/
struct _QuoteInfo
{
	DATE			UpdateDateTime;
	long			SeqNum;
	double			BidPrice;
	double			AskPrice;
	long			BidSize;
	long			AskSize;
	std::string		Currency;
	long			LotSize;
	double			LastPrice;
	double			OpenInterest;
	long			Volume;
	std::string		Exchange;
	double			OpenPrice;
	double			ClosePrice;
	double			HighPrice;
	double			LowPrice;
	long			Skipped;
	long			TotalRequests;
	long			WaitTime;
	double			NetChange;
	QuoteStatusType Status;
	std::string		BidExchange;
	std::string		AskExchange;
	LONG			PriceTick;      	
	_QuoteInfo() :
		BidExchange(""), 
		AskExchange(""), 
		Exchange(""), 
		Currency("") 
	{
		BidPrice	= AskPrice=LastPrice=OpenInterest=OpenPrice=ClosePrice=HighPrice=LowPrice=NetChange= BAD_DOUBLE_VALUE;
		SeqNum		= BidSize= AskSize= LotSize= Volume= Skipped= TotalRequests= WaitTime= PriceTick= BAD_LONG_VALUE;
		Status		= enQuoteStatus_OK;
	}

	void	AssignToQuoteUpdateInfo( QuoteUpdateInfo* pQuoteUpdate )
	{
		pQuoteUpdate->UpdateDateTime = UpdateDateTime;
		pQuoteUpdate->BidPrice= BidPrice;		
		pQuoteUpdate->AskPrice= AskPrice;		
		pQuoteUpdate->BidSize= BidSize;		
		pQuoteUpdate->AskSize= AskSize;		
		pQuoteUpdate->PriceTick= PriceTick;	
		pQuoteUpdate->LotSize= LotSize;		
		pQuoteUpdate->LastPrice= LastPrice;	
		pQuoteUpdate->Volume= Volume;	
		pQuoteUpdate->OpenPrice= OpenPrice;	
		pQuoteUpdate->ClosePrice= ClosePrice;	
		pQuoteUpdate->HighPrice= HighPrice;	
		pQuoteUpdate->LowPrice= LowPrice;		
		pQuoteUpdate->NetChange= NetChange;	
		pQuoteUpdate->Status = Status;
		pQuoteUpdate->OpenInterest = OpenInterest;

		if( ::SysStringLen(pQuoteUpdate->Currency) )
			::SysFreeString( pQuoteUpdate->Currency );
		pQuoteUpdate->Currency  = ::SysAllocString( ::CA2W( Currency.c_str() ) );			

		if( ::SysStringLen( pQuoteUpdate->Exchange ) )
			::SysFreeString( pQuoteUpdate->Exchange );
		pQuoteUpdate->Exchange  = ::SysAllocString( ::CA2W( Exchange.c_str() ) );	

		if( ::SysStringLen( pQuoteUpdate->BidExchange ) )
			::SysFreeString( pQuoteUpdate->BidExchange );
		pQuoteUpdate->BidExchange  = ::SysAllocString( ::CA2W( BidExchange.c_str() ) );	

		if( ::SysStringLen( pQuoteUpdate->AskExchange ) )
			::SysFreeString( pQuoteUpdate->AskExchange );
		pQuoteUpdate->AskExchange  = ::SysAllocString( ::CA2W( AskExchange.c_str() ) );	
	}
};

typedef boost::shared_ptr<_QuoteInfo>		_QuoteInfoPtr;
typedef boost::shared_ptr<_QuoteRequest>	_QuoteRequestPtr;
#define SafeBSTR( bstr )	( ( bstr ) && ::SysStringLen( ( bstr ) ) ) ? ( bstr ) : L""
#endif
