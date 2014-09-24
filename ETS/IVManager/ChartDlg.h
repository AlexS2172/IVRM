#if !defined(AFX_CHARTDLG_H__BFBD3353_FD05_461B_B477_2327D7EBDDE4__INCLUDED_)
#define AFX_CHARTDLG_H__BFBD3353_FD05_461B_B477_2327D7EBDDE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChartDlg dialog

#include "FlatComboBox.h"
#include "EGButton.h"

#include "DataProcessor.h"
#include "VolaEditorDlg.h"

#include "PopupText.h"


typedef std::vector <DATE> dates_vec;
typedef std::vector <double> price_vec;
typedef std::vector <std::basic_string<TCHAR> >  strings_vec;

// Base class for dialog window with chart
class CChartDlg : public CDialog
{
	friend class CChartWnd;
// Construction
public:
	void AdjustLogo();
	CChartDlg();   // standard constructor
	BOOL Create (CWnd * pWnd);

	virtual void InitializeChart() = 0 ;
	virtual void UpdateChart( bool bReinitialize ) = 0 ;
#ifdef _VOLA_MANAGER
	virtual void UpdateCustomVolatility( UINT nMessage, CCustomVolaNotification *pNotification ) = 0 ;
#endif //_VOLA_MANAGER

	// Vola management overrides
	virtual bool   UpdateCustomVolaPoint( long lMovingPtIdx, double dVola, double& dtMonth ) = 0;
	virtual void   InterpolateCustomVolaToImplied() = 0;
	virtual void   RestoreCustomVola() = 0;
	virtual void   UpdateCustomVola() = 0;
	virtual bool   SetupVolaEditor( long lPtIdx, CVolaEditorDlg& ve ) = 0;
	virtual void   SavePointValue( long lMovingPtIdx ) = 0;
	virtual double GetPointValue( long lMovingPtIdx ) = 0;

// Dialog Data
	//{{AFX_DATA(CChartDlg)
	enum { IDD = IDD_CHART };
	CEGButton		m_btFitToImplied;
	CEGButton		m_btUpdate;
	CEGButton		m_btRestore;
	CChart2D		m_ctlChart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	BOOL CheckAutoCenter()
	{
		return FALSE;
	}


#ifdef _VOLA_MANAGER
	void   SetIsDirty( bool bIsDirty );
	void   EnableVolaManagement( bool bManagement, bool bFitToImplied );
	void PostNotification( UINT nMessage, CCustomVolaNotification::Type enType, double dtMonth );
#endif //_VOLA_MANAGER
	void SetManageableVola( bool bEnable = true );

protected:
	void InitChart();
	void SetChartIsBatched(BOOL bIsBatched=TRUE );
	long GetVolaPointIdxByCoord(int x, int y, long *pSeries);
	long GetBasePointIdxByCoord(int x, int y);
	bool IsValidVola (double dVola)
	{
		return !fEQZero( dVola );
	}
	void AdjustControlArea( int nOffsetY );
	virtual CString GetXLabel (long lSeries, long lPtIdx)
		{ return _T(""); }
	CString GetCurveModeTitle();

	int					m_nIsBatchedCount;
	CAxis				m_AxisY2;
	CAxis				m_AxisY;
	CAxis				m_AxisX;
	CChartGroup			m_cGroup1;
	CChartGroup			m_cGroup2;
	CChartLabel			m_lblVolatility;
	CChartLabel			m_lblInfoIndicator;
	CChartLabel			m_lblInfo;
	CChartLabel			m_lblLogo;

	dates_vec			m_vecDates;
	price_vec			m_vecPrices;
	strings_vec         m_vecSymbols;
	CPopupText			m_wndPopupText;

	bool		m_bManageableVola;			// TRUE if in vola management mode - controla are visible
#ifdef _VOLA_MANAGER
	long		m_nSurfaceID;
public:
	bool		m_bIsDirty;					// TRUE if vola has been changed - update/restore are active
protected:
	bool		m_bVolaManagementEnabled;	// TRUE if there are manageble points - fit to implied and drag editing active
	bool		m_bFitToImpliedEnabled;		// TRUE if there are implied vola points - fit to implied active
#endif //_VOLA_MANAGER

	int			m_nControlAreaHeight;
	bool		m_bChartModify;
	long		m_lMovingPtIdx;
	long		m_lMovingBasePtIdx;
	long		m_lATMPointIdx;
	short		m_nShifted;
	long		m_lVolaMultiplier;
	
	static const int m_nOffset;

	static const long g_lBack;
	static const long g_lHistVolaHi;
	static const long g_lHistVolaLo;
	static const long g_lHistVolaAvg;
	static const long g_lVola;
	static const long g_lCustomVola;
	static const long g_lCustomVolaBasePoints;
	static const long g_lCustomVolaPrev;
	static const long g_lCustomVolaATM;
	
	static const long g_lHistVolaMax;
	static const long g_lHistVolaMin;

protected:
	void ResizeControls();
	void AdjustBack(double dMinX, double dMaxX, double dMinY, double dMaxY);
	void ClearSeries( bool bPreserveCustomVola );
	void SetTitle (LPCTSTR szTitle);

	// Generated message map functions
	//{{AFX_MSG(CChartDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMoveTsChart(short Button, short Shift, long X, long Y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnModifyStartTsChart(BOOL FAR* IsOK);
	afx_msg void OnModifyEndTsChart();
	afx_msg void OnMouseDownTsChart(short Button, short Shift, long X, long Y);
	afx_msg void OnMouseUpTsChart(short Button, short Shift, long X, long Y);
	afx_msg void OnUpdate();
	afx_msg void OnRestore();
	afx_msg void OnToimplied();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDblClickTsChart();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg BOOL OnHelpInfo( HELPINFO* lpHelpInfo );
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTDLG_H__BFBD3353_FD05_461B_B477_2327D7EBDDE4__INCLUDED_)
