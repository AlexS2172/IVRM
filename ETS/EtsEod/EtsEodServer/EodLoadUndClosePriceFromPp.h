#pragma once
#ifndef __EODLOADUNDCLOSEPRICEFROMPP_H__
#define __EODLOADUNDCLOSEPRICEFROMPP_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodLoadUndClosePriceFromPp :
	virtual public CEodStepProcessorBase,
	protected EODPP::CPpPriceProvider
{
public:
	CEodLoadUndClosePriceFromPp(void) : m_hUndReceived(NULL), m_hDisconnect(NULL), m_nLoadedUnd(0L),  m_nUpdatedUnd(0L) {};
	virtual ~CEodLoadUndClosePriceFromPp(void) ;

	void CancelRequest();
	void LoadFromPp();

	virtual void OnLastQuote(EODPP::CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult);
	virtual void OnLastQuoteDone();
	virtual void OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, EODPP::CPpPriceRequestKey* pReq  );
	virtual void OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );
	virtual void OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );

private:
	virtual void _Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr);
	EODMS::CMsUndByIDCollPtr _PrepareUndCollection(EODMS::CMsUndByIDCollPtr pUndColl);
	long _GetUpdatedUnderlyings(EODMS::CMsUndByIDCollPtr pUndColl);
	void _RequestUnderlyingPrice();
	void _IncrementProgress();
private:
	long							m_nUpdatedUnd;
	long							m_nLoadedUnd;
	EODMS::CMsUndByIDCollPtr		m_spUndColl;
	EODMS::CMsUndByIDColl::iterator m_itNextUnd;
	HANDLE							m_hUndReceived;
	HANDLE							m_hDisconnect;
	EODMS::CMsUndPtr				m_spUndCurr;
};

};

#endif// __EODLOADUNDCLOSEPRICEFROMPP_H__
