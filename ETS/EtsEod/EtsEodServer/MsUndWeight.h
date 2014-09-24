#ifndef __MSUNDWEIGHT_H__
#define __MSUNDWEIGHT_H__

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsUndWeight  
{
public:
	CMsUndWeight(void)
		: m_nUndID(0L), m_dWeight(0.)
	{
	}

	~CMsUndWeight(void)
	{
	}

public:
	long				m_nUndID;
	double				m_dWeight;
};

typedef boost::shared_ptr<CMsUndWeight>		CMsUndWeightPtr;
typedef std::map<long, CMsUndWeightPtr>		CMsUndWeightColl;
typedef std::pair<long, CMsUndWeightPtr>	CMsUndWeightPair;

typedef boost::shared_ptr<CMsUndWeightColl>	CMsUndWeightCollPtr;

};

#endif //__MSUNDWEIGHT_H__