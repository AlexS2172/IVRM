#if !defined(AFX_BORDER_H__44F20776_B260_41D2_B0A7_34321DF9B760__INCLUDED_)
#define AFX_BORDER_H__44F20776_B260_41D2_B0A7_34321DF9B760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CBorder wrapper class

class AFX_EXT_CLASS CBorder : public COleDispatchDriver
{
public:
	CBorder() {}		// Calls COleDispatchDriver default constructor
	CBorder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBorder(const CBorder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetWidth();
	void SetWidth(long);
	long GetType();
	void SetType(long);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BORDER_H__44F20776_B260_41D2_B0A7_34321DF9B760__INCLUDED_)
