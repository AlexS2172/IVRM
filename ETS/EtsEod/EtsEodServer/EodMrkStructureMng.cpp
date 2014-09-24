#include "StdAfx.h"
#include "EodMrkStructureMng.h"

namespace EOD
{

/////////////////////////////////////////////////////////////////////////////
//
CEodMktStructureMgr::~CEodMktStructureMgr(void)
{
	try
	{
		CleanUp();
	}
	catch(...)
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::Init(EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CAutoLock lock(m_csObjectLock);

	if(!pSettingsMgr)
		throw exception(_T("Invalid settings manager object."));

	try{
		CleanUp();
		m_spDB.Open(pSettingsMgr->DbConnection().GetString(), pSettingsMgr->DbConnectionTimeout(), /*pSettingsMgr->DbCommandTimeout()*/250);
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in Init: %s"), (LPCSTR)e.Description());
		throw exception((LPCSTR)e.Description());
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::CleanUp()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CAutoLock lock(m_csObjectLock);

	try
	{
		_ClearStorage();
		m_spDB.Close();
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in CleanUp: %s"), (LPCSTR)e.Description());
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_ClearStorage()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	m_spUndColl->clear();
	m_spRateColl->clear();
	m_spUndByIDColl->clear();
	m_spExpDateColl->clear();
	m_spFutRootColl->clear();

	m_spCommodityFutRootColl->clear();
	m_spInterestFutRootColl->clear();
	
/*	m_spInterestColl->clear();
	m_spInterestByIDColl->clear();
	m_spCommodityFXColl->clear();
	m_spCommodityFXByIDColl->clear();
*/
	m_spCommIRFXFuturesColl->clear();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
/*void CEodMktStructureMgr::_ClearCommodity()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	m_spCommodityFXColl->clear();
	m_spCommodityFXByIDColl->clear();
//	m_spCommodityFutRootColl->clear();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_ClearInterest()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	m_spInterestColl->clear();
	m_spInterestByIDColl->clear();
	//m_spInterestFutRootColl->clear();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
*/
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_ClearUnderlyingsPositionsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	for(EODMS::CMsUndByIDColl::iterator iter = m_spUndByIDColl->begin(); iter != m_spUndByIDColl->end(); iter++)
	{
		EODMS::CMsUndPtr spUnd(iter->second);
		ATLASSERT(spUnd);

		if(spUnd)
		{
			spUnd->m_dPos = BAD_DOUBLE_VALUE;
			spUnd->m_bHasSynthOptWithPos = false;
			spUnd->Opt()->clear();
			spUnd->OptRootWithPos()->clear();
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_ClearFuturesPositionsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	for(EODMS::CMsUndByIDColl::iterator iter = m_spUndByIDColl->begin(); iter != m_spUndByIDColl->end(); iter++)
	{
		EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(iter->second));
		if(spIndex)
		{
			spIndex->FutureOptionWithPos()->clear();
			spIndex->FutureWithPos()->clear();
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_ClearUnderlyingsExpirationData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	for(EODMS::CMsUndByIDColl::iterator iter = m_spUndByIDColl->begin(); iter != m_spUndByIDColl->end(); iter++)
	{
		EODMS::CMsUndPtr spUnd(iter->second);
		ATLASSERT(spUnd);

		if(spUnd)
			spUnd->EmptyIvExp()->clear();
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_ClearOptionRootsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	for(EODMS::CMsUndByIDColl::iterator iter = m_spUndByIDColl->begin(); iter != m_spUndByIDColl->end(); iter++)
	{
		EODMS::CMsUndPtr spUnd(iter->second);
		ATLASSERT(spUnd);

		if(spUnd)
			spUnd->OptRoot()->clear();
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//
/*void CEodMktStructureMgr::LoadCommodities()
{
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
		CAutoLock lock(&ObjectLock());

		_CheckDbConnection();
		_ClearCommodity();

		try
		{
			_LoadCommoditiesFX();
			_LoadCommodityFuturesRootsData();
		}
		catch(exception& e)
		{
			_ClearCommodity();
			throw exception(e);
		}
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));

}

void CEodMktStructureMgr::LoadInterests()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CAutoLock lock(&ObjectLock());

	_CheckDbConnection();
	_ClearInterest();
	try
	{
		_LoadCommoditiesFX();
		_LoadCommodityFuturesRootsData();

	}
	catch(exception& e)
	{
		_ClearInterest();
		throw exception(e);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));

}
*/

void CEodMktStructureMgr::LoadBaseData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CAutoLock lock(m_csObjectLock);

	_CheckDbConnection();
	_ClearStorage();

	try
	{
		_LoadRates();
		_LoadUnderlyingsBaseData();
		_LoadBasketIndicesData();
		_InitBasketDividends();
		_LoadFuturesRootsData();
		_LoadCommodityFuturesRootsData();
		_LoadInterestFuturesRootsData();
	}
	catch(exception& e)
	{
		_ClearStorage();
		throw exception(e);
	}
	catch(_com_error& e)
	{
		_ClearStorage();
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
// 
void CEodMktStructureMgr::LoadPositionsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CAutoLock lock(m_csObjectLock);

	try
	{
		_CheckDbConnection();

		_ClearUnderlyingsPositionsData();
		_LoadUnderlyingsWithPosition();
		_LoadUnderlyingsWithOptionPosition();

		_ClearFuturesPositionsData();
		_LoadFuturesWithPosition();
		_LoadFuturesOptionPosition();

		_UnderlyingsAdjustRates();

		_CommodityFuturesAdjustRates();
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in LoadPositionsData  : %d, %s"), e.Error(), (LPCSTR)e.Description());
		exception ex((LPCSTR)e.Description());
		throw exception(ex);
	}


	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::LoadEmptyIvExpirations()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	CAutoLock lock(m_csObjectLock);

	_CheckDbConnection();

	_ClearUnderlyingsExpirationData();

	try
	{
		CStoredProc<CClientRecordset> rsEmptyIv(m_spDB, L"usp_EodUndExpirationsWithEmptyIvSurface_Get");
		rsEmptyIv.Open();

		while(!rsEmptyIv.IsEOF())
		{			
			EODMS::CMsUndPtr spUnd(GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, static_cast<long>(rsEmptyIv[L"iContractID"])));
			ATLASSERT(spUnd);

			EODMS::CMsUndExpPtr spExp(new EODMS::CMsUndExp);
			spExp->m_bHasVolaPoints = false;
			spExp->m_nExpiryID = rsEmptyIv[L"iExpiryID"];

			spUnd->EmptyIvExp()->insert(EODMS::CMsUndExpColl::value_type (spExp->m_nExpiryID, spExp));

			++rsEmptyIv;
		}
	}
	catch(exception& e)
	{
		_ClearUnderlyingsExpirationData();
		throw exception(e);
	}
	catch (...) 
	{
		_ClearUnderlyingsExpirationData();

		throw exception("LoadEmptyIvExpirations unknown exception");
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::LoadOptionRootsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	CAutoLock lock(m_csObjectLock);

	_CheckDbConnection();

	_ClearOptionRootsData();

	try
	{
		EODMS::CMsUndPtr spUnd;
		EODMS::CMsOptRootByNameCollPtr spRootColl;

		long nRootID = 0L;

		CStoredProc<CClientRecordset> rsRoot(m_spDB, L"usp_EodOptionRoot_Get");
		rsRoot.Open();

		while(!rsRoot.IsEOF())
		{
			long nUndID = static_cast<long>(rsRoot[L"iUnderlyingID"]);

			if(!spUnd || spUnd->m_nID !=nUndID)
			{
				spUnd = GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, nUndID);
				ATLASSERT(spUnd);
				spRootColl = spUnd->OptRoot();
			}
			ATLASSERT(spUnd && spRootColl);

			CString sName = (LPCSTR) ((_bstr_t)rsRoot[L"vcSymbol"]);
			sName.Trim().MakeUpper();
			if(!IsExists(spRootColl,sName))
			{
				nRootID = rsRoot[L"iRootID"];
				EODMS::CMsOptRootPtr spRoot;

				if(!static_cast<bool>(rsRoot[L"tiIsSynthetic"]))
					spRoot = EODMS::CMsOptRootPtr(new EODMS::CMsOptRoot);
				else
				{
					EODMS::CMsSynthRootPtr spSynthRoot(new EODMS::CMsSynthRoot);

					spSynthRoot->m_dCashValue = rsRoot[L"fCash"];
					spSynthRoot->m_dSkew = rsRoot[L"fSyntheticSkew"];
					spSynthRoot->m_dKurt = rsRoot[L"fSyntheticKurt"];
					spSynthRoot->m_bIsBasket = rsRoot[L"tiSyntheticIsBasket"];
					spSynthRoot->m_dYield = rsRoot[L"fSyntheticYield"];

					spRoot = spSynthRoot;
				}

				spRoot->m_nID = nRootID;
				spRoot->m_sName = sName;
				spRoot->m_nLotSize = rsRoot[_T("iLotSize")];

				spRootColl->insert(EODMS::CMsOptRootByNameColl::value_type (sName, spRoot));
			}

			++rsRoot;
		}
	}
	catch(exception& e)
	{
		_ClearOptionRootsData();
		throw e;
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}


/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_LoadRates()
{
	m_spRateColl->clear();

	long nCurveID = 0L;

	// interest rates curves
	{
		CStoredProc<CClientRecordset> rsCurve(m_spDB, L"usp_EodIrCurve_Get");
		rsCurve.Open();

		while(!rsCurve.IsEOF())
		{
			nCurveID = rsCurve[_T("iCurveID")];
			if(!IsExists(m_spRateColl, nCurveID))
			{
				EODMS::CMsRatePtr spRate(new EODMS::CMsRate);
				spRate->m_nID = nCurveID;
				spRate->m_enRule = static_cast<EODMS::MsRateRuleTypeEnum>((long)rsCurve[L"iRuleTypeID"]);
				spRate->m_dPosThreshold = rsCurve[L"mPositionThreshold"];

				m_spRateColl->insert(EODMS::CMsRateColl::value_type(nCurveID, spRate));
			}

			++rsCurve;
		}
	}

	// interest rates points
	if(m_spRateColl->size() > 0L)
	{
		CStoredProc<CClientRecordset> rsPoint(m_spDB, L"usp_EodIrPoint_Get");
		rsPoint.Open();

		while(!rsPoint.IsEOF())
		{
			EODMS::CMsRatePtr spRate(GetCollectionItem<EODMS::CMsRatePtr>(m_spRateColl, (long)rsPoint[L"iCurveID"]));
			if(spRate)
			{
				spRate->AddRate(rsPoint[L"iDTE"],
					rsPoint[L"fRateShort"],
					rsPoint[L"fRateLong"],
					rsPoint[L"fRateHTB"]);
			}

			++rsPoint;
		}
	}
}

/*
void CEodMktStructureMgr::_LoadInterests()
{
	long nUndID = 0L;

	CStoredProc<CDefaultRecordset> rsUnd(m_spDB, _T("usp_EodInterest_Get"));
	rsUnd.Open();

	while(!rsUnd.IsEOF())
	{
		nUndID = rsUnd[_T("iContractID")];

		if(!m_spInterestByIDColl->IsExists(nUndID))
		{
			//EODMS::CMsCommodityPtr spUnd;
			EODMS::CMsCommodityPtr spUnd(new EODCO::CEgObject<EODMS::CMsCommodity>);

			m_spInterestByIDColl->Add(nUndID, spUnd);

			// contract properties
			spUnd->m_nID = nUndID;

			spUnd->m_bIsActive = rsUnd[_T("tiIsActive")];
			spUnd->m_nGreeksCalcType = rsUnd[_T("GreeksCalcType")];

			m_spInterestColl->Add(spUnd->m_sSymbol, spUnd);
		}

		++rsUnd;
	}

}

void CEodMktStructureMgr::_LoadCommoditiesFX()
{
	long nUndID = 0L;

	CStoredProc<CDefaultRecordset> rsUnd(m_spDB, _T("usp_EodCommodity_Get"));
	rsUnd.Open();

	while(!rsUnd.IsEOF())
	{
		nUndID = rsUnd[_T("iContractID")];

		if(!m_spCommodityFXByIDColl->IsExists(nUndID))
		{
			//EODMS::CMsCommodityPtr spUnd;
			EODMS::CMsCommodityPtr spUnd(new EODCO::CEgObject<EODMS::CMsCommodity>);
			m_spCommodityFXByIDColl->Add(nUndID, spUnd);

			// contract properties
			spUnd->m_nID = nUndID;
			
			spUnd->m_bIsActive = rsUnd[_T("tiIsActive")];
			spUnd->m_nGreeksCalcType = rsUnd[_T("GreeksCalcType")];

			m_spCommodityFXColl->Add(spUnd->m_sSymbol, spUnd);
		}

		++rsUnd;
	}

}
*/
/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_LoadUnderlyingsBaseData()
{
	long nUndID = 0L;

	CStoredProc<CClientRecordset> rsUnd(m_spDB, L"usp_EodUnderlying_Get");
	rsUnd.Open();

	while(!rsUnd.IsEOF())
	{
		nUndID = rsUnd[L"iUnderlyingID"];

		if(!IsExists(m_spUndByIDColl,nUndID))
		{
			EODMS::CMsUndPtr spUnd;
			bool IsStock = ((long)rsUnd[L"iUnderlyingTypeID"]) == 2L;
			
			if(IsStock)
			{
				EODMS::CMsStockPtr spStock(new EODMS::CMsStock);

				// stock properties
				spStock->m_bUseCustDivs = rsUnd[L"tiIsDivCustom"];
				spStock->m_nDivFreq = rsUnd[L"iDivFreq"];
				spStock->m_nDivDate = static_cast<long>(rsUnd[L"dtDivDate"]);
				spStock->m_dDivAmt = rsUnd[L"fDivAmt"];
				spStock->m_nDivFreqCust = rsUnd[L"iDivFreqCustom"];
				spStock->m_nDivDateCust = static_cast<long>(rsUnd[L"dtDivDateCustom"]);
				spStock->m_dDivAmtCust = rsUnd[L"fDivAmtCustom"];

				spUnd = spStock;
			}
			else
			{
				if(!static_cast<bool>(rsUnd[L"tiIsBasket"]))
					spUnd = boost::shared_dynamic_cast<EODMS :: CMsUnd>(EODMS::CMsIndexPtr(new EODMS::CMsIndex));
				else
					spUnd = boost::shared_dynamic_cast<EODMS :: CMsUnd>(EODMS::CMsBasketIndexPtr(new EODMS::CMsBasketIndex));
			}

			m_spUndByIDColl->insert(EODMS :: CMsUndByIDColl::value_type(nUndID, spUnd));

			// contract properties
			spUnd->m_nID = nUndID;
			spUnd->m_sSymbol =(LPCSTR)((_bstr_t)rsUnd[L"vcSymbol"]);
			spUnd->m_sSymbol.Trim().MakeUpper();

			m_spUndColl->insert(EODMS :: CMsUndColl::value_type(spUnd->m_sSymbol, spUnd));

			// underlying properties
			spUnd->m_dtLastMsUpdate = rsUnd[("dtLastMsUpdate")];
			spUnd->m_bIsAmerican = rsUnd[L"tiCalcOptionType"];
			spUnd->m_bIsHTB = rsUnd[L"tiIsHTB"];
			spUnd->m_nLotSize = 1L;
			spUnd->m_dSkew = rsUnd[L"fSkew"];
			spUnd->m_dKurt = rsUnd[L"fKurt"];
			spUnd->m_dPriceClose = rsUnd[L"fPriceClose"];
			spUnd->m_bIsActive = rsUnd[L"tiIsActive"];
		}

		++rsUnd;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_LoadBasketIndicesData()
{
	long nCompCount = 0L, nBasketIndexID = 0L, nUndID = 0L;
	double dWeight = 0.;

	CStoredProc<CClientRecordset> rsComp(m_spDB,L"usp_EodBasketIndexComp_Get");
	rsComp.Open();

	while(!rsComp.IsEOF())
	{
		dWeight = rsComp[L"fWeight"];
		if(EODCO::DoubleNEQZero(dWeight))
		{
			nBasketIndexID = rsComp[L"iBasketIndexID"];
			nUndID = rsComp[L"iUnderlyingID"];
			
			ATLASSERT(IsExists(m_spUndByIDColl, nUndID));

			EODMS::CMsBasketIndexPtr spBasketIndex(boost::shared_dynamic_cast<EODMS::CMsBasketIndex>(GetCollectionItem<EODMS :: CMsUndPtr>(m_spUndByIDColl, nBasketIndexID)));
			if(spBasketIndex && IsExists(m_spUndByIDColl, nUndID))
			{
				EODMS::CMsUndWeightCollPtr spUndWeightColl(spBasketIndex->UndWeight());

				if(!IsExists(spUndWeightColl,nUndID))
				{
					EODMS::CMsUndWeightPtr spUndWeight(new EODMS::CMsUndWeight);
					spUndWeight->m_nUndID = nUndID;
					spUndWeight->m_dWeight = dWeight;

					spUndWeightColl->insert(EODMS::CMsUndWeightColl::value_type (nUndID, spUndWeight));
				}
			}
		}

		++rsComp;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_InitBasketDividends()
{
	for(EODMS::CMsUndByIDColl::iterator itUnd = m_spUndByIDColl->begin(); itUnd != m_spUndByIDColl->end(); itUnd++)
	{
		EODMS::CMsBasketIndexPtr spBasketIndex(boost::shared_dynamic_cast<EODMS::CMsBasketIndex>(itUnd->second));

		if(spBasketIndex)
			spBasketIndex->InitBasketDivs(m_spUndByIDColl);
	}
}

void CEodMktStructureMgr::_LoadUnderlyingsWithPosition()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		CStoredProc<CClientRecordset> rsGetUndPos(m_spDB, L"usp_EodUndWithPositions_Get");
		rsGetUndPos.Open();

		while(!rsGetUndPos.IsEOF())
		{			
			EODMS::CMsUndPtr spUnd(GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, static_cast<long>(rsGetUndPos[L"iUnderlyingID"])));
			ATLASSERT(spUnd);
			spUnd->m_dPos = rsGetUndPos[L"fPosition"];
			++rsGetUndPos;
		}
	}
	catch(exception& e)
	{
		_ClearUnderlyingsPositionsData();
		throw exception(e);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_LoadUnderlyingsWithOptionPosition()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		_LoadOptionsWithPositions();
		_LoadSynthRootComponentsWithPositions();
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in LoadUnderlyingsWithOptionPosition  : %d, %s"), e.Error(), (LPCSTR)e.Description() );
		_ClearUnderlyingsPositionsData();
		exception ex((LPCSTR)e.Description());
		throw exception(ex);
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadUnderlyingsWithOptionPosition  : %s, %s"), typeid(e).name(), e.what());
		_ClearUnderlyingsPositionsData();
		throw exception(e);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_UnderlyingsAdjustRates()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		EODMS::CMsUndByIDColl::iterator iterUnd = m_spUndByIDColl->begin();
		for(; iterUnd != m_spUndByIDColl->end(); iterUnd++)
		{			
			EODMS::CMsUndPtr spUnd(iterUnd->second);
			ATLASSERT(spUnd);

			bool bHasFuturesOptPos = false;
			EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(spUnd));
			if(spIndex)
				bHasFuturesOptPos = (spIndex->FutureOptionWithPos()->size() != 0L);

			if(spUnd->Opt()->size() || bHasFuturesOptPos)
			{
				_UnderlyingAdjustRates(spUnd);
				_InitSynthRootBasketDividends(spUnd);
			}
		}
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _UnderlyingsAdjustRates  : %s, %s"), typeid(e).name(), e.what());
		_ClearUnderlyingsPositionsData();
		_ClearFuturesPositionsData();
		throw exception(e);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_LoadSynthRootComponentsWithPositions()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	EODMS::CMsUndPtr spUnd;
	EODMS::CMsOptRootCollPtr spSynthRootColl;

	long nCompUndID = 0L;

	CStoredProc<CClientRecordset> rsComp(m_spDB, L"usp_EodSynthRootCompWithPosition_Get");
	rsComp.Open();

	while(!rsComp.IsEOF())
	{
		long nUndID = static_cast<long>(rsComp[L"iUnderlyingID"]);

		if(!spUnd || spUnd->m_nID !=nUndID)
		{
			spUnd = GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, nUndID);
			spSynthRootColl = spUnd->OptRootWithPos();
		}

		ATLASSERT(spUnd);

		nCompUndID = rsComp[L"iCompUndID"];
		
		if(!IsExists(m_spUndByIDColl,nCompUndID))
			throw exception(_T("Invalid synthetic component underlying."));

		EODMS::CMsUndPtr spSynthUnd(GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, nCompUndID));

		ATLASSERT(spSynthUnd);
		spSynthUnd->m_bHasSynthOptWithPos = true;

		EODMS::CMsSynthRootPtr spSynthRoot(boost::shared_dynamic_cast<EODMS::CMsSynthRoot>(GetCollectionItem<EODMS::CMsOptRootPtr>(spSynthRootColl, static_cast<long>(rsComp[L"iRootID"]))));
		if(spSynthRoot)
		{
			EODMS::CMsUndWeightCollPtr spSynthRootCompColl(spSynthRoot->SynthRootComp());

			if(!IsExists(spSynthRootCompColl, nCompUndID))
			{
				EODMS::CMsUndWeightPtr spSynthRootComp(new EODMS::CMsUndWeight);
				spSynthRootComp->m_nUndID = nCompUndID;
				spSynthRootComp->m_dWeight = rsComp[L"fWeight"];

				spSynthRootCompColl->insert(EODMS::CMsUndWeightColl::value_type(nCompUndID, spSynthRootComp));
			}
		}

		++rsComp;
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_LoadOptionsWithPositions()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	EODMS::CMsUndPtr spUnd;
	EODMS::CMsOptRootPtr spOptRoot;

	CStoredProc<CClientRecordset> rsOpt(m_spDB, L"usp_EodOptionWithPos_Get");
	rsOpt << CDBNull();
	rsOpt << vt_date::GetCurrentDate(true);
	rsOpt.Open();

	while(!rsOpt.IsEOF())
	{
		long nUndID = static_cast<long>(rsOpt[L"iUnderlyingID"]);
		
		if(!spUnd || spUnd->m_nID !=nUndID)
			spUnd = GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, nUndID);

		ATLASSERT(spUnd);

		long nRootID = static_cast<long>(rsOpt[L"iOptionRootID"]);

		if(!spOptRoot || spOptRoot->m_nID !=nRootID)
		{
			CString sRootName = (LPCSTR)((_bstr_t)rsOpt[L"vcOptionRootName"]);
			sRootName.Trim().MakeUpper();
			spOptRoot = spUnd->OptRoot(sRootName);

			if(!spOptRoot)
			{
				if(!static_cast<bool>(rsOpt[L"tiIsSynthetic"]))
					spOptRoot = EODMS :: CMsOptRootPtr (new EODMS::CMsOptRoot);
				else
				{
					EODMS::CMsSynthRootPtr spSynthRoot(new EODMS::CMsSynthRoot);

					spSynthRoot->m_dCashValue = rsOpt[L"fCash"];
					spSynthRoot->m_dSkew = rsOpt[L"fSyntheticSkew"];
					spSynthRoot->m_dKurt = rsOpt[L"fSyntheticKurt"];
					spSynthRoot->m_bIsBasket = rsOpt[L"tiSyntheticIsBasket"];
					spSynthRoot->m_dYield = rsOpt[L"fSyntheticYield"];

					spOptRoot = spSynthRoot;
				}

				spOptRoot->m_nID = nRootID;
				spOptRoot->m_sName = sRootName;
				spOptRoot->m_nLotSize = rsOpt[L"iLotSize"];
			}
			
			spUnd->OptRootWithPos()->insert(EODMS::CMsOptRootColl::value_type(nRootID, spOptRoot));
		}

		if(spOptRoot)
		{
			long nOptID = rsOpt[_T("iOptionID")];

			EODMS::CMsOptionByIDCollPtr spOptByIDColl(spOptRoot->Opt());
			ATLASSERT(spOptByIDColl);
			if(!IsExists(spOptByIDColl, nOptID))
			{
				EODMS::CMsOptionPtr spOpt(new EODMS::CMsOption);

				// contract properties
				spOpt->m_nID = nOptID;
				spOpt->m_sSymbol = (LPCSTR)((_bstr_t)rsOpt[L"vcSymbol"]);
				spOpt->m_sSymbol.Trim().MakeUpper();

				spOpt->m_bIsCall = rsOpt[L"tiIsCall"];
				spOpt->m_dStrike = rsOpt[L"fStrike"];
				spOpt->m_dPriceClose = rsOpt[L"fPriceClose"];
				spOpt->m_dtExpiration = rsOpt[L"dtExpiry"];

				spOptByIDColl->insert(EODMS::CMsOptionByIDColl::value_type(nOptID, spOpt));

				EODMS::CMsOptionCollPtr spOptColl(spUnd->Opt());
				ATLASSERT(spOptColl);

				if(!IsExists(spOptColl, spOpt->m_sSymbol))
					spOptColl->insert(EODMS::CMsOptionColl::value_type(spOpt->m_sSymbol, spOpt));

				EODMS::CMsOptionByKeyCollPtr spOptByKeyColl(spUnd->OptByKey());
				ATLASSERT(spOptByKeyColl);

				EODMS::CMsOptionKey aKey;
				spUnd->MakeOptionKey(spOptRoot, spOpt, aKey);

				if(!IsExists(spOptByKeyColl, aKey))
					spOptByKeyColl->insert(EODMS::CMsOptionByKeyColl::value_type (aKey, spOpt));
			}				
		}

		++rsOpt;
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEodMktStructureMgr::_UnderlyingAdjustRates(EODMS::CMsUndPtr pUnd)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	EODMS::CMsUndPtr spUnd(pUnd);

	if(m_spRateColl->size() > 0L)
	{
		const EODMS::CMsRatePtr& spRate(m_spRateColl->begin()->second);

		spUnd->ApplyRates(spRate);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_InitSynthRootBasketDividends(EODMS::CMsUndPtr pUnd)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	EODMS::CMsUndPtr spUnd(pUnd);

	EODMS::CMsOptRootCollPtr spOptRoots(spUnd->OptRootWithPos());

	if(spOptRoots && spOptRoots->size())
	{
		for(EODMS::CMsOptRootColl::iterator iterRoot = spOptRoots->begin(); iterRoot != spOptRoots->end(); iterRoot++)
		{
			EODMS::CMsSynthRootPtr spSynthRoot(boost::shared_dynamic_cast<EODMS::CMsSynthRoot>(iterRoot->second));
			if(spSynthRoot)
			{
				spSynthRoot->InitBasketDivs(m_spUndByIDColl);
			}
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_CommodityFuturesAdjustRates()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));


	if(m_spRateColl->size() > 0L)
	{
		const EODMS::CMsRatePtr& spRate(m_spRateColl->begin()->second);

		//spUnd->ApplyRates(spRate);
		EODMS::CMsFuturePtr spFutTmp;
		if (m_spCommIRFXFuturesColl != NULL)
			if (m_spCommIRFXFuturesColl->size() > 0)
			{
				for (EODMS::CMsFutureByIdColl::iterator itrFut = m_spCommIRFXFuturesColl->begin(); itrFut != m_spCommIRFXFuturesColl->end(); itrFut++ )
				{
					spFutTmp =  itrFut->second;
					if (spFutTmp != NULL)
						spFutTmp->ApplyRates(spRate);


				}
			}

	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::LoadExpiryDates()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	try
	{
		CStoredProc<CClientRecordset> rsExpiryGet(m_spDB,L"usp_EodExpirationDate_Get");
		rsExpiryGet.Open();
		
		long nToday = static_cast<long>(COleDateTime::GetCurrentTime());

		while(!rsExpiryGet.IsEOF())
		{			
			if(!IsExists(m_spExpDateColl, static_cast<DATE>(rsExpiryGet[L"dtExpiryDate"])))
			{
				EODMS::CMsExpDatePtr spExpDate(new EODMS::CMsExpDate);
				ATLASSERT(spExpDate);
				spExpDate->m_dtExpiration = rsExpiryGet[L"dtExpiryDate"];

				long nExpiry = static_cast<long>(spExpDate->m_dtExpiration);

				ATLASSERT(nExpiry > nToday);
				spExpDate->m_nDTE = nExpiry - nToday;
				m_spExpDateColl->insert(EODMS::CMsExpDateColl::value_type(spExpDate->m_dtExpiration,spExpDate));
			}
			++rsExpiryGet;
		}
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodExpirationDate_Get : %s, %s"), typeid(e).name(), e.what());
		throw exception(e);
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_LoadFuturesRootsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	try
	{
		CStoredProc<CClientRecordset> rsFutRootGet(m_spDB, L"usp_EodFutureRoot_Get");
		rsFutRootGet.Open();

		while(!rsFutRootGet.IsEOF())
		{			
			long nFutureRootId = static_cast<long>(rsFutRootGet[L"iFutureRootID"]);			
			if(!IsExists(m_spFutRootColl, nFutureRootId))
			{
				EODMS::CMsFuturesRootPtr spRoot(new EODMS::CMsFuturesRoot);
				ATLASSERT(spRoot);
				spRoot->m_nRootId = nFutureRootId;
				spRoot->m_sRootName = (LPCSTR)((_bstr_t)rsFutRootGet[L"vcFutureRootName"]);
				spRoot->m_sRootSymbol = (LPCSTR)((_bstr_t)rsFutRootGet[L"vcFutureRootSymbol"]);

				m_spFutRootColl->insert(EODMS :: CMsFuturesRootColl::value_type(nFutureRootId, spRoot));
			}
			++rsFutRootGet;
		}
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodFutureRoot_Get : %s, %s"), typeid(e).name(), e.what());
		throw exception(e);
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_LoadCommodityFuturesRootsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	try
	{
		CStoredProc<CDefaultRecordset> rsFutRootGet(m_spDB, L"usp_EodCommodityFutureRoot_Get");
		rsFutRootGet.Open();

		while(!rsFutRootGet.IsEOF())
		{			
			long nFutureRootId = static_cast<long>(rsFutRootGet[L"iFutureRootID"]);			
			if(!IsExists(m_spCommodityFutRootColl, nFutureRootId))
			{
				EODMS::CMsFuturesRootPtr spRoot(new EODMS::CMsFuturesRoot);
				ATLASSERT(spRoot);
				spRoot->m_nRootId = nFutureRootId;
				spRoot->m_sRootName =(LPCSTR)((_bstr_t)rsFutRootGet[L"vcFutureRootName"]);
				spRoot->m_sRootSymbol =(LPCSTR)( (_bstr_t)rsFutRootGet[L"vcFutureRootSymbol"]);
				spRoot->m_nUndId = rsFutRootGet[L"iIndexID"];
				m_spCommodityFutRootColl->insert(EODMS::CMsFuturesRootPair(nFutureRootId, spRoot));
			}
			++rsFutRootGet;
		}
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodCommodityFutureRoot_Get : %s, %s"), typeid(e).name(), e.what());
		throw exception(e);
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_LoadInterestFuturesRootsData()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	try
	{
		CStoredProc<CDefaultRecordset> rsFutRootGet(m_spDB, L"usp_EodInterestFutureRoot_Get");
		rsFutRootGet.Open();

		while(!rsFutRootGet.IsEOF())
		{			
			long nFutureRootId = static_cast<long>(rsFutRootGet[L"iFutureRootID"]);			
			if(!IsExists(m_spInterestFutRootColl, nFutureRootId))
			{
				EODMS::CMsFuturesRootPtr spRoot(new EODMS::CMsFuturesRoot);
				ATLASSERT(spRoot);
				spRoot->m_nRootId = nFutureRootId;
				spRoot->m_sRootName = (LPCSTR)((_bstr_t)rsFutRootGet[L"vcFutureRootName"]);
				spRoot->m_sRootSymbol = (LPCSTR)((_bstr_t)rsFutRootGet[L"vcFutureRootSymbol"]);
				spRoot->m_nUndId = rsFutRootGet[_T("iIndexID")];
				m_spInterestFutRootColl->insert(EODMS::CMsFuturesRootPair(nFutureRootId, spRoot));
			}
			++rsFutRootGet;
		}
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodInterestFutureRoot_Get: %s"), (LPCSTR)e.Description());
		throw exception((LPCSTR)e.Description());
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in usp_EodInterestFutureRoot_Get : %s, %s"), typeid(e).name(), e.what());
		throw exception(e);
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_LoadFuturesWithPosition()
{
	EODMS::CMsIndexPtr spIndex;

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		CStoredProc<CClientRecordset> rsGetFutPos(m_spDB, L"usp_EodFuturesWithPositions_Get");
		rsGetFutPos.Open();


		while(!rsGetFutPos.IsEOF())
		{			
			long nIndexId = static_cast<long>(rsGetFutPos[L"iIndexID"]);
			CString strFutSymbol = (LPCSTR)((_bstr_t)rsGetFutPos[L"vcSymbol"]);
			if(!spIndex || spIndex->m_nID != nIndexId)
				spIndex = boost::shared_dynamic_cast<EODMS::CMsIndex>(GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, nIndexId));

			ATLASSERT(spIndex);

			EODMS::CMsFuturePtr spFut(new EODMS::CMsFuture);
			spFut->m_nID = rsGetFutPos[L"iFutureID"];
			spFut->m_sSymbol = strFutSymbol;
			spFut->m_dPos = rsGetFutPos[L"fPosition"];
			spFut->m_bIsActive = rsGetFutPos[L"tiIsActive"];
			spFut->m_dtMaturity = /*static_cast<long>(*/rsGetFutPos[L"dtMaturityDate"]/*)*/;
			spFut->m_dPriceClose = rsGetFutPos[L"fPriceClose"];
			
			if(!IsExists(spIndex->FutureWithPos(), strFutSymbol))
				spIndex->FutureWithPos()->insert(EODMS::CMsFutureColl::value_type(strFutSymbol, spFut));

			++rsGetFutPos;
		}
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesWithPosition  : %s, %s"), typeid(e).name(), e.what());
		_ClearFuturesPositionsData();
		throw exception(e);
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesWithPosition: %s"), (LPCSTR)e.Description());
		_ClearFuturesPositionsData();
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_LoadFuturesOptionPosition()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		EODMS::CMsFutureByIdCollPtr spAllFutures(_LoadFuturesWithOptionPosition());
		ATLASSERT(spAllFutures);

		_LoadFuturesOptionWithPosition(spAllFutures);

		_CopyFuturesOptionWithPositionInIndex();

	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionPosition  : %s, %s"), typeid(e).name(), e.what());
		_ClearFuturesPositionsData();
		throw exception(e);
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionPosition: %s"), (LPCSTR)e.Description());
		_ClearFuturesPositionsData();
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

EODMS::CMsFutureByIdCollPtr CEodMktStructureMgr::_LoadFuturesWithOptionPosition()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	EODMS::CMsFutureByIdCollPtr spAllFutures(new EODMS::CMsFutureByIdColl);
	ATLASSERT(spAllFutures);

	EODMS::CMsIndexPtr spIndex;

	try
	{
		CStoredProc<CClientRecordset> rsGetFutPos(m_spDB, L"usp_EodFuturesWithOptionPositions_Get");
		rsGetFutPos.Open();

		while(!rsGetFutPos.IsEOF())
		{			
			long nIndexId = static_cast<long>(rsGetFutPos[L"iIndexID"]);
			CString strFutSymbol = (LPCSTR)((_bstr_t)rsGetFutPos[L"vcSymbol"]);
			if(!spIndex || spIndex->m_nID != nIndexId)
				spIndex = boost::shared_dynamic_cast<EODMS::CMsIndex>(GetCollectionItem<EODMS::CMsUndPtr>(m_spUndByIDColl, nIndexId));

			ATLASSERT(spIndex);

			if(!IsExists(spIndex->FutureWithPos(), strFutSymbol))
			{
				EODMS::CMsFuturePtr spFut(new EODMS::CMsFuture);
				spFut->m_nID = rsGetFutPos[L"iFutureID"];
				spFut->m_sSymbol = strFutSymbol;
				spFut->m_bIsActive = rsGetFutPos[L"tiIsActive"];
				spFut->m_dtMaturity = /*static_cast<long>(*/rsGetFutPos[L"dtMaturityDate"]/*)*/;
				spFut->m_dPriceClose = rsGetFutPos[L"fPriceClose"];
				spIndex->FutureWithPos()->insert(EODMS::CMsFutureColl::value_type(strFutSymbol, spFut));
			}

			EODMS::CMsFuturePtr spFuture(spIndex->FutureWithPos(strFutSymbol));
			ATLASSERT(spFuture);

			spAllFutures->insert(EODMS :: CMsFutureByIdPair (spFuture->m_nID, spFuture));







			++rsGetFutPos;
		}
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesWithOptionPosition  : %s, %s"), typeid(e).name(), e.what());
		_ClearFuturesPositionsData();
		throw exception(e);
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesWithOptionPosition: %s"), (LPCSTR)e.Description());
		_ClearFuturesPositionsData();
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return spAllFutures;
}

void CEodMktStructureMgr::_LoadFuturesOptionWithPosition(EODMS::CMsFutureByIdCollPtr pFutColl)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	EODMS::CMsFutureByIdCollPtr spAllFutures(pFutColl);
	ATLASSERT(spAllFutures);
	try
	{
		CStoredProc<CClientRecordset> rsFutOpt(m_spDB, L"usp_EodFuturesOptionWithPositions_Get");
		rsFutOpt.Open();

		while(!rsFutOpt.IsEOF())
		{
			long nFutureId = static_cast<long>(rsFutOpt[L"iFutureID"]);

			EODMS::CMsFuturePtr spFuture(GetCollectionItem<EODMS::CMsFuturePtr>(spAllFutures, nFutureId));
			ATLASSERT(spFuture);

			if(spFuture)
			{
				CString strOptSymbol = (LPCSTR)((_bstr_t)rsFutOpt[L"vcOptionSymbol"]);

				EODMS::CMsFutureOptionCollPtr spOptColl(spFuture->Opt());
				ATLASSERT(spOptColl);

				if(!IsExists(spOptColl, strOptSymbol))
				{
					EODMS::CMsFutureOptionPtr spOpt(new EODMS::CMsFutureOption);

					// contract properties
					spOpt->m_nID = rsFutOpt[L"iID"];
					spOpt->m_sSymbol = strOptSymbol;
					spOpt->m_sSymbol.Trim().MakeUpper();

					spOpt->m_bIsCall = rsFutOpt[L"tiIsCall"];
					spOpt->m_dStrike = rsFutOpt[L"fStrike"];
					spOpt->m_dtExpiration = rsFutOpt[L"dtExpiry"];

					spOptColl->insert(EODMS :: CMsFutureOptionColl::value_type(spOpt->m_sSymbol, spOpt));
				}				
			}

			++rsFutOpt;
		}
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionWithPosition  : %s, %s"), typeid(e).name(), e.what());
		_ClearFuturesPositionsData();
		throw exception(e);
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionWithPosition: %s"), (LPCSTR)e.Description());
		_ClearFuturesPositionsData();
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_LoadCommFuturesOptionWithPosition(EODMS::CMsFutureByIdCollPtr spFutColl)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		EODMS::CMsFutureOptionCollPtr spOptColl;
		EODMS::CMsFuturePtr spFuture;
		if (spFutColl != NULL)
		{
			CStoredProc<CDefaultRecordset> rsFutOpt(m_spDB,L"usp_EodFuturesOptionWithPositions_Get");
			rsFutOpt.Open();

			while(!rsFutOpt.IsEOF())
			{
				long nFutureId = static_cast<long>(rsFutOpt[L"iFutureID"]);

				spFuture = GetCollectionItem<EODMS::CMsFuturePtr>(spFutColl, nFutureId);

				if(spFuture != NULL)
				{
					CString strOptSymbol = (LPCSTR)((_bstr_t)rsFutOpt[L"vcOptionSymbol"]);

					spOptColl = spFuture->Opt();
					if (spOptColl != NULL )
					{
						if(!IsExists(spOptColl,strOptSymbol))
						{
							EODMS::CMsFutureOptionPtr spOpt(new EODMS::CMsFutureOption);

							// contract properties
							spOpt->m_nID = rsFutOpt[L"iID"];
							spOpt->m_sSymbol = strOptSymbol;
							spOpt->m_sSymbol.Trim().MakeUpper();

							spOpt->m_bIsCall = rsFutOpt[L"tiIsCall"];
							spOpt->m_dStrike = rsFutOpt[L"fStrike"];
							spOpt->m_dtExpiration = rsFutOpt[L"dtExpiry"];

							spOptColl->insert(EODMS::CMsFutureOptionPair(spOpt->m_sSymbol, spOpt));
						}
					}
				}

				++rsFutOpt;
			}
			Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		}
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionWithPosition  : %s, %s"), typeid(e).name(), e.what());
		_ClearFuturesPositionsData();
		throw exception(e);
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionWithPosition: %s"), (LPCSTR)e.Description());
		_ClearFuturesPositionsData();
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CEodMktStructureMgr::_CopyFuturesOptionWithPositionInIndex()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		for(EODMS::CMsUndByIDColl::iterator iter = m_spUndByIDColl->begin(); iter != m_spUndByIDColl->end(); iter++)
		{
			EODMS::CMsIndexPtr spIndex(boost::shared_dynamic_cast<EODMS::CMsIndex>(iter->second));
			if(spIndex)
			{
				EODMS::CMsFutureOptionCollPtr spIndexOptColl(spIndex->FutureOptionWithPos());
				ATLASSERT(spIndexOptColl);

				for(EODMS::CMsFutureColl::iterator iterFut = spIndex->FutureWithPos()->begin(); iterFut != spIndex->FutureWithPos()->end(); iterFut++)
				{
					EODMS::CMsFuturePtr spFuture(iterFut->second);
					ATLASSERT(spFuture);

					for(EODMS::CMsFutureOptionColl::iterator iterOpt = spFuture->Opt()->begin(); iterOpt != spFuture->Opt()->end(); iterOpt++)
					{
						EODMS::CMsFutureOptionPtr spFutureOpt(iterOpt->second);
						ATLASSERT(spFutureOpt);

						if(!IsExists(spIndexOptColl, spFutureOpt->m_sSymbol))
							spIndexOptColl->insert(EODMS::CMsFutureOptionColl::value_type(spFutureOpt->m_sSymbol, spFutureOpt));
					}
				}
			}
		}

	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionWithPositionInIndex  : %s, %s"), typeid(e).name(), e.what());
		_ClearFuturesPositionsData();
		throw exception(e);
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception in _LoadFuturesOptionWithPositionInIndex: %s"), (LPCSTR)e.Description());
		_ClearFuturesPositionsData();
		throw exception((LPCSTR)e.Description());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

};