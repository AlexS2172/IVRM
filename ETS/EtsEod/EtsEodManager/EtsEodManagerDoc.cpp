// EtsEodManagerDoc.cpp : implementation of the CEtsEodManagerDoc class
//

#include "stdafx.h"
#include "EtsEodManager.h"
#include "EtsEodManagerDoc.h"
#include "ModuleVersionEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEtsEodManagerDoc

IMPLEMENT_DYNCREATE(CEtsEodManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CEtsEodManagerDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
END_MESSAGE_MAP()


// CEtsEodManagerDoc construction/destruction

CEtsEodManagerDoc::CEtsEodManagerDoc()
	: m_bIsCurrent(FALSE)
{
}

CEtsEodManagerDoc::~CEtsEodManagerDoc()
{
}

BOOL CEtsEodManagerDoc::OnNewDocument()
{
	if(!OnOpenDocument(g_App.m_strCurrentReportFilePath))
		return FALSE;

	SetPathName(g_App.m_strCurrentReportFilePath);
	return TRUE;
}

void CEtsEodManagerDoc::_InitStepsMap()
{
	m_mapSteps[enSmEmpty].IsSelected = false;
	m_mapSteps[enSmLoadAvgVolumes].IsSelected = false;
	m_mapSteps[enSmLoadIVBands].IsSelected = false;
	m_mapSteps[enSmLoadSkewKurtosis].IsSelected = false;
	m_mapSteps[enSmLoadMktStructure].IsSelected = false;
	m_mapSteps[enSmCalculateVega].IsSelected = false;
}

DWORD CEtsEodManagerDoc::_ReadReport()
{
	m_mapSteps.clear();

	_InitStepsMap();

	CString strBuf;

	char chBuf[MAX_BUFFER];
	DWORD nBufSize = MAX_BUFFER;
	DWORD dwRes = ERROR_SUCCESS;
	long nOffset = 0;
	long nRead = 0;
	
	m_strScrapsBuf.Empty();

	while(nBufSize)
	{
		strBuf.Empty();
		
		if(m_strScrapsBuf.GetLength())
		{
			strBuf += m_strScrapsBuf;
			m_strScrapsBuf.Empty();
		}
		
		nBufSize = MAX_BUFFER;
		
		if((dwRes = Read(chBuf, nBufSize)) != ERROR_SUCCESS)
			return dwRes;

		strBuf.Append(chBuf, nBufSize);
		
		nOffset = 0;
		while(nOffset < strBuf.GetLength())
		{
			if((dwRes = _ReadLine(strBuf, nOffset, &nRead, false)) != ERROR_SUCCESS)
			{
				if(ERROR_INSUFFICIENT_BUFFER == dwRes)
				{
					m_strScrapsBuf.Append(strBuf.Mid(nOffset), strBuf.GetLength() - nOffset);
					strBuf.Empty();
					break;
				}
				else
				{
					ASSERT(FALSE);
					FileClose();
					return dwRes;
				}
			}

			nOffset += nRead;
			nOffset += 2; // skip CRLF
		}
	}

	return ERROR_SUCCESS;
}

void CEtsEodManagerDoc::OnAppend(LPCSTR lpBuf, long nSize)
{
	CString strBuf;
	DWORD dwRes = ERROR_SUCCESS;
	long nOffset = 0;
	long nRead = 0;

	if(m_strScrapsBuf.GetLength())
	{
		strBuf += m_strScrapsBuf;
		m_strScrapsBuf.Empty();
	}

	strBuf.Append(lpBuf, nSize);

	nOffset = 0;
	while(nOffset < strBuf.GetLength())
	{
		if((dwRes = _ReadLine(strBuf, nOffset, &nRead, true)) != ERROR_SUCCESS)
		{
			if(ERROR_INSUFFICIENT_BUFFER == dwRes)
			{
				m_strScrapsBuf.Append(strBuf.Mid(nOffset), strBuf.GetLength() - nOffset);
				strBuf.Empty();
				break;
			}
			else
				return;
		}

		nOffset += nRead;
		nOffset += 2; // skip CRLF
	}
}

DWORD CEtsEodManagerDoc::_ReadLine(const CString& strBuf, const long nOffset, long* pnRead, bool bNotify)
{
	int iEnd = nOffset;
	CString strRecord;
	iEnd = strBuf.Find(_T("\r\n"), iEnd);
	if(iEnd < 0)
		return ERROR_INSUFFICIENT_BUFFER;

	strRecord = strBuf.Mid(nOffset, iEnd - nOffset);

	CEodReportRec aRec;
	long nStep = 0L;
	if(aRec.ReadFromString(strRecord, nStep))
	{
		CEodReportStep& aStep = m_mapSteps[nStep];
		
		{
			CAutoLock lock(m_csLock);
			aStep.AddRecord(aRec);
		}

		if(bNotify)
		{
			size_t nRecIdx = aStep.GetRecs().size() - 1;

			POSITION pos = GetFirstViewPosition();
			CView* pView = GetNextView(pos);
			if(pView)
			{
				if(aRec.m_nType != enRpMtProgress)
					pView->PostMessage(WM_EOD_NEW_RECORD, (WPARAM)nStep, (LPARAM)nRecIdx);
				else
					pView->PostMessage(WM_EOD_STEP_PROGRESS, (LPARAM)nStep, 0L);
			}
		}
	}

	*pnRead = strRecord.GetLength();
	return ERROR_SUCCESS;
}

// CEtsEodManagerDoc diagnostics

#ifdef _DEBUG
void CEtsEodManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEtsEodManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEtsEodManagerDoc commands

void CEtsEodManagerDoc::OnFileSave()
{
	DoSaveAs();
}

BOOL CEtsEodManagerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if(::CopyFile(m_strPathName, lpszPathName, FALSE))
	{
		SetModifiedFlag(FALSE);
		return TRUE;
	}
	return FALSE;
}

BOOL CEtsEodManagerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	DeleteContents();
	SetModifiedFlag();

	CWaitCursor wait;
	if(ERROR_SUCCESS == FileOpen(lpszPathName))
	{
		_ReadReport();

		m_bIsCurrent = _IsCurrent(lpszPathName);
		EnableNotify(m_bIsCurrent);
	}

	SetModifiedFlag(FALSE);
	return TRUE;
}

void CEtsEodManagerDoc::OnCloseDocument()
{
	FileClose();
	CDocument::OnCloseDocument();
}

void CEtsEodManagerDoc::DeleteContents()
{
	FileClose();
	CDocument::DeleteContents();
}

BOOL CEtsEodManagerDoc::_IsCurrent(LPCTSTR lpszPathName)
{
	return g_App.m_strCurrentReportFilePath.CompareNoCase(lpszPathName) == 0L;
}

BOOL CEtsEodManagerDoc::DoSaveAs()
{
	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT(pTemplate != NULL);

	CString strNewName;
	if(m_bIsCurrent)
	{
		strNewName = CModuleVersionEx::GetFilePath(m_strPathName);
		COleDateTime dtToday(COleDateTime::GetCurrentTime());

		strNewName += _T("EodReport_");
		strNewName += dtToday.Format(_T("%Y%m%d"));
		CString strExt;
		pTemplate->GetDocString(strExt, CDocTemplate::filterExt);
		strNewName += strExt;
	}
	else
		strNewName = m_strPathName;

	if(!AfxGetApp()->DoPromptFileName(strNewName,
		AFX_IDS_SAVEFILECOPY, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
		return FALSE;       // don't even attempt to save

	strNewName.Trim();

	if(_IsCurrent(strNewName))
	{
		AfxGetMainWnd()->MessageBox(_T("Can't save opened report to current report file."), NULL, MB_ICONSTOP);
		return FALSE;
	}

	CWaitCursor wait;
	if(!OnSaveDocument(strNewName))
		return FALSE;

	return TRUE;        // success
}

