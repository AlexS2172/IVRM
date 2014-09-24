// SageDlg.h : header file
//

#if !defined(__SAGEDLG_H__)
#define __SAGEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSageDlg dialog

class CSageDlg : public CDialog
{
// Construction
public:
	CSageDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSageDlg)
	enum { IDD = IDD_SAGE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void AutoStart(bool bEnable);
	void AutoStop(bool bEnable);

// Implementation
protected:
	HICON   m_hIcon;
    CMenu   m_Menu;
    HANDLE  m_hThread;
    BOOL    m_bWaitStopping;
    HACCEL  m_hAccel;

	CHandle m_hStopEvent;
	bool    m_bAutoStart;
    
	CToolBar    m_wndToolBar;
    
	// Generated message map functions
	//{{AFX_MSG(CSageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAppAbout();
	afx_msg void OnStart();
	afx_msg void OnUpdateControls(CCmdUI* pCmdUI);
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnKickIdle();
	afx_msg void OnStop();
	afx_msg void OnClose();
	afx_msg void OnProperties();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClearLog();
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
        
private:
    void UpdateToolbar();
    BOOL IsStarted();
    static UINT __stdcall _thread(void* p);
	static UINT __stdcall _threadStop(void* p);
};

inline BOOL CSageDlg::IsStarted()
{
    BOOL bStarted = FALSE;
    if (m_hThread != INVALID_HANDLE_VALUE)
        bStarted = (::WaitForSingleObject(m_hThread, 0) == WAIT_TIMEOUT);
    return bStarted;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SAGEDLG_H__)
