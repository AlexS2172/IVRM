// TraceItem.cpp: implementation of the CTraceItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TraceItem.h"
#include "versioninfo.h"
#include <limits.h>

const char tab = '\t';

std::fstream& fmt_date(std::fstream& stream, const DATE& d)
{
	if (0 == d)
	{
		stream << std::setw(22) << "[no data]" << tab;
		return stream;
	}
	
	SYSTEMTIME TimeBuffer;
	memset(&TimeBuffer, 0, sizeof(TimeBuffer));

	VariantTimeToSystemTime(d, &TimeBuffer);

	stream.fill('0');
	stream << std::setw(2) << std::right << TimeBuffer.wDay << '/';
	stream << std::setw(2) << std::right << std::setfill('0') << TimeBuffer.wMonth << '/';
	stream << std::setw(4) << TimeBuffer.wYear << ' ';
	stream << std::setw(2) << std::right << std::setfill('0') << TimeBuffer.wHour << ':';
	stream << std::setw(2) << std::right << std::setfill('0') << TimeBuffer.wMinute << ':';
	stream << std::setw(2) << std::right << std::setfill('0') << TimeBuffer.wSecond << tab;

	stream << std::setfill(' ');

	return stream;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraceItem::CTraceItem(): 
		m_bIsBuy(false), 
		m_bIsPut(false), 
		m_bIsStock(false),
		m_dtLogTime(0), 
		m_dtMaturity(0), 
		m_dtTradeTime(0), 
		m_dtTransactionTime(0), 
		m_dwQuantity(0), 
		m_ftPrice(0), 
		m_ftStrike(0), 
		m_sAccount(""), 
		m_sOperation(""), 
		m_sOwnSymbol(""), 
		m_sState(""), 
		m_sTradeId(""), 
		m_sUI("")
{

}

CTraceItem::CTraceItem(foc_rttf_trade_ex& Trade):	
			m_bIsBuy(Trade.buy_sell == 'B'), 
			m_bIsPut(Trade.put_call == 'P'), 
			m_bIsStock(Trade.security_type[0] != '0'),
			m_dwQuantity(Trade.quantity), 
			m_ftPrice(Trade.price), 
			m_ftStrike(Trade.strike),
			m_sAccount(Trade.account), 
			m_sOwnSymbol(Trade.trading_sym),
			m_sTradeId(Trade.trade_id), 
			m_sUI(Trade.base_sym),
			m_dtTradeTime(Trade.execution_date),
			m_dtMaturity(0),
			m_OptSymbol(Trade.MakeOptionSymbol())
{
	SYSTEMTIME tmDate;
	GetLocalTime(&tmDate); // tmDate contains current datetime

	// The SystemTimeToVariantTime function returns 0 on failure
	if (0 == SystemTimeToVariantTime(&tmDate, &m_dtLogTime))
		m_dtLogTime = LONG_MIN;	// LONG_MIN determined <in limits.h>

	// FOC_DATETIME determines operator SYSTEMTIME()
	tmDate = Trade.execution_date;
	
	if (0 == SystemTimeToVariantTime(&tmDate, &m_dtTradeTime))
		m_dtTradeTime = LONG_MIN;
	
	m_dtTransactionTime = m_dtTradeTime;
}

CTraceItem::~CTraceItem()
{
}

void CTraceItem::SetOpType(const std::string& Type)
{
	m_sOperation = Type;
}

void CTraceItem::SetState(const std::string& State)
{
	m_sState = State;
}

std::fstream& operator << (std::fstream& Stream, const CTraceItem& ItemToWrite)
{
	static BYTE btRunNumber = 0;

	if (0 == btRunNumber)
	{
		CVersionInfo aVerInfo;

		std::string sVerInfo = std::string("FOC Connector version ") + aVerInfo.GetVersionAsString();
		Stream  << sVerInfo << std::endl;

		Stream	<< std::setw(22)  << std::left << "Log date" << tab
				<< std::setw(17)  << std::left << "State" << tab 
				<< std::setw(15)  << std::left << "Operation" << tab 
				<< std::setw(11)  << std::left << "Price" << tab 
				<< std::setw(11)  << std::left << "Quantity" << tab 
				<< std::setw(11)  << std::left << "TradeId" << tab 
				<< std::setw(10)  << std::left << "Account" << tab 
				<< std::setw(10)  << std::left << "Side" << tab
				<< std::setw(13)  << std::left << "Symbol/Root" << tab 
				<< std::setw(11)  << std::left << "Stock or Option" << tab
				<< std::setw(22)  << std::left << "Trans date" << tab 
				<< std::setw(22)  << std::left << "Trade date" << tab
				<< std::setw(11)  << std::left << "Put/Call" << tab 
				<< std::setw(10)  << std::left << "Strike" << tab 
				<< std::setw(11)  << std::left << "UI" << tab
				<< std::setw(22)  << std::left << "Option symbol" << std::endl;
	}

	btRunNumber = 1;

	Stream << std::setw(22);
	fmt_date(Stream, ItemToWrite.m_dtLogTime);
	Stream	<< std::setfill(' ') 
			<< std::setw(17)  << std::left << ItemToWrite.m_sState << tab 
			<< std::setw(15)  << std::left << ItemToWrite.m_sOperation << tab
			<< std::setw(11)  << std::left << std::setprecision(4) << ItemToWrite.m_ftPrice << tab
			<< std::setw(11)  << std::left << ItemToWrite.m_dwQuantity << tab
			<< std::setw(11)  << std::left << ItemToWrite.m_sTradeId << tab
			<< std::setw(10)  << std::left << ItemToWrite.m_sAccount << tab
			<< std::setw(10)  << std::left << (ItemToWrite.m_bIsBuy ? "BUY" : "SELL") << tab
			<< std::setw(13)  << std::left << ItemToWrite.m_sOwnSymbol << tab
			<< std::setw(11)  << std::left << (ItemToWrite.m_bIsStock ? "STOCK" : "OPTION") << tab;
	Stream << std::setw(22);
	fmt_date(Stream, ItemToWrite.m_dtTransactionTime);
	Stream << std::setw(22);
	fmt_date(Stream, ItemToWrite.m_dtTradeTime);

	if (!ItemToWrite.m_bIsStock)	// i.e., if option
	{
//		fmt_date(Stream, ItemToWrite.m_dtMaturity);
		Stream	<< std::setw(11) << std::left << (ItemToWrite.m_bIsPut ? "PUT" : "CALL") << tab
				<< std::setfill(' ')
				<< std::setw(10)  << std::left << std::setprecision(4) << ItemToWrite.m_ftStrike << tab
				<< std::setw(11)  << std::left << ItemToWrite.m_sUI << tab
				<< std::setw(22)  << std::left << ItemToWrite.m_OptSymbol << std::endl;
	}
	else
		Stream << std::endl;

	return Stream;
}

// ---------------------------------------------------------------------------------------

void LogError(std::fstream& Stream, const foc_rttf_trade_ex& Trade, const std::string& Descr)
{
	SYSTEMTIME tmDate;
	DATE dtErrorOccured;
	GetLocalTime(&tmDate); // tmDate contains current datetime

	// The SystemTimeToVariantTime function returns 0 on failure
	if (0 == ::SystemTimeToVariantTime(&tmDate, &dtErrorOccured))
		dtErrorOccured = LONG_MIN;	// LONG_MIN determined <in limits.h>

	fmt_date(Stream, dtErrorOccured);

	Stream	<< " Occured while attempting to " << Trade.m_sType 
			<< ". Description: " << (Descr.size() != 0 ? Descr.c_str() : "was no provided ")
			<< " Trade id: " << Trade.trade_id << std::endl;
}

void LogStructureChange(std::fstream& Stream, const std::string& Event)
{
	SYSTEMTIME tmDate;
	DATE Logged;
	GetLocalTime(&tmDate); // tmDate contains current datetime

	// The SystemTimeToVariantTime function returns 0 on failure
	if (0 == SystemTimeToVariantTime(&tmDate, &Logged))
		Logged = LONG_MIN;	// LONG_MIN determined <in limits.h>

	fmt_date(Stream, Logged);

	Stream	<< Event << std::endl;
}