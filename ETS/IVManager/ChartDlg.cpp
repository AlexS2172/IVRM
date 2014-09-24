// ChartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVAnalyzer.h"
#include "ChartDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define XrtColorHILO	RGB(180, 210, 190)
//#define XrtColorHILO	RGB(240, 200, 150)
//#define XrtColorBkgnd	RGB(170, 210, 170)

/////////////////////////////////////////////////////////////////////////////
// CChartDlg dialog

const long CChartDlg::g_lBack		= 1L;
const long CChartDlg::g_lHistVolaHi	= 2L;
const long CChartDlg::g_lHistVolaLo	= 3L;

const long CChartDlg::g_lHistVolaAvg		  = 1L;
const long CChartDlg::g_lVola				  = 2L;
const long CChartDlg::g_lCustomVolaPrev		  = 3L;
const long CChartDlg::g_lCustomVola			  = 4L;
const long CChartDlg::g_lCustomVolaBasePoints = 5L;
const long CChartDlg::g_lCustomVolaATM		  = 6L;
const long CChartDlg::g_lHistVolaMax		  = 7L;
const long CChartDlg::g_lHistVolaMin		  = 8L;

const int CChartDlg::m_nOffset = 2;


CChartDlg::CChartDlg()
	: CDialog(CChartDlg::IDD, NULL),
	m_nIsBatchedCount(0), 
#ifdef _VOLA_MANAGER
	m_bManageableVola(TRUE), 
	m_nSurfaceID(-1),
	m_bIsDirty(false),
	m_bVolaManagementEnabled(false),
	m_bFitToImpliedEnabled(false),
#else
	m_bManageableVola(FALSE), 
#endif //_VOLA_MANGER
	m_nControlAreaHeight(0),
	m_bChartModify(false),
	m_lMovingPtIdx(0),
	m_lMovingBasePtIdx(0),
	m_lATMPointIdx(0),
	m_nShifted(false),
	m_lVolaMultiplier (100)
{
	//{{AFX_DATA_INIT(CChartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CChartDlg::Create (CWnd * pWnd)
{
	return CDialog::Create (IDD_CHART, pWnd );
}


void CChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartDlg)
	DDX_Control(pDX, IDC_TOIMPLIED, m_btFitToImplied);
	DDX_Control(pDX, IDC_UPDATE, m_btUpdate);
	DDX_Control(pDX, IDC_RESTORE, m_btRestore);
	DDX_Control(pDX, IDC_TS_CHART, m_ctlChart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartDlg, CDialog)
	//{{AFX_MSG_MAP(CChartDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
//	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_RESTORE, OnRestore)
	ON_BN_CLICKED(IDC_TOIMPLIED, OnToimplied)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartDlg message handlers

BEGIN_EVENTSINK_MAP(CChartDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CChartDlg)
	ON_EVENT(CChartDlg, IDC_TS_CHART, -606 /* MouseMove */, OnMouseMoveTsChart, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CChartDlg, IDC_TS_CHART, 2 /* ModifyStart */, OnModifyStartTsChart, VTS_PBOOL)
	ON_EVENT(CChartDlg, IDC_TS_CHART, 1 /* ModifyEnd */, OnModifyEndTsChart, VTS_NONE)
	ON_EVENT(CChartDlg, IDC_TS_CHART, -605 /* MouseDown */, OnMouseDownTsChart, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CChartDlg, IDC_TS_CHART, -607 /* MouseUp */, OnMouseUpTsChart, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CChartDlg, IDC_TS_CHART, -601 /* DblClick */, OnDblClickTsChart, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CChartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitChart();

	m_wndPopupText.Create (&m_ctlChart);

	m_btFitToImplied.SetFlatLook(TRUE);
	m_btUpdate.SetFlatLook(TRUE);
	m_btRestore.SetFlatLook(TRUE);

	// Calc control area height
	CRect rc;
	m_btFitToImplied.GetWindowRect( &rc );
	m_nControlAreaHeight = rc.Height() + m_nOffset; 

#ifdef _VOLA_MANAGER
	m_bManageableVola = TRUE;
#else
	m_bManageableVola = FALSE;
#endif //!_VOLA_MANAGER
	SetManageableVola( m_bManageableVola );

#ifdef _VOLA_MANAGER

	EnableVolaManagement( false, false );
	SetIsDirty( false );
#endif //_VOLA_MANAGER

/*
//	Transparency

	HWND hw = *GetParent();
	SetWindowLong(hw, GWL_EXSTYLE,
        GetWindowLong(hw, GWL_EXSTYLE) | WS_EX_LAYERED);
	// Make this window 70% alpha
	SetLayeredWindowAttributes(hw, 0, 200, LWA_ALPHA);
*/
	return TRUE;
}


void CChartDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CDialog::OnGetMinMaxInfo( lpMMI );

#ifdef _VOLA_MANAGER
	if( ::IsWindow( m_hWnd ) )
		{
		CRect rcSS;
		m_btFitToImplied.GetWindowRect( &rcSS );	// Assume left most and bottom most control
		ScreenToClient( &rcSS );

		CRect rcR;
		m_btRestore.GetWindowRect( &rcR );	// Assume right most control
		ScreenToClient( &rcR );

		CRect rcChrt;
		m_ctlChart.GetWindowRect( &rcChrt );	// Assume top most control
		ScreenToClient( &rcChrt );

		lpMMI->ptMinTrackSize.x = rcR.right + rcSS.left;
		lpMMI->ptMinTrackSize.y = rcSS.Height() * 7 + rcSS.left * 2;
		}
#endif // _VOLA_MANAGER
}


void CChartDlg::OnDestroy() 
{
	m_wndPopupText.DestroyWindow();

	CDialog::OnDestroy();

	m_lblInfoIndicator.DetachDispatch();
	m_lblInfo.DetachDispatch();
	m_lblVolatility.DetachDispatch();
//	m_lblLogo.DetachDispatch();

	m_AxisX.DetachDispatch();
	m_AxisY.DetachDispatch();
	m_AxisY2.DetachDispatch();
	m_cGroup1.DetachDispatch();
	m_cGroup2.DetachDispatch();
}


void CChartDlg::SetManageableVola( bool bEnable )
{
	m_bManageableVola = bEnable;
	
	if (IsWindow(m_hWnd))
	{
		if (IsWindow (m_ctlChart) ) // check that controls have been created
		{
			m_btUpdate.ShowWindow( m_bManageableVola ? SW_SHOW : SW_HIDE );
			m_btRestore.ShowWindow( m_bManageableVola ? SW_SHOW : SW_HIDE );
			m_btFitToImplied.ShowWindow( m_bManageableVola ? SW_SHOW : SW_HIDE );
		}

		ResizeControls();
	}
}

#ifdef _VOLA_MANAGER

void CChartDlg::SetIsDirty( bool bIsDirty )
{
	m_bIsDirty = bIsDirty;
/*
	if( !m_bIsDirty )
		{
		m_btUpdate.EnableWindow( FALSE );
		m_btRestore.EnableWindow( FALSE );
		}
	else
		{
*/
		m_btUpdate.EnableWindow( TRUE );
		m_btRestore.EnableWindow( TRUE );
//		}
}


void CChartDlg::EnableVolaManagement( bool bManagement, bool bFitToImplied )
{
	m_bVolaManagementEnabled = bManagement;
	m_bFitToImpliedEnabled	 = bFitToImplied;

	if( m_bVolaManagementEnabled && m_bFitToImpliedEnabled )
		m_btFitToImplied.EnableWindow( TRUE );
	else
		m_btFitToImplied.EnableWindow( FALSE );
}
#endif //_VOLA_MANAGER


void CChartDlg::SetChartIsBatched(BOOL bIsBatched )
{
	if(bIsBatched)
	{
		if(0 == m_nIsBatchedCount)
		{
			m_ctlChart.SetIsBatched(TRUE);
			m_cGroup1.GetData().SetIsBatched(TRUE);
			m_cGroup2.GetData().SetIsBatched(TRUE);
		}
		m_nIsBatchedCount++;
	}
	else if(m_nIsBatchedCount > 0)
	{
		m_nIsBatchedCount--;
		if(0 == m_nIsBatchedCount)
		{
			m_cGroup1.GetData().SetIsBatched(FALSE);
			m_cGroup2.GetData().SetIsBatched(FALSE);
			m_ctlChart.SetIsBatched(FALSE);
		}
	}
}


void CChartDlg::InitChart()
{
	COLORREF  clrBack = RGB (255, 255, 255);//::GetSysColor(COLOR_3DFACE);
	COLORREF  clrHILO = RGB (255, 255, 255);
	COLORREF  clrArea = RGB (240, 240, 240);//clrBack;
	COLORREF  clrVola = RGB (51, 51, 153);	
	COLORREF  clrMid  = RGB (204, 153, 0);
	COLORREF clrBorder = RGB(153, 153, 153);
	
	CChartStyle chStyle;

	m_AxisX = m_ctlChart.GetChartArea().GetAxes().GetItem(COleVariant(_T("x")));
	m_AxisY = m_ctlChart.GetChartArea().GetAxes().GetItem(COleVariant(_T("y")));
	m_AxisY2 = m_ctlChart.GetChartArea().GetAxes().GetItem(COleVariant(_T("y2")));
	m_cGroup1 = m_ctlChart.GetChartGroups().GetItem(COleVariant(short(1)));
	m_cGroup2 = m_ctlChart.GetChartGroups().GetItem(COleVariant(short(2)));

	SetChartIsBatched(TRUE);

	m_ctlChart.SetAllowUserChanges(FALSE);
	m_ctlChart.GetChartLabels().RemoveAll();

	COleCurrency cySize(10, 0);

	//Setup logo label
/*	m_lblLogo = m_ctlChart.GetChartLabels().Add();
	m_lblLogo.SetIsBatched(TRUE);
	m_lblLogo.SetIsShowing(FALSE);

	m_lblLogo.GetBorder().SetType(oc2dBorderNone);
	m_lblLogo.SetOffset(4);
	m_lblLogo.SetAnchor(oc2dAnchorSouthWest);
	m_lblLogo.SetAttachMethod(oc2dAttachCoord);
	m_lblLogo.GetInterior().SetBackgroundColor(clrArea);
	m_lblLogo.GetInterior().SetForegroundColor(XrtColorWhite);
	m_lblLogo.SetAdjust(oc2dAdjustRight);
	m_lblLogo.GetText().SetText( CString ( (LPCSTR) IDS_CHART_LOGO ));
	m_lblLogo.SetIsBatched(FALSE);
*/
	// setup floating label
	m_lblVolatility = m_ctlChart.GetChartLabels().Add();
	m_lblVolatility.SetIsBatched(TRUE);
	m_lblVolatility.SetIsShowing(FALSE);
	m_lblVolatility.GetBorder().SetType(oc2dBorderPlain);
	m_lblVolatility.GetBorder().SetWidth(1);
	m_lblVolatility.SetAttachMethod(oc2dAttachDataCoord);
	m_lblVolatility.GetInterior().SetBackgroundColor(::GetSysColor(COLOR_INFOBK));
	m_lblVolatility.GetInterior().SetForegroundColor(::GetSysColor(COLOR_INFOTEXT));
	m_lblVolatility.SetAnchor(oc2dAnchorSouthEast);
	m_lblVolatility.SetAdjust(oc2dAdjustLeft);
	m_lblVolatility.SetOffset(10);
	m_lblVolatility.SetIsBatched(FALSE);

	// setup information indicator label
	m_lblInfoIndicator = m_ctlChart.GetChartLabels().Add();
	m_lblInfoIndicator.SetIsBatched(TRUE);
	m_lblInfoIndicator.SetIsShowing(FALSE);
	m_lblInfoIndicator.GetBorder().SetType(oc2dBorderPlain);
	m_lblInfoIndicator.GetBorder().SetWidth(1);
	m_lblInfoIndicator.SetAttachMethod(oc2dAttachCoord);
	m_lblInfoIndicator.GetAttachCoord().SetX(0);
	m_lblInfoIndicator.GetAttachCoord().SetY(0);
	m_lblInfoIndicator.GetInterior().SetBackgroundColor(XrtColorYellow);
	m_lblInfoIndicator.GetInterior().SetForegroundColor(XrtColorBlack);
	m_lblInfoIndicator.SetAnchor(oc2dAnchorSouthEast);
	m_lblInfoIndicator.SetAdjust(oc2dAdjustCenter);
	m_lblInfoIndicator.SetOffset(4);
	m_lblInfoIndicator.GetFont().SetBold(TRUE);
	//m_lblInfoIndicator.GetFont().SetItalic(TRUE);
	m_lblInfoIndicator.GetFont().SetSize(cySize);
	m_lblInfoIndicator.GetText().SetText(_T(" ! "));
	m_lblInfoIndicator.SetIsBatched(FALSE);

	// setup information label
	m_lblInfo = m_ctlChart.GetChartLabels().Add();
	m_lblInfo.SetIsBatched(TRUE);
	m_lblInfo.SetIsShowing(FALSE);
	m_lblInfo.GetBorder().SetType(oc2dBorderPlain);
	m_lblInfo.GetBorder().SetWidth(1);
	m_lblInfo.SetAttachMethod(oc2dAttachCoord);
	m_lblInfo.GetAttachCoord().SetX(0);
	m_lblInfo.GetAttachCoord().SetY(0);
	m_lblInfo.GetInterior().SetBackgroundColor(::GetSysColor(COLOR_INFOBK));
	m_lblInfo.GetInterior().SetForegroundColor(::GetSysColor(COLOR_INFOTEXT));
	m_lblInfo.SetAnchor(oc2dAnchorSouthEast);
	m_lblInfo.SetAdjust(oc2dAdjustLeft);
	m_lblInfo.SetOffset(10);
	m_lblInfo.SetIsConnected(TRUE);
	m_lblInfo.SetIsBatched(FALSE);
	
	// setup chart appearance

	m_ctlChart.GetInterior().SetBackgroundColor(clrBack);
    m_ctlChart.GetInterior().SetForegroundColor(/*XrtColorBlack*/::GetSysColor(COLOR_BTNSHADOW));
	m_ctlChart.GetChartArea().GetInterior().SetBackgroundColor(clrBack);
	
/*	m_ctlChart.GetInterior().SetBackgroundColor(XrtColorAliceBlue);
    m_ctlChart.GetInterior().SetForegroundColor(XrtColorBlack);
    
	m_ctlChart.GetChartArea().GetInterior().SetBackgroundColor(XrtColorAliceBlue);
*/	
	m_ctlChart.GetChartArea().SetIsShowingOutlines(TRUE);
	
	m_ctlChart.GetChartArea().GetLocation().GetLeft().SetValue(0L);
	m_ctlChart.GetChartArea().GetLocation().GetTop().SetValue(0L);

	// Remove actions
	CActionMapCollection actions = m_ctlChart.GetActionMaps();
	actions.Remove( WM_MBUTTONDOWN, MK_MBUTTON, 0 );
	actions.Remove( WM_MBUTTONDOWN, MK_CONTROL, 0 );
	actions.Remove( WM_LBUTTONDOWN, MK_CONTROL, 0 );
	actions.Remove( WM_MBUTTONDOWN, MK_SHIFT, 0 );
	actions.Remove( WM_LBUTTONDOWN, MK_SHIFT, 0 );
	actions.Remove( WM_MOUSEMOVE, MK_MBUTTON, 0 );
	actions.Remove( WM_MOUSEMOVE, MK_MBUTTON+MK_SHIFT, 0 );
	actions.Remove( WM_MOUSEMOVE, MK_MBUTTON+MK_CONTROL, 0 );
	actions.Remove( WM_MOUSEMOVE, MK_LBUTTON+MK_SHIFT, 0 );
	actions.Remove( WM_MOUSEMOVE, MK_LBUTTON+MK_CONTROL, 0 );
	
	CPlotArea area = m_ctlChart.GetChartArea().GetPlotArea();
	area.SetIsBoxed(TRUE);

	area.GetInterior().SetBackgroundColor(clrArea);
	area.GetInterior().SetForegroundColor(clrArea);

	m_AxisX.GetAxisStyle().GetLineStyle().SetPattern(oc2dLineSolid);
	m_AxisX.GetAxisStyle().GetLineStyle().SetColor(XrtColorBlack);
	m_AxisX.GetAxisStyle().GetLineStyle().SetWidth(1);

	m_AxisX.SetAnnotationMethod(oc2dAnnotateValueLabels);

	//m_AxisX.SetAnnotationRotation(oc2dRotate90Degrees);
	//m_AxisX.SetAnnotationRotationAngle(45.);
	
	m_AxisY.GetAxisStyle().GetLineStyle().SetPattern(oc2dLineSolid);
	m_AxisY.GetAxisStyle().GetLineStyle().SetColor(XrtColorBlack);
	m_AxisY.GetAxisStyle().GetLineStyle().SetWidth(1);
	m_AxisY.SetOriginPlacement(oc2dOriginMinimum);

	m_AxisY.SetTitleRotation(oc2dRotate90Degrees);

	m_AxisY2.GetAxisStyle().GetLineStyle().SetPattern(oc2dLineSolid);
	m_AxisY2.GetAxisStyle().GetLineStyle().SetColor(XrtColorBlack);
	m_AxisY2.GetAxisStyle().GetLineStyle().SetWidth(1);
	m_AxisY2.SetMultiplier(1.);
	m_AxisY2.SetIsShowing(TRUE);

	m_ctlChart.GetBorder().SetType(oc2dBorderPlain);
 	m_ctlChart.GetBorder().SetWidth(1);

	// setup historical part
	m_cGroup1.SetDrawingOrder(2);
	m_cGroup1.SetChartType(oc2dTypeArea);
	m_cGroup1.GetData().SetLayout(oc2dDataGeneral);
	m_cGroup1.GetData().SetNumSeries(3);
	m_cGroup1.GetStyles().RemoveAll();

	// back chart style (for better view)


	chStyle = m_cGroup1.GetStyles().Add(COleVariant(g_lBack));
	chStyle.GetFill().GetInterior().SetBackgroundColor(clrArea);
	chStyle.GetFill().GetInterior().SetForegroundColor(clrArea);
	chStyle.GetFill().SetPattern(oc2dFillSolid);
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(1);
	chStyle.GetLine().SetColor(clrArea);
	chStyle.GetSymbol().SetShape(oc2dShapeNone);


	// Hist vola HI
	chStyle = m_cGroup1.GetStyles().Add(COleVariant(g_lHistVolaHi));
	
//	chStyle.GetFill().GetInterior().SetBackgroundColor(clrBack);
//	chStyle.GetFill().GetInterior().SetForegroundColor(clrBack);
	
	chStyle.GetFill().GetInterior().SetBackgroundColor(clrHILO);
	chStyle.GetFill().GetInterior().SetForegroundColor(clrHILO);
	
	
	chStyle.GetFill().SetPattern(oc2dFillSolid);
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(1);
	chStyle.GetLine().SetColor(clrHILO);
	chStyle.GetSymbol().SetShape(oc2dShapeNone);
	
	// Hist vola LO
	chStyle = m_cGroup1.GetStyles().Add(COleVariant(g_lHistVolaLo));
	chStyle.GetFill().GetInterior().SetBackgroundColor(clrArea);
	chStyle.GetFill().GetInterior().SetForegroundColor(clrArea);
	chStyle.GetFill().SetPattern(oc2dFillSolid);
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(1);
	chStyle.GetLine().SetColor(clrArea);
	
	chStyle.GetSymbol().SetShape(oc2dShapeNone);


	m_cGroup2.SetDrawingOrder(1);
	m_cGroup2.SetChartType(oc2dTypePlot);
	m_cGroup2.GetData().SetLayout(oc2dDataGeneral);
	m_cGroup2.GetData().SetNumSeries( 8 );
	m_cGroup2.GetStyles().RemoveAll();

	// Hist vola AVG
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lHistVolaAvg));
	chStyle.GetLine().SetPattern(oc2dLineLongDash);
	chStyle.GetLine().SetWidth(1);
	chStyle.GetLine().SetColor(clrMid);
	chStyle.GetSymbol().SetShape(oc2dShapeNone);//oc2dShapeDot);

	// Hist vola Max
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lHistVolaMax));
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(1);
	chStyle.GetLine().SetColor(XrtColorLightSkyBlue);
	chStyle.GetSymbol().SetShape(oc2dShapeNone);//oc2dShapeDot);

	// Hist vola Min
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lHistVolaMin));
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(1);
	chStyle.GetLine().SetColor(XrtColorDarkSeaGreen);
	chStyle.GetSymbol().SetShape(oc2dShapeNone);//oc2dShapeDot);

	//chStyle.GetSymbol().SetSize(7);
	//chStyle.GetSymbol().SetColor(XrtColorYellow);

	// setup HAL vola part
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lVola));
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(2);
	chStyle.GetLine().SetColor(XrtColorGray50);
	chStyle.GetSymbol().SetShape(oc2dShapeDot);
	chStyle.GetSymbol().SetSize(5);
	chStyle.GetSymbol().SetColor(XrtColorGray70);

	// Custom vola prev
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lCustomVolaPrev));
	chStyle.GetLine().SetPattern(oc2dLineLongDash);
	chStyle.GetLine().SetWidth(2);
	chStyle.GetLine().SetColor(XrtColorGray60);
	chStyle.GetSymbol().SetShape(oc2dShapeNone);//oc2dShapeDot);

	// Custom vola  
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lCustomVola));
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(3);
	chStyle.GetLine().SetColor(XrtColorBlue);
	chStyle.GetSymbol().SetShape(oc2dShapeDot);
	chStyle.GetSymbol().SetSize(4);
	chStyle.GetSymbol().SetColor(XrtColorCadetBlue);

	// Custom vola base points
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lCustomVolaBasePoints));
	chStyle.GetLine().SetPattern(oc2dLineNone);
	chStyle.GetLine().SetWidth(2);
	chStyle.GetLine().SetColor(XrtColorDarkGreen);
	chStyle.GetSymbol().SetShape(oc2dShapeDot);
	chStyle.GetSymbol().SetSize(4);
	chStyle.GetSymbol().SetColor(XrtColorGreenYellow);

	// Custom vola ATM
	chStyle = m_cGroup2.GetStyles().Add(COleVariant(g_lCustomVolaATM));
	chStyle.GetLine().SetPattern(oc2dLineSolid);
	chStyle.GetLine().SetWidth(2);
	chStyle.GetLine().SetColor(XrtColorCornflowerBlue);
	chStyle.GetSymbol().SetShape(oc2dShapeDot);
	chStyle.GetSymbol().SetSize(6);
	chStyle.GetSymbol().SetColor(XrtColorCoral);

	// setup legend
	m_ctlChart.GetLegend().SetIsShowing(FALSE);
	m_ctlChart.GetLegend().GetBorder().SetType(oc2dBorderNone);
	//m_ctlChart.GetLegend().GetBorder().SetWidth(1);
	m_ctlChart.GetLegend().GetInterior().SetBackgroundColor(XrtColorAliceBlue);//XrtColorGainsboro);
	m_ctlChart.GetLegend().SetAnchor(oc2dAnchorSouth);
	m_ctlChart.GetLegend().SetOrientation(oc2dOrientHorizontal);

	m_cGroup1.GetSeriesLabels().Add(_T(""), COleVariant(g_lBack));
	m_cGroup1.GetSeriesLabels().Add(_T("Hist VHL"), COleVariant(g_lHistVolaHi));
	m_cGroup2.GetSeriesLabels().Add(_T("Hist VA"), COleVariant(g_lHistVolaAvg));
	m_cGroup2.GetSeriesLabels().Add(_T("Implied VOL"), COleVariant(g_lVola));
	m_cGroup2.GetSeriesLabels().Add(_T("VOL"), COleVariant(g_lCustomVola));
	m_cGroup2.GetSeriesLabels().Add(_T("Previous VOL"), COleVariant(g_lCustomVolaPrev));

	m_cGroup1.GetData().SetNumPoints(g_lBack, 0);
	m_cGroup1.GetData().SetNumPoints(g_lHistVolaHi, 0);
	m_cGroup1.GetData().SetNumPoints(g_lHistVolaLo, 0);

	m_cGroup2.GetData().SetNumPoints(g_lHistVolaAvg, 0);
	m_cGroup2.GetData().SetNumPoints(g_lVola, 0);
	m_cGroup2.GetData().SetNumPoints(g_lCustomVola, 0);
	m_cGroup2.GetData().SetNumPoints(g_lCustomVolaPrev, 0);
	m_cGroup2.GetData().SetNumPoints(g_lCustomVolaATM, 0);
	m_cGroup2.GetData().SetNumPoints(g_lCustomVolaBasePoints, 0);

	m_cGroup2.GetData().SetNumPoints(g_lHistVolaMax, 0);
	m_cGroup2.GetData().SetNumPoints(g_lHistVolaMin, 0);

	SetChartIsBatched(FALSE);
}


void CChartDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	ResizeControls();	
}


void CChartDlg::AdjustControlArea( int nOffsetY )
{
	CRect rc;
	if( ::IsWindow( m_btFitToImplied ) )
	{
		m_btFitToImplied.GetWindowRect( &rc );
		ScreenToClient( &rc );
		m_btFitToImplied.SetWindowPos( NULL, rc.left, nOffsetY, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER );
	}

	if( ::IsWindow( m_btUpdate ) )
	{
		m_btUpdate.GetWindowRect( &rc );
		ScreenToClient( &rc );
		m_btUpdate.SetWindowPos( NULL, rc.left, nOffsetY, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER );
	}

	if( ::IsWindow( m_btRestore ) )
	{
		m_btRestore.GetWindowRect( &rc );
		ScreenToClient( &rc );
		m_btRestore.SetWindowPos( NULL, rc.left, nOffsetY, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER );
	}
}

	
long CChartDlg::GetVolaPointIdxByCoord(int x, int y, long *pSeries)
{
	ASSERT( pSeries );

	long lSeries, lPoint, lDistance;
	long ptX, ptY, lPoints, i;

	if( m_cGroup2.CoordToDataIndex( x, y, oc2dFocusXY, &lSeries, &lPoint, &lDistance ) == oc2dRegionInChartArea )
		{
		if( m_bManageableVola )
			{
			if( g_lCustomVola == lSeries && lDistance < 5 )
				{
				*pSeries = g_lCustomVola;
				return lPoint;
				}

			lPoints = m_cGroup2.GetData().GetNumPoints( g_lCustomVola );
			for( i = 1L; i < lPoints + 1L; i++ )
				{
				m_cGroup2.DataIndexToCoord( g_lCustomVola, i, &ptX, &ptY );
				if( labs(x - ptX) < 5 && labs(y - ptY) < 5 )
					{
					*pSeries = g_lCustomVola;
					return i;
					}
				}
			}

		if( g_lVola == lSeries && lDistance < 5 )
			{
			*pSeries = g_lVola;
			return lPoint;
			}

		lPoints = m_cGroup2.GetData().GetNumPoints( g_lVola );
		for( i = 1L; i < lPoints + 1L; i++ )
			{
			m_cGroup2.DataIndexToCoord( g_lVola, i, &ptX, &ptY );
			if( labs(x - ptX) < 5 && labs(y - ptY) < 5 )
				{
				*pSeries = g_lVola;
				return i;
				}
			}
		}

	return 0L;
}


long CChartDlg::GetBasePointIdxByCoord(int x, int y)
{
	long lSeries, lPoint, lDistance;
	long ptX, ptY, lPoints, i;

	if( m_cGroup2.CoordToDataIndex( x, y, oc2dFocusXY, &lSeries, &lPoint, &lDistance ) == oc2dRegionInChartArea )
	{
		if( m_bManageableVola )
		{
			if( g_lCustomVolaBasePoints == lSeries && lDistance < 5 )
				return lPoint;

			lPoints = m_cGroup2.GetData().GetNumPoints( g_lCustomVolaBasePoints );
			for( i = 1L; i < lPoints + 1L; i++ )
			{
				m_cGroup2.DataIndexToCoord( g_lCustomVolaBasePoints, i, &ptX, &ptY );
				if( labs(x - ptX) < 5 && labs(y - ptY) < 5 )
					return i;
			}
		}
	}

	return 0L;
}


void CChartDlg::SetTitle(LPCTSTR szTitle)
{
	CWnd * pWnd = GetParent ();
	pWnd->SetWindowText (szTitle);
}


void CChartDlg::OnModifyStartTsChart(BOOL FAR* IsOK) 
{
	m_bChartModify = m_bManageableVola ? true : false;
	*IsOK = FALSE;
}


void CChartDlg::OnModifyEndTsChart() 
{
	m_bChartModify = false;
}


void CChartDlg::OnMouseDownTsChart(short Button, short Shift, long X, long Y) 
{
#ifdef _VOLA_MANAGER
	if( m_bManageableVola && m_bVolaManagementEnabled && (1 == Button || 2 == Button) &&
		m_cGroup2.GetData().GetNumPoints( g_lCustomVola ) > 0
	  )
		{
		long lSeries;
		m_lMovingPtIdx = GetVolaPointIdxByCoord( X, Y, &lSeries );
		m_lMovingPtIdx = (lSeries == g_lCustomVola) ? m_lMovingPtIdx : 0;

		if( m_lMovingPtIdx != 0 )
		{
			SavePointValue( m_lMovingPtIdx );
			m_lMovingBasePtIdx = GetBasePointIdxByCoord( X, Y );
		}

		m_nShifted = Shift;
		}
#endif //_VOLA_MANAGER
}


void CChartDlg::OnMouseMoveTsChart(short Button, short Shift, long X, long Y) 
{
	bool bVolaInPercent = true;

	long   lSeries = g_lCustomVola;
	size_t lPtIdx = 0;
	
	if( m_lMovingPtIdx > 0 )
	{
		double dX, dY;
		m_cGroup2.CoordToDataCoord( X, Y, &dX, &dY );
		dY = dY < 0 ? 0 : dY;

		lPtIdx = m_lMovingPtIdx;
		if( Button == 1 && dY != m_cGroup2.GetData().GetHoleValue() )
		{
			SetChartIsBatched(TRUE);

			if( !m_nShifted )
			{
				m_cGroup2.GetData().SetY(g_lCustomVola, m_lMovingPtIdx, dY);
				if( m_lMovingPtIdx == m_lATMPointIdx )
					m_cGroup2.GetData().SetY(g_lCustomVolaATM, 1, dY);
				if( m_lMovingBasePtIdx != 0 )
					m_cGroup2.GetData().SetY(g_lCustomVolaBasePoints, m_lMovingBasePtIdx, dY);
			}
			else	// Some shift is enabled
			{
				long idx;

				//  Get moovin range
				long nFirstMovinPoint = 0, nLastMovinPoint = 0;
				if( m_nShifted == 1 )		// Shift - movin' entire curve
				{
					nFirstMovinPoint = 1;
					nLastMovinPoint  = m_cGroup2.GetData().GetNumPoints( g_lCustomVola );
				}
				else if( m_nShifted == 2 )	// Ctrl - movin' left part
				{
					nFirstMovinPoint = 1;
					nLastMovinPoint  = m_lMovingPtIdx;
				}
				else if( m_nShifted == 4 )	// ALt - movin' right part
				{
					nFirstMovinPoint = m_lMovingPtIdx;
					nLastMovinPoint  = m_cGroup2.GetData().GetNumPoints( g_lCustomVola );
				}

				// Get lower movin point
				double tmp, dMinY = m_cGroup2.GetData().GetY( g_lCustomVola, nFirstMovinPoint );
				for( idx = nFirstMovinPoint + 1; idx <= nLastMovinPoint; idx++ )
				{
					tmp = m_cGroup2.GetData().GetY( g_lCustomVola, idx );
					if( tmp < dMinY )
						dMinY = tmp;
				}
				
				// Get requested delta
				double delta = dY - m_cGroup2.GetData().GetY( g_lCustomVola, m_lMovingPtIdx );

				// Adjust delta
				if( dMinY + delta < 0 )
					delta = 0 - dMinY;

				// Move 
				for( idx = nFirstMovinPoint; idx <= nLastMovinPoint; idx++ )
				{
					dY = m_cGroup2.GetData().GetY( g_lCustomVola, idx );
					m_cGroup2.GetData().SetY(g_lCustomVola, idx, dY + delta );
					if( idx == m_lATMPointIdx )
						m_cGroup2.GetData().SetY(g_lCustomVolaATM, 1, dY + delta );
				}

				// Move base points marks
				double dFirtsStrike = m_cGroup2.GetData().GetX( g_lCustomVola, nFirstMovinPoint );
				double dLastStrike  = m_cGroup2.GetData().GetX( g_lCustomVola, nLastMovinPoint );
				for( idx = 1; idx <= m_cGroup2.GetData().GetNumPoints( g_lCustomVolaBasePoints ); idx++ )
				{
					double dStrike = m_cGroup2.GetData().GetX( g_lCustomVolaBasePoints, idx );
					if( dStrike < dFirtsStrike )
						continue;
					if( dStrike > dLastStrike )
						break;

					dY = m_cGroup2.GetData().GetY( g_lCustomVolaBasePoints, idx );
					m_cGroup2.GetData().SetY( g_lCustomVolaBasePoints, idx, dY + delta );
				}
			}
			
			SetChartIsBatched(FALSE);
		}
	}

	if( lPtIdx == 0 )
		lPtIdx = GetVolaPointIdxByCoord(X, Y, &lSeries);

	if( lPtIdx > 0L && GetActiveWindow() == this->GetParent() && m_lMovingPtIdx == 0 )
	{
		m_lblVolatility.SetIsBatched(TRUE);

		double dY;
		if( lSeries == g_lVola )
			dY = m_cGroup2.GetData().GetY(lSeries, lPtIdx);
		else if( lSeries == g_lCustomVola || lSeries == g_lCustomVolaBasePoints || lSeries == g_lCustomVolaATM )
			dY = GetPointValue( lPtIdx );
			
		m_lblVolatility.GetAttachDataCoord().SetX(m_cGroup2.GetData().GetX(lSeries, lPtIdx));
		m_lblVolatility.GetAttachDataCoord().SetY(dY);
		CString str;
				
		if( lSeries == g_lVola	&& m_vecSymbols.size() >= lPtIdx )
		{						
			str += m_vecSymbols [lPtIdx-1].c_str();
		}
		if( lSeries == g_lVola && m_vecPrices.size() >= lPtIdx )
		{
			CString strPrice;
			strPrice.Format(_T(" [%.2lf]"),m_vecPrices[lPtIdx-1]);
			str += strPrice; 
		}
		if( !str.IsEmpty() )
			str += _T("\n");
		
		CString strTmp;
		if(bVolaInPercent)
			strTmp.Format(_T("V: %.2f%%"),dY);
		else
			strTmp.Format(_T("V: %.4f"), dY);
		
		str += strTmp;
		
		CString strLabel = GetXLabel(lSeries, lPtIdx);
		
		if (! strLabel.IsEmpty())
		{
			str += _T(' ');
			str += strLabel;
		}

		if( lSeries == g_lVola && m_vecDates.size() >= lPtIdx )
		{
			COleDateTime dt = m_vecDates [lPtIdx-1];
			
			str += _T("\nD: ");
			str += dt.Format();
		}
		
		m_wndPopupText.SetWindowText(str);

		long ptXNew = 0, ptYNew = 0;
		m_cGroup2.DataIndexToCoord( lSeries, lPtIdx, &ptXNew, &ptYNew );

		CPoint pt (ptXNew, ptYNew);
		m_ctlChart.ClientToScreen(&pt);

		CSize szCursor;
		szCursor.cx = GetSystemMetrics (SM_CXCURSOR) / 2;
		szCursor.cy = GetSystemMetrics (SM_CYCURSOR) / 2;
		
		m_wndPopupText.Show (pt +  szCursor);

		m_lblVolatility.SetIsBatched(FALSE);
	}
	else
	{
		//m_lblVolatility.SetIsShowing(FALSE);
		m_wndPopupText.Hide();
		
		CRect rect(CPoint(m_lblInfoIndicator.GetLocation().GetLeft().GetValue(),
					m_lblInfoIndicator.GetLocation().GetTop().GetValue()),
					CSize(m_lblInfoIndicator.GetLocation().GetWidth().GetValue(),
						m_lblInfoIndicator.GetLocation().GetHeight().GetValue()));
		m_lblInfo.GetAttachCoord().SetX(rect.right - 1);
		m_lblInfo.GetAttachCoord().SetY(rect.bottom - 1);
		m_lblInfo.SetIsShowing(rect.PtInRect(CPoint(X, Y)));
	}


}


void CChartDlg::OnMouseUpTsChart(short Button, short Shift, long X, long Y) 
{
#ifdef _VOLA_MANAGER
	if( m_lMovingPtIdx > 0 )
	{
		double dX, dY;
		m_cGroup2.CoordToDataCoord( X, Y, &dX, &dY );
		dY = dY < 0 || dY == m_cGroup2.GetData().GetHoleValue() ? 0 : dY;

		bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
		long lVolaMultiplier = bVolaInPercent ? 100L : 1L;
		
		double dtMonth;
		bool bUpdated = false;
		if( m_nShifted )
		{
			for( long idx = 1; idx <= m_cGroup2.GetData().GetNumPoints( g_lCustomVola ); idx++ )
			{
				dY = m_cGroup2.GetData().GetY( g_lCustomVola, idx );
				UpdateCustomVolaPoint( idx, dY / lVolaMultiplier, dtMonth );
			}

			bUpdated = true;
		}
		else
		{
			bUpdated = UpdateCustomVolaPoint( m_lMovingPtIdx, dY / lVolaMultiplier, dtMonth );
		}

		m_lMovingPtIdx	   = 0;
		m_lMovingBasePtIdx = 0;
		m_nShifted		   = 0;

		SetIsDirty( true );
		UpdateChart( false );
		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, dtMonth );
	}
#endif //_VOLA_MANAGER
}


void CChartDlg::OnDblClickTsChart() 
{
#ifdef _VOLA_MANAGER
	if( m_bManageableVola &&
		m_cGroup2.GetData().GetNumPoints( g_lCustomVola ) > 0
	  )
	{
		POINT pt;
		GetCursorPos( &pt );
		m_ctlChart.ScreenToClient( &pt );

		long lSeries;
		long lPtIdx = GetVolaPointIdxByCoord( pt.x, pt.y, &lSeries );
			 lPtIdx = (lSeries == g_lCustomVola) ? lPtIdx : 0;

		if( lPtIdx == 0 )
			return;

		m_lblVolatility.SetIsShowing(FALSE);

		CVolaEditorDlg ve( this );
		SetupVolaEditor( lPtIdx, ve );
		if( ve.DoModal() == IDOK )
		{
			bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
			long lVolaMultiplier = bVolaInPercent ? 100L : 1L;
			
			double dtMonth;
			if( UpdateCustomVolaPoint( lPtIdx, ve.m_dNewVola / lVolaMultiplier, dtMonth ) )
			{
				SetIsDirty( TRUE );
				UpdateChart( false );
				PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, dtMonth );
			}
		}
	}
#endif //_VOLA_MANAGER
}


void CChartDlg::OnUpdate() 
{
#ifdef _VOLA_MANAGER
	CWaitCursor wait;

	UpdateCustomVola();

	SetIsDirty( FALSE );
	UpdateChart( false );
#endif //_VOLA_MANAGER
}


void CChartDlg::OnRestore() 
{
#ifdef _VOLA_MANAGER
	CWaitCursor wait;

	RestoreCustomVola();
	
	SetIsDirty( FALSE );
	UpdateChart( false );
#endif //_VOLA_MANAGER
}


void CChartDlg::OnToimplied() 
{
#ifdef _VOLA_MANAGER
	CWaitCursor wait;

	InterpolateCustomVolaToImplied();

	SetIsDirty( TRUE );
	UpdateChart( false );
#endif //_VOLA_MANAGER
}

void CChartDlg::ClearSeries( bool bPreserveCustomVola )
{
	m_cGroup1.GetData().SetNumPoints(g_lBack,		 0);
	m_cGroup1.GetData().SetNumPoints(g_lHistVolaHi,  0);
	m_cGroup1.GetData().SetNumPoints(g_lHistVolaLo,  0);
	m_cGroup2.GetData().SetNumPoints(g_lHistVolaAvg, 0);
	m_cGroup2.GetData().SetNumPoints(g_lHistVolaMin, 0);
	m_cGroup2.GetData().SetNumPoints(g_lHistVolaMax, 0);
	m_cGroup2.GetData().SetNumPoints(g_lVola,		 0);

	m_cGroup2.GetData().SetSeriesDisplay( g_lHistVolaHi,  oc2dDisplayHide );
	m_cGroup2.GetData().SetSeriesDisplay( g_lHistVolaLo,  oc2dDisplayHide );
	m_cGroup2.GetData().SetSeriesDisplay( g_lHistVolaAvg, oc2dDisplayHide );
	m_cGroup2.GetData().SetSeriesDisplay( g_lHistVolaMin, oc2dDisplayHide );
	m_cGroup2.GetData().SetSeriesDisplay( g_lHistVolaMax, oc2dDisplayHide );
	m_cGroup2.GetData().SetSeriesDisplay( g_lVola,		  oc2dDisplayHide );

	if( !bPreserveCustomVola )
	{
		m_cGroup2.GetData().SetNumPoints(g_lCustomVola, 0);
		m_cGroup2.GetData().SetNumPoints(g_lCustomVolaATM, 0);
		m_cGroup2.GetData().SetNumPoints(g_lCustomVolaBasePoints, 0);

		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVola, oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaATM, oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaBasePoints, oc2dDisplayHide );
	}
	m_cGroup2.GetData().SetNumPoints( g_lCustomVolaPrev, 0 );
	m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaPrev, oc2dDisplayHide );

	m_vecDates.clear();
	m_vecSymbols.clear();
	m_vecPrices.clear();
}

void CChartDlg::AdjustBack(double dMinX, double dMaxX, double dMinY, double dMaxY)
{
	double dV1 = dMaxY - dMinY;
	
	if(dV1 < 0.05)
		dV1 = 0.05;
	dMinY = dMinY - (10. * dV1 / 100.);
	dMaxY = dMaxY + (10. * dV1 / 100.);
	if(dMinY < 0.) dMinY = 0.;

	m_cGroup1.GetData().SetNumPoints(g_lBack, 3);
	m_cGroup1.GetData().SetX(g_lBack, 1, dMinX);
	m_cGroup1.GetData().SetY(g_lBack, 1, dMinY * m_lVolaMultiplier);

	m_cGroup1.GetData().SetX(g_lBack, 2, dMinX);
	m_cGroup1.GetData().SetY(g_lBack, 2, dMaxY * m_lVolaMultiplier);
	
	m_cGroup1.GetData().SetX(g_lBack, 3, dMaxX);
	m_cGroup1.GetData().SetY(g_lBack, 3, dMaxY * m_lVolaMultiplier);
}

void CChartDlg::ResizeControls()
{
	CRect rc;
	GetClientRect (rc);
	
	if( ::IsWindow( m_ctlChart ) )
	{
		CRect rect;
		m_ctlChart.GetWindowRect( &rect );
		ScreenToClient( rect );

		int nWidth  = rc.Width() - rect.left * 2;
		int nHeight = rc.Height() - rect.top  * 2;

		if( m_bManageableVola )
		{
			nHeight =  rc.Height() - ( m_nControlAreaHeight + rect.top );
			
			AdjustControlArea( nHeight + m_nOffset );
		}

		m_ctlChart.MoveWindow(rect.left, rect.top, nWidth, nHeight );
		
		m_ctlChart.GetChartArea().GetLocation().GetWidth().SetValue(nWidth);
		m_ctlChart.GetChartArea().GetLocation().GetHeight().SetValue(nHeight);
		
		AdjustLogo();
	}
}

BOOL CChartDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->hwnd == m_wndPopupText.m_hWnd && 
		pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
	{
		pMsg->hwnd = m_hWnd;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

CString CChartDlg::GetCurveModeTitle()
{
	CString strName;
	switch ( g_DataProcessor.GetCurveMode() )
	{
	case enCall:
		strName = _T("Call");
		break;
	case enPut:
		strName = _T("Put");
		break;
	case enOTM:
		strName = _T("OTM");
		break;
	}

	CString strResult;
	strResult.Format (_T("(%s)"), (LPCTSTR) strName);

	return strResult;
}


BOOL CChartDlg::OnEraseBkgnd(CDC* pDC) 
{
	if( IsWindow( m_hWnd ) && IsWindow( m_ctlChart.m_hWnd ) )
	{
		CRect rcWnd, rcChart;

		GetWindowRect(  &rcWnd );
		ScreenToClient( &rcWnd );
		m_ctlChart.GetWindowRect(  &rcChart );
		ScreenToClient( &rcChart );
		
		CRect rcFill( rcWnd.left, rcChart.bottom, rcWnd.right, rcWnd.bottom );
		pDC->FillSolidRect( &rcFill, ::GetSysColor(COLOR_3DFACE) );

		return true;
	}

	return CDialog::OnEraseBkgnd( pDC );
}


#ifdef _VOLA_MANAGER
void CChartDlg::PostNotification( UINT nMessage, CCustomVolaNotification::Type enType, double dtMonth )
{
	CWnd *pNotificationWnd = AfxGetMainWnd();
	if( pNotificationWnd )
	{
		CCustomVolaNotification *pNotification = new CCustomVolaNotification( enType, dtMonth );
		pNotificationWnd->PostMessage( nMessage, (WPARAM)pNotification, (LPARAM)GetParent()->m_hWnd );
	}
}
#endif //_VOLA_MANAGER


BOOL CChartDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (nID == IDOK || nID == IDCANCEL)
		return FALSE;
	else
		return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	
}

void CChartDlg::AdjustLogo()
{
/*	CSize szLogo;
	szLogo.cx = m_ctlChart.GetChartArea().GetLocation().GetWidth().GetValue() - 
		m_ctlChart.GetChartArea().GetPlotArea().GetRight().GetValue() - 2;
	szLogo.cy = m_ctlChart.GetChartArea().GetPlotArea().GetTop().GetValue();
	
	m_lblLogo.SetIsBatched (TRUE);
	m_lblLogo.GetAttachCoord().SetX(szLogo.cx);
	m_lblLogo.GetAttachCoord().SetY(szLogo.cy);

	m_lblLogo.SetIsShowing(TRUE);
	m_lblLogo.SetIsBatched (FALSE);
*/
}

/*
BOOL CChartDlg::OnHelpInfo( HELPINFO* lpHelpInfo )
{
	return TRUE;
}
*/