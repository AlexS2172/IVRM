// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include "PriceProviders_i.c"
#include "MyTrackCore.h"
//--------------------------------------------------------------------------------------------------//
HRESULT CExeModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid)
{
	HRESULT hres = CComModule::Init (p,h,plibid);
	pCore = new (CMyTrackCore);	
	return hres;
}
//--------------------------------------------------------------------------------------------------//
void CExeModule::Term()
{
	delete (CMyTrackCore*)pCore;
	CComModule::Term();
}	
	
/////////////////////////////////////////////////////////////////////////////
char GetMTStockExchange(char cExchange)
{
	cExchange = toupper(cExchange);
	CExeModule::_ExchangeCodes::const_iterator it = _Module.m_StockExchangeCodes.find(cExchange);
	return (it != _Module.m_StockExchangeCodes.end()) ? it->second : cExchange;
}

char GetMTOptionExchange(char cExchange)
{
	cExchange = toupper(cExchange);
	CExeModule::_ExchangeCodes::const_iterator it = _Module.m_OptionExchangeCodes.find(cExchange);
	return (it != _Module.m_OptionExchangeCodes.end()) ? it->second : cExchange;
}