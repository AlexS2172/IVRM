VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.UserControl ctlSplitter 
   BackColor       =   &H8000000C&
   ClientHeight    =   3600
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4800
   MousePointer    =   7  'Size N S
   ScaleHeight     =   3600
   ScaleWidth      =   4800
   Begin MSComctlLib.ImageList ilCursors 
      Left            =   1680
      Top             =   2280
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   32
      ImageHeight     =   32
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   2
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlSplitter.ctx":0000
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlSplitter.ctx":0222
            Key             =   ""
         EndProperty
      EndProperty
   End
End
Attribute VB_Name = "ctlSplitter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event SplitterMoved()

Public Enum SplitterOrientationEnum
    enSoVertical = 1
    enSoHorizontal = 2
End Enum

Const m_cDefSplitterMinLeft As Single = 0
Const m_cDefSplitterMinTop As Single = 0
Const m_cDefSplitterMaxWidth As Single = 0
Const m_cDefSplitterMaxHeight As Single = 0
Const m_cDefSplitterOrientation As Long = enSoVertical
Const m_cDefSplitterLeft As Single = 0
Const m_cDefSplitterTop As Single = 0
Const m_cDefSplitterWidth As Single = 0
Const m_cDefSplitterHeight As Single = 0

Dim m_sSplitterMinLeft As Single
Dim m_sSplitterMinTop As Single
Dim m_sSplitterMaxWidth As Single
Dim m_sSplitterMaxHeight As Single
Dim m_enSplitterOrientation As SplitterOrientationEnum
Dim m_sSplitterLeft As Single
Dim m_sSplitterTop As Single
Dim m_sSplitterWidth As Single
Dim m_sSplitterHeight As Single

Private m_bMouseCaptured As Boolean

'Private m_hwndPreviousCapture As Long

Public Function CaptureMouse()
    On Error Resume Next
    If Not m_bMouseCaptured Then
        'm_hwndPreviousCapture = SetCapture(UserControl.hwnd)
        m_bMouseCaptured = True
        If GetCapture <> UserControl.hwnd Then Debug.Assert False
    End If
End Function

Public Function ReleaseMouse()
    On Error Resume Next
    If m_bMouseCaptured Then
        ReleaseCapture
'        If m_hwndPreviousCapture <> 0 Then
'            'SetCapture m_hwndPreviousCapture
'            m_hwndPreviousCapture = 0
'        End If
        m_bMouseCaptured = False
    End If
End Function

Public Property Get SplitterOrientation() As SplitterOrientationEnum
    On Error Resume Next
    SplitterOrientation = m_enSplitterOrientation
End Property

'Public Property Let SplitterOrientation(ByVal NewValue As SplitterOrientationEnum)
'    On Error Resume Next
'
'    m_enSplitterOrientation = NewValue
'
'    Select Case True
'        Case NewValue = enSoHorizontal
'            UserControl.MousePointer = vbSizeNS
'
'        Case NewValue = enSoVertical
'            UserControl.MousePointer = vbSizeWE
'    End Select
'
'    PropertyChanged "SplitterOrientation"
'End Property

Public Property Let SplitterOrientation(ByVal NewValue As SplitterOrientationEnum)
    On Error Resume Next

    m_enSplitterOrientation = NewValue

    UserControl.MousePointer = vbCustom
    UserControl.MouseIcon = ilCursors.ListImages(NewValue).Picture

    PropertyChanged "SplitterOrientation"
End Property

Public Property Get SplitterLeft() As Single
    On Error Resume Next
    SplitterLeft = UserControl.Extender.Left
End Property

Public Property Let SplitterLeft(ByVal NewValue As Single)
    On Error Resume Next
    
    If m_enSplitterOrientation = enSoVertical Then
        If NewValue < UserControl.Extender.Left Then
            NewValue = m_sSplitterMinLeft
        End If
        
        If m_sSplitterMaxWidth > 0 And NewValue + SplitterWidth > m_sSplitterMaxWidth Then
            NewValue = m_sSplitterMaxWidth - SplitterWidth
        End If
    End If
    
    UserControl.Extender.Left = NewValue
    PropertyChanged "SplitterLeft"
    
    If m_enSplitterOrientation = enSoVertical Then
        RaiseEvent SplitterMoved
    End If
End Property

Public Property Get SplitterTop() As Single
    On Error Resume Next
    SplitterTop = UserControl.Extender.Top
End Property

Public Property Let SplitterTop(ByVal NewValue As Single)
    On Error Resume Next
    
    If m_enSplitterOrientation = enSoHorizontal Then
        If NewValue < m_sSplitterMinTop Then
            NewValue = m_sSplitterMinTop
        End If
        
        If m_sSplitterMaxHeight > 0 And NewValue + SplitterHeight > m_sSplitterMaxHeight Then
            NewValue = m_sSplitterMaxHeight - SplitterHeight
        End If
    End If
    
    UserControl.Extender.Top = NewValue
    PropertyChanged "SplitterTop"
    
    If m_enSplitterOrientation = enSoHorizontal Then
        RaiseEvent SplitterMoved
    End If
End Property

Public Property Get SplitterWidth() As Single
    On Error Resume Next
    SplitterWidth = UserControl.Extender.Width
End Property

Public Property Let SplitterWidth(ByVal NewValue As Single)
    On Error Resume Next
    UserControl.Extender.Width = NewValue
    PropertyChanged "SplitterWidth"
End Property

Public Property Get SplitterHeight() As Single
    On Error Resume Next
    SplitterHeight = UserControl.Extender.Height
End Property

Public Property Let SplitterHeight(ByVal NewValue As Single)
    On Error Resume Next
    UserControl.Extender.Height = NewValue
    PropertyChanged "SplitterHeight"
End Property

Private Sub UserControl_InitProperties()
    On Error Resume Next
    m_enSplitterOrientation = m_cDefSplitterOrientation
    m_sSplitterLeft = m_cDefSplitterLeft
    m_sSplitterTop = m_cDefSplitterTop
    m_sSplitterWidth = m_cDefSplitterWidth
    m_sSplitterHeight = m_cDefSplitterHeight
    m_sSplitterMinLeft = m_cDefSplitterMinLeft
    m_sSplitterMinTop = m_cDefSplitterMinTop
    m_sSplitterMaxWidth = m_cDefSplitterMaxWidth
    m_sSplitterMaxHeight = m_cDefSplitterMaxHeight
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    CaptureMouse
End Sub

Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    Static bMouseMoving As Boolean
    
    If Not bMouseMoving Then
        bMouseMoving = True
        If m_bMouseCaptured Then
            Select Case True
                Case SplitterOrientation = enSoVertical
                    SplitterLeft = SplitterLeft + X
                    
                Case SplitterOrientation = enSoHorizontal
                    SplitterTop = SplitterTop + Y
                    
            End Select
        End If
        bMouseMoving = False
    End If
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    ReleaseMouse
End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
    On Error Resume Next
    SplitterOrientation = PropBag.ReadProperty("SplitterOrientation", m_cDefSplitterOrientation)
    m_sSplitterLeft = PropBag.ReadProperty("SplitterLeft", m_cDefSplitterLeft)
    m_sSplitterTop = PropBag.ReadProperty("SplitterTop", m_cDefSplitterTop)
    m_sSplitterWidth = PropBag.ReadProperty("SplitterWidth", m_cDefSplitterWidth)
    m_sSplitterHeight = PropBag.ReadProperty("SplitterHeight", m_cDefSplitterHeight)
    m_sSplitterMinLeft = PropBag.ReadProperty("SplitterMinLeft", m_cDefSplitterMinLeft)
    m_sSplitterMinTop = PropBag.ReadProperty("SplitterMinTop", m_cDefSplitterMinTop)
    m_sSplitterMaxWidth = PropBag.ReadProperty("SplitterMaxWidth", m_cDefSplitterMaxWidth)
    m_sSplitterMaxHeight = PropBag.ReadProperty("SplitterMaxHeight", m_cDefSplitterMaxHeight)
    UserControl.BackColor = PropBag.ReadProperty("BackColor", &H80C0FF)
End Sub

Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
    On Error Resume Next
    PropBag.WriteProperty "SplitterOrientation", m_enSplitterOrientation, m_cDefSplitterOrientation
    PropBag.WriteProperty "SplitterLeft", m_sSplitterLeft, m_cDefSplitterLeft
    PropBag.WriteProperty "SplitterTop", m_sSplitterTop, m_cDefSplitterTop
    PropBag.WriteProperty "SplitterWidth", m_sSplitterWidth, m_cDefSplitterWidth
    PropBag.WriteProperty "SplitterHeight", m_sSplitterHeight, m_cDefSplitterHeight
    PropBag.WriteProperty "SplitterMinLeft", m_sSplitterMinLeft, m_cDefSplitterMinLeft
    PropBag.WriteProperty "SplitterMinTop", m_sSplitterMinTop, m_cDefSplitterMinTop
    PropBag.WriteProperty "SplitterMaxWidth", m_sSplitterMaxWidth, m_cDefSplitterMaxWidth
    PropBag.WriteProperty "SplitterMaxHeight", m_sSplitterMaxHeight, m_cDefSplitterMaxHeight
    PropBag.WriteProperty "BackColor", UserControl.BackColor, &H80C0FF
End Sub

Public Property Get SplitterMinLeft() As Single
    On Error Resume Next
    SplitterMinLeft = m_sSplitterMinLeft
End Property

Public Property Let SplitterMinLeft(ByVal NewValue As Single)
    On Error Resume Next
    m_sSplitterMinLeft = NewValue
    PropertyChanged "SplitterMinLeft"
End Property

Public Property Get SplitterMinTop() As Single
    On Error Resume Next
    SplitterMinTop = m_sSplitterMinTop
End Property

Public Property Let SplitterMinTop(ByVal NewValue As Single)
    On Error Resume Next
    m_sSplitterMinTop = NewValue
    PropertyChanged "SplitterMinTop"
End Property

Public Property Get SplitterMaxWidth() As Single
    On Error Resume Next
    SplitterMaxWidth = m_sSplitterMaxWidth
End Property

Public Property Let SplitterMaxWidth(ByVal NewValue As Single)
    On Error Resume Next
    m_sSplitterMaxWidth = NewValue
    PropertyChanged "SplitterMaxWidth"
End Property

Public Property Get SplitterMaxHeight() As Single
    On Error Resume Next
    SplitterMaxHeight = m_sSplitterMaxHeight
End Property

Public Property Let SplitterMaxHeight(ByVal NewValue As Single)
    On Error Resume Next
    m_sSplitterMaxHeight = NewValue
    PropertyChanged "SplitterMaxHeight"
End Property

Public Property Get BackColor() As OLE_COLOR
    On Error Resume Next
    BackColor = UserControl.BackColor
End Property

Public Property Let BackColor(ByVal NewValue As OLE_COLOR)
    On Error Resume Next
    UserControl.BackColor() = NewValue
    PropertyChanged "BackColor"
End Property


