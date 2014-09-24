#pragma once
#ifndef __EODLOADINDEXFUTERS_H__
#define __EODLOADINDEXFUTERS_H__

#pragma warning (push)
#pragma warning( disable : 4290 )

#include "EodStepProcessorBase.h"
#include "EodLoadMktStrUpdates.h"
#include "EodLoadFutClosePriceFromPp.h"
#include "EodLoadFutOptClosePriceFromPp.h"
#include "EodLoadFutureClosePriceStage.h"

namespace EOD
{

class CEodStepLoadIndexFutures :
	virtual public CEodStepProcessorBase, 
	public EODPP::CPpStructureProvider,
	public CEodLoadMarketStructureUpdates,
	public CEodLoadFutClosePriceFromPp,
	public CEodLoadFutOptClosePriceFromPp
{
public:
	CEodStepLoadIndexFutures(void) : m_hUndReceived(NULL), m_hAllOptReceived(NULL), m_hDisconnect(NULL), m_nLoadedUnd(0L), m_nLoadedFutRoots(0L), 
		 m_bCheckMktStrUpdates(false), m_bOnlyActive(false), m_bDeleteOldOptions(true), m_bUpdateFutures(false),
		m_hAllFutReceived(NULL), m_hAllFutOptReceived(NULL),
		m_bLoadUndClosePrices(true), m_bLoadOptClosePrices(true),
		 m_nLoadedFutures(0L), m_enCurrentStage(enEodInvalid) {};
	virtual ~CEodStepLoadIndexFutures(void) ;

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();

	virtual void OnStock(struct PP::StockParams* pReq,  struct PP::StockResultsEx* pResult);
	virtual void OnOption(struct PP::OptionParams* pReq,  struct PP::OptionResultsEx* pResult, VARIANT_BOOL IsLastOption);
	virtual void OnGetStockError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
		struct PP::StockParams* pReq );
	virtual void OnGetOptionError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
		struct PP::OptionParams* pReq );
	virtual void OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );
	virtual void OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );
	virtual void OnFutureByRoot(struct PP::FutureRootParams* pReq,  struct PP::FutureResultsEx* pResult, VARIANT_BOOL IsLastFuture);
	virtual void OnFuturesOption(struct PP::FuturesOptionParams* pReq,  struct PP::FuturesOptionResultsEx* pResult, VARIANT_BOOL IsLastOption);
	virtual void OnGetFutureByRootError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
	struct PP::FutureRootParams* pReq );
	virtual void OnGetFuturesOptionError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
	struct PP::FuturesOptionParams* pReq );

	void _PrepareFutureCollection(EODMS::CMsUndByIDCollPtr pUndColl);
	void _PrepareIndexCollection(EODMS::CMsUndByIDCollPtr pUndColl);

	/*EODMS::CMsFutureByIdCollPtr CommodityFutures()
	{
		return m_spFuturesColl;
	}*/

private:
	virtual void Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr);
	void _RequestUnderlyingStructure();
	void _SetAllNeedUpdate();
	void _FinishLoadUndOptions();
	void _FinishLoadRootFutures();
	void _FinishLoadFutures();
	void _GetUpdatedUndCount(long& nTotalCount, long& nUpdatedCount);
	void _GetUpdatedFutRootCount(long& nTotalCount, long& nUpdatedCount);
	void _GetUpdatedFutCount(long& nTotalCount, long& nUpdatedCount);
	EODMS::CMsUndByIDCollPtr _GetActiveColl();
	void _DeleteOldOrdersFromDb() throw(exception);
	void _InitMsLoadInDb() throw(exception);
	void _CompleteMsLoadInDb(long& nUndNeedReUpdate) throw(exception);
	void _Uninitialize() throw();

	void _LoadUnderlyingsAndOptions(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);
	void _LoadFutures(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);
	void _LoadCommodityFutures(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);
	void _LoadInterestFutures(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);
	
	void _LoadFutureOptions(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);

	void _LoadCommodityFutureOptions(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);
	void _LoadInterestFutureOptions(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);

	void _IncrementImportOptionsProgress() throw();
	void IncrementImportFuturesProgress() throw();
	void _IncrementImportFuturesOptionsProgress() throw();
	void _IncrementImportFuturesProgress();

public:
	void init(EodStepsMaskEnum mask, int m_bLoadPrices);

private:
	EodStepsMaskEnum				m_iMask;
	bool							m_bCheckMktStrUpdates;
	bool							m_bUpdateFutures;
	bool							m_bOnlyActive;
	bool							m_bDeleteOldOptions;
	bool                            m_bLoadUndClosePrices;
	bool                            m_bLoadOptClosePrices;
	long							m_nLoadedUnd;
	EODMS::CMsUndByIDCollPtr		m_spUndColl;
	HANDLE							m_hUndReceived;
	HANDLE							m_hAllOptReceived;
	HANDLE							m_hDisconnect;
	HANDLE							m_hAllFutReceived;
	HANDLE							m_hAllFutOptReceived;
	EODMS::CMsUndPtr				m_spUndCurr;

	EODMS::CMsFuturesRootCollPtr	m_spFutRootsColl;
	EODMS::CMsFutureByIdCollPtr		m_spFuturesColl;
	EODMS::CMsFuturesRootPtr		m_spFutRootCurr;
	EODMS::CMsFuturePtr				m_spFutureCurr;
	long							m_nLoadedFutRoots;
	long							m_nLoadedFutures;
	byte							m_bLoadPrices; /*0-no, 1 - close price, 2 - yest price*/
	EodLoadFutureClosePriceStage	m_enCurrentStage;
	long							m_lNumOptions;
};
typedef boost::shared_ptr<CEodStepLoadIndexFutures> CEodStepLoadIndexFuturesPtr;

};

#pragma warning (pop)
#endif// __EODLOADINDEXFUTERS_H__
