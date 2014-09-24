// VersionCheckerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VersionChecker.h"
#include "VersionCheckerDlg.h"
#include "ModulVer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEF_DELIMITER _T("------------------------------------------------------------------------------------")
#define DEF_EMPTY _T("")

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVersionCheckerDlg dialog



CVersionCheckerDlg::CVersionCheckerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVersionCheckerDlg::IDD, pParent)
	, n_strValue(_T(""))
	, m_strInstallPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVersionCheckerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VERSIONS, m_edLog);
	DDX_Text(pDX, IDC_EDIT_VERSIONS, n_strValue);
}

BEGIN_MESSAGE_MAP(CVersionCheckerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnBnClickedButtonCopy)
END_MESSAGE_MAP()


// CVersionCheckerDlg message handlers

BOOL CVersionCheckerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 14;                      // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "courier");        // request a face name ""
	m_edFont.CreateFontIndirect(&lf);  // create the font
	m_edLog.SetFont(&m_edFont);
	
	INT nTabs[2] = { 96, 216 };
	m_edLog.SetTabStops(2, nTabs);

	FillInfo();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVersionCheckerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVersionCheckerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVersionCheckerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// // Add string to log
void CVersionCheckerDlg::AddLog(const LPCTSTR lpStatus, const CString& strLog)
{
	int nLen = m_edLog.GetWindowTextLength();
	if (nLen > 20000)
		return;

	CString strText;
	CString strStatus(lpStatus);
	if (nLen > 0)
		strText = _T("\r\n");

	if(strStatus.GetLength())
	{
		CString str;
		str.Format(_T("%-30s%s"), (LPCTSTR)strStatus,(LPCTSTR)strLog);
		strText += str;
	}
	else
		strText += strLog;

	m_edLog.SetSel ( nLen, -1);
	m_edLog.ReplaceSel (strText);

}

void CVersionCheckerDlg::FillInfo(void)
{
	CRegKey key;
	m_strInstallPath = DEF_EMPTY;
	ULONG lSize = 0;
	if(ERROR_SUCCESS!=key.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Egar\\ETS", KEY_READ))
	{
		AddLog("Warning", "EMM.FW applications is not installed properly");
	}
	else
	{
		CString strPath;
		if(ERROR_SUCCESS==key.QueryStringValue("ProductVersion",NULL,&lSize) && lSize!=0)
		{
			key.QueryStringValue("ProductVersion",strPath.GetBufferSetLength(lSize+1),&lSize);
			strPath.ReleaseBuffer();
			AddLog("Product Version", strPath);
		}
		else
			AddLog("Product Version", "N/A");

		lSize = 0;
		if(ERROR_SUCCESS==key.QueryStringValue("Path",NULL,&lSize) && lSize!=0)
		{
			key.QueryStringValue("Path",strPath.GetBufferSetLength(lSize+1),&lSize);
			strPath.ReleaseBuffer();
			AddLog("Installation Path", strPath);
			m_strInstallPath = strPath;
		}
		else
			AddLog("Installation Path", "N/A");
	}
	AddLog(DEF_EMPTY,DEF_DELIMITER);
	AddLog(DEF_EMPTY,DEF_EMPTY);

	lSize = 0;
	if(ERROR_SUCCESS!=key.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Egar\\Common\\Transport",KEY_READ))
			AddLog("RT Host:", "N/A");
	else
	{
		CString strHost;
		if(ERROR_SUCCESS==key.QueryStringValue("RouterHost",NULL,&lSize) && lSize!=0)
		{
			key.QueryStringValue("RouterHost",strHost.GetBufferSetLength(lSize+1),&lSize);
			strHost.ReleaseBuffer();
			AddLog("RT Host:", strHost);
		}
		else
			AddLog("RT Host:", "N/A");

	}
	//AddLog(DEF_EMPTY,DEF_EMPTY);

	//AddLog("Floor", FillFileVersion("EtsFL.exe"));
	//
	//AddLog("Upstairs", FillFileVersion("EtsMM.exe"));

	//AddLog("Back Office", FillFileVersion("EtsBO.exe"));

	//AddLog("Hedger", FillFileVersion("EgarHedgerSQL.exe"));

	//AddLog("Volatility Manager", FillDllVersion("VolaManager.exe"));

	//AddLog(DEF_EMPTY, DEF_EMPTY);

	//AddLog("MQ RDS", FillFileVersion("MQRDS.exe"));
	//AddLog("HandHeld Init", FillFileVersion("HHinit.exe"));
	//
	//AddLog("OptionCalc", FillDllVersion("OptionCalc.dll"));
	//AddLog("edaTools", FillDllVersion("edaTools.dll"));
	//AddLog("Standard Lib", FillDllVersion("EgStdLib.dll"));
	//AddLog("EGOcx", FillDllVersion("egocx.dll"));
	//AddLog("BidAsk", FillObjectVersion(L"{3C8E82E1-98A7-11D2-9673-000001202745}"));
	//AddLog("Sage Connector", FillDllVersion("Sage.exe"));
	//AddLog("Trades Uploader", FillFileVersion("TradesUploader.exe"));
	//AddLog("FOC Connector", FillDllVersion("FOCConnector.exe"));
	//AddLog("O'Connor Connector", FillDllVersion("OConnorConnector.exe"));

	//AddLog(DEF_EMPTY, DEF_EMPTY);
	//AddLog("ETS General", FillObjectVersion(L"{B1B9E070-7E0D-4311-9D15-4BA8640F57F8}"));
	//AddLog("ETS HH Quotes", FillObjectVersion(L"{A08488D3-2DE4-47E6-B142-E629E9C170A2}"));
	//AddLog("ETS MM Quotes", FillObjectVersion(L"{5E68B484-3559-4E9E-A71B-C51443B392C7}"));
	//AddLog("ETS MM Vola", FillObjectVersion(L"{2B24A363-617D-4C50-B217-340317D57B9E}"));
	//AddLog("ETS MM Greeks", FillObjectVersion(L"{F397035A-207B-47CB-9656-9B2A4C3B0A12}"));
	//AddLog(DEF_EMPTY, DEF_EMPTY);

	////  Components
	//AddLog("Transport Service", FillObjectVersion(L"{8FD95AD7-C46C-4936-8FCF-0C50B82A812D}"));
	//AddLog("Router Service", FillObjectVersion(L"{3372D3D9-953E-4A68-B2DC-34DD4AC2C9D4}"));
	//AddLog("Transport Control", FillFileVersion("TpControl.exe"));
	//AddLog("Router Control", FillFileVersion("RtControl.exe"));
	//
	//AddLog("ATFData Provider",   FillObjectVersion(L"{812DDABA-1E36-47F2-B22D-C505802F67EF}"));
	//AddLog("ProTrack Provider",  FillObjectVersion(L"{FD68F05A-247C-4D19-B12D-852691429151}"));
	//AddLog("HyperFeed Provider", FillObjectVersion(L"{F9B96C5F-290E-4B9C-9F7D-A33F4F63D77B}"));
	//AddLog("ISE Provider",       FillObjectVersion(L"{13EC7F7D-37DA-4CF9-B979-200DB9C1401E}"));
	//AddLog("Price Providers", FillObjectVersion(L"{857DBBD6-F677-4F33-9D45-1081076D307F}"));
	//AddLog(DEF_EMPTY, DEF_EMPTY);

	//AddLog("VolatilitySources", FillObjectVersion(L"{A53B3209-BDB9-4DF2-B956-777A38C1606D}"));
	//AddLog("VME", FillObjectVersion(L"{4A625D7C-209F-4E26-A651-24474EA66C3B}"));
	//AddLog("VADBLayout", FillObjectVersion(L"{FBD72117-1F27-4FC1-98BF-ACD948A73AF6}"));
	//AddLog("MsgStruct", FillObjectVersion(L"{21BCCB2E-87CB-493D-AFD7-914A88B6850D}"));
	//AddLog("MMW", FillObjectVersion(L"{D9614A17-5E4D-47D9-8EDB-8A6209CF7A1A}"));
	//AddLog("axButton", FillObjectVersion(L"{9EEA74CB-D2D8-11D2-8AAE-0008C7D2400A}"));

	//AddLog("ElladaFlatControls", FillObjectVersion(L"{C3957760-A0DD-448F-9DE4-0CDB873A029C}"));
	//AddLog("FocusFlatControls", FillObjectVersion(L"{08987E33-2DBE-42E1-BE6A-45845820BACC}"));
	//AddLog("olch2x32", FillObjectVersion(L"{12FEEAB3-5B26-11CF-A683-C56BA13DBB18}"));

	//AddLog(DEF_EMPTY, DEF_EMPTY);
	//AddLog("VSFlex7", FillObjectVersion(L"{D76D712E-4A96-11D3-BD95-D296DC2DD072}"));
	//AddLog("VSOcx6", FillObjectVersion(L"{3B008051-905A-11D1-B4AE-444553540000}"));

	//AddLog(DEF_EMPTY, DEF_EMPTY);
	//AddLog("WorkFlowIC", FillObjectVersion(L"{ED6C5EAF-781E-11D2-A932-0060977EAB89}"));	 
	//AddLog("Version Checker", FillFileVersion("VersionChecker.exe"));
	//AddLog(DEF_EMPTY, DEF_EMPTY);
	//AddLog(DEF_EMPTY,DEF_DELIMITER);
	//LogDatabase();

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("ETS Application"));
	AddLog("ETS Back Office", FillFileVersion("EtsBO.exe"));
	AddLog("ETS Market Maker", FillFileVersion("EtsMM.exe"));
	AddLog("ETS General", FillObjectVersion(L"{B1B9E070-7E0D-4311-9D15-4BA8640F57F8}"));
	AddLog("ETS MM General", FillObjectVersion(L"{2476A3F1-BF81-4398-8EFC-8D617109C2B4}"));
	AddLog("ETS MM Reports", FillObjectVersion(L"{506B7B8C-BB01-471C-B1DE-6B74BF92FBF9}"));
	AddLog("ETS MM Greeks", FillObjectVersion(L"{F397035A-207B-47CB-9656-9B2A4C3B0A12}"));
	AddLog("ETS MM Quotes", FillObjectVersion(L"{5E68B484-3559-4E9E-A71B-C51443B392C7}"));
	AddLog("ETS MM Vola Analysis", FillObjectVersion(L"{2B24A363-617D-4C50-B217-340317D57B9E}"));
	AddLog("ETS MM Hedger Summary", FillObjectVersion(L"{D106A8EB-08F3-4223-9FFA-1467BE851A75}"));
	AddLog("ETS MM Stock Hedge", FillObjectVersion(L"{96102A2A-FEE8-4D5D-819E-560F51F9224F}"));
	AddLog("ETS MM Index Hedge", FillObjectVersion(L"{91987177-4C7B-41CB-BD77-2CA16260F57A}"));
	AddLog("ETS MM Risks", FillObjectVersion(L"{E80B67A5-CC66-4346-80D7-BC13229D70E1}"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	AddLog("EOD Server", FillObjectVersion(L"{A98F4C1B-358B-414C-9B9A-E44B1512F6C7}"));
	AddLog("EOD Manager", FillFileVersion("EtsEodManager.exe"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	AddLog("TNT Connector", FillObjectVersion(L"{4FCA0EDC-37FE-4D17-B8C5-46E1E2E58B9B}"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	AddLog("FIX Gateway", FillObjectVersion(L"{3B8EEAB2-D463-4BA3-B058-D570C74D0EBC}"));
	AddLog("FIX Manager", FillFileVersion("EtsFixManager.exe"));
	AddLog("FIX Interface", FillObjectVersion(L"{C4E7B0B1-A263-4394-A024-8626AA0E20EA}"));
	AddLog("FIX Objects", FillObjectVersion(L"{FC10275D-DD31-4BCD-BA9F-77CE49D7DB87}"));
	AddLog("FIX Providers", FillObjectVersion(L"{B5CC7811-7824-4556-B7A1-BBAD300AE529}"));
	AddLog("FIX O'Connor Provider", FillObjectVersion(L"{63E8FDC7-FC0A-4339-93FB-6AA9BF4FBE01}"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Price Providers"));
	AddLog("ATF Provider",   FillObjectVersion(L"{812DDABA-1E36-47F2-B22D-C505802F67EF}"));
	AddLog("HyperFeed Provider", FillObjectVersion(L"{F9B96C5F-290E-4B9C-9F7D-A33F4F63D77B}"));
	AddLog("ProTrack Provider",  FillObjectVersion(L"{FD68F05A-247C-4D19-B12D-852691429151}"));
	AddLog("Price Providers", FillObjectVersion(L"{857DBBD6-F677-4F33-9D45-1081076D307F}"));
	AddLog("ISE Config", FillFileVersion("ISEConfig.exe"));
	AddLog("ISE Logs", FillFileVersion("ISELogs.exe"));
	AddLog("ISE Provider", FillObjectVersion(L"{13EC7F7D-37DA-4CF9-B979-200DB9C1401E}"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Transport"));
	AddLog("Transport/Router Service", FillObjectVersion(L"{8FD95AD7-C46C-4936-8FCF-0C50B82A812D}"));
	AddLog("Transport Proxy", FillObjectVersion(L"{5381E86C-E13E-4BB1-ADCA-389DD7E4C6F7}"));
	AddLog("Transport Control", FillFileVersion("TpControl.exe"));
	AddLog("Router Control", FillFileVersion("RtControl.exe"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Messaging"));
	//AddLog("FIX Struct", FillObjectVersion(L"{648B55C1-BC4C-4B7C-B1A2-011395D3101A}"));
	AddLog("MMW", FillObjectVersion(L"{D9614A17-5E4D-47D9-8EDB-8A6209CF7A1A}"));
	AddLog("MsgStruct", FillObjectVersion(L"{5B7B0165-1126-4C59-B85C-C0C6B4E8642F}"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("RTTF"));
	AddLog("FOC Connector", FillDllVersion("FOCConnector.exe"));
	AddLog("Sage Connector", FillDllVersion("Sage.exe"));
	AddLog("Trades Uploader", FillFileVersion("TradesUploader.exe"));
	AddLog("O'Connor Connector", FillDllVersion("OConnorConnector.exe"));
	AddLog("CIBC Trade Feed Adapter", FillDllVersion("CIBCTradeFeedAdapter.exe"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Volatility Source"));
	AddLog("VADBLayout", FillObjectVersion(L"{FBD72117-1F27-4FC1-98BF-ACD948A73AF6}"));
	AddLog("VME", FillObjectVersion(L"{4A625D7C-209F-4E26-A651-24474EA66C3B}"));
	AddLog("VolatilitySources", FillObjectVersion(L"{A53B3209-BDB9-4DF2-B956-777A38C1606D}"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Common ETS Modules"));
	AddLog("EDA Tools", FillDllVersion("edaTools.dll"));
	AddLog("Egar Standard Lib", FillDllVersion("EgStdLib.dll"));
	AddLog("ElladaFlatControls", FillObjectVersion(L"{C3957760-A0DD-448F-9DE4-0CDB873A029C}"));
	AddLog("ETSManager", FillObjectVersion(L"{C213BE7D-AFAE-4F1C-8E9D-A7D248FA987B}"));
	AddLog("OptionCalc", FillDllVersion("OptionCalc.dll"));
	AddLog("Version Checker", FillFileVersion("VersionChecker.exe"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("IVolatility Access"));
	AddLog("IV Feeder", FillObjectVersion(L"{76AF32F9-5AF3-47D7-93E4-392249FB6320}"));
	AddLog("WorkFlowIC", FillObjectVersion(L"{ED6C5EAF-781E-11D2-A932-0060977EAB89}"));	 

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Delta Hedger"));
	AddLog("axButton", FillObjectVersion(L"{9EEA74CB-D2D8-11D2-8AAE-0008C7D2400A}"));
	AddLog("BidAskDH", FillObjectVersion(L"{3C8E82E1-98A7-11D2-9673-000001202745}"));
	AddLog("DHlButtons", FillObjectVersion(L"{78F6CA6D-4FE9-4BEE-A8FB-71F8DD5070BC}"));
	AddLog("DHMTFeeder", FillObjectVersion(L"{C1FC2FFD-A24B-4942-AF73-0B05FA48F497}"));
	AddLog("Egar Hedger", FillFileVersion("EgarHedgerSQL.exe"));
	AddLog("FocusFlatControls", FillObjectVersion(L"{08987E33-2DBE-42E1-BE6A-45845820BACC}"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Volatility Manager"));
	AddLog("EGOcx", FillDllVersion("egocx.dll"));
	AddLog("IVMCtrl", FillObjectVersion(L"{D778F36E-8D7D-4909-9623-ABE567BBBB55}"));
	AddLog("Volatility Manager", FillDllVersion("VolaManager.exe"));

	//AddLog(DEF_EMPTY,DEF_EMPTY);
	//AddLog(DEF_EMPTY, _T("Floor Support"));
	//AddLog("HandHeld Init", FillFileVersion("HHInit.exe"));
	//AddLog("MQRDS", FillFileVersion("MQRDS.exe"));

	AddLog(DEF_EMPTY,DEF_EMPTY);
	LogDatabase();
}

CString CVersionCheckerDlg::FillObjectVersion(LPOLESTR szCLSID )
{
	USES_CONVERSION;

	CString strVersion;
	CString strKey = _T("CLSID\\");
	strKey += W2T(szCLSID);

	strVersion = _T("N/A");	

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
			keyModule.QueryValue (szVal, DEF_EMPTY, &dwSize);
			strVal.ReleaseBuffer();

			if (!strVal.IsEmpty() )
			{
				CModuleVersion ver;
				CString strVer = _T("N/A");

				if (ver.GetFileVersionInfo (strVal))
				{
					if(ver.GetValue(_T("FileVersion"), strVer))
						_FixVersionString(strVer);
				}
				
				WIN32_FIND_DATA data;
				HANDLE hFind = FindFirstFile (strVal, &data);
				if( hFind != INVALID_HANDLE_VALUE )
				{
					FindClose(hFind);

					CString strFmt;
					strFmt.Format (_T("%-30s%s"), data.cFileName, (LPCTSTR) strVer);
					
					strVersion = strFmt;
				}					
			}
		}

		key.Close();
	}
	return strVersion;
}

CString CVersionCheckerDlg::FillFileVersion(LPCTSTR szFile)
{
	CModuleVersion ver;
	ver.GetFileVersionInfo(szFile);
	
	CString strFmt, strVer = _T("N/A");
	if(ver.GetValue(_T("FileVersion"), strVer))
		_FixVersionString(strVer);

	strFmt.Format (_T("%-30s%s"), szFile, (LPCTSTR) strVer);
	return strFmt;

}

void CVersionCheckerDlg::_FixVersionString(CString& strVer)
{
	strVer.Replace(_T(","), _T("."));
	strVer.Replace(_T(" "), _T(""));

	CString strToken, strResult;
	int nPos = 0;

	strToken = strVer.Tokenize(".", nPos);
	while(strToken.GetLength() > 0)
	{
		while(strToken[0] == _T('0') && strToken.GetLength() > 1)
		{
			strToken.Delete(0, 1);
		}
		strResult += strToken;

		strToken = strVer.Tokenize(".", nPos);

		if(strToken.GetLength() > 0)
			strResult += _T(".");
	}
	strVer = strResult;
}

CString CVersionCheckerDlg::FillDllVersion(LPCTSTR szDll)
{
	CModuleVersion ver;
	ver.GetModuleVersionInfo (szDll);
	
	CString strFmt, strVer = _T("N/A");
	if(ver.GetValue(_T("FileVersion"), strVer))
		_FixVersionString(strVer);

	strFmt.Format (_T("%-30s%s"), (LPCTSTR) szDll, (LPCTSTR) strVer);
	return strFmt;
	
}


void CVersionCheckerDlg::LogDatabase()
{
	CString strServer;
	CString strDatabase;
	CString strLogin;
	CString strPassword;
	CString strConnection;

	try
	{
		STG::ISettingsPtr spSettings;
		HRESULT hr = spSettings.CreateInstance(__uuidof(STG::Settings));
		if(SUCCEEDED(hr))
		{

			strConnection = (LPCTSTR)spSettings->DatabaseConnection;

			//SplitConnectionString(strConnection);
			CDBConnection db((LPCTSTR)strConnection);
			db.Open();

			CStoredProc <CDefaultRecordset> spVer (db, L"usp_DataInfo_Get");	
			spVer << 1;	
			spVer.Open();

			CString strVer = (LPCTSTR)(_bstr_t) spVer[L"vcKeyValue"];
			AddLog("Database Version",  strVer);
		}

	}
	catch (_com_error&) 
	{
			AddLog("Database Version", "N/A");
	}

	SplitConnectionString(strConnection);

	AddLog(DEF_EMPTY, DEF_EMPTY);
}

void CVersionCheckerDlg::SplitConnectionString(const CString& str)
{
	CString strRet;

	int nCurPos = 0, nValPos = 0;
	CString strValue, strServer, strDatabase;

	strValue = str.Tokenize(_T(";"), nCurPos);
	while(nCurPos >= 0)
	{
		if(!strValue.Find(_T("Data Source")))
		{
			if((nValPos = strValue.Find(_T("="))) >= 0)
				strServer = strValue.Mid(nValPos + 1);
		}
		else if(!strValue.Find(_T("Initial Catalog")))
		{
			if((nValPos = strValue.Find(_T("="))) >= 0)
				strDatabase = strValue.Mid(nValPos + 1);
		}

		strValue = str.Tokenize(_T(";"), nCurPos);
	}

	strValue = strServer + "\\" + strDatabase;
	int strLen = strValue.GetLength();

	if(strLen < 40)
	{
		if(strLen)
			AddLog("Database Path", strValue);
		return;
	}

	int iStart = 0;
	int iCopy = 40;

	while(1)
	{
		if(strLen > iCopy)
		{
			strRet = strValue.Mid(iStart, 40);
			if(iStart > 0)
				AddLog(" ", strRet);
			else
				AddLog("Database Path", strRet);
		}
		else
		{
			strRet = strValue.Mid(iStart);
			AddLog(" ", strRet);
			break;
		}
		iStart += 40;
		iCopy  += 40;
	}
}
void CVersionCheckerDlg::OnBnClickedButtonCopy()
{
	// Get a pointer to the structure for the selected label.  
	int nStartChar, nEndChar;
	m_edLog.GetSel(nStartChar, nEndChar);
	CString strCopy;
	if(nStartChar==nEndChar)
	{
		m_edLog.SetSel(0,-1, TRUE);
		m_edLog.Copy();
		m_edLog.SetSel(-1,-1,TRUE);
		return;
	}
	m_edLog.Copy();
    return; 
}
