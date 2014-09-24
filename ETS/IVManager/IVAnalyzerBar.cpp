// IVAnalyzerBar.cpp : implementation file
//

#include "stdafx.h"
#include "IVAnalyzer.h"
#include "IVAnalyzerBar.h"
#include "DBLayout.h"

//#include "OptionsSheet.h"
#include "OptionsDlg.h"
#include "MktStructureDlg.h"
#include "CreateDefaultVolaDlg.h"
#include "AboutDlg.h"
#include "SplashDlg.h"

#include "AddSymbolDlg.h"
#include "RemoveSymbolDlg.h"
#ifdef _VOLA_MANAGER
#include "SymbolGroupsDlg.h"
#include "RulesDlg.h"
#endif

#include "IRCurve.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIVAnalyzerBar dialog

const int CIVAnalyzerBar::m_nIconSize = 8; // Non-standard size 

#define ID_CHARTS_UPDATE_TIMER	20


CIVAnalyzerBar::CIVAnalyzerBar(CWnd* pParent /*=NULL*/)
	: CAppBar(CIVAnalyzerBar::IDD, pParent), 
	m_sizeMinMax (0, 0), m_nHeightDocked (0), 
	m_nHeightFloat (0),	m_bDrawSmallFrame (false), 
	m_bActiveCaption (false), m_bSaveState (true),
	m_nCurArrange (ID_WINDOW_TILE_HORZ),
	m_nNextArrange (ID_WINDOW_TILE_HORZ),
	m_bAnimation (false), m_bOnline (false)
{
	//{{AFX_DATA_INIT(CIVAnalyzerBar)
	//}}AFX_DATA_INIT
}

BOOL CIVAnalyzerBar::Create()
{
	return CAppBar::Create (IDD);
}

void CIVAnalyzerBar::DoDataExchange(CDataExchange* pDX)
{
	CAppBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIVAnalyzerBar)
	DDX_Control(pDX, IDC_SYMBOLS, m_cbSymbol);
	//}}AFX_DATA_MAP
}

void CIVAnalyzerBar::PostNcDestroy()
{
	delete this;
}

IMPLEMENT_DYNCREATE(CIVAnalyzerBar, CAppBar)

BEGIN_MESSAGE_MAP(CIVAnalyzerBar, CAppBar)
	//{{AFX_MSG_MAP(CIVAnalyzerBar)
	ON_WM_GETMINMAXINFO()
	ON_WM_CTLCOLOR()
	ON_MESSAGE (WM_CTLCOLORSTATIC,OnCtlColorStatic)
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	ON_WM_DESTROY()
	ON_COMMAND(ID_TIME_SKEW, OnTimeSkew)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
//	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND(ID_APPBAR_AUTOHIDE, OnAppbarAutohide)
	ON_WM_NCACTIVATE()
	ON_WM_NCRBUTTONUP()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_APPBAR_ALWAYSONTOP, OnAppbarAlwaysontop)
	ON_COMMAND(ID_STRUCTURE_VOLA, OnStructureVola)
	ON_COMMAND(ID_NEXTDAYSURFACE, OnNextDaySurface)
	ON_WM_CONTEXTMENU()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
	ON_CBN_EDITCHANGE(IDC_SYMBOLS, OnEditchangeSymbols)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_GO, OnGo)
	ON_CBN_EDITUPDATE(IDC_SYMBOLS, OnEditupdateSymbols)
	ON_COMMAND(ID_SERVICE_IMPORTALL, OnImportAll)
	ON_COMMAND(ID_SERVICE_GENDEFVOLA, OnCreateDefaultSurfaces )
	ON_COMMAND(ID_SERVICE_CURRENTSYMBOL,OnImportCurrentSymbol)
	ON_COMMAND(ID_SERVICE_INTERESTRATES, OnIRCurve)
	ON_COMMAND(ID_SELECT_SYMBOL, OnSelectSymbol)
	ON_COMMAND(ID_CANCEL_SYMBOL, OnCancelSymbol)
	ON_COMMAND(ID_OPTIONS, OnOptions)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_HELP, OnAppHelp)
	ON_COMMAND(ID_STRIKE_SKEW, OnStrikeSkew)
	ON_COMMAND(ID_WINDOW_ARRANGE, OnWindowArrange)
	ON_COMMAND(ID_CHARTS, OnCharts)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
	ON_COMMAND(ID_WINDOW_TILE_VERT, OnWindowTileVert)
	ON_COMMAND(ID_WINDOW_CASCADE, OnWindowCascade)
	ON_UPDATE_COMMAND_UI(ID_ADVERTIZING, OnUpdateAdvertizing)
	ON_UPDATE_COMMAND_UI(ID_STATUS, OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_GO, OnUpdateGo)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_ADD_NEW_SYMBOL, OnAddNewSymbol)
	ON_COMMAND(ID_REMOVE_SYMBOL, OnRemoveSymbol)
	ON_COMMAND(ID_SYMBOL_GROUPS, OnSymbolGroups)
	ON_COMMAND(ID_RULES, OnRules)
	ON_MESSAGE(ID_ADD_SYMBOL, OnAddSymbol)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	
	
//	ON_MESSAGE (WM_VOLA_CHANGED, OnVolaChangedNotification)
//	ON_MESSAGE (WM_UNDERLAYING_CHANGED, OnUndChangedNotification)
	ON_MESSAGE (WM_ERROR, OnError)
	ON_MESSAGE (WM_ONLINE, OnOnline)
	ON_MESSAGE (WM_STOCK_CHANGED, OnStockChanged)
#ifdef _VOLA_MANAGER
	ON_MESSAGE (WM_SETTINGS,				  OnSettingsChanged        )
	ON_MESSAGE (WM_CUSTOM_VOLA_CHANGED,		  OnCustomVolaChanged	   )
	ON_MESSAGE (WM_CUSTOM_VOLA_SAVED,		  OnCustomVolaSaved		   )
	ON_MESSAGE (WM_CUSTOM_VOLA_RESTORED,	  OnCustomVolaRestored	   )
	ON_MESSAGE (WM_CUSTOM_VOLA_PRICE_CHANGED, OnCustomVolaPriceChanged )

	ON_MESSAGE (WM_CUSTOM_VOLA_DEFAULTSURFACE_CHANGED, OnCustomVolaDefaultSurfaceChanged )

	ON_MESSAGE (WM_RULE_LIST_CHANGED,			 OnRulesListChanged			  )
	ON_MESSAGE (WM_NEXTDAY_SURFACE_LIST_CHANGED, OnNextDaySurfaceListChanged  )
	ON_MESSAGE (WM_NEXTDAY_SURFACE_OPEN,		 OnNextDaySurfaceOpen		  )
#endif //_VOLA_MANAGER
	ON_COMMAND_RANGE (ID_STRIKE_SKEW_FIRST, ID_STRIKE_SKEW_LAST, OnStrikeSkew)

	
    ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_NOTIFY(NM_NCHITTEST, AFX_IDW_REBAR, OnRebarHitTest)
	ON_NOTIFY(NM_NCHITTEST, AFX_IDW_TOOLBAR, OnRebarHitTest)
	ON_NOTIFY(NM_CLICK, AFX_IDW_REBAR, OnRebarClick)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIVAnalyzerBar message handlers

BOOL CIVAnalyzerBar::OnInitDialog() 
{
	CAppBar::OnInitDialog();
	HideFloatAdornments(TRUE);

	CWaitCursor wt;
	CSplashDlg dlg;

	dlg.SetAppearance(theApp.GetAppearance());

	dlg.Create (IDD_SPLASH, CWnd::GetDesktopWindow() );
//#ifdef _DEBUG
	dlg.SetWindowPos (&wndTop, 0, 0, 0, 0, 
		SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
//#else
//	dlg.SetWindowPos (&wndTopMost, 0, 0, 0, 0, 
//		SWP_SHOWWINDOW | SWP_NOSnIZE | SWP_NOMOVE);
//#endif
	dlg.UpdateWindow();

	InitImages();
	InitMenus();
	InitControlBars();

	HICON hIcon = AfxGetApp()->LoadIcon (IDI_IV_APP);
	SetIcon (hIcon, TRUE);
	SetIcon (hIcon, FALSE);

	g_DataProcessor.CreateThread();
	if( g_DataProcessor.WaitReady() )
	{
		try
		{
			while( 1 )
			{
				try
				{
					InitConnection();
					break;
				}
				catch(  const _com_error& e  )
				{
					ErrorMsgBox( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage() );

					COptionsDlg optsDlg;
					if( optsDlg.DoModal() != IDOK )
					{
						g_DataProcessor.SetErrorState();

						PostMessage( WM_CLOSE );
						return TRUE;
					}
				}
			}
			
			LoadSymbols();

			if( m_cbSymbol.GetCurSel() != -1 )
				GetData(true);
			
		}
		catch( const _com_error& e )
		{
			ErrorMsgBox( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage() );
			g_DataProcessor.SetErrorState();

			PostMessage( WM_CLOSE );
		}
	}
//	else
//	{
//		PostMessage( WM_CLOSE );
//	}

	InitAppBar();
	ResizeControls();

	dlg.DestroyWindow();
	
	SetTimer( ID_CHARTS_UPDATE_TIMER, 1000, NULL );
	
	return TRUE;
}

// Hides caption bar
void CIVAnalyzerBar::HideFloatAdornments(BOOL fHide) 
{
	if (fHide) 
	{
		m_bDrawSmallFrame = true;
		ModifyStyle(WS_CAPTION | WS_SYSMENU, 0, SWP_DRAWFRAME);
	} 
	else 
	{
		m_bDrawSmallFrame = false;
   		ModifyStyle(0, WS_CAPTION | WS_SYSMENU, SWP_DRAWFRAME);
	}

}


void CIVAnalyzerBar::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (m_sizeMinMax.cx > 0  && m_sizeMinMax.cy > 0)
	{
		lpMMI->ptMinTrackSize.x = m_sizeMinMax.cx;
		lpMMI->ptMinTrackSize.y = m_sizeMinMax.cy;
		
		lpMMI->ptMaxTrackSize.x = m_sizeMinMax.cx;
		lpMMI->ptMaxTrackSize.y = m_sizeMinMax.cy;
	}
	else
		CAppBar::OnGetMinMaxInfo(lpMMI);
}

// Reserved
HBRUSH CIVAnalyzerBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = NULL; 
	
/*	if (nCtlColor == CTLCOLOR_DLG)
	{
		hbr = (HBRUSH) GetStockObject (BLACK_BRUSH);
	}
	else*/
	hbr = CAppBar::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

// Reserved
LRESULT CIVAnalyzerBar::OnCtlColorStatic (WPARAM wParam, LPARAM lParam)
{
	CDC * pDC = CDC::FromHandle	( (HDC) wParam );
	pDC->SetBkColor (RGB(0,0,0) );
	
	return 0;
}

void CIVAnalyzerBar::OnSize(UINT nType, int cx, int cy) 
{
	CAppBar::OnSize(nType, cx, cy);
	ResizeControls();
}

void CIVAnalyzerBar::ResizeControls()
{
	if( IsWindow (m_wndReBar) )
	{
		CRect rect;
		GetClientRect (rect);

		RepositionBars(AFX_IDW_REBAR,
					   AFX_IDW_REBAR,
					   0, reposDefault, NULL, rect, TRUE);

		CReBarCtrl & ctl2 = m_wndReBar2.GetReBarCtrl();
		for( size_t i =0; i < ctl2.GetBandCount(); i++ )
		{
			m_wndReBar2.GetReBarCtrl().MaximizeBand(i);
		}

		CReBarCtrl & ctl = m_wndReBar.GetReBarCtrl();
		for( size_t i =0; i < ctl.GetBandCount(); i++ )
		{
			m_wndReBar.GetReBarCtrl().MaximizeBand(i);
		}
	}
}

// Reserved
BOOL CIVAnalyzerBar::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    pTTT->lpszText = MAKEINTRESOURCE(nID);
    pTTT->hinst = AfxGetResourceHandle();

    return(TRUE);
};

// Loads stocks list from database
void CIVAnalyzerBar::LoadSymbols()
{
	m_cbSymbol.ResetContent();
	
	CDBLayout db;
	db.Connect();

	CClientRecordset rs;
	db.GetSymbols(rs);

	for (; ! rs.IsEOF() ;++rs)
	{
		int nID = m_cbSymbol.AddString( (_bstr_t)  rs[L"vcSymbolName"]);
		int SymbolID = rs["iContractID"];
		m_cbSymbol.SetItemData(nID, SymbolID);

		long lContractTypeID = rs["iContractTypeID"];
		
		if (lContractTypeID == 2)
		{
			IEtsIndexDivAtomPtr spDividends(__uuidof(EtsIndexDivAtom));

			spDividends->DivType = (EtsDivTypeEnum)long(rs["isDivCustom"]);
			spDividends->DivFreq = rs["iDivFrequency"]; 
			spDividends->DivDate = rs["dtDivDate"];
			spDividends->DivAmt =  rs["fAmount"];
			spDividends->DivFreqCust = rs["iDivFrequencyCustom"];
			spDividends->DivDateCust = rs["dtDivDateCustom"];
			spDividends->DivAmtCust = rs["fAmountCustom"];

			CClientRecordset rsDividends;
			db.GetDividends(SymbolID, rsDividends);

			IEtsDivCollPtr spCustomColl(__uuidof(EtsDivColl));
			while(!rsDividends.IsEOF())
			{
				vt_date dDate   = rsDividends[L"DivYtes"];
				double 	dAmount = rsDividends[L"DivAmnt"];
				spCustomColl->AddNonUnique(dDate, dAmount);
				++rsDividends;
			}
			spDividends->CustomDivs = spCustomColl;

			g_DataProcessor.SetStockDividends(SymbolID, spDividends);
		}
	}

	db.Disconnect();
	
	CString strSymbol =	theApp.GetSymbol();
	strSymbol.TrimRight();
	strSymbol.TrimLeft();
	if(!strSymbol.GetLength())
	{
		CXMLParamsHelper XMLParams;
		if(SUCCEEDED(XMLParams.LoadXMLParams()))
		{
			_bstr_t sbsSymbol;
			XMLParams.GetXMLString(CIVAnalyzerApp::m_sbsXMLKey, L"LastSymbol", &sbsSymbol);
			strSymbol = sbsSymbol.GetBSTR();
		}
	}
	theApp.ClearSymbol();

	if (! strSymbol.IsEmpty())
	{
		int i = m_cbSymbol.FindStringExact(-1, strSymbol);
		if (i != -1)
			m_cbSymbol.SetCurSel (i);
	}

	if ( m_cbSymbol.GetCurSel () == -1 )
		m_cbSymbol.SetCurSel (0);

}



void CIVAnalyzerBar::OnDestroy() 
{

	if ( IsWindow(m_timeSkewWnd) )
		m_timeSkewWnd.DestroyWindow();
	
	for (months_map::iterator it = m_months.begin(); it != m_months.end(); it++  )
	{
		CChartWnd * pWnd = it->second.m_pWnd;
		if (pWnd)
		{
			pWnd->DestroyWindow();
			delete pWnd;
		}
		
	}

	m_mnuMain.DestroyMenu();
	CAppBar::OnDestroy();
}


void CIVAnalyzerBar::GetData(bool bInitial)
{
	SaveLastSymbol();
	CWaitCursor wt;

	StopOnlineIndication();

	CString strText = GetCurSymbol();
	
	bool bValid = false;

	int i     = m_cbSymbol.FindStringExact( -1, strText );
	int iData = m_cbSymbol.GetItemData( i );

	if( iData == -1 )
	{
#ifdef _VOLA_MANAGER
		CString strError;
		strError.Format( _T("Symbol %s is not in database."),(LPCTSTR)strText );
		AfxMessageBox( strError, MB_OK );

		StopAnimation();
		SetFocus();
		m_bOnline = true;
		m_cbSymbol.EnableWindow(TRUE);
		SaveLastSymbol();
		return;
#else
		CString strError;
		strError.Format( _T("Symbol %s is not in database.\n Do you want to add it?"),(LPCTSTR)strText );
		if( AfxMessageBox(strError, MB_YESNO) == IDYES )
		{
			size_t bufSize = (strText.GetLength()+1)*sizeof(TCHAR);
			TCHAR* buffer = new TCHAR[bufSize];
			if(buffer)
			{
				ZeroMemory(buffer, bufSize);
				_tcscpy(buffer, (LPCTSTR)strText);
				AfxGetMainWnd()->PostMessage(ID_ADD_SYMBOL, reinterpret_cast<WPARAM>(buffer), 0);			
				return;
			}			
		}
		else
		{
			StopAnimation();
			SetFocus();
			m_bOnline = true;
			m_cbSymbol.EnableWindow(TRUE);
			SaveLastSymbol();
			return;
		}
#endif
	}
	else
	{
		m_cbSymbol.SetCurSel(i);
	}

	bool bWndsWereVisible = IsWindow(m_timeSkewWnd) && m_timeSkewWnd.IsWindowVisible() 
		&& m_months.areAllWndsVisible();

	m_months.invalidate();
	g_DataProcessor.GetData(strText, iData , m_months, bInitial);
	m_months.closeInvalid();

#ifdef _VOLA_MANAGER
	m_wndVM.UpdateSymbolData( iData, m_months.getMonth() );
#endif //_VOLA_MANAGER

	if (bWndsWereVisible)
		OnCharts();	

	UpdateWindows( true );

#ifdef _VOLA_MANAGER
	if( ::IsWindow(m_wndNDS.m_hWnd) && m_wndNDS.IsWindowVisible() )
		m_wndNDS.SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	if( ::IsWindow(m_wndVM.m_hWnd) && m_wndVM.IsWindowVisible() )
		m_wndVM.SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
#endif //_VOLA_MANAGER
	
	m_cbSymbol.EnableWindow(TRUE);
}	

void CIVAnalyzerBar::OnTimeSkew() 
{	
	BOOL bShow = true;
	if( !IsWindow(m_timeSkewWnd) )
	{
		bShow = m_timeSkewWnd.Create();
	
		MONITORINFO mi;
		mi.cbSize = sizeof( MONITORINFO );

		bool bMultyScreen = theApp.IsMultyScreenSupported();

		if( bMultyScreen )
		{
			HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );

			CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );

			WINDOWPLACEMENT wpls;
			wpls.length = sizeof (WINDOWPLACEMENT);
			m_timeSkewWnd.GetWindowPlacement(&wpls);

			wpls.showCmd = SW_HIDE;

			wpls.rcNormalPosition = CRect( mi.rcMonitor.left + wpls.rcNormalPosition.left, 
										   mi.rcMonitor.top  + wpls.rcNormalPosition.top,
										   mi.rcMonitor.left + wpls.rcNormalPosition.right,
										   mi.rcMonitor.top  + wpls.rcNormalPosition.bottom ); 
				
			m_timeSkewWnd.SetWindowPlacement( &wpls );
		}
	}

	if( bShow )
		m_timeSkewWnd.Show();
}


void CIVAnalyzerBar::OnStrikeSkew(UINT nID)
{
	int i =0;
	for (months_map::iterator it = m_months.begin(); it != m_months.end(); it++, i++)
	{
		if (nID == ID_STRIKE_SKEW_FIRST + i)
		{
			ShowStrikeSkew (it->first.date_, it->second);
			break;
		}
	}	
}

// Updates chart windows
void CIVAnalyzerBar::UpdateWindows( bool bReinitialize )
{
	if( bReinitialize )
		m_timeSkewWnd.InitializeChart();
	m_timeSkewWnd.UpdateChart( false );

#ifdef _VOLA_MANAGER
	m_wndVM.UpdateByPrice();
#endif //_VOLA_MANAGER

	int i = 0;
	for (months_map::iterator it = m_months.begin(); it != m_months.end(); it++, i++)
	{
		CChartWnd * pWnd =  it->second.m_pWnd;
		if( pWnd )
		{
			if( bReinitialize )
				pWnd->InitializeChart();
			pWnd->UpdateChart( bReinitialize );
		}
	}
}


#ifdef _VOLA_MANAGER
LRESULT CIVAnalyzerBar::OnSettingsChanged( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case 1: // Database
		{
				CIVAnalyzerOptions optsOld;
				CIVAnalyzerOptions optsNew;
				theApp.GetOptions(optsOld);
				theApp.ReadOptions();
				theApp.GetOptions(optsNew);
				if(optsOld.m_strConnection != optsNew.m_strConnection)
				{
					try
					{
						// DataBase changed Notify
						InitConnection();
						LoadSymbols();
						GetData();
					}
					catch (_com_error & e)
					{
						ErrorMsgBox ( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage() );
					}
				}
		}
		break;
	case 2: // PriceProvider
		{
			CIVAnalyzerOptions optsOld;
			CIVAnalyzerOptions optsNew;
			theApp.GetOptions(optsOld);
			theApp.ReadOptions();
			theApp.GetOptions(optsNew);
			if(optsOld.m_enProviderType != optsNew.m_enProviderType)
			{
				// Provider changed Notify
				StopOnlineIndication();
				g_DataProcessor.ReloadPricesConnection(optsNew.m_enProviderType !=optsOld.m_enProviderType);
			}
		}
		break;
	case 3: // IVFeed
		{
			CIVAnalyzerOptions optsOld;
			CIVAnalyzerOptions optsNew;
			theApp.GetOptions(optsOld);
			theApp.ReadOptions();
			theApp.GetOptions(optsNew);
			if(optsOld.m_IVOpts != optsNew.m_IVOpts)
			{
				// Internet changed Notify
				g_DataProcessor.ReloadRanges();
			}
		}
		break;
	}
	return 0;
}

LRESULT CIVAnalyzerBar::OnCustomVolaChanged( WPARAM wParam, LPARAM lParam )
{
	CCustomVolaNotification *pNotification = (CCustomVolaNotification*)wParam;
	HWND hSourceWnd = (HWND)lParam; // lParam is HWND of window where changes were made

	if( hSourceWnd != m_wndVM.m_hWnd )
		m_wndVM.UpdateView();
	if( hSourceWnd != m_timeSkewWnd.m_hWnd )
		m_timeSkewWnd.UpdateChart( WM_CUSTOM_VOLA_CHANGED, pNotification );
	for( months_map::iterator it = m_months.begin(); it != m_months.end(); it++ )
		{
		CChartWnd *pWnd = it->second.m_pWnd;
		if( pWnd && hSourceWnd != pWnd->m_hWnd )
			pWnd->UpdateChart( WM_CUSTOM_VOLA_CHANGED, pNotification );
		}
	
	delete pNotification;

	return 0;
}


LRESULT CIVAnalyzerBar::OnCustomVolaSaved( WPARAM wParam, LPARAM lParam )
{
	CCustomVolaNotification *pNotification = (CCustomVolaNotification*)wParam;
	HWND hSourceWnd = (HWND)lParam; // lParam is HWND of window where changes were made

	if( hSourceWnd != m_wndVM.m_hWnd )
		m_wndVM.UpdateView();

	if( hSourceWnd != m_wndVM.m_hWnd )
		m_wndVM.UpdateView();
	if( hSourceWnd != m_timeSkewWnd.m_hWnd )
		m_timeSkewWnd.UpdateChart( WM_CUSTOM_VOLA_SAVED, pNotification );
	for( months_map::iterator it = m_months.begin(); it != m_months.end(); it++ )
		{
		CChartWnd *pWnd = it->second.m_pWnd;
		if( pWnd && hSourceWnd != pWnd->m_hWnd )
			pWnd->UpdateChart( WM_CUSTOM_VOLA_SAVED, pNotification );
		}
	
	delete pNotification;

	return 0;
}


LRESULT CIVAnalyzerBar::OnCustomVolaRestored( WPARAM wParam, LPARAM lParam )
{
	CCustomVolaNotification *pNotification = (CCustomVolaNotification*)wParam;
	HWND hSourceWnd = (HWND)lParam; // lParam is HWND of window where changes were made

	if( hSourceWnd != m_wndVM.m_hWnd )
		m_wndVM.UpdateView();

	if( hSourceWnd != m_wndVM.m_hWnd )
		m_wndVM.UpdateView();
	if( hSourceWnd != m_timeSkewWnd.m_hWnd )
		m_timeSkewWnd.UpdateChart( WM_CUSTOM_VOLA_RESTORED, pNotification );
	for( months_map::iterator it = m_months.begin(); it != m_months.end(); it++ )
		{
		CChartWnd *pWnd = it->second.m_pWnd;
		if( pWnd && hSourceWnd != pWnd->m_hWnd )
			pWnd->UpdateChart( WM_CUSTOM_VOLA_RESTORED, pNotification );
		}
	
	delete pNotification;

	return 0;
}


LRESULT CIVAnalyzerBar::OnCustomVolaPriceChanged( WPARAM wParam, LPARAM lParam )
{
	m_wndVM.UpdateByPrice();

	return 0;
}


LRESULT CIVAnalyzerBar::OnCustomVolaDefaultSurfaceChanged( WPARAM wParam, LPARAM lParam )
{
	CDBLayout db;
	db.Connect();
	CString strSurfaceName = db.VolaSurfaceNameGet( wParam );

	CString message;
	message.Format( _T("Default surface has been changed on %s. Do you want to switch to it"), strSurfaceName );
	if( ::MessageBox( ::IsWindow(m_wndVM.m_hWnd) && m_wndVM.IsWindowVisible() ? m_wndVM.m_hWnd : NULL, message, _T("Volatility Manager"), MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL ) == IDYES )
	{
		m_wndVM.OpenDefaultSurface( wParam );
		UpdateWindows();
	}

	return 0;
}


LRESULT CIVAnalyzerBar::OnRulesListChanged( WPARAM wParam, LPARAM lParam )
{
	m_wndVM.UpdateRules();

	return 0;
}


LRESULT CIVAnalyzerBar::OnNextDaySurfaceListChanged( WPARAM wParam, LPARAM lParam )
{
	m_wndNDS.UpdateList();

	return 0;
}
#endif //_VOLA_MANAGER


LRESULT CIVAnalyzerBar::OnOnline (WPARAM wParam, LPARAM lParam)
{
	m_bOnline = true;
	m_cbSymbol.EnableWindow(TRUE);
	StartAnimation();
	return 0;
}


LRESULT CIVAnalyzerBar::OnError (WPARAM wParam, LPARAM lParam)
{
	if( LOWORD( wParam ) == 1)
	{
		StopAnimation();
		m_bOnline = true;
		m_cbSymbol.EnableWindow(TRUE);
	}

	ErrorMsgBox( g_DataProcessor.GetErrorText() );	
	
	if( HIWORD( wParam ) ) //Terminate
		SendMessage (WM_CLOSE);

	return 0;
}


void CIVAnalyzerBar::OnToolbarDropDown(NMHDR* pnmtb, LRESULT *plr)
{
	CRect rc;
	NMTOOLBAR* pToolbar = reinterpret_cast<NMTOOLBAR*>(pnmtb);
	
	BCMenu * pMenu = NULL;
	switch (pToolbar->iItem)
	{
	case ID_CHARTS:
		{
			size_t i = 0;
			m_wndServicesToolBar.GetToolBarCtrl().GetRect(pToolbar->iItem,rc);
			m_wndServicesToolBar.ClientToScreen(rc);
			
			pMenu = (BCMenu*) m_mnuMain.GetSubMenu(1);
			
			UINT iDelStart = pMenu->GetMenuItemCount();
			
			for (i=0; i < pMenu->GetMenuItemCount(); i++ )
			{
				UINT nID = pMenu->GetMenuItemID(i);
				if (nID == ID_TIME_SKEW)
				{
					iDelStart = i+1;
					break;
				}
			}

			while (iDelStart < pMenu->GetMenuItemCount())
				pMenu->DeleteMenu (iDelStart, MF_BYPOSITION);

			if (m_months.size() > 0)	
				pMenu->AppendMenu (MF_SEPARATOR);
	
			i = 0;
			for (months_map::iterator it = m_months.begin(); it != m_months.end(); it++, i++)
			{
				COleDateTime dtExp = (DATE) it->first.date_;
				
				CString str = dtExp.Format (_T("%B,%d %Y"));
				pMenu->AppendMenu (MF_STRING, ID_STRIKE_SKEW_FIRST + i, str, 
					&m_ilCharts, it->second.m_bValid ? 2 : 3);
			}
		
		}
		break;
	case ID_WINDOW_ARRANGE:
		{
			m_wndServicesToolBar.GetToolBarCtrl().GetRect(pToolbar->iItem,rc);
			m_wndServicesToolBar.ClientToScreen(rc);

			pMenu = (BCMenu*) m_mnuMain.GetSubMenu(2);
		}
		break;
	case ID_OPTIONS:
		{
			m_wndServicesToolBar.GetToolBarCtrl().GetRect(pToolbar->iItem,rc);
			m_wndServicesToolBar.ClientToScreen(rc);

			pMenu = (BCMenu*) m_mnuMain.GetSubMenu(3);
			
		}
		break;
	}
	
	if (pMenu)
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL, rc.left, rc.bottom, this);

	*plr = TBDDRET_DEFAULT;
}


void CIVAnalyzerBar::OnRebarHitTest (NMHDR* lpnmmouse, LRESULT *plr)
{
	*plr = HTTRANSPARENT;
}

void CIVAnalyzerBar::OnRebarClick ( LPNMHDR lpnmh, LRESULT *plr)
{
	*plr = 0;
}



void CIVAnalyzerBar::OnAppBarStateChange (BOOL fProposed, UINT uStateProposed)
{
	if (!fProposed )
	{
		SaveBarState();
	}
}


void CIVAnalyzerBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CAppBar::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (m_bDrawSmallFrame)
	{
		lpncsp[0].rgrc->left +=  GetCaptionSize();
	}
}

void CIVAnalyzerBar::OnNcPaint() 
{
	Default();

	if (m_bDrawSmallFrame)
	{
		CWindowDC dcWin(this);
		
		CRect rc = GetCaptionRect();

		CDC dc;
		dc.CreateCompatibleDC (&dcWin);
		
		CBitmap bmp;
		bmp.CreateCompatibleBitmap (&dcWin, rc.Width(), rc.Height() );
		dc.SelectObject (&bmp);

		CBrush brush;
		brush.CreateSysColorBrush( m_bActiveCaption ? COLOR_ACTIVECAPTION: COLOR_INACTIVECAPTION);
		
		CBrush * pOldBrush = dc.SelectObject (&brush);
		dc.PatBlt (0, 0, rc.Width(), rc.Height(), PATCOPY);
		
		int nIconPos =  rc.Width() > m_nIconSize ? (rc.Width() - m_nIconSize) / 2 : 0 ;

		m_ilBarIcon.Draw (&dc, m_bActiveCaption ? 0 : 1, CPoint (nIconPos, nIconPos), ILD_NORMAL);
		
		dcWin.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&dc,0,0,SRCCOPY);
	}
}

CRect CIVAnalyzerBar::GetCaptionRect(bool bScreen)
{
	CRect rcCli;
	GetClientRect (rcCli);
	ClientToScreen (rcCli);

	CRect rc;
	GetWindowRect (rc);	

	CSize szFrame ( rc.right - rcCli.right, rcCli.top - rc.top);
	
	if (! bScreen)
		rc -= CPoint (rc.left, rc.top);

	rc.left += szFrame.cx;
	rc.top += szFrame.cy;
	
	rc.right = rc.left + GetCaptionSize();
	rc.bottom -= szFrame.cy;
	

	return rc;
}

LRESULT CIVAnalyzerBar::OnNcHitTest(CPoint point) 
{	
	if (PtInIcon(point) )
		return HTSYSMENU;
	else
		if ( PtInCaption(point)  )
			return HTCAPTION;
		else
		{
			CRect rc;
			m_wndStatusBar.GetClientRect(rc);
			m_wndStatusBar.ScreenToClient(&point);

			if (rc.PtInRect (point) && m_wndStatusBar.IsOverURL(point))
				return HTCLIENT;
		}

		return CAppBar::OnNcHitTest(point);

}

void CIVAnalyzerBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CAppBar::OnNcLButtonDown(nHitTest, point);
	if (PtInIcon (point) )
	{
		CRect rc = GetIconRect ();
		
		CRect rcCapt = GetCaptionRect(true);

		ShowMainMenu(CPoint (rcCapt.left, rc.bottom+1));
	}
}

void CIVAnalyzerBar::ShowMainMenu(const CPoint & pt)
{
	
	CMenu * pMenu = m_mnuMain.GetSubMenu(0);

	pMenu->SetDefaultItem (ID_FILE_CLOSE, FALSE);
	pMenu->CheckMenuItem (ID_APPBAR_AUTOHIDE, MF_BYCOMMAND |
		IsBarAutohide() ? MF_CHECKED : MF_UNCHECKED);
	pMenu->CheckMenuItem (ID_APPBAR_ALWAYSONTOP, MF_BYCOMMAND |
		IsBarAlwaysOnTop() ? MF_CHECKED : MF_UNCHECKED);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_VERTICAL, pt.x, pt.y, this);
}

void CIVAnalyzerBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if (m_uStateProposedPrev == ABE_FLOAT)
	{
		lpwndpos->flags = 0;
		lpwndpos->cx = m_sizeMinMax.cx;
		lpwndpos->cy = m_sizeMinMax.cy;
	}
	else
	if ( m_uStateProposedPrev != ABE_UNKNOWN)
	{
		lpwndpos->flags = 0;
		CRect rc (lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy) ;
		GetRect (m_uStateProposedPrev, &rc);
		
		lpwndpos->cx = rc.Width();
		lpwndpos->cy = rc.Height();
	}
	else
		CAppBar::OnWindowPosChanging(lpwndpos);
}



BOOL CIVAnalyzerBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (nID == IDOK || nID == IDCANCEL)
		return FALSE;
	else
		return CAppBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CIVAnalyzerBar::OnAppbarAutohide() 
{
   // Get the current state and change the auto-hide setting only.	
	APPBARSTATE abs;
	abs.m_cbSize = sizeof(abs);
	GetState(&abs);
	abs.m_fAutohide = !IsBarAutohide();
	SetState(abs);
}

BOOL CIVAnalyzerBar::OnNcActivate(BOOL bActive) 
{
	m_bActiveCaption = bActive == TRUE;

	SendMessage(WM_NCPAINT);
	return TRUE;
}


CRect CIVAnalyzerBar::GetIconRect()
{
	CRect rc = GetCaptionRect(true);
	int nIconPos =  rc.Width() > m_nIconSize ? (rc.Width() - m_nIconSize) / 2 : 0 ;
	
	rc.left += nIconPos;
	rc.top += nIconPos;
	rc.bottom = rc.top + m_nIconSize;
	rc.right = rc.left + m_nIconSize;

	return rc;
}

void CIVAnalyzerBar::OnNcRButtonUp(UINT nHitTest, CPoint point) 
{
	ShowMainMenu (point);
}

void CIVAnalyzerBar::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	CAppBar::OnNcLButtonDblClk(nHitTest, point);
}

void CIVAnalyzerBar::OnSysCommand(UINT nID, LPARAM lParam) 
{
	CAppBar::OnSysCommand(nID, lParam);
}

void CIVAnalyzerBar::OnAppbarAlwaysontop() 
{
	APPBARSTATE abs;
	abs.m_cbSize = sizeof(abs);
	GetState(&abs);
	abs.m_fAlwaysOnTop = !IsBarAlwaysOnTop();
	SetState(abs);
}

void CIVAnalyzerBar::InitControlBars()
{
	///////	Main /////////////////////////////////////
	m_wndSymbolToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST);

	CRect rcSymbol;
	m_cbSymbol.GetWindowRect(rcSymbol);

	m_cbSymbol.SetParent (&m_wndSymbolToolBar);
	m_cbSymbol.SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	m_wndSymbolToolBar.GetToolBarCtrl().SetExtendedStyle (TBSTYLE_EX_DRAWDDARROWS);
	m_wndSymbolToolBar.GetToolBarCtrl().SetImageList (&m_ilToolbar);

	m_wndSymbolToolBar.SetButtons (NULL, 2);

	m_wndSymbolToolBar.SetButtonInfo (0, 0, TBBS_SEPARATOR, rcSymbol.Width());
	m_wndSymbolToolBar.SetButtonInfo (1, ID_GO, TBBS_BUTTON, 0);

	m_wndSymbolToolBar.SetButtonTextByCmd (ID_GO);

	m_wndSymbolToolBar.AdjustSizes(1);

	
	///////	Services /////////////////////////////////////
	m_wndServicesToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST );
	m_wndServicesToolBar.GetToolBarCtrl().SetExtendedStyle (TBSTYLE_EX_DRAWDDARROWS);
	m_wndServicesToolBar.GetToolBarCtrl().SetImageList (&m_ilToolbar);

	m_wndServicesToolBar.SetButtons (NULL, 5);

	m_wndServicesToolBar.SetButtonInfo (0, ID_CHARTS, TBSTYLE_DROPDOWN, 1);
	m_wndServicesToolBar.SetButtonInfo (1, 0, TBBS_SEPARATOR, 0);

	m_wndServicesToolBar.SetButtonInfo (2, ID_WINDOW_ARRANGE, TBSTYLE_DROPDOWN, 2);
//	m_wndServicesToolBar.SetButtonInfo (3, ID_OPTIONS, TBSTYLE_DROPDOWN, 3);
	m_wndServicesToolBar.SetButtonInfo (3, 0, TBBS_SEPARATOR, 0);
	m_wndServicesToolBar.SetButtonInfo (4, ID_FILE_CLOSE, TBBS_BUTTON, 4);
	
	m_wndServicesToolBar.SetButtonTextByCmd (ID_CHARTS);
	m_wndServicesToolBar.SetButtonTextByCmd (ID_WINDOW_ARRANGE);
//	m_wndServicesToolBar.SetButtonTextByCmd (ID_OPTIONS);
	m_wndServicesToolBar.SetButtonTextByCmd (ID_FILE_CLOSE);

	m_wndServicesToolBar.AdjustSizes(4);

	//////////////////////////////////////////////////////////	

	
	m_wndInfoBar.Create (IDD_INFO_BAR, this);

	m_wndIconBar.Create (IDD_ICON_BAR, this);

	m_wndStatusBar.Create (this, WS_CHILD | WS_VISIBLE);
	
	UINT aIDs [] = {ID_STATUS, ID_ADVERTIZING};
	m_wndStatusBar.SetIndicators (aIDs, sizeof (aIDs) / sizeof (UINT));
	
	CreateReBar();	

	m_wndStatusBar.SetPaneStyle (0, SBPS_STRETCH | SBPS_NOBORDERS);

	CRect rcStatus;
	m_wndStatusBar.GetClientRect(rcStatus);
	m_wndStatusBar.SetPaneInfo (1, ID_ADVERTIZING, SBPS_NOBORDERS | SBPS_OWNERDRAW, 300);
}



void CIVAnalyzerBar::InitAppBar()
{
	// Set the CAppBar class's behavior flags
	m_fdwFlags = ABF_ALLOWTOPBOTTOM | ABF_ALLOWFLOAT | ABF_MIMICTASKBARAUTOHIDE | ABF_MIMICTASKBARALWAYSONTOP;

	// Setup default state data for the AppBar
	APPBARSTATE abs;
	abs.m_cbSize = sizeof(abs);
	abs.m_uState = ABE_TOP;
	abs.m_fAutohide = FALSE;
	abs.m_fAlwaysOnTop = TRUE;

	// Set the default floating location of the appbar
	GetWindowRect(&abs.m_rcFloat);
	CRect& rc = abs.m_rcFloat;


	CXMLParamsHelper XMLParams;
	if(SUCCEEDED(XMLParams.LoadXMLParams()))
	{
		XMLParams.GetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarState", (long*)&abs.m_uState, -2);
				
		long nFlag = abs.m_fAutohide;
		XMLParams.GetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarAutoHide", &nFlag, 0);
		abs.m_fAutohide = nFlag;
			
		nFlag = abs.m_fAlwaysOnTop;
		XMLParams.GetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarAlwaysOnTop", &nFlag, 1);
		abs.m_fAlwaysOnTop = nFlag;
			
		CPoint pt;
		XMLParams.GetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarLeft", &pt.x, 1);
		XMLParams.GetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarTop", &pt.y, 1);

		if (pt.x < 0) pt.x = 0;
		if (pt.y < 0) pt.y = 0;

		rc += pt;

		CRect  rcWorkArea;
		CPoint ptLeftTop( rc.left, rc.top );
		if( theApp.IsMultyScreenSupported() )
		{
			HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromPoint( ptLeftTop );

			MONITORINFO mi;
			mi.cbSize = sizeof( MONITORINFO );
			CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );
			rcWorkArea = mi.rcWork;
		}
		else
		{
			::SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWorkArea, 0 );
		}

		if( !rcWorkArea.PtInRect(ptLeftTop) )
			rc -= CPoint( rc.left - rcWorkArea.left, rc.top - rcWorkArea.top );

		if( rc.left == rc.right )
			rc.right = rc.left + m_sizeMinMax.cx;
		if( rc.top == rc.bottom )
			rc.bottom = rc.top + m_sizeMinMax.cy;
	}

	abs.m_auDimsDock[ABE_LEFT]   = rc.Width();
	abs.m_auDimsDock[ABE_TOP]    = rc.Height();
	abs.m_auDimsDock[ABE_RIGHT]  = rc.Width();
	abs.m_auDimsDock[ABE_BOTTOM] = rc.Height();

	SetState(abs);
}

void CIVAnalyzerBar::SaveBarState()
{
	if ( !m_bSaveState || m_abs.m_uState == ABE_UNKNOWN )
		return;

	CXMLParamsHelper XMLParams;
	if(FAILED(XMLParams.LoadXMLParams()))
	{
		MessageBox (_T("Unable to write into xml"));
		m_bSaveState = false;
		return;
	}

	CRect rc;
	GetWindowRect (rc);

	XMLParams.SetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarState", m_abs.m_uState);
	XMLParams.SetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarAutoHide", m_abs.m_fAutohide);
	XMLParams.SetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarAlwaysOnTop", m_abs.m_fAlwaysOnTop);
	XMLParams.SetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarLeft", rc.left);
	XMLParams.SetXMLLong(CIVAnalyzerApp::m_sbsXMLKey, L"AppBarTop", rc.top);
	
	if(FAILED(XMLParams.SaveXMLParams()))
	{
		MessageBox (_T("Unable to write into xml"));
		m_bSaveState = false;
	}
}


void CIVAnalyzerBar::OnStructureVola() 
{
#ifdef _VOLA_MANAGER
	CWaitCursor wait;

	if( !::IsWindow(m_wndVM.m_hWnd) )
	{
		CIVAnalyzerOptions opts;
		theApp.GetOptions( opts	 );

		m_wndVM.m_bAlwaysOnTop      = opts.m_bVMAlwaysOnTop;
		m_wndVM.m_bUseExtrapolation = opts.m_bVMUseExtrapolation;

		if( !m_wndVM.Create( CVolaManagementDlg::IDD, GetDesktopWindow() ) )
		{
			ErrorMsgBox( _T("Error opening volatility management window.") );
			return;
		}

		HICON hIcon = LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_IV_APP) );
		SetClassLong( m_wndVM.m_hWnd, GCL_HICON, (long)hIcon );
		
		DWORD dwStyle = GetWindowLong( m_wndVM.m_hWnd, GWL_STYLE );
		SetWindowLong( m_wndVM.m_hWnd, GWL_STYLE, dwStyle | WS_CLIPCHILDREN );

		MONITORINFO mi;
		mi.cbSize = sizeof( MONITORINFO );

		bool bMultyScreen = theApp.IsMultyScreenSupported();

		if( bMultyScreen )
		{
			HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );

			CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );

			WINDOWPLACEMENT wpls;
			wpls.length = sizeof (WINDOWPLACEMENT);
			m_wndVM.GetWindowPlacement(&wpls);

			wpls.showCmd = SW_HIDE;

			wpls.rcNormalPosition = CRect( mi.rcMonitor.left + wpls.rcNormalPosition.left, 
										   mi.rcMonitor.top  + wpls.rcNormalPosition.top,
										   mi.rcMonitor.left + wpls.rcNormalPosition.right,
										   mi.rcMonitor.top  + wpls.rcNormalPosition.bottom ); 
				
			m_wndVM.SetWindowPlacement (&wpls);
		}

		m_wndVM.SetWindowPos( m_wndVM.m_bAlwaysOnTop ? &wndTopMost : &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	}
	else
	{
		WINDOWPLACEMENT wpls;
		wpls.length = sizeof (WINDOWPLACEMENT);
		m_wndVM.GetWindowPlacement(&wpls);

		m_wndVM.SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
		if( wpls.showCmd == SW_SHOWMAXIMIZED )
			m_wndVM.ShowWindow( SW_SHOWMAXIMIZED );
		else
			m_wndVM.ShowWindow( SW_RESTORE );
	}
#endif //_VOLA_MANAGER
}


#ifdef _VOLA_MANAGER
long CIVAnalyzerBar::OnNextDaySurfaceOpen( WPARAM wParam, LPARAM lParam )
{
	CWaitCursor wait;

	// Change current symbol
	if( g_DataProcessor.GetContractSymbol() != (LPCTSTR)wParam )
	{
		m_cbSymbol.SetWindowText( (LPCTSTR)wParam );
		GetData();
	}

	if( !::IsWindow(m_wndVM.m_hWnd) )
	{
		CIVAnalyzerOptions opts;
		theApp.GetOptions( opts	 );

		m_wndVM.m_bAlwaysOnTop		= opts.m_bVMAlwaysOnTop;
		m_wndVM.m_bUseExtrapolation = opts.m_bVMUseExtrapolation;

		if( !m_wndVM.Create( CVolaManagementDlg::IDD, GetDesktopWindow() ) )
		{
			ErrorMsgBox( _T("Error opening volatility management window.") );
			return 0;
		}

		HICON hIcon = LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_IV_APP) );
		SetClassLong( m_wndVM.m_hWnd, GCL_HICON, (long)hIcon );
		
		DWORD dwStyle = GetWindowLong( m_wndVM.m_hWnd, GWL_STYLE );
		SetWindowLong( m_wndVM.m_hWnd, GWL_STYLE, dwStyle | WS_CLIPCHILDREN );

		MONITORINFO mi;
		mi.cbSize = sizeof( MONITORINFO );

		bool bMultyScreen = theApp.IsMultyScreenSupported();

		if( bMultyScreen )
		{
			HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );

			CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );

			WINDOWPLACEMENT wpls;
			wpls.length = sizeof (WINDOWPLACEMENT);
			m_wndVM.GetWindowPlacement(&wpls);

			wpls.showCmd = SW_HIDE;

			wpls.rcNormalPosition = CRect( mi.rcMonitor.left + wpls.rcNormalPosition.left, 
										   mi.rcMonitor.top  + wpls.rcNormalPosition.top,
										   mi.rcMonitor.left + wpls.rcNormalPosition.right,
										   mi.rcMonitor.top  + wpls.rcNormalPosition.bottom ); 
				
			m_wndVM.SetWindowPlacement (&wpls);
		}
	}

	m_wndVM.OpenNextDaySurface( lParam );

	m_wndNDS.SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );

	WINDOWPLACEMENT wpls;
	wpls.length = sizeof (WINDOWPLACEMENT);
	m_wndVM.GetWindowPlacement(&wpls);

	m_wndVM.SetWindowPos( m_wndVM.m_bAlwaysOnTop ? &wndTopMost : &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	if( wpls.showCmd == SW_SHOWMAXIMIZED )
		m_wndVM.ShowWindow( SW_SHOWMAXIMIZED );
	else
		m_wndVM.ShowWindow( SW_RESTORE );

	return 0;
}
#endif //_VOLA_MANAGER


void CIVAnalyzerBar::OnNextDaySurface() 
{
#ifdef _VOLA_MANAGER
	CWaitCursor wait;

	if( !::IsWindow(m_wndNDS.m_hWnd) )
	{
		if( !m_wndNDS.Create( CVolaNextDaySurfaceDlg::IDD, GetDesktopWindow() ) )
		{
			ErrorMsgBox( _T("Error opening next day surface window.") );
			return;
		}

		HICON hIcon = LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_IV_APP) );
		SetClassLong( m_wndNDS.m_hWnd, GCL_HICON, (long)hIcon );
		
		DWORD dwStyle = GetWindowLong( m_wndNDS.m_hWnd, GWL_STYLE );
		SetWindowLong( m_wndNDS.m_hWnd, GWL_STYLE, dwStyle | WS_CLIPCHILDREN );

		MONITORINFO mi;
		mi.cbSize = sizeof( MONITORINFO );

		if( theApp.IsMultyScreenSupported() )
		{
			HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );

			CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );

			WINDOWPLACEMENT wpls;
			wpls.length = sizeof (WINDOWPLACEMENT);
			m_wndNDS.GetWindowPlacement(&wpls);

			wpls.showCmd = SW_HIDE;

			wpls.rcNormalPosition = CRect( mi.rcMonitor.left + wpls.rcNormalPosition.left, 
										   mi.rcMonitor.top  + wpls.rcNormalPosition.top,
										   mi.rcMonitor.left + wpls.rcNormalPosition.right,
										   mi.rcMonitor.top  + wpls.rcNormalPosition.bottom ); 
				
			m_wndNDS.SetWindowPlacement (&wpls);
		}

		m_wndNDS.SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	}
	else
	{
		WINDOWPLACEMENT wpls;
		wpls.length = sizeof (WINDOWPLACEMENT);
		m_wndNDS.GetWindowPlacement(&wpls);

		m_wndNDS.SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
		if( wpls.showCmd == SW_SHOWMAXIMIZED )
			m_wndNDS.ShowWindow( SW_SHOWMAXIMIZED );
		else
			m_wndNDS.ShowWindow( SW_RESTORE );
	}
#endif //_VOLA_MANAGER
}


void CIVAnalyzerBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	ShowMainMenu (point);
}

void CIVAnalyzerBar::OnClose() 
{

#ifdef _VOLA_MANAGER
	CIVAnalyzerOptions opts;
	theApp.GetOptions( opts );

	opts.m_bVMAlwaysOnTop	   = m_wndVM.m_bAlwaysOnTop;
	opts.m_bVMUseExtrapolation = m_wndVM.m_bUseExtrapolation;

	theApp.SaveOptions( opts );

	if( ::IsWindow( m_wndVM.m_hWnd ) )
		m_wndVM.SendMessage( WM_CLOSE );
#endif//_VOLA_MANAGER

	StopProcessing();
	DestroyWindow();
}

void CIVAnalyzerBar::StopProcessing()
{
	CWaitCursor wt;

#ifdef _VOLA_MANAGER
	g_DataProcessor.ReleaseVolaManager();
#endif//_VOLA_MANAGER

	g_DataProcessor.Stop();
	
	WaitForSingleObject( g_DataProcessor.m_hThread, INFINITE );
}

void CIVAnalyzerBar::InitConnection()
{
	if (m_db.IsConnected())
		m_db.Disconnect();

	m_db.Connect(); // Global connection for initializing connection pooling
}

bool CIVAnalyzerBar::ShowStrikeSkew(DATE dtMonth, CMonthData &md)
{
	CChartWnd  * pWnd = md.m_pWnd;
	if (!pWnd)
	{
		pWnd = new CChartWnd(dtMonth);
		if( pWnd->Create() )
		{
			md.m_pWnd = pWnd;

			MONITORINFO mi;
			mi.cbSize = sizeof( MONITORINFO );

			if( theApp.IsMultyScreenSupported() )
			{
				HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );

				CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );

				WINDOWPLACEMENT wpls;
				wpls.length = sizeof (WINDOWPLACEMENT);
				pWnd->GetWindowPlacement(&wpls);

				wpls.showCmd = SW_HIDE;

				wpls.rcNormalPosition = CRect( mi.rcMonitor.left + wpls.rcNormalPosition.left, 
											   mi.rcMonitor.top  + wpls.rcNormalPosition.top,
											   mi.rcMonitor.left + wpls.rcNormalPosition.right,
											   mi.rcMonitor.top  + wpls.rcNormalPosition.bottom ); 
					
				pWnd->SetWindowPlacement (&wpls);
			}
		}
		else
		{
			delete pWnd;
			pWnd = NULL;
		}
	}
	
	if( pWnd )
		pWnd->Show();
	else
		return false;

	return true;
}


void CIVAnalyzerBar::OnWindowCloseall() 
{
#ifdef _VOLA_MANAGER
	if( IsWindow( m_wndVM ) )
		m_wndVM.ShowWindow(SW_HIDE);
	if( IsWindow( m_wndNDS ) )
		m_wndNDS.ShowWindow(SW_HIDE);
#endif //_VOLA_MANAGER

	if ( IsWindow(m_timeSkewWnd) &&  m_timeSkewWnd.IsWindowVisible()  )
		m_timeSkewWnd.ShowWindow(SW_HIDE);

	for (months_map::iterator it = m_months.begin(); it != m_months.end(); it++)
	{
		CChartWnd * pWnd = it->second.m_pWnd;

		if (pWnd  &&  pWnd->IsWindowVisible())
			pWnd->ShowWindow(SW_HIDE);
	}
}


void CIVAnalyzerBar::OnEditchangeSymbols() 
{
//	m_cbSymbol.ShowDropDown (TRUE);
}

BOOL CIVAnalyzerBar::PreTranslateMessage(MSG* pMsg) 
{
	return CAppBar::PreTranslateMessage(pMsg);
}


void CIVAnalyzerBar::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CAppBar::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
	{
		SetFocus();
	}
}

void CIVAnalyzerBar::OnGo() 
{
	m_cbSymbol.SetEditSel (0, -1);

	GetData();
}


void CIVAnalyzerBar::OnEditupdateSymbols() 
{

}

void CIVAnalyzerBar::OnSelectSymbol() 
{
	GetData ();
}

void CIVAnalyzerBar::OnCancelSymbol() 
{
	CString strSymbol = g_DataProcessor.GetContractSymbol();
	if (!strSymbol.IsEmpty())
		m_cbSymbol.SetWindowText(strSymbol);
}

void CIVAnalyzerBar::OnOptions() 
{
	CIVAnalyzerOptions optsOld, optsNew;
	theApp.GetOptions(optsOld);
		
//	COptionsSheet dlg( _T("Options") ) ;
//	dlg.m_psh.dwFlags &= ~PSH_HASHELP;
	COptionsDlg dlg;
	
	if ( dlg.DoModal() == IDOK)
	try
	{
		theApp.GetOptions(optsNew);
		
		if ( optsNew.m_strConnection  != optsOld.m_strConnection)
		{
			InitConnection();

			LoadSymbols();
			
			GetData();
		}
		else
		if (optsNew.m_enCurveMode != optsOld.m_enCurveMode || 
		   optsNew.m_enCalculationMode != optsOld.m_enCalculationMode ||
		   (optsNew.m_IVOpts != optsOld.m_IVOpts  && optsNew.m_PriceProvOpts != optsOld.m_PriceProvOpts ))			
			GetData();
		else
		if (optsNew.m_IVOpts != optsOld.m_IVOpts)
			g_DataProcessor.ReloadRanges();
		else
		if (optsNew.m_PriceProvOpts != optsOld.m_PriceProvOpts || optsNew.m_enProviderType !=optsOld.m_enProviderType )
		{
			StopOnlineIndication();
			g_DataProcessor.ReloadPricesConnection(optsNew.m_enProviderType !=optsOld.m_enProviderType);
		}
		else
		if( optsNew.m_enChartMode != optsOld.m_enChartMode ||
		  (optsNew.m_enChartMode == optsOld.m_enChartMode && (optsNew.m_fChartRangeFrom != optsOld.m_fChartRangeFrom || optsNew.m_fChartRangeTo != optsOld.m_fChartRangeTo) ||
		  optsNew.m_bFlatNoBid != optsOld.m_bFlatNoBid ||
		  optsNew.m_bFlatOutRange != optsOld.m_bFlatOutRange )
		  )
		{
			g_DataProcessor.ReloadSettings();
			UpdateWindows( true );
		}
		
	}
	catch (_com_error & e)
	{
		ErrorMsgBox ( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage() );
	}
}

void CIVAnalyzerBar::SaveLastSymbol()
{
	CXMLParamsHelper XMLParams;
	if(FAILED(XMLParams.LoadXMLParams()))
	{
		MessageBox (_T("Unable to write into xml"));
		return;
	}

	CString strText;
	m_cbSymbol.GetWindowText(strText);
	XMLParams.SetXMLString(CIVAnalyzerApp::m_sbsXMLKey, L"LastSymbol", (LPCTSTR)strText);

	if(FAILED(XMLParams.SaveXMLParams()))
		MessageBox (_T("Unable to write into xml"));
}

void CIVAnalyzerBar::OnAppAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
	
}

void CIVAnalyzerBar::OnAppHelp() 
{
	CWnd* pHelpWnd = NULL;
    
    pHelpWnd = FindWindow(NULL, _T("ETS Volatility Manager"));
    if(!pHelpWnd)
	{

		TCHAR szBuf[_MAX_PATH];

		DWORD dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));

		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFname[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];
		_tsplitpath_s( szBuf, szDrive,_MAX_DRIVE, szDir,_MAX_DIR, szFname,_MAX_FNAME, szExt,_MAX_EXT );

		TCHAR szHelpPath[_MAX_PATH];
		_sntprintf_s(szHelpPath,_MAX_PATH, _T("%s%sDocuments/ETS Volatility Manager.chm"), szDrive, szDir);

        if( (int)(ShellExecute(NULL, _T("Open"), szHelpPath, NULL, NULL, SW_SHOWNORMAL)) <= 32 )
		{
			CString strErr = _T("Fail to open help file '");
			strErr += szHelpPath;
			strErr += _T("'.");

			MessageBox (strErr,NULL,MB_OK|MB_ICONSTOP);

		}
	}
    else
	{
        if(pHelpWnd->IsIconic())
			pHelpWnd->ShowWindow(SW_RESTORE);
		else
			pHelpWnd->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
}

void CIVAnalyzerBar::OnStrikeSkew() 
{
	for (months_map::iterator it = m_months.begin(); it != m_months.end(); it++)
	{
		if (it->second.m_bValid)
		{
			if ( ! ShowStrikeSkew (it->first.date_, it->second) )
				break;
		}
	}
}

void CIVAnalyzerBar::OnWindowArrange() 
{
	SendMessage (WM_COMMAND, m_nNextArrange);
}


void CIVAnalyzerBar::OnImportAll()
{
	if ( MessageBox (_T("Are you sure want to update market structure for all symbols?"), NULL,
		MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES ) 
		return;

//	g_DataProcessor.CancelData();
	
	CMktStructureDlg dlg;
	dlg.DoModal();
	if( dlg.IsSymbolUpdated ( GetCurSymbol() ) )
		GetData();
}


void CIVAnalyzerBar::OnCreateDefaultSurfaces()
{
	if ( MessageBox (_T("Are you sure want to create default volatilities?"), NULL,
		MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES ) 
		return;

	CCreateDefaultVolaDlg dlg;
	dlg.DoModal();
	GetData();
}


void CIVAnalyzerBar::OnImportCurrentSymbol()
{
	CString strText = GetCurSymbol();
	
	int i = m_cbSymbol.FindStringExact (-1, strText);
	int iData = m_cbSymbol.GetItemData (i);
//	g_DataProcessor.CancelData();

	CMktStructureDlg dlg(strText, iData);
	dlg.DoModal();
	if( dlg.IsSymbolUpdated( GetCurSymbol() ) || dlg.IsFeedStopped() )
	{
		int ret = m_cbSymbol.FindStringExact(-1,GetCurSymbol());
		if(ret!=CB_ERR)
		{
			m_cbSymbol.SetCurSel(ret);
		}
		GetData();
	}
}

void CIVAnalyzerBar::OnIRCurve()
{
	CIRCurve dlg;
	try
	{
		if(-1 == dlg.DoModal())
		{
			CString strError;
			strError.Format(_T("Error creating Interest Rates Dialog (%d)"), GetLastError());
			ErrorMsgBox(strError);	
		}
		else
			if (dlg.AreRatesChanged())
			{
				g_DataProcessor.ReloadRates();
			}
	}
	catch (_com_error & e)
	{
		ErrorMsgBox ( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage() );
	}
}

void CIVAnalyzerBar::OnCharts() 
{
	for (months_map::reverse_iterator rit = m_months.rbegin(); 
		rit != m_months.rend(); rit++)
	{
		if (rit->second.m_bValid)
		{
			if ( ! ShowStrikeSkew (rit->first.date_, rit->second) )
				break;
		}
	}

	OnTimeSkew();
	SendMessage (WM_COMMAND, m_nCurArrange);
}


BOOL CIVAnalyzerBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint pt (GetMessagePos ());
	
	CRect rc;
	m_wndStatusBar.GetWindowRect (rc);
	
	if (rc.PtInRect (pt) )
		return m_wndStatusBar.SetCursor (&m_wndStatusBar, HTCLIENT, message);
	else
		return CAppBar::OnSetCursor(pWnd, nHitTest, message);
}

void CIVAnalyzerBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	m_wndStatusBar.ScreenToClient(&point);

	CRect rc;
	m_wndStatusBar.GetClientRect(rc);
	
	if (rc.PtInRect (point))
	{
		m_wndStatusBar.LButtonDown (nFlags, point);
	}
	else
		CAppBar::OnLButtonDown(nFlags, point);
}

void CIVAnalyzerBar::OnFileClose() 
{
	PostMessage( WM_CLOSE );
}

void CIVAnalyzerBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CAppBar::OnSettingChange(uFlags, lpszSection);

	setlocale (LC_ALL, "");
	
	if ( (uFlags == SPI_GETNONCLIENTMETRICS) && IsWindow(m_wndReBar))
	{
		m_cbSymbol.ShowWindow(SW_HIDE);
		m_cbSymbol.SetParent (this);
		m_wndReBar.DestroyWindow();

		InitControlBars();
		ResizeControls();
		if (m_bAnimation)
			StartAnimation();

	}
}


void CIVAnalyzerBar::GetChartWndsList(vec_wnds & rwnds)
{
	rwnds.reserve (m_months.size() +1 );	

/*	if ( IsWindow(m_timeSkewWnd) && m_timeSkewWnd.IsWindowVisible() )
		rwnds.push_back(&m_timeSkewWnd);
	
	for (months_map::reverse_iterator rit = m_months.rbegin(); 
		rit != m_months.rend(); rit++)
	{
		CChartWnd * pWnd = rit->second.m_pWnd;
		if ( pWnd  && pWnd->IsWindowVisible() )
			rwnds.push_back(pWnd);
	}
*/

	for (CWnd * pWnd = GetDesktopWindow()->GetTopWindow(); pWnd  ; pWnd = pWnd->GetNextWindow())
	{
		if (pWnd->IsKindOf (RUNTIME_CLASS (CChartWnd)) && pWnd->IsWindowVisible()  )
			rwnds.push_back(pWnd);

	}
}

void CIVAnalyzerBar::TileWnds(const vec_wnds &wnds, int nColCnt )
{
	int nCnt = wnds.size();
	if (nCnt == 0 ) return;

	MONITORINFO mi;
	mi.cbSize = sizeof( MONITORINFO );

	CRect rcScreen;
	if( theApp.IsMultyScreenSupported() )
	{
		HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );

		CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );
		rcScreen = mi.rcWork;
	}
	else
	{
		SystemParametersInfo (SPI_GETWORKAREA, 0,  &rcScreen, 0 );
	}
	
	int nColWidth = rcScreen.Width() / nColCnt;
	div_t divRows =  div ( nCnt, nColCnt);
	
	vec_wnds::const_iterator it = wnds.begin();
	const CWnd * pLast = &wndTop;

	for( int iCol = 0; iCol < nColCnt; iCol++ )
	{
		int nRows =  divRows.quot;
		if( divRows.rem - (nColCnt  - 1 - iCol) > 0 )
			nRows ++;
		
		int nHeight = rcScreen.Height() / nRows;
		for (int i = 0; i <  nRows ; i++, it++ )
		{
			ASSERT (it !=  wnds.end() );
			CWnd * pWnd = *it;	
			
			WINDOWPLACEMENT wpls;
			wpls.length = sizeof (WINDOWPLACEMENT);
			pWnd->GetWindowPlacement(&wpls);
			
			wpls.showCmd = SW_SHOWNOACTIVATE;
			if( theApp.IsMultyScreenSupported() )
				wpls.rcNormalPosition = CRect( CPoint( mi.rcMonitor.left + iCol * nColWidth, mi.rcMonitor.top + i * nHeight ),
											   CSize( nColWidth, nHeight ) ); 
			else
				wpls.rcNormalPosition = CRect( CPoint( iCol * nColWidth, i * nHeight ),
											   CSize( nColWidth, nHeight ) ); 
				
			pWnd->SetWindowPlacement (&wpls);

			pLast = pWnd;
		}
	}
}


void CIVAnalyzerBar::OnWindowTileHorz() 
{
	m_nCurArrange  = ID_WINDOW_TILE_HORZ;
	m_nNextArrange = ID_WINDOW_TILE_VERT;

	vec_wnds wnds;
	GetChartWndsList (wnds);
	
	int nCnt =  wnds.size();

	if (nCnt == 0 ) return;

	int nMaxRows = 3;			
	int nMaxCols = 1;			
	
	bool bIncCols = true;
	while (true)
	{
		int nCols  =  nCnt / nMaxRows;
		int nRemain  =  nCnt %  nMaxRows;
		if (nRemain > 0 ) nCols++;

		if ( nCols > nMaxCols )
		{
			if (bIncCols)
			{
				nMaxCols++;
				bIncCols = false;
			}
			else
			{
				nMaxRows++;
				bIncCols = true;
			}
		}
		else
			break;
	}

	TileWnds (wnds, nMaxCols);
}

void CIVAnalyzerBar::OnWindowTileVert() 
{
	m_nCurArrange  = ID_WINDOW_TILE_VERT;
	m_nNextArrange = ID_WINDOW_CASCADE;

	vec_wnds wnds;
	GetChartWndsList (wnds);

	int nCnt =  wnds.size();
	if (nCnt == 0 ) return;

	int nMaxRows = 1;			
	int nMaxCols = 3;			
	
	bool bIncCols = false;
	while (true)
	{
		int nRows  =  nCnt / nMaxCols;
		int nRemain  =  nCnt %  nMaxCols;
		if (nRemain > 0 ) nRows++;

		if ( nRows > nMaxRows )
		{
			if (bIncCols)
			{
				nMaxCols++;
				bIncCols = false;
			}
			else
			{
				nMaxRows++;
				bIncCols = true;
			}
		}
		else
		{
			nMaxCols = nCnt / nMaxRows;
			break;
		}
	}

	TileWnds (wnds, nMaxCols);
}

void CIVAnalyzerBar::OnWindowCascade() 
{
	m_nCurArrange  = ID_WINDOW_CASCADE;
	m_nNextArrange = ID_WINDOW_TILE_HORZ;

	vec_wnds wnds;
	GetChartWndsList (wnds);

	int nCnt =  wnds.size();
	if (nCnt == 0 ) return;

	int nMargin = GetSystemMetrics (SM_CYCAPTION) + GetSystemMetrics (SM_CXSIZEFRAME)-1;

	MONITORINFO mi;
	mi.cbSize = sizeof( MONITORINFO );

	CRect rcScreen;
	if( theApp.IsMultyScreenSupported() )
	{
		HMONITOR hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );

		CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );
		rcScreen = mi.rcWork;
	}
	else
	{
		SystemParametersInfo (SPI_GETWORKAREA, 0,  &rcScreen, 0 );
	}
	
	int nScrWidth  = rcScreen.Width(); 
	int nScrHeight = rcScreen.Height(); 

	// Solving quadratic equation - calculating the value that is necesssary to indent 
	// from each edge of the working area to get a half of area value

	int nDelta = 0;
	{
		double a = 1;
		double b = -(nScrWidth + nScrHeight);
		double c =  (nScrWidth * nScrHeight) / 2.0;

		double d = b * b - 4 * a * c;
		if (d > 0  )
		{
			double x1 = (-b + sqrt(d)) / 2*a;
			double x2 = (-b - sqrt(d)) / 2*a;
			if (x2 > 0 || x1 > 0)
			{
				nDelta = static_cast<int>(
					 x2 > 0  && x2 < nScrWidth && x2 < nScrHeight 
										? x2 : ( x1 > 0 ? x1 : 0 )); 
				nDelta =   nDelta / nMargin * nMargin;
			}
		}
	}

	int nWidth  = nScrWidth - nDelta;
	int nHeight = nScrHeight - nDelta;

	vec_wnds::reverse_iterator rit = wnds.rbegin(); 
	for( int i=0 ; rit != wnds.rend() ; rit++, i++ )
	{
		CWnd * pWnd = *rit;	
		
		WINDOWPLACEMENT wpls;
		wpls.length = sizeof (WINDOWPLACEMENT);
		pWnd->GetWindowPlacement(&wpls);

		wpls.showCmd = SW_SHOWNOACTIVATE;
		CPoint pt;
		if( theApp.IsMultyScreenSupported() )
			pt = CPoint( mi.rcMonitor.left + i * nMargin, mi.rcMonitor.top + i * nMargin );
		else
			pt = CPoint( i * nMargin, i * nMargin );
		
		if ( (nScrWidth  < pt.x + nWidth + nMargin) ||
			 (nScrHeight < pt.y + nHeight + nMargin) ) 
		{
			pt.x = nScrWidth - nWidth;
			pt.y = nScrHeight - nHeight;
			i = -1;
		}
			
		wpls.rcNormalPosition  = CRect (pt ,CSize( nWidth, nHeight) ); 
		pWnd->SetWindowPlacement (&wpls);
	}
}


void CIVAnalyzerBar::OnUpdateAdvertizing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
	CHAR szBuffer[1024] = {0};
	int dwSize = sizeof(szBuffer);
	if(!LoadString(theApp.GetAppearance(),IDS_ADV_DEFAULT, szBuffer, dwSize))
		pCmdUI->SetText ( CString ( (LPCSTR) IDS_ADV_DEFAULT)); 
	else
		pCmdUI->SetText ( szBuffer); 
}


void CIVAnalyzerBar::OnUpdateStatus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
	CString strText = _T(' ');
	strText += g_DataProcessor.GetStatusText();
	pCmdUI->SetText( strText );
}


void CIVAnalyzerBar::OnUpdateGo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( m_bOnline);
}


void CIVAnalyzerBar::InitImageList(CImageList &lst, UINT nID)
{
	lst.Create (16, 15, ILC_COLORDDB | ILC_MASK, 10, 2);

	CBitmap    bmp;	
	BOOL bResult = bmp.LoadBitmap (nID);
	ASSERT (bResult);

	lst.Add (&bmp, RGB (192, 192, 192));
}

void CIVAnalyzerBar::CreateReBar()
{
	m_wndReBar.Create (this, RBS_FIXEDORDER | RBS_VARHEIGHT | RBS_BANDBORDERS );	
	m_wndReBar.AdjustBorders();

	m_wndReBar2.Create (this, RBS_FIXEDORDER | RBS_VARHEIGHT | RBS_BANDBORDERS );
	m_wndReBar2.AdjustBorders();

	m_wndReBar2.AddBar (&m_wndSymbolToolBar, _T("Symbol"), NULL, RBBS_NOGRIPPER | RBBS_FIXEDBMP  );
	m_wndReBar2.AddBar (&m_wndServicesToolBar, _T(" "), NULL, RBBS_NOGRIPPER | RBBS_FIXEDBMP  );
	m_wndReBar2.AddBar (&m_wndStatusBar, NULL, NULL, RBBS_NOGRIPPER | RBBS_FIXEDBMP | RBBS_BREAK);


	m_wndReBar.AddBar (&m_wndIconBar, NULL, NULL, RBBS_FIXEDBMP | RBBS_NOGRIPPER);
	m_wndReBar.AddBar (&m_wndReBar2, NULL, NULL, RBBS_FIXEDBMP | RBBS_NOGRIPPER);
	m_wndReBar.AddBar (&m_wndInfoBar, NULL, NULL, RBBS_FIXEDSIZE | RBBS_NOGRIPPER);
	
	CRect rc;
	GetClientRect(&rc);

	CReBarCtrl & rctl2 =   m_wndReBar2.GetReBarCtrl();
	
	UINT nTotalWidth = 0;
	UINT nMaxHeight = 0;
	int nStatusHeight = 0;
	size_t i = 0;

	for (i = 0; i < rctl2.GetBandCount() -1; i++ )
	{

		REBARBANDINFO bi;
		bi.cbSize = sizeof (REBARBANDINFO);
		bi.fMask = RBBIM_SIZE | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_HEADERSIZE;
		rctl2.GetBandInfo (i, &bi);
		
		nTotalWidth += (bi.cxMinChild + bi.cxHeader);
		if (bi.cyMinChild > nMaxHeight )
			nMaxHeight = bi.cyMinChild;
	}


	REBARBANDINFO bi;
	bi.cbSize = sizeof (REBARBANDINFO);
	bi.fMask = RBBIM_SIZE | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_HEADERSIZE;
	rctl2.GetBandInfo (rctl2.GetBandCount() -1, &bi);
	
	nStatusHeight = bi.cyMinChild;
	nMaxHeight += bi.cyMinChild;

	CReBarCtrl & rctl =   m_wndReBar.GetReBarCtrl();
	bi.cbSize = sizeof (REBARBANDINFO);
	bi.fMask = RBBIM_SIZE | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_HEADERSIZE;
	rctl.GetBandInfo (rctl.GetBandCount() - 1, &bi);

	CSize sizeIB = m_wndInfoBar.m_picLogo.GetSize();
	bi.cxMinChild = sizeIB.cx + 6;
	bi.cyMinChild = sizeIB.cy + 6;

	rctl.SetBandInfo (rctl.GetBandCount() - 1, &bi);
	
	nTotalWidth += (bi.cxMinChild + bi.cxHeader);
	if (bi.cyMinChild > nMaxHeight )
		nMaxHeight = bi.cyMinChild;

	bi.fMask = RBBIM_SIZE | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_HEADERSIZE;
	rctl.GetBandInfo (0, &bi);
	
	nTotalWidth += (bi.cxMinChild + bi.cxHeader);
	if (bi.cyMinChild > nMaxHeight )
		nMaxHeight = bi.cyMinChild;

	for (i = 0; i < rctl2.GetBandCount() - 1; i++ )
	{
		REBARBANDINFO bi;
		bi.cbSize = sizeof (REBARBANDINFO);
		bi.fMask =  RBBIM_CHILDSIZE;

		rctl2.GetBandInfo (i, &bi);
		
		bi.cyMinChild = nMaxHeight - nStatusHeight - 2;

		rctl2.SetBandInfo (i, &bi);		
	}
	bi.fMask = RBBIM_CHILDSIZE;
	rctl2.GetBandInfo (rctl2.GetBandCount() - 1, &bi);
	bi.cyMinChild = nStatusHeight + 2;

	rctl2.SetBandInfo (i, &bi);		

	CRect rcBorders;
	rctl2.GetBandBorders (0, rcBorders);
	nMaxHeight += rcBorders.top + rcBorders.bottom;

	CRect rcReBar; rcReBar.SetRectEmpty();
	m_wndReBar.CalcInsideRect (rcReBar, TRUE);
	nTotalWidth += (35 + rcReBar.left);

	rc.bottom = nMaxHeight;
	rc.right = nTotalWidth;
	
	AdjustWindowRectEx(&rc, GetStyle(), FALSE, GetExStyle());
	
	m_sizeMinMax.cx = rc.Width();
	m_sizeMinMax.cy = rc.Height();	
	
	SetWindowPos (NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOMOVE  );
}


void CIVAnalyzerBar::InitImages()
{
	m_ilBarIcon.Create (8, 8, ILC_COLORDDB, 2, 1);

	CBitmap    bmp;	
	BOOL bResult = bmp.LoadBitmap (IDB_BAR_ICON);
	ASSERT (bResult);

	m_ilBarIcon.Add (&bmp,RGB(0,0,0));
	
	
	InitImageList (m_ilToolbar, IDB_TB_IMAGES);
	InitImageList (m_ilCharts, IDB_CHARTS_IMAGES);
	InitImageList (m_ilWindows, IDB_WNDS_IMAGES);
	InitImageList (m_ilSvcs, IDB_SERVICES_IMAGES);
}

void CIVAnalyzerBar::InitMenus()
{
	m_mnuMain.LoadMenu (IDR_MAIN_MENU);
	m_mnuMain.LoadToolbar (IDR_SYSMENU_TOOLBAR);
	
	m_mnuMain.ModifyODMenu (NULL, ID_CHARTS,	&m_ilCharts, 0);
	m_mnuMain.ModifyODMenu (NULL, ID_TIME_SKEW, &m_ilCharts, 1);

	m_mnuMain.ModifyODMenu (NULL, ID_WINDOW_ARRANGE,   &m_ilWindows, 0);
	m_mnuMain.ModifyODMenu (NULL, ID_WINDOW_CLOSEALL,  &m_ilWindows, 1);
	m_mnuMain.ModifyODMenu (NULL, ID_WINDOW_TILE_HORZ, &m_ilWindows, 2);
	m_mnuMain.ModifyODMenu (NULL, ID_WINDOW_TILE_VERT, &m_ilWindows, 3);
	m_mnuMain.ModifyODMenu (NULL, ID_WINDOW_CASCADE,   &m_ilWindows, 4);
	
	m_mnuMain.ModifyODMenu (NULL, ID_OPTIONS,			    &m_ilSvcs, 0);
	m_mnuMain.ModifyODMenu (NULL, ID_SERVICE_INTERESTRATES, &m_ilSvcs, 1);
	
#ifdef _VOLA_MANAGER
	m_mnuMain.ModifyODMenu (NULL, ID_STRUCTURE_VOLA, &m_ilSvcs, 2);
	m_mnuMain.ModifyODMenu (NULL, ID_NEXTDAYSURFACE, &m_ilSvcs, 2);
	m_mnuMain.ModifyODMenu (NULL, ID_SYMBOL_GROUPS,	 &m_ilSvcs, 2);
	m_mnuMain.ModifyODMenu (NULL, ID_RULES,			 &m_ilSvcs, 2);
#else			
	m_mnuMain.RemoveMenu (ID_STRUCTURE_VOLA, MF_BYCOMMAND);
#endif 
	
	m_mnuMain.ModifyODMenu (NULL, ID_SERVICE_IMPORTALL,	    &m_ilSvcs, 3);
	m_mnuMain.ModifyODMenu (NULL, ID_SERVICE_GENDEFVOLA,    &m_ilSvcs, 3);
	m_mnuMain.ModifyODMenu (NULL, ID_SERVICE_CURRENTSYMBOL, &m_ilSvcs, 4);
	m_mnuMain.ModifyODMenu (NULL, ID_ADD_NEW_SYMBOL,		&m_ilSvcs, 5);
	m_mnuMain.ModifyODMenu (NULL, ID_REMOVE_SYMBOL,			&m_ilSvcs, 6);
	int iu = m_ilSvcs.GetImageCount();

}

int CIVAnalyzerBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CAppBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxGetApp()->m_pMainWnd = this;
	
	return 0;
}


void CIVAnalyzerBar::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_CHARTS_UPDATE_TIMER)
	{
		notif_set ns;
		m_volaNotifClient.PopNotications (ns);
		
		for( notif_set::iterator it = ns.begin(); it != ns.end(); it++ )
		{
			switch( it->m_enType )
			{
			case CVolaNotification::enAll:
				UpdateWindows();
				return;
			case CVolaNotification::enTimeSkew:
				m_timeSkewWnd.UpdateChart( false );
				break;	
			case CVolaNotification::enMonth:
				{
					months_map::iterator itm = m_months.find( it->m_dtMonth );
					if( itm != m_months.end() )
					{
						CChartWnd * pWnd = itm->second.m_pWnd;
						if( pWnd )
							pWnd->UpdateChart( false );
					}
				}
				break;	
			};
		}

		return;
	}
	
	CAppBar::OnTimer(nIDEvent);
}

void CIVAnalyzerBar::OnRemoveSymbol() 
{
	CRemoveSymbolDlg dlg(&m_cbSymbol);
	bool bCurrent = false;
	int idx = CB_ERR;
	if(IDOK == dlg.DoModal())
	{
		CWaitCursor wait;
		DWORD deleteID = dlg.GetSelectedID();

		g_DataProcessor.CancelData();
		if(deleteID!=0)
		{
			// Delete current symbol or not
			idx = m_cbSymbol.GetCurSel();
			if(CB_ERR != idx)
			{
				if( m_cbSymbol.GetItemData(idx) == deleteID )
					bCurrent = true;
			}
		}
		Sleep(500);
		// Delete ...
		CDBLayout db;
		try 
		{
			db.Connect();
			db.BeginTran();
			if(db.DeleteSymbol(deleteID))
			{
				db.Commit();
				if(!bCurrent)
				{
					CString strText;
					dlg.GetSelectedText(strText);
					int id = m_cbSymbol.FindStringExact(-1, strText);
					if(CB_ERR != id)
						m_cbSymbol.DeleteString(id);
				}
				else
					m_cbSymbol.DeleteString(m_cbSymbol.GetCurSel());
			}
			else
			{
				db.Rollback();
				MessageBox( _T("Unable to Delete Symbol"), NULL, MB_OK|MB_ICONERROR|MB_TOPMOST );
			}
		}
		catch(_com_error& e)
		{
			db.Rollback();
			MessageBox( e.Description(), NULL, MB_OK|MB_ICONERROR|MB_TOPMOST );
		}
		if(bCurrent)
		{
			if(m_cbSymbol.GetCount())
			{
				if(CB_ERR == m_cbSymbol.SetCurSel(idx>0?idx-1:0))
				{
					m_cbSymbol.SetCurSel(0);
				}
			}
			else
			{
				StopAnimation();
				m_cbSymbol.SetWindowText(_T(""));
				SaveLastSymbol();
				m_months.invalidate();
				g_DataProcessor.GetData(_T(""), -1 , m_months);
				m_months.closeInvalid();
				UpdateWindows( true );

				SetFocus();
				m_cbSymbol.EnableWindow(TRUE);
			}
		}
		GetData();
	}
}


void CIVAnalyzerBar::OnAddNewSymbol() 
{
	AddSymbol();
}

void CIVAnalyzerBar::OnSymbolGroups() 
{
#ifdef _VOLA_MANAGER

	CSymbolGroupsDlg dlg;

	try
	{
		dlg.LoadGroups();
		dlg.DoModal();
	}
	catch(_com_error& e)
	{
		ErrorMsgBox(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
	}

#endif
}

void CIVAnalyzerBar::OnRules() 
{
#ifdef _VOLA_MANAGER

	CRulesDlg dlg;

	try
	{
		dlg.LoadRules();
		dlg.DoModal();
	}
	catch(_com_error& e)
	{
		ErrorMsgBox(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
	}

#endif
}

void CIVAnalyzerBar::StopOnlineIndication()
{
	StopAnimation();
	SetFocus();
	m_bOnline = false;
	m_cbSymbol.EnableWindow(FALSE);
}

LRESULT CIVAnalyzerBar::OnAddSymbol(WPARAM wParam, LPARAM lParam)
{
	TCHAR* pBuffer = reinterpret_cast<TCHAR*>(wParam);
	if(pBuffer)
	{
		AddSymbol(pBuffer);
		delete  pBuffer;
	}
	return 0;
}

void CIVAnalyzerBar::AddSymbol(LPCTSTR szSymbol)
{

	CAddSymbolDlg dlg(szSymbol);
	dlg.DoModal();

	if (dlg.IsAddedSomething() || (szSymbol && *szSymbol))
	try
	{
		bool bWasEmpty = m_cbSymbol.GetCount() == 0;

		LoadSymbols();

		if (bWasEmpty || szSymbol != NULL || dlg.IsFeedStopped())
		GetData();
	}
	catch (_com_error & e)
	{
		ErrorMsgBox ( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage() );
	}
	m_cbSymbol.EnableWindow(TRUE);
}

BOOL CIVAnalyzerBar::OnHelpInfo( HELPINFO* lpHelpInfo )
{
	return TRUE;
}


LRESULT CIVAnalyzerBar::OnStockChanged (WPARAM wParam, LPARAM lParam)
{
	PSTR pData = reinterpret_cast<PSTR>(wParam);
	if(pData)
	{

		int iCur = m_cbSymbol.GetCurSel();
		int i = m_cbSymbol.FindStringExact(-1, pData);
		if (i != -1 && i!= iCur)
		{
			m_cbSymbol.SetCurSel (i);
			GetData();
		}

		delete pData;
	}
	return 0;
}

//LRESULT CIVAnalyzerBar::OnNcHitTest(CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return CAppBar::OnNcHitTest(point);
//}
