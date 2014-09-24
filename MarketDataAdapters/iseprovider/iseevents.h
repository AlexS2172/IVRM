#ifndef ISE_EVENTS_H
#define ISE_EVENTS_H

#include "ISEProvider.h"

typedef enum 
{	
	_enNoRequest,

	_enRequestLastStock		= 0x00000001,
	_enRequestLastOption	= 0x00000002,
	_enRequestLastIndex		= 0x00000004,
	_enRequestLastQuote		= _enRequestLastStock | _enRequestLastOption | _enRequestLastIndex,
	
	_enRequestError			= 0x10000000,

	_enSubscribeStock		= 0x00000010,
	_enSubscribeOption		= 0x00000020,
	_enSubscribeIndex		= 0x00000040,
	_enSubscribeQuote		= _enSubscribeStock | _enSubscribeOption | _enSubscribeIndex,

	_enRequestStock			= 0x00000100,
	_enRequestOption		= 0x00000200,
	_enRequestAllStocks		= 0x00000400,

	_enRequestGroupStock	= 0x00001000,
	_enRequestGroupIndex	= 0x00002000,
	_enSubscribeGroupStock	= 0x00004000,
	_enSubscribeGroupIndex	= 0x00008000,

	_enRequestGroup			= _enRequestGroupStock | _enRequestGroupIndex,
	_enSubscribeGroup		= _enSubscribeGroupStock | _enSubscribeGroupIndex,
	
	_enSubmitQuote			= 0x00010000,
	_enQuoteTickWorsed		= 0x00020000,
	_enQuoteSteppedUp		= 0x00040000,
	_enQuoteRejected		= 0x00080000,
	_enQuoteSent			= 0x00100000,
	_enDeal					= 0x00200000,
	_enTrade				= 0x00400000,

	_enHeartbeat			= 0x01000000

} 	_EventTypeEnum;

struct CUnderlyingInfo
{
	string	m_sSymbol;
	string	m_sExchange;

	string	m_sCurrency;
	string	m_sDescription;
	long	m_lLotSize;
	long	m_lBinNum;
	TraderRoleEnum	m_TraderRole;

	unsigned char	m_uiType;
	bool	m_bLast;
};

struct COptionInfo
{
	string				m_sSeries;
	string				m_sExchange;

	double				m_dStrikePrice;
	unsigned short		m_uiExpirationDate;
	unsigned char		m_uiGroup;
	string				m_sCurrency;
	long				m_lLotSize;

	string				m_sUnderlying;
	bool				m_bLast;
};

struct CQuoteBBO
{
	double				m_dBid;
	double				m_dAsk;
	double				m_dLast;
	double				m_dClose;
	long				m_lBidSize;
	long				m_lAskSize;
	long				m_lStatus;
	string				m_sCurrency;
};

struct CISEEvent
{
	ISEInfoTypeEnum		m_Type;
	string				m_sText;
};

struct CISEDeal
{
	string				m_sSeries;
	long				m_lQuantity;
	double				m_dPrice;
	string				m_sOrderID;
	BidAskEnum			m_BidOrAsk;
};

struct CISETrade
{
	string			m_sSeries;
	long			m_lTradeNumber;
	TradeTypeEnum	m_Type;

	string			m_sOldSeries;
	long			m_lOldTradeNumber;
	TradeTypeEnum	m_OldType;

	double			m_dPrice;
	long			m_lQuantity;
	DATE			m_dtDateTime;
	string			m_sOrderID;
	bool			m_bIsBought;
	bool			m_bIsNew;
};

struct CSymbolParams
{
	string				m_sSymbol;
	string				m_sExchange;
	string				m_sParentSymbol;
	InstrumentTypeEnum	m_InstrumentType;

	bool operator! () const
	{
		return m_sSymbol.length() == 0 && m_sExchange.length() == 0 && m_sParentSymbol.length() == 0;
	}

	bool operator < (const CSymbolParams& Origin) const
	{
		int iRes = stricmp(m_sSymbol.c_str(), Origin.m_sSymbol.c_str());
		if(iRes)
			return iRes < 0;

		iRes = stricmp(m_sExchange.c_str(), Origin.m_sExchange.c_str());
		if(iRes)
			return iRes < 0;

		return stricmp(m_sParentSymbol.c_str(), Origin.m_sParentSymbol.c_str()) < 0;
	}

	void SplitExchanges(set<string>& setExchanges) const 
	{
		SplitExchanges(setExchanges, m_sExchange);
	}

	void SplitExchanges(set<string>& setExchanges, const string& sExchange) const
	{
		setExchanges.clear();

		int iCommaPos = 0;
		int iLen = sExchange.length();
		for(int iPos = 0; iPos <= iLen; iPos++)
		{
			if(sExchange[iPos] == ',' || sExchange[iPos] == 0)
			{
				string sTemp(sExchange.c_str() + iCommaPos, iPos - iCommaPos);
				const char*z = sTemp.c_str();
				setExchanges.insert(sTemp);

				iCommaPos = iPos + 1;
			}
		}
	}
};

struct CMMQuoteParams
{
	string				m_sSymbol;
	InstrumentTypeEnum	m_Type;
	BidAskEnum			m_BidOrAsk;

	bool operator! () const
	{
		return m_sSymbol.length() == 0;
	}

	bool operator < (const CMMQuoteParams& Origin) const
	{
		int iCompare = stricmp(m_sSymbol.c_str(), Origin.m_sSymbol.c_str());
		if(iCompare != 0 || m_Type == enSTK)
			return iCompare < 0;

		return m_BidOrAsk < Origin.m_BidOrAsk;
	}

};

struct CSendQuoteParams
{
	ErrorNumberEnum	m_Error;
	string			m_sOrderID;

	bool operator < (const CSendQuoteParams& Origin) const
	{
		if(m_Error != Origin.m_Error)
			return m_Error < Origin.m_Error;

		return stricmp(m_sOrderID.c_str(), Origin.m_sOrderID.c_str()) < 0;
	}
};

struct CQuoteParams
{
    string				m_sSeries;
    BidAskEnum			m_BidOrAsk;
    double				m_dPremium;
    long				m_lQuantity;
    long				m_lOpenQuantity;
    long				m_lStepUpBuffer;
	long				m_lTickLevel;
	CSendQuoteParams	m_SendResult;

	bool operator! () const
	{
		return m_sSeries.length() == 0;
	}

	bool operator < (const CQuoteParams& Origin) const
	{
		int iRes = stricmp(m_sSeries.c_str(), Origin.m_sSeries.c_str());
		if(iRes)
			return iRes < 0;

		return m_BidOrAsk < Origin.m_BidOrAsk;
	}
};

class  CISEEvents
{
public:
	
	void IseTrace(const ISEInfoTypeEnum Type, const char* szFormat, ...)
	{
		if(!szFormat)
			return;

		va_list arglist;
		va_start(arglist, szFormat);

		TCHAR szBuf[4096] = { 0 };

		_vsntprintf(szBuf, 4096, szFormat, arglist);

/*
		SYSTEMTIME st;
		::GetLocalTime(&st);

		TCHAR szTime[100];
		_stprintf(szTime, _T("[0x%04x]%-2.2i:%-2.2i:%-2.2i "), GetCurrentThreadId(), st.wHour, st.wMinute, st.wSecond);

		CAutoLock cs(&g_csLog);
*/
		OnMessage(Type, szBuf);
		EgTrace2(szBuf);
	};

protected:

	virtual void OnQuoteBBO(const CSymbolParams&		RequestParams, 
							const _EventTypeEnum		RequestType,
							const CQuoteBBO&			BBO) = 0;
	
	virtual void OnQuoteRejection(const CQuoteParams& Params, 
								  const string& sReason) = 0;
	
	virtual void OnQuoteStepUp(const CQuoteParams& Params) = 0;
	
	virtual void OnQuoteTickWorse(const CQuoteParams& Params,
								  const unsigned short uiTickWorseLevel,
								  const string& sReason) = 0;

	virtual void OnMessage(const ISEInfoTypeEnum Type, const string& sText) = 0;

	virtual void OnHeartbeat(const long lInstance) = 0;

	virtual void OnDeal(const CISEDeal& Deal) = 0;

	virtual void OnTrade(const CISETrade& Trade) = 0;

	virtual void OnConnected(const DWORD dwSessionID) = 0;

	virtual void OnDisconnected(const DWORD dwSessionID) = 0;
};

#endif