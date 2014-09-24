// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "SystemTray.h"
#define	WM_ICON_NOTIFY				WM_APP + 10

class CMainFrame :
	public CFrameWnd,
	public IDispEventSimpleImpl<1, CMainFrame, &DIID__IEodServerEvents>
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnFirstActivate();
	bool IsTimerSet();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CSystemTray	m_wndTray;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTaskbarCreated(WPARAM wp, LPARAM lp);
	afx_msg void OnOptionsSettings();
	afx_msg void OnOptionsConnections();
	afx_msg void OnDestroy();
	afx_msg void OnFileStartEod();
	afx_msg void OnUpdateFileStartEod(CCmdUI *pCmdUI);
	afx_msg void OnFileStopEod();
	afx_msg void OnUpdateFileStopEod(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()

protected:
	long					m_nEodSteps;

	bool					m_bEodRunPeriodically;
	COleDateTime			m_dtEodStartTime;
	EodCalcModelTypeEnum	m_enCalcModel;
	int						m_nLogLevel;
	int						m_nLogHistory;
	
	IEodServerPtr			m_spEodSrv;

	enum EodState
	{
		enEodStateUnavailable	= -1,
		enEodStateStopped		= enStStopped,
		enEodStateStarted		= enStStarted,
		enEodStateStopPending	= enStStopPending,
		enEodStateStartPending	= enStStartPending
	};

	long					m_nEodState;

private:
	void _InitEodServer();

public:
// Sink map
BEGIN_SINK_MAP(CMainFrame)
	SINK_ENTRY_INFO(1, DIID__IEodServerEvents, DISPID_OnEodStatusChanged, OnEodStatusChanged, &m_OnEodStatusChangedInfo)
	SINK_ENTRY_INFO(1, DIID__IEodServerEvents, DISPID_OnError, OnEodError, &m_OnEodErrorInfo)
END_SINK_MAP()

protected:
	static _ATL_FUNC_INFO	m_OnEodStatusChangedInfo;
	static _ATL_FUNC_INFO	m_OnEodErrorInfo;

	STDMETHOD(OnEodStatusChanged)(EodStatusEnum enStatus);
	STDMETHOD(OnEodError)(BSTR bsDescription);
public:
	afx_msg void OnClose();
	afx_msg void OnOpen();
};

