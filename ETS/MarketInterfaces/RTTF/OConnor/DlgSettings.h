//**************************************************************************************************************//
// solution:	OConnor
// project:		OConnor
// filename:	DlgSettings.h
// created:		28-Dec-2002 17:48 by Suchkov Dmitry
//
// purpose:		interface of CDlgSettings
//
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// class CDlgSettings
//**************************************************************************************************************//
class CDlgSettings : public CDialog
{
// Construction
public:

	CDlgSettings(const std::string& sSessionID,  CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgSettings)
	enum { IDD = IDD_SETTINGS };
	CString	m_sAddress;
	CString	m_sDatabase;
	CString	m_sPassword;
	CString	m_sPositionFile;
	CString	m_sTradesFile;
	CString	m_sUser;
	UINT	m_nReadTradesPeriod;
	COleDateTime	m_dtDownloadPositionTime;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CDlgSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//{{AFX_MSG(CDlgSettings)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBtSetup();
	afx_msg void OnChangeEdits();
	afx_msg void OnDatetimechangeDtDownloadpositiontime(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	bool SaveSettings();
	
	bool m_bSettingChanged;
	std::string m_SessionID;
public:
	BOOL m_bIsFtpPassive;
	afx_msg void OnBnClickedCheckPassive();
	afx_msg void OnBnClickedChkEncrypted();
	BOOL m_bEncryptedSource;
	CString m_strPassPhrase;
};

//--------------------------------------------------------------------------------------------------------------//
template <class T>
void GetSetting(const std::string& sSessionID, LPCTSTR lpszValue, T& Value)
{
	std::string sPath(SETTINGS_REGISTRY_KEY);
	sPath += "\\";
	sPath += sSessionID;
	CEgRegKey key;

	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, sPath.c_str(), KEY_READ);
    if (lResult == ERROR_SUCCESS)
    {
       	lResult = key.QueryValue(Value, lpszValue);
        key.Close();
    }
    
    HRESULT hr = HRESULT_FROM_WIN32(lResult);
	if ( FAILED(hr))
	{
        try
        {
            _bstr_t bsMsg = L"Failed to read <";
            bsMsg += lpszValue;
            bsMsg += L"> setting value from the registry.";
            utils::ThrowErrorNoSetErrorInfo(hr, bsMsg);
        }
	    catch (_com_error& e)
		{
            USES_CONVERSION;
		    TraceError(e.Error(), e.Description());
            throw;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------//
template <class T>
void SetSetting(const std::string& sSessionID, LPCTSTR lpszValue, const T& Value)
{

	std::string sPath(SETTINGS_REGISTRY_KEY);
	sPath += "\\";
	sPath += sSessionID;

   	CEgRegKey key;
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, sPath.c_str(), KEY_WRITE);
	if (lResult != ERROR_SUCCESS )
        lResult = key.Create(HKEY_LOCAL_MACHINE, sPath.c_str());

    HRESULT hr = HRESULT_FROM_WIN32(lResult);
	if ( SUCCEEDED(hr))
	{
        try
	    {
		    lResult = key.SetValue(Value, lpszValue);
			if (lResult)
            {
                _bstr_t bsMsg = L"Failed to write <";
                bsMsg += lpszValue;
                bsMsg += L"> setting value to the registry.";
                utils::ThrowErrorNoSetErrorInfo(HRESULT_FROM_WIN32(lResult), bsMsg);
            }
    	}
	    catch (_com_error& e)
		{
            USES_CONVERSION;
		    TraceError(e.Error(), e.Description());
            throw;
		}
	
		key.Close();
	}
}

//{{AFX_INSERT_LOCATION}}
