// FixSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EtsFixManager.h"
#include "FixSettingsDlg.h"

// CFixSettingsDlg dialog
IMPLEMENT_DYNAMIC(CFixSettingsDlg, CDialog)
CFixSettingsDlg::CFixSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFixSettingsDlg::IDD, pParent),
	m_nCheckGatewaysTime(5L),
	m_bRunAtStartup(false),
	m_bAutoStartStop(false),
	m_bIsChanged(false)
{
}

CFixSettingsDlg::~CFixSettingsDlg()
{
}

void CFixSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_DateTimeCtrl(pDX, IDC_DT_START_TIME, m_dtStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DT_STOP_TIME, m_dtStopTime);
}


BEGIN_MESSAGE_MAP(CFixSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_SETTINGS_RUN_AT_STATUP, OnRunAtStartupBnClicked)
	ON_BN_CLICKED(IDC_SETTINGS_AUTOSTART_AT_STATUP, OnAutoStartStopBnClicked)
	ON_EN_KILLFOCUS(IDC_EDIT_SETTING_CHECK_TIME, OnCheckGatewaysTimeEnKillFocus)
END_MESSAGE_MAP()

BOOL CFixSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_LoadData();

	OnAutoStartStopBnClicked();

	GetDlgItem(IDC_DT_START_TIME)->ModifyStyle(0, DTS_TIMEFORMAT);
	GetDlgItem(IDC_DT_STOP_TIME)->ModifyStyle(0, DTS_TIMEFORMAT);

	return TRUE;
}

void CFixSettingsDlg::_LoadData()
{
	CWaitCursor wait;

	try
	{
		CRegKey aKeyLoc, aKeyCur, aKey;
		bool bIsCurrent = false;
		//LONG lResCur = aKeyCur.Open(HKEY_CURRENT_USER, REG_GATEWAY_KEY, KEY_READ);		
		//LONG lResLoc = aKeyLoc.Open (HKEY_LOCAL_MACHINE, REG_GATEWAY_KEY, KEY_READ);
		//if(lResCur == ERROR_SUCCESS)
		//{
		//	bIsCurrent = true;
		//	aKey = aKeyCur;
		//}
		//else if(lResLoc == ERROR_SUCCESS)
		//	aKey = aKeyLoc;

		//if(lResCur == ERROR_SUCCESS || lResLoc == ERROR_SUCCESS)
		//{
		//	DWORD dwVal;
		//	LONG lResult = ERROR_SUCCESS;
		//	lResult = aKey.QueryDWORDValue(_T("CheckGatewaysTime"), dwVal);
		//	if (lResult != ERROR_SUCCESS && bIsCurrent)
		//		lResult = aKeyLoc.QueryDWORDValue(_T("CheckGatewaysTime"), dwVal);

		//	if(ERROR_SUCCESS == lResult)
		//		m_nCheckGatewaysTime = dwVal;
		//}

		//aKeyCur.Close();
		//aKeyLoc.Close();
		//aKey.Close();

		bIsCurrent = false;
		LONG lResCur = aKeyCur.Open(HKEY_CURRENT_USER, REG_AUTOSTART_KEY, KEY_READ);		
		LONG lResLoc = aKeyLoc.Open (HKEY_LOCAL_MACHINE, REG_AUTOSTART_KEY, KEY_READ);
		if(lResCur == ERROR_SUCCESS)
		{
			bIsCurrent = true;
			aKey = aKeyCur;
		}
		else if(lResLoc == ERROR_SUCCESS)
			aKey = aKeyLoc;
		else
			CComErrorWrapper::ThrowError(E_FAIL);

		DWORD dwSize;
		TCHAR szBuf[256] = {0};
		dwSize = sizeof(szBuf) / sizeof(*szBuf);

		LONG lResult = ERROR_SUCCESS;
		lResult = aKey.QueryStringValue(_T("ETS FIX Gateway Manager"), szBuf, &dwSize);
		if (lResult != ERROR_SUCCESS && bIsCurrent)
			lResult = aKeyLoc.QueryStringValue(_T("ETS FIX Gateway Manager"), szBuf, &dwSize);

		if(ERROR_SUCCESS == lResult)
			m_bRunAtStartup = true;

	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("FIX Gateway settings loading failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}
	
	_ControlChecked[IDC_SETTINGS_RUN_AT_STATUP] = m_bRunAtStartup ? TRUE : FALSE;
	_ControlChecked[IDC_SETTINGS_AUTOSTART_AT_STATUP] = m_bAutoStartStop ? TRUE : FALSE;

	CString strVal;
	strVal.Format(_T("%d"),m_nCheckGatewaysTime);
	_ControlText[IDC_EDIT_SETTING_CHECK_TIME] = strVal;
}

void CFixSettingsDlg::_SaveData()
{
	CWaitCursor wait;
	try
	{
		CRegKey aKey;
		LONG lResult = aKey.Open(HKEY_CURRENT_USER, REG_GATEWAY_KEY, KEY_WRITE);
		if(lResult == ERROR_FILE_NOT_FOUND)
			lResult = aKey.Create(HKEY_CURRENT_USER,REG_GATEWAY_KEY);
		if (lResult != ERROR_SUCCESS)
			CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(lResult), _T("Can't create registry key"));

		aKey.SetDWORDValue(_T("CheckGatewaysTime"), m_nCheckGatewaysTime);

		aKey.Close();

		lResult = aKey.Open(HKEY_CURRENT_USER, REG_AUTOSTART_KEY, KEY_WRITE);

		if(ERROR_SUCCESS == lResult)
		{
			if(m_bRunAtStartup)
			{
				TCHAR szAppPath[4096] = {0};

				if(::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, 4096))
				{
					CString strVal;
					strVal.Format(_T("\"%s\" /background"),  szAppPath);
					lResult = aKey.SetStringValue(_T("ETS FIX Gateway Manager"), strVal);
				}
			}
			else
			{
				lResult = aKey.DeleteValue(_T("ETS FIX Gateway Manager"));
			}
		}
	}
	catch(_com_error& e)
	{
		CString strError;
		strError.Format(_T("FIX Gateway settings saving failed: %s (%X)"),
			(LPCTSTR)e.Description() ? (LPCTSTR)e.Description() : e.ErrorMessage(), e.Error());
		MessageBox(strError, NULL, MB_ICONSTOP);
	}
}

void CFixSettingsDlg::OnOK()
{
	if(m_bIsChanged)
		_SaveData();

	CDialog::OnOK();
}

void CFixSettingsDlg::OnRunAtStartupBnClicked()
{
	m_bRunAtStartup = _ControlChecked[IDC_SETTINGS_RUN_AT_STATUP] ? true : false;
	m_bIsChanged = true;
}

void CFixSettingsDlg::OnAutoStartStopBnClicked()
{
	m_bAutoStartStop = _ControlChecked[IDC_SETTINGS_AUTOSTART_AT_STATUP] ? true : false;
	_ControlEnabled[IDC_STATIC_SETTING_START_TIME] = m_bAutoStartStop;
	_ControlEnabled[IDC_STATIC_SETTING_STOP_TIME] = m_bAutoStartStop;
	_ControlEnabled[IDC_DT_START_TIME] = m_bAutoStartStop;
	_ControlEnabled[IDC_DT_STOP_TIME] = m_bAutoStartStop;	
}

void CFixSettingsDlg::OnCheckGatewaysTimeEnKillFocus()
{
	m_nCheckGatewaysTime = _ttol(_ControlText[IDC_EDIT_SETTING_CHECK_TIME].Trim());
	m_bIsChanged = true;
}
