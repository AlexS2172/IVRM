// IRCurve.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "IRCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct SInterestRate
{
	long m_lID;
	long m_lPeriodID; 
	long m_lNumber;
	double m_dbBid; 
	double m_dbAsk;

	SInterestRate()
	{
		ZeroMemory(this,sizeof(SInterestRate));
	}


};
/////////////////////////////////////////////////////////////////////////////
// CIRCurve dialog
LPCTSTR CIRCurve::STR_NEWVALUE = "<New>";
const long CIRCurve::TYPE_PERIOD = -10;

CIRCurve::CIRCurve(CWnd* pParent /*=NULL*/)
	: CDialog(CIRCurve::IDD, pParent)
{
	m_State = STATE_NONE;
	m_lX  = 0;
	m_lY  = 0;
	m_bRatesChanged = false;
	m_bRowChanged = false;

	//{{AFX_DATA_INIT(CIRCurve)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIRCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIRCurve)
	DDX_Control(pDX, IDC_GRID, m_vsGrid);

	//}}AFX_DATA_MAP
}
  

BEGIN_MESSAGE_MAP(CIRCurve, CDialog)
	//{{AFX_MSG_MAP(CIRCurve)
	ON_WM_PARENTNOTIFY()	
	ON_COMMAND(ID_IR_ADD, OnAdd)
	ON_COMMAND(ID_IR_EDIT,OnEdit)
	ON_COMMAND(ID_IR_DELETE,OnDelete)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIRCurve message handlers

BOOL CIRCurve::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_mnuMain.LoadMenu (IDR_INTERESTRATE);
	m_mnuMain.LoadToolbar (IDR_TOOLBAR_IR);

	m_db.Connect();
	CurveDefInit();
    GridInit();
	CurveLoad();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIRCurve::GridInit()
{
	
		m_vsGrid.SetRows(1);
		m_vsGrid.SetCols(MINT_COLUMN_COUNT);

		m_vsGrid.SetAllowBigSelection(FALSE);
		m_vsGrid.SetAllowSelection(FALSE);
		m_vsGrid.SetAllowUserFreezing(0);// flexFreezeNone
		m_vsGrid.SetAllowUserResizing(1);// flexResizeColumns
		//m_vsGrid.SetAppearance(flex3D);// flex3DLight
		m_vsGrid.SetAutoResize(FALSE);
		m_vsGrid.SetAutoSearch(0);// flexSearchNone
		m_vsGrid.SetAutoSizeMouse(FALSE);
		m_vsGrid.SetDataMode(0);// flexDMFree
		m_vsGrid.SetEditable(2);// flexEDKbdMouse
		m_vsGrid.SetEllipsis(0);// flexNoEllipsis
		m_vsGrid.SetExplorerBar(0);// flexExNone
		m_vsGrid.SetExtendLastCol(TRUE);
		m_vsGrid.SetFixedCols(0);
		m_vsGrid.SetFixedRows(1);
		m_vsGrid.SetFocusRect(2);// flexFocusHeavy
		m_vsGrid.SetFrozenCols(0);
		m_vsGrid.SetFrozenRows(0);
		m_vsGrid.SetGridLines(1);// flexGridFlat
		m_vsGrid.SetGridLinesFixed(1);// flexGridFlat
		m_vsGrid.SetGridLineWidth(1);
		m_vsGrid.SetMergeCells(1);// flexMergeFree
		m_vsGrid.SetMergeCompare(0);// flexMCExact
		m_vsGrid.SetOutlineBar(6);// flexOutlineBarSymbolsLeaf
		m_vsGrid.SetScrollBars(3);// flexScrollBarBoth
		m_vsGrid.SetScrollTips(TRUE);
		m_vsGrid.SetSelectionMode(1);//flexSelectionByRow
		m_vsGrid.SetTabBehavior(1);// flexTabCells
		m_vsGrid.SetWallPaperAlignment(0);//flexPicAlignLeftTop
		m_vsGrid.SetWordWrap(FALSE);
		m_vsGrid.SetExtendLastCol(TRUE);

		m_vsGrid.SetBackColorFixed(0x8000000F);
		m_vsGrid.SetForeColorFixed(0x80000012);

        m_vsGrid.SetCell(2, _variant_t(0L),_variant_t(0L), _variant_t(0L), _variant_t(long(m_GridDef.m_lCols - 1)), _variant_t(4L));
        m_vsGrid.SetRows(1);
        m_vsGrid.SetCols(m_GridDef.m_lCols);
        for(int i = 0; i<m_GridDef.m_lCols;i++)
		{
			m_vsGrid.SetColHidden(i,!m_GridDef.m_Col[i].m_bVisible);
            m_vsGrid.SetTextMatrix(0, i, m_GridDef.m_Col[i].m_strCaption);
            m_vsGrid.SetColKey(i, m_GridDef.m_Col[i].m_strName);
            m_vsGrid.SetColWidth(i,  m_GridDef.m_Col[i].m_iColWidth);
            m_vsGrid.SetColDataType(i, m_GridDef.m_Col[i].m_Type);
            m_vsGrid.SetColFormat(i, m_GridDef.m_Col[i].m_strFormatString);
            m_vsGrid.SetColComboList(i,_T(""));
		}

}

void CIRCurve::CurveDefInit()
{
	long i = 0;
	m_GridDef.m_lCols = 8;
	typColumn* pData = NULL;

	pData = &m_GridDef.m_Col[0];
	pData->m_strName = _T("iCurvePointID");
    pData->m_Type = VT_I4;
    pData->m_bVisible = VARIANT_FALSE;
    pData->m_iColWidth = 500;
    pData->m_bDataBinding = VARIANT_TRUE;
        
	pData = &m_GridDef.m_Col[1];
    pData->m_strName = _T("Period");
    pData->m_Type = VT_BSTR;
	pData->m_strCaption = _T("Period");
    pData->m_bVisible = VARIANT_TRUE;
    pData->m_iColWidth = 1200;
    pData->m_iEditWidth = 1200;
    pData->m_bDataBinding = VARIANT_FALSE;
        
	pData = &m_GridDef.m_Col[2];
    pData->m_strName = _T("iPeriodTypeID");
    pData->m_Type = VT_I4;
	pData->m_strCaption = _T("PeriodTypeID");
    pData->m_bVisible = VARIANT_FALSE;
    pData->m_iColWidth = 1200;
    pData->m_iEditWidth = 1200;
    pData->m_bDataBinding = VARIANT_TRUE;
	pData->m_bDisableEdit = VARIANT_TRUE;

	pData = &m_GridDef.m_Col[3];
    pData->m_strName = _T("iNumber");
    pData->m_Type = VT_I4;
	pData->m_strCaption = _T("Num");
    pData->m_bVisible = VARIANT_FALSE;
    pData->m_iColWidth = 1200;
    pData->m_iEditWidth = 1200;
    pData->m_bDataBinding = VARIANT_TRUE;
	pData->m_bDisableEdit = VARIANT_TRUE;
        
	pData = &m_GridDef.m_Col[4];
    pData->m_strName = _T("Date");
    pData->m_Type = VT_DATE;
	pData->m_strCaption = _T("Date");
    pData->m_bVisible = VARIANT_TRUE;
    pData->m_iColWidth = 1200;
    pData->m_iEditWidth = 1200;
    pData->m_bDataBinding = VARIANT_FALSE;
	pData->m_bDisableEdit = VARIANT_TRUE;
       
	pData = &m_GridDef.m_Col[5];
    pData->m_strName = _T("fRateBid");
    pData->m_Type = VT_R8;
	pData->m_strCaption = _T("Bid Rate, %");
    pData->m_bVisible = VARIANT_TRUE;
    pData->m_iColWidth = 1200;
    pData->m_iEditWidth = 1200;
    pData->m_bDataBinding = VARIANT_TRUE;
	pData->m_strFormatString = _T("#.##");

	pData = &m_GridDef.m_Col[6];
    pData->m_strName = _T("fRateAsk");
    pData->m_Type = VT_R8;
	pData->m_strCaption = _T("Ask Rate, %");
    pData->m_bVisible = VARIANT_TRUE;
    pData->m_iColWidth = 1200;
    pData->m_iEditWidth = 1200;
    pData->m_bDataBinding = VARIANT_TRUE;
	pData->m_strFormatString = _T("#.##");
        
	pData = &m_GridDef.m_Col[7];
    pData->m_strName = _T("LastCol");
    pData->m_Type = VT_BSTR;
	pData->m_strCaption = _T("");
    pData->m_bVisible = VARIANT_FALSE;
    pData->m_iColWidth = 120;
    pData->m_iEditWidth = 120;
    pData->m_bDataBinding = VARIANT_FALSE;
	pData->m_bDisableEdit = VARIANT_TRUE;	
}

bool CIRCurve::CurveLoad()
{
	bool bRet = false;
    SetState(STATE_LOAD);

	try
	{
		m_rsCurve.Close();
		m_db.InterestRateCurveGet(0, m_rsCurve);

	    LoadCommonRates();
    
	    // displaying data
    
        m_vsGrid.SetRows(m_rsCurve.GetRecordCount() + 2);
		if(m_rsCurve.GetRecordCount())
		{
			long i = 1;
			for(m_rsCurve.MoveFirst();!m_rsCurve.IsEOF();m_rsCurve.MoveNext())
			{
	            FillRow(i);
				i++;
			}
		}

        m_vsGrid.AddItem(_T(""),vtMissing);
		for(long i=0; i<m_GridDef.m_lCols;i++)
		{
			if(! m_GridDef.m_Col[i].m_bDisableEdit)
			{
				m_vsGrid.SetCell(0,_variant_t(m_vsGrid.GetRows()-2L),_variant_t(i),_variant_t(m_vsGrid.GetRows()-1L),_variant_t(i),_variant_t(STR_NEWVALUE));
			}
			else
				m_vsGrid.SetTextMatrix (m_vsGrid.GetRows()-2, i, _T("")); 
		}
 		m_vsGrid.SetRowHidden(m_vsGrid.GetRows() - 1, TRUE);
		m_vsGrid.SetRow(1);
    
		ColorCellsAll();
    
		bRet = true;
	}
	catch(...)
	{
	}

    SetState(STATE_NONE);
	return bRet;
}

void CIRCurve::LoadCommonRates()
{

}

void CIRCurve::FillRow(long iRow)
{

	CString sPeriod;
	vt_date dExp;

	for(long i=0; i<m_GridDef.m_lCols;i++)
	{
		if(m_GridDef.m_Col[i].m_bDataBinding)
		{
			_bstr_t bsText = m_rsCurve[_bstr_t(m_GridDef.m_Col[i].m_strName)].GetValue();
			m_vsGrid.SetTextMatrix(iRow,i,bsText);
		}
	}
    
	long lPeriod = _variant_t(m_vsGrid.GetTextMatrix(iRow, 3));
	long lPeriodType = _variant_t(m_vsGrid.GetTextMatrix(iRow, 2));

	sPeriod = GetDateString(vt_date::GetCurrentDate(), (WORD)lPeriod, lPeriodType, dExp);
	if(lPeriod>1)
		 sPeriod += _T("s");

		m_vsGrid.SetTextMatrix(iRow, 1, sPeriod);

		_variant_t vt = (DATE)dExp; 
		vt.vt = VT_DATE;
		m_vsGrid.SetTextMatrix(iRow, 4, _bstr_t(vt));

		CString strNew;
		CString strOld;
		double  dRate;	
		TCHAR* szStop=NULL;

			
		strOld = m_vsGrid.GetTextMatrix(iRow, 5);
		dRate = _tcstod(strOld, &szStop);
		strNew.Format(_T("%.2f"),dRate*100.);
		m_vsGrid.SetTextMatrix(iRow, 5, strNew);

		strOld = m_vsGrid.GetTextMatrix(iRow, 6);
		dRate = _tcstod(strOld, &szStop);
		strNew.Format(_T("%.2f"), dRate*100.);
		m_vsGrid.SetTextMatrix(iRow, 6, strNew);
}

void CIRCurve::ColorCellsAll()
{
	for(long iRow=0; iRow< m_vsGrid.GetRows();iRow++)
	{
        ColorCells(iRow);
	}
}

// coloring cells of a row
void CIRCurve::ColorCells(long iRow)
{
	long iBackColorSel;
	long iForeColorSel;
	long iForeColor;

	if(iRow == m_vsGrid.GetRow())
	{
		switch(m_State)
		{
		case STATE_EDIT:
		case STATE_NEW:
			{
                iBackColorSel = 0xC0FFC0;
                iForeColorSel = 0xFF;
                iForeColor = 0x0;
			}
			break;
		default:
			{
                iBackColorSel = 0xC0FFC0;
	            if(iRow >= m_vsGrid.GetRows() - 2)
				{
                    iForeColorSel = 0xC0C0C0;
                    iForeColor = 0xC0C0C0;
				}
                else
				{
                    iForeColorSel = 0x0;
                    iForeColor = 0x0;
				}
			}
		}
	}
	else
	{
            if(iRow >= m_vsGrid.GetRows() - 2)
                iForeColor = 0xC0C0C0;
            else
                iForeColor = 0x0;
	}
	if(iRow == m_vsGrid.GetRow())
	{
       m_vsGrid.SetBackColorSel(iBackColorSel);
       m_vsGrid.SetForeColorSel(iForeColorSel);
	}

	m_vsGrid.SetCell(7,
		_variant_t(iRow), _variant_t(0L),
		_variant_t(iRow), _variant_t(m_GridDef.m_lCols - 1L),
		_variant_t(iForeColor));
}
        

void CIRCurve::SetState(eState x)
{
    m_State = x;
	if(STATE_LOAD == m_State)
		BeginWaitCursor(); 
	else
		EndWaitCursor();
    ColorCellsAll();
}
        

BEGIN_EVENTSINK_MAP(CIRCurve, CDialog)
    //{{AFX_EVENTSINK_MAP(CIRCurve)
	ON_EVENT(CIRCurve, IDC_VSFLEXGRIDADO1, 7 /* AfterRowColChange */, OnAfterRowColChange, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CIRCurve, IDC_VSFLEXGRIDADO1, 5 /* BeforeMouseDown */, OnBeforeMouseDown, VTS_I2 VTS_I2 VTS_R4 VTS_R4 VTS_PBOOL)
	ON_EVENT(CIRCurve, IDC_VSFLEXGRIDADO1, 6 /* BeforeRowColChange */, OnBeforeRowColChange, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CIRCurve, IDC_VSFLEXGRIDADO1, 28 /* ChangeEdit */, OnChangeEdit, VTS_NONE)
	ON_EVENT(CIRCurve, IDC_VSFLEXGRIDADO1, -604 /* KeyUp */, OnKeyUp, VTS_PI2 VTS_I2)
	ON_EVENT(CIRCurve, IDC_VSFLEXGRIDADO1, 22 /* StartEdit */, OnStartEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CIRCurve, IDC_VSFLEXGRIDADO1, 23 /* ValidateEdit */, OnValidateEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CIRCurve, IDC_GRID, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CIRCurve::OnAfterRowColChange(long OldRow, long OldCol, long NewRow, long NewCol) 
{
    if(OldRow != NewRow)
		ColorCells(NewRow);	
}

void CIRCurve::OnBeforeMouseDown(short Button, short Shift, long X, long Y, BOOL FAR* Cancel) 
{
//         Opening context menu
	if( Button & 2  && m_vsGrid.GetEditWindow() == 0)
	{
		m_vsGrid.SetRow(m_vsGrid.GetMouseRow());
		CPoint pt;
		pt.x = m_lX;
		pt.y = m_lY;
		ClientToScreen(&pt);

		PopupMenu (pt);
	}
}

void CIRCurve::OnBeforeRowColChange(long OldRow, long OldCol, long NewRow, long NewCol, BOOL FAR* Cancel) 
{
    if(NewRow != OldRow)
        *Cancel = !CurveCanChangeRow();
	
}

void CIRCurve::OnChangeEdit() 
{
    m_bRowChanged = true;
	CurveEdit();	
}

void CIRCurve::OnKeyUp(short FAR* KeyCode, short Shift) 
{
	switch(*KeyCode)
	{
    // escape handling
	case VK_ESCAPE:
		{
           if(((m_State == STATE_EDIT) || (m_State == STATE_NEW)) && (m_vsGrid.GetEditWindow() == 0))
			   CurveUndoEdit();
		}break;
    //   insert handling
	case VK_INSERT:
		{
            if(m_State == STATE_NONE)
				OnAdd();
		}break;
        // delete handling
	case VK_DELETE:
		{
            long nID = ReadLng(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 0));

			if( m_State == STATE_NONE  && nID  != 0 )
                CurveDelete();
		}break;
	case VK_F10:
		if(Shift!=1) break;
	case 93:
		{
			CPoint pt ( m_vsGrid.GetCellLeft(), m_vsGrid.GetCellTop() + m_vsGrid.GetCellHeight());
			
			CDC * pDC = m_vsGrid.GetDC();
			
			int m_nLogX = pDC->GetDeviceCaps (LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps (LOGPIXELSY);

			pt.x *= static_cast<long>((double)m_nLogX /1440.0);
			pt.y *= static_cast<long>(m_nLogY /1440.0);

			
			m_vsGrid.ClientToScreen(&pt);
			
			PopupMenu (pt);
		}break;
	}
}

void CIRCurve::OnStartEdit(long Row, long Col, BOOL FAR* Cancel) 
{
	*Cancel = FALSE;
	if(m_GridDef.m_Col[Col].m_bDisableEdit)
	{
        *Cancel = TRUE;
		return;
	}
	if(0L != long(_variant_t(m_vsGrid.GetCell(5,_variant_t(Row),_variant_t(Col),vtMissing,vtMissing))))
	{
        if(m_vsGrid.GetRow() == Row)
			CurveEdit();
		else
			*Cancel = TRUE;
	}
}

void CIRCurve::OnValidateEdit(long Row, long Col, BOOL FAR* Cancel) 
{
	if (! m_bRowChanged)
		return;

	switch(Col)
	{
	case 1: // ' period
		{
            CString sPeriod;
			long lNumber;
			long lPeriodType;
            if(ParsePeriod(m_vsGrid.GetEditText(), sPeriod, lNumber, lPeriodType))
			{
                m_vsGrid.SetEditText(sPeriod);
				CString str;
				str.Format(_T("%d"),lPeriodType);
                m_vsGrid.SetTextMatrix(Row, 2, str);

				str.Format(_T("%d"),lNumber);
                m_vsGrid.SetTextMatrix(Row, 3, str);

				_variant_t vtDate =  GetDate(vt_date::GetCurrentDate(), lNumber, lPeriodType);
				vtDate.vt = VT_DATE;
				_bstr_t bsDate(vtDate);

                m_vsGrid.SetTextMatrix(Row, 4, (LPCTSTR)bsDate);
			}
			else
                *Cancel = TRUE;
		}break;
	case 5:
	case 6:
		{
			LPTSTR szStop = NULL;
			double f;
			CString S;   
			CString str;

            S = m_vsGrid.GetEditText();
            f = 0.;
            f =  _tcstod(S, &szStop);
            f = fabs(f);
			str.Format(_T("%.2f"),f);
            m_vsGrid.SetEditText(str);
		} break;
	}    
}

bool CIRCurve::CurveCanChangeRow()
{
	bool bRet = true;
	if(m_State == STATE_NEW || m_State == STATE_EDIT)
	{
        bRet = CurveSave();
		/*if(AfxMessageBox(_T("Do you wish to save changes?"), MB_YESNO) == IDYES)
		{
			// saving data
            bRet = CurveSave();
		}
		else
		{
			bRet = CurveUndoEdit();
		}*/

	}
	return bRet;
}

bool CIRCurve::CurveUndoEdit()
{
	long iID = 0;
	long i = 0;
	try
	{

		switch(m_State)
		{
		case STATE_EDIT:
			{
//				if in edit mode then reloading current row
				CString str;
				str = m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 0);
				iID = _ttol(str);
				
				_bstr_t bsFilter = m_GridDef.m_Col[0].m_strName;
				bsFilter += _T("=");
				bsFilter += (LPCTSTR)str;
				m_rsCurve.SetFilter(bsFilter);

				if(m_rsCurve.GetRecordCount() > 0 )
					m_rsCurve.MoveFirst();

				FillRow(m_vsGrid.GetRow());
				SetState(STATE_NONE);
			}
			break;
		case STATE_NEW:
			{
//				 if in new mode then deleting current row
				for(i=0; i<m_GridDef.m_lCols;i++)
				{
					if(!m_GridDef.m_Col[i].m_bDisableEdit)
						m_vsGrid.SetCell(0,
							_variant_t(m_vsGrid.GetRows() - 2L), _variant_t(i),
							_variant_t(m_vsGrid.GetRows() - 1L), _variant_t(i),
							_variant_t(STR_NEWVALUE));
					else
						m_vsGrid.SetTextMatrix (m_vsGrid.GetRows() - 2, i, _T(""));

				}
				m_vsGrid.SetRowHidden(m_vsGrid.GetRows() - 1, TRUE);
				SetState(STATE_NONE);
			}
			break;
		}
	}
	catch(...)
	{
		return false;
	}

	m_bRowChanged = false;	
	return true;
}

bool CIRCurve::CurveSave()
{
	long i = 0;

	bool bRet = false;
	try
	{
		SInterestRate rate;

		rate.m_lID = ReadLng(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 0));
		rate.m_lPeriodID = ReadLng(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 2));
		rate.m_lNumber = ReadLng(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 3));
		rate.m_dbBid = ReadDbl(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 5))/100.0; 
		rate.m_dbAsk  = ReadDbl(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 6))/100.0; 

		if( rate.m_dbBid < 0 || rate.m_dbBid > 1 || rate.m_dbAsk < 0 || rate.m_dbAsk > 1 )
		{
			MessageBox( _T("Invalid bid or ask value."), NULL, MB_OK|MB_ICONSTOP );
			return false;
		}
		
		COleDateTime dtCur    = COleDateTime::GetCurrentTime();
		COleDateTime dtPeriod = GetDate( dtCur, rate.m_lNumber, rate.m_lPeriodID );
  
		CClientRecordset rs;
		m_db.InterestRateCurveGet( 0, rs );
		
		for( ; !rs.IsEOF(); ++rs )
			if( rate.m_lID != (long)rs[L"iCurvePointID"] )
			{
				COleDateTime dtRow = GetDate( dtCur, rs[L"iNumber"] , rs[L"iPeriodTypeID"] );
				if( dtRow == dtPeriod )
				{
					MessageBox( _T("There is rate with the same period."), NULL, MB_OK|MB_ICONSTOP );
					return false;
				}
			}
    
	//   writing
		m_db.InterestRateCurveSave( rate.m_lID, rate.m_lPeriodID, rate.m_lNumber,
								    rate.m_dbBid, rate.m_dbAsk );


	// setting id for a new tntity record
		if( m_State == STATE_NEW )
		{
			CString str;
			str.Format(_T("%d"), rate.m_lID);
			m_vsGrid.SetTextMatrix(m_vsGrid.GetRow(), 0, str);
			m_vsGrid.AddItem(_T(""), vtMissing);
			m_vsGrid.SetRowHidden(m_vsGrid.GetRows() - 1, TRUE);
		}
		SetState(STATE_NONE);
    
	//  reloading the list
		CurveReload();
		bRet = true;
		m_bRatesChanged = true;
		m_bRowChanged = false;
	}
	catch( const _com_error& e )
	{
		TRACE( e.ErrorInfo() ?(LPCSTR) e.Description() : e.ErrorMessage() );
	}

return bRet;    
}

void CIRCurve::CurveReload()
{
	long iCurRow = 0;
	long iTopRow = 0;

//        getting current position
        iCurRow = m_vsGrid.GetRow();
        iTopRow = m_vsGrid.GetTopRow();
//        ' loading data
        CurveLoad();
//        ' setting current position
        m_vsGrid.SetTopRow(iTopRow);
        m_vsGrid.SetRow(iCurRow);

}

void CIRCurve::CurveEdit()
{
	if(m_vsGrid.GetRow() == (m_vsGrid.GetRows() - 2))
	{
        SetState(STATE_NEW);
        m_vsGrid.SetRowHidden(m_vsGrid.GetRows() - 1, FALSE);
    }
	else
	{
        SetState(STATE_EDIT);
	}

}
        
bool CIRCurve::CurveDelete()
{
	long iID = 0;
	try
	{
		if(AfxMessageBox(_T("Do you wish to delete current record?"), MB_YESNO|MB_ICONQUESTION)!=IDYES)
		{
			return false;
		}

	  //      ' reading id
		iID = ReadLng(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 0));
        
	//      if new row then undoing chsnges
		if(iID == 0)
		{
			CurveUndoEdit();
			return true;
		}

	//    deleting item
		m_db.InterestRateCurveDel(iID);

		CurveReload();
		SetState(STATE_NONE);
	}
	catch(...)
	{
		return false;
	}
return true;
}

    
        
bool CIRCurve::ParsePeriod(CString sSource,CString& sPeriod,long& lNumber, long& lPeriodType)
{
	bool bRet = false;
	int N = 0;
	CString S;
    
    sSource.MakeLower();    
	if(sSource.Find(_T("day"))==-1)
        N = sSource.Find(_T('y'));
	else
	{
		N = sSource.Find(_T('d'));
		if(N > 0)
		{
			S = sSource.Left(N);
			S.TrimLeft();
			S.TrimRight();
			lNumber = _ttol(S);
			if(lNumber <= 0)
				return false;
				
			lPeriodType = 1; // day
			sPeriod.Format(_T("%d"), lNumber);
			if(lNumber>1)
				sPeriod += _T(" dayss");
			else
				sPeriod += _T(" day");
			bRet = true;
		}
		return bRet;
	}

    if(N > 0)
	{
		S = sSource.Left(N);
		S.TrimLeft();
		S.TrimRight();
		lNumber = _ttol(S);
		
		if(lNumber <= 0)
			return false;

		lPeriodType = 4; // year
		sPeriod.Format(_T("%d"),lNumber);
		if(lNumber>1)
			sPeriod += _T(" years");
		else
			sPeriod += _T(" year");
		bRet = true;
	}        
	else
	{
        N = sSource.Find(_T('m'));
		if(N > 0)
		{
			S = sSource.Left(N);
			S.TrimLeft();
			S.TrimRight();
			lNumber = _ttol(S);
			if(lNumber <= 0)
				return false;

			lPeriodType = 3; // month
			sPeriod.Format(_T("%d"),lNumber);
			if(lNumber>1)
				sPeriod += _T(" months");
			else
				sPeriod += _T(" month");
			bRet = true;
		}
		else
		{
			N = sSource.Find(_T('w'));
			if(N > 0)
			{
				S = sSource.Left(N);
				S.TrimLeft();
				S.TrimRight();
				lNumber = _ttol(S);
				if(lNumber <= 0)
					return false;


				lPeriodType = 2; // week
				sPeriod.Format(_T("%d"),lNumber);
				if(lNumber>1)
					sPeriod += _T(" weeks");
				else
					sPeriod += _T(" week");
				bRet = true;
			}
			else
			{
				N = sSource.Find(_T('d'));
				if(N > 0)
				{
					S = sSource.Left(N);
					S.TrimLeft();
					S.TrimRight();
					lNumber = _ttol(S);
					if(lNumber <= 0) lNumber = 1;
					lPeriodType = 1; // day
					sPeriod.Format(_T("%d"),lNumber);
					if(lNumber>1)
						sPeriod += _T(" days");
					else
						sPeriod += _T(" day");
					bRet = true;
				}
			}
		}
	}
	return bRet;
}
    

CString CIRCurve::GetDateString(DATE dtStartDate, WORD  lPeriod, long lPeriodType, vt_date& dExp)
{
CString sPeriod;

	switch(lPeriodType)
	{
	case 1: // day
		{
			sPeriod.Format(_T("%d day"),lPeriod);
			dExp = dtStartDate;
			dExp += vt_date_span((long)lPeriod, 0);
		}
		break;
	case 2: // week
		{
			sPeriod.Format(_T("%d week"),lPeriod);
			dExp = dtStartDate;
			dExp += vt_date_span(7L*lPeriod);
		}
		break;
	case 3: // month
		{
			sPeriod.Format(_T("%d month"),lPeriod);
			vt_date curDate(dtStartDate);
			
			WORD month = curDate.get_month();
			WORD year  = curDate.get_year();

			month+=lPeriod;
			while(month>12)
			{
				month -= 12; 
				year ++;
			}
			if(!month) month = 1;
			dExp = vt_date(year, month,curDate.get_day());
		}
		break;
	case 4: // year
		{
			sPeriod.Format(_T("%d year"),lPeriod);
			vt_date curDate(dtStartDate);
			WORD year  = curDate.get_year();
			year+=lPeriod;
			dExp = vt_date(year,curDate.get_month(),curDate.get_day());
		}
		break;

	}

	DATE dtExp = dExp;
	
	dExp = (long) dtExp;

	return sPeriod;
}

DATE CIRCurve::GetDate(DATE dtStartDate,long  lPeriod, long lPeriodType)
{
	vt_date dExp;

	switch(lPeriodType)
	{
	case 1: // day
		{
			dExp = dtStartDate;
			dExp += vt_date_span(lPeriod);
		}
		break;
	case 2: // week
		{
			dExp = dtStartDate;
			dExp += vt_date_span(7L*lPeriod);
		}
		break;
	case 3: // month
		{
			vt_date curDate(dtStartDate);
			
			WORD month = curDate.get_month();
			WORD year  = curDate.get_year();

			month += (WORD)lPeriod;
			while(month>12)
			{
				month -= 12; 
				year ++;
			}
			if(!month) month = 1;
			dExp = vt_date(year,month,curDate.get_day());
		}
		break;
	case 4: // year
		{
			vt_date curDate(dtStartDate);
			WORD year  = curDate.get_year();
			year+=(WORD)lPeriod;
			dExp = vt_date(year,curDate.get_month(),curDate.get_day());
		}
		break;

	}

	DATE dtExp= dExp;
	dtExp = (long) dtExp;

	return dtExp;
}
    

void CIRCurve::OnParentNotify( UINT message, LPARAM lParam )
{

	if(message == WM_RBUTTONDOWN)
	{
		m_lX = LOWORD(lParam);
		m_lY = HIWORD(lParam);	
	}
	CDialog::OnParentNotify( message, lParam );
}

void CIRCurve::OnAdd()
{
    m_vsGrid.SetRow(m_vsGrid.GetRows() - 2);
    m_vsGrid.EditCell();
}
void CIRCurve::OnEdit()
{
   m_vsGrid.EditCell();
}
void CIRCurve::OnDelete()
{
   CurveDelete();
}

void CIRCurve::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
{
	if (*KeyCode == VK_ESCAPE )
	{
		if(((m_State == STATE_EDIT) || (m_State == STATE_NEW)) && (m_vsGrid.GetEditWindow() == 0))
		   CurveUndoEdit();
		else
			PostMessage (WM_COMMAND, IDCANCEL);
	}
}

void CIRCurve::OnClose() 
{
	CurveCanChangeRow();
	
	CDialog::OnClose();
}

void CIRCurve::OnCancel() 
{
	CurveCanChangeRow();

	CDialog::OnCancel();
}

void CIRCurve::PopupMenu(CPoint pt)
{
	BCMenu * pMenu = (BCMenu*) m_mnuMain.GetSubMenu(0);
	if(pMenu)
	{
        long nID = ReadLng(m_vsGrid.GetTextMatrix(m_vsGrid.GetRow(), 0));

		pMenu->EnableMenuItem (ID_IR_DELETE, ( nID == 0 ? MF_GRAYED : MF_ENABLED) | MF_BYCOMMAND);
		pMenu->EnableMenuItem (ID_IR_EDIT, ( nID == 0 ? MF_GRAYED : MF_ENABLED) | MF_BYCOMMAND);
		
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,pt.x, pt.y, this);
	}	
}
