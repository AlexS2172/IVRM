
#ifndef TRADE_H
#define TRADE_H

#pragma warning(disable:4192 4786)

struct CTrade
{
	long nTradeID;
	tstring vcAcronym; //[8]
	short int siStatus;
	tstring vcSymbol; //[15]
	tstring vcAction; //[15]
	float fUnits;
	float fPrice;
	CTime dtTradeDate;
	tstring vcHedgeGroup; //[15]
};

#endif TRADE_H