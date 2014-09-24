#include "StdAfx.h"
#include "EodLoadFutOptClosePriceFromPp.h"

namespace EOD
{

	CEodLoadFutOptClosePriceFromPp::~CEodLoadFutOptClosePriceFromPp(void)
	{
		if(m_hIndexReceived)
		{
			CloseHandle(m_hIndexReceived);
			m_hIndexReceived = NULL;
		}
		if(m_hDisconnect)
		{
			CloseHandle(m_hDisconnect);
			m_hDisconnect = NULL;
		}
	}

	void CEodLoadFutOptClosePriceFromPp::CancelRequest()
	{
		CancelLastQuote();
	}

	void CEodLoadFutOptClosePriceFromPp::LoadFromPp()
	{
		Report(StepMask(),enRpMtMessage, _T("Loading future options close price from Price Provider..."));

		bool bIsStopped = false;
		m_nStartProgress = m_nProgress;
		try
		{
			EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

			_PrepareIndexCollection(spMktStrMgr->UndByIdColl());

			if(m_spIndexColl && m_spIndexColl->size())
			{
				EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

				_Connect(spSettingMgr);

				m_itNextIndex = m_spIndexColl->begin();
				ATLASSERT(m_itNextIndex != m_spIndexColl->end());

				HANDLE hStopEvent = GetStopEventHandle();

				m_hIndexReceived = CreateEvent(NULL,FALSE,FALSE,NULL);
				m_hDisconnect  = CreateEvent(NULL,FALSE,FALSE,NULL);

				const HANDLE hEvents[] = {hStopEvent, m_hIndexReceived, m_hDisconnect};

				SetEvent(m_hIndexReceived);

				while(true)
				{
					DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE );

					if(dwRes == WAIT_OBJECT_0)
						throw EODCO::CEodStopException();
					else if(dwRes == WAIT_OBJECT_0 + 1)
					{
						if(m_itNextIndex == m_spIndexColl->end())
							break;
						else
						{
							_RequestFutureOptionsPrice();
						}
					}
					else if(dwRes == WAIT_OBJECT_0 + 2)
					{
						_Connect(spSettingMgr);
						SetEvent(m_hIndexReceived);
					}
				}

				Disconnect();

				if(m_hIndexReceived)
				{
					CloseHandle(m_hIndexReceived);
					m_hIndexReceived = NULL;
				}
				if(m_hDisconnect)
				{
					CloseHandle(m_hDisconnect);
					m_hDisconnect = NULL;
				}

				long nIndexCount, nOptCount, nTotalOptCount;
				_GetUpdatedContracts(nIndexCount, nOptCount, nTotalOptCount);
				Report(StepMask(),enRpMtMessage, _T("Loaded close price for %d future options of %d indexes out of %d requested future options of %d indexes"),
					nOptCount, nIndexCount, nTotalOptCount, m_spIndexColl->size());
			}
			else
			{
				m_nProgress = 100L;
				Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
				CEodServer::s_pEodServer->StepsMgr()->IncProgress();
				Report(StepMask(),enRpMtMessage, _T("There are no future options with positions exists in DB"));
			}
		}
		catch(EODCO::CEodStopException)
		{
			bIsStopped = true;
		}
		catch (exception& e)
		{
			Disconnect();

			if(m_hIndexReceived)
			{
				CloseHandle(m_hIndexReceived);
				m_hIndexReceived = NULL;
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

	void CEodLoadFutOptClosePriceFromPp::_RequestFutureOptionsPrice()
	{
		m_spIndexCurr  = m_itNextIndex->second;
		ATLASSERT(m_spIndexCurr);

		EODPP::CPpRequestDataPtr spReqData(new EODPP::CPpRequestData);

		EODMS::CMsFutureOptionCollPtr spOptColl(m_spIndexCurr->FutureOptionWithPos());
		ATLASSERT(spOptColl);

		for(EODMS::CMsFutureOptionColl::iterator iterOpt = spOptColl->begin(); iterOpt != spOptColl->end(); iterOpt++)
		{
			EODMS::CMsFutureOptionPtr spOpt(iterOpt->second);
			ATLASSERT(spOpt);

			EODPP::CPpPriceRequestKey aReq;
			spOpt->MakePriceRequestKey(aReq);
			spReqData->Reqs()->insert(aReq);
		}

		SetRequestData(spReqData);
		RequestLastQuote();
	}

	void CEodLoadFutOptClosePriceFromPp::OnLastQuote(EODPP::CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult)
	{
		if(IsStopping())
			return;

		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), pReq->m_sSymbol);

		EODMS::CMsFutureOptionCollPtr spOptColl(m_spIndexCurr->FutureOptionWithPos());
		ATLASSERT(spOptColl);

		EODMS::CMsFutureOptionPtr spOpt(GetCollectionItem<EODMS::CMsFutureOptionPtr>(spOptColl, pReq->m_sSymbol));

		if(spOpt)
		{
			if(!EODCO::IsBadValue(pResult->ClosePrice))
			{
				try
				{
					CStoredProc<> spPriceSave(m_spDB, L"usp_EodOptClosePrice_Save");
					spPriceSave << spOpt->m_nID;
					spPriceSave << pResult->ClosePrice;

					spPriceSave.Execute();
					spOpt->m_dPriceClose = pResult->ClosePrice;
					m_spIndexCurr->m_nFutOptClosePriceUpdated++;
				}
				catch (exception& e)
				{
					Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptClosePrice_Save : %s, %s"), typeid(e).name(), e.what());
				}
			}
			else
				Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("'%s' future option of '%s' index has no close price on Price Provider"), spOpt->m_sSymbol, m_spIndexCurr->m_sSymbol);
		}
	}

	void CEodLoadFutOptClosePriceFromPp::OnLastQuoteDone()
	{
		if(IsStopping())
			return;

		m_itNextIndex++;
		_IncrementImportFuturesProgress();

		if(m_spIndexCurr->m_nFutOptClosePriceUpdated != m_spIndexCurr->FutureOptionWithPos()->size())
			Report(StepMask(),enRpMtMessage, _T("Can't upload all futures options close prices for %s index from Price Provider"), m_spIndexCurr->m_sSymbol);

		SetEvent(m_hIndexReceived);
	}

	void CEodLoadFutOptClosePriceFromPp::OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, EODPP::CPpPriceRequestKey* pReq  )
	{
		if(IsStopping())
			return;

		Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
		Report(StepMask(), enRpMtWarning, _T("Future option close price uploading error %d - %s for '%s'"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), pReq->m_sSymbol);
	}

	void CEodLoadFutOptClosePriceFromPp::OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
	{
		if(IsStopping())
			return;
		Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	}

	void CEodLoadFutOptClosePriceFromPp::OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
	{
		if(IsStopping())
			return;

		Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
		Report(StepMask(), enRpMtError, _T("Disconnect from Price provider: %d %s"),enErrorNumber, (LPCTSTR)_bstr_t(bsDescription));
		SetEvent(m_hDisconnect);
	}

	void CEodLoadFutOptClosePriceFromPp::_Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
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
	
	void CEodLoadFutOptClosePriceFromPp::_PrepareIndexCollection(EODMS::CMsUndByIDCollPtr pUndColl)
	{
		for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
		{
			EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(iterUnd->second));

			if(spIndex)
			{
				if(spIndex->FutureOptionWithPos()->size())
					m_spIndexColl->insert(EODMS :: CMsIndexByIdColl::value_type (spIndex->m_nID, spIndex));
			}
		}
	}

	void CEodLoadFutOptClosePriceFromPp::_GetUpdatedContracts(long& nIndexCount, long& nOptCount, long& nTotalOptCount)
	{
		nIndexCount = nOptCount = nTotalOptCount = 0L;
		for(EODMS::CMsIndexByIdColl::iterator iterIndex = m_spIndexColl->begin(); iterIndex != m_spIndexColl->end(); iterIndex++)
		{
			EODMS::CMsIndexPtr spIndex(iterIndex->second);
			ATLASSERT(spIndex);

			if(spIndex->FutureOptionWithPos()->size())
			{
				if(spIndex->m_nFutOptClosePriceUpdated)
				{
					nIndexCount++;
					nOptCount += spIndex->m_nFutOptClosePriceUpdated;
				}
				nTotalOptCount += static_cast<long>( spIndex->FutureOptionWithPos()->size());
			}
		}
	}
	void CEodLoadFutOptClosePriceFromPp::_IncrementImportFuturesProgress()
	{
		size_t nAllIndex = m_spIndexColl->size();

		double dPersent = static_cast<double>(++m_nLoadedIndex) / nAllIndex;

		size_t nProgress = m_nStartProgress + static_cast<long>(dPersent * (100L - m_nStartProgress));

		if(m_nProgress != nProgress)
		{
			m_nProgress = nProgress;
			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
		}
	}
	void CEodLoadFutOptClosePriceFromPp::SetType(byte type)
	{
		bType = type;
	}
}