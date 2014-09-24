// TradesUploaderDlg.h : header file
//

#if !defined(AFX_TRADESUPLOADERDLG_H__BCA20458_6C2A_46CF_ACE3_89B2E90BAF74__INCLUDED_)
#define AFX_TRADESUPLOADERDLG_H__BCA20458_6C2A_46CF_ACE3_89B2E90BAF74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FAILES_FILENAME_BEGIN		_T("SFailes")
#define FAILES_FILENAME_END			_T(".log")

/////////////////////////////////////////////////////////////////////////////
// CTradesUploaderDlg dialog

class CTradesUploaderDlg : public CDialog
{
// Construction
public:

    CTradesUploaderDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CTradesUploaderDlg();	// standard destructor

// Dialog Data
	//{{AFX_DATA(CTradesUploaderDlg)
	enum { IDD = IDD_TRADESUPLOADER_DIALOG };
	CEdit	m_edConnString;
	CDateTimeCtrl	m_ctlDateTime;
	CEdit	m_edLogView;
	COleDateTime	m_dtFileDate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradesUploaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL   

// Implementation
protected:
	HICON m_hIcon;    

	// Generated message map functions
	//{{AFX_MSG(CTradesUploaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUpload();	
    afx_msg void OnCancel();	
    afx_msg void OnDestroy( );
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    
    ISettingsPtr	m_spEtsSettings;
    IPubManagerPtr	m_spPubManager;
    ITradeUpdatePtr	m_spTradeUpdate;

    CDBConnection	m_Connection;

    _bstr_t			m_bsConnectionString;
	_bstr_t			m_bsConnStringLabel;

    HANDLE			m_hFile;
    HANDLE			m_hThread;
    HANDLE			m_hAppStop;
    HANDLE			m_hStopped;
    HANDLE			m_hTradesReady;
    TTradesVector	m_TradesVec;
	CTrade			m_Trade;    
	TCHAR			m_lpstrLogName[256];
	DWORD			m_dwStored;
    DWORD			m_dwPublished;    
	CLock			m_Lock;    
	BOOL			m_bConnected;

	HWND			m_hLogEdit;
	HWND			m_hBtnUpload;
	HWND			m_hBtnCancel;
	int				m_nPadWidth;
	int				m_nButtonRange;
    
    
    HRESULT	Start();
    void	Stop();

    HRESULT GetConnectionString();

	BOOL	InitLogPath();
    BOOL	CreateFileName();
    void	AddLogString(LPCTSTR lpszFormatStr, ... );
	
	bool	ConvertDateTime(double dIn, CFixTimeValue& tValue);
	bool	ConvertDateTime(double dIn, CFixDateValue& tValue);
	bool	ConvertDateTime(double dIn, CFixMonthYearValue& tValue, CFixDayOfMonthValue& dValue);
    
	DWORD	ParseRecord( const string sMsgBuf, const DWORD dwOffset, DWORD *pdwRead, const DWORD dwBufSize);    
    DWORD	StoreInDatabase(const CTrade& Trade);
    void	Publish(const CTrade& Trade);
    HRESULT	FillTradeUpdate(CClientRecordset& rs, const CTrade& Trade);
    bool	Reconnect();    
    DWORD	MsgWait(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds, BOOL bMode=FALSE);
    static UINT __stdcall	_Thread(LPVOID pParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADESUPLOADERDLG_H__BCA20458_6C2A_46CF_ACE3_89B2E90BAF74__INCLUDED_)
