#pragma once
#ifndef __MSEXPDATE_H__
#define __MSEXPDATE_H__

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsExpDate :  public EODCO::CEgTraceable
{
public:
	CMsExpDate(void)
		: m_dtExpiration(0.), m_dWtdVega(BAD_DOUBLE_VALUE), m_nDTE(BAD_LONG_VALUE)
	{
	}

	~CMsExpDate(void)
	{
	}

public:
	DATE	m_dtExpiration;
	long	m_nDTE;
	double	m_dWtdVega;
};

	typedef boost::shared_ptr<CMsExpDate> CMsExpDatePtr;
	typedef std::map<DATE, CMsExpDatePtr>	CMsExpDateColl;
	typedef boost::shared_ptr<CMsExpDateColl>	CMsExpDateCollPtr;
};

#endif //__MSEXPDATE_H__