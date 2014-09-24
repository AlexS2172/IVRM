// EtsEodManagerView.cpp : implementation of the CEtsEodManagerView class
//

#include "stdafx.h"
#include "EtsEodManager.h"
#include "EtsEodManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum RptGridColumnsEnum
{
	enRpcNone		= -1,

	enRpcKey		= 0,
	enRpcStep,
	enRpcType,
	enRpcTime,
	enRpcMsg,

	enRpcLastColCount,
	enRpcLastCol = enRpcLastColCount - 1
};

/////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNCREATE(CEtsEodManagerView, CFormView)

BEGIN_MESSAGE_MAP(CEtsEodManagerView, CFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_EOD_NEW_RECORD, OnEodNewRecord)
	ON_MESSAGE(WM_EOD_STEP_PROGRESS, OnEodNewStepProgress)
	ON_COMMAND(ID_VIEW_EXPAND_ALL, OnViewExpandAll)
	ON_COMMAND(ID_VIEW_COLLAPSE_ALL, OnViewCollapseAll)
	ON_COMMAND(ID_VIEW_SHOW_COLOR_CODES, OnViewShowColorCodes)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_COLOR_CODES, OnUpdateViewShowColorCodes)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_EVENTSINK_MAP(CEtsEodManagerView, CFormView)
	ON_EVENT(CEtsEodManagerView, IDC_FG_REPORT, DISPID_KEYPRESS, OnFgReportKeyPress, VTS_PI2)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CEtsEodManagerView::CEtsEodManagerView()
	: CFormView(CEtsEodManagerView::IDD),
	m_pFgRptLock(NULL),
	m_clrStepColor(RGB(235, 235, 235)),
	m_nShowColorCodes(0)
{
	m_clrDefRecColorBk = ::GetSysColor(COLOR_WINDOW);
	m_clrDefRecColorFg = ::GetSysColor(COLOR_WINDOWTEXT);
}

/////////////////////////////////////////////////////////////////////////////
//
CEtsEodManagerView::~CEtsEodManagerView()
{
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEtsEodManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFormView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_LOENGLISH, CSize(0, 0));		// prevent view's scrollbars appearence

	m_spFgRpt = GetDlgItem(IDC_FG_REPORT)->GetControlUnknown();
	
	if(m_pFgRptLock)
		delete m_pFgRptLock;

	m_pFgRptLock = new CGridLock(m_spFgRpt);

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	long nVal = 0;
	XMLParams.GetXMLLong(_T("ETS\\ETSEndofDayManager\\Settings"), _T("ShowColorCodes"), &nVal, 0);
	m_nShowColorCodes = nVal ? 1 : 0;

	_InitLogGrid();
	_InitLogColumns();
	_ShowReport();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_InitLogGrid()
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		m_spFgRpt->AllowBigSelection = VARIANT_FALSE;
		m_spFgRpt->AllowSelection = VARIANT_FALSE;
		m_spFgRpt->AllowUserFreezing = flexFreezeNone;
		m_spFgRpt->AllowUserResizing = flexResizeColumns;
		m_spFgRpt->Appearance = flex3DLight;
		m_spFgRpt->AutoSizeMode = flexAutoSizeColWidth;
		m_spFgRpt->AutoSearch = flexSearchNone;
		m_spFgRpt->AutoSizeMouse = VARIANT_TRUE;
		m_spFgRpt->Editable = flexEDNone;
		m_spFgRpt->Ellipsis = flexEllipsisEnd;
		m_spFgRpt->ExplorerBar = flexExNone;
		m_spFgRpt->ExtendLastCol = VARIANT_FALSE;
		m_spFgRpt->FixedCols = 0L;
		m_spFgRpt->FixedRows = 1L;
		m_spFgRpt->FocusRect = flexFocusSolid;
		m_spFgRpt->FrozenCols = 0L;
		m_spFgRpt->FrozenRows = 0L;
		m_spFgRpt->OutlineBar = flexOutlineBarSymbolsLeaf;
		m_spFgRpt->OutlineCol = 1;
		m_spFgRpt->MergeCells = flexMergeFree;
		m_spFgRpt->MergeCompare = flexMCExact;
		m_spFgRpt->ScrollBars = flexScrollBarBoth;
		m_spFgRpt->SelectionMode = flexSelectionFree;
		m_spFgRpt->TabBehavior = flexTabControls;
		m_spFgRpt->HighLight = flexHighlightWithFocus;
		m_spFgRpt->ScrollTrack = VARIANT_TRUE;
		m_spFgRpt->ColWidthMin = 150L;
		m_spFgRpt->FloodColor = RGB(0, 235, 0);
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_InitLogColumns()
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		long nCol;

		m_spFgRpt->Rows = 1L;
		m_spFgRpt->Cols = enRpcLastColCount;

		nCol = enRpcKey;
		m_spFgRpt->PutColHidden(nCol, VARIANT_TRUE);
		m_spFgRpt->PutColDataType(nCol, flexDTString);

		nCol = enRpcStep;
		m_spFgRpt->PutColHidden(nCol, VARIANT_FALSE);
		m_spFgRpt->PutColDataType(nCol, flexDTString);
		m_spFgRpt->PutColFormat(nCol, L"");
		m_spFgRpt->PutTextMatrix(0, nCol, L"Step");

		nCol = enRpcType;
		m_spFgRpt->PutColHidden(nCol, VARIANT_FALSE);
		m_spFgRpt->PutColDataType(nCol, flexDTString);
		m_spFgRpt->PutColFormat(nCol, L"");
		m_spFgRpt->PutTextMatrix(0, nCol, L"");

		nCol = enRpcTime;
		m_spFgRpt->PutColHidden(nCol, VARIANT_FALSE);
		m_spFgRpt->PutColDataType(nCol, flexDTDate);
		m_spFgRpt->PutColFormat(nCol, L"");
		m_spFgRpt->PutTextMatrix(0, nCol, L"Time/Completed");

		nCol = enRpcMsg;
		m_spFgRpt->PutColHidden(nCol, VARIANT_FALSE);
		m_spFgRpt->PutColDataType(nCol, flexDTString);
		m_spFgRpt->PutColFormat(nCol, L"");
		m_spFgRpt->PutTextMatrix(0, nCol, L"Message");

		m_spFgRpt->PutCell(flexcpAlignment, 0, 0, 0, m_spFgRpt->Cols - 1, flexAlignCenterCenter);

		//m_clrStepColor = m_spFgRpt->GridColor;
		m_clrDefRecColorBk = m_spFgRpt->BackColor;
		m_clrDefRecColorFg = m_spFgRpt->ForeColor;

		HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_TYPE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

		CPictureHolder pic;
		pic.CreateFromIcon(hIcon);
		_variant_t varPic(pic.GetPictureDispatch());
		
		m_spFgRpt->PutCell(flexcpPicture, 0L, enRpcType, 0L, enRpcType, varPic);
		m_spFgRpt->PutCell(flexcpPictureAlignment, 0L, enRpcType, 0L, enRpcType, flexPicAlignCenterCenter);

		if(hIcon)
			::DestroyIcon(hIcon);
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_ShowReport()
{
	try
	{
		CWaitCursor wait;

		CEtsEodManagerDoc* pDoc = GetDocument();
		ASSERT(pDoc);

		const CEodReportStepMap& mapRecSteps = pDoc->GetSteps();

		CGridAutoLock lock(m_pFgRptLock);
		m_spFgRpt->Rows = 1L;
		m_spFgRpt->PutMergeCol(enRpcStep, VARIANT_TRUE);

		CEodReportStepMap::const_iterator itRecStep = mapRecSteps.begin();
		for(; itRecStep != mapRecSteps.end(); itRecStep++)
			_AddReportStep(m_spFgRpt->Rows, itRecStep->first, itRecStep->second);

		m_spFgRpt->AutoSize(enRpcStep, enRpcTime, VARIANT_FALSE, 0L);
		AdjustLastColWidth();
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_AddReportStep(long nStepRow, long nStep, const CEodReportStep& aStep)
{
	try
	{
		CString strTemp;
		const CEodReportRecVec& vecRecs = aStep.GetRecs();

		m_spFgRpt->AddItem(L"", nStepRow);

		m_spFgRpt->PutIsSubtotal(nStepRow, VARIANT_TRUE);
		m_spFgRpt->PutRowOutlineLevel(nStepRow, 1L);

		strTemp.Format(_T("%d"), nStep);
		m_spFgRpt->PutTextMatrix(nStepRow, enRpcKey, (LPCTSTR)strTemp);

		m_spFgRpt->PutCell(flexcpFloodPercent, nStepRow, enRpcTime, nStepRow, enRpcTime, aStep.GetProgress());
		strTemp.Format(_T("%d%%"), aStep.GetProgress());
		m_spFgRpt->PutTextMatrix(nStepRow, enRpcTime, (LPCTSTR)strTemp);
		m_spFgRpt->PutCell(flexcpAlignment, nStepRow, enRpcTime, nStepRow, enRpcTime, flexAlignCenterCenter);
		m_spFgRpt->PutCell(flexcpFontBold, nStepRow, enRpcStep, nStepRow, enRpcMsg, VARIANT_TRUE);

		CEodReportRecVec::const_iterator itRec = vecRecs.begin();
		for(; itRec != vecRecs.end(); itRec++)
			_AddRecord(nStepRow, m_spFgRpt->Rows, *itRec);

		strTemp = CEodReportStep::StepDescription(nStep);
		m_spFgRpt->PutCell(flexcpText, nStepRow, enRpcStep, m_spFgRpt->Rows - 1L, enRpcStep, (LPCTSTR)strTemp);

		if(vecRecs.size() > 0)
			strTemp = vecRecs[vecRecs.size() - 1].m_strMessage;
		else
			strTemp = _T("");

		m_spFgRpt->PutTextMatrix(nStepRow, enRpcMsg, (LPCTSTR)strTemp);
		m_spFgRpt->PutIsCollapsed(nStepRow, flexOutlineCollapsed);
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_AddRecord(long nStepRow, long nRow, const CEodReportRec& aRec)
{
	try
	{
		CString strTemp;
		m_spFgRpt->AddItem(L"", nRow);

		m_spFgRpt->PutIsSubtotal(nRow, VARIANT_FALSE);
		m_spFgRpt->PutRowOutlineLevel(nRow, 2L);
		m_spFgRpt->PutCell(flexcpData, nRow, enRpcType, nRow, enRpcType, aRec.m_nType);

		m_spFgRpt->PutTextMatrix(nRow, enRpcKey, L"");
		m_spFgRpt->PutTextMatrix(nRow, enRpcTime, (LPCTSTR)aRec.m_dtTime.Format((DWORD)LOCALE_NOUSEROVERRIDE));
		m_spFgRpt->PutTextMatrix(nRow, enRpcMsg, (LPCTSTR)aRec.m_strMessage);

		_ShowRecordTypeImage(nStepRow, nRow, aRec);

		if(m_nShowColorCodes)
		{
			COLORREF clrRecordBkg = aRec.GetTypeBackColor();
			if(clrRecordBkg > 0)
			{
				m_spFgRpt->PutCell(flexcpBackColor, nRow, enRpcType, nRow, enRpcMsg, clrRecordBkg);
				m_spFgRpt->PutCell(flexcpForeColor, nRow, enRpcType, nRow, enRpcMsg, aRec.GetTypeForeColor());
			}
		}

		m_spFgRpt->PutCell(flexcpBackColor, nRow, enRpcStep, nRow, enRpcStep, m_clrStepColor);
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_UpdateRecordColors()
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		long nRows = m_spFgRpt->Rows;
		for(long nRow = 1L; nRow < nRows; nRow++)
		{
			if(m_spFgRpt->GetRowOutlineLevel(nRow) > 1L)
			{
				long nType = m_spFgRpt->GetCell(flexcpData, nRow, enRpcType, nRow, enRpcType);
				if(nType > 0 && m_nShowColorCodes)
				{
					COLORREF clrRecordBkg = CEodReportRec::GetTypeBackColor(nType);
					if(clrRecordBkg > 0)
					{
						m_spFgRpt->PutCell(flexcpBackColor, nRow, enRpcType, nRow, enRpcMsg, clrRecordBkg);
						m_spFgRpt->PutCell(flexcpForeColor, nRow, enRpcType, nRow, enRpcMsg, CEodReportRec::GetTypeForeColor(nType));
					}
				}
				else
				{
					m_spFgRpt->PutCell(flexcpBackColor, nRow, enRpcType, nRow, enRpcMsg, m_clrDefRecColorBk);
					m_spFgRpt->PutCell(flexcpForeColor, nRow, enRpcType, nRow, enRpcMsg, m_clrDefRecColorFg);
				}
			}
		}
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_ShowRecordTypeImage(long nStepRow, long nRow, const CEodReportRec& aRec)
{
	try
	{
		long nID = aRec.GetTypeIconResID();
		if(nID)
		{
			m_spFgRpt->PutCell(flexcpPicture, nRow, enRpcType, nRow, enRpcType, g_App.GetTypeIcon(nID));
			m_spFgRpt->PutCell(flexcpPictureAlignment, nRow, enRpcType, nRow, enRpcType, flexPicAlignCenterCenter);
			
			if(aRec.m_nType == enRpMtStart || aRec.m_nType == enRpMtFinishSuccess || aRec.m_nType == enRpMtFinishWithError)
			{
				m_spFgRpt->PutCell(flexcpPicture, nStepRow, enRpcType, nStepRow, enRpcType, g_App.GetTypeIcon(nID));
				m_spFgRpt->PutCell(flexcpPictureAlignment, nStepRow, enRpcType, nStepRow, enRpcType, flexPicAlignCenterCenter);
			}
		}
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_AddNewRecord(long nStep, long nRecIdx)
{
	try
	{
		CEtsEodManagerDoc* pDoc = GetDocument();
		ASSERT(pDoc);

		const CEodReportStepMap& mapRecSteps = pDoc->GetSteps();
		CEodReportStepMap::const_iterator itRecStep = mapRecSteps.find(nStep);

		ASSERT(itRecStep != mapRecSteps.end());

		if(itRecStep != mapRecSteps.end())
		{
			const CEodReportStep& aStep = itRecStep->second;

			CGridAutoLock lock(m_pFgRptLock);
			CString strTemp;

			strTemp.Format(_T("%d"), nStep);
			long nStepRow = m_spFgRpt->GetFindRow((LPCTSTR)strTemp, 1L, enRpcKey);
			if(nStepRow < 1)
				_AddReportStep(m_spFgRpt->Rows, nStep, aStep);
			else
			{
				const CEodReportRecVec& vecRecs = aStep.GetRecs();

				ASSERT(nRecIdx >= 0L && nRecIdx < (long)vecRecs.size());

				if(nRecIdx >= 0L && nRecIdx < (long)vecRecs.size())
				{
					long nOldRow = m_spFgRpt->Row;

					long nRow = nStepRow + 1L, nRows = m_spFgRpt->Rows;
					if(nRow < nRows)
					{
						for(; nRow < nRows; nRow++)
						{
							if(m_spFgRpt->GetRowOutlineLevel(nRow) == 1L)
								break;
						}
					}

					{
						CAutoLock lock(pDoc->GetLock());
						const CEodReportRec& aRec = vecRecs[nRecIdx];
						_AddRecord(nStepRow, nRow, aRec);

						CString strStep(CEodReportStep::StepDescription(nStep));
						m_spFgRpt->PutTextMatrix(nRow, enRpcStep, (LPCTSTR)strStep);
						m_spFgRpt->PutTextMatrix(nStepRow, enRpcMsg, (LPCTSTR)aRec.m_strMessage);
					}

					if(nRow > nStepRow + 1L)
					{
						if(m_spFgRpt->GetIsCollapsed(nStepRow) == flexOutlineCollapsed)
							m_spFgRpt->PutRowHidden(nRow, VARIANT_TRUE);
					}
					else
						m_spFgRpt->PutIsCollapsed(nStepRow, flexOutlineCollapsed);

					if(nRow <= nOldRow)
						m_spFgRpt->Row = nOldRow + 1L;
				}
			}
		}
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_ShowStepProgress(long nStep)
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		CString strTemp;

		CEtsEodManagerDoc* pDoc = GetDocument();
		ASSERT(pDoc);

		const CEodReportStepMap& mapRecSteps = pDoc->GetSteps();
		CEodReportStepMap::const_iterator itRecStep = mapRecSteps.find(nStep);

		ASSERT(itRecStep != mapRecSteps.end());

		if(itRecStep != mapRecSteps.end())
		{
			const CEodReportStep& aStep = itRecStep->second;

			strTemp.Format(_T("%d"), nStep);
			long nStepRow = m_spFgRpt->GetFindRow((LPCTSTR)strTemp, 1L, enRpcKey);
			if(nStepRow < 1)
				_AddReportStep(m_spFgRpt->Rows, nStep, aStep);
			else
			{
				m_spFgRpt->PutCell(flexcpFloodPercent, nStepRow, enRpcTime, nStepRow, enRpcTime, aStep.GetProgress());
				strTemp.Format(_T("%d%%"), aStep.GetProgress());
				m_spFgRpt->PutTextMatrix(nStepRow, enRpcTime, (LPCTSTR)strTemp);
			}
		}
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CEtsEodManagerView::OnEodNewRecord(WPARAM wParam, LPARAM lParam)
{
	_AddNewRecord((long)wParam, (long)lParam);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CEtsEodManagerView::OnEodNewStepProgress(WPARAM wParam, LPARAM lParam)
{
	_ShowStepProgress((long)wParam);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::OnFgReportKeyPress(short* KeyAscii)
{
	long nRow = m_spFgRpt->Row;
	if(m_spFgRpt->GetRowOutlineLevel(nRow) == 1)
	{
		ASSERT(KeyAscii);
		if('+' == *KeyAscii && m_spFgRpt->GetIsCollapsed(nRow) == flexOutlineCollapsed)
			m_spFgRpt->PutIsCollapsed(nRow, flexOutlineExpanded);
		else if('-' == *KeyAscii && m_spFgRpt->GetIsCollapsed(nRow) == flexOutlineExpanded)
			m_spFgRpt->PutIsCollapsed(nRow, flexOutlineCollapsed);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::_AdjustLayout()
{
	if(NULL != m_spFgRpt)
	{
		CRect rcClient, rc;
		
		GetClientRect(rcClient);

		rc.left = 0L;
		rc.top = 0L;
		rc.right = rcClient.Width();
		rc.bottom = rcClient.Height();
		GetDlgItem(IDC_FG_REPORT)->MoveWindow(rc, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	if(!(nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW || nType == SIZE_MINIMIZED))
		_AdjustLayout();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsEodManagerView::OnDestroy()
{
	CFormView::OnDestroy();

	if(m_pFgRptLock)
		delete m_pFgRptLock;

	m_spFgRpt = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
#ifdef _DEBUG
void CEtsEodManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CEtsEodManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEtsEodManagerDoc* CEtsEodManagerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEtsEodManagerDoc)));
	return (CEtsEodManagerDoc*)m_pDocument;
}
#endif //_DEBUG

void CEtsEodManagerView::AdjustLastColWidth()
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		CRect rcClient, rc;
		GetClientRect(rcClient);
		long nWidth = (rcClient.Width() - ::GetSystemMetrics(SM_CXVSCROLL) * 2) * 15
									- m_spFgRpt->GetColWidth(enRpcStep)
									- m_spFgRpt->GetColWidth(enRpcType)
									- m_spFgRpt->GetColWidth(enRpcTime);

		if(nWidth > 0)
			m_spFgRpt->PutColWidth(enRpcMsg, nWidth);
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

void CEtsEodManagerView::OnViewExpandAll()
{
	_ExpandCollapseAll(flexOutlineExpanded);
}

void CEtsEodManagerView::OnViewCollapseAll()
{
	_ExpandCollapseAll(flexOutlineCollapsed);
}

void CEtsEodManagerView::_ExpandCollapseAll(CollapsedSettings enCollapse)
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		long nRows = m_spFgRpt->Rows;
		for(long nRow = 1L; nRow < nRows; nRow++)
		{
			if(m_spFgRpt->GetRowOutlineLevel(nRow) == 1L)
				m_spFgRpt->PutIsCollapsed(nRow, enCollapse);
		}
	}
#ifdef _DEBUG
	catch(_com_error& e)
#else
	catch(_com_error&)
#endif
	{
		TRACE2(_T("%s: %s\n"), __FUNCTION__, (LPCTSTR)e.Description());
	}
}

void CEtsEodManagerView::OnViewShowColorCodes()
{
	m_nShowColorCodes = m_nShowColorCodes ? 0 : 1;

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();
	XMLParams.SetXMLLong(_T("ETS\\ETSEndofDayManager\\Settings"), _T("ShowColorCodes"), m_nShowColorCodes);
	XMLParams.SaveXMLParams();

	_UpdateRecordColors();
}

void CEtsEodManagerView::OnUpdateViewShowColorCodes(CCmdUI *pCmdUI)
{
	if(pCmdUI)
		pCmdUI->SetCheck(m_nShowColorCodes);
}

