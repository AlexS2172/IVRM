#if !defined(AFX_SYMBOLCOMBO_H__CB7EDA13_3887_45E8_8C83_AE51A7A6A782__INCLUDED_)
#define AFX_SYMBOLCOMBO_H__CB7EDA13_3887_45E8_8C83_AE51A7A6A782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymbolCombo.h : header file
//
#include "FlatComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSymbolCombo window

class CSymbolCombo : public CFlatComboBox
{
// Construction
public:
	CSymbolCombo();

// Attributes
public:
private:
	bool m_bClosing;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSymbolCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbolCombo)
	afx_msg void OnSelendokSymbols();
	afx_msg void OnSelendcancelSymbols();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLCOMBO_H__CB7EDA13_3887_45E8_8C83_AE51A7A6A782__INCLUDED_)
