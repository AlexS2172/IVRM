// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "AboutDlg.h"
#include "ModulVer.h"
#include "DBLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




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

		//ver.GetValue(_T("LegalCopyright"), m_strCopyRight);
		m_strCopyRight.LoadString(theApp.GetAppearance(),1003L);

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
	DDX_Control(pDX, IDC_COPYRIGHT, m_staticCotyright);
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
	CString strUser = theApp.GetUser();
	CString strCompany = theApp.GetCompany();

#ifndef _VOLA_MANAGER
	strFmt.Format (_T("User:\t%s"),! strUser.IsEmpty() ? (LPCTSTR) strUser : _T("N/A"));
	AddInfoStr (strFmt);

	strFmt.Format (_T("Company:\t%s"),! strCompany.IsEmpty() ? (LPCTSTR) strCompany : _T("N/A"));
	AddInfoStr (strFmt);
	
	strFmt.Format (_T("Product ID:\t%s"), (LPCTSTR) theApp.GetProductID() );
	AddInfoStr (strFmt);

	strFmt.Format (_T("Product Key:\t%s"), (LPCTSTR) theApp.GetProductKey() );
	AddInfoStr (strFmt);
	
	COleDateTime dtExp = theApp.GetKeyExpiration();
	strFmt.Format (_T("Key Expiration:\t%s"), (LPCTSTR) dtExp.Format(VAR_DATEVALUEONLY) );
	AddInfoStr (strFmt);

	AddInfoStr (_T(" "));
#endif//_VOLA_MANAGER

	AddInfoStr (_T("Components:\tVersion:"));
	FillDBVersion ();


#ifdef _DEBUG
	FillDllVersion (_T("EGOcxD.dll"));
	FillDllVersion (_T("OptionCalcD.dll"));
#else
	FillDllVersion (_T("EGOcx.dll"));
	FillDllVersion (_T("OptionCalc.dll"));
#endif

#ifndef _MULTIPROVIDERS
	FillObjectVersion (__uuidof (MT::MyTrackPriceProvider));
#else
	PP::IProvidersPtr  spProviders;
	spProviders.CreateInstance(__uuidof(PP::Providers));
	if(SUCCEEDED(spProviders->raw_Initialize()))
	{
		long lCount;
		lCount = spProviders->GetCount();
		for(long l=1; l<=lCount;l++)
		{
			PP::IProviderDataPtr spData;
			if(SUCCEEDED(spProviders->get_Item(l, &spData)))
			{
				_bstr_t bsProg = spData->GetProgId(PP::enPriceProvider);
				_bstr_t mL = spData->GetDescription();
				FillObjectVersion((LPCTSTR)bsProg);
			}
		}
	}

#endif

#ifdef _VOLA_MANAGER
	FillObjectVersion (__uuidof (DBLayout));
	FillObjectVersion (__uuidof (VME::VolatilityManagement));
#endif //_VOLA_MANAGER

	FillObjectVersion (__uuidof (IC::MD5));
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

void CAboutDlg::FillDllVersion(LPCTSTR szDll)
{
	CModuleVersion ver;
	ver.GetModuleVersionInfo (szDll);
	
	CString strFmt, strVer = _T("N/A");
	ver.GetValue(_T("FileVersion"), strVer);
	strFmt.Format (_T("%s\t%s"), (LPCTSTR) szDll, (LPCTSTR) strVer);
	
	AddInfoStr (strFmt);
}

void CAboutDlg::FillDBVersion()
{
	CString strVer = _T("N/A");
	try
	{
		CDBLayout db;
		db.Connect();
		
		strVer = db.GetVersion ();

		db.Disconnect();
	}
	catch (...) {}

	CString strFmt;
	strFmt.Format (_T("Database\t%s"), (LPCTSTR) strVer);

	AddInfoStr (strFmt);
}

void CAboutDlg::FillObjectVersion(LPCTSTR szProgID)
{
	CLSID cls;
	USES_CONVERSION;
	CLSIDFromProgID(T2W(szProgID), &cls);
	FillObjectVersion(cls);
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
		CEgRegKey keyModule;
		long lResult = keyModule.Open( key,  _T("LocalServer32"), KEY_READ );
		if (lResult != ERROR_SUCCESS)
			lResult = keyModule.Open( key,  _T("InprocServer32"), KEY_READ );

		if (lResult == ERROR_SUCCESS)
		{
			CString strVal;
			//LPTSTR szVal = strVal.GetBuffer(MAX_PATH);
			//DWORD dwSize = MAX_PATH;
			keyModule.QueryValue (strVal, _T(""));
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
