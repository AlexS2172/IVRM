// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CIBCTradeFeedAdapter.h"
#include "SettingsDlg.h"
#include "Settings.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog


CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	m_bSettingChanged = FALSE;
	//{{AFX_DATA_INIT(CSettingsDlg)
	m_strDBFrom = _T("");
	m_strDBTo = _T("");
	m_nTradeCount = 0;
	m_nWaitingTime = 0;
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Text(pDX, IDC_EDIT_DBFROM, m_strDBFrom);
	DDX_Text(pDX, IDC_EDIT_DBTO, m_strDBTo);
	DDX_Text(pDX, IDC_TRADE_COUNT_EDIT, m_nTradeCount);
	DDV_MinMaxUInt(pDX, m_nTradeCount, MIN_TRADES_COUNT, MAX_TRADES_COUNT);
	DDX_Text(pDX, IDC_WAITING_TIME_EDIT, m_nWaitingTime);
	DDV_MinMaxUInt(pDX, m_nWaitingTime, MIN_WAITING_TIME, MAX_WAITING_TIME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_FROM_SETUP, OnBtnFromSetup)
	ON_BN_CLICKED(IDC_BUTTON_TO_SETUP, OnBtnToSetup)
	ON_EN_CHANGE(IDC_EDIT_DBFROM, OnChangeEdits)
	ON_EN_CHANGE(IDC_EDIT_DBTO, OnChangeEdits)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

void CSettingsDlg::OnBtnFromSetup() 
{
	BtnSetup(m_strDBFrom, FALSE);
}

void CSettingsDlg::OnBtnToSetup() 
{
	BtnSetup(m_strDBTo, FALSE);
}

void CSettingsDlg::BtnSetup(CString& strDB, BOOL bUseSP)
{
    m_bSettingChanged = true;

    CDBConnection db;
    try 
    {
        IDataSourceLocatorPtr spLocator;

        HRESULT hr = spLocator.CreateInstance (__uuidof (DataLinks));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create DataLinks object.");
      
        spLocator->hWnd = (long)m_hWnd;

        _ConnectionPtr  spConnection;
    	hr = spConnection.CreateInstance (__uuidof (Connection));
        if (FAILED(hr))
            utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Connection object.");
    
        spConnection->ConnectionString = (LPCTSTR)strDB;

        CComPtr<IDispatch> spDispatch = spConnection;
        if (spDispatch == 0)
            utils::ThrowErrorNoSetErrorInfo(E_NOINTERFACE, L"Failed to get IDispatch interface.");

        if (spLocator->PromptEdit(&spDispatch.p) == VARIANT_TRUE)
        {
			_ConnectionPtr spNewConnection;
			spDispatch->QueryInterface(__uuidof(_Connection), (void**)&spNewConnection);

            if (spNewConnection == 0)
                utils::ThrowErrorNoSetErrorInfo(E_NOINTERFACE, L"Failed to get IDispatch interface.");

            CString strConnectionString = (LPCTSTR)spNewConnection->ConnectionString;
            
            try
            {
                db.Open((LPCTSTR)strConnectionString, 120, 120);
				if(bUseSP)
				{
					CStoredProc<CDefaultRecordset> spVer(db, L"usp_DataInfo_Get");	
					spVer << 1;
					spVer.Open();
					spVer[L"vcKeyValue"];
					spVer.Close();
				}
                db.Close();

                if (strDB != strConnectionString)
                {
                    strDB = strConnectionString;
                    UpdateData(FALSE);
                }
            }
            catch (_com_error& e)
            {
                try
                {
                    if (db.IsOpened())
                        db.Close();
                } catch (_com_error&) { }

                CString err = _T("Invalid database. ");
                err += (LPTSTR) e.Description();
                AfxMessageBox(err);
            }
        }
    }
    catch (_com_error& e)
    {
        CString err = _T("Can't show database connection dialog.");
        err += (LPTSTR) e.Description();
        AfxMessageBox(err);
    }	
}

void CSettingsDlg::OnChangeEdits() 
{
	m_bSettingChanged = true;
}

BOOL CSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
    try
    {
        if ( !CSettings::IsLoaded() )
            CSettings::LoadSettings();    
    }
	catch(_com_error& e)
    {
		CTracer::Trace(CTracer::enMtWarning, e.Description());
    }
    catch(...)
    {
        return TraceError(E_UNEXPECTED, _T("Unexpected error"));
    }

    m_strDBFrom = (LPCSTR) CSettings::m_bsExchange_DB_Path;
    m_strDBTo = (LPCSTR) CSettings::m_bsETS_DB_Path;
	m_nTradeCount = CSettings::m_nTradesCount;
    m_nWaitingTime = CSettings::m_nWaitingTime;

	m_bSettingChanged = true;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

bool CSettingsDlg::CheckDBPaths()
{
    CDBConnection dbConnection;

	try
	{
        dbConnection.Open((_bstr_t)m_strDBFrom);
        dbConnection.Close();
	}
	catch(_com_error&) 
	{
		dbConnection.Close();
        AfxMessageBox("Failed exchange db path");
		return false;
	}

	try
	{
        dbConnection.Open((_bstr_t)m_strDBTo);
        dbConnection.Close();
	}
	catch(_com_error&)
	{
		dbConnection.Close();
        AfxMessageBox("Failed ETS db path");
		return false;
	}

    return true;
}

void CSettingsDlg::OnOK() 
{
	if (FALSE == UpdateData(TRUE)) return;
    if (false == CheckDBPaths()) return;

	if (SaveSettings())
	{
		CMainFrame* pFrmMain = static_cast<CMainFrame*>(AfxGetMainWnd());
	}
        
    CDialog::OnOK();
}

bool CSettingsDlg::SaveSettings()
{
	bool bRet = false;

    if (m_bSettingChanged)
    {
        CSettings::m_bsExchange_DB_Path = m_strDBFrom;
        CSettings::m_bsETS_DB_Path = m_strDBTo;

        CSettings::m_nTradesCount = m_nTradeCount;
        CSettings::m_nWaitingTime = m_nWaitingTime;

        try
        {
            CSettings::SaveSettings();
        }
        catch(_com_error& e)
        {
            CTracer::Trace(CTracer::enMtWarning, e.Description());
        }
        catch(...)
        {
            TraceError(E_UNEXPECTED, _T("Unexpected error occurred due to settings' saving "));
            return false;
        }
        
        m_bSettingChanged = false;
        bRet = true;
    }

	return bRet;
}
