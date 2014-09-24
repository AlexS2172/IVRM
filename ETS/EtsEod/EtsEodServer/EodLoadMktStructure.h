#pragma once
#ifndef __EODLOADMKTSTRUCTURE_H__
#define __EODLOADMKTSTRUCTURE_H__

#pragma warning (push)
#pragma warning( disable : 4290 )

#include "EodStepProcessorBase.h"
#include "EodLoadMktStrUpdates.h"

namespace EOD
{

class CEodStepLoadMktStructure :
	virtual public CEodStepProcessorBase, 
	public EODPP::CPpStructureProvider,
	public CEodLoadMarketStructureUpdates
{
public:
	CEodStepLoadMktStructure(void) : m_nLoadedOptions(0L), m_nLoadedUnd(0L), m_nLoadedFutRoots(0L), m_lLoadFuturesPrice(0L),
		m_bCheckMktStrUpdates(false), m_bOnlyActive(false), m_bDeleteOldOptions(false), m_bUpdateFutures(false),
		m_bLoadUndClosePrices(true), m_bLoadOptClosePrices(true), m_bFilterDividends(true),m_bOptionsLoad(true),
		m_nLoadedFutures(0L) {};
	virtual ~CEodStepLoadMktStructure(void) ;

	void Init(
		bool bCheckMktStrUpdates, 
		bool bOnlyActive, 
		bool bDeleteOldOptions = false, 
		bool bLoadUndClosePrices = true, 
		bool bLoadOptClosePrices = true, 
		bool bFilterDividends = true, 
		bool bOptionsLoad =true,
		long lLoadFuturesPrice= 0
		);
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

private:
	virtual void _Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr);
	void _RequestUnderlyingStructure();
	void _SetAllNeedUpdate();
	void _FinishLoadUndOptions();
	void _FinishLoadRootFutures();
	void _FinishLoadFutures();
	void _GetUpdatedUndCount(long& nTotalCount, long& nUpdatedCount);
	void _GetUpdatedFutRootCount(long& nTotalCount, long& nUpdatedCount);
	void _GetUpdatedFutCount(long& nTotalCount, long& nUpdatedCount);
	EODMS::CMsUndCollPtr _GetActiveColl();
	void _DeleteOldOrdersFromDb() throw(exception);
	void _InitMsLoadInDb() throw(exception);
	void _CompleteMsLoadInDb(long& nUndNeedReUpdate) throw(exception);
	void _Uninitialize() throw();

	void _LoadUnderlyingsAndOptions(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);
	void _LoadFutures(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);
	void _LoadFutureOptions(EODSM::CSmSettingsMgrPtr pSettings) throw(exception);

	void _IncrementImportOptionsProgress() throw();
	void _IncrementImportFuturesProgress() throw();
	void _IncrementImportFuturesOptionsProgress() throw();

private:
	bool							m_bCheckMktStrUpdates;
	bool							m_bUpdateFutures;
	bool							m_bOnlyActive;
	bool							m_bDeleteOldOptions;
	bool                            m_bLoadUndClosePrices;
	bool                            m_bLoadOptClosePrices;
	long							m_nLoadedUnd;
	long                            m_nLoadedOptions;
	bool							m_bFilterDividends;
	bool                            m_bOptionsLoad;
	std::string						m_sOptionsUpdate;
	std::map<CString, long>     m_mapRoots;

	EODMS::CMsUndCollPtr			m_spUndColl;
	CHandle							m_hUndReceived;
	CHandle							m_hAllOptReceived;
	CHandle							m_hDisconnect;
	CHandle							m_hAllFutReceived;
	CHandle							m_hAllFutOptReceived;
	EODMS::CMsUndPtr				m_spUndCurr;

	EODMS::CMsFuturesRootCollPtr	m_spFutRootsColl;
	EODMS::CMsFutureByIdCollPtr		m_spFuturesColl;
	EODMS::CMsFuturesRootPtr		m_spFutRootCurr;
	EODMS::CMsFuturePtr				m_spFutureCurr;
	long							m_nLoadedFutRoots;
	long							m_nLoadedFutures;
	long							m_lLoadFuturesPrice;
};
typedef boost::shared_ptr<CEodStepLoadMktStructure> CEodStepLoadMktStructurePtr;

};

#pragma warning (pop)
#endif// __EODLOADMKTSTRUCTURE_H__
