// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "SystemTray.h"
#define	WM_ICON_NOTIFY				WM_APP + 10

class CMainFrame :
	public CFrameWnd,
	public IDispEventSimpleImpl<1, CMainFrame, &DIID__IFixGServerEvents>
{
public:
	// Sink map
	BEGIN_SINK_MAP(CMainFrame)
		SINK_ENTRY_INFO(1, DIID__IFixGServerEvents, DISPID_OnLogMessage, OnLogMessage, &m_OnLogMessage)
		SINK_ENTRY_INFO(1, DIID__IFixGServerEvents, DISPID_OnStatusChanged, OnStatusChanged, &m_OnStatusChanged)
	END_SINK_MAP()

	STDMETHOD(OnLogMessage)(DATE dtTime, FgLogMessageTypeEnum enType, BSTR bsMessage);
	STDMETHOD(OnStatusChanged)(FgServerStatusEnum enStatus);

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnFirstActivate();

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar			m_wndStatusBar;
	CToolBar			m_wndToolBar;
	CReBar				m_wndReBar;
	CSystemTray			m_wndTray;
	FgServerStatusEnum	m_enFixStatus;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTaskbarCreated(WPARAM wp, LPARAM lp);
	afx_msg void OnOptionsSettings();
	afx_msg void OnOptionsConnections();
	afx_msg void OnDestroy();
	afx_msg void OnFileStartFix();
	afx_msg void OnUpdateFileStartFix(CCmdUI *pCmdUI);
	afx_msg void OnFileStopFix();
	afx_msg void OnUpdateFileStopFix(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnOpen();
	afx_msg void OnCopyGrid();
	afx_msg void OnUpdateCopyGrid(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()

private:
	void _InitFixGServer();

protected:
	static _ATL_FUNC_INFO	m_OnLogMessage;
	static _ATL_FUNC_INFO	m_OnStatusChanged;
private:
	IFixGServerPtr			m_spFixGSrv;
	COleDateTime			m_dtFixStartTime;
	COleDateTime			m_dtFixStopTime;
	bool					m_bFixAutoStartStop;
};



