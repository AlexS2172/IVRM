// TradesUploaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TradesUploader.h"
#include "TradesUploaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTradesUploaderApp theApp;

#define LCID_ENGLISH_USA  MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT )

/////////////////////////////////////////////////////////////////////////////
// time utils
int MonthDays[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

BOOL OleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
	WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return FALSE;

	BOOL bLeapYear = ((wYear & 3) == 0) &&
		((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth =
		MonthDays[wMonth] - MonthDays[wMonth-1] +
		((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

	if (wDay < 1 || wDay > nDaysInMonth ||
		wHour > 23 || wMinute > 59 ||
		wSecond > 59)
	{
		return FALSE;
	}

	long nDate;
	double dblTime;

	nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 +
		MonthDays[wMonth-1] + wDay;

	if (wMonth <= 2 && bLeapYear)
		--nDate;

	nDate -= 693959L;

	dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		((long)wMinute * 60L) +  // mins in seconds
		((long)wSecond)) / 86400.;

	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
static UINT indicators[] =
{
	ID_SEPARATOR
};

/////////////////////////////////////////////////////////////////////////////
// CTradesUploaderDlg dialog

CTradesUploaderDlg::CTradesUploaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTradesUploaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradesUploaderDlg)
	m_dtFileDate = COleDateTime::GetCurrentTime();	
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bConnected = FALSE;
}

CTradesUploaderDlg::~CTradesUploaderDlg()	
{
}

void CTradesUploaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradesUploaderDlg)
	DDX_Control(pDX, IDC_CONNSTRING, m_edConnString);
	DDX_Control(pDX, IDC_DATE, m_ctlDateTime);
	DDX_Control(pDX, IDC_LOGEDIT, m_edLogView);
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_dtFileDate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTradesUploaderDlg, CDialog)
	//{{AFX_MSG_MAP(CTradesUploaderDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
    ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDUPLOAD, OnUpload)	
    ON_BN_CLICKED(IDC_CLOSE, OnCancel)	
    ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradesUploaderDlg message handlers

BOOL CTradesUploaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();     

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	GetDlgItem(IDC_LOGEDIT, &m_hLogEdit);
	GetDlgItem(IDUPLOAD, &m_hBtnUpload);
	GetDlgItem(IDC_CLOSE, &m_hBtnCancel);	
	
    CRect rcParent;
	CRect rcChild;
	CRect rcButton;

	::GetWindowRect(m_hWnd, &rcParent);
	::GetWindowRect(m_hLogEdit, &rcChild);
	::GetWindowRect(m_hBtnUpload, &rcButton);

	ScreenToClient(rcParent);
	ScreenToClient(rcChild);
	ScreenToClient(rcButton);
	
	m_nPadWidth = rcParent.bottom - rcChild.bottom -4;	
	m_nButtonRange = rcButton.top - rcChild.bottom;

	m_hAppStop = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hStopped = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hTradesReady = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	try
	{
		HRESULT hr = m_spPubManager.CreateInstance(__uuidof(MsgManager));
		if( FAILED(hr) )
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create MsgManager object.");

		hr = m_spTradeUpdate.CreateInstance(__uuidof (TradeUpdate));
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create TradeUpdate object.");

		m_spPubManager->RegPublisher(enMtTradeUpdate, NULL);

		UINT nID = 0;
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, _Thread, this, 0,&nID);
		if (!m_hThread)
			utils::ThrowErrorNoSetErrorInfo(E_HANDLE, L"Failed to start DB operation thread.");

		hr = GetConnectionString();
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to get DB connection string.");

		m_edConnString.SetWindowText(m_bsConnStringLabel);

		if (!InitLogPath())
			utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"Failed to initialize logs path.\nInstallation package is invalid or corrupted.");
	}
	catch(const _com_error& e)
	{
		_bstr_t bsErr = L"CRITICAL ERROR: " + e.Description();		
		
		AddLogString(bsErr);
		
		::EnableWindow(m_hBtnUpload, FALSE);
		::SetFocus(m_hBtnCancel);
		
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTradesUploaderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CTradesUploaderDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
    
	CWnd* pWnd = GetDlgItem(IDC_LOGEDIT);
    if (!pWnd)
        return;
	
    CRect rcChild;
	pWnd->GetWindowRect(rcChild);
	ScreenToClient(rcChild);
	rcChild.right = cx - rcChild.left;
    rcChild.bottom = cy - m_nPadWidth;
	pWnd->MoveWindow(rcChild);

	int nLogEditBottom = rcChild.bottom;

	pWnd = GetDlgItem(IDC_STATIC);
    if (!pWnd)
        return;

	pWnd->GetWindowRect(rcChild);
	ScreenToClient(rcChild);
	rcChild.right = cx - rcChild.left;
	pWnd->MoveWindow(rcChild);

	pWnd = GetDlgItem(IDUPLOAD);
	if (!pWnd)
		return;

	pWnd->GetWindowRect(rcChild);
	ScreenToClient(rcChild);
	int nButtonHeight = rcChild.bottom - rcChild.top;
	rcChild.top = nLogEditBottom + m_nButtonRange;
	rcChild.bottom = rcChild.top + nButtonHeight;	
	pWnd->MoveWindow(rcChild);

	pWnd = GetDlgItem(IDC_CLOSE);
	if (!pWnd)
		return;

	pWnd->GetWindowRect(rcChild);
	ScreenToClient(rcChild);
	rcChild.top = nLogEditBottom + m_nButtonRange;
	rcChild.bottom = rcChild.top + nButtonHeight;	
	pWnd->MoveWindow(rcChild);
}

HCURSOR CTradesUploaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTradesUploaderDlg::OnCancel() 
{
    ::SetEvent(m_hAppStop);

    if (m_bConnected)
        while ( MsgWait(1,&m_hStopped,INFINITE) != WAIT_OBJECT_0 );

    CDialog::OnCancel();
}

void CTradesUploaderDlg::OnDestroy() 
{
	if (m_hStopped)
		::CloseHandle(m_hStopped);

	if (m_hAppStop)
		::CloseHandle(m_hAppStop);

	if (m_hTradesReady)
		::CloseHandle(m_hTradesReady);

	try
    {     
        if(m_spPubManager)
		    m_spPubManager->UnregPublisher(enMtTradeUpdate, NULL);
	    // give time for transport to prepare for shutdown..
	   
	    Sleep(2000);
	   
	    m_spTradeUpdate = 0;
	    m_spPubManager = 0;
    }
    catch(_com_error&) { }
}

void CTradesUploaderDlg::OnUpload() 
{
	m_edLogView.Clear();

    if ( FAILED(Start()) )
        return;
    
    if (!CreateFileName())
	{	
		AddLogString( _T("ERROR: Unable to create filename.") );
		return;
	}

    
    m_hFile = CreateFile( m_lpstrLogName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    
    if (m_hFile == INVALID_HANDLE_VALUE)
    {        
        AddLogString(_T("ERROR: Unable to open the log file."));        
        Stop();
        return;
    }
    
    DWORD dwBufSize = 1;
    DWORD dwRead = 0;
    DWORD dwOffset = 0;
    DWORD dwRes = 0;
    char pBuf[512] = { 0 };

    CAutoLock Lock(m_Lock);	

    m_TradesVec.clear();    
    m_dwStored = 0;
    m_dwPublished = 0;

    string sScraps = "";
    string sRecord = "";

    while ( dwBufSize != 0 )
    {
        if ( sScraps.size() > 0 )
        {   
            sRecord += sScraps;
            sScraps = "";
        }

        if ( !ReadFile( m_hFile, pBuf, sizeof(pBuf), &dwBufSize, NULL) )
        {
            AddLogString( _T("ERROR: Unable to read from the log file.") );
            ::CloseHandle(m_hFile);
            Stop();            
            return;
        }
        sRecord.append( pBuf, dwBufSize );
        
        dwOffset = 0;
        while ( dwOffset < sRecord.size() )
        {  
           if ( ( dwRes = ParseRecord( sRecord, dwOffset, &dwRead, dwBufSize) ) != ERROR_SUCCESS )
           {
               if( dwRes == ERROR_INSUFFICIENT_BUFFER )
               {                      
                   sScraps.append( sRecord, dwOffset, sRecord.size() - dwOffset );
                   sRecord = "";
                   break;
               }
               else
               {   
                   m_TradesVec.clear();
                   AddLogString( _T("CRITICAL ERROR: Unable to parse the log file.") );
                   ::CloseHandle(m_hFile);
                   Stop();                   
                   return;
               }                   
           }
               
           dwOffset += dwRead;
           dwOffset +=2;
           m_TradesVec.push_back(m_Trade);
        }
    }

	::CloseHandle(m_hFile);
    ::SetEvent(m_hTradesReady);
}

HRESULT CTradesUploaderDlg::GetConnectionString()
{
   try
   {
       HRESULT hr = m_spEtsSettings.CreateInstance(__uuidof (Settings));
	   if(FAILED(hr))
		   utils::ThrowErrorNoSetErrorInfo(hr, L"CRITICAL ERROR: Failed to create Settings object.");
       

       m_bsConnectionString = m_spEtsSettings->DatabaseConnection;

	   
       _ConnectionPtr spConn;
   	   hr = spConn.CreateInstance( __uuidof(Connection) );
       if ( FAILED(hr) )
	 		 utils::ThrowErrorNoSetErrorInfo(hr, L"CRITICAL ERROR: Failed to create Connection object.");

       spConn->ConnectionString = m_bsConnectionString;
	   PropertiesPtr spParams = spConn->Properties;
 		
	   _bstr_t bs = spParams->Item[L"Data Source"]->Value;
	   bs += L"\\";
	   bs += (_bstr_t)spParams->Item[L"Initial Catalog"]->Value;
	   m_bsConnStringLabel = bs;
   }	
   catch(_com_error& e)
   {
       m_spEtsSettings = 0;
	   return e.Error();
   }

   m_spEtsSettings = 0;

   return S_OK;
}

HRESULT CTradesUploaderDlg::Start()
{
    USES_CONVERSION;
    
    HRESULT hr;
	if( FAILED(hr = GetConnectionString()) )
		return hr;    
    
    if (m_bConnected)
        return E_FAIL;

	try
	{
		AddLogString(_T("Connecting to database..."));
        m_Connection.Open(m_bsConnectionString);
        m_bConnected = TRUE;
        AddLogString(_T("Connected."));
	}
	catch(_com_error& e)
	{
		m_Connection.Close();
		_bstr_t bsError = "ERROR: Unable to connect database: " + e.Description();
        AddLogString((LPCTSTR)bsError);
		
        return e.Error();
	}
    
    return S_OK;
}

void CTradesUploaderDlg::Stop()
{
    if (m_bConnected)
    {
		try
		{
			m_Connection.Close();
			m_bConnected = FALSE;
			AddLogString( _T("Disconnected.") );
		}
		catch(_com_error&)
		{
		}

		::Sleep(500);
	}

    ::SetEvent(m_hStopped);
}

BOOL CTradesUploaderDlg::CreateFileName()
{    
    TCHAR lpstrFileDate[100] = { 0 };
    
	SYSTEMTIME st;
    if ( m_ctlDateTime.GetTime(&st) != ERROR_SUCCESS )
        return FALSE;

    _stprintf( lpstrFileDate, _T("_%2.2i_%2.2i_%4.4i"), st.wDay, st.wMonth, st.wYear );	

    _tcscpy( m_lpstrLogName, FAILES_FILENAME_BEGIN );
    _tcscat( m_lpstrLogName, lpstrFileDate );
    _tcscat( m_lpstrLogName, FAILES_FILENAME_END );

    return TRUE;
}

void CTradesUploaderDlg::AddLogString( LPCTSTR lpszFormatStr, ... )
{
    va_list arg_list;
	va_start(arg_list, lpszFormatStr);
	TCHAR szBuf[4096] = {0};

	_vsntprintf(szBuf, sizeof(szBuf)/sizeof(TCHAR), lpszFormatStr, arg_list);

    CString strText;	

	int nLen = m_edLogView.GetWindowTextLength();
    if (nLen > 20000)
    {
        m_edLogView.GetWindowText(strText);
        m_edLogView.Clear();
        m_edLogView.SetWindowText(strText.Right(10000));
        nLen = m_edLogView.GetWindowTextLength();
    }    
    
	if (nLen > 0)
		strText = _T("\r\n");

	strText += szBuf;    

	m_edLogView.SetSel ( nLen, -1 );	
	m_edLogView.ReplaceSel (strText);
}

//////////////////////////////////////////////////////////////////////////
#define FIND_NEXT_PIECE        lPosFirst = lPosNext; \
                               lPosNext = sRecord.find_first_of('|', lPosFirst+1 ); \
                               if (dwBufSize != 0) \
                               if ( lPosNext == -1 ) return ERROR_INSUFFICIENT_BUFFER;

//////////////////////////////////////////////////////////////////////////
#define MAKE_I4_CONVERSION     sToConvert = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) ); \
                               bsIn = _bstr_t(sToConvert.c_str()).copy(); \
                               hr = ::VarI4FromStr( bsIn, LCID_ENGLISH_USA, LOCALE_NOUSEROVERRIDE, &lRes ); \
                               ::SysFreeString(bsIn); \
                               if (FAILED(hr))  return ERROR_INVALID_DATA; 
//////////////////////////////////////////////////////////////////////////
#define MAKE_R4_CONVERSION     sToConvert = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) ); \
                               bsIn = _bstr_t(sToConvert.c_str()).copy(); \
                               hr = ::VarR4FromStr( bsIn, LCID_ENGLISH_USA, LOCALE_NOUSEROVERRIDE, &fRes ); \
                               ::SysFreeString(bsIn); \
                               if (FAILED(hr)) return ERROR_INVALID_DATA;

//////////////////////////////////////////////////////////////////////////
#define MAKE_R8_CONVERSION     sToConvert = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) ); \
                               bsIn = _bstr_t(sToConvert.c_str()).copy(); \
                               hr = ::VarR8FromStr( bsIn, LCID_ENGLISH_USA, LOCALE_NOUSEROVERRIDE, &dRes ); \
                               ::SysFreeString(bsIn); \
                               if (FAILED(hr)) return ERROR_INVALID_DATA;

//////////////////////////////////////////////////////////////////////////
bool CTradesUploaderDlg::ConvertDateTime(double dIn, CFixTimeValue& tValue)
{
	SYSTEMTIME st = {0};
	if (!::VariantTimeToSystemTime(dIn, &st))
		return false;
	
	
	tValue.tm_year = st.wYear - 1900;
	tValue.tm_mon = st.wMonth - 1;	
	tValue.tm_mday = st.wDay;
	tValue.tm_hour = st.wHour;
	tValue.tm_min = st.wMinute;
	tValue.tm_sec = st.wSecond;		
	
	return true;
}

bool CTradesUploaderDlg::ConvertDateTime(double dIn, CFixDateValue& tValue)
{
	SYSTEMTIME st = {0};
	if (!::VariantTimeToSystemTime(dIn, &st))
		return false;
	
	tValue.tm_year = st.wYear - 1900;
	tValue.tm_mon = st.wMonth - 1;	
	tValue.tm_mday = st.wDay;	
	tValue.tm_hour = 0;
	tValue.tm_min = 0;
	tValue.tm_sec = 0;
	
	return true;
}

bool CTradesUploaderDlg::ConvertDateTime(double dIn, CFixMonthYearValue& tValue, CFixDayOfMonthValue& dValue)
{
	SYSTEMTIME st = {0};
	if (!::VariantTimeToSystemTime(dIn, &st))
		return false;

	tValue.tm_year = st.wYear - 1900;
	tValue.tm_mon = st.wMonth - 1;	
	tValue.tm_mday = 1;
	tValue.tm_hour = 0;
	tValue.tm_min = 0;
	tValue.tm_sec = 0;

	dValue.tm_year = 70;
	dValue.tm_mon = 0;
	dValue.tm_mday = st.wDay;
	tValue.tm_hour = 0;
	tValue.tm_min = 0;
	tValue.tm_sec = 0;
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
DWORD CTradesUploaderDlg::ParseRecord( const string sRecord, const DWORD dwOffset, 
                                      DWORD *pdwRead, const DWORD dwBufSize )
{
    USES_CONVERSION;

	HRESULT	hr;
    string	sToConvert;
    BSTR	bsIn = 0;    

    long	lRes = 0;
    float	fRes = 0;
	double	dRes = 0;
    
    long lPosFirst = sRecord.find_first_of('|', dwOffset );
    if ( lPosFirst == -1 )
        //return ERROR_INVALID_DATA;
        return ERROR_INSUFFICIENT_BUFFER;
    string sNumOfChars = sRecord.substr( dwOffset, lPosFirst-dwOffset );
    
    DWORD dwRecordLen = 0;
    if ( VarUI4FromStr( A2W(sNumOfChars.c_str()), LCID_ENGLISH_USA, LOCALE_NOUSEROVERRIDE, &dwRecordLen ) != S_OK )
        return ERROR_INVALID_DATA;
    dwRecordLen += sNumOfChars.size();
    int sfr = sRecord.size();

    if ( sRecord.size() - dwOffset < dwRecordLen )
        return ERROR_INSUFFICIENT_BUFFER;
    
    long lPosNext = sRecord.find_first_of('|', lPosFirst+1 );
    if ( lPosNext == -1 )
        return ERROR_INSUFFICIENT_BUFFER;
    
    m_Trade.sAccount = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) );

    ///////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.sExecID = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) );

    ///////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.cExecTransType = sRecord.at(lPosFirst+1);

    ////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    MAKE_R4_CONVERSION
    
    m_Trade.fLastPrice = fRes;

    ///////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE    
    
    MAKE_I4_CONVERSION
    
    m_Trade.iLastShares = lRes;

    ///////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.cOrderStatus = sRecord.at(lPosFirst+1);

    ///////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.cSide = sRecord.at(lPosFirst+1);

    ///////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.sSymbol = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) );

    ///////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.sRoot = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) );

    ///////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE    
    
	MAKE_R8_CONVERSION
    
    if (dRes != 0.)
    {
		if (!ConvertDateTime(dRes, m_Trade.ftTransactTime))
            return ERROR_INVALID_DATA;
    }

    ///////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE    
    
    MAKE_R8_CONVERSION
    
    if (dRes != 0.)
    {
		if (!ConvertDateTime(dRes, m_Trade.fdTradeDate))
            return ERROR_INVALID_DATA;
	}

    /////////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.sSecurityType = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) );

    /////////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE

    m_Trade.sText = sRecord.substr( lPosFirst+1, lPosNext - (lPosFirst+1) );

    /////////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE
	
    MAKE_R8_CONVERSION

	if (dRes != 0.)
    {
		if (!ConvertDateTime(dRes, m_Trade.fmyMaturityMonthYear, m_Trade.fmdMaturityDay))
            return ERROR_INVALID_DATA;
	}
	
    /////////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE    

	MAKE_I4_CONVERSION

	if ( lRes != -1 )
        m_Trade.iPutOrCall = lRes;

    /////////////////////////////////////////////////////////////////////
    FIND_NEXT_PIECE    

	MAKE_R4_CONVERSION	    
	 
	if ( fRes != -1 )
        m_Trade.fStrikePrice = fRes;

    /////////////////////////////////////////////////////////////////////    
    FIND_NEXT_PIECE

    m_Trade.sUnderlying = sRecord.substr( lPosFirst+1, (dwOffset+dwRecordLen) - (lPosFirst+1) );

    /////////////////////////////////////////////////////////////////////     
    *pdwRead = dwRecordLen;

    return ERROR_SUCCESS;
}

DWORD CTradesUploaderDlg::StoreInDatabase(const CTrade& Trade)
{
    USES_CONVERSION;

	DWORD	dwTries = 10;
	bool	bOk = false;
	DWORD	dwRes = ERROR_SUCCESS;    
    long    lMaxSeqNum;
    long    lMinSeqNum;
    long    lStructureMissed;

	do
	{
		try
		{
			switch(Trade.cExecTransType)
			{
				case ETT_NEW:
				{                    
                    CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_New");

					sp << _bstr_t(Trade.sAccount.c_str()) << _bstr_t(Trade.sExecID.c_str());
					sp << ((Trade.cSide == SIDE_BUY) ? (int)1 : int(0));
					sp << Trade.iLastShares;
					sp << Trade.fLastPrice;

					DATE dtTradeDate;
					OleDateFromTm(Trade.ftTransactTime.tm_year + 1900, 
								  Trade.ftTransactTime.tm_mon + 1,
								  Trade.ftTransactTime.tm_mday, 
								  Trade.ftTransactTime.tm_hour,
								  Trade.ftTransactTime.tm_min, 
								  Trade.ftTransactTime.tm_sec,
								  dtTradeDate);

					sp << (vt_date)dtTradeDate;
					sp << _bstr_t(Trade.sSymbol.c_str());

					if(Trade.sSecurityType != T2A(ST_OPTION))
						sp << (int)2;
					else
					{
						sp << (int)3;
						sp << _bstr_t(Trade.sUnderlying.c_str());
						sp << (Trade.iPutOrCall == PC_CALL);
						sp << Trade.fStrikePrice;

						DATE dtExpiryDate;
						OleDateFromTm(Trade.fmyMaturityMonthYear.tm_year + 1900, 
									  Trade.fmyMaturityMonthYear.tm_mon + 1,
									  Trade.fmdMaturityDay.tm_mday, 
									  0, 0, 0,
								     dtExpiryDate);
						sp << (vt_date)dtExpiryDate;
                        sp << CDBNull();
                        sp << (BYTE)0;
                        sp << CDBNull();
                        sp << _bstr_t(Trade.sRoot.c_str());
					}

					rs.Open (sp);
                    if(rs.IsEOF())
                    {
                        rs.Close();                        
						utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"EOF reached.");
                    }                    

                    lMaxSeqNum = rs[L"iMaxSeqNum"];
                    lMinSeqNum = rs[L"iMinSeqNum"];
                    lStructureMissed = rs[L"iStructureMissed"];
                    rs.Close();
                    
                    if ( lMaxSeqNum == -1 
                        && lMinSeqNum == -1 
                        && lStructureMissed == -1) // unknown error
					{					
                        utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"Unknown database error");
					}
                    else if ( lMaxSeqNum == -1 && lStructureMissed == -1 ) //trade already exists
                    {
					    dwRes = ERROR_ALREADY_EXISTS;
                        AddLogString(_T("ERROR: Trade with ExecID = %s already exists in database. Symbol: %s." ), 
							Trade.sExecID.c_str(), Trade.sSymbol.c_str() );
                        break;
                    }
                    
                    AddLogString( _T("Trade with ExecID = %s stored in database. Symbol: %s."), 
						Trade.sExecID.c_str(), Trade.sSymbol.c_str() );

                    m_dwStored++;

                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
                    sp_get << (int)lMaxSeqNum;
                    sp_get << (int)lMaxSeqNum;
                    sp_get << CDBNull();
                    sp_get << (unsigned char)1;
					
                    rs.Open(sp_get);
                    if(rs.IsEOF())
					{
						AddLogString(_T("ERROR: Failed to retrieve trade from database - trade with ExecID = %s doesn't exist. Symbol: %s."), 
							Trade.sExecID.c_str(), Trade.sSymbol.c_str() );

                        dwRes = ERROR_SEEK;
					}
					else
					{
						HRESULT hRes;
						if(FAILED(hRes = FillTradeUpdate(rs, Trade)))
						{
							AddLogString(_T("ERROR: Failed to fill TradeUpdate message."));
                            dwRes = ERROR_WRITE_FAULT;
						}
						else if(hRes == S_FALSE)
						{
						    AddLogString(_T("ERROR: Failed to retrieve trade from database - trade with ExecID = %s doesn't exist. Symbol: %s."), 
								Trade.sExecID.c_str(), Trade.sSymbol.c_str() );
                            dwRes = ERROR_SEEK;
						}
					}

					break;
				}

				case ETT_CANCEL:
				{
					CClientRecordset rs;
					CStoredProc<> sp(m_Connection, L"usp_TradeFeed_1_Del");
					
					sp << _bstr_t(Trade.sExecID.c_str());
					DATE dtTradeDate;
					OleDateFromTm(Trade.fdTradeDate.tm_year + 1900, 
								  Trade.fdTradeDate.tm_mon + 1,
								  Trade.fdTradeDate.tm_mday, 
								  0, 0, 0,
								  dtTradeDate);

					sp << (vt_date)dtTradeDate;

					rs.Open (sp);                    
                    if(rs.IsEOF())
                    {
                        rs.Close();
                        utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"EOF reached.");
                    }

                    lMaxSeqNum = rs[L"iMaxSeqNum"];
                    lMinSeqNum = rs[L"iMinSeqNum"];
                    rs.Close();
                    
                    if (lMaxSeqNum == -1 
                        && lMinSeqNum == -1) // unknown error
					{					
                        utils::ThrowErrorNoSetErrorInfo(E_FAIL, L"Unknown database error.");
					}
                    else if (lMinSeqNum == -1) // already exists
                    {                            
					    dwRes = ERROR_ALREADY_EXISTS;
                        AddLogString( _T("ERROR: Trade with ExecID = %s already canceled. Symbol: %s."), 
							Trade.sExecID.c_str(), Trade.sSymbol.c_str() );

                        break;
                    }
                    else if (lMaxSeqNum == -1)
                    {
                        dwRes = ERROR_SEEK;
                        AddLogString( _T("ERROR: Trade with ExecID = %s doesn't exist in database. Symbol: %s."), 
							Trade.sExecID.c_str(), Trade.sSymbol.c_str() );

                        break;
                    }

                    AddLogString( _T("Trade with ExecID = %s deleted from database. Symbol: %s"), 
						Trade.sExecID.c_str(), Trade.sSymbol.c_str() );
					
                    m_dwStored++;
                    
                    CStoredProc<> sp_get(m_Connection, L"usp_TradeSeq_Get");
                    sp_get << (int)lMaxSeqNum;
					sp_get << (int)lMaxSeqNum;
                    sp_get << CDBNull();
                    sp_get << (BYTE)1;

                    rs.Open(sp_get);					
                    if(rs.IsEOF())
					{
						AddLogString(_T("ERROR: Failed to retrieve cancel record with ExecID = %s from database - it doesn't exist. Symbol: %s."), 
							Trade.sExecID.c_str(), Trade.sSymbol.c_str() );

                        dwRes = ERROR_SEEK;
					}
					else
					{
                        HRESULT hRes;
					    if(FAILED(hRes = FillTradeUpdate(rs, Trade)))
                        {
   					        AddLogString(_T("ERROR: Failed to fill TradeUpdate message."));
                            dwRes = ERROR_WRITE_FAULT;
                        }
					    else if(hRes == S_FALSE)
                        {
   					        AddLogString(_T("ERROR: Failed to retrieve cancel record with ExecID = %s from database - it doesn't exist. Symbol: %s."), 
								Trade.sExecID.c_str(), Trade.sSymbol.c_str() );
                            dwRes = ERROR_SEEK;
                        }                        
                    }
					break;
				}
				default:
				{					
					dwRes = ERROR_INVALID_PARAMETER;
                    AddLogString( _T("ERROR: Invalid parameter.") );
				}
			}

			bOk = true;
		}
		catch(const _com_error& e)
		{
			_bstr_t bsError = L"ERROR: Unable to store trade in database";
			_bstr_t bsDesc = e.Description();			
			if (bsDesc.length())
			{			
				bsError += L": "; 
				bsError += bsDesc;
			}

			AddLogString((LPCTSTR)bsError);

            if(--dwTries)
			{				
                Reconnect();
			}
			else
			{
				AddLogString(_T("CRITICAL ERROR : FAILED TO STORE TRADE IN DATABASE"));
                dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
				bOk = true;
			}
		}

	} while(!bOk);	

    return dwRes;
}

bool  CTradesUploaderDlg::Reconnect()
{
	USES_CONVERSION;

    AddLogString( _T("Trying to reconnect...") );

	try
	{
		m_Connection.Close();
	}
	catch(_com_error&)
	{
	}

	Sleep(10);

	try
	{
		m_Connection.Open(m_bsConnectionString);
	}
	catch(_com_error&)
	{
		AddLogString( (char*)m_Connection.GetErrorMessage() );
	}

	AddLogString(_T("Reconnection done."));

	return true;
}

BOOL CTradesUploaderDlg::InitLogPath()
{
    USES_CONVERSION;   
    
    TCHAR szBuf[_MAX_PATH];    
	if(!::GetModuleFileName(NULL, szBuf, _MAX_PATH))
        return FALSE;

    TCHAR lpstrDrive[_MAX_DRIVE];
	TCHAR lpstrDir[_MAX_DIR];
	TCHAR lpstrFname[_MAX_FNAME];
	TCHAR lpstrExt[_MAX_EXT];
    
    _tsplitpath( szBuf, lpstrDrive, lpstrDir, lpstrFname, lpstrExt );

    TCHAR lpstrLogsPath[2048] = { 0 };	
    _stprintf( lpstrLogsPath, _T("%s%sLogs"), lpstrDrive, lpstrDir );
      
    if ( !SetCurrentDirectory(lpstrLogsPath) )	
	    return FALSE;    

    return TRUE;
}

UINT __stdcall CTradesUploaderDlg::_Thread(LPVOID pParam)
{
    CTradesUploaderDlg* pThis = reinterpret_cast<CTradesUploaderDlg*>(pParam);

    HANDLE hEvents[] = 
    {
        pThis->m_hAppStop,
        pThis->m_hTradesReady
    };
	
    ::CoInitializeEx( NULL, COINIT_MULTITHREADED );

    DWORD dwStatus;
    while ( (dwStatus = pThis->MsgWait(2,hEvents,INFINITE)) != WAIT_OBJECT_0 )
    {   
		CAutoLock Lock(pThis->m_Lock);        

        for ( int i = 0; i< pThis->m_TradesVec.size(); i++ )
        {
            if ( ::WaitForSingleObject( hEvents[0], 0) == WAIT_OBJECT_0 )
                break;

			const CTrade& Trade = pThis->m_TradesVec[i];
			
			if ( pThis->StoreInDatabase(Trade) == ERROR_SUCCESS)
				pThis->Publish(Trade);
        }

        pThis->AddLogString( _T("") );
        pThis->AddLogString( _T("--------") );
        pThis->AddLogString( _T("%lu trades stored in database."), pThis->m_dwStored );
        pThis->AddLogString( _T("%lu trades published."), pThis->m_dwPublished );

        pThis->Stop();
    }   

    CoUninitialize();    
    return 0;
}

DWORD CTradesUploaderDlg::MsgWait(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds, BOOL bMode)
{
	DWORD dwStatus = -1;
	ATLASSERT(nCount > 0);
	MSG msg;

	for(;;)
	{
		dwStatus = ::MsgWaitForMultipleObjects(nCount,pHandles,bMode,dwMilliseconds,QS_ALLEVENTS);
		
		if(dwStatus!=WAIT_OBJECT_0+nCount) break;
		while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE ))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return dwStatus;
}

HBRUSH CTradesUploaderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_LOGEDIT)
	{
		pDC->SetBkColor (GetSysColor(COLOR_WINDOW));
		hbr = GetSysColorBrush (COLOR_WINDOW);
	}
	
	return hbr;
}

void CTradesUploaderDlg::Publish(const CTrade& Trade)
{
	try
	{
		m_spPubManager->PubTradeUpdate(m_spTradeUpdate);
        m_dwPublished++;

		if (Trade.cExecTransType == ETT_NEW)
		{
			AddLogString(_T("Trade with ExecID = %s published. Symbol: %s"), 
				Trade.sExecID.c_str(), Trade.sSymbol.c_str() );
		}
		else if (Trade.cExecTransType == ETT_CANCEL)
		{
			AddLogString(_T("Cancel record with ExecID = %s published. Symbol: %s"), 
				Trade.sExecID.c_str(), Trade.sSymbol.c_str() );
		}
	}
	catch(const _com_error& e)
    {   
        AddLogString(_T("ERROR: Failed to publish trade with ExecID = %s. Symbol: %s"), 
			Trade.sExecID.c_str(), Trade.sSymbol.c_str() );
		
		_bstr_t bsDesc = e.Description();
		if (bsDesc.length())
			AddLogString((LPCTSTR)bsDesc);
	}
};

HRESULT CTradesUploaderDlg::FillTradeUpdate(CClientRecordset& rs, const CTrade& Trade)
{
	USES_CONVERSION;

	try
	{
		m_spTradeUpdate->SeqNum = rs[L"iSeqNum"];
		if(m_spTradeUpdate->SeqNum == 0)
			_com_issue_error(S_FALSE);

		m_spTradeUpdate->TradeID = rs[L"iTradeID"];
		m_spTradeUpdate->Action = (TRADE_ACTION)(int)rs[L"tiActionID"];
		m_spTradeUpdate->TradeStatus = (TRADE_STATUS)(long)rs[L"tiStatus"];
		m_spTradeUpdate->Mark = rs[L"iMark"];
		m_spTradeUpdate->TradeDate = rs[L"dtTradeDate"];
		m_spTradeUpdate->TraderID = rs[L"iTraderID"];
		m_spTradeUpdate->SymbolID = rs[L"iContractID"];
		m_spTradeUpdate->Quantity = rs[L"iQuantity"];
		m_spTradeUpdate->Price = rs[L"fPrice"];
		m_spTradeUpdate->IsBuy = rs[L"tiIsBuy"];
		m_spTradeUpdate->StrategyID = rs[L"iStrategyID"];
		m_spTradeUpdate->BrokerID = rs[L"iBrokerID"];
		m_spTradeUpdate->BrokerCommission = rs[L"fBrokerCommission"];
		m_spTradeUpdate->ClearingBrokerID = rs[L"iClearingBrokerID"];
		m_spTradeUpdate->ClearingBrokerCommission = rs[L"fClearingBrokerCommission"];
		m_spTradeUpdate->SpotReference = rs[L"fSpotReference"];
		m_spTradeUpdate->TradedIV = rs[L"fTradedIV"];

		int iContractTypeID = rs[L"iContractTypeID"];
		switch(iContractTypeID)
		{
			case 1: 
				m_spTradeUpdate->SymbolType = enStIndex; 
				break;

			case 2: 
				m_spTradeUpdate->SymbolType = enStStock; 
				break;
			
			case 3: 
				m_spTradeUpdate->SymbolType = enStOption; 
				break;

			case 4: 
				m_spTradeUpdate->SymbolType = enStFuture; 
				break;

			case 5: 
				m_spTradeUpdate->SymbolType = enStFutureOption; 
				break;

			default:
                {
                    char buf[100] = {0};
                    _itoa(iContractTypeID, buf, 10);                    
                    AddLogString( _T("ERROR: Unknown SymbolType for ContractTypeID = %s"), buf );
				    _com_issue_error(E_FAIL);
                }
		}

		m_spTradeUpdate->Symbol = rs[L"vcSymbol"];
		m_spTradeUpdate->UndSymbolID = rs[L"iUnderlyingContractID"];
		m_spTradeUpdate->UndSymbol = rs[L"vcUnderlyingSymbol"];
		m_spTradeUpdate->Expiry = rs[L"dtExpiry"];
		m_spTradeUpdate->IsCall = rs[L"tiIsCall"];
		m_spTradeUpdate->Strike = rs[L"fStrike"];
		m_spTradeUpdate->PriceClose = rs[L"fPriceClose"];
		m_spTradeUpdate->PriceTheoClose = rs[L"fPriceTheoClose"];
		m_spTradeUpdate->LotSize = rs[L"iLotSize"];
        m_spTradeUpdate->OptionRootID = rs[L"iOptionRootID"];
   //  not existed            m_spTradeUpdate->FutID =  rs[L"iFutureID"];
    //  not existed           m_spTradeUpdate->FutRootID =  rs[L"iFutureRootID"];

		if(Trade.cExecTransType == ETT_NEW)
			m_spTradeUpdate->MsState = (TRADE_MS_STATE)(long)rs[L"iStructureMissed"];
	}
	catch(_com_error& e)
	{
		return e.Error();
	}

	return S_OK;
}
