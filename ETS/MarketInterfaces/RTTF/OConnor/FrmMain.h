//**************************************************************************************************************//
// solution:	OConnor
// project:		OConnor
// filename:	FrmMain.h
// created:		27-Dec-2002 17:12 by Suchkov Dmitry
//
// purpose:		interface of CFrmMain
//
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "ViewLog.h"

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define WM_INSERT_TRACE_MESSAGE WM_USER + 0x040

//**************************************************************************************************************//
// class CFrmMain
//**************************************************************************************************************//
class CFrmMain : 
	public CFrameWnd,
	public CThread
{
	
public:
	CFrmMain();
protected: 
	DECLARE_DYNAMIC(CFrmMain)

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CFrmMain)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFrmMain();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void AutoStart(bool bMode){m_bAutoStart = bMode;}
	void SetSessionID(const std::string& sSessionID)
	{
		m_strSessionID = sSessionID;
	}

	std::string GetSessionID(){return m_strSessionID;}

	virtual UINT Run();
protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar  m_wndDlgBar;
	CViewLog    m_wndView;
private:
	std::string m_strSessionID;
	bool        m_bAutoStart;
	CHandle     m_hCloseApplication;
// Generated message map functions
protected:
	//{{AFX_MSG(CFrmMain)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg LRESULT OnInsertTraceMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnActionStart();
	afx_msg void OnUpdateActionStart(CCmdUI* pCmdUI);
	afx_msg void OnActionStop();
	afx_msg void OnUpdateActionStop(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSetting();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	bool IsRunning() const;

protected:

	CString MakeItemString(int nItem);

	void WindowPlacementSave();
	void WindowPlacementLoad();
	HANDLE m_hThread;
	static UINT __stdcall OConnorProc(PVOID);
};

//{{AFX_INSERT_LOCATION}}
