//**************************************************************************************************************//
// solution:	Pequote
// project:		Pequote
// filename:	ViewLog.h
// created:		26-Dec-2002 15:23 by Suchkov Dmitry
//
// purpose:		interface of CViewLog
//
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// defines
//**************************************************************************************************************//
#define COLUMN_TYPE _T("Type")
#define COLUMN_DATE _T("Date")
#define COLUMN_TIME _T("Time")
#define COLUMN_DESC _T("Description")
#define COLUMN_SOURCE  _T("Source")

#define COLUMN_TYPE_ID 0
#define COLUMN_DATE_ID 1
#define COLUMN_TIME_ID 2
#define COLUMN_DESC_ID 3
#define COLUMN_SOURCE_ID 4

#define COLUMN_TYPE_W 70
#define COLUMN_DATE_W 70
#define COLUMN_TIME_W 70
#define COLUMN_DESC_W 370
#define COLUMN_SOURCE_W 370

#define REG_SECTION_LOG _T("Log")

#define LOG_INFORMATION _T("Information")
#define LOG_WARNING		_T("Warning")
#define LOG_ERROR		_T("Error")

//**************************************************************************************************************//
// class CViewLog
//**************************************************************************************************************//

class CViewLog : public CListCtrl
{
private:

// Data members

	CImageList m_ilType;

public:

// Constructors / Destructors
	CViewLog();
	virtual ~CViewLog();

// Overrides
	//{{AFX_VIRTUAL(CViewLog)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:

// Operations

	BOOL InsertTraceMessage(CTracer::MessageTypeEnum Type, sMessage* pMessage);

protected:

// Implementation

	//{{AFX_MSG(CViewLog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL WriteProfileInt(LPCTSTR lpszEntry, int nValue);
	UINT GetProfileInt(LPCTSTR lpszEntry, int nDefault);
};

//{{AFX_INSERT_LOCATION}}
