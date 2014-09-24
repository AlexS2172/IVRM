// DualQuadParams.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "DualQuadParams.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDualQuadParamsDlg dialog


CDualQuadParamsDlg::CDualQuadParamsDlg(CWnd* pParent, std::vector<DATE>& months,
									   std::map<DATE,double>& ATMStrikes,
									   VME::IVMSymbolVolatilitySurface* pSurface)
	: CDialog(CDualQuadParamsDlg::IDD, pParent),
	m_nMinW(0), 
	m_nMinH(0),
	m_nMargine(0),
	m_spSurface(pSurface),
	m_bNeedSave(false),
	m_bIsATMChanged(false)
{
	m_months.clear();
	m_months = months;

	m_ATMStrikes.clear();
	m_ATMStrikes= ATMStrikes;

	//{{AFX_DATA_INIT(CDualQuadParamsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDualQuadParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDualQuadParamsDlg)
	DDX_Control(pDX, IDC_UPPER_LINE, m_stcUpperLine);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_DUALQUADP_GRID, m_fgDualQuad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDualQuadParamsDlg, CDialog)
	//{{AFX_MSG_MAP(CDualQuadParamsDlg)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDualQuadParamsDlg message handlers


/////////////////////////////////////////////////////////////////////////////
//
void CDualQuadParamsDlg::GridFormat()
{
	m_fgDualQuad.SetRedraw( 0 );

	// Selections
	m_fgDualQuad.SetAllowSelection( FALSE );
	m_fgDualQuad.SetAllowBigSelection(FALSE);

	// Sorting
	m_fgDualQuad.SetExplorerBar( 0 );		/*flexExNone*/

	// Editing
	m_fgDualQuad.SetEditable(2);			/*flexEDKbdMouse*/

	// Look and feel
	m_fgDualQuad.SetAppearance( 2 );		/*flex3DLight*/
	m_fgDualQuad.SetGridLines( 1 );			/*flexGridFlat*/
	m_fgDualQuad.SetGridLinesFixed( 1 );	/*flexGridFlat*/

	m_fgDualQuad.SetRow(0L);
	m_fgDualQuad.SetCol(0L);

	m_fgDualQuad.SetRedraw( 2 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CDualQuadParamsDlg::GridInit()
{
	m_fgDualQuad.SetRedraw( 0 );

	long TopRow  = m_fgDualQuad.GetTopRow();
	long PrevRow = m_fgDualQuad.GetRow();
	long PrevCol = m_fgDualQuad.GetCol();

	m_fgDualQuad.SetFixedRows( 1 );
	m_fgDualQuad.SetFixedCols( 1 );
	m_fgDualQuad.SetCols( m_months.size() + 1 );
	m_fgDualQuad.SetRows( 1 );
	long nCols = m_fgDualQuad.GetCols();

	// Format first column
	m_fgDualQuad.SetColFormat( 0, _T("####.##") );

	// Add month related columns
	long i = 1;
	COleDateTime date;
	CString		 sdate;

	for( std::vector<DATE>::iterator it = m_months.begin(); it != m_months.end(); i++, it++ )	
	{
		date = *it;
		sdate = date.Format(_T("%b,%d %y "));
		
		char cFirst = sdate.GetAt(0);
		cFirst = (TCHAR)CharUpper( (LPTSTR)MAKELONG(cFirst, 0) );
		sdate.SetAt( 0, cFirst );

		// Month titles
		m_fgDualQuad.SetColDataType( i , 8/*flexDTString*/ );
		m_fgDualQuad.SetColData( i, _variant_t(date) );
		m_fgDualQuad.SetTextMatrix( 0, i, sdate );
		m_fgDualQuad.SetColFormat( i, _T("0.000000") );
	}
	
	long nRow, nNewRow;

	long clrNormal = m_fgDualQuad.GetBackColor();
	long clrDisabled = ::GetSysColor(COLOR_BTNHILIGHT); //gd.GetGridColor();
	CString strTemp;

	nRow = m_fgDualQuad.GetFindRow(CComVariant(_T("ATM")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
	if(nRow < 0)
	{
		m_fgDualQuad.AddItem(_T("ATM Vola"), vtMissing);
		nNewRow = m_fgDualQuad.GetRows() - 1;

		m_fgDualQuad.SetRowData(nNewRow, CComVariant(_T("ATM")));
		m_fgDualQuad.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(clrDisabled));
		m_fgDualQuad.SetCell(0 /*flexcpText*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(_T("N/A")));
		m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(4L));
		m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), vtMissing);
		nRow = nNewRow;
	}
	nRow = m_fgDualQuad.GetFindRow(CComVariant(_T("A1")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
	if(nRow < 0)
	{
		m_fgDualQuad.AddItem(_T("Coeff A1"), vtMissing);
		nNewRow = m_fgDualQuad.GetRows() - 1;

		m_fgDualQuad.SetRowData(nNewRow, CComVariant(_T("A1")));
		m_fgDualQuad.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(clrDisabled));
		m_fgDualQuad.SetCell(0 /*flexcpText*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(_T("N/A")));
		m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(4L));
		m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), vtMissing);
		nRow = nNewRow;
	}

	nRow = m_fgDualQuad.GetFindRow(CComVariant(_T("B1")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
	if(nRow < 0)
	{
		m_fgDualQuad.AddItem(_T("Coeff B1"), vtMissing);
		nNewRow = m_fgDualQuad.GetRows() - 1;

		m_fgDualQuad.SetRowData(nNewRow, CComVariant(_T("B1")));
		m_fgDualQuad.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(clrDisabled));
		m_fgDualQuad.SetCell(0 /*flexcpText*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(_T("N/A")));
		m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(4L));
		m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), vtMissing);
		nRow = nNewRow;
	}

	nRow = m_fgDualQuad.GetFindRow(CComVariant(_T("A2")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
	if(nRow < 0)
	{
		m_fgDualQuad.AddItem(_T("Coeff A2"), vtMissing);
		nNewRow = m_fgDualQuad.GetRows() - 1;

		m_fgDualQuad.SetRowData(nNewRow, CComVariant(_T("A2")));
		m_fgDualQuad.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(clrDisabled));
		m_fgDualQuad.SetCell(0 /*flexcpText*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(_T("N/A")));
		m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(4L));
		m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), vtMissing);
		nRow = nNewRow;
	}

	nRow = m_fgDualQuad.GetFindRow(CComVariant(_T("B2")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
	if(nRow < 0)
	{
		m_fgDualQuad.AddItem(_T("Coeff B2"), vtMissing);
		nNewRow = m_fgDualQuad.GetRows() - 1;

		m_fgDualQuad.SetRowData(nNewRow, CComVariant(_T("B2")));
		m_fgDualQuad.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(clrDisabled));
		m_fgDualQuad.SetCell(0 /*flexcpText*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(_T("N/A")));
		m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant(4L));
		m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(nNewRow), CComVariant(1L), CComVariant(nNewRow), CComVariant(nCols - 1), vtMissing);
		nRow = nNewRow;
	}

	m_fgDualQuad.SetCol(0L);
	m_fgDualQuad.SetSort(1L);

	PrevRow = m_fgDualQuad.GetRows() - 1 < PrevRow ? 0 : PrevRow;
	PrevCol = m_fgDualQuad.GetCols() - 1 < PrevCol ? 0 : PrevCol;

	m_fgDualQuad.SetTopRow( TopRow );
	m_fgDualQuad.SetRow(PrevRow);
	m_fgDualQuad.SetCol(PrevCol);

	m_fgDualQuad.SetRedraw( 2 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CDualQuadParamsDlg::GridFill( bool bForceFocus )
{
	CWaitCursor wait;
	
	m_fgDualQuad.SetRedraw( 0 );
	long nCols = m_fgDualQuad.GetCols();

	long clrDisabled = ::GetSysColor(COLOR_GRAYTEXT);
	long clrEnabled  = ::GetSysColor(COLOR_WINDOWTEXT);

	try
	{
		CString strValue;

		long TopRow  = m_fgDualQuad.GetTopRow();
		long PrevRow = m_fgDualQuad.GetRow();
		long PrevCol = m_fgDualQuad.GetCol();

		for( long col = 1; col < m_fgDualQuad.GetCols(); col++ )
		{
			DATE dtExpDate = m_fgDualQuad.GetColData( col ).date;

			// Get surface
			VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
			VME::IDualQuadraticDataPtr spDQData;
			spSkew->get_DualQuadraticParams(&spDQData);

			double dATMVola,dCoefA1,dCoefA2,dCoefB1,dCoefB2;
			spDQData->get_ATMVola(&dATMVola);
			spDQData->get_CoefA1(&dCoefA1);
			spDQData->get_CoefA2(&dCoefA2);
			spDQData->get_CoefB1(&dCoefB1);
			spDQData->get_CoefB2(&dCoefB2);

			long row = m_fgDualQuad.GetFindRow(CComVariant(_T("ATM")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
			if(row != -1)
			{
				strValue.Format( _T("%.6f"),  dATMVola);
				m_fgDualQuad.SetTextMatrix( row, col, strValue );
				m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(dATMVola));
				m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(7L));			
			}
			
			row = m_fgDualQuad.GetFindRow(CComVariant(_T("A1")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
			if(row != -1)
			{
				strValue.Format( _T("%.6f"),  dCoefA1);
				m_fgDualQuad.SetTextMatrix( row, col, strValue );
				m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(dCoefA1));
				m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(7L));			
			}

			row = m_fgDualQuad.GetFindRow(CComVariant(_T("B1")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
			if(row != -1)
			{
				strValue.Format( _T("%.6f"),  dCoefB1);
				m_fgDualQuad.SetTextMatrix( row, col, strValue );
				m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(dCoefB1));
				m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(7L));			
			}

			row = m_fgDualQuad.GetFindRow(CComVariant(_T("A2")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
			if(row != -1)
			{
				strValue.Format( _T("%.6f"),  dCoefA2);
				m_fgDualQuad.SetTextMatrix( row, col, strValue );
				m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(dCoefA2));
				m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(7L));			
			}

			row = m_fgDualQuad.GetFindRow(CComVariant(_T("B2")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
			if(row != -1)
			{
				strValue.Format( _T("%.6f"),  dCoefB2);
				m_fgDualQuad.SetTextMatrix( row, col, strValue );
				m_fgDualQuad.SetCell(20/*flexcpData*/,		CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(dCoefB2));
				m_fgDualQuad.SetCell(2 /*flexcpAlignment*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing, CComVariant(7L));			
			}
		}

		if( !bForceFocus )
		{
			m_fgDualQuad.SetTopRow( TopRow );
			m_fgDualQuad.SetRow(PrevRow);
			m_fgDualQuad.SetCol(PrevCol);
		}
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error filling grid with dual quadratic values. ");
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}

	m_fgDualQuad.AutoSize(0, CComVariant(nCols - 1L), CComVariant(1L), CComVariant(10L));
	m_fgDualQuad.SetRedraw( 2 );
}

BOOL CDualQuadParamsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Format grids
	GridFormat();
	GridInit();
	GridFill();

	CRect rc;
	GetWindowRect( &rc );
	m_nMinW = rc.Width();
	m_nMinH = rc.Height();
	ScreenToClient( &rc );

	CRect rcClose;
	m_btnCancel.GetWindowRect( &rcClose );
	ScreenToClient( &rcClose );
	m_nMargine = rc.right - rcClose.right - 4;
	
	_AlignControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
void CDualQuadParamsDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = m_nMinW;
	lpMMI->ptMinTrackSize.y = m_nMinH;
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CDualQuadParamsDlg::OnSize(UINT nType, int cx, int cy) 
{
	_AlignControls();

	CDialog::OnSize(nType, cx, cy);	
}

void CDualQuadParamsDlg::_AlignControls(  )
{
	LockWindowUpdate();

	CRect rcClient;
	GetClientRect( rcClient );
	rcClient.DeflateRect( m_nMargine, m_nMargine );

	CSize sz;

	// Cancel button
	CRect rc;
	m_btnCancel.GetWindowRect( &rc );
	ScreenToClient( &rc );
	InvalidateRect(rc);
	sz = rc.Size();
	rc.left   = rcClient.right  - sz.cx;
	rc.top    = rcClient.bottom - sz.cy;
	rc.right  = rcClient.right;
	rc.bottom = rcClient.bottom;
	m_btnCancel.MoveWindow(rc, FALSE);
	m_btnCancel.Invalidate();

	// OK button
	CRect rc1;
	m_btnOK.GetWindowRect( &rc1 );
	ScreenToClient( &rc1 );
	InvalidateRect( rc1 );
	sz = rc1.Size();
	rc1.right  = rc.left;
	rc1.left   = rc1.right - sz.cx;
	rc1.top    = rcClient.bottom - sz.cy;
	rc1.bottom = rcClient.bottom;
	m_btnOK.MoveWindow(rc1, FALSE);
	m_btnOK.Invalidate();

	rcClient.bottom = rc.top - m_nMargine;

	// Data grid
	m_fgDualQuad.GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect( rc );

	m_stcUpperLine.GetWindowRect(rc1);	
	ScreenToClient(rc1);
	rc.top = rc1.bottom + m_nMargine;

	rc.right  = rcClient.right;
	rc.bottom = rcClient.bottom;
	m_fgDualQuad.MoveWindow(rc, FALSE);
	m_fgDualQuad.Invalidate();

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

BEGIN_EVENTSINK_MAP(CDualQuadParamsDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDualQuadParamsDlg)
	ON_EVENT(CDualQuadParamsDlg, IDC_DUALQUADP_GRID, 23 /* ValidateEdit */, OnValidateEditDualquadpGrid, VTS_I4 VTS_I4 VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDualQuadParamsDlg::OnValidateEditDualquadpGrid(long Row, long Col, BOOL FAR* Cancel) 
{
	*Cancel = FALSE;

	double dValue = 0.;
	CString strValue = m_fgDualQuad.GetEditText();
	strValue.TrimLeft();
	strValue.TrimRight();

	long ATMrow = m_fgDualQuad.GetFindRow(CComVariant(_T("ATM")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));

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

		if(!*Cancel && ATMrow == Row)
		{
			dValue = ((LONGLONG)(dValue * 100L)) / 100.;

			if(dValue < 0.)
			{
				dValue = (_variant_t)m_fgDualQuad.GetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing);
				*Cancel = TRUE;
			}
			
			strValue.Format(_T("%.6f"), dValue);
			m_fgDualQuad.SetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing, COleVariant(dValue));
			m_fgDualQuad.SetEditText(strValue);
		}
		else if(!*Cancel)
		{
			strValue.Format(_T("%.6f"), dValue);
			m_fgDualQuad.SetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing, COleVariant(dValue));
			m_fgDualQuad.SetEditText(strValue);
		}
	}
	else
	{
		m_fgDualQuad.SetCell(20 /*flexcpData*/, CComVariant(Row), CComVariant(Col), vtMissing, vtMissing, COleVariant(0.));
		m_fgDualQuad.SetEditText(_T(""));
	}
	if(!*Cancel)
	{
		m_bNeedSave = true;
		
		if(ATMrow == Row)
			m_bIsATMChanged = true;
	}
}

void CDualQuadParamsDlg::OnOK() 
{
	if(m_bNeedSave)
	{
		try
		{
			for( long col = 1; col < m_fgDualQuad.GetCols(); col++ )
			{
				DATE dtExpDate = m_fgDualQuad.GetColData( col ).date;

				// Get surface
				VME::IVMStrikeSkewVolatilitiesPtr spSkew = m_spSurface->StrikeSkew[dtExpDate];
				VME::IDualQuadraticDataPtr spDQData;
		
				__CHECK_HRESULT( spDQData.CreateInstance(VME::CLSID_DualQuadraticData), _T("Error create dual quadratic params object.") );
				
				double dATMVola = 0;
				double dCoefA1 = 0;
				double dCoefA2 = 0;
				double dCoefB1 = 0;
				double dCoefB2 = 0;

				long row = m_fgDualQuad.GetFindRow(CComVariant(_T("ATM")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
				if(row != -1)
				{
					_variant_t vtData(m_fgDualQuad.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData.vt == VT_R8 )
					{
						dATMVola = vtData.dblVal;
					}
				}
				
				row = m_fgDualQuad.GetFindRow(CComVariant(_T("A1")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
				if(row != -1)
				{
					_variant_t vtData(m_fgDualQuad.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData.vt == VT_R8 )
					{
						dCoefA1 = vtData.dblVal;
					}
				}

				row = m_fgDualQuad.GetFindRow(CComVariant(_T("B1")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
				if(row != -1)
				{
					_variant_t vtData(m_fgDualQuad.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData.vt == VT_R8 )
					{
						dCoefB1 = vtData.dblVal;
					}
				}

				row = m_fgDualQuad.GetFindRow(CComVariant(_T("A2")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
				if(row != -1)
				{
					_variant_t vtData(m_fgDualQuad.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData.vt == VT_R8 )
					{
						dCoefA2 = vtData.dblVal;
					}
				}

				row = m_fgDualQuad.GetFindRow(CComVariant(_T("B2")), CComVariant(1L), vtMissing, CComVariant(FALSE), CComVariant(FALSE));
				if(row != -1)
				{
					_variant_t vtData(m_fgDualQuad.GetCell(20 /*flexcpData*/, CComVariant(row), CComVariant(col), vtMissing, vtMissing));
					if( vtData.vt == VT_R8 )
					{
						dCoefB2 = vtData.dblVal;
					}
				}

				std::map<DATE,double>::iterator iter = m_ATMStrikes.find(dtExpDate);
				if(iter != m_ATMStrikes.end())
				{
					double dATMStrike = iter->second;
					__CHECK_HRESULT(spSkew->Add( dATMStrike, dATMVola, VARIANT_TRUE ), _T("Error set ATMVola.") );
				}
				__CHECK_HRESULT( spDQData->put_ATMVola(dATMVola), _T("Error set ATMVola.") );
				__CHECK_HRESULT( spDQData->put_CoefA1(dCoefA1), _T("Error set CoefA1.") );
				__CHECK_HRESULT( spDQData->put_CoefA2(dCoefA2), _T("Error set CoefA2.") );
				__CHECK_HRESULT( spDQData->put_CoefB1(dCoefB1), _T("Error set CoefB1.") );
				__CHECK_HRESULT( spDQData->put_CoefB2(dCoefB2), _T("Error set CoefB2.") );

				__CHECK_HRESULT( spSkew->putref_DualQuadraticParams(spDQData), _T("Error put dual quadratic params.") );
			}

		}
		catch( const _com_error& e )
		{
			CString message = _T("Error save dual quadratic params.");
			message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
			MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
		}
	}
	
	CDialog::OnOK();
}
