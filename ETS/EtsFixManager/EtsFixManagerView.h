// EtsFixManagerView.h : interface of the CEtsFixManagerView class
//
#pragma once
#include "resource.h"
#include "GridLock.h"
#include "EtsFixManagerDoc.h"

/////////////////////////////////////////////////////////////////////////////
//
class CEtsFixManagerView : public CFormView
{
protected: // create from serialization only
	CEtsFixManagerView();
	DECLARE_DYNCREATE(CEtsFixManagerView)

// Attributes
public:
	virtual ~CEtsFixManagerView();

	void AdjustLastColWidth();

	CEtsFixManagerDoc* GetDocument() const;
	enum DlgResEnum { IDD = IDD_FIXMANAGERVIEW };

// Operations
public:

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnFixNewRecord(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewShowColorCodes();
	afx_msg void OnUpdateViewShowColorCodes(CCmdUI *pCmdUI);
	virtual void OnInitialUpdate();

	DECLARE_EVENTSINK_MAP()

private:
	void _AdjustLayout();
	void _InitLogGrid();
	void _InitLogColumns();
	void _AddNewRecord(long nRecIdx);
	void _ShowRecordTypeImage(long nRow, const CFixReportRec& aRec);
	void _UpdateRecordColors();

protected:
	IVSFlexGridPtr		m_spFgRpt;

	CGridLock*			m_pFgRptLock;

	COLORREF			m_clrDefRecColorBk;
	COLORREF			m_clrDefRecColorFg;

	int					m_nShowColorCodes;
};

#ifndef _DEBUG  // debug version in EtsFixManagerView.cpp
inline CEtsFixManagerDoc* CEtsFixManagerView::GetDocument() const
   { return reinterpret_cast<CEtsFixManagerDoc*>(m_pDocument); }
#endif

