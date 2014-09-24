#if !defined(AFX_MKTSTRUCTUREBASEDLG_H__59DBD945_C982_408A_AC94_8FB5E6D96136__INCLUDED_)
#define AFX_MKTSTRUCTUREBASEDLG_H__59DBD945_C982_408A_AC94_8FB5E6D96136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MktStructureBaseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMktStructureBaseDlg dialog


#include "IVConnection.h"

struct CContractRequestData
{
	int  m_nID;
	bool m_bGotOptions;
	long m_lGotOptions;
	bool m_bGotData;
	bool m_bGotYield;
	bool m_bError;
	CContractRequestData (int nID  = 0): 
		m_lGotOptions(0), m_nID (nID), m_bGotOptions(false),	m_bGotData (false), 
		m_bGotYield (true), m_bError (false) {}
};

typedef std::map <_bstr_t, CContractRequestData > stocks_map;


class CMktStructureBaseDlg;

typedef IDispEventImpl <1, CMktStructureBaseDlg,  & __uuidof (PP::_IMyTrackStructureInfoEvents)> _IMktStructureEventsImpl;


class CMktStructureBaseDlg : 
	public CDialog,
	public _IMktStructureEventsImpl
{
// Construction
public:
	
	CMktStructureBaseDlg(UINT nID, CWnd* pParent = NULL);   // standard constructor

BEGIN_SINK_MAP (CMktStructureBaseDlg)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IMyTrackStructureInfoEvents), 1, OnStock, &m_OnStockInfo)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IMyTrackStructureInfoEvents), 2, OnOption, &m_OnOptionInfo)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IMyTrackStructureInfoEvents), 3, OnError, &m_OnErrorInfo)
END_SINK_MAP()


	bool IsFeedStopped(){ return m_bFeedStopped;};

// Dialog Data
	//{{AFX_DATA(CMktStructureBaseDlg)
	enum { IDD = IDD_MKT_STRUCTURE };
	CProgressCtrl	m_pgContract;
	CEdit	m_edLog;
	CButton	m_btCancel;
	CStatic	m_stInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMktStructureBaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RequestYield();
	virtual bool HandleStock (PP::StockParams *pParams, PP::StockInfo *pResults)  {return false;}
	virtual void OnStop() {}
	
	STDMETHOD (OnStock)(PP::StockParams *Params, PP::StockInfo *Results);
	STDMETHOD (OnOption) (PP::OptionParams *Params, PP::OptionInfo *Results, VARIANT_BOOL bLast);
	STDMETHOD (OnError) (PP::ErrorNumberEnum ErrorNumber, BSTR Description, PP::RequestsTypeEnum ReqType, VARIANT Request);



	CString GetErrorMessage(HRESULT hr, BSTR bstrDesc);
	bool StartDownload(bool bDoSubscribe = true);
	void SubscribeNextStock();
	bool Stop();
	void UpdateProgress(stocks_map::iterator it);
	void StatusMessage(LPCTSTR szMessage, bool bLog = true);
	void StatusMessage(UINT  iMessage, bool bLog = true);

	void ErrorMessage (const _com_error & e);

	static _ATL_FUNC_INFO m_OnStockInfo;
	static _ATL_FUNC_INFO m_OnOptionInfo;
	static _ATL_FUNC_INFO m_OnErrorInfo;

	static _ATL_FUNC_INFO m_OnSOAPErrorInfo;
	static _ATL_FUNC_INFO m_OnSOAPResultInfo;
	static _ATL_FUNC_INFO m_OnIVAuthErrorInfo;
	static _ATL_FUNC_INFO m_OnIVAuthResultInfo;


	PP::IStructureProviderPtr m_spStructureInfo;

	CIVConnection m_ivc;
	
	stocks_map m_stocks;
	stocks_map::iterator m_itCur;
	bool m_bConnected;
	bool m_bStarted;
	bool m_bCanceled;
	int m_nCounter;
	static const int m_nPosPerContract;
	CIVAnalyzerOptions m_opts;

	void DoEvents();

	bool    m_bFeedStopped;

	// Generated message map functions
	//{{AFX_MSG(CMktStructureBaseDlg)
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnRequestYield (WPARAM wParam, LPARAM lParam);


	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MKTSTRUCTUREBASEDLG_H__59DBD945_C982_408A_AC94_8FB5E6D96136__INCLUDED_)
