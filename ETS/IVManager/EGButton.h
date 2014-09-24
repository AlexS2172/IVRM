#if !defined(AFX_EGBUTTON_H__EE435282_47C4_4952_A09D_0D86C89C505A__INCLUDED_)
#define AFX_EGBUTTON_H__EE435282_47C4_4952_A09D_0D86C89C505A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EGButton.h : header file
//

#include "ToolTipCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CEGButton window

// The maximum number of images we have to build per bitmap button
#define EG_MAX_IMAGE_COUNT			5

#ifndef WM_EG_APP
// You can redefine WM_EG_APP by defining it before including this file
#define WM_EG_APP					(WM_APP + 100)
#endif
#define WM_CHECK_FLAT_LOOK			(WM_EG_APP + 100)

// dropdown notification sent to parent window 
#define EGBBN_DROPDOWN				(WM_EG_APP + 110)
// toggle notification sent to parent window 
#define EGBBN_TOGGLE				(WM_EG_APP + 121)
// tooltip ID
#define EGBB_TOOLTIP_ID				1000

void AFXAPI DDX_Toggle(CDataExchange *pDX, int nIDC, int& value);

class CEGButton : public CButton
{
	DECLARE_DYNAMIC(CEGButton);

	// extended color helper struct
	struct ColorEx
	{
		BOOL		m_bUseSystem;
		COLORREF	m_clrColor;

		ColorEx()
			: m_bUseSystem(TRUE), m_clrColor(RGB(0, 0, 0))
		{
		}
	};

	// dropdown arrow styles
	enum DropDownArrowStyleEnum
	{
		enDropDownArrowNone		= 0,
		enDropDownArrowDown		= 1,
		enDropDownArrowRight	= 2
	};

	enum DropDownMenuAlignEnum
	{
		enDropDownMenuAlignLeftDown	= 0,
		enDropDownMenuAlignRightUp	= 1
	};

	// color indexes
	enum ColorIndexEnum
	{
		enColorIndexBtnFace		= 0,
		enColorIndexBtnHilight	= 1,
		enColorIndexBtnShadow	= 2,
		enColorIndexBtnText		= 3,
		enColorIndexBtnGrayText	= 4,
		enColorIndexAll			= 5
	};

	// color state indexes
	enum StateColorEnum
	{
		enStateColorCommonNormal			= 0,
		enStateColorCommonDisabled			= 1,
		enStateColorCommonFlatTracked		= 2,

		enStateColorCheckedNormal			= 3,
		enStateColorCheckedDisabled			= 4,

		enStateColorIndeterminateNormal		= 5,
		enStateColorIndeterminateDisabled	= 6,

		enStateColorAll						= 7
	};

public:
	// constructor
	CEGButton();

	// Loads the bitmap from resource.
	BOOL LoadBitmap(LPCTSTR lpszBitmapResource, BOOL bUseMask = FALSE, COLORREF crMask = CLR_NONE, BOOL bResize = FALSE);
	
	// Loads the bitmap from resource.
	inline BOOL LoadBitmap(UINT nIDBitmapResource, BOOL bUseMask = FALSE, COLORREF crMask = CLR_NONE, BOOL bResize = FALSE)
	{
		return LoadBitmap(MAKEINTRESOURCE(nIDBitmapResource), bUseMask, crMask, bResize);
	}

	// Loads the inactive bitmap from resource.
	BOOL LoadInactiveBitmap(LPCTSTR lpszBitmapResource);

	// Loads the inactive bitmap from resource.
	inline BOOL LoadInactiveBitmap(UINT nIDBitmapResource)
	{
		return LoadInactiveBitmap(MAKEINTRESOURCE(nIDBitmapResource));
	}

	// Loads the disabled bitmap from resource.
	BOOL LoadDisabledBitmap(LPCTSTR lpszBitmapResource);

	inline BOOL LoadDisabledBitmap(UINT nIDBitmapResource)
	{
		return LoadDisabledBitmap(MAKEINTRESOURCE(nIDBitmapResource));
	}

	// Loads the icon from resource
	BOOL LoadIcon(LPCTSTR lpszIconResource, BOOL bResize = TRUE, UINT nWidth = 0, UINT nHeight = 0);

	// Loads the icon from resource
	inline BOOL LoadIcon(UINT nIDIconResource, BOOL bResize = TRUE, UINT nWidth = 0, UINT nHeight = 0)
	{
		return LoadIcon(MAKEINTRESOURCE(nIDIconResource), bResize, nWidth, nHeight);
	}

	// Loads the inactive icon from resource
	BOOL LoadInactiveIcon(LPCTSTR lpszIconResource, UINT nWidth = 0, UINT nHeight = 0);

	// Loads the inactive icon from resource
	inline BOOL LoadInactiveIcon(UINT nIDIconResource,UINT nWidth = 0, UINT nHeight = 0)
	{
		return LoadInactiveIcon(MAKEINTRESOURCE(nIDIconResource),nWidth,nHeight);
	}

	// Loads the disabled icon from resource
	BOOL LoadDisabledIcon(LPCTSTR lpszIconResource, UINT nWidth = 0, UINT nHeight = 0);

	// Loads the disabled icon from resource
	inline BOOL LoadDisabledIcon(UINT nIDIconResource, UINT nWidth = 0, UINT nHeight = 0)
	{
		return LoadDisabledIcon(MAKEINTRESOURCE(nIDIconResource), nWidth, nHeight);
	}

	// Remove any images (bitmaps or icons) previously set to control
	BOOL RemoveImage(BOOL bResize = TRUE);

	// Get palette of the bitmap button
	CPalette* GetPalette();

	// Sizes the button to fit its contents (image and text)
	virtual void SizeToContent();

	// Get flat look mode
	BOOL GetFlatLook() const;

	// Set flat look mode
	BOOL SetFlatLook(BOOL bFlatLook = TRUE);

	// Get track look mode
	BOOL GetTrackLook() const;

	// Set track look mode
	BOOL SetTrackLook(BOOL bTrackLook = TRUE);

	// Set horizontal alignment of the image and text (BS_LEFT, BS_CENTER or BS_RIGHT)
	void SetHorizontalAlignment(DWORD nAlignment = BS_CENTER);

	// Get horizontal alignment of the image and text (BS_LEFT, BS_CENTER or BS_RIGHT)
	DWORD GetHorizontalAlignment() const;

	// Set vertical alignment of the image and text (BS_TOP, BS_VCENTER or BS_BOTTOM)
	void SetVerticalAlignment(DWORD nAlignment = BS_VCENTER);

	// Get vertical alignment of the image and text (BS_TOP, BS_VCENTER or BS_BOTTOM)
	DWORD GetVerticalAlignment() const;

	// Get current text font
	CFont* GetTextFont();

	// Set new text font. A copy of the font will be made and stored internally
	void SetTextFont(CFont* pTextFont);

	// Get text of this button (window text upto but not including the first '\n")
	CString GetText() const;

	// Set text of this button (window text upto but not including the first '\n")
	void SetText(LPCTSTR pszText);

	// Get tool tip text of this button (window text starting from but not including the first '\n")
	CString GetToolTipText() const;

	// Set tool tip text of this button (window text starting from but not including the first '\n")
	void SetToolTipText(LPCTSTR pszToolTipText);

	// Get whether tooltip is enabled for this window
	BOOL GetToolTip() const;

	// Set whether tooltip is enabled for this window
	BOOL SetToolTip(BOOL bEnable = TRUE);

	// Get size of button in pixels when it fits its entire contents
	virtual CSize GetFitButtonSize();

	// Get size of the button in pixels
	CSize GetButtonSize() const;

	// Get size of the image in pixels
	CSize GetImageSize() const;

	// Get size of the text in pixels
	CSize GetTextSize(BOOL bCompact=FALSE);

	// Set button cursor
	BOOL SetCursor(HCURSOR hCursor = NULL);

	// Set button toggle mode
	void SetToggle(BOOL bToggle = TRUE, BOOL bTriState = FALSE, BOOL bRedraw = TRUE);

	// Get button toggle mode
	BOOL GetToggle() const;

	// Get button tri-state toggle mode
	BOOL GetTriState() const;

	// Set button check state in toggle mode
	void SetCheck(int nCheck);

	// Get button check state in toggle mode
	int GetCheck() const;

	// Set button dropdown mode
	void SetDropDown(BOOL bDropDown = TRUE,
					HMENU hMenu = NULL,
					DropDownArrowStyleEnum enArrowStyle = enDropDownArrowDown,
					DropDownMenuAlignEnum enMenuAlign = enDropDownMenuAlignLeftDown,
					BOOL bDrawDropdownSeparator = TRUE,
					BOOL bRedraw = TRUE);

	// Get button dropdown mode
	BOOL GetDropDown() const;

	// Get button arrow style in dropdown mode
	DropDownArrowStyleEnum GetDropDownArrowStyle() const;

	// Get menu align in dropdown mode
	DropDownMenuAlignEnum GetDropDownMenuAlign() const;

	// Get button dropdown separator in dropdown mode
	BOOL GetDrawingDropdownSeparator() const;

	// Set button colors
	BOOL SetColor(StateColorEnum enStateColor, ColorIndexEnum enColorIndex, BOOL bUseSystem, COLORREF clrColor = 0);

	// Get size of reserved space (from the bottom and right) that shouldn't be filled with button's image or text
	virtual CSize GetReservedSpace();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// destructor
	virtual ~CEGButton();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEGButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	virtual void DistributeSpace(UINT nState, CRect itemRect, CRect& buttonRect, CRect& imageRect, CRect& textRect);

	virtual void SelectPalette(CDC* pDC, UINT nState, CRect buttonRect, CPalette*& pOldPalette);

	virtual void DrawButton(CDC* pDC, UINT nState, CRect buttonRect);

	virtual void DrawImage(CDC* pDC, UINT nState, CImageList* pImageList, CRect imageRect);

	virtual void DrawText(CDC* pDC, UINT nState, CString sText, CRect textRect);

	virtual void DrawFocusRectangle(CDC* pDC, UINT nState, CRect buttonRect, CRect bitmapRect);

	virtual void RestorePalette(CDC* pDC, UINT nState, CRect buttonRect, CPalette* pOldPalette);

	BOOL BuildGrayBitmap(LPCTSTR lpszBitmapResource, COLORREF crMask, CBitmap* pGrayBitmap);

	BOOL BuildGrayIcon(LPCTSTR lpszIconResource, HICON* phIcon);

	BOOL MakeGray(LPBITMAPINFOHEADER pBitmapInfoHeader, COLORREF crMask = CLR_NONE);

	BOOL BuildDisabledImage(HICON hSourceIcon, CSize imageSize, HICON& hDestIcon);

	static BOOL GetBitmapPalette(LPCTSTR lpszBitmapResource, CPalette& palette);

	static BOOL GetIconPalette(LPCTSTR lpszIconResource, CPalette& palette);

	static BOOL GetImagePalette(LPBITMAPINFOHEADER pBitmapInfoHeader, CPalette& palette);

	void CheckFlatLook(CPoint point);

	void PostCheckFlatLook();

	static CString GetSubString(LPCTSTR pszFullString, int nSubIndex, TCHAR cDelimiter);

	static BOOL LoadBitmap(LPCTSTR lpszBitmapResource, CBitmap& bitmap, CPalette& palette);

	virtual int GetDropDownArrowWidth();

	virtual void DrawDropDownArrow(CDC* pDC, UINT nState, CRect arrowRect);

	virtual void OnDropDown();

	virtual void OnToggle();

	BOOL Toggle();

	COLORREF GetColor(ColorIndexEnum enColorIndex) const;

	void AdjustSysColors();
	
	//{{AFX_MSG(CEGButton)
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnCheckFlatLook(WPARAM wParam, LPARAM lParam);

	virtual afx_msg BOOL OnClicked();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()

protected:

	CImageList				m_imageList;
	CPalette				m_palette;
	CFont					m_textFont;
	CToolTipCtrlEx			m_toolTip;

	BOOL					m_bFlatLook;
	BOOL					m_bTrackLook;
	BOOL					m_bMouseOverButton;
	BOOL					m_bMouseDown;

	CImageList				m_backgroundImage;
	HCURSOR					m_hCursor;

	BOOL					m_bAllowDraw;
	CMenu					m_Menu;

	BOOL					m_bToggle;
	BOOL					m_bTriState;
	int						m_nCheck;
	BOOL					m_bDropDown;
	DropDownArrowStyleEnum	m_enArrowStyle;
	DropDownMenuAlignEnum	m_enMenuAlign;

	BOOL					m_bDrawDropdownSeparator;
	int						m_nDropDownArrowWidth;

	static const CPoint		m_ptImageOffset;
	static const CPoint		m_ptTextOffset;
	static const CPoint		m_ptInnerFocusOffset;
	static const CPoint		m_ptFlatInnerFocusOffset;
	static const CPoint		m_ptDownOffset;
	static const CPoint		m_ptFlatDownOffset;
	static const CPoint		m_ptCheckedOffset;
	static const CPoint		m_ptFlatCheckedOffset;
	static const CPoint		m_ptArrowOffset;

	static const int		m_nNormalImageIndex;
	static const int		m_nInactiveImageIndex;
	static const int		m_nDisabledImageIndex;

	ColorEx					m_clrColors[enStateColorAll][enColorIndexAll];
	COLORREF				m_clrSystemColors[enColorIndexAll];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EGBUTTON_H__EE435282_47C4_4952_A09D_0D86C89C505A__INCLUDED_)
