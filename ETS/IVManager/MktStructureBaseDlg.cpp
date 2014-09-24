// MktStructureBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "MktStructureBaseDlg.h"
#include "DBLayout.h"
#include "DataProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDataProcessor g_DataProcessor;
/////////////////////////////////////////////////////////////////////////////
// CMktStructureBaseDlg dialog

#define WM_REQUEST_YIELD WM_USER+10

const int CMktStructureBaseDlg::m_nPosPerContract=800;


_ATL_FUNC_INFO CMktStructureBaseDlg::m_OnStockInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };
 
_ATL_FUNC_INFO CMktStructureBaseDlg::m_OnOptionInfo = 
	{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD,VT_RECORD,VT_I2} };

_ATL_FUNC_INFO CMktStructureBaseDlg::m_OnErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };


_ATL_FUNC_INFO CMktStructureBaseDlg::m_OnSOAPErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_BSTR}};

_ATL_FUNC_INFO CMktStructureBaseDlg::m_OnSOAPResultInfo = 
	{CC_STDCALL, VT_EMPTY, 3, {VT_BSTR, VT_VARIANT, VT_VARIANT}};


_ATL_FUNC_INFO CMktStructureBaseDlg::m_OnIVAuthErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_BSTR}};

_ATL_FUNC_INFO CMktStructureBaseDlg::m_OnIVAuthResultInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};


CMktStructureBaseDlg::CMktStructureBaseDlg(UINT nID, CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent),
	 m_bConnected (false), m_bStarted (false),
	 m_bCanceled (false), m_nCounter(0),m_bFeedStopped(false)

{
	//{{AFX_DATA_INIT(CMktStructureBaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMktStructureBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMktStructureBaseDlg)
	DDX_Control(pDX, IDC_STOCK_PROGRESS, m_pgContract);
	DDX_Control(pDX, IDC_LOG, m_edLog);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_STOCK_INFO, m_stInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMktStructureBaseDlg, CDialog)
	//{{AFX_MSG_MAP(CMktStructureBaseDlg)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_MESSAGE (WM_REQUEST_YIELD, OnRequestYield)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMktStructureBaseDlg message handlers
void CMktStructureBaseDlg::StatusMessage(UINT  iMessage, bool bLog)
{
	CString strMessage;
	if(strMessage.LoadString(iMessage))
	{
		StatusMessage((LPCTSTR)strMessage, bLog);
	}
}

void CMktStructureBaseDlg::StatusMessage(LPCTSTR szMessage, bool bLog)
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

void CMktStructureBaseDlg::ErrorMessage (const _com_error & e)
{
	StatusMessage ( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage()  );
}

STDMETHODIMP CMktStructureBaseDlg::OnStock(PP::StockParams *Params, PP::StockInfo *Results)
{
	USES_CONVERSION;

	HRESULT hr = S_OK;
	DoEvents();
	if(m_bCanceled  ) return hr;
	
	stocks_map::iterator it =  m_stocks.find (Params->Stock);
	if (it != m_stocks.end())
	try
	{
		it->second.m_bGotData = true;
		
		if (! HandleStock(Params, Results)  )
		{
			CDBLayout db;
			db.Connect(); 
			
			db.UpdateContractData (it->second.m_nID, Params->Stock, Results);

			db.Disconnect();
		}

		CString strMsg;
		strMsg.Format(IDS_MKTSTRUCTURE_GOTDATA, OLE2T(Params->Stock));
		
		StatusMessage (strMsg, false);
		UpdateProgress(it);
	}
	catch (_com_error & e)
	{
		ErrorMessage(e);
		Stop();
	}


	return hr;
}

STDMETHODIMP CMktStructureBaseDlg::OnOption(PP::OptionParams *Params, PP::OptionInfo *Results, VARIANT_BOOL bLast)
{
	USES_CONVERSION;

	HRESULT hr = S_OK;
	DoEvents();
	if(m_bCanceled) return hr;
/*
	if(bLast==VARIANT_FALSE && m_opts.m_enProviderType == enMyTrackPriceProvider)
	{
		if( fEQZero(Results->Last) && fEQZero(Results->Bid) && fEQZero(Results->Ask) )
			return S_OK;
	}
*/
	if(bLast==VARIANT_FALSE && Results->ExpirationDate < int(vt_date::GetCurrentDate()))
		return S_OK;

	stocks_map::iterator it =  m_stocks.find (Params->UnderlyingSymbol);
	if (it != m_stocks.end())
	try
	{
		USES_CONVERSION;

		if (bLast)
		{
			it->second.m_bGotOptions = true;
			UpdateProgress(it);
			return hr;
		}
		
		CDBLayout db;
		db.Connect(); 
		
		db.UpdateOptionData (it->second.m_nID, Params, Results);

		db.Disconnect();
		

		CString strMsg;
		strMsg.Format(IDS_MKTSTRUCTURE_GOTOPTION, OLE2T(Params->UnderlyingSymbol),
			OLE2T(Results->Series) );

		it->second.m_lGotOptions++;		
		StatusMessage (strMsg, false);
		UpdateProgress(it);
	}
	catch (_com_error & e)
	{
		ErrorMessage(e);
		Stop();
	}

	return hr;
}

STDMETHODIMP CMktStructureBaseDlg::OnError(PP::ErrorNumberEnum ErrorNumber, BSTR Description, PP::RequestsTypeEnum ReqType, VARIANT Request)
{
	HRESULT hr = S_OK;
	
	DoEvents();
	if(m_bCanceled) return S_OK;

	CString strError;
	_bstr_t bstrSymbol;
	
	USES_CONVERSION;
	
	switch (ErrorNumber)
	{
	case PP::enSymbolNotSupported:
	case PP::enNoDataAvailableForSymbol:
	case PP::enNoAccessForData:
		{
			if ( (Request.vt & VT_RECORD) == VT_RECORD )
			{
				if (ReqType == PP::enRequestStock )
				{
					PP::StockParams * pParams = (PP::StockParams *) Request.pvRecord;
					bstrSymbol =  pParams->Stock;
				}
				else
				{
					PP::OptionParams * pParams = (PP::OptionParams *) Request.pvRecord;
					bstrSymbol =  pParams->UnderlyingSymbol;
				}
				
				stocks_map::iterator it =  m_stocks.find ( bstrSymbol);
				if (it != m_stocks.end())
				{
					CContractRequestData & data = it->second;
					if (! data.m_bError)
					{
						it->second.m_bGotData = true;
						it->second.m_bGotYield = true;
						it->second.m_bError = true;

						strError.Format (IDS_MKTSTRUCTURE_ERR_GETDATA, (LPCTSTR)bstrSymbol, (LPCTSTR) CString (Description) );
						StatusMessage (strError);

						UpdateProgress(it); 
					}
				}
			}
		}
		return hr;
	default:
		strError.Format (IDS_MKTSTRUCTURE_ERROR, OLE2T(Description) );
		StatusMessage (strError);
		Stop();
	}
	return hr;
}

bool CMktStructureBaseDlg::StartDownload(bool bDoSubscribe  )
{
	m_edLog.SetWindowText (_T(""));
	m_bCanceled = false;
	m_nCounter = 0;
	m_pgContract.SetPos (0);

	g_DataProcessor.CancelData(); 
	Sleep(1000);
	m_bFeedStopped = true;

	try
	{
		StatusMessage (IDS_MKTSTRUCTURE_INITDATA);
		
		if (m_stocks.size() == 0 )
		{
			CDBLayout db;
			db.Connect();

			CClientRecordset rs;
			db.GetSymbols(rs);
			for (; ! rs.IsEOF() ;++rs)
			{
				DoEvents();
				if (m_bCanceled) false;
				
				CContractRequestData crd = (int) rs["iContractID"];
				crd.m_bGotYield = (int) rs[L"iContractTypeID"]  != 1;

				m_stocks[ rs[L"vcSymbolName"] ] = crd;
			}
			db.Disconnect();
		}
		else
		if (m_stocks.size()  == 1)
		{
			CContractRequestData & crd = m_stocks.begin()->second;
			_bstr_t bsFirast = m_stocks.begin()->first;
			if ( crd.m_nID != 0)
			{
				CDBLayout db;
				db.Connect();

				CClientRecordset rs;
				db.GetSymbolParams( crd.m_nID, rs);
				if (! rs.IsEOF())
					crd.m_bGotYield = (int) rs[L"iContractTypeID"]  != 1;
				db.Disconnect();
			}
		}


		m_pgContract.SetRange32 (0, m_stocks.size()*m_nPosPerContract);
		m_pgContract.SetStep (1);
		
		if (m_stocks.size() > 0 )
		{
			if (! m_bConnected )
			{
				StatusMessage (IDS_MKTSTRUCTURE_CONNECTING);

				m_spStructureInfo->Connect();
				m_bConnected = true;
		
				StatusMessage (IDS_MKTSTRUCTURE_REQUESTING);
			}

			m_itCur = m_stocks.begin();

			if (bDoSubscribe)
				SubscribeNextStock ();
		}
		else
		{
			StatusMessage (IDS_MKTSTRUCTURE_ERR_NOSTOCKS);
			Stop();
			return false;
		}

		return true;
	}
	catch (_com_error & e) 
	{
		ErrorMessage(e);
		Stop();
	}

	return false;
	
}

void CMktStructureBaseDlg::OnCancel() 
{
	if( Stop() )
	{
		StatusMessage (IDS_MKTSTRUCTURE_CANCELED);
	}
	else
		CDialog::OnCancel();
}


bool CMktStructureBaseDlg::Stop()
{
	bool bResult = false; 
	if (!m_bCanceled)
	{
		m_bCanceled = true;
		m_btCancel.SetWindowText (_T("Close"));

		if (m_bConnected)
		{
			m_spStructureInfo->raw_CancelOptions();
			m_spStructureInfo->raw_CancelStock();
		}

		m_ivc.StopRequests();
	
		bResult = true;
	}
	OnStop();
	return bResult;
}


void CMktStructureBaseDlg::OnClose() 
{
	EndDialog (IDCANCEL);
}

HBRUSH CMktStructureBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( pWnd->GetDlgCtrlID() == IDC_LOG )
	{
		pDC->SetBkColor (GetSysColor(COLOR_WINDOW));
		hbr = GetSysColorBrush (COLOR_WINDOW);
	}
	
	return hbr;
}

void CMktStructureBaseDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	m_ivc.Term();
	
	if (m_spStructureInfo)
	{
		if (m_bConnected)
			m_spStructureInfo->raw_Disconnect();
		
		m_bConnected = false;

		_IMktStructureEventsImpl::DispEventUnadvise (m_spStructureInfo);
		m_spStructureInfo=NULL;
	}

}

BOOL CMktStructureBaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	theApp.GetOptions(m_opts);
	
	m_ivc.Init();

	HRESULT hr = S_OK;

#ifndef _MULTIPROVIDERS
		 hr = m_spStructureInfo.CreateInstance (__uuidof (MTP::MyTrackStructureInfo) );
#else
		 hr = m_spStructureInfo.CreateInstance (__uuidof (PP::StructureInfo) );
		 if(SUCCEEDED(hr))
		 {
			 PP::IProviderPtr spProv = m_spStructureInfo;
			 spProv->PutType(m_opts.m_enProviderType);
		 }
#endif

	
	if ( FAILED(hr) ) 
	{
		MessageBox  ( _T("Error creating structure provider instance") );	
		EndDialog(IDCANCEL);
		return FALSE;
	}

	_IMktStructureEventsImpl::DispEventAdvise (m_spStructureInfo);	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMktStructureBaseDlg::DoEvents()
{
	MSG msg;

	// Process existing messages in the application's message queue.
	// When the queue is empty, do clean up and return.
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) && !m_bCanceled)
	{
		if (!AfxGetThread()->PumpMessage())
			return;
	}
}



void CMktStructureBaseDlg::UpdateProgress(stocks_map::iterator it)
{	
	CContractRequestData & data = it->second;

	if ( ( data.m_bGotData && data.m_bGotOptions && data.m_bGotYield) || data.m_bError  )
	{
		m_pgContract.SetPos ( ++m_nCounter * m_nPosPerContract );
		
		CString str;

		if (! data.m_bError)
		{
			str.Format (IDS_MKTSTRUCTURE_GOT, it->second.m_lGotOptions, (LPCTSTR) it->first);
			StatusMessage (str);
		}
		
		m_stocks.erase (m_itCur);
		m_itCur = m_stocks.begin();

		if ( m_itCur == m_stocks.end() )
		{
			StatusMessage(IDS_MKTSTRUCTURE_DONE);
			Stop();
		}
		else
			SubscribeNextStock();		

	}
	else
	{
		int nPos = m_pgContract.GetPos ();
		if (nPos < ( (m_nCounter + 1)  * m_nPosPerContract  -1) )
			m_pgContract.StepIt();
	}
}

void CMktStructureBaseDlg::SubscribeNextStock()
{
	if ( m_itCur == m_stocks.end() )
		return;

	try
	{
		PP::StockParams stock;
		stock.Exchange = NULL;
		stock.Stock = m_itCur->first;

		PP::OptionParams opt;
		opt.Exchange = NULL;
		opt.UnderlyingSymbol = stock.Stock;

		CDBLayout db;
		db.Connect(); 			
#ifdef _VOLA_MANAGER
		db.ClearOptionsDataCache(m_itCur->second.m_nID);
#else
		db.ClearOptopnsList(m_itCur->second.m_nID);
#endif//_VOLA_MANAGER
		db.Disconnect();

		m_spStructureInfo->RequestStock( &stock );
		m_spStructureInfo->RequestOptions( &opt );
		
		if (! m_itCur->second.m_bGotYield)
			RequestYield();
 	}
	catch (_com_error & e)
	{
		ErrorMessage(e);
		Stop();
	}
}



CString CMktStructureBaseDlg::GetErrorMessage(HRESULT hr, BSTR bstrDesc)
{
	CString strResult = bstrDesc;
	strResult.TrimRight();
	
	if (strResult.IsEmpty()  )
	{
		_com_error err = hr;
		strResult = err.ErrorMessage();
	}
	CString strFmt;
#ifdef _DEBUG
	strFmt.Format (_T("%s error code %X"), (LPCTSTR) strResult, hr);
#else
	strFmt = strResult;
#endif

	return strFmt;
}

void CMktStructureBaseDlg::RequestYield()
{
	PostMessage (WM_REQUEST_YIELD);
}

LRESULT CMktStructureBaseDlg::OnRequestYield (WPARAM wParam, LPARAM lParam)
{
	double dYield = 0;
	
	m_ivc.SetParams (m_opts.m_IVOpts);
	if (m_ivc.GetYield ( m_itCur->first, dYield))
		try
		{
			CContractRequestData & crd = m_itCur->second;
			
			CDBLayout db;
			db.Connect();
			db.UpdateYield( crd.m_nID, dYield);
			db.Disconnect();

			crd.m_bGotYield = true;

			CString strMsg;
			strMsg.Format(IDS_MKTSTRUCTURE_GOTYELD, (LPCTSTR) m_itCur->first );
			StatusMessage (strMsg, false);
		}
		catch (_com_error & e)
		{
			ErrorMessage(e);
			Stop();
		}
	else
		if (m_ivc.IsError() && !m_ivc.IsStopped() )
		{
			StatusMessage (m_ivc.GetErrorMessage());
			Stop();
		}

	return 0;
}