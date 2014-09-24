// AboutDlg.cpp : implementation of the CAboutDlg class
//
#include "stdafx.h"
#include "EtsEodManager.h"
#include "AboutDlg.h"
#include "ModuleVersionEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR REGKEY_SYSINFO_LOC[] = _T("SOFTWARE\\Microsoft\\Shared Tools Location");
const TCHAR REGVAL_SYSINFO_LOC[]  = _T("MSINFO");
const TCHAR REGKEY_SYSINFO[] = _T("SOFTWARE\\Microsoft\\Shared Tools\\MSINFO");
const TCHAR REGVAL_SYSINFO[] = _T("PATH");
const TCHAR SYSINFO_EXE[] = _T("\\MSINFO32.EXE");

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SYS_INFO, OnSysInfoClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CAboutDlg::~CAboutDlg()
{
}

/////////////////////////////////////////////////////////////////////////////
//
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWaitCursor wait;

	CModuleVersionEx ver;
	ver.SetModuleHandle(AfxGetInstanceHandle());
	
	CString strValue, strText;

	strValue.LoadString(AFX_IDS_APP_TITLE);
	GetDlgItem(IDC_APP_NAME)->SetWindowText(strValue);

	strText.Format(_T("About %s"), strValue);
	SetWindowText(strText);

	strText.Format(_T("Version: %s"), ver.GetFixedFileVersion());
	GetDlgItem(IDC_APP_VERSION)->SetWindowText(strText);

	GetDlgItem(IDC_APP_DESC)->SetWindowText(ver.GetFileDescription());
	GetDlgItem(IDC_APP_COPYRIGHT)->SetWindowText(ver.GetLegalCopyright());

	CString strModules("Component:\tVersion:\tPath:\r\n");

	strModules += GetDatabaseDescription();
	strModules += GetModuleDescription(__uuidof(EodServer), _T("EtsEodServer.exe"));
	strModules += GetModuleDescription(_T("OptionCalc.dll"));
	strModules += GetModuleDescription(_T("EgHttp.dll"));
	strModules += GetModuleDescription(__uuidof(EM::Settings), _T("ETSManager.exe"));
//	strModules += GetModuleDescription(__uuidof(VS::VolatilitySources), _T("VolatilitySources.dll"));
	strModules += GetModuleDescription(__uuidof(PP::PriceProvider), _T("PriceProviders.dll"));

	long nTabStops[2] = { 80, 120 };
	GetDlgItem(IDC_APP_COMPONENTS)->SendMessage(EM_SETTABSTOPS, (WPARAM)2, (LPARAM)nTabStops);
	GetDlgItem(IDC_APP_COMPONENTS)->SetWindowText(strModules);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() == IDC_APP_COMPONENTS)
	{
		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		hbr = GetSysColorBrush(COLOR_WINDOW);
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CAboutDlg::GetModuleDescription(LPCTSTR szFileName)
{
	CModuleVersionEx ver;
	CString strDesc;
	if(ver.SetModuleFileName(szFileName))
		strDesc.Format(_T("%s\t%s\t%s\r\n"), (LPCTSTR)ver.GetFileName(), (LPCTSTR)ver.GetFixedFileVersion(),
						(LPCTSTR)ver.GetFilePath());
	else
		strDesc.Format(_T("%s\tN/A\tN/A\r\n"), (LPCTSTR)szFileName);
	
	return strDesc;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CAboutDlg::GetModuleDescription(CLSID clsid, LPCTSTR szFileName)
{
	CModuleVersionEx ver;
	CString strDesc;
	if(ver.SetModuleCLSID(clsid))
	{
		strDesc.Format(_T("%s\t%s\t%s\r\n"), (LPCTSTR)ver.GetFileName(), (LPCTSTR)ver.GetFixedFileVersion(),
						(LPCTSTR)ver.GetFilePath());
		
	}
	else
		strDesc.Format(_T("%s\tN/A\tN/A\r\n"), (LPCTSTR)szFileName);

	return strDesc;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CAboutDlg::GetDatabaseDescription()
{
	CString strDesc(_T("Database\tN/A\r\n"));
	try
	{
		EM::ISettingsPtr spSettings;
		EM::ISettings2Ptr spSettings2;
		__CHECK_HRESULT(spSettings.CreateInstance(EM::CLSID_Settings), _T("Fail to create ETS settings object."));
		
		try
		{
			__CHECK_HRESULT(spSettings2.CreateInstance(EM::CLSID_Settings), _T("Can't get ISettings2 Interface"));
			spSettings2->InitForProfile("EOD");
		}
		catch(_com_error& e)
		{
			TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage());
		}

		

		_ConnectionPtr spConn;
		__CHECK_HRESULT(spConn.CreateInstance(CLSID_Connection), _T("Fail to create connection object."));

		spConn->ConnectionString = spSettings->DatabaseConnection;
		spConn->ConnectionTimeout = 120;
		spConn->Open(L"", L"", L"", adConnectUnspecified);

		_RecordsetPtr spRs;
		_variant_t vtRows = vtMissing;
		spRs = spConn->Execute(L"exec usp_DataInfo_Get @iDataInfoID = 1", &vtRows, adCmdText);

		if(!spRs->AdoEof)
			strDesc.Format(_T("Database\t%s\r\n"), (LPCTSTR)_bstr_t(spRs->Fields->Item[L"vcKeyValue"]->Value));

		spRs = NULL;
		spConn->Close();
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage());
	}
	return strDesc;
}

/////////////////////////////////////////////////////////////////////////////
//
void CAboutDlg::OnSysInfoClicked()
{
	TCHAR szBuf[4096] = { 0 };
	DWORD dwBufLen = 4096;
	HKEY hKey = 0L;
	long nResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGKEY_SYSINFO, 0, KEY_QUERY_VALUE, &hKey);
	if(ERROR_SUCCESS == nResult)
	{
		nResult = ::RegQueryValueEx(hKey, REGVAL_SYSINFO, NULL, NULL, (LPBYTE)szBuf, &dwBufLen);
		::RegCloseKey(hKey);
	}

	if(ERROR_SUCCESS != nResult)
	{
		nResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGKEY_SYSINFO_LOC, 0, KEY_QUERY_VALUE, &hKey);
		if(ERROR_SUCCESS == nResult)
		{
			nResult = ::RegQueryValueEx(hKey, REGVAL_SYSINFO_LOC, NULL, NULL, (LPBYTE)szBuf, &dwBufLen);
			::RegCloseKey(hKey);
		}
	}

	CString strSysInfoPath(szBuf);
	strSysInfoPath.MakeUpper();
	if(strSysInfoPath.Find(SYSINFO_EXE) < 0)
		strSysInfoPath += SYSINFO_EXE;

	nResult = ERROR_FILE_NOT_FOUND;
	if(::PathFileExists(strSysInfoPath))
		if(::ShellExecute(0, _T("open"), strSysInfoPath, NULL, NULL, SW_SHOWNORMAL) > (HINSTANCE)32L)
			nResult = ERROR_SUCCESS;

	if(ERROR_SUCCESS != nResult)
		MessageBox(_T("System Information is unavailable at this time."), NULL, MB_OK | MB_ICONINFORMATION);
}
