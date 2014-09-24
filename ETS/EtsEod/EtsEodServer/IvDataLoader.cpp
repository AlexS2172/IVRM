#include "StdAfx.h"
#include "IvDataLoader.h"

namespace EODIV
{

bool StrToDouble(const CEgString& strValue, double& dValue)
{
	if(strValue.GetLength() <= 0)
		return false;

	LPTSTR pBuf = NULL;
	LPCTSTR pLastPtr = strValue.GetString() + strValue.GetLength();
	dValue = _tcstod(strValue, &pBuf);
	return (pBuf == pLastPtr);
}

bool StrToLong(const CEgString& strValue, long& nValue)
{
	if(strValue.GetLength() <= 0)
		return false;

	LPTSTR pBuf = NULL;
	LPCTSTR pLastPtr = strValue.GetString() + strValue.GetLength();
	nValue = _tcstol(strValue, &pBuf, 10);
	return (pBuf == pLastPtr);
}

/////////////////////////////////////////////////////////////////////////////
//
void CIVRequestBase::Init(EODSM::CSmSettingsMgrPtr	spSettingsMgr, EODMS::CMsUndCollPtr pcollUnd, EODMS::CMsIndexCollPtr pcollIndex)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	m_spSettingsMgr = spSettingsMgr;
	m_collUnd = pcollUnd;
	m_collIndex = pcollIndex;
	m_iterNextUnd = m_collUnd->begin();
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

CString CIVRequestBase::GetContracts(long nCount)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if(!m_collUnd || !m_collUnd->size())
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Empty underlying list."));
		throw logic_error(_T("Empty underlying list."));
	}
	if(m_iterNextUnd == m_collUnd->end())
		throw length_error(_T("Request on all underlyings already send to IV"));

	long nUnd = 0L;

	CString sContracts = _T("&SA=");

	if(m_collIndex && m_collIndex->size())
	{

		EODMS::CMsIndexColl::iterator iterIndex = m_collIndex->begin();
		while(iterIndex != m_collIndex->end())
		{
			EODMS::CMsIndexPtr spIndex = iterIndex->second;
			
			sContracts += _GetUndSymbol(spIndex);

			iterIndex++;

			if(iterIndex != m_collIndex->end())
				sContracts += _T(",");
		}
		sContracts += _T(";");
	}

	while(m_iterNextUnd != m_collUnd->end())
	{
		EODMS::CMsUndPtr spUnd = m_iterNextUnd->second;
		
		sContracts += _GetUndSymbol(spUnd);

		m_iterNextUnd++;
		nUnd++;
		m_nUndSend++;

		if(m_iterNextUnd == m_collUnd->end() || nUnd >= nCount)
			break;
		else sContracts += _T(",");
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sContracts;
}

CString	CIVRequestBase::_GetUndSymbol(EODMS::CMsUndPtr pUnd)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	CString sSymbol;
	EODMS::CMsUndPtr spUnd(pUnd);
	
	ATLASSERT(spUnd);

	sSymbol = spUnd->m_sSymbol;
	//Remove bad symbols: ,./\	
	sSymbol.Remove(_T(','));
	sSymbol.Remove(_T('.'));
	//sSymbol.Remove(_T('/')); commented out because it isn't bad symbol
	sSymbol.Remove(_T('\\'));

	//Converts Symbol to form acceptable by IVolatility ASP's
	// AGR'A -> AGR''A
	sSymbol.Replace(_T("'"),_T("''")); 
	
	// Add ' to begin of string
	sSymbol.Insert(0,_T('\''));
	// Add ' to end of string
	sSymbol += _T("'");

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sSymbol;
}

/////////////////////////////////////////////////////////////////////////////
//

CString CIvSkewKurtosisRequest::GetURL()
{		
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CString sURL = _T("data_load/delta_hedge_HF.asp?f=EH&T=180&BM=8");
	sURL += GetContracts();
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

/////////////////////////////////////////////////////////////////////////////
//

CString CIvUndIndexBetaRequest::GetURL()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if(!m_collIndex || !m_collIndex->size())
		throw logic_error(_T("Empty Index list."));

	CString sURL = _T("data_load/delta_hedge_HF.asp?f=SY&T=252&BM=8");
	sURL += GetContracts();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CIvAvrgUndVolumesRequest::GetURL()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CString sURL = _T("data_load/delta_hedge_HF.asp?f=UV&T=252&BM=8");
	sURL += GetContracts();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CIvIvBandsRequest::GetURL()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CString sURL = _T("data_load/delta_hedge_HF.asp?f=OS&T=252&BM=8");
	sURL += GetContracts();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CIvDefaultIvRequest::GetURL()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CString sURL = _T("data_load/delta_hedge_HF.asp?f=OD&T=252&BM=8");
	sURL += GetContracts(10L);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CIvUndClosePriceRequest::GetURL()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	CString sURL = _T("data_load/delta_hedge_HF.asp?f=MTCP");

	sURL += GetContracts();

	COleDateTime odtToday = COleDateTime::GetCurrentTime();
	sURL += odtToday.Format(_T("&D=%Y-%m-%d"));

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CIvOptClosePriceRequest::GetURL()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if(m_iterNextUnd == m_collUnd->end())
		throw length_error(_T("Request on all underlyings already send to IV"));

	EODMS::CMsUndPtr spUnd(m_iterNextUnd->second);
	ATLASSERT(spUnd);

	CString sURL = _T("data_load/delta_hedge_HF.asp?f=MTOCP");

	sURL += GetContracts(1L); // Get underlyings one by one !

	sURL += _GetOptionsRangesString(spUnd);
	COleDateTime odtToday = COleDateTime::GetCurrentTime();
	sURL += odtToday.Format(_T("&D=%Y-%m-%d"));

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

CString CIvOptClosePriceRequest::_GetOptionsRangesString(EODMS::CMsUndPtr pUnd)
{
	EODMS::CMsOptionCollPtr spOptColl(pUnd->Opt());
	ATLASSERT(spOptColl);

	DATE dtExpMin = static_cast<DATE>(BAD_DOUBLE_VALUE);
	DATE dtExpMax = static_cast<DATE>(BAD_DOUBLE_VALUE);
	double dStrikeMin = BAD_DOUBLE_VALUE;
	double dStrikeMax = BAD_DOUBLE_VALUE;

	for(EODMS::CMsOptionColl::iterator iterOpt = spOptColl->begin(); iterOpt != spOptColl->end(); iterOpt++)
	{
		EODMS::CMsOptionPtr spOpt(iterOpt->second);
		ATLASSERT(spOpt);

		if(EODCO::IsBadValue(dtExpMin) || dtExpMin > spOpt->m_dtExpiration)
		{
			dtExpMin = spOpt->m_dtExpiration;
		}

		if(EODCO::IsBadValue(dtExpMax) || dtExpMax < spOpt->m_dtExpiration)
		{
			dtExpMax = spOpt->m_dtExpiration;
		}

		if(EODCO::IsBadValue(dStrikeMin) || dStrikeMin > spOpt->m_dStrike)
		{
			dStrikeMin =  spOpt->m_dStrike;
		}

		if(EODCO::IsBadValue(dStrikeMax) || dStrikeMax < spOpt->m_dStrike)
		{
			dStrikeMax = spOpt->m_dStrike;
		}
	}

	ATLASSERT(!EODCO::IsBadValue(dStrikeMin) && !EODCO::IsBadValue(dStrikeMax) 
		&& !EODCO::IsBadValue(dtExpMin) && !EODCO::IsBadValue(dtExpMax));

	COleDateTime odtExpMin(dtExpMin);
	odtExpMin.SetDate(odtExpMin.GetYear(), odtExpMin.GetMonth(), 1);
	CString strExpMin = odtExpMin.Format(_T("%Y-%m-%d"));

	COleDateTime odtExpMax(dtExpMax);

	int nMonth = odtExpMax.GetMonth();
	if(nMonth == 12)
	{
		odtExpMax.SetDate(odtExpMax.GetYear() + 1, 1, 1);
	}
	else
	{
		odtExpMax.SetDate(odtExpMax.GetYear(), odtExpMax.GetMonth() + 1, 1);
	}
	CString strExpMax = odtExpMax.Format(_T("%Y-%m-%d"));

	CString strRange;
	strRange.Format(_T("&E1=%s&E2=%s&S1=%.04f&S2=%.04f"), strExpMin, strExpMax, dStrikeMin, dStrikeMax);

	return strRange;
}

/////////////////////////////////////////////////////////////////////////////
//
void CIvMarketStructureUpdatesRequest::Init(EODSM::CSmSettingsMgrPtr	pSettingsMgr, EODMS::CMsUndCollPtr pcollUnd, DATE dtLastUpdate)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CIVRequestBase::Init(pSettingsMgr,pcollUnd);
	m_dtLastUpdate = dtLastUpdate;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

CString CIvMarketStructureUpdatesRequest::GetURL()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CString sURL = _T("data_load/delta_hedge_HF.asp?f=VV");

	if(m_dtLastUpdate > (DATE)(BAD_DOUBLE_VALUE))
	{
		COleDateTime odtLastUpdate(m_dtLastUpdate);
		sURL += odtLastUpdate.Format(_T("&D=%Y-%m-%d"));
	}
	else
	{
		COleDateTime odtLastUpdate(DATE(0.0f));
		sURL += odtLastUpdate.Format(_T("&D=%Y-%m-%d"));
	}
	sURL += GetContracts();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return sURL;
}

/////////////////////////////////////////////////////////////////////////////
//
CIvSkewKurtosisResultPtr CIvSkewKurtosisResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if((nPos = strResult.Find(_T(':'),nPos)) == -1)
	{
		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvSkewKurtosisResultPtr();
	}	

	nPos++;
	CIvSkewKurtosisResultPtr spResult(new CIvSkewKurtosisResult);	
	
	//symbol name
	CEgString strToken = Tokenize(strResult, _T(","), nPos);		
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Skew-Kurtosis response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Skew-Kurtosis response from IVolatility"));
	}
	strToken.Trim();
	spResult->m_strUndSymbol = strToken.GetString();

	// skew
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Skew-Kurtosis response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Skew-Kurtosis response from IVolatility"));
	}
	strToken.Trim();

	double dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Skew-Kurtosis response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Skew-Kurtosis response from IVolatility"));
	}
	spResult->m_dSkew = dTemp;

	// kurtosis
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Skew-Kurtosis response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Skew-Kurtosis response from IVolatility"));
	}
	strToken.Trim();

	dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Skew-Kurtosis response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Skew-Kurtosis response from IVolatility"));
	}
	spResult->m_dKurtosis = dTemp;

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}
/////////////////////////////////////////////////////////////////////////////
//
CIvUndIndexBetaResultPtr CIvUndIndexBetaResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if((nPos = strResult.Find(_T(':'),nPos)) == -1)
	{
		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvUndIndexBetaResultPtr();
	}

	nPos++;
	CIvUndIndexBetaResultPtr spResult(new CIvUndIndexBetaResult);	

	//symbol name
	CEgString strToken = Tokenize(strResult, _T(","), nPos);		
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Underlying-Index Beta response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Underlying-Index Beta response from IVolatility"));
	}
	strToken.Trim();
	spResult->m_strUndSymbol = strToken.GetString();

	// index name
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Underlying-Index Beta response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Underlying-Index Beta response from IVolatility"));
	}
	strToken.Trim();
	spResult->m_strIndexSymbol = strToken.GetString();

	// beta
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Underlying-Index Beta response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Underlying-Index Beta response from IVolatility"));
	}
	strToken.Trim();

	double dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Underlying-Index Beta response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Underlying-Index Beta response from IVolatility"));
	}
	spResult->m_dBeta = dTemp;

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}
/////////////////////////////////////////////////////////////////////////////
//
CIvAvrgUndVolumesResultPtr CIvAvrgUndVolumesResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if((nPos = strResult.Find(_T(':'),nPos)) == -1)
	{
		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvAvrgUndVolumesResultPtr();
	}
	nPos++;
	CIvAvrgUndVolumesResultPtr spResult(new CIvAvrgUndVolumesResult);	

	//symbol name
	CEgString strToken = Tokenize(strResult, _T(","), nPos);		
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Average Underlying Volumes response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Average Underlying Volumes response from IVolatility"));
	}
	strToken.Trim();
	spResult->m_strUndSymbol = strToken.GetString();

	// volume last 10 days
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Average Underlying Volumes response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Average Underlying Volumes response from IVolatility"));
	}
	strToken.Trim();

	long nTemp = 0L;
	if(!StrToLong(strToken, nTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Average Underlying Volumes response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Average Underlying Volumes response from IVolatility"));
	}
	spResult->m_nVolume10Day = nTemp;

	// volume last 5 expiration days
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Average Underlying Volumes response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Average Underlying Volumes response from IVolatility"));
	}
	strToken.Trim();

	nTemp = 0L;
	if(!StrToLong(strToken, nTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Average Underlying Volumes response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Average Underlying Volumes response from IVolatility"));
	}
	spResult->m_nVolume5Expiration = nTemp;

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}

/////////////////////////////////////////////////////////////////////////////
//
CIvIvBandsResultPtr CIvIvBandsResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	static long nBeginUndPos(0L);
	static long nEndUndPos(0L);
	static CString sUndSymbol(_T(""));

	if(!nPos)
	{
		nBeginUndPos = 0L;
		nEndUndPos = 0L;
	}

	if((nPos = strResult.Find(_T(':'),nPos)) == -1L)
	{
		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvIvBandsResultPtr();
	}

	nPos++;

	CEgString strToken;

	if(nPos > nEndUndPos)
	{
		nBeginUndPos = strResult.Find(_T("BEGIN STOCK DATA"),nEndUndPos);
		if(nBeginUndPos == -1L)
		{
			TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return CIvIvBandsResultPtr();
		}
		nEndUndPos = strResult.Find(_T("END STOCK DATA"),nBeginUndPos);
		if(nEndUndPos == -1L)
		{
			TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return CIvIvBandsResultPtr();
		}

		//symbol name
		strToken = Tokenize(strResult, _T("\r\n"), nPos);		
		if(nPos == -1)
		{
			TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of IV bands response from IVolatility: %s"),(LPCSTR)strResult);
			throw logic_error(_T("Bad format of IV bands response from IVolatility"));
		}
		strToken.Trim();
		sUndSymbol = strToken.GetString();

		if((nPos = strResult.Find(_T(':'),nPos)) == -1L)
		{
			TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return CIvIvBandsResultPtr();
		}
		nPos++;
	}


	CIvIvBandsResultPtr spResult(new CIvIvBandsResult);	

	//symbol name
	spResult->m_strUndSymbol = sUndSymbol;

	// Strike Moneyness
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of IV bands response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of IV bands response from IVolatility"));
	}
	strToken.Trim();

	long nTemp = 0L;
	if(!StrToLong(strToken, nTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of IV bands response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of IV bands response from IVolatility"));
	}
	spResult->m_nStrikeMoneyness = nTemp;

	// Expiration Period
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of IV bands response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of IV bands response from IVolatility"));
	}
	strToken.Trim();

	nTemp = 0L;
	if(!StrToLong(strToken, nTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of IV bands response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of IV bands response from IVolatility"));
	}
	spResult->m_nExpPeriod = nTemp;

	// IV Band Low
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of IV bands response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of IV bands response from IVolatility"));
	}
	strToken.Trim();

	double dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		dTemp = BAD_DOUBLE_VALUE;
	}
	spResult->m_dIVBandLow = dTemp;

	// IV Band High
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of IV bands response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of IV bands response from IVolatility"));
	}
	strToken.Trim();

	dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		dTemp = BAD_DOUBLE_VALUE;
	}
	spResult->m_dIVBandHigh = dTemp;

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}

/////////////////////////////////////////////////////////////////////////////
//
CIvDefaultIvResultPtr CIvDefaultIvResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	static long nBeginUndPos(0L);
	static long nEndUndPos(0L);
	static CString sUndSymbol(_T(""));

	if(!nPos)
	{
		nBeginUndPos = 0L;
		nEndUndPos = 0L;
	}

	if((nPos = strResult.Find(_T(':'),nPos)) == -1L)
	{
		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvDefaultIvResultPtr();
	}
	nPos++;

	CEgString strToken;
	if(nPos > nEndUndPos)
	{
		nBeginUndPos = strResult.Find(_T("BEGIN STOCK DATA"),nEndUndPos);
		if(nBeginUndPos == -1L)
		{
			TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return CIvDefaultIvResultPtr();
		}
		nEndUndPos = strResult.Find(_T("END STOCK DATA"),nBeginUndPos);
		if(nEndUndPos == -1L)
		{
			TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return CIvDefaultIvResultPtr();
		}

		//symbol name
		strToken = Tokenize(strResult, _T("\r\n"), nPos);		
		if(nPos == -1)
		{
			TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Default IV response from IVolatility: %s"),(LPCSTR)strResult);
			throw logic_error(_T("Bad format of Default IV response from IVolatility"));
		}
		strToken.Trim();
		sUndSymbol = strToken.GetString();

		if((nPos = strResult.Find(_T(':'),nPos)) == -1L)
		{
			TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return CIvDefaultIvResultPtr();
		}
		nPos++;
	}


	CIvDefaultIvResultPtr spResult(new CIvDefaultIvResult);	

	//symbol name
	spResult->m_strUndSymbol = sUndSymbol;

	// Expiration Date
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Default IV response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Default IV response from IVolatility"));
	}
	strToken.Trim();

	long nY(-1L), nM(-1L), nD(-1L);
	_stscanf_s(strToken, _T("%4d%2d%2d"), &nY, &nM, &nD);
	if( nY < 0L || nM < 0L || nD < 0L)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Default IV response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Default IV response from IVolatility"));
	}
	COleDateTime odtDate;
	odtDate.SetDate(nY, nM, nD);

	spResult->m_dtExpiration = odtDate;

	// Strike
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Default IV response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Default IV response from IVolatility"));
	}
	strToken.Trim();

	double dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Default IV response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Default IV response from IVolatility"));
	}
	spResult->m_dStrike = dTemp;

	// Volatility
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Default IV response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Default IV response from IVolatility"));
	}
	strToken.Trim();

	dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		dTemp = BAD_DOUBLE_VALUE;
	}
	spResult->m_dVolatility = dTemp;

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}

/////////////////////////////////////////////////////////////////////////////
//
CIvMarketStructureUpdatesResultPtr CIvMarketStructureUpdatesResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if((nPos = strResult.Find(_T(':'),nPos)) == -1)
	{
		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvMarketStructureUpdatesResultPtr();
	}

	nPos++;
	CIvMarketStructureUpdatesResultPtr spResult(new CIvMarketStructureUpdatesResult);	

	//symbol name
	CEgString strToken = Tokenize(strResult, _T(","), nPos);		
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Market Structure Update response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Market Structure Updates response from IVolatility"));
	}
	strToken.Trim();
	spResult->m_strUndSymbol = strToken.GetString();

	//symbol type
	strToken = Tokenize(strResult, _T(","), nPos);		
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Market Structure Update response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Market Structure Updates response from IVolatility"));
	}
	strToken.Trim();
	//spResult->m_strUndType = strToken.GetString();

	// Last Update Date
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Market Structure Update response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Market Structure Updates response from IVolatility"));
	}
	strToken.Trim();

	if(strToken.GetLength())
	{
		long nY(-1L), nM(-1L), nD(-1L);
		_stscanf_s(strToken, _T("%4d%2d%2d"), &nY, &nM, &nD);
		if( nY < 0L || nM < 0L || nD < 0L)
		{
			TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Market Structure Update response from IVolatility: %s"),(LPCSTR)strResult);
			throw logic_error(_T("Bad format of Market Structure Updates response from IVolatility"));
		}
		COleDateTime odtDate;
		odtDate.SetDate(nY, nM, nD);

		spResult->m_dtLastUpdate = odtDate;
	}
	else spResult->m_dtLastUpdate = 0.;

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}

/////////////////////////////////////////////////////////////////////////////
//
CIvUndClosePriceResultPtr CIvUndClosePriceResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	bool bIsFirstString = (nPos == 0L);
	
	if((nPos = strResult.Find(_T(':'),nPos)) == -1)
	{
		if(bIsFirstString)
			throw exception(_T("Close prices not yet populated"));

		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvUndClosePriceResultPtr();
	}

	nPos++;
	CIvUndClosePriceResultPtr spResult(new CIvUndClosePriceResult);

	//symbol name
	CEgString strToken = Tokenize(strResult, _T(","), nPos);		
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Underlying close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Underlying close price response from IVolatility"));
	}
	strToken.Trim();
	spResult->m_strUndSymbol = strToken.GetString();


	// Close price
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Underlying close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Underlying close price response from IVolatility"));
	}
	strToken.Trim();

	double dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		dTemp = BAD_DOUBLE_VALUE;
	}
	spResult->m_dClosePrice = dTemp;

	// Date
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Market Structure Update response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Market Structure Updates response from IVolatility"));
	}
	strToken.Trim();

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}

/////////////////////////////////////////////////////////////////////////////
//
CIvOptClosePriceResultPtr CIvOptClosePriceResult::Parse(const CEgString& strResult, long& nPos)
{
	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	static CString sUndSymbol(_T(""));

	bool bIsFirstString = (nPos == 0L);

	if((nPos = strResult.Find(_T(':'),nPos)) == -1)
	{
		if(bIsFirstString)
			throw exception(_T("Close prices not yet populated"));

		TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return CIvOptClosePriceResultPtr();
	}

	nPos++;
	CIvOptClosePriceResultPtr spResult(new CIvOptClosePriceResult);

	CString strToken;
	if(bIsFirstString)
	{
		//underlying symbol
		strToken = Tokenize(strResult, _T(","), nPos);		
		if(nPos == -1)
		{
			TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
			throw logic_error(_T("Bad format of Option close price response from IVolatility"));
		}
		strToken.Trim();
		sUndSymbol = strToken;

		strToken = Tokenize(strResult, _T("\r\n"), nPos);		
		if(nPos == -1)
		{
			TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
			throw logic_error(_T("Bad format of Option close price response from IVolatility"));
		}
		strToken.Trim();

		if(strToken.IsEmpty())
		{
			TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			spResult->m_strUndSymbol = sUndSymbol;
			return spResult;
		}

		if((nPos = strResult.Find(_T(':'),nPos)) == -1L)
		{
			TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
			throw logic_error(_T("Bad format of Option close price response from IVolatility"));
		}
		nPos++;
	}

	spResult->m_strUndSymbol = sUndSymbol;

	// Root symbol
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	strToken.Trim().MakeUpper();

	spResult->m_aKey.m_sRoot = strToken;

	// Expiry Date
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	strToken.Trim().MakeUpper();

	if(strToken.GetLength())
	{
		long nY(-1L), nM(-1L), nD(-1L);
		_stscanf_s(strToken, _T("%4d%2d%2d"), &nY, &nM, &nD);
		if( nY < 0L || nM < 0L || nD < 0L)
		{
			TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
			throw logic_error(_T("Bad format of Option close price response from IVolatility"));
		}
		COleDateTime odtDate;
		odtDate.SetDate(nY, nM, nD);

		spResult->m_aKey.m_nExpiryID = EODMS::CMsContract::GetExpiryID(odtDate);
	}
	else
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}

	// Strike
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	strToken.Trim();

	double dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	spResult->m_aKey.m_dStrike = dTemp;

	// Call/Put
	strToken = Tokenize(strResult, _T(","), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	strToken.Trim().MakeUpper();

	if(strToken != _T("C") && strToken != _T("P"))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	spResult->m_aKey.m_bIsCall = (strToken == _T("C"));

	// Close price
	strToken = Tokenize(strResult, _T("\r\n"), nPos);
	if(nPos == -1)
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	strToken.Trim();

	dTemp = 0.;
	if(!StrToDouble(strToken, dTemp))
	{
		TraceStatic(EODCO::enTrLogError,__FUNCTION__,_T("Bad format of Option close price response from IVolatility: %s"),(LPCSTR)strResult);
		throw logic_error(_T("Bad format of Option close price response from IVolatility"));
	}
	spResult->m_dClosePrice = dTemp;

	TraceStatic(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spResult;
}

}