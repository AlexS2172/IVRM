#if !defined(AFX_IVCONNECTION_H__FE9CEEF5_D9FD_4947_9071_AD29A7FC4E0F__INCLUDED_)
#define AFX_IVCONNECTION_H__FE9CEEF5_D9FD_4947_9071_AD29A7FC4E0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IVConnection.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CIVConnection - provides access to the IVolatility.com services
// This class is not synchronized and must be used from the single thread only
class CIVConnection :  protected CWinThread
{

public:
	CIVConnection ();
	virtual ~CIVConnection();
	BOOL Init ()  
		{ return CreateThread();}

	
	void Term() 
	{	
		if  (m_hThread)
		{
			PostThreadMessage (WM_QUIT, 0, 0); 
			WaitForSingleObject (m_hThread, INFINITE);
		}
	};

	void SetParams (const CIVOptions & opts);

	void StopRequests();
	bool GetHIVola (LPCTSTR szSymbol, const dte_vec & dtes, hist_ranges_map & ranges);
	bool GetYield (LPCTSTR szIndex, double & rdYield );

	bool IsStopped ()
		{  return m_bStopped; }
	bool IsError ()
		{return m_bError;}
	CString GetErrorMessage ()
		{	return m_strError;}
		

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVConnection)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool RequestData();
	void QueryStatusText (HINTERNET hFile, CString & rstr);
	DWORD QueryInfoStatusCode (HINTERNET hFile);
	void SetReady(bool bReady = true);
	void InitRequest();
	void SetError (LPCTSTR szError);
	void SetError(UINT nID);

	bool HttpError(HINTERNET hFile, DWORD dwStatus);
	void CloseSession();
	void GetHTTPData(LPCTSTR szQuery, CString & rstr);
	CString Login();
	void InitSession();
	bool CheckErrorMessage(LPCTSTR szSource);
	
	CString GetCommonIVError (LPCTSTR szError);

	IC::IMD5Ptr m_spIMD5;

	bool m_bStopped;
	bool m_bError;
	CString m_strError;

	::CEvent m_evtBusy;
	::CEvent m_evtReady;

	CCriticalSection m_csSession;


	CIVOptions m_opts;

	CString m_strRequest;
	CString m_strResult;
	CString m_strHeader;

	
	HINTERNET	m_hSession;

	// Generated message map functions
	//{{AFX_MSG(CIVConnection)
	afx_msg void OnPostRequest	(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVCONNECTION_H__FE9CEEF5_D9FD_4947_9071_AD29A7FC4E0F__INCLUDED_)
