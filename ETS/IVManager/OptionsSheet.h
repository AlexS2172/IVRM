#if !defined(AFX_OPTIONSSHEET_H__8C0329EF_F706_41DE_A093_D0AFF6092E1C__INCLUDED_)
#define AFX_OPTIONSSHEET_H__8C0329EF_F706_41DE_A093_D0AFF6092E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsSheet.h : header file
//

#include "OptionsConnectionPage.h"
#include "OptionsSkewPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsSheet

class COptionsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionsSheet)

// Construction
public:
	COptionsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionsSheet();

protected:
	CIVAnalyzerOptions m_opts;

	COptionsConnectionPage m_ConnectionPage;
	COptionsSkewPage	   m_SkewPage;

	void AddPages();

	// Generated message map functions
	//{{AFX_MSG(COptionsSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSSHEET_H__8C0329EF_F706_41DE_A093_D0AFF6092E1C__INCLUDED_)
