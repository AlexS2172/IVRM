VERSION 5.00
Begin VB.UserControl ctlButtonEx 
   ClientHeight    =   480
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   1605
   DefaultCancel   =   -1  'True
   ScaleHeight     =   480
   ScaleWidth      =   1605
   Begin VB.Image imgPicture 
      Height          =   120
      Index           =   3
      Left            =   0
      Top             =   0
      Width           =   150
   End
   Begin VB.Image imgPicture 
      Height          =   120
      Index           =   2
      Left            =   0
      Top             =   0
      Width           =   150
   End
   Begin VB.Image imgPicture 
      Height          =   120
      Index           =   1
      Left            =   0
      Top             =   0
      Width           =   150
   End
   Begin VB.Image imgPicture 
      Height          =   120
      Index           =   0
      Left            =   0
      Top             =   0
      Width           =   150
   End
End
Attribute VB_Name = "ctlButtonEx"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

' API declares
Private Type POINTAPI
    X As Long
    Y As Long
End Type

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Declare Function GetCursorPos& Lib "user32" (lpPoint As POINTAPI)
Private Declare Function WindowFromPoint& Lib "user32" (ByVal lpPointX As Long, ByVal lpPointY As Long)
'Private Declare Function DrawFocusRect Lib "user32" (ByVal hdc As Long, lpRect As RECT) As Long

Private Declare Function SetCapture Lib "user32" (ByVal hwnd As Long) As Long
Private Declare Function GetCapture Lib "user32" () As Long
Private Declare Function ReleaseCapture Lib "user32" () As Long
Private Declare Function GetFocus Lib "user32" () As Long

Private Enum ButtonStateEnum
    STATE_FIRST = 0
    STATE_NORMAL = 0
    STATE_PRESSED = 1
    STATE_HOVER = 2
    STATE_DISABLED = 3
    STATE_LAST = 3
End Enum

Public Enum HorizontalAlignmentEnum
    HA_LEFT = 0
    HA_CENTER = 1
    HA_RIGHT = 2
End Enum

Public Enum VerticalAlignmentEnum
    VA_TOP = 0
    VA_CENTER = 1
    VA_BOTTOM = 2
End Enum

Public Enum ShadowEnum
    SHADOW_NONE = 0
    SHADOW_SINGLE = 1
    SHADOW_DOUBLE = 2
End Enum

Private m_bInitializing As Boolean
Private m_nHorizCaptionShift As Long
Private m_nVertCaptionShift As Long
Private m_nHorizPictureShift As Long
Private m_nVertPictureShift As Long
Private m_enState As ButtonStateEnum
Private m_bLockPaint As Boolean
Private m_sCapPart1 As String
Private m_sCapPart2 As String
Private m_sCapPart3 As String

'Event Declarations:
Event Click() 'MappingInfo=UserControl,UserControl,-1,Click
Event DblClick() 'MappingInfo=UserControl,UserControl,-1,DblClick
Event KeyDown(KeyCode As Integer, Shift As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyDown
Event KeyPress(KeyAscii As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyPress
Event KeyUp(KeyCode As Integer, Shift As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyUp
Event MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseDown
Event MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseMove
Event MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseUp
'Default Property Values:
Const m_def_CanGetFocus = True
Const m_def_CaptionHorizAlign = HA_CENTER
Const m_def_CaptionVertAlign = VA_CENTER
Const m_def_PictureHorizAlign = HA_LEFT
Const m_def_PictureVertAlign = VA_CENTER
Const m_def_Shadow = SHADOW_DOUBLE
Const m_def_Caption = ""
'Property Variables:
Private m_CanGetFocus As Boolean
Private m_CaptionHorizAlign As HorizontalAlignmentEnum
Private m_CaptionVertAlign As VerticalAlignmentEnum
Private m_PictureHorizAlign As HorizontalAlignmentEnum
Private m_PictureVertAlign As VerticalAlignmentEnum
Private m_Shadow As ShadowEnum
Private m_Caption As String

Private m_BtnFaceColor(STATE_FIRST To STATE_LAST) As OLE_COLOR
Private m_BtnHiLightColor(STATE_FIRST To STATE_LAST) As OLE_COLOR
Private m_BtnInnerShadowColor(STATE_FIRST To STATE_LAST) As OLE_COLOR
Private m_BtnDkShadowColor(STATE_FIRST To STATE_LAST) As OLE_COLOR
Private m_BtnShadowColor(STATE_FIRST To STATE_LAST) As OLE_COLOR
Private m_BtnFrameColor(STATE_FIRST To STATE_LAST) As OLE_COLOR
Private m_BtnTextColor(STATE_FIRST To STATE_LAST) As OLE_COLOR

Private m_BackColor As OLE_COLOR

Private Sub UserControl_AccessKeyPress(KeyAscii As Integer)
    If m_CanGetFocus And Enabled Then
        RaiseEvent Click
    End If
End Sub

Private Sub UserControl_AmbientChanged(PropertyName As String)
    On Error Resume Next
    If PropertyName = "DisplayAsDefault" Then
        UserControl.FontBold = Extender.Default
        'Refresh
    End If
End Sub

Private Sub UserControl_EnterFocus()
    On Error Resume Next
    UserControl.Refresh
End Sub

Private Sub UserControl_ExitFocus()
    On Error Resume Next
    UserControl.Refresh
End Sub

Private Sub UserControl_Initialize()
    On Error Resume Next
    Dim i As ButtonStateEnum
    
    m_bLockPaint = False
    m_bInitializing = True
    m_enState = STATE_NORMAL
    m_CaptionHorizAlign = m_def_CaptionHorizAlign
    m_CaptionVertAlign = m_def_CaptionVertAlign
    m_PictureHorizAlign = m_def_PictureHorizAlign
    m_PictureVertAlign = m_def_PictureVertAlign
    m_Shadow = m_def_Shadow
    m_nHorizCaptionShift = 5
    m_nVertCaptionShift = 0
    m_nHorizPictureShift = 5
    m_nVertPictureShift = 2
    
    m_BackColor = &H8000000F
    
    i = STATE_NORMAL
    m_BtnFaceColor(i) = RGB(51, 51, 51)
    m_BtnHiLightColor(i) = RGB(152, 152, 152)
    m_BtnInnerShadowColor(i) = RGB(24, 24, 24)
    m_BtnShadowColor(i) = RGB(156, 156, 156)
    m_BtnDkShadowColor(i) = RGB(115, 115, 115)
    m_BtnFrameColor(i) = RGB(0, 0, 0)
    m_BtnTextColor(i) = RGB(255, 255, 255)
    imgPicture(i).Visible = False
    
    i = STATE_HOVER
    m_BtnFaceColor(i) = RGB(49, 99, 0)
    m_BtnHiLightColor(i) = RGB(102, 153, 102)
    m_BtnInnerShadowColor(i) = RGB(24, 24, 24)
    m_BtnShadowColor(i) = RGB(156, 156, 156)
    m_BtnDkShadowColor(i) = RGB(115, 115, 115)
    m_BtnFrameColor(i) = RGB(0, 0, 0)
    m_BtnTextColor(i) = RGB(255, 255, 255)
    imgPicture(i).Visible = False
    
    i = STATE_PRESSED
    m_BtnFaceColor(i) = RGB(0, 128, 0)
    m_BtnHiLightColor(i) = RGB(24, 24, 24)
    m_BtnInnerShadowColor(i) = RGB(102, 153, 102)
    m_BtnShadowColor(i) = RGB(156, 156, 156)
    m_BtnDkShadowColor(i) = RGB(115, 115, 115)
    m_BtnFrameColor(i) = RGB(0, 0, 0)
    m_BtnTextColor(i) = RGB(255, 255, 255)
    imgPicture(i).Visible = False
    
    i = STATE_DISABLED
    m_BtnFaceColor(i) = RGB(122, 122, 122)
    m_BtnHiLightColor(i) = RGB(188, 188, 188)
    m_BtnInnerShadowColor(i) = RGB(105, 105, 105)
    m_BtnShadowColor(i) = RGB(191, 191, 191)
    m_BtnDkShadowColor(i) = RGB(164, 164, 164)
    m_BtnFrameColor(i) = RGB(89, 89, 89)
    m_BtnTextColor(i) = RGB(204, 204, 204)
    imgPicture(i).Visible = False
    
    AdjustPictureVisibility
    
    m_bInitializing = False
End Sub

Private Sub UserControl_Paint()
    On Error Resume Next
    
    'If m_bLockPaint Then Exit Sub
    
    Dim XPixel As Long, YPixel As Long
    Dim W As Long, H As Long
    Dim sTmp1 As String, sTmp2 As String
    Dim tmpFontUnderline As Long
    
    'Debug.Print "Refresh " & Time
    
    XPixel = ScaleX(1, vbPixels, vbTwips)
    YPixel = ScaleY(1, vbPixels, vbTwips)
    
    'Line (0, 0)-(ScaleWidth - XPixel, ScaleHeight - YPixel), m_BackColor, BF
    
    If m_Shadow = SHADOW_DOUBLE Then
        W = ScaleWidth - 3 * XPixel
        H = ScaleHeight - 3 * YPixel
    ElseIf m_Shadow = SHADOW_SINGLE Then
        W = ScaleWidth - 2 * XPixel
        H = ScaleHeight - 2 * YPixel
    Else
        W = ScaleWidth - XPixel
        H = ScaleHeight - YPixel
    End If
    Line (0, 0)-(2 * XPixel, ScaleHeight - YPixel), m_BackColor, BF
    Line (W - 2 * XPixel, 0)-(ScaleWidth - XPixel, ScaleHeight - YPixel), m_BackColor, BF
    
    Line (2 * XPixel, 2 * YPixel)-(W - 2 * XPixel, H - 2 * YPixel), m_BtnFaceColor(m_enState), BF
    
    Line (3 * XPixel, 0)-(W - 3 * XPixel, 0), m_BtnFrameColor(m_enState)
    Line -(W, 3 * YPixel), m_BtnFrameColor(m_enState)
    Line -(W, H - 3 * YPixel), m_BtnFrameColor(m_enState)
    Line -(W - 3 * XPixel, H), m_BtnFrameColor(m_enState)
    Line -(3 * XPixel, H), m_BtnFrameColor(m_enState)
    Line -(0, H - 3 * YPixel), m_BtnFrameColor(m_enState)
    Line -(0, 3 * YPixel), m_BtnFrameColor(m_enState)
    Line -(3 * XPixel, 0), m_BtnFrameColor(m_enState)

    Line (W - 3 * XPixel, YPixel)-(3 * XPixel, YPixel), m_BtnHiLightColor(m_enState)
    Line -(XPixel, 3 * YPixel), m_BtnHiLightColor(m_enState)
    Line -(XPixel, H - 3 * YPixel), m_BtnHiLightColor(m_enState)
    Line -(4 * XPixel, H), m_BtnHiLightColor(m_enState)
    
    Line (4 * XPixel, H - YPixel)-(W - 3 * XPixel, H - YPixel), m_BtnInnerShadowColor(m_enState)
    Line -(W - XPixel, H - 3 * YPixel), m_BtnInnerShadowColor(m_enState)
    Line -(W - XPixel, 3 * YPixel), m_BtnInnerShadowColor(m_enState)
    Line -(W - 3 * XPixel, YPixel), m_BtnInnerShadowColor(m_enState)
    Line (W - 3 * XPixel, H - 2 * YPixel)-(W, H - 4 * YPixel), m_BtnInnerShadowColor(m_enState)
    
    If GetFocus = UserControl.hwnd And m_enState <> STATE_DISABLED Then
        DrawMode = vbXorPen
        'DrawStyle = vbDot
        'ForeColor = &HFFFFFF
        Line (3 * XPixel, 3 * YPixel)-(W - 3 * XPixel, H - 3 * YPixel), &H808080, B
        'DrawStyle = vbSolid
        DrawMode = vbCopyPen
        'Dim rc As RECT
        'rc.Left = 3
        'rc.Top = 3
        'rc.Right = ScaleX(W, vbTwips, vbPixels) - 2
        'rc.Bottom = ScaleY(H, vbTwips, vbPixels) - 2
        'DrawFocusRect UserControl.hdc, rc
    End If

    If m_enState <> STATE_PRESSED And (m_Shadow = SHADOW_DOUBLE Or m_Shadow = SHADOW_SINGLE) Then
        If m_Shadow = SHADOW_DOUBLE Then
            W = ScaleWidth - 2 * XPixel
            H = ScaleHeight - 2 * YPixel
        Else
            W = ScaleWidth - XPixel
            H = ScaleHeight - YPixel
        End If
        
        Line (5 * XPixel, H)-(W - 4 * XPixel, H), m_BtnDkShadowColor(m_enState)
        Line -(W, H - 4 * YPixel), m_BtnDkShadowColor(m_enState)
        Line -(W, 4 * YPixel), m_BtnDkShadowColor(m_enState)
        Line (W - 3 * XPixel, H)-(W + XPixel, H - 4 * YPixel), m_BtnDkShadowColor(m_enState)
        
        If m_Shadow = SHADOW_DOUBLE Then
            W = ScaleWidth - XPixel
            H = ScaleHeight - YPixel
            
            Line (6 * XPixel, H)-(W - 4 * XPixel, H), m_BtnShadowColor(m_enState)
            Line -(W, H - 4 * YPixel), m_BtnShadowColor(m_enState)
            Line -(W, 5 * YPixel), m_BtnShadowColor(m_enState)
        End If
    End If
    
    If m_Shadow = SHADOW_DOUBLE Then
        W = ScaleWidth - 3 * XPixel
        H = ScaleHeight - 3 * YPixel
    ElseIf m_Shadow = SHADOW_SINGLE Then
        W = ScaleWidth - 2 * XPixel
        H = ScaleHeight - 2 * YPixel
    Else
        W = ScaleWidth - XPixel
        H = ScaleHeight - YPixel
    End If
    
    sTmp1 = m_sCapPart1 & m_sCapPart2 & m_sCapPart3
    ' y position
    'CurrentY = (H - TextHeight(m_Caption)) / 2
    Select Case m_CaptionVertAlign
        Case VA_TOP
            CurrentY = m_nVertCaptionShift * YPixel
        Case VA_BOTTOM
            CurrentY = H - TextHeight(sTmp1) - m_nVertCaptionShift * YPixel
        Case Else
            CurrentY = (H - TextHeight(sTmp1) + m_nVertCaptionShift * YPixel) / 2
    End Select

    ' x position
    Select Case m_CaptionHorizAlign
        Case HA_LEFT
            CurrentX = m_nHorizCaptionShift * XPixel
        Case HA_RIGHT
            CurrentX = W - TextWidth(sTmp1) - m_nHorizCaptionShift * XPixel
        Case Else
            'CurrentX = (W - TextWidth(sTmp1) + m_nHorizCaptionShift * XPixel) / 2
            CurrentX = (W - TextWidth(sTmp1)) / 2
    End Select

    If m_enState = STATE_PRESSED Then
        CurrentX = CurrentX + XPixel
        CurrentY = CurrentY + YPixel
    End If
    
    ' cutting long caption
    'sTmp1 = sCap1 & sCap2 & sCap3
    sTmp2 = sTmp1
    Do While ((CurrentX + TextWidth(sTmp2)) > W - m_nHorizCaptionShift * XPixel + XPixel) And (Len(sTmp1) > 0)
        sTmp1 = Left$(sTmp1, Len(sTmp1) - 1)
        sTmp2 = sTmp1 & "..."
    Loop
    
    ' printinc a caption
    UserControl.ForeColor = m_BtnTextColor(m_enState)
    Print Left(sTmp2, Len(m_sCapPart1));
    tmpFontUnderline = Font.Underline
    UserControl.Font.Underline = True
    Print Mid(sTmp2, Len(m_sCapPart1) + 1, Len(m_sCapPart2));
    UserControl.Font.Underline = tmpFontUnderline
    Print Mid(sTmp2, Len(m_sCapPart1) + Len(m_sCapPart2) + 1, Len(m_sCapPart3));
    ' setting access key
    'UserControl.AccessKeys = sCap2
End Sub

Private Sub UserControl_Resize()
    AdjustPicturePos
    Refresh
End Sub
'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,BackColor
Public Property Get BackColor() As OLE_COLOR
    BackColor = m_BackColor
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
    m_BackColor = New_BackColor
    PropertyChanged "BackColor"
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Enabled
Public Property Get Enabled() As Boolean
    Enabled = UserControl.Enabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
    If UserControl.Enabled <> New_Enabled Then
        UserControl.Enabled = New_Enabled
        PropertyChanged "Enabled"
        m_enState = IIf(New_Enabled, STATE_NORMAL, STATE_DISABLED)
        If Not AdjustPictureVisibility Then Refresh
    End If
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Font
Public Property Get Font() As Font
    Set Font = UserControl.Font
End Property

Public Property Set Font(ByVal New_Font As Font)
    Set UserControl.Font = New_Font
    PropertyChanged "Font"
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,0
Public Property Get CanGetFocus() As Boolean
    CanGetFocus = m_CanGetFocus
End Property

Public Property Let CanGetFocus(ByVal New_CanGetFocus As Boolean)
    If m_CanGetFocus = New_CanGetFocus Then Exit Property
    m_CanGetFocus = New_CanGetFocus
    Refresh
    PropertyChanged "CanGetFocus"
End Property


'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Refresh
Public Sub Refresh()
    If Not m_bInitializing Then UserControl_Paint ' UserControl.Refresh
End Sub

Private Sub UserControl_KeyDown(KeyCode As Integer, Shift As Integer)
    If Not m_CanGetFocus Then
        KeyCode = 0
        Exit Sub
    End If
    If m_enState <> STATE_DISABLED Then
        RaiseEvent KeyDown(KeyCode, Shift)
        If KeyCode = vbKeySpace Then
            m_enState = STATE_PRESSED
            If Not AdjustPictureVisibility Then Refresh 'Refresh
            'UserControl.Refresh
            'imgPicture(m_enState).Refresh
'            RaiseEvent Click
        End If
    End If
End Sub

Private Sub UserControl_KeyPress(KeyAscii As Integer)
    If m_enState <> STATE_DISABLED Then
        RaiseEvent KeyPress(KeyAscii)
    End If
End Sub

Private Sub UserControl_KeyUp(KeyCode As Integer, Shift As Integer)
    If Not m_CanGetFocus Then
        KeyCode = 0
        Exit Sub
    End If
    If m_enState <> STATE_DISABLED Then
        RaiseEvent KeyUp(KeyCode, Shift)
        If KeyCode = vbKeySpace Then
            m_enState = STATE_NORMAL
            If Not AdjustPictureVisibility Then Refresh
            'imgPicture(m_enState).Refresh
            RaiseEvent Click
        End If
    End If
'    If m_enState <> STATE_DISABLED Then
'        RaiseEvent KeyUp(KeyCode, Shift)
'    End If
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_enState <> STATE_DISABLED Then
        m_enState = STATE_PRESSED
        If Not AdjustPictureVisibility Then Refresh
        RaiseEvent MouseDown(Button, Shift, X, Y)
    End If
End Sub

Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_enState <> STATE_DISABLED Then
        Dim enState As ButtonStateEnum
        Dim curPos As POINTAPI
         
        GetCursorPos curPos
         
        If WindowFromPoint(curPos.X, curPos.Y) <> hwnd Then
            enState = STATE_NORMAL
            If GetCapture = UserControl.hwnd Then ReleaseCapture
        Else
            If Button = vbLeftButton Then
                enState = STATE_PRESSED
            Else
                enState = STATE_HOVER
            End If
            If GetCapture <> UserControl.hwnd Then SetCapture hwnd
        End If
             
        If m_enState <> enState Then
            m_enState = enState
            If Not AdjustPictureVisibility Then Refresh
        End If
        
        RaiseEvent MouseMove(Button, Shift, X, Y)
    End If
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    
    If m_enState <> STATE_DISABLED Then
        Dim enState As ButtonStateEnum
        Dim enStateOld As ButtonStateEnum
        
        'Dim curPos As POINTAPI
         
        'GetCursorPos curPos
        enStateOld = m_enState
        
        'If WindowFromPoint(curPos.X, curPos.Y) <> hWnd Then
            enState = STATE_NORMAL
            'If GetCapture = UserControl.hWnd Then ReleaseCapture
        'Else
        '    enState = STATE_HOVER
            'If GetCapture <> UserControl.hWnd Then SetCapture hWnd
        'End If
        
        ReleaseCapture
        
        If enState <> m_enState Then
            m_enState = enState
            If Not AdjustPictureVisibility Then Refresh
        End If
        
        If enStateOld = STATE_PRESSED Then
            'imgPicture(m_enState).Refresh
            'Refresh
            'DoEvents
            RaiseEvent MouseUp(Button, Shift, X, Y)
            If (Button = vbLeftButton) Then RaiseEvent Click
        End If
    End If
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,MouseIcon
Public Property Get MouseIcon() As Picture
    Set MouseIcon = UserControl.MouseIcon
End Property

Public Property Set MouseIcon(ByVal New_MouseIcon As Picture)
    Set UserControl.MouseIcon = New_MouseIcon
    PropertyChanged "MouseIcon"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,MousePointer
Public Property Get MousePointer() As MousePointerConstants
    MousePointer = UserControl.MousePointer
End Property

Public Property Let MousePointer(ByVal New_MousePointer As MousePointerConstants)
    UserControl.MousePointer() = New_MousePointer
    PropertyChanged "MousePointer"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Picture
Public Property Get PictureNormal() As Picture
    'Set Picture = UserControl.Picture
    Set PictureNormal = imgPicture(STATE_NORMAL).Picture
End Property

Public Property Set PictureNormal(ByVal New_Picture As Picture)
    'Set UserControl.Picture = New_Picture
    Set imgPicture(STATE_NORMAL).Picture = New_Picture
    PropertyChanged "PictureNormal"
    If Not New_Picture Is Nothing Then
        imgPicture(STATE_NORMAL).Width = ScaleX(New_Picture.Width, vbHimetric, vbTwips)
        imgPicture(STATE_NORMAL).Height = ScaleY(New_Picture.Height, vbHimetric, vbTwips)
    End If
    AdjustPicturePos
    AdjustPictureVisibility
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Picture
Public Property Get PicturePressed() As Picture
    'Set Picture = UserControl.Picture
    Set PicturePressed = imgPicture(STATE_PRESSED).Picture
End Property

Public Property Set PicturePressed(ByVal New_Picture As Picture)
    'Set UserControl.Picture = New_Picture
    Set imgPicture(STATE_PRESSED).Picture = New_Picture
    PropertyChanged "PicturePressed"
    If Not New_Picture Is Nothing Then
        imgPicture(STATE_PRESSED).Width = ScaleX(New_Picture.Width, vbHimetric, vbTwips)
        imgPicture(STATE_PRESSED).Height = ScaleY(New_Picture.Height, vbHimetric, vbTwips)
    End If
    AdjustPicturePos
    AdjustPictureVisibility
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Picture
Public Property Get PictureHover() As Picture
    'Set Picture = UserControl.Picture
    Set PictureHover = imgPicture(STATE_HOVER).Picture
End Property

Public Property Set PictureHover(ByVal New_Picture As Picture)
    'Set UserControl.Picture = New_Picture
    Set imgPicture(STATE_HOVER).Picture = New_Picture
    PropertyChanged "PicturePressed"
    If Not New_Picture Is Nothing Then
        imgPicture(STATE_HOVER).Width = ScaleX(New_Picture.Width, vbHimetric, vbTwips)
        imgPicture(STATE_HOVER).Height = ScaleY(New_Picture.Height, vbHimetric, vbTwips)
    End If
    AdjustPicturePos
    AdjustPictureVisibility
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Picture
Public Property Get PictureDisabled() As Picture
    'Set Picture = UserControl.Picture
    Set PictureDisabled = imgPicture(STATE_DISABLED).Picture
End Property

Public Property Set PictureDisabled(ByVal New_Picture As Picture)
    'Set UserControl.Picture = New_Picture
    Set imgPicture(STATE_DISABLED).Picture = New_Picture
    PropertyChanged "PictureDisabled"
    If Not New_Picture Is Nothing Then
        imgPicture(STATE_DISABLED).Width = ScaleX(New_Picture.Width, vbHimetric, vbTwips)
        imgPicture(STATE_DISABLED).Height = ScaleY(New_Picture.Height, vbHimetric, vbTwips)
    End If
    AdjustPicturePos
    AdjustPictureVisibility
    Refresh
End Property

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
    Set UserControl.Font = Ambient.Font
    Caption = UserControl.Name
    m_enState = IIf(UserControl.Enabled, STATE_NORMAL, STATE_DISABLED)
    m_CanGetFocus = m_def_CanGetFocus
End Sub

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
    m_bInitializing = True
    
    m_BackColor = PropBag.ReadProperty("BackColor", &H8000000F)
    UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
    m_enState = IIf(UserControl.Enabled, STATE_NORMAL, STATE_DISABLED)
    Set UserControl.Font = PropBag.ReadProperty("Font", Ambient.Font)
    Set MouseIcon = PropBag.ReadProperty("MouseIcon", Nothing)
    UserControl.MousePointer = PropBag.ReadProperty("MousePointer", 0)
    Set imgPicture(STATE_NORMAL).Picture = PropBag.ReadProperty("PictureNormal", Nothing)
    Set imgPicture(STATE_PRESSED).Picture = PropBag.ReadProperty("PicturePressed", Nothing)
    Set imgPicture(STATE_HOVER).Picture = PropBag.ReadProperty("PictureHover", Nothing)
    Set imgPicture(STATE_DISABLED).Picture = PropBag.ReadProperty("PictureDisabled", Nothing)
    Caption = PropBag.ReadProperty("Caption", m_def_Caption)
    m_Shadow = PropBag.ReadProperty("Shadow", m_def_Shadow)
    m_CaptionHorizAlign = PropBag.ReadProperty("CaptionHorizAlign", m_def_CaptionHorizAlign)
    m_CaptionVertAlign = PropBag.ReadProperty("CaptionVertAlign", m_def_CaptionVertAlign)
    m_PictureHorizAlign = PropBag.ReadProperty("PictureHorizAlign", m_def_PictureHorizAlign)
    m_PictureVertAlign = PropBag.ReadProperty("PictureVertAlign", m_def_PictureVertAlign)
    m_CanGetFocus = PropBag.ReadProperty("CanGetFocus", m_def_CanGetFocus)
    
    AdjustPicturePos
    AdjustPictureVisibility
    m_bInitializing = False
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
    On Error Resume Next
    PropBag.WriteProperty "BackColor", m_BackColor, &H8000000F
    PropBag.WriteProperty "Enabled", UserControl.Enabled, True
    PropBag.WriteProperty "Font", UserControl.Font, Ambient.Font
    PropBag.WriteProperty "MouseIcon", MouseIcon, Nothing
    PropBag.WriteProperty "MousePointer", UserControl.MousePointer, 0
    PropBag.WriteProperty "PictureNormal", imgPicture(STATE_NORMAL), Nothing
    PropBag.WriteProperty "PicturePressed", imgPicture(STATE_PRESSED), Nothing
    PropBag.WriteProperty "PictureHover", imgPicture(STATE_HOVER), Nothing
    PropBag.WriteProperty "PictureDisabled", imgPicture(STATE_DISABLED), Nothing
    PropBag.WriteProperty "Caption", m_Caption, m_def_Caption
    PropBag.WriteProperty "Shadow", m_Shadow, m_def_Shadow
    PropBag.WriteProperty "CaptionHorizAlign", m_CaptionHorizAlign, m_def_CaptionHorizAlign
    PropBag.WriteProperty "CaptionVertAlign", m_CaptionVertAlign, m_def_CaptionVertAlign
    PropBag.WriteProperty "PictureHorizAlign", m_PictureHorizAlign, m_def_PictureHorizAlign
    PropBag.WriteProperty "PictureVertAlign", m_PictureVertAlign, m_def_PictureVertAlign
    PropBag.WriteProperty "CanGetFocus", m_CanGetFocus, m_def_CanGetFocus
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,0
Public Property Get Caption() As String
    Caption = m_Caption
End Property

Public Property Let Caption(ByVal New_Caption As String)
    m_Caption = New_Caption
    
    Dim nPos As Long
    m_sCapPart1 = m_Caption
    m_sCapPart2 = ""
    m_sCapPart3 = ""
    nPos = InStr(1, m_Caption, "&")
    If nPos > 0 Then
        If InStr(nPos + 1, m_Caption, "&") <> (nPos + 1) Then
            m_sCapPart1 = Left$(m_Caption, nPos - 1)
            m_sCapPart2 = Mid$(m_Caption, nPos + 1, 1)
            m_sCapPart3 = Mid$(m_Caption, nPos + 2)
        End If
    End If
    
    UserControl.AccessKeys = m_sCapPart2

    PropertyChanged "Caption"
    Refresh
End Property




'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,2
Public Property Get Shadow() As ShadowEnum
    Shadow = m_Shadow
End Property

Public Property Let Shadow(ByVal New_Shadow As ShadowEnum)
    m_Shadow = New_Shadow
    PropertyChanged "Shadow"
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get CaptionHorizAlign() As HorizontalAlignmentEnum
    CaptionHorizAlign = m_CaptionHorizAlign
End Property

Public Property Let CaptionHorizAlign(ByVal New_CaptionHorizAlign As HorizontalAlignmentEnum)
    m_CaptionHorizAlign = New_CaptionHorizAlign
    PropertyChanged "CaptionHorizAlign"
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get CaptionVertAlign() As Long
    CaptionVertAlign = m_CaptionVertAlign
End Property

Public Property Let CaptionVertAlign(ByVal New_CaptionVertAlign As VerticalAlignmentEnum)
    m_CaptionVertAlign = New_CaptionVertAlign
    PropertyChanged "CaptionVertAlign"
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get PictureHorizAlign() As HorizontalAlignmentEnum
    PictureHorizAlign = m_PictureHorizAlign
End Property

Public Property Let PictureHorizAlign(ByVal New_PictureHorizAlign As HorizontalAlignmentEnum)
    m_PictureHorizAlign = New_PictureHorizAlign
    PropertyChanged "PictureHorizAlign"
    AdjustPicturePos
    Refresh
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get PictureVertAlign() As VerticalAlignmentEnum
    PictureVertAlign = m_PictureVertAlign
End Property

Public Property Let PictureVertAlign(ByVal New_PictureVertAlign As VerticalAlignmentEnum)
    m_PictureVertAlign = New_PictureVertAlign
    PropertyChanged "PictureVertAlign"
    AdjustPicturePos
    Refresh
End Property

Public Property Get BtnFaceColor(index As Integer) As OLE_COLOR
Attribute BtnFaceColor.VB_MemberFlags = "400"
    BtnFaceColor = m_BtnFaceColor(index)
End Property

Public Property Let BtnFaceColor(index As Integer, ByVal New_BtnFaceColor As OLE_COLOR)
    If Ambient.UserMode = False Then Err.Raise 387
    m_BtnFaceColor(index) = New_BtnFaceColor
    PropertyChanged "BtnFaceColor"
    Refresh
End Property

Private Sub AdjustPicturePos()
    On Error Resume Next
    Dim nLeft As Long, nTop As Long
    Dim W As Long, H As Long
    Dim XPixel As Long, YPixel As Long
    
    m_bLockPaint = True
    
    XPixel = ScaleX(1, vbPixels, vbTwips)
    YPixel = ScaleY(1, vbPixels, vbTwips)
    
    If m_Shadow = SHADOW_DOUBLE Then
        W = ScaleWidth - 3 * XPixel
        H = ScaleHeight - 3 * YPixel
    ElseIf m_Shadow = SHADOW_SINGLE Then
        W = ScaleWidth - 2 * XPixel
        H = ScaleHeight - 2 * YPixel
    Else
        W = ScaleWidth - XPixel
        H = ScaleHeight - YPixel
    End If
    
    Dim i As ButtonStateEnum
    For i = STATE_FIRST To STATE_LAST
        If Not imgPicture(i).Picture Is Nothing Then
            Select Case m_PictureVertAlign
                Case VA_TOP
                    nTop = m_nVertPictureShift * YPixel
                Case VA_BOTTOM
                    nTop = H - imgPicture(i).Height - m_nVertPictureShift * YPixel
                Case Else
                    nTop = (H - imgPicture(i).Height + m_nVertPictureShift * YPixel) / 2
            End Select
            
            Select Case m_PictureHorizAlign
                Case HA_LEFT
                    nLeft = m_nHorizPictureShift * XPixel
                Case HA_RIGHT
                    nLeft = W - imgPicture(i).Width - m_nHorizPictureShift * XPixel
                Case Else
                    'nLeft = (W - imgPicture(i).Width + m_nHorizPictureShift * XPixel) / 2
                    nLeft = (W - imgPicture(i).Width) / 2
            End Select
            
            If i = STATE_PRESSED Then
                nLeft = nLeft + XPixel
                nTop = nTop + YPixel
            End If
            
            If imgPicture(i).Left <> nLeft Then imgPicture(i).Left = nLeft
            If imgPicture(i).Top <> nTop Then imgPicture(i).Top = nTop
        Else
            If imgPicture(i).Left <> ScaleWidth Then imgPicture(i).Left = ScaleWidth
            If imgPicture(i).Top <> ScaleHeight Then imgPicture(i).Top = ScaleHeight
        End If
    Next
    
    m_bLockPaint = False
End Sub

' if visibility changed AdjustPictureVisibility returns true
Private Function AdjustPictureVisibility() As Boolean
    On Error Resume Next
    Dim i As ButtonStateEnum
    m_bLockPaint = True
    AdjustPictureVisibility = False
    For i = STATE_FIRST To STATE_LAST
        If imgPicture(i).Picture.Handle <> 0 Then
            If i <> m_enState Then
                If imgPicture(i).Visible Then
                    imgPicture(i).Visible = False
                    AdjustPictureVisibility = True
                End If
            Else
                If Not imgPicture(i).Visible Then
                    imgPicture(i).Visible = True
                    AdjustPictureVisibility = True
                End If
            End If
        End If
    Next
    ' !!! test code
    AdjustPictureVisibility = False
    m_bLockPaint = False
End Function
