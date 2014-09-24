//{{AFX_INCLUDES()
#include "..\\EGOcx\\vsflexgrid.h"
#include "EGButton.h"
//}}AFX_INCLUDES
#if !defined(AFX_DUALQUADPARAMS_H__290A510B_4D47_425F_9E98_060CD5AFF7D5__INCLUDED_)
#define AFX_DUALQUADPARAMS_H__290A510B_4D47_425F_9E98_060CD5AFF7D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DualQuadParams.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDualQuadParamsDlg dialog

class CDualQuadParamsDlg : public CDialog
{
// Properties
	CDualQuadParVector m_vecDualQuadPar;
	std::vector<DATE>	m_months;
	std::map<DATE,double> m_ATMStrikes;

	long m_nMinW, m_nMinH;
	long m_nMargine;

	VME::IVMSymbolVolatilitySurfacePtr m_spSurface;

	bool m_bNeedSave;
public:
	bool m_bIsATMChanged;

// Construction
public:
	CDualQuadParamsDlg(CWnd* pParent , std::vector<DATE>& months, 
		std::map<DATE,double>& ATMStrikes, VME::IVMSymbolVolatilitySurface* pSurface);

// Dialog Data
	//{{AFX_DATA(CDualQuadParamsDlg)
	enum { IDD = IDD_DUALQUAD_PAR };
	CStatic	m_stcUpperLine;
	CEGButton	m_btnOK;
	CEGButton	m_btnCancel;
	CVSFlexGrid	m_fgDualQuad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDualQuadParamsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void GridInit();
	void GridFormat();
	void GridFill( bool bForceFocus = false );

	// Generated message map functions
	//{{AFX_MSG(CDualQuadParamsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnValidateEditDualquadpGrid(long Row, long Col, BOOL FAR* Cancel);
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Helpers
	void _AlignControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUALQUADPARAMS_H__290A510B_4D47_425F_9E98_060CD5AFF7D5__INCLUDED_)
