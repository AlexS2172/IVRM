#ifndef __EODMARKETSTRUCTUREMGR_H__
#define __EODMARKETSTRUCTUREMGR_H__

#include "EodGeneral.h"

#pragma warning (push)
#pragma warning( disable : 4290 )

namespace EOD
{

/////////////////////////////////////////////////////////////////////////////
//
class CEodMktStructureMgr : 
	 
	public EODCO::CEgTraceable
{
public:
	CEodMktStructureMgr(void)
		: m_spUndColl(new EODMS::CMsUndColl),
		m_spUndByIDColl(new EODMS::CMsUndByIDColl),
		m_spRateColl(new EODMS::CMsRateColl),
		m_spExpDateColl(new EODMS::CMsExpDateColl),
		m_spFutRootColl(new EODMS::CMsFuturesRootColl),
		m_spInterestFutRootColl(new EODMS::CMsFuturesRootColl),
		m_spCommodityFutRootColl(new EODMS::CMsFuturesRootColl),
		m_spCommIRFXFuturesColl(new EODMS::CMsFutureByIdColl)
	{
	}

	~CEodMktStructureMgr(void);

	void Init(EODSM::CSmSettingsMgrPtr pSettingsMgr) throw (exception);
	
	void CleanUp() throw (exception);

	void LoadBaseData() throw (exception);

/*	void LoadCommodities() throw (exception); 
	
	void LoadInterests() throw (exception);
*/
	void LoadOptionRootsData() throw (exception);

	void _LoadCommodityFuturesRootsData() throw (exception);

	void _LoadInterestFuturesRootsData() throw (exception);

	void LoadPositionsData() throw (exception);

	void LoadEmptyIvExpirations() throw (exception);

	void LoadExpiryDates() throw (exception);

	void _CommodityFuturesAdjustRates();

	EODMS::CMsRateCollPtr RateColl()
	{
		return m_spRateColl;
	}

	EODMS::CMsRatePtr RateColl(long nCurveID)
	{
		return GetCollectionItem<EODMS::CMsRatePtr>(m_spRateColl, nCurveID);
	}

	EODMS::CMsUndCollPtr UndColl()
	{
		return m_spUndColl;
	}

	EODMS::CMsUndByIDCollPtr UndByIdColl()
	{
		return m_spUndByIDColl;
	}

	EODMS::CMsUndPtr Und(long nUndID)
	{
		return GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, nUndID);
	}

	EODMS::CMsUndPtr Und(CString& sSymbol)
	{
		return GetCollectionItem<EODMS::CMsUndPtr>(m_spUndColl, sSymbol);
	}

	EODMS::CMsExpDateCollPtr ExpDate()
	{
		return m_spExpDateColl;
	}

	EODMS::CMsExpDatePtr ExpDate(DATE dtExp)
	{
		return GetCollectionItem<EODMS::CMsExpDatePtr>(m_spExpDateColl, dtExp);
	}

	EODMS::CMsFuturesRootCollPtr FuturesRoot()
	{
		return m_spFutRootColl;
	}

	EODMS::CMsFuturesRootPtr FuturesRoot(long nRootID)
	{
		return GetCollectionItem<EODMS::CMsFuturesRootPtr>(m_spFutRootColl, nRootID);
	}

	EODMS::CMsFuturesRootCollPtr FuturesCommoditiesRoot()
	{
		return m_spCommodityFutRootColl;
	}

	EODMS::CMsFuturesRootCollPtr FuturesInterestRoot()
	{
		return m_spInterestFutRootColl;
	}

	EODMS::CMsFutureByIdCollPtr CommFxIrFutures()
	{
		return m_spCommIRFXFuturesColl;
	}

private:
	void _CheckDbConnection() throw (exception)
	{
		if(!m_spDB.IsOpened())
			throw exception(_T("Database connection is not opened."));
	}

	void _ClearStorage();

	void _ClearCommodity();

	void _ClearInterest();

	void _ClearUnderlyingsPositionsData();

	void _ClearOptionRootsData();

	void _ClearUnderlyingsExpirationData();	

	void _ClearFuturesPositionsData();

	void _LoadRates() throw (exception);

	void _LoadUnderlyingsBaseData() throw (exception);

	void _LoadBasketIndicesData() throw (exception);

	void _InitBasketDividends();

	void _LoadUnderlyingsWithPosition() throw (exception);

	void _LoadUnderlyingsWithOptionPosition() throw (exception);	

	void _LoadSynthRootComponentsWithPositions() throw (exception);

	void _LoadOptionsWithPositions() throw (exception);

	void _UnderlyingsAdjustRates() throw (exception);

	void _UnderlyingAdjustRates(EODMS::CMsUndPtr pUnd) throw (exception);

	void _InitSynthRootBasketDividends(EODMS::CMsUndPtr pUnd) throw (exception);

	void _LoadFuturesRootsData() throw (exception);
	
	void _LoadFuturesWithPosition() throw (exception);

	void _LoadFuturesOptionPosition() throw (exception);

	void _LoadInterests() throw (exception);
	void _LoadCommoditiesFX() throw (exception);

	EODMS::CMsFutureByIdCollPtr _LoadFuturesWithOptionPosition() throw (exception);

	void _LoadFuturesOptionWithPosition(EODMS::CMsFutureByIdCollPtr pFutColl) throw (exception);

	void _CopyFuturesOptionWithPositionInIndex() throw (exception);

	void _LoadCommFuturesOptionWithPosition(EODMS::CMsFutureByIdCollPtr spFutColl);

private:
	CDBConnection					m_spDB;

	EODMS::CMsUndCollPtr			m_spUndColl;
	EODMS::CMsUndByIDCollPtr		m_spUndByIDColl;
	EODMS::CMsRateCollPtr			m_spRateColl;
	EODMS::CMsExpDateCollPtr		m_spExpDateColl;
	
	EODMS::CMsFuturesRootCollPtr	m_spFutRootColl;
	CLock							m_csObjectLock;

	EODMS::CMsFuturesRootCollPtr	m_spCommodityFutRootColl;
	EODMS::CMsFuturesRootCollPtr	m_spInterestFutRootColl;
	EODMS::CMsFutureByIdCollPtr		m_spCommIRFXFuturesColl;
};

typedef boost::shared_ptr<CEodMktStructureMgr> CEodMktStructureMgrPtr;

#pragma warning (pop)

};

#endif //__EODMARKETSTRUCTUREMGR_H__