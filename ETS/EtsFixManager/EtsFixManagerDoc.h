// EtsFixManagerDoc.h : interface of the CEtsFixManagerDoc class
//
#pragma once
#include "FixReportRec.h"
#pragma warning(push)
#pragma warning(disable: 4290)

#define WM_FIX_NEW_RECORD		(WM_USER + 1)

class CEtsFixManagerDoc : public CDocument
{
protected: // create from serialization only
	CEtsFixManagerDoc();
	DECLARE_DYNCREATE(CEtsFixManagerDoc)

public:

	const CFixReportRec& GetRecord(long nIdx) throw(HRESULT);
	void AddNewRecord(DATE dtTime, FgLogMessageTypeEnum enType, BSTR bsMessage);
	long GetRecordsCount();
	void CopyLogToClipboard();

// Overrides
	public:
	virtual BOOL OnNewDocument();

// Implementation
public:
	virtual ~CEtsFixManagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

protected:	
	CFixReportRecVec			m_vecRecord;
	CLock						m_csLock;
};
#pragma warning(pop)


