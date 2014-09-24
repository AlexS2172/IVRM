#ifndef __MSRATE_H__
#define __MSRATE_H__

namespace EODMS
{
/////////////////////////////////////////////////////////////////////////////
//
enum MsRateRuleTypeEnum
{
	enMsRrAlwaysUseNeutralRate		= 0,
	enMsRrRateBasedOnPosition		= 1
};

/////////////////////////////////////////////////////////////////////////////
//
class  CMsRate :  public EODCO::CEgTraceable
{
public:
	CMsRate(void)
		: m_nID(0L), m_enRule(enMsRrAlwaysUseNeutralRate), m_dPosThreshold(0.)
	{
	}

	~CMsRate(void)
	{
	}

	void AddRate(long nDTE, double dShortRate, double dLongRate, double dHTBRate);

	void ClearRates();

	double ShortRate(long nDTE)
	{
		return _InterpolateRate(nDTE, m_vecShort);
	}

	double ShortRate(long nToday, long nDate)
	{
		return _InterpolateRate(nDate - nToday, m_vecShort);
	}

	double LongRate(long nDTE)
	{
		return _InterpolateRate(nDTE, m_vecLong);
	}

	double LongRate(long nToday, long nDate)
	{
		return _InterpolateRate(nDate - nToday, m_vecLong);
	}

	double NeutralRate(long nDTE)
	{
		return _InterpolateRate(nDTE, m_vecNeutral);
	}

	double NeutralRate(long nToday, long nDate)
	{
		return _InterpolateRate(nDate - nToday, m_vecNeutral);
	}

	double HTBRate(long nDTE)
	{
		return _InterpolateRate(nDTE, m_vecHTB);
	}

	double HTBRate(long nToday, long nDate)
	{
		return _InterpolateRate(nDate - nToday, m_vecHTB);
	}

	double NeutralHTBRate(long nDTE)
	{
		return _InterpolateRate(nDTE, m_vecNeutralHTB);
	}

	double NeutralHTBRate(long nToday, long nDate)
	{
		return _InterpolateRate(nDate - nToday, m_vecNeutralHTB);
	}

protected:
	double _InterpolateRate(long nDTE, MsRateVec& vecRate);

public:
	long				m_nID;
	MsRateRuleTypeEnum	m_enRule;
	double				m_dPosThreshold;

protected:
	MsRateVec			m_vecShort;
	MsRateVec			m_vecLong;
	MsRateVec			m_vecNeutral;
	MsRateVec			m_vecHTB;
	MsRateVec			m_vecNeutralHTB;
};

typedef boost::shared_ptr<CMsRate> CMsRatePtr;
typedef std::map<long, CMsRatePtr>	CMsRateColl;
typedef std::pair<long, CMsRatePtr>	CMsRatePair;
typedef boost::shared_ptr<CMsRateColl>	CMsRateCollPtr;

};

#endif //__MSRATE_H__