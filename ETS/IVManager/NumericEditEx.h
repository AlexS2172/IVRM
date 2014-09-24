#if !defined(AFX_NUMERICEDITEX_H__6594BDD2_BEE5_47A6_B0AA_F106A64E0396__INCLUDED_)
#define AFX_NUMERICEDITEX_H__6594BDD2_BEE5_47A6_B0AA_F106A64E0396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumericEditEx.h : header file
//

#include "EditEx.h"
#include "EGSpin.h"
/////////////////////////////////////////////////////////////////////////////
// CNumericEditEx window

class CNumericEditEx : public CEditEx
{
// Construction
public:
	// constructor
	CNumericEditEx();

	// retrieves text without mask
	virtual CString GetInputData() const;
	
	// sets the group separator
	BOOL SetGroupSeparator(TCHAR chGroupSeparator);
	// retrieves the symbol that is used as group separator
	inline TCHAR GetGroupSeparator() const { return m_chGroupSeparator; }

	// sets the number of decimal digits in group (3 by default). If you don't 
	// want to organize your decimal digits in groups then specify
	// zero as function argument.
	BOOL SetGroupLength(int nGroupLength);
	// retrieves the number of decimal digits in group. Zero value means
	// decimal digits won't be organized in groups.
	inline int GetGroupLength() const { return m_nGroupLength; }

	// sets the decimal separator
	BOOL SetDecimalSeparator(TCHAR chDecimalSeparator);
	// retrieves the symbol that is used as decimal separator
	inline TCHAR GetDecimalSeparator() const { return m_chDecimalSeparator; }

	// sets the flag that specifies if we have to show leading zero or not
	void SetShowLeadingZero(BOOL bShowLeadingZero);
	// retrieves the flag that specifies if we have to show leading zero or not
	inline BOOL GetShowLeadingZero() const { return m_bShowLeadingZero; }

	// sets the number of fractional digits displayed
	BOOL SetFractionalDigitCount(int nFractionalDigitCount);
	// retrieves the number of fractional digits displayed
	inline int GetFractionalDigitCount() const { return m_nFractionalDigitCount; }

	// sets the max number of decimal digits displayed.
	// Can be any value >=-1. Value -1 means
	// that indefinite number of decimal 
	// digits can be displayed
	BOOL SetDecimalDigitCount(int nDecimalDigitCount);

	// retrieves the max number of decimal digits displayed. 
	inline int GetDecimalDigitCount() const { return m_nDecimalDigitCount; }

	// Sets the mask to be used while displaying 
	// control's positive value. Mask should be specified as 
	// "Literals1.1Literals", where "1.1" stands for value and "Literals" 
	// could consists of any symbol(s). E.g. if format "<1.1>" had been
	// specified then value 5.26 would have been displayed as "<5.26>"
	virtual BOOL SetPositiveFormat(LPCTSTR lpszFormatPositive);

	// Retrieves the control's display format string for negative value.
	inline CString GetPositiveFormat() const { return m_sFormatPositive; }

	// Sets the mask to be used while displaying control's negative value. 
	// Mask should be specified as "Literals1.1Literals", where "1.1" 
	// stands for value and "Literals" could consists of any symbol(s).
	// E.g. if format "(1.1)" had been specified then value -5.26 would have 
	// been displayed as "(5.26)"
	virtual BOOL SetNegativeFormat(LPCTSTR lpszFormatNegative);
	// Retrieves the control's display format string for negative value.
	inline CString GetNegativeFormat() const { return m_sFormatNegative; }

	// sets the color that CNumericEditEx object uses to draw text
	// when its value is negative. By default we use the same color
	// for positive and negative value. In order to set text color 
	// for positive value call CEditEx::SetTextColor() function
	void SetNegativeTextColor(COLORREF clrTextNegative, BOOL bRedraw=TRUE);
	// gets the color that CNumericEditEx object uses to draw text
	// when its value is negative. In order to retrieve text color 
	// for positive value call CEditEx::GetTextColor() function
	inline COLORREF GetNegativeTextColor() const { return m_clrTextNegative; }

	// retrieves numeric value associated with the control
	double GetValue() const { return m_dValue; }
	// Use to programmatically set numeric value into the edit control. 
	BOOL SetValue(double dValue);

	// Retrieves flag that specifies whether numeric value associated
	// with the control is negative or positive
	inline BOOL IsNegativeValue() const { return m_bNegativeValue; }

	// Sets the flag that specifies whether the event of typing in
	// of period character will be interpreted as decimal point 
	// all the time or not
	void SetForceToUsePeriod(BOOL bForceToUsePeriod) { m_bForceToUsePeriod = bForceToUsePeriod; }
	// gets the flag that specifies whether the event of typing in
	// of period character will be interpreted as decimal point 
	// all the time or not
	inline BOOL GetForceToUsePeriod() const { return m_bForceToUsePeriod; }

	void SetSpinned(BOOL bSpinned = TRUE, double dMin = -100., double dMax = 100., double dStep = 1.);
	
protected:
	// virtual function that is called when control is initialized
	// and every time system settings has been changed. Retrieves
	// default settings for displaying numeric values based on 
	// default locale
	virtual BOOL GetLocaleSettings();

	// initializes control
	virtual BOOL InitControl();

	// checks control's contents and updates it in order to conform 
	// with rules defining the appearance of numeric data 
	virtual int ValidateInput();

	// resets associated numeric value
	virtual void DeleteContents();
	
	// checks specified real position as position at which valid input data is located
	virtual BOOL IsInputData(int nPosition) const;

	// function is called whenever the sigh of value must be changed
	virtual int ChangeSign(BOOL bNegative);

	// called every time when user is deleting symbol or range of symbols
	virtual int DeleteRange(int& nSelectionStart, int& nSelectionEnd);
	// called every time user typed new symbol (insert mode is on)
	virtual int InsertAt(int nSelectionStart, TCHAR chNewChar);
	// called every time user typed new symbol (overwrite mode is on)
	virtual int SetAt(int nSelectionStart, TCHAR chNewChar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumericEditEx)
	//}}AFX_VIRTUAL

protected:
	DECLARE_DYNAMIC(CNumericEditEx)

	// group separator
	TCHAR m_chGroupSeparator;	
	// group length
	int m_nGroupLength;	

	// decimal separator
	TCHAR m_chDecimalSeparator;	

	// show leading zero
	BOOL m_bShowLeadingZero;

	// number of fractional digits (>=0)
	int m_nFractionalDigitCount;

	// number of decimal digits (>=-1)
	int m_nDecimalDigitCount;

	// display format for negative value
	CString m_sFormatNegative;
	// display format for positve value
	CString m_sFormatPositive;

	// text color for negative value
	COLORREF m_clrTextNegative;

	// numeric value associated with the control
	double m_dValue;

	// flag that specifies if value is negative or positive
	BOOL m_bNegativeValue;

	// flag that specifies whether the period character must be always 
	// intepreted as decimal separator when it pressed
	BOOL m_bForceToUsePeriod;

	// set of values retrived from the recent locale
	// 
	TCHAR m_chGroupSeparatorDefault;	
	// group length
	int m_nGroupLengthDefault;	
	// decimal separator
	TCHAR m_chDecimalSeparatorDefault;	
	// show leading zero
	BOOL m_bShowLeadingZeroDefault;
	// number of fractional digits (>=0)
	int m_nFractionalDigitCountDefault;
	// number of decimal digits (>=-1)
	int m_nDecimalDigitCountDefault;
	// display format for negative value
	CString m_sFormatNegativeDefault;
	// display format for positve value
	CString m_sFormatPositiveDefault;

	CEGSpin*		m_pSpin;
	BOOL			m_bMouseOverSpin;

protected:
	// retrieves number of decimal digits inserted in the control
	int GetDecimalDigitsInserted() const;
	// retrieves number of fractional digits inserted in the control
	int GetFractionalDigitsInserted() const;

	// retrieves real position of specified symbol. Search only within input data
	// symbols. If specified symbol hasn't been found, returns -1
	int FindSymbol(TCHAR chSymbol) const;

	// processes new symbol that is about to be added to control's contents
	// at specified position. The third argument specifies whether symbol at 
	// nSelectionStart position must be inserted or replaced.
	virtual int TranslateSymbol(int nSelectionStart, TCHAR chNewChar, 
		BOOL bReplaceExisting);

	virtual BOOL _RedrawWindow();

	virtual void OnTrackStart();
	virtual void OnTrackEnd();
	virtual BOOL IsTracked() { return m_bMouseOverSpin; }

	// Generated message map functions
	//{{AFX_MSG(CNumericEditEx)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnEgSmValueChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEgSmMouseEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEgSmMouseExit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMERICEDITEX_H__6594BDD2_BEE5_47A6_B0AA_F106A64E0396__INCLUDED_)
