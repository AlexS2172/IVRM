#include "StdAfx.h"
#include "EodLoadUndClosePriceFromIv.h"

namespace EOD
{

void CEodLoadUndClosePriceFromIv::LoadFromIv()
{
	Report(StepMask(),enRpMtMessage, _T("Loading underlying close price from IVolatility..."));
	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	EODMS::CMsUndCollPtr spUndColl(_PrepareUndCollection(spMktStrMgr->UndColl()));

	if(spUndColl && spUndColl->size())
	{
		EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

		m_Request.Init(spSettingMgr, spUndColl);

		RequestIvData(&m_Request);

		long nUndCount = _GetUpdatedUnderlyings(spUndColl);
		m_nUndLoaded = 0L;

		if(spUndColl->size() != nUndCount)
		{
			CString sDescr;
			sDescr.Format(_T("Loaded underlyings close price from IVolatility for %d underlying of %d requested"),
				nUndCount, spUndColl->size());

			throw exception(sDescr);
		}
		else
			Report(StepMask(),enRpMtMessage, _T("Loaded close price for %d underlying"), nUndCount);
	}
	else
	{
		m_nProgress = 90L;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
		Report(StepMask(),enRpMtMessage, _T("There are no underlyings with net positions exists in DB"));
	}
}

void CEodLoadUndClosePriceFromIv::OnIvData(EODIV::CIvUndClosePriceResultPtr pResult)
{	
	if(_IsCancel() || IsStopping())
		throw EODCO::CEodStopException();
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	EODMS::CMsUndCollPtr spUndColl = m_Request.Und();

	ATLASSERT(spUndColl);

	EODMS::CMsUndPtr spUnd(GetCollectionItem<EODMS::CMsUndPtr>(spUndColl, pResult->m_strUndSymbol));

	ATLASSERT(spUnd);

	if(spUnd)
	{
		if(!EODCO::IsBadValue(pResult->m_dClosePrice))
		{
			try
			{
				CStoredProc<> spPriceSave(m_spDB, L"usp_EodUndClosePrice_Save");
				spPriceSave << spUnd->m_nID;
				spPriceSave << pResult->m_dClosePrice;
				spPriceSave.Execute();
				spUnd->m_dPriceClose = pResult->m_dClosePrice;
				spUnd->m_bUndClosePriceUpdated = true;
			}
			catch (exception& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodUndClosePrice_Save : %s, %s"), typeid(e).name(), e.what());
			}

			_IncrementProgress(spUndColl);
		}
		else
		{
			Report(StepMask(),enRpMtMessage, _T("Can't upload underlying '%s' close price from IVolatility database . Try to upload later from Price Provider"), (LPCTSTR)spUnd->m_sSymbol);
		}

	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

EODMS::CMsUndCollPtr CEodLoadUndClosePriceFromIv::_PrepareUndCollection(EODMS::CMsUndCollPtr pUndColl)
{
	EODMS::CMsUndCollPtr spUndColl(new EODMS::CMsUndColl);
	for(EODMS::CMsUndColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		bool bHasFuturesOptPos = false;
		EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(spUnd));
		if(spIndex)
			bHasFuturesOptPos = (spIndex->FutureOptionWithPos()->size() != 0L);

		if(!EODCO::IsBadValue(spUnd->m_dPos) || spUnd->m_bHasSynthOptWithPos || spUnd->Opt()->size() || bHasFuturesOptPos)
		{
			spUndColl->insert(EODMS::CMsUndColl::value_type(spUnd->m_sSymbol, spUnd));
		}
	}
	return spUndColl;
}

long CEodLoadUndClosePriceFromIv::_GetUpdatedUnderlyings(EODMS::CMsUndCollPtr pUndColl)
{
	long nCount = 0L;
	for(EODMS::CMsUndColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->m_bUndClosePriceUpdated)
			nCount++;
	}
	return nCount;
}

void CEodLoadUndClosePriceFromIv::_IncrementProgress(EODMS::CMsUndCollPtr pUndColl)
{
	size_t nAllUnd = pUndColl->size();
	size_t nProgress = 90L * ++m_nUndLoaded / nAllUnd;

	if(m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
	}
}

}