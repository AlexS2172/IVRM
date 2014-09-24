// ProvidersTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProvidersTest.h"
#include "ProvidersTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CProvidersTestDlg dialog
_ATL_FUNC_INFO CProvidersTestDlg::m_OnStockInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };
_ATL_FUNC_INFO CProvidersTestDlg::m_OnOptionInfo = {CC_STDCALL, VT_EMPTY, 3, {VT_RECORD,VT_RECORD,VT_BOOL} };
_ATL_FUNC_INFO CProvidersTestDlg::m_OnErrorInfo = {CC_STDCALL, VT_EMPTY, 4, {VT_I4,VT_BSTR,VT_I4,VT_VARIANT} };
_ATL_FUNC_INFO CProvidersTestDlg::m_OnLastQuoteInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };
_ATL_FUNC_INFO CProvidersTestDlg::m_OnQuoteUpdateInfo = {CC_STDCALL, VT_EMPTY, 2, {VT_RECORD,VT_RECORD} };

CProvidersTestDlg::CProvidersTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProvidersTestDlg::IDD, pParent)
    , m_sExchange(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProvidersTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RESULTS, m_edResults);
    DDX_Text(pDX, IDC_EXCHANGE, m_sExchange);
}

BEGIN_MESSAGE_MAP(CProvidersTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_GETOPTIONS, OnBnClickedGetoptions)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_LASTQUOTE, OnBnClickedLastQuote)
	ON_BN_CLICKED(IDC_GETSTOCK, OnBnClickedGetstock)
	ON_BN_CLICKED(IDC_SUBQUOTE, OnBnClickedSubquote)
END_MESSAGE_MAP()


// CProvidersTestDlg message handlers

BOOL CProvidersTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    HRESULT hr;
    hr = m_spStructureProvider.CreateInstance( __uuidof(StructureInfo));
    if (FAILED(hr))
        return FALSE;

    hr = _StructureInfoEvents::DispEventAdvise(m_spStructureProvider);
    if (FAILED(hr))
        return FALSE;

    try
    {
        IProviderPtr spProvider = m_spStructureProvider;
        spProvider->Type = 3;
        m_spStructureProvider->Connect();
    }
    catch (_com_error& e)
    {
    }

    hr = m_spPriceProvider.CreateInstance( __uuidof(PriceInfo));
    if (FAILED(hr))
        return FALSE;

    hr = _PriceInfoEvents::DispEventAdvise(m_spPriceProvider);
    if (FAILED(hr))
        return FALSE;

    try
    {
        IProviderPtr spProvider = m_spPriceProvider;
        spProvider->Type = 3;
        m_spPriceProvider->Connect();
    }
    catch (_com_error& e)
    {
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProvidersTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProvidersTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProvidersTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProvidersTestDlg::OnBnClickedGetoptions()
{
    if (m_spStructureProvider)
    {
        UpdateData();
        _bstr_t bsExchange = (LPCTSTR)m_sExchange;
        _bstr_t bsUnderlyingSymbol = L"AAPL";

        OptionParams params;
        params.Exchange = bsExchange;
        params.UnderlyingSymbol = bsUnderlyingSymbol;

        try
        {
            m_edResults.SetSel(0,-1);
            m_edResults.Clear();
            m_spStructureProvider->RequestOptions(&params);
        }
        catch (_com_error& e)
        {
        }
    }
}

void CProvidersTestDlg::OnDestroy()
{
    if (m_spStructureProvider)
    {
        m_spStructureProvider->Disconnect();
        _StructureInfoEvents::DispEventUnadvise(m_spStructureProvider);
        m_spStructureProvider = 0;
    }
    if (m_spPriceProvider)
    {
        m_spPriceProvider->Disconnect();
        _PriceInfoEvents::DispEventUnadvise(m_spPriceProvider);
        m_spPriceProvider = 0;
    }

    CDialog::OnClose();
}

STDMETHODIMP CProvidersTestDlg::OnStock(
    struct StockParams * Params,
    struct StockInfo * Results )
{
	_bstr_t sEOL = L"\r\n";
	_bstr_t sTab = L"\t";
	_bstr_t sLine;
	sLine = Params->Stock + sTab + L"<" + Results->Exchange + L">" + sTab;
	sLine += Results->Description + sEOL;

	m_edResults.SetSel(-2, -1);
	m_edResults.ReplaceSel(sLine);

    return S_OK;
};

STDMETHODIMP CProvidersTestDlg::OnOption(
    struct OptionParams * Params,
    struct OptionInfo * Results,
    VARIANT_BOOL IsLastOption )
{
    _bstr_t sEOL = L"\r\n";
    _bstr_t sTab = L"\t";
    _bstr_t sLine;
    sLine = Results->Series + sTab + L"<" + Results->Exchange + L">" + sTab;
    sLine += _bstr_t(_variant_t(Results->StrikePrice)) + sTab + _bstr_t(_variant_t(Results->Last)) + sEOL;

    m_edResults.SetSel(-2, -1);
    m_edResults.ReplaceSel(sLine);

    return S_OK;
};

STDMETHODIMP CProvidersTestDlg::OnError(
    ErrorNumberEnum ErrorNumber,
    BSTR Description,
    RequestsTypeEnum ReqType,
    VARIANT Request )
{
    m_edResults.SetSel(-2, -1);
    m_edResults.ReplaceSel( L"Error occured: " + _bstr_t(Description) + L"\t Type:" + _bstr_t(_variant_t((long)ReqType)) + L"\r\n");

    return S_OK;
};

STDMETHODIMP CProvidersTestDlg::OnLastQuote(
    struct QuoteUpdateParams * Params,
    struct QuoteUpdateInfo * Results )
{
    _bstr_t sEOL = L"\r\n";
    _bstr_t sTab = L"\t";
    _bstr_t sLine;
    sLine = Params->Symbol + sTab + L"<" + Results->Exchange + L">" + sTab + _bstr_t(_variant_t(Results->LastPrice)) + sEOL;

    m_edResults.SetSel(-2, -1);
    m_edResults.ReplaceSel(sLine);

    return S_OK;
};

STDMETHODIMP CProvidersTestDlg::OnQuoteUpdate(
    struct QuoteUpdateParams * Params,
    struct QuoteUpdateInfo * Results )
{
	_bstr_t sEOL = L"\r\n";
	_bstr_t sTab = L"\t";
	_bstr_t sLine;
	sLine = Params->Symbol + sTab + L"<" + Results->Exchange + L">" + sTab + _bstr_t(_variant_t(Results->LastPrice)) + sEOL;

	m_edResults.SetSel(-2, -1);
	m_edResults.ReplaceSel(sLine);

	return S_OK;
};


void CProvidersTestDlg::OnBnClickedLastQuote()
{
    if (m_spPriceProvider)
    {
        UpdateData();
        _bstr_t bsExchange = (LPCTSTR)m_sExchange;
        _bstr_t bsSymbol = L"INVN";

        QuoteUpdateParams params;
        params.Exchange = bsExchange;
        params.Symbol = bsSymbol;
        params.Type = enSTK;

        try
        {
            m_edResults.SetSel(0,-1);
            m_edResults.Clear();
            IGroupPricePtr spGroup = m_spPriceProvider;
//            spGroup->RequestGroup(&params, enGroupRequestLastQuote);
            m_spPriceProvider->RequestLastQuote(&params);
        }
        catch (_com_error& e)
        {
        }
    }
}

void CProvidersTestDlg::OnBnClickedGetstock()
{
	if (m_spStructureProvider)
	{
		UpdateData();
		_bstr_t bsExchange = (LPCTSTR)m_sExchange;
		_bstr_t bsUnderlyingSymbol = L"AAPLXX";

		StockParams params;
		params.Exchange = bsExchange;
		params.Stock = bsUnderlyingSymbol;

		try
		{
			m_edResults.SetSel(0,-1);
			m_edResults.Clear();
			m_spStructureProvider->RequestStock(&params);
		}
		catch (_com_error& e)
		{
		}
	}
}

void CProvidersTestDlg::OnBnClickedSubquote()
{
	if (m_spPriceProvider)
	{
		UpdateData();
		_bstr_t bsExchange = (LPCTSTR)m_sExchange;
		_bstr_t bsSymbol = L"INVN";

		QuoteUpdateParams params;
		params.Exchange = bsExchange;
		params.Symbol = bsSymbol;
		params.Type = enSTK;

		try
		{
			m_edResults.SetSel(0,-1);
			m_edResults.Clear();
			m_spPriceProvider->SubscribeQuote(&params);
		}
		catch (_com_error& e)
		{
		}
	}
}

