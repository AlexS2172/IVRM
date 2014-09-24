#pragma once
#include "MsContract.h"
#include "SmSettingsMgr.h"

namespace EODMS
{

	/////////////////////////////////////////////////////////////////////////////
	// Forward declaration
	/*
	class CMsCommodity;
	typedef boost::shared_ptr<CMsCommodity> CMsCommodityPtr;
	typedef std::map<CString, CMsCommodityPtr, EODCO::CEgObjectBase>	CMsCommodityColl;
	typedef boost::shared_ptr<CMsCommodityColl>	CMsCommodityCollPtr;
	typedef std::map<long, CMsCommodityPtr, EODCO::CEgObjectBase>	CMsCommodityByIDColl;
	typedef boost::shared_ptr<CMsCommodityByIDColl>	CMsCommodityByIDCollPtr;
	*/

	/////////////////////////////////////////////////////////////////////////////
	//

class CMsCommodity : public CMsContract
{
public:
	CMsCommodity(void):	m_bIsActive(0), 
						m_nGreeksCalcType(0),
						m_bMsUpdated(false),
						m_nID(0)
	{
	}

	~CMsCommodity(void)
	{
	}
	
	SYMBOL_TYPE Type() const
	{
		return enStFuture;
	}
	bool			m_bIsActive;
	long			m_nGreeksCalcType;
	bool			m_bMsUpdated;
	long			m_nID;

};

typedef boost::shared_ptr<CMsCommodity> CMsCommodityPtr;
typedef std::map<CString, CMsCommodityPtr>	CMsCommodityColl;
typedef boost::shared_ptr<CMsCommodityColl>	CMsCommodityCollPtr;
typedef std::map<long, CMsCommodityPtr>	CMsCommodityByIDColl;
typedef boost::shared_ptr<CMsCommodityByIDColl>	CMsCommodityByIDCollPtr;

};
