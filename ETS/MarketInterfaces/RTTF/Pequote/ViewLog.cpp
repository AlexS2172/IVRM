//**************************************************************************************************************//
// solution:	Pequote
// project:		Pequote
// filename:	ViewLog.cpp
// created:		26-Dec-2002 15:27 by Suchkov Dmitry
//
// purpose:		implementation of CViewLog
//
//**************************************************************************************************************//

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "stdafx.h"
#include "Pequote.h"
#include "ViewLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CViewLog, CListCtrl)
	//{{AFX_MSG_MAP(CViewLog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------------------------------------//
CViewLog::CViewLog()
{
}

//--------------------------------------------------------------------------------------------------------------//
CViewLog::~CViewLog()
{
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CViewLog::
WriteProfileInt(LPCTSTR lpszEntry, int nValue)
{
	BOOL bOk = FALSE;
	CWinApp* pApp = AfxGetApp();
	if (pApp) bOk = pApp->WriteProfileInt(REG_SECTION_LOG, lpszEntry, nValue);
	return bOk;
}

//--------------------------------------------------------------------------------------------------------------//
UINT CViewLog::GetProfileInt(LPCTSTR lpszEntry, int nDefault)
{
	UINT nResult = nDefault;
	CWinApp* pApp = AfxGetApp();
	if (pApp) nResult = pApp->GetProfileInt(REG_SECTION_LOG, lpszEntry, nDefault);
	return nResult;
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CViewLog::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style |= WS_VISIBLE | WS_CHILD | LVS_REPORT;

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------//
int CViewLog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ilType.Create(MAKEINTRESOURCE(IDB_LOG_TYPE), 16, 3, RGB(0xFF,0x00,0xFF));
	SetImageList(&m_ilType, LVSIL_SMALL);

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	long nTypeW = COLUMN_TYPE_W;
	long nDateW = COLUMN_DATE_W;
	long nTimeW = COLUMN_TIME_W;
	long nDescW = COLUMN_DESC_W;
	long nSourceW = COLUMN_SOURCE_W;

	XMLParams.GetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_TYPE, &nTypeW, COLUMN_TYPE_W);
	XMLParams.GetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_DATE, &nDateW, COLUMN_DATE_W);
	XMLParams.GetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_TIME, &nTimeW, COLUMN_TIME_W);
	XMLParams.GetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_DESC, &nDescW, COLUMN_DESC_W);
	XMLParams.GetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_SOURCE, &nSourceW, COLUMN_SOURCE_W);

	int i = 0;
	InsertColumn(i++, COLUMN_TYPE,   LVCFMT_LEFT, nTypeW);
	InsertColumn(i++, COLUMN_DATE,   LVCFMT_LEFT, nDateW);
	InsertColumn(i++, COLUMN_TIME,   LVCFMT_LEFT, nTimeW);
	InsertColumn(i++, COLUMN_DESC,   LVCFMT_LEFT, nDescW);
	InsertColumn(i++, COLUMN_SOURCE, LVCFMT_LEFT, nSourceW);

	//InsertColumn(i++, COLUMN_TYPE,   LVCFMT_LEFT, GetProfileInt(COLUMN_TYPE,   COLUMN_TYPE_W));
	//InsertColumn(i++, COLUMN_DATE,   LVCFMT_LEFT, GetProfileInt(COLUMN_DATE,   COLUMN_DATE_W));
	//InsertColumn(i++, COLUMN_TIME,   LVCFMT_LEFT, GetProfileInt(COLUMN_TIME,   COLUMN_TIME_W));
	//InsertColumn(i++, COLUMN_DESC,   LVCFMT_LEFT, GetProfileInt(COLUMN_DESC,   COLUMN_DESC_W));
	//InsertColumn(i++, COLUMN_SOURCE, LVCFMT_LEFT, GetProfileInt(COLUMN_SOURCE, COLUMN_SOURCE_W));

	return 0;
}

//--------------------------------------------------------------------------------------------------------------//
void CViewLog::OnDestroy() 
{
	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	XMLParams.SetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_TYPE, GetColumnWidth(COLUMN_TYPE_ID));
	XMLParams.SetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_DATE, GetColumnWidth(COLUMN_DATE_ID));
	XMLParams.SetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_TIME, GetColumnWidth(COLUMN_TIME_ID));
	XMLParams.SetXMLLong(_T("ETS\\Pequote\\Log"), COLUMN_DESC, GetColumnWidth(COLUMN_DESC_ID));

	XMLParams.SaveXMLParams();

	//WriteProfileInt(COLUMN_TYPE, GetColumnWidth(COLUMN_TYPE_ID));
	//WriteProfileInt(COLUMN_DATE, GetColumnWidth(COLUMN_DATE_ID));
	//WriteProfileInt(COLUMN_TIME, GetColumnWidth(COLUMN_TIME_ID));
	//WriteProfileInt(COLUMN_DESC, GetColumnWidth(COLUMN_DESC_ID));
	
	CListCtrl::OnDestroy();
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CViewLog::InsertTraceMessage(CTracer::MessageTypeEnum Type, sMessage* pMessage)
{
	ASSERT(pMessage);

	int iItem = -1;
	int iImage = -1;
	LPCTSTR pszType = NULL;

	switch (Type)
	{
		case CTracer::enMtInformation:
			pszType = LOG_INFORMATION;
			iImage = 0;
			break;

		case CTracer::enMtWarning:
			pszType = LOG_WARNING;
			iImage = 1;
			break;

		case CTracer::enMtError: 
			pszType = LOG_ERROR;
			iImage = 2;
			break;

		default:
			TRACE("Invalid log entry type %d", Type);
	}

	iItem = InsertItem(0, pszType ? pszType : _T(""), iImage);
	if (-1 != iItem) 
	{
		TCHAR szDate[64];
		if (0 == ::GetDateFormat(
			LOCALE_USER_DEFAULT, 
			DATE_SHORTDATE, 
			NULL, 
			NULL, 
			szDate, 
			sizeof(szDate)/sizeof(TCHAR)))
		{
			szDate[0] = _T('?');
			szDate[1] = _T('\0');
		}

		TCHAR szTime[32];
		if (0 == ::GetTimeFormat(
			LOCALE_USER_DEFAULT,
			LOCALE_NOUSEROVERRIDE,
			NULL,
			NULL,
			szTime,
			sizeof(szTime)/sizeof(TCHAR)))
		{
			szTime[0] = _T('?');
			szTime[1] = _T('\0');
		}

		SetItemText(iItem, 1, szDate);
		SetItemText(iItem, 2, szTime);
		SetItemText(iItem, 3, pMessage->szMessage);
		if (pMessage->szSource) {
			SetItemText(iItem, 4, pMessage->szSource);
		}
	}
	
	return (-1 != iItem);
}