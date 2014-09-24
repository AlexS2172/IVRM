// VolaNextDaySurfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "VolaNextDaySurfaceDlg.h"

#include "DataProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolaNextDaySurfaceDlg dialog


CVolaNextDaySurfaceDlg::CVolaNextDaySurfaceDlg(CWnd* pParent /*=NULL*/): 
	CDialog(CVolaNextDaySurfaceDlg::IDD, pParent),
	m_bInitialized(false),
	m_nMinW(0), 
	m_nMinH(0),
	m_nMargine(0)
{
	//{{AFX_DATA_INIT(CVolaNextDaySurfaceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVolaNextDaySurfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolaNextDaySurfaceDlg)
	DDX_Control(pDX, ID_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
	DDX_Control(pDX, IDC_SURFACE, m_fgSurfaces);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolaNextDaySurfaceDlg, CDialog)
	//{{AFX_MSG_MAP(CVolaNextDaySurfaceDlg)
	ON_BN_CLICKED(ID_REFRESH, OnRefresh)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolaNextDaySurfaceDlg message handlers

BOOL CVolaNextDaySurfaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GridFormat();	
	GridFill();

	m_btnClose.SetFlatLook( TRUE );
	m_btnRefresh.SetFlatLook( TRUE );
	
	CRect rc;
	GetWindowRect( &rc );
	m_nMinW = rc.Width();
	m_nMinH = rc.Height();
	ScreenToClient( &rc );

	CRect rcClose;
	m_btnClose.GetWindowRect( &rcClose );
	ScreenToClient( &rcClose );
	m_nMargine = rc.right - rcClose.right - 4;

	_AlignControls();

	m_bInitialized = true;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaNextDaySurfaceDlg::GridFormat()
{
	m_fgSurfaces.SetRedraw( 0 );

	// Selections
	m_fgSurfaces.SetAllowSelection( FALSE );
	m_fgSurfaces.SetAllowBigSelection(FALSE);

	// Sorting
	m_fgSurfaces.SetExplorerBar( 0 );		/*flexExNone*/

	// Editing
	m_fgSurfaces.SetEditable(0);			/*flexEDNone*/

	// Look and feel
	m_fgSurfaces.SetAppearance( 2 );		/*flex3DLight*/
	m_fgSurfaces.SetGridLines( 1 );			/*flexGridFlat*/
	m_fgSurfaces.SetGridLinesFixed( 1 );	/*flexGridFlat*/

	m_fgSurfaces.SetRow(0L);
	m_fgSurfaces.SetCol(0L);

	m_fgSurfaces.AutoSize( 0, _variant_t( 0L ), _variant_t( 1L ), _variant_t( 1L ) );
	m_fgSurfaces.SetRedraw( 2 );
}


/////////////////////////////////////////////////////////////////////////////
//
void CVolaNextDaySurfaceDlg::GridFill()
{
	CWaitCursor wait;

	long nCols = 3;

	m_fgSurfaces.SetRedraw( 0 );

	m_fgSurfaces.SetFixedRows( 1 );
	m_fgSurfaces.SetFixedCols( 0 );
	m_fgSurfaces.SetCols( nCols );
	m_fgSurfaces.SetRows( 1 );

	// Format first column
	m_fgSurfaces.SetTextMatrix( 0, 0, _T("Stock")   );
	m_fgSurfaces.SetTextMatrix( 0, 1, _T("Surface") );
	m_fgSurfaces.SetTextMatrix( 0, 2, _T("Due Date") );

	CString strTemp;
	long nNewRow;
	try
	{
		m_vecSurfaces.clear();

		CDBLayout db;
		
		db.Connect();
		CClientRecordset rs;

		db.NextDaySurfaceGet( 0, rs );

		if( !rs.IsEOF() )
			rs.MoveFirst();
		while( !rs.IsBOF() && !rs.IsEOF() )
		{
			CNextDaySurfaceData sd;
			sd.m_nID		 = rs[L"iNextDaySurfaceID"];
			sd.m_strContract = (LPCTSTR)(_bstr_t)rs[L"vcSymbolName"];
			sd.m_nSurfaceID  = rs[L"iVolaSurfaceDataID"];

			m_vecSurfaces.push_back( sd );

			DATE dtDueDate = floor( (DOUBLE)rs[L"dtDueDate"] );
			DATE dtToday   = vt_date::GetCurrentDate(true);

			COleDateTime date( dtDueDate );

			strTemp.Format(_T("%s\t%s\t%s"), (LPCTSTR)(_bstr_t)rs[L"vcSymbolName"], (LPCTSTR)(_bstr_t)rs[L"SurfaceName"], date.Format( VAR_DATEVALUEONLY ) );
			m_fgSurfaces.AddItem(strTemp, vtMissing);
			nNewRow = m_fgSurfaces.GetRows() - 1;

			if( dtDueDate - dtToday >= 1 )
				m_fgSurfaces.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(0L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant((long)0xC0FFC0) );
			else
				m_fgSurfaces.SetCell(6 /*flexcpBackColor*/, CComVariant(nNewRow), CComVariant(0L), CComVariant(nNewRow), CComVariant(nCols - 1), CComVariant((long)0xC0C0FF) );

			rs.MoveNext();
		}
	}
	catch( const _com_error& e )
	{
		CString message = _T("Error loading next day surfaces list. ");
		message += (LPCSTR)e.Description();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}

	m_fgSurfaces.AutoSize( 0L, _variant_t( 2L ), _variant_t( 0L ), _variant_t( 1L ) );
	m_fgSurfaces.SetRedraw( 2 );
}


void CVolaNextDaySurfaceDlg::UpdateList()
{
	if( ::IsWindow( m_hWnd) )
	{
		GridFormat();	
		GridFill();
	}
}


void CVolaNextDaySurfaceDlg::OnRefresh() 
{
	UpdateList();
}


BEGIN_EVENTSINK_MAP(CVolaNextDaySurfaceDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVolaNextDaySurfaceDlg)
	ON_EVENT(CVolaNextDaySurfaceDlg, IDC_SURFACE, -601 /* DblClick */, OnSurfaceDblClick, VTS_NONE)
	ON_EVENT(CVolaNextDaySurfaceDlg, IDC_SURFACE, -600 /* Click */, OnSurfaceClick, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CVolaNextDaySurfaceDlg::OnSurfaceClick() 
{
	m_fgSurfaces.Select( 0L, 0L, vtMissing, vtMissing );
}


void CVolaNextDaySurfaceDlg::OnSurfaceDblClick() 
{
	long nRow = m_fgSurfaces.GetMouseRow();
	if( nRow < 1 )
	{
		m_fgSurfaces.Select( 0L, 0L, vtMissing, vtMissing );
		return;
	}

	m_fgSurfaces.Select( nRow, 0L, CComVariant(nRow), CComVariant( m_fgSurfaces.GetCols() - 1 ) );
	m_fgSurfaces.UpdateWindow();

	CWnd *pNotificationWnd = AfxGetMainWnd();
	if( !pNotificationWnd )
		return;

	CNextDaySurfaceData& sd = m_vecSurfaces[nRow - 1];
	pNotificationWnd->PostMessage( WM_NEXTDAY_SURFACE_OPEN, (WPARAM)(LPCTSTR)sd.m_strContract, sd.m_nSurfaceID );
}


void CVolaNextDaySurfaceDlg::_AlignControls()
{
	CRect rcClient;
	GetClientRect( rcClient );
	rcClient.DeflateRect( m_nMargine, m_nMargine );

	CSize sz;

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
	m_btnRefresh.GetWindowRect( &rc1 );
	ScreenToClient( &rc1 );
	InvalidateRect( rc1 );
	sz = rc1.Size();
	rc1.right  = rc.left;
	rc1.left   = rc1.right - sz.cx;
	rc1.top    = rcClient.bottom - sz.cy;
	rc1.bottom = rcClient.bottom;
	m_btnRefresh.MoveWindow(rc1, FALSE);
	m_btnRefresh.Invalidate();

	rcClient.bottom = rc.top - m_nMargine;

	// Data grid
	m_fgSurfaces.GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect( rc );
	rc.right  = rcClient.right;
	rc.bottom = rcClient.bottom;
	m_fgSurfaces.MoveWindow(rc, FALSE);
	m_fgSurfaces.Invalidate();
}


void CVolaNextDaySurfaceDlg::OnSize(UINT nType, int cx, int cy) 
{
	if( m_bInitialized )
		_AlignControls();

	CDialog::OnSize(nType, cx, cy);
}


void CVolaNextDaySurfaceDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = m_nMinW;
	lpMMI->ptMinTrackSize.y = m_nMinH;
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}
