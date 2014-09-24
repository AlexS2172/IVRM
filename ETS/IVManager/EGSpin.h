#if !defined(AFX_EGSPIN_H__F4011A8E_07F3_4492_9F41_A3BFD7DAB6AB__INCLUDED_)
#define AFX_EGSPIN_H__F4011A8E_07F3_4492_9F41_A3BFD7DAB6AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EGSpin.h : header file
//

extern UINT EG_SM_VALUECHANGED;
extern UINT EG_SM_MOUSEENTER;
extern UINT EG_SM_MOUSEEXIT;

struct EG_SM_VALUECHANGED_DATA
{
	HWND	hwndFrom;
	double	dValue;
};

/////////////////////////////////////////////////////////////////////////////
// CEGSpin window

class CEGSpin : public CWnd
{
	DECLARE_DYNAMIC(CEGSpin)

	enum EGSpinPressedStateEnum
	{
		enPressedNone		= 0x00,
		enPressedUpRight	= 0x01,
		enPressedDnLeft		= 0x02
	};

	enum EGSpinAlignmentEnum
	{
		enAlignmentRightInside		= 1,
		enAlignmentRightOutside		= 2,
		enAlignmentLeftInside		= 3,
		enAlignmentLeftOutside		= 4
	};

	enum EGSpinOrientationEnum
	{
		enOrientationVertical		= 1,
		enOrientationHorizontal		= 2
	};

	enum EGSpinBuddyStateEnum
	{
		enBuddyStateNormal		= 1,
		enBuddyStateFocused		= 2,
		enBuddyStateTracked		= 3,
		enBuddyStateDisabled	= 4
	};

	struct EGSpinAccel
	{
		UINT nSec;
		double dInc;
	};

	typedef EGSpinAccel FAR* LPEGSpinAccel;

// Construction
public:
	CEGSpin();

	// 
	BOOL Create(CWnd* pParentWnd, CWnd* pBuddyWnd, UINT uiWidth = 0,
				EGSpinAlignmentEnum enAlignment = enAlignmentRightInside,
				EGSpinOrientationEnum enOrientation = enOrientationVertical);

	// 
	BOOL SetAlignment(EGSpinAlignmentEnum enAlignment = enAlignmentRightInside, BOOL bRedraw = TRUE);
	// 
	inline EGSpinAlignmentEnum GetAlignment()
	{
		return m_enAlignment;
	}

	// 
	BOOL SetOrientation(EGSpinOrientationEnum enOrientation = enOrientationVertical, BOOL bRedraw = TRUE);
	// 
	inline EGSpinOrientationEnum GetOrientation()
	{
		return m_enOrientation;
	}

	// 
	BOOL SetWrap(BOOL bWrap = TRUE);
	BOOL GetWrap()
	{
		return m_bWrap;
	}

	// 
	BOOL SetFlat(BOOL bFlat = TRUE, BOOL bRedraw = TRUE);
	inline BOOL GetFlat()
	{
		return m_bFlat;
	}

	// 
	BOOL SetRange(double dMin, double dMax, BOOL bRedraw = TRUE);
	inline double GetMin()
	{
		return m_dMin;
	}
	inline double GetMax()
	{
		return m_dMax;
	}

	//
	BOOL SetStep(double dStep);
	inline double GetStep()
	{
		return m_dStep;
	}

	//
	BOOL SetValue(double dValue, BOOL bRedraw = TRUE);
	inline double GetValue()
	{
		return m_dValue;
	}

	//
	BOOL SetPrecision(UINT uiPresision);
	inline UINT GetPrecision()
	{
		return m_uiPresision;
	}

	//
	BOOL SetAccel(UINT uiSpinAccelCount, LPEGSpinAccel pSpinAccel);

	//
	inline void SetCursor(HCURSOR hCursor)
	{
		m_hCursor = hCursor;
	}

	//
	void OnBuddyStateChanged(EGSpinBuddyStateEnum enBuddyState, BOOL bRedraw = TRUE);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEGSpin)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	void _AlignWindow(BOOL bSet);
	void _GetButtonRects(CRect& rcClient, CRect& rcUp, CRect& rcDn);
	BOOL _GetValidValue(EGSpinPressedStateEnum enPressedState, double& dValue, UINT nRepCnt = 1);
	void _SendValueChanged();

	inline int _CompareDouble(double lhs, double rhs)
	{
		if((rhs - m_dEps) < lhs && lhs < (rhs + m_dEps))
			return 0;
		if(lhs < rhs)
			return -1;
		return 1;
	}

	inline virtual BOOL _PointInRect() const
	{
		ASSERT(::IsWindow(m_hWnd));
		CPoint pt;
		GetCursorPos(&pt);
		CRect rcItem;
		GetWindowRect(&rcItem);
		return rcItem.PtInRect(pt);
	}

// Implementation
public:
	virtual ~CEGSpin();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEGSpin)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	HWND					m_hwndBuddy;
	HWND					m_hwndParent;

	EGSpinAlignmentEnum		m_enAlignment;
	EGSpinOrientationEnum	m_enOrientation;
	EGSpinPressedStateEnum	m_enPressedState;

	BOOL					m_bWrap;
	BOOL					m_bFlat;
	UINT					m_uiWidth;

	double					m_dMin;
	double					m_dMax;
	double					m_dStep;
	double					m_dValue;

	UINT					m_uiPresision;
	double					m_dEps;

	UINT					m_uiSpinAccelCount;
	LPEGSpinAccel			m_pSpinAccel;

	HCURSOR					m_hCursor;

	UINT					m_uiTimerPeriod;
	BOOL					m_bMouseOver;
	BOOL					m_bMouseDown;
	EGSpinPressedStateEnum	m_enActivePressedState;

	EGSpinBuddyStateEnum	m_enBuddyState;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EGSPIN_H__F4011A8E_07F3_4492_9F41_A3BFD7DAB6AB__INCLUDED_)
