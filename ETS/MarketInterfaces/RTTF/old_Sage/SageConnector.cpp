#include "stdafx.h"

#include "SageConnector.h"
#include "FixTradeConst.h"
#include "Publisher.h"

DWORD __stdcall CSageConnector::Start(void* pParam)
{
	CTracer::Trace(_T("Starting publisher..."));

	DWORD dwErr = m_Publisher.Start(pParam);

	if(dwErr)
		return CTracer::TraceError(dwErr);

	CTracer::Trace(_T("Publisher started."));

	return CFixConnector::Start(pParam);
};

DWORD __stdcall CSageConnector::Stop()
{
	DWORD dwErr = CFixConnector::Stop();
	if(dwErr)
	{
		CTracer::TraceError();
	}

	Sleep(1000);

	CTracer::Trace(_T("Stopping publisher..."));

	dwErr = m_Publisher.Stop();

	CTracer::Trace(_T("Publisher stopped."));

	if(dwErr)
		return CTracer::TraceError(dwErr);

	return 0;
}

bool CSageConnector::OnApplicationMessage(CFixMessage& Msg)
{
	if(Msg.GetType() != enFmtExecutionReport)
	{
		CTracer::Trace(_T("WARNING. Unexpected message type for application layer = %c"), Msg.GetType());
		return true;
	}

#ifdef _DEBUG
		string Text;
		Text.assign((char*)Msg.GetData(), Msg.GetLength());
//		CTracer::Trace(Text.c_str());
#endif

	CFixTagValue	Value;
	CTrade			Trade;

	static TCHAR	szBuf[256];

	try
	{
		//-----------------[Data for all trades]-------------------------------------------
		// ExecTransType
		if (!Msg.GetTagValue(enFtExecTransType, Value) || !Value.GetChar(Trade.cExecTransType))
			throw _T("Failed to get value of ExecTransType field.");

		if(Trade.cExecTransType != ETT_NEW && Trade.cExecTransType != ETT_CANCEL)
		{
			_stprintf(szBuf, _T("ExecTransType = '%c'. Message ignored."), Trade.cExecTransType);
			throw szBuf;
		}

		// OrderStatus
		if (!Msg.GetTagValue(enFtOrdStatus, Value) || !Value.GetChar(Trade.cOrderStatus))
			throw _T("Failed to get value of OrdStatus field.");

		if(Trade.cOrderStatus != OS_PARTIALLY_FILLED && Trade.cOrderStatus != OS_FILLED)
		{
			_stprintf(szBuf, _T("OrdStatus = '%c'. Message ignored."), Trade.cOrderStatus);
			throw szBuf;
		}

		// Acount
		if (!Msg.GetTagValue(enFtAccount, Value) || !Value.GetString(Trade.sAccount))
			throw _T("Failed to get value of Account field.");
/*
		// OrderID - unique for chain
		if (!Msg.GetTagValue(enFtOrderID, Value) || !Value.GetString(Trade.sOrderID))
			throw "Failed to get value of OrderID field.";
*/
		// ExecID - not unique for chain
		if (!Msg.GetTagValue(enFtExecID, Value) || !Value.GetString(Trade.sExecID))
			throw _T("Failed to get value of ExecID field.");

		{
		    CTracer::Trace(_T("Trade received ExecID = '%s'"), Trade.sExecID.c_str());
			InterlockedIncrement((LPLONG)m_Publisher.GetCounterReceived());
		}

//		assert(Trade.sOrderID == Trade.sExecID);

		// Symbol
		if (!Msg.GetTagValue(enFtSymbol, Value) || !Value.GetString(Trade.sSymbol))
			throw _T("Failed to get value of Symbol field.");

		// Side
		if (!Msg.GetTagValue(enFtSide, Value) || !Value.GetChar(Trade.cSide))
			throw _T("Failed to get value of Side field.");

		if(Trade.cSide != SIDE_BUY && Trade.cSide != SIDE_SELL)
		{
			_stprintf(szBuf, _T("Size = '%c'. Message ignored."), Trade.cSide);
			throw szBuf;
		}

		
		// Quantity of shares bought/sold on this fill - where is lot size?
		if (!Msg.GetTagValue(enFtLastShares, Value) || !Value.GetInt(Trade.iLastShares))
			throw _T("Failed to get value of LastShares field.");

		// Last price
		if (!Msg.GetTagValue(enFtLastPx, Value) || !Value.GetFloat(Trade.fLastPrice))
			throw _T("Failed to get value of LastPx field.");

		// Time of transaction - Time of execution creation
		if (!Msg.GetTagValue(enFtTransactTime, Value) || !Value.GetTime(Trade.ftTransactTime))
			throw _T("Failed to get value of TransactTime field.");

		// Trade Date - Indicates date of trade referenced
		if (!Msg.GetTagValue(enFtTradeDate, Value) || !Value.GetDate(Trade.fdTradeDate))
		{
			time_t tNow = time(NULL);
			struct tm* tmNow = gmtime(&tNow);
			Trade.fdTradeDate.tm_year = tmNow->tm_year;
			Trade.fdTradeDate.tm_mon = tmNow->tm_mon;
			Trade.fdTradeDate.tm_mday = tmNow->tm_mday;
			Trade.fdTradeDate.tm_hour = 0;
			Trade.fdTradeDate.tm_min = 0;
			Trade.fdTradeDate.tm_sec = 0;
		}

		// Security type
		if (!Msg.GetTagValue(enFtSecurityType, Value) || !Value.GetString(Trade.sSecurityType))
			throw _T("Failed to get value of SecurityType field.");

		USES_CONVERSION;

		if(Trade.sSecurityType == T2A(ST_OPTION))
		{
			if (!Msg.GetTagValue(enFtMaturityMonthYear, Value) || !Value.GetMonthYear(Trade.fmyMaturityMonthYear))
				throw _T("Failed to get value of MaturityMonthYear field.");

			if (!Msg.GetTagValue(enFtPutOrCall, Value) || !Value.GetInt(Trade.iPutOrCall))
				throw _T("Failed to get value of PutOrCall field.");

			if (!Msg.GetTagValue(enFtStrikePrice, Value) || !Value.GetFloat(Trade.fStrikePrice))
				throw _T("Failed to get value of StrikePrice field.");

			if (!Msg.GetTagValue(enFtMaturityDay, Value) || !Value.GetDayOfMonth(Trade.fmdMaturityDay))
				throw _T("Failed to get value of MaturityDay field.");

			if (!Msg.GetTagValue(enFtUnderlying, Value) || !Value.GetString(Trade.sUnderlying))
				throw _T("Failed to get value of Underlying field.");

			Trade.sRoot = Trade.sSymbol;
            Trade.MakeOptionSymbol();
		}
		else if(Trade.sSecurityType == T2A(ST_STOCK) || 
			Trade.sSecurityType == T2A(ST_MUTUAL_FOND) || 
			Trade.sSecurityType == T2A(ST_PREFERRED_STOCK))
		{
			Trade.sUnderlying = Trade.sSymbol;
		}
		else
		{
			_stprintf(szBuf, _T("Security type = '%s'. Message ignored."), Trade.sSecurityType.c_str());
			throw szBuf;
		}


		// Text

		if (Msg.GetTagValue(enFtText, Value) && Value.GetString(Trade.sText))
		{
			CTracer::Trace(_T("!!! enFtText = '%s'"), Trade.sText.c_str());

/* Uncomment Oleg
			if(strstr(Trade.sText.c_str(), "%%ASSIGNMENT%%") ||
				strstr(Trade.sText.c_str(), "%%EXERCISE%%"))
				return true;
  */

		}

		m_Publisher.Push(Trade);
	}
	catch(const _TCHAR* szText)
	{
		CTracer::Trace(_T("Error in execution report message: %s"), szText);
		
		return true;
	}

	return true;
};

/*
		Msg.DeleteTag(enFtBeginString);
		Msg.DeleteTag(enFtBodyLength);
		Msg.DeleteTag(enFtMsgType);
		Msg.DeleteTag(enFtSenderCompID);
		Msg.DeleteTag(enFtTargetCompID);
		Msg.DeleteTag(enFtMsgSeqNum);
		Msg.DeleteTag(enFtSendingTime);

		Msg.DeleteTag(enFtExecTransType);
		Msg.DeleteTag(enFtOrdStatus);
		Msg.DeleteTag(enFtAccount);
		Msg.DeleteTag(enFtOrderID);
		Msg.DeleteTag(enFtExecID);
		Msg.DeleteTag(enFtSymbol);
		Msg.DeleteTag(enFtSide);
		Msg.DeleteTag(enFtLastShares);
		Msg.DeleteTag(enFtLastPx);
		Msg.DeleteTag(enFtTransactTime);
		Msg.DeleteTag(enFtTradeDate);
		Msg.DeleteTag(enFtSecurityType);
		Msg.DeleteTag(enFtMaturityMonthYear);
		Msg.DeleteTag(enFtPutOrCall);
		Msg.DeleteTag(enFtStrikePrice);
		Msg.DeleteTag(enFtMaturityDay);
		Msg.DeleteTag(enFtUnderlying);
		Msg.DeleteTag(enFtOpenClose);
		Msg.DeleteTag(enFtReportToExch);
		Msg.DeleteTag(enFtTag375);

		Msg.DumpTags();
*/
