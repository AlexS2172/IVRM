 #if !defined(AFX_VOLAMANAGEMENTDLG_H__11473FED_DBF3_46BE_A847_31B6F1C21117__INCLUDED_)
#define AFX_VOLAMANAGEMENTDLG_H__11473FED_DBF3_46BE_A847_31B6F1C21117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolaManagementDlg.h : header file
//

//{{AFX_INCLUDES()
#include "EGButton.h"
#include "FlatComboBox.h"
#include "NumericEditEx.h"
#include "..\\EGOcx\\vsflexgrid.h"
//}}AFX_INCLUDES

/////////////////////////////////////////////////////////////////////////////
// CVolaManagementDlg dialog

class CVolaManagementDlg : public CDialog
{
// Properties

	bool	m_bInitialized;
	bool	m_bNexdDaySurfaceMode;

	CString	m_strTitle;
	long    m_nContractID;
	VME::IVMSymbolVolatilitySurfacePtr m_spSurface;

	double	m_dIFactor;
	long	m_nIFactor;
	bool	m_bEditing;

	CSurfacesVector m_vecSurfaces;
	long m_nCurrentSurface;

	CRulesVector m_vecRules;
	long m_nCurrentRule;
	bool m_bGroupRuleSet;
	bool m_bContractRuleSet;
	
	std::vector<DATE> m_months;

	long m_nMinW, m_nMinH;
	long m_nMargine;

	long m_nMouseRow;
	long m_nMouseCol;
	
	bool m_bNeedSave;

// Construction
public:
	CVolaManagementDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVolaManagementDlg)
	enum { IDD = IDD_VOLA_MANAGEMENT_DIALOG };
	CStatic			m_stcUpperLine;
	CEGButton		m_btnClose;
	CButton			m_chkDiscreteAccel;
	CButton			m_chkPriceOverride;
	CNumericEditEx	m_edtLoVoly;
	CNumericEditEx	m_edtHiVoly;
	CNumericEditEx	m_edtUnderlinePrice;
	CNumericEditEx	m_edtAccelerator;
	CSliderCtrl		m_sldFactor;
	CNumericEditEx	m_edtFactor;
	CEGButton		m_btnVolaFitToImplied;
	CEGButton		m_btnVolaClear;
	CEGButton		m_btnVolaRegenerate;
	CEGButton		m_btnVolaUpdate;
	CEGButton		m_btnVolaRestore;
	CEGButton		m_btnNewRule;
	CEGButton		m_btnUpdateRule;
	CEGButton		m_btnDeleteRule;
	CFlatComboBox	m_cmbCurrentRule;
	CVSFlexGrid		m_fgVola;
	CEGButton		m_btnDualQuadPar;
	//}}AFX_DATA
	bool m_bAlwaysOnTop;
	bool m_bUseExtrapolation;

	void   UpdateSymbolData( int nID, const std::vector<DATE>& months );
	void   UpdateView();
	void   UpdateByPrice();
	void   UpdateRules();
	void   PostNotification( UINT nMessage );
	void   PostNotification( UINT nMessage, CCustomVolaNotification::Type enType, double dtMonth = 0 );
	double GetInterpolationFactor();
	bool   OpenNextDaySurface( long nSurfaceID );
	bool   OpenDefaultSurface( long nSurfaceID );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolaManagementDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void GridInit();
	void GridFormat();
	void GridFill( bool bForceFocus = false );
	void GridDrawATMMarks();

	void SetupDialog( double dUnderlinePrice );
	bool SetupSurfaces();
	void SetupRules();
	void SetupControls();
	void EnableControls( bool bForceDisable );
	void SetupUnderlinePrice( CSurfaceData& sd );

	// Generated message map functions
	//{{AFX_MSG(CVolaManagementDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	afx_msg void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeFactor();
	afx_msg void OnComboCurrentRuleSelchange();
	afx_msg void OnPriceOverrideCheck();
	afx_msg void OnClose();
	afx_msg void OnVolaStartEdit(long Row, long Col, BOOL FAR* Cancel);
	afx_msg void OnVolaValidateEdit(long Row, long Col, BOOL FAR* Cancel);
	afx_msg void OnVolaAfterEdit(long Row, long Col);
	afx_msg void OnUpdateRule();
	afx_msg void OnDiscreteAccelCheck();
	afx_msg void OnChangeHiVolatility();
	afx_msg void OnChangeLoVolatility();
	afx_msg void OnVolaRegenerate();
	afx_msg void OnVolaUpdate();
	afx_msg void OnVolaRestore();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVolaKeyUp(short FAR* KeyCode, short Shift);
	afx_msg void OnVolaMouseUp(short Button, short Shift, float X, float Y);
	afx_msg void OnVmMakeBasePoint();
	afx_msg void OnVmMakeComomonPoint();
	afx_msg void OnVmSurfaceOpen();
	afx_msg void OnVmSurfaceNew();
	afx_msg void OnVmSurfaceEdit();
	afx_msg void OnVmSurfaceDelete();
	afx_msg void OnVmViewGenerationOptions();
	afx_msg void OnVmSelectAll();
	afx_msg void OnNewRule();
	afx_msg void OnDeleteRule();
	afx_msg void OnVmSurfaceSetAsDefault();
	afx_msg void OnChangeAccelerator();
	afx_msg void OnDestroy();
	afx_msg void OnVmSurfaceNextday();
	afx_msg void OnVmViewAlwaysontop();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnVolaClear();
	afx_msg void OnVolaFittoimplied();
	afx_msg void OnVmViewShowAll();
	afx_msg void OnVmSurfaceSaveas();
	afx_msg void OnDualquadrPar();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg void OnCommitUnderlinePrice();
	afx_msg LRESULT OnUpdateView( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

// Helpers
	long _GetATMStrikeRow( long col );
	void _AlignControls( bool bShowOptions );
	void _UnbasePoints();
	void _ChangeCurrentSurface( size_t nNewCurrentSurface );
	void _ExtrapolateVolatility();
	void _ExtrapolateMonthVolatility( long col, const VME::IVMStrikeSkewVolatilitiesPtr& spSkew, 
		bool bIsFlatWings = false, double dStrikeLow = 0.0f, double dStrikeHi = 0.0f);
	void _SelectAll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLAMANAGEMENTDLG_H__11473FED_DBF3_46BE_A847_31B6F1C21117__INCLUDED_)
