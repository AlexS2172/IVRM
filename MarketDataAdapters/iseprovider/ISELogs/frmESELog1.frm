VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "elladaflatcontrols.ocx"
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmISELog1 
   Caption         =   "ISE Event Log"
   ClientHeight    =   5595
   ClientLeft      =   60
   ClientTop       =   390
   ClientWidth     =   8610
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmESELog1.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   5595
   ScaleWidth      =   8610
   Begin VB.PictureBox picBottom 
      Align           =   2  'Align Bottom
      Appearance      =   0  'Flat
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   495
      Left            =   0
      ScaleHeight     =   495
      ScaleWidth      =   8610
      TabIndex        =   4
      Top             =   5100
      Width           =   8610
      Begin VB.Timer tmrFlash 
         Enabled         =   0   'False
         Interval        =   500
         Left            =   2040
         Top             =   0
      End
      Begin VB.Timer Timer1 
         Left            =   2520
         Top             =   0
      End
      Begin VB.Timer Timer2 
         Left            =   3000
         Top             =   0
      End
      Begin VB.Timer Timer3 
         Interval        =   1000
         Left            =   3480
         Top             =   0
      End
      Begin ElladaFlatControls.FlatButton btnClear 
         Height          =   285
         Left            =   5160
         TabIndex        =   2
         Top             =   120
         Width           =   1215
         _ExtentX        =   2143
         _ExtentY        =   503
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Clear"
      End
      Begin ElladaFlatControls.FlatButton btnClose 
         Cancel          =   -1  'True
         Height          =   285
         Left            =   6480
         TabIndex        =   3
         Top             =   120
         Width           =   1215
         _ExtentX        =   2143
         _ExtentY        =   503
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Close"
      End
      Begin MSComctlLib.ImageList imgList2 
         Left            =   1320
         Top             =   0
         _ExtentX        =   1005
         _ExtentY        =   1005
         BackColor       =   -2147483643
         ImageWidth      =   32
         ImageHeight     =   32
         MaskColor       =   12632256
         _Version        =   393216
         BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
            NumListImages   =   5
            BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":058A
               Key             =   ""
            EndProperty
            BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":0E64
               Key             =   ""
            EndProperty
            BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":173E
               Key             =   ""
            EndProperty
            BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":2018
               Key             =   ""
            EndProperty
            BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":28F2
               Key             =   ""
            EndProperty
         EndProperty
      End
      Begin MSComctlLib.ImageList imgList 
         Left            =   720
         Top             =   0
         _ExtentX        =   1005
         _ExtentY        =   1005
         BackColor       =   -2147483643
         ImageWidth      =   16
         ImageHeight     =   16
         MaskColor       =   12632256
         _Version        =   393216
         BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
            NumListImages   =   6
            BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":31CC
               Key             =   ""
            EndProperty
            BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":3766
               Key             =   ""
            EndProperty
            BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":3D00
               Key             =   ""
            EndProperty
            BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":429A
               Key             =   ""
            EndProperty
            BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":4834
               Key             =   ""
            EndProperty
            BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmESELog1.frx":4DCE
               Key             =   ""
            EndProperty
         EndProperty
      End
      Begin ElladaFlatControls.FlatButton btnCopy 
         Height          =   285
         Left            =   3840
         TabIndex        =   5
         ToolTipText     =   "Copy messages to clipboard"
         Top             =   120
         Width           =   1215
         _ExtentX        =   2143
         _ExtentY        =   503
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Copy"
      End
      Begin VB.Image Image1 
         Height          =   375
         Left            =   0
         Top             =   0
         Width           =   375
      End
   End
   Begin VSFlex7Ctl.VSFlexGrid grdLog 
      Height          =   1695
      Left            =   60
      TabIndex        =   0
      Top             =   60
      Width           =   5805
      _cx             =   10239
      _cy             =   2990
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   1
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VSFlex7Ctl.VSFlexGrid grdLog2 
      Height          =   1695
      Left            =   60
      TabIndex        =   1
      Top             =   1800
      Width           =   5805
      _cx             =   10239
      _cy             =   2990
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   1
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VB.Menu mnuContext 
      Caption         =   "<context>"
      Visible         =   0   'False
      Begin VB.Menu mnuCopy 
         Caption         =   "Copy"
      End
      Begin VB.Menu mnuClear 
         Caption         =   "Clear"
      End
      Begin VB.Menu mnuSeparator 
         Caption         =   "-"
      End
      Begin VB.Menu mnuClose 
         Caption         =   "Close"
      End
   End
   Begin VB.Menu mnuContext2 
      Caption         =   "<context2>"
      Visible         =   0   'False
      Begin VB.Menu mnuCopy2 
         Caption         =   "Copy"
      End
      Begin VB.Menu mnuClear2 
         Caption         =   "Clear"
      End
      Begin VB.Menu mnuSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuClose2 
         Caption         =   "Close"
      End
   End
End
Attribute VB_Name = "frmISELog1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Declare Function FlashWindow Lib "user32" (ByVal hwnd As Long, ByVal bInvert As Long) As Long

Private Enum ColIndexEnum
    IDX_ID = 0
    IDX_TIME
    IDX_MESSAGE
    COLUMN_COUNT
End Enum

Private Enum StateEnum
    StateWaiting
    StateOK
    StateWarning
    StateBroken
End Enum

Private WithEvents ISETracer1 As ISEPROVIDERLib.ISETracer
Attribute ISETracer1.VB_VarHelpID = -1

Private m_nFlash As Long
Private m_nEventID As Long
Private t1 As Date
Private k As Long
Private f As Boolean
Private pState As StateEnum

Private Const TIMEOUT = 6
Private Const TIMEOUT2 = 12

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute() As Boolean
    Load Me
    Set ISETracer1 = New ISEPROVIDERLib.ISETracer
    ISETracer1.Bind
    If LoadSettings Then
        Me.Show
    End If
#If TEST Then
    Timer1.Interval = 20000
    Timer1.Enabled = True
#End If
    Timer2.Interval = 1000
    Timer2.Enabled = False
    Timer3.Interval = 1000
    Timer3.Enabled = True
    t1 = Now
    State = StateWaiting
    Execute = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Get State() As StateEnum
    State = pState
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Let State(ByVal vNewValue As StateEnum)
Dim im As Long
Dim tt As String
    Debug.Print "set state", vNewValue
    Select Case vNewValue
    Case StateWaiting:      im = 1: tt = "Waiting for responce"
    Case StateOK
        im = IIf(f, 2, 3)
        tt = "Connection OK"
        Timer2.Enabled = True
    Case StateWarning:      im = 4: tt = "Conneciton delayed"
        If pState <> StateOK Then
            Exit Property
        End If
    Case StateBroken:       im = 5: tt = "Connection broken"
    Case Else:              im = 1: tt = ""
    End Select
    pState = vNewValue
    Set Image1.Picture = imgList2.ListImages(im).Picture
    Image1.ToolTipText = tt
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If Shift = vbCtrlMask Then
        Debug.Print "Key code", KeyCode
        If KeyCode = vbKeyL Then
            KeyCode = 0
            mnuClose_Click
        ElseIf KeyCode = vbKeyH Then
            KeyCode = 0
            ISETracer1_OnHeartbeat 0
        ElseIf KeyCode = vbKeyM Then
            KeyCode = 0
            TestMessage
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ISETracer1_OnHeartbeat(ByVal Instance As Long)
    On Error Resume Next
    t1 = Now
    State = StateOK
    Debug.Print "Got hartbeat"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Resize()
    On Error Resume Next
    With grdLog
        .Width = ScaleWidth - 120
        .Height = (ScaleHeight - .Top - picBottom.Height - 60) / 2
    End With
    With grdLog2
        .Top = grdLog.Height + 120
        .Width = grdLog.Width
        .Height = grdLog.Height
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub picBottom_Resize()
    On Error Resume Next
    With btnClose
        .Left = picBottom.ScaleWidth - .Width - 60
    End With
    With btnClear
        .Left = btnClose.Left - .Width - 60
    End With
    With btnCopy
        .Left = btnClear.Left - .Width - 60
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Timer1_Timer()
#If TEST Then
    On Error Resume Next
    TestMessage
#End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub TestMessage()
    On Error Resume Next
    Dim s As String
    k = k + 1
    If k > 5 Then k = 0
    Select Case k
    Case enError:   s = "Test Error"
    Case enFatal:   s = "Test Fatal"
    Case enInfo:    s = "Test Info"
    Case enISE:     s = "Test ISE"
    Case enWarning: s = "Test Warning"
    Case Else:      s = "Test Unknown"
    End Select
    ISETracer1_OnMessage k, s
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Timer2_Timer()
Dim t2 As Date
    On Error Resume Next
    t2 = Now
    If DateDiff("s", t1, t2) > TIMEOUT Then
        If DateDiff("s", t1, t2) > TIMEOUT2 Then
            State = StateBroken
        Else
            State = StateWarning
        End If
    Else
        State = StateOK
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Timer3_Timer()
    f = Not f
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ISETracer1_OnMessage(ByVal MessageType As ISEPROVIDERLib.ISEInfoTypeEnum, ByVal Text As String)
Dim ImageIndex As Long
Dim MsgType As VbMsgBoxStyle
    On Error Resume Next
'    t1 = Now
'    State = StateOK
    
    Select Case MessageType
    Case enError    '0
        ImageIndex = 5
        MsgType = vbCritical
    Case enWarning  '1
        ImageIndex = 2
        MsgType = vbExclamation
    Case enInfo     '2
        ImageIndex = 1
        MsgType = vbInformation
    Case enFatal    '4
        ImageIndex = 4
        MsgType = vbCritical
    Case enISE      '5
        ImageIndex = 3
        MsgType = vbInformation
        '/////////////////////////////
        LogEvent2 Text, ImageIndex '/
        '///////////////////////////
    Case Else
        ImageIndex = 6
        MsgType = vbExclamation
    End Select
    
    '/////////////////////////////
    LogMessage Text, MsgType   '/
    '///////////////////////////
    
    '/////////////////////////////
    LogEvent1 Text, ImageIndex '/
    '///////////////////////////

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogEvent1( _
    ByVal Msg As String, _
    Optional ByVal ImageIndex As Long _
)
    LogEvent grdLog, Msg, ImageIndex
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogEvent2( _
    ByVal Msg As String, _
    Optional ByVal ImageIndex As Long _
)
    LogEvent grdLog2, Msg, ImageIndex
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogEvent( _
    grd As VSFlex7Ctl.VSFlexGrid, _
    ByVal Msg As String, _
    Optional ByVal ImageIndex As Long _
)
    On Error Resume Next
    With grd
        .Redraw = flexRDNone
        If .Rows > 500 Then
            .RemoveItem 1
        End If
        Msg = Replace(Msg, vbCr, " ", , , vbTextCompare)
        Msg = Replace(Msg, vbLf, ". ", , , vbTextCompare)
        m_nEventID = m_nEventID + 1
        .AddItem m_nEventID & vbTab & Now & vbTab & Msg
        Set .Cell(flexcpPicture, .Rows - 1, IDX_TIME) = imgList.ListImages(ImageIndex).Picture
        .Cell(flexcpPictureAlignment, .Rows - 1, IDX_TIME) = flexPicAlignLeftCenter
        .ShowCell .Rows - 1, IDX_TIME
        .Select .Rows - 1, IDX_MESSAGE
        .AutoSize 0, COLUMN_COUNT - 1, False, 100
        
        .Redraw = flexRDBuffered
    End With
        
'    If Me.Visible Then
'    Else
'        m_nFlash = 0
'        tmrFlash.Enabled = True
'    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FormatGrid(grd As VSFlex7Ctl.VSFlexGrid)
    On Error Resume Next
    
    With grd
        .Rows = 1
        .Cols = COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        
        '.BackColorAlternate = GCOLOR_TABLE_EVEN_ROW
        
        ' format columns
        .ColHidden(IDX_ID) = False
        .TextMatrix(0, IDX_ID) = "#"
        .ColDataType(IDX_ID) = flexDTLong
        
        .ColHidden(IDX_TIME) = False
        .TextMatrix(0, IDX_TIME) = "Time"
        .ColDataType(IDX_TIME) = flexDTDate
        .ColFormat(IDX_TIME) = ""
        
        .ColHidden(IDX_MESSAGE) = False
        .TextMatrix(0, IDX_MESSAGE) = "Message"
        .ColDataType(IDX_MESSAGE) = flexDTString
        .ColFormat(IDX_MESSAGE) = ""
        
        .ColAlignment(IDX_TIME) = flexAlignCenterCenter
        .ColAlignment(IDX_TIME) = flexAlignLeftCenter
        .ColAlignment(IDX_MESSAGE) = flexAlignLeftCenter
        
        .Cell(flexcpAlignment, 0, 0, 0, COLUMN_COUNT - 1) = flexAlignCenterCenter
        .Cell(flexcpPictureAlignment, 0, 0, 0, COLUMN_COUNT - 1) = flexPicAlignRightCenter
        '.AutoSize 0, COLUMN_COUNT - 1, False, 200
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCopy_Click()
    On Error Resume Next
    mnuCopy_Click
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnClear_Click()
    On Error Resume Next
    mnuClear_Click
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnClose_Click()
    On Error Resume Next
    mnuClose_Click
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
Dim lLogTop As Long
Dim lLogLeft As Long
Dim lLogWidth As Long
Dim lLogHeight As Long
    On Error Resume Next
'    CustomizeForm Me
    
    lLogTop = GetSettingString("ISELogs", "LogTop", "0")
    lLogLeft = GetSettingString("ISELogs", "LogLeft", "0")
    lLogWidth = GetSettingString("ISELogs", "LogWidth", "0")
    lLogHeight = GetSettingString("ISELogs", "LogHeight", "0")
    
    If lLogWidth > 0 And lLogHeight > 0 Then
        If lLogLeft < 0 Then lLogLeft = 0
        If lLogTop < 0 Then lLogTop = 0
        If lLogLeft > Screen.Width - 300 Then lLogLeft = Screen.Width - 300
        If lLogTop > Screen.Height - 300 Then lLogTop = Screen.Height - 300
        Me.Move lLogLeft, lLogTop, lLogWidth, lLogHeight
    Else
        With Me
            .Move (Screen.Width - .Width) / 2, (Screen.Height - .Height) / 2
        End With
    End If
    
    m_nEventID = 0
    FormatGrid grdLog
    FormatGrid grdLog2
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If Not ISETracer1 Is Nothing Then
        ISETracer1.Unbind
        Set ISETracer1 = Nothing
    End If
    With Me
        SetSettingString "ISELogs", "LogTop", .Top
        SetSettingString "ISELogs", "LogLeft", .Left
        SetSettingString "ISELogs", "LogWidth", .Width
        SetSettingString "ISELogs", "LogHeight", .Height
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdLog_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    With grdLog
        If Button = 2 And .MouseRow > -1 And .MouseCol > -1 Then
            PopupMenu mnuContext
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdLog2_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    With grdLog2
        If Button = 2 And .MouseRow > -1 And .MouseCol > -1 Then
            PopupMenu mnuContext2
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuClear_Click()
    On Error Resume Next
    With grdLog
        .Redraw = flexRDNone
        .Rows = 1
        .Redraw = flexRDBuffered
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuClear2_Click()
    On Error Resume Next
    With grdLog2
        .Redraw = flexRDNone
        .Rows = 1
        .Redraw = flexRDBuffered
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuClose_Click()
    On Error Resume Next
    Unload Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuClose2_Click()
    On Error Resume Next
    Unload Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub tmrFlash_Timer()
    On Error Resume Next
    If m_nFlash < 4 Then
        FlashWindow Me.hwnd, 1
        m_nFlash = m_nFlash + 1
    Else
        FlashWindow Me.hwnd, 0
        tmrFlash.Enabled = False
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuCopy_Click()
    On Error GoTo Herr
    CopyGridToClipboard grdLog
    Exit Sub
Herr:
    ShowError "Fail to copy the grid content to the clipboard."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuCopy2_Click()
    On Error GoTo Herr
    CopyGridToClipboard grdLog2
    Exit Sub
Herr:
    ShowError "Fail to copy the grid content to the clipboard."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Private Sub mnuPrint_Click()
'    On Error Resume Next
'    Load frmPrintPreview
'    frmPrintPreview.WindowState = vbMaximized
'
'    frmPrintPreview.PrintGrid grdLog, "ISE Events Log"
'    frmPrintPreview.Show vbModal, frmMain
'    Unload frmPrintPreview
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Private Sub mnuPrint2_Click()
'    On Error Resume Next
'    Load frmPrintPreview
'    frmPrintPreview.WindowState = vbMaximized
'
'    frmPrintPreview.PrintGrid grdLog2, "ISE Mesages Log"
'    frmPrintPreview.Show vbModal, frmMain
'    Unload frmPrintPreview
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadSettings() As Boolean
'    A = GetSettingString(Key, "")
    LoadSettings = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveSettings() As Boolean
'    SetSettingString Key, "", A
    
    SaveSettings = True
End Function

