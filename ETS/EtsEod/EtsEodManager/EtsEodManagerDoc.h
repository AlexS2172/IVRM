// EtsEodManagerDoc.h : interface of the CEtsEodManagerDoc class
//
#pragma once

#include "EodReportRec.h"
#include "FileMonitor.h"

#define WM_EOD_NEW_RECORD		(WM_USER + 1)
#define WM_EOD_STEP_PROGRESS	(WM_USER + 2)

/////////////////////////////////////////////////////////////////////////////
//
class CEtsEodManagerDoc : public CDocument, public CFileMonitor
{
protected: // create from serialization only
	CEtsEodManagerDoc();
	DECLARE_DYNCREATE(CEtsEodManagerDoc)

public:
	const CEodReportStepMap& GetSteps() const
	{
		return m_mapSteps;
	}

	CLock& GetLock()
	{
		return m_csLock;
	}

public:
	virtual void OnAppend(LPCSTR lpBuf, long nSize);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	BOOL DoSaveAs();

public:
	afx_msg void OnFileSave();

// Implementation
public:
	virtual ~CEtsEodManagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
	DWORD _ReadReport();
	DWORD _ReadLine(const CString& strBuf, const long nOffset, long* pnRead, bool bNotify);
	BOOL _IsCurrent(LPCTSTR lpszPathName);
	void _InitStepsMap();

protected:
	CEodReportStepMap			m_mapSteps;
	CString						m_strScrapsBuf;
	CLock						m_csLock;

public:
	BOOL						m_bIsCurrent;
};


