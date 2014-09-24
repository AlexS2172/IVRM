//**************************************************************************************************************//
// solution:	OConnor
// project:		OConnor
// filename:	DlgAbout.h
// created:		27-Dec-2002 17:08 by Suchkov Dmitry
//
// purpose:		implementation of CDlgAbout
//
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "resource.h"

//**************************************************************************************************************//
// class CDlgAbout
//**************************************************************************************************************//
class CDlgAbout : public CDialog
{
public:
	CDlgAbout();

// Dialog Data
	//{{AFX_DATA(CDlgAbout)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_sLegalCopyRight;
	CString	m_sProductName;
	CString	m_sProductVersion;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAbout)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDlgAbout)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
