// DBLayoutSQL.h: interface for the CDBLayoutSQL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBLAYOUTSQL_H__E491231D_4AA7_42D6_BA33_25D47B7BF581__INCLUDED_)
#define AFX_DBLAYOUTSQL_H__E491231D_4AA7_42D6_BA33_25D47B7BF581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBLayoutBase.h"

class CDBLayoutSQL : public CDBLayoutBase 
{
public:
	CDBLayoutSQL();
	~CDBLayoutSQL();

	CString GetVersion();

	void GetSymbols (CClientRecordset & rs); 
	void GetDividends(UINT uiSymbol, CClientRecordset & rs); 
	void GetSymbolParams(int nID, CClientRecordset & rs);
	void GetOptions (int nID, CURVE_MODE enCurveMode, CClientRecordset & rs);
	void GetRates (CClientRecordset & rs); 	

	void GetIRCurve (CClientRecordset & rs); 	
	void GetIRPoint (int iCurveID, CClientRecordset & rs);
	void GetUnderlyingPosGet (int nContractID, CClientRecordset & rs);
	void GetLastTradeSeqNum ( CClientRecordset & rs);

	void GetOptionRoot (int nRootID, CClientRecordset & rs);
	void GetSyntheticRootParams (int nRootID, CClientRecordset & rs);

	void GetIndexDefinition (int nID, CClientRecordset & rs);

	// Vola bands data
	void VolaBandsDataGet( int nContractID, CClientRecordset & rs );

	// Custom vola data
	void CustomStrikeSkewDataGet( int nContractID, DATE vtExpDate, int SetupMode, CClientRecordset & rs );
	void CustomStrikeSkewDataPut( CString& strContractID, DATE vtExpDate, int nPointID, double dVola );

	void CustomStrikeSkewPointGet( int nContractID, int SetupMode, CClientRecordset & rs );
	void CustomStrikeSkewPointPut( int nContractID, int SetupMode, int nPointID, float fStrike );
	void CustomStrikeSkewPointDel( int nContractID, int SetupMode, int nPointID );
	
    bool ClearOptopnsList(int iContractID);
	bool DeleteSymbol(int iContractID);

	void InterestRateCurveGet(int mode, CClientRecordset & rs);
	void InterestRateCurveSave(long& vtID, long lPeriodID, long iNumber, double Bid, double Ask);
	void InterestRateCurveDel(long lID);

	void UserActionControl(DATE PresentDateTime, short LogOnOut, _bstr_t DBLogin, _bstr_t  ServerName, _bstr_t CompanyName);

	void UpdateContractMarketData(int nID, double dPrice);
	void UpdateOptionDataCache(const _bstr_t & bstrSymbol, const COptionData & opt);

	double GetContractMarketPrice (int nID);
	void GetOptionsDataCache (int nID, CClientRecordset & rs, CURVE_MODE enCurveMode);
	void ClearOptionsDataCache( int nID );

	void UpdateContractData (int iContractID, const _bstr_t & bstrSymbol, PP::StockInfo * pResults);
	void UpdateOptionData(int iContractID, PP::OptionParams* pParams, PP::OptionInfo * pResults);

	bool IsSymbolExists(LPCTSTR szSymbol);
	int  AddNewContract(const _bstr_t & bstrSymbol, PP::StockInfo * pResults);
	void UpdateYield(int nID, double dYield);

	void GetSymbolGroups(CClientRecordset& rs); 
	void GetSymbolGroupItems(long nID, CClientRecordset& rs);

	void DeleteSymbolGroup(long nID);
	void SaveSymbolGroup(long& nID, const _bstr_t& bstrName, long nRuleID, bool bIsActive);
	void SaveSymbolGroupItem(long nID, long nGroupID);
	void ClearSymbolGroupItems(long nGroupID);

	void GetGenerationRule(long nID, CClientRecordset& rs); 
	void SaveGenerationRule(long& nID, long nType, const _bstr_t& bstrName, long nStrikeIncrement, long nRequiredPoints,
							double dSmileAccelerator, double dCallShift, double dPutShift,
							double dInterpolationFactor, double dStrikeMoneynessHi, double dStrikeMoneynessLo, bool bIsDefault); 
	void DeleteGenerationRule(long nID); 
	
	void VolaSurfaceDataGet(  long nContractID, long nSurfaceID, CClientRecordset& rs );
	long VolaSurfaceDataSave( long nSurfaceID, long nContractID, 
							  double dBaseUnderlinePrice, double dSmileAccelerator, double dInterpolationFactor, 
							  double dStrikeMoneynessHi,  double dStrikeMoneynessLo, 
							  LPCTSTR szSurfaceName, long nIsPriceOverride, long nIsDiscreteAcceleration, long nIsDefaultSurface );
	void VolaSurfaceDataDel( long nSurfaceID );
	CString VolaSurfaceNameGet(  long nSurfaceID );
	
	void CustomStrikeSkewPointsCopy( long nSourceSurfaceID, long nDestSurfaceID );

	void ContractGenerationRuleGet( long nID, CClientRecordset& rs );
	void ContractGenerationRuleSave( long nID, long nRuleID );

	void NextDaySurfaceGet( long nID, CClientRecordset& rs );
	void NextDaySurfaceSave( long& nID, long nContractID, long nSurfaceID, DATE dtDueDate );
	void NextDaySurfaceDel( long nID, long nContractID, long nSurfaceID );

#ifdef _VOLA_MANAGER
	void CusomVolatilityPointIsExistGet( CClientRecordset& rs );
	void CustomStrikeSkewDefaultPointSave( long nContractID, DATE dtExpDate, EOptType OptType, double dVola );
#endif

protected:	
	int GetExchangeID (const _bstr_t & bstrCode);
};

#endif // !defined(AFX_DBLAYOUTSQL_H__E491231D_4AA7_42D6_BA33_25D47B7BF581__INCLUDED_)
