// RuleDefinitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "RuleDefinitionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CRuleDefinitionDlg, CDialog)
	//{{AFX_MSG_MAP(CRuleDefinitionDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnTypeSelChange)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_FACTOR, OnFactorChange)
	ON_EN_CHANGE(IDC_ACCELERATOR, OnAcceleratorChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CRuleDefinitionDlg::CRuleDefinitionDlg(CWnd* pParent,
									CRulesVector& vecRules,
									long nCurRuleIdx)
	: CDialog(CRuleDefinitionDlg::IDD, pParent),
	m_vecRules(vecRules),
	m_nCurRuleIdx(nCurRuleIdx),
	m_nCurFactorPos(-1)
{
	//{{AFX_DATA_INIT(CRuleDefinitionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CRuleDefinitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleDefinitionDlg)
	DDX_Control(pDX, IDC_SMILE_ACCELERATOR_TITLE, m_stcSmileAccelerator);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_HI_VOLY, m_edtHiVoly);
	DDX_Control(pDX, IDC_LO_VOLY, m_edtLoVoly);
	DDX_Control(pDX, IDC_FACTOR, m_edtFactor);
	DDX_Control(pDX, IDC_PUT_SHIFT, m_edtPutShift);
	DDX_Control(pDX, IDC_CALL_SHIFT, m_edtCallShift);
	DDX_Control(pDX, IDC_ACCELERATOR, m_edtAccelerator);
	DDX_Control(pDX, IDC_INCREMENT, m_edtIncrement);
	DDX_Control(pDX, IDC_NAME_EDIT, m_edtName);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_SLIDER_FACTOR, m_sldFactor);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CRuleDefinitionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bInitNow = true;

	_InitControls();

	const CRuleData& rdCur = m_vecRules[m_nCurRuleIdx];

	m_edtName.SetWindowText(rdCur.m_strName);
	m_edtName.SetLimitText(250);

	m_mapNames.clear();
	for(size_t i = 0; i < m_vecRules.size(); i++)
	{
		m_mapNames[m_vecRules[i].m_strName] = m_vecRules[i].m_nID;
	}

	if(rdCur.m_bIsNew)
	{
		SetWindowText(_T("New Rule"));
	}

	if(rdCur.m_bIsDefault)
	{
		m_edtName.EnableWindow(FALSE);
	}

	m_cmbType.AddString(RULE_TYPE_VOLA_PTS_NAME);
	m_cmbType.AddString(RULE_TYPE_VOLA_PERC_NAME);
	m_cmbType.AddString(RULE_TYPE_VOLA_PERC_PREV_NAME);
	m_cmbType.AddString(RULE_TYPE_CURVE_FIT_NAME);
	m_cmbType.AddString(RULE_TYPE_DUAL_QUADRATIC_NAME);

	m_cmbType.SetCurSel(rdCur.m_nType);

	m_edtCallShift.SetValue(rdCur.m_dCallShift);
	m_edtPutShift.SetValue(rdCur.m_dPutShift);
	m_edtAccelerator.SetValue(rdCur.m_dSmileAccelerator);
	m_edtFactor.SetValue(rdCur.m_dInterpolationFactor);
	m_sldFactor.SetPos(static_cast<int>(rdCur.m_dInterpolationFactor * 100));
	m_edtHiVoly.SetValue(rdCur.m_dStrikeMoneynessHi);
	m_edtLoVoly.SetValue(rdCur.m_dStrikeMoneynessLo);

	if(rdCur.m_nType != RULE_TYPE_CURVE_FIT)
	{
		m_edtIncrement.SetValue(rdCur.m_nStrikeIncrement);
	}
	else
	{
		m_edtIncrement.SetValue(rdCur.m_nRequiredPoints);
	}

#ifdef _WSB
	m_edtAccelerator.ShowWindow( SW_HIDE );
	m_stcSmileAccelerator.ShowWindow( SW_HIDE );
#endif//_WSB

	OnTypeSelChange();
	m_bInitNow = false;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CRuleDefinitionDlg::OnOK() 
{
	bool bContinue = true;
	CWaitCursor wait;
	
	CRuleData& rdCur = m_vecRules[m_nCurRuleIdx];

	if(!rdCur.m_bIsDefault)
	{
		/////////////////////////////////////////////////////////////////////////////
		// check rule name
		CString strRuleName;

		m_edtName.GetWindowText(strRuleName);
		strRuleName.TrimLeft();
		strRuleName.TrimRight();

		// if name changed
		if(strRuleName.Compare(rdCur.m_strName))
		{
			// if name changed and not only a case changed
			if(strRuleName.CompareNoCase(rdCur.m_strName))
			{
				CNamesMap::iterator it = m_mapNames.find(strRuleName);
				// if no rule with given name not exists in current rules
				if(m_mapNames.end() != it)
				{
					// raise an error message
					CString strMessage(_T("Rule with name '"));
					strMessage += strRuleName;
					strMessage += _T("' already exists. Please specify other name.");
					AfxMessageBox(strMessage, MB_OK | MB_ICONSTOP);
					bContinue = false;
				}
			}
		}

		if(!bContinue)
		{
			return;
		}

		rdCur.m_strName = strRuleName;
	}

	rdCur.m_nType = m_cmbType.GetCurSel();
	rdCur.m_dCallShift = m_edtCallShift.GetValue();
	rdCur.m_dPutShift = m_edtPutShift.GetValue();
	rdCur.m_dSmileAccelerator = m_edtAccelerator.GetValue();
	rdCur.m_dInterpolationFactor = m_edtFactor.GetValue();

	if(rdCur.m_dInterpolationFactor < 0)
	{
		rdCur.m_dInterpolationFactor = 0.;
	}
	else if(rdCur.m_dInterpolationFactor > 1)
	{
		rdCur.m_dInterpolationFactor = 1.;
	}

	rdCur.m_dStrikeMoneynessHi = m_edtHiVoly.GetValue();
	rdCur.m_dStrikeMoneynessLo = m_edtLoVoly.GetValue();

	if(rdCur.m_nType != RULE_TYPE_CURVE_FIT)
	{
		rdCur.m_nStrikeIncrement = static_cast<long>(m_edtIncrement.GetValue());
	}
	else
	{
		rdCur.m_nRequiredPoints = static_cast<long>(m_edtIncrement.GetValue());
	}

	rdCur.m_bChanged = true;

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CRuleDefinitionDlg::OnTypeSelChange() 
{
	long nType = m_cmbType.GetCurSel();
	if(nType != RULE_TYPE_CURVE_FIT && nType != RULE_TYPE_DUAL_QUADRATIC)
	{
		m_edtCallShift.ShowWindow(SW_SHOWNORMAL);
		m_edtPutShift.ShowWindow(SW_SHOWNORMAL);
		m_edtIncrement.ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_CALL_SHIFT_TITLE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_PUT_SHIFT_TITLE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_TYPE_TITLE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_TYPE_TITLE)->SetWindowText(_T("Strike &Increment :"));
	}
	else if(nType == RULE_TYPE_CURVE_FIT)
	{
		m_edtCallShift.ShowWindow(SW_HIDE);
		m_edtPutShift.ShowWindow(SW_HIDE);
		m_edtIncrement.ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_CALL_SHIFT_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PUT_SHIFT_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TYPE_TITLE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_TYPE_TITLE)->SetWindowText(_T("P&oints Amount :"));
	}
	else if(nType == RULE_TYPE_DUAL_QUADRATIC)
	{
		m_edtCallShift.ShowWindow(SW_HIDE);
		m_edtPutShift.ShowWindow(SW_HIDE);
		m_edtIncrement.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CALL_SHIFT_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PUT_SHIFT_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TYPE_TITLE)->ShowWindow(SW_HIDE);
	}

	if(!m_bInitNow)
	{
		CRuleData& rdCur = m_vecRules[m_nCurRuleIdx];
		rdCur.m_nType = nType;
		rdCur.m_bChanged = true;
	}

	_InitControls();
}

/////////////////////////////////////////////////////////////////////////////
//
void CRuleDefinitionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(!m_bInitNow && pScrollBar->GetDlgCtrlID() == IDC_SLIDER_FACTOR)
	{
		int nNewPos = m_sldFactor.GetPos();
		if(nNewPos != m_nCurFactorPos)
		{
			m_bInitNow = true;
			m_nCurFactorPos = nNewPos;

			CRuleData& rdCur = m_vecRules[m_nCurRuleIdx];
			rdCur.m_dInterpolationFactor = m_nCurFactorPos / 100.;
			m_edtFactor.SetValue(rdCur.m_dInterpolationFactor);
			
			m_bInitNow = false;
		}
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CRuleDefinitionDlg::OnFactorChange() 
{
	if(!m_bInitNow)
	{
		m_bInitNow = true;
		
		CRuleData& rdCur = m_vecRules[m_nCurRuleIdx];
		rdCur.m_dInterpolationFactor = m_edtFactor.GetValue();
		m_sldFactor.SetPos(static_cast<long>(rdCur.m_dInterpolationFactor * 100));
		
		m_bInitNow = false;
	}
}


void CRuleDefinitionDlg::OnAcceleratorChange() 
{
	if(!m_bInitNow)
	{
		m_bInitNow = true;
		
		CRuleData& rdCur = m_vecRules[m_nCurRuleIdx];

		double dAccel = m_edtAccelerator.GetValue();
		if( dAccel < 0.0 || dAccel > 1.0 )
			m_edtAccelerator.SetValue( rdCur.m_dSmileAccelerator );
		else
			rdCur.m_dSmileAccelerator = m_edtAccelerator.GetValue();
	
		m_bInitNow = false;
	}
}


/////////////////////////////////////////////////////////////////////////////
//
void CRuleDefinitionDlg::_InitControls()
{
//	m_edtHiVoly.SetFlat(TRUE);
	m_edtHiVoly.SetForceToUsePeriod(FALSE);
	m_edtHiVoly.SetPositiveFormat(_T("1.1"));
	m_edtHiVoly.SetNegativeFormat(_T("1.1"));
	m_edtHiVoly.SetFractionalDigitCount(2);
	m_edtHiVoly.SetDecimalDigitCount(3);
	
//	m_edtLoVoly.SetFlat(TRUE);
	m_edtLoVoly.SetForceToUsePeriod(FALSE);
	m_edtLoVoly.SetPositiveFormat(_T("1.1"));
	m_edtLoVoly.SetNegativeFormat(_T("1.1"));
	m_edtLoVoly.SetFractionalDigitCount(2);
	m_edtLoVoly.SetDecimalDigitCount(3);

//	m_edtFactor.SetFlat(TRUE);
	m_edtFactor.SetForceToUsePeriod(FALSE);
	m_edtFactor.SetPositiveFormat(_T("1.1"));
	m_edtFactor.SetNegativeFormat(_T("1.1"));
	m_edtFactor.SetFractionalDigitCount(2);
	m_edtFactor.SetDecimalDigitCount(1);

	long Type = m_vecRules[m_nCurRuleIdx].m_nType;

//	m_edtPutShift.SetFlat(TRUE);
	m_edtPutShift.SetForceToUsePeriod(FALSE);
	m_edtPutShift.SetPositiveFormat(_T("1.1"));
	m_edtPutShift.SetNegativeFormat(_T("-1.1"));
	m_edtPutShift.SetFractionalDigitCount( Type == RULE_TYPE_VOLA_PTS ? 0 : 2 );
	m_edtPutShift.SetDecimalDigitCount( Type == RULE_TYPE_VOLA_PTS ? 5 : 2 );

//	m_edtCallShift.SetFlat(TRUE);
	m_edtCallShift.SetForceToUsePeriod(FALSE);
	m_edtCallShift.SetPositiveFormat(_T("1.1"));
	m_edtCallShift.SetNegativeFormat(_T("-1.1"));
	m_edtCallShift.SetFractionalDigitCount( Type == RULE_TYPE_VOLA_PTS ? 0 : 2 );
	m_edtCallShift.SetDecimalDigitCount( Type == RULE_TYPE_VOLA_PTS ? 5 : 2 );

//	m_edtAccelerator.SetFlat(TRUE);
	m_edtAccelerator.SetForceToUsePeriod(FALSE);
	m_edtAccelerator.SetPositiveFormat(_T("1.1"));
	m_edtAccelerator.SetNegativeFormat(_T("1.1"));
	m_edtAccelerator.SetFractionalDigitCount(2);
	m_edtAccelerator.SetDecimalDigitCount(2);

//	m_edtIncrement.SetFlat(TRUE);
	m_edtIncrement.SetForceToUsePeriod(FALSE);
	m_edtIncrement.SetPositiveFormat(_T("1.1"));
	m_edtIncrement.SetNegativeFormat(_T("1.1"));
	m_edtIncrement.SetFractionalDigitCount(0);
	m_edtIncrement.SetDecimalDigitCount(2);
	
//	m_edtName.SetFlat(TRUE);

//	m_btnOK.SetFlatLook(TRUE);
//	m_btnCancel.SetFlatLook(TRUE);

	m_sldFactor.SetRange(0, 100);
	m_sldFactor.SetTicFreq(10);
}
