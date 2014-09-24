// EtsFixManagerView.cpp : implementation of the CEtsFixManagerView class
//

#include "stdafx.h"
#include "EtsFixManager.h"
#include "EtsFixManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum RptGridColumnsEnum
{
	enRpcNone		= -1,

	enRpcType,
	enRpcTime,
	enRpcMsg,

	enRpcLastColCount,
	enRpcLastCol = enRpcLastColCount - 1
};


// CEtsFixManagerView

IMPLEMENT_DYNCREATE(CEtsFixManagerView, CFormView)

BEGIN_MESSAGE_MAP(CEtsFixManagerView, CFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_FIX_NEW_RECORD, OnFixNewRecord)
	ON_COMMAND(ID_VIEW_SHOW_COLOR_CODES, OnViewShowColorCodes)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_COLOR_CODES, OnUpdateViewShowColorCodes)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_EVENTSINK_MAP(CEtsFixManagerView, CFormView)
END_EVENTSINK_MAP()

// CEtsFixManagerView construction/destruction

CEtsFixManagerView::CEtsFixManagerView()
	: CFormView(CEtsFixManagerView::IDD),
	m_pFgRptLock(NULL),
	m_nShowColorCodes(0)
{
	m_clrDefRecColorBk = ::GetSysColor(COLOR_WINDOW);
	m_clrDefRecColorFg = ::GetSysColor(COLOR_WINDOWTEXT);
}

CEtsFixManagerView::~CEtsFixManagerView()
{
}

BOOL CEtsFixManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFormView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsFixManagerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


/////////////////////////////////////////////////////////////////////////////
//
void CEtsFixManagerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_LOENGLISH, CSize(0, 0));		// prevent view's scrollbars appearence

	m_spFgRpt = GetDlgItem(IDC_FG_REPORT)->GetControlUnknown();
	
	if(m_pFgRptLock)
		delete m_pFgRptLock;

	m_pFgRptLock = new CGridLock(m_spFgRpt);
	m_nShowColorCodes = g_App.GetProfileInt(_T("Settings"), _T("ShowColorCodes"), 0) ? 1 : 0;

	_InitLogGrid();
	_InitLogColumns();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsFixManagerView::_InitLogGrid()
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
void CEtsFixManagerView::_InitLogColumns()
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		long nCol;

		m_spFgRpt->Rows = 1L;
		m_spFgRpt->Cols = enRpcLastColCount;

		nCol = enRpcType;
		m_spFgRpt->PutColHidden(nCol, VARIANT_FALSE);
		m_spFgRpt->PutColDataType(nCol, flexDTString);
		m_spFgRpt->PutColFormat(nCol, L"");
		m_spFgRpt->PutTextMatrix(0, nCol, L"");

		nCol = enRpcTime;
		m_spFgRpt->PutColHidden(nCol, VARIANT_FALSE);
		m_spFgRpt->PutColDataType(nCol, flexDTDate);
		m_spFgRpt->PutColFormat(nCol, L"");
		m_spFgRpt->PutTextMatrix(0, nCol, L"Time");

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
void CEtsFixManagerView::_AddNewRecord(long nRecIdx)
{
	try
	{
		CEtsFixManagerDoc* pDoc = GetDocument();
		ASSERT(pDoc);

		const CFixReportRec& aRec = pDoc->GetRecord(nRecIdx);
		CString strTemp;
		long nRow = 1L;
		m_spFgRpt->AddItem(L"", nRow);

		m_spFgRpt->PutCell(flexcpData, nRow, enRpcType, nRow, enRpcType, aRec.m_nType);

		m_spFgRpt->PutTextMatrix(nRow, enRpcTime, (LPCTSTR)aRec.m_dtTime.Format((DWORD)LOCALE_NOUSEROVERRIDE));
		m_spFgRpt->PutTextMatrix(nRow, enRpcMsg, (LPCTSTR)aRec.m_strMessage);

		_ShowRecordTypeImage(nRow, aRec);

		if(m_nShowColorCodes)
		{
			COLORREF clrRecordBkg = aRec.GetTypeBackColor();
			if(clrRecordBkg > 0)
			{
				m_spFgRpt->PutCell(flexcpBackColor, nRow, enRpcType, nRow, enRpcMsg, clrRecordBkg);
				m_spFgRpt->PutCell(flexcpForeColor, nRow, enRpcType, nRow, enRpcMsg, aRec.GetTypeForeColor());
			}
		}

		if(m_spFgRpt->Rows == 2L)
		{
			m_spFgRpt->AutoSize(enRpcType, enRpcTime, VARIANT_FALSE, 0L);
			AdjustLastColWidth();
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
void CEtsFixManagerView::_UpdateRecordColors()
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		long nRows = m_spFgRpt->Rows;
		for(long nRow = 1L; nRow < nRows; nRow++)
		{
			long nType = m_spFgRpt->GetCell(flexcpData, nRow, enRpcType, nRow, enRpcType);
			if(nType > 0 && m_nShowColorCodes)
			{
				COLORREF clrRecordBkg = CFixReportRec::GetTypeBackColor(nType);
				if(clrRecordBkg > 0)
				{
					m_spFgRpt->PutCell(flexcpBackColor, nRow, enRpcType, nRow, enRpcMsg, clrRecordBkg);
					m_spFgRpt->PutCell(flexcpForeColor, nRow, enRpcType, nRow, enRpcMsg, CFixReportRec::GetTypeForeColor(nType));
				}
			}
			else
			{
				m_spFgRpt->PutCell(flexcpBackColor, nRow, enRpcType, nRow, enRpcMsg, m_clrDefRecColorBk);
				m_spFgRpt->PutCell(flexcpForeColor, nRow, enRpcType, nRow, enRpcMsg, m_clrDefRecColorFg);
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
void CEtsFixManagerView::_ShowRecordTypeImage(long nRow, const CFixReportRec& aRec)
{
	try
	{
		long nID = aRec.GetTypeIconResID();
		if(nID)
		{
			m_spFgRpt->PutCell(flexcpPicture, nRow, enRpcType, nRow, enRpcType, g_App.GetTypeIcon(nID));
			m_spFgRpt->PutCell(flexcpPictureAlignment, nRow, enRpcType, nRow, enRpcType, flexPicAlignCenterCenter);
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
LRESULT CEtsFixManagerView::OnFixNewRecord(WPARAM wParam, LPARAM lParam)
{
	_AddNewRecord((long)wParam);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsFixManagerView::_AdjustLayout()
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
void CEtsFixManagerView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if(!(nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW || nType == SIZE_MINIMIZED))
		_AdjustLayout();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEtsFixManagerView::OnDestroy()
{
	CFormView::OnDestroy();

	if(m_pFgRptLock)
		delete m_pFgRptLock;

	m_spFgRpt = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
#ifdef _DEBUG
void CEtsFixManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CEtsFixManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEtsFixManagerDoc* CEtsFixManagerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEtsFixManagerDoc)));
	return (CEtsFixManagerDoc*)m_pDocument;
}
#endif //_DEBUG

void CEtsFixManagerView::AdjustLastColWidth()
{
	try
	{
		CGridAutoLock lock(m_pFgRptLock);
		CRect rcClient, rc;
		GetClientRect(rcClient);
		long nWidth = (rcClient.Width() - ::GetSystemMetrics(SM_CXVSCROLL) * 2) * 15
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

void CEtsFixManagerView::OnViewShowColorCodes()
{
	m_nShowColorCodes = m_nShowColorCodes ? 0 : 1;
	g_App.WriteProfileInt(_T("Settings"), _T("ShowColorCodes"), m_nShowColorCodes);
	_UpdateRecordColors();
}

void CEtsFixManagerView::OnUpdateViewShowColorCodes(CCmdUI *pCmdUI)
{
	if(pCmdUI)
		pCmdUI->SetCheck(m_nShowColorCodes);
}
