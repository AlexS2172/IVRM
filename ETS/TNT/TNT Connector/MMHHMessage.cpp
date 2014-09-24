#include "StdAfx.h"
#include "MMHHMessage.h"
//#include "MMHHTAdapter.h"
using namespace time_zone;

#define CHECK_RESULT(x,msg) if(!x){m_bsDescription = msg; return false;}

namespace tnt_msg
{
	struct SMMHHError
	{
		char    chMmhtErrorCodeEnum;
		LPCWSTR szDescription;
	};

	SMMHHError MMHHError[] = {
//		{' ', L"No error"},
		{ 0 , L"No error"},
		{'1', L"Sequence gap"},
		{'2', L"Disabled or not registered"},
		{'3', L"Unsupported FMMC version"},
		{'4', L"Duplicate sequence number received"},
		{'5', L"Other non-recognized error"},
		{'6', L"Line has not been logged in"},
		{'7', L"Sequence number not numeric"},
		{'A', L"Trade date not numeric or format invalid"},
		{'B', L"Trade date not current date"},
		{'C', L"Trade time not numeric or invalid format"},
		{'D', L"Trade time outside normal trade hours"},
		{'E', L"Trade time exceeds POETS system time"},
		{'F', L"Executing firm format invalid"},
		{'G', L"Executing member format invalid or not found"},
		{'H', L"Contra firm format invalid"},
		{'I', L"Contra member format invalid"},
		{'J', L"Buy\\sell not \"B\" or \"S\""},
		{'K', L"Option call\\put not \"C\" or \"P\""},
		{'L', L"Spread indicator not \"S\" or blank"},
		{'M', L"Type qualifier must be \"O\" or \"C\" or blank"},
		{'N', L"Option series invalid"},
		{'O', L"Option expiration date has value in DD"},
		{'P', L"Premium price not numeric"},
		{'Q', L"Trade quantity not numeric"},
		{'R', L"Other unrecognized trade add transaction message error"},
		{'=', NULL}
	};
}

namespace time_zone
{
	TIME_ZONE_INFORMATION CTimeZone::m_TimeZoneInfo;
	bool                  CTimeZone::m_bTimeZoneInitialized = false;


	bool CTimeZone::LocalTimeToPasificTime(SYSTEMTIME& LocalTime, SYSTEMTIME& PasificTime)
	{
		bool bRet = false;
		if(InitializeTimeZone())
		{
			SYSTEMTIME tmSystemTime;
			TIME_ZONE_INFORMATION tzCurrentTimeZone;
			::GetTimeZoneInformation(&tzCurrentTimeZone);

			if(ConvertTzSpecificLocalTimeToSystemTime(tzCurrentTimeZone, LocalTime, &tmSystemTime))
				if(SystemTimeToTzSpecificLocalTime(&m_TimeZoneInfo, &tmSystemTime, &PasificTime))
					bRet = true;
		}
		return bRet;
	}
	bool CTimeZone::PasificTimeToLocalTime(SYSTEMTIME& PasificTime, SYSTEMTIME& LocalTime)
	{
		bool bRet = false;
		if(InitializeTimeZone())
		{
			SYSTEMTIME tmSystemTime;
			if(ConvertTzSpecificLocalTimeToSystemTime(m_TimeZoneInfo, PasificTime, &tmSystemTime))
			{
				TIME_ZONE_INFORMATION tzCurrentTimeZone;
				::GetTimeZoneInformation(&tzCurrentTimeZone);
				if(SystemTimeToTzSpecificLocalTime(&tzCurrentTimeZone, &tmSystemTime, &LocalTime))
					bRet = true;
			}
		}
		return bRet;
	}

	bool CTimeZone::InitializeTimeZone()
	{
		bool bRet = false;
		if(m_bTimeZoneInitialized)
			bRet = true;
		else
		{
			ZeroMemory(&m_TimeZoneInfo, sizeof(m_TimeZoneInfo));
			eg_reg::CEgRegKey reg;
			OSVERSIONINFO osV;
			osV.dwOSVersionInfoSize = sizeof(osV);
			GetVersionEx(&osV);
			if(osV.dwPlatformId == VER_PLATFORM_WIN32_NT)
				reg.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\Pacific Standard Time"),KEY_READ);
			else
				reg.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Time Zones\\Pacific Standard Time"),KEY_READ);
			if(reg!=NULL)
			{
				struct REGTIMEZONEINFORMATION {
					long       Bias;
					long       StandardBias;
					long	   DaylightBias;
					SYSTEMTIME StandardDate;
					SYSTEMTIME DaylightDate;
				} tzI;
				ULONG ulBufferSize = sizeof(tzI);
				if(RegQueryValueEx(reg, _T("TZI"),NULL,NULL,reinterpret_cast<LPBYTE>(&tzI), &ulBufferSize)== ERROR_SUCCESS)
				{
					m_TimeZoneInfo.Bias = tzI.Bias;
					m_TimeZoneInfo.StandardBias = tzI.StandardBias;
					m_TimeZoneInfo.DaylightBias = tzI.DaylightBias;
					m_TimeZoneInfo.StandardDate = tzI.StandardDate;
					m_TimeZoneInfo.DaylightDate = tzI.DaylightDate;
					m_bTimeZoneInitialized = true;
					bRet = true;

					_bstr_t bsStdName;
					_bstr_t bsDLName;
					reg.QueryValue(bsStdName, _T("Std"));
					reg.QueryValue(bsDLName, _T("Dlt"));
					wcsncpy(m_TimeZoneInfo.DaylightName, bsDLName, 32);
					wcsncpy(m_TimeZoneInfo.StandardName, bsStdName, 32);
				}
			}
		}
		return bRet;
	}

	BOOL CTimeZone::ConvertTzSpecificLocalTimeToSystemTime(const TIME_ZONE_INFORMATION &TimeZoneInformation, const SYSTEMTIME &LocalTime, LPSYSTEMTIME lpUniversalTime )
	{
		if(lpUniversalTime == NULL)
		{
			ATLASSERT(lpUniversalTime);
			return FALSE;
		}
		try
		{
			
			//UTC = local time + bias
			LONG Bias = TimeZoneInformation.Bias;
			if(TimeZoneInformation.DaylightDate.wMonth!=0)
				Bias+=TimeZoneInformation.DaylightBias;
			if(TimeZoneInformation.StandardDate.wMonth!=0)
				Bias+=TimeZoneInformation.StandardBias;

			eg_lib::vt_date vtLocal(LocalTime);
			eg_lib::vt_date_span vtBias(0,0,Bias);
			vtLocal +=vtBias;
			vtLocal.GetSystemTime(*lpUniversalTime);
			lpUniversalTime->wMilliseconds = LocalTime.wMilliseconds;
/*#ifdef _DEBUG
			SYSTEMTIME lt;
			TIME_ZONE_INFORMATION tzi = TimeZoneInformation;
			SYSTEMTIME st = *lpUniversalTime;
			if(SystemTimeToTzSpecificLocalTime(&tzi,&st, &lt))
			{
				ATLASSERT(memcmp(&LocalTime,&lt, sizeof(lt)) == 0);
			}

#endif*/
		}catch(...)
		{
			ATLASSERT(0);
			return FALSE;
		}
		return TRUE;
	}
}


namespace tnt_msg
{
	void CHeaderData::CopyString(char* sBuffer, CStringA& strSource, int iSize)
	{
		for(int i=0; i<iSize; i++)
		{
			if(strSource[i]>='1' && strSource[i]<='9')
				sBuffer[i] = strSource[i];
			else
				sBuffer[i] = '0';				
		}
	}
	void CHeaderData::SetBodySize(DWORD dwSize)
	{
		CStringA str;
		str.Format("%03d", dwSize);
		CopyString(m_pData->m_Length, str, sizeof(m_pData->m_Length));		
	}

	void CHeaderData::SetID(LPCSTR szID)
	{
		if(szID)
		{
			size_t dw = strlen(szID);
			memcpy(m_pData->m_Id, szID, dw);
		}
	}

	void CHeaderData::SetSequenceNumber(long lSeqNumber)
	{
		m_lSequenceNumber = lSeqNumber;

		CStringA str;
		str.Format("%06d", lSeqNumber);
		CopyString(m_pData->m_SequenceNo, str, sizeof(m_pData->m_SequenceNo));		
	}

	void CHeaderData::SetCurrentTime()
	{
		SYSTEMTIME st;
		SYSTEMTIME PassificTime;
		GetLocalTime(&st);
		if(CTimeZone::LocalTimeToPasificTime(st, PassificTime))
		{
			CStringA strTime;
			strTime.Format("%02d%02d%02d", PassificTime.wHour, PassificTime.wMinute, PassificTime.wSecond);
			CopyString(m_pData->m_Time, strTime, sizeof(m_pData->m_Time));		
		}
	}


	CMMHHMessage* CAcknowlegementMessage::CreateEmpty()
	{
		return new CAcknowlegementMessage();
	}

	bool CAcknowlegementMessage::Execute(CTNTRoot* pRoot)
	{
		ATLASSERT(pRoot);
		return pRoot->OnAcknowlegement(this);
	}

	void CAcknowlegementMessage::Initialize()
	{
		memset(m_message.m_body.m_OriginalSequenceNo, '0', sizeof(m_message.m_body.m_OriginalSequenceNo));
		memset(m_message.m_body.m_MmhtErrorCodeEnum1, ' ', sizeof(m_message.m_body.m_MmhtErrorCodeEnum1));
		memset(m_message.m_body.m_MmhtErrorCodeEnum2, ' ', sizeof(m_message.m_body.m_MmhtErrorCodeEnum2));
		memset(m_message.m_body.m_MmhtErrorCodeEnum3, ' ', sizeof(m_message.m_body.m_MmhtErrorCodeEnum3));
		memset(m_message.m_body.m_ExpectSequenceNo, '0', sizeof(m_message.m_body.m_ExpectSequenceNo));
		memset(m_message.m_body.m_POEID, '0', sizeof(m_message.m_body.m_POEID));

		m_message.m_header.m_Source = 'A';
		m_message.m_header.m_MsgType = 'Z';
		strcpy(m_message.m_header.m_Version,STR_HEADER_VERSION);
		strcpy(m_message.m_header.m_Length,"028");

	}

	void CTNTMessage::Initialize()
	{
		memset(m_message.m_body.m_TradeID, ' ', sizeof(m_message.m_body.m_TradeID));
		m_message.m_body.m_BySell = ' ';
		memset(m_message.m_body.m_TradeDate, '0', sizeof(m_message.m_body.m_TradeDate));
		memset(m_message.m_body.m_TradeTime, '0', sizeof(m_message.m_body.m_TradeTime));
		memset(m_message.m_body.m_Quantity, '0', sizeof(m_message.m_body.m_Quantity));
		m_message.m_body.m_CallPut = ' ';
		memset(m_message.m_body.m_OptionSymbol, ' ', sizeof(m_message.m_body.m_OptionSymbol));
		memset(m_message.m_body.m_OptionExpirationDate, '0', sizeof(m_message.m_body.m_OptionExpirationDate));
		memset(m_message.m_body.m_StrikeDollar, '0', sizeof(m_message.m_body.m_StrikeDollar));
		memset(m_message.m_body.m_StrikeFraction, '0', sizeof(m_message.m_body.m_StrikeFraction));
		memset(m_message.m_body.m_PremiumPrice, '0', sizeof(m_message.m_body.m_PremiumPrice));
		memset(m_message.m_body.m_ExecutingFirm, ' ', sizeof(m_message.m_body.m_ExecutingFirm));
		memset(m_message.m_body.m_ExecutingMember, ' ', sizeof(m_message.m_body.m_ExecutingMember));
		memset(m_message.m_body.m_CMTA, ' ', sizeof(m_message.m_body.m_CMTA));
		m_message.m_body.m_TypeQualifier = ' ';
		memset(m_message.m_body.m_ContraMember, ' ', sizeof(m_message.m_body.m_ContraMember));
		memset(m_message.m_body.m_OptionalData, ' ', sizeof(m_message.m_body.m_OptionalData));


		if(m_bServerVersion)
		{
			m_message.m_header.m_Source = 'N';
			m_message.m_header.m_MsgType = 'A';
		}else
		{
			m_message.m_header.m_Source = 'A';
			m_message.m_header.m_MsgType = 'N';
		}

		DWORD dw = sizeof(m_message.m_body);
		strcpy(m_message.m_header.m_Version,STR_HEADER_VERSION);
		strcpy(m_message.m_header.m_Length,"100");
		memset(m_message.m_header.m_Reserved, ' ', sizeof(m_message.m_header.m_Reserved));
		GetHeader().SetCurrentTime();

	}


	bool CTNTMessage::GetMessage(CComRecPtr<TNTData>& dataRec)
	{
		bool bRet = true;
		dataRec->TradeID      = GetString(m_message.m_body.m_TradeID, sizeof(m_message.m_body.m_TradeID)).copy();
		dataRec->OptionSymbol = GetString(m_message.m_body.m_OptionSymbol, sizeof(m_message.m_body.m_OptionSymbol)).copy();
		dataRec->ExecutingFirm = GetString(m_message.m_body.m_ExecutingFirm, sizeof(m_message.m_body.m_ExecutingFirm)).copy();
		dataRec->ExecutingMember = GetString(m_message.m_body.m_ExecutingMember, sizeof(m_message.m_body.m_ExecutingMember)).copy();

		dataRec->ContraFirm = GetString(m_message.m_body.m_ContraFirm, sizeof(m_message.m_body.m_ContraFirm)).copy();
		dataRec->ContraMember = GetString(m_message.m_body.m_ContraMember, sizeof(m_message.m_body.m_ContraMember)).copy();

		dataRec->CMTA = GetString(m_message.m_body.m_CMTA, sizeof(m_message.m_body.m_CMTA)).copy();
		dataRec->OptionalData = GetString(m_message.m_body.m_OptionalData, sizeof(m_message.m_body.m_OptionalData)).copy();

		dataRec->isBuy = (m_message.m_body.m_BySell=='B' || m_message.m_body.m_BySell=='b')?VARIANT_TRUE:VARIANT_FALSE;
		dataRec->isCall = (m_message.m_body.m_CallPut=='C' || m_message.m_body.m_CallPut=='c')?VARIANT_TRUE:VARIANT_FALSE;


		dataRec->SpreadIndicator = GetString(&m_message.m_body.m_SpreadIndicator, sizeof(m_message.m_body.m_SpreadIndicator)).copy();
		dataRec->Quantifier = GetString(&m_message.m_body.m_TypeQualifier, sizeof(m_message.m_body.m_TypeQualifier)).copy();


		dataRec->TradeQuantity = atol(GetString(m_message.m_body.m_Quantity, sizeof(m_message.m_body.m_Quantity)));
		dataRec->Strike = atol(GetString(m_message.m_body.m_StrikeDollar, sizeof(m_message.m_body.m_StrikeDollar)));
		dataRec->Strike += double(atol(GetString(m_message.m_body.m_StrikeFraction, sizeof(m_message.m_body.m_StrikeFraction))))/1000000;

		dataRec->PremiumPrice = atol(GetString(m_message.m_body.m_PremiumPrice, 5));
		dataRec->PremiumPrice += double(atol(GetString(m_message.m_body.m_PremiumPrice+5, 6)))/1000000;

		SYSTEMTIME stExpiration;
		SYSTEMTIME stPasificTrade;
		SYSTEMTIME stLocalTime;

		ZeroMemory(&stPasificTrade,sizeof(stPasificTrade));
		ZeroMemory(&stExpiration,sizeof(stExpiration));
		ZeroMemory(&stLocalTime,sizeof(stLocalTime));

		stExpiration.wYear  =(WORD)atol(GetString(m_message.m_body.m_OptionExpirationDate,4));
		stExpiration.wMonth =(WORD)atol(GetString(m_message.m_body.m_OptionExpirationDate+4,2));
		stExpiration.wDay   =1; //It always ZERO.  //(WORD)atol(GetString(m_message.m_body.m_OptionExpirationDate+6,2));

		stPasificTrade.wYear = (WORD)atol(GetString(m_message.m_body.m_TradeDate,4));
		stPasificTrade.wMonth = (WORD)atol(GetString(m_message.m_body.m_TradeDate+4,2));
		stPasificTrade.wDay = (WORD)atol(GetString(m_message.m_body.m_TradeDate+6,2));
		stPasificTrade.wHour = (WORD)atol(GetString(m_message.m_body.m_TradeTime,2));
		stPasificTrade.wMinute = (WORD)atol(GetString(m_message.m_body.m_TradeTime+2,2));
		stPasificTrade.wSecond = (WORD)atol(GetString(m_message.m_body.m_TradeTime+4,2));

		CTimeZone::PasificTimeToLocalTime(stPasificTrade, stLocalTime);
		eg_lib::vt_date vtTradeDate(stLocalTime);
		eg_lib::vt_date vtExpiration(stExpiration);

		dataRec->TradeDateTime = vtTradeDate;
		dataRec->OptionExpDate = vtExpiration;
		return bRet;
	}

	bool CTNTMessage::Execute(CTNTRoot* pRoot)
	{
		ATLASSERT(pRoot);
		return pRoot->OnTNTData(this);
	}

	/*
bool CTNTMessage::SetExecutingMember(BSTR bszMember)
	{
		CHECK_RESULT(AssignString(m_message.m_body.m_ExecutingFirm,bszMember,0,sizeof(m_message.m_body.m_ExecutingFirm),false,' '),_T("The ExecutingFirm Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_ExecutingMember,bszMember,0,sizeof(m_message.m_body.m_ExecutingMember),false,' '),_T("The ExecutingMember Field is not properly set"));
		return true;
	}
*/

	bool CTNTMessage::SetMessage(const struct TNTData* pData/*, BSTR pszId*/)
	{
		m_bsDescription = _T("");
		Initialize();
		if(!pData)
		{
			m_bsDescription = _T("The TNT Data is not Set");
			return false;
		}

		_bstr_t ExecMem = pData->ExecutingMember;
		_bstr_t ContraMem = pData->ContraMember;
		
		if(ExecMem == ContraMem)
		{
			m_bsDescription = _T("Executing member can't be equal to Contra member.");
			return false;
		}

		CHECK_RESULT(AssignString(m_message.m_header.m_Id,pData->ExecutingMember,0,sizeof(m_message.m_body.m_TradeID),false,' '),_T("The Heder.ID Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_TradeID,pData->TradeID,2,sizeof(m_message.m_body.m_TradeID),true,' '),_T("The TradeID Field is not properly set"));

		CHECK_RESULT(AssignString(m_message.m_body.m_OptionSymbol,pData->OptionSymbol,1,sizeof(m_message.m_body.m_OptionSymbol),false,' '),_T("The Option Symbol Field is not properly set"));

		CHECK_RESULT(AssignString(m_message.m_body.m_ExecutingFirm,pData->ExecutingFirm,0,sizeof(m_message.m_body.m_ExecutingFirm),false,' '),_T("The ExecutingFirm Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_ExecutingMember,pData->ExecutingMember,0,sizeof(m_message.m_body.m_ExecutingMember),false,' '),_T("The ExecutingMember Field is not properly set"));

//		CHECK_RESULT(AssignString(m_message.m_body.m_ExecutingFirm,pszId,0,sizeof(m_message.m_body.m_ExecutingFirm),false,' '),_T("The ExecutingFirm Field is not properly set"));
//		CHECK_RESULT(AssignString(m_message.m_body.m_ExecutingMember,pszId,0,sizeof(m_message.m_body.m_ExecutingMember),false,' '),_T("The ExecutingMember Field is not properly set"));



		CHECK_RESULT(AssignString(m_message.m_body.m_ContraFirm,pData->ContraFirm,0,sizeof(m_message.m_body.m_ContraFirm),false,' '),_T("The ContraFirm Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_ContraMember,pData->ContraMember,0,sizeof(m_message.m_body.m_ContraMember),false,' '),_T("The ContraMember Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_CMTA,pData->CMTA,0,sizeof(m_message.m_body.m_CMTA),false,' '),_T("The CMTA Field is not properly set"));

		AssignString(m_message.m_body.m_OptionalData,pData->OptionalData,0,sizeof(m_message.m_body.m_OptionalData),true,' ');

		m_message.m_body.m_BySell           =pData->isBuy!=VARIANT_FALSE?'B':'S';
		m_message.m_body.m_CallPut          =pData->isCall!=VARIANT_FALSE?'C':'P';

		if(pData->SpreadIndicator)
			m_message.m_body.m_SpreadIndicator  =*CW2A(pData->SpreadIndicator);
		else
			m_message.m_body.m_SpreadIndicator = ' ';

		if(m_message.m_body.m_SpreadIndicator == 0)
			m_message.m_body.m_SpreadIndicator = ' ';

		if(pData->Quantifier)
			m_message.m_body.m_TypeQualifier    =*CW2A(pData->Quantifier);
		else
			m_message.m_body.m_TypeQualifier= ' ';

		if(m_message.m_body.m_TypeQualifier == 0)
			m_message.m_body.m_TypeQualifier= ' ';

		CHECK_RESULT(AssignString(m_message.m_body.m_Quantity,pData->TradeQuantity,sizeof(m_message.m_body.m_Quantity),true,'0'),_T("The TradeQuantity Field is not properly set"));


		double fStrikeFraction,fStrikeInteger;
		fStrikeFraction = modf(pData->Strike, &fStrikeInteger);

		CHECK_RESULT(AssignString(m_message.m_body.m_StrikeDollar,long(fStrikeInteger),sizeof(m_message.m_body.m_StrikeDollar),true,'0'),_T("The StrikeDollar Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_StrikeFraction,long(fStrikeFraction*1000000),sizeof(m_message.m_body.m_StrikeFraction),false,'0'),_T("The StrikeFraction Field is not properly set"));

		double fPriceFraction,fPriceInteger;
		fPriceFraction = modf(pData->PremiumPrice, &fPriceInteger);

		CHECK_RESULT(AssignString(m_message.m_body.m_PremiumPrice,long(fPriceInteger),5,true,'0'),_T("The PremiumPrice Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_PremiumPrice+5,long(fPriceFraction*1000000),6,false,'0'),_T("The Premium Pice Field fraction is not properly set"));

		eg_lib::vt_date vtTradeDate(pData->TradeDateTime);
		eg_lib::vt_date vtExpiration(pData->OptionExpDate);
		SYSTEMTIME stLocal, stPasific,stExpiration;

		vtExpiration.GetSystemTime(stExpiration);
		vtTradeDate.GetSystemTime(stLocal);

		if(!CTimeZone::LocalTimeToPasificTime(stLocal,stPasific))
		{
			m_bsDescription = _T("Unable to convert tradeTime to Pasific Time Zone time");
			return false;
		}

		CString strTradeDate;
		CString strTradeTime;
		CString strExpirationDate;
		strTradeDate.Format(_T("%04d%02d%02d"),stPasific.wYear,stPasific.wMonth,stPasific.wDay);
		strTradeTime.Format(_T("%02d%02d%02d"),stPasific.wHour,stPasific.wMinute,stPasific.wSecond);
		strExpirationDate.Format(_T("%04d%02d%02d"),stExpiration.wYear,stExpiration.wMonth,(WORD)/*stExpiration.wDay*/0); //See specs DD is always zeros.

		CHECK_RESULT(AssignString(m_message.m_body.m_TradeDate, (BSTR)_bstr_t(strTradeDate), 1, sizeof(m_message.m_body.m_TradeDate),true,'0'),_T("The TradeDateTime Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_TradeTime, (BSTR)_bstr_t(strTradeTime), 1, sizeof(m_message.m_body.m_TradeTime),true,'0'),_T("The TradeDateTime Field is not properly set"));
		CHECK_RESULT(AssignString(m_message.m_body.m_OptionExpirationDate, (BSTR)_bstr_t(strExpirationDate), 1, sizeof(m_message.m_body.m_OptionExpirationDate),true,'0'),_T("The OptionExpirationDate Field is not properly set"));
		return true;
	}


	CMMHHMessage*CTNTMessage::CreateEmpty()
	{
		return new CTNTMessage(m_bServerVersion);
	}

	CMMHHMessage* CTerminateConnectionMessage::CreateEmpty()
	{
		return new CTerminateConnectionMessage();
	}

	bool   CTerminateConnectionMessage::Execute(CTNTRoot* pRoot)
	{
		ATLASSERT(pRoot);
		return pRoot->OnTerminateConnection();
	}

	void CTerminateConnectionMessage::Initialize()
	{
		m_message.m_header.m_Source = 'A';
		m_message.m_header.m_MsgType = 'C';
		strcpy(m_message.m_header.m_Version,STR_HEADER_VERSION);
		strcpy(m_message.m_header.m_Length,"000");


		//Empty body

		return;
	}

	bool CAcknowlegementMessage::CheckError(long* plErrorCount)
	{
		bool bIsMessage1 = (*m_message.m_body.m_MmhtErrorCodeEnum1!=' ' && *m_message.m_body.m_MmhtErrorCodeEnum1!=0 && *m_message.m_body.m_MmhtErrorCodeEnum1!='1');
		bool bIsMessage2 = (*m_message.m_body.m_MmhtErrorCodeEnum2!=' ' && *m_message.m_body.m_MmhtErrorCodeEnum2!=0 && *m_message.m_body.m_MmhtErrorCodeEnum2!='1');
		bool bIsMessage3 = (*m_message.m_body.m_MmhtErrorCodeEnum3!=' ' && *m_message.m_body.m_MmhtErrorCodeEnum3!=0 && *m_message.m_body.m_MmhtErrorCodeEnum3!='1');
		if(plErrorCount)
		{
			*plErrorCount = 0;
			*plErrorCount+=bIsMessage1?1:0;
			*plErrorCount+=bIsMessage2?1:0;
			*plErrorCount+=bIsMessage3?1:0;       
		}
		return (bIsMessage1 || bIsMessage2 || bIsMessage3);
	}

	void CAcknowlegementMessage::GetError(long lPosition,MmhtErrorCodeEnum* penCode, _bstr_t* pbsDescription)
	{
		*penCode = GetMmhtErrorCodeEnum(lPosition);
		*pbsDescription = L"Unrecognized Error";
		int i=0;
		while(MMHHError[i].szDescription != NULL)
		{
			if(*penCode  == MMHHError[i].chMmhtErrorCodeEnum)
			{

				//bsCode = MMHHError[i].chMmhtErrorCodeEnum;
				*pbsDescription = MMHHError[i].szDescription;
				break;
			}
			i++;
		}
	}

	long CAcknowlegementMessage::GetOriginalSequenceNumber()
	{
		CStringA str;
		char* szStr = str.GetBufferSetLength(sizeof(GetBody().m_OriginalSequenceNo)+1);
		memcpy(szStr,GetBody().m_OriginalSequenceNo,sizeof(GetBody().m_OriginalSequenceNo));
		szStr[sizeof(GetBody().m_OriginalSequenceNo)] = 0;
		str.ReleaseBuffer();
		return atol(str);
	}

	_bstr_t CAcknowlegementMessage::GetPOEID()
	{
		return GetString(m_message.m_body.m_POEID,sizeof(m_message.m_body.m_POEID));
	}

	MmhtErrorCodeEnum CAcknowlegementMessage::GetMmhtErrorCodeEnum(long lPosition)
	{
		char cRet = ' ';
		switch(lPosition)
		{
		case 0:
			cRet = *m_message.m_body.m_MmhtErrorCodeEnum1;
			break;
		case 1:
			cRet = *m_message.m_body.m_MmhtErrorCodeEnum2;
			break;
		case 2:
			cRet = *m_message.m_body.m_MmhtErrorCodeEnum3;
			break;
		default: 
			break;
		}
		return cRet == ' '? enMmhtErNoError: (MmhtErrorCodeEnum)cRet; 
	}
	long CAcknowlegementMessage::GetMessagesCount()
	{
		long lCount = 0;
		CheckError(&lCount);
		if(!lCount)
			lCount++;
		return lCount;
	}

	HRESULT CAcknowlegementMessage::GetErrorDescriptions(LPSAFEARRAY* pSA)
	{
		ATLASSERT(pSA);
		HRESULT hr = S_OK;
		try
		{
			CComRecPtr<ErrorDescription> errDesc;
			errDesc.Init();

			long lMessages = GetMessagesCount();
			SAFEARRAYBOUND sab;
			sab.cElements = lMessages;
			sab.lLbound = 0;

			LPSAFEARRAY  pArray = ::SafeArrayCreateEx(VT_RECORD,1,&sab,errDesc.GetRecordInfo());
			if(pArray)
			{
				::SafeArrayLock(pArray);
				LPVOID pData = NULL;
				hr = ::SafeArrayAccessData(pArray, &pData);
				if(SUCCEEDED(hr))
				{
					ErrorDescription* pDesc = reinterpret_cast<ErrorDescription*>(pData);
					for(long l=0;l<lMessages;l++)
					{
						CComRecPtr<ErrorDescription> Descript;
						Descript.Init();
						MmhtErrorCodeEnum enCode;
						_bstr_t bsDescription;
						GetError(l,&enCode,&bsDescription);
						errDesc->ErrCode      = enCode;
						errDesc->Description  =bsDescription.copy();
						pDesc[l].Description = SysAllocString(errDesc->Description);
						pDesc[l].ErrCode = errDesc->ErrCode;
					}
				}
				hr = ::SafeArrayUnaccessData(pArray);
				::SafeArrayUnlock(pArray);
			}
			*pSA = pArray;
		}catch(...)
		{
			ATLASSERT(0);
		}
		return hr;
	}
}

