#ifndef __MSOPTION_H__
#define __MSOPTION_H__

#include "MsContract.h"
#include "MsOptionKey.h"

namespace EODMS
{
/////////////////////////////////////////////////////////////////////////////
//
class  CMsOption : public CMsContract
{
public:
	CMsOption(void)
		: m_bIsCall(true), m_dStrike(0.), m_dtExpiration(0.), m_dRate(BAD_DOUBLE_VALUE), 
		m_dPriceTheo(BAD_DOUBLE_VALUE), m_bHasPositions(false), m_bClosePriceUpdated(false),
		m_dtExpiryOV(BAD_DOUBLE_VALUE), m_dtCloseTime(0.)
	{
	}

	~CMsOption(void)
	{
	}

	SYMBOL_TYPE Type() const
	{
		return enStOption;
	}
	
public:
	bool			m_bHasPositions;
	bool			m_bIsCall;
	double			m_dStrike;
	DATE			m_dtExpiration;
	DATE			m_dtExpiryOV;
	DATE			m_dtCloseTime;
	double			m_dRate;
	double			m_dPriceTheo;
	bool			m_bClosePriceUpdated;
};

typedef boost::shared_ptr<CMsOption> CMsOptionPtr;
typedef std::map<CString, CMsOptionPtr>	CMsOptionColl;
typedef boost::shared_ptr<CMsOptionColl>	CMsOptionCollPtr;
typedef std::map<long, CMsOptionPtr>	CMsOptionByIDColl;
typedef boost::shared_ptr<CMsOptionByIDColl>	CMsOptionByIDCollPtr;
typedef std::map<CMsOptionKey, CMsOptionPtr>	CMsOptionByKeyColl;
typedef boost::shared_ptr<CMsOptionByKeyColl>	CMsOptionByKeyCollPtr;

};

#endif //__MSOPTION_H__