#if !defined(AFX_COORD_H__93C0F61C_C1BF_4294_9981_BFE92AF6D858__INCLUDED_)
#define AFX_COORD_H__93C0F61C_C1BF_4294_9981_BFE92AF6D858__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CCoord wrapper class

class AFX_EXT_CLASS CCoord : public COleDispatchDriver
{
public:
	CCoord() {}		// Calls COleDispatchDriver default constructor
	CCoord(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCoord(const CCoord& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetX();
	void SetX(long);
	long GetY();
	void SetY(long);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COORD_H__93C0F61C_C1BF_4294_9981_BFE92AF6D858__INCLUDED_)