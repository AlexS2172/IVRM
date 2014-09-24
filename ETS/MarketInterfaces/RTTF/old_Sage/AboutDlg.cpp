// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SageApp.h"
#include "resource.h"
#include "AboutDlg.h"
#include "ModulVer.h"
#include "Publisher.h"
#include "Sage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSageApp    theApp;
extern _bstr_t     m_bsConnectionString;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strCaption = _T("");
	m_strCopyRight = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	if (!pDX->m_bSaveAndValidate)	
	{
		CModuleVersion ver;
		ver.GetModuleVersionInfo (AfxGetInstanceHandle());
		
		CString strProduct, strVer;
		
		ver.GetValue(_T("ProductName"), strProduct);
		ver.GetValue(_T("ProductVersion"), strVer);
		
		LPTSTR szVer = strVer.GetBuffer(1);
		for (int i= 0, nCnt=0 ; szVer[i]; i++ )
		{
			if (szVer[i] == _T(',') && ++nCnt == 3 )
				szVer[i] = _T('\0');
		}
		strVer.ReleaseBuffer();

		m_strCaption.Format (_T("%s \t Version %s"), (LPCTSTR) strProduct, (LPCTSTR) strVer ); 

		ver.GetValue(_T("LegalCopyright"), m_strCopyRight);

		CString strWndText;
		GetWindowText( strWndText );
		strWndText += strProduct;
		SetWindowText( strWndText );
	}

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_PRODUCT_INFO, m_edProductInfo);
	DDX_Text(pDX, IDC_CAPTION, m_strCaption);
	DDX_Text(pDX, IDC_COPYRIGHT, m_strCopyRight);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)	
	{
		FillProductInfo();
	}
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutDlg::FillProductInfo()
{
	CWaitCursor wt;

	m_edProductInfo.SetTabStops (80);

	CString strFmt;

	AddInfoStr (_T("Components:\tVersion:\r\n"));
	FillDBVersion ();
}

void CAboutDlg::AddInfoStr(LPCTSTR szStr)
{
	int nLen = m_edProductInfo.GetWindowTextLength();

	CString strText;
	if (nLen > 0)
		strText = _T("\r\n");

	strText += szStr;

	m_edProductInfo.SetSel ( nLen, -1 );
	
	m_edProductInfo.ReplaceSel (strText);
}

void CAboutDlg::FillDBVersion()
{
	CString strVer = _T("N/A");
	try
	{
        _bstr_t bsConnectionString;

        if (CSage::m_hStopApplication)
        {
            bsConnectionString = CPublisher::m_bsConnectionString;
        }
        else
        {
            ISettingsPtr spSettings;
	    	HRESULT hr = spSettings.CreateInstance(__uuidof (Settings));
		    if(FAILED(hr))
			    utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");
            bsConnectionString = spSettings->DatabaseConnection;
        }

        CDBConnection db;
        db.Open(bsConnectionString, 120, 120);

    	CStoredProc<CDefaultRecordset> spVer(db, L"usp_DataInfo_Get");	
    	spVer << 1;	
	    spVer.Open();
	    strVer = (LPCTSTR) (_bstr_t) spVer[L"vcKeyValue"];
        spVer.Close();
        db.Close();
	}
    catch (...) { }

	CString strFmt;
	strFmt.Format (_T("Database\t%s"), (LPCTSTR) strVer);

	AddInfoStr (strFmt);
}

void CAboutDlg::FillObjectVersion(CLSID clsid)
{
	LPOLESTR szCLSID=NULL;
	StringFromCLSID (clsid, &szCLSID);
	
	CString strKey = _T("CLSID\\");
	strKey += CString  (szCLSID);
	CoTaskMemFree (szCLSID);

	CRegKey key;	
	long lResult =  key.Open(HKEY_CLASSES_ROOT, strKey , KEY_READ );
	if (lResult == ERROR_SUCCESS)
	{
		CRegKey keyModule;
		long lResult = keyModule.Open( key,  _T("LocalServer32"), KEY_READ );
		if (lResult != ERROR_SUCCESS)
			lResult = keyModule.Open( key,  _T("InprocServer32"), KEY_READ );

		if (lResult == ERROR_SUCCESS)
		{
			CString strVal;
			LPTSTR szVal = strVal.GetBuffer(MAX_PATH);
			DWORD dwSize = MAX_PATH;
			keyModule.QueryValue (szVal, _T(""), &dwSize);
			strVal.ReleaseBuffer();

			if (!strVal.IsEmpty() )
			{
				CModuleVersion ver;
				CString strVer = _T("N/A");

				if (ver.GetFileVersionInfo (strVal))
					ver.GetValue(_T("FileVersion"), strVer);
				
				WIN32_FIND_DATA data;
				HANDLE hFind = FindFirstFile (strVal, &data);
				if( hFind != INVALID_HANDLE_VALUE )
				{
					FindClose(hFind);

					CString strFmt;
					strFmt.Format (_T("%s\t%s"), data.cFileName, (LPCTSTR) strVer);
					
					AddInfoStr (strFmt);
				}					
			}
		}

		key.Close();
    }
    else
    {
        TRACE1( "Failed, hr = 0x%.8X.\n", HRESULT_FROM_WIN32(lResult));
    }
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( pWnd->GetDlgCtrlID() == IDC_PRODUCT_INFO )
	{
		pDC->SetBkColor (GetSysColor(COLOR_WINDOW));
		hbr = GetSysColorBrush (COLOR_WINDOW);
	}
	
	return hbr;
}
