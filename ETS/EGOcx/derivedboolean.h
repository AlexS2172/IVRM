#if !defined(AFX_DERIVEDBOOLEAN_H__5BBBA7A8_8492_4708_842B_EA6B0CD528AF__INCLUDED_)
#define AFX_DERIVEDBOOLEAN_H__5BBBA7A8_8492_4708_842B_EA6B0CD528AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CDerivedBoolean wrapper class

class AFX_EXT_CLASS CDerivedBoolean : public COleDispatchDriver
{
public:
	CDerivedBoolean() {}		// Calls COleDispatchDriver default constructor
	CDerivedBoolean(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDerivedBoolean(const CDerivedBoolean& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetValue();
	void SetValue(BOOL);
	BOOL GetIsDefault();
	void SetIsDefault(BOOL);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DERIVEDBOOLEAN_H__5BBBA7A8_8492_4708_842B_EA6B0CD528AF__INCLUDED_)
