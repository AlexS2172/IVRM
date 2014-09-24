// Trade.h: interface for the CTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADE_H__42E57F39_B26F_4BE9_B29C_8CE55D280276__INCLUDED_)
#define AFX_TRADE_H__42E57F39_B26F_4BE9_B29C_8CE55D280276__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//using std::string;

#define NEW_FLAG 0
#define UPD_FLAG 1
#define DEL_FLAG 2

#define ACTION_BUY _bstr_t(L"B")

struct CTrade  
{
	long iSeqID;
	long nTradeID;
	_bstr_t vcAcronym;
	int siStatus;
	_bstr_t vcSymbol;
	_bstr_t vcAction;
	int tiIsBuy;
	double fUnits;
	double fPrice;
	vt_date dtTradeDate;
	_bstr_t vcHedgeGroup;
	double dStockRefPrice;
};

#endif // !defined(AFX_TRADE_H__42E57F39_B26F_4BE9_B29C_8CE55D280276__INCLUDED_)
