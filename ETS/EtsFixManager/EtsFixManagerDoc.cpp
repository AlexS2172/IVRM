// EtsFixManagerDoc.cpp : implementation of the CEtsFixManagerDoc class
//

#include "stdafx.h"
#include "EtsFixManager.h"

#include "EtsFixManagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEtsFixManagerDoc

IMPLEMENT_DYNCREATE(CEtsFixManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CEtsFixManagerDoc, CDocument)
END_MESSAGE_MAP()


// CEtsFixManagerDoc construction/destruction

CEtsFixManagerDoc::CEtsFixManagerDoc()
{
	// TODO: add one-time construction code here

}

CEtsFixManagerDoc::~CEtsFixManagerDoc()
{
}

BOOL CEtsFixManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CEtsFixManagerDoc diagnostics

#ifdef _DEBUG
void CEtsFixManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEtsFixManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


const CFixReportRec& CEtsFixManagerDoc::GetRecord(long nIdx)
{
	CAutoLock lock(m_csLock);

	if(m_vecRecord.size() <= static_cast<size_t>(nIdx))
		throw E_INVALIDARG;

	return m_vecRecord[nIdx];
}

long CEtsFixManagerDoc::GetRecordsCount()
{
	CAutoLock lock(m_csLock);
	return static_cast<long>(m_vecRecord.size());
}

void CEtsFixManagerDoc::AddNewRecord(DATE dtTime, FgLogMessageTypeEnum enType, BSTR bsMessage)
{
	CFixReportRec aRecord;
	aRecord.m_dtTime = dtTime;
	aRecord.m_nType = enType;
	aRecord.m_strMessage = bsMessage;
	
	size_t nRecIdx = 0;
	{
		CAutoLock lock(m_csLock);
		m_vecRecord.push_back(aRecord);

		nRecIdx = m_vecRecord.size() - 1;
	}

	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	if(pView)
	{
		pView->PostMessage(WM_FIX_NEW_RECORD, (WPARAM)nRecIdx);
	}
}

void CEtsFixManagerDoc::CopyLogToClipboard()
{
	CAutoLock lock(m_csLock);

	CString strBuffer(_T("Type\tTime\tMessage\r\n"));
	size_t nCount = m_vecRecord.size();
	for(size_t i = 0; i < nCount; i++)
	{
		CFixReportRec& aRecord = m_vecRecord[nCount - i - 1];
		CString strTemp;

		strTemp.Format(_T("%d\t%s\t%s\r\n"), aRecord.m_nType, (LPCTSTR)aRecord.m_dtTime.Format(), (LPCTSTR)aRecord.m_strMessage);
		strBuffer += strTemp;
	}

	// Create an OLE data source on the heap
	COleDataSource* pData = new COleDataSource;

	HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE, (strBuffer.GetLength() + 1) * sizeof(TCHAR));
	if (hData)
	{
		LPSTR psz = (LPSTR)::GlobalLock(hData);
		memcpy(psz, (LPCTSTR)strBuffer, (strBuffer.GetLength() + 1) * sizeof(TCHAR));
		GlobalUnlock(hData);

		pData->CacheGlobalData(CF_OEMTEXT, hData );
		pData->SetClipboard();
	}
}
