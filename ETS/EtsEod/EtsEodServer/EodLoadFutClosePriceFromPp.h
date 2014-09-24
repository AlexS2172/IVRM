#pragma once
#ifndef __EODLOADFUTCLOSEPRICEFROMPP_H__
#define __EODLOADFUTCLOSEPRICEFROMPP_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

	class CEodLoadFutClosePriceFromPp :
		virtual public CEodStepProcessorBase,
		protected EODPP::CPpPriceProvider
	{
	public:
		CEodLoadFutClosePriceFromPp(void) : m_hFutureReceived(NULL), m_hDisconnect(NULL), m_nLoadedFuture(0L), 
			m_spFutColl(new EODMS::CMsFutureColl), m_nStartProgress(0L){};
		virtual ~CEodLoadFutClosePriceFromPp(void) ;

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
		void _PrepareFutureCollection(EODMS::CMsUndByIDCollPtr pUndColl);
		void _RequestFuturePrice();
		long _GetUpdatedFutures();
		void _IncrementImportFuturesProgress();
	private:		
		EODMS::CMsFutureColl::iterator	m_itNextFut;
		HANDLE							m_hFutureReceived;
		HANDLE							m_hDisconnect;
		EODMS::CMsFuturePtr				m_spFutCurr;
		
		size_t							m_nStartProgress;
		byte							bType; /*0-defaut, 1 - close prices, 2 - yest prices*/
		bool							m_bLoadFromGlobex;
	public:
		long							m_nLoadedFuture;
		EODMS::CMsFutureCollPtr			m_spFutColl;
		EODMS::CMsFutureCollPtr			m_spFutCollGlobex;
	};

};

#endif// __EODLOADFUTCLOSEPRICEFROMPP_H__
