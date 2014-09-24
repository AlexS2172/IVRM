// CreateDefaultVolaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "CreateDefaultVolaDlg.h"
#include "DBLayout.h"
#include "DataProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CCreateDefaultVolaDlg::m_nPosPerContract=800;

/////////////////////////////////////////////////////////////////////////////
// CCreateDefaultVolaDlg dialog

CCreateDefaultVolaDlg::CCreateDefaultVolaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateDefaultVolaDlg::IDD, pParent),
	m_bStarted(false),
	m_bCanceled(false)
{
	//{{AFX_DATA_INIT(CCreateDefaultVolaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCreateDefaultVolaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateDefaultVolaDlg)
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_STOCK_PROGRESS, m_pgContract);
	DDX_Control(pDX, IDC_STOCK_INFO, m_stInfo);
	DDX_Control(pDX, IDC_LOG, m_edLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateDefaultVolaDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateDefaultVolaDlg)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_START_VOLA_GENERATION, OnStartGeneration)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateDefaultVolaDlg message handlers

BOOL CCreateDefaultVolaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText( _T("Creating default volatilities surfaces") );

	return TRUE;
}


LRESULT CCreateDefaultVolaDlg::OnStartGeneration( WPARAM wParam, LPARAM lParam )
{
	StartGeneration();
	return 0;
}


BOOL CCreateDefaultVolaDlg::ContinueModal()
{
	if( !m_bStarted )
	{
		if( IsWindowVisible() )
		{
			PostMessage( WM_START_VOLA_GENERATION );
			m_bStarted = true;
		}
	}

	return CDialog::ContinueModal();
}


bool CCreateDefaultVolaDlg::StartGeneration()
{
	CWaitCursor wait;

	m_edLog.SetWindowText( _T("") );
	m_bCanceled = false;
	m_nCounter  = 0;
	m_pgContract.SetPos (0);

	try
	{
		StatusMessage( IDS_MKTSTRUCTURE_INITDATA );
		
		if( m_stocks.size() == 0 )
		{
			CDBLayout db;
			db.Connect();

			CClientRecordset rs;
			db.CusomVolatilityPointIsExistGet( rs );
			for( ; !rs.IsEOF(); ++rs )
			{
				DoEvents();
				if( m_bCanceled ) 
					false;
				
				if( (long)rs[L"CustomVolatilityPointsCount"] == 0 )
				{
					CContractInfo ci;
					ci.nID		 = rs[L"iContractID"];
					ci.dtExpDate = rs[L"dtExpiry"];

					m_stocks.insert( contracts_map::value_type( rs[L"vcSymbolName"], ci ) );
				}
			}
			db.Disconnect();
		}

		m_pgContract.SetRange32( 0, m_stocks.size() * m_nPosPerContract );
		m_pgContract.SetStep( 1 );
		
		if( m_stocks.size() > 0 )
		{
			m_itCur = m_stocks.begin();

			GetNextStock();
		}
		else
		{
			StatusMessage( IDS_MKTSTRUCTURE_ERR_NOSTOCKS );
			Stop();
			return false;
		}

		return true;
	}
	catch( const _com_error& e ) 
	{
		ErrorMessage( e );
		Stop();
	}

	return false;
}


void CCreateDefaultVolaDlg::StatusMessage(UINT  iMessage, bool bLog)
{
	CString strMessage;
	if(strMessage.LoadString(iMessage))
	{
		StatusMessage((LPCTSTR)strMessage, bLog);
	}
}


void CCreateDefaultVolaDlg::StatusMessage(LPCTSTR szMessage, bool bLog)
{
	m_stInfo.SetWindowText (szMessage);
	if (bLog)
	{
		int nLen = m_edLog.GetWindowTextLength();
		if (nLen > 20000)
		{
			m_edLog.SetWindowText(_T(""));
			nLen = 0;
		}

		CString strText;
		if (nLen > 0)
			strText = _T("\r\n");

		strText += szMessage;

		m_edLog.SetSel ( nLen, -1);
		m_edLog.ReplaceSel (strText);
	}
}


void CCreateDefaultVolaDlg::ErrorMessage( const _com_error& e )
{
	StatusMessage( e.ErrorInfo() ? (LPCTSTR)e.Description() : e.ErrorMessage() );
}


void CCreateDefaultVolaDlg::DoEvents()
{
	MSG msg;

	// Process existing messages in the application's message queue.
	// When the queue is empty, do clean up and return.
	while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE) && !m_bCanceled )
	{
		if( !AfxGetThread()->PumpMessage() )
			return;
	}
}


bool CCreateDefaultVolaDlg::Stop()
{
	bool bResult = false; 
	if( !m_bCanceled )
	{
		m_bCanceled = true;
		m_btCancel.SetWindowText( _T("Close") );

		bResult = true;
	}

	OnStop();

	return bResult;
}


void CCreateDefaultVolaDlg::GetNextStock()
{
	if( m_itCur == m_stocks.end() || m_bCanceled )
		return;

	try
	{
		CContractInfo ci = m_itCur->second;

		CDBLayout db;
		db.Connect(); 			

		db.CustomStrikeSkewDefaultPointSave( ci.nID, ci.dtExpDate, OTM, 20. );

		db.Disconnect();

		DoEvents();
		UpdateProgress();
	}
	catch( const _com_error& e )
	{
		ErrorMessage(e);
		Stop();
	}
}


void CCreateDefaultVolaDlg::UpdateProgress()
{
	if( m_bCanceled )
		return;

	m_pgContract.SetPos ( ++m_nCounter * m_nPosPerContract );
	
	CString str;

	CContractInfo ci = m_itCur->second;
	COleDateTime dt( ci.dtExpDate );

	str.Format( _T("Set default volatility data for '%s' %s"), dt.Format(_T("%b %y ")), (LPCTSTR)m_itCur->first );
	StatusMessage (str);
	
	m_stocks.erase (m_itCur);
	m_itCur = m_stocks.begin();

	if( m_itCur == m_stocks.end() )
	{
		StatusMessage(IDS_MKTSTRUCTURE_DONE);
		Stop();
	}
	else
		GetNextStock();		
}


HBRUSH CCreateDefaultVolaDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor ) 
{
	HBRUSH hbr = CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
	
	if( pWnd->GetDlgCtrlID() == IDC_LOG )
	{
		pDC->SetBkColor( GetSysColor(COLOR_WINDOW) );
		hbr = GetSysColorBrush( COLOR_WINDOW );
	}
	
	return hbr;
}


void CCreateDefaultVolaDlg::OnCancel() 
{
	if( Stop() )
	{
		StatusMessage( IDS_MKTSTRUCTURE_CANCELED );
	}
	else
		CDialog::OnCancel();
}


void CCreateDefaultVolaDlg::OnClose() 
{
	EndDialog (IDCANCEL);
}
