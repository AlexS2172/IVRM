VERSION 5.00
Begin VB.UserControl CoolFormHeader 
   Alignable       =   -1  'True
   AutoRedraw      =   -1  'True
   BackColor       =   &H00088ECE&
   CanGetFocus     =   0   'False
   ClientHeight    =   1200
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   2955
   FillColor       =   &H00C0E0FF&
   FillStyle       =   0  'Solid
   ForeColor       =   &H00FFFFC0&
   ScaleHeight     =   80
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   197
   ToolboxBitmap   =   "ctlCoolFormHeader.ctx":0000
   Begin VB.Image imgClose 
      Height          =   375
      Left            =   840
      MouseIcon       =   "ctlCoolFormHeader.ctx":0532
      MousePointer    =   99  'Custom
      Top             =   480
      Width           =   375
   End
   Begin VB.Label lblCaption 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H8000000E&
      Height          =   195
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   585
   End
End
Attribute VB_Name = "CoolFormHeader"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Private Const INT_CAPHEIGHT = 25
Private Const INT_MARGINE = 5
Private Const INT_BUTTONMARGINE = 2
Private Const INT_SHADDOWWIDTH = 3

Dim arrFormPoly(9) As POINTAPI
Dim arrCapPoly(3) As POINTAPI
Dim iParH As Long
Dim iParW As Long

Private iCaptionRgnPtr As Long
Private iFormRgnPtr As Long

Const COLOR_SHADOW = &H606060
Const COLOR_BACKCOLOR = &H4080&
Const COLOR_FORECOLOR = &HFFFFFF



Private Sub lblCaption_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
On Error Resume Next
    ReleaseCapture
    SendMessage Parent.hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0&
End Sub


Private Sub imgClose_Click()
On Error Resume Next
    Unload Parent
End Sub

Private Sub UserControl_InitProperties()
On Error Resume Next
    UserControl.BackColor = COLOR_BACKCOLOR
    'Set lblCaption.Font = Ambient.Font
    lblCaption.ForeColor = COLOR_FORECOLOR
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
On Error Resume Next
    ReleaseCapture
    SendMessage Parent.hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0&
End Sub


Public Sub DrawCaption()
Static bInProc As Boolean
Dim iPen As Long
Dim arrLine(1) As POINTAPI
On Error Resume Next
        
        
    If bInProc Then Exit Sub
    bInProc = True
    
    iParW = Parent.Width / Screen.TwipsPerPixelX
    iParH = Parent.Height / Screen.TwipsPerPixelY
    
    Extender.Align = vbAlignTop
    Extender.Height = INT_CAPHEIGHT * Screen.TwipsPerPixelY
    Parent.AutoRedraw = True
    
    
    With imgClose
        .Width = (INT_CAPHEIGHT - INT_MARGINE - 2 * INT_BUTTONMARGINE)
        .Height = (INT_CAPHEIGHT - INT_MARGINE - 2 * INT_BUTTONMARGINE)
        .Left = (iParW - INT_MARGINE - INT_BUTTONMARGINE) - .Width - INT_SHADDOWWIDTH
        .Top = (INT_MARGINE + INT_BUTTONMARGINE)
        
        FillColor = Parent.BackColor
        FillStyle = 0
        Line (.Left, .Top)-(.Left + .Width - 1, .Top + .Height - 1), FillColor, B
        
        iPen = CreatePen(0, 2, BackColor)
        SelectObject hdc, iPen
'        arrLine(0).X = .Left + 2
'        arrLine(0).Y = .Top + 2
'        arrLine(1).X = .Left + .Width - 3
'        arrLine(1).Y = .Top + .Height - 3
'        Polyline hdc, arrLine(0), 2
'        arrLine(0).X = .Left + .Width - 1 - 2
'        arrLine(0).Y = .Top + 2
'        arrLine(1).X = .Left + -1 + 3
'        arrLine(1).Y = .Top + .Height - 3
'        Polyline hdc, arrLine(0), 2
        arrLine(0).X = .Left + 3
        arrLine(0).Y = .Top + 3
        arrLine(1).X = .Left + .Width - 4
        arrLine(1).Y = .Top + .Height - 4
        Polyline hdc, arrLine(0), 2
        arrLine(0).X = .Left + .Width - 1 - 3
        arrLine(0).Y = .Top + 3
        arrLine(1).X = .Left + -1 + 4
        arrLine(1).Y = .Top + .Height - 4
        Polyline hdc, arrLine(0), 2
        DeleteObject iPen
        
    End With
    
    With lblCaption
        .Caption = Parent.Caption
        '.Left = (INT_CAPHEIGHT + INT_MARGINE)
        .Left = 0
        .Top = (INT_MARGINE + INT_BUTTONMARGINE)
        .Width = ScaleWidth
        .Height = ScaleHeight
    End With
    
    bInProc = False
End Sub

Public Sub ShapeForm()
Static bInProc As Boolean
Dim iPen As Long
Dim iBrush As Long
Dim arrBorder(6) As POINTAPI
Dim arrShaddow(4) As POINTAPI
On Error Resume Next

    If bInProc Then Exit Sub
    bInProc = True
    
    DrawCaption
    
    Parent.BorderStyle = 0
    Parent.ControlBox = False
    Parent.Cls
    
    arrCapPoly(0).X = iParW - INT_MARGINE - INT_SHADDOWWIDTH
    arrCapPoly(0).Y = INT_MARGINE
    arrCapPoly(1).X = iParW - INT_MARGINE - INT_SHADDOWWIDTH
    arrCapPoly(1).Y = INT_CAPHEIGHT
    arrCapPoly(2).X = INT_MARGINE + 1
    arrCapPoly(2).Y = INT_CAPHEIGHT
    arrCapPoly(3).X = INT_CAPHEIGHT + 1 '+ INT_MARGINE
    arrCapPoly(3).Y = INT_MARGINE
    
    If iCaptionRgnPtr <> 0 Then DeleteObject iCaptionRgnPtr
    iCaptionRgnPtr = CreatePolygonRgn(arrCapPoly(0), 4, 1)
    SetWindowRgn hwnd, iCaptionRgnPtr, True
    
    '=======================================================
    
    arrFormPoly(0).X = INT_CAPHEIGHT
    arrFormPoly(0).Y = 0
    arrFormPoly(1).X = iParW - INT_SHADDOWWIDTH
    arrFormPoly(1).Y = 0
    arrFormPoly(2).X = iParW - INT_SHADDOWWIDTH
    arrFormPoly(2).Y = INT_SHADDOWWIDTH
    arrFormPoly(3).X = iParW
    arrFormPoly(3).Y = INT_SHADDOWWIDTH
    arrFormPoly(4).X = iParW
    arrFormPoly(4).Y = iParH - INT_CAPHEIGHT
    arrFormPoly(5).X = iParW - INT_CAPHEIGHT
    arrFormPoly(5).Y = iParH
    arrFormPoly(6).X = INT_SHADDOWWIDTH
    arrFormPoly(6).Y = iParH
    arrFormPoly(7).X = INT_SHADDOWWIDTH
    arrFormPoly(7).Y = iParH - INT_SHADDOWWIDTH
    arrFormPoly(8).X = 0
    arrFormPoly(8).Y = iParH - INT_SHADDOWWIDTH
    arrFormPoly(9).X = 0
    arrFormPoly(9).Y = INT_CAPHEIGHT
    
    'If Ambient.UserMode Then
        If iFormRgnPtr <> 0 Then DeleteObject iFormRgnPtr
        iFormRgnPtr = CreatePolygonRgn(arrFormPoly(0), 10, 1)
        SetWindowRgn Parent.hwnd, iFormRgnPtr, True
    'End If
    
    
    
    ' drawing border
    arrBorder(0).X = arrFormPoly(0).X
    arrBorder(0).Y = arrFormPoly(0).Y
    arrBorder(1).X = arrFormPoly(1).X - 1
    arrBorder(1).Y = arrFormPoly(1).Y
    arrBorder(2).X = arrFormPoly(4).X - 1 - INT_SHADDOWWIDTH
    arrBorder(2).Y = arrFormPoly(4).Y - 1
    arrBorder(3).X = arrFormPoly(5).X - 1
    arrBorder(3).Y = arrFormPoly(5).Y - 1 - INT_SHADDOWWIDTH
    arrBorder(4).X = arrFormPoly(8).X
    arrBorder(4).Y = arrFormPoly(8).Y - 1
    arrBorder(5).X = arrFormPoly(9).X
    arrBorder(5).Y = arrFormPoly(9).Y
    arrBorder(6).X = arrBorder(0).X
    arrBorder(6).Y = arrBorder(0).Y
    
    iPen = CreatePen(0, 1, 0)
    SelectObject Parent.hdc, iPen
    Polyline Parent.hdc, arrBorder(0), 7
    DeleteObject iPen
    
'    iBrush = CreateSolidBrush(0)
'    FrameRgn Parent.hdc, CreatePolygonRgn(arrFormPoly(0), 10, 1), iBrush, 1, 1
'    DeleteObject iBrush
    
    
    ' drawing shaddow
    arrShaddow(0).X = arrFormPoly(3).X - 1
    arrShaddow(0).Y = arrFormPoly(3).Y
    arrShaddow(1).X = arrFormPoly(4).X - 1
    arrShaddow(1).Y = arrFormPoly(4).Y '- 1
    arrShaddow(2).X = arrFormPoly(5).X '- 1
    arrShaddow(2).Y = arrFormPoly(5).Y - 1
    arrShaddow(3).X = arrFormPoly(6).X - 1
    arrShaddow(3).Y = arrFormPoly(6).Y - 1
    
    iPen = CreatePen(0, 4, COLOR_SHADOW)
    SelectObject Parent.hdc, iPen
    Polyline Parent.hdc, arrShaddow(0), 4
    DeleteObject iPen
    
    DeleteObject iPen
    
    bInProc = False

End Sub

Private Sub UserControl_Paint()
On Error Resume Next
    lblCaption.Caption = Parent.Caption
End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
On Error Resume Next

    UserControl.BackColor = PropBag.ReadProperty("BackColor", COLOR_BACKCOLOR)
    Set lblCaption.Font = PropBag.ReadProperty("Font", Ambient.Font)
    lblCaption.ForeColor = PropBag.ReadProperty("ForeColor", COLOR_FORECOLOR)
End Sub

Private Sub UserControl_Resize()
On Error Resume Next
    ShapeForm
End Sub
'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,BackColor
Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
Attribute BackColor.VB_UserMemId = -501
On Error Resume Next
    BackColor = UserControl.BackColor
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
On Error Resume Next
    UserControl.BackColor() = New_BackColor
    DrawCaption
    PropertyChanged "BackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=lblCaption,lblCaption,-1,Font
Public Property Get Font() As Font
Attribute Font.VB_Description = "Returns a Font object."
Attribute Font.VB_UserMemId = -512
On Error Resume Next
    Set Font = lblCaption.Font
End Property

Public Property Set Font(ByVal New_Font As Font)
On Error Resume Next
    Set lblCaption.Font = New_Font
    PropertyChanged "Font"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=lblCaption,lblCaption,-1,ForeColor
Public Property Get ForeColor() As OLE_COLOR
Attribute ForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
Attribute ForeColor.VB_UserMemId = -513
On Error Resume Next
    ForeColor = lblCaption.ForeColor
End Property

Public Property Let ForeColor(ByVal New_ForeColor As OLE_COLOR)
On Error Resume Next
    lblCaption.ForeColor() = New_ForeColor
    PropertyChanged "ForeColor"
End Property

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
On Error Resume Next

    Call PropBag.WriteProperty("BackColor", UserControl.BackColor, COLOR_BACKCOLOR)
    Call PropBag.WriteProperty("Font", lblCaption.Font, Ambient.Font)
    Call PropBag.WriteProperty("ForeColor", lblCaption.ForeColor, COLOR_FORECOLOR)
End Sub

Public Sub Refresh()
Attribute Refresh.VB_UserMemId = -550
On Error Resume Next
    ShapeForm
End Sub

