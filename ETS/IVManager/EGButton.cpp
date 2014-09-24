// EGButton.cpp : implementation file
//

#include "stdafx.h"
#include "EGButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
void AFXAPI DDX_Toggle(CDataExchange *pDX, int nIDC, int& value)
{
	HWND hWnd = pDX->PrepareCtrl(nIDC);
	ASSERT(hWnd != NULL);

	CEGButton* pButton = (CEGButton*)CWnd::FromHandle(hWnd);
	if(pDX->m_bSaveAndValidate)
	{
		if(!pButton->GetToggle())
			value = -1;
		else if(pButton->GetCheck() == 2)	// indeterminate
			value = 2;
		else if(pButton->GetCheck() == 1)	// checked
			value = 1;
		else
			value = 0;
	}
	else
	{
		if(pButton->GetToggle())
		{
			switch(value)
			{
			case 1:
				pButton->SetCheck(1);	// checked
				break;
			case 2:
				pButton->SetCheck(2);	// indeterminate
				break;
			case 0:
			default:
				value = 0;
				pButton->SetCheck(0);	// not checked
			}
		}
	}
}

IMPLEMENT_DYNAMIC(CEGButton, CButton)

// We define the structs GRPICONDIR, GRPICONDIRENTRY and ICONIMAGE
// which represent an icon directory and an icon from resource
// See also 'Icons in Win32' (John Hornick - Microsoft Corporation)

// ... #pragmas are used here to insure that the structure's
//     packing in memory matches the packing of the EXE or DLL.
#pragma pack(push)
#pragma pack(2)
	typedef struct
	{
		BYTE   bWidth;					// Width, in pixels, of the image
		BYTE   bHeight;					// Height, in pixels, of the image
		BYTE   bColorCount;				// Number of colors in image (0 if >=8bpp)
		BYTE   bReserved;				// Reserved
		WORD   wPlanes;					// Color Planes
		WORD   wBitCount;				// Bits per pixel
		DWORD  dwBytesInRes;			// how many bytes in this resource?
		WORD   nID;						// the ID
	} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;
	
	typedef struct 
	{
		WORD            idReserved;		// Reserved (must be 0)
		WORD            idType;			// Resource type (1 for icons)
		WORD            idCount;		// How many images?
		GRPICONDIRENTRY idEntries[1];	// The entries for each image (idCount of 'em)
	} GRPICONDIR, *LPGRPICONDIR;

	typedef struct
	{
		BITMAPINFOHEADER	icHeader;	// DIB header
		RGBQUAD				icColors[1]; // Color table (more than 1!)
		BYTE				icXOR[1];	// DIB bits for XOR mask (more than 1!)
		BYTE				icAND[1];	// DIB bits for AND mask (more than 1!)
	} ICONIMAGE, *LPICONIMAGE;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

//	Button with image (and text) layout

//   +----------------------------------------+
//   |                                        |
//   |                 By                     |
//   |                                        |
//   |         +------------------+           |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |   Bx    |   B I T M A P    |    Bx     |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |         |                  |           |
//   |         +------------------+           |
//   |                                        |
//   |                 By                     |
//   |                                        |
//   |       +----------------------+         |
//   |  Tx   |       T E X T        |   Tx    |
//   |       +----------------------+         |
//   |                                        |
//   |                 Ty                     |
//   |                                        |
//   +----------------------------------------+

// Bx = m_ptImageOffset.x
// By = m_ptImageOffset.y
// Tx = m_ptTextOffset.x
// Ty = m_ptTextOffset.y

// Position of the images in the image list
const int CEGButton::m_nNormalImageIndex = 0;
const int CEGButton::m_nInactiveImageIndex = 1;
const int CEGButton::m_nDisabledImageIndex = 2;

// ... The number of pixels the picture is offset from the button border (or the text)
const CPoint CEGButton::m_ptImageOffset = CPoint(-2, -2);
// ... The number of pixels the text is offset from the button border 
const CPoint CEGButton::m_ptTextOffset = CPoint(-7, -3);
// ... The number of pixels the inner focus rect is offset from the button border
const CPoint CEGButton::m_ptInnerFocusOffset = CPoint(-4, -4);
// ... The number of pixels the inner focus rect is offset from the button border in flat mode
const CPoint CEGButton::m_ptFlatInnerFocusOffset = CPoint(-2, -2);
// ... How much the image and the text must move when the button is down
const CPoint CEGButton::m_ptDownOffset = CPoint(1, 1);
// ... How much the image and the text must move when the button is down in flat mode
const CPoint CEGButton::m_ptFlatDownOffset = CPoint(1, 1);
// ... How much the image and the text must move when the button is checked
const CPoint CEGButton::m_ptCheckedOffset = CPoint(0, 0);
// ... How much the image and the text must move when the button is checked in flat mode
const CPoint CEGButton::m_ptFlatCheckedOffset = CPoint(0, 0);
// ... The number of pixels the dropdown arrow rect is offset from the button border
const CPoint CEGButton::m_ptArrowOffset = CPoint(-2, -3);

BEGIN_MESSAGE_MAP(CEGButton, CButton)
	//{{AFX_MSG_MAP(CEGButton)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_SYSKEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_ENABLE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_CHECK_FLAT_LOOK, OnCheckFlatLook)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
CEGButton::CEGButton()
	: m_bFlatLook(FALSE),
	m_bTrackLook(TRUE),
	m_bMouseOverButton(FALSE),
	m_bMouseDown(FALSE),
	m_hCursor(NULL),
	m_bToggle(FALSE),
	m_bTriState(FALSE),
	m_nCheck(0),
	m_bDropDown(FALSE),
	m_enArrowStyle(enDropDownArrowNone),
	m_enMenuAlign(enDropDownMenuAlignLeftDown),
	m_bDrawDropdownSeparator(FALSE),
	m_bAllowDraw(TRUE)
{
	ASSERT_VALID(this);

	m_nDropDownArrowWidth = GetDropDownArrowWidth();
	AdjustSysColors();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::LoadBitmap(LPCTSTR lpszBitmapResource,
						   BOOL bUseMask /*= FALSE*/,
						   COLORREF crMask /*= CLR_NONE*/,
						   BOOL bResize /*= FALSE*/)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

#ifdef _DEBUG
	if(AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP) == NULL)
		TRACE0("CEGButton::LoadBitmap : Specified bitmap resource not found\n");
#endif // _DEBUG

	// BS_BITMAP style is no longer supported
	// This button must have the BS_OWNERDRAW style
	ASSERT((GetStyle() & BS_BITMAP) != BS_BITMAP);
	ASSERT((GetStyle() & BS_OWNERDRAW) == BS_OWNERDRAW);

	// Clear possible previous contents
	if (m_imageList.m_hImageList != NULL)
		VERIFY(m_imageList.DeleteImageList());

	// Get and compute the necessary bitmaps
	CBitmap bitmap;
	CBitmap grayBitmap;
	CBitmap disabledBitmap;

	// Try to load the new bitmap
	if(!LoadBitmap(lpszBitmapResource, bitmap, m_palette))
	{
		TRACE(_T("CEGButton::LoadBitmap : Failed to load bitmap \n"));
		return FALSE;
	}

	if(!bUseMask)
	{
		CWindowDC dc(NULL);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		if(GDI_ERROR != (ULONG)memDC.SelectObject(bitmap))
			crMask = memDC.GetPixel(0, 0);
	}

	// Build the gray bitmap 
	if(!BuildGrayBitmap(lpszBitmapResource, crMask, &grayBitmap))
	{
		TRACE(_T("CEGButton::LoadBitmap : Failed to build the gray scale bitmap, continuing\n"));
	}

	BITMAP bmInfo;
	ZeroMemory(&bmInfo, sizeof(bmInfo));
	VERIFY(bitmap.GetObject(sizeof(bmInfo), &bmInfo) == sizeof(bmInfo));
	CSize bitmapSize(bmInfo.bmWidth, bmInfo.bmHeight);
	BOOL bMask = crMask != CLR_NONE; 

	// Let the image list use a device-dependent bitmap for its internal data structure
	// instead of the default ILC_COLOR4 (4-bit (16 color) device-independent bitmap (DIB) section)
	// Otherwise the colors will shift (e.g. disabled button look)
	VERIFY(m_imageList.Create(bitmapSize.cx, bitmapSize.cy, bMask | ILC_COLORDDB, 0, EG_MAX_IMAGE_COUNT));
	VERIFY(m_imageList.Add(&bitmap, crMask) == m_nNormalImageIndex);
	VERIFY(m_imageList.Add(&grayBitmap, crMask) == m_nInactiveImageIndex);

	// Build the disabled image
	HICON hSourceIcon = m_imageList.ExtractIcon(m_nNormalImageIndex);
	HICON hDestIcon = NULL;
	if(BuildDisabledImage(hSourceIcon, bitmapSize, hDestIcon))
	{
		// Add the disabled image to the image list and use the
		// mask of the normal image
		VERIFY(m_imageList.Add(hDestIcon) == m_nDisabledImageIndex);
		VERIFY(::DestroyIcon(hDestIcon));
	}
	else
	{
		TRACE0("CEGButton::LoadBitmap : Failed to build the disabled image, continuing\n");
	}
	VERIFY(::DestroyIcon(hSourceIcon));

	// Size to content
	if (bResize)
		SizeToContent();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::LoadInactiveBitmap(LPCTSTR lpszBitmapResource)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

	// image list must have at least one image
	if(m_imageList.GetImageCount()<1)
	{
		TRACE(_T("CEGButton::LoadInactiveBitmap: before calling the function you must associate default image with the button!\n"));
		return FALSE;
	}

#ifdef _DEBUG
	if(AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP) == NULL)
		TRACE0("CEGButton::LoadInactiveBitmap : Specified bitmap resource not found\n");
#endif // _DEBUG

	// Get the necessary bitmap
	CBitmap bitmap;
	// Try to load the new bitmap
	CPalette palette;
	if(!LoadBitmap(lpszBitmapResource, bitmap, palette))
	{
		TRACE(_T("CEGButton::LoadInactiveBitmap: Failed to load bitmap \n"));
		return FALSE;
	}

	VERIFY(m_imageList.Replace(m_nInactiveImageIndex, &bitmap, NULL));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::LoadDisabledBitmap(LPCTSTR lpszBitmapResource)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

	// image list must have at least one image
	if(m_imageList.GetImageCount() < 1)
	{
		TRACE(_T("CEGButton::LoadDisabledBitmap: before calling the function you must associate default image with the button!\n"));
		return FALSE;
	}

#ifdef _DEBUG
	if (AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP) == NULL)
		TRACE0("CEGButton::LoadDisabledBitmap : Specified bitmap resource not found\n");
#endif // _DEBUG

	// Get the necessary bitmap
	CBitmap bitmap;
	// Try to load the new bitmap
	CPalette palette;
	if(!LoadBitmap(lpszBitmapResource, bitmap, palette))
	{
		TRACE(_T("CEGButton::LoadDisabledBitmap: Failed to load bitmap \n"));
		return FALSE;
	}

	VERIFY(m_imageList.Replace(m_nDisabledImageIndex, &bitmap, NULL));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::LoadIcon(LPCTSTR lpszIconResource, BOOL bResize /* = TRUE */,
							UINT nWidth /* = 0 */, UINT nHeight /* = 0 */)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

#ifdef _DEBUG
	if (AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON) == NULL)
		TRACE0("CEGButton::LoadIcon : Specified icon resource not found\n");
#endif // _DEBUG

	// BS_BITMAP style is no longer supported
	// This button must have the BS_OWNERDRAW style
	ASSERT((GetStyle() & BS_BITMAP) != BS_BITMAP);
	ASSERT((GetStyle() & BS_OWNERDRAW) == BS_OWNERDRAW);

	// Clear possible previous contents
	if (m_imageList.m_hImageList != NULL)
		VERIFY(m_imageList.DeleteImageList());
	
	// Get and compute the necessary bitmaps
	HICON hIcon;
	CBitmap grayBitmap;
	CBitmap disabledBitmap;

	// Try to load the new bitmap
	if(nWidth == 0)
		nWidth = ::GetSystemMetrics(SM_CXICON);
	if(nHeight == 0)
		nHeight = ::GetSystemMetrics(SM_CYICON);
	hIcon = (HICON)::LoadImage(AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON),
								lpszIconResource, IMAGE_ICON, nWidth, nHeight, LR_DEFAULTCOLOR);
	if (hIcon == NULL)
	{
		TRACE0("CEGButton::LoadIcon : Failed to load icon\n");
		return FALSE;
	}

	// Get the icon's palette
	if (!GetIconPalette(lpszIconResource, m_palette))
		TRACE0("CEGButton::LoadIcon : Failed to get the palette, continuing\n");

	// Build the gray bitmap and gray palette
	HICON hGrayIcon = NULL;
	if (!BuildGrayIcon(lpszIconResource, &hGrayIcon))
		TRACE0("CEGButton::LoadIcon : Failed to build the gray scale icon, continuing\n");

	ICONINFO iconInfo;
	ZeroMemory(&iconInfo, sizeof(iconInfo));
	VERIFY(::GetIconInfo(hIcon, &iconInfo));

	BITMAP bmInfo;
	ZeroMemory(&bmInfo, sizeof(bmInfo));
	VERIFY(::GetObject(iconInfo.hbmColor, sizeof(bmInfo), &bmInfo) == sizeof(bmInfo));
	CSize iconSize(bmInfo.bmWidth, bmInfo.bmHeight);

	// Let the image list use a device-dependent bitmap for its internal data structure
	// instaed of the default ILC_COLOR4 (4-bit (16 color) device-independent bitmap (DIB) section)
	// Otherwise the colors will shift (e.g. disabled button look)
	VERIFY(m_imageList.Create(iconSize.cx, iconSize.cy, TRUE | ILC_COLORDDB, 0, EG_MAX_IMAGE_COUNT));
	VERIFY(m_imageList.Add(hIcon) == m_nNormalImageIndex);
	VERIFY(m_imageList.Add(hGrayIcon) == m_nInactiveImageIndex);

	if(hGrayIcon!=NULL)
		::DestroyIcon(hGrayIcon);

	// Build the disabled bitmap
	HICON hDestIcon = NULL;
	if(BuildDisabledImage(hIcon, iconSize, hDestIcon))
	{
		// Add the disabled image to the image list and use the
		// mask of the normal image
		VERIFY(m_imageList.Add(hDestIcon) == m_nDisabledImageIndex);
		VERIFY(::DestroyIcon(hDestIcon));
	}
	else
		TRACE0("CEGButton::LoadIcon : Failed to build the disabled image, continuing\n");

	::DeleteObject(iconInfo.hbmColor);
	iconInfo.hbmColor = NULL;
	// ... Delete the mask bitmap
	::DeleteObject(iconInfo.hbmMask);
	iconInfo.hbmMask = NULL;

	// Size to content
	if(bResize)
		SizeToContent();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::LoadInactiveIcon(LPCTSTR lpszIconResource, UINT nWidth /* = 0 */, UINT nHeight /* = 0 */)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

#ifdef _DEBUG
	if(AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON) == NULL)
		TRACE0("CEGButton::LoadInactiveIcon : Specified icon resource not found\n");
#endif // _DEBUG

	// Get and compute the necessary bitmaps
	HICON hIcon;

	// Try to load the new bitmap
	if(nWidth == 0)
		nWidth=::GetSystemMetrics(SM_CXICON);
	if(nHeight == 0)
		nHeight = ::GetSystemMetrics(SM_CYICON);
	hIcon = (HICON)::LoadImage(AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON),
								lpszIconResource, IMAGE_ICON, nWidth, nHeight, LR_DEFAULTCOLOR);
	if (hIcon == NULL)
	{
		TRACE0("CEGButton::LoadInactiveIcon : Failed to load icon\n");
		return FALSE;
	}

	VERIFY(m_imageList.Replace(m_nInactiveImageIndex, hIcon));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::LoadDisabledIcon(LPCTSTR lpszIconResource,
										UINT nWidth /* = 0 */, UINT nHeight /* = 0 */)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

#ifdef _DEBUG
	if (AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON) == NULL)
		TRACE0("CEGButton::LoadDisabledIcon : Specified icon resource not found\n");
#endif // _DEBUG

	// Get and compute the necessary bitmaps
	HICON hIcon;

	// Try to load the new bitmap
	if(nWidth == 0)
		nWidth = ::GetSystemMetrics(SM_CXICON);
	if(nHeight == 0)
		nHeight=::GetSystemMetrics(SM_CYICON);
	hIcon = (HICON)::LoadImage(AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON),
								lpszIconResource, IMAGE_ICON, nWidth, nHeight, LR_DEFAULTCOLOR);
	if (hIcon == NULL)
	{
		TRACE0("CEGButton::LoadDisabledIcon: Failed to load icon\n");
		return FALSE;
	}

	VERIFY(m_imageList.Replace(m_nDisabledImageIndex, hIcon));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::RemoveImage(BOOL bResize /* = TRUE */)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

	// BS_BITMAP style is no longer supported
	// This button must have the BS_OWNERDRAW style
	ASSERT((GetStyle() & BS_BITMAP) != BS_BITMAP);
	ASSERT((GetStyle() & BS_OWNERDRAW) == BS_OWNERDRAW);

	// Clear all contents
	if(m_imageList.m_hImageList != NULL)
		VERIFY(m_imageList.DeleteImageList());
	
	// Size to content
	if(bResize)
		SizeToContent();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
CPalette* CEGButton::GetPalette()
{
	return &m_palette;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SizeToContent()
{
	ASSERT(::IsWindow(m_hWnd));

	CSize oldButtonSize = GetButtonSize();
	CSize newButtonSize = GetFitButtonSize();
	VERIFY(SetWindowPos(NULL, -1, -1, newButtonSize.cx, newButtonSize.cy,
				SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));

	// Invalidate the union of the old and the new button rect
	CRect invalidRect;
	invalidRect.UnionRect(CRect(CPoint(0, 0), oldButtonSize), CRect(CPoint(0, 0), newButtonSize));
	// ... Invalidate through the parent window, because this button may have shrunk
	//     and thus part of the parent window has to be invalidated as well
	ClientToScreen(invalidRect);
	ASSERT(GetParent() != NULL);
	GetParent()->ScreenToClient(invalidRect);
	GetParent()->InvalidateRect(invalidRect);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetFlatLook() const
{
	return m_bFlatLook;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::SetFlatLook(BOOL bFlatLook /* = TRUE */)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

	if((GetStyle() & BS_OWNERDRAW) != BS_OWNERDRAW)
	{
		TRACE0("CEGButton::SetFlatLook : Can only set flat look for owner draw buttons, failing\n");
		return FALSE;
	}

	if(bFlatLook != m_bFlatLook)
	{
		m_bFlatLook = bFlatLook;
		Invalidate();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetTrackLook() const
{
	return m_bTrackLook;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::SetTrackLook(BOOL bTrackLook /* = TRUE */)
{
	// This control should already be created
	ASSERT(::IsWindow(m_hWnd));

	if((GetStyle() & BS_OWNERDRAW) != BS_OWNERDRAW)
	{
		TRACE0("CEGButton::SetTrackLook : Can only set track look for owner draw buttons, failing\n");
		return FALSE;
	}

	if(bTrackLook != m_bTrackLook)
	{
		m_bTrackLook = bTrackLook;
		Invalidate();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetHorizontalAlignment(DWORD nAlignment /* = BS_CENTER */)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((nAlignment == BS_LEFT) || (nAlignment == BS_CENTER) || (nAlignment == BS_RIGHT));

	if(GetHorizontalAlignment() != nAlignment)
	{
		ModifyStyle((BS_LEFT | BS_CENTER | BS_RIGHT), nAlignment);
		// ... Alignment changed, redraw button
		Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
DWORD CEGButton::GetHorizontalAlignment() const
{
	ASSERT(::IsWindow(m_hWnd));
	DWORD nReturn=GetStyle() & (BS_LEFT | BS_CENTER | BS_RIGHT);
	if(nReturn==0)
		return BS_CENTER;
	else
		return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetVerticalAlignment(DWORD nAlignment /* = BS_VCENTER */)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((nAlignment == BS_TOP) || (nAlignment == BS_VCENTER) || (nAlignment == BS_BOTTOM));

	if(GetVerticalAlignment() != nAlignment)
	{
		ModifyStyle((BS_TOP | BS_VCENTER | BS_BOTTOM), nAlignment);
		// ... Alignment changed, redraw button
		Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
DWORD CEGButton::GetVerticalAlignment() const
{
	ASSERT(::IsWindow(m_hWnd));
	DWORD nReturn=GetStyle() & (BS_TOP | BS_VCENTER | BS_BOTTOM);
	if(nReturn==0)
		return BS_VCENTER;
	else
		return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
CFont* CEGButton::GetTextFont()
{
	ASSERT(::IsWindow(m_hWnd));

	// Get the font associated with this control 
	CFont* pFont = GetFont();
	if (pFont == NULL)
	{
		TRACE0("CEGButton::GetTextFont : Failed to get the font\n");
		// ... Return an empty font
		m_textFont.DeleteObject();
	}
	else if (pFont->GetSafeHandle() == (HFONT)m_textFont.m_hObject)
	{
		// If the handles are the same, the wrapping object must be the same as well
		ASSERT(pFont == &m_textFont);
	}
	else
	{
		// The window font is different from the font we have
		// (This means the font has not been set with SetTextFont() but with WM_SETFONT)
		// Make a copy of the window font
		LOGFONT logFont;
		m_textFont.DeleteObject();
		VERIFY(pFont->GetLogFont(&logFont));
		VERIFY(m_textFont.CreateFontIndirect(&logFont));
	}

	return &m_textFont;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetTextFont(CFont* pTextFont)
{
	ASSERT(pTextFont != NULL);
	ASSERT(::IsWindow(m_hWnd));

	// Make a copy of the supplied font if necessary
	BOOL bChanged = FALSE;
	if(pTextFont->GetSafeHandle() != (HFONT)m_textFont.m_hObject)
	{
		LOGFONT logFont;
		m_textFont.DeleteObject();
		VERIFY(pTextFont->GetLogFont(&logFont));
		VERIFY(m_textFont.CreateFontIndirect(&logFont));
		bChanged = TRUE;
	}

	// ... Set the window font
	SetFont(&m_textFont, bChanged);
}

/////////////////////////////////////////////////////////////////////////////
//
CString CEGButton::GetText() const
{
	ASSERT(::IsWindow(m_hWnd));

	CString sText;
	GetWindowText(sText);
	// Get everything in front of first EndOfLine char
	return GetSubString(sText, 1, _T('\n'));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetText(LPCTSTR pszText)
{
	ASSERT(::IsWindow(m_hWnd));

	CString sWindowText(pszText);
	// ... Get the tooltip text so it can be used again
	CString sToolTipText = GetToolTipText();
	if(!sToolTipText.IsEmpty())
		sWindowText += _T("\n") + sToolTipText;
	SetWindowText(sWindowText);
}

/////////////////////////////////////////////////////////////////////////////
//
CString CEGButton::GetToolTipText() const
{
	ASSERT(::IsWindow(m_hWnd));

	CString sText;
	GetWindowText(sText);
	// Get everything after the first EndOfLine char and before te second
	return GetSubString(sText, 2, _T('\n'));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetToolTipText(LPCTSTR pszToolTipText)
{
	ASSERT(::IsWindow(m_hWnd));

	CString sToolTipText(pszToolTipText);
	// ... Get the text so it can be used again
	CString sWindowText = GetText();
	if(!sToolTipText.IsEmpty())
		sWindowText += _T("\n") + sToolTipText;

	// A tool will be added to (or deleted from) the tool tip control 
	// in the WM_SETTEXT handler

	SetWindowText(sWindowText);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetToolTip() const
{
	ASSERT(::IsWindow(m_hWnd));

	// Tooltip is enabled if the tool tip control is creted
	return (m_toolTip.m_hWnd != NULL);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::SetToolTip(BOOL bEnable /* = TRUE */)
{
	ASSERT(::IsWindow(m_hWnd));

	if(bEnable)
	{
		if(m_toolTip.m_hWnd != NULL)
		{
			::IsWindow(m_toolTip.m_hWnd);
			TRACE0("CEGButton::SetToolTip : Tooltip window already exists, continuing\n");
			return TRUE;
		}
		if(!m_toolTip.Create(this))
		{
			TRACE0("CEGButton::SetToolTip : Failed to create tool tip control\n");
			return FALSE;
		}

		// ... Set the initial tool tip text
		CString sToolTipText = GetToolTipText();
		CRect rectWindow;
		GetClientRect(rectWindow);
		if(!sToolTipText.IsEmpty())
			VERIFY(m_toolTip.AddTool(this, sToolTipText, rectWindow, EGBB_TOOLTIP_ID));

		m_toolTip.Activate(TRUE);
	}
	else if(m_toolTip.m_hWnd != NULL)
	{
		m_toolTip.DelTool(this, EGBB_TOOLTIP_ID);
		m_toolTip.DestroyWindow();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
CSize CEGButton::GetFitButtonSize()
{
	CSize imageAndGapSize;
	CSize textAndGapSize;
	CSize totalSize;

	// Get the needed sizes for image and text
	if(GetVerticalAlignment() == BS_VCENTER)
	{
		// if vertical alignment is BS_VCENTER then text and image are
		// located on the same line
		imageAndGapSize = GetImageSize();
		textAndGapSize = GetTextSize(TRUE);

		if(imageAndGapSize.cx > 0 && textAndGapSize.cx > 0)
		{
			if((imageAndGapSize.cx != 0) && (imageAndGapSize.cy != 0))
				imageAndGapSize += CSize(2 * -m_ptImageOffset.x, 2 * -m_ptImageOffset.y);
			if((textAndGapSize.cx != 0) && (textAndGapSize.cy != 0))
				// ... Only add text gap if text is non-empty
				textAndGapSize += CSize(-m_ptTextOffset.x, 2 * -m_ptTextOffset.y);
		}
		else if(imageAndGapSize.cx == 0)
		{
			textAndGapSize += CSize(2 * -m_ptTextOffset.x, 2 * -m_ptTextOffset.y);
		}
		else if(textAndGapSize.cx == 0)
		{
			imageAndGapSize += CSize(2 * -m_ptImageOffset.x, 2 * -m_ptImageOffset.y);
		}

		// Use the sum for the widtht and the largest for the height 
		if(imageAndGapSize.cy < textAndGapSize.cy)
			totalSize.cy = textAndGapSize.cy;
		else
			totalSize.cy = imageAndGapSize.cy;
		totalSize.cx = imageAndGapSize.cx + textAndGapSize.cx;
	}
	else
	{
		CSize imageSize=GetImageSize();
		CSize textSize=GetTextSize(TRUE);
		imageAndGapSize = imageSize;
		textAndGapSize = textSize;
		if((textSize.cx != 0) && (textSize.cy != 0))
		{
			// ... Set text and gap to correct value if text exists
			// ... Subtract text and space size from limit size (only height)
			textAndGapSize += CSize(-m_ptTextOffset.x * 2, -m_ptTextOffset.y);
		}

		if((imageSize.cx != 0) && (imageSize.cy != 0))
		{
			imageAndGapSize += CSize(2 * -m_ptImageOffset.x, 2 * -m_ptImageOffset.y);
		}
		else
		{
			if((textSize.cx != 0) && (textSize.cy != 0))
			{
				// ... Set text and gap to correct value if text exists
				// ... Subtract text and space size from limit size (only height)
				textAndGapSize += CSize(0, -m_ptTextOffset.y);
			}
		}

		// Use the sum for the height and the largest for the width 
		if(imageAndGapSize.cx < textAndGapSize.cx)
			totalSize.cx = textAndGapSize.cx;
		else
			totalSize.cx = imageAndGapSize.cx;
		totalSize.cy = imageAndGapSize.cy + textAndGapSize.cy;

		if(totalSize.cy == 0)
		{
			totalSize.cy += 2 * -m_ptTextOffset.y;
		}
		if(totalSize.cx == 0)
		{
			totalSize.cx += 2 * -m_ptTextOffset.x;
		}
	}

	totalSize += GetReservedSpace();

	return totalSize;
}

/////////////////////////////////////////////////////////////////////////////
//
CSize CEGButton::GetButtonSize() const
{
	ASSERT(::IsWindow(m_hWnd));

	CRect clientRect;
	GetClientRect(clientRect);
	return clientRect.Size();
}

/////////////////////////////////////////////////////////////////////////////
//
CSize CEGButton::GetImageSize() const
{
	ASSERT(::IsWindow(m_hWnd));

	if(m_imageList.m_hImageList == NULL)
		return CSize(0, 0);

	IMAGEINFO imageInfo;
	::ZeroMemory(&imageInfo, sizeof(imageInfo));
	VERIFY(m_imageList.GetImageInfo(m_nNormalImageIndex, &imageInfo));
	return CRect(imageInfo.rcImage).Size();
}

/////////////////////////////////////////////////////////////////////////////
//
CSize CEGButton::GetTextSize(BOOL bCompact)
{
	ASSERT(::IsWindow(m_hWnd));

	CWindowDC dc(this);
	CRect textRect(0, 0, 0, 0);
	CString sText = GetText();
	if(!sText.IsEmpty())
	{
		// ... Use correct font
		CFont* pOldFont = dc.SelectObject(GetTextFont());
		UINT nFormat=DT_SINGLELINE | DT_CALCRECT | DT_LEFT | DT_TOP;
		VERIFY(dc.DrawText(sText, textRect, nFormat) != 0);

		CSize sizeImage=GetImageSize();
		if(bCompact)
		{
			if((GetStyle() & BS_MULTILINE) == BS_MULTILINE)
			{
				nFormat = DT_WORDBREAK | DT_CALCRECT | DT_LEFT | DT_TOP;

				textRect -= textRect.TopLeft();
				CRect oldRect(0, 0, 0, 0);
				while(TRUE)
				{
					VERIFY(dc.DrawText(sText, textRect, nFormat) != 0);

					float nXCoef = (float)textRect.Width() / (float)textRect.Height();
					float nYCoef = (float)textRect.Height() / (float)textRect.Width();
					
					if(oldRect == textRect || (nXCoef < 2.6F && nYCoef < 1.F))
					{
						break;
					}

					oldRect = textRect;

					if(nXCoef >= 2)
					{
						textRect.right-=textRect.Width() / 3;
					}
					else
					{
						textRect.right+=textRect.Width() / 4;
					}
				}
			}
		}
		else
		{
			CRect buttonRect;
			GetClientRect(buttonRect);

			buttonRect.right -= GetReservedSpace().cx;
			buttonRect.bottom -= GetReservedSpace().cy;

			buttonRect.InflateRect(m_ptTextOffset.x, m_ptTextOffset.y);
			if(sizeImage.cx!=0 && sizeImage.cy != 0)
			{
				switch(GetVerticalAlignment())
				{
				case BS_TOP:
				case BS_BOTTOM:
					{
						buttonRect.bottom -= sizeImage.cy - 2 * m_ptImageOffset.y + m_ptTextOffset.y;
						if(buttonRect.bottom < buttonRect.top)
							buttonRect.SetRectEmpty();
						break;
					}
				case BS_VCENTER:
					{
						buttonRect.right -= sizeImage.cx - 2 * m_ptImageOffset.x + m_ptTextOffset.x;
						if(buttonRect.right < buttonRect.left)
							buttonRect.SetRectEmpty();
						break;
					}
				default:
					ASSERT(FALSE);
				}
			}

			if((GetStyle() & BS_MULTILINE) == BS_MULTILINE)
			{
				textRect = buttonRect;

				nFormat = DT_WORDBREAK | DT_CALCRECT | DT_LEFT | DT_TOP;
				VERIFY(dc.DrawText(sText, textRect, nFormat) != 0);
			}

			if(sizeImage.cx != 0 && sizeImage.cy != 0)
			{
				switch(GetVerticalAlignment())
				{
				case BS_TOP:
				case BS_BOTTOM:
					{
						textRect.right = __min(textRect.Width(), buttonRect.Width());
						textRect.bottom = buttonRect.Height();
						break;
					}
				case BS_VCENTER:
					{
						textRect.right = buttonRect.Width();
						textRect.bottom = __min(textRect.Height(), buttonRect.Height());
						break;
					}
				default:
					ASSERT(FALSE);
				}
		
				textRect.left = 0;
				textRect.top = 0;
			}
		}

		dc.SelectObject(pOldFont);
	}
	return textRect.Size();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::SetCursor(HCURSOR hCursor /*= NULL*/)
{
	// Check whether we are reseting the cursor
	m_hCursor = hCursor;
	return TRUE;
}

#ifdef _DEBUG
/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::AssertValid() const
{
	CButton::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::Dump(CDumpContext& dc) const
{
	CButton::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
//
CEGButton::~CEGButton()
{
	if(m_Menu.m_hMenu)
		// We don't own the object so don't delete it.
		m_Menu.Detach();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// Only draw if drawing is allowed
	if(!m_bAllowDraw)
		return;

	ASSERT(lpDrawItemStruct != NULL);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT nState = lpDrawItemStruct->itemState;
	CRect itemRect = lpDrawItemStruct->rcItem;
	CRect buttonRect;
	CRect imageRect;
	CRect textRect;
	CRect arrowRect = itemRect;

	CDC dcCompatible;
	if(!dcCompatible.CreateCompatibleDC(pDC))
	{
		TRACE(_T("CEGButton::DrawItem:Failed to create compatible DC"));
		return;
	}
	CBitmap bitmap;
	if(!bitmap.CreateCompatibleBitmap(pDC, itemRect.Width(), itemRect.Height()))
	{
		TRACE(_T("CEGButton::DrawItem:Failed to create compatible bitmap"));
		return;
	}
	CBitmap* pOldBitmap = dcCompatible.SelectObject(&bitmap);
	CFont* pOldFont = dcCompatible.SelectObject(pDC->GetCurrentFont());
	CRect rcOriginal = itemRect;
	itemRect -= itemRect.TopLeft();

	// Adjust the state for a possible pseudo-disable mode
	if(IsWindowEnabled())
		nState &= ~ODS_DISABLED;
	else
		nState |= ODS_DISABLED;

	// First distribute the available space (image etc)
	CRect rectFree = itemRect;
	rectFree.right -= GetReservedSpace().cx;
	rectFree.bottom -= GetReservedSpace().cy;
	arrowRect = rectFree;
	DistributeSpace(nState, rectFree, buttonRect, imageRect, textRect);

	// ... Select and realize palette
	CPalette* pOldCompatiblePalette = NULL;
	SelectPalette(&dcCompatible, nState, buttonRect, pOldCompatiblePalette);

	// ... Draw the button borders 
	DrawButton(&dcCompatible, nState, itemRect);
	// ... Draw the image in its correct state
	DrawImage(&dcCompatible, nState, &m_imageList, imageRect);
	DrawText(&dcCompatible, nState, GetText(), textRect);

	if(GetDropDownArrowStyle() != enDropDownArrowNone)
	{
		arrowRect.left = arrowRect.right;
		arrowRect.right = arrowRect.left + (m_nDropDownArrowWidth - m_ptArrowOffset.x);
		arrowRect.InflateRect(0, m_ptArrowOffset.y, m_ptArrowOffset.x, m_ptArrowOffset.y);
		DrawDropDownArrow(&dcCompatible, nState, arrowRect);
	}

	if(nState & ODS_FOCUS)
	{
		// ... Draw a focus rectangle on top of the button and image
		DrawFocusRectangle(&dcCompatible, nState, itemRect, imageRect);
	}

	CPalette* pPalette = dcCompatible.GetCurrentPalette();
	ASSERT(pPalette != NULL);
	CPalette* pOldPalette = pDC->SelectPalette(pPalette, FALSE);
	pDC->RealizePalette();

	pDC->BitBlt(rcOriginal.left, rcOriginal.top, rcOriginal.Width(), rcOriginal.Height(), &dcCompatible, 0, 0, SRCCOPY);
	
	// ... Deselect palette
	RestorePalette(&dcCompatible, nState, buttonRect, pOldCompatiblePalette);
	if(pOldBitmap)
		dcCompatible.SelectObject(pOldBitmap);
	if(pOldFont)
		dcCompatible.SelectObject(pOldFont);

	if(pOldPalette!=NULL)
		pDC->SelectPalette(pOldPalette, FALSE);

}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::DistributeSpace(UINT nState, CRect itemRect, CRect& buttonRect, CRect& imageRect, CRect& textRect)
{
	// Let the button use the entire space
	buttonRect = itemRect;

	// Take horizontal and vertical alignment into account
	// ... Get the alignment
	DWORD nHorizontalAlignment = GetHorizontalAlignment();
	DWORD nVerticalAlignment = GetVerticalAlignment();

	// ... Outer allowable position for image and text
	const CRect rectLimit = buttonRect;
	CPoint originLimit = rectLimit.TopLeft();
	CSize sizeLimit = rectLimit.Size();

	// Calculate the image size
	CSize imageSize = GetImageSize();
	CSize imageAndGapSize = imageSize;
	// Calculate the text size
	CSize textSize = GetTextSize();
	CSize textAndGapSize = textSize;

	// if vertical alignment is BS_VCENTER then text and image are
	// located on the same line
	BOOL bSpecialCase = (nVerticalAlignment == BS_VCENTER && imageSize.cx > 0 && textSize.cx > 0);

	CPoint textOrigin;
	CPoint imageOrigin;

	if(bSpecialCase)
	{
		if((imageSize.cx != 0) && (imageSize.cy != 0))
		{
			imageAndGapSize += CSize(2 * -m_ptImageOffset.x, 2 * -m_ptImageOffset.y);
		}

		if((textSize.cx != 0) && (textSize.cy != 0))
		{
			// ... Set text and gap to correct value if text exists
			// ... Subtract text and space size from limit size (only width)
				textAndGapSize += CSize(-m_ptTextOffset.x, 2 * -m_ptTextOffset.y);
		}

		// ... Adjust size if already to big
		if(sizeLimit.cx < textAndGapSize.cx)
		{
			textAndGapSize.cx = sizeLimit.cx;
			textSize.cx = textAndGapSize.cx - (-m_ptTextOffset.x);
			if (textSize.cx < 0)
				textSize.cx = 0;
		}
		if(sizeLimit.cy < textAndGapSize.cy)
		{
			textAndGapSize.cy = sizeLimit.cy;
			textSize.cy = textAndGapSize.cy - (2 * -m_ptTextOffset.y);
			if(textSize.cy < 0)
				textSize.cy = 0;
		}

		// ... Subtract text and space size from limit size (only width)
		sizeLimit.cx -= textAndGapSize.cx;

		// ... May not have used more size than available
		ASSERT((0 <= sizeLimit.cx) || (0 <= sizeLimit.cy));

		// ... Adjust size if already to big
		if(sizeLimit.cx < imageAndGapSize.cx)
		{
			imageAndGapSize.cx = sizeLimit.cx;
			imageSize.cx = imageAndGapSize.cx - 2 * (-m_ptImageOffset.x);
			if(imageSize.cx < 0)
				imageSize.cx = 0;
		}
		if(sizeLimit.cy < imageAndGapSize.cy)
		{
			imageAndGapSize.cy = sizeLimit.cy;
			imageSize.cy = imageAndGapSize.cy - 2 * (-m_ptImageOffset.y);
			if(imageSize.cy < 0)
				imageSize.cy = 0;
		}

		// ... Subtract image and space size from limit size (only width)
		sizeLimit.cx -= imageAndGapSize.cx;

		if(imageSize.cx==0)
		{
			textSize.cx = textAndGapSize.cx - (2 * -m_ptTextOffset.x);
			if(textSize.cx < 0)
				textSize.cx = 0;
		}

		// ... May not have used more size than available
		//     The width that is still left will be used between the image and the border
		//     or the text and the border (not between the text and the image)
		ASSERT((0 <= sizeLimit.cx) || (0 <= sizeLimit.cy));

		// Calculate the image and text position
		textOrigin.x = originLimit.x + -m_ptTextOffset.x;
		textOrigin.y = originLimit.y + -m_ptTextOffset.y;
		imageOrigin = originLimit + -m_ptImageOffset;

		if(nHorizontalAlignment == BS_LEFT)
		{
			ASSERT(textOrigin.x == originLimit.x + -m_ptTextOffset.x);
			if(textAndGapSize.cx !=0 )
			{
				imageOrigin.x = textAndGapSize.cx + -m_ptImageOffset.x;
			}
		}
		else	//  (nHorizontalAlignment == BS_RIGHT) or other
		{
			ASSERT(imageOrigin.x == originLimit.x + -m_ptImageOffset.x);
			if(imageAndGapSize.cx != 0)
			{
				textOrigin.x = imageAndGapSize.cx;
			}
		}

		ASSERT(textOrigin.y == originLimit.y + -m_ptTextOffset.y);
		ASSERT(imageOrigin.y == originLimit.y + -m_ptImageOffset.y);

		textOrigin.y = (rectLimit.Height() - textAndGapSize.cy) / 2 + -m_ptTextOffset.y;
		imageOrigin.y = (rectLimit.Height() - imageAndGapSize.cy) / 2 + -m_ptImageOffset.y;
	}
	else
	{
		if((textSize.cx != 0) && (textSize.cy != 0))
		{
			// ... Set text and gap to correct value if text exists
			// ... Subtract text and space size from limit size (only height)
			textAndGapSize += CSize(-m_ptTextOffset.x * 2, -m_ptTextOffset.y);
		}

		if((imageSize.cx != 0) && (imageSize.cy != 0))
		{
			imageAndGapSize += CSize(2 * -m_ptImageOffset.x, 2 * -m_ptImageOffset.y);

			if(sizeLimit.cy < textAndGapSize.cy)
			{
				textAndGapSize.cy = sizeLimit.cy;
				textSize.cy = textAndGapSize.cy - (-m_ptTextOffset.y);
				if(textSize.cy < 0)
					textSize.cy = 0;
			}
		}
		else
		{
			if((textSize.cx != 0) && (textSize.cy != 0))
			{
				// ... Set text and gap to correct value if text exists
				// ... Subtract text and space size from limit size (only height)
				textAndGapSize += CSize(0, -m_ptTextOffset.y);
			}
			if(sizeLimit.cy < textAndGapSize.cy)
			{
				textAndGapSize.cy = sizeLimit.cy;
				textSize.cy = textAndGapSize.cy - 2 * (-m_ptTextOffset.y);
				if(textSize.cy < 0)
					textSize.cy = 0;
			}
		}
			
		// ... Adjust size if already to big
		if(sizeLimit.cx < textAndGapSize.cx)
		{
			textAndGapSize.cx = sizeLimit.cx;
			textSize.cx = textAndGapSize.cx - 2 * (-m_ptTextOffset.x);
			if(textSize.cx < 0)
				textSize.cx = 0;
		}

		// ... Subtract text and space size from limit size (only height)
		sizeLimit.cy -= textAndGapSize.cy;

		// ... May not have used more size than available
		ASSERT((0 <= sizeLimit.cx) || (0 <= sizeLimit.cy));

		// ... Adjust size if already to big
		if(sizeLimit.cx < imageAndGapSize.cx)
		{
			imageAndGapSize.cx = sizeLimit.cx;
			imageSize.cx = imageAndGapSize.cx - 2 * (-m_ptImageOffset.x);
			if(imageSize.cx < 0)
				imageSize.cx = 0;
		}
		if(sizeLimit.cy < imageAndGapSize.cy)
		{
			imageAndGapSize.cy = sizeLimit.cy;
			imageSize.cy = imageAndGapSize.cy - 2 * (-m_ptImageOffset.y);
			if(imageSize.cy < 0)
				imageSize.cy = 0;
		}
		// ... Subtract image and space size from limit size (only height)
		sizeLimit.cy -= imageAndGapSize.cy;

		// ... May not have used more size than available
		//     The width that is still left will be used between the image and the border
		//     or the text and the border (not between the text and the image)
		ASSERT((0 <= sizeLimit.cx) || (0 <= sizeLimit.cy));

		// Calculate the image and text position
		textOrigin = originLimit + -m_ptTextOffset;
		imageOrigin = originLimit + -m_ptImageOffset;

		if(nHorizontalAlignment == BS_LEFT)
		{
			ASSERT(textOrigin.x == originLimit.x + -m_ptTextOffset.x);
			ASSERT(imageOrigin.x == originLimit.x + -m_ptImageOffset.x);
		}
		else if(nHorizontalAlignment == BS_RIGHT)
		{
			textOrigin.x = sizeLimit.cx - textAndGapSize.cx + -m_ptTextOffset.x;
			imageOrigin.x = sizeLimit.cx - imageAndGapSize.cx + -m_ptImageOffset.x;
		}
		else	// (nHorizontalAlignment == BS_CENTER) or other
		{
			textOrigin.x = (sizeLimit.cx - textAndGapSize.cx) / 2 + -m_ptTextOffset.x;
			imageOrigin.x = (sizeLimit.cx - imageAndGapSize.cx) / 2 + -m_ptImageOffset.x;
		}

		// Take vertical alignment into account
		if(nVerticalAlignment == BS_TOP)
		{
			// ... Image at top, text underneath
			ASSERT(imageOrigin.y == originLimit.y + -m_ptImageOffset.y);
			if(imageAndGapSize.cy > 0)
			{
				textOrigin.y = originLimit.y + imageAndGapSize.cy;
			}
		}
		else if(nVerticalAlignment == BS_BOTTOM)
		{
			// ... Image at bottom, text above it
			imageOrigin.y = rectLimit.Height() - (imageSize.cy + -m_ptImageOffset.y);
			if(imageAndGapSize.cy == 0)
			{
				textOrigin.y = rectLimit.Height() - (textSize.cy + -m_ptTextOffset.y);
			}
		}
		else	// (nVerticalAlignment == BS_VCENTER) or other
		{
			ASSERT(textOrigin.y == originLimit.y + -m_ptTextOffset.y);
			ASSERT(imageOrigin.y == originLimit.y + -m_ptImageOffset.y);
			// ... Image and text vertically centered (text under image)
			int nImageAndAndTextAndAllGaps = imageAndGapSize.cy + textAndGapSize.cy;
			int nTopBottomSpace = (rectLimit.Height() - nImageAndAndTextAndAllGaps) / 2;
			imageOrigin.y = nTopBottomSpace + -m_ptImageOffset.y;
			if(imageAndGapSize.cy > 0)
			{
				textOrigin.y = nTopBottomSpace + imageAndGapSize.cy;
			}
			else
			{
				textOrigin.y = nTopBottomSpace + -m_ptTextOffset.y;
			}
		}
	}

	// Assign image result
	if((imageSize.cx != 0) && (imageSize.cy != 0))
		imageRect = CRect(imageOrigin, imageSize);
	else
		// ... No image to show
		imageRect.SetRectEmpty();

	// Assign text result
	if((textSize.cx != 0) && (textSize.cy != 0))
		textRect = CRect(textOrigin, textSize);
	else
		// ... No text to show
		textRect.SetRectEmpty();

	// Add small adjustments if necessary
	if((nState & ODS_SELECTED) == ODS_SELECTED)
	{
		// ... If button is pressed : move the image and text a little bit
		//     to bottom right
		if(!imageRect.IsRectEmpty())
			imageRect += GetFlatLook() ? m_ptFlatDownOffset : m_ptDownOffset;
		if(!textRect.IsRectEmpty())
			textRect += GetFlatLook() ? m_ptFlatDownOffset : m_ptDownOffset;
	}
	else if(GetCheck() != 0)
	{
		// ... If button is pressed : move the image and text a little bit
		//     to bottom right
		if(!imageRect.IsRectEmpty())
			imageRect += GetFlatLook() ? m_ptFlatCheckedOffset : m_ptCheckedOffset;
		if(!textRect.IsRectEmpty())
			textRect += GetFlatLook() ? m_ptFlatCheckedOffset : m_ptCheckedOffset;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SelectPalette(CDC* pDC, UINT /* nState */, CRect /* buttonRect */, CPalette*& pOldPalette)
{
	pOldPalette = NULL;
	if((HPALETTE)m_palette != NULL)
	{
		pOldPalette = pDC->SelectPalette(&m_palette, FALSE);
		pDC->RealizePalette();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::DrawButton(CDC* pDC, UINT nState, CRect buttonRect)
{
	COLORREF clrBtnFace, clrBtnHilight, clrBtnShadow, clrWndFrame = ::GetSysColor(COLOR_WINDOWFRAME);
	clrBtnFace = GetColor(enColorIndexBtnFace);
	clrBtnHilight = GetColor(enColorIndexBtnHilight);
	clrBtnShadow = GetColor(enColorIndexBtnShadow);

	if(!m_bFlatLook)
	{
		if(!GetToggle())
		{
			pDC->FillSolidRect(buttonRect, clrBtnFace);

			if((nState & ODS_SELECTED) == ODS_SELECTED)
			{
				pDC->Draw3dRect(buttonRect, clrWndFrame, clrBtnHilight);
				buttonRect.DeflateRect(1, 1);
				pDC->Draw3dRect(buttonRect, clrBtnShadow, clrBtnShadow);
			}
			else
			{
				if((nState & ODS_FOCUS) == ODS_FOCUS)
					buttonRect.DeflateRect(1, 1);

				pDC->Draw3dRect(buttonRect, clrBtnHilight, clrWndFrame);
				buttonRect.DeflateRect(1, 1);
				pDC->FillSolidRect(buttonRect.left, buttonRect.bottom, buttonRect.right - buttonRect.left, -1, clrBtnShadow);
				pDC->FillSolidRect(buttonRect.right, buttonRect.top, -1, buttonRect.bottom - buttonRect.top, clrBtnShadow);
			}
		}
		else
		{
			if(GetCheck() == 0)
				pDC->FillSolidRect(buttonRect, clrBtnFace);
			else
				pDC->FillSolidRect(buttonRect, clrBtnHilight);

			if(GetCheck() != 0 || (nState & ODS_SELECTED) == ODS_SELECTED)
			{
				pDC->Draw3dRect(buttonRect, clrWndFrame, clrBtnHilight);
				buttonRect.DeflateRect(1, 1);
				pDC->Draw3dRect(buttonRect, clrBtnShadow, clrBtnFace);
			}
			else
			{
				pDC->Draw3dRect(buttonRect, clrBtnHilight, clrWndFrame);
				buttonRect.DeflateRect(1, 1);
				
				pDC->FillSolidRect(buttonRect.left, buttonRect.bottom, buttonRect.right - buttonRect.left, -1, clrBtnShadow);
				pDC->FillSolidRect(buttonRect.right, buttonRect.top, -1, buttonRect.bottom - buttonRect.top, clrBtnShadow);
			}
		}
	}
	else
	{
		if(GetCheck() == 0)
			pDC->FillSolidRect(buttonRect, clrBtnFace);
		else
			pDC->FillSolidRect(buttonRect, clrBtnHilight);

		if(GetCheck() != 0 || (nState & ODS_SELECTED) == ODS_SELECTED)
		{
			pDC->Draw3dRect(buttonRect, clrBtnShadow, clrBtnHilight);
		}
		else if(m_bMouseOverButton && ((nState & ODS_DISABLED) != ODS_DISABLED))
		{
			pDC->Draw3dRect(buttonRect, clrBtnHilight, clrBtnShadow);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::DrawImage(CDC* pDC, UINT nState, CImageList* pImageList, CRect imageRect)
{
	if((pImageList == NULL) || (pImageList->m_hImageList == NULL) || 
		imageRect.left == imageRect.right || imageRect.top == imageRect.bottom)
		// ... Must have the image list created before we can draw something
		return;

	// Select the image to draw
	// ... Other scenarios
	int nImageIndex = m_nNormalImageIndex;
	if((nState & ODS_DISABLED) == ODS_DISABLED)
		// .. Disabled look (never used in hyper look)
		nImageIndex = m_nDisabledImageIndex;
	else if(m_bFlatLook && GetTrackLook() && !m_bMouseOverButton && ((nState & ODS_SELECTED) != ODS_SELECTED))
		// ... Flat look with mouse not over button and button not pressed
		nImageIndex = m_nInactiveImageIndex;
	else if(GetCheck() == 2)
		// ... Flat look with mouse not over button and button not pressed
		nImageIndex = m_nInactiveImageIndex;

	// Check whether image index is valid
	if(pImageList->GetImageCount() <= nImageIndex)
	{
		// ... Should have at least one image
		ASSERT(1 <= pImageList->GetImageCount());
		// ... The needed image is not available, use the normal image (best we can do)
		nImageIndex = m_nNormalImageIndex;
	}

	// Draw the image
	VERIFY(pImageList->Draw(pDC, nImageIndex, imageRect.TopLeft(), ILD_TRANSPARENT));
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::DrawText(CDC* pDC, UINT nState, CString sText, CRect textRect)
{
	if(sText.IsEmpty() || textRect.left == textRect.right || textRect.top == textRect.bottom)
		// ... Nothing to do : return immediately
		return;

	DWORD nHorizontalAlignment = GetHorizontalAlignment();
	UINT nFormat = ((GetStyle() & BS_MULTILINE) == BS_MULTILINE ? 
		DT_WORDBREAK : DT_SINGLELINE) | (nHorizontalAlignment == BS_LEFT ? DT_LEFT :
		(nHorizontalAlignment == BS_RIGHT ? DT_RIGHT : DT_CENTER))| DT_VCENTER;

	// ... Set the correct text color
	COLORREF oldColor = pDC->SetTextColor(GetColor(enColorIndexBtnText));
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

	// Draw the text
	if((nState & ODS_DISABLED) == ODS_DISABLED)
	{
		if(!m_bFlatLook)
		{
			pDC->SetTextColor(GetColor(enColorIndexBtnHilight));
			pDC->DrawText(sText, textRect + CPoint(1, 1), nFormat);
		}
		pDC->SetTextColor(GetColor(enColorIndexBtnGrayText));
	}
	else if(GetCheck() == 2)
	{
		// ... Indeterminate state
		pDC->SetTextColor(GetColor(enColorIndexBtnGrayText));
	}

	pDC->DrawText(sText, textRect, nFormat);

	// ... Reset the color
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(nOldBkMode);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::DrawFocusRectangle(CDC* pDC, UINT nState, CRect buttonRect, CRect imageRect)
{
	// Draw inner focus rect
	CRect innerFocusRect = buttonRect;
	if(GetFlatLook())
		innerFocusRect.InflateRect(m_ptFlatInnerFocusOffset.x, m_ptFlatInnerFocusOffset.y);
	else
	{
		innerFocusRect.InflateRect(m_ptInnerFocusOffset.x, m_ptInnerFocusOffset.y);
		if(GetToggle())
			innerFocusRect.InflateRect(1, 1);
	}

	pDC->SetBkColor(GetColor(enColorIndexBtnFace));

	pDC->DrawFocusRect(&innerFocusRect);

	// Draw outer focus rect (if not in flat look mode)
	if(!m_bFlatLook && !GetToggle())
	{
		CRect outerFocusRect = buttonRect;
		pDC->Draw3dRect(outerFocusRect, ::GetSysColor(COLOR_WINDOWFRAME), ::GetSysColor(COLOR_WINDOWFRAME));
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::RestorePalette(CDC* pDC, UINT /* nState */, CRect /* buttonRect */, CPalette* pOldPalette)
{
	if(pOldPalette != NULL)
		pDC->SelectPalette(pOldPalette, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::BuildGrayBitmap(LPCTSTR lpszBitmapResource, COLORREF crMask, CBitmap* pGrayBitmap)
{
	// Bitmap object should exist but not yet be attached
	ASSERT(pGrayBitmap != NULL);
	ASSERT(pGrayBitmap->m_hObject == NULL);

	// Get the bitmap resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hBitmapGlobal = NULL;
	HGLOBAL hBitmapCopy = NULL;
	DWORD nResourceSize = 0;
	LPBITMAPINFOHEADER pBitmapInfoHeader = NULL;

	hInstance = AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP);
	hResourceInfoBlock = ::FindResource(hInstance, lpszBitmapResource, RT_BITMAP);
	hBitmapGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if(hBitmapGlobal == NULL)
	{
		TRACE0("CEGButton::BuildGrayBitmap : Failed to load bitmap resource, failing\n");
		return FALSE;
	}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(BITMAPINFOHEADER) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hBitmapGlobal);
	hBitmapCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hBitmapCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	
	// Change the bitmap color table to gray scale
	BOOL bGraySuccess = MakeGray(pBitmapInfoHeader, crMask);

	// Build the bitmap
	HBITMAP hBitmap = NULL;
	if(bGraySuccess)
	{
		int nNumColors = (1 << pBitmapInfoHeader->biBitCount);
		CClientDC dc(NULL);
		hBitmap = ::CreateDIBitmap(dc.m_hDC, pBitmapInfoHeader,	CBM_INIT,
			(LPBYTE)pBitmapInfoHeader + pBitmapInfoHeader->biSize + 
			nNumColors * sizeof(RGBQUAD),
			(LPBITMAPINFO)pBitmapInfoHeader, DIB_RGB_COLORS);
		pGrayBitmap->Attach(hBitmap);
	}
	BOOL bBitmapSuccess = (hBitmap != NULL);
#ifdef _DEBUG
	if(!bBitmapSuccess)
		TRACE0("CEGButton::BuildGrayBitmap : Failed to create the bitmap\n");
#endif // _DEBUG
	
	// Clean up
	::UnlockResource(hBitmapGlobal);
	if((::GlobalUnlock(hBitmapCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hBitmapCopy) == NULL);

	return bGraySuccess && bBitmapSuccess;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::BuildGrayIcon(LPCTSTR lpszIconResource, HICON* phGrayIcon)
{
	// Icon object should exist but not yet be attached
	ASSERT(phGrayIcon != NULL);
	ASSERT(AfxIsValidAddress(phGrayIcon, sizeof(HICON*)));
	ASSERT(*phGrayIcon == NULL);

	// Get the icon  resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hGroupIconGlobal = NULL;
	HGLOBAL hIconGlobal = NULL;
	HGLOBAL hIconCopy = NULL;
	DWORD nResourceSize = 0;
	LPGRPICONDIR  pGroupIconDir = NULL;
	LPICONIMAGE pIconImage = NULL;
	LPBITMAPINFOHEADER pBitmapInfoHeader = NULL;
	WORD nIconID = 0;

	// First get the icon directory
	hInstance = AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON);
	hResourceInfoBlock = ::FindResource(hInstance, lpszIconResource, RT_GROUP_ICON);
	hGroupIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if(hGroupIconGlobal == NULL)
	{
		TRACE0("CEGButton::BuildGrayIcon : Failed to load icon group resource, failing\n");
		return FALSE;
	}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(GRPICONDIR) <= nResourceSize);

	pGroupIconDir = (LPGRPICONDIR)::LockResource(hGroupIconGlobal);
	ASSERT(pGroupIconDir != NULL);
	ASSERT(pGroupIconDir->idType == 1);
	ASSERT(0 < pGroupIconDir->idCount);

	nIconID = pGroupIconDir->idEntries[0].nID;
	if((::GlobalUnlock(hGroupIconGlobal) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hGroupIconGlobal) == NULL);

	// Then get the icon itself
	hResourceInfoBlock = ::FindResource(hInstance, MAKEINTRESOURCE(nIconID), RT_ICON);
	hIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if(hIconGlobal == NULL)
	{
		TRACE0("CEGButton::BuildGrayIcon : Failed to load icon resource, failing\n");
		return FALSE;
	}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(ICONIMAGE) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hIconGlobal);
	ASSERT(pOldBitmapInfoHeader != NULL);
	hIconCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hIconCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	pIconImage = (LPICONIMAGE)pBitmapInfoHeader;
	
	// Change the bitmap color table to gray scale
	BOOL bGraySuccess = MakeGray(pBitmapInfoHeader);

	// Build the icon
	if(bGraySuccess)
		*phGrayIcon = ::CreateIconFromResource((PBYTE)pIconImage, nResourceSize, TRUE, 	0x00030000);
	BOOL bIconSuccess = (*phGrayIcon != NULL);
#ifdef _DEBUG
	if(!bIconSuccess)
		TRACE0("CEGButton::BuildGrayBitmap : Failed to create the icon\n");
#endif // _DEBUG
	
	// Clean up
	::UnlockResource(hIconGlobal);
	if((::GlobalUnlock(hIconCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hIconCopy) == NULL);

	return bGraySuccess && bIconSuccess;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::MakeGray(LPBITMAPINFOHEADER pBitmapInfoHeader, COLORREF crMask /* = CLR_NONE */)
{
	// See also 'Retrieving Palette Information from a Bitmap Resource'
	// MSDN PSS ID Number: Q124947

	ASSERT(pBitmapInfoHeader != NULL);
	ASSERT(AfxIsValidAddress(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER)));

	// Check palette existance
	int nNumColors = 0;
	if(pBitmapInfoHeader->biBitCount <= 8)
		nNumColors = (1 << pBitmapInfoHeader->biBitCount);
	else
		// ... No palette used for 24 BPP DIB
		nNumColors = 0;
	if(nNumColors == 0)
	{
		TRACE0("CEGButton::MakeGray : Resource does not have a palette, failing\n");
		return FALSE;
	}

	// Change the colors to gray scale
	int nColor;
	int nWhite;
	LPBITMAPINFO pBitmapInfo = (LPBITMAPINFO)pBitmapInfoHeader;
	ASSERT(AfxIsValidAddress(pBitmapInfo, sizeof(BITMAPINFO)));
	COLORREF orgColor;
	for(nColor = 0;  nColor < nNumColors;  nColor++)
	{
		// ... This formula will compute the 'white factor' of a color
		//     It translates an RGB value to a gray scale value
		orgColor = RGB(pBitmapInfo->bmiColors[nColor].rgbRed, 
			pBitmapInfo->bmiColors[nColor].rgbGreen, 
			pBitmapInfo->bmiColors[nColor].rgbBlue);

		nWhite = (pBitmapInfo->bmiColors[nColor].rgbRed *    77 +
				  pBitmapInfo->bmiColors[nColor].rgbGreen * 153 +
				  pBitmapInfo->bmiColors[nColor].rgbBlue *   25 ) / 255;

		// Round the value to 0, 128, 192 or 255 (gray scale colors which are always available)
		if(nWhite < 64)
			nWhite = 0;			// ... [0, 63]
		else if(nWhite < 160)
			nWhite = 128;		// ... [64, 159]
		else if(nWhite < 224)
			nWhite = 192;		// ... [160, 223]
		else 
			nWhite = 255;		// ... [224, 255]
		
		if(orgColor != crMask)
		{
			// Change the color entries in the bitmap
			pBitmapInfo->bmiColors[nColor].rgbRed	= (BYTE)nWhite;
			pBitmapInfo->bmiColors[nColor].rgbGreen = (BYTE)nWhite;
			pBitmapInfo->bmiColors[nColor].rgbBlue	= (BYTE)nWhite;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::BuildDisabledImage(HICON hSourceIcon, CSize imageSize, HICON& hDestIcon)
{
	CBitmap helperBitmap;

	CClientDC dc(NULL);
	// Create a mem DC with a new bitmap in it
	CDC memDC;
	VERIFY(memDC.CreateCompatibleDC(&dc));
	VERIFY(helperBitmap.CreateCompatibleBitmap(&dc, imageSize.cx, imageSize.cy));
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&helperBitmap);
	// ... Default background is the button color
	memDC.FillSolidRect(CRect(CPoint(0, 0), imageSize), GetColor(enColorIndexBtnFace));

	// Draw the source icon on the mem DC in a disabled state
	BOOL bSuccess = memDC.DrawState(CPoint(0, 0), imageSize, hSourceIcon, DSS_DISABLED, (HBRUSH)NULL);
	memDC.SelectObject(pOldBitmap);

	// Convert the disabled bitmap to an icon by using the same mask as the source icon
	ICONINFO iconInfo;
	::ZeroMemory(&iconInfo, sizeof(iconInfo));
	// ... ::GetIconInfo craetes two new bitmaps (color and mask)
	VERIFY(::GetIconInfo(hSourceIcon, &iconInfo));
	// ... Delete the color bitmap
	::DeleteObject(iconInfo.hbmColor);
	iconInfo.hbmColor = NULL;
	iconInfo.hbmColor = helperBitmap;
	hDestIcon = ::CreateIconIndirect(&iconInfo);
	// ... Delete the mask bitmap
	::DeleteObject(iconInfo.hbmMask);
	iconInfo.hbmMask = NULL;

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetBitmapPalette(LPCTSTR lpszBitmapResource, CPalette& palette)
{
	// Get the bitmap resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hBitmapGlobal = NULL;
	HGLOBAL hBitmapCopy = NULL;
	DWORD nResourceSize = 0;
	LPBITMAPINFOHEADER pBitmapInfoHeader = NULL;

	hInstance = AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP);
	hResourceInfoBlock = ::FindResource(hInstance, lpszBitmapResource, RT_BITMAP);
	hBitmapGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if(hBitmapGlobal == NULL)
	{
		TRACE0("CEGButton::GetBitmapPalette : Failed to load bitmap resource, failing\n");
		return FALSE;
	}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(BITMAPINFOHEADER) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hBitmapGlobal);
	hBitmapCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hBitmapCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	
	// Change the bitmap color table to gray scale
	BOOL bPaletteSuccess = GetImagePalette(pBitmapInfoHeader, palette);

	// Clean up
	::UnlockResource(hBitmapGlobal);
	if((::GlobalUnlock(hBitmapCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hBitmapCopy) == NULL);

	return bPaletteSuccess;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetIconPalette(LPCTSTR lpszIconResource, CPalette& palette)
{
	// Get the icon  resource data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hGroupIconGlobal = NULL;
	HGLOBAL hIconGlobal = NULL;
	HGLOBAL hIconCopy = NULL;
	DWORD nResourceSize = 0;
	LPGRPICONDIR  pGroupIconDir = NULL;
	LPICONIMAGE pIconImage = NULL;
	LPBITMAPINFOHEADER pBitmapInfoHeader = NULL;
	WORD nIconID = 0;

	// First get the icon directory
	hInstance = AfxFindResourceHandle(lpszIconResource, RT_GROUP_ICON);
	hResourceInfoBlock = ::FindResource(hInstance, lpszIconResource, RT_GROUP_ICON);
	hGroupIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if(hGroupIconGlobal == NULL)
	{
		TRACE0("CEGButton::GetIconPalette : Failed to load icon group resource, failing\n");
		return FALSE;
	}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(GRPICONDIR) <= nResourceSize);

	pGroupIconDir = (LPGRPICONDIR)::LockResource(hGroupIconGlobal);
	ASSERT(pGroupIconDir != NULL);
	ASSERT(pGroupIconDir->idType == 1);
	ASSERT(0 < pGroupIconDir->idCount);

	nIconID = pGroupIconDir->idEntries[0].nID;
	if((::GlobalUnlock(hGroupIconGlobal) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hGroupIconGlobal) == NULL);

	// Then get the icon itself
	hResourceInfoBlock = ::FindResource(hInstance, MAKEINTRESOURCE(nIconID), RT_ICON);
	hIconGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if(hIconGlobal == NULL)
	{
		TRACE0("CEGButton::GetIconPalette : Failed to load icon resource, failing\n");
		return FALSE;
	}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(ICONIMAGE) <= nResourceSize);

	// Memory may be read only, make a copy 
	void* pOldBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hIconGlobal);
	ASSERT(pOldBitmapInfoHeader != NULL);
	hIconCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hIconCopy);
	ASSERT(pBitmapInfoHeader != NULL);
	::CopyMemory(pBitmapInfoHeader, pOldBitmapInfoHeader, nResourceSize);
	pIconImage = (LPICONIMAGE)pBitmapInfoHeader;
	
	// Change the bitmap color table to gray scale
	BOOL bPaletteSuccess = GetImagePalette(pBitmapInfoHeader, palette);

	// Clean up
	::UnlockResource(hIconGlobal);
	if((::GlobalUnlock(hIconCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hIconCopy) == NULL);

	return bPaletteSuccess;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetImagePalette(LPBITMAPINFOHEADER pBitmapInfoHeader, CPalette& palette)
{
	// ... Initialize output parameter
	if(palette.m_hObject != NULL)
		palette.DeleteObject();

	// Check the palette existance
	int nNumColors = 0;
	if(pBitmapInfoHeader->biBitCount <= 8)
		nNumColors = (1 << pBitmapInfoHeader->biBitCount);
	else
		// ... No palette used for 24 BPP DIB
		nNumColors = 0;  
	if(nNumColors == 0)
	{
		TRACE0("CEGButton::GetImagePalette : Resource does not have a palette, failing\n");
		return FALSE;
	}

	// Create the logical palette	
	BYTE* pBuffer = NULL;
	LPLOGPALETTE pLogPalette = NULL;
	const int nBufferSize = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nNumColors;
	pBuffer = new BYTE[nBufferSize];
	::ZeroMemory(pBuffer, nBufferSize);
	pLogPalette = (LPLOGPALETTE)pBuffer;

	// Initialize the palette
	pLogPalette->palVersion = 0x300;
	pLogPalette->palNumEntries = (unsigned short)nNumColors;

	// Get the palette colors 
	LPBITMAPINFO pBitmapInfo = (LPBITMAPINFO)pBitmapInfoHeader;
	ASSERT(AfxIsValidAddress(pBitmapInfo, sizeof(BITMAPINFO)));
	int nColor;
	for(nColor = 0;  nColor < nNumColors;  nColor++)
	{
		pLogPalette->palPalEntry[nColor].peRed   = pBitmapInfo->bmiColors[nColor].rgbRed;
		pLogPalette->palPalEntry[nColor].peGreen = pBitmapInfo->bmiColors[nColor].rgbGreen;
		pLogPalette->palPalEntry[nColor].peBlue  = pBitmapInfo->bmiColors[nColor].rgbBlue;
		pLogPalette->palPalEntry[nColor].peFlags = 0;
	}

	// Build the palette from the logical palette
	BOOL bPaletteSuccess = palette.CreatePalette(pLogPalette);

#ifdef _DEBUG
	if(!bPaletteSuccess)
		TRACE0("CEGButton::GetImagePalette : Failed to create the palette\n");
#endif // _DEBUG

	// Clean up
	delete[] pBuffer;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::CheckFlatLook(CPoint point)
{
	if(!m_bFlatLook)
		// Only do something when we are in flat look mode
		return;

	// Check whether the mouse is over the button now
	CRect rcWindow;
	GetWindowRect(rcWindow);
	BOOL bNewMouseOverButton = rcWindow.PtInRect(point);

	if(bNewMouseOverButton && IsWindowEnabled())
	{
		// Moved inside this control window
		// ... Do capture the mouse input while the mouse is still down
		//     (may have triggered an action, e.g. show msgbox)
		if(!m_bMouseDown)
			SetCapture();
		// ... When we changed state invalidate the window and get the focus
		//	   so that pressing a key will have the right effect
		if(m_bFlatLook && !m_bMouseOverButton)
		{
			BOOL bRedraw = (GetCheck() != 2);
			if(GetFocus()!=this)
			{
				// don't set focus - there is no that much sense in it (the only
				// thing we miss is kry board input - we'll get it when user click 
				// on the button)
//				SetFocus();
				bRedraw=TRUE;
			}
			if(bRedraw)
				Invalidate();
		}
		m_bMouseOverButton = TRUE;
	}
	else
	{
		// ... We release the mouse capture (may have already lost it)
		if(!m_bMouseDown)
			ReleaseCapture();
		// ... When we changed state invalidate the window
		if(m_bFlatLook && m_bMouseOverButton)
		{
			BOOL bRedraw = (GetCheck() != 2);
			if(bRedraw)
				Invalidate();
		}
		m_bMouseOverButton = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::PostCheckFlatLook() 
{
	if(m_bFlatLook && m_bMouseOverButton && (GetCapture() != this))
	{
		// We have lost the capture
		m_bMouseOverButton = FALSE;
		if(m_bFlatLook)
		{
			BOOL bRedraw = (GetCheck() != 2);
			if(bRedraw)
				Invalidate();
		}
		// Recheck the mouse position as soon as possible
		PostMessage(WM_CHECK_FLAT_LOOK);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
CString CEGButton::GetSubString(LPCTSTR pszFullString, int nSubIndex, TCHAR cDelimiter)
{
	// NOTE!!! The nSubIndex is ONE-based index of the substring requested
	ASSERT(0 < nSubIndex);

	CString sSubString;
	if(pszFullString == NULL)
	{
		// Nothing to search : nothing to find
		ASSERT(sSubString.IsEmpty());
		return sSubString;
	}

	// Set pszStart to first charecter and pszEnd after last charecter
	LPCTSTR pszBegin = pszFullString;
	LPCTSTR pszEnd = pszFullString + _tcslen(pszFullString);
	LPCTSTR pszDelimiter = _tcschr(pszBegin, cDelimiter); 
	if(pszDelimiter == NULL)
		pszDelimiter = pszEnd;

	ASSERT((*pszDelimiter == cDelimiter) || (*pszDelimiter == _T('\0')));
	ASSERT(pszBegin <= pszDelimiter);

	while(--nSubIndex != 0)
	{
		if(*pszDelimiter == _T('\0'))
		{
			// Search to end of string and not found
			ASSERT(sSubString.IsEmpty());
			return sSubString;
		}
		pszBegin = pszDelimiter + 1;
		pszDelimiter = _tcschr(pszBegin, cDelimiter);
		if(pszDelimiter == NULL)
			pszDelimiter = pszEnd;
		ASSERT((*pszDelimiter == cDelimiter) || (*pszDelimiter == _T('\0')));
		ASSERT(pszBegin <= pszDelimiter);
	}

	int nLen = pszDelimiter - pszBegin;
	ASSERT(0 <= nLen);
	LPTSTR pszSubString = sSubString.GetBufferSetLength(nLen);
	::CopyMemory(pszSubString, pszBegin, nLen * sizeof(TCHAR));

	return sSubString;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::LoadBitmap(LPCTSTR lpszBitmapResource, CBitmap& bitmap, CPalette& palette)
{
	// See also "How to Use a DIB Stored as a Windows Resource"
	// Article ID: Q67883 :
	//  When the display is a 256-color 8514 unit, the same action [LoadBitmap()] 
	//  will map the 256 bitmap colors into the 20 reserved system colors, 
	//  and an 8 bits-per-pixel bitmap will be returned. 

	// ... Assume failure
	BOOL bPaletteSuccess = FALSE;
	BOOL bBitmapSuccess = FALSE;

	// ... First delete a possible previous bitmap contents
	bitmap.DeleteObject();

	// ... Without deleting the palette, reloading the bitmap would fail
	// Thanks to Ali
	palette.DeleteObject();

	// Load bitmap
	// ... Bitmap and palette object should not yet be attached
	ASSERT(bitmap.m_hObject == NULL);
	ASSERT(palette.m_hObject == NULL);

	// ... Get the bitmap data
	HINSTANCE hInstance = NULL;
	HRSRC hResourceInfoBlock = NULL;
	HGLOBAL hBitmapGlobal = NULL;
	HGLOBAL hBitmapCopy = NULL;
	DWORD nResourceSize = 0;
	LPBITMAPINFOHEADER pBitmapInfoHeader = NULL;
	LPBITMAPINFOHEADER pCopyBitmapInfoHeader = NULL;

	hInstance = AfxFindResourceHandle(lpszBitmapResource, RT_BITMAP);
	hResourceInfoBlock = ::FindResource(hInstance, lpszBitmapResource, RT_BITMAP);
	hBitmapGlobal = ::LoadResource(hInstance, hResourceInfoBlock);
	if(hBitmapGlobal == NULL)
	{
		TRACE0("CEGButton::LoadBitmap : Failed to load bitmap resource, failing\n");
		return FALSE;
	}

	nResourceSize = ::SizeofResource(hInstance, hResourceInfoBlock);
	ASSERT(sizeof(BITMAPINFOHEADER) <= nResourceSize);

	// Memory may be read only, make a copy 
	pBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hBitmapGlobal);
	hBitmapCopy = ::GlobalAlloc(GMEM_MOVEABLE, nResourceSize);
	pCopyBitmapInfoHeader = (LPBITMAPINFOHEADER)::GlobalLock(hBitmapCopy);
	ASSERT(pCopyBitmapInfoHeader != NULL);
	::CopyMemory(pCopyBitmapInfoHeader, pBitmapInfoHeader, nResourceSize);

	// Get the bitmap color table 
	bPaletteSuccess = GetImagePalette(pCopyBitmapInfoHeader, palette);
	if(!bPaletteSuccess)
		TRACE0("CEGButton::LoadBitmap : Failed to load palette, continuing\n");

	// Get the bitmap bits itself
	DWORD nPaletteByteSize = 0;
	LPVOID pBitmapBits = NULL;
	if(pCopyBitmapInfoHeader->biSize == sizeof(BITMAPINFOHEADER))
	{
		if(pCopyBitmapInfoHeader->biBitCount <= 8)
			nPaletteByteSize = (1 << pCopyBitmapInfoHeader->biBitCount) * sizeof(RGBQUAD);
	}
	else
	{
		if(((LPBITMAPCOREHEADER)pCopyBitmapInfoHeader)->bcBitCount <= 8)
			nPaletteByteSize = (1 << ((LPBITMAPCOREHEADER)pCopyBitmapInfoHeader)->bcBitCount) * sizeof(RGBTRIPLE);
	}
	pBitmapBits = (LPBYTE)pCopyBitmapInfoHeader + pCopyBitmapInfoHeader->biSize + nPaletteByteSize;
	{
		CWindowDC dc(NULL);
		CPalette* pOldPalette = NULL;
		if(bPaletteSuccess)
		{
			dc.SelectPalette(&palette, FALSE);
			dc.RealizePalette();
		}
		HBITMAP hBitmap = ::CreateDIBitmap (dc.GetSafeHdc(), pCopyBitmapInfoHeader,
							CBM_INIT, pBitmapBits, (LPBITMAPINFO)pCopyBitmapInfoHeader, DIB_RGB_COLORS);
		bBitmapSuccess = (hBitmap != NULL);
		if(bBitmapSuccess)
			bitmap.Attach(hBitmap);
		else
			TRACE0("CEGButton::LoadBitmap : Failed to load bitmap, failing\n");

		if(pOldPalette != NULL)
			dc.SelectPalette (pOldPalette, FALSE);
	}

	::UnlockResource(hBitmapGlobal);
	if((::GlobalUnlock(hBitmapCopy) == 0) && (::GetLastError() == NO_ERROR))
		VERIFY(::GlobalFree(hBitmapCopy) == NULL);

	// ... Return whether successful or not
	return bBitmapSuccess;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::AdjustSysColors()
{
	m_clrSystemColors[enColorIndexBtnFace] = ::GetSysColor(COLOR_BTNFACE);
	m_clrSystemColors[enColorIndexBtnHilight] = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrSystemColors[enColorIndexBtnShadow] = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrSystemColors[enColorIndexBtnText] = ::GetSysColor(COLOR_BTNTEXT);
	m_clrSystemColors[enColorIndexBtnGrayText] = ::GetSysColor(COLOR_GRAYTEXT);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnSysColorChange()
{
	CButton::OnSysColorChange();
	
	AdjustSysColors();

	// Redraw the button
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	// Check whether the mouse is over the button now
	ClientToScreen(&point);
	CheckFlatLook(point);

	CButton::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	if(nChar == VK_SPACE)
	{
		OnClicked();
	}

	CButton::OnKeyUp(nChar, nRepCnt, nFlags);
	// The previous function may have released the mouse capture,
	// so recheck the flat look
	PostCheckFlatLook();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnKillFocus(CWnd* pNewWnd)
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnKillFocus(pNewWnd);
	// The previous function may have released the mouse capture,
	// so recheck the flat look
//	PostCheckFlatLook();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnMButtonUp(UINT nFlags, CPoint point)
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnMButtonUp(nFlags, point);
	// The previous function may have released the mouse capture,
	// so recheck the flat look
	PostCheckFlatLook();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;
	
	CButton::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CButton::OnSysKeyUp(nChar, nRepCnt, nFlags);
	// The previous function may have released the mouse capture,
	// so recheck the flat look
	PostCheckFlatLook();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	m_bMouseDown = TRUE;
	CButton::OnLButtonDown(nFlags, point);

	// The previous function may have released the mouse capture,
	// so recheck the flat look
	PostCheckFlatLook();
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!IsWindowEnabled())
		// ... Do nothing when this window is disabled
		return;

	CRect rect;
	GetWindowRect(rect);
	ClientToScreen(&point);
	if(rect.PtInRect(point) && 
	   (GetState() & 0x0004))
	{
		OnClicked();
	}

	CButton::OnLButtonUp(nFlags, point);
	m_bMouseDown = FALSE;

	// The previous function may have released the mouse capture,
	// so recheck the flat look
	PostCheckFlatLook();
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CEGButton::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = DefWindowProc(WM_SETTEXT, wParam, lParam);

	// Adjust a possible tool tip control's text
	if(m_toolTip.m_hWnd != 0)
	{
		// Get everything after the first EndOfLine char and before te second
		CString sToolTipText = GetSubString((LPCTSTR)lParam, 2, _T('\n'));
		if(!sToolTipText.IsEmpty())
		{
			m_toolTip.DelTool(this, EGBB_TOOLTIP_ID);
			CRect rectWindow;
			GetClientRect(rectWindow);
			VERIFY(m_toolTip.AddTool(this, sToolTipText, rectWindow, EGBB_TOOLTIP_ID));
		}
		else
			m_toolTip.DelTool(this, EGBB_TOOLTIP_ID);
	}

	return result;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::PreTranslateMessage(MSG* pMsg) 
{
	// ... Pass the message on to the tool tip
	if(m_toolTip.m_hWnd != NULL)
	{
		m_toolTip.Activate(TRUE);
		m_toolTip.RelayEvent(pMsg);
	}

	return CButton::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CEGButton::OnCheckFlatLook(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam == 0);
	ASSERT(lParam == 0);
	UNUSED(wParam);
	UNUSED(lParam);

	// Check whether the mouse is over the button now
	CPoint ptCursor;
	VERIFY(::GetCursorPos(&ptCursor));
	CheckFlatLook(ptCursor);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// Use our own cursor if we have one
	if(IsWindowEnabled())
	{
		if(m_hCursor != NULL)
		{
			::SetCursor(m_hCursor);
			// ... We handled the message
			return TRUE;
		}
	}

	// ... Call base class implementation
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	// The previous function may have changed the enable state of the button
	// so recheck the flat look
	PostCheckFlatLook();
}

/////////////////////////////////////////////////////////////////////////////
//
int CEGButton::GetDropDownArrowWidth()
{
	HDC hDC = ::GetDC(NULL);
	ASSERT(hDC != NULL);

	HFONT hFont = NULL;
	HFONT oldFont = NULL;
	int nDropDownArrowWidth = 0;
	if((hFont = CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0, 
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL)
	{
		oldFont = (HFONT)SelectObject(hDC, hFont);
	}
	
	VERIFY(GetCharWidth(hDC, _T('6'), _T('6'), &nDropDownArrowWidth));

	if(oldFont!=NULL)
	{
		SelectObject(hDC, oldFont);
	}
	if(hFont!=NULL)
	{
		DeleteObject(hFont);
	}
	::ReleaseDC(NULL, hDC);
	
	return nDropDownArrowWidth;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::DrawDropDownArrow(CDC* pDC, UINT nState, CRect arrowRect)
{
	ASSERT(GetDropDown());

	if(GetDropDownArrowStyle() == enDropDownArrowNone)
	{
		arrowRect.SetRectEmpty();
	}

	COLORREF clrHilight = GetColor(enColorIndexBtnHilight);
	COLORREF clrShadow = GetColor(enColorIndexBtnShadow);
	
	if(GetDrawingDropdownSeparator() && !(m_bFlatLook && !m_bMouseOverButton))
	{
		CRect dividerRect = arrowRect;
		dividerRect.left = dividerRect.left - 3;
		dividerRect.right = dividerRect.left + 2;
		if((nState & ODS_SELECTED) == ODS_SELECTED)
		{
			dividerRect += GetFlatLook() ? m_ptFlatDownOffset : m_ptDownOffset;
		}
		pDC->Draw3dRect(dividerRect, clrShadow, clrHilight);
	}

	arrowRect.left -= 1;
	if((nState & ODS_SELECTED) == ODS_SELECTED)
	{
		arrowRect += GetFlatLook() ? m_ptFlatDownOffset : m_ptDownOffset;
	}

	CFont font;
	CFont* pOldFont=NULL;
	if((font.CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0, 
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL)
	{
		pOldFont=(CFont*)pDC->SelectObject(&font);
	}
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = CLR_NONE;
	if((nState & ODS_DISABLED) == ODS_DISABLED)
	{
		oldTextColor=pDC->SetTextColor(clrShadow);
	}

	if(GetDropDownArrowStyle() == enDropDownArrowRight)
	{
		pDC->DrawText(_T("8"), arrowRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		pDC->DrawText(_T("6"), arrowRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	if((nState & ODS_DISABLED) == ODS_DISABLED)
	{
		pDC->SetTextColor(oldTextColor);
	}
	pDC->SetBkMode(nOldBkMode);
	if(pOldFont != NULL)
	{
		pDC->SelectObject(pOldFont);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetToggle(BOOL bToggle /*= TRUE*/, BOOL bTriState /*= FALSE*/, BOOL bRedraw /*= TRUE*/)
{
	if(bToggle && !GetToggle())
	{
		if(GetDropDown())
		{
			if(m_Menu.m_hMenu)
				m_Menu.Detach();
			m_bDropDown = FALSE;
			m_enArrowStyle = enDropDownArrowNone;
		}
	}

	m_bToggle = bToggle;
	if(m_bToggle)
	{
		m_bTriState = bTriState;
	}
	else
	{
		m_bTriState = FALSE;
		m_nCheck = 0;
	}

	if(bRedraw && ::IsWindow(GetSafeHwnd()))
		RedrawWindow();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetToggle() const
{
	return m_bToggle;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetTriState() const
{
	return m_bTriState;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetCheck(int nCheck)
{
	ASSERT(GetToggle());
	if(GetToggle())
	{
		if(nCheck < 0)
			nCheck = 0;
		else if(nCheck > 2)
			nCheck = 2;
		m_nCheck = nCheck; 
	}
}

/////////////////////////////////////////////////////////////////////////////
//
int CEGButton::GetCheck() const
{
	if(GetToggle())
		return m_nCheck;
	else
		return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::SetDropDown(BOOL bDropDown /*= TRUE*/,
							HMENU hMenu /*= NULL*/,
							DropDownArrowStyleEnum enArrowStyle /*= enDropDownArrowDown*/,
							DropDownMenuAlignEnum enMenuAlign /*= enDropDownMenuAlignLeftDown*/,
							BOOL bDrawDropdownSeparator /*= TRUE*/,
							BOOL bRedraw /*= TRUE*/)
{
	if(bDropDown && !GetDropDown())
	{
		if(GetToggle())
		{
			m_bToggle = FALSE;
			m_bTriState = FALSE;
			m_nCheck = 0;
		}
	}

	m_bDropDown = bDropDown;
	if(m_bDropDown)
	{
		if(m_Menu.m_hMenu)
			m_Menu.Detach();
		if(hMenu)
			m_Menu.Attach(hMenu);
		m_enArrowStyle = enArrowStyle;
		m_bDrawDropdownSeparator = bDrawDropdownSeparator;
		m_enMenuAlign = enMenuAlign;
	}
	else
	{
		if(m_Menu.m_hMenu)
			m_Menu.Detach();
		m_enArrowStyle = enDropDownArrowNone;
		m_bDrawDropdownSeparator = FALSE;
		m_enMenuAlign = enDropDownMenuAlignLeftDown;
	}

	if(bRedraw && ::IsWindow(GetSafeHwnd()))
		RedrawWindow();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetDropDown() const
{
	return m_bDropDown;
}

/////////////////////////////////////////////////////////////////////////////
//
CEGButton::DropDownArrowStyleEnum CEGButton::GetDropDownArrowStyle() const
{
	return m_enArrowStyle;
}

/////////////////////////////////////////////////////////////////////////////
//
CEGButton::DropDownMenuAlignEnum CEGButton::GetDropDownMenuAlign() const
{
	return m_enMenuAlign;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::GetDrawingDropdownSeparator() const
{
	return m_bDrawDropdownSeparator;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::OnClicked()
{
	ASSERT(::IsWindow(m_hWnd));

	if(GetDropDown())
	{
		CWnd* pParentWnd = GetParent();
		ASSERT(pParentWnd);
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		hdr.code = EGBBN_DROPDOWN;
		if(!pParentWnd->SendMessage(WM_NOTIFY, hdr.idFrom, (LPARAM)&hdr))
		{
			OnDropDown();
		}

		RedrawWindow();
	}

	if(GetToggle())
	{
		CWnd* pParentWnd = GetParent();
		ASSERT(pParentWnd);
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		hdr.code = EGBBN_TOGGLE;
		if(!pParentWnd->SendMessage(WM_NOTIFY, hdr.idFrom, (LPARAM)&hdr))
		{
			Toggle();
			OnToggle();
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnDropDown()
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(GetDropDown());
	if(m_Menu.m_hMenu)
	{
		CRect rect;
		GetWindowRect(rect);

		int x, y;
		TPMPARAMS tpmParams;
		tpmParams.cbSize = sizeof(TPMPARAMS);
		tpmParams.rcExclude = rect;
		UINT uiFlags = TPM_VERTICAL;
		if(GetDropDownArrowStyle() == enDropDownArrowDown)
		{
			uiFlags |= TPM_TOPALIGN;
			if(GetDropDownMenuAlign() == enDropDownMenuAlignLeftDown)
			{
				x = rect.left;			
				y = rect.bottom;
				uiFlags |= TPM_LEFTALIGN;
			}
			else
			{
				x = rect.right;			
				y = rect.bottom;
				uiFlags |= TPM_RIGHTALIGN;
			}
		}
		else
		{	
			uiFlags |= TPM_LEFTALIGN;
			if(GetDropDownMenuAlign() == enDropDownMenuAlignLeftDown)
			{
				x = rect.right;
				y = rect.top;
				uiFlags |= TPM_TOPALIGN;
			}
			else
			{
				x = rect.right;
				y = rect.bottom;
				uiFlags |= TPM_BOTTOMALIGN;
			}
		}

		// We want to prevent all redrawing of the button while the popup menu is
		// displayed. This means we keep the button displayed as depressed.
		m_bAllowDraw = FALSE;

		CWnd* pWnd;
		CWnd* pParent = GetParent();
		ASSERT(pParent);
		if(pParent->IsKindOf(RUNTIME_CLASS(CView)))
			pWnd = GetParentFrame();
		else
			pWnd = pParent;

		if(::TrackPopupMenuEx(m_Menu.m_hMenu, uiFlags, x, y, pWnd->m_hWnd, &tpmParams))
		{
			// The popup menu has now been dismissed. We swallow any left clicks
			// over the button. This ensures that we do not redisplay the menu if
			// the user clicked over the button to dismiss the menu.
			MSG msg;
			::PeekMessage(&msg, m_hWnd, WM_LBUTTONDOWN, WM_LBUTTONDOWN, PM_REMOVE);
		}

		// Now redraw the button in it's correct state.
		m_bAllowDraw = TRUE;
		InvalidateRect(NULL, FALSE);
		UpdateWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CEGButton::OnToggle()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(GetToggle());
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= BS_OWNERDRAW;
	return CButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::Toggle()
{
	if(GetToggle())
	{
		if(GetCheck() == 0)
		{
			SetCheck(1);
		}
		else
		{
			if(GetTriState())
			{
				if(GetCheck() == 2)
				{
					SetCheck(0);
				}
				else
				{
					SetCheck(2);
				}
			}
			else
			{
				SetCheck(0);
			}
		}
		return GetCheck() == 1;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
CSize CEGButton::GetReservedSpace()
{
	CSize sizeReserved(0, 0);

	if(GetDropDownArrowStyle() != enDropDownArrowNone)
	{
		sizeReserved.cx += m_nDropDownArrowWidth - m_ptArrowOffset.x;
	}

	return sizeReserved;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::SetColor(StateColorEnum enStateColor, ColorIndexEnum enColorIndex, BOOL bUseSystem, COLORREF clrColor /*= 0*/)
{
	ASSERT(enColorIndex >= 0 && enColorIndex <= enColorIndexAll);
	ASSERT(enStateColor >= 0 && enStateColor <= enStateColorAll);

	if(enColorIndex < 0 || enColorIndex > enColorIndexAll || enStateColor < 0 || enStateColor > enStateColorAll)
		return FALSE;

	if(enStateColor != enStateColorAll)
	{
		if(enColorIndex != enColorIndexAll)
		{
			m_clrColors[enStateColor][enColorIndex].m_bUseSystem = bUseSystem;
			m_clrColors[enStateColor][enColorIndex].m_clrColor = clrColor;
		}
		else
		{
			for(long n = 0; n < (long)enColorIndexAll; n++)
			{
				m_clrColors[enStateColor][n].m_bUseSystem = bUseSystem;
				m_clrColors[enStateColor][n].m_clrColor = clrColor;
			}
		}
	}
	else
	{
		for(long i = 0; i < (long)enStateColorAll; i++)
		{
			if(enColorIndex != enColorIndexAll)
			{
				m_clrColors[i][enColorIndex].m_bUseSystem = bUseSystem;
				m_clrColors[i][enColorIndex].m_clrColor = clrColor;
			}
			else
			{
				for(long n = 0; n < (long)enColorIndexAll; n++)
				{
					m_clrColors[i][n].m_bUseSystem = bUseSystem;
					m_clrColors[i][n].m_clrColor = clrColor;
				}
			}
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
COLORREF CEGButton::GetColor(ColorIndexEnum enColorIndex) const
{
	ASSERT(enColorIndex >= 0 && enColorIndex <= enColorIndexAll);
	if(enColorIndex < 0 || enColorIndex > enColorIndexAll)
		return RGB(0, 0, 0);

	ColorEx color;

	if(!GetToggle() || GetCheck() == 0)
	{
		if(IsWindowEnabled())
		{
			if(m_bFlatLook && m_bMouseOverButton)
				color = m_clrColors[enStateColorCommonFlatTracked][enColorIndex];
			else
				color = m_clrColors[enStateColorCommonNormal][enColorIndex];
		}
		else
			color = m_clrColors[enStateColorCommonDisabled][enColorIndex];
	}
	else
	{
		if(IsWindowEnabled())
		{
			if(GetCheck() == 1)
				color = m_clrColors[enStateColorCheckedNormal][enColorIndex];
			else
				color = m_clrColors[enStateColorIndeterminateNormal][enColorIndex];
		}
		else
		{
			if(GetCheck() == 1)
				color = m_clrColors[enStateColorCheckedDisabled][enColorIndex];
			else
				color = m_clrColors[enStateColorIndeterminateDisabled][enColorIndex];
		}
	}

	if(color.m_bUseSystem)
		return m_clrSystemColors[enColorIndex];
	else
		return color.m_clrColor;
}

void CEGButton::PostNcDestroy()
{
	if(m_Menu.m_hMenu != NULL)
		m_Menu.Detach();
	CButton::PostNcDestroy();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CEGButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

