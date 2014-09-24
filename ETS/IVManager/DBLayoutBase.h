// DBLayoutBase.h: interface for the CDBLayoutBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBLAYOUTBASE_H__6F6F2372_8BAC_441A_8893_39845244779C__INCLUDED_)
#define AFX_DBLAYOUTBASE_H__6F6F2372_8BAC_441A_8893_39845244779C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDef.h"

class CDBLayoutBase  
{
public:
	CDBLayoutBase();
	virtual ~CDBLayoutBase();

	void Connect (LPCTSTR szConn = theApp.GetConnection() );
	void Disconnect ();
	bool IsConnected() {return m_bConnected;}

	void BeginTran();
	void Commit();
	void Rollback();


	virtual CString GetVersion()=0;

	virtual void GetSymbols (CClientRecordset & rs)=0; 
	virtual void GetSymbolParams(int nID, CClientRecordset & rs)=0;
	virtual void GetOptions (int nID, CURVE_MODE enCurveMode, CClientRecordset & rs)=0;
	virtual void GetRates (CClientRecordset & rs)=0; 	
	
	virtual void InterestRateCurveGet(int mode, CClientRecordset & rs)=0;
	virtual void InterestRateCurveSave(long& vtID, long lPeriodID, long iNumber, double Bid, double Ask)=0;
	virtual void InterestRateCurveDel(long lID)=0;

	virtual void UpdateContractMarketData(int nID, double dPrice)=0;
	virtual void UpdateOptionDataCache(const _bstr_t & bstrSymbol, const COptionData & opt)=0;

	virtual double GetContractMarketPrice (int nID)=0;
	virtual	void GetOptionsDataCache (int nID, CClientRecordset & rs, CURVE_MODE enCurveMode)=0;
	
	virtual void ClearOptionsDataCache (int nID)=0;

	virtual void UpdateContractData (int iContractID, const _bstr_t & bstrSymbol, PP::StockInfo * pResults)=0;
	virtual void UpdateOptionData(int iContractID, PP::OptionParams* pParams, PP::OptionInfo * pResults)=0;
	virtual bool IsSymbolExists(LPCTSTR szSymbol)=0;
	virtual int AddNewContract(const _bstr_t & bstrSymbol, PP::StockInfo * pResults)=0;
	
	virtual bool ClearOptopnsList(int iContractID) = 0;

	 _bstr_t CreateOptionDescription(PP::OptionParams* pParams, PP::OptionInfo * pResults);
	
#ifdef _VOLA_MANAGER

	virtual void GetSymbolGroups(CClientRecordset& rs) = 0; 
	virtual void GetSymbolGroupItems(long nID, CClientRecordset& rs) = 0;

	virtual void DeleteSymbolGroup(long nID) = 0;
	virtual void SaveSymbolGroup(long& nID, const _bstr_t& bstrName, long nRuleID, bool bIsActive) = 0;
	virtual void ClearSymbolGroupItems(long nGroupID) = 0;
	virtual void SaveSymbolGroupItem(long nID, long nGroupID) = 0;

	virtual void GetGenerationRule(long nID, CClientRecordset& rs) = 0; 
	virtual void SaveGenerationRule(long& nID, long nType, const _bstr_t& bstrName, long nStrikeIncrement, long nRequiredPoints,
							double dSmileAccelerator, double dCallShift, double dPutShift,
							double dInterpolationFactor, double dStrikeMoneynessHi, double dStrikeMoneynessLo, bool bIsDefault) = 0; 
	virtual void DeleteGenerationRule(long nID) = 0; 

#endif //_VOLA_MANAGER

protected:
	
	void AddCallParam (EgLib::CDBCommand& sp, CURVE_MODE enMode)
	{
		if (enMode  < enOTM )
			sp << (enMode == enCall ? 1 : 0) ;
		else
			sp << EgLib::CDBNull();
	}

	bool m_bConnected;
	CDBConnection m_db;

};

#endif // !defined(AFX_DBLAYOUTBASE_H__6F6F2372_8BAC_441A_8893_39845244779C__INCLUDED_)
