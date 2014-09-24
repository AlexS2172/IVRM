#if !defined(AFX_ROUTERSETTINGS_H__D3C88BCB_65F3_4953_B4DF_6219468FE068__INCLUDED_)
#define AFX_ROUTERSETTINGS_H__D3C88BCB_65F3_4953_B4DF_6219468FE068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RouterSettings.h : header file
//
#include "resource.h"
typedef map < DWORD,int > IP_INDEX_MAP;

/////////////////////////////////////////////////////////////////////////////
// CRouterSettings dialog

class CRouterSettings : public CPropertyPage
{
	DECLARE_DYNCREATE(CRouterSettings)

// Construction
public:
	CRouterSettings();
	~CRouterSettings();

	void ResetSelection();

	DWORD m_dwAddress;
// Dialog Data
	//{{AFX_DATA(CRouterSettings)
	enum { IDD = IDD_ROUTER_SETTINGS };
	CComboBox	m_cmbLocRouterAddr;
	DWORD	m_dwCheckHostTime;
	DWORD	m_dwConnTimeout;
	DWORD	m_dwLocRouterPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRouterSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRouterSettings)
	afx_msg void OnChange();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	IP_INDEX_MAP	m_mapIPIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUTERSETTINGS_H__D3C88BCB_65F3_4953_B4DF_6219468FE068__INCLUDED_)
