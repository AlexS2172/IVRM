VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlStockHedgeView 
   ClientHeight    =   6060
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   8955
   KeyPreview      =   -1  'True
   ScaleHeight     =   6060
   ScaleWidth      =   8955
   Begin VSFlex7Ctl.VSFlexGrid fgPos 
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
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   4560
      Top             =   4080
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
Attribute VB_Name = "ctlStockHedgeView"
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
'Private m_gdPos As New clsGridDef
'
'Private WithEvents gePos As clsGridEx
'
'Private WithEvents aParams As clsParams
'Private WithEvents PriceProvider As PRICEPROVIDERSLib.PriceInfo
'Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
'Private VolaSource As VolatilitySourcesLib.VolatilitySource
'Private TradeChannel As clsTradeChannel
'
'Private WithEvents frmLayout As frmGridLayout
'Private WithEvents frmMaintenance As frmStockHedgeMaintenance
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
'Private m_QuoteRequests As New clsShRequestColl
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
'Private m_nFilter(SFC_TRADER To SFC_UNDERLYING) As Long
'Private m_nFilterPrev(SFC_TRADER To SFC_UNDERLYING) As Long
'
'Private m_bKeyDown(GT_STOCKHEDGE_FILTER To GT_STOCKHEDGE_POSITIONS) As Boolean
'Private m_GridLock(GT_STOCKHEDGE_FILTER To GT_STOCKHEDGE_POSITIONS) As New clsGridLock
'
'Private m_bGroupRequest As Boolean
'Private m_bGroupRequestSupported As Boolean
'Private m_bShutDown As Boolean
'
'Private m_Und As New EtsMmStockHedgeLib.MmShUndColl
'Private m_Grp As New EtsMmStockHedgeLib.MmShTraderGroupColl
'
'Private m_dSlice#
'Private m_nLotSize&
'Private m_nMaxSize&
'
'Private m_dTotalUndNetDelta#
'Private m_nTotalHedgePos&
'
'Public Function Init() As Boolean
'    On Error GoTo EH
'
'    m_dSlice = g_Params.StockHedgeSlice
'    m_nLotSize = g_Params.StockHedgeLotSize
'    m_nMaxSize = g_Params.StockHedgeMaxSize
'
'    m_dTotalUndNetDelta = BAD_DOUBLE_VALUE
'    m_nTotalHedgePos = BAD_LONG_VALUE
'
'    m_bShutDown = False
'
'    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
'    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
'    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
'
'    ResetMenuData
'    m_sCurrentOriginalText = ""
'    m_bKeyDown(GT_STOCKHEDGE_FILTER) = False
'    m_bKeyDown(GT_STOCKHEDGE_TOTALS) = False
'    m_bKeyDown(GT_STOCKHEDGE_POSITIONS) = False
'
'    m_GridLock(GT_STOCKHEDGE_FILTER).Init fgFlt
'    m_GridLock(GT_STOCKHEDGE_TOTALS).Init fgTot
'    m_GridLock(GT_STOCKHEDGE_POSITIONS).Init fgPos
'
'    InitColumns
'    InitGrids
'
'    Set gePos = New clsGridEx
'    gePos.Init fgPos
'
'    FormatFltGrid
'    FormatFltColumns
'
'    FormatTotGrid
'    FormatTotColumns
'
'    FormatPosGrid
'    FormatPosColumns
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
'    fgFlt.Col = SFC_TRADER
'    fgFlt.Row = 1
'
'    Set frmLayout = New frmGridLayout
'    Set frmMaintenance = g_frmStockHedgeMaintenance
'    Set aParams = g_Params
'
'    Init = True
'
'    UpdateTotals
'    AdjustCaption
'    AdjustState
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize stock hedge."
'End Function
'
'Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
'    On Error Resume Next
'    Dim i&
'
'    Select Case nType
'        Case TYPE_TRADER
'            m_nFilter(SFC_TRADER) = nValue
'        Case TYPE_STRATEGY
'            m_nFilter(SFC_STRATEGY) = nValue
'        Case TYPE_UNDERLYING
'            m_nFilter(SFC_UNDERLYING) = nValue
'    End Select
'
'    For i = SFC_TRADER To SFC_LAST_COLUMN
'        m_nFilterPrev(i) = BAD_LONG_VALUE
'    Next
'
'    FilterUpdate
'    FilterUpdateChangedStatus
'    AdjustState
'End Sub
'
'Private Sub fgPos_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    If Col <> SPC_HEDGE_POS Or m_bShutDown Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc Then Exit Sub
'    Dim sValue$, dValue#, nValue&, bRecalcTotals As Boolean: bRecalcTotals = False
'    Dim aRowData As clsShRowData
'
'    With fgPos
'        sValue = Trim$(.TextMatrix(Row, Col))
'        If m_sCurrentOriginalText <> sValue Then
'            Set aRowData = .RowData(Row)
'            If Not aRowData Is Nothing Then
'                nValue = ReadLng(sValue)
'                If nValue <> 0# And aRowData.UndGrp.HedgeQty <> nValue Then
'                    aRowData.UndGrp.HedgeQty = nValue
'                    bRecalcTotals = True
'                End If
'                .TextMatrix(Row, Col) = aRowData.UndGrp.HedgeQty
'
'                If bRecalcTotals Then
'                    CalcTotals False
'                    UpdateTotals
'                End If
'
'                Set aRowData = Nothing
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgPos_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim nIdx&
'
'    If Col >= 0 And Row < 0 Then
'        With fgPos
'            nIdx = .ColKey(Col)
'            If nIdx > SPC_KEY And nIdx <= SPC_LAST_COLUMN Then
'                m_gdPos.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub fgPos_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = (Col <> SPC_HEDGE_POS) Or IsDblClickHandled Or m_bShutDown Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
'    If Not Cancel Then m_sCurrentOriginalText = Trim$(fgPos.TextMatrix(Row, Col))
'End Sub
'
'Private Sub fgPos_DblClick()
'    On Error Resume Next
'    With fgPos
'        m_enMenuGrid = GT_STOCKHEDGE_POSITIONS
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick True
'    End With
'End Sub
'
'Private Sub fgPos_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridCommonOrTrade(KeyCode, Shift) Then
'        m_bKeyDown(GT_STOCKHEDGE_POSITIONS) = True
'    End If
'End Sub
'
'Private Sub fgPos_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_STOCKHEDGE_POSITIONS) Then
'        m_bKeyDown(GT_STOCKHEDGE_POSITIONS) = False
'        With fgPos
'            m_enMenuGrid = GT_STOCKHEDGE_POSITIONS
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
'Private Sub fgPos_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgPos
'            m_enMenuGrid = GT_STOCKHEDGE_POSITIONS
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
'                Case STC_SLICE
'                    dValue = Abs(ReadDbl(sValue)) / 100#
'                    If dValue > 0.01 And dValue <= 1# And m_dSlice <> dValue Then
'                        m_dSlice = dValue
'                        bRecalcTotals = True
'                        g_Params.StockHedgeSlice = m_dSlice
'                    End If
'                    .TextMatrix(Row, Col) = m_dSlice * 100#
'
'                Case STC_LOT_SIZE
'                    nValue = Abs(ReadLng(sValue))
'                    If nValue > 0# And m_nLotSize <> nValue Then
'                        m_nLotSize = nValue
'                        bRecalcTotals = True
'                        g_Params.StockHedgeLotSize = m_nLotSize
'                    End If
'                    .TextMatrix(Row, Col) = m_nLotSize
'
'                Case STC_MAX_SIZE
'                    nValue = Abs(ReadLng(sValue))
'                    If nValue > 0# And m_nMaxSize <> nValue Then
'                        m_nMaxSize = nValue
'                        bRecalcTotals = True
'                        g_Params.StockHedgeMaxSize = m_nMaxSize
'                    End If
'                    .TextMatrix(Row, Col) = m_nMaxSize
'
'            End Select
'
'            If bRecalcTotals Then
'                CalcTotals
'                PositionsUpdate False
'                UpdateTotals
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgTot_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = (Col >= STC_NET_DELTA) Or IsDblClickHandled Or m_bShutDown Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
'    If Not Cancel Then m_sCurrentOriginalText = Trim$(fgFlt.TextMatrix(Row, Col))
'End Sub
'
'Private Sub fgTot_DblClick()
'    On Error Resume Next
'    With fgTot
'        m_enMenuGrid = GT_STOCKHEDGE_TOTALS
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
'        m_bKeyDown(GT_STOCKHEDGE_TOTALS) = True
'    End If
'End Sub
'
'Private Sub fgTot_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_STOCKHEDGE_TOTALS) Then
'        m_bKeyDown(GT_STOCKHEDGE_TOTALS) = False
'        With fgTot
'            m_enMenuGrid = GT_STOCKHEDGE_TOTALS
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
'            m_enMenuGrid = GT_STOCKHEDGE_TOTALS
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub frmMaintenance_OnOK()
'    On Error Resume Next
'    SetRefreshHint True
'End Sub
'
'Private Sub mnuCtxAutosizeCol_Click()
'    On Error Resume Next
'    Dim nIdx&
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_STOCKHEDGE_POSITIONS
'            With fgPos
'                m_GridLock(GT_STOCKHEDGE_POSITIONS).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'
'                nIdx = .ColKey(m_nMenuGridCol)
'                If nIdx > SPC_KEY And nIdx <= SPC_LAST_COLUMN Then
'                    m_gdPos.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
'                End If
'
'                m_GridLock(GT_STOCKHEDGE_POSITIONS).UnlockRedraw
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
'        Case GT_STOCKHEDGE_POSITIONS
'            With fgPos
'                m_GridLock(GT_STOCKHEDGE_POSITIONS).LockRedraw
'                .AutoSize 0, .Cols - 1
'
'                nCount = .Cols - 1
'                For i = 0 To nCount
'                    nIdx = .ColKey(i)
'                    If nIdx > SPC_KEY And nIdx <= SPC_LAST_COLUMN Then
'                        m_gdPos.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
'                    End If
'                Next
'
'                m_GridLock(GT_STOCKHEDGE_POSITIONS).UnlockRedraw
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
'    Dim aRowData As clsShRowData
'    Dim sContractDesc$, aGUnd As EtsGeneralLib.UndAtom
'
'    m_bInProc = True
'    nUndID = 0
'    nTrdID = 0
'
'    If m_enMenuGrid = GT_STOCKHEDGE_POSITIONS Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
'            Set aRowData = fgPos.RowData(m_nMenuGridRow)
'        End If
'
'        If Not aRowData Is Nothing Then
'            If Not aRowData.TrdGrp Is Nothing Then _
'                nTrdID = aRowData.TrdGrp.Trader.ID
'
'            If Not aRowData.UndGrp Is Nothing Then _
'                nUndID = aRowData.UndGrp.Und.ID
'
'            Set aRowData = Nothing
'        End If
'
'    ElseIf m_enMenuGrid = GT_STOCKHEDGE_FILTER Then
'        nUndID = m_nFilter(SFC_UNDERLYING)
'        nTrdID = m_nFilter(SFC_TRADER)
'    End If
'
'    m_bInProc = False
'    If nUndID <> 0 Then
'        Set aGUnd = g_Stock(nUndID)
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
'        Case GT_STOCKHEDGE_FILTER
'            frmPrintPreview.Execute m_frmOwner, "Stock Hedge Filter", "Stock Hedge Filter", fgFlt
'
'        Case GT_STOCKHEDGE_TOTALS
'            frmPrintPreview.Execute m_frmOwner, "Stock Hedge Filter", "Stock Hedge Filter", fgTot
'
'        Case GT_STOCKHEDGE_POSITIONS
'            frmPrintPreview.Execute m_frmOwner, "Stock Hedge Underlyings", "Stock Hedge Underlyings", fgPos
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
'    SetRefreshHint False
'
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
'    Dim bSucceeded As Boolean
'    bSucceeded = HedgeDataLoad
'    If bSucceeded Then bSucceeded = PositionsLoad
'
'    If bSucceeded Then
'        PositionsUpdate True
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
'Private Sub HedgeDataReadRow(rs As ADODB.Recordset)
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'    Dim aTrader As EtsGeneralLib.EtsTraderAtom
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
'    Dim aUnd As EtsGeneralLib.UndAtom
'    Dim nStrategyID&, nUndID&
'
'    Set aTrader = g_Trader(ReadLng(rs!iTraderID))
'    If Not aTrader Is Nothing Then
'        nStrategyID = ReadLng(rs!iStrategyID)
'        If nStrategyID <> 0 Then
'            Set aStrategy = g_Strategy(nStrategyID)
'        Else
'            nStrategyID = NO_STRATEGY_ID
'        End If
'
'        nUndID = ReadLng(rs!iContractID)
'        Set aUnd = g_Stock(nUndID)
'
'        If nStrategyID = NO_STRATEGY_ID And aUnd Is Nothing _
'            Or (nStrategyID = NO_STRATEGY_ID Or Not aStrategy Is Nothing) And Not aUnd Is Nothing Then
'
'            Set aTrdGrp = m_Grp(aTrader.ID)
'            If aTrdGrp Is Nothing Then
'                Set aTrdGrp = m_Grp.Add(aTrader.ID, aTrader.Name)
'                Set aTrdGrp.Trader = aTrader
'                aTrdGrp.Status = DEFAULT_TRADER_HEDGE_STATUS
'            End If
'
'            If Not aUnd Is Nothing Then
'                Set aStgGrp = aTrdGrp.Strategy(nStrategyID)
'                If aStgGrp Is Nothing Then
'                    If nStrategyID = NO_STRATEGY_ID Then
'                        Set aStrategy = New EtsGeneralLib.EtsStrategyAtom
'                        aStrategy.ID = NO_STRATEGY_ID
'                        aStrategy.Name = NO_STRATEGY_NAME
'                    End If
'
'                    Set aStgGrp = aTrdGrp.Strategy.Add(aStrategy.ID, aStrategy.Name)
'                    Set aStgGrp.Strategy = aStrategy
'                End If
'
'                Set aUndGrp = aStgGrp.Und(aUnd.ID)
'                If aUndGrp Is Nothing Then
'                    Set aUndGrp = aStgGrp.Und.Add(aUnd.ID, aUnd.Symbol)
'                    Set aUndGrp.Und = aUnd
'
'                    If aTrdGrp.Status = enMmShGroupStatusChilds Then
'                        aUndGrp.Status = IIf(ReadByte(rs!tiState) <> 0, enMmShGroupStatusAll, enMmShGroupStatusNone)
'                    Else
'                        aUndGrp.Status = aTrdGrp.Status
'                    End If
'                    Set aUndGrp = Nothing
'                End If
'
'                Set aStgGrp = Nothing
'
'            ElseIf nUndID = 0 Then
'                aTrdGrp.Status = ReadByte(rs!tiState)
'            End If
'            Set aTrdGrp = Nothing
'        End If
'
'        Set aTrader = Nothing
'    End If
'End Sub
'
'Private Function HedgeDataLoad() As Boolean
'    On Error GoTo EH
'    HedgeDataLoad = False
'    If m_bInProc Then Exit Function
'
'    Dim rs As ADODB.Recordset
'
'    m_bInProc = True
'    m_bDataLoad = True
'    AdjustState
'
'    If m_bDataLoad Then
'        lblStatus.Visible = False
'        imgStop.Visible = True
'        imgStopDis.Visible = False
'        pbProgress.Min = 0
'        pbProgress.Value = 0
'        pbProgress.Visible = True
'        lblProcess.Caption = "Hedge maintenance data loading..."
'        lblProcess.Visible = True
'        lblProcess.Refresh
'
'        Set rs = gDBW.usp_MmHedgeData_Get
'        If rs.RecordCount > 0 Then pbProgress.Max = rs.RecordCount
'
'        Do While Not rs.EOF
'            HedgeDataReadRow rs
'
'            DoEvents
'            If Not m_bDataLoad Then Exit Do
'            IncProgress pbProgress
'
'            rs.MoveNext
'        Loop
'
'        HedgeDataLoad = m_bDataLoad
'    End If
'
'    m_bInProc = False
'EX:
'    On Error Resume Next
'    Set rs = Nothing
'    m_bDataLoad = False
'    AdjustState
'    Exit Function
'EH:
'    m_bInProc = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load stock hedge maintenance data."
'    GoTo EX
'End Function
'
'Private Function CheckHedgeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
'    On Error Resume Next
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'
'    CheckHedgeFilter = (DEFAULT_TRADER_HEDGE_STATUS <> enMmShGroupStatusNone)
'
'    Set aTrdGrp = m_Grp(aTrd.Trader.ID)
'    If Not aTrdGrp Is Nothing Then
'        Set aStgGrp = aTrdGrp.Strategy(IIf(aTrd.StrategyID <> 0, aTrd.StrategyID, NO_STRATEGY_ID))
'
'        If Not aStgGrp Is Nothing Then
'            Set aUndGrp = aStgGrp.Und(aTrd.UndID)
'
'            If Not aUndGrp Is Nothing Then
'                CheckHedgeFilter = (aUndGrp.Status <> enMmShGroupStatusNone)
'                Set aUndGrp = Nothing
'            End If
'
'            Set aStgGrp = Nothing
'        End If
'
'        Set aTrdGrp = Nothing
'    End If
'End Function
'
'Private Function PositionsLoad() As Boolean
'    On Error GoTo EH
'    PositionsLoad = False
'    If m_bInProc Then Exit Function
'
'    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'    Dim aPos As EtsMmStockHedgeLib.MmShPosAtom
'    Dim aUnd As EtsMmStockHedgeLib.MmShUndAtom
'    Dim aOpt As EtsMmStockHedgeLib.MmShOptAtom
'    Dim aOptRoot As EtsMmStockHedgeLib.MmShOptRootAtom
'    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom
'    Dim aSynthUnd As EtsMmStockHedgeLib.MmShUndAtom
'    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
'    Dim aGUnd As EtsGeneralLib.UndAtom
'    Dim sKey$
'
'    m_bInProc = True
'    m_bDataLoad = True
'    AdjustState
'
'    If m_bDataLoad Then
'        lblStatus.Visible = False
'        imgStop.Visible = True
'        imgStopDis.Visible = False
'        pbProgress.Min = 0
'        pbProgress.Value = 0
'        pbProgress.Max = g_TradeChannel.Trades.Count
'        pbProgress.Visible = True
'        lblProcess.Caption = "Data loading..."
'        lblProcess.Visible = True
'        lblProcess.Refresh
'
'        For Each aTrd In g_TradeChannel.Trades
'            If CheckTradeFilter(aTrd) Then
'                If CheckHedgeFilter(aTrd) Then
'                    Set aTrdGrp = m_Grp(aTrd.Trader.ID)
'                    If aTrdGrp Is Nothing Then
'                        Set aTrdGrp = m_Grp.Add(aTrd.Trader.ID, aTrd.Trader.Name)
'                        Set aTrdGrp.Trader = aTrd.Trader
'                        aTrdGrp.Status = DEFAULT_TRADER_HEDGE_STATUS
'                    End If
'
'                    Set aStgGrp = aTrdGrp.Strategy(IIf(aTrd.StrategyID <> 0, aTrd.StrategyID, NO_STRATEGY_ID))
'                    If aStgGrp Is Nothing Then
'                        If aTrd.StrategyID = 0 Then
'                            Set aStgGrp = aTrdGrp.Strategy.Add(NO_STRATEGY_ID, NO_STRATEGY_NAME)
'                            Set aStgGrp.Strategy = New EtsGeneralLib.EtsStrategyAtom
'                            aStgGrp.Strategy.ID = NO_STRATEGY_ID
'                            aStgGrp.Strategy.Name = NO_STRATEGY_NAME
'                        Else
'                            Set aStgGrp = aTrdGrp.Strategy.Add(aTrd.StrategyID, aTrd.Strategy.Name)
'                            Set aStgGrp.Strategy = aTrd.Strategy
'                        End If
'                    End If
'
'                    Set aUndGrp = aStgGrp.Und(aTrd.UndID)
'                    If aUndGrp Is Nothing Then
'                        Set aUndGrp = aStgGrp.Und.Add(aTrd.UndID, aTrd.Und.Symbol)
'                        Set aUndGrp.Und = aTrd.Und
'                        If aTrdGrp.Status <> enMmShGroupStatusNone Then
'                            aUndGrp.Status = enMmShGroupStatusAll
'                        Else
'                            aUndGrp.Status = enMmShGroupStatusNone
'                        End If
'                    End If
'
'                    Set aUnd = m_Und(aTrd.UndID)
'                    If aUnd Is Nothing Then
'                        Set aUnd = m_Und.Add(aTrd.UndID)
'                        aUnd.ID = aTrd.Und.ID
'                        aUnd.Symbol = aTrd.Und.Symbol
'                        aUnd.UndType = aTrd.Und.UndType
'                        If Not aTrd.Und.UseCustDivs Then
'                            aUnd.DivFreq = aTrd.Und.DivFreq
'                            aUnd.DivDate = aTrd.Und.DivDate
'                            aUnd.DivAmt = aTrd.Und.DivAmt
'                        Else
'                            aUnd.DivFreq = aTrd.Und.DivFreqCust
'                            aUnd.DivDate = aTrd.Und.DivDateCust
'                            aUnd.DivAmt = aTrd.Und.DivAmtCust
'                        End If
'
'                        aUnd.IsHTB = aTrd.Und.IsHTB
'                        aUnd.IsAmerican = aTrd.Und.IsAmerican
'                        aUnd.Skew = aTrd.Und.Skew
'                        aUnd.Kurt = aTrd.Und.Kurt
'                        aUnd.UndPosForRates = aTrd.Und.UndPosForRates
'                        aUnd.TotalQtyInShares = aTrd.Und.TotalQtyInShares
'
'                        Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
'                        Set aUnd.OptPriceProfile = aTrd.Und.OptPriceProfile
'
'                        InitVola aUnd
'
'                        sKey = CStr(IIf(aUnd.UndType = enCtStock, enCtStock, enCtIndex)) & "_" & aUnd.Symbol
'                        If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, aUnd, Nothing
'                    End If
'
'                    If aTrd.ContractType = enCtOption Then
'                        Set aOptRoot = aUnd.OptRoot(aTrd.OptRootID)
'                        If aOptRoot Is Nothing Then
'                            Set aOptRoot = aUnd.OptRoot.Add(aTrd.OptRootID)
'                            aOptRoot.ID = aTrd.OptRootID
'                            aOptRoot.LotSize = aTrd.OptRoot.LotSize
'
'                            If aTrd.OptRoot.IsSynth And aTrd.Und.HaveSyntheticRoots And Not aTrd.Und.SyntheticRoots Is Nothing Then
'                                Set aSynthRoot = aTrd.Und.SyntheticRoots(aOptRoot.ID)
'
'                                If Not aSynthRoot Is Nothing Then
'                                    aOptRoot.IsSynth = True
'                                    Set aOptRoot.SynthOptRoot = aSynthRoot
'
'                                    For Each aSynthRootComp In aSynthRoot.SynthRootComponents
'                                        If aSynthRootComp.UndID <> aUnd.ID Then
'
'                                            Set aSynthUnd = m_Und(aSynthRootComp.UndID)
'                                            Set aGUnd = g_UnderlyingAll(aSynthRootComp.UndID)
'
'                                            If Not aGUnd Is Nothing Then
'                                                If aSynthUnd Is Nothing Then
'
'                                                    Set aSynthUnd = m_Und.Add(aSynthRootComp.UndID)
'                                                    aSynthUnd.ID = aSynthRootComp.UndID
'
'                                                    aSynthUnd.UndType = aGUnd.UndType
'                                                    aSynthUnd.Symbol = aGUnd.Symbol
'
'                                                    'If aSynthUnd.UndType = enCtIndex Then
'                                                    '    Set aSynthUnd.BasketIndex = g_Index(aSynthUnd.ID)
'                                                    '    If Not aSynthUnd.BasketIndex Is Nothing Then
'                                                    '        If Not aSynthUnd.BasketIndex.IsBasket Then Set aSynthUnd.BasketIndex = Nothing
'                                                    '    End If
'                                                    '
'                                                    '    aSynthUnd.Yield = aGUnd.Yield
'                                                    'End If
'
'                                                    If aSynthUnd.UndType = enCtStock Then
'                                                        If aGUnd.UseCustDivs Then
'                                                            aSynthUnd.DivFreq = aGUnd.DivFreqCust
'                                                            aSynthUnd.DivDate = aGUnd.DivDateCust
'                                                            aSynthUnd.DivAmt = aGUnd.DivAmtCust
'                                                        Else
'                                                            aSynthUnd.DivFreq = aGUnd.DivFreq
'                                                            aSynthUnd.DivDate = aGUnd.DivDate
'                                                            aSynthUnd.DivAmt = aGUnd.DivAmt
'                                                        End If
'                                                    End If
'
'                                                    aSynthUnd.IsAmerican = aGUnd.IsAmerican
'                                                    aSynthUnd.IsHTB = aGUnd.IsHTB
'                                                    aSynthUnd.Skew = aGUnd.Skew
'                                                    aSynthUnd.Kurt = aGUnd.Kurt
'                                                    Set aSynthUnd.UndPriceProfile = aGUnd.UndPriceProfile
'                                                    Set aSynthUnd.OptPriceProfile = aGUnd.OptPriceProfile
'
'                                                    InitVola aSynthUnd
'
'                                                    sKey = CStr(IIf(aSynthUnd.UndType = enCtStock, enCtStock, enCtIndex)) & "_" & aSynthUnd.Symbol
'                                                    If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, aSynthUnd, Nothing
'                                                End If
'                                            End If
'
'                                            Set aSynthUnd = Nothing
'                                        End If
'
'                                        Set aGUnd = Nothing
'                                    Next
'                                    Set aSynthRoot = Nothing
'                                End If
'                            End If
'                        End If
'
'                        Set aOpt = aUnd.Opt(aTrd.ContractID)
'                        If aOpt Is Nothing Then
'                            Set aOpt = aUnd.Opt.Add(aTrd.ContractID)
'                            aOpt.ID = aTrd.ContractID
'                            aOpt.Symbol = aTrd.Opt.Symbol
'                            aOpt.Expiry = aTrd.Opt.Expiry
'                            aOpt.OptType = aTrd.Opt.OptType
'                            aOpt.Strike = aTrd.Opt.Strike
'                            Set aOpt.OptRoot = aOptRoot
'
'                            aOpt.Vola = aUnd.VolaSrv.OptionVola(aOpt.Expiry, aOpt.Strike)
'
'                            sKey = CStr(enCtOption) & "_" & aOpt.Symbol
'                            If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, aOpt
'                        End If
'                    End If
'
'                    Set aPos = aUndGrp.Pos(aTrd.ContractID)
'                    If aPos Is Nothing Then
'                        Set aPos = aUndGrp.Pos.Add(aTrd.ContractID)
'                        aPos.ID = aTrd.ContractID
'                        Set aPos.Und = aUnd
'                        Set aPos.Opt = aOpt
'                    End If
'
'                    If aPos.Qty = BAD_LONG_VALUE Then aPos.Qty = 0
'                    aPos.Qty = aPos.Qty + IIf(aTrd.IsBuy, 1, -1) * aTrd.Quantity * aTrd.LotSize
'
'                    Set aPos = Nothing
'                    Set aOptRoot = Nothing
'                    Set aOpt = Nothing
'                    Set aUnd = Nothing
'                    Set aPos = Nothing
'                    Set aUndGrp = Nothing
'                    Set aStgGrp = Nothing
'                    Set aTrdGrp = Nothing
'                End If
'            End If
'
'            DoEvents
'            If Not m_bDataLoad Then Exit For
'            IncProgress pbProgress
'        Next
'
'        UnderlyingsAdjustRates True
'
'        PositionsLoad = m_bDataLoad
'    End If
'
'    m_bInProc = False
'EX:
'    On Error Resume Next
'    m_bDataLoad = False
'    AdjustState
'    Exit Function
'EH:
'    m_bInProc = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load positions."
'    GoTo EX
'End Function
'
'Private Sub PositionsUpdate(ByVal bReload As Boolean)
'    On Error Resume Next
'    Dim i&, nRow&
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'    Dim aRowData As clsShRowData
'
'    Dim clsShRowData As clsShRowData
'
'    With fgPos
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).LockRedraw
'
'        If bReload Then
'            .Rows = 1
'            For Each aTrdGrp In m_Grp
'                For Each aStgGrp In aTrdGrp.Strategy
'                    For Each aUndGrp In aStgGrp.Und
'                        If aUndGrp.Pos.Count > 0 Then
'                            .AddItem ""
'                            nRow = .Rows - 1
'
'                            Set aRowData = New clsShRowData
'                            Set aRowData.TrdGrp = aTrdGrp
'                            Set aRowData.StgGrp = aStgGrp
'                            Set aRowData.UndGrp = aUndGrp
'
'                            .RowData(nRow) = aRowData
'                            .TextMatrix(nRow, SPC_KEY) = 0
'                            PositionUpdate nRow, True, aRowData
'
'                            Set aRowData = Null
'                        End If
'                    Next
'                Next
'            Next
'
'            If .Rows > 1 Then .Row = 1
'
'            .Col = SPC_TRADER
'            .Row = 1
'            .Sort = flexSortGenericAscending
'            gePos.ShowSortImage SPC_TRADER, 1
'
'            FormatPosColumns
'        Else
'            For nRow = 1 To .Rows - 1
'                Set aRowData = .RowData(nRow)
'                PositionUpdate nRow, False, aRowData
'                Set aRowData = Nothing
'            Next
'        End If
'
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub PositionUpdate(ByVal nRow As Long, ByVal bSymbol As Boolean, ByRef aRowData As clsShRowData)
'    On Error Resume Next
'    Dim nCol&, sValue$
'
'    With fgPos
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).LockRedraw
'
'        If aRowData Is Nothing Then Set aRowData = .RowData(nRow)
'
'        If Not aRowData Is Nothing Then
'            Dim i&, nIdx&
'            i = 0
'            nIdx = m_gdPos.Idx(0)
'            While nIdx >= 0 And i <= SPC_LAST_COLUMN
'                nCol = i + 1
'                If bSymbol Then
'                    Select Case nIdx
'                        Case SPC_TRADER
'                            .TextMatrix(nRow, nCol) = aRowData.TrdGrp.Trader.Name
'
'                        Case SPC_STRATEGY
'                            .TextMatrix(nRow, nCol) = aRowData.StgGrp.Strategy.Name
'
'                        Case SPC_SYMBOL
'                            .TextMatrix(nRow, nCol) = aRowData.UndGrp.Und.Symbol
'                    End Select
'                End If
'
'                Select Case nIdx
'                    Case SPC_NET_DELTA
'                        .TextMatrix(nRow, nCol) = IIf(aRowData.UndGrp.NetDelta > BAD_DOUBLE_VALUE, aRowData.UndGrp.NetDelta, STR_NA)
'                        '.TextMatrix(nRow, nCol) = IIf(aRowData.UndGrp.DeltaQty > BAD_LONG_VALUE, aRowData.UndGrp.DeltaQty, STR_NA)
'
'                    Case SPC_HEDGE_POS
'                        .TextMatrix(nRow, nCol) = IIf(aRowData.UndGrp.HedgeQty > BAD_LONG_VALUE, aRowData.UndGrp.HedgeQty, STR_NA)
'                End Select
'
'                i = i + 1
'                nIdx = m_gdPos.Idx(i)
'            Wend
'        End If
'
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub CalcPositions()
'    On Error Resume Next
'    If m_bInProc Or m_Und.Count = 0 Then Exit Sub
'
'    Dim aUnd As EtsMmStockHedgeLib.MmShUndAtom
'    Dim aOpt As EtsMmStockHedgeLib.MmShOptAtom
'    Dim aPos As EtsMmStockHedgeLib.MmShPosAtom
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
'        pbProgress.Max = m_Und.Count
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
'        For Each aUnd In m_Und
'            aUnd.CalcAllOptions m_Und, g_Params.CalcModel, _
'                            g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
'                            g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
'                            nCalcSleepFreq, nCalcSleepAmt
'            DoEvents
'            If Not m_bRecalc Then Exit For
'            IncProgress pbProgress
'        Next
'
'        CalcTotals
'    End If
'    m_bRecalc = False
'    m_bInProc = False
'    AdjustState
'End Sub
'
'Private Sub CalcTotals(Optional ByVal bWithRowTotals As Boolean = True)
'    On Error Resume Next
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'    Dim aPos As EtsMmStockHedgeLib.MmShPosAtom
'    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom
'    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
'
'    If bWithRowTotals Then m_dTotalUndNetDelta = BAD_DOUBLE_VALUE
'    m_nTotalHedgePos = BAD_LONG_VALUE
'
'    For Each aTrdGrp In m_Grp
'        For Each aStgGrp In aTrdGrp.Strategy
'            For Each aUndGrp In aStgGrp.Und
'                If aUndGrp.Pos.Count > 0 Then
'                    If bWithRowTotals Then
'                        aUndGrp.NetDelta = BAD_DOUBLE_VALUE
'                        aUndGrp.DeltaQty = BAD_LONG_VALUE
'                        aUndGrp.HedgeQty = BAD_LONG_VALUE
'
'                        For Each aPos In aUndGrp.Pos
'                            aPos.Delta = BAD_DOUBLE_VALUE
'                            If Not aPos.Opt Is Nothing Then
'                                If aPos.Opt.Delta > BAD_DOUBLE_VALUE Then
'                                    Set aSynthRoot = aPos.Opt.OptRoot.SynthOptRoot
'                                    If Not aPos.Opt.OptRoot.IsSynth Or aSynthRoot Is Nothing Then
'                                        aPos.Delta = aPos.Qty * aPos.Opt.Delta
'                                    Else
'                                        Set aSynthRootComp = aSynthRoot.SynthRootComponents(aUndGrp.Und.ID)
'                                        If Not aSynthRootComp Is Nothing Then
'                                            aPos.Delta = aPos.Qty * aPos.Opt.Delta * aSynthRootComp.Weight
'                                            Set aSynthRootComp = Nothing
'                                        End If
'                                    End If
'                                    Set aSynthRoot = Nothing
'                                End If
'                            Else
'                                aPos.Delta = aPos.Qty
'                            End If
'
'                            If aPos.Delta > BAD_DOUBLE_VALUE Then
'                                If aUndGrp.NetDelta = BAD_DOUBLE_VALUE Then aUndGrp.NetDelta = 0&
'                                aUndGrp.NetDelta = aUndGrp.NetDelta + aPos.Delta
'                            End If
'                        Next
'                    End If
'
'                    If bWithRowTotals Then
'                        If aUndGrp.NetDelta > BAD_DOUBLE_VALUE Then
'                            aUndGrp.DeltaQty = aUndGrp.NetDelta * m_dSlice
'                            aUndGrp.HedgeQty = -Sgn(aUndGrp.DeltaQty) * Int(Abs(aUndGrp.DeltaQty) / m_nLotSize + 0.5) * m_nLotSize
'
'                            If m_dTotalUndNetDelta = BAD_DOUBLE_VALUE Then m_dTotalUndNetDelta = 0#
'                            m_dTotalUndNetDelta = m_dTotalUndNetDelta + aUndGrp.NetDelta
'                            If m_nTotalHedgePos = BAD_LONG_VALUE Then m_nTotalHedgePos = 0&
'                            m_nTotalHedgePos = m_nTotalHedgePos + aUndGrp.HedgeQty
'                        End If
'                    Else
'                        If aUndGrp.HedgeQty > BAD_LONG_VALUE Then
'                            If m_nTotalHedgePos = BAD_LONG_VALUE Then m_nTotalHedgePos = 0&
'                            m_nTotalHedgePos = m_nTotalHedgePos + aUndGrp.HedgeQty
'                        End If
'                    End If
'                End If
'            Next
'        Next
'    Next
'End Sub
'
'Private Sub UpdateTotals()
'    On Error Resume Next
'    With fgTot
'        m_GridLock(GT_STOCKHEDGE_TOTALS).LockRedraw
'
'        .TextMatrix(1, STC_SLICE) = m_dSlice * 100#
'        .TextMatrix(1, STC_LOT_SIZE) = m_nLotSize
'        .TextMatrix(1, STC_MAX_SIZE) = m_nMaxSize
'
'        .TextMatrix(1, STC_NET_DELTA) = IIf(m_dTotalUndNetDelta > BAD_DOUBLE_VALUE, m_dTotalUndNetDelta, STR_NA)
'        .TextMatrix(1, STC_HEDGE_POS) = IIf(m_nTotalHedgePos > BAD_LONG_VALUE, m_nTotalHedgePos, STR_NA)
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_STOCKHEDGE_TOTALS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub UnderlyingsAdjustRates(ByVal bForceRateUpdate As Boolean)
'    On Error Resume Next
'    Dim aUnd As EtsMmStockHedgeLib.MmShUndAtom
'
'    For Each aUnd In m_Und
'        UnderlyingAdjustRates aUnd, bForceRateUpdate
'    Next
'End Sub
'
'Private Function UnderlyingAdjustRates(aUnd As EtsMmStockHedgeLib.MmShUndAtom, ByVal bForceRateUpdate As Boolean) As Boolean
'    On Error Resume Next
'    Dim aOpt As EtsMmStockHedgeLib.MmShOptAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
'
'    UnderlyingAdjustRates = False
'    If aUnd Is Nothing Then Exit Function
'
'    dPos = aUnd.UndPosForRates
'
'    If GetIrRuleType = enRateBasedOnPosition Then
'        cPosThreshold = Abs(GetPosThreshold)
'        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
'
'        If Not bForceRateUpdate And Not bUseMidRates Then _
'            bForceRateUpdate = (Sgn(dPos) * Sgn(aUnd.UndPosForRates) < 0)
'    Else
'        bUseMidRates = True
'    End If
'
'    If bForceRateUpdate Or aUnd.UseMidRates <> bUseMidRates Then
'        aUnd.UseMidRates = bUseMidRates
'        aUnd.UndPosForRates = dPos
'
'        For Each aOpt In aUnd.Opt
'            If bUseMidRates Then
'                If Not aUnd.IsHTB Then
'                    aOpt.Rate = GetNeutralRate(Date, aOpt.Expiry)
'                Else
'                    aOpt.Rate = GetNeutralHTBRate(Date, aOpt.Expiry)
'                End If
'            Else
'                If Not aUnd.IsHTB Then
'                    aOpt.Rate = IIf(dPos < 0#, GetShortRate(Date, aOpt.Expiry), GetLongRate(Date, aOpt.Expiry))
'                Else
'                    aOpt.Rate = IIf(dPos < 0#, GetHTBRate(Date, aOpt.Expiry), GetLongRate(Date, aOpt.Expiry))
'                End If
'            End If
'        Next
'        UnderlyingAdjustRates = True
'    End If
'End Function
'
'Private Sub RequestLastQuotes()
'    On Error GoTo EH
'    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aReq As clsShRequestAtom
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
'                        PriceProvider.RequestLastQuote aParam
'                        m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                        DoEvents
'
'                        If Not m_bLastQuoteReqNow Then Exit For
'
'                        If aReq.Und.Opt.Count > 0 Then
'                            aParam.Type = IIf(aParam.Type = enSTK, enGrSTK, enGrIDX)
'                            m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
'                            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                            DoEvents
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
'    Dim aReq As clsShRequestAtom, bFinished As Boolean
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
'                CalcPositions
'                PositionsUpdate False
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
'    Dim aReq As clsShRequestAtom, sKey$
'    Dim aUnd As EtsMmStockHedgeLib.MmShUndAtom, aOpt As EtsMmStockHedgeLib.MmShOptAtom
'    Dim dPriceBid#, dPriceAsk#, dPriceLast#
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
'
'            If Not aOpt Is Nothing Then
'                aOpt.PriceBid = dPriceBid
'                aOpt.PriceAsk = dPriceAsk
'                aOpt.PriceLast = dPriceLast
'            ElseIf Not aUnd Is Nothing Then
'                aUnd.PriceBid = dPriceBid
'                aUnd.PriceAsk = dPriceAsk
'                aUnd.PriceLast = dPriceLast
'            End If
'
'            Set aOpt = Nothing
'            Set aUnd = Nothing
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
'        CalcPositions
'        PositionsUpdate False
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
'Private Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean, nValue&
'
'    bMatched = (aTrd.Und.UndType = enCtStock And aTrd.FutRootID = 0)
'    If Not NoFilter Then
'        If bMatched Then
'            nValue = m_nFilter(SFC_UNDERLYING)
'            bMatched = (nValue = 0 Or nValue = aTrd.UndID)
'        End If
'
'        If bMatched Then
'            nValue = m_nFilter(SFC_TRADER)
'            bMatched = (nValue = 0)
'            If Not bMatched Then
'                bMatched = (aTrd.Trader.ID = nValue)
'            End If
'        End If
'
'        If bMatched Then
'            nValue = m_nFilter(SFC_STRATEGY)
'            bMatched = (nValue = 0)
'            If Not bMatched Then
'                bMatched = (aTrd.StrategyID = IIf(nValue > 0, nValue, 0))
'            End If
'        End If
'
'        CheckTradeFilter = bMatched
'    Else
'        CheckTradeFilter = bMatched
'    End If
'End Function
'
'Private Sub FilterUpdate()
'    On Error Resume Next
'    Dim nValue&
'
'    With fgFlt
'        m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
'
'        nValue = m_nFilter(SFC_TRADER)
'        If g_Trader(nValue) Is Nothing Then nValue = 0
'        m_nFilter(SFC_TRADER) = nValue
'        .TextMatrix(1, SFC_TRADER) = nValue
'
'        nValue = m_nFilter(SFC_STRATEGY)
'        If nValue > 0 Then _
'            If g_Strategy(nValue) Is Nothing Then nValue = 0
'        m_nFilter(SFC_STRATEGY) = nValue
'        .TextMatrix(1, SFC_STRATEGY) = nValue
'
'        nValue = m_nFilter(SFC_UNDERLYING)
'        If g_Stock(nValue) Is Nothing Then nValue = 0
'        m_nFilter(SFC_UNDERLYING) = nValue
'        .TextMatrix(1, SFC_UNDERLYING) = nValue
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub InitFltData()
'    On Error Resume Next
'    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom, nValue&
'    Dim aTrader As EtsGeneralLib.EtsTraderAtom
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
'
'    With fgFlt
'        m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = "#0;<All>"
'        For Each aTrader In g_Trader
'            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
'            If aTrader.ID = m_nFilter(SFC_TRADER) Then
'                nValue = aTrader.ID
'                sValue = Trim$(Str$(nValue))
'            End If
'        Next
'        .ColComboList(SFC_TRADER) = sComboList
'        .TextMatrix(1, SFC_TRADER) = sValue
'        m_nFilter(SFC_TRADER) = nValue
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = "#0;<All>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
'        If m_nFilter(SFC_STRATEGY) = NO_STRATEGY_ID Then
'            nValue = NO_STRATEGY_ID
'            sValue = NO_STRATEGY_NAME
'        End If
'
'        For Each aStrategy In g_Strategy
'            sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
'            If aStrategy.ID = m_nFilter(SFC_STRATEGY) Then
'                nValue = aStrategy.ID
'                sValue = Trim$(Str$(aStrategy.ID))
'            End If
'        Next
'        .ColComboList(SFC_STRATEGY) = sComboList
'        .TextMatrix(1, SFC_STRATEGY) = sValue
'        m_nFilter(SFC_STRATEGY) = nValue
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = g_Params.StockComboList
'        If Len(sComboList) > 0 Then
'            sComboList = "#0;<All>|" & sComboList
'        Else
'            sComboList = "#0;<All>"
'        End If
'
'        Set aUnd = g_Underlying(m_nFilter(SFC_UNDERLYING))
'        If Not aUnd Is Nothing Then
'            nValue = aUnd.ID
'            sValue = Trim$(Str$(aUnd.ID))
'            Set aUnd = Nothing
'        End If
'        .ColComboList(SFC_UNDERLYING) = sComboList
'        .TextMatrix(1, SFC_UNDERLYING) = sValue
'        m_nFilter(SFC_UNDERLYING) = nValue
'
'        m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Function NoFilter() As Boolean
'    On Error Resume Next
'    NoFilter = Not (m_nFilter(SFC_TRADER) <> 0 _
'                    Or m_nFilter(SFC_STRATEGY) <> 0 _
'                    Or m_nFilter(SFC_UNDERLYING) <> 0)
'End Function
'
'Private Sub InitColumns()
'    On Error Resume Next
'    g_DefGrids(GT_STOCKHEDGE_FILTER).CopyTo m_gdFlt
'    g_DefGrids(GT_STOCKHEDGE_TOTALS).CopyTo m_gdTot
'    g_DefGrids(GT_STOCKHEDGE_POSITIONS).CopyTo m_gdPos
'End Sub
'
'Private Sub InitGrids()
'    On Error Resume Next
'    With fgFlt
'        .Rows = 2
'        .Cols = SFC_COLUMN_COUNT
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
'        .Cols = STC_COLUMN_COUNT
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
'    With fgPos
'        .Rows = 1
'        .Cols = SPC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbdMouse
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
'        m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
'
'        nCols = SFC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = SFC_TRADER To SFC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
'            .ColFormat(i) = m_gdFlt.Col(i).Format
'            .ColDataType(i) = m_gdFlt.Col(i).DataType
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatFltColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont
'
'    With fgFlt
'        m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
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
'        For i = SFC_TRADER To SFC_LAST_COLUMN
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
'        m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatTotGrid()
'    On Error Resume Next
'    Dim i&, nCols&
'
'    With fgTot
'        m_GridLock(GT_STOCKHEDGE_TOTALS).LockRedraw
'
'        nCols = STC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = STC_SLICE To STC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gdTot.Col(i).Caption
'            .ColFormat(i) = m_gdTot.Col(i).Format
'            .ColDataType(i) = m_gdTot.Col(i).DataType
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_STOCKHEDGE_TOTALS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatTotColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont
'
'    With fgTot
'        m_GridLock(GT_STOCKHEDGE_TOTALS).LockRedraw
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
'        For i = STC_SLICE To STC_LAST_COLUMN
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
'        m_GridLock(GT_STOCKHEDGE_TOTALS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatPosGrid()
'    On Error Resume Next
'    Dim i&, nCols&
'
'    With fgPos
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).LockRedraw
'
'        nCols = SPC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = SPC_TRADER To SPC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gdPos.Col(i).Caption
'            .ColFormat(i) = m_gdPos.Col(i).Format
'            .ColDataType(i) = m_gdPos.Col(i).DataType
'            .ColWidth(i) = IIf(m_gdPos.Col(i).Width > 0, m_gdPos.Col(i).Width, -1)
'            .ColKey(i) = i
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'        gePos.AdjustSortImages
'
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatPosColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont
'
'    With fgPos
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).LockRedraw
'
'        aFont.Name = m_gdPos.Font.Name
'        aFont.Size = m_gdPos.Font.Size
'        aFont.Bold = m_gdPos.Font.Bold
'        aFont.Italic = m_gdPos.Font.Italic
'        aFont.Strikethrough = m_gdPos.Font.Strikethrough
'        aFont.Underline = m_gdPos.Font.Underline
'
'        Set .Font = aFont
'
'        .ColHidden(SPC_KEY) = True
'        .ColDataType(SPC_KEY) = flexDTString
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = SPC_TRADER To SPC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdPos.Col(i).BackColor <> 0, m_gdPos.Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdPos.Col(i).ForeColor <> 0, m_gdPos.Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If m_gdPos.HorizLinesVisible Then
'            If m_gdPos.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdPos.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdPos.GridLinesColor
'
'        .BackColorSel = m_gdPos.BackColorSel
'        .ForeColorSel = m_gdPos.ForeColorSel
'
'        '.AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_STOCKHEDGE_POSITIONS).UnlockRedraw
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
'    Dim bUpdate As Boolean, aFilterAtom As clsFilterAtom
'
'    bUpdate = False
'
'    For Each aFilterAtom In collUpdUnd
'        If Not m_Und(aFilterAtom.ID) Is Nothing Then
'            bUpdate = True
'            Set aFilterAtom = Nothing
'            Exit For
'        End If
'        Set aFilterAtom = Nothing
'    Next
'    If bUpdate Then SetRefreshHint True
'End Sub
'
'Private Sub aParams_UndPriceToleranceChange()
'    SetRefreshHint True
'End Sub
'
'Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
'    On Error Resume Next
'    Dim aUnd As EtsGeneralLib.UndAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(SFC_UNDERLYING) <> nNewUndID Then
'        Set aUnd = g_Stock(nNewUndID)
'        If Not aUnd Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
'
'                sValue = "0"
'                nValue = 0
'
'                sComboList = g_Params.StockComboList
'                If Len(sComboList) > 0 Then
'                    sComboList = "#0;<All>|" & sComboList
'                Else
'                    sComboList = "#0;<All>"
'                End If
'
'                Set aUnd = g_Stock(m_nFilter(SFC_UNDERLYING))
'                If Not aUnd Is Nothing Then
'                    nValue = aUnd.ID
'                    sValue = Trim$(Str$(aUnd.ID))
'                    Set aUnd = Nothing
'                End If
'                .ColComboList(SFC_UNDERLYING) = sComboList
'                .TextMatrix(1, SFC_UNDERLYING) = sValue
'                m_nFilter(SFC_UNDERLYING) = nValue
'
'                m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
'            End With
'        End If
'    End If
'End Sub
'
'Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
'    On Error Resume Next
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(SFC_STRATEGY) <> nStrategyID Then
'        Set aStrategy = g_Strategy(nStrategyID)
'        If Not aStrategy Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
'
'                sValue = "0"
'                nValue = 0
'                sComboList = "#0;<All>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
'                If m_nFilter(SFC_STRATEGY) = NO_STRATEGY_ID Then
'                    nValue = NO_STRATEGY_ID
'                    sValue = NO_STRATEGY_NAME
'                End If
'
'                For Each aStrategy In g_Strategy
'                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
'                    If aStrategy.ID = m_nFilter(SFC_STRATEGY) Then
'                        nValue = aStrategy.ID
'                        sValue = Trim$(Str$(aStrategy.ID))
'                    End If
'                Next
'                .ColComboList(SFC_STRATEGY) = sComboList
'                .TextMatrix(1, SFC_STRATEGY) = sValue
'                m_nFilter(SFC_STRATEGY) = nValue
'
'                m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
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
'        m_enMenuGrid = GT_STOCKHEDGE_FILTER
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
'        m_bKeyDown(GT_STOCKHEDGE_FILTER) = True
'    End If
'End Sub
'
'Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_STOCKHEDGE_FILTER) Then
'        m_bKeyDown(GT_STOCKHEDGE_FILTER) = False
'        With fgFlt
'            m_enMenuGrid = GT_STOCKHEDGE_FILTER
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
'            m_enMenuGrid = GT_STOCKHEDGE_FILTER
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
'        Case GT_STOCKHEDGE_FILTER
'            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_nFilter(SFC_UNDERLYING) <> 0 _
'                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = False
'            mnuCtxAutosizeGrid.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_STOCKHEDGE_TOTALS
'            mnuCtxTradeNew.Enabled = False
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = False
'            mnuCtxAutosizeGrid.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_STOCKHEDGE_POSITIONS
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
'        Case GT_STOCKHEDGE_FILTER
'            gdGrid.CopyTo m_gdFlt
'
'            m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
'            FormatFltGrid
'            FormatFltColumns
'            m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
'
'        Case GT_STOCKHEDGE_TOTALS
'            gdGrid.CopyTo m_gdTot
'
'            m_GridLock(GT_STOCKHEDGE_TOTALS).LockRedraw
'            FormatTotGrid
'            FormatTotColumns
'            m_GridLock(GT_STOCKHEDGE_TOTALS).UnlockRedraw
'
'        Case GT_STOCKHEDGE_POSITIONS
'            gdGrid.CopyTo m_gdPos
'
'            m_GridLock(GT_STOCKHEDGE_POSITIONS).LockRedraw
'            FormatPosGrid
'            FormatPosColumns
'            m_GridLock(GT_STOCKHEDGE_POSITIONS).UnlockRedraw
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
'        Case GT_STOCKHEDGE_FILTER
'            g_ClipMgr.CopyGridToClipboard fgFlt
'
'        Case GT_STOCKHEDGE_TOTALS
'            g_ClipMgr.CopyGridToClipboard fgTot
'
'        Case GT_STOCKHEDGE_POSITIONS
'            g_ClipMgr.CopyGridToClipboard fgPos
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxGridLayout_Click()
'    On Error Resume Next
'    Select Case m_enMenuGrid
'        Case GT_STOCKHEDGE_FILTER
'            CustomizeFltGridLayout
'
'        Case GT_STOCKHEDGE_TOTALS
'            CustomizeTotGridLayout
'
'        Case GT_STOCKHEDGE_POSITIONS
'            CustomizePosGridLayout
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
'    With fgPos
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
'    Set frmMaintenance = Nothing
'    Set m_GroupPriceProvider = Nothing
'    Set m_Und = Nothing
'    Set m_Grp = Nothing
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
'    GetCaption = "Stock Hedge"
'End Function
'
'Public Function GetShortCaption() As String
'    On Error Resume Next
'    GetShortCaption = "Stock Hedge"
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
'        For i = SFC_TRADER To SFC_LAST_COLUMN
'            m_nFilterPrev(i) = m_nFilter(i)
'        Next
'    Else
'        For i = SFC_TRADER To SFC_LAST_COLUMN
'            If m_nFilterPrev(i) <> BAD_LONG_VALUE Then m_nFilter(i) = m_nFilterPrev(i)
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
'        m_GridLock(GT_STOCKHEDGE_FILTER).LockRedraw
'
'        For i = SFC_TRADER To SFC_LAST_COLUMN
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
'        m_GridLock(GT_STOCKHEDGE_FILTER).UnlockRedraw
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
'        For i = SFC_TRADER To SFC_LAST_COLUMN
'            If m_nFilter(i) <> m_nFilterPrev(i) And m_nFilterPrev(i) <> BAD_LONG_VALUE Then
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
'    fgPos.Rows = 1
'
'    m_Und.Clear
'    m_Grp.Clear
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
'    For i = SFC_TRADER To SFC_LAST_COLUMN
'        aStorage.SetLongValue "StockHedgeFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
'    Next
'
'    aStorage.SetDoubleValue "StockHedgeTot" & sKey, "Slice", m_dSlice
'    aStorage.SetLongValue "StockHedgeTot" & sKey, "LotSize", m_nLotSize
'    aStorage.SetLongValue "StockHedgeTot" & sKey, "MaxSize", m_nMaxSize
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
'    For i = SFC_TRADER To SFC_LAST_COLUMN
'        m_nFilter(i) = aStorage.GetLongValue("StockHedgeFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
'    Next
'
'    m_dSlice = aStorage.GetDoubleValue("StockHedgeTot" & sKey, "Slice", m_dSlice)
'    If m_dSlice < 0.01 Then
'        m_dSlice = 0.01
'    ElseIf m_dSlice > 1# Then
'        m_dSlice = 1#
'    End If
'
'    m_nLotSize = aStorage.GetLongValue("StockHedgeTot" & sKey, "LotSize", m_nLotSize)
'    If m_nLotSize <= 0 Then _
'        m_nLotSize = 100
'
'    m_nMaxSize = aStorage.GetLongValue("StockHedgeTot" & sKey, "MaxSize", m_nMaxSize)
'    If m_nMaxSize <= 0 Then _
'        m_nMaxSize = 10000
'
'    fgTot.TextMatrix(1, STC_SLICE) = m_dSlice * 100#
'    fgTot.TextMatrix(1, STC_LOT_SIZE) = m_nLotSize
'    fgTot.TextMatrix(1, STC_MAX_SIZE) = m_nMaxSize
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
'    frmPrintPreview.Execute m_frmOwner, "Stock Hedge", "Stock Hedge", fgFlt, fgTot, fgPos
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Function CurUnderlyingID() As Long
'    On Error Resume Next
'    Dim aRowData As clsShRowData
'
'    CurUnderlyingID = 0
'
'    With fgPos
'        If .Row > 0 Then
'            Set aRowData = .RowData(.Row)
'            If Not aRowData Is Nothing Then
'                CurUnderlyingID = aRowData.UndGrp.Und.ID
'                Set aRowData = Nothing
'            End If
'        End If
'    End With
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
'    frmLayout.Execute GT_STOCKHEDGE_FILTER, SFC_TRADER, SFC_LAST_COLUMN, m_gdFlt, m_frmOwner
'End Sub
'
'Public Sub CustomizeTotGridLayout()
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute GT_STOCKHEDGE_TOTALS, STC_SLICE, STC_LAST_COLUMN, m_gdTot, m_frmOwner
'End Sub
'
'Public Sub CustomizePosGridLayout()
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute GT_STOCKHEDGE_POSITIONS, SPC_TRADER, SPC_LAST_COLUMN, m_gdPos, m_frmOwner
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
'    Dim nUndID&, nID&, bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'
'    m_bInProc = True
'    nUndID = 0
'    bBuy = True
'
'    If m_enMenuGrid = GT_STOCKHEDGE_FILTER Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
'            On Error Resume Next
'            Set aUnd = g_Stock(m_nFilter(SFC_UNDERLYING))
'            On Error GoTo EH
'        End If
'        If Not aUnd Is Nothing Then
'            bBuy = (g_Params.QuoteOtherColBuy <> 0)
'            nUndID = aUnd.ID
'            Set aUnd = Nothing
'        End If
'
'    ElseIf m_enMenuGrid = GT_STOCKHEDGE_POSITIONS Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
'            On Error Resume Next
'            Set aUnd = fgPos.RowData(m_nMenuGridRow).UndGrp.Und
'            On Error GoTo EH
'        End If
'        If Not aUnd Is Nothing Then
'            bBuy = (g_Params.QuoteOtherColBuy <> 0)
'            nUndID = aUnd.ID
'            Set aUnd = Nothing
'        End If
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
'Private Sub InitVola(ByRef aUnd As EtsMmStockHedgeLib.MmShUndAtom)
'    On Error GoTo EH
'    aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.UndType = enCtStock, enCtStock, enCtIndex), g_VolaSource
'    Exit Sub
'EH:
'    LogEvent EVENT_ERROR, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
'End Sub
'
'Public Sub HedgeMaintenance()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    If frmMaintenance.IsOpened Then Exit Sub
'    frmMaintenance.Execute m_frmOwner
'End Sub
'
'Public Sub SaveOrders(ByVal sPath As String)
'    On Error GoTo EH
'    If m_bInProc Then Exit Sub
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'    Dim aPos As EtsMmStockHedgeLib.MmShPosAtom
'    Dim nFileB&, nFileS&, nFileSS&, sPrefix$, nTotalQty&, nExportCount&
'    Dim nQtyB&, nQtyS&, nQtySS&, nOrdRemainder&, nOrdCount&, i&
'
'    m_bInProc = True
'    nExportCount = 0
'
'    For Each aTrdGrp In m_Grp
'        nFileB = 0
'        nFileS = 0
'        nFileSS = 0
'
'        For Each aStgGrp In aTrdGrp.Strategy
'            For Each aUndGrp In aStgGrp.Und
'                If aUndGrp.Pos.Count > 0 Then
'                    nTotalQty = m_Und(aUndGrp.Und.ID).TotalQtyInShares
'
'                    If aUndGrp.HedgeQty > BAD_LONG_VALUE And aUndGrp.HedgeQty <> 0 And nTotalQty > BAD_LONG_VALUE Then
'                        'nTotalQty = -Sgn(nTotalQty) * Int(Abs(nTotalQty) / m_nLotSize + 0.5) * m_nLotSize
'
'                        sPrefix = aUndGrp.Und.Symbol
'                        If Len(sPrefix) < 8 Then sPrefix = sPrefix & Space$(8 - Len(sPrefix))
'
'                        nQtyB = 0&
'                        nQtyS = 0&
'                        nQtySS = 0&
'
'                        If aUndGrp.DeltaQty < 0 Then
'                            nQtyB = Abs(aUndGrp.HedgeQty)
'
'                        ElseIf aUndGrp.DeltaQty > nTotalQty And nTotalQty > 0 Then
'                            nQtyS = Abs(-Sgn(nTotalQty) * Int(Abs(nTotalQty) / m_nLotSize + 0.5) * m_nLotSize)
'                            nQtySS = Abs(Abs(aUndGrp.HedgeQty) - nQtyS)
'
'                        ElseIf aUndGrp.DeltaQty <= nTotalQty And aUndGrp.DeltaQty > 0 Then
'                            nQtyS = Abs(aUndGrp.HedgeQty)
'
'                        ElseIf aUndGrp.DeltaQty > 0 And nTotalQty <= 0 Then
'                            nQtySS = Abs(aUndGrp.HedgeQty)
'                        End If
'
'                        If nQtyB <> 0 Or nQtyS <> 0 Or nQtySS <> 0 Then
'                            If nQtyB <> 0 Then
'                                nOrdCount = Int(nQtyB / m_nMaxSize)
'                                nOrdRemainder = nQtyB - nOrdCount * m_nMaxSize
'
'                                If nFileB = 0 Then
'                                    nFileB = FreeFile
'                                    Open sPath & "b" & UCase$(aTrdGrp.Trader.Name) & ".txt" For Output Access Write Lock Read Write As #nFileB
'                                End If
'
'                                For i = 1 To nOrdCount
'                                    Print #nFileB, sPrefix & Trim$(Str$(m_nMaxSize))
'                                    nExportCount = nExportCount + 1
'                                Next
'
'                                If nOrdRemainder > 0 Then
'                                    Print #nFileB, sPrefix & Trim$(Str$(nOrdRemainder))
'                                    nExportCount = nExportCount + 1
'                                End If
'                            End If
'
'                            If nQtyS <> 0 Then
'                                nOrdCount = Int(nQtyS / m_nMaxSize)
'                                nOrdRemainder = nQtyS - nOrdCount * m_nMaxSize
'
'                                If nFileS = 0 Then
'                                    nFileS = FreeFile
'                                    Open sPath & "s" & UCase$(aTrdGrp.Trader.Name) & ".txt" For Output Access Write Lock Read Write As #nFileS
'                                End If
'
'                                For i = 1 To nOrdCount
'                                    Print #nFileS, sPrefix & Trim$(Str$(m_nMaxSize))
'                                    nExportCount = nExportCount + 1
'                                Next
'
'                                If nOrdRemainder > 0 Then
'                                    Print #nFileS, sPrefix & Trim$(Str$(nOrdRemainder))
'                                    nExportCount = nExportCount + 1
'                                End If
'                            End If
'
'                            If nQtySS <> 0 Then
'                                nOrdCount = Int(nQtySS / m_nMaxSize)
'                                nOrdRemainder = nQtySS - nOrdCount * m_nMaxSize
'
'                                If nFileSS = 0 Then
'                                    nFileSS = FreeFile
'                                    Open sPath & "ss" & UCase$(aTrdGrp.Trader.Name) & ".txt" For Output Access Write Lock Read Write As #nFileSS
'                                End If
'
'                                For i = 1 To nOrdCount
'                                    Print #nFileSS, sPrefix & Trim$(Str$(m_nMaxSize))
'                                    nExportCount = nExportCount + 1
'                                Next
'
'                                If nOrdRemainder > 0 Then
'                                    Print #nFileSS, sPrefix & Trim$(Str$(nOrdRemainder))
'                                    nExportCount = nExportCount + 1
'                                End If
'                            End If
'                        End If
'                    End If
'                End If
'            Next
'        Next
'
'        If nFileB <> 0 Then Close #nFileB
'        If nFileS <> 0 Then Close #nFileS
'        If nFileSS <> 0 Then Close #nFileSS
'    Next
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
'
