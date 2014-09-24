#include "StdAfx.h"
#include "EodLoadOptClosePriceFromPp.h"

namespace EOD
{

CEodLoadOptClosePriceFromPp::~CEodLoadOptClosePriceFromPp(void)
{
	if(m_hUndReceived)
	{
		CloseHandle(m_hUndReceived);
		m_hUndReceived = NULL;
	}
	if(m_hDisconnect)
	{
		CloseHandle(m_hDisconnect);
		m_hDisconnect = NULL;
	}
}

void CEodLoadOptClosePriceFromPp::CancelRequest()
{
	Disconnect();
}

void CEodLoadOptClosePriceFromPp::LoadFromPp()
{
	Report(StepMask(),enRpMtMessage, _T("Loading options close price from Price Provider..."));

	bool bIsStopped = false;
	try
	{
		EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

		m_spUndColl = _PrepareUndCollection(spMktStrMgr->UndByIdColl());

		if(m_spUndColl && m_spUndColl->size())
		{
			EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

			_Connect(spSettingMgr);

			m_itNextUnd = m_spUndColl->begin();
			ATLASSERT(m_itNextUnd != m_spUndColl->end());

			HANDLE hStopEvent = GetStopEventHandle();

			m_hUndReceived = CreateEvent(NULL,FALSE,FALSE,NULL);
			m_hDisconnect  = CreateEvent(NULL,FALSE,FALSE,NULL);

			const HANDLE hEvents[] = {hStopEvent, m_hUndReceived, m_hDisconnect};

			SetEvent(m_hUndReceived);

			while(true)
			{
				DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE );

				if(dwRes == WAIT_OBJECT_0)
					throw EODCO::CEodStopException();
				else if(dwRes == WAIT_OBJECT_0 + 1)
				{
					if(m_itNextUnd == m_spUndColl->end())
						break;
					else
					{
						_RequestOptionsPrice();
					}
				}
				else if(dwRes == WAIT_OBJECT_0 + 2)
				{
					_Connect(spSettingMgr);
					SetEvent(m_hUndReceived);
				}
			}

			Disconnect();

			if(m_hUndReceived)
			{
				CloseHandle(m_hUndReceived);
				m_hUndReceived = NULL;
			}
			if(m_hDisconnect)
			{
				CloseHandle(m_hDisconnect);
				m_hDisconnect = NULL;
			}

			long nUndCount, nOptCount, nTotalOptCount;
			_GetUpdatedContracts(m_spUndColl, nUndCount, nOptCount, nTotalOptCount);
			Report(StepMask(),enRpMtMessage, _T("Loaded close price for %d options of %d underlying out of %d requested options of %d underlying"),
				nOptCount + m_nUpdatedOpt, nUndCount + m_nUpdatedUnd, nTotalOptCount + m_nUpdatedOpt,  m_spUndColl->size() + m_nUpdatedUnd);

		}
		else
		{
			m_nProgress = 90L;
			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
			Report(StepMask(),enRpMtMessage, _T("There are no options with positions exists in DB"));
		}
	}
	catch(EODCO::CEodStopException)
	{
		bIsStopped = true;
	}
	catch (exception& e)
	{
		Disconnect();

		if(m_hUndReceived)
		{
			CloseHandle(m_hUndReceived);
			m_hUndReceived = NULL;
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

void CEodLoadOptClosePriceFromPp::_RequestOptionsPrice()
{
	m_spUndCurr  = m_itNextUnd->second;
	ATLASSERT(m_spUndCurr);

	EODPP::CPpRequestDataPtr spReqData(new EODPP::CPpRequestData);

	EODMS::CMsOptionCollPtr spOptColl(m_spUndCurr->Opt());
	ATLASSERT(spOptColl);

	for(EODMS::CMsOptionColl::iterator iterOpt = spOptColl->begin(); iterOpt != spOptColl->end(); iterOpt++)
	{
		EODMS::CMsOptionPtr spOpt(iterOpt->second);
		ATLASSERT(spOpt);

		if(!spOpt->m_bClosePriceUpdated)
		{
			EODPP::CPpPriceRequestKey aReq;
			spOpt->MakePriceRequestKey(aReq);
			spReqData->Reqs()->insert(aReq);
		}
	}
	EODPP::CPpPriceRequestKey aGrReq;
	m_spUndCurr->MakeGroupPriceRequestKey(aGrReq);
	spReqData->UndGroupReqs()->insert(aGrReq);

	SetRequestData(spReqData);
	RequestLastQuote();
}

void CEodLoadOptClosePriceFromPp::OnLastQuote(EODPP::CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult)
{
	if(IsStopping())
		return;

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), (LPCTSTR)pReq->m_sSymbol);

	EODMS::CMsOptionCollPtr spOptColl(m_spUndCurr->Opt());
	ATLASSERT(spOptColl);
	
	EODMS::CMsOptionPtr spOpt(GetCollectionItem<EODMS::CMsOptionPtr>(spOptColl, pReq->m_sSymbol));

	if(spOpt && !spOpt->m_bClosePriceUpdated)
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
				spOpt->m_bClosePriceUpdated = true;
				m_spUndCurr->m_nOptClosePriceUpdated++;
			}
			catch (exception& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptClosePrice_Save : %s, %s"), typeid(e).name(), e.what());
			}
		}
		else
			Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("'%s' option of '%s' underlying has no close price on Price Provider"), (LPCTSTR)spOpt->m_sSymbol, (LPCTSTR)m_spUndCurr->m_sSymbol);
	}
}

void CEodLoadOptClosePriceFromPp::OnLastQuoteDone()
{
	if(IsStopping())
		return;

	m_itNextUnd++;

	_IncrementProgress();

	if(m_spUndCurr->m_nOptClosePriceUpdated != m_spUndCurr->Opt()->size())
		Report(StepMask(),enRpMtMessage, _T("Can't upload all options close prices for %s underlying from Price Provider"), (LPCTSTR)m_spUndCurr->m_sSymbol);

	SetEvent(m_hUndReceived);
}

void CEodLoadOptClosePriceFromPp::OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, EODPP::CPpPriceRequestKey* pReq  )
{
	if(IsStopping())
		return;

	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	Report(StepMask(), enRpMtWarning, _T("Option close price uploading error %d - %s for '%s'"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCTSTR)pReq->m_sSymbol);
}

void CEodLoadOptClosePriceFromPp::OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
}

void CEodLoadOptClosePriceFromPp::OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(IsStopping())
		return;

	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	Report(StepMask(), enRpMtError, _T("Disconnect from Price provider: %d %s"),enErrorNumber, (LPCTSTR)_bstr_t(bsDescription));
	SetEvent(m_hDisconnect);
}

void CEodLoadOptClosePriceFromPp::_Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
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
EODMS::CMsUndByIDCollPtr CEodLoadOptClosePriceFromPp::_PrepareUndCollection(EODMS::CMsUndByIDCollPtr pUndColl)
{
	EODMS::CMsUndByIDCollPtr spUndColl(new EODMS::CMsUndByIDColl);
	m_nUpdatedUnd = 0L;
	m_nUpdatedOpt = 0L;

	for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		size_t nOptCount = spUnd->Opt()->size();
		if(nOptCount)
		{
			if(spUnd->m_nOptClosePriceUpdated != nOptCount)
			{
				spUndColl->insert(EODMS :: CMsUndByIDColl::value_type(spUnd->m_nID, spUnd));
			}
			else
			{
				m_nUpdatedOpt += spUnd->m_nOptClosePriceUpdated;
				m_nUpdatedUnd++;
			}

		}
	}
	return spUndColl;
}

void CEodLoadOptClosePriceFromPp::_GetUpdatedContracts(EODMS::CMsUndByIDCollPtr pUndColl, long& nUndCount, long& nOptCount, long& nTotalOptCount)
{
	nUndCount = nOptCount = nTotalOptCount = 0L;
	for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->Opt()->size())
		{
			if(spUnd->m_nOptClosePriceUpdated)
			{
				nUndCount++;
				nOptCount += spUnd->m_nOptClosePriceUpdated;
			}
			nTotalOptCount += static_cast<long>(spUnd->Opt()->size());
		}
	}
}
void CEodLoadOptClosePriceFromPp::_IncrementProgress()
{
	size_t nAllUnd = m_spUndColl->size() + m_nUpdatedUnd;

	size_t nProgress = 90L * ( ++m_nLoadedUnd + m_nUpdatedUnd ) / nAllUnd;

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}

}