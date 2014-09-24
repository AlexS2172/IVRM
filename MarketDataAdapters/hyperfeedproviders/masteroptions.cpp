// MasterOptions.cpp : Implementation of CMasterOptions
#include "stdafx.h"
#include "MasterOptions.h"
#include <ATLComTime.h>
using namespace ATL;

#define NUM_STRIKEPRICES 26
#define NUM_EXCHANGES 8
#define NUM_EXPIRATIONS 12
#define NUM_SPECIAL_DIVIDENDS 4
#define NUM_CALENDAR_DATES 24
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterOptions::CMasterOptions()
{
	m_nKeyNum = UNDERLYING_KEY_NUM;
	m_hCancel = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CMasterOptions::~CMasterOptions()
{
	Detach();
	if(m_hCancel)
	{
		CloseHandle(m_hCancel);
		m_hCancel = NULL;
	}
}

long CMasterOptions::Attach()
{
	long nResult = CDatabase<DBA_MASTER_OPTION>::Attach();

	if(nResult == DBA_ERR_NO_ERROR)
		_LoadMonthlyExpirationsDays();

	return nResult;
}

void CMasterOptions::LoadOptionFundamentalInfo(BSTR bsUnderlying)
{
	_SetUndDbKey(bsUnderlying);
	long nResult = GetEqual();

	if(nResult == DBA_ERR_NO_ERROR)
	{
		_FillUnderlyingInfo();
	}
}

void CMasterOptions::EnumOptionableStocks()
{
	_SetDbKeyToFirstUnd();
	
	long nResult = GetHigher();

	if(nResult == DBA_ERR_NO_ERROR)
	{
		while(nResult == DBA_ERR_NO_ERROR && m_dbaRec.masterOption.rec1111.key.dataType == DATA_TYPE_OPTION_CLASS )
		{
			if(_CheckCanceled())
				return;

			if(_FillUnderlyingInfo())			
				PublishUnderlyingLoaded(_bstr_t(m_clCurrentUnderlying.m_sSymbol.c_str()));
			
			nResult = GetNext();
		}
		PublishUnderlyingLoaded(NULL);
	}
	else
	{
		_bstr_t bs =  "Couldn't get master option info. Error: ";
		bs += EtGetMessage(DBA_ERROR,nResult);
		TCHAR buffer[0x100] = {0};				
		_ltot(nResult,buffer,10);
		bs += " (";
		bs += buffer;
		bs += ")";
		PublicGetMasterOptionError(enProviderInternalError, bs);
	}
}

bool  CMasterOptions::_FillUnderlyingInfo()
{
	if(	m_dbaRec.masterOption.rec1111.key.dataType == DATA_TYPE_OPTION_CLASS && 
		m_dbaRec.masterOption.rec1111.key.recordCategory == REC_CAT_CLASS_HEADER &&
		(m_dbaRec.masterOption.rec1111.key.classType == CLASS_TYPE_EQUITY ||
		 m_dbaRec.masterOption.rec1111.key.classType == CLASS_TYPE_INDEX ||
		 m_dbaRec.masterOption.rec1111.key.classType == CLASS_TYPE_INDEX_OPEN_SETTLED ) 
	  )
	{
		char szUnderlying[MAX_TICKER + 1];
		ZeroMemory(szUnderlying,sizeof(szUnderlying));
		CreateSz(szUnderlying, (const char *)m_dbaRec.masterOption.symDef.key.symbol, MAX_TICKER);

		m_clCurrentUnderlying.m_sSymbol = szUnderlying;
		m_clCurrentUnderlying.m_mapRoots.clear();

		long nClasses = m_dbaRec.masterOption.classHd.numSymbols;
		for(USHORT i = 0; i < nClasses; i++)
		{
			if(GetNext() == DBA_ERR_EOF)
				break;

			if(	m_dbaRec.masterOption.rec1111.key.dataType == DATA_TYPE_OPTION_CLASS )
			{
				char szRoot[MAX_TICKER + 1];
				ZeroMemory(szRoot,sizeof(szRoot));
				CreateSz(szRoot, (const char *)m_dbaRec.masterOption.symDef.key.tickerSymbol, MAX_TICKER);

				COptionRootInfo clRoot;					
				clRoot.m_sSymbol = szRoot;
				clRoot.m_bIsMonthlyExpirationCalendar = (m_dbaRec.masterOption.symDef.expCalendarCat == CLNDR_CAT_EQUITY && m_dbaRec.masterOption.symDef.expCalendarType == CLNDR_TYPE_MONTHLY_EXP);
				clRoot.m_dCash = Price2Dbl(m_dbaRec.masterOption.symDef.strikePrcType, m_dbaRec.masterOption.symDef.cashSettlement);
				clRoot.m_enExerciseType = m_dbaRec.masterOption.symDef.exerciseStyle == 1 ? enOetEuropean : enOetAmerican;
				clRoot.m_nAdditionalUnderlyings = m_dbaRec.masterOption.symDef.numAddUnd;
				clRoot.m_nMultiplier = m_dbaRec.masterOption.symDef.contractSize;
				clRoot.m_nSPC = m_dbaRec.masterOption.symDef.primaryShares;
				_GetExchangesList(m_dbaRec.masterOption.symDef, clRoot.m_sExchanges);
				
				clRoot.m_CyclesMap.clear();
				SYMBOL_DEF &symbdef = m_dbaRec.masterOption.symDef;
				for(int j = 0; j < sizeof(symbdef.cycleTable)/sizeof(symbdef.cycleTable[0]); ++j)
				{
					if (symbdef.cycleTable[j].expMonth)
					{
						COptionRootInfo::cycle_t rCycle;
						rCycle.expMonth = symbdef.cycleTable[j].expMonth;
						rCycle.expYear = symbdef.cycleTable[j].expYear;

						clRoot.m_CyclesMap.insert(map<CHAR, COptionRootInfo::cycle_t>::value_type(rCycle.expMonth + 'A' - 1, rCycle));
					}
					else
						break;
				}

				clRoot.m_StrikesMap.clear();
				clRoot.m_usStrikePriceType = symbdef.strikePrcType;
				for(j = 0; j < sizeof(symbdef.strikeTable)/sizeof(symbdef.strikeTable[0]); ++j)
				{
					clRoot.m_StrikesMap[j + 'A'] = symbdef.strikeTable[j];
					clRoot.m_StrikesMap.insert(map<CHAR, LONG>::value_type(j + 'A', symbdef.strikeTable[j]));
				}

				if(m_clCurrentUnderlying.m_mapRoots.find(clRoot.m_sSymbol) == m_clCurrentUnderlying.m_mapRoots.end())
					m_clCurrentUnderlying.m_mapRoots.insert(optionrootmap::value_type(clRoot.m_sSymbol, clRoot));
			}
		}
		return true;
	}
	return false;
}

void CMasterOptions::_SetUndDbKey(BSTR bsUnderlying)
{
	m_nKeyNum = UNDERLYING_KEY_NUM;
	m_dbaKey.moKey.underlyingKey.countryCode[0] = 'U';
	m_dbaKey.moKey.underlyingKey.countryCode[1] = 'S';	
	m_dbaKey.moKey.underlyingKey.dataType = 0;
	m_dbaKey.moKey.underlyingKey.itemType = 0;	
	CreateSpaced((char *)m_dbaKey.moKey.underlyingKey.symbol, bsUnderlying, MAX_TICKER);
}

void CMasterOptions::_SetDbKeyToFirstUnd()
{
	m_nKeyNum = PRIMARY_KEY_NUM;	
	CreateSpaced((char *)m_dbaKey.moKey.primaryKey.countryCode, L"US", MAX_COUNTRY);
	m_dbaKey.moKey.primaryKey.dataType = DATA_TYPE_OPTION_CLASS;
	m_dbaKey.moKey.primaryKey.itemType = IT_EQUITY;
	m_dbaKey.moKey.primaryKey.recordCategory = REC_CAT_CLASS_HEADER;
	m_dbaKey.moKey.primaryKey.recordSequence = 0;
	memset(m_dbaKey.moKey.primaryKey.symbol,' ', MAX_TICKER);
	memset(m_dbaKey.moKey.primaryKey.tickerSymbol,' ', MAX_TICKER);
}

void CMasterOptions::_SetCalendarDbKey()
{
	m_nKeyNum = PRIMARY_KEY_NUM;	
	ZeroMemory(&m_dbaKey.moKey.calendarKey, sizeof(m_dbaKey.moKey.calendarKey));
	CreateSpaced((char *)m_dbaKey.moKey.calendarKey.countryCode, L"US", MAX_COUNTRY);
	m_dbaKey.moKey.calendarKey.calendarCategory = CLNDR_CAT_EQUITY;
	m_dbaKey.moKey.calendarKey.calendarSequence = 0;
	m_dbaKey.moKey.calendarKey.calendarType = CLNDR_TYPE_MONTHLY_EXP;
	m_dbaKey.moKey.calendarKey.dataType = DATA_TYPE_CALENDAR;
	FillMemory(m_dbaKey.moKey.calendarKey.unused1, MAX_TICKER, ' ');
	FillMemory(m_dbaKey.moKey.calendarKey.unused2, MAX_TICKER, ' ');
}

DATE CMasterOptions::_CorrectExpiryDate(DATE dtExpiryDate)
{
	COleDateTime odtExpiry(dtExpiryDate);
	int nYear = odtExpiry.GetYear();
	int nMonth = odtExpiry.GetMonth();
	long nExpiryId = (nYear - 2000L) * 12L + nMonth;

	expirationsday::iterator iter = m_mapExpiryDays.find(nExpiryId);
	if(iter != m_mapExpiryDays.end())
		odtExpiry.SetDate(nYear, nMonth, iter->second);

	return (DATE)odtExpiry;
} 

void CMasterOptions::_LoadMonthlyExpirationsDays()
{
	_SetCalendarDbKey();
	m_mapExpiryDays.clear();

	long nResult = GetEqual();
	if(nResult == DBA_ERR_NO_ERROR)
	{		

		for(long nIdx = 0; nIdx < MAX_CALENDARS; nIdx++)
		{
			DATE_1111& date1111 = m_dbaRec.masterOption.calendar.calendar.calendarDate[nIdx];

			if (date1111.century > 0)
			{				
				long nExpiryId = (date1111.century - 20L) * 1200L + date1111.year * 12L + date1111.month;
				m_mapExpiryDays.insert(expirationsday::value_type(nExpiryId, date1111.day));
			}
		}
	}
}

void CMasterOptions::_GetExchangesList(const SYMBOL_DEF& recOption, string& strExchanges)
{
	for(long nIdx = 0; nIdx < 8; nIdx++)
	{
		if(recOption.exchangeTable.exchCode[nIdx][0] && recOption.exchangeTable.exchCode[nIdx][0] != ' ')
		{
			strExchanges += recOption.exchangeTable.exchCode[nIdx][0];
		}
	}
}

void CMasterOptions::CancelRequest()
{
	SetEvent(m_hCancel);
}
bool CMasterOptions::_CheckCanceled()
{
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__ , _T("Waiting at CheckCanceled Start"));	
	bool bRes = (WaitForSingleObject(m_hCancel, 0) == WAIT_OBJECT_0);
	EgLib::CEgLibTraceManager::Trace(LogCustom, __FUNCTION__ , _T("Waiting at CheckCanceled End"));	
	return bRes;
}

void CMasterOptions::CopyFundamentalInfoToInfo(_OptionInfo& Info)
{
	_bstr_t bsSymbol(Info->Series);
	string sRoot(bsSymbol);
	sRoot = sRoot.substr(0, sRoot.length() - 2);

	if(m_clCurrentUnderlying.m_mapRoots.find(sRoot) != m_clCurrentUnderlying.m_mapRoots.end())
	{
		const COptionRootInfo& clRoot = m_clCurrentUnderlying.m_mapRoots[sRoot];
		Info->LotSize = clRoot.m_nMultiplier;
		if(clRoot.m_bIsMonthlyExpirationCalendar)
			Info->ExpirationDate = _CorrectExpiryDate(Info->ExpirationDate);
	}
	else
	{
		Info->LotSize = 100L;
	}
}

void CMasterOptions::CopyFundamentalInfoToResult(_OptionResults& Result)
{
	_bstr_t bsSymbol(Result->Series);
	string sRoot(bsSymbol);
	sRoot = sRoot.substr(0, sRoot.length() - 2);

	if(m_clCurrentUnderlying.m_mapRoots.find(sRoot) != m_clCurrentUnderlying.m_mapRoots.end())
	{
		const COptionRootInfo& clRoot = m_clCurrentUnderlying.m_mapRoots[sRoot];
		Result->LotSize = clRoot.m_nMultiplier;
		if(clRoot.m_bIsMonthlyExpirationCalendar)
			Result->ExpirationDate = _CorrectExpiryDate(Result->ExpirationDate);
	}
	else
	{
		Result->LotSize = 100L;
	}
}

void CMasterOptions::CopyFundamentalInfoToResult(_OptionResultsEx& Result)
{
	_bstr_t bsSymbol(Result->Series);
	string sRoot(bsSymbol);
	sRoot = sRoot.substr(0, sRoot.length() - 2);

	if(m_clCurrentUnderlying.m_mapRoots.find(sRoot) != m_clCurrentUnderlying.m_mapRoots.end())
	{
		const COptionRootInfo& clRoot = m_clCurrentUnderlying.m_mapRoots[sRoot];
		Result->LotSize = clRoot.m_nMultiplier;
		if(clRoot.m_bIsMonthlyExpirationCalendar)
			Result->ExpirationDate = _CorrectExpiryDate(Result->ExpirationDate);
	}
	else
	{
		Result->LotSize = 100L;
	}
}

void CMasterOptions::CopyFundamentalInfoToIvResult(_OptionForIvResult& Result)
{
	_bstr_t bsSymbol(Result->Ticker);
	string sSymbol(bsSymbol);
	string sRoot(sSymbol.substr(0, sSymbol.length() - 2));

	optionrootmap::const_iterator iter = m_clCurrentUnderlying.m_mapRoots.find(sRoot);
	if( iter != m_clCurrentUnderlying.m_mapRoots.end())
	{
		const COptionRootInfo& clRoot = iter->second;
		Result->Multiplier = clRoot.m_nMultiplier;
		Result->SPC = clRoot.m_nSPC;
		Result->Cash = clRoot.m_dCash;
		Result->ExersiseType = clRoot.m_enExerciseType;
		Result->Exchanges = _bstr_t(clRoot.m_sExchanges.c_str()).copy();		
		Result->DeliveryType = _GetDeliveryType(clRoot);
		Result->Strike     = GetStrike(sSymbol);

		if(IsCallOption(sSymbol))
			Result->OptionType = enCall;
		else
			Result->OptionType = enPut;

		/*if(clRoot.m_bIsMonthlyExpirationCalendar)
			Result->ExpirationDate = _CorrectExpiryDate(Result->ExpirationDate);*/
	}
	else
	{
		Result->Multiplier = 100L;
		Result->SPC = 100L;
		Result->Cash = .0;
		Result->ExersiseType = enOetAmerican;
		Result->Exchanges = _bstr_t(L"").copy();		
		Result->DeliveryType = enOdGeneral;
	}
}

OptionDeliveryType CMasterOptions::_GetDeliveryType(const COptionRootInfo& clRoot)
{	
	if(EgLib::EgNumeric::EQZero(clRoot.m_dCash) && clRoot.m_nMultiplier == 100L && clRoot.m_nSPC == 100L && !clRoot.m_nAdditionalUnderlyings)
		return enOdGeneral;
	else if(EgLib::EgNumeric::EQZero(clRoot.m_dCash) && !clRoot.m_nAdditionalUnderlyings)
		return enOdSingleStock;
	else if(!clRoot.m_nAdditionalUnderlyings)
		return enOdSingleWithCash;
	else if(clRoot.m_nAdditionalUnderlyings == 1)
		return enOdTwoStock;
	else
		return enOdThreeOrMoreStock;
}

void CMasterOptions::RetrieveTheoOptListForCurUnderlying(optionlist &OptList)
{
	optionrootmap::iterator iter;
	string sSymbol;

	for (iter = m_clCurrentUnderlying.m_mapRoots.begin();
		 iter != m_clCurrentUnderlying.m_mapRoots.end();
		 iter++)
	{
		const COptionRootInfo& clRoot = iter->second;
		string sRoot = iter->first;
		char chExpiration;
		
		map<CHAR, COptionRootInfo::cycle_t>::const_iterator iter = clRoot.m_CyclesMap.begin();
		for (; iter != clRoot.m_CyclesMap.end(); iter++)
		{
			for (char chStrike = 'A'; chStrike <= 'Z'; ++chStrike)
			{
				chExpiration = iter->first;
				sSymbol = sRoot;
				sSymbol += chExpiration;
				sSymbol += chStrike;
				OptList.push_back(_bstr_t(sSymbol.c_str()));

				chExpiration += 12;
				sSymbol = sRoot;
				sSymbol += chExpiration;
				sSymbol += chStrike;
				OptList.push_back(_bstr_t(sSymbol.c_str()));
			}
		}
    }
}

/*DATE CMasterOptions::_CreateExpDate(INT nMonth, INT nYear)
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

DATE CMasterOptions::GetExpDate(const _bstr_t &bsSymbol)
{
	string sRoot(bsSymbol);
	string sRootPre  = sRoot.substr(0, sRoot.length() - 2);
	string sRootPost = sRoot.substr(sRoot.length() - 2, sRoot.length());

	optionrootmap::const_iterator iter = m_clCurrentUnderlying.m_mapRoots.find(sRootPre);
	if(iter != m_clCurrentUnderlying.m_mapRoots.end())
	{
		const COptionRootInfo& clRoot = iter->second;
		
		CHAR chExpiration = sRootPost[0];
		if (sRootPost[0] >= 'M')
			chExpiration -= 12;

		map<CHAR, COptionRootInfo::cycle_t>::const_iterator iter = clRoot.m_CyclesMap.find(chExpiration);
		if(iter != clRoot.m_CyclesMap.end())
		{
			COptionRootInfo::cycle_t cycle = iter->second;
		}
	}

	return 0.;
}*/

double CMasterOptions::GetStrike(const string &sSymbol)
{
	string sRootPre  = sSymbol.substr(0, sSymbol.length() - 2);
	string sRootPost = sSymbol.substr(sSymbol.length() - 2, sSymbol.length());

	optionrootmap::const_iterator iter = m_clCurrentUnderlying.m_mapRoots.find(sRootPre);
	if(iter != m_clCurrentUnderlying.m_mapRoots.end())
	{
		const COptionRootInfo& clRoot = iter->second;

		map<CHAR, LONG>::const_iterator iter = clRoot.m_StrikesMap.find(sRootPost[1]);
		if (iter != clRoot.m_StrikesMap.end())
			return Price2Dbl(clRoot.m_usStrikePriceType, iter->second);
	}

	return .0;
} 

bool CMasterOptions::IsCallOption(const string &sSymbol)
{
	if (sSymbol.size() < 2)
		return false;

	char chExpiration = sSymbol[sSymbol.size() - 2];
	if (chExpiration >= 'A' && chExpiration <= 'L')
		return true;
	return false;
}
