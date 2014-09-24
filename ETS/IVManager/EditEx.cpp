// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////	
	
IMPLEMENT_DYNAMIC(CEditEx, CEdit)

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT,OnCut)
	ON_MESSAGE(WM_COPY,OnCopy)
	ON_MESSAGE(WM_PASTE,OnPaste)
	ON_MESSAGE(WM_CLEAR,OnClear)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
END_MESSAGE_MAP()

CEditEx::CEditEx()
{
	m_bNeedValidate = TRUE;

	m_clrText=::GetSysColor(COLOR_WINDOWTEXT); 
	m_clrBack=::GetSysColor(COLOR_WINDOW); 

	m_clrDisabledText = ::GetSysColor(COLOR_GRAYTEXT);	
	m_clrDisabledBack = ::GetSysColor(COLOR_BTNFACE);	

	m_clrReadonlyText = m_clrText;	
	m_clrReadonlyBack = m_clrDisabledBack;	

	m_bInsertMode=TRUE;

	TCHAR chMask[2];
	chMask[0]=EXEDITABLETEXT_SYMBOL;
	chMask[1]=_T('\0');
	VERIFY(SetMask(chMask));

	m_bInitialized=FALSE;
	m_bIsFlat = FALSE;

	m_nSetTextSemaphor=0;
	m_bNotifyParent=TRUE;

	m_bHighlightOnSetFocus=FALSE;

	m_bNoSaveFont=FALSE;

	m_bPainted = FALSE;
	m_bHasFocus = FALSE;

	m_clrHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrNormal = ::GetSysColor(COLOR_BTNFACE);
	m_iHorzFrameWidth = ::GetSystemMetrics(SM_CYEDGE);
	m_iVertFrameWidth = ::GetSystemMetrics(SM_CXEDGE);
}

void CEditEx::SetTextColor(COLORREF clrText, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrText!=clrText)
	{
		m_clrText=clrText; 
		if(bRedraw)
			_RedrawWindow();
	}
}

void CEditEx::SetDisabledTextColor(COLORREF clrDisabledText, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrDisabledText!=clrDisabledText)
	{
		m_clrDisabledText=clrDisabledText; 
		if(bRedraw)
			_RedrawWindow();
	}
}

void CEditEx::SetReadonlyTextColor(COLORREF clrText, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrText!=clrText)
	{
		m_clrText=clrText; 
		if(bRedraw)
			_RedrawWindow();
	}
}

void CEditEx::SetFlat(BOOL bFlat, BOOL bRedraw /*= TRUE*/)
{
	if(m_bIsFlat != bFlat)
	{
		m_bIsFlat = bFlat;
		if(bRedraw)
			_RedrawWindow();
	}
}

void CEditEx::SetBkColor(COLORREF clrBack, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrBack!=clrBack)
	{
		m_clrBack=clrBack; 
		if(bRedraw)
			_RedrawWindow();
	}
}

void CEditEx::SetDisabledBkColor(COLORREF clrDisabledBack, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrDisabledBack!=clrDisabledBack)
	{
		m_clrDisabledBack=clrDisabledBack; 
		if(bRedraw)
			_RedrawWindow();
	}
}

void CEditEx::SetReadonlyBkColor(COLORREF clrReadonlyBack, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrReadonlyBack!=clrReadonlyBack)
	{
		m_clrReadonlyBack=clrReadonlyBack; 
		if(bRedraw)
			_RedrawWindow();
	}
}

BOOL CEditEx::SetToolTipText(LPCTSTR sText)
{ 
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_sToolTipText=sText; 
		m_ttc.UpdateTipText(m_sToolTipText, this, ID_EXEDIT_TOOLTIP);

		return TRUE;
	}

	return FALSE;
}

HBRUSH CEditEx::CtlColor(CDC* pDC, UINT nCtlColor)
{
	UNREFERENCED_PARAMETER(nCtlColor);

	COLORREF clrText=GetTextColor();
	COLORREF clrBk=GetBkColor();
	if(!IsWindowEnabled())
	{
		clrText = m_clrDisabledText;
		clrBk = m_clrDisabledBack;
	}
	else if(GetStyle()&ES_READONLY)
	{
		clrText = m_clrReadonlyText;
		clrBk = m_clrReadonlyBack;
	}
	pDC->SetTextColor(clrText);
	pDC->SetBkColor(clrBk);

	if((HFONT)m_font!=NULL)
	{
		pDC->SelectObject(&m_font);
	}

	static CBrush brush;
	if((HBRUSH)brush!=NULL)
		brush.DeleteObject();
	brush.CreateSolidBrush(clrBk);

	return (HBRUSH)brush;
}

BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_ttc.Activate(TRUE); 
		m_ttc.RelayEvent(pMsg);
	}

	if(WM_KEYDOWN == pMsg->message
		//&& m_bCheckCommitDiscard
		&& (GetStyle() & ES_READONLY) != ES_READONLY
		&& (GetStyle() & ES_MULTILINE) != ES_MULTILINE
		&& (GetStyle() & ES_WANTRETURN) != ES_WANTRETURN
		&& GetSafeHwnd() == pMsg->hwnd
		&& (VK_RETURN == pMsg->wParam || VK_CANCEL == pMsg->wParam || VK_ESCAPE == pMsg->wParam))
	{
		HWND hwndParent = GetParent()->GetSafeHwnd();
		if(NULL != hwndParent)
		{
			if(VK_RETURN == pMsg->wParam)
				NotifyParent(EN_EX_COMMIT);
			else
				NotifyParent(EN_EX_DISCARD);
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CEditEx::PreSubclassWindow()
{
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL && !m_bInitialized)
	{
		if(!InitControl())
			TRACE(_T("CEditEx::PreSubclassWindow: failed to initialize edit control\n"));
	}

	CEdit::PreSubclassWindow();
}

void CEditEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bIsFlat)
	{
		SetTimer(1,10,NULL);
		OnTimer(1);
		OnTrackStart();
	}
	CEdit::OnMouseMove(nFlags, point);
}

void CEditEx::OnTimer(UINT nIDEvent) 
{
	if(!m_bIsFlat)
	{
		KillTimer(1);
		return;
	}

	if(!PointInRect() && !m_bHasFocus && !IsTracked())
	{
		KillTimer(1);
		OnTrackEnd();
		if(m_bPainted == TRUE)
		{
			_RedrawWindow();
			//DrawEdit(m_clrNormal, m_clrNormal);
			m_bPainted = FALSE;
		}
	}
	else
	{
		if(m_bPainted == TRUE)
		{
			return;
		}
		else
		{
			m_bPainted = TRUE;
			_RedrawWindow();
			//DrawEdit(m_clrShadow, m_clrHilite);
		}
	}
}

void CEditEx::OnPaint() 
{
	Default();

	if(m_bIsFlat)
	{
		if(PointInRect() || m_bHasFocus || IsTracked())
		{
			DrawEdit(m_clrShadow, m_clrHilite);
		}
		else
		{
			DrawEdit(m_clrNormal, m_clrNormal);
		}
	}
}

void CEditEx::DrawEdit(COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if(m_bIsFlat)
	{
		CDC* pDC = GetDC();
		CRect rcItem;
		GetClientRect(&rcItem);
		rcItem.InflateRect(m_iHorzFrameWidth, m_iVertFrameWidth);

		pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
		rcItem.DeflateRect(1,1);
		
		if(!IsWindowEnabled())
		{
			pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite);
		}
		else
		{
			pDC->Draw3dRect(rcItem, m_clrNormal, m_clrNormal);
		}

		if(!IsWindowEnabled())
		{
			ReleaseDC(pDC);
			return;
		}

		ReleaseDC(pDC);
	}
}

void CEditEx::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);
	
	// update the original size of the edit control window
	CRect rect;
	GetWindowRect(rect);
	CWnd* pParent=GetParent();
	if(pParent!=NULL)
	{
		pParent->ScreenToClient(rect);
	}

	// Change information about window's rect in tooltip control
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		CToolInfo toolInfo;
		if(m_ttc.GetToolInfo(toolInfo,this,ID_EXEDIT_TOOLTIP))
		{
			toolInfo.rect.left=0;
			toolInfo.rect.top=0;
			toolInfo.rect.right=rect.Width();
			toolInfo.rect.bottom=rect.Height();
			m_ttc.SetToolInfo(&toolInfo);
		}
	}
}

BOOL CEditEx::InitControl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	GetLocaleSettings();

	// Create the tooltip
	CRect rect; 
	GetClientRect(rect);
	m_ttc.Create(this);
	m_ttc.AddTool(this, m_sToolTipText, rect, ID_EXEDIT_TOOLTIP);

	// save font
	VERIFY(SaveFont());

	CString sText;
	GetWindowText(sText);
	EmptyData();
	SetInputData(sText);

	m_bInitialized=TRUE;

	return TRUE;
}

BOOL CEditEx::Create(DWORD dwStyle, const RECT& rect, 
					 CWnd* pParentWnd, UINT nID)
{
	BOOL bResult=CEdit::Create(dwStyle, rect, pParentWnd, nID);
	if(bResult)
	{
		// Initialize edit control
		if(!InitControl())
		{
			TRACE(_T("CEditEx::Create: failed to initialize edit control"));
			return FALSE;
		}
	}

	return bResult;
}

void CEditEx::OnDestroy()
{
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_ttc.DelTool(this, ID_EXEDIT_TOOLTIP);
		m_ttc.DestroyWindow();
	}

	CEdit::OnDestroy();
}


BOOL CEditEx::SaveFont()
{
	LOGFONT lf;
	CFont* pFont=GetFont();
	if(pFont!=NULL)
	{
		if(!pFont->GetLogFont(&lf))
			return FALSE;
	}
	
	if((HFONT)m_font!=NULL)
		m_font.DeleteObject();
	if(pFont!=NULL)
	{
		if(!m_font.CreateFontIndirect(&lf))
			return FALSE;
	}

	return TRUE;
}


LRESULT CEditEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=CEdit::WindowProc(message, wParam, lParam);

	switch(message)
	{
	case WM_SETFONT:
		{
			if(!m_bNoSaveFont)
			{
				VERIFY(SaveFont());
			}
			break;
		}

	case WM_SETTINGCHANGE:
		{
			CString sInputData=GetInputData();
			GetLocaleSettings();
			EmptyData(TRUE);
			VERIFY(SetInputData(sInputData));
			break;
		}
	}

	return lResult;
}


void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((GetStyle()&ES_READONLY)==ES_READONLY)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	BOOL bIsShiftKeyDown=::GetAsyncKeyState(VK_SHIFT)<0;
	if(nChar==VK_DELETE)
	{
		int nSelectionStart=0;
		int nSelectionEnd=0;
		GetSel(nSelectionStart, nSelectionEnd);
		// Delete has two functions, it can delete the selection and
		// it can delete characters to the right.
		if(nSelectionStart==nSelectionEnd)
		{
			nSelectionEnd++; // Do the equivalent of a selection.
			if(DeleteRange(nSelectionStart, nSelectionEnd))
			{
				Update(nSelectionStart);
			}
			else	// Must be on a literal, so continue moving to right
					// and re-attempting the delete until we either delete
					// a character or run out of characters.
			{
				while(nSelectionEnd<=
					m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize())
				{
					nSelectionStart++;
					nSelectionEnd++; // Do the equivalent of a selection.
					if(DeleteRange(nSelectionStart, nSelectionEnd))
					{
						Update(nSelectionStart);
						break;
					}
				}
			}
		}
		else if(DeleteRange(nSelectionStart, nSelectionEnd))
		{
			Update(nSelectionStart);
		}
		else	// Must be on a literal, so continue moving to right
				// and reattempting the delete until we either delete
				// a character or run out of characters.
		{
			while(nSelectionEnd<=
					m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize())
			{
				nSelectionStart++;
				nSelectionEnd++; // Do the equivalent of a selection.
				if(DeleteRange(nSelectionStart, nSelectionEnd))
				{
					Update(nSelectionStart);
					break;
				}
			}
		}
	}
	else if(nChar==VK_HOME)
	{
		// If the shift key is not down, then HOME is a navigation and we need to 
		// move the insertion point to the first available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			if(!IsInputPosition(nSelectionStart))
				UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if(nChar==VK_END)
	{
		// If the shift key is not down, then HOME is a navigation and we need to 
		// move the insertion point to the first available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			if(!IsInputPosition(nSelectionStart))
				UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if(nChar==VK_LEFT)
	{
		// If the shift key is not down, then LEFT is a navigation and we need to 
		// move the insertion point to the previous available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if(nChar==VK_UP)
	{
		// If the shift key is not down, then UP is a navigation and we need to 
		// move the insertion point to the previous available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if(nChar==VK_RIGHT)
	{
		// If the shift key is not down, then RIGHT is a navigation and we need to 
		// move the insertion point to the next available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if(nChar==VK_DOWN)
	{
		// If the shift key is not down, then DOWN is a navigation and we need to 
		// move the insertion point to the next available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if(nChar==VK_INSERT)
	{
		// The standard CEdit control does not support over-typing. 
		// This flag is used to manage over-typing internally. 
		SetInsertMode(!GetInsertMode());
	}
	else
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}


void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((GetStyle()&ES_READONLY)==ES_READONLY)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	BOOL bAcceptReturnKey=(GetStyle()&ES_MULTILINE) && (GetStyle()&ES_WANTRETURN);
	if(nChar==10)
	{
		nChar=VK_RETURN;
		bAcceptReturnKey=(GetStyle()&ES_MULTILINE);
	}

	// If character value is above 32, then it is ANSI or Extended. 
	// Below 32 are control and navigation characters. 
	if(nChar>=32 || (nChar==VK_RETURN && bAcceptReturnKey))
	{
		if(nSelectionStart==nSelectionEnd)
		{
			if(GetStyle()&ES_NUMBER && nChar!=VK_RETURN && 
				(nChar<_T('0') || nChar>_T('9')))
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
				return;
			}

			if(IsInputPosition(nSelectionStart))
			{
				int nActualInsertionPoint=nSelectionStart;
				if(m_bInsertMode)
				{
					if(nChar==VK_RETURN)
					{
						// InsertAt will return -1 if the character cannot be inserted here. 
						nActualInsertionPoint=InsertAt(nSelectionStart,_T('\r'));
						if(nActualInsertionPoint>=0)
						{
							nSelectionStart=nActualInsertionPoint+1;
							nActualInsertionPoint=InsertAt(nSelectionStart,_T('\n'));
						}
					}
					else
					{
						nActualInsertionPoint=InsertAt(nSelectionStart,(TCHAR)nChar);
					}
				}
				else
				{
					if(nChar==VK_RETURN)
					{
						nActualInsertionPoint=SetAt(nSelectionStart,_T('\r'));
						// SetAt will return -1 if the character cannot be set here. 
						if(nActualInsertionPoint>=0)
						{
							nSelectionStart=nActualInsertionPoint+1;
							nActualInsertionPoint=SetAt(nSelectionStart,_T('\n'));
						}
					}
					else
					{
						nActualInsertionPoint=SetAt(nSelectionStart,(TCHAR)nChar);
					}
				}
				
				// InsertAt will return -1 if the character cannot be inserted here. 
				if(nActualInsertionPoint>=0)
					nSelectionStart=nActualInsertionPoint+1;
				else
					ValidationError();
				
				Update(nSelectionStart);
			}
			else
			{
				// Beep if trying to type over a literal. 
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
		else
		{
			// First delete the remaining selection. 
			// The function will return a valid count if 
			// some input characters were deleted. We use 
			// this value to determine if it makes sense to insert. 
			if(DeleteRange(nSelectionStart,nSelectionEnd))
			{
				// InsertAt will place the character at the next available position, 
				// then return that positition
				int nActualInsertionPoint=nSelectionStart;
				if(nChar==VK_RETURN)
				{
					// InsertAt will return -1 if the character cannot be inserted here. 
					nActualInsertionPoint=InsertAt(nSelectionStart,_T('\r'));
					if(nActualInsertionPoint>=0)
					{
						nSelectionStart=nActualInsertionPoint+1;
						nActualInsertionPoint=InsertAt(nSelectionStart,_T('\n'));
					}
				}
				else
				{
					nActualInsertionPoint=InsertAt(nSelectionStart,(TCHAR)nChar);
				}
				
				// InsertAt will return -1 if the character cannot be inserted here. 
				if(nActualInsertionPoint>=0)
					nSelectionStart=nActualInsertionPoint+1;
				else
					ValidationError();
				
				Update(nSelectionStart);
			}
			else  // Must be on a literal, so beep and move to a valid location. 
			{
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
	}
	else
	{
		if(nChar==VK_BACK)
		{
			// Backspace performs two functions, if there is a selection,
			// then the backspace is the same as deleting the selection.
			// If there is no selection, then the backspace deletes the
			// first non-literal character to the left.
			if(nSelectionStart==nSelectionEnd)
			{
				if(nSelectionStart>=1)
				{
					while(nSelectionStart>0)
					{
						nSelectionStart--; // Do the equivalent of a backspace.

						if(DeleteRange(nSelectionStart,nSelectionEnd))
						{
							Update(nSelectionStart);
							break;
						}

						nSelectionEnd--;
					}
				}
			}
			else if(DeleteRange(nSelectionStart,nSelectionEnd))
			{
				Update(nSelectionStart);
			}
			else	// Must be on a literal, so continue moving to left
					// and re-attempting the delete until we either delete
					// a character or run out of characters.
			{
				if(nSelectionStart>=1)
				{
					while(nSelectionStart>0)
					{
						nSelectionStart--; // Do the equivalent of a backspace.

						if(DeleteRange(nSelectionStart,nSelectionEnd))
						{
							Update(nSelectionStart);
							break;
						}
						
						nSelectionEnd--;
					}
				}
			}
		}
		else
			// let edit control to do its job 
			CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CEditEx::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	if(!GetHighlightOnSetFocus())
	{
		// The default behavior is to highlight the entire string. 
		// If this is the case, then move the insertion to the first input position. 
		int nSelectionStart=0;
		int nSelectionEnd=0;
		GetSel(nSelectionStart, nSelectionEnd);
		if((nSelectionStart==0) && (nSelectionEnd==GetWindowTextLength()))
		{
			// Only update the insertion point if the entire string is selected. 
			// This will allow the mouse to be used to set the cursor without 
			// our interfering. 
			UpdateInsertionPointForward(0);
		}
	}

	m_bNoSaveFont=TRUE;
	SetFont(&m_font);
	m_bNoSaveFont=FALSE;

	m_bHasFocus = TRUE;
	if(m_bIsFlat)
		_RedrawWindow();
		//DrawEdit(m_clrShadow, m_clrHilite);
}

void CEditEx::OnSysColorChange() 
{
	CEdit::OnSysColorChange();

	m_clrHilite	= ::GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow	= ::GetSysColor(COLOR_BTNSHADOW);
	m_clrNormal	= ::GetSysColor(COLOR_BTNFACE);
}

BOOL CEditEx::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

LONG CEditEx::OnCut(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart,nSelectionEnd);

	// Before updating, let the control do its normal thing. 
	// This will save us the effort of filling the clipboard. 
	CEdit::Default();

	// First do our version of the cut. 
	int nDeleteCount=DeleteRange(nSelectionStart,nSelectionEnd);

	// Now we update with our standard mask. 
	Update(nSelectionStart);
	if(nDeleteCount==0)
	{
		// I don't think we want to beep if no input characters were cut. 
		//ValidationError();
	}

	return 0;
}


LONG CEditEx::OnCopy(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Just let copy do its thing and copy the selected text. 
	CEdit::Default();

	return 0;
}


LONG CEditEx::OnPaste(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	CEdit::Default();

	// This is a real dump paste routine that expects SetInputData do 
	// do the filtering.  There is probably no easy solution to this 
	// task because anything can be pasted.  We could try and match 
	// the literals, but maybe we will get to that later. 
	CString sNewString;
	GetWindowText(sNewString);

	if(nSelectionStart!=nSelectionEnd)
	{
		CString sLeftLiterals=GetLeftLiterals();
		CString sRightLiterals=GetRightLiterals();

		if(sNewString.Find(sLeftLiterals)==0)
			sNewString=sNewString.Mid(sLeftLiterals.GetLength());
		int nPosition=sNewString.Find(sRightLiterals);
		if(nPosition!=-1 && 
			nPosition+sRightLiterals.GetLength()==sNewString.GetLength())
			sNewString=sNewString.Left(nPosition);
	}

	EmptyData(TRUE);
	SetInputData(sNewString,0);
	// Setting the insertion point after a paste is tricky because the 
	// expected location is after the last valid pasted character. 
	// Try and determine this location by setting the insertion point 
	// to the first empty location after the specified starting point. 
	int nNewInsertionPoint=GetNextInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint,nNewInsertionPoint);

	return 0;
}


LONG CEditEx::OnClear(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE(_T("CEditEx::OnClear\n"));

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart,nSelectionEnd);

	// Before updating, let the control do its normal thing. 
	CEdit::Default();

	// First do our version of the cut. 
	int nDeleteCount=DeleteRange(nSelectionStart,nSelectionEnd);

	// Now we update with our standard mask. 
	if(nDeleteCount>0)
		Update(nSelectionStart);

	return 0;
}


void CEditEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonDown(nFlags,point);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart,nSelectionEnd);
	if(nSelectionStart==nSelectionEnd && !IsInputPosition(nSelectionStart))
		UpdateInsertionPointForward(0);
}


LONG CEditEx::OnSetText(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if(m_nSetTextSemaphor>0)
	{
		LONG result=CEdit::Default();
		NotifyParent(EN_UPDATE);
		if(m_bNotifyParent)
			NotifyParent(EN_CHANGE);
		_RedrawWindow();
		return result;
	}
	else
	{
		ASSERT(m_nSetTextSemaphor==0);
		CString sText=(LPCTSTR)lParam;
		
		m_arrInputData.RemoveAll();
		//EmptyData(TRUE);

		m_bNotifyParent=FALSE;
		SetInputData(sText,0);
		m_bNotifyParent=TRUE;
		return TRUE;
	}
}

BOOL CEditEx::SetMask(LPCTSTR lpszMask)
{
	CString sMask=lpszMask;
	if(sMask.IsEmpty() || sMask.Find(EXEDITABLETEXT_SYMBOL)==-1)
		return FALSE;

	DeleteContents();

	BOOL bLeftLiteral=TRUE;
	for(int nIndex=0; nIndex<sMask.GetLength(); nIndex++)
	{
		BOOL bSaveLiteral=TRUE;
		TCHAR chSymbol=sMask[nIndex];
		TCHAR chSpecialSymbol=EXEDITABLETEXT_SYMBOL;
		if(chSymbol==chSpecialSymbol && (nIndex==sMask.GetLength()-1 || 
			sMask[nIndex+1]!=chSpecialSymbol))
		{
			bSaveLiteral=FALSE;
			bLeftLiteral=FALSE;
		}
		else if(chSymbol==chSpecialSymbol && nIndex<sMask.GetLength()-1 && 
			sMask[nIndex+1]==chSpecialSymbol)
		{
			nIndex++;
		}

		if(bSaveLiteral)
		{
			if(bLeftLiteral)
				m_arrLeftLiterals.Add(chSymbol);
			else
				m_arrRightLiterals.Add(chSymbol);
		}
	}

	if(bLeftLiteral)
	{
		VERIFY(SetMask(GetMask()));
		return FALSE;
	}

	m_sMask=sMask;

	if(m_bNeedValidate)
		ValidateInput();

	Update();

	return TRUE;
}


void CEditEx::DeleteContents()
{
	m_arrLeftLiterals.RemoveAll();
	m_arrRightLiterals.RemoveAll();
	m_arrInputData.RemoveAll();
}


CString CEditEx::GetInputData()const
{
	CString sInputData;
	for(int nIndex=0; nIndex<m_arrInputData.GetSize(); nIndex++)
	{
		sInputData+=m_arrInputData[nIndex];
	}
	return sInputData;
}


BOOL CEditEx::SetInputData(LPCTSTR pszInputData, int nBeginPos/*=0*/)
{
	if(nBeginPos<0 || nBeginPos>m_arrInputData.GetSize())
		return FALSE;

	CString sInputData=pszInputData;
	for(int nIndex=0; nIndex<sInputData.GetLength(); nIndex++)
	{
		if(GetInsertMode())
			m_arrInputData.InsertAt(nBeginPos+nIndex,sInputData[nIndex]);
		else
			m_arrInputData.SetAtGrow(nBeginPos+nIndex,sInputData[nIndex]);
	}
	
	if(m_bNeedValidate)
		ValidateInput();

	Update(-1);

	return TRUE;
}


void CEditEx::EmptyData(BOOL bOnlyInput/*=FALSE*/)
{
	if(bOnlyInput)
		m_arrInputData.RemoveAll();
	else
		DeleteContents();
	
	Update();
}

BOOL CEditEx::GetInsertMode() const
{
	// The standard CEdit control does not support over-typing. 
	// This flag is used to manage over-typing internally. 
	return m_bInsertMode;
}

void CEditEx::SetInsertMode(BOOL bInsertMode)
{
	// The standard CEdit control does not support over-typing. 
	// This flag is used to manage over-typing internally. 
	m_bInsertMode=bInsertMode;
}


CString CEditEx::GetFormattedText() const
{
	CString sFormattedText;
	for(int nIndex=0; nIndex<m_arrLeftLiterals.GetSize(); nIndex++)
		sFormattedText+=m_arrLeftLiterals[nIndex];
	for(int nIndex=0; nIndex<m_arrInputData.GetSize(); nIndex++)
		sFormattedText+=m_arrInputData[nIndex];
	for(int nIndex=0; nIndex<m_arrRightLiterals.GetSize(); nIndex++)
		sFormattedText+=m_arrRightLiterals[nIndex];

	return sFormattedText;
}


BOOL CEditEx::IsInputData(int nPosition) const
{
	// We frequently need to know if a position refers to 
	// input data or to a literal. 
	return((nPosition>=m_arrLeftLiterals.GetSize()&& 
		nPosition<m_arrLeftLiterals.GetSize()+
		m_arrInputData.GetSize())? TRUE : FALSE);
}


BOOL CEditEx::IsInputPosition(int nPosition) const
{
	// We frequently need to know if a position refers to 
	// input data or to a literal. 
	return((nPosition>=m_arrLeftLiterals.GetSize() && 
		nPosition<=m_arrLeftLiterals.GetSize()+
		m_arrInputData.GetSize())? TRUE : FALSE);
}


int CEditEx::DeleteRange(int& nSelectionStart, int& nSelectionEnd)
{
	int nDeleteCount=0;
	TCHAR chLastDeleted=_T('\0');
	TCHAR chFirstDeleted=_T('\0');
	int nIndex = 0;
	for(nIndex=nSelectionEnd-1; nIndex>=nSelectionStart; nIndex--)
	{
		if(IsInputData(nIndex))
		{
			if(chFirstDeleted==_T('\0'))
				chFirstDeleted=m_arrInputData[RPtoLP(nIndex)];
			chLastDeleted=m_arrInputData[RPtoLP(nIndex)];
			m_arrInputData.RemoveAt(RPtoLP(nIndex));
			nDeleteCount++;
		}
	}
	if(chLastDeleted==_T('\r') && RPtoLP(nIndex+1)!=-1 && 
		m_arrInputData[RPtoLP(nIndex+1)]==_T('\n'))
	{
		m_arrInputData.RemoveAt(RPtoLP(nIndex+1));
		nDeleteCount++;
	}
	if(chFirstDeleted==_T('\n') && RPtoLP(nSelectionStart-1)!=-1 && 
		m_arrInputData[RPtoLP(nSelectionStart-1)]==_T('\r'))
	{
		m_arrInputData.RemoveAt(RPtoLP(nSelectionStart-1));
		nDeleteCount++;
		nSelectionStart--;
	}

	if(nDeleteCount)
		Update(-1);

	// return the deleted count so that an error can be generated 
	// if none were deleted. 
	return nDeleteCount;
}

int CEditEx::InsertAt(int nSelectionStart, TCHAR chNewChar)
{
	int nInsertionPoint=nSelectionStart;
	if(!IsInputPosition(nInsertionPoint))
	{
		nInsertionPoint=GetNextInputLocation(nInsertionPoint+1);
		ASSERT(IsInputPosition(nInsertionPoint));
	}

	m_arrInputData.InsertAt(RPtoLP(nInsertionPoint),chNewChar);

	int nPt = 0;
	if(m_bNeedValidate)
		ValidateInput();
	nInsertionPoint+=nPt;

	Update(-1);

	return nInsertionPoint;
}


int CEditEx::SetAt(int nSelectionStart, TCHAR chNewChar)
{
	int nInsertionPoint=nSelectionStart;
	if(!IsInputData(nInsertionPoint))
	{
		nInsertionPoint=GetNextInputLocation(nInsertionPoint+1);
		if(!IsInputData(nInsertionPoint))
			return -1;
	}

	m_arrInputData.SetAt(RPtoLP(nInsertionPoint),chNewChar);

	int nPt = 0;
	if(m_bNeedValidate)
		ValidateInput();
	nInsertionPoint+=nPt;

	Update(-1);

	return nInsertionPoint;
}


int CEditEx::GetNextInputLocation(int nSelectionStart)
{
	int nNextInputLocation=nSelectionStart;
	if(!IsInputPosition(nNextInputLocation))
	{
		nNextInputLocation=m_arrLeftLiterals.GetSize()+
			m_arrInputData.GetSize();
		for(int nIndex=nSelectionStart; 
			nIndex<m_arrInputData.GetSize()+m_arrLeftLiterals.GetSize(); 
			nIndex++)
		{
			if(IsInputPosition(nIndex))
			{
				nNextInputLocation=nIndex;
				break;
			}
		}
	}

	ASSERT(IsInputPosition(nNextInputLocation));
	return nNextInputLocation;
}


int CEditEx::GetPreviousInputLocation(int nSelectionStart)
{
	int nPreviousInputLocation=nSelectionStart;
	if(!IsInputPosition(nPreviousInputLocation))
	{
		nPreviousInputLocation=m_arrLeftLiterals.GetSize();
		for(int nIndex=nSelectionStart; 
			nIndex>=m_arrLeftLiterals.GetSize(); 
			nIndex--)
		{
			if(IsInputPosition(nIndex))
			{
				nPreviousInputLocation=nIndex;
				break;
			}
		}
	}

	ASSERT(IsInputPosition(nPreviousInputLocation));
	return nPreviousInputLocation;
}


void CEditEx::Update(int nSelectionStart/*=0*/)
{
	// Update the edit control if it exists. 
	if(::IsWindow(GetSafeHwnd()))
	{
		m_nSetTextSemaphor++;
		SetWindowText(GetFormattedText());
		m_nSetTextSemaphor--;
		// We usually need to update the insertion point. 
		if(nSelectionStart>=0)
			UpdateInsertionPointForward(nSelectionStart);
	}
}


void CEditEx::UpdateInsertionPointForward(int nSelectionStart)
{
	int nNewInsertionPoint=GetNextInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint,nNewInsertionPoint);
}


void CEditEx::UpdateInsertionPointBackward(int nSelectionStart)
{
	int nNewInsertionPoint=GetPreviousInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint,nNewInsertionPoint);
}


int CEditEx::RPtoLP(int nRealPos)const 
{
	// All CEditEx functions that take cusor position as argument interpret it
	// as real position within edit control(taking into account all symbols including 
	// literals). But sometimes we want to know which non-literal symbol is at  
	// particular real position. In that case this function is really useful

	if(nRealPos<m_arrLeftLiterals.GetSize() || 
		nRealPos>m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize())
		return -1;
	else 
		return(nRealPos-m_arrLeftLiterals.GetSize());
}

		
int CEditEx::LPtoRP(int nLogicalPos)const 
{
	if(nLogicalPos<0 || nLogicalPos>m_arrInputData.GetSize())
		return -1;
	else
		return(nLogicalPos+m_arrLeftLiterals.GetSize());
}


void CEditEx::ValidationError()
{
	::MessageBeep(MB_ICONEXCLAMATION);
}


CString CEditEx::GetLeftLiterals() const
{
	CString sLeftLiterals;
	for(int nIndex=0; nIndex<m_arrLeftLiterals.GetSize(); nIndex++)
	{
		sLeftLiterals+=m_arrLeftLiterals[nIndex];
	}
	return sLeftLiterals;
}


CString CEditEx::GetRightLiterals() const
{
	CString sRightLiterals;
	for(int nIndex=0; nIndex<m_arrRightLiterals.GetSize(); nIndex++)
	{
		sRightLiterals+=m_arrRightLiterals[nIndex];
	}
	return sRightLiterals;
}


BOOL CEditEx::NotifyParent(UINT nNotificationID)
{
	CWnd* pParentWnd=GetParent();
	if(pParentWnd==NULL)
		return FALSE;

	pParentWnd->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),nNotificationID),
		(LPARAM)GetSafeHwnd());
	return TRUE;
}


void CEditEx::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	m_bHasFocus = FALSE;
	if(m_bIsFlat)
		_RedrawWindow();
		//DrawEdit(m_clrNormal, m_clrNormal);	
}

BOOL CEditEx::_RedrawWindow()
{
	CWnd* pWnd = GetParent();
	if(::IsWindow(pWnd->GetSafeHwnd()))
	{
		RedrawWindow();
		/*CRect rc;
		GetWindowRect(&rc);
		pWnd->ScreenToClient(&rc);
		pWnd->InvalidateRect(rc, FALSE);*/
		return TRUE;
	}
	else
		return FALSE;
}
