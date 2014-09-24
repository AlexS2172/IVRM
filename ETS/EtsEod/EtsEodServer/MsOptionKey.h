#ifndef __MSOPTIONKEY_H__
#define __MSOPTIONKEY_H__

namespace EODMS
{
const double cStrikeEps = 0.0001;

class CMsOptionKey
{	
public:
	CMsOptionKey() : m_sRoot(_T("")), m_nExpiryID(BAD_LONG_VALUE), m_dStrike(BAD_DOUBLE_VALUE), m_bIsCall(false)
	{}

	CMsOptionKey(const CMsOptionKey& aKey) :
		m_sRoot(aKey.m_sRoot), m_nExpiryID(aKey.m_nExpiryID), m_dStrike(aKey.m_dStrike), m_bIsCall(aKey.m_bIsCall)
	{}

	virtual ~CMsOptionKey(void)
	{
	}

	bool operator < (const CMsOptionKey& aKey) const
	{
		return m_sRoot < aKey.m_sRoot
			|| m_sRoot == aKey.m_sRoot && m_nExpiryID < aKey.m_nExpiryID
			|| m_sRoot == aKey.m_sRoot && m_nExpiryID == aKey.m_nExpiryID && (m_dStrike < aKey.m_dStrike && fabs(m_dStrike - aKey.m_dStrike) >= cStrikeEps)
			|| m_sRoot == aKey.m_sRoot && m_nExpiryID == aKey.m_nExpiryID && fabs(m_dStrike - aKey.m_dStrike) < cStrikeEps && m_bIsCall < aKey.m_bIsCall;
	}

	bool operator == (const CMsOptionKey& aKey) const
	{
		return m_sRoot == aKey.m_sRoot && m_nExpiryID == aKey.m_nExpiryID && fabs(m_dStrike - aKey.m_dStrike) < cStrikeEps && m_bIsCall == aKey.m_bIsCall;
	}	

public:
	CString m_sRoot;
	long	m_nExpiryID;
	double	m_dStrike;
	bool	m_bIsCall;
};

const CMsOptionKey badOptionKey;

};

#endif// __MSOPTIONKEY_H__
