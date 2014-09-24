#if !defined(AFX_TIMESKEWDLG_H__FED9C4FF_5F42_4B47_B57F_A7876549843F__INCLUDED_)
#define AFX_TIMESKEWDLG_H__FED9C4FF_5F42_4B47_B57F_A7876549843F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeSkewDlg.h : header file
//
#include "ChartDlg.h"
#ifdef _VOLA_MANAGER
#include "CustomVolaData.h"
#endif //_VOLA_MANAGER

/////////////////////////////////////////////////////////////////////////////
// CTimeSkewDlg dialog

class CTimeSkewDlg : public CChartDlg
{
// Construction
public:
	CTimeSkewDlg();   // standard constructor

	void InitializeChart();
	void UpdateChart( bool bReinitialize );
#ifdef _VOLA_MANAGER
	void UpdateCustomVolatility( UINT nMessage, CCustomVolaNotification *pNotification );
#endif //_VOLA_MANAGER
	
	bool UpdateCustomVolaPoint( long lMovingPtIdx, double dVola, double& dtMonth );
	void InterpolateCustomVolaToImplied();
	void RestoreCustomVola();
	void UpdateCustomVola();
	bool SetupVolaEditor( long lPtIdx, CVolaEditorDlg& ve );
	void SavePointValue( long lMovingPtIdx );
	double GetPointValue( long lMovingPtIdx );

// Dialog Data
	//{{AFX_DATA(CTimeSkewDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeSkewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strTitle;

	CString GetXLabel (long lSeries, size_t lPtIdx);
	
#ifdef _VOLA_MANAGER
	VME::IVMTimeSkewVolatilitiesPtr m_spSkew;

	CTimeSkewValues m_tsv; 
	CTimeSkewValues m_tsvPrev; 
	CCustomTimePoint m_ptPrev;

	void ConstructSkew( const time_skew_map & tsdata );
	bool UpdateSkewPoint( CCustomTimePoint& pt, double dVola );
	
	typedef vola_bands_map::const_iterator vola_bands_it;	
	
	bool GetStrikeInterpolationIVBands(double dStrikeMoneyness,vola_bands_it itFirst,
		vola_bands_it itLast,CVolaBandsData& Data);

#endif //_VOLA_MANAGER

protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeSkewDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMESKEWDLG_H__FED9C4FF_5F42_4B47_B57F_A7876549843F__INCLUDED_)
