VERSION 5.00
Begin VB.UserControl CoolBar 
   Alignable       =   -1  'True
   AutoRedraw      =   -1  'True
   ClientHeight    =   1725
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   540
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   FontTransparent =   0   'False
   ScaleHeight     =   1725
   ScaleWidth      =   540
   Begin VB.PictureBox picTop 
      Align           =   1  'Align Top
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "Arial Narrow"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   60
      Left            =   0
      Picture         =   "ctlCoolBar.ctx":0000
      ScaleHeight     =   60
      ScaleWidth      =   540
      TabIndex        =   1
      Top             =   0
      Width           =   540
   End
   Begin VB.PictureBox picBottom 
      Align           =   2  'Align Bottom
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "Arial Narrow"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   150
      Left            =   0
      Picture         =   "ctlCoolBar.ctx":0062
      ScaleHeight     =   150
      ScaleWidth      =   540
      TabIndex        =   0
      Top             =   1575
      Width           =   540
   End
End
Attribute VB_Name = "CoolBar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Private Const LF_FACESIZE = 32
Private Const LOGPIXELSY = 90
Private Const FC_GROUPBUTTON_W = 405
Private Const FC_BORDER_INDENT_X = 120
Private Const FC_BORDER_INDENT_Y = 120
Private Const TRANSPARENT = 1
Private Const OPAQUE = 2

Private Type LOGFONT
    lfHeight As Long
    lfWidth As Long
    lfEscapement As Long
    lfOrientation As Long
    lfWeight As Long
    lfItalic As Byte
    lfUnderLine As Byte
    lfStrikeOut As Byte
    lfCharSet As Byte
    lfOutPrecision As Byte
    lfClipPrecision As Byte
    lfQuality As Byte
    lsngPithAndFamily As Byte
    lfFaceName(LF_FACESIZE - 1) As Byte
End Type
Private Declare Function CreateFontIndirect Lib "gdi32" Alias "CreateFontIndirectA" (lpLogFont As LOGFONT) As Long

Private msCaption As String
Private mbEnabled As Boolean
Private mbActive As Boolean

Dim msngSX1 As Single
Dim msngSY1 As Single
Dim msngXratio As Single
Dim msngYratio As Single

Dim mbResize As Boolean

'Event Declarations:
Event KeyDown(KeyCode As Integer, Shift As Integer)
Event KeyPress(KeyAscii As Integer)
Event KeyUp(KeyCode As Integer, Shift As Integer)
Event Click()
Event DblClick()
Event MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
Event MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
Event MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)


'Default Property Values:
Const m_def_ActiveColor = vbRed
Const m_def_InactiveColor = vbBlue
'Property Variables:
Dim m_ActiveColor As OLE_COLOR
Dim m_InactiveColor As OLE_COLOR




Private Sub PaintBody()
Dim y1 As Single
On Error Resume Next
    
    With UserControl
        
        y1 = picTop.Height
        'H1 = .ScaleHeight - picBottom.Height - picTop.Height
    
        UserControl.Cls
        
        If mbActive Then
            UserControl.Line (0, y1)-(.ScaleWidth, .ScaleHeight - picBottom.Height), m_ActiveColor, BF
        Else
            UserControl.Line (0, y1)-(.ScaleWidth, .ScaleHeight - picBottom.Height), m_InactiveColor, BF
        End If
        
        UserControl.Line (0, y1)-(0, .ScaleHeight - picBottom.Height), &H636563
        UserControl.Line (15, y1)-(15, .ScaleHeight - picBottom.Height), &HFFFFFF
        UserControl.Line (30, y1)-(30, .ScaleHeight - picBottom.Height), &HBDBABD
        UserControl.Line (45, y1)-(45, .ScaleHeight - picBottom.Height), &H6B696B
        
        UserControl.Line (.ScaleWidth - 15, y1)-(.ScaleWidth - 15, .ScaleHeight - picBottom.Height), &H5A5D63
        UserControl.Line (.ScaleWidth - 30, y1)-(.ScaleWidth - 30, .ScaleHeight - picBottom.Height), &H636563
        UserControl.Line (.ScaleWidth - 45, y1)-(.ScaleWidth - 45, .ScaleHeight - picBottom.Height), &HFFFFFF
        UserControl.Line (.ScaleWidth - 60, y1)-(.ScaleWidth - 60, .ScaleHeight - picBottom.Height), &HBDBABD
        UserControl.Line (.ScaleWidth - 75, y1)-(.ScaleWidth - 75, .ScaleHeight - picBottom.Height), &H6B696B
        
    End With
    
End Sub

Private Sub SetDevice()
    Dim sngSX2 As Single
    Dim sngSY2 As Single
    Dim sngPX2 As Single
    Dim sngPY2 As Single
    Dim nScaleMode As Integer
    
    nScaleMode = UserControl.ScaleMode
    msngSX1 = UserControl.ScaleLeft
    msngSY1 = UserControl.ScaleTop
    sngSX2 = msngSX1 + UserControl.ScaleWidth
    sngSY2 = msngSY1 + UserControl.ScaleHeight
        
    UserControl.ScaleMode = vbPixels
    sngPX2 = UserControl.ScaleWidth
    sngPY2 = UserControl.ScaleHeight
        
    If nScaleMode = 0 Then
        UserControl.Scale (msngSX1, msngSY1)-(sngSX2, sngSY2)
    Else
        UserControl.ScaleMode = nScaleMode
    End If
    
    msngXratio = sngPX2 / (sngSX2 - msngSX1)
    msngYratio = sngPY2 / (sngSY2 - msngSY1)
    
End Sub


Private Function XtoP(sngX As Single) As Long
    XtoP = (sngX - msngSX1) * msngXratio
End Function

Private Function YtoP(sngY As Single) As Long
    YtoP = (sngY - msngSY1) * msngYratio
End Function



Private Sub DrawCaption(sText As String)
    On Error Resume Next
    
    Dim sFontName As String
    Dim Lf As LOGFONT
    Dim sngSX1 As Single
    Dim sngSY1 As Single
    Dim sngXRatio As Single
    Dim sngYRatio As Single
    Dim hFnt As Long
    Dim hFntPrev As Long
    Dim lRes As Long
    Dim bytBuf() As Byte
    Dim N As Integer
    Dim nBkMode As Integer
    Dim x As Long
    Dim Y As Long
    
    SetDevice
            
    sFontName = UserControl.Font.Name
    If sFontName = "" Then sFontName = "Arial"
        
    bytBuf = StrConv(sFontName & Chr$(0), vbFromUnicode)
    For N = 0 To UBound(bytBuf)
        Lf.lfFaceName(N) = bytBuf(N)
    Next
        
    Lf.lfHeight = UserControl.Font.Size * GetDeviceCaps(UserControl.hdc, LOGPIXELSY) \ 72
        
    Lf.lfWeight = UserControl.Font.Weight
        
    If UserControl.Font.Italic Then
        Lf.lfItalic = 1
    Else
        Lf.lfItalic = 0
    End If
        
    If UserControl.Font.Underline Then
        Lf.lfUnderLine = 1
    Else
        Lf.lfUnderLine = 0
    End If
        
    Lf.lfEscapement = 900
    Lf.lfOrientation = 900
        
    'x = XtoP((FC_GROUPBUTTON_W - UserControl.TextHeight(sText)) / 2)
    x = msngXratio * ((FC_GROUPBUTTON_W - UserControl.TextHeight(sText)) / 2)
    Y = msngYratio * (UserControl.TextWidth(sText) + FC_BORDER_INDENT_Y) * 0.98
        
    Call SetBkMode(UserControl.hdc, TRANSPARENT)
    hFnt = CreateFontIndirect(Lf)
    hFntPrev = SelectObject(UserControl.hdc, hFnt)
    lRes = TextOut(UserControl.hdc, x, Y, sText, Len(sText))
    hFnt = SelectObject(UserControl.hdc, hFntPrev)
    DeleteObject hFnt
    Call SetBkMode(UserControl.hdc, OPAQUE)
    
End Sub


Public Sub Repaint()
    PaintBody
    DrawCaption msCaption
End Sub


Public Property Get Caption() As String
    Caption = msCaption
End Property

Public Property Let Caption(ByVal New_Caption As String)
    On Error Resume Next
    msCaption = New_Caption
    Repaint
    PropertyChanged "Caption"
End Property

Public Property Get Enabled() As Boolean
    Enabled = mbEnabled
End Property

Public Property Let Enabled(ByVal New_Value As Boolean)
    mbEnabled = New_Value
    PropertyChanged "Enabled"
End Property


Public Property Get Active() As Boolean
    Active = mbActive
End Property

Public Property Let Active(ByVal New_Value As Boolean)
    mbActive = New_Value
    Repaint
    PropertyChanged "Active"
End Property

Public Property Get Font() As Font
    Set Font = UserControl.Font
End Property

Public Property Set Font(ByVal New_Font As Font)
    On Error Resume Next
    Set UserControl.Font = New_Font
    Repaint
    PropertyChanged "Font"
End Property


Private Sub UserControl_Initialize()
'
End Sub

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
    On Error Resume Next
    Set UserControl.Font = Ambient.Font
    UserControl.Font.Name = "Arial"
    picTop.BackColor = UserControl.BackColor
    picBottom.BackColor = UserControl.BackColor

    m_ActiveColor = m_def_ActiveColor
    m_InactiveColor = m_def_InactiveColor
End Sub

Private Sub UserControl_KeyDown(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub UserControl_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub UserControl_KeyUp(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub


Private Sub UserControl_Click()
    If mbEnabled Then
        RaiseEvent Click
    End If
End Sub

Private Sub UserControl_DblClick()
    If mbEnabled Then
        RaiseEvent DblClick
    End If
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
    RaiseEvent MouseDown(Button, Shift, x, Y)
End Sub

Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
    RaiseEvent MouseMove(Button, Shift, x, Y)
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
    RaiseEvent MouseUp(Button, Shift, x, Y)
End Sub


'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
    On Error Resume Next
    Set UserControl.Font = PropBag.ReadProperty("Font", Ambient.Font)
    UserControl.BackColor = PropBag.ReadProperty("BackColor", &H8000000F)
    msCaption = PropBag.ReadProperty("Caption", "")
    mbEnabled = PropBag.ReadProperty("Enabled", True)
    Active = PropBag.ReadProperty("Active", True)
    UserControl.ScaleWidth = FC_GROUPBUTTON_W
    picTop.BackColor = UserControl.BackColor
    picBottom.BackColor = UserControl.BackColor
    m_ActiveColor = PropBag.ReadProperty("ActiveColor", m_def_ActiveColor)
    m_InactiveColor = PropBag.ReadProperty("InactiveColor", m_def_InactiveColor)
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If Not mbResize Then
        mbResize = True
        If UserControl.Width <> FC_GROUPBUTTON_W Then
            UserControl.Width = FC_GROUPBUTTON_W
        End If
        Repaint
        mbResize = False
    End If
End Sub



Private Sub UserControl_Terminate()
'
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
    Call PropBag.WriteProperty("Font", UserControl.Font, Ambient.Font)
    Call PropBag.WriteProperty("BackColor", UserControl.BackColor, &H8000000F)
    Call PropBag.WriteProperty("Caption", msCaption, "")
    Call PropBag.WriteProperty("Enabled", mbEnabled, True)
    Call PropBag.WriteProperty("Active", mbActive, True)
    Call PropBag.WriteProperty("ActiveColor", m_ActiveColor, m_def_ActiveColor)
    Call PropBag.WriteProperty("InactiveColor", m_InactiveColor, m_def_InactiveColor)
End Sub

Public Property Get BackColor() As OLE_COLOR
    BackColor = UserControl.BackColor
End Property

Public Property Let BackColor(NewValue As OLE_COLOR)
    UserControl.BackColor = NewValue
    picTop.BackColor = NewValue
    picBottom.BackColor = NewValue
    Repaint
    PropertyChanged "BackColor"
End Property

Public Property Get MousePointer() As MousePointerConstants
    MousePointer = UserControl.MousePointer
End Property

Public Property Let MousePointer(NewValue As MousePointerConstants)
    UserControl.MousePointer = NewValue
End Property

Public Property Get MouseIcon() As Picture
    Set MouseIcon = UserControl.MouseIcon
End Property

Public Property Set MouseIcon(ByVal New_MouseIcon As Picture)
    Set UserControl.MouseIcon = New_MouseIcon
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ActiveColor() As OLE_COLOR
    ActiveColor = m_ActiveColor
End Property

Public Property Let ActiveColor(ByVal New_ActiveColor As OLE_COLOR)
    m_ActiveColor = New_ActiveColor
    PropertyChanged "ActiveColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get InactiveColor() As OLE_COLOR
    InactiveColor = m_InactiveColor
End Property

Public Property Let InactiveColor(ByVal New_InactiveColor As OLE_COLOR)
    m_InactiveColor = New_InactiveColor
    PropertyChanged "InactiveColor"
End Property

