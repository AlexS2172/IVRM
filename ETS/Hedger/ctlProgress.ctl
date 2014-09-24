VERSION 5.00
Object = "{3B008041-905A-11D1-B4AE-444553540000}#1.0#0"; "VSocx6.ocx"
Begin VB.UserControl ctlProgress 
   Appearance      =   0  'Flat
   BackColor       =   &H00000000&
   ClientHeight    =   525
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   2565
   FillColor       =   &H00C0FFFF&
   ForeColor       =   &H00C0FFFF&
   ScaleHeight     =   525
   ScaleWidth      =   2565
   Begin vsOcx6LibCtl.vsElastic vsProgress 
      Height          =   240
      Left            =   0
      TabIndex        =   0
      TabStop         =   0   'False
      Top             =   0
      Width           =   2280
      _ExtentX        =   4022
      _ExtentY        =   423
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Enabled         =   -1  'True
      Appearance      =   0
      MousePointer    =   0
      _ConvInfo       =   1
      Version         =   600
      BackColor       =   49152
      ForeColor       =   8421376
      FloodColor      =   32768
      ForeColorDisabled=   8421376
      Caption         =   ""
      Align           =   0
      Appearance      =   0
      AutoSizeChildren=   0
      BorderWidth     =   0
      ChildSpacing    =   4
      Splitter        =   0   'False
      FloodDirection  =   1
      FloodPercent    =   10
      CaptionPos      =   1
      WordWrap        =   -1  'True
      MaxChildSize    =   0
      MinChildSize    =   0
      TagWidth        =   0
      TagPosition     =   0
      Style           =   0
      TagSplit        =   0   'False
      PicturePos      =   4
      CaptionStyle    =   0
      ResizeFonts     =   0   'False
      GridRows        =   0
      GridCols        =   0
      _GridInfo       =   ""
      Begin VB.Timer Timer 
         Enabled         =   0   'False
         Interval        =   100
         Left            =   -720
         Top             =   0
      End
   End
   Begin VB.Shape Shape1 
      BorderColor     =   &H00C0C0C0&
      Height          =   375
      Left            =   0
      Top             =   0
      Width           =   2415
   End
End
Attribute VB_Name = "ctlProgress"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private m_lInc As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    Timer.Enabled = False
    Timer.Interval = 50
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    On Error Resume Next
    With Shape1
        .Width = ScaleWidth
        .Height = ScaleHeight
    End With
    With vsProgress
        .Width = ScaleWidth
        .Height = ScaleHeight
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let ProgressBackColor(ByVal clr As OLE_COLOR)
    vsProgress.BackColor = clr
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let ProgressForeColor(ByVal clr As OLE_COLOR)
    vsProgress.ForeColor = clr
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IsRunning() As Boolean
    IsRunning = Timer.Enabled
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartProgress()
    m_lInc = 1
    vsProgress.FloodPercent = 0
    vsProgress.FloodDirection = fdRight
    Timer.Enabled = True
    vsProgress.Visible = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StopProgress()
    Timer.Enabled = False
    vsProgress.Visible = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Timer_Timer()
    On Error Resume Next
    vsProgress.FloodPercent = vsProgress.FloodPercent + m_lInc
    If vsProgress.FloodPercent = 100 Then
        If vsProgress.FloodDirection = fdRight Then
            vsProgress.FloodDirection = fdLeft
            m_lInc = -1
        Else
            vsProgress.FloodDirection = fdRight
            m_lInc = -1
        End If
    ElseIf vsProgress.FloodPercent = 0 Then
        If vsProgress.FloodDirection = fdLeft Then
            m_lInc = 1
        Else
            m_lInc = 1
        End If
    End If
    DoEvents
End Sub

