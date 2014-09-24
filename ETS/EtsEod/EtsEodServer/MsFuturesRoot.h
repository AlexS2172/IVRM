#ifndef __MSFUTURESROOT_H__
#define __MSFUTURESROOT_H__

#include "MsFuture.h"
namespace EODMS
{

	/////////////////////////////////////////////////////////////////////////////
	//
	class  CMsFuturesRoot :  public EODCO::CEgTraceable
	{
	public:
		CMsFuturesRoot(void)
			: m_nRootId(0L)
			, m_bMsUpdated(false),
			m_spFutures(new CMsFutureColl)
		{
		}

		~CMsFuturesRoot(void)
		{
			
		}

		CMsFutureCollPtr Futures()
		{
			return m_spFutures;
		}

		CMsFuturePtr Futures(const CString& sFutureSymbol)
		{
			return GetCollectionItem<CMsFuturePtr>(m_spFutures, sFutureSymbol);
		}

	public:
		long	m_nRootId;
		long	m_nUndId;
		CString	m_sRootName;
		CString	m_sRootSymbol;
		bool	m_bMsUpdated;
	protected:
		CMsFutureCollPtr	m_spFutures; // new uploaded futures
	};

	typedef boost::shared_ptr<CMsFuturesRoot> CMsFuturesRootPtr;
	typedef std::map<long, CMsFuturesRootPtr>	CMsFuturesRootColl;
	typedef std::pair<long, CMsFuturesRootPtr>	CMsFuturesRootPair;

	typedef boost::shared_ptr<CMsFuturesRootColl>	CMsFuturesRootCollPtr;

};

#endif //__MSFUTURESROOT_H__