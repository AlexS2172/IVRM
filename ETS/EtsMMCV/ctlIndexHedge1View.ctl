VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlIndexHedge1View 
   ClientHeight    =   6990
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   10710
   KeyPreview      =   -1  'True
   ScaleHeight     =   6990
   ScaleWidth      =   10710
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   4560
      Top             =   4080
   End
   Begin VSFlex7Ctl.VSFlexGrid fgUnd 
      Height          =   2055
      Left            =   0
      TabIndex        =   2
      Top             =   1440
      Width           =   7095
      _cx             =   12515
      _cy             =   3625
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
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
      ScrollTrack     =   -1  'True
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
         Name            =   "MS Sans Serif"
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
   Begin VSFlex7Ctl.VSFlexGrid fgTot 
      Height          =   495
      Left            =   0
      TabIndex        =   1
      Top             =   600
      Width           =   7575
      _cx             =   13361
      _cy             =   873
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
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
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxPositionTransfer 
         Caption         =   "Position Transfer..."
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxRefresh 
         Caption         =   "Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuCtxCopy 
         Caption         =   "Copy Grid"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuCtxPrint 
         Caption         =   "Print Grid..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxAutosizeCol 
         Caption         =   "Autosize Column"
      End
      Begin VB.Menu mnuCtxAutosizeGrid 
         Caption         =   "Autosize Grid"
      End
      Begin VB.Menu mnuCtxSeparator8 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlIndexHedge1View"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public m_frmOwner As Form
'
'Public Event OnSetCaption()
'Public Event OnStateChange()
'
'Private m_gdFlt As New clsGridDef
'Private m_gdTot As New clsGridDef
'Private m_gdUnd As New clsGridDef
'
'Private WithEvents geUnd As clsGridEx
'
'Private WithEvents aParams As clsParams
'Private WithEvents PriceProvider As PRICEPROVIDERSLib.PriceInfo
'Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
'Private VolaSource As VolatilitySourcesLib.VolatilitySource
'Private TradeChannel As clsTradeChannel
'
'Private WithEvents frmLayout As frmGridLayout
'
'Public pbProgress As MSComctlLib.ProgressBar
'Public lblProcess As VB.Label
'Public lblStatus As VB.Label
'Public WithEvents imgStop As VB.Image
'Public imgStopDis As VB.Image
'
'Private m_bInProc As Boolean
'Private m_bDataLoad As Boolean
'Private m_bLastQuoteReqNow As Boolean
'Private m_bRecalc As Boolean
'
'Private m_QuoteRequests As New clsIhRequestColl
'Private m_nQuoteReqCount As Long
'Private m_nQuoteReqDone As Long
'Private m_nQuoteGroupReqCount As Long
'Private m_nQuoteGroupReqDone As Long
'
'Private m_sCurrentOriginalText As String
'
'Private m_enMenuGrid As GridTypeEnum
'Private m_nMenuGridCol As Long
'Private m_nMenuGridRow As Long
'Private m_nMenuGridCols As Long
'Private m_nMenuGridRows As Long
'
'Private m_nFilter(IFC_TRADER To IFC_INDEX) As Long
'Private m_nFilterPrev(IFC_TRADER To IFC_INDEX) As Long
'
'Private m_bKeyDown(GT_INDEXHEDGE1_FILTER To GT_INDEXHEDGE1_UNDERLYINGS) As Boolean
'Private m_GridLock(GT_INDEXHEDGE1_FILTER To GT_INDEXHEDGE1_UNDERLYINGS) As New clsGridLock
'
'Private m_bGroupRequest As Boolean
'Private m_bGroupRequestSupported As Boolean
'Private m_bShutDown As Boolean
'
'Private m_Idx As EtsMmIndexHedgeLib.MmIhIndexAtom
'
'Private m_dSlice#
'Private m_nLotSize&
'Private m_nMaxSize&
'Private m_dExposure#
'Private m_nContracts&
'Private m_dDelta#
'
'Public Function Init() As Boolean
'    On Error GoTo EH
'
'    m_dSlice = g_Params.IndexHedgeSlice
'    m_nLotSize = g_Params.IndexHedgeLotSize
'    m_nMaxSize = g_Params.IndexHedgeMaxSize
'
'    m_dExposure = BAD_DOUBLE_VALUE
'    m_nContracts = 0&
'    m_dDelta = 0#
'
'    m_bShutDown = False
'
'    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
'    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
'    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
'
'    ResetMenuData
'    m_sCurrentOriginalText = ""
'    m_bKeyDown(GT_INDEXHEDGE1_FILTER) = False
'    m_bKeyDown(GT_INDEXHEDGE1_TOTALS) = False
'    m_bKeyDown(GT_INDEXHEDGE1_UNDERLYINGS) = False
'
'    m_GridLock(GT_INDEXHEDGE1_FILTER).Init fgFlt
'    m_GridLock(GT_INDEXHEDGE1_TOTALS).Init fgTot
'    m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).Init fgUnd
'
'    InitColumns
'    InitGrids
'
'    Set geUnd = New clsGridEx
'    geUnd.Init fgUnd
'
'    FormatFltGrid
'    FormatFltColumns
'
'    FormatTotGrid
'    FormatTotColumns
'
'    FormatUndGrid
'    FormatUndColumns
'
'    InitFltData
'
'    Set VolaSource = g_VolaSource
'    Set TradeChannel = g_TradeChannel
'
'    Dim aPT As PRICEPROVIDERSLib.IProvider
'    Set PriceProvider = New PRICEPROVIDERSLib.PriceInfo
'
'    Set aPT = PriceProvider
'    aPT.Type = g_Params.PriceProviderType
'    Set aPT = Nothing
'
'    PriceProvider.Connect
'
'    m_bGroupRequest = False
'
'    m_bGroupRequestSupported = g_Params.PriceProviderIsGroupRequestSupported
'    If m_bGroupRequestSupported Then
'        Set m_GroupPriceProvider = PriceProvider
'        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
'    Else
'        Set m_GroupPriceProvider = Nothing
'    End If
'
'    fgFlt.Col = IFC_TRADER
'    fgFlt.Row = 1
'
'    Set frmLayout = New frmGridLayout
'    Set aParams = g_Params
'
'    Init = True
'
'    UpdateTotals
'    AdjustCaption
'    AdjustState
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize index hedge."
'End Function
'
'Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
'    On Error Resume Next
'    Dim i&
'
'    Select Case nType
'        Case TYPE_TRADER
'            m_nFilter(IFC_TRADER) = nValue
'        Case TYPE_STRATEGY
'            m_nFilter(IFC_STRATEGY) = nValue
'        Case TYPE_UNDERLYING
'            m_nFilter(IFC_INDEX) = nValue
'    End Select
'
'    For i = IFC_TRADER To IFC_LAST_COLUMN
'        m_nFilterPrev(i) = 0
'    Next
'
'    FilterUpdate
'    FilterUpdateChangedStatus
'    AdjustState
'End Sub
'
'Private Sub fgUnd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim nIdx&
'
'    If Col >= 0 And Row < 0 Then
'        With fgUnd
'            nIdx = .ColKey(Col)
'            If nIdx > IUC_KEY And nIdx <= IUC_LAST_COLUMN Then
'                m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub fgUnd_DblClick()
'    On Error Resume Next
'    With fgUnd
'        m_enMenuGrid = GT_INDEXHEDGE1_UNDERLYINGS
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick True
'    End With
'End Sub
'
'Private Sub fgUnd_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridCommonOrTrade(KeyCode, Shift) Then
'        m_bKeyDown(GT_INDEXHEDGE1_UNDERLYINGS) = True
'    End If
'End Sub
'
'Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_INDEXHEDGE1_UNDERLYINGS) Then
'        m_bKeyDown(GT_INDEXHEDGE1_UNDERLYINGS) = False
'        With fgUnd
'            m_enMenuGrid = GT_INDEXHEDGE1_UNDERLYINGS
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'            Case KeyCode = vbKeyInsert And Shift = 0
'                mnuCtxTradeNew_Click
'
'            Case KeyCode = vbKeyG And Shift = vbCtrlMask
'                mnuCtxAutosizeGrid_Click
'
'            Case KeyCode = vbKeyA And Shift = vbCtrlMask And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
'                mnuCtxAutosizeCol_Click
'        End Select
'    End If
'End Sub
'
'Private Sub fgUnd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgUnd
'            m_enMenuGrid = GT_INDEXHEDGE1_UNDERLYINGS
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgTot_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    If m_bShutDown Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc Then Exit Sub
'    Dim sValue$, dValue#, nValue&, bRecalcTotals As Boolean: bRecalcTotals = False
'
'    With fgTot
'        sValue = Trim$(.TextMatrix(Row, Col))
'        If m_sCurrentOriginalText <> sValue Then
'            Select Case Col
'                Case ITC_SLICE
'                    dValue = Abs(ReadDbl(sValue)) / 100#
'                    If dValue > 0.01 And dValue <= 1# And m_dSlice <> dValue Then
'                        m_dSlice = dValue
'                        bRecalcTotals = True
'                        g_Params.IndexHedgeSlice = m_dSlice
'                    End If
'                    .TextMatrix(Row, Col) = m_dSlice * 100#
'
'                Case ITC_LOT_SIZE
'                    nValue = Abs(ReadLng(sValue))
'                    If nValue > 0# And m_nLotSize <> nValue Then
'                        m_nLotSize = nValue
'                        bRecalcTotals = True
'                        g_Params.IndexHedgeLotSize = m_nLotSize
'                    End If
'                    .TextMatrix(Row, Col) = m_nLotSize
'
'                Case ITC_MAX_SIZE
'                    nValue = Abs(ReadLng(sValue))
'                    If nValue > 0# And m_nMaxSize <> nValue Then
'                        m_nMaxSize = nValue
'                        bRecalcTotals = True
'                        g_Params.IndexHedgeMaxSize = m_nMaxSize
'                    End If
'                    .TextMatrix(Row, Col) = m_nMaxSize
'
'                Case ITC_CONTRACTS
'                    nValue = ReadLng(sValue)
'                    If m_nContracts <> nValue Then
'                        m_nContracts = nValue
'                        bRecalcTotals = True
'                    End If
'                    .TextMatrix(Row, Col) = IIf(m_nContracts <> 0&, m_nContracts, "")
'
'                Case ITC_DELTA
'                    dValue = ReadDbl(sValue) / 100#
'                    If m_dDelta <> dValue Then
'                        m_dDelta = dValue
'                        bRecalcTotals = True
'                    End If
'                    .TextMatrix(Row, Col) = IIf(m_dDelta <> 0#, m_dDelta * 100#, 0#)
'
'            End Select
'
'            If bRecalcTotals Then
'                CalcTotals False
'                UnderlyingsUpdate False
'                UpdateTotals
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgTot_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = (Col >= ITC_NET_DELTA) Or IsDblClickHandled Or m_bShutDown Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
'    If Not Cancel Then m_sCurrentOriginalText = Trim$(fgFlt.TextMatrix(Row, Col))
'End Sub
'
'Private Sub fgTot_DblClick()
'    On Error Resume Next
'    With fgTot
'        m_enMenuGrid = GT_INDEXHEDGE1_TOTALS
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick False
'    End With
'End Sub
'
'Private Sub fgTot_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridClip(KeyCode, Shift) Then
'        m_bKeyDown(GT_INDEXHEDGE1_TOTALS) = True
'    End If
'End Sub
'
'Private Sub fgTot_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_INDEXHEDGE1_TOTALS) Then
'        m_bKeyDown(GT_INDEXHEDGE1_TOTALS) = False
'        With fgTot
'            m_enMenuGrid = GT_INDEXHEDGE1_TOTALS
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'        End Select
'    End If
'End Sub
'
'Private Sub fgTot_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgTot
'            m_enMenuGrid = GT_INDEXHEDGE1_TOTALS
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub mnuCtxAutosizeCol_Click()
'    On Error Resume Next
'    Dim nIdx&
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE1_UNDERLYINGS
'            With fgUnd
'                m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'
'                nIdx = .ColKey(m_nMenuGridCol)
'                If nIdx > IUC_KEY And nIdx <= IUC_LAST_COLUMN Then
'                    m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
'                End If
'
'                m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).UnlockRedraw
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
'        Case GT_INDEXHEDGE1_UNDERLYINGS
'            With fgUnd
'                m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).LockRedraw
'                .AutoSize 0, .Cols - 1
'
'                nCount = .Cols - 1
'                For i = 0 To nCount
'                    nIdx = .ColKey(i)
'                    If nIdx > IUC_KEY And nIdx <= IUC_LAST_COLUMN Then
'                        m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
'                    End If
'                Next
'
'                m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).UnlockRedraw
'            End With
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxPositionTransfer_Click()
'    On Error Resume Next
'    If m_bShutDown Or m_bInProc Then Exit Sub
'    Dim nUndID&, nTrdID&
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim sContractDesc$, aGUnd As EtsGeneralLib.UndAtom
'
'    m_bInProc = True
'    nUndID = 0
'    nTrdID = m_nFilter(IFC_TRADER)
'
'    If m_enMenuGrid = GT_INDEXHEDGE1_UNDERLYINGS Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then _
'            nUndID = fgUnd.RowData(m_nMenuGridRow).ID
'
'    ElseIf m_enMenuGrid = GT_INDEXHEDGE1_FILTER Then
'        nUndID = m_nFilter(IFC_INDEX)
'    End If
'
'    m_bInProc = False
'    If nUndID <> 0 Then
'        Set aGUnd = g_Underlying(nUndID)
'        If Not aGUnd Is Nothing Then
'            sContractDesc = aGUnd.Symbol & " (" & aGUnd.ContractName & ")"
'            Set aGUnd = Nothing
'        End If
'
'        On Error GoTo EH
'        frmPositionTransfer.TransferPosition nUndID, nUndID, nTrdID, sContractDesc, m_frmOwner
'    End If
'    Exit Sub
'EH:
'    m_bInProc = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to transfer position."
'    ResetMenuData
'End Sub
'
'Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE1_FILTER
'            frmPrintPreview.Execute m_frmOwner, "Index Hedge Filter", "Index Hedge Filter", fgFlt
'
'        Case GT_INDEXHEDGE1_TOTALS
'            frmPrintPreview.Execute m_frmOwner, "Index Hedge Parameters/Totals", "Index Hedge Parameters/Totals", fgTot
'
'        Case GT_INDEXHEDGE1_UNDERLYINGS
'            frmPrintPreview.Execute m_frmOwner, "Index Hedge Stocks", "Index Hedge Stocks", fgUnd
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub tmrShow_Timer()
'    On Error Resume Next
'    tmrShow.Enabled = False
'
'    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc Then Exit Sub
'
'    ClearViewAndData
'    If Not IsValidFilter Then Exit Sub
'
'    SetRefreshHint False
'    CopyFilters True
'    FilterUpdateChangedStatus
'
'    If Not PriceProvider Is Nothing Then
'        If m_bLastQuoteReqNow Then PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'        m_bLastQuoteReqNow = False
'    End If
'
'    AdjustCaption
'
'    If UnderlyingsLoad Then
'        UpdateTotals
'        UnderlyingsUpdate True
'        AdjustCaption
'
'        lblProcess.Caption = "Last quotes request..."
'        lblProcess.Refresh
'
'        RequestLastQuotes
'    Else
'        ClearViewAndData
'        AdjustCaption
'        AdjustState
'
'        pbProgress.Visible = False
'        lblProcess.Visible = False
'        lblStatus.Visible = True
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'    End If
'End Sub
'
'Private Function UnderlyingsLoad() As Boolean
'    On Error GoTo EH
'    UnderlyingsLoad = False
'    If m_bInProc Then Exit Function
'    If Not IsValidFilter Then Exit Function
'
'    Dim aGIdx As EtsGeneralLib.IndexAtom
'    Dim aGUnd As EtsGeneralLib.UndAtom
'    Dim aIdxComp As EtsGeneralLib.IndexCompAtom
'    Dim aUndByTrd As clsTradeByUndAtom
'    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim aOpt As EtsMmIndexHedgeLib.MmIhOptAtom
'    Dim sKey$
'
'    m_bInProc = True
'    m_bDataLoad = True
'    AdjustState
'
'    If m_bDataLoad Then
'        Set aGIdx = g_BasketIndex(m_nFilter(IFC_INDEX))
'        Set aGUnd = g_Underlying(m_nFilter(IFC_INDEX))
'        If Not aGIdx Is Nothing And Not aGUnd Is Nothing Then
'            Set aUndByTrd = TradeChannel.TradesByUnd(CStr(aGIdx.ID))
'
'            lblStatus.Visible = False
'            imgStop.Visible = True
'            imgStopDis.Visible = False
'            pbProgress.Min = 0
'            pbProgress.Value = 0
'            pbProgress.Max = g_TradeChannel.Trades.Count
'            If Not aUndByTrd Is Nothing Then pbProgress.Max = pbProgress.Max + aUndByTrd.Trd.Count
'            pbProgress.Visible = True
'            lblProcess.Caption = "Data loading..."
'            lblProcess.Visible = True
'            lblProcess.Refresh
'
'            ' index
'            Set m_Idx = New EtsMmIndexHedgeLib.MmIhIndexAtom
'            m_Idx.ID = aGUnd.ID
'            m_Idx.Symbol = aGUnd.Symbol
'            m_Idx.Yield = aGUnd.Yield
'            m_Idx.IsHTB = aGUnd.IsHTB
'            m_Idx.IsAmerican = aGUnd.IsAmerican
'            m_Idx.Skew = aGUnd.Skew
'            m_Idx.Kurt = aGUnd.Kurt
'            Set m_Idx.BasketIndex = aGIdx
'
'            Set m_Idx.UndPriceProfile = aGUnd.UndPriceProfile
'            Set m_Idx.OptPriceProfile = aGUnd.OptPriceProfile
'
'            InitIndexVola
'
'            sKey = CStr(enCtIndex) & "_" & m_Idx.Symbol
'            If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, m_Idx, Nothing
'
'            ' underlyings
'            For Each aTrd In g_TradeChannel.Trades
'                If CheckUndTradeFilter(aTrd) Then
'                    Set aUnd = m_Idx.Und(aTrd.UndID)
'                    If aUnd Is Nothing Then
'                        Set aUnd = m_Idx.Und.Add(aTrd.UndID, aTrd.Und.Symbol)
'                        aUnd.ID = aTrd.Und.ID
'                        aUnd.Symbol = aTrd.Und.Symbol
'                        aUnd.TotalQtyInShares = aTrd.Und.TotalQtyInShares
'                        Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
'
'                        If aUnd.CurShares = BAD_LONG_VALUE Then aUnd.CurShares = 0
'                        aUnd.CurShares = aUnd.CurShares + IIf(aTrd.IsBuy, 1, -1) * aTrd.Quantity * aTrd.LotSize
'
'                        Set aIdxComp = m_Idx.BasketIndex.Components(aUnd.ID)
'                        If Not aIdxComp Is Nothing Then
'                            aUnd.Weight = aIdxComp.Weight
'                            Set aIdxComp = Nothing
'                        End If
'
'                        sKey = CStr(enCtStock) & "_" & aUnd.Symbol
'                        If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, aUnd, Nothing, Nothing
'                    End If
'                End If
'
'                DoEvents
'                If Not m_bDataLoad Then Exit For
'                IncProgress pbProgress
'            Next
'
'            ' index option trades
'            If Not aUndByTrd Is Nothing Then
'                For Each aTrd In aUndByTrd.Trd
'                    If CheckIdxTradeFilter(aTrd) Then
'                        Set aOpt = m_Idx.Opt(aTrd.ContractID)
'                        If aOpt Is Nothing Then
'                            Set aOpt = m_Idx.Opt.Add(aTrd.ContractID)
'                            aOpt.ID = aTrd.ContractID
'                            aOpt.Symbol = aTrd.Opt.Symbol
'                            aOpt.Expiry = aTrd.Opt.Expiry
'                            aOpt.OptType = aTrd.Opt.OptType
'                            aOpt.Strike = aTrd.Opt.Strike
'
'                            aOpt.Vola = m_Idx.VolaSrv.OptionVola(aOpt.Expiry, aOpt.Strike)
'
'                            sKey = CStr(enCtOption) & "_" & aOpt.Symbol
'                            If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, Nothing, aOpt
'                        End If
'
'                        If aOpt.Qty = BAD_LONG_VALUE Then aOpt.Qty = 0
'                        aOpt.Qty = aOpt.Qty + IIf(aTrd.IsBuy, 1, -1) * aTrd.Quantity * aTrd.LotSize
'                        Set aOpt = Nothing
'                    End If
'
'                    DoEvents
'                    If Not m_bDataLoad Then Exit For
'                    IncProgress pbProgress
'                Next
'            End If
'
'            AdjustIndexRates
'            UnderlyingsLoad = m_bDataLoad
'        End If
'    End If
'
'    m_bInProc = False
'EX:
'    On Error Resume Next
'    Set aOpt = Nothing
'    Set aIdxComp = Nothing
'    Set aGIdx = Nothing
'    Set aGUnd = Nothing
'    Set aUndByTrd = Nothing
'    m_bDataLoad = False
'    AdjustState
'    Exit Function
'EH:
'    m_bInProc = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load stocks."
'    GoTo EX
'End Function
'
'Private Sub UnderlyingsUpdate(ByVal bReload As Boolean)
'    On Error Resume Next
'    Dim i&, nRow&
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'
'    With fgUnd
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).LockRedraw
'
'        If bReload Then
'            .Rows = 1
'            If Not m_Idx Is Nothing Then
'                For Each aUnd In m_Idx.Und
'                    .AddItem ""
'                    nRow = .Rows - 1
'
'                    .RowData(nRow) = aUnd
'                    .TextMatrix(nRow, IUC_KEY) = 0
'                    UnderlyingUpdate nRow, True, aUnd
'                Next
'
'                If .Rows > 1 Then .Row = 1
'
'                .Col = IUC_SYMBOL
'                .Row = 1
'                .Sort = flexSortGenericAscending
'                geUnd.ShowSortImage IUC_SYMBOL, 1
'
'                FormatUndColumns
'            End If
'        Else
'            If Not m_Idx Is Nothing Then
'                For nRow = 1 To .Rows - 1
'                    Set aUnd = .RowData(nRow)
'                    UnderlyingUpdate nRow, False, aUnd
'                    Set aUnd = Nothing
'                Next
'            End If
'        End If
'
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub UnderlyingUpdate(ByVal nRow As Long, ByVal bSymbol As Boolean, ByRef aUnd As EtsMmIndexHedgeLib.MmIhUndAtom)
'    On Error Resume Next
'    Dim nCol&, sValue$
'
'    With fgUnd
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).LockRedraw
'
'        If aUnd Is Nothing Then Set aUnd = .RowData(nRow)
'
'        If Not aUnd Is Nothing Then
'            Dim i&, nIdx&
'            i = 0
'            nIdx = m_gdUnd.Idx(0)
'            While nIdx >= 0 And i <= IUC_LAST_COLUMN
'                nCol = i + 1
'                If bSymbol Then
'                    Select Case nIdx
'                        Case IUC_SYMBOL
'                            .TextMatrix(nRow, nCol) = aUnd.Symbol
'
'                        Case IUC_WEIGHT
'                            If aUnd.Weight > BAD_DOUBLE_VALUE Then
'                                .TextMatrix(nRow, nCol) = aUnd.Weight * 100#
'                            Else
'                                .TextMatrix(nRow, nCol) = STR_NA
'                            End If
'
'                        Case IUC_CUR_SHARES
'                            .TextMatrix(nRow, nCol) = IIf(aUnd.CurShares > BAD_LONG_VALUE, aUnd.CurShares, STR_NA)
'                    End Select
'                End If
'
'                Select Case nIdx
'                    Case IUC_PRICE
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceLast > BAD_DOUBLE_VALUE, aUnd.PriceLast, STR_NA)
'
'                    Case IUC_IDX_SHARES
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.IndexShares1 > BAD_LONG_VALUE, aUnd.IndexShares1, STR_NA)
'
'                    Case IUC_TXN_SHARES
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.IndexTxn > BAD_LONG_VALUE, aUnd.IndexTxn, STR_NA)
'                End Select
'
'                i = i + 1
'                nIdx = m_gdUnd.Idx(i)
'            Wend
'        End If
'
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub AdjustIndexRates()
'    On Error Resume Next
'    If m_Idx Is Nothing Then Exit Sub
'
'    Dim aOpt As EtsMmIndexHedgeLib.MmIhOptAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
'    Dim aUnd As EtsGeneralLib.UndAtom, bForceRateUpdate As Boolean
'
'    Set aUnd = g_UnderlyingAll(m_Idx.ID)
'    If aUnd Is Nothing Then Exit Sub
'
'    dPos = aUnd.UndPosForRates
'
'    If GetIrRuleType = enRateBasedOnPosition Then
'        cPosThreshold = Abs(GetPosThreshold)
'        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
'    Else
'        bUseMidRates = True
'    End If
'
'    For Each aOpt In m_Idx.Opt
'        If bUseMidRates Then
'            If Not m_Idx.IsHTB Then
'                aOpt.Rate = GetNeutralRate(Date, aOpt.Expiry)
'            Else
'                aOpt.Rate = GetNeutralHTBRate(Date, aOpt.Expiry)
'            End If
'        Else
'            If Not m_Idx.IsHTB Then
'                aOpt.Rate = IIf(dPos < 0#, GetShortRate(Date, aOpt.Expiry), GetLongRate(Date, aOpt.Expiry))
'            Else
'                aOpt.Rate = IIf(dPos < 0#, GetHTBRate(Date, aOpt.Expiry), GetLongRate(Date, aOpt.Expiry))
'            End If
'        End If
'    Next
'
'    Set aUnd = Nothing
'End Sub
'
'Private Sub RequestLastQuotes()
'    On Error GoTo EH
'    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aReq As clsIhRequestAtom
'    Dim nCount&
'
'    If Not PriceProvider Is Nothing Then
'        Debug.Assert Not m_bLastQuoteReqNow
'
'        nCount = m_QuoteRequests.Count
'        If nCount > 0 Then
'            m_bGroupRequest = m_bGroupRequestSupported And Not g_Params.RiskReqTypeAlwaysNonGroup
'
'            m_bInProc = True
'            m_bLastQuoteReqNow = True
'            AdjustState
'
'            m_nQuoteReqCount = nCount
'            m_nQuoteReqDone = 0&
'
'            pbProgress.Min = 0
'            pbProgress.Value = 0
'            pbProgress.Max = m_nQuoteReqCount
'            pbProgress.Visible = True
'
'            lblStatus.Visible = False
'            lblProcess.Visible = True
'            lblProcess.Caption = "Last quotes request..."
'            imgStop.Visible = True
'            imgStopDis.Visible = False
'            DoEvents
'
'            If Not m_bGroupRequest Then
'                m_nQuoteGroupReqCount = 0&
'                m_nQuoteGroupReqDone = 0&
'
'                aParam.Type = enMStart
'                PriceProvider.RequestLastQuote aParam
'
'                For Each aReq In m_QuoteRequests
'                    If Not m_bLastQuoteReqNow Then Exit For
'                    PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
'                    DoEvents
'                Next
'
'                aParam.Type = enMStop
'                PriceProvider.RequestLastQuote aParam
'            Else
'                m_nQuoteGroupReqCount = 0&
'                m_nQuoteGroupReqDone = 0&
'
'                For Each aReq In m_QuoteRequests
'                    If Not m_bLastQuoteReqNow Then Exit For
'
'                    aParam = aReq.GetQuoteUpdateParam
'                    If aParam.Type = enSTK Or aParam.Type = enIDX Then
'                        If aReq.Idx Is Nothing And aReq.Opt Is Nothing Then
'                            If Not m_bLastQuoteReqNow Then Exit For
'                            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                            PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
'                            DoEvents
'                        Else
'                            PriceProvider.RequestLastQuote aParam
'                            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                            DoEvents
'
'                            If Not m_bLastQuoteReqNow Then Exit For
'
'                            If aReq.Idx.Opt.Count > 0 Then
'                                aParam.Type = IIf(aParam.Type = enSTK, enGrSTK, enGrIDX)
'                                m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
'                                m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                                DoEvents
'                            End If
'                        End If
'                    End If
'                Next
'            End If
'
'            m_bInProc = False
'            AdjustState
'        Else
'            pbProgress.Visible = False
'            lblStatus.Visible = True
'            lblProcess.Visible = False
'            lblProcess.Caption = ""
'            imgStop.Visible = False
'            imgStopDis.Visible = False
'        End If
'    Else
'        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
'    End If
'    Exit Sub
'EH:
'    m_bLastQuoteReqNow = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
'    On Error Resume Next
'    pbProgress.Visible = False
'    lblStatus.Visible = True
'    lblProcess.Visible = False
'    lblProcess.Caption = ""
'    imgStop.Visible = False
'    imgStopDis.Visible = False
'    m_bInProc = False
'    AdjustState
'
'    PriceProvider.CancelLastQuote
'    If m_bGroupRequest Then
'        m_GroupPriceProvider.CancelGroup
'    End If
'End Sub
'
'Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
'    On Error Resume Next
'    Dim aReq As clsIhRequestAtom, bFinished As Boolean
'    Dim aRequest As PRICEPROVIDERSLib.QuoteUpdateParams, sKey$
'
'    aRequest = Request
'    bFinished = False
'
'    If Not Request Is Nothing And ReqType = enRequestLastQuote Then
'        If m_bLastQuoteReqNow Then
'            If PpIsNonGroupReqType(Request.Type) Then
'
'                sKey = CStr(IIf(aRequest.Type = enOPT, enCtOption, IIf(aRequest.Type = enSTK, enCtStock, enCtIndex))) & "_" & aRequest.Symbol
'
'                Set aReq = m_QuoteRequests(sKey)
'                If Not aReq Is Nothing Then
'                    LogEvent EVENT_WARNING, aRequest.Symbol & ": " & Description
'
'                    m_nQuoteReqDone = m_nQuoteReqDone + 1
'                    IncProgress pbProgress
'
'                    If m_bGroupRequest And aRequest.Type <> enOPT Then
'                        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'                    End If
'                    bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
'                                Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'                End If
'            ElseIf PpIsGroupReqType(Request.Type) Then
'                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
'                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'                bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'            End If
'
'            If bFinished Then
'                m_bLastQuoteReqNow = False
'
'                PriceProvider.CancelLastQuote
'                If m_bGroupRequest Then
'                    m_GroupPriceProvider.CancelGroup
'                End If
'
'                CalcIndex
'                UnderlyingsUpdate False
'                UpdateTotals
'
'                pbProgress.Visible = False
'                lblStatus.Visible = True
'                lblProcess.Visible = False
'                imgStop.Visible = False
'                imgStopDis.Visible = False
'
'                AdjustState
'            End If
'        Else
' '           Debug.Assert False
'        End If
'
'    Else
'        LogEvent EVENT_ERROR, Description
'    End If
'End Sub
'
'Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
'    On Error Resume Next
'    If Not m_bLastQuoteReqNow Then Exit Sub
'
'    Dim aReq As clsIhRequestAtom, sKey$
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom, aOpt As EtsMmIndexHedgeLib.MmIhOptAtom
'    Dim dPriceBid#, dPriceAsk#, dPriceLast#, aIdx As EtsMmIndexHedgeLib.MmIhIndexAtom
'    Dim bFinished As Boolean
'    bFinished = False
'
'    If PpIsNonGroupReqType(Params.Type) Then
'        sKey = CStr(IIf(Params.Type = enOPT, enCtOption, IIf(Params.Type = enSTK, enCtStock, enCtIndex))) & "_" & Params.Symbol
'        Set aReq = m_QuoteRequests(sKey)
'
'        If Not aReq Is Nothing Then
'            dPriceBid = Results.BidPrice
'            dPriceAsk = Results.AskPrice
'            dPriceLast = Results.LastPrice
'
'            Set aOpt = aReq.Opt
'            Set aUnd = aReq.Und
'            Set aIdx = aReq.Idx
'
'            If Not aOpt Is Nothing Then
'                aOpt.PriceBid = dPriceBid
'                aOpt.PriceAsk = dPriceAsk
'                aOpt.PriceLast = dPriceLast
'
'            ElseIf Not aUnd Is Nothing Then
'                aUnd.PriceBid = dPriceBid
'                aUnd.PriceAsk = dPriceAsk
'                aUnd.PriceLast = dPriceLast
'
'            ElseIf Not aIdx Is Nothing Then
'                aIdx.PriceBid = dPriceBid
'                aIdx.PriceAsk = dPriceAsk
'                aIdx.PriceLast = dPriceLast
'            End If
'
'            Set aOpt = Nothing
'            Set aUnd = Nothing
'            Set aIdx = Nothing
'
'            m_nQuoteReqDone = m_nQuoteReqDone + 1
'            IncProgress pbProgress
'            Set aReq = Nothing
'
'            If m_bGroupRequest And Params.Type <> enOPT Then
'                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'            End If
'            bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
'                        Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'        End If
'    ElseIf PpIsGroupReqType(Params.Type) Then
'        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'        bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'    End If
'
'    If bFinished Then
'        m_bLastQuoteReqNow = False
'
'        PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'
'        CalcIndex
'        UnderlyingsUpdate False
'        UpdateTotals
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'
'        AdjustState
'    End If
'End Sub
'
'Private Function CheckIdxTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean, nValue&
'    bMatched = (m_nFilter(IFC_INDEX) = aTrd.UndID And aTrd.ContractType = enCtOption)
'
'    If bMatched Then
'        nValue = m_nFilter(IFC_TRADER)
'        bMatched = (aTrd.Trader.ID = nValue)
'    End If
'
'    If bMatched Then
'        nValue = m_nFilter(IFC_STRATEGY)
'        bMatched = (aTrd.StrategyID = nValue)
'    End If
'
'    CheckIdxTradeFilter = bMatched
'End Function
'
'Private Function CheckUndTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean, nValue&
'
'    bMatched = (aTrd.Und.UndType = enCtStock And aTrd.FutRootID = 0)
'
'    If bMatched Then
'        nValue = m_nFilter(IFC_TRADER)
'        bMatched = (aTrd.Trader.ID = nValue)
'    End If
'
'    If bMatched Then
'        nValue = m_nFilter(IFC_STRATEGY)
'        bMatched = (aTrd.StrategyID = nValue)
'    End If
'
'    CheckUndTradeFilter = bMatched
'End Function
'
'Private Sub FilterUpdate()
'    On Error Resume Next
'    Dim nValue&
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
'
'        nValue = m_nFilter(IFC_TRADER)
'        If g_Trader(nValue) Is Nothing Then nValue = 0
'        m_nFilter(IFC_TRADER) = nValue
'        .TextMatrix(1, IFC_TRADER) = nValue
'
'        nValue = m_nFilter(IFC_STRATEGY)
'        If nValue > 0 Then _
'            If g_Strategy(nValue) Is Nothing Then nValue = 0
'        m_nFilter(IFC_STRATEGY) = nValue
'        .TextMatrix(1, IFC_STRATEGY) = nValue
'
'        nValue = m_nFilter(IFC_INDEX)
'        If g_BasketIndex(nValue) Is Nothing Then nValue = 0
'        m_nFilter(IFC_INDEX) = nValue
'        .TextMatrix(1, IFC_INDEX) = nValue
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub InitFltData()
'    On Error Resume Next
'    Dim i&, nCount&, sComboList$, sValue$, aIdx As EtsGeneralLib.IndexAtom, nValue&
'    Dim aTrader As EtsGeneralLib.EtsTraderAtom
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = "#0;<Select>"
'        For Each aTrader In g_Trader
'            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
'            If aTrader.ID = m_nFilter(IFC_TRADER) Then
'                nValue = aTrader.ID
'                sValue = Trim$(Str$(nValue))
'            End If
'        Next
'        .ColComboList(IFC_TRADER) = sComboList
'        .TextMatrix(1, IFC_TRADER) = sValue
'        m_nFilter(IFC_TRADER) = nValue
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = "#0;<Select>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
'        If m_nFilter(IFC_STRATEGY) = NO_STRATEGY_ID Then
'            nValue = NO_STRATEGY_ID
'            sValue = NO_STRATEGY_NAME
'        End If
'
'        For Each aStrategy In g_Strategy
'            sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
'            If aStrategy.ID = m_nFilter(IFC_STRATEGY) Then
'                nValue = aStrategy.ID
'                sValue = Trim$(Str$(aStrategy.ID))
'            End If
'        Next
'        .ColComboList(IFC_STRATEGY) = sComboList
'        .TextMatrix(1, IFC_STRATEGY) = sValue
'        m_nFilter(IFC_STRATEGY) = nValue
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = g_Params.BasketIndexComboList
'        If Len(sComboList) > 0 Then
'            sComboList = "#0;<Select>|" & sComboList
'        Else
'            sComboList = "#0;<Select>"
'        End If
'
'        Set aIdx = g_BasketIndex(m_nFilter(IFC_INDEX))
'        If Not aIdx Is Nothing Then
'            nValue = aIdx.ID
'            sValue = Trim$(Str$(aIdx.ID))
'            Set aIdx = Nothing
'        End If
'        .ColComboList(IFC_INDEX) = sComboList
'        .TextMatrix(1, IFC_INDEX) = sValue
'        m_nFilter(IFC_INDEX) = nValue
'
'        m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub InitColumns()
'    On Error Resume Next
'    g_DefGrids(GT_INDEXHEDGE1_FILTER).CopyTo m_gdFlt
'    g_DefGrids(GT_INDEXHEDGE1_TOTALS).CopyTo m_gdTot
'    g_DefGrids(GT_INDEXHEDGE1_UNDERLYINGS).CopyTo m_gdUnd
'End Sub
'
'Private Sub InitGrids()
'    On Error Resume Next
'    With fgFlt
'        .Rows = 2
'        .Cols = IFC_COLUMN_COUNT
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
'
'        .HighLight = flexHighlightWithFocus
'    End With
'
'    With fgTot
'        .Rows = 2
'        .Cols = ITC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeNone
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
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
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'
'        .HighLight = flexHighlightWithFocus
'    End With
'
'    With fgUnd
'        .Rows = 1
'        .Cols = IUC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDNone
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExSort
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusHeavy
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .OutlineCol = 1
'        .ScrollBars = flexScrollBarBoth
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionByRow
'        .TabBehavior = flexTabControls
'    End With
'End Sub
'
'Private Sub FormatFltGrid()
'    On Error Resume Next
'    Dim i&, nCols&
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
'
'        nCols = IFC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = IFC_TRADER To IFC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
'            .ColFormat(i) = m_gdFlt.Col(i).Format
'            .ColDataType(i) = m_gdFlt.Col(i).DataType
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatFltColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
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
'        For i = IFC_TRADER To IFC_LAST_COLUMN
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
'        m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatTotGrid()
'    On Error Resume Next
'    Dim i&, nCols&
'
'    With fgTot
'        m_GridLock(GT_INDEXHEDGE1_TOTALS).LockRedraw
'
'        nCols = ITC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = ITC_INDEX To ITC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gdTot.Col(i).Caption
'            .ColFormat(i) = m_gdTot.Col(i).Format
'            .ColDataType(i) = m_gdTot.Col(i).DataType
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_INDEXHEDGE1_TOTALS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatTotColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont
'
'    With fgTot
'        m_GridLock(GT_INDEXHEDGE1_TOTALS).LockRedraw
'
'        aFont.Name = m_gdTot.Font.Name
'        aFont.Size = m_gdTot.Font.Size
'        aFont.Bold = m_gdTot.Font.Bold
'        aFont.Italic = m_gdTot.Font.Italic
'        aFont.Strikethrough = m_gdTot.Font.Strikethrough
'        aFont.Underline = m_gdTot.Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = ITC_INDEX To ITC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdTot.Col(i).BackColor <> 0, m_gdTot.Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdTot.Col(i).ForeColor <> 0, m_gdTot.Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If m_gdTot.HorizLinesVisible Then
'            If m_gdTot.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdTot.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdTot.GridLinesColor
'
'        .BackColorSel = m_gdTot.BackColorSel
'        .ForeColorSel = m_gdTot.ForeColorSel
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_INDEXHEDGE1_TOTALS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatUndGrid()
'    On Error Resume Next
'    Dim i&, nCols&
'
'    With fgUnd
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).LockRedraw
'
'        nCols = IUC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = IUC_SYMBOL To IUC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gdUnd.Col(i).Caption
'            .ColFormat(i) = m_gdUnd.Col(i).Format
'            .ColDataType(i) = m_gdUnd.Col(i).DataType
'            .ColWidth(i) = IIf(m_gdUnd.Col(i).Width > 0, m_gdUnd.Col(i).Width, -1)
'            .ColKey(i) = i
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'        geUnd.AdjustSortImages
'
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatUndColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont
'
'    With fgUnd
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).LockRedraw
'
'        aFont.Name = m_gdUnd.Font.Name
'        aFont.Size = m_gdUnd.Font.Size
'        aFont.Bold = m_gdUnd.Font.Bold
'        aFont.Italic = m_gdUnd.Font.Italic
'        aFont.Strikethrough = m_gdUnd.Font.Strikethrough
'        aFont.Underline = m_gdUnd.Font.Underline
'
'        Set .Font = aFont
'
'        .ColHidden(IUC_KEY) = True
'        .ColDataType(IUC_KEY) = flexDTString
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = IUC_SYMBOL To IUC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(i).BackColor <> 0, m_gdUnd.Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(i).ForeColor <> 0, m_gdUnd.Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If m_gdUnd.HorizLinesVisible Then
'            If m_gdUnd.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdUnd.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdUnd.GridLinesColor
'
'        .BackColorSel = m_gdUnd.BackColorSel
'        .ForeColorSel = m_gdUnd.ForeColorSel
'
'        '.AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub aParams_PriceProfilesChange()
'    SetRefreshHint True
'End Sub
'
'Private Sub aParams_PriceRoundingRuleChange()
'    SetRefreshHint True
'End Sub
'
'Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As clsFilterAtomColl)
'    On Error Resume Next
'    If m_Idx Is Nothing Then Exit Sub
'    Dim aFilterAtom As clsFilterAtom
'
'    For Each aFilterAtom In collUpdUnd
'        If m_Idx.ID = aFilterAtom.ID Then
'            SetRefreshHint True
'            Exit For
'        End If
'    Next
'End Sub
'
'Private Sub aParams_UndPriceToleranceChange()
'    SetRefreshHint True
'End Sub
'
'Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
'    On Error Resume Next
'    Dim aIdx As EtsGeneralLib.IndexAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(IFC_INDEX) <> nNewUndID Then
'        Set aIdx = g_BasketIndex(nNewUndID)
'        If Not aIdx Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
'
'                nValue = 0
'                sValue = CStr(nValue)
'                sComboList = g_Params.BasketIndexComboList
'                If Len(sComboList) > 0 Then
'                    sComboList = "#0;<Select>|" & sComboList
'                Else
'                    sComboList = "#0;<Select>"
'                End If
'
'                Set aIdx = g_BasketIndex(m_nFilter(IFC_INDEX))
'                If Not aIdx Is Nothing Then
'                    nValue = aIdx.ID
'                    sValue = Trim$(Str$(aIdx.ID))
'                    Set aIdx = Nothing
'                End If
'                .ColComboList(IFC_INDEX) = sComboList
'                .TextMatrix(1, IFC_INDEX) = sValue
'                m_nFilter(IFC_INDEX) = nValue
'
'                m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'            End With
'        End If
'    End If
'End Sub
'
'Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
'    On Error Resume Next
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(IFC_STRATEGY) <> nStrategyID Then
'        Set aStrategy = g_Strategy(nStrategyID)
'        If Not aStrategy Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
'
'                sValue = "0"
'                nValue = 0
'                sComboList = "#0;<Select>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
'                If m_nFilter(IFC_STRATEGY) = NO_STRATEGY_ID Then
'                    nValue = NO_STRATEGY_ID
'                    sValue = NO_STRATEGY_NAME
'                End If
'
'                For Each aStrategy In g_Strategy
'                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
'                    If aStrategy.ID = m_nFilter(IFC_STRATEGY) Then
'                        nValue = aStrategy.ID
'                        sValue = Trim$(Str$(aStrategy.ID))
'                    End If
'                Next
'                .ColComboList(IFC_STRATEGY) = sComboList
'                .TextMatrix(1, IFC_STRATEGY) = sValue
'                m_nFilter(IFC_STRATEGY) = nValue
'
'                m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'            End With
'        End If
'    End If
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
'                .AutoSize 0, .Cols - 1, , 100
'            End If
'
'            FilterUpdate
'            FilterUpdateChangedStatus
'            AdjustCaption
'            AdjustState
'        End If
'    End With
'End Sub
'
'Private Sub fgFlt_DblClick()
'    On Error Resume Next
'    With fgFlt
'        m_enMenuGrid = GT_INDEXHEDGE1_FILTER
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick False
'    End With
'End Sub
'
'Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridClipOrTrade(KeyCode, Shift) Then
'        m_bKeyDown(GT_INDEXHEDGE1_FILTER) = True
'    End If
'End Sub
'
'Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_INDEXHEDGE1_FILTER) Then
'        m_bKeyDown(GT_INDEXHEDGE1_FILTER) = False
'        With fgFlt
'            m_enMenuGrid = GT_INDEXHEDGE1_FILTER
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'            Case KeyCode = vbKeyInsert And Shift = 0
'                mnuCtxTradeNew_Click
'        End Select
'    End If
'End Sub
'
'Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = IsDblClickHandled Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
'    If Not Cancel Then m_sCurrentOriginalText = Trim$(fgFlt.TextMatrix(Row, Col))
'End Sub
'
'Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgFlt
'            m_enMenuGrid = GT_INDEXHEDGE1_FILTER
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub ShowPopup()
'    On Error Resume Next
'    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
'
'    mnuCtxCopy.Enabled = Not m_bInProc
'
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE1_FILTER
'            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_nFilter(IFC_INDEX) <> 0 _
'                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = False
'            mnuCtxAutosizeGrid.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_INDEXHEDGE1_TOTALS
'            mnuCtxTradeNew.Enabled = False
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = False
'            mnuCtxAutosizeGrid.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_INDEXHEDGE1_UNDERLYINGS
'            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeGrid.Enabled = True
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
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
'        Case GT_INDEXHEDGE1_FILTER
'            gdGrid.CopyTo m_gdFlt
'
'            m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
'            FormatFltGrid
'            FormatFltColumns
'            m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'
'        Case GT_INDEXHEDGE1_TOTALS
'            gdGrid.CopyTo m_gdTot
'
'            m_GridLock(GT_INDEXHEDGE1_TOTALS).LockRedraw
'            FormatTotGrid
'            FormatTotColumns
'            m_GridLock(GT_INDEXHEDGE1_TOTALS).UnlockRedraw
'
'        Case GT_INDEXHEDGE1_UNDERLYINGS
'            gdGrid.CopyTo m_gdUnd
'
'            m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).LockRedraw
'            FormatUndGrid
'            FormatUndColumns
'            m_GridLock(GT_INDEXHEDGE1_UNDERLYINGS).UnlockRedraw
'    End Select
'
'    UserControl_Resize
'End Sub
'
'Private Sub imgStop_Click()
'    On Error Resume Next
'    If m_bLastQuoteReqNow Then
'        m_bLastQuoteReqNow = False
'        PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        AdjustState
'
'    ElseIf m_bDataLoad Then
'        m_bDataLoad = False
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        AdjustState
'
'    ElseIf m_bRecalc Then
'        m_bRecalc = False
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        AdjustState
'
'    ElseIf m_bInProc Then
'        m_bInProc = False
'        AdjustState
'    End If
'End Sub
'
'Private Sub mnuCtxCopy_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE1_FILTER
'            g_ClipMgr.CopyGridToClipboard fgFlt
'
'        Case GT_INDEXHEDGE1_TOTALS
'            g_ClipMgr.CopyGridToClipboard fgTot
'
'        Case GT_INDEXHEDGE1_UNDERLYINGS
'            g_ClipMgr.CopyGridToClipboard fgUnd
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxGridLayout_Click()
'    On Error Resume Next
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE1_FILTER
'            CustomizeFltGridLayout
'
'        Case GT_INDEXHEDGE1_TOTALS
'            CustomizeTotGridLayout
'
'        Case GT_INDEXHEDGE1_UNDERLYINGS
'            CustomizeUndGridLayout
'    End Select
'End Sub
'
'Private Sub mnuCtxRefresh_Click()
'    Refresh
'End Sub
'
'Private Sub UserControl_Hide()
'    On Error Resume Next
'    m_bDataLoad = False
'    m_bLastQuoteReqNow = False
'End Sub
'
'Private Sub UserControl_Resize()
'    On Error Resume Next
'    If m_bShutDown Or gCmn Is Nothing Then Exit Sub
'
'    With fgFlt
'        .Top = 0
'        .Left = 0
'        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'        .Width = ScaleWidth
'    End With
'
'    With fgTot
'        .Top = fgFlt.Height + GRID_VERT_SPACE
'        .Left = 0
'        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'        .Width = ScaleWidth
'    End With
'
'    With fgUnd
'        .Left = 0
'        .Top = fgFlt.Height + fgTot.Height + GRID_VERT_SPACE * 2
'        .Width = ScaleWidth
'        .Height = ScaleHeight - .Top
'    End With
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
'
'    m_bShutDown = True
'
'    Set pbProgress = Nothing
'    Set lblProcess = Nothing
'    Set lblStatus = Nothing
'
'    If Not PriceProvider Is Nothing Then
'        PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'        Set PriceProvider = Nothing
'    End If
'
'    ClearViewAndData
'
'    Set m_GroupPriceProvider = Nothing
'    Set m_Idx = Nothing
'
'    Set imgStop = Nothing
'    Set imgStopDis = Nothing
'    Set frmLayout = Nothing
'    Set aParams = Nothing
'End Sub
'
'Private Sub AdjustCaption()
'    On Error Resume Next
'    RaiseEvent OnSetCaption
'End Sub
'
'Private Sub AdjustState()
'    On Error Resume Next
'    UpdateMenu
'    RaiseEvent OnStateChange
'End Sub
'
'Private Sub UpdateMenu()
'    On Error Resume Next
'    mnuCtxRefresh.Enabled = Not (m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc)
'End Sub
'
'Public Function GetCaption() As String
'    On Error Resume Next
'    Dim sCaption$
'    GetCaption = "Index Hedge"
'End Function
'
'Public Function GetShortCaption() As String
'    On Error Resume Next
'    If Not m_Idx Is Nothing Then
'        GetShortCaption = m_Idx.Symbol
'    Else
'        GetShortCaption = "<Empty>"
'    End If
'End Function
'
'Public Property Get InProc() As Boolean
'    InProc = m_bInProc
'End Property
'
'Public Property Get LastQuoteReqNow() As Boolean
'    LastQuoteReqNow = m_bLastQuoteReqNow
'End Property
'
'Private Sub CopyFilters(ByVal bToPrev As Boolean)
'    On Error Resume Next
'    Dim i&
'
'    If bToPrev Then
'        For i = IFC_TRADER To IFC_LAST_COLUMN
'            m_nFilterPrev(i) = m_nFilter(i)
'        Next
'    Else
'        For i = IFC_TRADER To IFC_LAST_COLUMN
'            m_nFilter(i) = m_nFilterPrev(i)
'       Next
'    End If
'End Sub
'
'Private Sub FilterUpdateChangedStatus()
'    On Error Resume Next
'    Dim i&, bSetHint As Boolean
'
'    bSetHint = False
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE1_FILTER).LockRedraw
'
'        For i = IFC_TRADER To IFC_LAST_COLUMN
'            If m_nFilter(i) <> m_nFilterPrev(i) Then
'                .Cell(flexcpFontBold, 1, i) = True
'                bSetHint = True
'            Else
'                .Cell(flexcpFontBold, 1, i) = False
'            End If
'        Next
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_INDEXHEDGE1_FILTER).UnlockRedraw
'        SetRefreshHint bSetHint
'    End With
'End Sub
'
'Public Sub FilterRevert()
'    On Error Resume Next
'    CopyFilters False
'    FilterUpdate
'    FilterUpdateChangedStatus
'    AdjustState
'End Sub
'
'Public Function FilterRevertEnabled() As Boolean
'    On Error Resume Next
'    Dim i&, bCanRevert As Boolean: bCanRevert = False
'
'    If Not m_bInProc Then
'        bCanRevert = False
'        For i = IFC_TRADER To IFC_LAST_COLUMN
'            If m_nFilter(i) <> m_nFilterPrev(i) Then
'                bCanRevert = True
'                Exit For
'            End If
'        Next
'    End If
'
'    FilterRevertEnabled = bCanRevert
'End Function
'
'Private Sub ClearViewAndData()
'    On Error Resume Next
'    fgUnd.Rows = 1
'
'    Set m_Idx = Nothing
'    m_QuoteRequests.Clear
'End Sub
'
'Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&, sSection$
'
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    For i = IFC_TRADER To IFC_LAST_COLUMN
'        aStorage.SetLongValue "IndexHedgeFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
'    Next
'
'    aStorage.SetDoubleValue "IndexHedgeTot" & sKey, "Slice", m_dSlice
'    aStorage.SetLongValue "IndexHedgeTot" & sKey, "LotSize", m_nLotSize
'    aStorage.SetLongValue "IndexHedgeTot" & sKey, "MaxSize", m_nMaxSize
'
'    Exit Sub
'EH:
'    gCmn.ErrorHandler ""
'End Sub
'
'Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&, sSection$
'
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    For i = IFC_TRADER To IFC_LAST_COLUMN
'        m_nFilter(i) = aStorage.GetLongValue("IndexHedgeFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
'    Next
'
'    m_dSlice = aStorage.GetDoubleValue("IndexHedgeTot" & sKey, "Slice", m_dSlice)
'    If m_dSlice < 0.01 Then
'        m_dSlice = 0.01
'    ElseIf m_dSlice > 1# Then
'        m_dSlice = 1#
'    End If
'
'    m_nLotSize = aStorage.GetLongValue("IndexHedgeTot" & sKey, "LotSize", m_nLotSize)
'    If m_nLotSize <= 0 Then _
'        m_nLotSize = 100
'
'    m_nMaxSize = aStorage.GetLongValue("IndexHedgeTot" & sKey, "MaxSize", m_nMaxSize)
'    If m_nMaxSize <= 0 Then _
'        m_nMaxSize = 10000
'
'    fgTot.TextMatrix(1, ITC_SLICE) = m_dSlice * 100#
'    fgTot.TextMatrix(1, ITC_LOT_SIZE) = m_nLotSize
'    fgTot.TextMatrix(1, ITC_MAX_SIZE) = m_nMaxSize
'
'    FilterUpdate
'
'    tmrShow.Enabled = True
'    Exit Sub
'EH:
'    gCmn.ErrorHandler ""
'End Sub
'
'Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, "Index Hedge", "Index Hedge", fgFlt, fgTot, fgUnd
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Function CurUnderlyingID() As Long
'    On Error Resume Next
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'
'    CurUnderlyingID = 0
'
'    If ActiveControl Is fgUnd Then
'        With fgUnd
'            If .Row > 0 Then
'                Set aUnd = .RowData(.Row)
'                If Not aUnd Is Nothing Then
'                    CurUnderlyingID = aUnd.ID
'                    Set aUnd = Nothing
'                End If
'            End If
'        End With
'    Else
'        CurUnderlyingID = m_nFilter(IFC_INDEX)
'    End If
'End Function
'
'Public Sub Refresh()
'    On Error Resume Next
'    With fgFlt
'        If .EditWindow <> 0 Then .Select .Row, .Col
'    End With
'
'    tmrShow.Enabled = True
'End Sub
'
'Public Sub CustomizeFltGridLayout()
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute GT_INDEXHEDGE1_FILTER, IFC_TRADER, IFC_LAST_COLUMN, m_gdFlt, m_frmOwner
'End Sub
'
'Public Sub CustomizeTotGridLayout()
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute GT_INDEXHEDGE1_TOTALS, ITC_INDEX, ITC_LAST_COLUMN, m_gdTot, m_frmOwner
'End Sub
'
'Public Sub CustomizeUndGridLayout()
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute GT_INDEXHEDGE1_UNDERLYINGS, IUC_SYMBOL, IUC_LAST_COLUMN, m_gdUnd, m_frmOwner
'End Sub
'
'Private Sub SetRefreshHint(ByVal bSet As Boolean)
'    On Error Resume Next
'    lblStatus.Visible = True
'    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
'End Sub
'
'Private Sub HandleGridDblClick(ByVal bTradeNewAvailable As Boolean)
'    On Error Resume Next
'    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
'        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
'
'        If bTradeNewAvailable And Not (IsCtrlPressed Or IsShiftPressed) Then
'            mnuCtxTradeNew_Click
'            FixDblClickHandled
'        End If
'    End If
'End Sub
'
'Private Sub mnuCtxTradeNew_Click()
'    On Error GoTo EH
'    If m_bInProc Then Exit Sub
'    Dim nUndID&, nID&, bBuy As Boolean
'
'    m_bInProc = True
'    nUndID = 0
'    bBuy = True
'
'    If m_enMenuGrid = GT_INDEXHEDGE1_FILTER Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then nUndID = m_nFilter(IFC_INDEX)
'        bBuy = (g_Params.QuoteOtherColBuy <> 0)
'
'    ElseIf m_enMenuGrid = GT_INDEXHEDGE1_UNDERLYINGS Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
'            On Error Resume Next
'            nUndID = fgUnd.RowData(m_nMenuGridRow).ID
'            On Error GoTo EH
'        End If
'        bBuy = (g_Params.QuoteOtherColBuy <> 0)
'    End If
'
'    m_bInProc = False
'    If nUndID <> 0 Then frmTradeEditor.NewTrade nUndID, nUndID, bBuy, m_frmOwner
'
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
'    ResetMenuData
'End Sub
'
'Private Sub InitIndexVola()
'    On Error GoTo EH
'    If Not m_Idx Is Nothing Then m_Idx.VolaSrv.Init m_Idx.Symbol, enCtIndex, g_VolaSource
'    Exit Sub
'EH:
'    LogEvent EVENT_ERROR, "Fail to init volatility for '" & m_Idx.Symbol & "': " & Err.Description
'End Sub
'
'Public Sub SaveOrders(ByVal sPath As String)
'    On Error GoTo EH
'    If m_bInProc Or m_Idx Is Nothing Or Not IsValidFilter Then Exit Sub
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim nFileB&, nFileS&, nFileSS&, sPrefix$, nTotalQty&, nExportCount&
'    Dim nQtyB&, nQtyS&, nQtySS&, nOrdRemainder&, nOrdCount&, i&, nUndTotalQty&
'
'    m_bInProc = True
'    nExportCount = 0
'
'    nFileB = 0
'    nFileS = 0
'    nFileSS = 0
'
'    For Each aUnd In m_Idx.Und
'        If aUnd.TotalQtyInShares > BAD_LONG_VALUE And aUnd.CurShares > BAD_LONG_VALUE _
'            And aUnd.IndexTxn > BAD_LONG_VALUE And aUnd.IndexTxn <> 0& Then
'
'            sPrefix = aUnd.Symbol
'            If Len(sPrefix) < 8 Then sPrefix = sPrefix & Space$(8 - Len(sPrefix))
'            nQtyB = 0&
'            nQtyS = 0&
'            nQtySS = 0&
'
'            nUndTotalQty = aUnd.TotalQtyInShares
'
'            If aUnd.IndexTxn > 0 Then
'                nQtyB = aUnd.IndexTxn
'
'            Else ' aUnd.IndexTxn < 0
'                If nUndTotalQty <= 0 Then
'                    nQtySS = Abs(aUnd.IndexTxn)
'
'                ElseIf Abs(aUnd.IndexTxn) > nUndTotalQty And nUndTotalQty > 0 Then
'                    nQtyS = Abs(-Sgn(nUndTotalQty) * Int(Abs(nUndTotalQty) / m_nLotSize + 0.5) * m_nLotSize)
'                    nQtySS = Abs(Abs(aUnd.IndexTxn) - nQtyS)
'
'                ElseIf Abs(aUnd.IndexTxn) <= nUndTotalQty Then
'                    nQtyS = Abs(aUnd.IndexTxn)
'                End If
'            End If
'
'            If nQtyB <> 0 Or nQtyS <> 0 Or nQtySS <> 0 Then
'                If nQtyB <> 0 Then
'                    nOrdCount = Int(nQtyB / m_nMaxSize)
'                    nOrdRemainder = nQtyB - nOrdCount * m_nMaxSize
'
'                    If nFileB = 0 Then
'                        nFileB = FreeFile
'                        Open sPath & "bsktBuy.txt" For Output Access Write Lock Read Write As #nFileB
'                    End If
'
'                    For i = 1 To nOrdCount
'                        Print #nFileB, sPrefix & Trim$(Str$(m_nMaxSize))
'                        nExportCount = nExportCount + 1
'                    Next
'
'                    If nOrdRemainder > 0 Then
'                        Print #nFileB, sPrefix & Trim$(Str$(nOrdRemainder))
'                        nExportCount = nExportCount + 1
'                    End If
'                End If
'
'                If nQtyS <> 0 Then
'                    nOrdCount = Int(nQtyS / m_nMaxSize)
'                    nOrdRemainder = nQtyS - nOrdCount * m_nMaxSize
'
'                    If nFileS = 0 Then
'                        nFileS = FreeFile
'                        Open sPath & "bsktSell.txt" For Output Access Write Lock Read Write As #nFileS
'                    End If
'
'                    For i = 1 To nOrdCount
'                        Print #nFileS, sPrefix & Trim$(Str$(m_nMaxSize))
'                        nExportCount = nExportCount + 1
'                    Next
'
'                    If nOrdRemainder > 0 Then
'                        Print #nFileS, sPrefix & Trim$(Str$(nOrdRemainder))
'                        nExportCount = nExportCount + 1
'                    End If
'                End If
'
'                If nQtySS <> 0 Then
'                    nOrdCount = Int(nQtySS / m_nMaxSize)
'                    nOrdRemainder = nQtySS - nOrdCount * m_nMaxSize
'
'                    If nFileSS = 0 Then
'                        nFileSS = FreeFile
'                        Open sPath & "bsktSht.txt" For Output Access Write Lock Read Write As #nFileSS
'                    End If
'
'                    For i = 1 To nOrdCount
'                        Print #nFileSS, sPrefix & Trim$(Str$(m_nMaxSize))
'                        nExportCount = nExportCount + 1
'                    Next
'
'                    If nOrdRemainder > 0 Then
'                        Print #nFileSS, sPrefix & Trim$(Str$(nOrdRemainder))
'                        nExportCount = nExportCount + 1
'                    End If
'                End If
'            End If
'        End If
'    Next
'
'    If nFileB <> 0 Then Close #nFileB
'    If nFileS <> 0 Then Close #nFileS
'    If nFileSS <> 0 Then Close #nFileSS
'
'    gCmn.MyMsgBox m_frmOwner, CStr(nExportCount) & " order(s) saved.", vbInformation
'EX:
'    On Error Resume Next
'    Close #nFileB
'    Close #nFileS
'    Close #nFileSS
'
'    m_bInProc = False
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to save orders."
'    ResetMenuData
'    GoTo EX
'End Sub
'
'Private Sub UpdateTotals()
'    On Error Resume Next
'    With fgTot
'        m_GridLock(GT_INDEXHEDGE1_TOTALS).LockRedraw
'
'        If Not m_Idx Is Nothing Then
'            .TextMatrix(1, ITC_INDEX) = m_Idx.Symbol
'            .TextMatrix(1, ITC_NET_DELTA) = IIf(m_Idx.NetDelta > BAD_DOUBLE_VALUE, m_Idx.NetDelta, STR_NA)
'            .TextMatrix(1, ITC_PRICE) = IIf(m_Idx.PriceLast > BAD_DOUBLE_VALUE, m_Idx.PriceLast, STR_NA)
'        Else
'            .TextMatrix(1, ITC_INDEX) = "<Not Selected>"
'            .TextMatrix(1, ITC_NET_DELTA) = STR_NA
'            .TextMatrix(1, ITC_PRICE) = STR_NA
'        End If
'
'        .TextMatrix(1, ITC_SLICE) = m_dSlice * 100#
'        .TextMatrix(1, ITC_LOT_SIZE) = m_nLotSize
'        .TextMatrix(1, ITC_MAX_SIZE) = m_nMaxSize
'        .TextMatrix(1, ITC_CONTRACTS) = IIf(m_nContracts <> 0&, m_nContracts, "")
'        .TextMatrix(1, ITC_DELTA) = IIf(m_dDelta <> 0#, m_dDelta * 100#, "")
'        .TextMatrix(1, ITC_EXPOSURE) = IIf(m_dExposure > BAD_DOUBLE_VALUE, m_dExposure, STR_NA)
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_INDEXHEDGE1_TOTALS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub CalcTotals(Optional ByVal bCalcNetDelta As Boolean = True)
'    On Error Resume Next
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim aOpt As EtsMmIndexHedgeLib.MmIhOptAtom
'    Dim dUndMidPrice#
'
'    m_dExposure = BAD_DOUBLE_VALUE
'
'    If Not m_Idx Is Nothing Then
'        If bCalcNetDelta Then
'            m_Idx.NetDelta = BAD_DOUBLE_VALUE
'            For Each aOpt In m_Idx.Opt
'                If aOpt.Delta > BAD_DOUBLE_VALUE Then
'                    If m_Idx.NetDelta = BAD_DOUBLE_VALUE Then m_Idx.NetDelta = 0&
'                    m_Idx.NetDelta = m_Idx.NetDelta + aOpt.Delta * aOpt.Qty
'                End If
'            Next
'        End If
'
'        If m_Idx.PriceLast > 0# Then
'            If m_nContracts <> 0 And m_dDelta <> 0# Then
'                m_dExposure = m_Idx.PriceLast * m_nContracts * m_dDelta * 100#
'            Else
'                If m_Idx.NetDelta > BAD_DOUBLE_VALUE Then _
'                    m_dExposure = m_Idx.PriceLast * m_Idx.NetDelta
'            End If
'        End If
'
'        For Each aUnd In m_Idx.Und
'            dUndMidPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, _
'                                                        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'
'            If m_dExposure > BAD_DOUBLE_VALUE And dUndMidPrice > 0# And aUnd.Weight > 0# And aUnd.CurShares > BAD_LONG_VALUE Then
'                aUnd.IndexShares1 = m_dExposure * aUnd.Weight / aUnd.PriceLast
'                aUnd.IndexShares1 = Sgn(aUnd.IndexShares1) * Int(Abs(aUnd.IndexShares1) / m_nLotSize + 0.5) * m_nLotSize
'                aUnd.IndexTxn = -(aUnd.IndexShares1 + aUnd.CurShares) * m_dSlice
'                aUnd.IndexTxn = Sgn(aUnd.IndexTxn) * Int(Abs(aUnd.IndexTxn) / m_nLotSize + 0.5) * m_nLotSize
'            Else
'                aUnd.IndexShares1 = BAD_LONG_VALUE
'                aUnd.IndexTxn = BAD_LONG_VALUE
'            End If
'        Next
'    End If
'End Sub
'
'Private Sub CalcIndex()
'    On Error Resume Next
'    If m_bInProc Or m_Idx Is Nothing Then Exit Sub
'
'    m_bInProc = True
'    m_bRecalc = True
'    AdjustState
'
'    If m_bRecalc Then
'        lblStatus.Visible = False
'        imgStop.Visible = True
'        imgStopDis.Visible = False
'        pbProgress.Min = 0
'        pbProgress.Value = 0
'        pbProgress.Max = 100
'        pbProgress.Visible = True
'        lblProcess.Caption = "Calculation..."
'        lblProcess.Visible = True
'        lblProcess.Refresh
'
'        Dim nCalcSleepFreq&: nCalcSleepFreq = 0&
'        Dim nCalcSleepAmt&: nCalcSleepAmt = 0&
'
'        If g_Params.UseCalcSleep And g_Params.CalcSleepAmt <> 0 Then
'            nCalcSleepFreq = g_Params.CalcSleepFreq
'            nCalcSleepAmt = g_Params.CalcSleepAmt
'        End If
'
'        m_Idx.CalcAllOptions g_Params.CalcModel, _
'                        g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
'                        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
'                        nCalcSleepFreq, nCalcSleepAmt
'        CalcTotals
'    End If
'    m_bRecalc = False
'    m_bInProc = False
'    AdjustState
'End Sub
'
'Public Function IsValidFilter() As Boolean
'    On Error Resume Next
'    IsValidFilter = (m_nFilter(IFC_TRADER) <> 0 _
'                    And m_nFilter(IFC_STRATEGY) <> 0 _
'                    And m_nFilter(IFC_INDEX) <> 0)
'End Function
'
'
'
