#if !defined(AFX_HOSTSTATISTICS_H__6931F250_9579_42D3_AC70_5184DA39D01B__INCLUDED_)
#define AFX_HOSTSTATISTICS_H__6931F250_9579_42D3_AC70_5184DA39D01B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HostStatistics.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHostStatistics dialog

class CHostStatistics : public CDialog
{
// Construction
public:
	CHostStatistics(BSTR bsHostIP, CWnd* pParent = NULL);   // standard constructor
	

// Dialog Data
	//{{AFX_DATA(CHostStatistics)
	enum { IDD = IDD_HOSTSTATISTIC };
	CString	m_sConfirmedMsg;
	CString	m_sReceivedMsg;
	CString	m_sSentMsg;
	CString	m_sStartLogonTime;
	CString	m_sUnsentMsg;
	CString	m_sHostIP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostStatistics)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Update();

	// Generated message map functions
	//{{AFX_MSG(CHostStatistics)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	UINT m_nTimer;
private:
	void					StartTimer()
	{
		m_nTimer = SetTimer(1, 5000, NULL);
	}
	void                    StopTimer()
	{
		if(m_nTimer)
		{
			KillTimer(m_nTimer);
			m_nTimer = 0;
		}
	}

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOSTSTATISTICS_H__6931F250_9579_42D3_AC70_5184DA39D01B__INCLUDED_)
