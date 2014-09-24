#pragma once
#ifndef __EODLOADFUTOPTCLOSEPRICEFROMPP_H__
#define __EODLOADFUTOPTCLOSEPRICEFROMPP_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

	class CEodLoadFutOptClosePriceFromPp :
		virtual public CEodStepProcessorBase,
		protected EODPP::CPpPriceProvider
	{
	public:
		CEodLoadFutOptClosePriceFromPp(void) : m_hIndexReceived(NULL), m_hDisconnect(NULL), m_nLoadedIndex(0L), 
			m_spIndexColl(new EODMS::CMsIndexByIdColl), m_nStartProgress(0L) {};
		virtual ~CEodLoadFutOptClosePriceFromPp(void) ;

		void		 CancelRequest();
		virtual void LoadFromPp();

		void SetType(byte);

		virtual void OnLastQuote(EODPP::CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult);
		virtual void OnLastQuoteDone();
		virtual void OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, EODPP::CPpPriceRequestKey* pReq  );
		virtual void OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );
		virtual void OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription );

	private:
		virtual void _Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr);
		void _PrepareIndexCollection(EODMS::CMsUndByIDCollPtr pUndColl);
		void _RequestFutureOptionsPrice();
		void _GetUpdatedContracts(long& nIndexCount,  long& nOptCount, long& nTotalOptCount);
		void _IncrementImportFuturesProgress();
	private:
		long							m_nLoadedIndex;		
		EODMS::CMsIndexByIdColl::iterator m_itNextIndex;
		HANDLE							m_hIndexReceived;
		HANDLE							m_hDisconnect;
		EODMS::CMsIndexPtr				m_spIndexCurr;
		size_t							m_nStartProgress;
		byte							bType; /*0-defaut, 1 - close prices, 2 - yest prices*/
	public:
		EODMS::CMsIndexByIdCollPtr		m_spIndexColl;
		long							m_nLoadedFuture;
	};

};

#endif// __EODLOADFUTOPTCLOSEPRICEFROMPP_H__
