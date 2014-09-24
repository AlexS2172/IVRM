#if !defined(AFX_TIMESCALE_H__49366673_F0C7_4066_B8E8_7778D5EE32C5__INCLUDED_)
#define AFX_TIMESCALE_H__49366673_F0C7_4066_B8E8_7778D5EE32C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CDerivedString;

/////////////////////////////////////////////////////////////////////////////
// CTimeScale wrapper class

class AFX_EXT_CLASS CTimeScale : public COleDispatchDriver
{
public:
	CTimeScale() {}		// Calls COleDispatchDriver default constructor
	CTimeScale(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTimeScale(const CTimeScale& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetUnit();
	void SetUnit(long);
	CDerivedString GetFormat();
	LPDISPATCH GetParent();
	DATE GetBase();
	void SetBase(DATE);

// Operations
public:
	DATE ValueToDate(double Value);
	double DateToValue(DATE Date);
	CString FormatDate(DATE Date, LPCTSTR Format);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMESCALE_H__49366673_F0C7_4066_B8E8_7778D5EE32C5__INCLUDED_)