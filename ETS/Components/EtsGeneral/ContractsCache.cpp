#include "StdAfx.h"
#include "ContractsCache.h"
#include "EtsOptAtom.h"
#include "EtsFutOptAtom.h"
#include "EtsFutAtom.h"
#include "Asset.h"
#include "EtsMain.h"
#include "ContractColl.h"
#include "UndColl.h"
#include "ContractRootColl.h"

//--------------------------------------------------------------------------------------------------------
CContractsCache::CContractsCache(void) {

	CComObject<CContractColl>*	pNewVal = {0};
	
	_CHK(CComObject<CContractColl>::CreateInstance(&pNewVal), 
	_T("Fail to create Contracts Collection."));
	
	m_spContracts.Attach(pNewVal, TRUE);

	m_spTickerContract = CTickerContractMapPtr(new CTickerContractMap());
	m_spCalculator = CCalculatorPtr(new CCalculator());
};
//--------------------------------------------------------------------------------------------------------
CContractsCache::~CContractsCache(void) {
	UnSubscribeQuote();
	Clear();
};
//--------------------------------------------------------------------------------------------------------
void 
CContractsCache::Clear() {

	if (m_spContracts != NULL)
		m_spContracts->Clear();

	m_spContracts = NULL;
	m_spTickerContract->clear();
};
//--------------------------------------------------------------------------------------------------------
IContractCollPtr 
CContractsCache::GetContractsColl() {
	ATLASSERT(static_cast<bool>(m_spContracts));
	return m_spContracts;
};
//--------------------------------------------------------------------------------------------------------
void 
CContractsCache::SubscribeContract(IContractPtr spContract) {

	try	{
	
		if (static_cast<bool>(spContract)) {
		
			CAbstractContract* pContract = 0;
			
			pContract = dynamic_cast<CAbstractContract*>(spContract.GetInterfacePtr());
			
			if ( pContract ) {
			
				CTickerContractMap::iterator itC = m_spTickerContract->find(*(pContract->GetTicker()));
				
				if (itC == m_spTickerContract->end()) {
				
					TRACE_DEBUG(_TEXT("ContractID: [%d]"), pContract->m_nID);

					m_spTickerContract->insert(std::pair<CTicker, IContractPtr>(*(pContract->GetTicker()), spContract));
					SubscribeQuotes(*(pContract->GetTicker()));
					SubscribeRisks(*(pContract->GetTicker()));
				}
			}
		}
	}
	catch(_com_error& err) {
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...)	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	};
};
//--------------------------------------------------------------------------------------------------------
IAssetPtr	
CContractsCache::GetAsset(long lAssetID)
{
	try
	{
		TRACE_DEBUG(_TEXT("AssetID: [%d]"), lAssetID);

		if (static_cast<bool>(GetMain()))
		{
			IUndCollPtr spAssets = NULL;
			_CHK(GetMain()->get_Underlying(&spAssets), _T("Fail to get Asset collection"));

			if (static_cast<bool>(spAssets))
			{
				IUndAtomPtr spUnd = NULL;

				CUndColl* pAssetsColl = dynamic_cast<CUndColl*>(spAssets.GetInterfacePtr());
				_CHK(pAssetsColl->get_Item(lAssetID, &spUnd), _T("Fail to get Underlying"));

				if (static_cast<bool>(spUnd))
				{
					IAssetPtr	spAsset = NULL;
					spAsset.Attach(dynamic_cast<IAsset*>(spUnd.GetInterfacePtr()), true);

					//--------------------------------------------------------------------//
					CAsset* pAsset = dynamic_cast<CAsset*>(spAsset.GetInterfacePtr());
					if (pAsset)
					{
						VARIANT_BOOL	vIsInitialized = VARIANT_FALSE;
						IVolaControlPtr	 spVolatilitySource = NULL;
						spVolatilitySource.Attach(pAsset->GetVolatilitySource());
						_CHK(spVolatilitySource->get_IsInitialized(&vIsInitialized), _T("Fail to get property."));

						if (vIsInitialized == VARIANT_FALSE && pAsset->m_nID != 0)
							spVolatilitySource->Init(	pAsset->m_bstrSymbol, 
																pAsset->m_enUndType == enCtStock ? enCtStock : enCtIndex,
																GetVolatilitySource(),
																VARIANT_FALSE);
					}
					//--------------------------------------------------------------------//
					return spAsset;
				}
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;		
	}
	catch(...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------
IEtsPriceProfileAtomPtr	
CContractsCache::GetPriceProfile(long lID)
{
	try
	{
		TRACE_DEBUG(_TEXT("PriceProfileID: [%d]"), lID);

		if (static_cast<bool>(GetMain()))
		{
			IEtsPriceProfileCollPtr spPriceProfiles = NULL;

			_CHK(GetMain()->get_PriceProfile(&spPriceProfiles), _T("Fail to get PriceProfiles coll"));

			if (static_cast<bool>(spPriceProfiles))
			{
				IEtsPriceProfileAtomPtr spProfile = NULL;

				_CHK(spPriceProfiles->get_Item(lID, &spProfile), _T("Fail to get PriceProfile"));

				spProfile.AddRef();

				return spProfile;
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		return NULL;		
	}
	catch(...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		return NULL;
	}
	return NULL;
}
//--------------------------------------------------------------------------------------------------------
IVolatilitySourcePtr	
CContractsCache::GetVolatilitySource()
{
	try
	{
		if (static_cast<bool>(GetMain()))
		{
			IVolatilitySourcePtr spVolatilitySource = NULL;
			
			_CHK(GetMain()->get_VolatilitySource(&spVolatilitySource), _T("Fail to get VolatilitySource."));
			
			spVolatilitySource.AddRef();

			return spVolatilitySource;
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...) 
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------
bool CContractsCache::AddContract(IContractPtr spContract)
{
	try
	{
		if (spContract == NULL)
			throw IvRmException::GeneralCacheException("Contract cache failed. Attempt to add null to contract cache");
			
		if (m_spContracts == NULL)
			throw IvRmException::GeneralCacheException("Contract cache has been corrupted");
		
		// Get C++ class pointer from COM interface
		CAbstractContract* contract = dynamic_cast<CAbstractContract*>(spContract.GetInterfacePtr());

		if (contract == NULL)
			throw IvRmException::GeneralCacheException("Contract cache failed. Attempt to add unknown object");
		
		// lookup for dealing and pricing asset
		IAssetPtr dealingAsset = GetAsset(contract->m_nAssetID);
		IAssetPtr pricingAsset = GetAsset(0); 
		
		if (dealingAsset == NULL || pricingAsset == NULL)
			throw IvRmException::GeneralCacheException("Contract cache failed. Can not add contract with dealing or pricing asset that is not in asset cache");

		contract->SetAsset(dealingAsset);
		contract->m_spPricingAsset = pricingAsset;

		// set base contact if exists 
		contract->SetBaseContract(GetContract(contract->m_nBaseContractID));

		IContractPtr	spRet = NULL;
		CContractColl*	pContracts = dynamic_cast<CContractColl*>(m_spContracts.GetInterfacePtr());

		_CHK(pContracts->Add(contract->m_nID, (BSTR)contract->m_bstrSymbol, spContract, &spRet), _T("Contract cache failed. Internal storage error."));
		
	}
	catch (IvRmException::AbstractException& e)
	{
		throw IvRmException::GeneralCacheException("ContractCache::addContract failed.", e);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	
	return true;
}
//--------------------------------------------------------------------------------------------------------//
long CContractsCache::GetContractID(CTicker* ptrTicker)
{
	InitializeDB();
	try
	{
		TRACE_DEBUG(_TEXT("ContractID: [%s]"), ptrTicker->m_sSymbol.c_str());

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_ContractByTicker_Get");

		/*Set procedure params*/
		rs << _bstr_t(ptrTicker->m_sSymbol.c_str());
		rs << ptrTicker->m_enType;

		/*execute query*/
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				long lContractID = static_cast<long>((long)rs[L"iContractID"]);
				rs.Close();
				return lContractID;
			}
		};

		return -1;
	}
	catch(_com_error& err)
	{
		TRACE_COM_ERROR_EX(err,_T("ContractID: [%s] "), ptrTicker->m_sSymbol.c_str());
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	/*if contract not found*/
	return enCtNone;	
};
//--------------------------------------------------------------------------------------------------------//
IContractPtr CContractsCache::GetContract(long contractId)
{
	try
	{
		if (static_cast<bool>(m_spContracts) && contractId > 0)
		{
			CContractColl*	pContracts = dynamic_cast<CContractColl*>(m_spContracts.GetInterfacePtr());
			IContractPtr	spSingleContract = NULL;

			_CHK(pContracts->get_Item(contractId, &spSingleContract), _T("Fail to get contract."));

			if (static_cast<bool>(spSingleContract))
				return	spSingleContract;

			spSingleContract = LoadSingleContract(contractId);
				
			if (spSingleContract == NULL) // was unable to load contract
					FOCL_THROW_EXCEPTION(IvRmException::ContractNotFound, "Contract cache could not load contract (id = " << contractId << ") from Database");
				
			AddContract(spSingleContract);

			SubscribeContract(spSingleContract);

			return spSingleContract;
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR_EX(err,_T("ContractID: [%d] "), contractId);
		ATLASSERT(false);
		throw;
	}
	catch (IvRmException::AbstractException& e)
	{
		throw e;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	return	NULL;
};
//--------------------------------------------------------------------------------------------------------
EtsContractTypeEnum		
CContractsCache::GetContractType(long lContractID)
{
	InitializeDB();
	try
	{
		TRACE_DEBUG(_TEXT("ContractID: [%d]"), lContractID);

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_ContractType_Get");

		/*Set procedure params*/
		rs << lContractID;

		/*execute query*/
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				EtsContractTypeEnum enType = static_cast<EtsContractTypeEnum>((long)rs[L"iContractType"]);
				rs.Close();
				return enType;
			}
		};	
	}
	catch(_com_error& err)
	{
		TRACE_COM_ERROR_EX(err,_T("ContractID: [%d] "), lContractID);
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	/*if contract not found*/
	return enCtNone;
};
//--------------------------------------------------------------------------------------------------------
IContractPtr	
CContractsCache::LoadSingleContract(long lContractID) {
	
	try	{
	
		EtsContractTypeEnum	enContractType = GetContractType(lContractID);
			
		if (enContractType == enCtStock)		return LoadSingleSpotContract(lContractID);
		if (enContractType == enCtIndex)		return LoadSingleSpotContract(lContractID);
		if (enContractType == enCtOption)		return LoadSingleSpotOptionContract(lContractID);
		if (enContractType == enCtFuture)		return LoadSingleFutureContract(lContractID);
		if (enContractType == enCtFutOption)	return LoadSingleFutureOptionContract(lContractID);
		
	}
	catch (_com_error& err)	{
		TRACE_COM_ERROR_EX(err,_T("ContractID: [%d] "), lContractID);
		ATLASSERT(false);
		throw;
	}
	catch (...)	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	};
	return NULL;
};
//--------------------------------------------------------------------------------------------------------
IContractPtr	
CContractsCache::LoadSingleFutureContract(long lContractID)
{
	InitializeDB();
	try
	{
		TRACE_DEBUG(_TEXT("ContractID: [%d]"), lContractID);

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_Future_Get");

		/*Set procedure params*/
		rs << lContractID;

		/*execute query*/
		rs.Open();

		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IContractPtr spContract = ParseFutureContract(rs);
				rs.Close();
				return spContract;
			}
		}
	}
	catch(_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------
IContractPtr	
CContractsCache::LoadSingleSpotContract(long lContractID)
{
	try
	{
		IAssetPtr	spAsset = NULL;
		spAsset = GetAsset(lContractID);

		if (static_cast<bool>(spAsset))
		{
			ISpotContractPtr	spSpotContract = NULL;
			_CHK(spAsset->get_Spot(&spSpotContract), _T("Fail to get Spot Contract"));
			if (static_cast<bool>(spSpotContract))
			{
				IContractPtr	spContract = NULL;
				spContract.Attach(dynamic_cast<IContract*>(spSpotContract.GetInterfacePtr()), true);

				CAbstractContract* pContract = dynamic_cast<CAbstractContract*>(spContract.GetInterfacePtr());
				if (pContract)
					pContract->m_nAssetID = lContractID;

				return	spContract;
			}
		}
	}
	catch(_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
IContractPtr	
CContractsCache::LoadSingleSpotOptionContract(long lContractID)
{
	InitializeDB();

	try
	{
		TRACE_DEBUG(_TEXT("ContractID: [%d]"), lContractID);

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_SpotOption_Get");

		/*Set procedure params*/
		rs << lContractID;

		/*execute query*/
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IContractPtr spContract = ParseSpotOptionContract(rs);
				rs.Close();
				return spContract;
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------
IContractPtr	
CContractsCache::LoadSingleFutureOptionContract(long lContractID)
{
	InitializeDB();
	try
	{
		TRACE_DEBUG(_TEXT("ContractID: [%d]"), lContractID);

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_FutureOption_Get");

		/*Set procedure params*/
		rs << lContractID;

		/*execute query*/
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IContractPtr spContract = ParseFutureOptionContract(rs);
				rs.Close();
				return spContract;
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;	
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
IContractPtr
CContractsCache::ParseSpotOptionContract(CStoredProc<CClientRecordset>& recordset)
{	
	try
	{
		IContractPtr spRet = CFactory::getInstance()->CreateSpotOption();

		CSpotOptionContract* pOption = dynamic_cast<CSpotOptionContract*>(spRet.GetInterfacePtr());
		if (pOption)
		{
			pOption->m_nID					= recordset[L"iContractID"];

			pOption->m_enType				= static_cast<EtsContractTypeEnum>((long)recordset[L"iContractType"]);
			pOption->m_bstrSymbol			= CComBSTR(((_bstr_t)recordset[L"vcSymbol"]).GetBSTR());
			pOption->m_bstrContractName		= CComBSTR(((_bstr_t)recordset[L"vcContractName"]).GetBSTR());
			pOption->m_bstrSeries			= CComBSTR(((_bstr_t)recordset[L"vcSeries"]).GetBSTR());
			pOption->m_bstrImportID			= CComBSTR(((_bstr_t)recordset[L"vcImportID"]).GetBSTR());
			pOption->m_dManualPrice			= recordset[L"fManualPrice"];
			pOption->m_dClosePrice			= recordset[L"fClosePrice"];
			pOption->m_dTheoClosePrice		= recordset[L"fTheoClosePrice"];

			pOption->m_enOptType			= static_cast<EtsOptionTypeEnum>((long)recordset[L"iOptionType"]);
			pOption->m_dStrike				= recordset[L"fStrike"];
			pOption->m_dtExpiry				= static_cast<vt_date>(recordset[L"dtExpiry"]);
			pOption->m_dtExpiryOV			= static_cast<vt_date>(recordset[L"dtExpiryOV"]);
			pOption->m_dtTradingClose		= static_cast<vt_date>(recordset[L"dtTradingClose"]);

			double	dIntPart = 0.;
			pOption->m_dtTradingClose		= modf(pOption->m_dtTradingClose, &dIntPart);

			pOption->m_IsFlex				= recordset[L"iIsFlex"];
			pOption->m_nRootID				= recordset[L"iRootID"];

			pOption->m_nBaseContractID		= recordset[L"iBaseContractID"];
			pOption->m_nAssetID				= recordset[L"iAssetID"];

			pOption->m_spRoot =
				CContractRootCache::getInstance()->getContractRoot(pOption->m_nRootID);

			TRACE_DEBUG(_T("Parsed: [%s]"), pOption->GetTicker()->m_sSymbol.c_str());

			return spRet;
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
IContractPtr	
CContractsCache::ParseFutureContract(CStoredProc<CClientRecordset>& recordset)
{
	try
	{
		IContractPtr spRet = NULL;

		CComObject<CEtsFutAtom>* pNewFuture = NULL;	
		_CHK(CComObject<CEtsFutAtom>::CreateInstance(&pNewFuture), _T("Fail to create Future."));

		CFutureContract* pFuture = dynamic_cast<CFutureContract*>(pNewFuture);
		if (pFuture)
		{
			pFuture->m_nID					= recordset[L"iContractID"];
			pFuture->m_enType				= static_cast<EtsContractTypeEnum>((long)recordset[L"iContractType"]);
			pFuture->m_bstrSymbol			= CComBSTR(((_bstr_t)recordset[L"vcSymbol"]).GetBSTR());
			pFuture->m_bstrContractName		= CComBSTR(((_bstr_t)recordset[L"vcContractName"]).GetBSTR());
			pFuture->m_dManualPrice			= recordset[L"fManualPrice"];
			pFuture->m_dClosePrice			= recordset[L"fClosePrice"];
			pFuture->m_dTheoClosePrice		= recordset[L"fTheoClosePrice"];

			pFuture->m_spBaseContract		= NULL;

			pFuture->m_spUndPriceProfile	= GetPriceProfile((long)recordset[L"iUndPriceProfileID"]);
			pFuture->m_spOptPriceProfile	= GetPriceProfile((long)recordset[L"iOptPriceProfileID"]);

			pFuture->m_nRootID				= recordset[L"iRootID"];
			pFuture->m_nFutRootID			= pFuture->m_nRootID;

			pFuture->m_dtMaturityDate		= static_cast<vt_date>(recordset[L"dtMaturityDate"]);
			pFuture->m_bIsAmerican			= static_cast<VARIANT_BOOL>((bool)recordset[L"iOptionStyle"]);
			pFuture->m_bIsActive			= static_cast<VARIANT_BOOL>((bool)recordset[L"iIsActive"]);
			pFuture->m_bHaveOptions			= static_cast<VARIANT_BOOL>((bool)recordset[L"iHaveOptions"]);
			pFuture->m_bShowInFutureContractEq = static_cast<VARIANT_BOOL>((bool)recordset[L"iShowInFutureContractEq"]);

			pFuture->m_dBasis				= recordset[L"fBasis"];
			pFuture->m_dActiveFutureRatio	= recordset[L"fActiveFutureRatio"];
			pFuture->m_bUseInCalc			= static_cast<VARIANT_BOOL>((bool)recordset[L"iIsActiveFuture"]);

			pFuture->m_dPriceQuotationUnit	= recordset[L"iQuotationUnitAmount"];
			pFuture->m_dQuoteFormat			= recordset[L"iQuoteFormat"];
			pFuture->m_dTickSize			= recordset[L"fTickSize"];
			pFuture->m_dTickValue			= recordset[L"fTickValue"];

			pFuture->m_nAssetID				= recordset[L"iAssetID"];
			pFuture->m_nBaseContractID		= 0;//pFuture->m_nAssetID;

			pFuture->m_spRoot = 
				CContractRootCache::getInstance()->getContractRoot(pFuture->m_nRootID);
			//-----------------------------------------------------------------------------------
			if (pFuture->m_dPriceQuotationUnit != 0. && pFuture->m_dTickSize != 0.)
			{
				pFuture->m_dKEq = pFuture->m_dTickValue / pFuture->m_dTickSize * (1. / (pFuture->m_dPriceQuotationUnit * (pFuture->m_dQuoteFormat == 1.0 ? 100 : 1)));
			}
			else
			{
				pFuture->m_dKEq = 1.;
			}
			//-----------------------------------------------------------------------------------
			if (pFuture->m_bShowInFutureContractEq == VARIANT_FALSE &&
				pFuture->m_dPriceQuotationUnit != 0. &&
				pFuture->m_dTickSize != 0.)
			{
				pFuture->m_dK = pFuture->m_dKEq;
				pFuture->m_bMultOptDltEq = VARIANT_FALSE;
			}
			else
			{
				pFuture->m_bMultOptDltEq = VARIANT_TRUE;
			}
			//-----------------------------------------------------------------------------------

			spRet.Attach(pNewFuture, true);

			return spRet;
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
IContractPtr		
CContractsCache::ParseSpotContract(CStoredProc<CClientRecordset>& recordset)
{
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
IContractPtr	
CContractsCache::ParseFutureOptionContract(CStoredProc<CClientRecordset>& recordset)
{
	try
	{
		IContractPtr spRet = NULL;

		CComObject<CEtsFutOptAtom>* pNewOption = NULL;	
		_CHK(CComObject<CEtsFutOptAtom>::CreateInstance(&pNewOption), _T("Fail to create FutureOption."));

		CFutureOptionContract* pOption = dynamic_cast<CFutureOptionContract*>(pNewOption);

		if (pOption)
		{
			pOption->m_nID					= recordset[L"iContractID"];
			pOption->m_enType				= static_cast<EtsContractTypeEnum>((long)recordset[L"iContractType"]);
			pOption->m_bstrSymbol			= CComBSTR(((_bstr_t)recordset[L"vcSymbol"]).GetBSTR());
			pOption->m_bstrContractName		= CComBSTR(((_bstr_t)recordset[L"vcContractName"]).GetBSTR());
			pOption->m_bstrSeries			= CComBSTR(((_bstr_t)recordset[L"vcSeries"]).GetBSTR());
			pOption->m_bstrImportID			= CComBSTR(((_bstr_t)recordset[L"vcImportID"]).GetBSTR());
			pOption->m_dManualPrice			= recordset[L"fManualPrice"];
			pOption->m_dClosePrice			= recordset[L"fClosePrice"];
			pOption->m_dTheoClosePrice		= recordset[L"fTheoClosePrice"];

			pOption->m_enOptType			= static_cast<EtsOptionTypeEnum>((long)recordset[L"iOptionType"]);
			pOption->m_dStrike				= recordset[L"fStrike"];
			pOption->m_dtExpiry				= static_cast<vt_date>(recordset[L"dtExpiry"]);
			pOption->m_dtExpiryOV			= static_cast<vt_date>(recordset[L"dtExpiryOV"]);
			pOption->m_dtTradingClose		= static_cast<vt_date>(recordset[L"dtTradingClose"]);

			double	dIntPart = 0.;
			pOption->m_dtTradingClose		= modf(pOption->m_dtTradingClose, &dIntPart);					

			pOption->m_IsFlex				= 0;
			pOption->m_nRootID				= recordset[L"iRootID"];

			pOption->m_nBaseContractID		= recordset[L"iBaseContractID"];
			pOption->m_nAssetID				= recordset[L"iAssetID"];

			pOption->m_spRoot	=	
				CContractRootCache::getInstance()->getContractRoot(pOption->m_nRootID);

			spRet.Attach(pNewOption, true);

			return spRet;
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------
void 
CContractsCache::DoCalc()
{
	try
	{
		CQuoteTickerMapPtr spQuotes = GetRealTimeQuotes();

		CQuoteTickerMap::iterator itBegin = spQuotes->begin();
		CQuoteTickerMap::iterator itEnd = spQuotes->end();
		CQuoteTickerMap::iterator it = itBegin;

		for ( ; it!=itEnd ; it++)
		{
			CTickerContractMap::iterator itScan = m_spTickerContract->find(it->first);
			if (itScan != m_spTickerContract->end())
			{
				IContractPtr spContract = itScan->second;

				ATLASSERT(static_cast<bool>(spContract)); 

				CAbstractContract* pContract = dynamic_cast<CAbstractContract*>(spContract.GetInterfacePtr());
				
				if (pContract != NULL){
					it->second->CopyTo(*(pContract->m_spQuote));
				}			
			}
		}
		spQuotes->clear();

		CEtsMain* system = dynamic_cast<CEtsMain*>(GetMain().GetInterfacePtr());
		ATLASSERT(system != 0);
		system->RecalculateForwardCurves();

	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	}
};
//--------------------------------------------------------------------------------------------------------
void 
CContractsCache::DoRisksUpdate()
{
  try
  {
	  ATLTRACE("DoRisksUpdate Enter\n");

	  CRisksTickerMapPtr spRisks = GetRealTimeRisks();
	  
	  CRisksTickerMap::iterator itBegin = spRisks->begin();
	  CRisksTickerMap::iterator itEnd = spRisks->end();
	  CRisksTickerMap::iterator it = itBegin;

	  for ( ; it!=itEnd ; it++)
	  {
		  CTickerContractMap::iterator itScan = m_spTickerContract->find(it->first);
		  if (itScan != m_spTickerContract->end())
		  {
			  IContractPtr spContract = itScan->second;

			  ATLASSERT(static_cast<bool>(spContract)); 

			  CAbstractContract* pContract = dynamic_cast<CAbstractContract*>(spContract.GetInterfacePtr());
				
			  it->second->CopyTo(*(pContract->m_spRisks));
		  }
	  }
	  spRisks->clear();

	  ATLTRACE("DoRisksUpdate Exit\n");
  }
  catch (_com_error& err)
  {
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
  }
  catch (...) 
  {
	  TRACE_UNKNOWN_ERROR();
	  ATLASSERT(false);
	  throw;
  }
};
//--------------------------------------------------------------------------------------------------------
CAbstractContract* CContractsCache::find_contract_by_ticker(CTicker& ticker) 
{
	CTickerContractMap::iterator it = m_spTickerContract->find(ticker);

	if (it == m_spTickerContract->end()) 
	{
		if (ticker.m_enType == enSTK) 
		{
			ticker.m_enType = enIDX;
			it = m_spTickerContract->find(ticker);
			if (it == m_spTickerContract->end())
				ticker.m_enType = enSTK;
		}
		else if (ticker.m_enType == enIDX) {
			ticker.m_enType = enSTK;
			it = m_spTickerContract->find(ticker);
			if (it == m_spTickerContract->end())
				ticker.m_enType = enIDX;
		}		
		if (it == m_spTickerContract->end()) return 0;
	}
	
	return dynamic_cast<CAbstractContract*>(it->second.GetInterfacePtr());
}
//--------------------------------------------------------------------------------------------------------
CAbstractContract* CContractsCache::SetQuote(CTicker* ptrTicker, CQuote* ptrQuote) {

	try	{
	
		if ( ptrTicker != NULL && ptrQuote != NULL) {
		
			CString strInfo;
						
			CAbstractContract* contract = find_contract_by_ticker(*ptrTicker);
			
			if (contract) 
			{
			
				strInfo.Format(_T("Symbol: %s Bid= %.2f Ask= %.2f Last= %.2f"),
								ptrTicker->m_sSymbol.c_str(),
								ptrQuote->BidPrice,
								ptrQuote->AskPrice,
								ptrQuote->LastPrice);

				TRACE_DEBUG(_TEXT("Set quotes for: [%s]"), strInfo);
				
				ptrQuote->CopyTo(*contract->m_spQuote.get());
				
				CAutoLock lk(updated_contract_lock);
				
				changed_contracts[contract->get_id()] = contract;
									
				return contract;

			}
		}
	}
	catch (_com_error& err)	
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(false);
		throw;
	}
	catch (IvRmException::ContractNotFound)	
	{
		throw;
	}
	catch (...)	
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(false);
		throw;
	};
	
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
void
CContractsCache::getDerivativeContracts(CAbstractContract* Contract, 
										IDContractMap& Contracts) {

	if (Contract && Contract->m_spDerivativeContracts) {

		CContractColl* pContracts = 
			dynamic_cast<CContractColl*>(Contract->m_spDerivativeContracts.GetInterfacePtr());

		CContractColl::CollType::iterator itr = 
			pContracts->m_collRef.begin();

		CContractColl::CollType::iterator itr_end = 
			pContracts->m_collRef.end();

		for(; itr != itr_end; ++itr) {

			CAbstractContract* pItContract = 
				dynamic_cast<CAbstractContract*>(itr->second->second);
			
			ATLASSERT(pItContract != 0);

			Contracts[pItContract->m_nID] = pItContract;
		};
	};
};
//--------------------------------------------------------------------------------------------------------//
void 
CContractsCache::GetRecalculationNeededContracts(ContractsVector& contracts)
{

	CAutoLock lk(updated_contract_lock);
	
	IDContractMap::iterator itr		= changed_contracts.begin();
	IDContractMap::iterator itr_end = changed_contracts.end();
	
	IDContractMap id_contract_map;

	for( ; itr != itr_end; itr++) {

		CAbstractContract* pContract = itr->second;

		ATLASSERT(pContract != 0);
		
		if ( pContract && pContract->m_spDerivativeContracts ) {
			getDerivativeContracts(pContract, id_contract_map);
		};
		
		id_contract_map[pContract->m_nID] = pContract;
	};
	
	contracts.reserve(id_contract_map.size());
	{
		IDContractMap::iterator itr = id_contract_map.begin();
		IDContractMap::iterator itr_end = id_contract_map.end();

		for (;itr != itr_end; itr++) {
			contracts.push_back(itr->second);
		};

		id_contract_map.clear();
	};
	
	changed_contracts.clear();
};
//--------------------------------------------------------------------------------------------------------//
void					
CContractsCache::LoadMultipleSpotOptionContracts(long lAssetID)
{
	try
	{
		InitializeDB();
		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_AssetSpotOptions_Get");

		/*Set procedure params*/
		rs << lAssetID;

		/*execute query*/
		rs.Open();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IContractPtr spLoadedContract = ParseSpotOptionContract(rs);
				if (static_cast<bool>(spLoadedContract))
				{
					long lContractID = 0;
					_CHK(spLoadedContract->get_ID(&lContractID), _T("Fail to get contractID."));

					IContractPtr	spSingleContract = NULL;
					_CHK(m_spContracts->get_Item(lContractID, &spSingleContract), _T("Fail to get contract."));

					if (false == static_cast<bool>(spSingleContract))
					{
						AddContract(spLoadedContract);
						SubscribeContract(spLoadedContract);
					};
				}
				rs.MoveNext();
			};
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	};
};
//--------------------------------------------------------------------------------------------------------//
void					
CContractsCache::InitializeForAsset(CTicker* ptrAsset)
{
	try
	{
		if ( ptrAsset )
		{
			if (ptrAsset->m_enType == enSTK || ptrAsset->m_enType == enIDX)
			{
				TRACE_INFO(_TEXT("Loading spot option contract's for Asset: [%s]"), ptrAsset->m_sSymbol.c_str());

				long lAssetID = GetContractID(ptrAsset);

				if ( lAssetID > 0 )			
					LoadMultipleSpotOptionContracts(lAssetID);					
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	};
};
//--------------------------------------------------------------------------------------------------------//
bool					
CContractsCache::IsContractExist(IContractPtr spContract)
{
	if (static_cast<bool>(spContract) && static_cast<bool>(m_spContracts)){

		long lContractID = 0;
		_CHK(spContract->get_ID(&lContractID), _T("Fail to get contractID."));

		IContractPtr	spSingleContract = NULL;
		CContractColl*	pContracts = dynamic_cast<CContractColl*>(m_spContracts.GetInterfacePtr());

		_CHK(pContracts->get_Item(lContractID, &spSingleContract), _T("Fail to get contract."));

		if (static_cast<bool>(spSingleContract)) return true;
	}
	return false;
};
//--------------------------------------------------------------------------------------------------------//
void					
CContractsCache::InitializeFromTrades()
{	
	try
	{

		CEtsMain* pMain = dynamic_cast<CEtsMain*>(GetMain().GetInterfacePtr());
		_bstr_t bsStatus("Loading Contracts from Trades ...");
		pMain->Fire_Progress(bsStatus, -1);
		long lLoaded = 0;
		long lLastPerc = -1;

		InitializeDB();
		
		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_ContractsFromTrades_Get");
			
		/*execute query*/
		rs.Open();


		//recordset for Spot contracts with trades
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				long lContractID = rs[L"iContractID"];
				IContractPtr spLoadedContract = LoadSingleSpotContract(lContractID);
				if (static_cast<bool>(spLoadedContract))
				{
					if (!IsContractExist(spLoadedContract)){
						AddContract(spLoadedContract);
						SubscribeContract(spLoadedContract);
					};
				}
				rs.MoveNext();
			};
		};

		//recordset for SpotOption contracts with trades
		rs.NextRecordset();
		if(rs.GetRecordCount())
		{
			long lTotalCount = rs.GetRecordCount();
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IContractPtr spLoadedContract = ParseSpotOptionContract(rs);
				if (static_cast<bool>(spLoadedContract))
				{
					if (!IsContractExist(spLoadedContract)){
						AddContract(spLoadedContract);
						SubscribeContract(spLoadedContract);
					};
				}
				rs.MoveNext();
				++lLoaded;
				long lPerc = static_cast<long>( 100.* double(lLoaded)/ double(lTotalCount));
				if(lPerc != lLastPerc)
				{
					lLastPerc = lPerc;
					pMain->Fire_Progress(bsStatus, lLastPerc);
				}
			};
		}
		//recordset for Future contracts with trades
		rs.NextRecordset();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IContractPtr spLoadedContract = ParseFutureContract(rs);
				if (static_cast<bool>(spLoadedContract))
				{
					if (!IsContractExist(spLoadedContract)){
						AddContract(spLoadedContract);
						SubscribeContract(spLoadedContract);
					};
				}
				rs.MoveNext();
			};
		}
		//recordset for Future Option contracts with trades
		rs.NextRecordset();
		if(rs.GetRecordCount())
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				IContractPtr spLoadedContract = ParseFutureOptionContract(rs);
				if (static_cast<bool>(spLoadedContract))
				{
					if (!IsContractExist(spLoadedContract)){
						AddContract(spLoadedContract);
						SubscribeContract(spLoadedContract);
					};
				}
				rs.MoveNext();
			};
		}
		rs.Close();

		ProcessRequests(true);

	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	};
};
//--------------------------------------------------------------------------------------------------------//
IContractPtr 
CContractsCache::CreateFlexContract(LONG					AssetID,
									DATE					Expiration,
									DOUBLE					Strike,
									EtsOptionTypeEnum		Type,
									DATE					TradingClose,
									DATE					SettlementValueTime,
									ExerciseStyleEnum		ExerciseStyle){
	try{
		


		IAssetPtr spAsset = GetAsset(AssetID);
		_CHK((bool)spAsset ? S_OK : E_INVALIDARG, 
			_T("Can't get Asset for current contract while creatting OccFlexContract."));

		_bstr_t bsAssetSymbol;
		_CHK(spAsset->get_Symbol(bsAssetSymbol.GetAddress()),
			_T("Fail to get Asset symbol while creatting OccFlexContract."));

		
		_bstr_t bsRootSymbol;
		_bstr_t bsExpiration(vt_date(Expiration).Format("%y%m%d"));
		wchar_t bsOptionSymbol[32];


		// serialize option root symbol
		if (ExerciseStyle == enEsAmerican){
			if (SettlementValueTime < 0.5){
				bsRootSymbol = _bstr_t(L"1");
			}else{
				bsRootSymbol = _bstr_t(L"2");
			}
		}else if (ExerciseStyle == enEsEuropean){
			if (SettlementValueTime < 0.5){
				bsRootSymbol = _bstr_t(L"3");
			}else{
				bsRootSymbol = _bstr_t(L"4");
			}
		}
		bsRootSymbol += bsAssetSymbol;
		

		/*Symbol – 6 bytes
		Year – 2 bytes
		Month – 2 bytes
		Day – 2 bytes
		Call/Put Indicator – 1 byte
		Strike Dollar – 5 bytes
		Strike Decimal – 3 bytes*/

		swprintf(bsOptionSymbol, 32, L"%-6s%-6s%-1s%-.8i", 
								bsRootSymbol.GetBSTR(),
								bsExpiration.GetBSTR(),
								(Type == enOtCall ? _bstr_t("C").GetBSTR() : _bstr_t("P").GetBSTR()),
								static_cast<long>(Strike * 1000.0));


		InitializeDB();

		/*init request*/
		CStoredProc<> rs(GetDBConnection(), L"usp_OccFlexOption_Save");
											//@iAssetID int,
											//@iOptionID int,
											//@vcSymbol varchar(32),
											//@vcRootSymbol varchar(16), 
											//@iIsCall tinyint,
											//@fStrike float,
											//@dtExpiration datetime,
											//@iExerciseStyle int,
											//@dtSettlementValueTime datetime,
											//@dtTradingClose datetime

		rs << AssetID;
		rs << CDBNull();
		rs << bsOptionSymbol;
		rs << bsRootSymbol;
		rs << (Type == enOtCall ? 1 : 0);
		rs << Strike;
		rs << vt_date(Expiration);
		rs << static_cast<LONG>(ExerciseStyle);
		rs << vt_date(SettlementValueTime);
		rs << vt_date(TradingClose);

		rs.Execute();

		long lResult = rs.GetResult();

		_CHK(lResult > 0 ? S_OK : E_FAIL, 
			_T("Can't create flex contract in database."));

		return GetContract (lResult);
		
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	};
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
CContractRootCache::CContractRootCache(void){
	m_spColl = CFactory::getInstance()->CreateContractRootColl();
};
//--------------------------------------------------------------------------------------------------------//
CContractRootCache::~CContractRootCache(void){
	if (m_spColl =! NULL)
		m_spColl->Clear();
	m_spColl = NULL;
}
//--------------------------------------------------------------------------------------------------------//
/*static*/
CContractRootCache::ptr
CContractRootCache::getInstance(){
	try{
		static CContractRootCache::ptr __instance = 
			CContractRootCache::ptr(new CContractRootCache);

		return __instance;
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		throw;
	}
	return 0;
};
//--------------------------------------------------------------------------------------------------------//
/*
@params: rootid on database
@return: return IContractRootPtr.
*/
IContractRootPtr
CContractRootCache::getContractRoot(long RootID){
	try{
		IContractRootPtr spRet = NULL;

		_CHK(m_spColl->get_Item(RootID, &spRet),
			_T("Fail to get Contract Root from Cache collection."));

		//check collection 
		if (spRet != NULL){
			return spRet;
		}
		else{
			spRet = LoadSingleRoot(RootID);
			addContractRoot(spRet);
			return spRet;
		}
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
IContractRootPtr
CContractRootCache::ParseContractRoot(CStoredProc<CClientRecordset>& recordset){	
	try{

		IContractRootPtr	spRet = 
			CFactory::getInstance()->CreateContractRoot();

		CContractRoot*		pRoot = 
			dynamic_cast<CContractRoot*>(spRet.GetInterfacePtr());

		if (pRoot){

			pRoot->m_ID						= recordset[L"iRootID"];
			pRoot->m_Symbol					= CComBSTR(((_bstr_t)recordset[L"vcSymbol"]).GetBSTR());
			pRoot->m_Name					= CComBSTR(((_bstr_t)recordset[L"vcName"]).GetBSTR());

			pRoot->m_SettlementValueTime	= static_cast<vt_date>(recordset[L"dtSettlementValueTime"]);
			pRoot->m_ExerciseStyle			= (ExerciseStyleEnum)static_cast<LONG>(recordset[L"iExerciseStyle"]);

			pRoot->m_LotSize				= static_cast<long>(recordset[L"iLotSize"]);
			pRoot->m_AssetID				= static_cast<long>(recordset[L"iUnderlyingID"]);

			//pRoot->m_bIsFittingEnabled		= (bool)recordset[L"tiIsFitEnabled"] ? VARIANT_TRUE : VARIANT_FALSE;
			//pRoot->m_bIsSynth				= (bool)recordset[L"tiIsSynthetic"] ? VARIANT_TRUE : VARIANT_FALSE;

			return spRet;
		}
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
void
CContractRootCache::addContractRoot(IContractRootPtr spContractRoot){
	try{

		_CHK((bool)spContractRoot ? S_OK : E_POINTER, 
			_T("Invalid pointer of spContractRoot."));

		_CHK((bool)m_spColl ? S_OK : E_POINTER,
			_T("Contract Root Collection not created."));

		CContractRootColl* collection = dynamic_cast<CContractRootColl*>(m_spColl.GetInterfacePtr());

		_CHK(collection != 0 ? S_OK : E_FAIL, 
			_T("Can't cast pointer of ContractRoot collection."));

		long contractRootID = BAD_LONG_VALUE;
		BSTR bsSymbol		= NULL;
		
		_CHK(spContractRoot->get_ID(&contractRootID), 
			_T("Can't get ContractRootID."));
		
		_CHK(spContractRoot->get_Symbol(&bsSymbol), 
			_T("Can't get Symbol."));

		IContractRootPtr spRet = NULL;
		_CHK(collection->Add(contractRootID, bsSymbol, spContractRoot, &spRet),
			_T("Can't add ContractRoot to cache collection."));
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
};
//--------------------------------------------------------------------------------------------------------//
IContractRootPtr
CContractRootCache::LoadSingleRoot(long RootID){
	try {

		InitializeDB();

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_ContractRoot_Get");

		rs << RootID;
		rs << CDBNull();

		/*execute query*/
		rs.Open();


		//recordset for Spot contracts with trades
		if(rs.GetRecordCount()){

			rs.MoveFirst();

			return ParseContractRoot(rs);
		}
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return NULL;
};
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
IContractRootCollPtr
CContractRootCache::getCollection(){
	return m_spColl;
};
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
CFactory::CFactory(){

};
//--------------------------------------------------------------------------------------------------------//
CFactory::ptr
CFactory::getInstance(){
		static CFactory::ptr __instance = 
			CFactory::ptr(new CFactory);
		return CFactory::ptr(__instance);
};
//--------------------------------------------------------------------------------------------------------//
/*virtual*/
IContractPtr 
CFactory::CreateSpotOption(){
	
	IContractPtr				spRet		= NULL;
	CComObject<CEtsOptAtom>*	pNewOption	= NULL;	
	
	_CHK(CComObject<CEtsOptAtom>::CreateInstance(&pNewOption), 
								_T("Fail to create SpotOption."));	

	spRet.Attach(pNewOption, true);

	return spRet;
};
//--------------------------------------------------------------------------------------------------------//
/*virtual*/
IContractRootPtr
CFactory::CreateContractRoot(){

	IContractRootPtr				spRet		= NULL;
	CComObject<CEtsOptRootAtom>*	pNewRoot	= NULL;

	_CHK(CComObject<CEtsOptRootAtom>::CreateInstance(&pNewRoot),
									_T("Fail to create Root."));

   spRet.Attach(pNewRoot, true);

   return spRet;
};
//--------------------------------------------------------------------------------------------------------//
/*virtual*/
IContractRootPtr
CFactory::CreateFutureRoot(){

	IContractRootPtr				spRet		= NULL;
	CComObject<CEtsFutRootAtom>*	pNewRoot	= NULL;

	_CHK(CComObject<CEtsFutRootAtom>::CreateInstance(&pNewRoot),
		_T("Fail to create Future Root."));

	spRet.Attach(pNewRoot, true);

	return spRet;	
};
//--------------------------------------------------------------------------------------------------------//
/*virtual*/
IContractRootCollPtr
CFactory::CreateContractRootColl(){

	IContractRootCollPtr spRet						= NULL;
	CComObject<CContractRootColl>* pNewCollection	= NULL;

	_CHK(CComObject<CContractRootColl>::CreateInstance(&pNewCollection),
		_T("Fail to create ContractRoot Collection."));

	spRet.Attach(pNewCollection, true);

	return spRet;
};
//--------------------------------------------------------------------------------------------------------//
//container for asset correlation
CAssetCorrelation::CAssetCorrelation(){
};
//--------------------------------------------------------------------------------------------------------//
CAssetCorrelation::~CAssetCorrelation(){
	clear_surface();
};
//--------------------------------------------------------------------------------------------------------//
/*
@return value != BAD_DOUBLE_VALUE
*/
double
CAssetCorrelation::getCorrelation(const CAsset &assetA, const CAsset &assetB){
	try{
		CCorrelation::ptr correlation = find_correlation(assetA.m_nID, assetB.m_nID);

		if (correlation == 0){
			correlation = LoadCorrelation(assetA, assetB);
			if (correlation){
				add_new(assetA.m_nID, assetB.m_nID, correlation);
				return correlation->Correlation;
			}
		}
		else{
			return correlation->Correlation;
		}
	}
	catch(_com_error& err){
		TRACE_COM_ERROR(err);
		throw;
	}
	catch(...){
		TRACE_UNKNOWN_ERROR();
		throw;
	}
	return BAD_DOUBLE_VALUE;
};
//--------------------------------------------------------------------------------------------------------//
CAssetCorrelation::CCorrelation::ptr
CAssetCorrelation::LoadCorrelation(const CAsset& assetA, const CAsset& assetB){
	try{

		InitializeDB();

		/*init request*/
		CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_Correlation_Get");
		
		rs << assetA.m_nID;
		rs << assetB.m_nID;

		/*execute query*/
		rs.Open();

		//process recordset
		if(rs.GetRecordCount()){

			rs.MoveFirst();

			return ParseCorrelationRecordset(rs);
		}
	}
	catch(_com_error& err){
		TRACE_COM_ERROR(err);
		throw;
	}
	catch(...){
		TRACE_UNKNOWN_ERROR();
		throw;
	}
	return 0;
};
//--------------------------------------------------------------------------------------------------------//
CAssetCorrelation::CCorrelation::ptr
CAssetCorrelation::ParseCorrelationRecordset(CStoredProc<CClientRecordset>& recordset){
	try{

		CCorrelation::ptr correlation = CCorrelation::ptr(new CCorrelation);

		correlation->ComponentA		=	recordset[L"iComponentA"];
		correlation->ComponentB		=	recordset[L"iComponentB"];
		correlation->Correlation	=	recordset[L"fCorrelation"];
		
		return correlation;
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}	
	return 0;
};
//--------------------------------------------------------------------------------------------------------//
void
CAssetCorrelation::addCorrelation(const CAsset& assetA, const CAsset& assetB, double Correlation){
	CCorrelation::ptr correlation = CCorrelation::ptr(new CCorrelation);
	
	correlation->Correlation = Correlation;
	correlation->ComponentA = assetA.m_nID;
	correlation->ComponentB = assetB.m_nID;
	
	add_new(correlation->ComponentA, 
			correlation->ComponentB,	
			correlation);		
};
//--------------------------------------------------------------------------------------------------------//
/*static*/
CAssetCorrelation::ptr
CAssetCorrelation::getInstance(){
	try{
		static CAssetCorrelation::ptr __instance = 
			CAssetCorrelation::ptr(new CAssetCorrelation);

		return __instance;
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		throw;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		throw;
	}
	return 0;
};
//--------------------------------------------------------------------------------------------------------//
CAssetCorrelation::CCorrelation::CCorrelation(){
	CCorrelation::Correlation = BAD_DOUBLE_VALUE;
};
//--------------------------------------------------------------------------------------------------------//
CAssetCorrelation::CCorrelation::ptr
CAssetCorrelation::add_new(long ComponentA, long ComponentB, CAssetCorrelation::CCorrelation::ptr Correlation){

	CCorrelation::ptr Item = find_correlation(ComponentA, ComponentB);

	if (Item){	
		delete Item;
		Item = 0;
	}

	if (Correlation == 0)
		Correlation = CCorrelation::ptr(new CCorrelation);

	(m_Surface[ComponentA])[ComponentB] = (m_Surface[ComponentB])[ComponentA] = Correlation;

	return Correlation;
};
//--------------------------------------------------------------------------------------------------------//
CAssetCorrelation::CCorrelation::ptr
CAssetCorrelation::find_correlation(long ComponentA, long ComponentB){

	correlation_surface::iterator itX = m_Surface.find(ComponentA);
	if (itX != m_Surface.end()){
		correlation_set& y_set = itX->second;
		correlation_set::iterator itY = y_set.find(ComponentB);
		if (itY != y_set.end()){
			return itY->second;
		}
	}
	return 0;
};
//--------------------------------------------------------------------------------------------------------//
void
CAssetCorrelation::clear_surface(){
	correlation_surface::iterator it = m_Surface.begin();
	correlation_surface::iterator itEnd = m_Surface.end();

	for (;it != itEnd; it++){
		it->second.clear();
	};
	m_Surface.clear();
};
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
///*static*/ CAbstractSerializer::ptr
//CSerializer::getInstance(){
//	static CAbstractSerializer::ptr __instance = CAbstractSerializer::ptr(new CDbSerializer);
//	return __instance;
//};
////--------------------------------------------------------------------------------------------------------//
//CDbSerializerSettings::CDbSerializerSettings(std::string ConnectionString){
//	connection_string = ConnectionString;
//};
////--------------------------------------------------------------------------------------------------------//
//CDbSerializer::CDbSerializer(){
//};
////--------------------------------------------------------------------------------------------------------//
//CDbSerializer::~CDbSerializer(){
//};
////--------------------------------------------------------------------------------------------------------//
///*virtual*/ void
//CDbSerializer::Initialize(CSerializerSettings::ptr Settings){
//	__settings = CDbSerializerSettings::ptr(Settings);
//
//	//if (__settings && __settings->connection_string.length() > 0){
//	//	if (__dbConnection.IsOpened())
//	//		__dbConnection.Open(_bstr_t(__settings->connection_string.c_str()), 10, 120, 300, 300);
//	//};
//};
////--------------------------------------------------------------------------------------------------------//
///*virtual*/ long
//CDbSerializer::Serialize(CSpotOptionContract &Contract){
//	return 0;
//};
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
