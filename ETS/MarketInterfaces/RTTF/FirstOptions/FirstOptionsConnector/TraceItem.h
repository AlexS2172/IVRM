// TraceItem.h: interface for the CTraceItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACEITEM_H__2ED19D7F_ED4F_4DD9_9DF5_C9ED39A39F07__INCLUDED_)
#define AFX_TRACEITEM_H__2ED19D7F_ED4F_4DD9_9DF5_C9ED39A39F07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <fstream>
#include <iomanip>
#include <windows.h>

#include "portfolio.h"

class CTraceItem  
{
public:
	CTraceItem();
	CTraceItem(foc_rttf_trade_ex& Trade);
	virtual ~CTraceItem();

	friend std::fstream& operator << (std::fstream& Stream, const CTraceItem& ItemToWrite);

	void SetOpType(const std::string& Type);	// to set up the m_sOperation member
	void SetState(const std::string& State);

protected:
	DATE m_dtLogTime;			// when the event has been logged in
	std::string m_sState;		// state of operation. Possible values are: OK, NOTFOUND, REPROCESSFAIL
	std::string m_sOperation;	// type of operation. One of: NEW, UPDATE, DELETE
	float m_ftPrice;			// Instrument's price
	DWORD m_dwQuantity;			// Shares
	std::string m_sTradeId;		// Id that was assigned to the trade by a clearing house
	std::string m_sAccount;		// Id of order's sender
	bool m_bIsBuy;				// Buy | Sell
	std::string m_sUI;			// Underlying instrument
	bool m_bIsStock;			// Stock | Option
	DATE m_dtTransactionTime;	// Same as next field
	DATE m_dtTradeTime;			// Execution date in FOC terms
	bool m_bIsPut;				// Call | Put (has meaning only 4 options)
	float m_ftStrike;			// Strike price (4 options only)
	std::string m_sOwnSymbol;	// Instrument's name ( == m_sUI for stocks)
	DATE m_dtMaturity;			// ?
	std::string m_OptSymbol;
};

void LogError(std::fstream& Stream, 
			  const foc_rttf_trade_ex& Trade, 
			  const std::string& Descr = "");

void LogStructureChange(std::fstream& Stream, const std::string& Event);

#endif // !defined(AFX_TRACEITEM_H__2ED19D7F_ED4F_4DD9_9DF5_C9ED39A39F07__INCLUDED_)
