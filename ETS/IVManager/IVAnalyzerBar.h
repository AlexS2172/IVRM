#if !defined(AFX_IVANALYZERBAR_H__9CEF97E9_EAC9_4F60_87E9_039A589BEABA__INCLUDED_)
#define AFX_IVANALYZERBAR_H__9CEF97E9_EAC9_4F60_87E9_039A589BEABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IVAnalyzerBar.h : header file
//
#include "FlatComboBox.h"
#include "AppBar.h"

#include "DataProcessor.h"
#include "ChartWnd.h"

#include "BCMenu.h"
#include "IVControls.h"
#include "InfoBar.h"
#include "IconBar.h"
#include "SymbolCombo.h"

#ifdef _VOLA_MANAGER
#include "VolaManagementDlg.h"
#include "VolaNextDaySurfaceDlg.h"
#endif //_VOLA_MANAGER

/////////////////////////////////////////////////////////////////////////////
// CIVAnalyzerBar dialog

struct CMonthData
{
	CMonthData (CChartWnd * pWnd = NULL, bool bValid=false):
		m_pWnd (pWnd), m_bValid(bValid)
	{
	}

	CChartWnd * m_pWnd;
	bool m_bValid;
};

// Months map - maps month to the strike skew window
// Used long - is is DATE without the time
class  months_map:
	public std::map <CExpDate, CMonthData >,
	public months_container
{
public:
	void invalidate ()
	{
		for (iterator it = begin(); it != end(); it++ )
			it->second.m_bValid = false;
	}
	bool areAllWndsVisible()
	{
		for (iterator it = begin(); it != end(); it++ )
		{
			CMonthData & md = it->second;
			if ( ! md.m_pWnd  || ! md.m_pWnd->IsWindowVisible())
				return false;
		}
		return true;
	}

	void closeInvalid ()
	{
		iterator it = begin();
		while ( it != end() )
		{
			CMonthData & md = it->second;
			if (! md.m_bValid  )
			{
				if (md.m_pWnd)
				{
					md.m_pWnd->DestroyWindow();
					delete md.m_pWnd;
					md.m_pWnd = NULL;
				}

				it = erase (it);
			}
			else
				it++;
		}
	}

	void addMonth (DATE dt)
	{
		iterator it = find (CExpDate(dt));
		if ( it == end() )
			insert ( std::make_pair (CExpDate(dt), CMonthData ( NULL, true ))  );
		else
			it->second.m_bValid = true; 
	}
	std::vector<DATE> getMonth() const
	{
		std::vector<DATE> month;

		for (const_iterator it = begin(); it != end(); it++ )
			if( it->second.m_bValid )
				month.push_back( it->first.date_);

		return month;
	}
};

typedef std::vector <CWnd *> vec_wnds;

class CIVAnalyzerBar;


// Main window -  designed as application toolbar
class CIVAnalyzerBar : 
public CAppBar
{
	DECLARE_DYNCREATE(CIVAnalyzerBar)

// Construction
public:
	CIVAnalyzerBar(CWnd* pParent = NULL);   // standard constructor
	BOOL Create();
// Dialog Data
	//{{AFX_DATA(CIVAnalyzerBar)
	enum { IDD = IDD_APP_BAR };
	CSymbolCombo	  m_cbSymbol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVAnalyzerBar)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	CVolaNotificationClient * GetVolaNotificationClient () { return &m_volaNotifClient;};


	// Tests hit into control menu icon area
	bool PtInIcon(CPoint pt)
	{
		CRect rc = GetIconRect();
		CRect rcCapt = GetCaptionRect(true);
		
		rc.left = rcCapt.left;
		rc.right = rcCapt.right;

		return rc.PtInRect (pt) == TRUE;
	}

	// Tests hit into caption area
	bool PtInCaption(CPoint pt)
	{
		CRect rc = GetCaptionRect(true);
		return rc.PtInRect (pt)  == TRUE;
	}
protected:
	void InitControlBars();
	int GetCaptionSize()
	{
		return m_nIconSize + 4;
	}

	CRect GetCaptionRect(bool bScreen = false);
	CRect GetIconRect();
	
	void ShowMainMenu(const CPoint & pt);
	
	
	void UpdateWindows( bool bReinitialize = false );
	void OnAppBarStateChange (BOOL fProposed, UINT uStateProposed);
	
	// Returns current text in the symbol combo
	CString GetCurSymbol ()
	{
		CString strSymbol;
		m_cbSymbol.GetWindowText(strSymbol);

		return strSymbol;
	}
	
	void StartAnimation()
	{
		//m_wndInfoBar.m_picLogo.Start();
		m_bAnimation = true;
	}

	void StopAnimation()
	{
		//m_wndInfoBar.m_picLogo.Stop();
		m_bAnimation = false;
	}

	void LoadSymbols();
	void ResizeControls();
	void GetData (bool bInitial = false);
	void HideFloatAdornments(BOOL fHide); 
	
	void ErrorMsgBox (LPCTSTR szMessage)
	{
		CWnd  * pWnd = NULL;
		if (this->IsWindowVisible())
			pWnd = this;

		pWnd->MessageBox (szMessage, NULL, MB_OK | MB_ICONERROR  | MB_TOPMOST | MB_TASKMODAL ); 		
	}

	CImageList	m_ilBarIcon; 
	CImageList	m_ilToolbar; 
	CImageList  m_ilCharts;
	CImageList  m_ilWindows;
	CImageList  m_ilSvcs;

	CIVToolBar	m_wndSymbolToolBar;
	CIVToolBar  m_wndServicesToolBar;
	
	CInfoBar		m_wndInfoBar;
	CIconBar		m_wndIconBar;
	CIVStatusBar	m_wndStatusBar;

	CIVReBar  m_wndReBar;
	CIVReBar  m_wndReBar2;
	BCMenu m_mnuMonths;
	BCMenu m_mnuMain;
	
	CSize m_sizeMinMax;
	int m_nHeightDocked;
	int m_nHeightFloat;
	UINT m_nCurArrange;
	UINT m_nNextArrange;
	bool m_bOnline;	
	CVolaNotificationClient m_volaNotifClient;

	CDBLayout m_db;

	static const int m_nIconSize;

	CChartWnd m_timeSkewWnd;
	months_map m_months;

	sUndPrice m_sCurSymbolPriceData;

	
	bool m_bDrawSmallFrame;
	bool m_bActiveCaption;
	bool m_bSaveState;
	bool m_bAnimation;

#ifdef _VOLA_MANAGER
	CVolaManagementDlg	    m_wndVM;
	CVolaNextDaySurfaceDlg	m_wndNDS;
#endif //_VOLA_MANAGER
	
protected:
	void StopOnlineIndication();
	void InitMenus();
	void InitImages();
	void CreateReBar();
	void InitImageList (CImageList & lst, UINT nID);
	void TileWnds (const vec_wnds & wnds, int nColCnt);
	void GetChartWndsList(vec_wnds &  rwnds );
	void SaveLastSymbol();
	bool ShowStrikeSkew(DATE dtMonth, CMonthData & md);
	void InitConnection();
	void StopProcessing();
	void SaveBarState();
	void InitAppBar();
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	void AddSymbol(LPCTSTR szSymbol = NULL);

//	afx_msg LRESULT OnVolaChangedNotification (WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnUndChangedNotification(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnError (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOnline (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStockChanged (WPARAM wParam, LPARAM lParam);
#ifdef _VOLA_MANAGER
	afx_msg LRESULT OnSettingsChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCustomVolaChanged ( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCustomVolaSaved	( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCustomVolaRestored( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCustomVolaPriceChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCustomVolaDefaultSurfaceChanged( WPARAM wParam, LPARAM lParam );

	afx_msg LRESULT OnRulesListChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnNextDaySurfaceListChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnNextDaySurfaceOpen( WPARAM wParam, LPARAM lParam );
#endif //_VOLA_MANAGER
	
	afx_msg void OnStrikeSkew(UINT nID);
    afx_msg void OnToolbarDropDown(NMHDR* pnmtb, LRESULT *plr);
	afx_msg void OnRebarHitTest (NMHDR* lpnmmouse, LRESULT *plr);
	afx_msg void OnRebarClick ( LPNMHDR lpnmh, LRESULT *plr);

	// Generated message map functions
	//{{AFX_MSG(CIVAnalyzerBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnCtlColorStatic (WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnAddSymbol(WPARAM wParam, LPARAM lParam);	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimeSkew();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
//	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnAppbarAutohide();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnAppbarAlwaysontop();
	afx_msg void OnStructureVola();
	afx_msg void OnNextDaySurface();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnWindowCloseall();
	afx_msg void OnEditchangeSymbols();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnGo();
	afx_msg void OnEditupdateSymbols();
	afx_msg void OnImportAll();
	afx_msg void OnCreateDefaultSurfaces();
	afx_msg void OnImportCurrentSymbol();
	afx_msg void OnIRCurve();
	afx_msg void OnSelectSymbol();
	afx_msg void OnCancelSymbol();
	afx_msg void OnOptions();
	afx_msg void OnAppAbout();
	afx_msg void OnAppHelp();
	afx_msg void OnStrikeSkew();
	afx_msg void OnWindowArrange();
	afx_msg BOOL OnHelpInfo( HELPINFO* lpHelpInfo );
	afx_msg void OnCharts();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileClose();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnWindowTileHorz();
	afx_msg void OnWindowTileVert();
	afx_msg void OnWindowCascade();
	afx_msg void OnUpdateAdvertizing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGo(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAddNewSymbol();
	afx_msg void OnRemoveSymbol();
	afx_msg void OnSymbolGroups();
	afx_msg void OnRules();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVANALYZERBAR_H__9CEF97E9_EAC9_4F60_87E9_039A589BEABA__INCLUDED_)
