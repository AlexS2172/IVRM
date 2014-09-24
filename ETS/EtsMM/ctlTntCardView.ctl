VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.UserControl ctlTntCardView 
   ClientHeight    =   7560
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   12360
   KeyPreview      =   -1  'True
   ScaleHeight     =   7560
   ScaleWidth      =   12360
   Begin VB.PictureBox picLog 
      BorderStyle     =   0  'None
      Height          =   1395
      Left            =   0
      ScaleHeight     =   1395
      ScaleWidth      =   12075
      TabIndex        =   3
      TabStop         =   0   'False
      Top             =   5640
      Width           =   12075
      Begin VSFlex7Ctl.VSFlexGrid fgLog 
         Height          =   1095
         Left            =   0
         TabIndex        =   2
         Top             =   255
         Width           =   10995
         _cx             =   19394
         _cy             =   1931
         _ConvInfo       =   1
         Appearance      =   1
         BorderStyle     =   1
         Enabled         =   -1  'True
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
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
      Begin VB.Label lblLogCaption 
         BackColor       =   &H80000002&
         Caption         =   " Event Log"
         ForeColor       =   &H80000009&
         Height          =   195
         Left            =   0
         TabIndex        =   5
         Top             =   0
         Width           =   10995
      End
   End
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   1080
      Top             =   4440
   End
   Begin VSFlex7Ctl.VSFlexGrid fgFlt 
      Height          =   495
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   7575
      _cx             =   13361
      _cy             =   873
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
   Begin VSFlex7Ctl.VSFlexGrid fgCrd 
      Height          =   2535
      Left            =   0
      TabIndex        =   1
      Top             =   600
      Width           =   7575
      _cx             =   13361
      _cy             =   4471
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
   Begin MSComctlLib.ImageList ilEvent 
      Left            =   1740
      Top             =   3600
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   3
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlTntCardView.ctx":0000
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlTntCardView.ctx":005E
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlTntCardView.ctx":00BC
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin EtsMM.ctlSplitter ctlSplit 
      Height          =   75
      Left            =   0
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   5400
      Width           =   10755
      _ExtentX        =   18971
      _ExtentY        =   132
      SplitterOrientation=   2
   End
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
      End
      Begin VB.Menu mnuCtxTntCardAccept 
         Caption         =   "Accept/ACK Trade Message(s)..."
      End
      Begin VB.Menu mnuCtxTntCardReject 
         Caption         =   "Reject Trade Message(s)..."
      End
      Begin VB.Menu mnuCtxTntCardDelete 
         Caption         =   "Delete Trade Message(s)..."
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxDefCrdSort 
         Caption         =   "Default Trade Messages Sorting"
         Shortcut        =   ^D
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxCopy 
         Caption         =   "Copy Grid"
      End
      Begin VB.Menu mnuCtxPrint 
         Caption         =   "Print Grid..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxHideCol 
         Caption         =   "Hide Column"
      End
      Begin VB.Menu mnuCtxAutosizeCol 
         Caption         =   "Autosize Column"
      End
      Begin VB.Menu mnuCtxAutosizeGrid 
         Caption         =   "Autosize Grid"
      End
      Begin VB.Menu mnuCtxSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxClearLog 
         Caption         =   "Clear"
      End
   End
End
Attribute VB_Name = "ctlTntCardView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public Event OnSetCaption()
'Public Event OnStateChange()
'
'Private WithEvents TntProcessor As clsTntProcessor
'Private WithEvents frmLayout As frmGridLayout
'
'Public pbProgress As MSComctlLib.ProgressBar
'Public lblProcess As VB.Label
'Public lblStatus As VB.Label
'
'Private m_gdFlt As New clsGridDef
'Private m_gdCrd As New clsGridDef
'Private m_gdLog As New clsGridDef
'
'Private WithEvents geCrd As clsGridEx
'
'Private m_nFilter(CFC_FIRST_COLUMN To CFC_LAST_COLUMN) As Long
'
'Private m_bInProc As Boolean
'Private m_bShutDown As Boolean
'Private m_sCurrentOriginalText As String
'
'Private m_enMenuGrid As GridTypeEnum
'Private m_nMenuGridCol As Long
'Private m_nMenuGridRow As Long
'Private m_nMenuGridCols As Long
'Private m_nMenuGridRows As Long
'
'Private m_bKeyDown(GT_TNTCARDS_FILTER To GT_TNTCARDS_LOG) As Boolean
'Private m_GridLock(GT_TNTCARDS_FILTER To GT_TNTCARDS_LOG) As New clsGridLock
'Private m_nCards As Long
'
'Public m_frmOwner As Form
'Private m_bLogVisible As Boolean
'Private m_bLogLoaded As Boolean
'
'Public Function Init() As Boolean
'    On Error GoTo EH
'
'    m_bLogVisible = True
'    m_bLogLoaded = False
'
'    ctlSplit.BackColor = &H8000000F
'    ctlSplit.SplitterTop = ScaleHeight - picLog.Height
'
'    picLog.Left = 0
'    fgLog.Left = 0
'    lblLogCaption.Left = 0
'    ctlSplit.Left = 0
'    fgFlt.Top = 0
'    fgFlt.Left = 0
'    fgCrd.Left = 0
'
'    mnuCtxTntCardNew.Caption = "New Message..." & vbTab & "Shift+Alt+Ins"
'    mnuCtxTntCardAccept.Caption = "Accept/ACK Message(s)..." & vbTab & "Enter"
'    mnuCtxTntCardReject.Caption = "Reject Message(s)..." & vbTab & "Del"
'    mnuCtxTntCardDelete.Caption = "Delete Message(s)..." & vbTab & "Ctrl+Del"
'    mnuCtxCopy.Caption = "Copy Grid" & vbTab & "Ctrl+C"
'    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
'    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
'    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
'    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
'
'    ResetMenuData
'    m_sCurrentOriginalText = ""
'    m_bKeyDown(GT_TNTCARDS_FILTER) = False
'    m_bKeyDown(GT_TNTCARDS_LIST) = False
'    m_bKeyDown(GT_TNTCARDS_LOG) = False
'    m_nCards = 0
'
'    m_GridLock(GT_TNTCARDS_FILTER).Init fgFlt
'    m_GridLock(GT_TNTCARDS_LIST).Init fgCrd
'    m_GridLock(GT_TNTCARDS_LOG).Init fgLog
'
'    InitColumns
'    InitGrids
'
'    Set geCrd = New clsGridEx
'    geCrd.Init fgCrd
'
'    FormatFltGrid
'    FormatFltColumns
'
'    FormatCrdGrid
'    FormatCrdColumns
'
'    FormatLogGrid
'    FormatLogColumns
'
'    InitFltData
'
'    Set TntProcessor = g_TntProcessor
'
'    fgFlt.Col = CFC_FIRST_COLUMN
'    fgFlt.Row = 1
'
'    Set frmLayout = New frmGridLayout
'    Init = True
'
'    AdjustCaption
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to init trades messages view."
'End Function
'
'Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
'    On Error Resume Next
'    Select Case nType
'        Case TYPE_UNDERLYING
'            m_nFilter(CFC_UNDERLYING) = nValue
'        Case TYPE_GROUP
'            m_nFilter(CFC_GROUP) = nValue
'        Case TYPE_CNT_PTY
'            m_nFilter(CFC_CNT_PTY) = nValue
'    End Select
'
'    FilterUpdate
'
'    tmrShow.Enabled = True
'End Sub
'
'Private Sub FilterUpdate()
'    On Error Resume Next
'    Dim nValue&
'
'    With fgFlt
'        m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'
'        nValue = m_nFilter(CFC_UNDERLYING)
'        If g_Underlying(nValue) Is Nothing Then nValue = 0
'        m_nFilter(CFC_UNDERLYING) = nValue
'        .TextMatrix(1, CFC_UNDERLYING) = nValue
'
'        .TextMatrix(1, CFC_STATUS) = m_nFilter(CFC_STATUS)
'
'        nValue = m_nFilter(CFC_GROUP)
'        If g_UnderlyingGroup(nValue) Is Nothing Then nValue = 0
'        m_nFilter(CFC_GROUP) = nValue
'        .TextMatrix(1, CFC_GROUP) = nValue
'
'        nValue = m_nFilter(CFC_CNT_PTY)
'        If g_TntCntPty(nValue) Is Nothing Then nValue = 0
'        m_nFilter(CFC_CNT_PTY) = nValue
'        .TextMatrix(1, CFC_CNT_PTY) = nValue
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub InitFltData()
'    On Error Resume Next
'    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom, nValue&
'    Dim aCntPty As EtsMmGeneralLib.MmTntCounterPartyAtom, aUndGroup As EtsGeneralLib.EtsUndGroupAtom
'
'    With fgFlt
'        m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'
'        sValue = "0"
'        nValue = 0
'        sComboList = g_Params.UnderlyingComboList
'        If Len(sComboList) > 0 Then
'            sComboList = "#0;<All>|" & sComboList
'        Else
'            sComboList = "#0;<All>"
'        End If
'
'        Set aUnd = g_Underlying(m_nFilter(CFC_UNDERLYING))
'        If Not aUnd Is Nothing Then
'            nValue = aUnd.ID
'            sValue = Trim$(Str$(aUnd.ID))
'            Set aUnd = Nothing
'        End If
'        .ColComboList(CFC_UNDERLYING) = sComboList
'        .TextMatrix(1, CFC_UNDERLYING) = sValue
'        m_nFilter(CFC_UNDERLYING) = nValue
'
'        'enTcsOutPendingExecute      = 1
'        'enTcsOutExecuted            = 2
'        'enTcsOutFailed              = 3
'        'enTcsInNew                  = 4
'        'enTcsInPendingAccept        = 5
'        'enTcsInAccepted             = 6
'        'enTcsInFailed               = 7
'        'enTcsInRejected             = 8
'
'        .ColComboList(CFC_STATUS) = "#0;<All>|#1;New|#2;Executions|#3;Accepts|#4;Rejects|#5;Fails"
'        .TextMatrix(1, CFC_STATUS) = "0"
'
'        sValue = "0"
'        nValue = 0
'        sComboList = "#0;<All>"
'        For Each aUndGroup In g_UnderlyingGroup
'            sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
'            If aUndGroup.ID = m_nFilter(CFC_GROUP) Then
'                nValue = aUndGroup.ID
'                sValue = Trim$(Str$(aUndGroup.ID))
'            End If
'        Next
'        .ColComboList(CFC_GROUP) = sComboList
'        .TextMatrix(1, CFC_GROUP) = sValue
'        m_nFilter(CFC_GROUP) = nValue
'
'        sValue = "0"
'        nValue = 0
'        sComboList = "#0;<All>"
'        For Each aCntPty In g_TntCntPty
'            sComboList = sComboList & "|#" & Trim$(Str$(aCntPty.ID)) & ";" & aCntPty.Name
'            If aCntPty.ID = m_nFilter(CFC_CNT_PTY) Then
'                nValue = aCntPty.ID
'                sValue = Trim$(Str$(nValue))
'            End If
'        Next
'        .ColComboList(CFC_CNT_PTY) = sComboList
'        .TextMatrix(1, CFC_CNT_PTY) = sValue
'        m_nFilter(CFC_CNT_PTY) = nValue
'
'        m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub CardsLogShow()
'    On Error Resume Next
'    Dim aEvent As EtsGeneralLib.EtsEventAtom
'    With fgLog
'        m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'        .Rows = 1
'
'        pbProgress.Max = TntProcessor.Events.Count
'
'        For Each aEvent In TntProcessor.Events
'            CardsLogAddEvent aEvent, False
'
'            DoEvents
'            If m_bShutDown Then Exit Sub
'            IncProgress pbProgress
'        Next
'
'        .AutoSize 0, 1
'        .ColWidth(2) = .Width - .ColWidth(0) - .ColWidth(1) - ScaleX(GetSystemMetrics(SM_CXVSCROLL) + 10, vbPixels, vbTwips)
'
'        m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'
'        If .Rows > 0 Then .Row = .Rows - 1
'        SendMessage .hWnd, WM_VSCROLL, SB_BOTTOM, 0&
'    End With
'End Sub
'
'Private Sub CardsLogAddEvent(ByRef aEvent As EtsGeneralLib.EtsEventAtom, ByVal bShowNewRow As Boolean)
'    On Error Resume Next
'    If aEvent Is Nothing Then Exit Sub
'
'    Dim nRow&, nMsgRows&
'
'    With fgLog
'        m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'
'        bShowNewRow = bShowNewRow And (.Row < 1 Or .Row = .Rows - 1)
'
'        .AddItem aEvent.EvtID & vbTab & aEvent.EvtTime & vbTab & aEvent.EvtMessage
'        nRow = .Rows - 1
'
'        nMsgRows = InStrCount(aEvent.EvtMessage, vbCrLf)
'
'        If nMsgRows > 0 Then
'            .RowHeight(nRow) = .RowHeight(0) * (nMsgRows + 1)
'        End If
'
'        If aEvent.EvtType <> enEvtUnknown Then
'            Set .Cell(flexcpPicture, nRow, CEC_TIME) = ilEvent.ListImages(aEvent.EvtType).Picture
'            .Cell(flexcpPictureAlignment, nRow, CEC_TIME) = flexPicAlignLeftCenter
'        End If
'
'        m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'
'        If bShowNewRow Then
'            If .Col < 0 Then .Col = CEC_MESSAGE
'            .Row = nRow
'            SendMessage .hWnd, WM_VSCROLL, SB_BOTTOM, 0&
'        End If
'    End With
'End Sub
'
'Private Sub CardsShow(ByVal bReload As Boolean)
'    On Error Resume Next
'    Dim i&, nCount&, aCrd As EtsMmGeneralLib.MmTntCardAtom, nRow&, nOldRow&, nOldCol&
'
'    m_bInProc = True
'    RaiseEvent OnStateChange
'
'    Screen.MousePointer = vbArrowHourglass
'    DoEvents
'
'    lblStatus.Visible = False
'    pbProgress.Min = 0
'    pbProgress.Value = 0
'    pbProgress.Visible = True
'    lblProcess.Caption = IIf(bReload, "Data loading...", "Filter applying...")
'    lblProcess.Refresh
'    lblProcess.Visible = True
'
'    If Not m_bLogLoaded Then
'        lblProcess.Caption = "Trades messages event log loading..."
'        CardsLogShow
'        m_bLogLoaded = True
'    End If
'
'    With fgCrd
'        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'        nOldRow = .Row
'        nOldCol = .Col
'
'        m_nCards = 0
'
'        If bReload Then
'            .Rows = 1
'            nCount = TntProcessor.Cards.Count
'            pbProgress.Max = nCount
'
'            For Each aCrd In TntProcessor.Cards
'                If Not aCrd Is Nothing Then
'                    .AddItem ""
'                    nRow = .Rows - 1
'
'                    .RowData(nRow) = aCrd
'                    .TextMatrix(nRow, CLC_KEY) = aCrd.TradeID
'
'                    CardUpdate nRow, aCrd, False
'
'                    If CheckCardFilter(aCrd) Then
'                        .RowHidden(nRow) = False
'                        m_nCards = m_nCards + 1
'                    Else
'                        .RowHidden(nRow) = True
'                    End If
'
'                    Set aCrd = Nothing
'                End If
'
'                DoEvents
'                If m_bShutDown Then Exit Sub
'                IncProgress pbProgress
'            Next
'
'            If m_bShutDown Then Exit Sub
'            lblProcess.Caption = "Formatting..."
'            lblProcess.Refresh
'
'            i = -1
'            i = .ColIndex(CLC_DATE)
'            If i >= 0 Then
'                .Col = i
'                .Row = 1
'                .Sort = flexSortGenericDescending
'            End If
'            geCrd.ShowSortImage i, -1
'
'            FormatCrdColumns
'            CardsUpdateColors
'        Else
'            nCount = .Rows - 1
'            pbProgress.Max = nCount
'
'            For nRow = 1 To nCount
'                Set aCrd = .RowData(nRow)
'                If Not aCrd Is Nothing Then
'                    If CheckCardFilter(aCrd) Then
'                        .RowHidden(nRow) = False
'                        m_nCards = m_nCards + 1
'                    Else
'                        .RowHidden(nRow) = True
'                        .IsSelected(nRow) = False
'                    End If
'                    Set aCrd = Nothing
'                Else
'                    Debug.Assert False
'                End If
'
'                DoEvents
'                If m_bShutDown Then Exit Sub
'                IncProgress pbProgress
'            Next
'
'            lblProcess.Caption = "Formatting..."
'            lblProcess.Refresh
'        End If
'
'        If nOldRow > 0 And m_nCards > 0 Then
'            If .RowHidden(nOldRow) Then
'                For nRow = nOldRow To nCount
'                    If Not .RowHidden(nRow) Then
'                        nOldRow = nRow
'                        Exit For
'                    End If
'                Next
'
'                If .RowHidden(nOldRow) Then
'                    For nRow = nOldRow To 1 Step -1
'                        If Not .RowHidden(nRow) Then
'                            nOldRow = nRow
'                            Exit For
'                        End If
'                    Next
'                End If
'
'                If .RowHidden(nOldRow) Then nOldRow = -1
'            End If
'
'            .Row = nOldRow
'            If nOldCol > 0 Then .Col = nOldCol Else .Col = 1
'            .SetFocus
'        Else
'            If m_nCards > 0 Then
'                .Row = -1
'
'                For nRow = 1 To nCount
'                    If Not .RowHidden(nRow) Then
'                        .Row = nRow
'                        Exit For
'                    End If
'                Next
'
'                If nOldCol > 0 Then .Col = nOldCol Else .Col = 1
'                .SetFocus
'            Else
'                .Row = -1
'                .Col = -1
'
'                fgFlt.SetFocus
'            End If
'        End If
'
'        UpdateCardsCountLabel
'
'        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'    End With
'
'    DoEvents
'    If m_bShutDown Then Exit Sub
'    pbProgress.Visible = False
'    lblProcess.Visible = False
'    lblStatus.Visible = True
'
'    m_bInProc = False
'    RaiseEvent OnStateChange
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub UpdateCardsCountLabel()
'    On Error Resume Next
'    lblStatus.Caption = Format$(m_nCards, "#,##0") & " trade message(s)"
'End Sub
'
'Private Sub CardsUpdateColors()
'    On Error Resume Next
'    Dim nRow&
'
'    With fgCrd
'        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'        For nRow = 1 To .Rows - 1
'            CardUpdateColors nRow, Nothing
'        Next
'
'        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Sub CardUpdateColors(ByVal nRow As Long, ByRef aRowCrd As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    Dim nCol&, bUseAltColor As Boolean, clrBackColor As OLE_COLOR
'    Dim aCrd As EtsMmGeneralLib.MmTntCardAtom
'
'    With fgCrd
'        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'        If Not aRowCrd Is Nothing Then
'            Set aCrd = aRowCrd
'        Else
'            Set aCrd = .RowData(nRow)
'        End If
'
'        If Not aCrd Is Nothing Then
'
'            bUseAltColor = aCrd.IsExpired
'
'            Dim i&, nIdx&
'            i = 0
'            nIdx = m_gdCrd.Idx(0)
'            While nIdx >= 0 And i <= CLC_LAST_COLUMN
'                nCol = i + 1
'
'                clrBackColor = m_gdCrd.Col(nIdx).BackColor
'                If bUseAltColor Then clrBackColor = GetAlternateColor(clrBackColor)
'
'                .Cell(flexcpBackColor, nRow, nCol, nRow, nCol) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))
'
'                i = i + 1
'                nIdx = m_gdCrd.Idx(i)
'            Wend
'
'            Set aCrd = Nothing
'        End If
'
'        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Sub CardUpdate(ByVal nRow As Long, ByRef aRowCrd As EtsMmGeneralLib.MmTntCardAtom, ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim nCol&, bUseAltColor As Boolean, clrBackColor As OLE_COLOR
'    Dim aCrd As EtsMmGeneralLib.MmTntCardAtom, sValue$
'
'    With fgCrd
'        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'        If Not aRowCrd Is Nothing Then
'            Set aCrd = aRowCrd
'        Else
'            Set aCrd = .RowData(nRow)
'        End If
'
'        If Not aCrd Is Nothing Then
'
'            bUseAltColor = aCrd.IsExpired
'
'            Dim i&, nIdx&
'            i = 0
'            nIdx = m_gdCrd.Idx(0)
'            While nIdx >= 0 And i <= CLC_LAST_COLUMN
'
'                nCol = i + 1
'
'                Select Case nIdx
'
'                    Case CLC_UNDERLYING
'                        .TextMatrix(nRow, nCol) = aCrd.Und.Symbol
'
'                    Case CLC_SYMBOL
'                        .TextMatrix(nRow, nCol) = aCrd.Opt.Symbol
'
'                    Case CLC_SERIES
'                        .TextMatrix(nRow, nCol) = aCrd.Opt.Series
'
'                    Case CLC_TRADE_ID
'                        .TextMatrix(nRow, nCol) = aCrd.TradeID
'
'                    Case CLC_BS
'                        .TextMatrix(nRow, nCol) = aCrd.IsBuy
'
'                    Case CLC_QTY
'                        .TextMatrix(nRow, nCol) = aCrd.Qty
'
'                    Case CLC_PRICE
'                        .TextMatrix(nRow, nCol) = aCrd.Price
'
'                    Case CLC_STATUS
'                        .TextMatrix(nRow, nCol) = aCrd.CardStatusString
'
'                    Case CLC_DATE
'                        .TextMatrix(nRow, nCol) = aCrd.TradeDate
'
'                    Case CLC_ACCOUNT
'                        .TextMatrix(nRow, nCol) = aCrd.Account
'
'                    Case CLC_CNT_PTY
'                        .TextMatrix(nRow, nCol) = aCrd.CntPty.Name
'
'                    Case CLC_OPT_TYPE
'                        .TextMatrix(nRow, nCol) = (aCrd.Opt.OptType = enOtCall)
'
'                    Case CLC_EXPIRY
'                        .TextMatrix(nRow, nCol) = aCrd.Opt.Expiry
'
'                    Case CLC_STRIKE
'                        .TextMatrix(nRow, nCol) = aCrd.Opt.Strike
'
'                    Case CLC_LOTSIZE
'                        .TextMatrix(nRow, nCol) = aCrd.OptRoot.LotSize
'
'                    Case CLC_TOT_PRICE
'                        .TextMatrix(nRow, nCol) = aCrd.Qty * aCrd.Price * aCrd.OptRoot.LotSize
'
'                    Case CLC_MESSAGE
'                        .TextMatrix(nRow, nCol) = aCrd.Msg
'
'                End Select
'
'                If bUpdateColors Then
'                    clrBackColor = m_gdCrd.Col(nIdx).BackColor
'                    If bUseAltColor Then clrBackColor = GetAlternateColor(clrBackColor)
'
'                    .Cell(flexcpBackColor, nRow, nCol, nRow, nCol) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))
'                End If
'
'                i = i + 1
'                nIdx = m_gdCrd.Idx(i)
'            Wend
'
'            Set aCrd = Nothing
'        End If
'
'        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Function NoFilter() As Boolean
'    On Error Resume Next
'    NoFilter = Not (m_nFilter(CFC_UNDERLYING) <> 0 _
'                    Or m_nFilter(CFC_STATUS) <> 0 _
'                    Or m_nFilter(CFC_GROUP) <> 0 _
'                    Or m_nFilter(CFC_CNT_PTY))
'End Function
'
'Private Function CheckCardFilter(ByRef aCrd As EtsMmGeneralLib.MmTntCardAtom) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean, nValue&
'
'    If Not NoFilter Then
'        nValue = m_nFilter(CFC_UNDERLYING)
'        bMatched = (nValue = 0 Or nValue = aCrd.Und.ID)
'
'        If bMatched Then
'            ' "#0;<All>|#1;New|#2;Executions|#3;Accepts|#4;Rejects|#5;Fails"
'            nValue = m_nFilter(CFC_STATUS)
'            bMatched = (nValue = 0)
'
'            If Not bMatched Then
'                Select Case nValue
'                    Case 1 ' New
'                        bMatched = (aCrd.Status = enTcsInNew)
'
'                    Case 2 ' Executions
'                        bMatched = (aCrd.Status = enTcsOutPendingExecute _
'                                Or aCrd.Status = enTcsOutExecuted)
'
'                    Case 3 ' Accepts
'                        bMatched = (aCrd.Status = enTcsInPendingAccept _
'                                Or aCrd.Status = enTcsInAccepted)
'
'                    Case 4 ' Rejects
'                        bMatched = (aCrd.Status = enTcsInRejected)
'
'                    Case 5 ' Fails
'                        bMatched = (aCrd.Status = enTcsOutFailed _
'                                Or aCrd.Status = enTcsInFailed)
'                End Select
'            End If
'        End If
'
'        If bMatched Then
'            nValue = m_nFilter(CFC_GROUP)
'            bMatched = (nValue = 0)
'            If Not bMatched Then
'                bMatched = Not g_UnderlyingGroup(nValue).Und(aCrd.Und.ID) Is Nothing
'            End If
'        End If
'
'        If bMatched Then
'            nValue = m_nFilter(CFC_CNT_PTY)
'            bMatched = (nValue = 0 Or aCrd.CntPty.ID = nValue)
'        End If
'
'        CheckCardFilter = bMatched
'    Else
'        CheckCardFilter = True
'    End If
'End Function
'
'Private Sub InitColumns()
'    On Error Resume Next
'    g_DefGrids(GT_TNTCARDS_FILTER).CopyTo m_gdFlt
'    g_DefGrids(GT_TNTCARDS_LIST).CopyTo m_gdCrd
'    g_DefGrids(GT_TNTCARDS_LOG).CopyTo m_gdLog
'End Sub
'
'Private Sub InitGrids()
'    On Error Resume Next
'    With fgFlt
'        .Rows = 2
'        .Cols = CFC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeNone
'        .AutoSizeMode = flexAutoSizeColWidth
'        .Appearance = flex3DLight
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbdMouse
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExNone
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarNone
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'        .HighLight = flexHighlightWithFocus
'    End With
'
'    With fgCrd
'        .Rows = 1
'        .Cols = 10
'
'        .AllowBigSelection = False
'        .AllowSelection = True
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDNone
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExSortAndMove
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusHeavy
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarBoth
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionListBox
'        .TabBehavior = flexTabControls
'        .HighLight = flexHighlightAlways
'    End With
'
'    With fgLog
'        .Rows = 1
'        .Cols = CEC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = True
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .AutoSizeMode = flexAutoSizeColWidth
'        .Appearance = flex3DLight
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDNone
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExNone
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarBoth
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'        .HighLight = flexHighlightWithFocus
'        '.ColWidthMin = 1350
'    End With
'End Sub
'
'Private Sub FormatFltGrid()
'    On Error Resume Next
'    Dim i&, nCols&
'
'    With fgFlt
'        m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'
'        nCols = CFC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = CFC_FIRST_COLUMN To CFC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
'            .ColFormat(i) = m_gdFlt.Col(i).Format
'            .ColDataType(i) = m_gdFlt.Col(i).DataType
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatFltColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont
'
'    With fgFlt
'        m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'
'        aFont.Name = m_gdFlt.Font.Name
'        aFont.Size = m_gdFlt.Font.Size
'        aFont.Bold = m_gdFlt.Font.Bold
'        aFont.Italic = m_gdFlt.Font.Italic
'        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
'        aFont.Underline = m_gdFlt.Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = CFC_FIRST_COLUMN To CFC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).BackColor <> 0, m_gdFlt.Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).ForeColor <> 0, m_gdFlt.Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If m_gdFlt.HorizLinesVisible Then
'            If m_gdFlt.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdFlt.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdFlt.GridLinesColor
'
'        .BackColorSel = m_gdFlt.BackColorSel
'        .ForeColorSel = m_gdFlt.ForeColorSel
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatCrdGrid()
'    On Error Resume Next
'    Dim i&, nCols&, nIdx&
'
'    With fgCrd
'        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'        nCols = 0
'        For i = 0 To CLC_LAST_COLUMN
'            If m_gdCrd.Idx(i) <> CLC_NONE Then
'                nCols = nCols + 1
'            Else
'                Exit For
'            End If
'        Next
'
'        If nCols = 0 Then
'            m_gdCrd.Idx(0) = CLC_UNDERLYING
'            nCols = 1
'        End If
'
'        nCols = nCols + 1 ' for key column
'        .Cols = nCols
'
'        .ColDataType(CLC_KEY) = flexDTString
'        .ColHidden(CLC_KEY) = True
'        .ColKey(CLC_KEY) = CLC_NONE
'
'        For i = 0 To CLC_LAST_COLUMN
'            nIdx = m_gdCrd.Idx(i)
'            If nIdx <> CLC_NONE Then
'                .ColDataType(i + 1) = m_gdCrd.Col(nIdx).DataType
'                .ColFormat(i + 1) = m_gdCrd.Col(nIdx).Format
'                .ColKey(i + 1) = nIdx
'                .TextMatrix(0, i + 1) = m_gdCrd.Col(nIdx).Caption
'                .ColWidth(i + 1) = IIf(m_gdCrd.Col(nIdx).Width > 0, m_gdCrd.Col(nIdx).Width, -1)
'            Else
'                Exit For
'            End If
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'        geCrd.AdjustSortImages
'
'        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatCrdColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
'    Dim aRowData As clsOvRowData
'
'    With fgCrd
'        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'        aFont.Name = m_gdCrd.Font.Name
'        aFont.Size = m_gdCrd.Font.Size
'        aFont.Bold = m_gdCrd.Font.Bold
'        aFont.Italic = m_gdCrd.Font.Italic
'        aFont.Strikethrough = m_gdCrd.Font.Strikethrough
'        aFont.Underline = m_gdCrd.Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = 0 To nCols
'            nIdx = .ColKey(i)
'            If nIdx <> CLC_NONE Then
'                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdCrd.Col(nIdx).BackColor <> 0, m_gdCrd.Col(nIdx).BackColor, RGB(1, 1, 1))
'                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdCrd.Col(nIdx).ForeColor <> 0, m_gdCrd.Col(nIdx).ForeColor, RGB(1, 1, 1))
'
'                .ColAlignment(i) = IIf(nIdx = CLC_BS Or nIdx = CLC_STATUS _
'                                        Or nIdx = CLC_OPT_TYPE, _
'                                        flexAlignCenterCenter, flexAlignGeneral)
'            End If
'        Next
'
'        If m_gdCrd.HorizLinesVisible Then
'            If m_gdCrd.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdCrd.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdCrd.GridLinesColor
'
'        .BackColorSel = m_gdCrd.BackColorSel
'        .ForeColorSel = m_gdCrd.ForeColorSel
'
'        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatLogGrid()
'    On Error Resume Next
'    Dim i&, nCols&, nIdx&
'
'    With fgLog
'        m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'
'        nCols = CEC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = 0 To CEC_LAST_COLUMN
'            nIdx = m_gdLog.Idx(i)
'            If nIdx <> CEC_NONE Then
'                .TextMatrix(0, i) = m_gdLog.Col(i).Caption
'                .ColFormat(i) = m_gdLog.Col(i).Format
'                .ColDataType(i) = m_gdLog.Col(i).DataType
'                .ColKey(i) = nIdx
'
'                If nIdx = CEC_NUM Then
'                    .ColAlignment(i) = flexAlignCenterCenter
'                End If
'            Else
'                Exit For
'            End If
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatLogColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont, nCol&, nBaseHeight!, nMsgRows&
'
'    With fgLog
'        m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'
'        aFont.Name = m_gdLog.Font.Name
'        aFont.Size = m_gdLog.Font.Size
'        aFont.Bold = m_gdLog.Font.Bold
'        aFont.Italic = m_gdLog.Font.Italic
'        aFont.Strikethrough = m_gdLog.Font.Strikethrough
'        aFont.Underline = m_gdLog.Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = 0 To nCols
'            nIdx = .ColKey(i)
'            If nIdx <> CEC_NONE Then
'                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdLog.Col(nIdx).BackColor <> 0, m_gdLog.Col(nIdx).BackColor, RGB(1, 1, 1))
'                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdLog.Col(nIdx).ForeColor <> 0, m_gdLog.Col(nIdx).ForeColor, RGB(1, 1, 1))
'            End If
'        Next
'
'        If m_gdLog.HorizLinesVisible Then
'            If m_gdLog.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdLog.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdLog.GridLinesColor
'
'        .BackColorSel = m_gdLog.BackColorSel
'        .ForeColorSel = m_gdLog.ForeColorSel
'
'        ' adjust heights for multiline rows
'        nCol = .ColIndex(CEC_MESSAGE)
'        If nCol >= 0 And .Rows > 1 Then
'            nBaseHeight = .RowHeight(0)
'
'            For i = 1 To .Rows - 1
'                nMsgRows = InStrCount(.TextMatrix(i, nCol), vbCrLf)
'                If nMsgRows > 0 Then
'                    .RowHeight(i) = nBaseHeight * (nMsgRows + 1)
'                End If
'            Next
'        End If
'
'        m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'    End With
'End Sub
'
'Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim sValue$, nValue&
'
'    With fgFlt
'        sValue = Trim$(.TextMatrix(Row, Col))
'        If m_sCurrentOriginalText <> sValue Then
'            nValue = CLng(sValue)
'            If Err.Number <> 0 Then nValue = Val(sValue)
'            If Err.Number = 0 Then
'                m_nFilter(Col) = nValue
'
'                AdjustCaption
'                CardsShow False
'
'                .AutoSize 0, .Cols - 1, , 100
'            Else
'                FilterUpdate
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgFlt_DblClick()
'    On Error Resume Next
'
'    With fgFlt
'        m_enMenuGrid = GT_TNTCARDS_FILTER
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick
'    End With
'End Sub
'
'Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
'        m_bKeyDown(GT_TNTCARDS_FILTER) = True
'    End If
'End Sub
'
'Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_TNTCARDS_FILTER) Then
'        m_bKeyDown(GT_TNTCARDS_FILTER) = False
'        With fgFlt
'            m_enMenuGrid = GT_TNTCARDS_FILTER
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case KeyCode = vbKeyInsert And Shift = vbAltMask
'                mnuCtxOrderNewStock_Click
'
'            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
'                mnuCtxOrderNewOption_Click
'
'            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
'                mnuCtxTntCardNew_Click
'
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'        End Select
'    End If
'End Sub
'
'Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgFlt
'            m_enMenuGrid = GT_TNTCARDS_FILTER
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = True
'
'    If IsDblClickHandled Then Exit Sub
'
'    With fgFlt
'        If Not m_bInProc And Not m_bShutDown And Col >= CFC_FIRST_COLUMN And Col <= CFC_LAST_COLUMN Then
'            Cancel = Not m_gdFlt.Col(Col).CanEdit
'            If Not Cancel Then
'                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgLog_DblClick()
'    On Error Resume Next
'
'    With fgLog
'        m_enMenuGrid = GT_TNTCARDS_LIST
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick
'    End With
'End Sub
'
'Private Sub fgLog_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridCommonOrOrderCard(KeyCode, Shift) Then
'        m_bKeyDown(GT_TNTCARDS_LOG) = True
'    End If
'End Sub
'
'Private Sub fgLog_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_TNTCARDS_LOG) Then
'        m_bKeyDown(GT_TNTCARDS_LOG) = False
'        With fgLog
'            m_enMenuGrid = GT_TNTCARDS_LOG
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case KeyCode = vbKeyInsert And Shift = vbAltMask
'                mnuCtxOrderNewStock_Click
'
'            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
'                mnuCtxOrderNewOption_Click
'
'            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
'                mnuCtxTntCardNew_Click
'
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'            Case KeyCode = vbKeyG And Shift = vbCtrlMask
'                mnuCtxAutosizeGrid_Click
'
'            Case KeyCode = vbKeyA And Shift = vbCtrlMask
'                mnuCtxAutosizeCol_Click
'
'        End Select
'    End If
'End Sub
'
'Private Sub fgLog_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgLog
'            m_enMenuGrid = GT_TNTCARDS_LOG
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgCrd_AfterMoveColumn(ByVal Col As Long, Position As Long)
'    On Error Resume Next
'    Dim nIdx&, i&, nOldPos&, nNewPos&
'    If Col = Position Then Exit Sub
'
'    nOldPos = Col - CLC_FIRST_COLUMN
'    nNewPos = Position - CLC_FIRST_COLUMN
'    nIdx = m_gdCrd.Idx(nOldPos)
'    If nOldPos > nNewPos Then
'        For i = nOldPos To nNewPos Step -1
'            If i > nNewPos Then m_gdCrd.Idx(i) = m_gdCrd.Idx(i - 1)
'        Next
'    Else
'        For i = nOldPos To nNewPos
'            If i < nNewPos Then m_gdCrd.Idx(i) = m_gdCrd.Idx(i + 1)
'        Next
'    End If
'    m_gdCrd.Idx(nNewPos) = nIdx
'End Sub
'
'Private Sub fgCrd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim nIdx&
'
'    If Col >= 0 And Row < 0 Then
'        With fgCrd
'            nIdx = .ColKey(Col)
'            If nIdx >= CLC_FIRST_COLUMN And nIdx <= CLC_LAST_COLUMN Then
'                m_gdCrd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub fgCrd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
'    On Error Resume Next
'    Dim nOldIdx&, nNewIdx&
'
'    nOldIdx = fgCrd.ColKey(Col)
'    nNewIdx = fgCrd.ColKey(Position)
'
'    If nOldIdx = CLC_NONE Or nNewIdx = CLC_NONE Then
'        Position = Col
'    ElseIf Not m_gdCrd.Col(nOldIdx).CanChangeOrder Or Not m_gdCrd.Col(nNewIdx).CanChangeOrder Then
'        Position = Col
'    End If
'End Sub
'
'Private Sub fgCrd_DblClick()
'    On Error Resume Next
'    With fgCrd
'        m_enMenuGrid = GT_TNTCARDS_LIST
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick
'    End With
'End Sub
'
'Private Sub fgCrd_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridCommonOrOrderCard(KeyCode, Shift) _
'        Or Shift = 0 And (KeyCode = vbKeyReturn Or KeyCode = vbKeyDelete) _
'        Or Shift = vbCtrlMask And KeyCode = vbKeyDelete Then
'        m_bKeyDown(GT_TNTCARDS_LIST) = True
'    End If
'End Sub
'
'Private Sub fgCrd_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_TNTCARDS_LIST) Then
'        m_bKeyDown(GT_TNTCARDS_LIST) = False
'        With fgCrd
'            m_enMenuGrid = GT_TNTCARDS_LIST
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case KeyCode = vbKeyReturn And Shift = 0
'                mnuCtxTntCardAccept_Click
'
'            Case KeyCode = vbKeyDelete And Shift = 0
'                mnuCtxTntCardReject_Click
'
'            Case KeyCode = vbKeyDelete And Shift = vbCtrlMask
'                mnuCtxTntCardDelete_Click
'
'            Case KeyCode = vbKeyInsert And Shift = vbAltMask
'                mnuCtxOrderNewStock_Click
'
'            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
'                mnuCtxOrderNewOption_Click
'
'            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
'                mnuCtxTntCardNew_Click
'
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'            Case KeyCode = vbKeyG And Shift = vbCtrlMask
'                mnuCtxAutosizeGrid_Click
'
'            Case KeyCode = vbKeyA And Shift = vbCtrlMask
'                mnuCtxAutosizeCol_Click
'
'        End Select
'    End If
'End Sub
'
'Private Sub fgCrd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgCrd
'            m_enMenuGrid = GT_TNTCARDS_LIST
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'
'            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
'                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
'                And .SelectedRows <= 1 Then
'                .Row = m_nMenuGridRow
'            End If
'
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub ShowPopup()
'    On Error Resume Next
'    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
'    Dim aCrd As EtsMmGeneralLib.MmTntCardAtom
'
'    'mnuCtxTntCardNew        "New Message..."
'    'mnuCtxTntCardAccept     "Accept/ACK Message(s)..."
'    'mnuCtxTntCardReject     "Reject Message(s)..."
'    'mnuCtxTntCardDelete     "Delete Message(s)..."
'    'mnuCtxOrderNewStock     "New Stock Order..."
'    'mnuCtxOrderNewOption    "New Option Order..."
'    'mnuCtxDefCrdSort        "Default Cards Sorting"
'    'mnuCtxCopy              "Copy Grid"
'    'mnuCtxPrint             "Print Grid..."
'    'mnuCtxHideCol           "Hide Column"
'    'mnuCtxAutosizeCol       "Autosize Column"
'    'mnuCtxAutosizeGrid      "Autosize Grid"
'    'mnuCtxGridLayout        "Grid Layout..."
'    'mnuCtxClearLog          "Clear"
'
'    mnuCtxTntCardNew.Enabled = Not m_bInProc
'    mnuCtxCopy.Enabled = Not m_bInProc
'    mnuCtxDefCrdSort.Enabled = Not m_bInProc And fgCrd.ColIndex(CLC_DATE) >= 0
'    mnuCtxOrderNewStock.Enabled = Not m_bInProc
'    mnuCtxOrderNewOption.Enabled = Not m_bInProc
'    mnuCtxTntCardAccept.Enabled = False
'    mnuCtxTntCardReject.Enabled = False
'    mnuCtxTntCardDelete.Enabled = False
'
'    Select Case m_enMenuGrid
'        Case GT_TNTCARDS_FILTER
'            mnuCtxHideCol.Enabled = False
'            mnuCtxAutosizeCol.Enabled = False
'            mnuCtxAutosizeGrid.Enabled = False
'            mnuCtxClearLog.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTntCardAccept
'
'        Case GT_TNTCARDS_LIST
'            With fgCrd
'                If Not m_bInProc And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
'                    And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
'
'                    Dim i&, nRow&
'
'                    If .Rows > 1 Then
'                        If .SelectedRows > 1 Then
'                            mnuCtxTntCardAccept.Enabled = False
'
'                            For i = 0 To .SelectedRows - 1
'                                nRow = .SelectedRow(i)
'                                If Not .RowHidden(nRow) Then
'                                    Set aCrd = .RowData(nRow)
'
'                                    If Not aCrd Is Nothing Then
'                                        mnuCtxTntCardAccept.Enabled = True
'                                        Set aCrd = Nothing
'                                    Else
'                                        Exit For
'                                    End If
'                                End If
'                            Next
'
'                            mnuCtxTntCardReject.Enabled = mnuCtxTntCardAccept.Enabled
'                            mnuCtxTntCardDelete.Enabled = mnuCtxTntCardAccept.Enabled
'                        Else
'                            Set aCrd = .RowData(m_nMenuGridRow)
'                            mnuCtxTntCardAccept.Enabled = Not aCrd Is Nothing
'                            mnuCtxTntCardReject.Enabled = mnuCtxTntCardAccept.Enabled
'                            mnuCtxTntCardDelete.Enabled = mnuCtxTntCardAccept.Enabled
'                            Set aCrd = Nothing
'                        End If
'                    End If
'                End If
'
'                mnuCtxHideCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols And m_gdCrd.Idx(1) <> CLC_NONE)
'                mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
'                mnuCtxAutosizeGrid.Enabled = True
'
'                PopupMenu mnuCtx, , , , mnuCtxTntCardAccept
'            End With
'
'        Case GT_TNTCARDS_LOG
'            mnuCtxHideCol.Enabled = False
'            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeGrid.Enabled = True
'            mnuCtxClearLog.Enabled = True
'
'            PopupMenu mnuCtx, , , , mnuCtxTntCardAccept
'
'    End Select
'End Sub
'
'Private Sub ResetMenuData()
'    On Error Resume Next
'    m_enMenuGrid = -1
'    m_nMenuGridCol = -1
'    m_nMenuGridRow = -1
'    m_nMenuGridCols = 0
'    m_nMenuGridRows = 0
'End Sub
'
'Private Sub frmLayout_OnApply(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
'    On Error Resume Next
'    SetGridLayout enGridType, gdGrid
'End Sub
'
'Private Sub frmLayout_OnOK(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    SetGridLayout enGridType, gdGrid
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
'    On Error Resume Next
'
'    Select Case enGridType
'        Case GT_TNTCARDS_FILTER
'            gdGrid.CopyTo m_gdFlt
'
'            m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'            FormatFltGrid
'            FormatFltColumns
'            m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'
'        Case GT_TNTCARDS_LIST
'            gdGrid.CopyTo m_gdCrd
'
'            m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'            FormatCrdGrid
'            FormatCrdColumns
'            CardsUpdate True
'            m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'
'            AdjustState
'
'        Case GT_TNTCARDS_LOG
'            gdGrid.CopyTo m_gdLog
'
'            m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'            FormatLogGrid
'            FormatLogColumns
'            m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'
'    End Select
'
'    UserControl_Resize
'End Sub
'
'Private Sub mnuCtxAutosizeCol_Click()
'    On Error Resume Next
'    Dim nIdx&
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_TNTCARDS_LIST
'            With fgCrd
'                m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'
'                nIdx = .ColKey(m_nMenuGridCol)
'                If nIdx >= CLC_FIRST_COLUMN And nIdx <= CLC_LAST_COLUMN Then
'                    m_gdCrd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
'                End If
'
'                m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'            End With
'
'        Case GT_TNTCARDS_LOG
'            With fgLog
'                m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'                m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'            End With
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxAutosizeGrid_Click()
'    On Error Resume Next
'    Dim nIdx&, i&, nCount&
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_TNTCARDS_LIST
'            With fgCrd
'                m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'                .AutoSize 0, .Cols - 1
'
'                nCount = .Cols - 1
'                For i = 0 To nCount
'                    nIdx = .ColKey(i)
'                    If nIdx >= CLC_FIRST_COLUMN And nIdx <= CLC_LAST_COLUMN Then
'                        m_gdCrd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
'                    End If
'                Next
'
'                m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'            End With
'
'        Case GT_TNTCARDS_LOG
'            With fgLog
'                m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'                .AutoSize 0, .Cols - 1
'                m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'            End With
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxClearLog_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'
'    Select Case m_enMenuGrid
'        Case GT_TNTCARDS_LOG
'            ClearLog
'
'    End Select
'
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxCopy_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_TNTCARDS_FILTER
'            g_ClipMgr.CopyGridToClipboard fgFlt
'
'        Case GT_TNTCARDS_LIST
'            g_ClipMgr.CopyGridToClipboard fgCrd
'
'        Case GT_TNTCARDS_LOG
'            g_ClipMgr.CopyGridToClipboard fgLog
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxGridLayout_Click()
'    On Error Resume Next
'    Select Case m_enMenuGrid
'        Case GT_TNTCARDS_FILTER
'            CustomizeFltGridLayout
'
'        Case GT_TNTCARDS_LIST
'            CustomizeCrdGridLayout
'
'        Case GT_TNTCARDS_LOG
'            CustomizeLogGridLayout
'
'    End Select
'End Sub
'
'
'Private Sub mnuCtxDefCrdSort_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    SortCardsByDefault
'End Sub
'
'Public Sub SortCardsByDefault()
'    On Error Resume Next
'    Dim i&, nRow&, nCol&
'
'    With fgCrd
'        i = -1
'        i = .ColIndex(CLC_DATE)
'        If i >= 0 Then
'            m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'            Screen.MousePointer = vbHourglass
'
'            nRow = .Row
'            nCol = .Col
'            .Col = i
'            .Row = 1
'            .Sort = flexSortGenericDescending
'
'            .Row = nRow
'            .Col = nCol
'
'            geCrd.ShowSortImage i, -1
'
'            m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'            Screen.MousePointer = vbDefault
'        End If
'
'    End With
'
'End Sub
'
'Public Property Get CanSortCardsByDefault() As Boolean
'    On Error Resume Next
'    CanSortCardsByDefault = Not m_bInProc And fgCrd.ColIndex(CLC_DATE) >= 0
'End Property
'
'Private Sub CardsUpdate(ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim i&
'    Dim aCrd As EtsMmGeneralLib.MmTntCardAtom
'
'    With fgCrd
'        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'        For i = 1 To .Rows - 1
'            Set aCrd = .RowData(i)
'            If Not aCrd Is Nothing Then
'                CardUpdate i, aCrd, bUpdateColors
'                Set aCrd = Nothing
'            End If
'        Next
'
'        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Function GetFirstVisibleSelectedRow&()
'    On Error Resume Next
'    Dim i&, nRow&
'    GetFirstVisibleSelectedRow = 0
'    For i = 0 To fgCrd.SelectedRows - 1
'        nRow = fgCrd.SelectedRow(i)
'        If Not fgCrd.RowHidden(nRow) Then
'            GetFirstVisibleSelectedRow = nRow
'            Exit For
'        End If
'    Next
'End Function
'
'Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
'                                        ByRef aUnd As EtsGeneralLib.UndAtom, _
'                                        ByRef aOpt As EtsGeneralLib.EtsOptAtom, _
'                                        ByRef bBuy As Boolean, ByRef dPrice#, ByRef nQty&)
'    On Error Resume Next
'    Dim nRow&, aCrd As EtsMmGeneralLib.MmTntCardAtom
'
'    bBuy = True
'    dPrice = 0#
'    nQty = IIf(bIsStock, 100, 10)
'
'    If m_enMenuGrid = GT_TNTCARDS_LIST Then
'
'        With fgCrd
'            nRow = GetFirstVisibleSelectedRow()
'            If nRow > 0 Then
'                Set aCrd = .RowData(m_nMenuGridRow)
'                If Not aCrd Is Nothing Then
'                    Set aUnd = aCrd.Und
'
'                    If Not bIsStock And Not aCrd.Opt Is Nothing Then
'                        Set aOpt = New EtsGeneralLib.EtsOptAtom
'                        aOpt.ID = aCrd.Opt.ID
'                        aOpt.OptType = aCrd.Opt.OptType
'                        aOpt.RootID = aCrd.Opt.RootID
'                        aOpt.Symbol = aCrd.Opt.Symbol
'                        aOpt.Expiry = aCrd.Opt.Expiry
'                        aOpt.Strike = aCrd.Opt.Strike
'
'                        nQty = aCrd.Qty
'                        bBuy = aCrd.IsBuy
'                        dPrice = aCrd.Price
'                    End If
'
'                    Set aCrd = Nothing
'                End If
'            End If
'        End With
'    End If
'
'    If aUnd Is Nothing Then Set aUnd = g_Underlying(m_nFilter(CFC_UNDERLYING))
'
'    On Error Resume Next
'End Sub
'
'Private Sub OrderNew(ByVal bIsStock As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, nRow&, nQty&
'    Dim aOpt As EtsGeneralLib.EtsOptAtom
'
'    m_bInProc = True
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection bIsStock, aUnd, aOpt, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, True, nQty, dPrice
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, aOpt, True, nQty, dPrice
'    End If
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Trades Messages View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'End Sub
'
'Private Sub mnuCtxOrderNewOption_Click()
'    On Error Resume Next
'    OrderNew False
'End Sub
'
'Private Sub mnuCtxOrderNewStock_Click()
'    On Error Resume Next
'    OrderNew True
'End Sub
'
'Private Sub mnuCtxTntCardNew_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, nQty&
'    Dim aOpt As EtsGeneralLib.EtsOptAtom
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection False, aUnd, aOpt, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    frmTntCardEntry.NewCard m_frmOwner, aUnd, aOpt, bBuy, nQty, dPrice
'
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Trades Messages View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'End Sub
'
'Private Function IsSelectedCardsExists() As Boolean
'    On Error Resume Next
'    Dim nRow&, i&, aSelRow As clsFilterAtom, aCrd As EtsMmGeneralLib.MmTntCardAtom
'
'    IsSelectedCardsExists = False
'
'    If m_enMenuGrid = GT_TNTCARDS_LIST Then
'        With fgCrd
'            For i = 0 To .SelectedRows - 1
'                nRow = .SelectedRow(i)
'                If Not .RowHidden(nRow) Then
'                    Set aCrd = .RowData(nRow)
'                    If Not aCrd Is Nothing Then
'                        IsSelectedCardsExists = True
'                        Set aCrd = Nothing
'                        Exit Function
'                    End If
'                End If
'            Next
'        End With
'    End If
'End Function
'
'Private Function GetSuffixForInvalidAcceptAttempt(ByVal enStatus As EtsGeneralLib.EtsTntCardStatusEnum) As String
'    On Error Resume Next
'    Select Case enStatus
'        Case enTcsInRejected
'            GetSuffixForInvalidAcceptAttempt = "already rejected."
'
'        Case enTcsInAccepted
'            GetSuffixForInvalidAcceptAttempt = "already accepted."
'
'        Case enTcsInPendingAccept
'            GetSuffixForInvalidAcceptAttempt = "already waiting for accept."
'
'        Case enTcsOutPendingExecute, enTcsOutExecuted, enTcsOutFailed
'            GetSuffixForInvalidAcceptAttempt = "created by you."
'
'        Case Else
'            GetSuffixForInvalidAcceptAttempt = "invalid."
'
'    End Select
'End Function
'
'Private Sub mnuCtxTntCardAccept_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim i&, nRow&, aCrd As EtsMmGeneralLib.MmTntCardAtom
'    Dim sExpiredCardsDesc$, nExpCount&
'    Dim aSelRow As clsFilterAtom, aSellRows As New clsFilterAtomColl
'
'    If IsSelectedCardsExists() Then
'        If Not g_TntProcessor.CheckUsingTngMessagingWithWarning(m_frmOwner) Then Exit Sub
'
'        If gCmn.MyMsgBox(m_frmOwner, "Are you sure that you want to accept/ACK selected trades message(s)?", _
'                        vbYesNo + vbQuestion, "Accept/ACK Confirmation") = vbYes Then
'
'            With fgCrd
'                m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'                nExpCount = 0
'
'                For i = 0 To .SelectedRows - 1
'                    nRow = .SelectedRow(i)
'                    If Not .RowHidden(nRow) Then
'                        Set aCrd = .RowData(nRow)
'                        If Not aCrd Is Nothing Then
'                            If aCrd.Status = enTcsInNew Or aCrd.Status = enTcsInFailed Then
'                                If Not aCrd.IsExpired Then
'                                    aSellRows.Add(CStr(nRow)).ID = nRow
'                                    TntProcessor.CardAccept aCrd
'                                Else
'                                    nExpCount = nExpCount + 1
'                                    If nExpCount <= 10 Then
'                                        sExpiredCardsDesc = sExpiredCardsDesc & vbCrLf & aCrd.Desc
'
'                                    ElseIf nExpCount = 11 Then
'                                        sExpiredCardsDesc = sExpiredCardsDesc & vbCrLf & "(more...)"
'                                    End If
'                                End If
'                            Else
'                                TntProcessor.NewEvent enEvtWarning, _
'                                    "The trade message " & aCrd.Desc & " can't be accepted because it is " & _
'                                    GetSuffixForInvalidAcceptAttempt(aCrd.Status)
'                            End If
'                            Set aCrd = Nothing
'                        End If
'                    End If
'                Next
'
'                For Each aSelRow In aSellRows
'                    .IsSelected(aSelRow.ID) = False
'                Next
'                aSellRows.Clear
'
'                m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'
'                If nExpCount > 0 Then
'                    Dim nResult As VbMsgBoxResult
'
'                    nResult = gCmn.MyMsgBox(m_frmOwner, "WARNING:  90 seconds are already expired after the following trade messages were submitted:" & _
'                                        sExpiredCardsDesc & vbCrLf & "Are you sure that you want to accept these trade message(s)?", _
'                                        vbYesNoCancel + vbExclamation, "Accept/ACK Confirmation", "&Accept", "&Reject")
'
'                    If nResult <> vbCancel Then
'                        m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'                        For i = 0 To .SelectedRows - 1
'                            nRow = .SelectedRow(i)
'                            If Not .RowHidden(nRow) Then
'                                Set aCrd = .RowData(nRow)
'                                If Not aCrd Is Nothing Then
'                                    If aCrd.Status = enTcsInNew Or aCrd.Status = enTcsInFailed Then
'                                        aSellRows.Add(CStr(nRow)).ID = nRow
'                                        If nResult = vbYes Then
'                                            TntProcessor.CardAccept aCrd
'                                        Else
'                                            TntProcessor.CardReject aCrd
'                                        End If
'                                    Else
'                                        TntProcessor.NewEvent enEvtWarning, _
'                                            "The trade message " & aCrd.Desc & " can't be " & _
'                                            IIf(nResult = vbYes, "accepted", "rejected") & " because it is " & _
'                                            GetSuffixForInvalidAcceptAttempt(aCrd.Status)
'                                    End If
'                                    Set aCrd = Nothing
'                                End If
'                            End If
'                        Next
'
'                        For Each aSelRow In aSellRows
'                            .IsSelected(aSelRow.ID) = False
'                        Next
'                        aSellRows.Clear
'
'                        m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'                    End If
'                End If
'            End With
'        End If
'    End If
'
'    ResetMenuData
'End Sub
'
'Private Sub mnuCtxTntCardReject_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim nRow&, i&, aCrd As EtsMmGeneralLib.MmTntCardAtom
'    Dim aSelRow As clsFilterAtom, aSellRows As New clsFilterAtomColl
'
'    If IsSelectedCardsExists() Then
'        If gCmn.MyMsgBox(m_frmOwner, "Are you sure that you want to reject selected trades message(s)?", _
'                        vbYesNo + vbQuestion, "Reject Confirmation") = vbYes Then
'
'            With fgCrd
'                m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'                For i = 0 To .SelectedRows - 1
'                    nRow = .SelectedRow(i)
'                    If Not .RowHidden(nRow) Then
'                        Set aCrd = .RowData(nRow)
'                        If Not aCrd Is Nothing Then
'                            If aCrd.Status = enTcsInNew Or aCrd.Status = enTcsInFailed Then
'                                aSellRows.Add(CStr(nRow)).ID = nRow
'                                TntProcessor.CardReject aCrd
'                            Else
'                                TntProcessor.NewEvent enEvtWarning, _
'                                    "The trade message " & aCrd.Desc & " can't be rejected because it is " & _
'                                    GetSuffixForInvalidAcceptAttempt(aCrd.Status)
'                            End If
'                            Set aCrd = Nothing
'                        End If
'                    End If
'                Next
'
'                For Each aSelRow In aSellRows
'                    .IsSelected(aSelRow.ID) = False
'                Next
'                aSellRows.Clear
'
'                m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'            End With
'        End If
'    End If
'
'    ResetMenuData
'End Sub
'
'Private Sub mnuCtxTntCardDelete_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim nRow&, i&, aCrd As EtsMmGeneralLib.MmTntCardAtom, aCards As New EtsMmGeneralLib.MmTntCardColl
'
'    If IsSelectedCardsExists() Then
'        If gCmn.MyMsgBox(m_frmOwner, "Are you sure that you want to delete selected trades message(s)?", _
'                        vbYesNo + vbQuestion, "Delete Confirmation") = vbYes Then
'
'            With fgCrd
'                For i = 0 To .SelectedRows - 1
'                    nRow = .SelectedRow(i)
'                    If Not .RowHidden(nRow) Then
'                        Set aCrd = .RowData(nRow)
'                        If Not aCrd Is Nothing Then
'                            aCards.Add aCrd.TradeID, aCrd.TradeDate, aCrd
'                            Set aCrd = Nothing
'                        End If
'                    End If
'                Next
'            End With
'
'            For Each aCrd In aCards
'                TntProcessor.CardDelete aCrd
'            Next
'
'            aCards.Clear
'        End If
'    End If
'
'    ResetMenuData
'End Sub
'
'Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_TNTCARDS_FILTER
'            frmPrintPreview.Execute m_frmOwner, "Trades Messages Filter", "Trades Messages Filter", fgFlt
'
'        Case GT_TNTCARDS_LIST
'            frmPrintPreview.Execute m_frmOwner, "Trades Messages List", "Trades Messages List", fgCrd
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub tmrShow_Timer()
'    On Error Resume Next
'    tmrShow.Enabled = False
'    CardsShow True
'End Sub
'
'Private Sub TntProcessor_OnClearEvents()
'    On Error Resume Next
'    m_GridLock(GT_TNTCARDS_LOG).LockRedraw
'    fgLog.Rows = 1
'    m_GridLock(GT_TNTCARDS_LOG).UnlockRedraw
'End Sub
'
'Private Sub TntProcessor_OnNewEvent(aEvent As EtsGeneralLib.EtsEventAtom)
'    On Error Resume Next
'    CardsLogAddEvent aEvent, True
'End Sub
'
'Private Sub TntProcessor_OnNewCard(aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    Dim nRow&
'
'    With fgCrd
'        nRow = .FindRow(CStr(aCard.TradeID), 1, CLC_KEY, , True)
'        If nRow < 0 Then
'            m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'            .AddItem "", 1
'            nRow = 1
'
'            .RowData(nRow) = aCard
'            .TextMatrix(nRow, CLC_KEY) = aCard.TradeID
'
'            CardUpdate nRow, aCard, False
'
'            If CheckCardFilter(aCard) Then
'                .RowHidden(nRow) = False
'                m_nCards = m_nCards + 1
'                UpdateCardsCountLabel
'            Else
'                .RowHidden(nRow) = True
'            End If
'
'            FormatCrdColumns
'            CardsUpdateColors
'
'            m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'        End If
'    End With
'End Sub
'
'Private Sub TntProcessor_OnUpdateCard(aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    Dim nRow&
'
'    With fgCrd
'        nRow = .FindRow(CStr(aCard.TradeID), 1, CLC_KEY, , True)
'        If nRow > 0 Then
'            m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'            CardUpdate nRow, aCard, True
'
'            If CheckCardFilter(aCard) Then
'                If .RowHidden(nRow) Then
'                    .RowHidden(nRow) = False
'                    m_nCards = m_nCards + 1
'                End If
'            Else
'                If Not .RowHidden(nRow) Then
'                    .RowHidden(nRow) = True
'                    m_nCards = m_nCards - 1
'                End If
'            End If
'
'            UpdateCardsCountLabel
'
'            m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'        End If
'    End With
'End Sub
'
'Private Sub TntProcessor_OnDeleteCard(aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    Dim nRow&
'
'    With fgCrd
'        nRow = .FindRow(CStr(aCard.TradeID), 1, CLC_KEY, , True)
'        If nRow > 0 Then
'            m_GridLock(GT_TNTCARDS_LIST).LockRedraw
'
'            If Not .RowHidden(nRow) Then
'                m_nCards = m_nCards - 1
'                UpdateCardsCountLabel
'            End If
'
'            .RemoveItem nRow
'
'            m_GridLock(GT_TNTCARDS_LIST).UnlockRedraw
'        End If
'    End With
'End Sub
'
'Private Sub UserControl_Resize()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    Dim nSplitterTop&
'
'    With fgFlt
'        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'        .Width = ScaleWidth
'    End With
'
'    With fgCrd
'        .Top = fgFlt.Height + GRID_VERT_SPACE
'        .Width = ScaleWidth
'    End With
'
'    If m_bLogVisible Then
'        picLog.Width = ScaleWidth
'        fgLog.Width = ScaleWidth
'        lblLogCaption.Width = ScaleWidth
'
'        With ctlSplit
'            .SplitterMinTop = fgFlt.Height
'            .SplitterMaxHeight = ScaleHeight - .SplitterHeight
'            .SplitterTop = ScaleHeight - picLog.Height - ctlSplit.Height
'            .Width = ScaleWidth
'        End With
'    Else
'        With fgCrd
'            .Height = ScaleHeight - .Top
'        End With
'    End If
'End Sub
'
'Private Sub ctlSplit_SplitterMoved()
'    On Error Resume Next
'    If m_bLogVisible Then
'        Dim bMoveDown As Boolean
'        If picLog.Top < ctlSplit.Top + ctlSplit.Height Then ' move down
'            picLog.Top = ctlSplit.Top + ctlSplit.Height
'            fgCrd.Height = ctlSplit.Top - fgCrd.Top
'            picLog.Height = ScaleHeight - picLog.Top
'            fgLog.Height = ScaleHeight - picLog.Top - fgLog.Top
'        Else
'            picLog.Height = ScaleHeight - ctlSplit.Top - ctlSplit.Height
'            fgLog.Height = ScaleHeight - ctlSplit.Top - ctlSplit.Height - fgLog.Top
'            fgCrd.Height = ctlSplit.Top - fgCrd.Top
'            picLog.Top = ctlSplit.Top + ctlSplit.Height
'        End If
'    End If
'End Sub
'
'Private Sub UserControl_Terminate()
'    On Error Resume Next
'    Term
'End Sub
'
'Public Sub Term()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    m_bShutDown = True
'
'    Set pbProgress = Nothing
'    Set lblProcess = Nothing
'    Set lblStatus = Nothing
'    Set geCrd = Nothing
'
'    Set frmLayout = Nothing
'    Set TntProcessor = Nothing
'End Sub
'
'Private Sub AdjustCaption()
'    On Error Resume Next
'    RaiseEvent OnSetCaption
'End Sub
'
'Public Function GetCaption() As String
'    On Error Resume Next
'    Dim sCaption$
'
'    sCaption = "Trades Messages - " & fgFlt.Cell(flexcpTextDisplay, 1, CFC_STATUS)
'
'    GetCaption = sCaption
'End Function
'
'Public Function GetShortCaption() As String
'    On Error Resume Next
'    GetShortCaption = fgFlt.Cell(flexcpTextDisplay, 1, CFC_STATUS)
'End Function
'
'Public Property Get InProc() As Boolean
'    InProc = m_bInProc
'End Property
'
'Public Sub CustomizeFltGridLayout()
'    On Error Resume Next
'    If Not frmLayout Is Nothing Then
'        frmLayout.Execute GT_TNTCARDS_FILTER, CFC_FIRST_COLUMN, CFC_LAST_COLUMN, m_gdFlt, m_frmOwner
'    End If
'End Sub
'
'Public Sub CustomizeCrdGridLayout()
'    On Error Resume Next
'    If Not frmLayout Is Nothing Then
'        frmLayout.Execute GT_TNTCARDS_LIST, CLC_FIRST_COLUMN, CLC_LAST_COLUMN, m_gdCrd, m_frmOwner
'    End If
'End Sub
'
'Public Sub CustomizeLogGridLayout()
'    On Error Resume Next
'    If Not frmLayout Is Nothing Then
'        frmLayout.Execute GT_TNTCARDS_LOG, CEC_FIRST_COLUMN, CEC_LAST_COLUMN, m_gdLog, m_frmOwner
'    End If
'End Sub
'
'Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    For i = CFC_FIRST_COLUMN To CFC_LAST_COLUMN
'        aStorage.SetLongValue "TntCardFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
'    Next
'
'    m_gdFlt.WriteToStorage "TntCardFltGrid" & sKey, aStorage, False
'    m_gdCrd.WriteToStorage "TntCardOrdGrid" & sKey, aStorage
'    m_gdLog.WriteToStorage "TntCardLogGrid" & sKey, aStorage
'
'    aStorage.SetLongValue "TntCard" & sKey, "LogVisible", IIf(m_bLogVisible, 1, 0)
'    aStorage.SetLongValue "TntCard" & sKey, "SplitterTop", ctlSplit.SplitterTop
'    Exit Sub
'EH:
'    gCmn.ErrorHandler ""
'End Sub
'
'Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    For i = CFC_FIRST_COLUMN To CFC_LAST_COLUMN
'        m_nFilter(i) = aStorage.GetLongValue("TntCardFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
'    Next
'
'    If m_nFilter(CFC_STATUS) < 0 Or m_nFilter(CFC_STATUS) > 3 Then m_nFilter(CFC_STATUS) = 0
'
'    m_gdFlt.ReadFromStorage "TntCardFltGrid" & sKey, aStorage, False
'    m_gdCrd.ReadFromStorage "TntCardOrdGrid" & sKey, aStorage
'    m_gdLog.ReadFromStorage "TntCardLogGrid" & sKey, aStorage
'
'    FilterUpdate
'    FormatFltGrid
'    FormatFltColumns
'
'    fgCrd.Rows = 1
'    FormatCrdGrid
'    FormatCrdColumns
'
'    FormatLogGrid
'    FormatLogColumns
'
'    ctlSplit.SplitterTop = aStorage.GetLongValue("TntCard" & sKey, "SplitterTop", ctlSplit.SplitterTop)
'
'    m_bLogVisible = (aStorage.GetLongValue("TntCard" & sKey, "LogVisible", 1) <> 0)
'    AdjustLogVisible
'    AdjustState
'
'    If Not m_bLogVisible Then UserControl_Resize
'
'    tmrShow.Enabled = True
'    Exit Sub
'EH:
'    gCmn.ErrorHandler ""
'End Sub
'
'Public Function CurUnderlyingID() As Long
'    On Error Resume Next
'    Dim aCrd As EtsMmGeneralLib.MmTntCardAtom
'
'    CurUnderlyingID = 0
'    With fgCrd
'        If .Row > 0 Then
'            If Not .RowHidden(.Row) Then
'                Set aCrd = .RowData(.Row)
'                If Not aCrd Is Nothing Then
'                    CurUnderlyingID = aCrd.Und.ID
'                    Set aCrd = Nothing
'                End If
'            End If
'        End If
'    End With
'
'    If CurUnderlyingID = 0 Then
'        If m_nFilter(CFC_UNDERLYING) <> 0 Then
'            CurUnderlyingID = m_nFilter(CFC_UNDERLYING)
'        End If
'    End If
'End Function
'
'Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
'    On Error Resume Next
'    Dim aUnd As EtsGeneralLib.UndAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(CFC_UNDERLYING) <> nNewUndID Then
'        Set aUnd = g_Underlying(nNewUndID)
'        If Not aUnd Is Nothing Then
'            If aUnd.IsTraderContract Then
'                With fgFlt
'                    m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'
'                    sValue = "0"
'                    nValue = 0
'
'                    sComboList = g_Params.UnderlyingComboList
'                    If Len(sComboList) > 0 Then
'                        sComboList = "#0;<All>|" & sComboList
'                    Else
'                        sComboList = "#0;<All>"
'                    End If
'
'                    Set aUnd = g_Underlying(m_nFilter(CFC_UNDERLYING))
'                    If Not aUnd Is Nothing Then
'                        nValue = aUnd.ID
'                        sValue = Trim$(Str$(aUnd.ID))
'                        Set aUnd = Nothing
'                    End If
'                    .ColComboList(CFC_UNDERLYING) = sComboList
'                    .TextMatrix(1, CFC_UNDERLYING) = sValue
'                    m_nFilter(CFC_UNDERLYING) = nValue
'
'                    m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'                End With
'            End If
'        End If
'    End If
'End Sub
'
'Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
'    On Error Resume Next
'    Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(CFC_GROUP) <> nNewUndGroupID Then
'        Set aUndGroup = g_UnderlyingGroup(nNewUndGroupID)
'        If Not aUndGroup Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'
'                sValue = "0"
'                nValue = 0
'                sComboList = "#0;<All>"
'                For Each aUndGroup In g_UnderlyingGroup
'                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
'                    If aUndGroup.ID = m_nFilter(CFC_GROUP) Then
'                        nValue = aUndGroup.ID
'                        sValue = Trim$(Str$(aUndGroup.ID))
'                    End If
'                Next
'                .ColComboList(CFC_GROUP) = sComboList
'                .TextMatrix(1, CFC_GROUP) = sValue
'                m_nFilter(CFC_GROUP) = nValue
'
'                m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'            End With
'        End If
'    End If
'End Sub
'
'Public Sub NewTntCntPtyAdded(ByVal nNewCntPtyID As Long)
'    On Error Resume Next
'    Dim aCntPty As EtsMmGeneralLib.MmTntCounterPartyAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(CFC_CNT_PTY) <> nNewCntPtyID Then
'        Set aCntPty = g_TntCntPty(nNewCntPtyID)
'        If Not aCntPty Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_TNTCARDS_FILTER).LockRedraw
'
'                sValue = "0"
'                nValue = 0
'                sComboList = "#0;<All>"
'                For Each aCntPty In g_TntCntPty
'                    sComboList = sComboList & "|#" & Trim$(Str$(aCntPty.ID)) & ";" & aCntPty.Name
'                    If aCntPty.ID = m_nFilter(CFC_CNT_PTY) Then
'                        nValue = aCntPty.ID
'                        sValue = Trim$(Str$(nValue))
'                    End If
'                Next
'                .ColComboList(CFC_CNT_PTY) = sComboList
'                .TextMatrix(1, CFC_CNT_PTY) = sValue
'                m_nFilter(CFC_CNT_PTY) = nValue
'
'                m_GridLock(GT_TNTCARDS_FILTER).UnlockRedraw
'            End With
'        End If
'    End If
'End Sub
'
'Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, "Trade Messages", "Trade Messages", fgFlt, fgCrd
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Sub ClearLog()
'    On Error Resume Next
'    TntProcessor.ClearEvents
'End Sub
'
'Public Property Get LogVisible() As Boolean
'    On Error Resume Next
'    LogVisible = m_bLogVisible
'End Property
'
'Public Property Let LogVisible(ByVal bVisible As Boolean)
'    On Error Resume Next
'    m_bLogVisible = bVisible
'    AdjustLogVisible
'    UserControl_Resize
'    AdjustState
'End Property
'
'Private Sub AdjustLogVisible()
'    On Error Resume Next
'    ctlSplit.Visible = m_bLogVisible
'    picLog.Visible = m_bLogVisible
'End Sub
'
'Private Sub AdjustState()
'    On Error Resume Next
'    RaiseEvent OnStateChange
'End Sub
'
'Private Sub HandleGridDblClick()
'    On Error Resume Next
'    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
'        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
'
'        If IsAltPressed Then
'            Select Case True
'                Case IsCtrlPressed
'                    mnuCtxOrderNewOption_Click
'
'                Case IsShiftPressed
'                    mnuCtxTntCardNew_Click
'
'                Case Else
'                    mnuCtxOrderNewStock_Click
'            End Select
'
'            FixDblClickHandled
'        End If
'    End If
'End Sub
'
'
