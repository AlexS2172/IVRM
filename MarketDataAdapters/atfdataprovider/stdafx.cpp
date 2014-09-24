// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#endif

#include "PriceProviders_i.c"

//#define ATFSTK_TO_ETS_CASE(_exch)	case ATFSTK_##_exch: bsRet = C2BSTR(ETS_##_exch); break;
//
//_bstr_t GetMTStockExchange(char cExchange)
//{
//	_bstr_t bsRet;
//	cExchange = toupper(cExchange);
//	switch(cExchange)
//	{
//	ATFSTK_TO_ETS_CASE(BOSTON)
//	ATFSTK_TO_ETS_CASE(CINCINNATI)
//	ATFSTK_TO_ETS_CASE(MIDWEST)
//	ATFSTK_TO_ETS_CASE(NASDAQ_NATIONALMARKET)
//	ATFSTK_TO_ETS_CASE(NASDAQ_SMALLCAP)
//	ATFSTK_TO_ETS_CASE(NASDAQ_INTERMARKET)
//	ATFSTK_TO_ETS_CASE(NASDAQ_BULLETINBOARD)
//	ATFSTK_TO_ETS_CASE(NASDAQ_OTHER)
//	ATFSTK_TO_ETS_CASE(NEWYORK)
//	ATFSTK_TO_ETS_CASE(AMEX)
//	ATFSTK_TO_ETS_CASE(PHILADELPHIA)
//	ATFSTK_TO_ETS_CASE(PACIFIC)
//	ATFSTK_TO_ETS_CASE(CHICAGO)
//	default:
//		bsRet = C2BSTR(cExchange);
//	}
//
//	return bsRet;
//}

#define ETS_TO_ATFSTK_CASE(_exch)	case(ETS_##_exch): bsRet = C2BSTR(ATFSTK_##_exch); break;

_bstr_t GetATStockExchange(char cExchange)
{
	_bstr_t bsRet;
	cExchange = toupper(cExchange);
	//switch(cExchange)
	//{
	//ETS_TO_ATFSTK_CASE(BOSTON)
	//ETS_TO_ATFSTK_CASE(CINCINNATI)
	//ETS_TO_ATFSTK_CASE(MIDWEST)
	//ETS_TO_ATFSTK_CASE(NASDAQ_NATIONALMARKET)
	//ETS_TO_ATFSTK_CASE(NASDAQ_SMALLCAP)
	//ETS_TO_ATFSTK_CASE(NASDAQ_INTERMARKET)
	//ETS_TO_ATFSTK_CASE(NASDAQ_BULLETINBOARD)
	//ETS_TO_ATFSTK_CASE(NASDAQ_OTHER)
	//ETS_TO_ATFSTK_CASE(NEWYORK)
	//ETS_TO_ATFSTK_CASE(AMEX)
	//ETS_TO_ATFSTK_CASE(PHILADELPHIA)
	//ETS_TO_ATFSTK_CASE(PACIFIC)
	//ETS_TO_ATFSTK_CASE(CHICAGO)
	//default:
	//	bsRet = C2BSTR(cExchange);
	//}

	CExeModule::_ExchangeCodes::const_iterator it = _Module.m_StockExchangeCodes.find(cExchange);
	if (it != _Module.m_StockExchangeCodes.end())
		bsRet = C2BSTR(it->second);
	else
		bsRet = C2BSTR(cExchange);

	return bsRet;
}

//#define ATFOPT_TO_ETS_CASE(_exch)	case(ATFOPT_##_exch): bsRet = C2BSTR(ETS_##_exch); break;
//
//_bstr_t GetMTOptionExchange(char cExchange)
//{
//
//	_bstr_t bsRet;
//	cExchange = toupper(cExchange);
//	switch(cExchange)
//	{
//	ATFOPT_TO_ETS_CASE(AMEX)
//	ATFOPT_TO_ETS_CASE(CHICAGO)
//	ATFOPT_TO_ETS_CASE(PHILADELPHIA)
//	ATFOPT_TO_ETS_CASE(PACIFIC)
//	ATFOPT_TO_ETS_CASE(ISE)
//	default:
//		bsRet = C2BSTR(cExchange);
//	}
//
//	return bsRet;
//}

#define ETS_TO_ATFOPT_CASE(_exch)	case(ETS_##_exch): bsRet = C2BSTR(ATFOPT_##_exch); break;

_bstr_t GetATOptionExchange(char cExchange)
{
	_bstr_t bsRet;
	cExchange = toupper(cExchange);
	//switch(cExchange)
	//{
	//ETS_TO_ATFOPT_CASE(AMEX)
	//ETS_TO_ATFOPT_CASE(CHICAGO)
	//ETS_TO_ATFOPT_CASE(PHILADELPHIA)
	//ETS_TO_ATFOPT_CASE(PACIFIC)
	//ETS_TO_ATFOPT_CASE(ISE)
	//default:
	//	bsRet = C2BSTR(cExchange);
	//}

	CExeModule::_ExchangeCodes::const_iterator it = _Module.m_OptionExchangeCodes.find(cExchange);
	if (it != _Module.m_OptionExchangeCodes.end())
		bsRet = C2BSTR(it->second);
	else
		bsRet = C2BSTR(cExchange);

	return bsRet;
}
