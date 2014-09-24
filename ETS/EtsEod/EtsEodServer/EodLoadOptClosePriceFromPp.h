#pragma once
#ifndef __EODLOADOPTCLOSEPRICEFROMPP_H__
#define __EODLOADOPTCLOSEPRICEFROMPP_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodLoadOptClosePriceFromPp :
	virtual public CEodStepProcessorBase,
	protected EODPP::CPpPriceProvider
{
public:
	CEodLoadOptClosePriceFromPp(void) : m_hUndReceived(NULL), m_hDisconnect(NULL), m_nLoadedUnd(0L), 
		m_nUpdatedUnd(0L), m_nUpdatedOpt(0L) {};
	virtual ~CEodLoadOptClosePriceFromPp(void) ;

	void		 CancelRequest();
	virtual void LoadFromPp();

	virtual void OnLastQuote(EODPP::CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult);
	virtual void OnLastQuoteDone();
	virtual void OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, EODPP::CPpPriceRequestKey* pReq  );
	virtual void OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );
	virtual void OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );

private:
	virtual void _Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr);
	EODMS::CMsUndByIDCollPtr _PrepareUndCollection(EODMS::CMsUndByIDCollPtr pUndColl);
	void _RequestOptionsPrice();
	void _GetUpdatedContracts(EODMS::CMsUndByIDCollPtr pUndColl, long& nUndCount,  long& nOptCount, long& nTotalOptCount);
	void _IncrementProgress();
private:
	long							m_nUpdatedUnd;
	long							m_nUpdatedOpt;
	long							m_nLoadedUnd;
	EODMS::CMsUndByIDCollPtr		m_spUndColl;
	EODMS::CMsUndByIDColl::iterator m_itNextUnd;
	HANDLE							m_hUndReceived;
	HANDLE							m_hDisconnect;
	EODMS::CMsUndPtr				m_spUndCurr;
};

};

#endif// __EODLOADOPTCLOSEPRICEFROMPP_H__
