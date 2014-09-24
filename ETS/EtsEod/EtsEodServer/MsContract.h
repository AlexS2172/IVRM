#ifndef __MSCONTRACT_H__
#define __MSCONTRACT_H__

#include "PpPriceRequest.h"
namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
class  CMsContract :  public EODCO::CEgTraceable
{
public:
	CMsContract(void)
		: m_nID(0L), m_dPriceClose(BAD_DOUBLE_VALUE)
	{
	}

	~CMsContract(void)
	{
	}

	virtual SYMBOL_TYPE Type() const = 0;

	void MakePriceRequestKey(EODPP::CPpPriceRequestKey& aKey) const
	{
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

		aKey.m_sSymbol = m_sSymbol;

		switch(Type())
		{
		case enStStock:
			aKey.m_enType = PP::enSTK;
			break;
		case enStIndex:
			aKey.m_enType = PP::enIDX;
			break;
		case enStOption:
			aKey.m_enType = PP::enOPT;
			break;
		case enStFuture:
			aKey.m_enType = PP::enFUT;
			break;
		case enStFutureOption:
			aKey.m_enType = PP::enFOP;
			break;
		};

		aKey.m_sExchange =_T("");

		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	}
	static long GetExpiryID(const COleDateTime& dtExpiration) 	{
		return static_cast<long>( dtExpiration.m_dt);
	}

public:
	long			m_nID;
	CString			m_sSymbol;
	double			m_dPriceClose;
};

typedef boost::shared_ptr<CMsContract> CMsContractPtr;
typedef std::map<CString, CMsContractPtr>	CMsContractColl;
typedef boost::shared_ptr<CMsContractColl>	CMsContractCollPtr;

};

#endif //__MSCONTRACT_H__
