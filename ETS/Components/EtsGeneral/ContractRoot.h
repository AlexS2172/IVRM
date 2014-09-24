#pragma once

#include "CommonSPtr.h"

class CContractRoot
{
public:
	CContractRoot(void);
	virtual ~CContractRoot(void);
	
	//CComBSTR	
	//getName();

	//void	
	//setName(CComBSTR Name);

	//CComBSTR	
	//getSymbol();

	//void
	//setSymbol(CComBSTR Symbol);

	//long	
	//getID();

	//void
	//setID(long ID);

	//virtual
	//ExerciseStyleEnum
	//getExerciseStyle();

	//void
	//setExrciseStyle(ExerciseStyleEnum Style);

	//virtual
	//DATE
	//getSettlementValueTime();

	//void
	//setSettlementValueTime(DATE ValueTime);

protected:
public:
	long				m_ID;
	CComBSTR			m_Symbol;
	CComBSTR			m_Name;
	ExerciseStyleEnum	m_ExerciseStyle;
	DATE				m_SettlementValueTime;
	long				m_LotSize;
	long				m_QuotationUnitID;
	CComBSTR			m_QuotationUnitName;	
	double				m_TickSize;
	double				m_TickValue;
	double				m_PriceQuotationUnit;
	long				m_QuoteFormat;
	long				m_AssetID;
};
