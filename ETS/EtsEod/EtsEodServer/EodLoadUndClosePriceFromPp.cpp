#include "StdAfx.h"
#include "EodLoadUndClosePriceFromPp.h"

namespace EOD
{

CEodLoadUndClosePriceFromPp::~CEodLoadUndClosePriceFromPp(void)
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

void CEodLoadUndClosePriceFromPp::CancelRequest()
{
	Disconnect();
}

void CEodLoadUndClosePriceFromPp::LoadFromPp()
{
	Report(StepMask(),enRpMtMessage, _T("Loading underlying close price from Price Provider..."));

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
						_RequestUnderlyingPrice();
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

			long nUndCount = _GetUpdatedUnderlyings(m_spUndColl);
			Report(StepMask(),enRpMtMessage, _T("Loaded close price for %d underlying of %d requested"), nUndCount + m_nUpdatedUnd, m_spUndColl->size() + m_nUpdatedUnd);
		}
		else
		{
			m_nProgress = 90L;
			Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
			CEodServer::s_pEodServer->StepsMgr()->IncProgress();
			Report(StepMask(),enRpMtMessage, _T("There are no underlyings with net positions exists in DB"));
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

void CEodLoadUndClosePriceFromPp::_RequestUnderlyingPrice()
{
	m_spUndCurr  = m_itNextUnd->second;
	ATLASSERT(m_spUndCurr);

	EODPP::CPpRequestDataPtr spReqData(new EODPP::CPpRequestData);
	EODPP::CPpPriceRequestKey spReq;
	m_spUndCurr->MakePriceRequestKey(spReq);
	spReqData->Reqs()->insert(spReq);
	SetRequestData(spReqData);
	RequestLastQuote();
}

void CEodLoadUndClosePriceFromPp::OnLastQuote(EODPP::CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult)
{
	if(IsStopping())
		return;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("%s"), pReq->m_sSymbol);
	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	ATLASSERT(pReq->m_enType == PP::enSTK || pReq->m_enType == PP::enIDX);

	ATLASSERT(m_spUndCurr->m_sSymbol == pReq->m_sSymbol);

	if(m_spUndCurr->m_sSymbol == pReq->m_sSymbol)
	{
		if(!EODCO::IsBadValue(pResult->ClosePrice))
		{
			try
			{
				CStoredProc<> spPriceSave(m_spDB, _T("usp_EodUndClosePrice_Save"));
				spPriceSave << m_spUndCurr->m_nID;
				spPriceSave << pResult->ClosePrice;
				spPriceSave.Execute();
				m_spUndCurr->m_dPriceClose = pResult->ClosePrice;
				m_spUndCurr->m_bUndClosePriceUpdated = true;
			}
			catch (exception& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUndClosePrice_Save : %s, %s"), typeid(e).name(), e.what());
			}
		}
		else
			Report(StepMask(),enRpMtWarning, _T("Underlying %s has no close price on Price Provider"), (LPCTSTR)m_spUndCurr->m_sSymbol);
	}
}

void CEodLoadUndClosePriceFromPp::OnLastQuoteDone()
{
	if(IsStopping())
		return;

	m_itNextUnd++;

	_IncrementProgress();
	SetEvent(m_hUndReceived);
}

void CEodLoadUndClosePriceFromPp::OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription, EODPP::CPpPriceRequestKey* pReq  )
{
	if(IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	Report(StepMask(), enRpMtWarning, _T("Underlying close price uploading error %d - %s for '%s'"), enErrorNumber, (LPCTSTR)_bstr_t(bsDescription), (LPCTSTR)pReq->m_sSymbol);
}

void CEodLoadUndClosePriceFromPp::OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(IsStopping())
		return;
	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
}

void CEodLoadUndClosePriceFromPp::OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription )
{
	if(IsStopping())
		return;

	Trace(EODCO::enTrLogError,__FUNCTION__,(LPCTSTR)_bstr_t(bsDescription));
	Report(StepMask(), enRpMtError, _T("Disconnect from Price provider: %d %s"),enErrorNumber, (LPCTSTR)_bstr_t(bsDescription));
	SetEvent(m_hDisconnect);
}

void CEodLoadUndClosePriceFromPp::_Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
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
				Report(StepMask(), enRpMtWarning, _T("Error connect to Price provider: %s. Try to reconnect"), e.what());
		}
	}
}
EODMS::CMsUndByIDCollPtr CEodLoadUndClosePriceFromPp::_PrepareUndCollection(EODMS::CMsUndByIDCollPtr pUndColl)
{
	EODMS::CMsUndByIDCollPtr spUndColl(new EODMS::CMsUndByIDColl);
	m_nUpdatedUnd = 0L;
	for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);
		
		bool bHasFuturesOptPos = false;
		EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(spUnd));
		if(spIndex)
			bHasFuturesOptPos = (spIndex->FutureOptionWithPos()->size() != 0L);

		if(!EODCO::IsBadValue(spUnd->m_dPos) || spUnd->m_bHasSynthOptWithPos || spUnd->Opt()->size() || bHasFuturesOptPos)
		{
			if(!spUnd->m_bUndClosePriceUpdated)
				spUndColl->insert(EODMS :: CMsUndByIDPair (spUnd->m_nID, spUnd));
			else
				m_nUpdatedUnd++;
		}
	}
	return spUndColl;
}

long CEodLoadUndClosePriceFromPp::_GetUpdatedUnderlyings(EODMS::CMsUndByIDCollPtr pUndColl)
{
	long nCount = 0L;
	for(EODMS::CMsUndByIDColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->m_bUndClosePriceUpdated)
			nCount++;
	}
	return nCount;
}
void CEodLoadUndClosePriceFromPp::_IncrementProgress()
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