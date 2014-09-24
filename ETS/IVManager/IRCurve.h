//{{AFX_INCLUDES()
#include "..\\EGOcx\\vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_IRCURVE_H__53C83421_E9B1_4259_9D2C_3970498EE4FE__INCLUDED_)
#define AFX_IRCURVE_H__53C83421_E9B1_4259_9D2C_3970498EE4FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IRCurve.h : header file
//
#include "EgButton.h"
#include "DBLayOut.h"
#include "BCMenu.h"
//max number of editing columns
#define MINT_COLUMN_COUNT  12

/////////////////////////////////////////////////////////////////////////////
// CIRCurve dialog

class CIRCurve : public CDialog
{
typedef struct typColumn
{
	typColumn()
	{
		m_Type=0; 
		m_bVisible=FALSE;
		m_iColWidth=0;
		m_iEditWidth=0;
		m_bDataBinding=FALSE;
		m_bDisableEdit=FALSE;
		m_bChangeEvent=FALSE;
		m_bValidateEvent=FALSE;
	}
	CString			m_strCaption;
	CString			m_strName;
	VARTYPE			m_Type; 
	BOOL			m_bVisible;
    int				m_iColWidth;
    int				m_iEditWidth;
    BOOL			m_bDataBinding;
    BOOL			m_bDisableEdit;
    BOOL			m_bChangeEvent;
    BOOL			m_bValidateEvent;
    CString			m_strFormatString;
} typColumn;

typedef enum eState
{
    STATE_NONE,
    STATE_LOAD,
    STATE_EDIT,
    STATE_NEW,
    STATE_DELETE
} eState;

// entity parameters definition
typedef struct typGridInfo
{
    long      m_lCols;
    typColumn m_Col[MINT_COLUMN_COUNT];
}typGridInfo;


// Construction
public:
	void PopupMenu (CPoint pt);
	CIRCurve(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIRCurve()
	{
		m_mnuMain.DestroyMenu();
	}
	
	bool AreRatesChanged()
		{ return m_bRatesChanged; }

// Dialog Data
	//{{AFX_DATA(CIRCurve)
	enum { IDD = IDD_INTEREST_RATES };
	CVSFlexGrid	m_vsGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIRCurve)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void CurveDefInit();
	void GridInit();
	bool CurveLoad();
	void CurveReload();
	void CurveEdit();
	bool CurveDelete();

	bool ParsePeriod(CString sSource,CString& sPeriod,long& lNumber, long& lPeriodType);
	DATE GetDate(DATE dtStartDate, long lNumber, long lPeriodType);
	CString GetDateString(DATE dtStartDate,WORD  lNumber, long lPeriodType, vt_date& dtExp);

	void LoadCommonRates();
	void FillRow(long iRow);
	void ColorCellsAll();
	void ColorCells(long iRow = -1);
	void SetState(eState x);
	bool CurveCanChangeRow();
	bool CurveUndoEdit();
	bool CurveSave();

	long ReadLng(CString V)
	{
		long lRet = 0;
		if(V.GetLength() && V != CString(STR_NEWVALUE))
		{
			_variant_t vt = _bstr_t((LPCTSTR)V);
			lRet = vt;
		}
		return lRet;
	}

	double ReadDbl(CString V)
	{
		double dRet = 0.;
		if(V.GetLength() && V != CString(STR_NEWVALUE))
		{
			_variant_t vt = _bstr_t((LPCTSTR)V);
			dRet = vt;
		}

		return dRet;
	}

	double ReadDate(CString V)
	{
		DATE dRet = 0.;
		if(V.GetLength() && V != CString(STR_NEWVALUE))
		{
			_variant_t vt = _bstr_t((LPCTSTR)V);
			dRet = vt;
		}
		return dRet;
	}



protected:

	CClientRecordset	m_rsCurve;
	CDBLayout			m_db;
	bool				m_bRatesChanged;
	bool				m_bRowChanged;

	static LPCTSTR    STR_NEWVALUE;
	static const long TYPE_PERIOD;

	long      m_lX;
	long      m_lY;
	eState    m_State;
	BCMenu	  m_mnuMain;

	// array of entity definitions
	typGridInfo m_GridDef;

	// Generated message map functions
	//{{AFX_MSG(CIRCurve)
	virtual BOOL OnInitDialog();
	afx_msg void OnAfterRowColChange(long OldRow, long OldCol, long NewRow, long NewCol);
	afx_msg void OnBeforeMouseDown(short Button, short Shift, long X, long Y, BOOL FAR* Cancel);
	afx_msg void OnBeforeRowColChange(long OldRow, long OldCol, long NewRow, long NewCol, BOOL FAR* Cancel);
	afx_msg void OnChangeEdit();
	afx_msg void OnKeyUp(short FAR* KeyCode, short Shift);
	afx_msg void OnStartEdit(long Row, long Col, BOOL FAR* Cancel);
	afx_msg void OnValidateEdit(long Row, long Col, BOOL FAR* Cancel);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	afx_msg void OnClose();
	virtual void OnCancel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IRCURVE_H__53C83421_E9B1_4259_9D2C_3970498EE4FE__INCLUDED_)
