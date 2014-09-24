#if !defined(AFX_OPTIONSDLG_H__98E7AFA4_12E4_4F28_BFA9_37E610C8D547__INCLUDED_)
#define AFX_OPTIONSDLG_H__98E7AFA4_12E4_4F28_BFA9_37E610C8D547__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CButton		m_btUseProxyPrice;
	CEdit		m_edPortPrice;
	CEdit		m_edProxyPrice;

	CButton		m_btUseProxyIV;
	CEdit		m_edPortIV;
	CEdit		m_edProxyIV;
	
	CEdit		m_edChatrModeFrom;
	CEdit		m_edChatrModeTo;
	
	CButton		m_btProxyAuth;
	CEdit       m_edProxyUserID;
	CEdit       m_edProxyUserPass;

	CComboBox   m_listProviders;
	CString		m_strDisplayConnection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void OnOK();
// Implementation
protected:
	IDataInitialize * GetDataInit ()
	{
		if (! m_spDataInit)
			m_spDataInit.CoCreateInstance (CLSID_MSDAINITIALIZE);

		return m_spDataInit;
	}
	IDBPromptInitialize * GetDBPromptInit ()
	{
		if (! m_spPromptInit) 
			m_spPromptInit.CoCreateInstance(CLSID_DataLinks);
		
		return m_spPromptInit;
	}

	CComPtr <IDBPromptInitialize> m_spPromptInit;
	CComPtr <IDataInitialize> m_spDataInit;
	CComPtr <IDBInitialize> m_spDBInitialize;
	
	CString m_strConnection;
	CIVAnalyzerOptions m_opts;

	PP::IProvidersPtr m_spProvider;
	
	void InsertProvider(const CLSID& cls, LPCTSTR szDescription, long type);
	void InsertProvider(LPCTSTR szDescription, long type);
	void InitProvidersList();
	bool GetObject(const CLSID& clsid);

	int    m_iChartMode;
	int    m_iRangeCheck;

	int    m_iFlatOutRange;
	int    m_iFlatNoBid;
	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetupConnection();
	afx_msg void OnUseProxy();
	afx_msg void OnUseProxy2();
	afx_msg void OnChartMode();
	afx_msg void OnProxyAuth();
	afx_msg void OnSelchangeProviderType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__98E7AFA4_12E4_4F28_BFA9_37E610C8D547__INCLUDED_)
