// RtControlDoc.h : interface of the CRtControlDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTCONTROLDOC_H__CEF42E55_95C9_4510_84C3_FC255C77DD9C__INCLUDED_)
#define AFX_RTCONTROLDOC_H__CEF42E55_95C9_4510_84C3_FC255C77DD9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRtControlDoc : public CDocument
{
protected: // create from serialization only
	CRtControlDoc();
	DECLARE_DYNCREATE(CRtControlDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRtControlDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRtControlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRtControlDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTCONTROLDOC_H__CEF42E55_95C9_4510_84C3_FC255C77DD9C__INCLUDED_)
