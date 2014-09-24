// DBLayoutSQL.cpp: implementation of the CDBLayoutSQLSQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ivanalyzer.h"
#include "DBLayoutSQL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDBLayoutSQL::CDBLayoutSQL()
{
}

CDBLayoutSQL::~CDBLayoutSQL()
{
}

void CDBLayoutSQL::GetDividends(UINT uiSymbol, CClientRecordset & rs)
{
	ASSERT (m_db.IsOpened());

	CStoredProc <> spGetDividends (m_db, L"usp_MmCustomDividend_Get");
	spGetDividends << (long)uiSymbol;
	rs.Open(spGetDividends);
}

void CDBLayoutSQL::GetSymbols (CClientRecordset & rs) 
{
	ASSERT (m_db.IsOpened());

	CStoredProc <> spGetSymbols (m_db, L"usp_Contract_Get");
	spGetSymbols << CDBNull() << CDBNull() << 1;


	rs.Open(spGetSymbols);
}

void CDBLayoutSQL::GetSymbolParams(int nID, CClientRecordset & rs)
{
	ASSERT (m_db.IsOpened());
	
	CStoredProc <> spGetSymbols (m_db, L"usp_Contract_Get");
	spGetSymbols << nID << CDBNull() << 1;

	rs.Open(spGetSymbols);
}

void CDBLayoutSQL::GetOptions (int nID, CURVE_MODE enCurveMode, CClientRecordset & rs)
{
	CStoredProc <> spGetOption  (m_db, L"usp_OptionVM_Get");

	COleDateTime dt =  COleDateTime::GetCurrentTime();
	dt.m_dt = (long) dt.m_dt;

	spGetOption << CDBNull() << nID << vt_date(dt); 
	
	AddCallParam (spGetOption, enCurveMode);
	rs.Open (spGetOption );
}

void CDBLayoutSQL::GetOptionRoot (int nRootID, CClientRecordset & rs)
{
	CStoredProc <> spGetOptionRoot  (m_db, L"usp_OptionRoot_Get");

	spGetOptionRoot << nRootID; 
	
	rs.Open (spGetOptionRoot );
}

void CDBLayoutSQL::GetSyntheticRootParams (int nRootID, CClientRecordset & rs)
{
	CStoredProc <> spGetSyntheticRootParams  (m_db, L"usp_SyntheticParamsVM_Get");

	spGetSyntheticRootParams << nRootID; 
	
	rs.Open (spGetSyntheticRootParams );
}

void CDBLayoutSQL::GetIndexDefinition (int nID, CClientRecordset & rs)
{
	CStoredProc <> spIndexDef  (m_db, L"usp_IndexDefinition_Get");

	spIndexDef << nID; 
	
	rs.Open (spIndexDef );
}

void CDBLayoutSQL::CustomStrikeSkewDataGet( int nContractID, DATE vtExpDate, int SetupMode, CClientRecordset & rs )
{
	CStoredProc<> spGetStrikeSkew( m_db, L"usp_CustomStrikeSkewData_Get" );
	spGetStrikeSkew << nContractID << vtExpDate << SetupMode;
	rs.Open( spGetStrikeSkew );
}


void CDBLayoutSQL::CustomStrikeSkewDataPut( CString& strContractID, DATE vtExpDate, int nPointID, double dVola )
{
	CStoredProc<> spPutStrikeSkew( m_db, L"usp_CustomStrikeSkewData_Save" );
	
	spPutStrikeSkew << CDBNull();
	spPutStrikeSkew << nPointID;
	spPutStrikeSkew << dVola;
	spPutStrikeSkew << CDBNull();
	spPutStrikeSkew << _bstr_t((LPCTSTR)strContractID);
	spPutStrikeSkew << vtExpDate;

	spPutStrikeSkew.Execute();
}


void CDBLayoutSQL::CustomStrikeSkewPointGet( int nContractID, int SetupMode, CClientRecordset & rs )
{
	CStoredProc<> spGetStrikeSkewPoint( m_db, L"usp_CustomStrikeSkewPoint_Get" );
	spGetStrikeSkewPoint << nContractID << SetupMode << CDBNull() ;
	rs.Open( spGetStrikeSkewPoint );
}


void CDBLayoutSQL::CustomStrikeSkewPointPut( int nContractID, int SetupMode, int nPointID, float fStrike )
{
	CStoredProc<> spPutStrikeSkewPoint( m_db, L"usp_CustomStrikeSkewPoint_Save" );
	if( nPointID == 0 )
		spPutStrikeSkewPoint << CDBNull();		//@iCustomStrikeSkewPointID
	else
		spPutStrikeSkewPoint << nPointID;	//@iCustomStrikeSkewPointID
	spPutStrikeSkewPoint << CDBNull();			//@iVolaManagementDataID
	spPutStrikeSkewPoint << SetupMode;		//@iSetupMode
	spPutStrikeSkewPoint << fStrike;		//@fStrike
	spPutStrikeSkewPoint << nContractID;		//@iContractID

	spPutStrikeSkewPoint.Execute();
}


void CDBLayoutSQL::CustomStrikeSkewPointDel( int nContractID, int SetupMode, int nPointID )
{
	CStoredProc<> spDelStrikeSkewPoint( m_db, L"usp_CustomStrikeSkewPoint_Del" );
	spDelStrikeSkewPoint << nContractID;		//@iContractID
	spDelStrikeSkewPoint << SetupMode;		//@iSetupMode
	if( nPointID == 0 )
		spDelStrikeSkewPoint << CDBNull();		//@iCustomStrikeSkewPointID
	else
		spDelStrikeSkewPoint << nPointID;	//@iCustomStrikeSkewPointID

	spDelStrikeSkewPoint.Execute();
}


bool CDBLayoutSQL::ClearOptopnsList(int iContractID)
{
	CStoredProc<> spClearOptopnsList( m_db, L"usp_Option_Del" );

	spClearOptopnsList << iContractID;

	spClearOptopnsList.Execute();

	return true;
}


bool CDBLayoutSQL::DeleteSymbol(int iContractID)
{
	if( !ClearOptopnsList(iContractID) )
		return false;

	CStoredProc<> spDeleteSymbol( m_db, L"usp_Contract_Del" );

	spDeleteSymbol<< iContractID;

	spDeleteSymbol.Execute();

	return true;
}

	
void CDBLayoutSQL::GetRates (CClientRecordset & rs) 	
{
	ASSERT (m_db.IsOpened());

	CStoredProc<> spInterestRate (m_db, L"usp_InterestRateCurve_Get");
	
	spInterestRate << CDBNull() << 1;

	rs.Open (spInterestRate);

}

void CDBLayoutSQL::InterestRateCurveSave(long& lID, long lPeriodID, long iNumber, double Bid, double Ask)
{

	CStoredProc<> spInterestRateCurve( m_db, L"usp_InterestRateCurve_Save" );
	long lCurvePointID = 0;
	if(lID)
		spInterestRateCurve << lID; // @iCurvePointID int = null output,
	else
		spInterestRateCurve << &lCurvePointID; // @iCurvePointID int = null output,
	spInterestRateCurve << lPeriodID; //@iPeriodTypeID int = null,
	spInterestRateCurve << iNumber; //@iNumber int = null,
	spInterestRateCurve << Ask; //@fRateAsk float = null,
	spInterestRateCurve << Bid; //@fRateBid float = null
	spInterestRateCurve.Execute();
	
	if(!lID)
		lID = lCurvePointID;
}

void CDBLayoutSQL::UserActionControl(DATE PresentDateTime, short LogOnOut, _bstr_t DBLogin, _bstr_t  ServerName, _bstr_t CompanyName)
{
	try
	{
		CStoredProc<> spUserActionControl( m_db, L"usp_RegUserAction_Save" );

		spUserActionControl << PresentDateTime; 
		spUserActionControl << LogOnOut; 
		spUserActionControl << DBLogin; 
		spUserActionControl << ServerName;
		spUserActionControl << CompanyName;
		spUserActionControl.Execute();
	}
	catch (_com_error& /*e*/)
	{
		
	}
}

void CDBLayoutSQL::InterestRateCurveDel(long lID)
{
	CStoredProc<> spInterestRateCurve( m_db, L"usp_InterestRateCurve_Del" );

	spInterestRateCurve << lID;
	spInterestRateCurve.Execute();
}

void CDBLayoutSQL::InterestRateCurveGet(int mode, CClientRecordset & rs)
{
	CStoredProc<> spInterestRateCurve( m_db, L"usp_InterestRateCurve_Get" );
	spInterestRateCurve<<CDBNull();
	spInterestRateCurve<<mode;

	rs.Open(spInterestRateCurve);
}

void CDBLayoutSQL::UpdateContractMarketData(int nID, double dPrice)
{
	ASSERT (m_db.IsOpened() );

	CStoredProc<> spSaveContractMarketData  (m_db, L"usp_ContractMarketData_Save");
	spSaveContractMarketData << nID << CDBNull() << dPrice;
	
	spSaveContractMarketData.Execute ();
}

void CDBLayoutSQL::UpdateOptionDataCache(const _bstr_t & bstrSymbol, const COptionData & opt)
{
	ASSERT (m_db.IsOpened() );
	CStoredProc <> spSaveOptionDataCache (m_db, L"usp_OptionDataCache_Save");

	spSaveOptionDataCache 
		<< bstrSymbol
		<< vt_date(opt.m_dtPriceDate)
		<< opt.m_dVola 
		<< opt.m_dPrice;
	
	spSaveOptionDataCache.Execute();
}

void CDBLayoutSQL::ClearOptionsDataCache( int nID )
{
	ASSERT (m_db.IsOpened() );
	
	CStoredProc <> spDel (m_db, L"usp_OptionDataCache_Del");
	spDel << CDBNull() << nID;
	
	spDel.Execute();
}

double CDBLayoutSQL::GetContractMarketPrice (int nID)
{
	ASSERT (m_db.IsOpened() );
	
	CStoredProc <CDefaultRecordset> spGetPrice (m_db, L"usp_ContractMarketData_Get");
	
	spGetPrice << nID;

	spGetPrice.Open();

	if (! spGetPrice.IsEOF() )
		return spGetPrice[L"fPriceMid"];
	else
		return 0;
}

void CDBLayoutSQL::GetOptionsDataCache (int nID, CClientRecordset & rs, CURVE_MODE enCurveMode)
{
	ASSERT (m_db.IsOpened() );

	CStoredProc <> spGetOptionsCache (m_db, L"usp_OptionDataCache_Get");
	
	spGetOptionsCache << nID << CDBNull() ;
	AddCallParam (spGetOptionsCache, enCurveMode);

	rs.Open (spGetOptionsCache);
}

void CDBLayoutSQL::UpdateOptionData(int iContractID, PP::OptionParams* pParams, PP::OptionInfo * pResults)
{
	ASSERT (pResults);
	ASSERT (m_db.IsOpened() );

	
	CStoredProc <> spOptionSave( m_db, L"usp_OptionVM_Save" );

	spOptionSave <<CDBNull();				//	@iOptionID int = null output,

	spOptionSave << 3 ;					//	@iContractTypeID int = null,

	spOptionSave <<  _bstr_t(pResults->Series);	//	@iSymbolName varchar(12) = null,
	spOptionSave <<  CDBNull();			//	@iExchangeID int = null,
	spOptionSave <<  CreateOptionDescription(pParams, pResults);			//	@vcDescription varchar(50) = null,
	spOptionSave <<  pResults->LotSize;	//	@iLotSize int = null,

	spOptionSave <<  iContractID;			//	@iUnderlyingID int = null,

	spOptionSave <<  (pResults->OptionType == PP::enCall ? 1 : 0) ; //	@iIsCall bit = null,

	spOptionSave <<  pResults->StrikePrice;				//	@fStrike float = null,
	spOptionSave <<  vt_date(pResults->ExpirationDate);	//	@dtExpDate datetime = null

	spOptionSave.Execute();

}

void CDBLayoutSQL::UpdateContractData (int iContractID, const _bstr_t & bstrSymbol, PP::StockInfo * pResults)
{
	ASSERT (pResults);
	ASSERT (m_db.IsOpened() );

	CStoredProc<> spContractSave( m_db, L"usp_ContractVM_Save" );

	spContractSave 
		<< iContractID						//	@iContractID int = null output,
		<< CDBNull()							// @iContractTypeID int = null, 
		<< CDBNull()							//@vcSymbolName varchar(12) = null,	-- index's ticker
		<< CDBNull()							//@iExchangeID int = null,		-- exchange's identifier
		<< _bstr_t(pResults->Description)	//@vcDescription varchar(50) = null,	-- description
		<< pResults->LotSize							//@iLotSize int = null,			-- lot size (not used)
		<< pResults->DivFreq				
		<< pResults->DivAmount
		<< vt_date(pResults->DivLastDate);

	spContractSave.Execute();
}

CString CDBLayoutSQL::GetVersion()
{
	ASSERT (m_db.IsOpened() );
	
	CStoredProc <CDefaultRecordset> spVer (m_db, L"usp_DataInfo_Get");	
	spVer << 1;
	
	spVer.Open();

	return  (LPCTSTR) (_bstr_t) spVer[L"vcKeyValue"];
}


bool CDBLayoutSQL::IsSymbolExists(LPCTSTR szSymbol)
{
	ASSERT( m_db.IsOpened() );

	CStoredProc <CDefaultRecordset> spIsExists(m_db, L"usp_IsSymbolExists_Get");	
	spIsExists << _bstr_t(szSymbol);

	spIsExists.Open();

	return !spIsExists.IsEOF();
}


int CDBLayoutSQL::AddNewContract(const _bstr_t & bstrSymbol, PP::StockInfo * pResults)
{
	ASSERT(FALSE);
	return 0;
/*
	ASSERT (m_db.IsOpened());
	ASSERT (pResults);

	int nContractID = 0;

	m_db.BeginTran();
	try
	{
		CSQLQuery<> cmdInsert(m_db, 
			L"INSERT INTO Contract ( contractTypeID, symbolName, exchangeID, description, lotSize ) VALUES (?, ?, ?, ?, ?)");
		
		int nContractType = (pResults->StockType  == MTP::enStock) ? 2 : 1;
		int nExchangeID =GetExchangeID (pResults->Exchange);
		
		cmdInsert << nContractType << bstrSymbol;

		if (nExchangeID == 0)
			cmdInsert << CDBNull(); 
		else
			cmdInsert << nExchangeID;

		cmdInsert << pResults->Description  << pResults->LotSize;
		cmdInsert.Execute();

		CSQLQuery<CDefaultRecordset> cmdGetID(m_db, 
			L"SELECT contractID FROM Contract WHERE contractTypeID = ? and symbolName = ?");

		cmdGetID << nContractType << bstrSymbol ;
		cmdGetID.Open();

		ASSERT ( !cmdGetID.IsEOF() );
		nContractID = cmdGetID[L"contractID"];

		if (pResults->StockType  == MTP::enStock)
		{
			CSQLQuery<> cmdStockInsert(m_db, 
				L"INSERT INTO [Stock] ( stockID, divFrequency) VALUES (?, ?)");
			
			cmdStockInsert << nContractID << pResults->DivFreq;
			cmdStockInsert.Execute();
			
			CSQLQuery<> cmdDivInsert(m_db, 
				L"INSERT INTO Dividend ( stockID, amount, [date]) VALUES (?, ?, ?)");

			cmdDivInsert << nContractID << pResults->DivAmount << pResults->DivLastDate;
			cmdDivInsert.Execute();		
			
		}

		m_db.Commit();
	}
	catch (...)
	{
		m_db.Rollback();
		throw;
	}

	return nContractID;
*/
}


int CDBLayoutSQL::GetExchangeID(const _bstr_t & bstrCode)
{
	ASSERT(FALSE);
	return 0;
/*
	ASSERT (m_db.IsOpened());

	CSQLQuery<CDefaultRecordset> cmdExchange(m_db, 
		L"SELECT exchangeID FROM exchange WHERE mtk_code = ?");
	cmdExchange << bstrCode;
	cmdExchange.Open();
	
	if(!cmdExchange.IsEOF())
		return  cmdExchange[L"exchangeID"];
	else
		return 0;
*/
}


void CDBLayoutSQL::UpdateYield(int nID, double dYield)
{
	ASSERT (m_db.IsOpened());
	
	CStoredProc<> sp(m_db, L"usp_IndexYield_Save");
	sp << nID;
	sp << dYield;

	sp.Execute();
}

void CDBLayoutSQL::GetSymbolGroups(CClientRecordset& rs)
{
	ASSERT(m_db.IsOpened());

	CStoredProc<> sp(m_db, L"usp_GenerationRuleGroup_Get");
	sp << CDBNull();

	rs.Open(sp);
}

void CDBLayoutSQL::GetSymbolGroupItems(long nID, CClientRecordset& rs)
{
	ASSERT(m_db.IsOpened());

	CStoredProc<> sp(m_db, L"usp_ContractByGroupID_Get");
	if( nID != 0 )
		sp << nID;
	else
		sp << CDBNull();

	rs.Open(sp);
}

void CDBLayoutSQL::DeleteSymbolGroup(long nID)
{
	ASSERT(m_db.IsOpened());
	CStoredProc<> sp(m_db, L"usp_GenerationRuleGroup_Del");

	sp << nID;

	sp.Execute();
}

void CDBLayoutSQL::SaveSymbolGroup(long& nID, const _bstr_t& bstrName, long nRuleID, bool bIsActive)
{
	ASSERT(m_db.IsOpened());
	CStoredProc<> sp(m_db, L"usp_GenerationRuleGroup_Save");

	if( nID != 0 )
		sp << nID;
	else
		sp << CDBNull();

	sp << bstrName;

	if( nRuleID != 0 )
		sp << nRuleID;
	else
		sp << CDBNull();

	sp << bIsActive;

	sp.Execute();

	nID = sp.GetResult();
}

void CDBLayoutSQL::ClearSymbolGroupItems(long nGroupID)
{
	ASSERT(m_db.IsOpened());
	CStoredProc<> sp(m_db, L"usp_GenerationGroupItemsClear_Save");

	sp << nGroupID;

	sp.Execute();
}

void CDBLayoutSQL::SaveSymbolGroupItem(long nID, long nGroupID)
{
	ASSERT(m_db.IsOpened());
	CStoredProc<> sp(m_db, L"usp_ContractGroupItem_Save");

	sp << nID;

	if( nGroupID != 0 )
		sp << nGroupID;
	else
		sp << CDBNull();

	sp.Execute();
}

void CDBLayoutSQL::GetGenerationRule(long nID, CClientRecordset& rs)
{
	ASSERT(m_db.IsOpened());

	CStoredProc<> sp(m_db, L"usp_GenerationRule_Get");
	if( nID != 0 )
		sp << nID;
	else
		sp << CDBNull();

	rs.Open(sp);
}

void CDBLayoutSQL::SaveGenerationRule(long& nID, long nType, const _bstr_t& bstrName, long nStrikeIncrement, long nRequiredPoints,
										double dSmileAccelerator, double dCallShift, double dPutShift,
										double dInterpolationFactor, double dStrikeMoneynessHi, double dStrikeMoneynessLo, bool bIsDefault) 
{
	ASSERT(m_db.IsOpened());
	CStoredProc<> sp(m_db, L"usp_GenerationRule_Save");

	if(nID)								// @iRuleID int=Null
		sp << nID;
	else
		sp << CDBNull();

	sp << nType							// @iRuleTypeID int=Null
		<< bstrName						// @vcRuleName varchar(250)=Null
		<< dInterpolationFactor			// @fInterpolationFactor float=Null,--the Factor of Interpolation
		<< dCallShift					// @fCallShift int=Null,				--the CallRange
		<< dPutShift					// @fPutShift int=Null,				--the PutRange
		<< dSmileAccelerator			// @fSmileAccelerator float=Null,		--the Smile Accelerator
		<< nStrikeIncrement				// @iStrikeIncrement int=Null,			--the Strike Increment
		<< dStrikeMoneynessHi			// @fStrikeMoneynessHi float=Null,			--the Amount Of Points Required
		<< dStrikeMoneynessLo			// @fStrikeMoneyness float=Null,			--the Amount Of Points low value
		<< bIsDefault					// @bIsDefaultRule bit=Null			--the IsDefault atribute
		<< nRequiredPoints;				// @iRequiredPoints int=Null			--the number of Points Required

	sp.Execute();

	nID = sp.GetResult();
}

void CDBLayoutSQL::DeleteGenerationRule(long nID)
{
	ASSERT(m_db.IsOpened());
	CStoredProc<> sp(m_db, L"usp_GenerationRule_Del");

	sp << nID;

	sp.Execute();
}


void CDBLayoutSQL::VolaSurfaceDataGet( long nContractID, long nSurfaceID, CClientRecordset& rs )
{
	ASSERT(m_db.IsOpened());

	CStoredProc<> sp(m_db, L"usp_VolaSurfaceData_Get");
	if( nContractID != 0 )
		sp << nContractID;
	else
		sp << CDBNull();
	if( nSurfaceID > 0 )
		sp << nSurfaceID;
	else
		sp << CDBNull();

	rs.Open(sp);
}


CString CDBLayoutSQL::VolaSurfaceNameGet(  long nSurfaceID )
{
	ASSERT(m_db.IsOpened());

	CString strName;

	try
	{
	CStoredProc<CDefaultRecordset> sp(m_db, L"usp_VolaSurfaceData_Get");
	sp << CDBNull();
	sp << nSurfaceID;

	sp.Open();

	strName = (LPCTSTR)(_bstr_t)sp[L"vcSurfaceName"];
	}
	catch( ... )
	{
	}

	return strName;
}


long CDBLayoutSQL::VolaSurfaceDataSave( long nSurfaceID, long nContractID, 
										double dBaseUnderlinePrice, double dSmileAccelerator, double dInterpolationFactor, 
										double dStrikeMoneynessHi,  double dStrikeMoneynessLo, 
										LPCTSTR szSurfaceName, 
										long nIsPriceOverride, long nIsDiscreteAcceleration, long nIsDefaultSurface )
{
	ASSERT(m_db.IsOpened());
	long nNewID = nSurfaceID;
	
	CStoredProc<> sp(m_db, L"usp_VolaSurfaceData_Save");
	if( nSurfaceID > 0 )
		sp << nSurfaceID;
	else
		sp << &nNewID;

	if( nContractID != 0 )
		sp << nContractID;
	else
		sp << CDBNull();

	if( dBaseUnderlinePrice >= 0 )
		sp << dBaseUnderlinePrice;
	else
		sp << CDBNull();

	if( dSmileAccelerator >= 0 )
		sp << dSmileAccelerator;
	else
		sp << CDBNull();

	if( dInterpolationFactor >= 0 )
		sp << dInterpolationFactor;
	else
		sp << CDBNull();

	if( dStrikeMoneynessHi >= 0 )
		sp << dStrikeMoneynessHi;
	else
		sp << CDBNull();

	if( dStrikeMoneynessLo >= 0 )
		sp << dStrikeMoneynessLo;
	else
		sp << CDBNull();

	if( szSurfaceName > NULL )
		sp << _bstr_t(szSurfaceName);
	else
		sp << CDBNull();

	if( nIsPriceOverride >= 0 )
		sp << nIsPriceOverride;
	else
		sp << CDBNull();

	if( nIsDiscreteAcceleration >= 0 )
		sp << nIsDiscreteAcceleration;
	else
		sp << CDBNull();

	if( nIsDefaultSurface >= 0 )
		sp << nIsDefaultSurface;
	else
		sp << CDBNull();

	sp << CDBNull();

	sp.Execute();

	return nNewID;
}

	
void CDBLayoutSQL::VolaSurfaceDataDel( long nSurfaceID )
{
	ASSERT(m_db.IsOpened());
	
	CStoredProc<> sp(m_db, L"usp_VolaSurfaceData_Del");
	if( nSurfaceID > 0 )
		sp << nSurfaceID;
	else
		sp << CDBNull();

	sp.Execute();
}


void CDBLayoutSQL::CustomStrikeSkewPointsCopy( long nSourceSurfaceID, long nDestSurfaceID )
{
	ASSERT(m_db.IsOpened());
	
	CStoredProc<> sp(m_db, L"usp_CustomStrikeSkewPoint");
	sp << nSourceSurfaceID;
	sp << nDestSurfaceID;

	sp.Execute();
}


void CDBLayoutSQL::ContractGenerationRuleGet( long nID, CClientRecordset& rs )
{
	ASSERT(m_db.IsOpened());

	CStoredProc<> sp(m_db, L"usp_ContractGenerationRule_Get");
	sp << nID;

	rs.Open(sp);
}


void CDBLayoutSQL::ContractGenerationRuleSave( long nID, long nRuleID )
{
	ASSERT(m_db.IsOpened());
	
	CStoredProc<> sp(m_db, L"usp_ContractVM_Save");
	sp << nID;//		@iContractID int = null output,		-- contract's identifier

	sp << CDBNull();//		@iContractTypeID int = null,		-- contract's type identifier
	sp << CDBNull();//		@vcSymbolName varchar(12) = null,	-- index's ticker
	sp << CDBNull();//		@iExchangeID int = null,		-- exchange's identifier
	sp << CDBNull();//		@vcDescription varchar(50) = null,	-- description
	sp << CDBNull();//		@iLotSize int = null,			-- lot size (not used)

	sp << CDBNull();//		@iDivFrequency int = null,		-- dividend's frequency
	sp << CDBNull();//		@fAmount float = null, 			-- amount
	sp << CDBNull();//		@dDivDate datetime = null,		-- dividend's date

	sp << CDBNull();//		@fYield float = null,			-- yield
	sp << CDBNull();//		@bIsAutoUpdateWeight bit = null,	-- flag for enable/disable index's weights updating

	sp << CDBNull();//		@iIndexBaseID int = null,		-- index's base identifier
	sp << CDBNull();//		@rDenominator real = null,		-- denominator
	sp << CDBNull();//		@bIsActive bit = null,			-- flag for enable/disable

	sp << CDBNull();//		@iUnderlyingID int = null,		-- underlying identifier
	sp << CDBNull();//		@iIsCall bit = null,			-- flag for call/put
	sp << CDBNull();//		@fStrike float = null,			-- strike
	sp << CDBNull();//		@dtExpDate datetime = null,		-- expiration's date

	sp << CDBNull();//		@iGroupID int = null,
	sp << nRuleID;//	@iRuleID int = null

	sp.Execute();
}


void CDBLayoutSQL::NextDaySurfaceGet( long nID, CClientRecordset& rs )
{
	ASSERT(m_db.IsOpened());

	CStoredProc<> sp(m_db, L"usp_NextDaySurface_Get");
	if( nID == 0 )
		sp << CDBNull();
	else
		sp << nID;

	rs.Open(sp);
}


void CDBLayoutSQL::NextDaySurfaceSave( long& nID, long nContractID, long nSurfaceID, DATE dtDueDate )
{
	ASSERT(m_db.IsOpened());
	
	CStoredProc<> sp(m_db, L"usp_NextDaySurface_Save");
	if( nID > 0 )
		sp << nID;
	else
		sp << CDBNull();

	if( nContractID == 0 )
		sp << CDBNull();
	else
		sp << nContractID;

	if( nSurfaceID > 0 )
		sp << nSurfaceID;
	else
		sp << CDBNull();

	if( dtDueDate > 0 )
		sp << vt_date( dtDueDate );
	else
		sp << CDBNull();

	sp.Execute();
	nID = sp.GetResult();
}


void CDBLayoutSQL::NextDaySurfaceDel( long nID, long nContractID, long nSurfaceID )
{
	ASSERT(m_db.IsOpened());
	
	CStoredProc<> sp(m_db, L"usp_NextDaySurface_Del");
	if( nID > 0 )
		sp << nID;
	else
		sp << CDBNull();

	if( nContractID == 0 )
		sp << CDBNull();
	else
		sp << nContractID;

	if( nSurfaceID > 0 )
		sp << nSurfaceID;
	else
		sp << CDBNull();

	sp.Execute();
}

#ifdef _VOLA_MANAGER

void CDBLayoutSQL::CusomVolatilityPointIsExistGet( CClientRecordset& rs )
{
	ASSERT(m_db.IsOpened());

	CStoredProc<> sp(m_db, L"usp_CustomVolatilityPointIsExist_Get");

	rs.Open(sp);
}


void CDBLayoutSQL::CustomStrikeSkewDefaultPointSave( long nContractID, DATE dtExpDate, EOptType OptType, double dVola )
{
	ASSERT(m_db.IsOpened());
	
	CStoredProc<> sp(m_db, L"usp_CustomStrikeSkewDefaultPoint_Save");
	sp << nContractID;
	sp << vt_date(dtExpDate);
	sp << OptType;
	sp << dVola;

	sp.Execute();
}

void CDBLayoutSQL::GetIRCurve(CClientRecordset & rs) 	
{
	ASSERT (m_db.IsOpened());

	CStoredProc<> spIRCurve (m_db, L"usp_IRCurve_Get");
	
	spIRCurve << CDBNull(); // CurveID == NULL

	rs.Open (spIRCurve);
}

void CDBLayoutSQL::GetIRPoint( int iCurveID, CClientRecordset & rs)
{
	ASSERT (m_db.IsOpened());

	CStoredProc<> spIRPoint (m_db, L"usp_IRPoint_Get");
	
	spIRPoint << iCurveID;

	rs.Open (spIRPoint);

}
void CDBLayoutSQL::GetUnderlyingPosGet (int nContractID, CClientRecordset & rs)
{
	ASSERT (m_db.IsOpened());

	CStoredProc<> spUndPosGet (m_db, L"usp_UnderlyingPos_Get");
	
	spUndPosGet << nContractID;

	rs.Open (spUndPosGet);
}

void CDBLayoutSQL::GetLastTradeSeqNum ( CClientRecordset & rs)
{
	ASSERT (m_db.IsOpened());

	CStoredProc<> spUndPosGet (m_db, L"usp_LastTradeSeqNum_Get");
	
	rs.Open (spUndPosGet);
}

void CDBLayoutSQL::VolaBandsDataGet( int nContractID, CClientRecordset & rs )
{
	ASSERT (m_db.IsOpened());

	CStoredProc<> spVolaBandsGet (m_db, L"usp_VolaBands_Get");
	spVolaBandsGet << nContractID;
	
	rs.Open (spVolaBandsGet);
}


#endif