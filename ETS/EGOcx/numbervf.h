#if !defined(AFX_NUMBERVF_H__AEC7CD34_A4FF_48B3_8C0F_B26055825A0D__INCLUDED_)
#define AFX_NUMBERVF_H__AEC7CD34_A4FF_48B3_8C0F_B26055825A0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CDerivedEnum;
class CDerivedBoolean;

/////////////////////////////////////////////////////////////////////////////
// CNumberVF wrapper class

class AFX_EXT_CLASS CNumberVF : public COleDispatchDriver
{
public:
	CNumberVF() {}		// Calls COleDispatchDriver default constructor
	CNumberVF(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CNumberVF(const CNumberVF& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetDecimalPlaces();
	void SetDecimalPlaces(long);
	BOOL GetUseSeparators();
	void SetUseSeparators(BOOL);
	CDerivedEnum GetOrderSignValue();
	void SetOrderSignValue(LPDISPATCH);
	CDerivedBoolean GetSpaceSeparatingSign();
	void SetSpaceSeparatingSign(LPDISPATCH);
	unsigned long GetNegativeValueColor();
	void SetNegativeValueColor(unsigned long);
	CDerivedBoolean GetUseBrackets();
	void SetUseBrackets(LPDISPATCH);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMBERVF_H__AEC7CD34_A4FF_48B3_8C0F_B26055825A0D__INCLUDED_)
