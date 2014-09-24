// RulesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "RulesDlg.h"
#include "DBLayout.h"
#include "RuleDefinitionDlg.h"

#include "DataProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// default rule name
#define DEF_RULE_NAME				_T("<Default>")
#define NEW_RULE_NAME_PREFIX		_T("New Rule")

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CRulesDlg, CDialog)
	//{{AFX_MSG_MAP(CRulesDlg)
	ON_LBN_SELCHANGE(IDC_LIST_RULES, OnListRulesSelChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_LBN_DBLCLK(IDC_LIST_RULES, OnListRulesDblClk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CRulesDlg::CRulesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRulesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRulesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRulesDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_LIST_RULES, m_lstRules);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CRulesDlg::OnInitDialog()
{
	CWaitCursor wait;

	CDialog::OnInitDialog();
/*
	m_btnOK.SetFlatLook();
	m_btnCancel.SetFlatLook();
	m_btnNew.SetFlatLook();
	m_btnModify.SetFlatLook();
	m_btnDelete.SetFlatLook();
*/
	// load rules to listbox
	for(size_t i = 0; i < m_vecRules.size(); i++)
	{
		const CRuleData& rd = m_vecRules[i];
		m_lstRules.AddString(rd .m_strName);
		m_lstRules.SetItemData(m_lstRules.FindStringExact(-1, rd.m_strName), i);
	}

	// select 1st item
	if(m_lstRules.GetCount() > 0)
		m_lstRules.SetCurSel(0);

	// update buttons
	_UpdateButtons();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::LoadRules()
{
	CWaitCursor wait;
	CDBLayout db;
	db.Connect();

	/////////////////////////////////////////////////////////////////////////////
	// loading rules

	// clear rules before
	m_vecRules.clear();
	m_mapDelIDs.clear();

	CClientRecordset rsRules;
	CRuleData rd;
	bool bAddDefault = true;

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
			rd.m_bChanged = false;
		}
		else
		{
			bAddDefault = false;
			if(rd.m_strName != DEF_RULE_NAME)
			{
				rd.m_strName = DEF_RULE_NAME;
				rd.m_bChanged = true;
			}
		}
		rd.m_nStrikeIncrement  = rsRules[L"iStrikeIncrement"];
		rd.m_nRequiredPoints   = rsRules[L"iRequiredPoints"];
		rd.m_dSmileAccelerator = rsRules[L"fSmileAccelerator"];
		rd.m_dCallShift = rsRules[L"fCallShift"];
		rd.m_dPutShift  = rsRules[L"fPutShift"];
		rd.m_dInterpolationFactor = rsRules[L"fInterpolationFactor"];
		
		if(rd.m_dInterpolationFactor < 0.)
		{
			rd.m_dInterpolationFactor = 0.;
		}
		else if(rd.m_dInterpolationFactor > 1.)
		{
			rd.m_dInterpolationFactor = 1.;
		}

		rd.m_dStrikeMoneynessHi = rsRules[L"fStrikeMoneynessHi"];
		rd.m_dStrikeMoneynessLo = rsRules[L"fStrikeMoneynessLo"];

		m_vecRules.push_back(rd);
		++rsRules;
	}

	if(bAddDefault)
	{
		CRuleData rdDef;
		rdDef.m_strName = DEF_RULE_NAME;
		rdDef.m_bChanged = true;
		m_vecRules.push_back(rd);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::OnOK()
{
	BOOL bOK = TRUE;
	CDBLayout db;

	try
	{
		CWaitCursor wait;

		db.Connect();

		// walk through rules
		for(CRulesVector::iterator it = m_vecRules.begin(); it != m_vecRules.end(); it++)
		{
			CRuleData& rd = *it;
			
			// if no deletion
			if(m_mapDelIDs.end() == m_mapDelIDs.find(rd.m_nID))
			{
				// save only changed rules
				if(rd.m_bChanged)
				{
					// save rule
					db.SaveGenerationRule(rd.m_nID, rd.m_nType, (LPCTSTR)rd.m_strName, rd.m_nStrikeIncrement, rd.m_nRequiredPoints, 
											rd.m_dSmileAccelerator, rd.m_dCallShift, rd.m_dPutShift,
											rd.m_dInterpolationFactor, rd.m_dStrikeMoneynessHi, rd.m_dStrikeMoneynessLo, rd.m_bIsDefault);
				}
			}
			else
			{
				try
				{
					// delete rule
					db.DeleteGenerationRule(rd.m_nID);
				}
				catch(const _com_error& e)
				{
					if( e.Error() == DB_E_INTEGRITYVIOLATION )
					{
						CString message;
						message.Format( _T("Can't delete rule \"%s\" because it's connected to stock group."), rd.m_strName );
						AfxMessageBox( message, MB_OK | MB_ICONERROR );

						m_mapDelIDs.erase( rd.m_nID );

						bOK = FALSE;
					}
					else
						throw;
				}
			}
		}

		db.Disconnect();
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage(), MB_OK | MB_ICONERROR);
		db.Disconnect();
		bOK = FALSE;
	}

	// Send notification
	CWnd *pNotificationWnd = AfxGetMainWnd();
	if( pNotificationWnd )
		pNotificationWnd->PostMessage( WM_RULE_LIST_CHANGED, 0, (long)m_hWnd );

	if( bOK )
	{
		CDialog::OnOK();
	}
	else
	{
		m_lstRules.ResetContent();

		// load rules to listbox
		for(size_t i = 0; i < m_vecRules.size(); i++)
		{
			const CRuleData& rd = m_vecRules[i];
			if( m_mapDelIDs.end() != m_mapDelIDs.find(rd.m_nID) )
				continue;

			m_lstRules.AddString(rd .m_strName);
			m_lstRules.SetItemData(m_lstRules.FindStringExact(-1, rd.m_strName), i);
		}

		// select 1st item
		if(m_lstRules.GetCount() > 0)
			m_lstRules.SetCurSel(0);

		// update buttons
		_UpdateButtons();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::OnListRulesSelChange()
{
	_UpdateButtons();
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::OnButtonDelete()
{
	int nIdx = m_lstRules.GetCurSel();
	if(LB_ERR != nIdx)
	{
		size_t nRuleIdx = m_lstRules.GetItemData(nIdx);
		
		ASSERT(nRuleIdx > 0 && nRuleIdx < m_vecRules.size());

		CRuleData& rd = m_vecRules[nRuleIdx];
		CString strMessage(_T("Are you sure to delete rule '"));
		strMessage += rd.m_strName; strMessage += _T("'?");

		// if deletion confirmed
		if(AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// if not a new group 
			if(rd.m_nID != 0)
			{
				// move reference to group in deletion map
				m_mapDelIDs[rd.m_nID] = nRuleIdx;
			}
			
			// remove reference to group from listbox
			m_lstRules.DeleteString(nIdx);
			m_lstRules.SetCurSel(nIdx - 1);

			_UpdateButtons();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::OnButtonNew()
{
	int nIdx;

	CRuleData rdNew;
	rdNew.m_nID = -1;
	rdNew.m_nType = RULE_TYPE_VOLA_PTS;
	rdNew.m_strName = _GetNewName();
	rdNew.m_bIsNew  = true;
	rdNew.m_dStrikeMoneynessLo = 10.0;
	rdNew.m_dStrikeMoneynessHi = 150.0;
	rdNew.m_nStrikeIncrement   = 1;
	rdNew.m_nRequiredPoints    = 1;
	rdNew.m_dCallShift		   = 100;
	rdNew.m_dPutShift		   = 100;
	m_vecRules.push_back(rdNew);
	size_t nRuleIdx = m_vecRules.size() - 1;
	
	ASSERT(nRuleIdx > 0 && nRuleIdx < m_vecRules.size());
	CRuleDefinitionDlg dlg(NULL, m_vecRules, nRuleIdx);

	if(dlg.DoModal() == IDOK)
	{
		// change listbox string
		CRuleData& rdCur = m_vecRules[nRuleIdx];
		rdCur.m_bIsNew   = false;
		rdCur.m_bChanged = true;

		m_lstRules.AddString(rdCur.m_strName);
		nIdx = m_lstRules.FindStringExact(-1, rdCur.m_strName);
		m_lstRules.SetItemData(nIdx, nRuleIdx);
		m_lstRules.SetCurSel(nIdx);

		_UpdateButtons();
	}
	else
	{
		m_vecRules.erase(m_vecRules.begin() + nRuleIdx);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::OnButtonModify()
{
	int nIdx = m_lstRules.GetCurSel();
	if(LB_ERR != nIdx)
	{
		size_t nRuleIdx = m_lstRules.GetItemData(nIdx);
		
		ASSERT(nRuleIdx >= 0 && nRuleIdx < m_vecRules.size());
		CRuleDefinitionDlg dlg(NULL, m_vecRules, nRuleIdx);

		CRuleData& rdCur = m_vecRules[nRuleIdx];

		if(dlg.DoModal() == IDOK)
		{
			// change listbox string
			m_lstRules.DeleteString(nIdx);
			m_lstRules.AddString(rdCur.m_strName);
			nIdx = m_lstRules.FindStringExact(-1, rdCur.m_strName);
			m_lstRules.SetItemData(nIdx, nRuleIdx);
			m_lstRules.SetCurSel(nIdx);

			_UpdateButtons();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::_UpdateButtons()
{
	int nIdx = m_lstRules.GetCurSel();
	if(LB_ERR != nIdx)
	{
		// allow deletion only for non-default rule
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(!m_vecRules[m_lstRules.GetItemData(nIdx)].m_bIsDefault);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CRulesDlg::OnListRulesDblClk() 
{
	OnButtonModify();
}

/////////////////////////////////////////////////////////////////////////////
//
CString CRulesDlg::_GetNewName()
{
	long i = 0;
	CString strName;

	do
	{
		i++;
		strName.Format(_T("%s %d"), NEW_RULE_NAME_PREFIX, i);
	}
	while(LB_ERR != m_lstRules.FindStringExact(-1, strName));

	return strName;
}
