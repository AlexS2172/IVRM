#include "StdAfx.h"
#include "EodLoadOptClosePriceFromIv.h"

namespace EOD
{

void CEodLoadOptClosePriceFromIv::LoadFromIv()
{
	Report(StepMask(),enRpMtMessage, _T("Loading options close price from IVolatility..."));

	EOD::CEodMktStructureMgrPtr spMktStrMgr = CEodServer::s_pEodServer->MktStructureMgr();

	EODMS::CMsUndCollPtr spUndColl(_PrepareUndCollection(spMktStrMgr->UndColl()));

	if(spUndColl && spUndColl->size())
	{
		EODSM::CSmSettingsMgrPtr spSettingMgr(CEodServer::s_pEodServer->SettingsMgr());

		m_Request.Init(spSettingMgr, spUndColl);

		m_spCurrentUnd.reset();
		m_nUndLoaded = 0L;

		RequestIvData(&m_Request);

		if(m_spCurrentUnd && m_spCurrentUnd->m_nOptClosePriceUpdated != m_spCurrentUnd->Opt()->size())
			Report(StepMask(),enRpMtMessage, _T("Can't upload all options close prices for %s underlying from IVolatility. Try to upload later from Price Provider"), (LPCTSTR)m_spCurrentUnd->m_sSymbol);

		long nUndCount, nOptCount, nTotalOptCount;
		_GetUpdatedContracts(spUndColl, nUndCount, nOptCount, nTotalOptCount);

		if(nTotalOptCount != nOptCount)
		{
			CString sDescr;
			sDescr.Format(_T("Loaded close price from IVolatility for %d options of %d underlying out of %d requested options of %d underlying"),
				nOptCount, nUndCount, nTotalOptCount, spUndColl->size());

			throw exception(sDescr);
		}
		else
			Report(StepMask(),enRpMtMessage, _T("Loaded close price for %d options of %d underlying"),nOptCount, nUndCount);
	}	
	else
	{
		m_nProgress = 90L;
		Report(StepMask(),enRpMtProgress, _T("%d"), m_nProgress);
		CEodServer::s_pEodServer->StepsMgr()->IncProgress();
		Report(StepMask(),enRpMtMessage, _T("There are no options with positions exists in DB"));
	}
}

void CEodLoadOptClosePriceFromIv::OnIvData(EODIV::CIvOptClosePriceResultPtr pResult)
{	
	if(_IsCancel() || IsStopping())
		throw EODCO::CEodStopException();
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	EODMS::CMsUndCollPtr spUndColl = m_Request.Und();

	ATLASSERT(spUndColl);

	if(spUndColl)
	{
		EODMS::CMsUndPtr spUnd(GetCollectionItem<EODMS::CMsUndPtr>(spUndColl, pResult->m_strUndSymbol));

		ATLASSERT(spUnd);

		if(spUnd)
		{
			EODMS::CMsOptionByKeyCollPtr spOptColl(spUnd->OptByKey());

			ATLASSERT(spOptColl);

			if(spOptColl)
			{
				if(!(pResult->m_aKey == EODMS::badOptionKey))
				{
					EODMS::CMsOptionPtr spOpt(GetCollectionItem<EODMS::CMsOptionPtr>(spOptColl, pResult->m_aKey));

					if(spOpt && !EODCO::IsBadValue(pResult->m_dClosePrice) && !spOpt->m_bClosePriceUpdated)
					{
						try
						{
							CStoredProc<> spPriceSave(m_spDB, L"usp_EodOptClosePrice_Save");
							spPriceSave << spOpt->m_nID;
							spPriceSave << pResult->m_dClosePrice;
							spPriceSave.Execute();

							spOpt->m_dPriceClose = pResult->m_dClosePrice;
							spOpt->m_bClosePriceUpdated = true;
							spUnd->m_nOptClosePriceUpdated++;
						}
						catch (exception& e)
						{
							Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodOptClosePrice_Save : %s, %s"), typeid(e).name(), e.what());
						}
					}
					
					if(!m_spCurrentUnd || m_spCurrentUnd->m_nID != spUnd->m_nID)
					{

						if(m_spCurrentUnd && m_spCurrentUnd->m_nOptClosePriceUpdated != m_spCurrentUnd->Opt()->size())
							Report(StepMask(),enRpMtMessage, _T("Can't upload all options close prices for %s underlying from IVolatility. Try to upload later from Price Provider"), (LPCTSTR)m_spCurrentUnd->m_sSymbol);

						m_spCurrentUnd = spUnd;

						_IncrementProgress(spUndColl);
					}
				}
				else
					Report(StepMask(),enRpMtMessage, _T("Can't upload options close prices for %s underlying from IVolatility. Try to upload later from Price Provider"), (LPCTSTR)spUnd->m_sSymbol);
			}
		}
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

EODMS::CMsUndCollPtr CEodLoadOptClosePriceFromIv::_PrepareUndCollection(EODMS::CMsUndCollPtr pUndColl)
{
	EODMS::CMsUndCollPtr spUndColl(new EODMS::CMsUndColl);
	for(EODMS::CMsUndColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->Opt()->size())
		{
			spUndColl->insert(EODMS::CMsUndColl::value_type(spUnd->m_sSymbol, spUnd));
		}
	}
	return spUndColl;
}

void CEodLoadOptClosePriceFromIv::_GetUpdatedContracts(EODMS::CMsUndCollPtr pUndColl, long& nUndCount, long& nOptCount, long& nTotalOptCount)
{
	nUndCount = nOptCount = nTotalOptCount = 0L;
	for(EODMS::CMsUndColl::iterator iterUnd = pUndColl->begin(); iterUnd != pUndColl->end(); iterUnd++)
	{
		EODMS::CMsUndPtr spUnd(iterUnd->second);
		ATLASSERT(spUnd);

		if(spUnd->m_nOptClosePriceUpdated)
		{
			nUndCount++;
			nOptCount += spUnd->m_nOptClosePriceUpdated;
		}
		nTotalOptCount += static_cast<long>(spUnd->Opt()->size());
	}
}
void CEodLoadOptClosePriceFromIv::_IncrementProgress(EODMS::CMsUndCollPtr pUndColl)
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