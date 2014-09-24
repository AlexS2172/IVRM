// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


#include <atlimpl.cpp>

#include "PriceProviders_i.c"

//CHyperFeedTrace* g_pTrace = NULL;

CMyLock		g_hLock;

char GetHFStockExchange(char cExchange)
{
	cExchange = toupper(cExchange);
	CServiceModule::_ExchangeCodes::const_iterator it = _Module.m_StockExchangeCodes.find(cExchange);
	return (it != _Module.m_StockExchangeCodes.end()) ? it->second : cExchange;
}

char GetHFOptionExchange(char cExchange)
{
	cExchange = toupper(cExchange);
	CServiceModule::_ExchangeCodes::const_iterator it = _Module.m_OptionExchangeCodes.find(cExchange);
	return (it != _Module.m_OptionExchangeCodes.end()) ? it->second : cExchange;
}

char GetETSStockExchange(char cExchange)
{
	cExchange = toupper(cExchange);
	for(CServiceModule::_ExchangeCodes::const_iterator it = _Module.m_StockExchangeCodes.begin(); it != _Module.m_StockExchangeCodes.end(); it++)
	{
		if(it->second == cExchange)
			return it->first;
	}
	return cExchange;
}

char GetETSOptionExchange(char cExchange)
{
	cExchange = toupper(cExchange);
	for(CServiceModule::_ExchangeCodes::const_iterator it = _Module.m_OptionExchangeCodes.begin(); it != _Module.m_OptionExchangeCodes.end(); it++)
	{
		if(it->second == cExchange)
			return it->first;
	}
	return cExchange;
}

long _bstr_cmp(const _bstr_t& cmp1, const _bstr_t& cmp2)
{
	long nRes = 0;

	if (cmp1.length() && cmp2.length())
	{
		nRes = wcscmp(cmp1, cmp2);
	}
	else if (cmp1.length() == 0)
	{
		if (cmp2.length() == 0)
		{
			nRes = 0;
		}
		else
		{
			nRes = -1;
		}
	}
	else
	{
		nRes = 1;
	}

	return nRes;
}
