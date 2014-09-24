// StrikeSkewDlg.h: interface for the CStrikeSkewDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRIKESKEWDLG_H__AAC3224F_5A13_4638_B051_F6424697B905__INCLUDED_)
#define AFX_STRIKESKEWDLG_H__AAC3224F_5A13_4638_B051_F6424697B905__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartDlg.h"
#ifdef _VOLA_MANAGER
#include "CustomVolaData.h"
#endif //_VOLA_MANAGER

typedef  std::vector <bool> types_vec; 

class CStrikeSkewDlg : public CChartDlg  
{
public:
	void InitializeChart();
	void UpdateChart( bool bReinitialize );
#ifdef _VOLA_MANAGER
	void UpdateCustomVolatility( UINT nMessage, CCustomVolaNotification *pNotification );
#endif //_VOLA_MANAGER

	// Vola management overrides
	bool UpdateCustomVolaPoint( long lMovingPtIdx, double dVola, double& dtMonth );
	void InterpolateCustomVolaToImplied();
	void RestoreCustomVola();
	void UpdateCustomVola();
	bool SetupVolaEditor( long lPtIdx, CVolaEditorDlg& ve );
	void SavePointValue( long lMovingPtIdx );
	double GetPointValue( long lMovingPtIdx );

	CStrikeSkewDlg( DATE dtMonth );

// Implementation
protected:
	CString				m_strTitle;
	COleDateTime		m_dtMonth;
	COleDateTime		m_dtExp;
	types_vec			m_vecTypes;

#ifdef _VOLA_MANAGER
	VME::IVMStrikeSkewVolatilitiesPtr m_spSkew;

	long m_nUpdateMark;
	CStrikeSkewValues  m_ssv; 
	CStrikeSkewValues  m_ssvAll; 	
	CStrikeSkewValues  m_ssvPrev; 
	CCustomStrikePoint m_ptPrev;	// Filled when starting drag editing

	void   ConstructSkew( bool bATMPointOnly );
	bool   UpdateSkewPoint( const CCustomStrikePoint& pt );
	void   UpdateSkew();
	void   _ExtrapolateMonthVolatility( bool bIsFlatWings = false, double dStrikeLow = 0.0f, double dStrikeHi = 0.0f);
	
#endif //_VOLA_MANAGER

	CString GetXLabel (long lSeries, long lPtIdx);

protected:

	// Generated message map functions
	//{{AFX_MSG(CStrikeSkewDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_STRIKESKEWDLG_H__AAC3224F_5A13_4638_B051_F6424697B905__INCLUDED_)

