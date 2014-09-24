#if !defined(AFX_EDITEX_H__2F2541D0_1B9C_4B1C_A14F_25D44E41ED8F__INCLUDED_)
#define AFX_EDITEX_H__2F2541D0_1B9C_4B1C_A14F_25D44E41ED8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditEx.h : header file
//

#include <afxtempl.h>
#include <atlbase.h>
#include "ToolTipCtrlEx.h"

// identificator for tool tip control
#define ID_EXEDIT_TOOLTIP		0x00000001

// additional notification messages
#define EN_EX_COMMIT			0x0801
#define EN_EX_DISCARD			0x0802


const TCHAR EXEDITABLETEXT_SYMBOL = _T('#');
const TCHAR EXCURRENCY_SYMBOL = _T('$');
const CString EXNUMERICVALUE_PLACEHOLDER(_T("1.1"));
const int EXNUMERICVALUE_PLACEHOLDER_LENGTH = EXNUMERICVALUE_PLACEHOLDER.GetLength();

/////////////////////////////////////////////////////////////////////////////
// CEditEx window

class CEditEx : public CEdit
{
public:
	CEditEx();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	void SetTextColor(COLORREF clrText, BOOL bRedraw = TRUE);
	inline COLORREF GetTextColor() const { return m_clrText; }

	void SetBkColor(COLORREF clrBack, BOOL bRedraw = TRUE);
	inline COLORREF GetBkColor() const { return m_clrBack; }

	void SetDisabledTextColor(COLORREF clrDisabledText, BOOL bRedraw = TRUE);
	inline COLORREF GetDisabledColor() const { return m_clrDisabledText; }

	void SetDisabledBkColor(COLORREF clrDisabledBack, BOOL bRedraw = TRUE);
	inline COLORREF GetDisabledBkColor() const { return m_clrDisabledBack; }

	void SetReadonlyTextColor(COLORREF clrReadonlyText, BOOL bRedraw = TRUE);
	inline COLORREF GetReadonlyTextColor() const { return m_clrReadonlyText; }

	void SetReadonlyBkColor(COLORREF clrReadonlyBack, BOOL bRedraw = TRUE);
	inline COLORREF GetReadonlyBkColor() const { return m_clrReadonlyBack; }

	BOOL SetToolTipText(LPCTSTR sText);
	inline CString GetToolTipText() const { return m_sToolTipText; }

	inline CToolTipCtrl* GetToolTip() { return &m_ttc; }

	// lpszMask -	mask to be used while displaying control contents.
	//				Mask should be specified as "LeftLiterals#RightLiterals"
	//				where # stands for editable contents and LeftLiterals
	//				defines the text that will be displayed from the left 
	//				side of editable text and RightLiterals
	//				defines the text that will be displayed from the right 
	//				side of editable text, e.g. "Phone: #".
	//				if you have to use '#' symbol as literal then you 
	//				have to use '##' instead.
	//				By default we use "#" mask.
	BOOL SetMask(LPCTSTR lpszMask);
	inline CString GetMask() const { return m_sMask; }

	CString GetFormattedText() const;

	virtual BOOL SetInputData(LPCTSTR pszInputData, int nBeginPos = 0);
	virtual CString GetInputData() const;
	virtual void EmptyData(BOOL bOnlyInput = FALSE);

	void SetInsertMode(BOOL bInsertMode);
	BOOL GetInsertMode() const;

	void SetFlat(BOOL bFlat, BOOL bRedraw = TRUE);
	inline BOOL GetFlat() const
	{
		return m_bIsFlat;
	}

	// converts real position within masked edit control to corresponding
	// logical one
	int RPtoLP(int nRealPos) const;
	
	// convert logical position within masked edit control to 
	// corresponding real one
	int LPtoRP(int nLogicalPos) const;

	// called when a user error occurs. 
	virtual void ValidationError();

	inline void SetHighlightOnSetFocus(BOOL bHighlightOnSetFocus) 
	{ 
		m_bHighlightOnSetFocus = bHighlightOnSetFocus; 
	}

	inline BOOL GetHighlightOnSetFocus() const { return m_bHighlightOnSetFocus; }

	inline virtual BOOL PointInRect() const
	{
		ASSERT(::IsWindow(m_hWnd));
		CPoint pt;
		GetCursorPos(&pt);
		CRect rcItem;
		GetWindowRect(&rcItem);
		return rcItem.PtInRect(pt);
	}

protected:
	virtual BOOL GetLocaleSettings() { return TRUE; }

	virtual BOOL InitControl();

	virtual int ValidateInput() { return 0; }

	virtual void DeleteContents();
	
	virtual BOOL IsInputData(int nPosition) const;
	virtual BOOL IsInputPosition(int nPosition) const;

	CString GetLeftLiterals() const;
	CString GetRightLiterals() const;

	virtual int DeleteRange(int& nSelectionStart, int& nSelectionEnd);
	virtual int InsertAt(int nSelectionStart, TCHAR chNewChar);
	virtual int SetAt(int nSelectionStart, TCHAR chNewChar);
	virtual int GetNextInputLocation(int nSelectionStart);
	virtual int GetPreviousInputLocation(int nSelectionStart);

	virtual void UpdateMask() {};

	void Update(int nSelectionStart = 0);
	void UpdateInsertionPointForward (int nSelectionStart);
	void UpdateInsertionPointBackward(int nSelectionStart);

	BOOL NotifyParent(UINT nNotificationID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	DECLARE_DYNAMIC(CEditEx)

	BOOL m_bPainted;
	BOOL m_bHasFocus;
	COLORREF m_clrText;	
	COLORREF m_clrBack;	

	COLORREF m_clrDisabledText;	
	COLORREF m_clrDisabledBack;	

	COLORREF m_clrReadonlyText;	
	COLORREF m_clrReadonlyBack;	

	COLORREF m_clrHilite;
	COLORREF m_clrShadow;
	COLORREF m_clrNormal;
	int m_iHorzFrameWidth;
	int m_iVertFrameWidth;

	CFont m_font;

	CString m_sMask;
	CArray<TCHAR,TCHAR> m_arrLeftLiterals;
	CArray<TCHAR,TCHAR> m_arrRightLiterals;
	CArray<TCHAR,TCHAR> m_arrInputData;

	BOOL m_bInsertMode;
	BOOL m_bIsFlat;

	CString m_sToolTipText;
	CToolTipCtrlEx m_ttc;

	int m_nSetTextSemaphor;
	int m_bNotifyParent;

	BOOL m_bNeedValidate;

	// flag that specifies the behaviour of the control when 
	// focus is being set to it. The default behaviour of the 
	// standard edit control is to select all text and cursor 
	// is being set at the end of the text. CEditEx handles this 
	// event in the following way: it sets cursor at the first 
	// symbol that can be modified without selecting any text.
	// Turning this flag on and off you can specify whether you 
	// would like to use the standard functionality or new one
	// (by default we use the new functionality when the text is 
	// not selected, i.e. the flag is set to FALSE)
	BOOL m_bHighlightOnSetFocus;

	// internal flag that stipulates that the class calls SetFocus function 
	// itself for internal purposes
	BOOL m_bNoSaveFont;

private:
	BOOL m_bInitialized;


protected:
	BOOL SaveFont();

	inline BOOL IsInitialized() const { return m_bInitialized; }
	virtual BOOL _RedrawWindow();

	virtual void DrawEdit(COLORREF clrTopLeft, COLORREF clrBottomRight);

	virtual void OnTrackStart() {}
	virtual void OnTrackEnd() {}
	virtual BOOL IsTracked() { return FALSE; }

	// Generated message map functions
	//{{AFX_MSG(CEditEx)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg LONG OnCut(UINT wParam, LONG lParam);
	afx_msg LONG OnCopy(UINT wParam, LONG lParam);
	afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	afx_msg LONG OnClear(UINT wParam, LONG lParam);
	afx_msg LONG OnSetText(UINT wParam, LONG lParam) ;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEX_H__2F2541D0_1B9C_4B1C_A14F_25D44E41ED8F__INCLUDED_)
