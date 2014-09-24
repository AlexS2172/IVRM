// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__EBD15770_FDBF_4D87_B124_328AC305461C__INCLUDED_)
#define AFX_CHILDVIEW_H__EBD15770_FDBF_4D87_B124_328AC305461C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define COLUMN_TYPE _T("Type")
#define COLUMN_DATE _T("Date")
#define COLUMN_TIME _T("Time")
#define COLUMN_DESC _T("Description")

#define COLUMN_TYPE_ID 0
#define COLUMN_DATE_ID 1
#define COLUMN_TIME_ID 2
#define COLUMN_DESC_ID 3

#define COLUMN_TYPE_W 70
#define COLUMN_DATE_W 70
#define COLUMN_TIME_W 70
#define COLUMN_DESC_W 370

#define REG_SECTION_LOG _T("Log")

#define LOG_INFORMATION _T("Information")
#define LOG_WARNING		_T("Warning")
#define LOG_ERROR		_T("Error")


/////////////////////////////////////////////////////////////////////////////
// CChildView window
#include "tracer.h"

class CChildView : public CListCtrl
{
// Construction
public:
	CChildView();

protected:
BOOL WriteProfileInt(LPCTSTR lpszEntry, int nValue);
UINT GetProfileInt(LPCTSTR lpszEntry, int nDefault);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	CImageList m_ilImageList;

// Implementation
public:
	virtual ~CChildView();
	BOOL InsertTraceMessage(CTracer::MessageTypeEnum Type, LPCTSTR pszMessage);
	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__EBD15770_FDBF_4D87_B124_328AC305461C__INCLUDED_)
