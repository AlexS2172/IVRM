#if !defined(AFX_CREATEDEFAULTVOLADLG_H__1A52BF77_69D8_4520_A80E_924C9FF020BE__INCLUDED_)
#define AFX_CREATEDEFAULTVOLADLG_H__1A52BF77_69D8_4520_A80E_924C9FF020BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateDefaultVolaDlg.h : header file
//

#define WM_START_VOLA_GENERATION WM_USER+1


/////////////////////////////////////////////////////////////////////////////
// CCreateDefaultVolaDlg dialog

class CCreateDefaultVolaDlg : public CDialog
{
public:
	struct CContractInfo
	{
		long nID;
		DATE dtExpDate;
	};

	typedef std::multimap<_bstr_t, CContractInfo> contracts_map;

// Construction
public:
	CCreateDefaultVolaDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL ContinueModal();

// Dialog Data
	//{{AFX_DATA(CCreateDefaultVolaDlg)
	enum { IDD = IDD_MKT_STRUCTURE };
	CButton	m_btCancel;
	CProgressCtrl	m_pgContract;
	CStatic	m_stInfo;
	CEdit	m_edLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateDefaultVolaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static const int m_nPosPerContract;

	contracts_map m_stocks;
	contracts_map::iterator m_itCur;

	bool m_bStarted;
	bool m_bCanceled;
	long m_nCounter;

	bool StartGeneration();
	void GetNextStock();

	bool Stop();
	virtual void OnStop() {}
	void UpdateProgress();

	void StatusMessage(LPCTSTR szMessage, bool bLog = true);
	void StatusMessage(UINT  iMessage, bool bLog = true);
	void ErrorMessage (const _com_error & e);

	void DoEvents();

	// Generated message map functions
	//{{AFX_MSG(CCreateDefaultVolaDlg)
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnStartGeneration( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEDEFAULTVOLADLG_H__1A52BF77_69D8_4520_A80E_924C9FF020BE__INCLUDED_)
