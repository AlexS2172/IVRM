#include "StdAfx.h"
#include "BorrowingRatesCache.h"
#include "BorrowingRatesCurves.h"
#include "EtsRateColl.h"
//--------------------------------------------------------------------------
CBorrowingRatesCache::CBorrowingRatesCache(void)
{
	CComObject<CBorrowingRatesCurves>*	pNewVal = NULL;
	_CHK(CComObject<CBorrowingRatesCurves>::CreateInstance(&pNewVal), _T("Fail to create Borrowing Rates Curevs."));
	m_spBorrowingRatesCurevs.Attach(pNewVal, TRUE);
}
//--------------------------------------------------------------------------
CBorrowingRatesCache::~CBorrowingRatesCache(void)
{
	Clear();
}
//--------------------------------------------------------------------------
void CBorrowingRatesCache::Clear()
{
	if (m_spBorrowingRatesCurevs != NULL)
			m_spBorrowingRatesCurevs->Clear();
	
	m_spBorrowingRatesCurevs = NULL;	
};
//--------------------------------------------------------------------------
bool CBorrowingRatesCache::AddCurve(IEtsRateCollPtr spCurve)
{
	try
	{
		if (static_cast<bool>(m_spBorrowingRatesCurevs) && static_cast<bool>(spCurve))
		{
			long	lCurveID = 0;
			_CHK(spCurve->get_ID(&lCurveID), _T("Fail to get CurveID."));

			IEtsRateCollPtr	spRet = NULL;
			_CHK(m_spBorrowingRatesCurevs->Add(lCurveID, spCurve, &spRet), _T("Fail to Add new Curve to collection."));
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
		return false;
	}
	return false;
};
//--------------------------------------------------------------------------
IEtsRateCollPtr	CBorrowingRatesCache::GetCurve(long lCurveID) //now lCurveID must be ContractID
{
	try
	{
		if (static_cast<bool>(m_spBorrowingRatesCurevs))
		{
			IEtsRateCollPtr	spCurve = NULL;
						
			CBorrowingRatesCurves* pCurvesColl = 
				dynamic_cast<CBorrowingRatesCurves*>(m_spBorrowingRatesCurevs.GetInterfacePtr());

			_CHK(pCurvesColl->get_Item(lCurveID, &spCurve), _T("Fail to get Curve."));

			if (static_cast<bool>(spCurve) == false)
			{
				spCurve = LoadSingleCurve(lCurveID);

				AddCurve(spCurve);
			}

			return spCurve;	
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
		return NULL;
	}
	return NULL;
};
//--------------------------------------------------------------------------
IEtsRateCollPtr	CBorrowingRatesCache::LoadSingleCurve(long lCurveID)
{
	InitializeDB();

	try
	{
		//create new curve
		IEtsRateCollPtr	spRet = NULL;
		CComObject<CEtsRateColl>* pNewCurve = NULL;	
		_CHK(CComObject<CEtsRateColl>::CreateInstance(&pNewCurve), _T("Fail to create new Rate Curve."));		
		spRet.Attach(pNewCurve, true);

		ATLASSERT(pNewCurve != NULL);
		pNewCurve->m_nID = lCurveID;

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_BorrowingRatesCurve_Get");
		/*Set procedure params*/
		rs << lCurveID;
		/*execute query*/
		rs.Open();
		
		if(rs.GetRecordCount())
		{	
			//parse recordset
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IEtsRateAtomPtr spRate			= NULL;
				CComObject<CEtsRateAtom>* pRate = NULL;

				EtsPeriodTypeEnum enPeriodType	= static_cast<EtsPeriodTypeEnum>((LONG)rs[L"iPeriodTypeID"]);
				LONG nPeriodAmount				= rs[L"iNum"];
				LONG nPeriodInDays				= 0;

				pNewCurve->GetPeriodInDays(	vt_date::GetCurrentDate(true),
											enPeriodType,
											nPeriodAmount,
											&nPeriodInDays);

				spRate = pNewCurve->AddNew(nPeriodInDays, &pRate);

				if(spRate != NULL && pRate != NULL)
				{
					//CEtsRateAtom
					pRate->m_nID			= rs[L"iIRPointID"];
					pRate->m_dLongRate		= rs[L"fLongRate"];
					pRate->m_dShortRate		= rs[L"fShortRate"];
					pRate->m_dNeutralRate	= rs[L"NeutralRate"];

					pRate->m_enPeriodType	= enPeriodType;
					pRate->m_nPeriodAmount	= nPeriodAmount;
					
					//CBorrowingRatesCurve
					pNewCurve->m_enRule			= static_cast<InterestRateRuleTypeEnum>((long)rs[L"iRule"]);
					pNewCurve->m_nID			= lCurveID;//static_cast<long>((long)rs[L"iCurveID"]);
					pNewCurve->m_dPosThreshold	= static_cast<double>((double)rs[L"iPosThreshold"]);
					pNewCurve->m_bstrName		= CComBSTR(((_bstr_t)rs[L"vcCurveName"]).GetBSTR());
				}
				rs.MoveNext();
			}
		}
		//return params
		return spRet;
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
		return NULL;
	}
	return NULL;
};
//--------------------------------------------------------------------------


