// RtControlDoc.cpp : implementation of the CRtControlDoc class
//

#include "stdafx.h"
#include "RtControl.h"

#include "RtControlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRtControlDoc

IMPLEMENT_DYNCREATE(CRtControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CRtControlDoc, CDocument)
	//{{AFX_MSG_MAP(CRtControlDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRtControlDoc construction/destruction

CRtControlDoc::CRtControlDoc()
{
	// TODO: add one-time construction code here

}

CRtControlDoc::~CRtControlDoc()
{
}

BOOL CRtControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRtControlDoc serialization

void CRtControlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRtControlDoc diagnostics

#ifdef _DEBUG
void CRtControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRtControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRtControlDoc commands
