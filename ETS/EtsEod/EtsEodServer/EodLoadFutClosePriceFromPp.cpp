#include "StdAfx.h"
#include "EodLoadFutClosePriceFromPp.h"

namespace EOD
{

	CEodLoadFutClosePriceFromPp::~CEodLoadFutClosePriceFromPp(void)
	{
		if(m_hFutureReceived)
		{
			CloseHandle(m_hFutureReceived);
			m_hFutureReceived = NULL;
		}
		if(m_hDisconnect)
		{
			CloseHandle(m_hDisconnect);
			m_hDisconnect = NULL;
		}
	}

	void CEodLoadFutClosePriceFromPp::CancelRequest()
	{
		CancelLastQuote();
	}

	void CEodLoadFutClosePriceFromPp::LoadFromPp()
	{
		Report(StepMask(),enRpMtMessage, _T("Loading futures close price from Price Provider..."));
		m_nStartProgress = m_nProgress;

		bool bIsStopped = false;
		try
		{
			EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

			_PrepareFutureCollection(spMktStrMgr->UndByIdColl());

			if(m_spFutColl && m_spFutColl->size())
			{
				EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

				_Connect(spSettingMgr);

				m_itNextFut = m_spFutColl->begin();
				ATLASSERT(m_itNextFut != m_spFutColl->end());

				HANDLE hStopEvent = GetStopEventHandle();

				m_hFutureReceived = CreateEvent(NULL,FALSE,FALSE,NULL);
				m_hDisconnect  = CreateEvent(NULL,FALSE,FALSE,NULL);

				const HANDLE hEvents[] = {hStopEvent, m_hFutureReceived, m_hDisconnect};

				SetEvent(m_hFutureReceived);

				while(true)
				{
					DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE );

					if(dwRes == WAIT_OBJECT_0)
						throw EODCO::CEodStopException();
					else if(dwRes == WAIT_OBJECT_0 + 1)
					{
						if(m_itNextFut == m_spFutColl->end())
							break;
						else
						{
							_RequestFuturePrice();
						}
					}
					else if(dwRes == WAIT_OBJECT_0 + 2)
					{
						_Connect(spSettingMgr);
						SetEvent(m_hFutureReceived);
					}
				}

				Disconnect();

				if(m_hFutureReceived)
				{
					CloseHandle(m_hFutureReceived);
					m_hFutureReceived = NULL;
				}
				if(m_hDisconnect)
				{
					CloseHandle(m_hDisconnect);
					m_hDisconnect = NULL;
				}
				long nUpdatedFutCount = _GetUpdatedFutures();
				Report(StepMask(),enRpMtMessage, _T("Loaded close price for %d futures out of %d requested"),
					nUpdatedFutCount, m_spFutColl->size());
			}
			else
			{
				m_nProgress = 100L;
				Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
				CEodServer::s_pEodServer->StepsMgr()->IncProgress();
				Report(StepMask(),enRpMtMessage, _T("There are no futures with positions exists in DB"));
			}
		}
		catch(EODCO::CEodStopException)
		{
			bIsStopped = true;
		}
		catch (exception& e)
		{
			Disconnect();

			if(m_hFutureReceived)
			{
				CloseHandle(m_hFutureReceived);
				m_hFutureReceived = NULL;
			}
			if(m_hDisconnect)
			{
				CloseHandle(m_hDisconnect);
				m_hDisconnect = NULL;
			}
			throw e;
		}
		if(bIsStopped)
			throw EODCO::CEodStopException();
	}

	void CEodLoadFutClosePriceFromPp::_RequestFuturePrice()
	{
		m_spFutCurr  = m_itNextFut->second;
		ATLASSERT(m_spFutCurr);

		EODPP::CPpRequestDataPtr spReqData(new EODPP::CPpRequestData);
		EODPP::CPpPriceRequestKey spReq;

		m_spFutCurr->MakePriceRequestKey(spReq);
		spReqData->Reqs()->insert(spReq);
		SetRequestData(spReqData);
		RequestLastQuote();
	}

	void CEodLoadFutClosePriceFromPp::OnLastQuote(EODPP::CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult)
	{
		if(IsStopping())
			return;
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), pReq->m_sSymbol);

		ATLASSERT(pReq->m_enType == PP::enFUT);

		ATLASSERT(m_spFutCurr->m_sSymbol == pReq->m_sSymbol);

		if(m_spFutCurr->m_sSymbol == pReq->m_sSymbol)
		{
			if(!EODCO::IsBadValue(pResult->ClosePrice))
			{
				try
				{
					CStoredProc<> spPriceSave(m_spDB, _T("usp_EodUndClosePrice_Save"));
					spPriceSave << m_spFutCurr->m_nID;
					spPriceSave << pResult->ClosePrice;
					spPriceSave.Execute();
					m_spFutCurr->m_dPriceClose = pResult->ClosePrice;
					m_spFutCurr->m_bClosePriceUpdated = true;
				}
				catch (exception& e)
				{
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUndClosePrice_Save : %s, %s"), typeid(e).name(), e.what());
				}
			}
			else
				Report(StepMask(),enRpMtMessage, _T("Underlying %s has no close price on Price Provider"), (LPCTSTR)m_spFutCurr->m_sSymbol);
		}
	}

	void CEodLoadFutClosePriceFromPp::OnLastQuoteDone()
	{
		if(IsStopping())
			return;

		m_itNextFut++;
		
		_IncrementImportFuturesProgress();

		SetEvent(m_hFutureReceived);
	}

	void CEodLoadFutClosePriceFromPp::OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, EODPP::CPpPriceRequestKey* pReq  )
	{
		if(IsStopping())
			return;

		Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
		Report(StepMask(), enRpMtWarning, _T("Future close price uploading error %d - %s for '%s'"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCTSTR)pReq->m_sSymbol);
	}

	void CEodLoadFutClosePriceFromPp::OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
	{
		if(IsStopping())
			return;
		Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	}

	void CEodLoadFutClosePriceFromPp::OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
	{
		if(IsStopping())
			return;

		Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
		Report(StepMask(), enRpMtError, _T("Disconnect from Price provider: %d %s"),enErrorNumber, (LPCTSTR)_bstr_t(bsDescription));
		SetEvent(m_hDisconnect);
	}

	void CEodLoadFutClosePriceFromPp::_Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
	{
		long nTry = 0L;
		while(true)
		{
			if(nTry)
			{
				DWORD dwRes = ::WaitForSingleObject(GetStopEventHandle(), 1000 * pSettingsMgr->PpWaitInterval());
				if(dwRes == WAIT_OBJECT_0)
					throw EODCO::CEodStopException();
			}

			try
			{
				EODPP::CPpPriceProvider::Connect(pSettingsMgr);
				break;
			}
			catch (exception& e)
			{
				nTry++;
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Error connect to Price provider: %s %s"), typeid(e).name(), e.what());

				if(nTry > pSettingsMgr->PpReconnectAttempt())
				{
					Report(StepMask(), enRpMtError, _T("Error connect to Price provider: %s"), e.what());
					throw e;
				}
				else
					Report(StepMask(), enRpMtWarning, _T("Error connect to Price provider: %s. Try to recconnect"), e.what());
			}
		}
	}
	void CEodLoadFutClosePriceFromPp::_PrepareFutureCollection(EODMS::CMsUndByIDCollPtr pUndColl)
	{
		m_nLoadedFuture = 0L;
		m_spFutColl->clear();

		for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
		{
			EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(iterUnd->second));

			if(spIndex)
			{
				EODMS::CMsFutureCollPtr spFutures(spIndex->FutureWithPos());

				ATLASSERT(spFutures);
				for(EODMS::CMsFutureColl::iterator iterFut = spFutures->begin(); iterFut != spFutures->end(); iterFut++)
					m_spFutColl->insert(EODMS :: CMsFutureColl::value_type(iterFut->first, iterFut->second));
			}
		}
	}

	long CEodLoadFutClosePriceFromPp::_GetUpdatedFutures()
	{
		long nCount = 0L;
		for(EODMS::CMsFutureColl::iterator iterFut = m_spFutColl->begin(); iterFut != m_spFutColl->end(); iterFut++)
		{
			EODMS::CMsFuturePtr spFut(iterFut->second);
			ATLASSERT(spFut);

			if(spFut->m_bClosePriceUpdated)
				nCount++;
		}
		return nCount;
	}
	void CEodLoadFutClosePriceFromPp::_IncrementImportFuturesProgress()
	{
		size_t nAllFut = m_spFutColl->size();

		double dPersent = static_cast<double>(++m_nLoadedFuture) / nAllFut;

		size_t nProgress = m_nStartProgress + static_cast<long>(dPersent * (100L - m_nStartProgress));

		if(m_nProgress != nProgress)
		{
			m_nProgress = nProgress;
			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
		}
	}
	void CEodLoadFutClosePriceFromPp::SetType(byte type)
	{
		bType = type;
	}
}