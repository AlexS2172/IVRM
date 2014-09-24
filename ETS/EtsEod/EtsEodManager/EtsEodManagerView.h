// EtsEodManagerView.h : interface of the CEtsEodManagerView class
//
#pragma once
#include "resource.h"
#include "GridLock.h"
#include "EtsEodManagerDoc.h"

/////////////////////////////////////////////////////////////////////////////
//
class CEtsEodManagerView : public CFormView
{
protected:
	CEtsEodManagerView();
	DECLARE_DYNCREATE(CEtsEodManagerView)

public:
	virtual ~CEtsEodManagerView();

	void AdjustLastColWidth();

	CEtsEodManagerDoc* GetDocument() const;
	enum DlgResEnum { IDD = IDD_EODMANAGERVIEW };

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
	afx_msg LRESULT OnEodNewRecord(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEodNewStepProgress(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewExpandAll();
	afx_msg void OnViewCollapseAll();
	afx_msg void OnViewShowColorCodes();
	afx_msg void OnUpdateViewShowColorCodes(CCmdUI *pCmdUI);
	virtual void OnInitialUpdate();

	DECLARE_EVENTSINK_MAP()
	void OnFgReportKeyPress(short* KeyAscii);

private:
	void _AdjustLayout();
	void _InitLogGrid();
	void _InitLogColumns();
	void _ShowReport();
	void _AddReportStep(long nStepRow, long nStep, const CEodReportStep& aStep);
	void _AddRecord(long nStepRow, long nRow, const CEodReportRec& aRec);
	void _AddNewRecord(long nStep, long nRecIdx);
	void _ShowStepProgress(long nStep);
	void _ShowRecordTypeImage(long nStepRow, long nRow, const CEodReportRec& aRec);
	void _ExpandCollapseAll(CollapsedSettings enCollapse);
	void _UpdateRecordColors();

protected:
	IVSFlexGridPtr		m_spFgRpt;

	CGridLock*			m_pFgRptLock;

	COLORREF			m_clrStepColor;
	COLORREF			m_clrDefRecColorBk;
	COLORREF			m_clrDefRecColorFg;

	int					m_nShowColorCodes;
};

#ifndef _DEBUG  // debug version in EtsEodManagerView.cpp
inline CEtsEodManagerDoc* CEtsEodManagerView::GetDocument() const
   { return reinterpret_cast<CEtsEodManagerDoc*>(m_pDocument); }
#endif

