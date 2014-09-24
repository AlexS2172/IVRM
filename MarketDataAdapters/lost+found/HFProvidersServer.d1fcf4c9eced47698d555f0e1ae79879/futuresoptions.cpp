// FuturesOptions.cpp : Implementation of CFuturesOptions
#include "stdafx.h"
#include "FuturesOptions.h"
#include "Futures.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFuturesOptions::CFuturesOptions() : m_bIsGotOptions(false), m_nStreamError(DBA_ERR_NO_ERROR), 
m_pCurrentFuture(NULL), m_strCurrentRoot(""), m_bIsResultsEx(false),
CStream(&m_blk, &m_dbaKey, &m_dbaRec)
{
}

void CFuturesOptions::GetFutureOptionsResults(CFutureInfo& clFuture, bool bIsResultEx)
{
	if(IsStreamRunning())
		return;
	m_bIsResultsEx = bIsResultEx;
	m_bIsGotOptions = false;
	m_nStreamError = DBA_ERR_NO_ERROR;
	m_pCurrentFuture = &clFuture;

	bool bIsGotCriticalError = false;

	for(futureoptionrootmap::iterator iterRoot = clFuture.m_mapRoots.begin(); iterRoot != clFuture.m_mapRoots.end(); iterRoot++)
	{
		m_strCurrentRoot = iterRoot->first;
		long nResult = _GetFutureOptionsResultsByRoot(m_strCurrentRoot.c_str());
		
		if(nResult != DBA_ERR_NO_ERROR && nResult != DBA_ERR_KEY_NOT_FOUND)
		{
			bIsGotCriticalError = true;
			if(nResult == DBA_TERMINATED)
			{
				PublicGetFutureOptionError(enInternalError, _bstr_t(L"Operation was canceled by user"));
			}
			else
			{
				_bstr_t bs =  "Couldn't get options for future. Error: ";
				bs += EtGetMessage(DBA_ERROR,nResult);
				TCHAR buffer[0x100] = {0};				
				_ltot(nResult,buffer,10);
				bs += " (";
				bs += buffer;
				bs += ")";
				PublicGetFutureOptionError(enProviderInternalError, bs);
			}
			break;
		}
	}

	if(m_bIsGotOptions && !bIsGotCriticalError)
	{
		PublicLastFutureOption();
	}
	else if(!m_bIsGotOptions && !bIsGotCriticalError) 
	{
		PublicGetFutureOptionError(enNoDataAvailableForSymbol, _bstr_t(L"Couldn't get future options for future. Unknown symbol."));
	}
}

long CFuturesOptions::_GetFutureOptionsResultsByRoot(const char* szBaseSymbol)
{
	if(IsStreamRunning())
		return DBA_ERR_TERMINATE_STREAM;

	m_nStreamError = DBA_ERR_NO_ERROR;

	_SetDbKey(szBaseSymbol);

	m_streamFilter.ulPositionFunction = DBA_GET_APPROX;
	long nResult = GetNextStream(0);
	if(nResult == DBA_ERR_NO_ERROR)
	{
		WaitStreamTerminate();
		if(m_bTerminate)
		{
			return DBA_TERMINATED;
		}
		return m_nStreamError;
	}
	return nResult;
}

long CFuturesOptions::StreamData(long nStatus, long nFunc)
{
	BOOL bRet = FALSE;
	if(nStatus == DBA_ERR_NO_ERROR && nFunc != DBA_TERMINATE_STREAM_FUNCTION)
	{
		if(!_CheckIsFutureOptionSymbolOur((const char*)m_pDbaKey->dbaKey.symbol))
		{
			bRet = TRUE;
		}
		else if(m_pDbaRec->extended.item.type == IT_FUTURE_OPTION)
		{
			char szName[MAX_TICKER + 1];
			ZeroMemory(szName,sizeof(szName));
			CreateSz(szName, (const char *)m_pDbaRec->extended.item.symbol, MAX_TICKER);
			bool bIsCall; double dStrike;
			if(_CheckIsFutureOptionSymbolValid(szName, bIsCall, dStrike))
			{
				m_bIsGotOptions = true;
				if(!m_bIsResultsEx)
				{
					_FuturesOptionResults results;
					results.Init();
					results->StrikePrice = dStrike;
					_bstr_t bsName(szName);
					results->Series = bsName;
					if(bIsCall)
						results->OptionType = enCall;
					else
						results->OptionType = enPut;

					PublicFutureOption(results);
				}
				else
				{
					_FuturesOptionResultsEx results;
					results.Init();
					results->StrikePrice = dStrike;
					_bstr_t bsName(szName);
					results->Series = bsName;
					if(bIsCall)
						results->OptionType = enCall;
					else
						results->OptionType = enPut;

					results->ClosePrice = SetDoubleValue(Price2Dbl(m_pDbaRec->extended.priceType, m_pDbaRec->extended.close.price));

					PublicFutureOption(results);
				}
			}
		}
	}
	else 
	{
		if(nStatus != DBA_ERR_EOF)
		{
			m_nStreamError = nStatus;
		}
		bRet = TRUE;
	}	
	return bRet;
}

void CFuturesOptions::_SetDbKey(const char* szBaseSymbol)
{	
	m_dbaKey.dbaKey.type = IT_FUTURE_OPTION;
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.countryCode, "US", 2);
	CreateSpacedUpper((char*)m_dbaKey.dbaKey.currencyCode, "USD", 3);
	CreateSpaced((char *)m_dbaKey.dbaKey.symbol, szBaseSymbol, MAX_TICKER);
	m_dbaKey.dbaKey.exchangeCode[0] = ' ';
	m_dbaKey.dbaKey.exchangeCode[1] = ' ';
}

bool CFuturesOptions::_CheckIsFutureOptionSymbolValid(const char* szSymbol, bool& IsCall, double& dStrike)
{
	bool bIsGetStrike = false;
	for(int nIdx = strlen(szSymbol) - 1; nIdx >= 0; nIdx--)
	{
		if(!isdigit(szSymbol[nIdx]))
		{
			if(!bIsGetStrike)
			{
				dStrike = _ConvertStrikePriceFromFuturesClosePrice(_ttol(szSymbol + nIdx + 1));
				bIsGetStrike = true;
			}

			if(szSymbol[nIdx] == 'C')
			{
				IsCall = true;
				return true;
			}
			else if(szSymbol[nIdx] == 'P')
			{
				IsCall = false;
				return true;
			}
			else if(szSymbol[nIdx] != ' ')
				return false;
		}
	}
	return false;
}

void CFuturesOptions::CancelRequest()
{
	TerminateStream();
}

double CFuturesOptions::_ConvertStrikePriceFromFuturesClosePrice(long nStrike)
{
	double dTemp = fabs((double)nStrike);
	
	if(m_pCurrentFuture && dTemp > DBL_EPSILON)
	{
		double dFutureClosePrice = fabs(m_pCurrentFuture->m_dFutureClosePrice);

		dFutureClosePrice = (dFutureClosePrice > DBL_EPSILON) ? dFutureClosePrice : BAD_FUTURE_CLOSE_PRICE_REPLACER;

		while(dTemp >= 3 * dFutureClosePrice)
			dTemp /= 10.;

		while(dTemp < .3 * dFutureClosePrice)
			dTemp *= 10.;
	}

	return dTemp;
}

//////////////////
bool CFuturesOptions::_CheckIsFutureOptionSymbolOur(const char* szSymbol)
{
	if(_strnicmp(szSymbol,(const char*)m_strCurrentRoot.c_str(), m_strCurrentRoot.length()))
		return false;
	return true;
}
