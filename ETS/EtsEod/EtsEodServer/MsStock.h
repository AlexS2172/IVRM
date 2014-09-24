#ifndef __MSSTOCK_H__
#define __MSSTOCK_H__

#include "MsUnd.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsStock : public CMsUnd
{
public:
	CMsStock(void)
		: m_bUseCustDivs(false), m_nDivFreq(0L), m_nDivDate(0L), m_dDivAmt(0.),
		m_nDivFreqCust(0L), m_nDivDateCust(0L), m_dDivAmtCust(0.)
	{
	}

	~CMsStock(void)
	{
	}

	SYMBOL_TYPE Type() const
	{
		return enStStock;
	}

	bool IsValidDivs(bool bCheckCust) const;

	void GetRegularDiv(bool bCust, REGULAR_DIVIDENDS& aDiv) const;

	long FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
								long nToday, long nDTE, double& dYield);

public:
	bool			m_bUseCustDivs;
	long			m_nDivFreq;
	long			m_nDivDate;
	double			m_dDivAmt;
	long			m_nDivFreqCust;
	long			m_nDivDateCust;
	double			m_dDivAmtCust;
};

typedef boost::shared_ptr<CMsStock> CMsStockPtr;
typedef std::map<CString, CMsStockPtr>	CMsStockColl;
typedef boost::shared_ptr<CMsStockColl>	CMsStockCollPtr;

};

#endif //__MSSTOCK_H__