#ifndef __MSOPTROOT_H__
#define __MSOPTROOT_H__

#include "MsOption.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsOptRoot :  public EODCO::CEgTraceable
{
public:
	CMsOptRoot(void)
		: m_nID(0L), m_nLotSize(0L),
		m_spOpt(new CMsOptionByIDColl)
	{
	}

	~CMsOptRoot(void)
	{
	}

	virtual bool IsSynth()
	{
		return false;
	}

	CMsOptionByIDCollPtr Opt()
	{
		return m_spOpt;
	}

	CMsOptionPtr Opt(long nOptID)
	{
		return GetCollectionItem<CMsOptionPtr>(m_spOpt, nOptID);
	}

public:
	long					m_nID;
	CString					m_sName;
	long					m_nLotSize;

protected:
	CMsOptionByIDCollPtr	m_spOpt; // options by option ID
};

typedef boost::shared_ptr<CMsOptRoot> CMsOptRootPtr;
typedef std::map<long, CMsOptRootPtr>	CMsOptRootColl;
typedef std::pair<long, CMsOptRootPtr>	CMsOptRootPair;

typedef boost::shared_ptr<CMsOptRootColl>	CMsOptRootCollPtr;
typedef std::map<CString, CMsOptRootPtr>	CMsOptRootByNameColl;
typedef std::pair<CString, CMsOptRootPtr>	CMsOptRootByNamePair;

typedef boost::shared_ptr<CMsOptRootByNameColl>	CMsOptRootByNameCollPtr;

};

#endif //__MSOPTROOT_H__