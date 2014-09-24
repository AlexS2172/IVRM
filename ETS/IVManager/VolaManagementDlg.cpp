// VolaManagementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"

#include "VolaManagementDlg.h"
#include "VolaAddNewSurfaceDlg.h"
#include "VolaOpenSurfaceDlg.h"

#include "DataProcessor.h"
#include "RuleDefinitionDlg.h"
#include "DualQuadParams.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FACTOR_DECIMALDIGITCOUNT		1
#define FACTOR_FRACTIONALDIGITCOUNT		2
#define FACTOR_POSITIVE_FORMAT			_T("1.1")
#define FACTOR_NEGATIVE_FORMAT			_T("1.1")

#define PRICE_DECIMALDIGITCOUNT			5
#define PRICE_FRACTIONALDIGITCOUNT		2
#define PRICE_POSITIVE_FORMAT			_T("1.1")
#define PRICE_NEGATIVE_FORMAT			_T("1.1")

#define VOLA_PTS_DECIMALDIGITCOUNT		3
#define VOLA_PTS_FRACTIONALDIGITCOUNT	2
#define VOLA_PTS_POSITIVE_FORMAT		_T("1.1")
#define VOLA_PTS_NEGATIVE_FORMAT		_T("1.1")


/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CVolaManagementDlg, CDialog)
	//{{AFX_MSG_MAP(CVolaManagementDlg)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_FACTOR, OnChangeFactor)
	ON_CBN_SELCHANGE(IDC_COMBO_CURRENT_RULE, OnComboCurrentRuleSelchange)
	ON_BN_CLICKED(IDC_PRICE_OVERRIDE_CHECK, OnPriceOverrideCheck)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_UPDATE_RULE, OnUpdateRule)
	ON_BN_CLICKED(IDC_DISCRETE_ACCEL_CHECK, OnDiscreteAccelCheck)
	ON_EN_CHANGE(IDC_HI_VOLATILITY, OnChangeHiVolatility)
	ON_EN_CHANGE(IDC_LO_VOLATILITY, OnChangeLoVolatility)
	ON_BN_CLICKED(IDC_VOLA_REGENERATE, OnVolaRegenerate)
	ON_BN_CLICKED(IDC_VOLA_UPDATE, OnVolaUpdate)
	ON_BN_CLICKED(IDC_VOLA_RESTORE, OnVolaRestore)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_COMMAND(IDC_VM_MAKE_BASE_POINT, OnVmMakeBasePoint)
	ON_COMMAND(IDC_VM_MAKE_COMOMON_POINT, OnVmMakeComomonPoint)
	ON_COMMAND(IDC_VM_SURFACE_OPEN, OnVmSurfaceOpen)
	ON_COMMAND(IDC_VM_SURFACE_NEW, OnVmSurfaceNew)
	ON_COMMAND(IDC_VM_SURFACE_EDIT, OnVmSurfaceEdit)
	ON_COMMAND(IDC_VM_SURFACE_DELETE, OnVmSurfaceDelete)
	ON_COMMAND(IDC_VM_VIEW_GENERATION_OPTIONS, OnVmViewGenerationOptions)
	ON_COMMAND(IDC_VM_SELECT_ALL_EXP, OnVmSelectAll)
	ON_BN_CLICKED(IDC_NEW_RULE, OnNewRule)
	ON_BN_CLICKED(IDC_DELETE_RULE, OnDeleteRule)
	ON_COMMAND(IDC_VM_SURFACE_SET_AS_DEFAULT, OnVmSurfaceSetAsDefault)
	ON_EN_CHANGE(IDC_ACCELERATOR, OnChangeAccelerator)
	ON_WM_DESTROY()
	ON_COMMAND(IDC_VM_SURFACE_NEXTDAY, OnVmSurfaceNextday)
	ON_COMMAND(IDC_VM_VIEW_ALWAYSONTOP, OnVmViewAlwaysontop)
	ON_WM_SHOWWINDOW()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_VOLA_CLEAR, OnVolaClear)
	ON_BN_CLICKED(IDC_VOLA_FITTOIMPLIED, OnVolaFittoimplied)
	ON_COMMAND(IDC_VM_VIEW_SHOWALL, OnVmViewShowAll)
	ON_COMMAND(IDC_VM_SURFACE_SAVEAS, OnVmSurfaceSaveas)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_COMMAND(IDOK, OnOK)
	ON_BN_CLICKED(IDC_DUALQUADR_PAR, OnDualquadrPar)
	//}}AFX_MSG_MAP
	ON_CONTROL(EN_EX_COMMIT, IDC_UNDERLINE_PRICE, OnCommitUnderlinePrice )
	ON_MESSAGE(WM_CUSTOM_VOLA_UPDATE_VIEW, OnUpdateView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_EVENTSINK_MAP(CVolaManagementDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVolaManagementDlg)
	ON_EVENT(CVolaManagementDlg, IDC_VOLA, 22 /* StartEdit */, OnVolaStartEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CVolaManagementDlg, IDC_VOLA, 23 /* ValidateEdit */, OnVolaValidateEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CVolaManagementDlg, IDC_VOLA, 24 /* AfterEdit */, OnVolaAfterEdit, VTS_I4 VTS_I4)
	ON_EVENT(CVolaManagementDlg, IDC_VOLA, -604 /* KeyUp */, OnVolaKeyUp, VTS_PI2 VTS_I2)
	ON_EVENT(CVolaManagementDlg, IDC_VOLA, -607 /* MouseUp */, OnVolaMouseUp, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CVolaManagementDlg::CVolaManagementDlg(CWnd* pParent /*=NULL*/): 
		CDialog(CVolaManagementDlg::IDD, pParent),
		m_bInitialized(false),
		m_bNexdDaySurfaceMode(false),
		m_bAlwaysOnTop(true),
		m_bUseExtrapolation(true),
		m_nContractID(0),
		m_strTitle( _T("Volatility Surface") ),
		m_nCurrentRule(-1),
		m_nCurrentSurface(-1),
		m_dIFactor(0),
		m_nIFactor(0),
		m_bEditing(false),
		m_bGroupRuleSet(false),
		m_bContractRuleSet(false),
		m_nMinW(0), 
		m_nMinH(0),
		m_nMargine(0),
		m_nMouseRow(-1),
		m_nMouseCol(-1),
		m_bNeedSave(false)
{
	//{{AFX_DATA_INIT(CVolaManagementDlg)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolaManagementDlg)
	DDX_Control(pDX, IDC_DUALQUADR_PAR, m_btnDualQuadPar);
	DDX_Control(pDX, IDC_UPPER_LINE, m_stcUpperLine);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
	DDX_Control(pDX, IDC_DISCRETE_ACCEL_CHECK, m_chkDiscreteAccel);
	DDX_Control(pDX, IDC_PRICE_OVERRIDE_CHECK, m_chkPriceOverride);
	DDX_Control(pDX, IDC_LO_VOLATILITY, m_edtLoVoly);
	DDX_Control(pDX, IDC_HI_VOLATILITY, m_edtHiVoly);
	DDX_Control(pDX, IDC_UNDERLINE_PRICE, m_edtUnderlinePrice);
	DDX_Control(pDX, IDC_ACCELERATOR, m_edtAccelerator);
	DDX_Control(pDX, IDC_SLIDER_FACTOR, m_sldFactor);
	DDX_Control(pDX, IDC_FACTOR, m_edtFactor);
	DDX_Control(pDX, IDC_VOLA_FITTOIMPLIED, m_btnVolaFitToImplied);
	DDX_Control(pDX, IDC_VOLA_CLEAR, m_btnVolaClear);
	DDX_Control(pDX, IDC_VOLA_REGENERATE, m_btnVolaRegenerate);
	DDX_Control(pDX, IDC_VOLA_UPDATE,  m_btnVolaUpdate);
	DDX_Control(pDX, IDC_VOLA_RESTORE, m_btnVolaRestore);
	DDX_Control(pDX, IDC_NEW_RULE, m_btnNewRule);
	DDX_Control(pDX, IDC_UPDATE_RULE,  m_btnUpdateRule);
	DDX_Control(pDX, IDC_DELETE_RULE, m_btnDeleteRule);
	DDX_Control(pDX, IDC_COMBO_CURRENT_RULE,    m_cmbCurrentRule);
	DDX_Control(pDX, IDC_VOLA, m_fgVola);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
//
BOOL CVolaManagementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btnUpdateRule.SetFlatLook(TRUE);
	m_btnUpdateRule.LoadIcon(IDI_VMR_EDIT, FALSE, 16, 16);
	m_btnUpdateRule.SetVerticalAlignment(BS_VCENTER);
	m_btnUpdateRule.SetHorizontalAlignment(BS_CENTER);
	m_btnUpdateRule.SetToolTipText(_T("Modify Rule"));
	m_btnUpdateRule.SetToolTip(TRUE);

	m_btnNewRule.SetFlatLook(TRUE);
	m_btnNewRule.LoadIcon(IDI_VMR_NEW, FALSE, 16, 16);
	m_btnNewRule.SetVerticalAlignment(BS_VCENTER);
	m_btnNewRule.SetHorizontalAlignment(BS_CENTER);
	m_btnNewRule.SetToolTipText(_T("New Rule"));
	m_btnNewRule.SetToolTip(TRUE);

	m_btnDeleteRule.SetFlatLook(TRUE);
	m_btnDeleteRule.LoadIcon(IDI_VMR_DEL, FALSE, 16, 16);
	m_btnDeleteRule.SetVerticalAlignment(BS_VCENTER);
	m_btnDeleteRule.SetHorizontalAlignment(BS_CENTER);
	m_btnDeleteRule.SetToolTipText(_T("Delete Rule"));
	m_btnDeleteRule.SetToolTip(TRUE);

	m_btnDualQuadPar.SetFlatLook(TRUE);
	m_btnDualQuadPar.LoadIcon(IDI_DQ_PAR, FALSE, 16, 16);
	m_btnDualQuadPar.SetVerticalAlignment(BS_VCENTER);
	m_btnDualQuadPar.SetHorizontalAlignment(BS_CENTER);
	m_btnDualQuadPar.SetToolTipText(_T("Dual Quadratic Params"));
	m_btnDualQuadPar.SetToolTip(TRUE);
	

	m_btnVolaFitToImplied.SetFlatLook();
	m_btnVolaClear.SetFlatLook();
	m_btnVolaRegenerate.SetFlatLook();
	m_btnVolaRestore.SetFlatLook();
	m_btnVolaUpdate.SetFlatLook();
	m_btnClose.SetFlatLook();

	m_edtFactor.SetFlat(TRUE);
	m_edtFactor.SetForceToUsePeriod( FALSE );
	m_edtFactor.SetPositiveFormat(FACTOR_POSITIVE_FORMAT);
	m_edtFactor.SetNegativeFormat(FACTOR_NEGATIVE_FORMAT);
	m_edtFactor.SetFractionalDigitCount(FACTOR_FRACTIONALDIGITCOUNT);
	m_edtFactor.SetDecimalDigitCount(FACTOR_DECIMALDIGITCOUNT);

	m_edtUnderlinePrice.SetFlat(TRUE);
	m_edtUnderlinePrice.SetForceToUsePeriod( FALSE );
	m_edtUnderlinePrice.SetPositiveFormat(PRICE_POSITIVE_FORMAT);
	m_edtUnderlinePrice.SetNegativeFormat(PRICE_NEGATIVE_FORMAT);
	m_edtUnderlinePrice.SetFractionalDigitCount(PRICE_FRACTIONALDIGITCOUNT);
	m_edtUnderlinePrice.SetDecimalDigitCount(PRICE_DECIMALDIGITCOUNT);

	m_edtAccelerator.SetFlat(TRUE);
	m_edtAccelerator.SetForceToUsePeriod( FALSE );
	m_edtAccelerator.SetPositiveFormat(FACTOR_POSITIVE_FORMAT);
	m_edtAccelerator.SetNegativeFormat(FACTOR_NEGATIVE_FORMAT);
	m_edtAccelerator.SetFractionalDigitCount(FACTOR_FRACTIONALDIGITCOUNT);
	m_edtAccelerator.SetDecimalDigitCount(FACTOR_DECIMALDIGITCOUNT);

	m_edtHiVoly.SetFlat(TRUE);
	m_edtHiVoly.SetForceToUsePeriod(FALSE);
	m_edtHiVoly.SetPositiveFormat(VOLA_PTS_POSITIVE_FORMAT);
	m_edtHiVoly.SetNegativeFormat(VOLA_PTS_NEGATIVE_FORMAT);
	m_edtHiVoly.SetFractionalDigitCount(VOLA_PTS_FRACTIONALDIGITCOUNT);
	m_edtHiVoly.SetDecimalDigitCount(VOLA_PTS_DECIMALDIGITCOUNT);

	m_edtLoVoly.SetFlat(TRUE);
	m_edtLoVoly.SetForceToUsePeriod(FALSE);
	m_edtLoVoly.SetPositiveFormat(VOLA_PTS_POSITIVE_FORMAT);
	m_edtLoVoly.SetNegativeFormat(VOLA_PTS_NEGATIVE_FORMAT);
	m_edtLoVoly.SetFractionalDigitCount(VOLA_PTS_FRACTIONALDIGITCOUNT);
	m_edtLoVoly.SetDecimalDigitCount(VOLA_PTS_DECIMALDIGITCOUNT);

	m_sldFactor.SetRange(0, 100);
	m_sldFactor.SetTicFreq(10);

	// Format grids
	GridFormat();

	SetupDialog( g_DataProcessor.GetUndPrice().GetMidMarket() );

	CRect rc;
	GetWindowRect( &rc );
	m_nMinW = rc.Width();
	m_nMinH = rc.Height();
	ScreenToClient( &rc );

	CRect rcClose;
	m_btnClose.GetWindowRect( &rcClose );
	ScreenToClient( &rcClose );
	m_nMargine = rc.right - rcClose.right - 4;

	_AlignControls( true );

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::UpdateSymbolData( int nID, const std::vector<DATE>& months )
{
	m_bNexdDaySurfaceMode = false;

	m_months.clear();
	m_months = months;

	m_strTitle.Format( _T("Volatility Management - [%s]"), (LPCTSTR)g_DataProcessor.GetContractSymbol() );
	m_nContractID = nID;

	SetupDialog( g_DataProcessor.GetUndPrice().GetMidMarket() );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::UpdateView()
{
	if( !::IsWindow(m_hWnd) || !IsWindowVisible() )
		return;

	PostMessage( WM_CUSTOM_VOLA_UPDATE_VIEW );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::UpdateByPrice()
{
	if( m_nCurrentSurface == -1 || m_spSurface == NULL )
		return;

	CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
	sd.m_dUnderlinePrice = m_spSurface->UnderlinePrice;

	if( !::IsWindow(m_hWnd) || !IsWindowVisible() )
		return;

	// Setup underline price
	SetupUnderlinePrice( sd );
	GridDrawATMMarks();
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::UpdateRules()
{
	if( !::IsWindow(m_hWnd) )
		return;

	UpdateWindow();

	// Get current ruleID
	long nCurrentRuleID = m_vecRules[m_nCurrentRule].m_nID;

	SetupRules();

	// Restore rule selection
	for( size_t i = 0; i < m_vecRules.size(); i++ )
	{
		if( m_vecRules[i].m_nID == nCurrentRuleID )
		{
			m_nCurrentRule = i;
			m_cmbCurrentRule.SetCurSel( m_nCurrentRule );
			break;
		}
	}

	SetupControls();
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::PostNotification( UINT nMessage )
{
	CWnd *pNotificationWnd = AfxGetMainWnd();
	if( pNotificationWnd )
		pNotificationWnd->PostMessage( nMessage, (WPARAM)0, (LPARAM)0 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::PostNotification( UINT nMessage, CCustomVolaNotification::Type enType, double dtMonth )
{
	CWnd *pNotificationWnd = AfxGetMainWnd();
	if( pNotificationWnd )
	{
		CCustomVolaNotification *pNotification = new CCustomVolaNotification( enType, dtMonth );
		pNotificationWnd->PostMessage( nMessage, (WPARAM)pNotification, (LPARAM)m_hWnd );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
double CVolaManagementDlg::GetInterpolationFactor()
{
	return m_dIFactor;
}


/////////////////////////////////////////////////////////////////////////////
//
bool CVolaManagementDlg::OpenNextDaySurface( long nSurfaceID )
{
	for( size_t i = 0; i < m_vecSurfaces.size(); i++ )
		if( m_vecSurfaces[i].m_nID == nSurfaceID )
		{
			_ChangeCurrentSurface( i );
			m_bNexdDaySurfaceMode = true;

			if( ::IsWindow( m_hWnd ) )
			{
				SetupControls();
				GridInit();
				GridFill();
			}

			g_DataProcessor.GetVolaManager()->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())] = m_vecSurfaces[m_nCurrentSurface].m_nID;
			PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );

			return true;
		}

	return false;
}


/////////////////////////////////////////////////////////////////////////////
//
bool CVolaManagementDlg::OpenDefaultSurface( long nSurfaceID )
{
	g_DataProcessor.GetVolaManager()->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())] = nSurfaceID;
	if( !SetupSurfaces() )
		return false;

	if( ::IsWindow( m_hWnd ) )
	{
		SetupControls();
		GridInit();
		GridFill();
	}

	PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );

	return true;
}

	
/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::GridFormat()
{
	m_fgVola.SetRedraw( 0 );

	// Selections
	m_fgVola.SetAllowSelection( FALSE );
	m_fgVola.SetAllowBigSelection(FALSE);

	// Sorting
	m_fgVola.SetExplorerBar( 0 );		/*flexExNone*/

	// Editing
	m_fgVola.SetEditable(2);			/*flexEDKbdMouse*/

	// Look and feel
	m_fgVola.SetAppearance( 2 );		/*flex3DLight*/
	m_fgVola.SetGridLines( 1 );			/*flexGridFlat*/
	m_fgVola.SetGridLinesFixed( 1 );	/*flexGridFlat*/

	m_fgVola.SetRow(0L);
	m_fgVola.SetCol(0L);

	m_fgVola.SetRedraw( 2 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::GridInit()
{
	if( m_vecSurfaces.size() <= 0 || m_vecRules.size() <= 0 || m_nCurrentSurface < 0 || m_nCurrentRule < 0 )
	{
		EnableControls( TRUE );
		return;
	}

	m_fgVola.SetRedraw( 0 );

	long TopRow  = m_fgVola.GetTopRow();
	long PrevRow = m_fgVola.GetRow();
	long PrevCol = m_fgVola.GetCol();

	m_fgVola.SetFixedRows( 1 );
	m_fgVola.SetFixedCols( 1 );
	m_fgVola.SetCols( m_months.size() + 1 );
	m_fgVola.SetRows( 2 );
	long nCols = m_fgVola.GetCols();

	// Format first column
	m_fgVola.SetColFormat( 0, _T("####.##") );
	m_fgVola.SetTextMatrix( 0, 0, _T("Strike, $") );

	// Add month related columns
	long i = 1;
	COleDateTime date;
	CString		 sdate;

	CIdMap mapCols;

	for( std::vector<DATE>::iterator it = m_months.begin(); it != m_months.end(); i++, it++ )	
	{
		date = *it;
		sdate = date.Format(_T("%b,%d %y "));
		
		char cFirst = sdate.GetAt(0);
		cFirst = (TCHAR)CharUpper( (LPTSTR)MAKELONG(cFirst, 0) );
		sdate.SetAt( 0, cFirst );

		// Month titles
		m_fgVola.SetColDataType( i , 8/*flexDTString*/ );
		m_fgVola.SetColData( i, _variant_t(date) );
		m_fgVola.SetTextMatrix( 0, i, sdate );

		mapCols[(long)date.m_dt] = i;
	}
	
	m_fgVola.SetFrozenRows( 1 );

	long nRow, nNewRow, nCol;
	const options_map& opts = g_DataProcessor.GetOptions();

	long clrNormal = m_fgVola.GetBackColor();
	long clrDisabled = ::GetSysColor(COLOR_BTNHILIGHT); //gd.GetGridColor();
	CString strTemp;

	for(options_map::const_iterator it_opt = opts.begin(); it_opt != opts.end(); it_opt++)
	{
		const COptionData& opt = it_opt->second;
		nRow = m_fgVola.GetFindRow(CComVariant(opt.m_dStrike), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
		if(nRow < 0)
		{
			strTemp.Format(_T("%.2f"), opt.m_dStrike);
			m_fgVola.AddItem(strTemp, vtMissing);
			nNewRow = m_fgVola.GetRows() - 1;
			m_fgVola.SetRowData(nNewRow, CComVariant(opt.m_dStrike));
			m_fgVola.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(clrDisabled));
			m_fgVola.SetCell(0 /*flexcpText*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(_T("N/A")));
			m_fgVola.SetCell(2 /*flexcpAlignment*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(4L));
			m_fgVola.SetCell(20/*flexcpData*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), vtMissing);
			nRow = nNewRow;
		}
		date = opt.m_dtExpiration;
		date -= date.GetDay() - 1L;
		CIdMap::iterator itCol = mapCols.find((long)date.m_dt);
		ASSERT(itCol != mapCols.end());

		//strTemp.Format(_T("%.2f"), opt.m_dStrike);
		nCol = itCol->second;

		m_fgVola.SetTextMatrix(nRow, nCol, _T(""));
		m_fgVola.SetCell(20/*flexcpData*/,		CComVariant(nRow), CComVariant(nCol), vtMissing, vtMissing, CComVariant(0.));
		m_fgVola.SetCell(2 /*flexcpAlignment*/, CComVariant(nRow), CComVariant(nCol), vtMissing, vtMissing, CComVariant(7L));
		m_fgVola.SetCell(6 /*flexcpBackColor*/, CComVariant(nRow), CComVariant(nCol), vtMissing, vtMissing, CComVariant(clrNormal));
	}

	m_fgVola.SetCol(0L);
	m_fgVola.SetSort(1L);

	PrevRow = m_fgVola.GetRows() - 1 < PrevRow ? 0 : PrevRow;
	PrevCol = m_fgVola.GetCols() - 1 < PrevCol ? 0 : PrevCol;

	m_fgVola.SetTopRow( TopRow );
	m_fgVola.SetRow(PrevRow);
	m_fgVola.SetCol(PrevCol);

	m_fgVola.SetRedraw( 2 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::GridFill( bool bForceFocus )
{
	CWaitCursor wait;

	if( m_vecSurfaces.size() <= 0 || m_vecRules.size() <= 0 || m_nCurrentSurface < 0 || m_nCurrentRule < 0 )
	{
		EnableControls( TRUE );
		return;
	}
	
	m_fgVola.SetRedraw( 0 );
	long nCols = m_fgVola.GetCols();

	long clrDisabled = ::GetSysColor(COLOR_GRAYTEXT);
	long clrEnabled  = ::GetSysColor(COLOR_WINDOWTEXT);

	try
	{
		CString strValue;

		long TopRow  = m_fgVola.GetTopRow();
		long PrevRow = m_fgVola.GetRow();
		long PrevCol = m_fgVola.GetCol();

		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		m_spSurface->InterpolationFactor  = 1 - m_dIFactor;
		m_spSurface->UnderlinePrice	      = m_edtUnderlinePrice.GetValue();
		m_spSurface->SmileAccelerator     = m_edtAccelerator.GetValue();
		m_spSurface->DiscreteAcceleration = m_chkDiscreteAccel.GetCheck() ? VARIANT_TRUE : VARIANT_FALSE;

		// For every month
		for( long col = 1; col < m_fgVola.GetCols(); col++ )
		{
			DATE dtExpDate = m_fgVola.GetColData( col ).date;

			// Get surface
			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
			
			// Checkers
			bool bIsChecked = spSkew->BasePointsInterpolation == VARIANT_TRUE;
			m_fgVola.SetCell( 5/**flexcpChecked*/, CComVariant(1L), CComVariant(col), vtMissing, vtMissing, CComVariant(bIsChecked ? 1/*flexChecked*/ : 2/*flexUnChecked*/ ) );
			m_fgVola.SetCell( 4/*flexcpPictureAlignment*/, CComVariant(1L), CComVariant(col), vtMissing, vtMissing, CComVariant(4)/*flexPicAlignCenterCenter*/ );

			// For each explicit strike in surface
			for( long i = 0; i < spSkew->Count; i++ )
			{
				double dStrike = spSkew->Strike[i];

				if( spSkew->Status[dStrike] == Deleted )
					continue;

				long row = m_fgVola.GetFindRow(CComVariant(dStrike), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
				if( row != -1 )
				{
					try
					{
						_variant_t vtVola = spSkew->Volatility[dStrike];
						if( vtVola.vt == VT_EMPTY )
						{
							m_fgVola.SetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, vtMissing);
						}
						else
						{
							strValue.Format( _T("%.2f"), vtVola.dblVal );
							m_fgVola.SetTextMatrix( row, col, strValue );
							m_fgVola.SetCell(20/*flexcpData*/,		CComVariant(row), CComVariant(col), vtMissing, vtMissing, vtVola);
							m_fgVola.SetCell(2 /*flexcpAlignment*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(7L));

							if( spSkew->IsBasePoint[dStrike] == VARIANT_TRUE )
								m_fgVola.SetCell(13 /*flexcpFontBold*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(true));

							if( m_bNexdDaySurfaceMode )
								m_fgVola.SetCell(7 /*flexcpForeColor*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(clrDisabled));
							else
								m_fgVola.SetCell(7 /*flexcpForeColor*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(clrEnabled));
						}
					}
					catch( const _com_error& e )
					{
						_bstr_t msg = e.Description();
					}
				}
			}

			long   nATMRow    = _GetATMStrikeRow( col );
			double dATMStrike = m_fgVola.GetRowData( nATMRow ).dblVal;
			m_fgVola.SetCell(6 /*flexcpBackColor*/, CComVariant(nATMRow), CComVariant(col), vtMissing, vtMissing, CComVariant((long)0xC0FFC0) );
			if( m_bNexdDaySurfaceMode )
				m_fgVola.SetCell(7 /*flexcpForeColor*/, CComVariant(nATMRow), CComVariant(col), vtMissing, vtMissing, CComVariant(clrEnabled));

			if( col == 1 && bForceFocus )
			{
				m_fgVola.SetRow(nATMRow);
				m_fgVola.SetCol(col);
			}
		}

		if( !bForceFocus )
		{
			m_fgVola.SetTopRow( TopRow );
			m_fgVola.SetRow(PrevRow);
			m_fgVola.SetCol(PrevCol);
		}
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error filling grid with volatility values. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}

	m_fgVola.AutoSize(0, CComVariant(nCols - 1L), CComVariant(1L), CComVariant(10L));
	m_fgVola.SetRedraw( 2 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::GridDrawATMMarks()
{
	m_fgVola.SetRedraw( 0 );

	try
	{
		long clrNormal = m_fgVola.GetBackColor();

		// For every month
		for( long col = 1; col < m_fgVola.GetCols(); col++ )
		{
			DATE dtExpDate = m_fgVola.GetColData( col ).date;

			// Get surface
			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
			
			long nATMRow = _GetATMStrikeRow( col );

			// For each strike in grid
			for( long row = 2; row < m_fgVola.GetRows(); row++ )
			{
				// Check if there is option for strike
				_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
				if( vtData == vtMissing )
					continue;

				if( row == nATMRow )
					m_fgVola.SetCell(6 /*flexcpBackColor*/, CComVariant(nATMRow), CComVariant(col), vtMissing, vtMissing, CComVariant((long)0xC0FFC0) );
				else
					m_fgVola.SetCell(6 /*flexcpBackColor*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(clrNormal) );
			}
		}
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error drawing ATM marks. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}

	m_fgVola.SetRedraw( 2 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::SetupDialog( double dUnderlinePrice )
{
	m_bInitialized = false;

	if( !SetupSurfaces() && ::IsWindow(m_hWnd) )
	{
		EnableControls( TRUE );
		return;
	}

	if( m_nCurrentSurface >= 0 && m_spSurface != NULL )
	{
		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		if( !sd.m_bIsPriceOverride )
			sd.m_dUnderlinePrice = dUnderlinePrice;
		m_spSurface->UnderlinePrice = sd.m_dUnderlinePrice;
	}

	if( ::IsWindow(m_hWnd) )
	{
		SetWindowText(m_strTitle);

		SetupRules();

		SetupControls();
		GridInit();
		GridFill( true );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::SetupRules()
{
	try
	{
		CDBLayout db;
		
		m_vecRules.clear();
		m_cmbCurrentRule.ResetContent();

		db.Connect();
		CClientRecordset rs;

		db.ContractGenerationRuleGet( m_nContractID, rs );

		rs.MoveFirst();
		while( !rs.IsBOF() && !rs.IsEOF() )
		{
			CRuleData r;

			r.m_nID				     = rs[L"iRuleID"];
			r.m_nType				 = rs[L"iRuleType"];
			r.m_strName			     = (LPCTSTR)(_bstr_t)rs[L"vcRuleName"];
			r.m_nStrikeIncrement	 = rs[L"iStrikeIncrement"];
			r.m_nRequiredPoints		 = rs[L"iRequiredPoints"];
			r.m_dSmileAccelerator    = rs[L"fSmileAccelerator"];
			r.m_dCallShift			 = rs[L"fCallShift"];
			r.m_dPutShift			 = rs[L"fPutShift"];
			r.m_dInterpolationFactor = rs[L"fInterpolationFactor"];
			r.m_dStrikeMoneynessHi	 = rs[L"fStrikeMoneynessHi"];
			r.m_dStrikeMoneynessLo	 = rs[L"fStrikeMoneynessLo"];
			r.m_bIsDefault	         = rs[L"bIsDefaultRule"];
			r.m_bIsGroupRule		 = rs[L"bIsGroupRule"];
			r.m_bIsContractRule	     = rs[L"bIsContractRule"];

			m_vecRules.push_back( r );

			rs.MoveNext();
		}
		
		m_bGroupRuleSet    = false;
		m_bContractRuleSet = false;
		for( size_t i = 0; i < m_vecRules.size(); i++ )
		{
			m_cmbCurrentRule.InsertString( i, m_vecRules[i].m_strName );
			m_cmbCurrentRule.SetItemData(  i, m_vecRules[i].m_nID );

			if( m_vecRules[i].m_bIsGroupRule )
			{
				m_nCurrentRule = i;
				m_bGroupRuleSet = true;
			}
			else if( !m_bGroupRuleSet && m_vecRules[i].m_bIsContractRule )
			{
				m_nCurrentRule = i;
				m_bContractRuleSet = true;
			}
			else if( !m_bGroupRuleSet && !m_bContractRuleSet && m_vecRules[i].m_bIsDefault )
			{
				m_nCurrentRule = i;
			}
		}
		m_cmbCurrentRule.SetCurSel( m_nCurrentRule );

		if( m_vecRules.size() == 0 )
		{
			m_cmbCurrentRule.AddString( _T("<No generation rules available>") );
			m_btnUpdateRule.EnableWindow( FALSE );
			m_cmbCurrentRule.SetCurSel( 0 );
		}
		else
		{

		}
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error loading interpolation rules. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
bool CVolaManagementDlg::SetupSurfaces()
{
	try
	{
		m_vecSurfaces.clear();

		CDBLayout db;
		db.Connect();
		CClientRecordset rs;

		db.VolaSurfaceDataGet( m_nContractID, -1, rs );

		if( rs.IsBOF() && rs.IsEOF() )
		{
			
			rs.Close();
			db.VolaSurfaceDataSave( -1, m_nContractID,
									g_DataProcessor.GetUndPrice().GetMidMarket(), 0.0, 
									-1, -1, -1, _T("<Default>"), 
									-1, 0,
									1 );
			db.VolaSurfaceDataGet( m_nContractID, -1, rs );

			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
		}

		rs.MoveFirst();
		while( !rs.IsEOF() )
		{
			CSurfaceData sd;
			sd.m_nID				  = rs[L"iVolaSurfaceDataID"];
			sd.m_strName			  = (LPCTSTR)(_bstr_t)rs[L"vcSurfaceName"];
			sd.m_dSmileAccelerator    = rs[L"fSmileAccelerator"].IsNull()    ? -1.0f : (double)rs[L"fSmileAccelerator"];
			sd.m_nInterpolationFactor = rs[L"fInterpolationFactor"].IsNull() ? -1 : static_cast<long>((double)rs[L"fInterpolationFactor"] * 100);
			sd.m_dStrikeMoneynessLo	  = rs[L"fStrikeMoneynessLo"].IsNull()   ? -1 : (double)rs[L"fStrikeMoneynessLo"];
			sd.m_dStrikeMoneynessHi	  = rs[L"fStrikeMoneynessHi"].IsNull()   ? -1 : (double)rs[L"fStrikeMoneynessHi"];
			sd.m_bIsDefault			  = (bool)rs[L"bIsDefaultSurface"];
			sd.m_bIsPriceOverride	  = (bool)rs[L"bIsPriceOverride"];
			sd.m_bIsDiscreteAccel 	  = (bool)rs[L"bIsDiscreteAcceleration"];
			sd.m_bIsNextDaySurface 	  = (bool)rs[L"bIsNextDaySurface"];
			sd.m_dUnderlinePrice = rs[L"fBaseUnderlinePrice"];
			if( !sd.m_bIsPriceOverride && fEQZero( sd.m_dUnderlinePrice ) )
				sd.m_dUnderlinePrice = g_DataProcessor.GetContractPrice();

			m_vecSurfaces.push_back( sd );

			rs.MoveNext();
		}

		long nActiveSurfaceID = -1;
		long nCurrentSurface  = -1;
		try
		{
			nActiveSurfaceID = g_DataProcessor.GetVolaManager()->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())];
		}
		catch( ... ) {}

		for( size_t i = 0; i < m_vecSurfaces.size(); i++ )
			if( m_vecSurfaces[i].m_nID == nActiveSurfaceID )
			{
				nCurrentSurface = i;
				break;
			}
			else if( m_vecSurfaces[i].m_bIsDefault )
			{
				nCurrentSurface = i;
			}

		if( nCurrentSurface == -1 && m_vecSurfaces.size() != 0 )
			nCurrentSurface = 0;

		_ChangeCurrentSurface( nCurrentSurface );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error loading surface info. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 

		return false;
	}

	return true;
}


void CVolaManagementDlg::SetupUnderlinePrice( CSurfaceData& sd )
{
	try
	{
		m_chkPriceOverride.SetCheck( sd.m_bIsPriceOverride ? 1 : 0 );
		if( sd.m_bIsPriceOverride && sd.m_dUnderlinePrice >= 0 )
			m_edtUnderlinePrice.SetValue( sd.m_dUnderlinePrice );
		else
		{
			m_edtUnderlinePrice.SetValue( sd.m_dUnderlinePrice );
		}
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error updating underline price. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		TRACE( message ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::SetupControls()
{
	m_bInitialized = false;

	HWND hFocusWnd = GetFocus()->GetSafeHwnd();

	// Menu Items
	CMenu *pViewMenu = GetMenu();
	if( pViewMenu )
		pViewMenu = pViewMenu->GetSubMenu( 1 );
	if( pViewMenu )
	{
		if( m_bAlwaysOnTop )
		{
			pViewMenu->CheckMenuItem( IDC_VM_VIEW_ALWAYSONTOP, MF_BYCOMMAND|MF_CHECKED );
			SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
		}
		else
		{
			pViewMenu->CheckMenuItem( IDC_VM_VIEW_ALWAYSONTOP, MF_BYCOMMAND|MF_UNCHECKED );
			SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
		}
		if( m_bUseExtrapolation )
		{
			pViewMenu->CheckMenuItem( IDC_VM_VIEW_SHOWALL, MF_BYCOMMAND|MF_CHECKED );
		}
		else
		{
			pViewMenu->CheckMenuItem( IDC_VM_VIEW_SHOWALL, MF_BYCOMMAND|MF_UNCHECKED );
		}
	}

	if( m_vecSurfaces.size() <= 0 || m_vecRules.size() <= 0 || m_nCurrentSurface < 0 || m_nCurrentRule < 0 )
	{
		EnableControls( TRUE );
		return;
	}

	CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

	// Caption
	if( !sd.m_strName.IsEmpty() )
	{
		CString strTitle = m_strTitle + _T(" - ") + sd.m_strName;
		SetWindowText( strTitle );
	}

	// Setup underline price
	SetupUnderlinePrice( sd );

	// Smile Accelerator
	if( m_vecRules.size() != 0 )
	{
		if( sd.m_dSmileAccelerator >= 0 )
			m_edtAccelerator.SetValue( sd.m_dSmileAccelerator );
		else
			m_edtAccelerator.SetValue( m_vecRules[m_nCurrentRule].m_dSmileAccelerator );
	}
	else
		m_edtAccelerator.SetValue( 0.0f );

	// Discrete acceleration
	m_chkDiscreteAccel.SetCheck( sd.m_bIsDiscreteAccel > 0 ? 1 : 0 );

	// Setup interpolation factor
	
	if( sd.m_nInterpolationFactor >= 0 )
	{
		m_nIFactor = sd.m_nInterpolationFactor;
		m_dIFactor = m_nIFactor/100.;
	}
	else if( m_vecRules.size() != 0 )
	{
		m_dIFactor = m_vecRules[m_nCurrentRule].m_dInterpolationFactor;
		m_nIFactor = static_cast<long>(m_dIFactor * 100.);
		if( m_dIFactor > m_nIFactor/100. )
			m_nIFactor = static_cast<long>(ceil(m_dIFactor * 100.));
		else if( m_dIFactor > m_nIFactor/100. )
			m_nIFactor = static_cast<long>(floor(m_dIFactor * 100.));
	}
	else
	{
		m_dIFactor = 1;
		m_nIFactor = 100;
	}

	m_edtFactor.SetValue( m_dIFactor );
	m_sldFactor.SetPos(   m_nIFactor );

	// Points Hi&Lo
	if( sd.m_dStrikeMoneynessHi >= 0 )
		m_edtHiVoly.SetValue( sd.m_dStrikeMoneynessHi );
	else
		m_edtHiVoly.SetValue( m_vecRules[m_nCurrentRule].m_dStrikeMoneynessHi );
	if( sd.m_dStrikeMoneynessLo >= 0 )
		m_edtLoVoly.SetValue( sd.m_dStrikeMoneynessLo );
	else
		m_edtLoVoly.SetValue( m_vecRules[m_nCurrentRule].m_dStrikeMoneynessLo );

	if(m_vecRules[m_nCurrentRule].m_nType != RULE_TYPE_DUAL_QUADRATIC)
	{
		m_btnDualQuadPar.ShowWindow( SW_HIDE );
	}
	else
	{
		m_btnDualQuadPar.ShowWindow( SW_SHOW );
	}

	UpdateData( FALSE );

	EnableControls( FALSE );

	if( hFocusWnd )
		::SetFocus( hFocusWnd );

	m_bInitialized = true;
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::EnableControls( bool bForceDisable )
{
	CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

	if( !m_bNexdDaySurfaceMode && !bForceDisable )
	{
		// Surface menu
		GetMenu()->EnableMenuItem( 0, MF_BYPOSITION|MF_ENABLED );
		CMenu* pFileMenu = GetMenu()->GetSubMenu( 0 );
		if( sd.m_bIsDefault || m_vecSurfaces.size() <= 1 )
			pFileMenu->EnableMenuItem( IDC_VM_SURFACE_DELETE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		else
			pFileMenu->EnableMenuItem( IDC_VM_SURFACE_DELETE, MF_BYCOMMAND|MF_ENABLED );
		if( sd.m_bIsDefault )
			pFileMenu->EnableMenuItem( IDC_VM_SURFACE_SET_AS_DEFAULT, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		else
			pFileMenu->EnableMenuItem( IDC_VM_SURFACE_SET_AS_DEFAULT, MF_BYCOMMAND|MF_ENABLED );
		if( sd.m_bIsNextDaySurface )
			pFileMenu->CheckMenuItem( IDC_VM_SURFACE_NEXTDAY, MF_BYCOMMAND|MF_CHECKED );
		else
			pFileMenu->CheckMenuItem( IDC_VM_SURFACE_NEXTDAY, MF_BYCOMMAND|MF_UNCHECKED );

		// Rule remove button
		if( m_nCurrentRule == -1 || m_vecRules[m_nCurrentRule].m_bIsDefault )
			m_btnDeleteRule.EnableWindow( FALSE );
		else
			m_btnDeleteRule.EnableWindow( TRUE );

		// Underline price override
		m_edtUnderlinePrice.EnableWindow( sd.m_bIsPriceOverride );

		m_cmbCurrentRule.EnableWindow( TRUE );
		m_btnNewRule.EnableWindow( TRUE );
		m_btnUpdateRule.EnableWindow( TRUE );
		m_chkPriceOverride.EnableWindow( TRUE );
		m_chkDiscreteAccel.EnableWindow( TRUE );
		m_edtFactor.EnableWindow( TRUE );
		m_sldFactor.EnableWindow( TRUE );
		m_edtLoVoly.EnableWindow( TRUE );
		m_edtHiVoly.EnableWindow( TRUE );
		m_edtAccelerator.EnableWindow( TRUE );

		m_fgVola.EnableWindow( TRUE );
		m_btnVolaFitToImplied.EnableWindow( TRUE );
		m_btnVolaClear.EnableWindow( TRUE );
		m_btnVolaRegenerate.EnableWindow( TRUE );
		m_btnVolaUpdate.EnableWindow( TRUE );
		m_btnVolaRestore.EnableWindow( TRUE );

		GetDlgItem( IDC_STATIC_RULE )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_IF )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_LV )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_LVP )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_HV )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_HVP )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_UP )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_SA )->EnableWindow( TRUE );
	}
	else
	{
		GetMenu()->EnableMenuItem( 0, MF_BYPOSITION|MF_DISABLED|MF_GRAYED );

		m_edtUnderlinePrice.EnableWindow( FALSE );
		m_cmbCurrentRule.EnableWindow( FALSE );
		m_btnNewRule.EnableWindow( FALSE );
		m_btnUpdateRule.EnableWindow( FALSE );
		m_chkPriceOverride.EnableWindow( FALSE );
		m_chkDiscreteAccel.EnableWindow( FALSE );
		m_edtFactor.EnableWindow( FALSE );
		m_sldFactor.EnableWindow( FALSE );
		m_edtLoVoly.EnableWindow( FALSE );
		m_edtHiVoly.EnableWindow( FALSE );
		m_edtAccelerator.EnableWindow( FALSE );

		m_btnVolaFitToImplied.EnableWindow( FALSE );
		m_btnVolaClear.EnableWindow( FALSE );
		m_btnVolaRegenerate.EnableWindow( FALSE );
		if( bForceDisable )
		{
			m_fgVola.EnableWindow( FALSE );
			m_btnVolaUpdate.EnableWindow( FALSE );
			m_btnVolaRestore.EnableWindow( FALSE );
		}

		GetDlgItem( IDC_STATIC_RULE )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_IF )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_LV )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_LVP )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_HV )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_HVP )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_UP )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_SA )->EnableWindow( FALSE );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnOK()
{
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnCancel()
{
	if(m_bNeedSave)
	{
		CString strMsg;
		strMsg.LoadString(IDS_MSG_SURFACE_SAVE);
		if(IDYES == MessageBox((LPCTSTR)strMsg,NULL, MB_YESNO|MB_ICONQUESTION))
		{
			OnVolaUpdate();
		}
		m_bNeedSave = false;
	}
	::UnregisterHotKey( m_hWnd, 0x0000 );
	::UnregisterHotKey( m_hWnd, 0x0001 );
	::UnregisterHotKey( m_hWnd, 0x0002 );
	::UnregisterHotKey( m_hWnd, 0x0003 );
	::UnregisterHotKey( m_hWnd, 0x0004 );
	::UnregisterHotKey( m_hWnd, 0x0005 );
	::UnregisterHotKey( m_hWnd, 0x0006 );

	PostMessage(WM_CLOSE);
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnChangeFactor() 
{
	if(m_bInitialized && !m_bEditing)
	{
		m_bEditing = true;

		m_dIFactor = m_edtFactor.GetValue();

		if(m_dIFactor < 0.)
		{
			m_dIFactor = 0.;
		}
		else if(m_dIFactor > 1.)
		{
			m_dIFactor = 1.;
		}

		m_nIFactor = static_cast<long>(m_dIFactor * 100);
		m_sldFactor.SetPos(m_nIFactor);

		if(m_dIFactor != m_edtFactor.GetValue())
		{
			MessageBeep(MB_OK);
			m_edtFactor.SetValue(m_dIFactor);
		}

		if( m_vecSurfaces[m_nCurrentSurface].m_nInterpolationFactor != -1 &&
			m_vecSurfaces[m_nCurrentSurface].m_nInterpolationFactor != m_nIFactor 
		  )
		{
			m_vecSurfaces[m_nCurrentSurface].m_nInterpolationFactor = m_nIFactor;

			SetupControls();
			GridInit();
			GridFill();
			
			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
		}

		m_bEditing = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(m_bInitialized && !m_bEditing && pScrollBar->GetDlgCtrlID() == IDC_SLIDER_FACTOR)
	{
		int nNewPos = m_sldFactor.GetPos();
		if( nNewPos != m_nIFactor )
		{
			m_bEditing = true;
			m_nIFactor = nNewPos;
			m_dIFactor = m_nIFactor / 100.;

			m_edtFactor.SetValue(m_dIFactor);

			m_vecSurfaces[m_nCurrentSurface].m_nInterpolationFactor = m_nIFactor;

			SetupControls();
			GridInit();
			GridFill();

			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );

			m_bEditing = false;
		}
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnComboCurrentRuleSelchange() 
{
	long nOldRuleType = m_vecRules[m_nCurrentRule].m_nType;
	m_nCurrentRule = m_cmbCurrentRule.GetCurSel();

	SetupControls();

	if( nOldRuleType != RULE_TYPE_CURVE_FIT && m_vecRules[m_nCurrentRule].m_nType == RULE_TYPE_CURVE_FIT )
	{
		_UnbasePoints();

		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnClose() 
{
	m_bInitialized		  = false;
	m_bNexdDaySurfaceMode = false;

	ShowWindow( SW_HIDE );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnDestroy() 
{
	m_bInitialized		  = false;
	m_bNexdDaySurfaceMode = false;

	CDialog::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = m_nMinW;
	lpMMI->ptMinTrackSize.y = m_nMinH;
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnSize(UINT nType, int cx, int cy) 
{
	if( m_bInitialized )
	{
		CMenu* pMenu  = GetMenu()->GetSubMenu( 1 );
		UINT   uState = pMenu->GetMenuState( IDC_VM_VIEW_GENERATION_OPTIONS, MF_BYCOMMAND );

		_AlignControls( (uState & MF_CHECKED) != 0 );
	}

	CDialog::OnSize(nType, cx, cy);
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	m_bInitialized = (bShow != 0);

	if( m_bInitialized )
	{
		SetupControls();
		GridInit();
		GridFill( true );
	}
	else
	{
		m_fgVola.SetFocus();
	}


	CDialog::OnShowWindow(bShow, nStatus);
}


/////////////////////////////////////////////////////////////////////////////
//
BOOL CVolaManagementDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_HOTKEY )
	{
		SHORT nVK  = HIWORD( pMsg->lParam );
		SHORT nMod = LOWORD( pMsg->lParam );

		switch( nVK )
		{
			case VK_F5:
				if( !m_bNexdDaySurfaceMode )
					OnVolaRegenerate();
				break;
			case _T('S'):
			case _T('s'):
				if( nMod == MOD_CONTROL )
					OnVolaUpdate();
				break;
			case _T('Z'):
			case _T('z'):
				if( nMod == MOD_CONTROL )
					OnVolaUpdate();
				break;
			case _T('A'):
			case _T('a'):
				if( nMod == MOD_CONTROL )
					_SelectAll();
				break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if( nState == WA_INACTIVE || bMinimized )
	{
		::UnregisterHotKey( m_hWnd, 0x0000 );
		::UnregisterHotKey( m_hWnd, 0x0001 );
		::UnregisterHotKey( m_hWnd, 0x0002 );
		::UnregisterHotKey( m_hWnd, 0x0003 );
		::UnregisterHotKey( m_hWnd, 0x0004 );
		::UnregisterHotKey( m_hWnd, 0x0005 );
		::UnregisterHotKey( m_hWnd, 0x0006 );
	}
	else
	{
		CMenu* pMenu  = GetMenu()->GetSubMenu( 1 );
		UINT   uState = pMenu->GetMenuState( IDC_VM_VIEW_GENERATION_OPTIONS, MF_BYCOMMAND );

		_AlignControls( (uState & MF_CHECKED) != 0 );

		m_bInitialized = true;
		EnableControls( false );

		::RegisterHotKey( m_hWnd, 0x0000, 0, VK_F5 );
		::RegisterHotKey( m_hWnd, 0x0001, MOD_CONTROL, _T('S') );
		::RegisterHotKey( m_hWnd, 0x0002, MOD_CONTROL, _T('s') );
		::RegisterHotKey( m_hWnd, 0x0003, MOD_CONTROL, _T('Z') );
		::RegisterHotKey( m_hWnd, 0x0004, MOD_CONTROL, _T('z') );
		::RegisterHotKey( m_hWnd, 0x0005, MOD_CONTROL, _T('A') );
		::RegisterHotKey( m_hWnd, 0x0006, MOD_CONTROL, _T('a') );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaStartEdit(long Row, long Col, BOOL FAR* Cancel) 
{
	if( m_bNexdDaySurfaceMode )
	{
		long nATMRow = _GetATMStrikeRow( Col );
		if( Row != nATMRow && Row != 1)
			*Cancel = TRUE;
	}
	else
	{
		_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing));
		*Cancel = (vtData == vtMissing);
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaValidateEdit(long Row, long Col, BOOL FAR* Cancel) 
{
	*Cancel = FALSE;

	double dValue = 0.;
	CString strValue = m_fgVola.GetEditText();
	strValue.TrimLeft();
	strValue.TrimRight();

	if( strchr(strValue, 'e') )
	{
		MessageBeep( MB_OK );
		*Cancel = TRUE;
	}

	if( !*Cancel && !strValue.IsEmpty() )
	{
		_variant_t vt = strValue;
		try
		{
			dValue = vt;
		}
		catch( ... )
		{
			MessageBeep( MB_OK );
			*Cancel = TRUE;
		}

		if(!*Cancel)
		{
			dValue = ((LONGLONG)(dValue * 100L)) / 100.;

			if(dValue < 0.)
			{
				dValue = (_variant_t)m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing);
				*Cancel = TRUE;
			}

			strValue.Format(_T("%.2f"), dValue);
			m_fgVola.SetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing, COleVariant(dValue));
			m_fgVola.SetEditText(strValue);
		}
	}
	else
	{
		m_fgVola.SetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing, COleVariant(0.));
		m_fgVola.SetEditText(_T(""));
	}
	if(!*Cancel)
		m_bNeedSave = true;
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaAfterEdit(long Row, long Col) 
{
	try
	{
		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		DATE dtExpDate   = m_fgVola.GetColData( Col ).date;
		VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];

		if( Row == 1 )
		{
			_variant_t vtCheck( m_fgVola.GetCell( 5/**flexcpChecked*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing ) );
			if( (long)vtCheck == 1L )
				spSkew->BasePointsInterpolation = VARIANT_TRUE;
			else
				spSkew->BasePointsInterpolation = VARIANT_FALSE;
		}
		else if( Row > 1 )
		{
			double dStrike = m_fgVola.GetRowData( Row ).dblVal;
			double dVola   = m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing ).dblVal;

			if( !m_bNexdDaySurfaceMode )
			{
				bool bIsATMPoint = false;
				try
				{
					bIsATMPoint = spSkew->IsATM[dStrike] == VARIANT_TRUE;
				}
				catch( ... ) {}

				if( dVola != 0 )
				{
					spSkew->Add( dStrike, dVola, VARIANT_TRUE );
					if( m_bUseExtrapolation )
						_ExtrapolateMonthVolatility( Col, spSkew );
				}
				else
				{
					try
					{
						spSkew->Remove( dStrike );
						if( m_bUseExtrapolation )
							_ExtrapolateMonthVolatility( Col, spSkew );
					}
					catch( ... ) {}
				}
			}
			else
			{
				spSkew->ProportionalShift( dStrike, dVola );
			}
		}

		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (dtExpDate) );

		m_fgVola.SetRow( Row );
		m_fgVola.SetCol( Col );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error editing volatility point. ");
		message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaKeyUp(short FAR* KeyCode, short Shift) 
{
	if( *KeyCode == VK_DELETE )
	{
		try
		{
			long Row = m_fgVola.GetRow();
			long Col = m_fgVola.GetCol();

			if( Row < 2 )
				return;
			
			double dStrike   = m_fgVola.GetRowData( Row ).dblVal;
			DATE   dtExpDate = m_fgVola.GetColData( Col ).date;

			m_fgVola.SetTextMatrix( Row, Col, _T("") );

			CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
			try
			{
				spSkew->Remove( dStrike );
				if( m_bUseExtrapolation )
					_ExtrapolateMonthVolatility( Col, spSkew );
			}
			catch( ... ) 
			{}

			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (dtExpDate) );
			GridInit();
			GridFill();

			m_fgVola.SetRow( Row );
			m_fgVola.SetCol( Col );
		}
		catch( const _com_error& e )
		{
			CString message = _T("Error deleting volatility point. ");
			message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
			MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
		}
	}
}


void CVolaManagementDlg::OnVolaMouseUp(short Button, short Shift, float X, float Y) 
{
	m_nMouseRow = m_fgVola.GetMouseRow();
	m_nMouseCol = m_fgVola.GetMouseCol();

	if( m_nMouseRow == 0 && m_nMouseCol > 0 )
	{
		DATE dtExp = m_fgVola.GetColData( m_nMouseCol ).date;

		_variant_t vtCheck( m_fgVola.GetCell( 5/**flexcpChecked*/, CComVariant(1L), CComVariant(m_nMouseCol), vtMissing, vtMissing ) );
		bool bIsChecked = vtCheck.iVal == 1;

		m_fgVola.SetCell( 5/**flexcpChecked*/, CComVariant(1L), CComVariant(m_nMouseCol), vtMissing, vtMissing, CComVariant(bIsChecked ? 2/*flexUnChecked*/ : 1/*flexChecked*/ ) );
		m_spSurface->StrikeSkew[dtExp]->BasePointsInterpolation = bIsChecked ? VARIANT_FALSE : VARIANT_TRUE;

		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (dtExp) );

		m_nMouseRow = -1;
		m_nMouseCol = -1;

		return;
	}

	if( Button == 2 )
	{
		if( m_nMouseCol < 1 || m_nMouseRow < 2 )
		{
			m_nMouseRow = -1;
			m_nMouseCol = -1;

			return;
		}

		_variant_t vtData( m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(m_nMouseRow), CComVariant(m_nMouseCol), vtMissing, vtMissing) );
		if( vtData == vtMissing )
		{
			m_nMouseRow = -1;
			m_nMouseCol = -1;

			return;
		}

		UINT uMakeBaseItemFlags   = MF_STRING;
		UINT uMakeCommonItemFlags = MF_STRING;

		try
		{
			double dStrike = m_fgVola.GetRowData( m_nMouseRow ).dblVal;
			DATE   dtExp   = m_fgVola.GetColData( m_nMouseCol ).date;

			CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExp];
			bool bIsBasePoint = spSkew->IsBasePoint[dStrike] == VARIANT_TRUE ? true : false;

			if( spSkew->Status[dStrike] == Deleted )
			{
				uMakeBaseItemFlags   |= MF_GRAYED;
				uMakeCommonItemFlags |= MF_GRAYED;
			}
			else if( bIsBasePoint )
				uMakeBaseItemFlags   |= MF_GRAYED;
			else
				uMakeCommonItemFlags |= MF_GRAYED;
		}
		catch( ... )
		{
			uMakeBaseItemFlags   |= MF_GRAYED;
			uMakeCommonItemFlags |= MF_GRAYED;
		}

		CMenu popupMenu;

		CPoint pt;
		GetCursorPos( &pt );

		popupMenu.CreatePopupMenu();
		popupMenu.AppendMenu( uMakeBaseItemFlags,   IDC_VM_MAKE_BASE_POINT,    _T("Make Base") );
		popupMenu.AppendMenu( uMakeCommonItemFlags, IDC_VM_MAKE_COMOMON_POINT, _T("Make Common") );
		
		popupMenu.TrackPopupMenu( TPM_RIGHTBUTTON, pt.x, pt.y, this ); 

		popupMenu.DestroyMenu();
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmMakeBasePoint() 
{
	if( m_nMouseCol < 1 || m_nMouseRow < 2 )
		return;

	try
	{
		double dStrike   = m_fgVola.GetRowData( m_nMouseRow ).dblVal;
		DATE   dtExpDate = m_fgVola.GetColData( m_nMouseCol ).date;

		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
		spSkew->IsBasePoint[dStrike] = VARIANT_TRUE;

		GridInit();
		GridFill();
		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enPoint, (dtExpDate) );
	}
	catch( ... )
	{
	}

	m_nMouseRow = -1;
	m_nMouseCol = -1;
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmMakeComomonPoint() 
{
	if( m_nMouseCol < 1 || m_nMouseRow < 2 )
		return;

	try
	{
		double dStrike   = m_fgVola.GetRowData( m_nMouseRow ).dblVal;
		DATE   dtExpDate = m_fgVola.GetColData( m_nMouseCol ).date;

		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
		spSkew->IsBasePoint[dStrike] = VARIANT_FALSE;

		GridInit();
		GridFill();
		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enPoint, (dtExpDate) );
	}
	catch( ... )
	{
	}

	m_nMouseRow = -1;
	m_nMouseCol = -1;
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmSurfaceOpen() 
{
	long nCurrentSurface = m_nCurrentSurface;
	CVolaOpenSurfaceDlg dlg( m_vecSurfaces, nCurrentSurface, this );

	int nRes = dlg.DoModal();
	if( nRes != IDOK )
		return;

	if( nCurrentSurface != m_nCurrentSurface )
		_ChangeCurrentSurface( nCurrentSurface );

	try
	{
		SetupControls();
		GridInit();
		GridFill();

		g_DataProcessor.GetVolaManager()->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())] = m_vecSurfaces[m_nCurrentSurface].m_nID;
		PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error opening surface. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmSurfaceSetAsDefault() 
{
	try
	{
		for( size_t i = 0; i < m_vecSurfaces.size(); i++ )
			if( i != m_nCurrentSurface )
				m_vecSurfaces[i].m_bIsDefault = false;
			else
				m_vecSurfaces[i].m_bIsDefault = true;

		// Save volatility data
		m_spSurface->Save();

		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		g_DataProcessor.GetVolaManager()->DefaultSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())] = sd.m_nID;

		SetupControls();
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error setting current surface as default. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmSurfaceNextday() 
{
	try
	{

		CDBLayout db;
		db.Connect();

		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		if( !sd.m_bIsNextDaySurface )
		{
			for( size_t i = 0; i < m_vecSurfaces.size(); i++ )
				if( m_vecSurfaces[i].m_bIsNextDaySurface )
				{
					m_vecSurfaces[i].m_bIsNextDaySurface = false;
					db.NextDaySurfaceDel( -1, m_nContractID, m_vecSurfaces[i].m_nID );
				}
			sd.m_bIsNextDaySurface = true;
			
			COleDateTime date = COleDateTime::GetCurrentTime();
			long nDayOfWeek;
			do
			{
				date += COleDateTimeSpan( 1, 0, 0, 0 );

				nDayOfWeek = date.GetDayOfWeek();
			}
			while( nDayOfWeek == 1 || nDayOfWeek == 7 );

			long nNewID = -1;
			db.NextDaySurfaceSave( nNewID, m_nContractID, sd.m_nID, (DATE)date );
		}
		else
		{
			sd.m_bIsNextDaySurface = false;

			db.NextDaySurfaceDel( -1, m_nContractID, sd.m_nID );
		}

		SetupControls();
		PostNotification( WM_NEXTDAY_SURFACE_LIST_CHANGED );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error setting current surface as default. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmSurfaceNew() 
{
	long nCurrentSurface = m_nCurrentSurface;
	CVolaAddNewSurfaceDlg dlg( CVolaAddNewSurfaceDlg::enNew, m_vecSurfaces, nCurrentSurface, this );

	int nRes = dlg.DoModal();
	if( nRes != IDOK )
		return;

	UpdateData( FALSE );

	CSurfaceData s;
	s.m_nID					 = -1;
	s.m_strName				 = dlg.m_strName;
	s.m_dUnderlinePrice		 = g_DataProcessor.GetContractPrice();
	s.m_nInterpolationFactor = -1;
	s.m_dStrikeMoneynessLo	 = -1;
	s.m_dStrikeMoneynessHi	 = -1;
	s.m_bIsDefault			 = false;
	s.m_bIsPriceOverride	 = 0;
	s.m_bIsDiscreteAccel     = 0;
	s.m_dSmileAccelerator	 = -1;

	try
	{
		CDBLayout db;
		db.Connect();

		s.m_nID = db.VolaSurfaceDataSave( s.m_nID, m_nContractID,
										  s.m_dUnderlinePrice, 
										  s.m_dSmileAccelerator, 
										  s.m_nInterpolationFactor/100., 
										  s.m_dStrikeMoneynessHi, s.m_dStrikeMoneynessLo, 
										  s.m_strName, 
										  s.m_bIsPriceOverride, s.m_bIsDiscreteAccel,
										  s.m_bIsDefault );

		m_vecSurfaces.push_back( s );
		long nCurrentSurface = m_vecSurfaces.size() - 1;
		_ChangeCurrentSurface( nCurrentSurface );

		SetupControls();
		GridInit();
		GridFill();

		g_DataProcessor.GetVolaManager()->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())] = m_vecSurfaces[m_nCurrentSurface].m_nID;
		PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error adding new surface. ");
		message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmSurfaceSaveas() 
{
	long nCurrentSurface   = m_nCurrentSurface;
	long nCurrentSurfaceID = m_vecSurfaces[m_nCurrentSurface].m_nID;

	CVolaAddNewSurfaceDlg dlg( CVolaAddNewSurfaceDlg::enSaveAs, m_vecSurfaces, nCurrentSurface, this );
	int nRes = dlg.DoModal();
	if( nRes != IDOK )
		return;

	const sUndPrice& priceData = g_DataProcessor.GetUndPrice();

	UpdateData( FALSE );

	CSurfaceData s;
	s.m_nID					 = -1;
	s.m_strName				 = dlg.m_strName;
	s.m_dUnderlinePrice		 = m_edtUnderlinePrice.GetValue();
	s.m_nInterpolationFactor = m_nIFactor;
	s.m_dStrikeMoneynessLo	 = m_edtLoVoly.GetValue();
	s.m_dStrikeMoneynessHi	 = m_edtHiVoly.GetValue();
	s.m_bIsDefault			 = false;
	s.m_bIsPriceOverride	 = m_chkPriceOverride.GetCheck();
	s.m_bIsDiscreteAccel     = m_chkDiscreteAccel.GetCheck();
	s.m_dSmileAccelerator	 = m_edtAccelerator.GetValue();

	try
	{
		CDBLayout db;
		db.Connect();

		s.m_nID = db.VolaSurfaceDataSave( s.m_nID, m_nContractID,
										  s.m_dUnderlinePrice, 
										  s.m_dSmileAccelerator, 
										  s.m_nInterpolationFactor/100., 
										  s.m_dStrikeMoneynessHi, s.m_dStrikeMoneynessLo, 
										  s.m_strName, 
										  s.m_bIsPriceOverride, s.m_bIsDiscreteAccel,
										  s.m_bIsDefault );
		m_spSurface->SaveAs( s.m_nID, false );

		m_vecSurfaces.push_back( s );
		long nCurrentSurface = m_vecSurfaces.size() - 1;
		_ChangeCurrentSurface( nCurrentSurface );

		SetupControls();
		GridInit();
		GridFill();

		g_DataProcessor.GetVolaManager()->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())] = m_vecSurfaces[m_nCurrentSurface].m_nID;
		PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error saving surface. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmSurfaceEdit() 
{
	long nCurrentSurface = m_nCurrentSurface;
	CVolaAddNewSurfaceDlg dlg( CVolaAddNewSurfaceDlg::enEdit, m_vecSurfaces, nCurrentSurface, this );

	int nRes = dlg.DoModal();
	if( nRes != IDOK )
		return;

	CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

	try
	{
		CDBLayout db;
		db.Connect();

		db.VolaSurfaceDataSave( sd.m_nID, m_nContractID,
								-1, -1, 
								-1, -1, -1, dlg.m_strName, 
								-1, -1,
								sd.m_bIsDefault );
		
		SetupControls();
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error modifying surface. ");
		message += e.Description().length() ?(LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmSurfaceDelete() 
{
	int nRes = MessageBox( _T("Do you realy want to delete surface?"), NULL, MB_YESNO|MB_ICONQUESTION );
	if( nRes != IDYES )
		return;

	try
	{
		CDBLayout db;
		db.Connect();

		db.VolaSurfaceDataDel( m_vecSurfaces[m_nCurrentSurface].m_nID );

		SetupSurfaces();
		SetupControls();
		GridInit();
		GridFill();

		g_DataProcessor.GetVolaManager()->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())] = m_vecSurfaces[m_nCurrentSurface].m_nID;
		PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );
		PostNotification( WM_NEXTDAY_SURFACE_LIST_CHANGED );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error deleting surface. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmViewGenerationOptions() 
{
	CMenu* pMenu  = GetMenu()->GetSubMenu( 1 );
	UINT   uState = pMenu->GetMenuState( IDC_VM_VIEW_GENERATION_OPTIONS, MF_BYCOMMAND );

	if( uState & MF_CHECKED )
	{
		pMenu->CheckMenuItem( IDC_VM_VIEW_GENERATION_OPTIONS, MF_BYCOMMAND|MF_UNCHECKED );
		_AlignControls( false );
	}
	else
	{
		pMenu->CheckMenuItem( IDC_VM_VIEW_GENERATION_OPTIONS, MF_BYCOMMAND|MF_CHECKED );
		_AlignControls( true );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmViewAlwaysontop() 
{
	m_bAlwaysOnTop = !m_bAlwaysOnTop;
	SetupControls();
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVmViewShowAll() 
{
	m_bUseExtrapolation = !m_bUseExtrapolation;
	SetupControls();

	if( m_bUseExtrapolation )
	{
		_ExtrapolateVolatility();

		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnNewRule() 
{
	long nOldRuleType = m_vecRules[m_nCurrentRule].m_nType;

	CRuleData rdNew;

	long i = 0;
	CString strName;

	do
	{
		i++;
		strName.Format(_T("%s %d"), _T("New Rule"), i);
	}
	while( LB_ERR != m_cmbCurrentRule.FindStringExact(-1, strName) );
	
	rdNew.m_nID = -1;
	rdNew.m_nType = RULE_TYPE_VOLA_PTS;
	rdNew.m_strName = strName;
	rdNew.m_bIsNew  = true;
	rdNew.m_dStrikeMoneynessLo = 10.0;
	rdNew.m_dStrikeMoneynessHi = 150.0;
	rdNew.m_nStrikeIncrement   = 1;
	rdNew.m_nRequiredPoints    = 1;
	rdNew.m_dCallShift		   = 100;
	rdNew.m_dPutShift		   = 100;
	m_vecRules.push_back(rdNew);

	CRuleDefinitionDlg dlg( this, m_vecRules, m_vecRules.size() - 1 );
	
	if( dlg.DoModal() == IDOK )
	{
		m_nCurrentRule = m_vecRules.size() - 1;

		// change list box string
		CRuleData& rdCur = m_vecRules[m_nCurrentRule];
		rdCur.m_bIsNew = false;
		m_cmbCurrentRule.InsertString( m_nCurrentRule, rdCur.m_strName );
		m_cmbCurrentRule.SetCurSel( m_nCurrentRule );

		try
		{
			CDBLayout db;
			db.Connect();

			db.SaveGenerationRule(rdCur.m_nID, rdCur.m_nType, (LPCTSTR)rdCur.m_strName, rdCur.m_nStrikeIncrement, rdCur.m_nRequiredPoints, 
								  rdCur.m_dSmileAccelerator, rdCur.m_dCallShift, rdCur.m_dPutShift,
								  rdCur.m_dInterpolationFactor, rdCur.m_dStrikeMoneynessHi, rdCur.m_dStrikeMoneynessLo, rdCur.m_bIsDefault);

			SetupControls();

			if( nOldRuleType != RULE_TYPE_CURVE_FIT && rdCur.m_nType == RULE_TYPE_CURVE_FIT )
				_UnbasePoints();

			GridInit();
			GridFill();
		}
		catch( const _com_error& e )
		{
			CString message = _T("Error adding new rule. ");
			message += e.Description().length() ?(LPCSTR)e.Description() : e.ErrorMessage();
			MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
		}

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
	}
	else
	{
		m_vecRules.erase( m_vecRules.end() - 1 );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnUpdateRule() 
{
	CRuleDefinitionDlg dlg( this, m_vecRules, m_nCurrentRule );

	CRuleData& rdCur = m_vecRules[m_nCurrentRule];
	long nOldRuleType = rdCur.m_nType;

	if( dlg.DoModal() == IDOK )
	{
		// change list box string
		m_cmbCurrentRule.DeleteString( m_nCurrentRule );
		m_cmbCurrentRule.InsertString( m_nCurrentRule, rdCur.m_strName );
		m_cmbCurrentRule.SetItemData(  m_nCurrentRule, rdCur.m_nID );
		m_cmbCurrentRule.SetCurSel( m_nCurrentRule );

		try
		{
			CDBLayout db;
			db.Connect();

			db.SaveGenerationRule(rdCur.m_nID, rdCur.m_nType, (LPCTSTR)rdCur.m_strName, rdCur.m_nStrikeIncrement, rdCur.m_nRequiredPoints, 
								  rdCur.m_dSmileAccelerator, rdCur.m_dCallShift, rdCur.m_dPutShift,
								  rdCur.m_dInterpolationFactor, rdCur.m_dStrikeMoneynessHi, rdCur.m_dStrikeMoneynessLo, rdCur.m_bIsDefault);

			SetupControls();

			if( nOldRuleType != RULE_TYPE_CURVE_FIT && rdCur.m_nType == RULE_TYPE_CURVE_FIT )
				_UnbasePoints();

			GridInit();
			GridFill();
		}
		catch( const _com_error& e )
		{
			CString message = _T("Error saving rule. ");
			message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
			MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
		}

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnDeleteRule() 
{
	int nRes = MessageBox( _T("Do you realy want to delete current rule?"), NULL, MB_YESNO|MB_ICONQUESTION );
	if( nRes != IDYES )
		return;

	try
	{
		CDBLayout db;
		db.Connect();

		db.DeleteGenerationRule( m_vecRules[m_nCurrentRule].m_nID );

		SetupRules();
		SetupControls();
		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		if( e.Error() == DB_E_INTEGRITYVIOLATION )
		{
			CString message;
			message.Format( _T("Can't delete rule \"%s\" because it's connected to stock group."), m_vecRules[m_nCurrentRule].m_strName );
			AfxMessageBox( message, MB_OK | MB_ICONERROR );
		}
		else
		{
			CString message = _T("Error deleting rule. ");
			message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
			MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnChangeHiVolatility() 
{
	if( !m_bEditing && m_bInitialized )
	{
		m_bEditing = true;

		m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessHi = m_edtHiVoly.GetValue();
		if( m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessHi < 100.0 )
		{
			m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessHi = 100.0;

			m_edtHiVoly.SetValue( m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessHi ); 
		}

		m_bEditing = false;
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnChangeLoVolatility() 
{
	if( !m_bEditing && m_bInitialized )
	{
		m_bEditing = true;

		m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessLo = m_edtLoVoly.GetValue();
		if( m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessLo > 100.0 )
		{
			m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessLo = 100.0;

			m_edtLoVoly.SetValue( m_vecSurfaces[m_nCurrentSurface].m_dStrikeMoneynessLo ); 
		}

		m_bEditing = false;
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnCommitUnderlinePrice()
{
	if( !m_bEditing && m_bInitialized )
	{
		m_bEditing = true;

		try
		{
			m_vecSurfaces[m_nCurrentSurface].m_dUnderlinePrice = m_edtUnderlinePrice.GetValue();
			m_spSurface->UnderlinePrice = m_vecSurfaces[m_nCurrentSurface].m_dUnderlinePrice;

			SetupControls();
			GridInit();
			GridFill();

			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
		}
		catch( const _com_error& e )
		{
			CString message = _T("Error updating underline price. ");
		message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
			MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
		}

		m_bEditing = false;
	}
}


LRESULT CVolaManagementDlg::OnUpdateView( WPARAM wParam, LPARAM lParam )
{
	GridInit();
	GridFill();

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnPriceOverrideCheck() 
{
	try
	{
		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

		sd.m_bIsPriceOverride = m_chkPriceOverride.GetCheck() != 0;
		if( sd.m_bIsPriceOverride )
		{
			m_spSurface->PriceOverride  = sd.m_bIsPriceOverride ? VARIANT_TRUE : VARIANT_FALSE;
			m_spSurface->UnderlinePrice = sd.m_dUnderlinePrice;
		}
		else
		{
			sd.m_dUnderlinePrice = g_DataProcessor.GetContractPrice();
			m_spSurface->UnderlinePrice = sd.m_dUnderlinePrice;
			m_spSurface->PriceOverride  = sd.m_bIsPriceOverride ? VARIANT_TRUE : VARIANT_FALSE;
		}

		SetupControls();
		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error updating price override property. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnChangeAccelerator() 
{
	if( !m_bEditing && m_bInitialized )
	{
		m_bEditing = true;

		try
		{
			double dAccel = m_edtAccelerator.GetValue();
			if( dAccel < 0.0 || dAccel > 1.0 )
			{
				if( m_vecSurfaces[m_nCurrentSurface].m_dSmileAccelerator != -1 )
					m_edtAccelerator.SetValue( m_vecSurfaces[m_nCurrentSurface].m_dSmileAccelerator );
				else
					m_edtAccelerator.SetValue( m_vecRules[m_nCurrentRule].m_dSmileAccelerator );
			}
			else if( m_vecSurfaces[m_nCurrentSurface].m_dSmileAccelerator != m_edtAccelerator.GetValue() && 
					 m_vecSurfaces[m_nCurrentSurface].m_dSmileAccelerator != -1
				   )
			{
				m_vecSurfaces[m_nCurrentSurface].m_dSmileAccelerator = m_edtAccelerator.GetValue();
				m_spSurface->SmileAccelerator = m_vecSurfaces[m_nCurrentSurface].m_dSmileAccelerator;
			}
		}
		catch( const _com_error& e )
		{
			CString message;
			message.Format( _T("Error updating smile accelerator. (%X) "), e.Error() );
			message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
			if( e.Error() == RPC_E_CANTCALLOUT_ININPUTSYNCCALL )
				TRACE( message );
			else
				MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
		}

		m_bEditing = false;
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnDiscreteAccelCheck() 
{
	try
	{
		m_vecSurfaces[m_nCurrentSurface].m_bIsDiscreteAccel = m_chkDiscreteAccel.GetCheck() != 0;
		m_spSurface->DiscreteAcceleration = m_vecSurfaces[m_nCurrentSurface].m_dSmileAccelerator ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error updating discrete acceleration property. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaRegenerate() 
{
	if( m_nCurrentSurface == -1 || m_nCurrentRule == -1 )
	{
		MessageBox( _T("Error generating volatility - either surface or rule not set."), NULL, MB_OK|MB_ICONSTOP );
		return;
	}

	CWaitCursor wait;
		
	try
	{
		long nCols = m_fgVola.GetCols();

		CDBLayout db;
		db.Connect();

		// Save surface properties
		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

		sd.m_dSmileAccelerator	  = m_edtAccelerator.GetValue();
		sd.m_dUnderlinePrice	  = m_edtUnderlinePrice.GetValue();
		sd.m_nInterpolationFactor = m_nIFactor;
		sd.m_dStrikeMoneynessLo	  = m_edtLoVoly.GetValue();
		sd.m_dStrikeMoneynessHi	  = m_edtHiVoly.GetValue();
		sd.m_bIsPriceOverride	  = m_chkPriceOverride.GetCheck();
		sd.m_bIsDiscreteAccel     = m_chkDiscreteAccel.GetCheck();

		// Save contract properties
		db.ContractGenerationRuleSave( m_nContractID, m_vecRules[m_nCurrentRule].m_nID );

		const CRuleData& rd = m_vecRules[m_nCurrentRule];
		long nRuleType = rd.m_nType;

		// For every month
		for( long col = 1; col < m_fgVola.GetCols(); col++ )
		{
			_variant_t vtCheck( m_fgVola.GetCell( 5/*flexcpChecked*/, CComVariant(1), CComVariant(col), vtMissing, vtMissing ) );
			if( vtCheck.uiVal != 1 /*flexChecked*/ )
				continue;

			long row = 2;
			m_bNeedSave = true;

			DATE dtExpDate = m_fgVola.GetColData( col ).date;

			m_spSurface->UnderlinePrice	      = sd.m_dUnderlinePrice;
			m_spSurface->InterpolationFactor  = 1 - m_dIFactor;
			m_spSurface->SmileAccelerator     = sd.m_dSmileAccelerator;
			m_spSurface->DiscreteAcceleration = sd.m_bIsDiscreteAccel ? VARIANT_TRUE : VARIANT_FALSE;

			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];

			long   nATMRow    = _GetATMStrikeRow( col );
			double dATMStrike = m_fgVola.GetRowData( nATMRow ).dblVal;
			double dStrikeHi = dATMStrike * sd.m_dStrikeMoneynessHi / 100.0f;
			double dStrikeLow = dATMStrike * sd.m_dStrikeMoneynessLo / 100.0f;
			double dStrikePostLow, dStrikePreHi;
			dStrikePostLow = dStrikePreHi = dATMStrike;

			if( g_DataProcessor.GetFlatOutRange()) // if flat wings
			{
				for( row = nATMRow - 1; row > 1; row-- )
				{
					// Check if strike in range
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
					if( dRowStrike < dStrikeLow )
						break;

					dStrikePostLow = dRowStrike;
				}
				for( row = nATMRow + 1; row < m_fgVola.GetRows(); row++ )
				{
					// Check if strike in range
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
					if( dRowStrike > dStrikeHi )
						break;

					dStrikePreHi = dRowStrike;
				}
			}


			if( nRuleType == RULE_TYPE_VOLA_PTS  || 
				nRuleType == RULE_TYPE_VOLA_PERC || 
				nRuleType == RULE_TYPE_VOLA_PERC_PREV )
			{
				spSkew->RemoveAll();

				// Add ATM point
				double dATMVola   = 0;
				_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(nATMRow), CComVariant(col), vtMissing, vtMissing));
				if( vtData.vt == VT_R8 && vtData.dblVal != 0 )
				{
					dATMVola = vtData.dblVal;
					spSkew->Add( dATMStrike, dATMVola, VARIANT_TRUE );
				}
				else
				{
					PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (dtExpDate) );
					continue;
				}

				long nStrikeInc = rd.m_nStrikeIncrement != 0 ? abs(rd.m_nStrikeIncrement) : 1;
				long nSteps = 0;

				// Generate puts
				double dRowVola   = dATMVola;

				VARIANT_BOOL vtbIsBase = VARIANT_FALSE;

				nSteps = 0;
				for( row = nATMRow - 1; row > 1; row-- )
				{
					// Check if strike in range
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
					if( dRowStrike < dStrikeLow )
						break;

					// Check if there is option for strike
					_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData == vtMissing )
						continue;

					nSteps++;
					vtbIsBase = nSteps % nStrikeInc == 0 ? VARIANT_TRUE : VARIANT_FALSE;

					// Get vola
					if( vtbIsBase == VARIANT_TRUE )
					{
						if( nRuleType == RULE_TYPE_VOLA_PTS )
							dRowVola += rd.m_dPutShift / 100.0f;
						else if( nRuleType == RULE_TYPE_VOLA_PERC )
							dRowVola += dATMVola * rd.m_dPutShift / 100;
						else if( nRuleType == RULE_TYPE_VOLA_PERC_PREV )
							dRowVola += dRowVola * rd.m_dPutShift / 100;
					}

					// Add point
					spSkew->Add( dRowStrike, dRowVola, vtbIsBase );
				}

				// Generate calls
				dRowVola = dATMVola;

				nSteps = 0;
				for( row = nATMRow + 1; row < m_fgVola.GetRows(); row++ )
				{
					// Check if strike in range
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
					if( dRowStrike > dStrikeHi )
						break;

					// Check if there is option for strike
					_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData == vtMissing )
						continue;

					nSteps++;
					vtbIsBase = nSteps % nStrikeInc == 0 ? VARIANT_TRUE : VARIANT_FALSE;

					// Get vola
					if( vtbIsBase == VARIANT_TRUE )
					{
						if( nRuleType == RULE_TYPE_VOLA_PTS )
							dRowVola += rd.m_dCallShift / 100.0f;
						else if( nRuleType == RULE_TYPE_VOLA_PERC )
							dRowVola += dATMVola * rd.m_dCallShift / 100;
						else if( nRuleType == RULE_TYPE_VOLA_PERC_PREV )
							dRowVola += dRowVola * rd.m_dCallShift / 100;
					}

					// Add point
					spSkew->Add( dRowStrike, dRowVola, vtbIsBase );
				}
			}
			else if( nRuleType == RULE_TYPE_CURVE_FIT )
			{
				if( rd.m_nRequiredPoints > spSkew->BasePointsCount )
				{
					CString strMessage;
					strMessage.Format( _T("Not enough points for %s. Enter some points or update current rule."), m_fgVola.GetTextMatrix(0, col) );

					MessageBox( strMessage, NULL, MB_OK|MB_ICONINFORMATION );
					continue;
				}

				// Remove all common points
				for( row = 2; row < m_fgVola.GetRows(); row++ )
				{
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
	
					// Check if there is option for strike
					_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData == vtMissing )
						continue;

					// Skip base points
					try
					{
						if( spSkew->IsBasePoint[dRowStrike] == VARIANT_TRUE )
							continue;
						spSkew->Remove( dRowStrike );
					}
					catch( ... )
					{
					}
				}

				for( row = 2; row < m_fgVola.GetRows(); row++ )
				{
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
					if( dStrikeLow > dRowStrike )
						continue;
					else if( dStrikeHi < dRowStrike )
						break;

					// Check if there is option for strike
					_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData == vtMissing )
						continue;

					// Skip base points
					try
					{
						if( spSkew->IsBasePoint[dRowStrike] == VARIANT_TRUE )
							continue;
					}
					catch( ... )
					{
					}

					// Get interpolated vola
					double dVola = spSkew->Volatility[dRowStrike];

					// Add point
					spSkew->Add( dRowStrike, dVola, VARIANT_FALSE );
				}
			}
			else if( nRuleType == RULE_TYPE_DUAL_QUADRATIC )
			{
				spSkew->RemoveAll();

				// Add ATM point
				double dATMVola   = 0;
				_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(nATMRow), CComVariant(col), vtMissing, vtMissing));
				if( vtData.vt == VT_R8 && vtData.dblVal != 0 )
				{
					dATMVola = vtData.dblVal;
					spSkew->Add( dATMStrike, dATMVola, VARIANT_TRUE );
				}
				else
				{
					CString strMessage;
					strMessage.Format( _T("Not enough data for %s. Enter ATM Vola or update current rule."), m_fgVola.GetTextMatrix(0, col) );

					MessageBox( strMessage, NULL, MB_OK|MB_ICONINFORMATION );
					continue;
				}

				VME::IDualQuadraticDataPtr spDQData;
				spSkew->get_DualQuadraticParams(&spDQData);

				double dCoefA1,dCoefA2,dCoefB1,dCoefB2;
				spDQData->get_CoefA1(&dCoefA1);
				spDQData->get_CoefA2(&dCoefA2);
				spDQData->get_CoefB1(&dCoefB1);
				spDQData->get_CoefB2(&dCoefB2);

				// Generate puts

				for( row = nATMRow - 1; row > 1; row-- )
				{
					// Check if strike in range
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
					if( dRowStrike < dStrikeLow )
						break;

					// Check if there is option for strike
					_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData == vtMissing )
						continue;
					
					double dVol = dCoefA1 * ((dRowStrike - dATMStrike) * (dRowStrike - dATMStrike)) + dCoefB1 * ( dRowStrike - dATMStrike ) + dATMVola ;

					dVol = dVol < 0 ? 0. : dVol;

					// Add point
					spSkew->Add( dRowStrike, dVol, VARIANT_TRUE );
				}

				// Generate calls

				for( row = nATMRow + 1; row < m_fgVola.GetRows(); row++ )
				{
					// Check if strike in range
					double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
					if( dRowStrike > dStrikeHi )
						break;

					// Check if there is option for strike
					_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData == vtMissing )
						continue;
					
					double dVol = dCoefA2 * ((dRowStrike - dATMStrike) * (dRowStrike - dATMStrike)) + dCoefB2 * (dRowStrike - dATMStrike) + dATMVola;

					dVol = dVol < 0 ? 0. : dVol;

					// Add point
					spSkew->Add( dRowStrike, dVol, VARIANT_TRUE );
					
				}

			}

			if( m_bUseExtrapolation )
			{
				if(g_DataProcessor.GetFlatOutRange())
				{
					double dExtrVola = spSkew->Volatility[dStrikePostLow];	
					spSkew->Add( dStrikePostLow, dExtrVola, VARIANT_TRUE );
					dExtrVola = spSkew->Volatility[dStrikePreHi];	
					spSkew->Add( dStrikePreHi, dExtrVola, VARIANT_TRUE );
				}

				_ExtrapolateMonthVolatility( col, spSkew, g_DataProcessor.GetFlatOutRange() ,dStrikePostLow,  dStrikePreHi);
			}

			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (dtExpDate) );
		}

		SetupControls();
		GridInit();
		GridFill();
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error generating volatility. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaClear() 
{
	CWaitCursor wait;

	try
	{
		long nCols = m_fgVola.GetCols();

		// For every month
		for( long col = 1; col < m_fgVola.GetCols(); col++ )
		{
			_variant_t vtCheck( m_fgVola.GetCell( 5/*flexcpChecked*/, CComVariant(1), CComVariant(col), vtMissing, vtMissing ) );
			if( vtCheck.uiVal != 1 /*flexChecked*/ )
				continue;

			DATE dtExpDate = m_fgVola.GetColData( col ).date;

			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
			spSkew->RemoveAll();

			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (dtExpDate) );
		}

		GridInit();
		GridFill();
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error clearing volatility. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


void CVolaManagementDlg::OnVolaFittoimplied() 
{
	CWaitCursor wait;
	try
	{
		long nCols = m_fgVola.GetCols();

		// Save surface properties
		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

		sd.m_dSmileAccelerator	  = m_edtAccelerator.GetValue();
		sd.m_dUnderlinePrice	  = m_edtUnderlinePrice.GetValue();
		sd.m_nInterpolationFactor = m_nIFactor;
		sd.m_dStrikeMoneynessLo	  = m_edtLoVoly.GetValue();
		sd.m_dStrikeMoneynessHi	  = m_edtHiVoly.GetValue();
		sd.m_bIsPriceOverride	  = m_chkPriceOverride.GetCheck();
		sd.m_bIsDiscreteAccel     = m_chkDiscreteAccel.GetCheck();

		// For every month
		for( long col = 1; col < m_fgVola.GetCols(); col++ )
		{
			_variant_t vtCheck( m_fgVola.GetCell( 5/*flexcpChecked*/, CComVariant(1), CComVariant(col), vtMissing, vtMissing ) );
			if( vtCheck.uiVal != 1 /*flexChecked*/ )
				continue;

			DATE dtExpDate = m_fgVola.GetColData( col ).date;

			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
			spSkew->RemoveAll();

			long   nATMRow    = _GetATMStrikeRow( col );
			double dATMStrike = m_fgVola.GetRowData( nATMRow ).dblVal;
			double dStrikeFirstWithBid, dStrikeLastWithBid;
			dStrikeFirstWithBid = dStrikeLastWithBid = dATMStrike;

			bool bFlatNoBid =  g_DataProcessor.GetFlatNoBid() ;

			strikes_map strikesData;
			{
				CDataProcessor::CLockVola lv;

				COleDateTime dtExp;
				long nCntVola = g_DataProcessor.GetOptionsDataByEpiration( dtExpDate, dtExp, strikesData );
				if( nCntVola == 0 )
					continue;
				long row;

				if(bFlatNoBid)
				{

					// Add ATM point
					double dATMVola   = 0.0f;
					strikes_map::iterator it = strikesData.find( dATMStrike );
					if( it != strikesData.end() )
					{
						m_bNeedSave = true;
						spSkew->Add( dATMStrike, it->second->m_dVola * 100, VARIANT_TRUE );

					}
					
					// Generate puts

					for( row = nATMRow - 1; row > 1; row-- )
					{

						double dRowStrike = m_fgVola.GetRowData( row ).dblVal;

						it = strikesData.find( dRowStrike );
						if( it != strikesData.end() )
						{
							m_bNeedSave = true;

							if(it->second->m_dPriceBid != 0 && it->second->m_dPriceBid > BAD_DOUBLE_VALUE)
							{
								dStrikeFirstWithBid = dRowStrike;
								spSkew->Add( dRowStrike, it->second->m_dVola * 100, VARIANT_TRUE );
							}
						}
					}

					// Generate calls

					for( row = nATMRow + 1; row < m_fgVola.GetRows(); row++ )
					{
						double dRowStrike = m_fgVola.GetRowData( row ).dblVal;
						
						it = strikesData.find( dRowStrike );
						if( it != strikesData.end() )
						{
							m_bNeedSave = true;

							if(it->second->m_dPriceBid != 0 && it->second->m_dPriceBid > BAD_DOUBLE_VALUE)
							{
								dStrikeLastWithBid = dRowStrike;
								spSkew->Add( dRowStrike, it->second->m_dVola * 100, VARIANT_TRUE );
							}
						}
					}

				}
				else
				{
				
					for( row = 2; row < m_fgVola.GetRows(); row++ )
					{
						double dRowStrike = m_fgVola.GetRowData( row ).dblVal;

						strikes_map::iterator it = strikesData.find( dRowStrike );
						if( it != strikesData.end() )
						{
							m_bNeedSave = true;
							spSkew->Add( dRowStrike, it->second->m_dVola * 100, VARIANT_TRUE );
						}
					}
				}
			}

			if( m_bUseExtrapolation )
			{
				if(bFlatNoBid)
				{
					double dExtrVola = spSkew->Volatility[dStrikeFirstWithBid];	
					spSkew->Add( dStrikeFirstWithBid, dExtrVola, VARIANT_TRUE );
					dExtrVola = spSkew->Volatility[dStrikeLastWithBid];	
					spSkew->Add( dStrikeLastWithBid, dExtrVola, VARIANT_TRUE );
				}
				_ExtrapolateMonthVolatility( col, spSkew,bFlatNoBid,dStrikeFirstWithBid,dStrikeLastWithBid );
			}

			PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (dtExpDate) );
		}
	
		GridInit();
		GridFill();
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error generation volatility by implied. ");
		message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaUpdate() 
{
	CWaitCursor wait;
	m_bNeedSave = false;
	try
	{
		long nCols = m_fgVola.GetCols();

		CDBLayout db;
		db.Connect();

		// Save surface properties
		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];
		db.VolaSurfaceDataSave( sd.m_nID, m_nContractID,
								sd.m_dUnderlinePrice, sd.m_dSmileAccelerator, 
								sd.m_nInterpolationFactor/100., sd.m_dStrikeMoneynessHi, sd.m_dStrikeMoneynessLo, sd.m_strName, 
								sd.m_bIsPriceOverride, sd.m_bIsDiscreteAccel,
								sd.m_bIsDefault );

		// Save volatility data
		m_spSurface->Save();

		for( long Col = 1; Col < m_fgVola.GetCols(); Col++ )
		{
			DATE dtExp = m_fgVola.GetColData( Col ).date;

			m_spSurface->StrikeSkew[dtExp]->BasePointsInterpolation = VARIANT_FALSE;
		}

		if( m_bNexdDaySurfaceMode )
		{
			COleDateTime date = COleDateTime::GetCurrentTime();
			long nDayOfWeek;
			do
			{
				date += COleDateTimeSpan( 1, 0, 0, 0 );

				nDayOfWeek = date.GetDayOfWeek();
			}
			while( nDayOfWeek == 1 || nDayOfWeek == 7 );

			long nNewID = -1;
			db.NextDaySurfaceSave( nNewID, m_nContractID, sd.m_nID, (DATE)date );

			m_bNexdDaySurfaceMode = false;
		}

		SetupControls();
		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_SAVED, CCustomVolaNotification::enSkew, 0 );
		PostNotification( WM_NEXTDAY_SURFACE_LIST_CHANGED );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error updating volatility. ");
		message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaManagementDlg::OnVolaRestore() 
{
	CWaitCursor wait;

	m_bNeedSave = false;
	try
	{
		CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

		CDBLayout db;
		db.Connect();
		CClientRecordset rs;

		db.VolaSurfaceDataGet( m_nContractID, sd.m_nID, rs );

		if( !rs.IsEOF() )
		{
			rs.MoveFirst();

			sd.m_nID				 = rs[L"iVolaSurfaceDataID"];
			sd.m_strName			 = (LPCTSTR)(_bstr_t)rs[L"vcSurfaceName"];
			sd.m_dSmileAccelerator   = rs[L"fSmileAccelerator"].IsNull()    ? -1.0f : (double)rs[L"fSmileAccelerator"];
			sd.m_nInterpolationFactor= rs[L"fInterpolationFactor"].IsNull() ? -1 : static_cast<long>((double)rs[L"fInterpolationFactor"] * 100);
			sd.m_dStrikeMoneynessLo	 = rs[L"fStrikeMoneynessLo"].IsNull()   ? -1.0f : (double)rs[L"fStrikeMoneynessLo"];
			sd.m_dStrikeMoneynessHi	 = rs[L"fStrikeMoneynessHi"].IsNull()   ? -1.0f : (double)rs[L"fStrikeMoneynessHi"];
			sd.m_bIsDefault			 = (bool)rs[L"bIsDefaultSurface"];
			sd.m_bIsPriceOverride	 = (bool)rs[L"bIsPriceOverride"];
			sd.m_bIsDiscreteAccel 	 = (bool)rs[L"bIsDiscreteAcceleration"];
			sd.m_dUnderlinePrice = rs[L"fBaseUnderlinePrice"];
			if( !sd.m_bIsPriceOverride && fEQZero( sd.m_dUnderlinePrice ) )
				sd.m_dUnderlinePrice = g_DataProcessor.GetContractPrice();
		}

		m_spSurface->Reload();

		SetupControls();
		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error restoring volatility. ");
		message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}


/////////////////////////////////////////////////////////////////////////////
//	Helpers
/////////////////////////////////////////////////////////////////////////////
long CVolaManagementDlg::_GetATMStrikeRow( long col )
{
	long n = m_fgVola.GetRows();
	long rowsWatched = 0;
	double delta = 0, temp = 0;

	CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

	for( long row = 2; row < m_fgVola.GetRows(); row++ )
	{
		_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
		if( vtData == vtMissing )
			continue;

		temp = fabs( sd.m_dUnderlinePrice - m_fgVola.GetRowData( row ).dblVal );
		if( rowsWatched == 0 || temp < delta )
		{
			n     = row;
			delta = temp;
		}
		rowsWatched++;
	}

	return n;
}


void CVolaManagementDlg::_AlignControls( bool bShowOptions )
{
	LockWindowUpdate();

	CRect rcClient;
	GetClientRect( rcClient );
	rcClient.DeflateRect( m_nMargine, m_nMargine );

	CSize sz;

	if( bShowOptions )
	{
		GetDlgItem( IDC_STATIC_RULE )->ShowWindow( SW_SHOW );
		m_cmbCurrentRule.ShowWindow( SW_SHOW );
		m_btnNewRule.ShowWindow( SW_SHOW );
		m_btnUpdateRule.ShowWindow( SW_SHOW );
		m_btnDeleteRule.ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_IF )->ShowWindow( SW_SHOW );
		m_edtFactor.ShowWindow( SW_SHOW );
		m_sldFactor.ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_LV )->ShowWindow( SW_SHOW );
		m_edtLoVoly.ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_LVP )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_HV )->ShowWindow( SW_SHOW );
		m_edtHiVoly.ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_HVP )->ShowWindow( SW_SHOW );

#ifdef _WSB
		m_edtUnderlinePrice.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_UP )->ShowWindow( SW_HIDE );
		m_edtAccelerator.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_SA )->ShowWindow( SW_HIDE );
		m_chkPriceOverride.ShowWindow( SW_HIDE );
		m_chkDiscreteAccel.ShowWindow( SW_HIDE );
#else
		m_edtUnderlinePrice.ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_UP )->ShowWindow( SW_SHOW );
		m_edtAccelerator.ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_SA )->ShowWindow( SW_SHOW );
		m_chkPriceOverride.ShowWindow( SW_SHOW );
		m_chkDiscreteAccel.ShowWindow( SW_SHOW );
#endif //WSB
	}
	else
	{
		GetDlgItem( IDC_STATIC_RULE )->ShowWindow( SW_HIDE );
		m_cmbCurrentRule.ShowWindow( SW_HIDE );
		m_btnNewRule.ShowWindow( SW_HIDE );
		m_btnUpdateRule.ShowWindow( SW_HIDE );
		m_btnDeleteRule.ShowWindow( SW_HIDE );
		m_chkPriceOverride.ShowWindow( SW_HIDE );
		m_edtUnderlinePrice.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_UP )->ShowWindow( SW_HIDE );
		m_chkDiscreteAccel.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_IF )->ShowWindow( SW_HIDE );
		m_edtFactor.ShowWindow( SW_HIDE );
		m_sldFactor.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_LV )->ShowWindow( SW_HIDE );
		m_edtLoVoly.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_LVP )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_HV )->ShowWindow( SW_HIDE );
		m_edtHiVoly.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_HVP )->ShowWindow( SW_HIDE );
		m_edtAccelerator.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_SA )->ShowWindow( SW_HIDE );
	}

	// Close button
	CRect rc;
	m_btnClose.GetWindowRect( &rc );
	ScreenToClient( &rc );
	InvalidateRect(rc);
	sz = rc.Size();
	rc.left   = rcClient.right  - sz.cx;
	rc.top    = rcClient.bottom - sz.cy;
	rc.right  = rcClient.right;
	rc.bottom = rcClient.bottom;
	m_btnClose.MoveWindow(rc, FALSE);
	m_btnClose.Invalidate();

	// Restore button
	CRect rc1;
	m_btnVolaRestore.GetWindowRect( &rc1 );
	ScreenToClient( &rc1 );
	InvalidateRect( rc1 );
	sz = rc1.Size();
	rc1.right  = rc.left;
	rc1.left   = rc1.right - sz.cx;
	rc1.top    = rcClient.bottom - sz.cy;
	rc1.bottom = rcClient.bottom;
	m_btnVolaRestore.MoveWindow(rc1, FALSE);
	m_btnVolaRestore.Invalidate();

	// Update button
	CRect rc2;
	m_btnVolaUpdate.GetWindowRect( &rc2 );
	ScreenToClient( &rc2 );
	InvalidateRect( rc2 );
	sz = rc2.Size();
	rc2.right  = rc1.left;
	rc2.left   = rc2.right - sz.cx;
	rc2.top    = rcClient.bottom - sz.cy;
	rc2.bottom = rcClient.bottom;
	m_btnVolaUpdate.MoveWindow(rc2, FALSE);
	m_btnVolaUpdate.Invalidate();

	// Clear button
	CRect rc3;
	m_btnVolaClear.GetWindowRect( &rc3 );
	ScreenToClient( &rc3 );
	InvalidateRect( rc3 );
	sz = rc3.Size();
	rc3.right  = rc2.left;
	rc3.left   = rc3.right - sz.cx;
	rc3.top    = rcClient.bottom - sz.cy;
	rc3.bottom = rcClient.bottom;
	m_btnVolaClear.MoveWindow(rc3, FALSE);
	m_btnVolaClear.Invalidate();

	// Regenerate button
	CRect rc4;
	m_btnVolaRegenerate.GetWindowRect( &rc4 );
	ScreenToClient( &rc4 );
	InvalidateRect( rc4 );
	sz = rc4.Size();
	rc4.right  = rc3.left;
	rc4.left   = rc4.right - sz.cx;
	rc4.top    = rcClient.bottom - sz.cy;
	rc4.bottom = rcClient.bottom;
	m_btnVolaRegenerate.MoveWindow(rc4, FALSE);
	m_btnVolaRegenerate.Invalidate();

	// Fit to implied button
	CRect rc5;
	m_btnVolaFitToImplied.GetWindowRect( &rc5 );
	ScreenToClient( &rc5 );
	InvalidateRect( rc5 );
	sz = rc5.Size();
	rc5.right  = rc4.left;
	rc5.left   = rc5.right - sz.cx;
	rc5.top    = rcClient.bottom - sz.cy;
	rc5.bottom = rcClient.bottom;
	m_btnVolaFitToImplied.MoveWindow(rc5, FALSE);
	m_btnVolaFitToImplied.Invalidate();

	rcClient.bottom = rc.top - m_nMargine;

	// Data grid
	m_fgVola.GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect( rc );

	if( !bShowOptions )
	{
		m_cmbCurrentRule.GetWindowRect(rc1);	// Assuming one of topmost controls
		ScreenToClient(rc1);
		rc.top = rc1.top;
	}
	else
	{
#ifdef _WSB
		m_edtLoVoly.GetWindowRect(rc1);	// Assuming one of bottommost controls
#else
		m_edtUnderlinePrice.GetWindowRect(rc1);	// Assuming one of bottommost controls
#endif//_WSB

		ScreenToClient(rc1);
		rc.top = rc1.bottom + m_nMargine;
	}

	rc.right  = rcClient.right;
	rc.bottom = rcClient.bottom;
	m_fgVola.MoveWindow(rc, FALSE);
	m_fgVola.Invalidate();

	rcClient.InflateRect( m_nMargine, m_nMargine );

	// Upperline
	m_stcUpperLine.GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect( rc );
	rc.left = 0;
	rc.right = rcClient.right;
	InvalidateRect( rc );
	m_stcUpperLine.MoveWindow(rc, TRUE);

	UnlockWindowUpdate();
}


void CVolaManagementDlg::_UnbasePoints()
{
	CSurfaceData& sd = m_vecSurfaces[m_nCurrentSurface];

	// For each month
	for( long Col = 1; Col < m_fgVola.GetCols(); Col++ )
	{
		DATE dtExpDate = m_fgVola.GetColData( Col ).date;

		VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];

		long   nATMRow    = _GetATMStrikeRow( Col );
		double dATMStrike = m_fgVola.GetRowData( nATMRow ).dblVal;
		double dStrikeLow = dATMStrike * sd.m_dStrikeMoneynessLo / 100.0f;
		double dStrikeHi  = dATMStrike * sd.m_dStrikeMoneynessHi / 100.0f;

		bool bLowBased = false, bATMBased = false;

		double dStrike = -1;
		for( long i = 0; i < spSkew->Count; i++ )
		{
			dStrike = spSkew->Strike[i];

			if( spSkew->Status[dStrike] == Deleted )
			{
				dStrike = -1;
				continue;
			}

			if( dStrike >= dStrikeLow && spSkew->IsBasePoint[dStrike] == VARIANT_TRUE && !bLowBased )
			{
				bLowBased = true;
				continue;
			}
			if( dStrike == dATMStrike && !bATMBased )
			{
				spSkew->IsBasePoint[dStrike] = VARIANT_TRUE;
				bATMBased = true;
				continue;
			}

			spSkew->IsBasePoint[dStrike] = VARIANT_FALSE;
		}

		if( dStrike != -1 )
			spSkew->IsBasePoint[dStrike] = VARIANT_TRUE;
	}
}


void CVolaManagementDlg::_ChangeCurrentSurface( size_t nNewCurrentSurface )
{
	if( nNewCurrentSurface < 0 || nNewCurrentSurface > m_vecSurfaces.size() - 1 )
	{
		ASSERT( FALSE );
		return;
	}

	m_nCurrentSurface = nNewCurrentSurface;
	if( m_spSurface != NULL )
	{
		m_spSurface->Reload();
		m_spSurface->EnableEvents = VARIANT_FALSE;
	}
	m_spSurface = g_DataProcessor.GetVolaManager()->Surface[m_vecSurfaces[m_nCurrentSurface].m_nID][OTM];
	m_spSurface->EnableEvents = VARIANT_TRUE;
}


void CVolaManagementDlg::_ExtrapolateVolatility()
{
	// For every month
	for( long col = 1; col < m_fgVola.GetCols(); col++ )
	{
		DATE dtExpDate = m_fgVola.GetColData( col ).date;

		// Get surface
		VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];

		// Extrapolate
		_ExtrapolateMonthVolatility( col, spSkew );
	}
}


void CVolaManagementDlg::_ExtrapolateMonthVolatility( long col, const VME::IVMStrikeSkewVolatilitiesPtr& spSkew ,
															bool bIsFlatWings, double dStrikeLow, double dStrikeHi )
{

	// For each row in column
	for( long row = 2; row < m_fgVola.GetRows(); row++ )
	{
		// Check if there is option for strike
		_variant_t vtData(m_fgVola.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
		if( vtData == vtMissing )
			continue;

		// Get row strike
		double dStrike = m_fgVola.GetRowData( row ).dblVal;

		// Check if there is explicitly set point for strike
		EVolaItemStatus enStatus = Deleted;
		try
		{
			enStatus = spSkew->Status[dStrike];
		}
		catch( const _com_error& )
		{
		}

		if( enStatus == Deleted )
		{
			try
			{
				double dVola;

				if(bIsFlatWings && ( dStrike < dStrikeLow || dStrike > dStrikeHi ) )
				{
					if(dStrike < dStrikeLow)
						dVola = spSkew->Volatility[dStrikeLow];
					else if(dStrike > dStrikeHi)
						dVola = spSkew->Volatility[dStrikeHi];

					spSkew->Add( dStrike, dVola, VARIANT_TRUE );
				}
				else
				{
					dVola = spSkew->Volatility[dStrike];
	
					spSkew->Add( dStrike, dVola, VARIANT_FALSE );
				}

			}
			catch( const _com_error& )
			{
				break;
			}
		}
	}
}


void CVolaManagementDlg::_SelectAll()
{
	try
	{
		// For every month
		for( long col = 1; col < m_fgVola.GetCols(); col++ )
		{
			DATE dtExpDate = m_fgVola.GetColData( col ).date;

			m_fgVola.SetCell( 5/**flexcpChecked*/, CComVariant(1L), CComVariant(col), vtMissing, vtMissing, CComVariant( 1/*flexChecked*/ ) );
			m_spSurface->StrikeSkew[dtExpDate]->BasePointsInterpolation = VARIANT_TRUE;

			// Extrapolate
			_ExtrapolateMonthVolatility( col, m_spSurface->StrikeSkew[dtExpDate] );
		}

		GridInit();
		GridFill();

		PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error selecting all expirations. ");
		message += e.Description().length() ?(LPCSTR) e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}
}

void CVolaManagementDlg::OnVmSelectAll()
{
	_SelectAll();
}

void CVolaManagementDlg::OnDualquadrPar() 
{

	std::map<DATE,double> mapATMStrike;
	for( long col = 1; col < m_fgVola.GetCols(); col++ )
	{
		DATE dtExpDate = m_fgVola.GetColData( col ).date;
		long   nATMRow    = _GetATMStrikeRow( col );
		double dATMStrike = m_fgVola.GetRowData( nATMRow ).dblVal;
		
		mapATMStrike.insert(std::make_pair (dtExpDate, dATMStrike));
	}

	CDualQuadParamsDlg dlg( this, m_months,mapATMStrike, m_spSurface );

	if( dlg.DoModal() == IDOK && dlg.m_bIsATMChanged)
	{
		m_bNeedSave = true;
		SetupControls();

		_UnbasePoints();

		GridInit();
		GridFill();
	}	
}
