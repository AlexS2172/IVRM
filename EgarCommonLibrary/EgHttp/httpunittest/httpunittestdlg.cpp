// HttpUnitTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HttpUnitTest.h"
#include "HttpUnitTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

/////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

/////////////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHttpUnitTestDlg dialog
CHttpUnitTestDlg::CHttpUnitTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHttpUnitTestDlg::IDD, pParent)
	, m_strProxyURL(_T(""))
	, m_iProxyPort(0)
	, m_strProxyLogin(_T(""))
	, m_strProxyPassword(_T(""))
	, m_strServerAddress(_T(""))
	, m_iServerPort(0)
	, m_strServerPath(_T(""))
	, m_strServerLogin(_T(""))
	, m_strServerPassword(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strServerPath = _T("default.htm");
	m_cpParams.m_strServerAddress = m_strServerAddress = _T("http://www.IVolatility.com");//_T("http://www.microsoft.com");
	m_cpParams.m_lServerPort = m_iServerPort = 80;
	//m_cpParams.m_strServerLogin = 
	m_strServerLogin = _T("test");
	//m_cpParams.m_strServerPassword = 
	m_strServerPassword = _T("");

	m_cpParams.m_enProxyAuth = enEgPaDisabled; // enEgPaEnabled
	m_cpParams.m_enProxySettings = enEgPsDefault; // enEgPsCustom; // enEgPsDisabled;
	
	m_cpParams.m_strProxyAddress = m_strProxyURL = _T("proxy");
	m_cpParams.m_lProxyPort = m_iProxyPort = 3128;
	m_cpParams.m_ProxyAcount.m_strLogin = m_strProxyLogin = _T("EGAR\\tuaevr");
	m_cpParams.m_ProxyAcount.m_strPassword = m_strProxyPassword = _T("");
}

/////////////////////////////////////////////////////////////////////////////
void CHttpUnitTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROXY_URL, m_strProxyURL);
	DDX_Text(pDX, IDC_EDIT_PROXY_PORT, m_iProxyPort);
	DDX_Text(pDX, IDC_EDIT_PROXY_LOGIN, m_strProxyLogin);
	DDX_Text(pDX, IDC_EDIT_PROXY_PASSWORD, m_strProxyPassword);
	DDX_Control(pDX, IDC_CHECK_PROXY_AUTH, m_chkProxyAuth);
	DDX_Control(pDX, IDC_CHECK_USE_PROXY, m_chkUseProxy);
	DDX_Text(pDX, IDC_EDIT_URL, m_strServerAddress);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_iServerPort);
	if (pDX->m_bSaveAndValidate)
	{
		m_cpParams.m_enProxyAuth = 
			(m_chkProxyAuth.GetCheck() == BST_CHECKED) ? enEgPaEnabled : enEgPaDisabled;

		switch(m_chkUseProxy.GetCheck())
		{
		case BST_UNCHECKED:
			m_cpParams.m_enProxySettings = enEgPsDisabled;
			break;
		case BST_CHECKED:
			m_cpParams.m_enProxySettings = enEgPsCustom;
			break;
		case BST_INDETERMINATE:
			m_cpParams.m_enProxySettings = enEgPsDefault;
			break;
		}
	}
	else
	{
		int iCheck = BST_UNCHECKED;
		switch(m_cpParams.m_enProxySettings)
		{
		case enEgPsCustom:
			iCheck = BST_CHECKED;
			break;
		case enEgPsDefault:
			iCheck = BST_INDETERMINATE;
			break;
		}
		m_chkUseProxy.SetCheck(iCheck);
	}
	DDX_Text(pDX, IDC_EDIT_PATH, m_strServerPath);
	DDX_Text(pDX, IDC_EDIT_SERVER_LOGIN, m_strServerLogin);
	DDX_Text(pDX, IDC_EDIT_SERVER_PASSWORD, m_strServerPassword);
}

BEGIN_MESSAGE_MAP(CHttpUnitTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GO, OnBnClickedButtonGo)
	ON_BN_CLICKED(IDC_CHECK_PROXY_AUTH, OnBnClickedCheckProxyAuth)
	ON_BN_CLICKED(IDC_CHECK_USE_PROXY, OnBnClickedCheckUseProxy)
END_MESSAGE_MAP()


// CHttpUnitTestDlg message handlers

BOOL CHttpUnitTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	OnBnClickedCheckProxyAuth();
	OnBnClickedCheckUseProxy();

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

	::SetFocus(::GetDlgItem(m_hWnd, IDC_EDIT_PATH));

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CHttpUnitTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHttpUnitTestDlg::OnPaint() 
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
HCURSOR CHttpUnitTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
void CHttpUnitTestDlg::OnBnClickedButtonGo()
{
	CString strURL;
	GetDlgItemText(IDC_EDIT_URL, strURL);

	if (!UpdateData(TRUE))
		return;

	SetDlgItemText(IDC_EDIT_HTML_BODY, _T(""));
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HTML_BODY);
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	m_cpParams.m_strServerAddress  = m_strServerAddress;
	m_cpParams.m_lServerPort       = m_iServerPort;
//	m_cpParams.m_strServerLogin    = m_strServerLogin;
//	m_cpParams.m_strServerPassword = m_strServerPassword;

	if (m_cpParams.m_enProxySettings == enEgPsCustom)
	{
		m_cpParams.m_strProxyAddress  = m_strProxyURL;
		m_cpParams.m_lProxyPort       = m_iProxyPort;
		if (m_cpParams.m_enProxyAuth == enEgPaEnabled)
		{
			m_cpParams.m_ProxyAcount.m_strLogin    = m_strProxyLogin;
			m_cpParams.m_ProxyAcount.m_strPassword = m_strProxyPassword;
		}
	}

	try
	{
		
		CString strTotal(_T(">>> 1) test scenario for CEgIvProvider: <<<\r\n\r\n"));
		// 1) test scenario for CEgIvProvider
		try
		{
			CWaitCursor wait;
			CEgIvProvider aIvProvider;
			CEgAccount account(m_strServerLogin, m_strServerPassword);
			aIvProvider.InitIvSession(m_cpParams, account);
			CEgString strSID = aIvProvider.Login();
			strTotal += strSID + _T("\r\n");
			strSID = aIvProvider.Login();
			strTotal += strSID + _T("\r\n");
			strSID = aIvProvider.Login();
			strTotal += strSID + _T("\r\n");
			aIvProvider.CloseSession();
			SetDlgItemText(IDC_EDIT_HTML_BODY, strTotal);
		}
		catch(CEgHttpError aError)
		{
			CEgString strError(aError.Description());
#ifdef _DEBUG
			strError.AppendFormat(_T("\r\nError = 0x%08x (%d)"), aError.Error(), aError.Error());
#endif
			if (strError.IsEmpty())
				strError.Format(_T("Cannot get error message! code = 0x%08x"), aError.Error());

			AfxMessageBox(strError, MB_ICONSTOP | MB_OK);
			strTotal += strError;
			SetDlgItemText(IDC_EDIT_HTML_BODY, strTotal);
		}

		strTotal += _T("\r\n\r\n");

		// 2) test scenario for CEgIvProvider
		try 
		{
			strTotal += _T(">>> 2) test scenario for CEgHttpProvider: <<<\r\n\r\n");
			CWaitCursor wait;
			CEgHttpProvider aHttpProvider;
			aHttpProvider.InitSession(m_cpParams);
			CEgString strPath(m_strServerPath);
			aHttpProvider.OpenURL(strPath);

			CEgIvProvider::CheckErrorMessage(_T("Server to busy"));
			bool bDataPresent = false;
			CEgString strBody, strData;
			do 
			{
				bDataPresent = aHttpProvider.ReadData(strData);
				strBody += strData;
			}
			while (bDataPresent);
			aHttpProvider.CloseURL();
			aHttpProvider.CloseSession();
			strTotal += strBody;
			SetDlgItemText(IDC_EDIT_HTML_BODY, strTotal);
		}
		catch(CEgHttpError aError)
		{
			CEgString strError(aError.Description());
#ifdef _DEBUG
			strError.AppendFormat(_T("\r\nError = 0x%08x (%d)"), aError.Error(), aError.Error());
#endif
			if (strError.IsEmpty())
			{
				strError.Format(_T("Cannot get error message! code = 0x%08x"), aError.Error());
			}


			AfxMessageBox(strError, MB_ICONSTOP | MB_OK);
			strTotal += strError;
			SetDlgItemText(IDC_EDIT_HTML_BODY, strTotal);
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("Unknown exception catched!"), 
			MB_ICONSTOP | MB_OK);

		SetDlgItemText(IDC_EDIT_HTML_BODY, _T("Unknown exception catched!"));
	}
}

/////////////////////////////////////////////////////////////////////////////
void CHttpUnitTestDlg::OnBnClickedCheckProxyAuth()
{
	BOOL bChecked = m_chkProxyAuth.GetCheck() == BST_CHECKED;
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PROXY_LOGIN),    bChecked);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PROXY_PASSWORD), bChecked);
}

/////////////////////////////////////////////////////////////////////////////
void CHttpUnitTestDlg::OnBnClickedCheckUseProxy()
{
	int iUseProxyCheck = m_chkUseProxy.GetCheck();
	BOOL bProxyAuth    = m_chkProxyAuth.GetCheck();

	m_chkProxyAuth.EnableWindow(iUseProxyCheck == BST_CHECKED);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PROXY_URL),      
		iUseProxyCheck == BST_CHECKED);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PROXY_PORT),     
		iUseProxyCheck == BST_CHECKED);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PROXY_LOGIN),    
		iUseProxyCheck == BST_CHECKED && bProxyAuth);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PROXY_PASSWORD), 
		iUseProxyCheck == BST_CHECKED && bProxyAuth);
}

/////////////////////////////////////////////////////////////////////////////
