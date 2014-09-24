// Options.cpp : Implementation of COptions
#include "stdafx.h"
#include "Options.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptions::COptions() : 
m_bIsGotOptions(false),
m_bIsGotError(false),
m_bTerminate(true)
{
	m_optionsFilter.lpfnDbOptionsProc = _OptionsProcStatic;
	m_optionsFilter.pUser = this;
	m_optionsFilter.hThread = 0;
}

COptions::~COptions()
{
	Detach();
}
void COptions::GetOptions(BSTR bsUnderlying)
{
	CancelRequest();

	_SetDbKey(bsUnderlying);
	
	long nResult = _GetOptions();

	if(DBA_ERR_NO_ERROR == nResult)
	{
		if(m_bIsGotOptions && !m_bIsGotError)
		{
			PublicLastOption();
		}
		else if(!m_bIsGotOptions && !m_bIsGotError) 
		{
			PublicGetOptionError(enNoDataAvailableForSymbol, _bstr_t(L"Couldn't get options for underlying. No options found."));
		}
	}
	else
	{
		if(nResult == DBA_ERR_KEY_NOT_FOUND)
		{
			PublicGetOptionError(enNoDataAvailableForSymbol, _bstr_t(L"Couldn't get options for underlying. No options found."));
		}
		else if(nResult == DBA_TERMINATED)
		{
			PublicGetOptionError(enInternalError, _bstr_t(L"Operation canceled by user."));
		}
		else
		{
			_bstr_t bs =  "Couldn't get options for underlying. Error: ";
			bs += EtGetMessage(DBA_ERROR,nResult);
			TCHAR buffer[0x100] = {0};				
			_ltot(nResult,buffer,10);
			bs += " (";
			bs += buffer;
			bs += ")";
			PublicGetOptionError(enProviderInternalError, bs);
		}
	}
}
long COptions::_GetOptions()
{
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("_GetOptions Enter %s"), _T(m_dbaKey.dbaKey.symbol) );
	long nStatus = 0;
	try
	{
		m_bIsGotOptions = false;
		m_bIsGotError = false;
		m_bTerminate = false;

		int nRecLen = sizeof(OPTIONS_FILTER);
		int nKeyLen = sizeof(DBA_KEY);

		m_optionsFilter.flags.mask = 0;
		m_optionsFilter.flags.mask += DBA_OFM_COMPOSITES;
		m_optionsFilter.flags.mask += DBA_OFM_CALLS;
		m_optionsFilter.flags.mask += DBA_OFM_PUTS;

		m_optionsFilter.months = -1;
		m_optionsFilter.count = -1;
		m_optionsFilter.nRecLen = sizeof(DBA_OPTIONS_FILTER_RECORD);

		long nFunc = DBA_GET_OPTIONS;
		nStatus = dba(&m_blk, nFunc, &m_dbaKey.dbaKey, &nKeyLen, &m_optionsFilter, &nRecLen, 0);
		
		if (nStatus == DBA_ERR_NO_ERROR)
		{
			EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("_GetOptions NoError") );
			::WaitForSingleObject(m_optionsFilter.hThread, INFINITE);
			if(m_bTerminate)
			{
				nStatus = DBA_TERMINATED;
				EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("_GetOptions nStatus = DBA_TERMINATED") );
			}
				
		}
		if(m_optionsFilter.hThread)
		{
			//::CloseHandle(m_optionsFilter.hThread);
			m_optionsFilter.hThread = 0;
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgStd::CEgTracingClass::TraceStatic(enlogSubs, __FUNCTION__ , _T("_GetOptions Exit %s"), _T(m_dbaKey.dbaKey.symbol));
	return nStatus;
}
long CALLBACK COptions::_OptionsProcStatic(VOID *pblk, ULONG ulFunction, DBA_KEY *pDbaKey, int nKeyLen, DBA_OPTIONS_FILTER_RECORD *pDbaRec, int nRecLen, ULONG ulKey, DWORD dwStatus, void *p) 
{
	OPTIONS_FILTER* pFilter = (OPTIONS_FILTER*)p;
	ULONG ulFunc = ulFunction & DBAX_FUNCTION_MASK;

	COptions* pThis = (COptions*)pFilter->pUser;
	_ASSERTE(pThis);

	if(pThis)
		return pThis->_OptionsProc(ulFunction,pDbaKey, pDbaRec, dwStatus);

	return TRUE;
}

long COptions::_OptionsProc(ULONG ulFunction, DBA_KEY *pDbaKey, DBA_OPTIONS_FILTER_RECORD *pDbaRec, DWORD dwStatus) 
{
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION || m_bTerminate)
		return FALSE;

	if (dwStatus == DBA_ERR_NO_ERROR)
	{
		if(!pDbaKey->exchangeCode[0] || pDbaKey->exchangeCode[0] == ' ')
		{
			m_bIsGotOptions = true;
			PublicOption(pDbaRec);
		}
		return FALSE;
	}
	else 
	{
		if(dwStatus == DBA_ERR_KEY_NOT_FOUND)
		{
			PublicGetOptionError(enNoDataAvailableForSymbol, _bstr_t(L"Couldn't get options for underlying. Unknown symbol."));
		}
		else if(dwStatus != DBA_ERR_EOF)
		{
			_bstr_t bs =  "Couldn't get options for underlying. Error: ";
			bs += EtGetMessage(DBA_ERROR,dwStatus);
			TCHAR buffer[0x100] = {0};				
			_ltot(dwStatus,buffer,10);
			bs += " (";
			bs += buffer;
			bs += ")";
			PublicGetOptionError(enProviderInternalError, bs);
		}
		m_bIsGotError = true;
	}

	return FALSE;
}

void COptions::_SetDbKey(BSTR bsUnderlying)
{
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.countryCode, "US", 2);
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.currencyCode, "USD", 3);
	CreateSpaced((char *)m_dbaKey.dbaKey.symbol, bsUnderlying, MAX_TICKER);
	m_dbaKey.dbaKey.exchangeCode[0] = ' ';
	m_dbaKey.dbaKey.exchangeCode[1] = ' ';

	if(m_dbaKey.dbaKey.symbol[0] == '$')
	{
		m_dbaKey.dbaKey.type = IT_INDEX;
		RemoveBucksFromName((char*)m_dbaKey.dbaKey.symbol);
	}
	else
	{
		m_dbaKey.dbaKey.type = IT_EQUITY;
	}
}
DATE COptions::_CreateExpiryDate(INT nRelativeMonth)
{
	SYSTEMTIME	stCurDate = { 0 };
	DATE		dRet = 0.;

	if (VariantTimeToSystemTime((LONG)EgLib::vt_date::GetCurrentDate(), &stCurDate))
	{
		INT nRelativeYear = (stCurDate.wMonth + nRelativeMonth) / 12;
		
		stCurDate.wYear += nRelativeYear;
		stCurDate.wMonth += nRelativeMonth - nRelativeYear * 12;
		stCurDate.wDay = 15;
	
		SystemTimeToVariantTime(&stCurDate, &dRet);
	}

	return dRet;
}

void COptions::CancelRequest()
{
	if (m_optionsFilter.hThread != 0)
	{
		m_bTerminate = true;
		::WaitForSingleObject(m_optionsFilter.hThread, INFINITE);
	}
}

void COptions::CopyOptionFilterToInfo(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionInfo& Info)
{
	Info->ExpirationDate = _CreateExpiryDate(rec.relativemonth);			
	Info->StrikePrice = Price2Dbl(rec.strikePriceType, rec.strikePrice);

	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)rec.priceRec.dbaRec.item.symbol, MAX_TICKER);
	RemoveSpaceFromName(szName);

	_bstr_t bsSymbol(szName);
	_bstr_t bsCurrency(L"USD");

	if(!rec.type)
		Info->OptionType = enCall;
	else
		Info->OptionType = enPut;

	Info->Series = bsSymbol.Detach();
	Info->Currency = bsCurrency.Detach();

	Info->Ask = Price2Dbl(rec.priceRec.dbaRec.priceType, rec.priceRec.dbaRec.ask.price);
	Info->Bid = Price2Dbl(rec.priceRec.dbaRec.priceType, rec.priceRec.dbaRec.bid.price);
	Info->Last = Price2Dbl(rec.priceRec.dbaRec.priceType, rec.priceRec.dbaRec.lastSale.price);
	Info->OpenInterest = rec.priceRec.dbaRec.openInterest;
	Info->Volume = rec.priceRec.dbaRec.volume;
	Info->YVolume = rec.priceRec.yestVolume;
}

void COptions::CopyOptionFilterToResult(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionResults& Result)
{
	Result->ExpirationDate = _CreateExpiryDate(rec.relativemonth);			
	Result->StrikePrice = Price2Dbl(rec.strikePriceType, rec.strikePrice);

	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)rec.priceRec.dbaRec.item.symbol, MAX_TICKER);
	RemoveSpaceFromName(szName);

	_bstr_t bsSymbol(szName);
	_bstr_t bsCurrency(L"USD");

	if(!rec.type)
		Result->OptionType = enCall;
	else
		Result->OptionType = enPut;

	Result->Series = bsSymbol.Detach();
	Result->Currency = bsCurrency.Detach();
}

void COptions::CopyOptionFilterToResult(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionResultsEx& Result)
{
	Result->ExpirationDate = _CreateExpiryDate(rec.relativemonth);			
	Result->StrikePrice = Price2Dbl(rec.strikePriceType, rec.strikePrice);

	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)rec.priceRec.dbaRec.item.symbol, MAX_TICKER);
	RemoveSpaceFromName(szName);

	_bstr_t bsSymbol(szName);
	_bstr_t bsCurrency(L"USD");

	if(!rec.type)
		Result->OptionType = enCall;
	else
		Result->OptionType = enPut;

	Result->Series = bsSymbol.Detach();
	Result->Currency = bsCurrency.Detach();

	Result->ClosePrice = Price2Dbl(rec.priceRec.dbaRec.priceType, rec.priceRec.dbaRec.close.price);
}

void COptions::CopyOptionFilterToIvResult(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionForIvResult& Result)
{
	Result->ExpirationDate = _CreateExpiryDate(rec.relativemonth);			
	Result->Strike = Price2Dbl(rec.strikePriceType, rec.strikePrice);

	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)rec.priceRec.dbaRec.item.symbol, MAX_TICKER);
	RemoveSpaceFromName(szName);

	_bstr_t bsSymbol(szName);

	if(!rec.type)
		Result->OptionType = enCall;
	else
		Result->OptionType = enPut;

	Result->Ticker = bsSymbol.Detach();

	Result->BestBid = Price2Dbl(rec.priceRec.dbaRec.priceType, rec.priceRec.dbaRec.bid.price);
	Result->BestBidExchange = static_cast<HfUsaOptionsExchanges>(rec.priceRec.dbaRec.bid.exchangeCode[0]);
	Result->BestBidSize = rec.priceRec.dbaRec.bid.size;

	Result->BestAsk = Price2Dbl(rec.priceRec.dbaRec.priceType, rec.priceRec.dbaRec.ask.price);
	Result->BestAskExchange = static_cast<HfUsaOptionsExchanges>(rec.priceRec.dbaRec.ask.exchangeCode[0]);
	Result->BestAskSize = rec.priceRec.dbaRec.ask.size;

	Result->LastSale = Price2Dbl(rec.priceRec.dbaRec.priceType, rec.priceRec.dbaRec.lastSale.price);
	Result->LastSaleExchange = static_cast<HfUsaOptionsExchanges>(rec.priceRec.dbaRec.lastSale.exchangeCode[0]);
	Result->LastSaleSize = rec.priceRec.dbaRec.lastSale.size;

	Result->Volume = rec.priceRec.dbaRec.volume;
	Result->OpenInterest = rec.priceRec.dbaRec.openInterest;
}
