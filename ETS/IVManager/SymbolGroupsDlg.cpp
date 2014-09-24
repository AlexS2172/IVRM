// SymbolGroupsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "DataProcessor.h"
#include "SymbolGroupsDlg.h"
#include "DBLayout.h"
#include "GroupStructDlg.h"
#include "RulesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// default group name
#define DEF_GROUP_NAME				_T("<Default>")
#define DEF_RULE_NAME				_T("<Default>")
#define NEW_GROUP_NAME_PREFIX		_T("New Group")

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CSymbolGroupsDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolGroupsDlg)
	ON_LBN_SELCHANGE(IDC_LIST_GROUPS, OnListGroupsSelChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_LBN_DBLCLK(IDC_LIST_GROUPS, OnListGroupsDblClk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CSymbolGroupsDlg::CSymbolGroupsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSymbolGroupsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSymbolGroupsDlg)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolGroupsDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_LIST_GROUPS, m_lstGroups);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSymbolGroupsDlg::OnInitDialog() 
{
	CWaitCursor wait;

	CDialog::OnInitDialog();

	// load groups to listbox
	for(size_t i = 1; i < m_vecGroups.size(); i++)
	{
		const CGroupData& gd = m_vecGroups[i];
		m_lstGroups.AddString(gd.m_strName);
		m_lstGroups.SetItemData(m_lstGroups.FindStringExact(-1, gd.m_strName), i);
	}

	// select 1st item
	if(m_lstGroups.GetCount() > 0)
		m_lstGroups.SetCurSel(0);

	// update buttons
	_UpdateButtons();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::OnOK() 
{
	BOOL bOK = TRUE;
	CDBLayout db;

	try
	{
		CWaitCursor wait;

		db.Connect();

		// walk through groups
		for(CGroupsVector::iterator it = m_vecGroups.begin(); it != m_vecGroups.end(); it++)
		{
			CGroupData& gd = *it;
			
			if(0L != gd.m_nID || gd.m_bIsNew) // process only non-default groups
			{
				// if no deletion
				if(m_mapDelIDs.end() == m_mapDelIDs.find(gd.m_nID))
				{
					// save only changed groups
					if(gd.m_bChanged)
					{
						// if group exists then clear dependencies
						if(gd.m_nID != 0)
						{
							db.ClearSymbolGroupItems(gd.m_nID);
						}

						// save group
						db.SaveSymbolGroup(gd.m_nID, (LPCTSTR)gd.m_strName, gd.m_nRuleID, true);

						// save stock-group dependensies
						for(CGroupSymbolsVector::iterator itSym = gd.m_vecSymbols.begin(); itSym != gd.m_vecSymbols.end(); itSym++)
						{
							db.SaveSymbolGroupItem(itSym->m_nID, gd.m_nID);
						}
					}
				}
				else
				{
					// delete group
					db.DeleteSymbolGroup(gd.m_nID);
				}
			}
		}

		// walk through rules
		for(CRulesVector::iterator itRule = m_vecRules.begin(); itRule != m_vecRules.end(); itRule++)
		{
			CRuleData& rd = *itRule;
			
			// save only changed rules
			if( rd.m_bChanged )
			{
				// save rule
				db.SaveGenerationRule(rd.m_nID, rd.m_nType, (LPCTSTR)rd.m_strName, rd.m_nStrikeIncrement, rd.m_nRequiredPoints, 
										rd.m_dSmileAccelerator, rd.m_dCallShift, rd.m_dPutShift,
										rd.m_dInterpolationFactor, rd.m_dStrikeMoneynessHi, rd.m_dStrikeMoneynessLo, rd.m_bIsDefault);
			}
		}

		db.Disconnect();

		// Send notification
		CWnd *pNotificationWnd = AfxGetMainWnd();
		if( pNotificationWnd )
			pNotificationWnd->PostMessage( WM_RULE_LIST_CHANGED, 0, (long)m_hWnd );
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage(), MB_OK | MB_ICONERROR);
		db.Disconnect();
		bOK = FALSE;
	}

	if(bOK)
	{
		CDialog::OnOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::LoadGroups()
{
	CWaitCursor wait;

	CGroupData gd;
	// map of group IDs for assign stocks to group
	CIdMap mapGroupID;
	long nIdx;
	long nID;
	
	CDBLayout db;
	db.Connect();

	/////////////////////////////////////////////////////////////////////////////
	// loading rules at first

	// clear rules before
	m_vecRules.clear();

	CClientRecordset rsRules;
	CRuleData rd;

	// read rules
	db.GetGenerationRule(0, rsRules);
	while(!rsRules.IsEOF())
	{
		rd.m_nID = rsRules[L"iRuleID"];
		rd.m_nType = rsRules[L"iRuleType"];
		
		if(rd.m_nType < RULE_TYPE_VOLA_PTS || rd.m_nType > RULE_TYPE_DUAL_QUADRATIC)
		{
			rd.m_nType = RULE_TYPE_VOLA_PTS;
		}

		rd.m_bIsDefault = (bool)rsRules[L"bIsDefaultRule"];

		if(!rd.m_bIsDefault)
		{
			rd.m_strName = (LPCTSTR)(_bstr_t)rsRules[L"vcRuleName"];
		}
		else
		{
			rd.m_strName = DEF_RULE_NAME;
		}
		rd.m_nStrikeIncrement = rsRules[L"iStrikeIncrement"];
		rd.m_nRequiredPoints = rsRules[L"iRequiredPoints"];
		rd.m_dSmileAccelerator = rsRules[L"fSmileAccelerator"];
		rd.m_dCallShift = rsRules[L"fCallShift"];
		rd.m_dPutShift = rsRules[L"fPutShift"];
		rd.m_dInterpolationFactor = rsRules[L"fInterpolationFactor"];
		rd.m_dStrikeMoneynessHi = rsRules[L"fStrikeMoneynessHi"];
		rd.m_dStrikeMoneynessLo = rsRules[L"fStrikeMoneynessLo"];

		m_vecRules.push_back(rd);
		++rsRules;
	}

	/////////////////////////////////////////////////////////////////////////////
	// loading groups

	// clear groups before
	m_vecGroups.clear();
	m_mapDelIDs.clear();

	// create default group
	gd.m_strName = DEF_GROUP_NAME;
	m_vecGroups.push_back(gd);
	nIdx = m_vecGroups.size() - 1;
	mapGroupID[gd.m_nID] = nIdx;

	CClientRecordset rsGroups, rsSymbols;
	CGroupSymbolData gsd;

	// read groups
	db.GetSymbolGroups(rsGroups);
	while(!rsGroups.IsEOF())
	{
		gd.m_nID = rsGroups[L"iGroupID"];
		gd.m_strName = (LPCTSTR)(_bstr_t)rsGroups[L"vcGroupName"];
		gd.m_nRuleID = rsGroups[L"iRuleID"];

		m_vecGroups.push_back(gd);
		nIdx = m_vecGroups.size() - 1;
		mapGroupID[gd.m_nID] = nIdx;

		++rsGroups;
	}

	// read group items
	db.GetSymbolGroupItems(0, rsSymbols);
	while(!rsSymbols.IsEOF())
	{
		gsd.m_nID = rsSymbols[L"iContractID"];
		gsd.m_strName = (LPCTSTR)(_bstr_t)rsSymbols[L"vcSymbolName"];
		gsd.m_nRuleID = rsSymbols[L"iRuleID"];
		nID = rsSymbols[L"iGroupID"];

		CIdMap::iterator it = mapGroupID.find(nID);
		
		ASSERT(mapGroupID.end() != it);

		m_vecGroups[it->second].m_vecSymbols.push_back(gsd);

		++rsSymbols;
	}

	db.Disconnect();
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::OnListGroupsSelChange() 
{
	_UpdateButtons();
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::OnButtonDelete() 
{
	int nIdx = m_lstGroups.GetCurSel();
	if(LB_ERR != nIdx)
	{
		size_t nGroupIdx = m_lstGroups.GetItemData(nIdx);
		
		ASSERT(nGroupIdx > 0 && nGroupIdx < m_vecGroups.size());

		CGroupData& gd = m_vecGroups[nGroupIdx];
		CString strMessage(_T("Are you sure to delete group '"));
		strMessage += gd.m_strName; strMessage += _T("'?");

		// if deletion confirmed
		if(AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// if not a new group 
			if(gd.m_nID != 0)
			{
				// move reference to group in deletion map
				m_mapDelIDs[gd.m_nID] = nGroupIdx;
			}
			else
			{
				// delete group from groups vector
				m_vecGroups[0].m_vecSymbols.insert( m_vecGroups[0].m_vecSymbols.begin(), gd.m_vecSymbols.begin(), gd.m_vecSymbols.end() );
				m_vecGroups.erase( m_vecGroups.begin() + nGroupIdx );
			}
			
			// remove reference to group from listbox
			m_lstGroups.DeleteString( nIdx );
			if( m_lstGroups.GetCount() > 0 )
			{
				if( nIdx == 0 )
					m_lstGroups.SetCurSel( 0 );
				else
					m_lstGroups.SetCurSel( nIdx - 1 );
			}

			_UpdateButtons();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::OnButtonNew() 
{
	int nIdx;

	CGroupData gdNew;
	gdNew.m_strName = _GetNewName();
	gdNew.m_bIsNew = true;
	m_vecGroups.push_back(gdNew);
	size_t nGroupIdx = m_vecGroups.size() - 1;
	
	ASSERT(nGroupIdx > 0 && nGroupIdx < m_vecGroups.size());
	CGroupStructDlg dlg(NULL, m_vecGroups, m_vecRules, 0, nGroupIdx);

	if(dlg.DoModal() == IDOK)
	{
		// change list box string
		const CGroupData& gdCur = m_vecGroups[nGroupIdx];
		m_lstGroups.AddString(gdCur.m_strName);
		nIdx = m_lstGroups.FindStringExact(-1, gdCur.m_strName);
		m_lstGroups.SetItemData(nIdx, nGroupIdx);
		m_lstGroups.SetCurSel(nIdx);

		_UpdateButtons();
	}
	else
	{
		m_vecGroups.erase(m_vecGroups.begin() + nGroupIdx);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::OnButtonModify() 
{
	int nIdx = m_lstGroups.GetCurSel();
	if(LB_ERR != nIdx)
	{
		size_t nGroupIdx = m_lstGroups.GetItemData(nIdx);
		
		ASSERT(nGroupIdx > 0 && nGroupIdx < m_vecGroups.size());
		CGroupStructDlg dlg(NULL, m_vecGroups, m_vecRules, 0, nGroupIdx);

		CGroupData& gdCur = m_vecGroups[nGroupIdx];

		if(dlg.DoModal() == IDOK)
		{
			// change listbox string
			m_lstGroups.DeleteString(nIdx);
			m_lstGroups.AddString(gdCur.m_strName);
			nIdx = m_lstGroups.FindStringExact(-1, gdCur.m_strName);
			m_lstGroups.SetItemData(nIdx, nGroupIdx);
			m_lstGroups.SetCurSel(nIdx);

			_UpdateButtons();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::_UpdateButtons()
{
	int nIdx = m_lstGroups.GetCurSel();
	if( LB_ERR != nIdx )
	{
		// allow deletion only for non-default groups
		//GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(m_lstGroups.GetItemData(nIdx) != 0);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CSymbolGroupsDlg::OnListGroupsDblClk() 
{
	OnButtonModify();	
}

/////////////////////////////////////////////////////////////////////////////
//
CString CSymbolGroupsDlg::_GetNewName()
{
	long i = 0;
	CString strName;

	do
	{
		i++;
		strName.Format(_T("%s %d"), NEW_GROUP_NAME_PREFIX, i);
	}
	while(LB_ERR != m_lstGroups.FindStringExact(-1, strName));

	return strName;
}
