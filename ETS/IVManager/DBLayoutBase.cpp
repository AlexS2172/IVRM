// DBLayoutBase.cpp: implementation of the CDBLayoutBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ivanalyzer.h"
#include "DBLayoutBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBLayoutBase::CDBLayoutBase(): m_bConnected (false)
{
}

CDBLayoutBase::~CDBLayoutBase()
{
	if (m_bConnected)
		Disconnect();
}


void CDBLayoutBase::Connect (LPCTSTR szConn)
{
	if ( _tcslen (szConn) == 0 )
		utils::ThrowError ( E_FAIL, L"No database connection defined");

	if (! m_db.IsOpened())
		m_db.Open( _bstr_t( szConn), 150, 200);
	m_bConnected = true;
}	


void CDBLayoutBase::Disconnect ()
{
//	if (--m_nRefCnt == 0)
	m_db.Close();
	m_bConnected = false;
}


void CDBLayoutBase::BeginTran()
{
	m_db.BeginTran();
}


void CDBLayoutBase::Commit()
{
	if(m_db.InTransaction())
		m_db.Commit();
}


void CDBLayoutBase::Rollback()
{
	if(m_db.InTransaction())
		m_db.Rollback();
}

 _bstr_t CDBLayoutBase::CreateOptionDescription(PP::OptionParams* pParams, PP::OptionInfo * pResults)
 {
	 vt_date dt(pResults->ExpirationDate);
	 LPTSTR  arrMonth[12] = 
	 {
		 _T(" January "),
		 _T(" February "),
		 _T(" March "),
		 _T(" April "),
		 _T(" May "),
		 _T(" June "),
		 _T(" July "),
		 _T(" August "),
		 _T(" September "),
		 _T(" October "),
		 _T(" November "),
		 _T(" December "),
	 };

	 _bstr_t bsRet = L"";
	 bsRet += pParams->UnderlyingSymbol;
	 bsRet += arrMonth[dt.get_month()-1];

	 CString str;
	 str.Format(_T("%d "),dt.get_year());
	 bsRet +=  (LPCTSTR)str;
	 if(pResults->OptionType == enCall)
		 bsRet +=  _T("Call");
	 else
		 bsRet +=  _T("Put");
	
	 str.Format(_T(" %.3f"),pResults->StrikePrice);
	 bsRet += (LPCTSTR)str;

	 return bsRet;
 }
