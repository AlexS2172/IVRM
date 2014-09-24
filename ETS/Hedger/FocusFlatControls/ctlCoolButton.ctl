VERSION 5.00
Begin VB.UserControl CoolButton 
   AutoRedraw      =   -1  'True
   BackColor       =   &H0080FF80&
   ClientHeight    =   2190
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3705
   KeyPreview      =   -1  'True
   ScaleHeight     =   146
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   247
   Begin VB.Image imgIcon 
      Appearance      =   0  'Flat
      Height          =   375
      Left            =   600
      Stretch         =   -1  'True
      Top             =   1350
      Width           =   495
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "MET Options Calculator"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   1440
      TabIndex        =   0
      Top             =   1200
      Width           =   2055
   End
End
Attribute VB_Name = "CoolButton"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Const INT_BEFOREHEADER_WIDTH = 15
Const INT_HEADER_WIDTH = 55
Const INT_HEADER_HEIGHT = 4
Const INT_BUTTON_WIDTH = 155
Const INT_BUTTON_HEIGHT_COLLAPSED = 25
Const INT_BUTTON_HEIGHT_EXPANDED = 40
Const INT_CORNER_HEIGHT = INT_BUTTON_HEIGHT_COLLAPSED - 2 * INT_HEADER_HEIGHT
Const INT_SHADOW_WIDTH = 2

Const INT_ICON_LEFT = INT_BEFOREHEADER_WIDTH + INT_HEADER_HEIGHT + 3
Const INT_ICON_TOP = INT_HEADER_HEIGHT + 4
Const INT_ICON_WIDTH = INT_HEADER_WIDTH - 6
Const INT_ICON_HEIGHT = INT_BUTTON_HEIGHT_EXPANDED - 2 * INT_HEADER_HEIGHT - INT_SHADOW_WIDTH - 5

Const INT_ARROW_TOP = 15
Const INT_ARROW_HEIGHT = 8
Const INT_ARROW_WIDTH = INT_ARROW_HEIGHT * 2 - 1


Const COLOR_COLLAPSED_E = &H109EEF
Const COLOR_EXPANDED_E = &H4AC5FF
Const COLOR_HEADER_E = &H318EC6
Const COLOR_TEXT_E = &H10305A
Const COLOR_BORDER_E = vbBlack
Const COLOR_SHADOW_E = &H505050
Const COLOR_HEADER_DOTS_E = vbWhite
Const COLOR_ARROW_E = COLOR_HEADER_E
Const COLOR_ARROW_BORDER_E = &H42BEEF

Const COLOR_COLLAPSED_D = &H909090
Const COLOR_EXPANDED_D = &HC0C0C0
Const COLOR_HEADER_D = &H505050
Const COLOR_TEXT_D = vbBlack
Const COLOR_BORDER_D = vbBlack
Const COLOR_SHADOW_D = &H505050
Const COLOR_HEADER_DOTS_D = vbWhite
Const COLOR_ARROW_D = &H707070
Const COLOR_ARROW_BORDER_D = &H909090

Private miCOLOR_COLLAPSED As OLE_COLOR
Private miCOLOR_EXPANDED As OLE_COLOR
Private miCOLOR_HEADER As OLE_COLOR
Private miCOLOR_TEXT As OLE_COLOR
Private miCOLOR_BORDER As OLE_COLOR
Private miCOLOR_SHADOW As OLE_COLOR
Private miCOLOR_HEADER_DOTS As OLE_COLOR
Private miCOLOR_ARROW As OLE_COLOR
Private miCOLOR_ARROW_BORDER As OLE_COLOR



'Default Property Values:
Const m_def_Collapsed = True

Private m_ColorCollapsed As OLE_COLOR
Private m_ColorExpanded As OLE_COLOR
Private m_ColorHeader As OLE_COLOR
Private m_ColorText As OLE_COLOR
Private m_ColorBorder As OLE_COLOR
Private m_ColorShadow As OLE_COLOR
Private m_ColorHeaderDots As OLE_COLOR
Private m_ColorArrow As OLE_COLOR
Private m_ColorArrowBorder As OLE_COLOR


'Dim m_Picture As Picture
Dim mbCollapsed As Boolean
Dim mbEnabled As Boolean
Dim mbHovered As Boolean
Dim mbPressed As Boolean


Private marrButtonRgn(20) As POINTAPI
Private miButtonRgnPtr As Long
Private miRgnPointCount As Long

Private marrButtonBorder(20) As POINTAPI
Private miBorderPointCount As Long

Private marrButtonShadow(20) As POINTAPI
Private miShadowPointCount As Long

Private marrCorner(5) As POINTAPI
Private miCornerPointCount As Long

Private marrHeader(10) As POINTAPI
Private miHeaderPointCount As Long

Private marrDots(4) As POINTAPI

Private marrArrow(10) As POINTAPI
Private miArrowPointCount As Long


Private miCornerRgnPtr As Long
Private miHeaderRgnPtr As Long


'Event Declarations:
Event KeyDown(KeyCode As Integer, Shift As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyDown
Attribute KeyDown.VB_Description = "Occurs when the user presses a key while an object has the focus."
Event KeyPress(KeyAscii As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyPress
Attribute KeyPress.VB_Description = "Occurs when the user presses and releases an ANSI key."
Event KeyUp(KeyCode As Integer, Shift As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyUp
Attribute KeyUp.VB_Description = "Occurs when the user releases a key while an object has the focus."
Event Click() 'MappingInfo=UserControl,UserControl,-1,Click
Event DblClick() 'MappingInfo=UserControl,UserControl,-1,DblClick
Event MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseDown
Event MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseMove
Event MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseUp
Event Collapse(ByVal Collapsed As Boolean)

Public Tag2 As Variant
Public ParentSubGroup As String
Public ParentGroup As String
Public ButtonID As Long

Const DBLCLICK_TIME = 0.01
Private mfDblClickTime As Double

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Enabled
Public Property Get Enabled() As Boolean
Attribute Enabled.VB_Description = "Returns/sets a value that determines whether an object can respond to user-generated events."
    Enabled = mbEnabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
    mbEnabled = New_Enabled
    DrawButton
    PropertyChanged "Enabled"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,0
Public Property Get Collapsed() As Boolean
    Collapsed = mbCollapsed
End Property

Public Property Let Collapsed(ByVal New_Collapsed As Boolean)
    mbCollapsed = New_Collapsed
    DrawButton
    PropertyChanged "Collapsed"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=lblCaption,lblCaption,-1,Caption
Public Property Get Caption() As String
Attribute Caption.VB_Description = "Returns/sets the text displayed in an object's title bar or below an object's icon."
    Caption = lblCaption.Caption
End Property

Public Property Let Caption(ByVal New_Caption As String)
    lblCaption.Caption() = New_Caption
    PropertyChanged "Caption"
End Property



Private Sub UserControl_GotFocus()
    DrawButton
End Sub

Private Sub UserControl_Hide()
On Error Resume Next
    DeleteObject miCornerRgnPtr
    DeleteObject miHeaderRgnPtr
End Sub

Private Sub UserControl_Initialize()
    mfDblClickTime = -1
End Sub

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
    mbCollapsed = m_def_Collapsed
    mbEnabled = True
'    Set m_Picture = LoadPicture("")
    m_ColorCollapsed = COLOR_COLLAPSED_E
    m_ColorExpanded = COLOR_EXPANDED_E
    m_ColorHeader = COLOR_HEADER_E
    m_ColorHeaderDots = COLOR_HEADER_DOTS_E
    m_ColorText = COLOR_TEXT_E
    m_ColorBorder = COLOR_BORDER_E
    m_ColorShadow = COLOR_SHADOW_E
    m_ColorArrow = COLOR_ARROW_E
    m_ColorArrowBorder = COLOR_ARROW_BORDER_E
End Sub

Private Sub UserControl_KeyUp(KeyCode As Integer, Shift As Integer)
    
    RaiseEvent KeyUp(KeyCode, Shift)
    
    Select Case KeyCode
    Case vbKeySpace, vbKeyF4
        mbCollapsed = Not mbCollapsed
        DrawButton
        Exit Sub
    End Select
End Sub

Private Sub UserControl_LostFocus()
    DrawButton
End Sub

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)

    mbEnabled = PropBag.ReadProperty("Enabled", True)
    mbCollapsed = PropBag.ReadProperty("Collapsed", m_def_Collapsed)
    Set Picture = PropBag.ReadProperty("Picture", Nothing)
    lblCaption.Caption = PropBag.ReadProperty("Caption", "MET Options Calculator")
'    Set m_Picture = PropBag.ReadProperty("Picture", Nothing)
    Set Picture = PropBag.ReadProperty("Picture", Nothing)
    
    Tag2 = PropBag.ReadProperty("Tag2", Empty)

    Set MouseIcon = PropBag.ReadProperty("MouseIcon", Nothing)
    UserControl.MousePointer = PropBag.ReadProperty("MousePointer", 0)
    m_ColorCollapsed = PropBag.ReadProperty("ColorCollapsed", COLOR_COLLAPSED_E)
    m_ColorExpanded = PropBag.ReadProperty("ColorExpanded", COLOR_EXPANDED_E)
    m_ColorHeader = PropBag.ReadProperty("ColorHeader", COLOR_HEADER_E)
    m_ColorHeaderDots = PropBag.ReadProperty("ColorHeaderDots", COLOR_HEADER_DOTS_E)
    m_ColorText = PropBag.ReadProperty("ColorText", COLOR_TEXT_E)
    m_ColorBorder = PropBag.ReadProperty("ColorBorder", COLOR_BORDER_E)
    m_ColorShadow = PropBag.ReadProperty("ColorShadow", COLOR_SHADOW_E)
    m_ColorArrow = PropBag.ReadProperty("ColorArrow", COLOR_ARROW_E)
    m_ColorArrowBorder = PropBag.ReadProperty("ColorArrowBorder", COLOR_ARROW_BORDER_E)
End Sub

Private Sub UserControl_Resize()
    DrawButton
End Sub

Private Sub UserControl_Terminate()
'
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)

    Call PropBag.WriteProperty("Enabled", mbEnabled, True)
    Call PropBag.WriteProperty("Collapsed", mbCollapsed, m_def_Collapsed)
    Call PropBag.WriteProperty("Picture", Picture, Nothing)
    Call PropBag.WriteProperty("Caption", lblCaption.Caption, "MET Options Calculator")
'    Call PropBag.WriteProperty("Picture", m_Picture, Nothing)
    Call PropBag.WriteProperty("Picture", Picture, Nothing)

    Call PropBag.WriteProperty("Tag2", Tag2, Empty)
    Call PropBag.WriteProperty("MouseIcon", MouseIcon, Nothing)
    Call PropBag.WriteProperty("MousePointer", UserControl.MousePointer, 0)
    Call PropBag.WriteProperty("ColorCollapsed", m_ColorCollapsed, COLOR_COLLAPSED_E)
    Call PropBag.WriteProperty("ColorExpanded", m_ColorExpanded, COLOR_EXPANDED_E)
    Call PropBag.WriteProperty("ColorHeader", m_ColorHeader, COLOR_HEADER_E)
    Call PropBag.WriteProperty("ColorHeaderDots", m_ColorHeaderDots, COLOR_HEADER_DOTS_E)
    Call PropBag.WriteProperty("ColorText", m_ColorText, COLOR_TEXT_E)
    Call PropBag.WriteProperty("ColorBorder", m_ColorBorder, COLOR_BORDER_E)
    Call PropBag.WriteProperty("ColorShadow", m_ColorShadow, COLOR_SHADOW_E)
    Call PropBag.WriteProperty("ColorArrow", m_ColorArrow, COLOR_ARROW_E)
    Call PropBag.WriteProperty("ColorArrowBorder", m_ColorArrowBorder, COLOR_ARROW_BORDER_E)
End Sub



Private Sub DrawCarvedButton()
Dim iPen As Long
Dim iBrush As Long
Dim iBackColor As Long
Dim i As Long
Static bInProc As Boolean

    'If bInProc Then Exit Sub
    bInProc = True

    marrButtonRgn(0).x = 0
    marrButtonRgn(0).Y = INT_HEADER_HEIGHT
    marrButtonRgn(1).x = INT_BEFOREHEADER_WIDTH
    marrButtonRgn(1).Y = marrButtonRgn(0).Y
    marrButtonRgn(2).x = marrButtonRgn(1).x + INT_HEADER_HEIGHT
    marrButtonRgn(2).Y = 0
    marrButtonRgn(3).x = marrButtonRgn(2).x + INT_HEADER_WIDTH
    marrButtonRgn(3).Y = 0
    marrButtonRgn(4).x = marrButtonRgn(3).x + INT_HEADER_HEIGHT
    marrButtonRgn(4).Y = marrButtonRgn(0).Y
    marrButtonRgn(5).x = INT_BUTTON_WIDTH
    marrButtonRgn(5).Y = marrButtonRgn(0).Y
    marrButtonRgn(6).x = marrButtonRgn(5).x
    marrButtonRgn(6).Y = marrButtonRgn(5).Y + INT_SHADOW_WIDTH
    marrButtonRgn(7).x = marrButtonRgn(6).x + INT_SHADOW_WIDTH
    marrButtonRgn(7).Y = marrButtonRgn(6).Y
    
    If mbCollapsed Then
        miRgnPointCount = 14
        marrButtonRgn(8).x = marrButtonRgn(7).x
        marrButtonRgn(8).Y = marrButtonRgn(7).Y + INT_CORNER_HEIGHT
        marrButtonRgn(9).x = INT_BEFOREHEADER_WIDTH + INT_HEADER_HEIGHT + INT_SHADOW_WIDTH
        marrButtonRgn(9).Y = marrButtonRgn(8).Y
        marrButtonRgn(10).x = marrButtonRgn(9).x - INT_HEADER_HEIGHT
        marrButtonRgn(10).Y = marrButtonRgn(9).Y + INT_HEADER_HEIGHT
        marrButtonRgn(11).x = marrButtonRgn(10).x - INT_BEFOREHEADER_WIDTH
        marrButtonRgn(11).Y = marrButtonRgn(10).Y
        marrButtonRgn(12).x = marrButtonRgn(11).x
        marrButtonRgn(12).Y = INT_BUTTON_HEIGHT_COLLAPSED
        marrButtonRgn(13).x = 0
        marrButtonRgn(13).Y = marrButtonRgn(12).Y
        
    Else
        miRgnPointCount = 17
        marrButtonRgn(8).x = marrButtonRgn(7).x
        marrButtonRgn(8).Y = INT_BUTTON_HEIGHT_EXPANDED + INT_SHADOW_WIDTH - INT_CORNER_HEIGHT
        marrButtonRgn(9).x = marrButtonRgn(7).x - INT_CORNER_HEIGHT
        marrButtonRgn(9).Y = INT_BUTTON_HEIGHT_EXPANDED + INT_SHADOW_WIDTH
        marrButtonRgn(10).x = INT_BEFOREHEADER_WIDTH + INT_HEADER_HEIGHT * 2 + INT_HEADER_WIDTH + INT_SHADOW_WIDTH
        marrButtonRgn(10).Y = marrButtonRgn(9).Y
        marrButtonRgn(11).x = marrButtonRgn(10).x - INT_HEADER_HEIGHT
        marrButtonRgn(11).Y = marrButtonRgn(10).Y - INT_HEADER_HEIGHT
        marrButtonRgn(12).x = marrButtonRgn(11).x - INT_HEADER_WIDTH
        marrButtonRgn(12).Y = marrButtonRgn(11).Y
        marrButtonRgn(13).x = marrButtonRgn(12).x - INT_HEADER_HEIGHT
        marrButtonRgn(13).Y = marrButtonRgn(12).Y + INT_HEADER_HEIGHT
        marrButtonRgn(14).x = marrButtonRgn(13).x - INT_BEFOREHEADER_WIDTH
        marrButtonRgn(14).Y = marrButtonRgn(13).Y
        marrButtonRgn(15).x = marrButtonRgn(14).x
        marrButtonRgn(15).Y = marrButtonRgn(14).Y - INT_SHADOW_WIDTH
        marrButtonRgn(16).x = marrButtonRgn(15).x - INT_SHADOW_WIDTH
        marrButtonRgn(16).Y = marrButtonRgn(15).Y
    End If
    
    If miButtonRgnPtr <> 0 Then DeleteObject miButtonRgnPtr
    miButtonRgnPtr = CreatePolygonRgn(marrButtonRgn(0), miRgnPointCount, 1)
    SetWindowRgn hWnd, miButtonRgnPtr, True
    
    
    ' border
    marrButtonBorder(0).x = marrButtonRgn(0).x
    marrButtonBorder(0).Y = marrButtonRgn(0).Y
    marrButtonBorder(1).x = marrButtonRgn(1).x
    marrButtonBorder(1).Y = marrButtonRgn(1).Y
    marrButtonBorder(2).x = marrButtonRgn(2).x
    marrButtonBorder(2).Y = marrButtonRgn(2).Y
    marrButtonBorder(3).x = marrButtonRgn(3).x - 1
    marrButtonBorder(3).Y = marrButtonRgn(3).Y
    marrButtonBorder(4).x = marrButtonRgn(4).x - 1
    marrButtonBorder(4).Y = marrButtonRgn(4).Y
    marrButtonBorder(5).x = marrButtonRgn(5).x - 1
    marrButtonBorder(5).Y = marrButtonRgn(5).Y
    If mbCollapsed Then
        miBorderPointCount = 11
        marrButtonBorder(6).x = marrButtonRgn(8).x - INT_SHADOW_WIDTH - 1
        marrButtonBorder(6).Y = marrButtonRgn(8).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(7).x = marrButtonRgn(9).x - INT_SHADOW_WIDTH + 1
        marrButtonBorder(7).Y = marrButtonRgn(9).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(8).x = marrButtonRgn(10).x - INT_SHADOW_WIDTH + 1
        marrButtonBorder(8).Y = marrButtonRgn(10).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(9).x = marrButtonRgn(11).x - INT_SHADOW_WIDTH
        marrButtonBorder(9).Y = marrButtonRgn(11).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(10).x = marrButtonBorder(0).x
        marrButtonBorder(10).Y = marrButtonBorder(0).Y
    Else
        miBorderPointCount = 14
        marrButtonBorder(6).x = marrButtonRgn(8).x - INT_SHADOW_WIDTH - 1
        marrButtonBorder(6).Y = marrButtonRgn(8).Y - INT_SHADOW_WIDTH + 1
        marrButtonBorder(7).x = marrButtonRgn(9).x - INT_SHADOW_WIDTH + 1
        marrButtonBorder(7).Y = marrButtonRgn(9).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(8).x = marrButtonRgn(10).x - INT_SHADOW_WIDTH - 1
        marrButtonBorder(8).Y = marrButtonRgn(10).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(9).x = marrButtonRgn(11).x - INT_SHADOW_WIDTH - 1
        marrButtonBorder(9).Y = marrButtonRgn(11).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(10).x = marrButtonRgn(12).x - INT_SHADOW_WIDTH + 1
        marrButtonBorder(10).Y = marrButtonRgn(12).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(11).x = marrButtonRgn(13).x - INT_SHADOW_WIDTH + 1
        marrButtonBorder(11).Y = marrButtonRgn(13).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(12).x = marrButtonRgn(14).x - INT_SHADOW_WIDTH
        marrButtonBorder(12).Y = marrButtonRgn(14).Y - INT_SHADOW_WIDTH - 1
        marrButtonBorder(13).x = marrButtonBorder(0).x
        marrButtonBorder(13).Y = marrButtonBorder(0).Y
    End If
    
    If mbCollapsed Then
        iBackColor = miCOLOR_COLLAPSED
    Else
        iBackColor = miCOLOR_EXPANDED
    End If
    iBrush = CreateSolidBrush(iBackColor)
    
    If GetFocus = hWnd Then
        iPen = CreatePen(0, 3, miCOLOR_BORDER)
    Else
        iPen = CreatePen(0, 1, miCOLOR_BORDER)
    End If

    SelectObject hdc, iPen
    SelectObject hdc, iBrush
    Polygon hdc, marrButtonBorder(0), miBorderPointCount
    DeleteObject iPen
    DeleteObject iBrush
    
    
    
'    If GetFocus = hwnd Then
'        iPen = CreatePen(0, 3, miCOLOR_BORDER)
'    Else
'        iPen = CreatePen(0, 1, miCOLOR_BORDER)
'    End If
'
'    SelectObject hdc, iPen
'    Polyline hdc, marrButtonBorder(0), miBorderPointCount
'    DeleteObject iPen
    
    
    ' corner
    miCornerPointCount = 3
    If GetFocus = hWnd Then
        iPen = CreatePen(0, 3, miCOLOR_BORDER)
    Else
        iPen = CreatePen(0, 1, miCOLOR_BORDER)
    End If
    If mbCollapsed Then
        iBrush = CreateSolidBrush(miCOLOR_EXPANDED)
        marrCorner(0).x = marrButtonBorder(5).x
        marrCorner(0).Y = marrButtonBorder(5).Y + 1
        marrCorner(1).x = marrButtonBorder(6).x
        marrCorner(1).Y = marrButtonBorder(6).Y
        marrCorner(2).x = marrCorner(1).x - INT_CORNER_HEIGHT + 2
        marrCorner(2).Y = marrCorner(1).Y
    Else
        iBrush = CreateSolidBrush(miCOLOR_COLLAPSED)
        marrCorner(0).x = marrButtonBorder(6).x
        marrCorner(0).Y = marrButtonBorder(6).Y
        marrCorner(1).x = marrButtonBorder(7).x
        marrCorner(1).Y = marrButtonBorder(7).Y
        marrCorner(2).x = marrCorner(1).x
        marrCorner(2).Y = marrCorner(0).Y
    End If
    
    SelectObject hdc, iPen
    SelectObject hdc, iBrush
    Polygon hdc, marrCorner(0), miCornerPointCount
    DeleteObject iPen
    DeleteObject iBrush
    
    If miCornerRgnPtr <> 0 Then DeleteObject miCornerRgnPtr
    miCornerRgnPtr = CreatePolygonRgn(marrCorner(0), miCornerPointCount, 1)
    
    
    ' shadow
    If mbCollapsed Then
        miShadowPointCount = 5
        marrButtonShadow(0).x = marrButtonRgn(7).x - 1
        marrButtonShadow(0).Y = marrButtonRgn(7).Y - 1
        marrButtonShadow(1).x = marrButtonRgn(8).x - 1
        marrButtonShadow(1).Y = marrButtonRgn(8).Y - 1
        marrButtonShadow(2).x = marrButtonRgn(9).x - 1
        marrButtonShadow(2).Y = marrButtonRgn(9).Y - 1
        marrButtonShadow(3).x = marrButtonRgn(10).x - 1
        marrButtonShadow(3).Y = marrButtonRgn(10).Y - 1
        marrButtonShadow(4).x = marrButtonRgn(11).x - 1
        marrButtonShadow(4).Y = marrButtonRgn(11).Y - 1
    Else
        miShadowPointCount = 8
        marrButtonShadow(0).x = marrButtonRgn(7).x - 1
        marrButtonShadow(0).Y = marrButtonRgn(7).Y - 1
        marrButtonShadow(1).x = marrButtonRgn(8).x - 1
        marrButtonShadow(1).Y = marrButtonRgn(8).Y - 1
        marrButtonShadow(2).x = marrButtonRgn(9).x - 1
        marrButtonShadow(2).Y = marrButtonRgn(9).Y - 1
        marrButtonShadow(3).x = marrButtonRgn(10).x - 3
        marrButtonShadow(3).Y = marrButtonRgn(10).Y - 1
        marrButtonShadow(4).x = marrButtonRgn(11).x - 3
        marrButtonShadow(4).Y = marrButtonRgn(11).Y - 1
        marrButtonShadow(5).x = marrButtonRgn(12).x - 1
        marrButtonShadow(5).Y = marrButtonRgn(12).Y - 1
        marrButtonShadow(6).x = marrButtonRgn(13).x - 1
        marrButtonShadow(6).Y = marrButtonRgn(13).Y - 1
        marrButtonShadow(7).x = marrButtonRgn(14).x - 1
        marrButtonShadow(7).Y = marrButtonRgn(14).Y - 1
    End If
    iPen = CreatePen(0, 2, miCOLOR_SHADOW)
    SelectObject hdc, iPen
    Polyline hdc, marrButtonShadow(0), miShadowPointCount
    DeleteObject iPen
    'DeleteObject miCornerRgnPtr
    
    
    UserControl.Size (marrButtonShadow(0).x + 1) * Screen.TwipsPerPixelX, (marrButtonShadow(miShadowPointCount - 1).Y + 1) * Screen.TwipsPerPixelY
    
    
    ' header
    miHeaderPointCount = 6
    marrHeader(0).x = marrButtonBorder(0).x + 2
    marrHeader(0).Y = marrButtonBorder(0).Y + 2
    marrHeader(1).x = marrButtonBorder(1).x + 1
    marrHeader(1).Y = marrButtonBorder(1).Y + 2
    marrHeader(2).x = marrButtonBorder(2).x + 1
    marrHeader(2).Y = marrButtonBorder(2).Y + 2
    marrHeader(3).x = marrButtonBorder(3).x - 1
    marrHeader(3).Y = marrButtonBorder(3).Y + 2
    marrHeader(4).x = marrButtonBorder(4).x - 1
    marrHeader(4).Y = marrButtonBorder(4).Y + 2
    marrHeader(5).x = marrButtonBorder(5).x - 2
    marrHeader(5).Y = marrButtonBorder(5).Y + 2
    
    If Not mbCollapsed Then
        iPen = CreatePen(0, 1, miCOLOR_HEADER)
        iBrush = CreateSolidBrush(miCOLOR_HEADER)
        SelectObject hdc, iPen
        SelectObject hdc, iBrush
        Polygon hdc, marrHeader(0), miHeaderPointCount
        DeleteObject iPen
        DeleteObject iBrush
        
    End If
    
    Dim arrHeader(3) As POINTAPI
    arrHeader(0).x = marrButtonRgn(1).x
    arrHeader(0).Y = marrButtonRgn(1).Y
    arrHeader(1).x = marrButtonRgn(2).x
    arrHeader(1).Y = marrButtonRgn(2).Y
    arrHeader(2).x = marrButtonRgn(3).x
    arrHeader(2).Y = marrButtonRgn(3).Y
    arrHeader(3).x = marrButtonRgn(4).x
    arrHeader(3).Y = marrButtonRgn(4).Y
    If miHeaderRgnPtr <> 0 Then DeleteObject miHeaderRgnPtr
    miHeaderRgnPtr = CreatePolygonRgn(arrHeader(0), 4, 1)
    
    ' dots
    iPen = CreatePen(0, 1, miCOLOR_HEADER_DOTS)
    SelectObject hdc, iPen
    Dim iStartX As Long
    Dim iStepX As Long
    iStartX = marrButtonBorder(2).x + 7
    iStepX = 4
    For i = 0 To 2
        marrDots(0).x = iStartX + iStepX * i
        marrDots(0).Y = marrButtonBorder(2).Y + 3
        marrDots(1).x = marrDots(0).x + 1
        marrDots(1).Y = marrDots(0).Y
        marrDots(2).x = marrDots(1).x
        marrDots(2).Y = marrDots(1).Y + 1
        marrDots(3).x = marrDots(2).x - 1
        marrDots(3).Y = marrDots(2).Y
        marrDots(4).x = marrDots(0).x
        marrDots(4).Y = marrDots(0).Y
        
        Polyline hdc, marrDots(0), 5
    Next i
    DeleteObject iPen
    'DeleteObject miHeaderRgnPtr
    
    ' arrows
    If Not mbCollapsed Then
        iPen = CreatePen(0, 1, miCOLOR_ARROW_BORDER)
        iBrush = CreateSolidBrush(miCOLOR_ARROW)
        SelectObject hdc, iPen
        SelectObject hdc, iBrush
        
        miArrowPointCount = 3
        marrArrow(0).x = 3
        marrArrow(0).Y = INT_ARROW_TOP
        marrArrow(1).x = marrArrow(0).x + INT_ARROW_WIDTH - 1
        marrArrow(1).Y = marrArrow(0).Y
        marrArrow(2).x = marrArrow(0).x + INT_ARROW_HEIGHT - 1
        marrArrow(2).Y = marrArrow(0).Y + INT_ARROW_HEIGHT - 1
        Polygon hdc, marrArrow(0), miArrowPointCount
        
        miArrowPointCount = 3
        marrArrow(0).x = 3
        marrArrow(0).Y = marrArrow(2).Y
        marrArrow(1).x = marrArrow(0).x + INT_ARROW_WIDTH - 1
        marrArrow(1).Y = marrArrow(0).Y
        marrArrow(2).x = marrArrow(0).x + INT_ARROW_HEIGHT - 1
        marrArrow(2).Y = marrArrow(0).Y + INT_ARROW_HEIGHT - 1
        Polygon hdc, marrArrow(0), miArrowPointCount
        
        miArrowPointCount = 4
        marrArrow(0).x = 3
        marrArrow(0).Y = marrArrow(2).Y
        marrArrow(1).x = marrArrow(0).x + INT_ARROW_WIDTH - 1
        marrArrow(1).Y = marrArrow(0).Y
        marrArrow(2).x = marrArrow(1).x
        marrArrow(2).Y = marrArrow(0).Y + 2
        marrArrow(3).x = marrArrow(0).x
        marrArrow(3).Y = marrArrow(0).Y + 2
        Polygon hdc, marrArrow(0), miArrowPointCount
        
        DeleteObject iPen
        DeleteObject iBrush
        
    End If

    bInProc = False
End Sub


Private Sub DrawRectButton()
Dim iPen As Long
Dim iBrush As Long
Dim iBackColor As Long
Dim iButtonHeight As Long
Dim i As Long
Static bInProc As Boolean

    If bInProc Then Exit Sub
    bInProc = True


    If mbCollapsed Then
        iButtonHeight = INT_BUTTON_HEIGHT_COLLAPSED
        iBackColor = miCOLOR_COLLAPSED
    Else
        iButtonHeight = INT_BUTTON_HEIGHT_EXPANDED
        iBackColor = miCOLOR_EXPANDED
    End If
    UserControl.Size (INT_BUTTON_WIDTH) * Screen.TwipsPerPixelX, (iButtonHeight) * Screen.TwipsPerPixelY


    'shadow
    DrawWidth = INT_SHADOW_WIDTH
    Line (-1, iButtonHeight - INT_SHADOW_WIDTH + 1)-(INT_BUTTON_WIDTH - 1, iButtonHeight - INT_SHADOW_WIDTH + 1), miCOLOR_SHADOW
    Line (INT_BUTTON_WIDTH - INT_SHADOW_WIDTH + 1, 0)-(INT_BUTTON_WIDTH - INT_SHADOW_WIDTH + 1, iButtonHeight), miCOLOR_SHADOW

    'body
    If GetFocus = hWnd Then
        DrawWidth = 3
    Else
        DrawWidth = 1
    End If
    FillColor = iBackColor
    FillStyle = 0
    Line (0, 0)-(INT_BUTTON_WIDTH - INT_SHADOW_WIDTH - 1, iButtonHeight - INT_SHADOW_WIDTH - 1), miCOLOR_BORDER, B
    
    'header
    DrawWidth = 1
    FillColor = miCOLOR_HEADER
    FillStyle = 0
    Line (2, 2)-(INT_BUTTON_WIDTH - INT_SHADOW_WIDTH - 1 - 2, 2 + INT_HEADER_HEIGHT), miCOLOR_HEADER, BF
    
    'dots
    Dim iStartX As Long
    Dim iStepX As Long
    iStartX = INT_BEFOREHEADER_WIDTH + 7
    iStepX = 4
    DrawWidth = 1
    For i = 0 To 2
        Line (iStartX + iStepX * i, 3)-(iStartX + iStepX * i + 1, 3 + 1), miCOLOR_HEADER_DOTS, BF
    Next i

    'arrows
    If Not mbCollapsed Then
        iPen = CreatePen(0, 1, miCOLOR_ARROW_BORDER)
        iBrush = CreateSolidBrush(miCOLOR_ARROW)
        SelectObject hdc, iPen
        SelectObject hdc, iBrush

        miArrowPointCount = 3
        marrArrow(0).x = 3
        marrArrow(0).Y = INT_ARROW_TOP
        marrArrow(1).x = marrArrow(0).x + INT_ARROW_WIDTH - 1
        marrArrow(1).Y = marrArrow(0).Y
        marrArrow(2).x = marrArrow(0).x + INT_ARROW_HEIGHT - 1
        marrArrow(2).Y = marrArrow(0).Y + INT_ARROW_HEIGHT - 1
        Polygon hdc, marrArrow(0), miArrowPointCount

        miArrowPointCount = 3
        marrArrow(0).x = 3
        marrArrow(0).Y = marrArrow(2).Y
        marrArrow(1).x = marrArrow(0).x + INT_ARROW_WIDTH - 1
        marrArrow(1).Y = marrArrow(0).Y
        marrArrow(2).x = marrArrow(0).x + INT_ARROW_HEIGHT - 1
        marrArrow(2).Y = marrArrow(0).Y + INT_ARROW_HEIGHT - 1
        Polygon hdc, marrArrow(0), miArrowPointCount

        miArrowPointCount = 4
        marrArrow(0).x = 3
        marrArrow(0).Y = marrArrow(2).Y
        marrArrow(1).x = marrArrow(0).x + INT_ARROW_WIDTH - 1
        marrArrow(1).Y = marrArrow(0).Y
        marrArrow(2).x = marrArrow(1).x
        marrArrow(2).Y = marrArrow(0).Y + 2
        marrArrow(3).x = marrArrow(0).x
        marrArrow(3).Y = marrArrow(0).Y + 2
        Polygon hdc, marrArrow(0), miArrowPointCount

        DeleteObject iPen
        DeleteObject iBrush

    End If


    ' corner
    If GetFocus = hWnd Then
        iPen = CreatePen(0, 3, miCOLOR_BORDER)
    Else
        iPen = CreatePen(0, 1, miCOLOR_BORDER)
    End If
    If mbCollapsed Then
        iBrush = CreateSolidBrush(miCOLOR_EXPANDED)
    Else
        iBrush = CreateSolidBrush(miCOLOR_COLLAPSED)
    End If

    miCornerPointCount = 3
    marrCorner(0).x = INT_BUTTON_WIDTH - INT_SHADOW_WIDTH - 1
    marrCorner(0).Y = iButtonHeight - INT_SHADOW_WIDTH - INT_CORNER_HEIGHT
    marrCorner(1).x = marrCorner(0).x
    marrCorner(1).Y = iButtonHeight - INT_SHADOW_WIDTH - 1
    marrCorner(2).x = INT_BUTTON_WIDTH - INT_SHADOW_WIDTH - INT_CORNER_HEIGHT
    marrCorner(2).Y = marrCorner(1).Y

    SelectObject hdc, iPen
    SelectObject hdc, iBrush
    Polygon hdc, marrCorner(0), miCornerPointCount
    DeleteObject iPen
    DeleteObject iBrush
    
    bInProc = False
End Sub



Public Sub DrawButton()

    If mbEnabled Then
        miCOLOR_COLLAPSED = m_ColorCollapsed
        miCOLOR_EXPANDED = m_ColorExpanded
        miCOLOR_HEADER = m_ColorHeader
        miCOLOR_TEXT = m_ColorText
        miCOLOR_BORDER = m_ColorBorder
        miCOLOR_SHADOW = m_ColorShadow
        miCOLOR_HEADER_DOTS = m_ColorHeaderDots
        miCOLOR_ARROW = m_ColorArrow
        miCOLOR_ARROW_BORDER = m_ColorArrowBorder
    Else
        miCOLOR_COLLAPSED = COLOR_COLLAPSED_D
        miCOLOR_EXPANDED = COLOR_EXPANDED_D
        miCOLOR_HEADER = COLOR_HEADER_D
        miCOLOR_TEXT = COLOR_TEXT_D
        miCOLOR_BORDER = COLOR_BORDER_D
        miCOLOR_SHADOW = COLOR_SHADOW_D
        miCOLOR_HEADER_DOTS = COLOR_HEADER_DOTS_D
        miCOLOR_ARROW = COLOR_ARROW_D
        miCOLOR_ARROW_BORDER = COLOR_ARROW_BORDER_D
    End If


    Cls
    
    On Error Resume Next
    '''UserControl.BackColor = Parent.BackColor
    
    
    If gbIsWin95 Then
        DrawRectButton
    Else
        DrawCarvedButton
    End If
    
    
    ' icon
    With imgIcon
        If mbCollapsed Then
            .Visible = False
        Else
            .Visible = True
            .Left = INT_ICON_LEFT
            .Top = INT_ICON_TOP
            .Width = INT_ICON_WIDTH
            .Height = INT_ICON_HEIGHT
        End If
    End With
    
    ' label
    With lblCaption
        .ForeColor = miCOLOR_TEXT
        If mbCollapsed Then
            .Left = INT_ICON_LEFT
            .Top = INT_ICON_TOP - 1
            .Width = INT_BUTTON_WIDTH - .Left - 4
            .Height = INT_BUTTON_HEIGHT_COLLAPSED - .Top - INT_HEADER_HEIGHT - 2
        Else
            .Left = imgIcon.Left + imgIcon.Width + 4
            .Top = INT_ICON_TOP
            .Width = INT_BUTTON_WIDTH - .Left - 4
            .Height = INT_BUTTON_HEIGHT_EXPANDED - .Top - INT_HEADER_HEIGHT - 2
        End If
    End With
    
End Sub


Private Function PointInCorner(ByVal x As Single, ByVal Y As Single) As Boolean
Dim iRes As Long
Dim b As Boolean
Dim a As Double
    If gbIsWin95 Then
        a = (marrCorner(0).x - marrCorner(2).x) / (marrCorner(0).Y - marrCorner(2).Y)
        a = a * (Y - marrCorner(2).Y) + marrCorner(2).x
        b = (x <= marrCorner(1).x) And (Y <= marrCorner(1).Y)
        b = b And (x >= a)
        PointInCorner = b
    Else
        iRes = PtInRegion(miCornerRgnPtr, CLng(x), CLng(Y))
        PointInCorner = iRes <> 0
    End If

End Function

Private Function PointInHeader(ByVal x As Single, ByVal Y As Single) As Boolean
Dim iRes As Long
    iRes = PtInRegion(miHeaderRgnPtr, CLng(x), CLng(Y))
    PointInHeader = iRes <> 0
End Function


Private Sub UserControl_Click()
Dim curPos As POINTAPI
Dim WinRect As RECT

    GetCursorPos curPos
    GetWindowRect hWnd, WinRect
    
    If PointInCorner(curPos.x - WinRect.Left, curPos.Y - WinRect.Top) Then
        mfDblClickTime = Timer
        mbCollapsed = Not mbCollapsed
        RaiseEvent Collapse(mbCollapsed)
        DrawButton
        Exit Sub
    End If
    
    'If PointInHeader(curPos.x - WinRect.Left, curPos.y - WinRect.Top) Then Debug.Print "Header"
    
    If mbEnabled Then
        RaiseEvent Click
    End If
End Sub

Private Sub UserControl_DblClick()
    If mfDblClickTime = -1 Or Timer - mfDblClickTime > DBLCLICK_TIME Then
        If mbEnabled Then
            RaiseEvent DblClick
        End If
    End If
    mfDblClickTime = -1
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    Dim curPos As POINTAPI
    Dim WinRect As RECT

    GetCursorPos curPos
    GetWindowRect hWnd, WinRect
    
    If Not PointInCorner(curPos.x - WinRect.Left, curPos.Y - WinRect.Top) Then
        RaiseEvent MouseDown(Button, Shift, x, Y)
    End If
    
End Sub

Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
    RaiseEvent MouseMove(Button, Shift, x, Y)
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    Dim curPos As POINTAPI
    Dim WinRect As RECT

    GetCursorPos curPos
    GetWindowRect hWnd, WinRect
    
    If Not PointInCorner(curPos.x - WinRect.Left, curPos.Y - WinRect.Top) Then
        RaiseEvent MouseUp(Button, Shift, x, Y)
    End If
    
End Sub


Private Sub lblCaption_Click()
    UserControl_Click
End Sub

Private Sub lblCaption_DblClick()
    UserControl_DblClick
End Sub

Private Sub lblCaption_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    UserControl_MouseDown Button, Shift, lblCaption.Left + x, lblCaption.Top + Y
End Sub

Private Sub lblCaption_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
    UserControl_MouseMove Button, Shift, lblCaption.Left + x, lblCaption.Top + Y
End Sub

Private Sub lblCaption_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    UserControl_MouseUp Button, Shift, lblCaption.Left + x, lblCaption.Top + Y
End Sub



Private Sub imgIcon_Click()
    UserControl_Click
End Sub

Private Sub imgIcon_DblClick()
    UserControl_DblClick
End Sub

Private Sub imgIcon_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    UserControl_MouseDown Button, Shift, imgIcon.Left + x, imgIcon.Top + Y
End Sub

Private Sub imgIcon_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
    UserControl_MouseMove Button, Shift, imgIcon.Left + x, imgIcon.Top + Y
End Sub

Private Sub imgIcon_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    UserControl_MouseUp Button, Shift, imgIcon.Left + x, imgIcon.Top + Y
End Sub


'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=imgIcon,imgIcon,-1,Picture
Public Property Get Picture() As Picture
Attribute Picture.VB_Description = "Returns/sets a graphic to be displayed in a control."
    Set Picture = imgIcon.Picture
End Property

Public Property Set Picture(ByVal New_Picture As Picture)
    Set imgIcon.Picture = New_Picture
    PropertyChanged "Picture"
End Property

Private Sub UserControl_KeyDown(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub UserControl_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,MouseIcon
Public Property Get MouseIcon() As Picture
Attribute MouseIcon.VB_Description = "Sets a custom mouse icon."
    Set MouseIcon = UserControl.MouseIcon
End Property

Public Property Set MouseIcon(ByVal New_MouseIcon As Picture)
    Set UserControl.MouseIcon = New_MouseIcon
    Set imgIcon.MouseIcon = New_MouseIcon
    Set lblCaption.MouseIcon = New_MouseIcon
    PropertyChanged "MouseIcon"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,MousePointer
Public Property Get MousePointer() As MousePointerConstants
Attribute MousePointer.VB_Description = "Returns/sets the type of mouse pointer displayed when over part of an object."
    MousePointer = UserControl.MousePointer
End Property

Public Property Let MousePointer(ByVal New_MousePointer As MousePointerConstants)
    UserControl.MousePointer() = New_MousePointer
    imgIcon.MousePointer() = New_MousePointer
    lblCaption.MousePointer() = New_MousePointer
    PropertyChanged "MousePointer"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorCollapsed() As OLE_COLOR
    ColorCollapsed = m_ColorCollapsed
End Property

Public Property Let ColorCollapsed(ByVal New_ColorCollapsed As OLE_COLOR)
    m_ColorCollapsed = New_ColorCollapsed
    DrawButton
    PropertyChanged "ColorCollapsed"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorExpanded() As OLE_COLOR
    ColorExpanded = m_ColorExpanded
End Property

Public Property Let ColorExpanded(ByVal New_ColorExpanded As OLE_COLOR)
    m_ColorExpanded = New_ColorExpanded
    DrawButton
    PropertyChanged "ColorExpanded"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorHeader() As OLE_COLOR
    ColorHeader = m_ColorHeader
End Property

Public Property Let ColorHeader(ByVal New_ColorHeader As OLE_COLOR)
    m_ColorHeader = New_ColorHeader
    DrawButton
    PropertyChanged "ColorHeader"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorHeaderDots() As OLE_COLOR
    ColorHeaderDots = m_ColorHeaderDots
End Property

Public Property Let ColorHeaderDots(ByVal New_ColorHeaderDots As OLE_COLOR)
    m_ColorHeaderDots = New_ColorHeaderDots
    DrawButton
    PropertyChanged "ColorHeaderDots"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorText() As OLE_COLOR
    ColorText = m_ColorText
End Property

Public Property Let ColorText(ByVal New_ColorText As OLE_COLOR)
    m_ColorText = New_ColorText
    DrawButton
    PropertyChanged "ColorText"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorBorder() As OLE_COLOR
    ColorBorder = m_ColorBorder
End Property

Public Property Let ColorBorder(ByVal New_ColorBorder As OLE_COLOR)
    m_ColorBorder = New_ColorBorder
    DrawButton
    PropertyChanged "ColorBorder"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorShadow() As OLE_COLOR
    ColorShadow = m_ColorShadow
End Property

Public Property Let ColorShadow(ByVal New_ColorShadow As OLE_COLOR)
    m_ColorShadow = New_ColorShadow
    DrawButton
    PropertyChanged "ColorShadow"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorArrow() As OLE_COLOR
    ColorArrow = m_ColorArrow
End Property

Public Property Let ColorArrow(ByVal New_ColorArrow As OLE_COLOR)
    m_ColorArrow = New_ColorArrow
    DrawButton
    PropertyChanged "ColorArrow"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ColorArrowBorder() As OLE_COLOR
    ColorArrowBorder = m_ColorArrowBorder
End Property

Public Property Let ColorArrowBorder(ByVal New_ColorArrowBorder As OLE_COLOR)
    m_ColorArrowBorder = New_ColorArrowBorder
    DrawButton
    PropertyChanged "ColorArrowBorder"
End Property

