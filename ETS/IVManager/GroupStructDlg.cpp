// GroupStructDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "GroupStructDlg.h"
#include "RuleDefinitionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CGroupStructDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupStructDlg)
	ON_BN_CLICKED(IDC_BUTTON_FROM, OnButtonFrom)
	ON_BN_CLICKED(IDC_BUTTON_TO, OnButtonTo)
	ON_LBN_SELCHANGE(IDC_LIST_SEL, OnListSelSelChange)
	ON_LBN_SELCHANGE(IDC_LIST_ALL, OnListAllSelChange)
	ON_LBN_DBLCLK(IDC_LIST_ALL, OnListAllDblClk)
	ON_LBN_DBLCLK(IDC_LIST_SEL, OnListSelDblClk)
	ON_BN_CLICKED(IDC_RULE_EDIT_BUTTON, OnRuleEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CGroupStructDlg::CGroupStructDlg(CWnd* pParent,
								CGroupsVector& vecGroups,
								CRulesVector& vecRules,
								long nDefGroupIdx, long nCurGroupIdx)
	: CDialog(CGroupStructDlg::IDD, pParent),
	m_vecGroups(vecGroups),
	m_vecRules(vecRules),
	m_nDefGroupIdx(nDefGroupIdx),
	m_nCurGroupIdx(nCurGroupIdx)
{
	//{{AFX_DATA_INIT(CGroupStructDlg)
	//}}AFX_DATA_INIT
	ASSERT(nDefGroupIdx == 0 && m_nCurGroupIdx > 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupStructDlg)
	DDX_Control(pDX, IDC_RULE_COMBO, m_cmbRule);
	DDX_Control(pDX, IDC_NAME_EDIT, m_edtName);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RULE_EDIT_BUTTON, m_btnRuleEdit);
	DDX_Control(pDX, IDC_BUTTON_TO, m_btnTo);
	DDX_Control(pDX, IDC_BUTTON_FROM, m_btnFrom);
	DDX_Control(pDX, IDC_LIST_SEL, m_lstSel);
	DDX_Control(pDX, IDC_LIST_ALL, m_lstAll);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CGroupStructDlg::OnInitDialog() 
{
	CWaitCursor wait;

	CDialog::OnInitDialog();
	m_bInitNow = true;
/*
	m_edtName.SetFlat(TRUE);
	m_btnOK.SetFlatLook();
	m_btnCancel.SetFlatLook();
	m_btnRuleEdit.SetFlatLook();
	m_btnTo.SetFlatLook();
	m_btnFrom.SetFlatLook();
*/
	const CGroupData& gdCur = m_vecGroups[m_nCurGroupIdx];

	m_edtName.SetWindowText(gdCur.m_strName);
	m_edtName.SetLimitText(250);

	size_t i, nIdx;

	m_mapNames.clear();
	for(i = 1; i < m_vecGroups.size(); i++)
	{
		m_mapNames[m_vecGroups[i].m_strName] = m_vecGroups[i].m_nID;
	}

	if(gdCur.m_bIsNew)
	{
		SetWindowText(_T("New Group"));
	}
	
	for(i = 0; i < m_vecRules.size(); i++)
	{
		const CRuleData& rd = m_vecRules[i];
		m_cmbRule.AddString(rd.m_strName);
		nIdx = m_cmbRule.FindStringExact(-1, rd.m_strName);
		m_cmbRule.SetItemData(nIdx, i);
		if(rd.m_nID == gdCur.m_nRuleID && gdCur.m_nRuleID > 0 || gdCur.m_nRuleID == 0 && rd.m_bIsDefault)
		{
			m_cmbRule.SetCurSel(nIdx);
		}
	}

	m_mapCurSyms.clear();
	for(i = 0; i < gdCur.m_vecSymbols.size(); i++)
	{
		const CGroupSymbolData& gsd = gdCur.m_vecSymbols[i];
		m_lstSel.AddString(gsd.m_strName);
		m_mapCurSyms[gsd.m_strName] = i;
		//m_lstSel.SetItemData(m_lstSel.FindStringExact(gsd.m_strName), gsd.m_nID);
	}

	const CGroupData& gdDef = m_vecGroups[m_nDefGroupIdx];
	m_mapDefSyms.clear();
	for(i = 0; i < gdDef.m_vecSymbols.size(); i++)
	{
		const CGroupSymbolData& gsd = gdDef.m_vecSymbols[i];
		m_lstAll.AddString(gsd.m_strName);
		m_mapDefSyms[gsd.m_strName] = i;
		//m_lstAll.SetItemData(m_lstAll.FindStringExact(gsd.m_strName), gsd.m_nID);
	}

	if(m_lstAll.GetCount() > 0)
		m_lstAll.SetSel(0);

	if(m_lstSel.GetCount() > 0)
		m_lstSel.SetSel(0);

	_UpdateButtons();
	
	m_bInitNow = false;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnButtonFrom() 
{
	int nCount = m_lstSel.GetSelCount();
	if(nCount > 0)
	{
		CWaitCursor wait;
		LockWindowUpdate();

		nCount = m_lstSel.GetCount();

		for(int i = nCount - 1; i >= 0; i--)
		{
			if(m_lstSel.GetSel(i))
			{
				CString strName;
				m_lstSel.GetText(i, strName);

				CNamesMap::iterator it = m_mapCurSyms.find(strName);
				ASSERT(it != m_mapCurSyms.end());

				m_lstAll.AddString(strName);
				m_lstAll.SetSel(m_lstAll.FindStringExact(-1, strName));
				m_mapDefSyms[strName] = it->second;

				m_lstSel.DeleteString(i);
				m_mapCurSyms.erase(it);
			}
		}

		UnlockWindowUpdate();

		_UpdateButtons();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnButtonTo() 
{
	int nCount = m_lstAll.GetSelCount();
	if(nCount > 0)
	{
		CWaitCursor wait;
		LockWindowUpdate();

		nCount = m_lstAll.GetCount();

		for(int i = nCount - 1; i >= 0; i--)
		{
			if(m_lstAll.GetSel(i))
			{
				CString strName;
				m_lstAll.GetText(i, strName);

				CNamesMap::iterator it = m_mapDefSyms.find(strName);
				ASSERT(it != m_mapDefSyms.end());

				m_lstSel.AddString(strName);
				m_lstSel.SetSel(m_lstSel.FindStringExact(-1, strName));
				m_mapCurSyms[strName] = it->second;

				m_lstAll.DeleteString(i);
				m_mapDefSyms.erase(it);
			}
		}

		UnlockWindowUpdate();

		_UpdateButtons();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnOK() 
{
	bool bContinue = true;
	CWaitCursor wait;
	
	/////////////////////////////////////////////////////////////////////////////
	// check group name
	CString strGroupName;

	CGroupData& gdCur = m_vecGroups[m_nCurGroupIdx];
	CGroupData& gdDef = m_vecGroups[0];
	long nCount, i;

	m_edtName.GetWindowText(strGroupName);
	strGroupName.TrimLeft();
	strGroupName.TrimRight();

	// if name changed
	if( strGroupName.Compare(gdCur.m_strName) )
	{
		// if name changed and not only a case changed
		if(strGroupName.CompareNoCase(gdCur.m_strName))
		{
			CNamesMap::iterator it = m_mapNames.find(strGroupName);
			// if no group with given name not exists in current groups
			if(m_mapNames.end() != it)
			{
				// raise an error message
				CString strMessage(_T("Group with name '"));
				strMessage += strGroupName;
				strMessage += _T("' already exists. Please specify other name.");
				AfxMessageBox(strMessage, MB_OK | MB_ICONSTOP);
				bContinue = false;
			}
		}
	}

	if( !bContinue )
		return;

	gdCur.m_strName = strGroupName;

	/////////////////////////////////////////////////////////////////////////////
	// check a rule
	i = m_cmbRule.GetCurSel();
	ASSERT( CB_ERR != i );
	i = m_cmbRule.GetItemData( i );
	gdCur.m_nRuleID = m_vecRules[i].m_nID;
	
	/////////////////////////////////////////////////////////////////////////////
	// move stocks from default group to selected
	nCount = gdDef.m_vecSymbols.size();
	for(i = nCount - 1; i >= 0; i--)
	{
		// if stock not in updated default group
		if(m_mapDefSyms.find(gdDef.m_vecSymbols[i].m_strName) == m_mapDefSyms.end())
		{
			// move it to selected
			gdCur.m_vecSymbols.push_back(gdDef.m_vecSymbols[i]);
			// and delete from default
			gdDef.m_vecSymbols.erase(gdDef.m_vecSymbols.begin() + i);
		}
	}

	// move stocks from selected group to default
	nCount = gdCur.m_vecSymbols.size();
	for(i = nCount - 1; i >= 0; i--)
	{
		// if stock not in updated current group
		if(m_mapCurSyms.find(gdCur.m_vecSymbols[i].m_strName) == m_mapCurSyms.end())
		{
			// move it to default
			gdDef.m_vecSymbols.push_back(gdCur.m_vecSymbols[i]);
			// and delete from current
			gdCur.m_vecSymbols.erase(gdCur.m_vecSymbols.begin() + i);
		}
	}

	gdCur.m_bChanged = true;
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnListSelSelChange() 
{
	_UpdateButtons();
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnListAllSelChange() 
{
	_UpdateButtons();
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnListAllDblClk() 
{
	OnButtonTo();
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnListSelDblClk() 
{
	OnButtonFrom();
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::_UpdateButtons()
{
	int nCount = m_lstAll.GetSelCount();
	BOOL bEnable = (nCount > 0 && m_nDefGroupIdx != m_nCurGroupIdx);
	GetDlgItem(IDC_BUTTON_TO)->EnableWindow(bEnable);

	nCount = m_lstSel.GetSelCount();
	bEnable = (nCount > 0 && m_nDefGroupIdx != m_nCurGroupIdx);
	GetDlgItem(IDC_BUTTON_FROM)->EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
//
void CGroupStructDlg::OnRuleEdit() 
{
	int nIdx = m_cmbRule.GetCurSel();
	if(LB_ERR != nIdx)
	{
		long nRuleIdx = m_cmbRule.GetItemData(nIdx);
		
		ASSERT(nRuleIdx >= 0 && nRuleIdx < m_vecRules.size());
		CRuleDefinitionDlg dlg(NULL, m_vecRules, nRuleIdx);

		CRuleData& rdCur = m_vecRules[nRuleIdx];

		if(dlg.DoModal() == IDOK)
		{
			// change listbox string
			m_cmbRule.DeleteString(nIdx);
			m_cmbRule.AddString(rdCur.m_strName);
			nIdx = m_cmbRule.FindStringExact(-1, rdCur.m_strName);
			m_cmbRule.SetItemData(nIdx, nRuleIdx);
			m_cmbRule.SetCurSel(nIdx);

			_UpdateButtons();
		}
	}
}
