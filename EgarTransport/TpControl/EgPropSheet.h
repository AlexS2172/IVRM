#if !defined(AFX_EGPROPSHEET_H__D5548EFA_C142_436E_BBB4_93D92A93820B__INCLUDED_)
#define AFX_EGPROPSHEET_H__D5548EFA_C142_436E_BBB4_93D92A93820B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EgPropSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEgPropSheet

class CEgPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CEgPropSheet)

// Construction
public:
	CEgPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CEgPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEgPropSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEgPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEgPropSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EGPROPSHEET_H__D5548EFA_C142_436E_BBB4_93D92A93820B__INCLUDED_)
