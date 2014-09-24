// ProvidersTestDlg.h : header file
//

#pragma once
#include "afxwin.h"

class CProvidersTestDlg;
typedef IDispEventSimpleImpl<1, CProvidersTestDlg, &__uuidof(_IMyTrackStructureInfoEvents)> _StructureInfoEvents;
typedef IDispEventSimpleImpl<2, CProvidersTestDlg, &__uuidof(_IPriceInfoEvents)> _PriceInfoEvents;

// CProvidersTestDlg dialog
class CProvidersTestDlg : 
    public CDialog,
    public _StructureInfoEvents,
    public _PriceInfoEvents
{
// Construction
public:
	CProvidersTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PROVIDERSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedGetoptions();
    afx_msg void OnDestroy();

    BEGIN_SINK_MAP(CProvidersTestDlg)	
        SINK_ENTRY_INFO(1, __uuidof(_IMyTrackStructureInfoEvents), 0x1, OnStock, &m_OnStockInfo)
        SINK_ENTRY_INFO(1, __uuidof(_IMyTrackStructureInfoEvents), 0x2, OnOption, &m_OnOptionInfo)       
        SINK_ENTRY_INFO(1, __uuidof(_IMyTrackStructureInfoEvents), 0x3, OnError, &m_OnErrorInfo)    
        SINK_ENTRY_INFO(2, __uuidof(_IPriceInfoEvents), 0x1, OnLastQuote, &m_OnLastQuoteInfo)
        SINK_ENTRY_INFO(2, __uuidof(_IPriceInfoEvents), 0x2, OnQuoteUpdate, &m_OnQuoteUpdateInfo)       
        SINK_ENTRY_INFO(2, __uuidof(_IPriceInfoEvents), 0x3, OnError, &m_OnErrorInfo)    
    END_SINK_MAP()
protected:
    IStructureProviderPtr   m_spStructureProvider;
    IPriceProviderPtr       m_spPriceProvider;

    static _ATL_FUNC_INFO     m_OnStockInfo;
    static _ATL_FUNC_INFO     m_OnOptionInfo;
    static _ATL_FUNC_INFO     m_OnErrorInfo;
    static _ATL_FUNC_INFO     m_OnLastQuoteInfo;
    static _ATL_FUNC_INFO     m_OnQuoteUpdateInfo;

    STDMETHOD(OnStock)(
        struct StockParams * Params,
        struct StockInfo * Results );
    STDMETHOD(OnOption)(
        struct OptionParams * Params,
        struct OptionInfo * Results,
        VARIANT_BOOL IsLastOption );
    STDMETHOD(OnError)(
        ErrorNumberEnum ErrorNumber,
        BSTR Description,
        RequestsTypeEnum ReqType,
        VARIANT Request );
    STDMETHOD(OnLastQuote)(
        struct QuoteUpdateParams * Params,
        struct QuoteUpdateInfo * Results );
    STDMETHOD(OnQuoteUpdate)(
        struct QuoteUpdateParams * Params,
        struct QuoteUpdateInfo * Results );

    CEdit m_edResults;
protected:
    CString m_sExchange;
public:
    afx_msg void OnBnClickedLastQuote();
	afx_msg void OnBnClickedGetstock();
	afx_msg void OnBnClickedSubquote();
};
