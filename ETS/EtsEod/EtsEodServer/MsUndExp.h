#pragma once
#ifndef __MSUNDEXP_H__
#define __MSUNDEXP_H__

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsUndExp :  public EODCO::CEgTraceable
{
public:
	CMsUndExp(void)
		: m_nExpiryID(0L), m_bHasVolaPoints(false)
	{
	}

	~CMsUndExp(void)
	{
	}

public:
	long	m_nExpiryID;
	bool	m_bHasVolaPoints;
};

typedef boost::shared_ptr<CMsUndExp> CMsUndExpPtr;
typedef std::map<long, CMsUndExpPtr>	CMsUndExpColl;
typedef boost::shared_ptr<CMsUndExpColl>	CMsUndExpCollPtr;
};

#endif //__MSUNDEXP_H__