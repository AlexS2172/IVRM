VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.UserControl ctlRiskView 
   ClientHeight    =   7035
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9420
   KeyPreview      =   -1  'True
   ScaleHeight     =   7035
   ScaleWidth      =   9420
   Begin MSComCtl2.DTPicker dtPick 
      Height          =   253
      Left            =   5445
      TabIndex        =   3
      Top             =   4598
      Visible         =   0   'False
      Width           =   1705
      _ExtentX        =   3016
      _ExtentY        =   450
      _Version        =   393216
      CustomFormat    =   "MM/d/yyyy hh:mm tt"
      Format          =   61145091
      CurrentDate     =   38910
   End
   Begin VB.Timer tmrUndCalc 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   3240
      Top             =   4440
   End
   Begin VB.Timer tmrRealTime 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   2520
      Top             =   4440
   End
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   1800
      Top             =   4440
   End
   Begin VSFlex7Ctl.VSFlexGrid fgPos 
      Height          =   2055
      Left            =   0
      TabIndex        =   2
      Top             =   1680
      Width           =   7095
      _cx             =   12515
      _cy             =   3625
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
      Top             =   120
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
   Begin VSFlex7Ctl.VSFlexGrid fgTot 
      Height          =   495
      Left            =   0
      TabIndex        =   1
      Top             =   968
      Width           =   7634
      _cx             =   13466
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
   Begin VB.Image imgInSpread 
      Height          =   240
      Left            =   4800
      Picture         =   "ctlRiskView.ctx":0000
      Top             =   4680
      Width           =   240
   End
   Begin VB.Image imgBadPrice 
      Height          =   240
      Left            =   4680
      Picture         =   "ctlRiskView.ctx":058A
      Top             =   4680
      Width           =   75
   End
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxFitVolatilityToMarket 
         Caption         =   "Fit Vola To Implied"
      End
      Begin VB.Menu mnuCtxSeparator10 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxPositionTransfer 
         Caption         =   "Position Transfer..."
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxOTCOptionCalc 
         Caption         =   "OTC Option Calc..."
         Shortcut        =   ^{F12}
      End
      Begin VB.Menu mnuCtxSeparator 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxTradeExercise 
         Caption         =   "Exercise Trades..."
      End
      Begin VB.Menu mnuCtxTradeExpiry 
         Caption         =   "Expiry Trades..."
      End
      Begin VB.Menu mnuCtxSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxRealTime 
         Caption         =   "Real Time Mode"
         Shortcut        =   {F9}
      End
      Begin VB.Menu mnuCtxRefresh 
         Caption         =   "Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuCtxRefreshPrices 
         Caption         =   "Refresh &Prices"
         Shortcut        =   +{F5}
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
      Begin VB.Menu mnuCtxWtdVega 
         Caption         =   "Weighted Vega Settings..."
      End
      Begin VB.Menu mnuCtxSeparator7 
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
      Begin VB.Menu mnuCtxSeparator8 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
      Begin VB.Menu mnuCtxSeparator9 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxUseMaualPrice 
         Caption         =   "Use manual price"
      End
      Begin VB.Menu mnuCtxSaveTheoAsClose 
         Caption         =   "Save Theoretical Prices"
      End
   End
End
Attribute VB_Name = "ctlRiskView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event OnSetCaption()
Public Event OnStateChange()
Public Event OnSetRefreshHint(ByVal bSet As Boolean, ByRef strHint$)
Public Event OnManualPriceChanged(ByVal UndID As Long, ByVal ID As Long, ByVal price As Double, ByVal CtType As EtsContractTypeEnum, ByVal Status As ManualPriceUpdateEnum)
Public Event OnRefreshComplete()
Public Event OnRefreshError()
Public Event OnRefreshEmpty()
Public Event OnRefreshCancel()

Private m_gdFlt As clsGridDef
Private m_gdTot As clsGridDef
Private m_gdPos As clsGridDef

Private m_bDateChanged As Boolean
Private m_bFirstTime  As Boolean

Private WithEvents m_RiskView As EtsMmRisksLib.MmRiskView
Attribute m_RiskView.VB_VarHelpID = -1
Private m_bIsInitialized As Boolean

Private WithEvents gePos As clsGridEx
Attribute gePos.VB_VarHelpID = -1

Private WithEvents VolaSource As VolatilitySourcesLib.VolatilitySource
Attribute VolaSource.VB_VarHelpID = -1
Private WithEvents TradeChannel As clsTradeChannel
Attribute TradeChannel.VB_VarHelpID = -1
Private WithEvents frmWtdVega As frmWtdVegaSettings
Attribute frmWtdVega.VB_VarHelpID = -1

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As vB.Label
Public lblStatus As vB.Label
Public WithEvents imgStop As vB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As vB.Image

Public ProcessRealTime As Boolean

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bTmrUndCalcNow As Boolean
Private m_bIsPosRefresh As Boolean

Private m_nOpenedExpiry As Long

Private m_bCurUndChanging As Boolean
Private m_nCurUnd As Long

Private m_bVolaChangedNow As Boolean
Private m_bVolaUpdated As Boolean
Public m_frmOwner As Form

Private m_sCurrentOriginalText As String

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_RISKS_FILTER To GT_RISKS_POSITIONS) As Boolean

Private m_bShutDown As Boolean

Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

' calculation statistic data
Private m_nLastRecalcCycle As Long
Private m_nLastOutCycle As Long
Private m_nLastDataSize As Long

Private m_nOperation As Long
Private m_nUndResponses As Long
Private m_nOptResponses As Long
Private m_nFutResponses As Long

Private m_Aux As clsAuxRiskView
Private m_AuxOut As clsAuxRiskViewOut
Private m_AuxClc As clsAuxRiskViewCalc
Private m_nUndPositions As Integer
Private m_nOptPositions As Integer

Private TradeQueue As clsTradeQueueColl
Dim nSelectedRow As Long

Private m_bIsNewTrades As Boolean
Private m_AggCols() As Long
Private m_bTradeActionrefreshPositions As Boolean
Private simulation_scenario As MarketSimulationScenario
Private recalculation_time_stemp As Date

Public Sub SetLastRowSelected()
    Dim nRow As Long
    If fgPos.rows > 1 Then
        For nRow = 1 To fgPos.rows - 1
            If nRow = nSelectedRow Then
                fgPos.IsSelected(nRow) = True
            Else
                fgPos.IsSelected(nRow) = False
            End If
        Next
    End If
End Sub

Public Sub ctlStopRT()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_Aux.RealTime Then
        StopRealTime
    End If
End Sub

Public Property Get Filter() As EtsGeneralLib.EtsFilterData
    Set Filter = m_Aux.FilterData
End Property

Public Function Init() As Boolean
On Error GoTo EH

    ReDim m_AggCols(3)
    m_AggCols(0) = RPC_UND
    m_AggCols(1) = RPC_FUTURES
    m_AggCols(2) = RPC_EXPIRY
    
    Set m_gdFlt = New clsGridDef
    Set m_gdTot = New clsGridDef
    Set m_gdPos = New clsGridDef
    
    Set m_Aux = New clsAuxRiskView
    Set m_AuxOut = New clsAuxRiskViewOut
    Set m_AuxClc = New clsAuxRiskViewCalc
    Set m_RiskView = m_Aux.RiskView
    
    Set simulation_scenario = Nothing
    
    recalculation_time_stemp = 0#
    
    m_RiskView.ConnectionString = g_Params.DbConnection
        
    m_bDateChanged = False
    m_bFirstTime = True
    
    m_bDateChanged = False
        
    m_bShutDown = False
    m_bVolaChangedNow = False
    m_bVolaUpdated = False
    m_bIsNewTrades = False
    m_bIsInitialized = False
    m_bIsPosRefresh = False
    m_bTradeActionrefreshPositions = False
    
    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    Set VolaSource = g_VolaSource
        
    fgFlt.TextMatrix(1, RFC_SIM_DATE) = Now
    
    Set m_Aux.VolaSource = VolaSource
    Set m_Aux.gdFlt = m_gdFlt
    Set m_Aux.gdTot = m_gdTot
    Set m_Aux.gdPos = m_gdPos
    Set m_Aux.fgFlt = fgFlt
    Set m_Aux.fgTot = fgTot
    Set m_Aux.fgPos = fgPos
        
    m_AuxClc.Init m_Aux
    
    m_AuxOut.Init m_Aux
    Set m_AuxOut.gdFlt = m_gdFlt
    Set m_AuxOut.gdTot = m_gdTot
    Set m_AuxOut.gdPos = m_gdPos
    Set m_AuxOut.fgTot = fgTot
    Set m_AuxOut.fgPos = fgPos
    Set m_AuxOut.imgBadPrice = imgBadPrice
    
    InitColumns
    
    ResetMenuData
    m_nUndPositions = 0
    m_nOptPositions = 0
    
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_RISKS_FILTER) = False
    m_bKeyDown(GT_RISKS_TOTALS) = False
    m_bKeyDown(GT_RISKS_POSITIONS) = False

    m_Aux.GridLock(GT_RISKS_FILTER).Init fgFlt
    m_Aux.GridLock(GT_RISKS_TOTALS).Init fgTot
    m_Aux.GridLock(GT_RISKS_POSITIONS).Init fgPos
    
    m_Aux.SortColKey = -2
    m_Aux.SortColOrder = flexSortGenericAscending
    
    m_Aux.InitGrids
    
    Set gePos = New clsGridEx
    gePos.Init fgPos
    Set m_Aux.gePos = gePos
    
    m_Aux.FormatFltGrid
    m_Aux.FormatFltColumns
    
    m_Aux.FormatTotGrid
    m_Aux.FormatTotColumns
    
    m_Aux.FormatPosGrid
    m_Aux.FormatPosColumns
    
    m_Aux.InitFltData
    
    Set TradeChannel = g_TradeChannel
    Set frmWtdVega = g_frmWtdVegaSettings
    
    fgFlt.Col = RFC_SYMBOL
    fgFlt.Row = 1
    
    fgTot.Col = 0
    fgTot.Row = 1
    
    Set TradeQueue = New clsTradeQueueColl
    Set frmLayout = New frmGridLayout
    
    Init = True
    Set aParams = g_Params
    
    m_RiskView.NetExposureAUM = g_Params.NetExposureAUM
    
    AdjustCaption
    AdjustState
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize risk view."
    RaiseEvent OnRefreshError
End Function

Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    
    If m_bShutDown Then Exit Sub
    
    m_Aux.Filter(RFC_SYMBOL) = aFilters.Data(RFC_SYMBOL)
    m_Aux.Filter(RFC_GROUPS) = aFilters.Data(RFC_GROUPS)
    m_Aux.Filter(RFC_TRADER_GROUP) = aFilters.Data(RFC_TRADER_GROUP)
    m_Aux.Filter(RFC_TRADER) = aFilters.Data(RFC_TRADER)
    m_Aux.Filter(RFC_STRATEGY) = aFilters.Data(RFC_STRATEGY)
    m_Aux.Filter(RFC_TRADES) = aFilters.Data(RFC_TRADES)
    
    m_Aux.FilterUpdateAll
    tmrShow.Enabled = True
End Sub

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)

    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Select Case nType
        Case TYPE_UNDERLYING
            If (Not g_Main.ContractAll(nValue) Is Nothing) Then
                If (Not g_Main.ContractAll(nValue).Und Is Nothing) Then
                    If (Not g_Main.ContractAll(nValue).Und.HeadComponent Is Nothing) Then
                        m_Aux.Filter(RFC_SYMBOL) = g_Main.ContractAll(nValue).Und.HeadComponent.ID
                    End If
                End If
            Else
                m_Aux.Filter(RFC_SYMBOL) = nValue
            End If
        Case TYPE_GROUP
            m_Aux.Filter(RFC_GROUPS) = nValue
        Case TYPE_TRADER
            m_Aux.Filter(RFC_TRADER) = nValue
        Case TYPE_TRADER_GROUP
            m_Aux.Filter(RFC_TRADER_GROUP) = nValue
        Case TYPE_STRATEGY
            m_Aux.Filter(RFC_STRATEGY) = nValue
    End Select
    
    m_Aux.FilterUpdateAll
    tmrShow.Enabled = True

End Sub

Public Sub FilterUpdateModel(ByVal bAutosize As Boolean)
    On Error Resume Next
    m_Aux.FilterUpdateModel bAutosize
End Sub

Private Function PositionsLoad() As Boolean
    On Error Resume Next
    PositionsLoad = False
    m_AuxClc.UndCount = 0
    m_AuxClc.OptCount = 0
    
    If m_bInProc Then Exit Function
    
    Dim i&, nCount&, aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aExp As EtsGeneralLib.EtsMmEntityAtom
    
    Dim nStart&
    nStart = GetTickCount
    
    m_Aux.Grp.GroupType = TYPE_ALL
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState

    Err.Clear
    m_Aux.Grp.Name = ""
    If m_Aux.Filter(RFC_SYMBOL) > 0 Then
        m_Aux.Grp.Name = g_Underlying(m_Aux.Filter(RFC_SYMBOL)).Symbol
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_Aux.Filter(RFC_GROUPS) > 0 Then
        If Len(m_Aux.Grp.Name) > 0 Then m_Aux.Grp.Name = m_Aux.Grp.Name & "; "
        m_Aux.Grp.Name = m_Aux.Grp.Name & g_UnderlyingGroup(m_Aux.Filter(RFC_GROUPS)).Name
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_Aux.Filter(RFC_TRADER) > 0 Then
        If Len(m_Aux.Grp.Name) > 0 Then m_Aux.Grp.Name = m_Aux.Grp.Name & "; "
        m_Aux.Grp.Name = m_Aux.Grp.Name & g_Trader(m_Aux.Filter(RFC_TRADER)).Name
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_Aux.Filter(RFC_TRADER_GROUP) > 0 Then
        If Len(m_Aux.Grp.Name) > 0 Then m_Aux.Grp.Name = m_Aux.Grp.Name & "; "
        m_Aux.Grp.Name = m_Aux.Grp.Name & g_TraderGroup(m_Aux.Filter(RFC_TRADER_GROUP)).Name
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_Aux.Filter(RFC_STRATEGY) <> 0 Then
        If Len(m_Aux.Grp.Name) > 0 Then m_Aux.Grp.Name = m_Aux.Grp.Name & "; "
        If m_Aux.Filter(RFC_STRATEGY) > 0 Then
            m_Aux.Grp.Name = m_Aux.Grp.Name & g_Strategy(m_Aux.Filter(RFC_STRATEGY)).Name
        Else
            m_Aux.Grp.Name = m_Aux.Grp.Name & NO_STRATEGY_NAME
        End If
        
        If Err.Number <> 0 Then m_bDataLoad = False
        
    End If
    If Len(m_Aux.Grp.Name) = 0 Then
        If m_bFirstTime Then
            m_Aux.Grp.ID = -1
            m_bInProc = False
            
            AdjustState
            m_bDataLoad = False
        End If
        m_Aux.Grp.Name = "<All>"
    End If
    
    m_bFirstTime = False
    
    If Not m_bDataLoad Then GoTo Ex
    On Error GoTo EH
    
    Dim mmTradesColl As MmTradeInfoColl
    
    
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 100
    pbProgress.Visible = True
    
    Set m_Aux.RiskView.EtsMain = g_Main
    Set m_Aux.RiskView.VolaSource = VolaSource
    Set m_Aux.RiskView.TradesCache = g_TradeChannel.TradeChannel
       
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogInformation, "Started Positions Loading.", m_frmOwner.GetCaption
            
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Call TradeChannel.UpdateManualActivePrices", m_frmOwner.GetCaption
    
    g_TradeChannel.TradeChannel.UpdateManualActivePrices
    
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Done TradeChannel.UpdateManualActivePrices", m_frmOwner.GetCaption
    
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Call RiskView.LoadPositions", m_frmOwner.GetCaption
    
    m_Aux.RiskView.LoadPositions m_Aux.FilterData
        
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Done RiskView.LoadPositions", m_frmOwner.GetCaption
        
    m_bIsInitialized = True
    
    If Not m_bDataLoad Then GoTo Ex
        
    'm_AuxClc.UnderlyingsAdjustRates True
    
    PositionsLoad = m_bDataLoad
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogInformation, "Finished Data Loading. Duration " & GetTickCount - nStart & " ms", m_frmOwner.GetCaption
Ex:
    On Error Resume Next
    m_bInProc = False
    m_bDataLoad = False
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load positions."
    RaiseEvent OnRefreshError
    GoTo Ex
End Function

Public Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_RISKS_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_RISKS_TOTALS).CopyTo m_gdTot
    g_DefGrids(GT_RISKS_POSITIONS).CopyTo m_gdPos
End Sub

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    RaiseEvent OnSetRefreshHint(bSet, IIf(bSet, "Use F5 to refresh results.", ""))
End Sub

Private Sub aParams_NetExposureAUMChange(ByVal nNewVal As Double)
    On Error Resume Next
    m_RiskView.NetExposureAUM = nNewVal
End Sub

Private Sub aParams_PriceProfilesChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.GroupType = 0 Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
    Else
        RefreshView
    End If
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.GroupType = 0 Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
    Else
        RefreshView
    End If
End Sub

Private Sub DoTradeAction(TrdTmp As clsTradeQueueAtom)
   With TrdTmp
       Select Case .enTradeAction
            Case enTaTradeNew
                Debug.Assert Not .TrdNew Is Nothing
                AddTrade .TrdNew
                Debug.Print "DoTradeAction.Add"
                
            Case enTaTradeUpdate
                Debug.Assert Not .TrdNew Is Nothing
                Debug.Assert Not .TrdOld Is Nothing
                DeleteTrade .TrdOld
                AddTrade .TrdNew
                Debug.Print "DoTradeAction.Update"
    
            Case enTaTradeDelete
                Debug.Assert Not .TrdOld Is Nothing
                DeleteTrade .TrdOld
                Debug.Print "DoTradeAction.Delete"
        End Select
    End With
End Sub

Private Sub CheckTradeQueue()
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "CheckTradeQueue Enter: " & CStr(TradeQueue.Count), m_frmOwner.GetCaption
    
    ' TradeQueue manage
    Dim TrdTmp As clsTradeQueueAtom
    For Each TrdTmp In TradeQueue
        If Not IsNull(TrdTmp) Then
            DoTradeAction TrdTmp
            Set TrdTmp = Nothing
        End If
    Next
  
    TradeQueue.Clear
  
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "CheckTradeQueue Exit", m_frmOwner.GetCaption
  
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom, aFut As EtsMmRisksLib.MmRvFutAtom
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aContract As EtsGeneralLib.EtsContractAtom
    
    If m_Aux.Grp.GroupType = 0 Or m_Aux.Grp.ID = 0 Then Exit Sub
    bUpdate = False
    
    If Not m_Aux.RealTime Then
        For Each aFilterAtom In collUpdUnd
            If Not m_Aux.Und(aFilterAtom.ID) Is Nothing Then
                bUpdate = True
                Set aFilterAtom = Nothing
                Exit For
            End If
        Next
        If bUpdate Then SetRefreshHint True
    Else
        For Each aFilterAtom In collUpdUnd
            Set aContract = g_Contract(aFilterAtom.ID)
            Set aUnd = m_Aux.Und(aContract.Und.ID)
            If Not aUnd Is Nothing And Not aContract Is Nothing Then
                If aContract.ContractType <> enCtFuture Then
                    aUnd.IsAmerican = aContract.Und.IsAmerican
                    Set aUnd.UndPriceProfile = aContract.Und.UndPriceProfile
                    Set aUnd.OptPriceProfile = aContract.Und.OptPriceProfile
                    Set aUnd.Dividend = aContract.Und.Dividend
                    
                    aUnd.Yield = aContract.Und.Yield
                    aUnd.Skew = aContract.Und.Skew
                    aUnd.Kurt = aContract.Und.Kurt
                    aUnd.IsHTB = aContract.Und.IsHTB
                    aUnd.HasSynthetic = aContract.Und.HaveSyntheticRoots
                    Set aUnd.SynthRoots = aContract.Und.SyntheticRoots
                Else
                    Set aFut = aUnd.Fut(aContract.Fut.ID)
                    If Not aFut Is Nothing Then
                        aFut.IsAmerican = aContract.Fut.IsAmerican
                        Set aFut.UndPriceProfile = aContract.Fut.UndPriceProfile
                        Set aFut.OptPriceProfile = aContract.Fut.OptPriceProfile
                        Set aFut = Nothing
                    End If
                End If
                bUpdate = True
            End If
            Set aUnd = Nothing
            Set aContract = Nothing
            Set aFilterAtom = Nothing
        Next
        If bUpdate Then RefreshView
    End If
End Sub

Private Sub aParams_UndPriceToleranceChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.GroupType = 0 Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
    Else
        RefreshView
    End If
End Sub

Private Sub dtPick_Change()
       fgFlt.TextMatrix(1, RFC_SIM_DATE) = dtPick.Value
       dtPick.Visible = False
       m_bDateChanged = True
   
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter changed date. New date: " & dtPick.Value, m_frmOwner.GetCaption
End Sub

Private Sub dtPick_KeyDown(KeyCode As Integer, Shift As Integer)
    Select Case KeyCode
        Case vbKeyEscape
            dtPick.Visible = False
        Case vbKeyReturn
            dtPick.Visible = False
    End Select
    Dim nUnds&, nFuts&, nOpts&
    
    Dim dtLocalCalcDate As Date
    
    If m_bDateChanged And Not dtPick.Visible And Not m_Aux.RealTime And m_Aux.Grp.ID <> 0 Then
        m_bInProc = True
            fgFlt.TextMatrix(1, RFC_SIM_DATE) = dtPick.Value
                    
            dtLocalCalcDate = dtPick.Value
            dtLocalCalcDate = dtLocalCalcDate - TimeSerial(0, 0, Second(dtLocalCalcDate))
            m_AuxClc.CalcDate = GetNewYorkTime + (dtLocalCalcDate - Date - TimeSerial(Hour(Now), Minute(Now), 0))
            
            m_RiskView.SetDirty
            m_AuxClc.UnderlyingsCalc True, True
            m_AuxOut.UnderlyingsUpdate False
            m_AuxOut.TotalsUpdate
            RefreshPositions
        m_bInProc = False
   End If

End Sub

Private Sub dtPick_LostFocus()
    On Error GoTo Ex
    dtPick.Visible = False
    Dim nUnds&, nOpts&, nFuts&
    Dim dtLocalCalcDate As Date
    If m_bDateChanged And Not dtPick.Visible And Not m_Aux.RealTime Then
        m_bInProc = True
            fgFlt.TextMatrix(1, RFC_SIM_DATE) = dtPick.Value
            
            dtLocalCalcDate = dtPick.Value
            dtLocalCalcDate = dtLocalCalcDate - TimeSerial(0, 0, Second(dtLocalCalcDate))
            m_AuxClc.CalcDate = GetNewYorkTime + (dtLocalCalcDate - Date - TimeSerial(Hour(Now), Minute(Now), 0))
        
            m_RiskView.SetDirty
            m_AuxClc.UnderlyingsCalc True, True
            m_AuxOut.UnderlyingsUpdate False
            m_AuxOut.TotalsUpdate
            RefreshPositions
        m_bInProc = False
   End If
   Exit Sub
Ex:
    m_bInProc = False
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Filter_DblClick Enter", m_frmOwner.GetCaption
    
    If m_bShutDown Then Exit Sub
    With fgFlt
        m_enMenuGrid = GT_RISKS_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .rows
        
        HandleGridDblClick False
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Filter_DblClick Exit", m_frmOwner.GetCaption

End Sub

Private Sub fgPos_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    nSelectedRow = NewRow
    SetLastRowSelected
End Sub

Private Sub fgPos_AfterSelChange(ByVal OldRowSel As Long, ByVal OldColSel As Long, ByVal NewRowSel As Long, ByVal NewColSel As Long)
    nSelectedRow = NewRowSel
    SetLastRowSelected
End Sub

Private Sub fgPos_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&
    
    If Col >= 0 And Row < 0 Then
        nIdx = fgPos.ColKey(Col)
        If nIdx >= RPC_SYMBOL And nIdx <= g_RPC_LAST_COLUMN Then
            m_gdPos.Col(nIdx).Width = IIf(fgPos.ColWidth(Col) > 0, fgPos.ColWidth(Col), -1)
        End If
    End If
End Sub

Private Sub fgPos_Compare(ByVal Row1 As Long, ByVal Row2 As Long, Cmp As Integer)
    On Error Resume Next
'    If Not m_bShutDown Then m_Aux.PosGridCompare Row1, Row2, Cmp
End Sub

Private Sub fgTot_DblClick()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgTot
        m_enMenuGrid = GT_RISKS_TOTALS
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .rows
        
        HandleGridDblClick False
    End With
End Sub

Private Sub fgTot_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_RISKS_TOTALS) = True
    End If
End Sub

Private Sub fgTot_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bKeyDown(GT_RISKS_TOTALS) Then
        m_bKeyDown(GT_RISKS_TOTALS) = False
        With fgTot
            m_enMenuGrid = GT_RISKS_TOTALS
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Totals Hot key: Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Totals Hot key: Alt+Ctrl+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Totals Hot key: Alt+Shift+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Totals Hot key: Ctrl+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxCopy_Click
        End Select
    End If
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim sValue$, nValue&, sVal$
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Filter_AfterEdit Enter", m_frmOwner.GetCaption
    
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = CLng(sValue)
            If Err.Number <> 0 Then nValue = val(sValue)
            If Err.Number = 0 Then
                m_RiskView.SetAllRowsCollapsed
                Select Case Col
                    Case RFC_SYMBOL, RFC_GROUPS To RFC_STRATEGY
                        StoreRecentFilters
                        m_Aux.Filter(Col) = nValue
                        If (Col = RFC_SYMBOL) Then
                            If (Not g_UnderlyingAll(nValue) Is Nothing) Then
                                If (g_UnderlyingAll(nValue).IsHead) Then
                                    m_Aux.Filter(Col) = g_UnderlyingAll(nValue).ID
                                Else
                                    If (Not g_UnderlyingAll(nValue).HeadComponent Is Nothing) Then
                                        m_Aux.Filter(Col) = g_UnderlyingAll(nValue).HeadComponent.ID
                                    Else
                                        m_Aux.Filter(Col) = nValue
                                    End If
                                End If
                            End If
                        End If
                        .AutoSize 0, .Cols - 1, , 100
                        tmrShow.Enabled = True
                    Case RFC_TRADES
                        m_Aux.Filter(RFC_TRADES) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        StoreRecentFilters
                        tmrShow.Enabled = True
                    Case RFC_TRADER_GROUP
                        m_Aux.Filter(RFC_TRADER_GROUP) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        StoreRecentFilters
                        tmrShow.Enabled = True
                    Case RFC_TRADER
                        m_Aux.Filter(RFC_TRADER) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        StoreRecentFilters
                        tmrShow.Enabled = True
                    Case RFC_EXPIRY
                        m_Aux.Filter(RFC_EXPIRY) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                       
                        RefreshPositions
                        
                        m_bInProc = True
                        m_AuxClc.UnderlyingsCalc True, True
                        m_AuxOut.UnderlyingsUpdate True
                        m_AuxOut.TotalsUpdate
                        m_bInProc = False
                        
                    Case RFC_SIM_DATE
                   
                    Case RFC_INDEX
                        m_Aux.Filter(RFC_INDEX) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        IndexLoad
                End Select
            Else
                .TextMatrix(1, Col) = m_Aux.Filter(Col)
            End If
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Filter_AfterEdit Exit", m_frmOwner.GetCaption

End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If IsHotKeyGridClipOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_RISKS_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bKeyDown(GT_RISKS_FILTER) Then
        m_bKeyDown(GT_RISKS_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_RISKS_FILTER
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Filter Hot key: Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                 mnuCtxTradeNew_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Filter Hot key Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Filter Hot key Alt+Ctrl+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Filter Hot key Alt+Shift+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                'mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Filter Hot key Alt+Shift+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxCopy_Click
        
        End Select
    End If
End Sub

Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Button = vbRightButton Then
        With fgFlt
            m_enMenuGrid = GT_RISKS_FILTER
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
            ShowPopup
        End With
    End If
    If fgFlt.Visible = True And fgFlt.MouseCol <> RFC_SIM_DATE Then
        dtPick.Visible = False
    End If
End Sub

Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Filter_StartEdit Enter", m_frmOwner.GetCaption
    
    If m_bShutDown Then Exit Sub
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    
    With fgFlt
        If Not m_bInProc And Not m_bDataLoad And _
            Col >= RFC_SYMBOL And Col <= RFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
           
            If Not Cancel Then m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
        End If
    End With
    
    If Col = RFC_SIM_DATE And m_Aux.Grp.ID <> 0 And m_Aux.RealTime = False Then
        ' we'll handle the editing ourselves
        Cancel = True
        dtPick.Value = fgFlt.TextMatrix(1, RFC_SIM_DATE)
        ' position date picker control over cell
        dtPick.Move fgFlt.CellLeft, fgFlt.CellTop, fgFlt.CellWidth, fgFlt.CellHeight
        ' initialize value, save original in tag in case user hits escape
        dtPick.Tag = fgFlt
        ' show and activate date picker control
        dtPick.Visible = True
        dtPick.SetFocus
        ' make it drop down the calendar
        'SendKeys "{f4}"
    End If
      
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Filter_StartEdit Exit", m_frmOwner.GetCaption
    
End Sub

Private Sub fgPos_AfterEdit(ByVal Row As Long, ByVal Col As Long)

    On Error Resume Next
    
    If m_bShutDown Then Exit Sub
    
    Dim nKey&, sValue$, dValue#, bCalcUnd As Boolean, bCalcPos As Boolean, bCalcSynthUnd As Boolean
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, nUndRow&
    Dim aRowData As EtsMmRisksLib.MmRvRowData, aSynthGreeks As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim aUndData As EtsMmRisksLib.IMmRvAggregationDataAtom
    Dim aIdxData As EtsMmRisksLib.IMmRvAggregationDataAtom
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, bChangeUnd As Boolean, bChangePos As Boolean
    Dim aTradeColl As MmTradeInfoColl
    Dim aFut As MmRvFutAtom
    Dim actPriceChg As Boolean
    
    actPriceChg = False
       
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    With fgPos
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nKey = .ColKey(Col)
            If Err.Number = 0 Then
                bCalcUnd = False
                bCalcPos = False
                bCalcSynthUnd = False
                bChangeUnd = False
                bChangePos = False
                               
                Set aRowData = m_RiskView.PosRowData(Row)
                Set aUnd = aRowData.Und
                Set aPos = aRowData.Pos
                Set aSynthGreeks = aRowData.SynthGreeks
                Set aUndData = aUnd
                Set aIdxData = m_Aux.Idx
                Set aFut = aRowData.Fut
                
                Select Case nKey
                    Case RPC_ACTIVEPRC
                    
                            actPriceChg = True
                            dValue = Abs(ReadDbl(sValue))
                            If Not aPos Is Nothing Then
                                    If Not g_PerformanceLog Is Nothing Then _
                                                g_PerformanceLog.LogMmInfo enLogUserAction, "Position ActivePrice Changed " _
                                                                                            & "OldValue=""" & aPos.Quote.price.Active & """ " _
                                                                                            & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                    
                                        nUndRow = .GetNodeRow(Row, flexNTParent)
                                        
                                        Debug.Assert nUndRow > 0
                                        
                                        Select Case aPos.ContractType
                                            Case enCtOption, enCtFutOption
                                                If dValue > 0 Then
                                                    UpdatePosOptManualPrice aPos, dValue, enUsAdd
                                                End If
                                                bCalcPos = True
                                                
                                            Case enCtFuture
                                                If dValue > 0 Then
                                                    UpdateFutManualPrice aPos.Fut, dValue, enUsAdd
                                                    If (Not aPos.Quote Is Nothing) Then
                                                        aPos.Quote.IsDirty = True
                                                    End If
                                                End If
                                                
                                                bCalcUnd = True
                                                
                                            Case Else
                                                If dValue > 0 Then
                                                    UpdateUndManualPrice aUnd, dValue, enUsAdd
                                                    If (Not aPos.Quote Is Nothing) Then
                                                        aPos.Quote.IsDirty = True
                                                    End If
                                                End If
                                                
                                                bCalcUnd = True
                                                
                                                If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Active = aUndData.price.Active
                                        End Select
                                              
                            ElseIf aSynthGreeks Is Nothing Then
                                If Not g_PerformanceLog Is Nothing Then _
                                                g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying ActivePrice Changed " _
                                                                                            & "OldValue=""" & aUndData.price.Active & """ " _
                                                                                            & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                                                                            
                                    If (Not aFut Is Nothing) Or (Not aUnd Is Nothing) Then
                                  
                                         If Not aFut Is Nothing Then
                                             If dValue > 0 Then
                                                UpdateFutManualPrice aFut, dValue, enUsAdd
                                                aFut.price.IsDirty = True
                                             End If
                                         Else
                                             If dValue > 0 Then
                                                 If (Not g_ContractAll(aUnd.ID) Is Nothing) Then
                                                    If (Not g_ContractAll(aUnd.ID).Und Is Nothing) Then
                                                        UpdateUndManualPrice aUnd, dValue, enUsAdd
                                                        aUnd.price.IsDirty = True
                                                        If (Not m_Aux.Und(aUnd.ID) Is Nothing) Then
                                                            m_Aux.Und(aUnd.ID).price.Active = dValue
                                                            m_Aux.Und(aUnd.ID).price.IsDirty = True
                                                            m_Aux.Und(aUnd.ID).price.IsUseManualActive = True
                                                        End If
                                                    End If
                                                 End If
                                             End If
                                         End If
                                         
                                         bCalcUnd = True
                                         nUndRow = Row
                                         
                                         Set aPos = aUnd.Pos(aUnd.ID)
                                         If Not aPos Is Nothing Then aPos.Quote.price.Active = aUndData.price.Active
                                         If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Active = aUndData.price.Active
                                    End If
                            Else
                                Set aUnd = m_Aux.Und(aSynthGreeks.SynthUndID)
                                If dValue > 0 Then
                                    aUnd.price.Active = dValue
                                    aUnd.price.IsDirty = True
                                    UpdateUndManualPrice aUnd, dValue, enUsAdd
                                End If
                                bCalcSynthUnd = True
                                    
                                If Not aUnd.Pos Is Nothing And aUnd.Pos.Count > 0 Then
                                    nUndRow = .FindRow(CStr(aUnd.ID) & "_" & CStr(aUnd.ID), 1, RPC_KEY, , True)
                                    bCalcUnd = True
                                End If
                                    
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.Quote.price.Active = aUndData.price.Active
                                If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Active = aUndData.price.Active

                            End If

                    Case RPC_BID
                        dValue = Abs(ReadDbl(sValue))
                        If Not aPos Is Nothing Then
                                If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Position PriceBid Changed " _
                                                                                        & "OldValue=""" & aPos.Quote.price.Bid & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                If dValue > 0 Then
                                    aPos.Quote.price.Bid = dValue
                                Else
                                    dValue = aPos.Quote.price.Bid
                                End If
                                nUndRow = .GetNodeRow(Row, flexNTParent)
                                
                                Debug.Assert nUndRow > 0
                                
                                Select Case aPos.ContractType
                                
                                    Case enCtOption, enCtFutOption
                                        bCalcPos = True
                                
                                    Case enCtFuture
                                        If dValue > 0 Then
                                            aPos.Fut.price.Bid = dValue
                                        Else
                                            dValue = aPos.Fut.price.Bid
                                        End If
                                        
                                        bCalcUnd = True
                                        
                                    Case Else
                                    
                                        aUndData.price.Bid = aPos.Quote.price.Bid
                                        aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUndData.price.Bid, aUndData.price.Ask, aUndData.price.Last, dToleranceValue, enRoundingRule)
                                        bCalcUnd = True
                                                                              
                                        If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Bid = aUndData.price.Bid
                                        
                                End Select
                            
                        ElseIf aSynthGreeks Is Nothing Then
                                 If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceBid Changed " _
                                                                                        & "OldValue=""" & aUndData.price.Bid & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                If Not aFut Is Nothing Then
                                    If dValue > 0 Then
                                        aFut.price.Bid = dValue
                                    Else
                                        dValue = aFut.price.Bid
                                    End If
                                Else
                                    If dValue > 0 Then
                                        aUndData.price.Bid = dValue
                                    Else
                                        dValue = aUndData.price.Bid
                                    End If
                                End If
                           
                                aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUndData.price.Bid, aUndData.price.Ask, aUndData.price.Last, dToleranceValue, enRoundingRule)
                                bCalcUnd = True
                                nUndRow = Row
                                
                                If (Not m_Aux.Und(aUnd.ID) Is Nothing) Then
                                    m_Aux.Und(aUnd.ID).price.Bid = dValue
                                End If
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.Quote.price.Bid = aUndData.price.Bid
                                If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Bid = aUndData.price.Bid
                        Else
                            Set aUnd = m_Aux.Und(aSynthGreeks.SynthUndID)
                            
                            If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceBid Changed " _
                                                                                        & "OldValue=""" & aUndData.price.Bid & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            If dValue > 0 Then
                                aUndData.price.Bid = dValue
                            Else
                                dValue = aUndData.price.Bid
                            End If
                            aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUndData.price.Bid, aUndData.price.Ask, aUndData.price.Last, dToleranceValue, enRoundingRule)
                            bCalcSynthUnd = True
                            
                            If Not aUnd.Pos Is Nothing And aUnd.Pos.Count > 0 Then
                                nUndRow = .FindRow(CStr(aUnd.ID) & "_" & CStr(aUnd.ID), 1, RPC_KEY, , True)
                                bCalcUnd = True
                            End If
                            
                            Set aPos = aUnd.Pos(aUnd.ID)
                            If Not aPos Is Nothing Then aPos.Quote.price.Bid = aUndData.price.Bid
                            If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Bid = aUndData.price.Bid
                            
                        End If
                        
                    Case RPC_ASK
                        dValue = ReadDbl(sValue)
                        If Not aPos Is Nothing Then
                                If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Position PriceAsk Changed " _
                                                                                        & "OldValue=""" & aPos.Quote.price.Ask & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                              
                                If dValue > 0 Then
                                    aPos.Quote.price.Ask = dValue
                                Else
                                    dValue = aPos.Quote.price.Ask
                                End If
                                nUndRow = .GetNodeRow(Row, flexNTParent)
                                
                                Debug.Assert nUndRow > 0
                                
                                Select Case aPos.ContractType
                                
                                    Case enCtOption, enCtFutOption
                                    
                                        bCalcPos = True
                                
                                    Case enCtFuture
                                        If dValue > 0 Then
                                            aPos.Fut.price.Ask = dValue
                                        Else
                                            dValue = aPos.Fut.price.Ask
                                        End If
                                        bCalcUnd = True
                                        
                                    Case Else
                                    
                                        aUndData.price.Ask = aPos.Quote.price.Ask
                                        aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUndData.price.Bid, aUndData.price.Ask, aUndData.price.Last, dToleranceValue, enRoundingRule)
                                        bCalcUnd = True
                                        
                                        If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Ask = aUndData.price.Ask
                                        
                                End Select
                            
                        ElseIf aSynthGreeks Is Nothing Then
                            If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceAsk Changed " _
                                                                                        & "OldValue=""" & aUndData.price.Ask & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                              
                                If Not aFut Is Nothing Then
                                    If dValue > 0 Then
                                        aFut.price.Ask = dValue
                                    Else
                                        dValue = aFut.price.Ask
                                    End If
                                Else
                                    If dValue > 0 Then
                                        aUndData.price.Ask = dValue
                                    Else
                                        dValue = aUndData.price.Ask
                                    End If
                                End If
                                
                                aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUndData.price.Bid, aUndData.price.Ask, aUndData.price.Last, dToleranceValue, enRoundingRule)
                                bCalcUnd = True
                                nUndRow = Row
                                
                                                                        
                                If (Not m_Aux.Und(aUnd.ID) Is Nothing) Then
                                    m_Aux.Und(aUnd.ID).price.Ask = dValue
                                End If
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.Quote.price.Ask = aUndData.price.Ask
                                If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Ask = aUndData.price.Ask
                        Else
                            Set aUnd = m_Aux.Und(aSynthGreeks.SynthUndID)
                                If dValue > 0 Then
                                    aUndData.price.Ask = dValue
                                Else
                                    dValue = aUndData.price.Ask
                                End If
                                aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUndData.price.Bid, aUndData.price.Ask, aUndData.price.Last, dToleranceValue, enRoundingRule)
                                bCalcSynthUnd = True
                                
                                If Not aUnd.Pos Is Nothing And aUnd.Pos.Count > 0 Then
                                    nUndRow = .FindRow(CStr(aUnd.ID) & "_" & CStr(aUnd.ID), 1, RPC_KEY, , True)
                                    bCalcUnd = True
                                End If
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.Quote.price.Ask = aUndData.price.Ask
                                If m_Aux.Idx.ID = aUnd.ID Then aIdxData.price.Ask = aUndData.price.Ask
                        End If
                        .TextMatrix(Row, Col) = dValue

                    Case RPC_THEO_PRICE_CLOSE
                    
                        dValue = Abs(ReadDbl(sValue))
                        
                        If Not aPos Is Nothing Then
                            If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Position PriceTheoClose Changed " _
                                                                                    & "OldValue=""" & aPos.Quote.price.TheoClose & """ " _
                                                                                    & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                            nUndRow = .GetNodeRow(Row, flexNTParent)
                                
                            Debug.Assert nUndRow > 0
                            
                            Select Case aPos.ContractType
                                Case enCtOption
                                    bCalcPos = True
                                    Set aTradeColl = TradeChannel.TradesByUnd(aPos.UndID)
                                    If Not aTradeColl Is Nothing Then
                                        For Each aTrd In aTradeColl
                                            If aTrd.ContractID = aPos.ID Then
                                                If aTrd.Opt.PriceTheoclose <> aPos.Quote.price.TheoClose Then bChangePos = True
                                                aTrd.Opt.PriceTheoclose = aPos.Quote.price.TheoClose
                                            End If
                                            Set aTrd = Nothing
                                        Next
                                    End If
                                    
                                Case enCtFutOption
                                    Set aTradeColl = TradeChannel.TradesByFut(aPos.Fut.ID)
                                    If Not aTradeColl Is Nothing Then
                                        For Each aTrd In aTradeColl
                                            If aTrd.ContractID = aPos.ID Then
                                                If aTrd.FutOpt.PriceTheoclose <> aPos.Quote.price.TheoClose Then bChangePos = True
                                                aTrd.FutOpt.PriceTheoclose = aPos.Quote.price.TheoClose
                                            End If
                                            Set aTrd = Nothing
                                        Next
                                    End If
                                    bCalcUnd = True
                                
                                Case enCtFuture
                                    aPos.Fut.price.TheoClose = dValue
                                    For Each aTrd In TradeChannel.TradesByFut(aPos.Fut.ID)
                                        If aTrd.ContractID = aPos.ID Then aTrd.Fut.PriceTheoclose = aPos.Fut.price.TheoClose
                                        Set aTrd = Nothing
                                        bChangePos = True
                                    Next
                                    bCalcUnd = True
                                        
                                Case Else
                                    aUnd.price.TheoClose = aPos.Quote.price.TheoClose
                                    bChangeUnd = True
                                    bCalcUnd = True
                                        
                                    If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.price.TheoClose = aUnd.price.TheoClose
                                    bChangePos = True
                                   
                            End Select
                            
                        ElseIf aSynthGreeks Is Nothing Then
                            If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceClose Changed " _
                                                                            & "OldValue=""" & aUnd.price.TheoClose & """ " _
                                                                            & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                            
                            If aUnd.ContractType <> enCtFutUnd And aFut Is Nothing Then
                            
                                aUnd.price.TheoClose = dValue
                                bChangeUnd = True
                                bCalcUnd = True
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then
                                    aPos.Quote.price.TheoClose = aUnd.price.TheoClose
                                    For Each aTrd In TradeChannel.TradesByUnd(aPos.UndID)
                                        If aTrd.ContractID = aPos.ID Then aTrd.Opt.PriceTheoclose = aPos.Quote.price.TheoClose
                                        Set aTrd = Nothing
                                    Next
                                    bChangePos = True
                                End If
                                g_ContractAll(aUnd.ID).Und.PriceTheoclose = dValue
    
                                If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.price.TheoClose = aUnd.price.TheoClose
                                
                            Else
                            
                                    aFut.price.TheoClose = dValue
                                    bChangeUnd = True
                                    bCalcUnd = True
                                    For Each aTrd In TradeChannel.TradesByFut(aFut.ID)
                                        If aTrd.ContractID = aFut.ID Then aTrd.Fut.PriceTheoclose = aFut.price.TheoClose
                                        Set aTrd = Nothing
                                    Next
                                    bChangePos = True
                                    g_ContractAll(aFut.ID).Fut.PriceTheoclose = dValue
                                    
                            End If
                        End If
                        .TextMatrix(Row, Col) = dValue
                
                    Case RPC_CLOSE
                        dValue = Abs(ReadDbl(sValue))
                        If Not aPos Is Nothing Then
                            If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Position PriceClose Changed " _
                                                                                    & "OldValue=""" & aPos.Quote.price.Close & """ " _
                                                                                    & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                            nUndRow = .GetNodeRow(Row, flexNTParent)
                                
                            Debug.Assert nUndRow > 0
                            
                            Select Case aPos.ContractType
                                Case enCtOption
                                    bCalcPos = True
                                    Set aTradeColl = TradeChannel.TradesByUnd(aPos.UndID)
                                    If Not aTradeColl Is Nothing Then
                                        For Each aTrd In aTradeColl
                                            If aTrd.ContractID = aPos.ID Then
                                                If aTrd.Opt.PriceClose <> aPos.Quote.price.Close Then bChangePos = True
                                                aTrd.Opt.PriceClose = aPos.Quote.price.Close
                                            End If
                                            Set aTrd = Nothing
                                        Next
                                    End If
                                    
                                Case enCtFutOption
                                    Set aTradeColl = TradeChannel.TradesByFut(aPos.Fut.ID)
                                    If Not aTradeColl Is Nothing Then
                                        For Each aTrd In aTradeColl
                                            If aTrd.ContractID = aPos.ID Then
                                                If aTrd.FutOpt.PriceClose <> aPos.Quote.price.Close Then bChangePos = True
                                                aTrd.FutOpt.PriceClose = aPos.Quote.price.Close
                                            End If
                                            Set aTrd = Nothing
                                        Next
                                    End If
                                    bCalcUnd = True
                                
                                Case enCtFuture
                                    aPos.Fut.price.Close = dValue
                                    For Each aTrd In TradeChannel.TradesByFut(aPos.Fut.ID)
                                        If aTrd.ContractID = aPos.ID Then aTrd.Fut.PriceClose = aPos.Fut.price.Close
                                        Set aTrd = Nothing
                                        bChangePos = True
                                    Next
                                    bCalcUnd = True
                                        
                                Case Else
                                    aUnd.price.Close = aPos.Quote.price.Close
                                    bChangeUnd = True
                                    bCalcUnd = True
                                        
                                    If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.price.Close = aUnd.price.Close
                                    bChangePos = True
                                   
                            End Select
                            
                        ElseIf aSynthGreeks Is Nothing Then
                            If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceClose Changed " _
                                                                            & "OldValue=""" & aUnd.price.Close & """ " _
                                                                            & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                            
                            If aUnd.ContractType <> enCtFutUnd And aFut Is Nothing Then
                            
                                aUnd.price.Close = dValue
                                bChangeUnd = True
                                bCalcUnd = True
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then
                                    aPos.Quote.price.Close = aUnd.price.Close
                                    For Each aTrd In TradeChannel.TradesByUnd(aPos.UndID)
                                        If aTrd.ContractID = aPos.ID Then aTrd.Opt.PriceClose = aPos.Quote.price.Close
                                        Set aTrd = Nothing
                                    Next
                                    bChangePos = True
                                End If
                                g_ContractAll(aUnd.ID).Und.PriceClose = dValue
                                
                                If (Not m_Aux.Und(aUnd.ID) Is Nothing) Then
                                    m_Aux.Und(aUnd.ID).price.Close = dValue
                                End If
                                
                                If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.price.Close = aUnd.price.Close
                                
                            Else
                            
                                aFut.price.Close = dValue
                                bChangeUnd = True
                                bCalcUnd = True
                                For Each aTrd In TradeChannel.TradesByFut(aFut.ID)
                                    If aTrd.ContractID = aFut.ID Then aTrd.Fut.PriceClose = aFut.price.Close
                                    Set aTrd = Nothing
                                Next
                                bChangePos = True
                                g_ContractAll(aFut.ID).Fut.PriceClose = dValue
                                    
                            End If
                              
                              
                        Else
                            Set aUnd = m_Aux.Und(aSynthGreeks.SynthUndID)
                            If aUnd.price.Close <> dValue Then
                                If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceClose Changed " _
                                                                                        & "OldValue=""" & aUnd.price.Close & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                              
                                aUnd.price.Close = dValue
                                bChangeUnd = True
                                bCalcSynthUnd = True
                                
                                If Not aUnd.Pos Is Nothing And aUnd.Pos.Count > 0 Then
                                    nUndRow = .FindRow(CStr(aUnd.ID) & "_" & CStr(aUnd.ID), 1, RPC_KEY, , True)
                                    bCalcUnd = True
                                End If
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then
                                    aPos.Quote.price.Close = aUnd.price.Close
                                    For Each aTrd In TradeChannel.TradesByUnd(aPos.UndID)
                                        If aTrd.ContractID = aPos.ID Then aTrd.Opt.PriceClose = aPos.Quote.price.Close
                                        Set aTrd = Nothing
                                    Next
                                    bChangePos = True
                                End If
                                
                                If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.price.Close = aUnd.price.Close
                            Else
                            
                              If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceClose Wasn't Changed " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            End If
                        End If
                End Select
                    
                If bChangeUnd Or bChangePos Then
                
                    If bChangeUnd Then
                        g_Underlying(aUnd.ID).PriceClose = aUnd.price.Close
                        g_Underlying(aUnd.ID).PriceTheoclose = aUnd.price.TheoClose
                    End If
                    
                    Dim bPricePub, dwUndID&, dwPosID&
                    bPricePub = False
                    If Not aPos Is Nothing Then
                        m_AuxClc.ClearPosQty aPos
                        If SavePriceClose(aPos.ContractType, aPos.ID, aPos.Quote.price.Close, aPos.Quote.price.TheoClose) Then
                            If Not bPricePub Then
                                If aPos.ContractType = enCtStock Or aPos.ContractType = enCtIndex Then
                                    dwUndID = aPos.ID
                                    dwPosID = aPos.ID
                                Else
                                    dwUndID = aPos.UndID
                                    dwPosID = aPos.ID
                                End If
                                TradeChannel.PriceCloseForPub aPos.ContractType, dwPosID, dwUndID, aPos.Quote.price.Close, aPos.Quote.price.TheoClose
                                bCalcPos = True
                                bPricePub = True
                            End If
                        End If
                    Else
                        If Not aFut Is Nothing Then
                            If SavePriceClose(enCtFuture, aFut.ID, aFut.price.Close, aFut.price.TheoClose) Then
                                TradeChannel.PriceCloseForPub enCtFuture, aFut.ID, aFut.UndID, aFut.price.Close, aFut.price.TheoClose
                                bPricePub = True
                                bCalcPos = True
                            End If
                        ElseIf Not aUnd Is Nothing Then
                            If SavePriceClose(aUnd.ContractType, aUnd.ID, aUnd.price.Close, aUnd.price.TheoClose) Then
                                TradeChannel.PriceCloseForPub aUnd.ContractType, 0, aUnd.ID, aUnd.price.Close, aUnd.price.TheoClose
                                bPricePub = True
                                bCalcPos = True
                            End If
                        End If
                    End If
                 
                End If
                
                If bCalcUnd Or bCalcPos Or bCalcSynthUnd Then
                    If bCalcPos Then
                        If bChangePos And Not aPos Is Nothing Then m_AuxClc.CalcOptionGreeks aUnd, aPos, IIf(g_Params.UseTheoVolatility, GM_NONE, GM_ALL)
                    End If
            
                    m_AuxClc.UnderlyingsCalc True, True, False, False
                    RefreshPositions
                    m_AuxOut.TotalsUpdate
                ElseIf bPricePub Then
                    RefreshPositions
                    m_AuxOut.TotalsUpdate
                End If
                
            End If
        End If
    End With
End Sub

Private Sub fgPos_AfterCollapse(ByVal Row As Long, ByVal State As Integer)
    On Error Resume Next
    If m_bShutDown Or m_bIsPosRefresh Then Exit Sub
    Dim nRow&, aPos As EtsMmRisksLib.MmRvPosAtom, nFirstRow&, nLastRow&, nAggRow&
    Dim aRowData As EtsMmRisksLib.MmRvRowData
    
    m_RiskView.IsRowExpanded(Row) = State = flexOutlineExpanded
    RefreshPositions
    
    If Not m_bCurUndChanging Then m_nCurUnd = 0
End Sub

Private Sub fgPos_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    m_Aux.GridBeforeMoveColumn fgPos, m_gdPos, Col, Position, RPC_NONE
End Sub

Private Sub fgPos_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    m_Aux.GridAfterMoveColumn m_gdPos, Col, Position, RPC_SYMBOL
End Sub

Private Sub gePos_BeforeSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    Dim SortColKey&
    m_Aux.PosBeforeSort Col, Order
    SortColKey = fgPos.ColKey(Col)
    m_RiskView.SortField = SortColKey
    RefreshPositions
    gePos.FinishSorting Col, IIf(m_RiskView.SortOrder = enSortAscending, flexSortGenericAscending, flexSortGenericAscending)
End Sub

Private Sub gePos_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    m_Aux.PosAfterSort Col, Order
End Sub

Private Sub fgPos_DblClick()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgPos
        m_enMenuGrid = GT_RISKS_POSITIONS
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .rows
        
        HandleGridDblClick True
    End With
End Sub

Private Sub fgPos_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If IsHotKeyGridCommonOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_RISKS_POSITIONS) = True
    End If
End Sub

Private Sub fgPos_KeyPress(KeyAscii As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgPos
        If .Col = 1 And .RowOutlineLevel(.Row) = USD_ID Then
            If KeyAscii = Asc("+") And .IsCollapsed(.Row) = flexOutlineCollapsed Then
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Key ""+"" pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                .IsCollapsed(.Row) = flexOutlineExpanded
                .SetFocus
            ElseIf KeyAscii = Asc("-") And .IsCollapsed(.Row) <> flexOutlineCollapsed Then
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Key """" pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                .IsCollapsed(.Row) = flexOutlineCollapsed
                .SetFocus
            End If
        End If
    End With
End Sub

Private Sub fgPos_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bKeyDown(GT_RISKS_POSITIONS) Then
        m_bKeyDown(GT_RISKS_POSITIONS) = False
        With fgPos
            m_enMenuGrid = GT_RISKS_POSITIONS
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Hot key: Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxTradeNew_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Hot key: Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Hot key: Alt+Ctrl+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Hot key: Alt+Shift+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                'mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Hot key: Ctrl+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxCopy_Click
        
            Case KeyCode = vbKeyG And Shift = vbCtrlMask
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Hot key: Ctrl+G pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxAutosizeGrid_Click
    
            Case KeyCode = vbKeyA And Shift = vbCtrlMask And m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Hot key: Ctrl+A pressed. " & GetOptionInfo, m_frmOwner.Caption
                
                mnuCtxAutosizeCol_Click
        
        End Select
    End If
End Sub

Private Sub fgPos_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Button = vbRightButton Then
        With fgPos
            m_enMenuGrid = GT_RISKS_POSITIONS
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
            
            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
                .Row = m_nMenuGridRow
            End If
            
            ShowPopup
        End With
    End If
End Sub

Private Sub fgPos_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nKey&
    Dim aRowData As MmRvRowData
    
    Cancel = True
    If m_bInProc Or m_Aux.RealTime Or m_bDataLoad Then
        If Not IsDblClickHandled And g_Params.ShowMessageUnableToEdit And fgPos.ColKey(Col) = RPC_CLOSE Then
            FixDblClickHandled
            Dim aFrm As New frmInfoMsgBox
            aFrm.Mode = 1
            aFrm.Show vbModeless
            Set aFrm = Nothing
        End If
        Exit Sub
    End If

    If IsDblClickHandled Then Exit Sub
      
    If m_Aux.Grp.ID <> 0 Then
        With fgPos
            nKey = .ColKey(Col)
            If Err.Number = 0 Then Cancel = Not m_gdPos.Col(nKey).CanEdit
            
            If Not Cancel Then
                Set aRowData = m_RiskView.PosRowData(Row)
                If Not m_RiskView.PosRowData(Row).SynthGreeks Is Nothing Then
                    Cancel = Not (m_RiskView.PosRowData(Row).SynthGreeks.IsTotal And m_RiskView.PosRowData(Row).SynthGreeks.SynthUndID <> USD_ID)
                End If
            End If
            
            If Not Cancel Then
                If aRowData.OutlineLevel = 0 Then
                    If nKey = RPC_ACTIVEPRC And Not aRowData.Und.ActiveFuture Is Nothing Then
                        Cancel = True
                    End If
                End If
            End If
            
            If Not Cancel Then m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
        End With
    End If
End Sub

Private Sub fgTot_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    m_Aux.GridBeforeMoveColumn fgTot, m_gdTot, Col, Position, RTC_NONE
End Sub

Private Sub fgTot_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    m_Aux.GridAfterMoveColumn m_gdTot, Col, Position, 0
End Sub

Private Sub fgTot_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Button = vbRightButton Then
        With fgTot
            m_enMenuGrid = GT_RISKS_TOTALS
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
            ShowPopup
        End With
    End If
End Sub

Private Sub frmLayout_OnApply(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    SetGridLayout enGridType, gdGrid
End Sub

Private Sub frmLayout_OnOK(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Screen.MousePointer = vbHourglass
    SetGridLayout enGridType, gdGrid
    Screen.MousePointer = vbDefault
End Sub

Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    Dim i&, aRowData As EtsMmRisksLib.MmRvRowData
    
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Select Case enGridType
        Case GT_RISKS_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_Aux.GridLock(GT_RISKS_FILTER).LockRedraw
            m_Aux.FormatFltGrid
            m_Aux.FormatFltColumns
            m_Aux.GridLock(GT_RISKS_FILTER).UnlockRedraw
            
        Case GT_RISKS_TOTALS
            gdGrid.CopyTo m_gdTot
            
            m_Aux.GridLock(GT_RISKS_TOTALS).LockRedraw
            m_Aux.FormatTotGrid
            m_AuxOut.TotalsUpdate
            m_Aux.FormatTotColumns
            m_AuxOut.TotalsUpdateBadStatus
            m_Aux.GridLock(GT_RISKS_TOTALS).UnlockRedraw
    
        Case GT_RISKS_POSITIONS
            gdGrid.CopyTo m_gdPos
            RefreshPositions
    
    End Select
    
    UserControl_Resize
End Sub

Private Sub frmWtdVega_OnOK()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ID = 0 Then Exit Sub
        
    m_bInProc = True
    m_AuxClc.UnderlyingsCalcWtdVega
    m_AuxOut.UnderlyingsUpdate False
    m_AuxOut.TotalsUpdate
    RefreshPositions
    m_bInProc = False
    
End Sub

Private Sub m_RiskView_Progress(ByVal bsDescription As String, ByVal Precent As Double)

    pbProgress.Value = Precent
    lblProcess.Caption = bsDescription
    lblProcess.Refresh
    DoEvents

End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bShutDown Or m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_RISKS_FILTER
        If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Filter popup menu. ""RightClick -> Copy Grid"" selected. ", m_frmOwner.GetCaption
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_RISKS_TOTALS
        If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Totals popup menu. ""RightClick -> Copy Grid"" selected. ", m_frmOwner.GetCaption
            g_ClipMgr.CopyGridToClipboard fgTot

        Case GT_RISKS_POSITIONS
        If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> Copy Grid"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
            g_ClipMgr.CopyGridToClipboard fgPos

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxFitVolatilityToMarket_Click()
On Error GoTo error_exception:
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> mnuCtxFitVolatilityToMarket"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    If (Not m_Aux Is Nothing) Then
        If (Not m_Aux.RiskView Is Nothing) Then
            m_Aux.RiskView.FitToMarketVolatility
        End If
    End If
    
    Exit Sub
    
error_exception:
    gCmn.ErrorMsgBox m_frmOwner, "Risks View: Fail to Fit volatility to Implied."
End Sub

Private Sub mnuCtxOtcOptionCalc_Click()
   If Not g_PerformanceLog Is Nothing Then _
                 g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> OTC Option Calc"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    CallOtcOptionCalcRV
End Sub

Private Sub mnuCtxSaveTheoAsClose_Click()
On Error GoTo ErrorExeption

If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> SaveTheoPricesAsClose"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

    If (Not m_RiskView Is Nothing And m_bShutDown <> True And m_Aux.RealTime = False) Then
        pbProgress.Max = 100
        pbProgress.Visible = True
        lblProcess.Visible = True
        lblStatus.Visible = False
        m_RiskView.ConnectionString = g_Params.DbConnection
        m_RiskView.SaveTheoPricesAsClose TradeChannel.Trades
        Refresh
        
        Dim aData As MSGSTRUCTLib.UnderlyingUpdate
        Set aData = New MSGSTRUCTLib.UnderlyingUpdate
        aData.UpdStatus = enUndAllTheoCloseUpdate
        g_TradeChannel.PubUnderlyingUpdate aData
    End If
    
    Exit Sub
    
ErrorExeption:
    gCmn.ErrorMsgBox m_frmOwner, "Risks View: Fail to Save theo price as close"
End Sub

Private Sub mnuCtxTradeExercise_Click()
    On Error Resume Next
    
    If m_bShutDown Or m_bInProc Or m_enMenuGrid <> GT_RISKS_POSITIONS Then Exit Sub
    
    Dim aRowData As EtsMmRisksLib.MmRvRowData, bIsSynth As Boolean
    Dim collTrades As New clsTradeExecColl, frmExecTrades As frmExerciseTrades
    
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
        Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow) 'fgPos.RowData(m_nMenuGridRow)
        
        If Not aRowData Is Nothing Then
            If Not aRowData.Pos Is Nothing Then
                If aRowData.Pos.ContractType = enCtOption Or aRowData.Pos.ContractType = enCtFutOption Then
                    
                    If aRowData.Pos.ContractType = enCtOption Then
                        If Not aRowData.Und.SynthRoots Is Nothing Then
                            bIsSynth = Not aRowData.Und.SynthRoots(aRowData.Pos.OptionRootID) Is Nothing
                        End If
                    
                        TradeChannel.MakeTradesListForExec collTrades, aRowData.Und.ID, 0, aRowData.Pos.ID
                    Else
                        TradeChannel.MakeTradesListForExec collTrades, 0, aRowData.Pos.Fut.ID, aRowData.Pos.ID
                    End If
                    
                    If collTrades.Count > 0 Then
                        Set frmExecTrades = New frmExerciseTrades
                        frmExecTrades.Execute collTrades, True, m_frmOwner
                        Set frmExecTrades = Nothing
                    Else
                        gCmn.MyMsgBox m_frmOwner, "No actual/manual trades for exercise.", vbExclamation
                    End If
                End If
            End If
            Set aRowData = Nothing
        End If
    End If
    
End Sub

Private Sub mnuCtxTradeExpiry_Click()
    On Error Resume Next
    
    If m_bShutDown Or m_bInProc Or m_enMenuGrid <> GT_RISKS_POSITIONS Then Exit Sub
    Dim aRowData As EtsMmRisksLib.MmRvRowData
    Dim collTrades As New clsTradeExecColl, frmExecTrades As frmExerciseTrades
    
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
        Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow) 'fgPos.RowData(m_nMenuGridRow)
        
        If Not aRowData Is Nothing Then
            If Not aRowData.Pos Is Nothing Then
                If aRowData.Pos.ContractType = enCtOption Or aRowData.Pos.ContractType = enCtFutOption Then
                    If aRowData.Pos.ContractType = enCtOption Then
                        TradeChannel.MakeTradesListForExec collTrades, aRowData.Und.ID, 0, aRowData.Pos.ID
                    Else
                        TradeChannel.MakeTradesListForExec collTrades, 0, aRowData.Pos.Fut.ID, aRowData.Pos.ID
                    End If
                    
                    If collTrades.Count > 0 Then
                        Set frmExecTrades = New frmExerciseTrades
                        frmExecTrades.Execute collTrades, False, m_frmOwner
                        Set frmExecTrades = Nothing
                    Else
                        gCmn.MyMsgBox m_frmOwner, "No actual/manual trades for expiry.", vbExclamation
                    End If
                End If
            End If
            Set aRowData = Nothing
        End If
    End If
    
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, _
                                        ByRef aOpt As EtsGeneralLib.EtsOptAtom, _
                                        ByRef bBuy As Boolean, ByRef dPrice#, ByRef nQty&)
    On Error Resume Next
     
    Dim aCurUnd As EtsMmRisksLib.MmRvUndAtom, aCurPos As EtsMmRisksLib.MmRvPosAtom
    Dim aRowData As EtsMmRisksLib.MmRvRowData, aCurSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim aCurUndData As EtsMmRisksLib.IMmRvAggregationDataAtom
    
    bBuy = True
    dPrice = 0#
    nQty = IIf(bIsStock, 100, 10)
    
    If m_enMenuGrid = GT_RISKS_POSITIONS Then
        Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow) '.fgPos.RowData(m_nMenuGridRow)
        Set aCurUnd = aRowData.Und
        Set aCurUndData = aCurUnd
        Set aCurPos = aRowData.Pos
        Set aCurSynthGreek = aRowData.SynthGreeks
        
        Set aUnd = g_Underlying(aCurUnd.ID)
        
        If Not aUnd Is Nothing Then
            If bIsStock Then
                If aCurSynthGreek Is Nothing Then
                    nQty = Round(aCurUndData.NetDelta / 100) * 100
                Else
                    Set aCurUnd = m_Aux.Und(aCurSynthGreek.SynthUndID)
                    Set aCurUndData = aCurUnd
                    nQty = Round(aCurSynthGreek.DeltaInShares / 100) * 100
                End If
                
                If nQty >= 0 Then
                    bBuy = False
                    dPrice = aCurUndData.price.Bid
                    If nQty = 0 Then nQty = 100
                    
                ElseIf nQty < 0 Then
                    nQty = Abs(nQty)
                    bBuy = True
                    dPrice = aCurUndData.price.Ask
                End If
                
                If dPrice <= 0# Then dPrice = aCurUndData.price.Last
                
            Else
                If Not aCurPos Is Nothing Then
                    If aCurPos.ContractType = enCtOption Then
                        Set aOpt = New EtsGeneralLib.EtsOptAtom
                        aOpt.ID = aCurPos.ID
                        aOpt.OptType = IIf(aCurPos.OptType <> 0, enOtCall, enOtPut)
                        aOpt.RootID = aCurPos.OptionRootID
                        aOpt.Symbol = aCurPos.Symbol
                        aOpt.Expiry = aCurPos.Expiry
                        aOpt.Strike = aCurPos.Strike
                    
                        dPrice = aCurPos.Quote.price.Ask
                        If dPrice <= 0# Then dPrice = aCurPos.Quote.price.Last
                    End If
                End If
            End If
            
        End If
        
        Set aRowData = Nothing
        Set aCurUnd = Nothing
        Set aCurPos = Nothing
        Set aCurSynthGreek = Nothing
        
    End If

    If aUnd Is Nothing And m_Aux.Filter(RFC_SYMBOL) = TYPE_UNDERLYING Then _
        Set aUnd = g_Underlying(m_Aux.Filter(RFC_SYMBOL))
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
    On Error Resume Next
    
    If m_bInProc Then Exit Sub
    
    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
    Dim dPrice#, aOpt As EtsGeneralLib.EtsOptAtom, nQty&

    m_bInProc = True
    FillDataForOrderFromCurrentSelection bIsStock, aUnd, aOpt, bBuy, dPrice, nQty
    m_bInProc = False

    On Error GoTo EH
    If bIsStock Then
        frmOrderEntry.NewStockOrder aUnd, bBuy, nQty, dPrice
    Else
        frmOrderEntry.NewOptionOrder aUnd, aOpt, bBuy, nQty, dPrice
    End If

    Set aUnd = Nothing
    Set aOpt = Nothing
    Exit Sub
    
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Risks View: Fail to create new order."
    ResetMenuData
    Set aUnd = Nothing
    Set aOpt = Nothing
    
End Sub

Private Sub mnuCtxOrderNewOption_Click()
    On Error Resume Next
     If Not g_PerformanceLog Is Nothing Then _
                 g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> New Trade"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

    If Not m_bShutDown Then OrderNew False
End Sub

Private Sub mnuCtxOrderNewStock_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
                 g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> New Trade"" selected. " & GetStockInfo, m_frmOwner.GetCaption

    If Not m_bShutDown Then OrderNew True
End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error Resume Next
    
    If m_bShutDown Or m_bInProc Then Exit Sub
    Dim nUndID&, nID&, bBuy As Boolean, aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, nColIdx&
    Dim aRowData As EtsMmRisksLib.MmRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim aFut As EtsMmRisksLib.MmRvFutAtom
    m_bInProc = True
    nUndID = 0
    nID = 0
    bBuy = True
    
    If m_enMenuGrid = GT_RISKS_POSITIONS Then
    
        If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> New Trade"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow) 'fgPos.RowData(m_nMenuGridRow)
            Set aUnd = aRowData.Und
            Set aPos = aRowData.Pos
            Set aSynthGreek = aRowData.SynthGreeks
        End If
        
        If Not aUnd Is Nothing Then
            nColIdx = fgPos.ColKey(m_nMenuGridCol)
            If nColIdx = RPC_BID Then
                bBuy = (g_Params.RiskBidColBuy <> 0)
            ElseIf nColIdx = RPC_ASK Then
                bBuy = (g_Params.RiskBidColBuy = 0)
            Else
                bBuy = (g_Params.RiskOtherColBuy <> 0)
            End If
            
            If Not aPos Is Nothing Then
                nUndID = aUnd.ID
                nID = aPos.ID
                Set aPos = Nothing
            Else
                If aSynthGreek Is Nothing Then
                    If aRowData.IsAggregation Then
                        nUndID = aUnd.ID
                        nID = aRowData.GetAggregationTradeSymbolID
                    Else
                        nUndID = aUnd.ID
                        If aUnd.ContractType <> enCtFutUnd Then
                            nID = nUndID
                        Else
                           nID = nUndID
                           For Each aFut In aUnd.Fut
                            nID = aFut.ID
                            GoTo Done
                           Next
Done:
                        End If
                    End If
                Else
                    nUndID = aSynthGreek.SynthUndID
                    nID = nUndID
                End If
            End If
            
            Set aUnd = Nothing
        End If
        
        Set aRowData = Nothing
        Set aSynthGreek = Nothing
        
    ElseIf m_enMenuGrid = GT_RISKS_FILTER Then
    
        If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Filter popup menu. ""RightClick -> New Trade"" selected. ", m_frmOwner.GetCaption

        If m_Aux.Filter(RFC_SYMBOL) = TYPE_UNDERLYING And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_Aux.Filter(RFC_SYMBOL)
            nID = nUndID
            bBuy = (g_Params.RiskOtherColBuy <> 0)
        End If
        
    End If

    m_bInProc = False
    On Error GoTo EH
    If nUndID <> 0 Then frmTradeEditor.NewTrade nID, nUndID, bBuy, m_frmOwner
    Exit Sub
    
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    ResetMenuData
End Sub

Private Sub mnuCtxPositionTransfer_Click()
    On Error Resume Next
    
    If m_bShutDown Or m_bInProc Then Exit Sub
    Dim nUndID&, nID&, aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom
    Dim aRowData As EtsMmRisksLib.MmRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim sContractDesc$, aGUnd As EtsGeneralLib.UndAtom

    m_bInProc = True
    nUndID = 0
    nID = 0
    
    If m_enMenuGrid = GT_RISKS_POSITIONS Then
    
        If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> New Position Transfer"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow)
            Set aUnd = aRowData.Und
            Set aPos = aRowData.Pos
            Set aSynthGreek = aRowData.SynthGreeks
        End If
        
        If Not aUnd Is Nothing Then
            If Not aPos Is Nothing Then
                nUndID = aUnd.ID
                nID = aPos.ID
                
                Select Case aPos.ContractType
                    Case enCtOption
                        sContractDesc = aPos.Symbol & " (" & aUnd.Symbol & " " & Format$(aPos.Expiry, "MMMM YYYY") & " " & _
                                    CStr(aPos.Strike) & IIf(aPos.OptType = enOtCall, " Call)", " Put)")
                                
                    Case enCtFuture
                        sContractDesc = aPos.Symbol & " (" & aPos.Fut.ContractName & ")"
                
                    Case enCtFutOption
                        sContractDesc = aPos.Symbol & " (" & aPos.Fut.Symbol & " " & Format$(aPos.Expiry, "MMMM YYYY") & " " & _
                                    CStr(aPos.Strike) & IIf(aPos.OptType = enOtCall, " Call)", " Put)")
                End Select
                
                Set aPos = Nothing
            Else
                If aSynthGreek Is Nothing Then
                    nUndID = aUnd.ID
                Else
                    nUndID = aSynthGreek.SynthUndID
                End If
                nID = nUndID
            End If
            
            Set aUnd = Nothing
        End If
        
        Set aRowData = Nothing
        Set aSynthGreek = Nothing
        
    ElseIf m_enMenuGrid = GT_RISKS_FILTER Then
    
        If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Filter popup menu. ""RightClick -> Position Transfer"" selected. ", m_frmOwner.GetCaption

        If m_Aux.Filter(RFC_SYMBOL) > 0 And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_Aux.Filter(RFC_SYMBOL)
        End If
        
    End If

    m_bInProc = False
    If nUndID <> 0 Then
        If nID = nUndID Then ' it's an underlying position or symthetic underlying row
            Set aGUnd = g_UnderlyingAll(nUndID)
            If Not aGUnd Is Nothing Then
                sContractDesc = aGUnd.Symbol & " (" & aGUnd.ContractName & ")"
                Set aGUnd = Nothing
            End If
        End If
        
        On Error GoTo EH
        frmPositionTransfer.TransferPosition nUndID, nID, _
                        IIf(m_Aux.Filter(RFC_TRADER) > 0, m_Aux.Filter(RFC_TRADER), 0), _
                        sContractDesc, m_frmOwner
    End If
    Exit Sub
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    ResetMenuData
End Sub

Private Sub mnuCtxUseMaualPrice_Click()

    Dim aRowData As EtsMmRisksLib.MmRvRowData
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aFut As MmRvFutAtom
    Dim bUseManualPrice As Boolean, nID As Long, dPrice As Double
    
    Dim updFut As MmRvFutAtom
    Dim updUnd As MmRvUndAtom

    If m_enMenuGrid = GT_RISKS_POSITIONS Then
    
        If m_nMenuGridRow > 0 Then
            Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow)
            Set aUnd = aRowData.Und
            Set aPos = aRowData.Pos
            Set aFut = aRowData.Fut
            
            bUseManualPrice = Not mnuCtxUseMaualPrice.Checked
            
            If Not aPos Is Nothing Then
                If (aPos.ContractType = enCtFuture) Then
                    If (Not aPos.Und Is Nothing) Then
                        If (Not aPos.Und.Fut(aPos.ID) Is Nothing) Then
                            Set updFut = aPos.Und.Fut(aPos.ID)
                            If (IsFutEditable(updFut)) Then
                                dPrice = updFut.price.Active
                                UpdateFutManualPrice updFut, dPrice, IIf(bUseManualPrice, enUsAdd, enUsDelete)
                            End If
                        End If
                    End If
                ElseIf (aPos.ContractType = enCtIndex Or aPos.ContractType = enCtStock) Then
                    If (Not aPos.Und Is Nothing) Then
                        If (aPos.ID = aPos.Und.ID) Then
                            Set updUnd = aPos.Und
                            If (IsUndEditable(updUnd)) Then
                                dPrice = updUnd.price.Active
                                UpdateUndManualPrice updUnd, dPrice, IIf(bUseManualPrice, enUsAdd, enUsDelete)
                            End If
                        End If
                    End If
                ElseIf (aPos.ContractType = enCtFutOption Or aPos.ContractType = enCtOption) Then
                    UpdatePosOptManualPrice aPos, aPos.Quote.price.Active, IIf(bUseManualPrice, enUsAdd, enUsDelete)
                End If
            ElseIf Not aFut Is Nothing Then
                If (IsFutEditable(aFut)) Then
                    dPrice = aFut.price.Active
                    UpdateFutManualPrice aFut, dPrice, IIf(bUseManualPrice, enUsAdd, enUsDelete)
                End If
            ElseIf Not aUnd Is Nothing Then
                If (IsUndEditable(aUnd)) Then
                    dPrice = aUnd.price.Active
                    UpdateUndManualPrice aUnd, dPrice, IIf(bUseManualPrice, enUsAdd, enUsDelete)
                End If
            End If
                       
            'Recalc And Refresh
            RefreshRiskView
            
            Set updFut = Nothing
            Set updUnd = Nothing
                    
        End If
        
    End If
    
End Sub
Public Sub RefreshRiskView()
On Error Resume Next

    m_AuxClc.UnderlyingsCalc True, True, False, False
    m_AuxOut.UnderlyingsUpdate False
    
    RefreshPositions
    
    m_AuxOut.TotalsUpdate
    
End Sub

Private Sub mnuCtxWtdVega_Click()
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Weighted Vega Settings"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

    If Not m_bShutDown Then ShowWeightedVegaSettings
End Sub

Private Sub CheckRtCount()
    Dim aForm As clsFormAtom
    Dim nRtCount As Long
    Dim i As Long
   
    nRtCount = 0&
   
    For i = 1 To g_ViewFrm.Count
        Set aForm = g_ViewFrm(i)
        If TypeOf aForm.Frm Is frmRiskView Then
            If aForm.Frm.IsRealTime Then
                nRtCount = nRtCount + 1
                aForm.Frm.CheckPassiveRealtime
            End If
            Set aForm = Nothing
        End If
    Next
    g_RvRTQuantity = nRtCount
    
    Debug.Print "Count of RT Risks windows: " & Str(nRtCount)
    
End Sub

Private Sub tmrRealTime_Timer()
On Error GoTo error_handler
        
        Recalculate
        
        RefreshPositions
        
        AdjustCaption
    Exit Sub
error_handler:
    MsgBox "Exception OnTimer() (" & Err.Description & " : " & Err.Number & ")"
End Sub

Private Sub Recalculate()
On Error GoTo error_handler
                
        m_AuxClc.UnderlyingsCalc True, True, True
        m_AuxOut.UnderlyingsUpdate False, True
        m_AuxOut.TotalsUpdate
        
        UpdateRecalculationTimeStemp
        
    Exit Sub
    
error_handler:
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Exception while calling Recalculate() " & _
                                                  "Error: " & Err.Description & "(" & CStr(Err.Number) & ")", _
                                                   m_frmOwner.GetCaption
End Sub

'comment this sub in production for prevent statistic output
Private Sub UpdateStat()
    On Error Resume Next
    Debug.Print Format$(m_nLastDataSize, "#0000") & _
                vbTab & Format$(m_nLastRecalcCycle, "#000000") & _
                vbTab & Format$(m_nLastOutCycle, "#000000") & _
                vbTab & Format$(m_nLastRecalcCycle + m_nLastOutCycle, "#000000")
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    Dim nOperation&, i&
    Dim aRowData As EtsMmRisksLib.MmRvRowData
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Risk Refresh Started.", m_frmOwner.GetCaption
        
    tmrShow.Interval = 10
    tmrShow.Enabled = False
    
    If m_bShutDown Or m_bDataLoad Then Exit Sub
    m_bInProc = True

    m_AuxClc.CalcDate = GetNewYorkTime
    fgFlt.TextMatrix(1, RFC_SIM_DATE) = Now
    
    If m_Aux.Grp.ID <> 0 Then
        
        If m_Aux.RealTime Then
            If g_RvRTQuantity > 0 Then
                g_RvRTQuantity = g_RvRTQuantity - 1
            End If
        End If
        
        'm_Aux.RealTime = False
        
        If m_bTmrUndCalcNow Then
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "FAIL: in calculation mode now.", m_frmOwner.GetCaption
            
            ' another try
            tmrShow.Interval = 50
            tmrShow.Enabled = True
            Exit Sub
        End If
    
        imgStop.Visible = False
        imgStopDis.Visible = False
        pbProgress.Visible = False
        lblProcess.Visible = False
        lblStatus.Visible = True
        lblProcess.Caption = ""
    End If
    
    ClearViewAndData
    
    m_bInProc = False
    
    If Not g_PerformanceLog Is Nothing Then _
        nOperation = g_PerformanceLog.BeginLogMmOperation
    

    lblStatus.Visible = False
    imgStop.Visible = True
    imgStopDis.Visible = False
    
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = TradeChannel.Trades.Count
    pbProgress.Visible = True
    
    lblProcess.Caption = "Data loading..."
    lblProcess.Visible = True
    lblProcess.Refresh
        
    If PositionsLoad Then
    
        AdjustCaption
        
        IndexLoad
        
        lblProcess.Caption = "Filter applying..."
        lblProcess.Refresh
        
        m_Aux.Filter(RFC_EXPIRY) = m_nOpenedExpiry
        m_Aux.FilterUpdateExpiry False
    
        imgStop.Visible = False
        imgStopDis.Visible = False
        pbProgress.Visible = False
        lblProcess.Visible = False
        lblStatus.Visible = True
        
        If g_Params.RiskDirectlyToRealtime Then
            StartRealTime
        End If
    Else
    
        If m_Aux.Grp.ID <> -1 Then
            ClearViewAndData
            m_Aux.FilterUpdateAll
            AdjustCaption
            AdjustState
        Else
            SetRefreshHint True
        End If
        
    End If
    
    imgStop.Visible = False
    imgStopDis.Visible = False
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = True
    
    m_nOpenedExpiry = 0
    
    Screen.MousePointer = vbDefault
    
    Recalculate
    
    RefreshPositions
    
    UserControl_Resize
    
    RaiseEvent OnRefreshComplete
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Risk Refresh Finished.", m_frmOwner.GetCaption
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation nOperation, OPER_LOADDATA, m_frmOwner.GetCaption, m_AuxClc.UndCount, m_AuxClc.OptCount, m_AuxClc.FutCount

End Sub

Private Function IndexLoad() As Boolean
    On Error Resume Next
    IndexLoad = False
    If m_bInProc Then Exit Function
    Dim aGIdx As EtsGeneralLib.IndexAtom, aComp As EtsGeneralLib.IndexCompAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, sKey$, sKeyActiveFuture$
    Dim aReq As EtsMmRisksLib.MmRvReqAtom, aReqActiveFuture As EtsMmRisksLib.MmRvReqAtom, nKey&
    Dim aUndActive As UndAtom, aUndIdx As UndAtom
    
    m_bInProc = True
    AdjustState
    
    If m_Aux.Idx.ID <> 0 Then
        For Each aUnd In m_Aux.Und
            aUnd.Beta = BAD_DOUBLE_VALUE
        Next
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogInformation, "Set active index " & m_Aux.Idx.Symbol, m_frmOwner.GetCaption
        
        sKey = CStr(enCtIndex) & "_" & m_Aux.Idx.Symbol
        Set aReq = m_AuxClc.QuoteReqsAll(sKey)
        
        Dim aAcFut As EtsMmRisksLib.MmRvFutAtom
        ' check for existing pp request for active future for previous future
        If Not m_Aux.Idx.ActiveFuture Is Nothing Then
            sKeyActiveFuture = CStr(enCtFuture) & "_" & m_Aux.Idx.ActiveFuture.Symbol
            Set aReqActiveFuture = m_AuxClc.QuoteReqsAll(sKeyActiveFuture)
        End If
        
        If Not aReq Is Nothing Then
            If aReq.IndexOnly Then
                Set aReq = Nothing
                
                m_AuxClc.QuoteReqsAll.Remove sKey
                m_AuxClc.UndCount = m_AuxClc.UndCount - 1
                
                If Not aReqActiveFuture Is Nothing Then
                    m_AuxClc.QuoteReqsAll.Remove sKeyActiveFuture
                    m_AuxClc.FutCount = m_AuxClc.FutCount - 1
                End If
                
                Set aReqActiveFuture = Nothing
                
            End If
        End If
    
        m_Aux.Idx.Clear
    End If
    
    If m_Aux.Filter(RFC_INDEX) <> 0 Then
    
        nKey = m_Aux.Filter(RFC_INDEX)
        Set aGIdx = g_HedgeSymbols(nKey)
        Debug.Assert Not aGIdx Is Nothing
        
        If Not aGIdx Is Nothing Then
            m_Aux.Idx.ID = m_Aux.Filter(RFC_INDEX)
            m_Aux.Idx.Symbol = aGIdx.Symbol
            m_Aux.Idx.ContractType = enCtIndex
            
            For Each aUnd In m_Aux.Und
                Set aComp = aGIdx.Components(aUnd.ID)
                
                If Not aComp Is Nothing Then
                    aUnd.Beta = aComp.Beta
                    Set aComp = Nothing
                End If
            Next
            
            sKey = CStr(enCtIndex) & "_" & m_Aux.Idx.Symbol
            Set aReq = m_AuxClc.QuoteReqsAll(sKey)
            If aReq Is Nothing Then
                Set aReq = m_AuxClc.QuoteReqsAll.Add(sKey)
                Set aReq.Und = m_Aux.Idx
                aReq.IndexOnly = True
                m_AuxClc.UndCount = m_AuxClc.UndCount + 1
            Else
                m_Aux.Idx.price.Bid = aReq.Und.price.Bid
                m_Aux.Idx.price.Ask = aReq.Und.price.Ask
                m_Aux.Idx.price.Last = aReq.Und.price.Last
                m_Aux.Idx.price.NetChange = aReq.Und.price.NetChange
            End If
            
            Set aUndIdx = g_ContractAll(m_Aux.Idx.ID).Und
            
            If (Not aUndIdx Is Nothing) Then
                m_Aux.Idx.price.Close = aUndIdx.PriceClose
                m_Aux.Idx.price.TheoClose = aUndIdx.PriceTheoclose
                If (aUndIdx.manualActivePrice > 0) Then
                    m_Aux.Idx.price.IsUseManualActive = True
                    m_Aux.Idx.price.Active = aUndIdx.manualActivePrice
                End If
                Set aUndIdx = Nothing
            End If
            
            Set m_Aux.Idx.UndPriceProfile = aGIdx.UndPriceProfile
            Set m_Aux.Idx.OptPriceProfile = aGIdx.OptPriceProfile
            
            ' try to get active future for selected index
            '******************************************************
            Set m_Aux.Idx.ActiveFuture = Nothing    ' delete active future for previous active

            If m_Aux.Idx.ActiveFuture Is Nothing Then
                If Not g_ContractAll(m_Aux.Idx.ID) Is Nothing Then
                    Set aUndActive = g_ContractAll(m_Aux.Idx.ID).Und
                    If Not aUndActive Is Nothing Then
                        If Not aUndActive.ActiveFuture Is Nothing Then
                            If m_Aux.Idx.ActiveFuture Is Nothing Then
                                sKey = CStr(enCtFuture) & "_" & aUndActive.ActiveFuture.Symbol 'm_Aux.Idx.ActiveFuture.Symbol
                                Set aReq = m_AuxClc.QuoteReqsAll(sKey)
                                If Not aReq Is Nothing Then
                                    Set m_Aux.Idx.ActiveFuture = aReq.Fut
                                Else
                                    Set m_Aux.Idx.ActiveFuture = New EtsMmRisksLib.MmRvFutAtom
                                    ' fill up active future info for newly selected index
                                    If Not aUndActive Is Nothing Then
                                        m_Aux.Idx.ActiveFuture.ID = aUndActive.ActiveFuture.ID
                                        Set m_Aux.Idx.ActiveFuture.UndPriceProfile = aUndActive.ActiveFuture.UndPriceProfile
                                        Set m_Aux.Idx.ActiveFuture.OptPriceProfile = aUndActive.ActiveFuture.OptPriceProfile
                                        m_Aux.Idx.ActiveFuture.CalcGreeks = False
                                        m_Aux.Idx.ActiveFuture.bAsIs = aUndActive.ActiveFuture.FutureBasis
                                        m_Aux.Idx.ActiveFuture.Symbol = aUndActive.ActiveFuture.Symbol
                                        m_Aux.Idx.ActiveFuture.ContractName = aUndActive.ActiveFuture.ContractName
                                        m_Aux.Idx.ActiveFuture.IsAmerican = aUndActive.ActiveFuture.IsAmerican
                                        m_Aux.Idx.ActiveFuture.FutRootID = aUndActive.ActiveFuture.FutRootID
                                        m_Aux.Idx.ActiveFuture.Maturity = aUndActive.ActiveFuture.MaturityDate
                                        m_Aux.Idx.ActiveFuture.price.Close = aUndActive.ActiveFuture.PriceClose
                                        m_Aux.Idx.ActiveFuture.price.TheoClose = aUndActive.ActiveFuture.PriceTheoclose
                                        If Not aUndActive.FutRoots(m_Aux.Idx.ActiveFuture.FutRootID) Is Nothing Then
                                            m_Aux.Idx.ActiveFuture.FutRootSymbol = aUndActive.FutRoots(m_Aux.Idx.ActiveFuture.FutRootID).Symbol
                                            m_Aux.Idx.ActiveFuture.FutLotSize = aUndActive.FutRoots(m_Aux.Idx.ActiveFuture.FutRootID).FutLotSize
                                            m_Aux.Idx.ActiveFuture.UndID = aUndActive.FutRoots(m_Aux.Idx.ActiveFuture.FutRootID).UndID
                                            m_Aux.Idx.ActiveFuture.OptLotSize = aUndActive.FutRoots(m_Aux.Idx.ActiveFuture.FutRootID).OptLotSize
                                        End If
                                        If aReq Is Nothing Then
                                            Set aReq = m_AuxClc.QuoteReqsAll.Add(sKey)
                                            Set aReq.Fut = m_Aux.Idx.ActiveFuture
                                            m_AuxClc.FutCount = m_AuxClc.FutCount + 1
                                        End If
                                    
                                    End If
                                End If
                            End If
                        End If
                    End If
                End If
            End If
            '******************************************************
            
            Set aGIdx = Nothing
            Set aReq = Nothing
        Else
            m_Aux.Filter(RFC_INDEX) = 0
            m_Aux.FilterUpdateIndex True
        End If
    End If
    
    IndexLoad = True
    m_bInProc = False
    AdjustState
End Function

Private Sub ClearViewAndData()
    On Error Resume Next
    
    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        .FlexDataSource = Nothing
        'fgPos.Rows = 1
        m_Aux.FormatPosGrid
        m_Aux.FormatPosColumns
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
    
    m_Aux.RiskView.cvRTContext.Clear
    
    fgTot.Cell(flexcpText, 1, 0, 1, fgTot.Cols - 1) = ""
    fgTot.Row = 1

    m_Aux.Grp.Clear
    m_Aux.Grp.ID = -1
    m_Aux.Grp.Name = "<All>"
    
    m_Aux.RiskView.QuoteReqsAll.Clear
    m_Aux.RiskView.Und.Clear
    
    m_nCurUnd = 0
    
    m_Aux.FormatFltGrid
    m_Aux.FormatFltColumns
    
    m_Aux.FormatTotGrid
    m_Aux.FormatTotColumns
    
    m_Aux.FormatPosGrid

    SetRefreshHint False
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    Dim aRowData As EtsMmRisksLib.MmRvRowData
    Dim aPos As MmRvPosAtom
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxTradeExercise.Enabled = False
    mnuCtxTradeExpiry.Enabled = False
    mnuCtxOrderNewStock.Visible = g_Params.OrdersVisible
    mnuCtxOrderNewOption.Visible = g_Params.OrdersVisible
    
    mnuCtxOrderNewStock.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    mnuCtxOrderNewOption.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    Select Case m_enMenuGrid
        Case GT_RISKS_FILTER
            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_Aux.Filter(RFC_SYMBOL) = TYPE_UNDERLYING And m_Aux.Filter(RFC_SYMBOL) <> 0 _
                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
            
            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
            
            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            
            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
        Case GT_RISKS_TOTALS
            mnuCtxTradeNew.Enabled = False
            mnuCtxPositionTransfer.Enabled = False
            
            mnuCtxHideCol.Enabled = (m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols And m_gdTot.Idx(1) <> RTC_NONE)
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            
            PopupMenu mnuCtx
        
        Case GT_RISKS_POSITIONS
            Dim bEnabled As Boolean: bEnabled = False
            Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow) 'fgPos.RowData(m_nMenuGridRow)
            
            If Not aRowData Is Nothing Then
                If Not aRowData.SynthGreeks Is Nothing Then
                    bEnabled = (aRowData.SynthGreeks.IsTotal = False _
                                Or aRowData.SynthGreeks.SynthUndID <> USD_ID)
                Else
                    bEnabled = True
                    
                    If Not aRowData.Pos Is Nothing Then
                        mnuCtxTradeExpiry.Enabled = (Not m_bInProc _
                                                    And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                                    And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
                                                    And (aRowData.Pos.ContractType = enCtOption Or aRowData.Pos.ContractType = enCtFutOption))
                                                                        
                        mnuCtxTradeExercise.Enabled = mnuCtxTradeExpiry.Enabled
                    End If
                End If
            End If
            
            mnuCtxTradeNew.Enabled = (Not m_bInProc _
                                        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                        And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
                                        And bEnabled)
            
            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
            
            mnuCtxHideCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols And m_gdPos.Idx(1) <> RPC_NONE)
            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxAutosizeGrid.Enabled = True
            
            '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
            '''''''''''''''''''''''''''''Manual Price Menu'''''''''''''''''''''''''''''''''''''''
            '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

            Set aPos = aRowData.Pos
            If Not aPos Is Nothing Then
                mnuCtxUseMaualPrice.Checked = aRowData.Pos.Quote.price.IsUseManualActive
                If (aPos.ContractType = enCtFuture) Then
                    mnuCtxUseMaualPrice.Enabled = IsFutEditable(aPos.Fut)
                ElseIf (aPos.ContractType = enCtStock Or aPos.ContractType = enCtIndex) Then
                    mnuCtxUseMaualPrice.Enabled = IsUndEditable(aPos.Und)
                Else
                    mnuCtxUseMaualPrice.Enabled = True
                End If
                Set aPos = Nothing
            Else
                If Not aRowData.Fut Is Nothing Then
                        mnuCtxUseMaualPrice.Checked = aRowData.Fut.price.IsUseManualActive
                        mnuCtxUseMaualPrice.Enabled = IsFutEditable(aRowData.Fut)
                Else
                        If Not aRowData.Und Is Nothing Then
                            mnuCtxUseMaualPrice.Checked = aRowData.Und.price.IsUseManualActive
                            mnuCtxUseMaualPrice.Enabled = IsUndEditable(aRowData.Und)
                        End If
                End If
            End If
            
            
            Set aRowData = Nothing
            
            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
    End Select
    
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
End Sub

Private Sub mnuCtxRealTime_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Real Time Mode"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    ToggleRealtime
End Sub

Public Sub ToggleRealtime()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
       
    SetRefreshHint False
        
    'In real time day shift 0
    m_AuxClc.CalcDate = GetNewYorkTime
    'mov to date shift menu today DATE
    fgFlt.TextMatrix(1, RFC_SIM_DATE) = Now
    'Start Real Time
    'Hide date shift menu
    'fgFlt.ColHidden(RFC_SIM_DATE) = Not m_Aux.RealTime And m_AuxClc.QuoteReqsAll.Count > 0
   
    If Not m_Aux.RealTime Then
        StartRealTime
    Else
        StopRealTime
    End If
                        
End Sub

Private Sub StopRealTime()
    On Error Resume Next
    
    tmrRealTime.Enabled = False
    m_Aux.RealTime = False
    
    m_Aux.RiskView.setRealtime False
    
    lblStatus.Visible = True
    pbProgress.Visible = False
    lblProcess.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    lblStatus.Caption = "Real Time (Quotes unsubscription...)"
    lblStatus.Refresh
    
    If g_RvRTQuantity > 0 Then
       g_RvRTQuantity = g_RvRTQuantity - 1
    End If
    
    Screen.MousePointer = vbDefault
    AdjustState
    AdjustCaption
End Sub

Private Function StartRealTime() As Boolean

    tmrRealTime.Enabled = True
    
    m_Aux.RealTime = True
    m_Aux.RealTimeConnected = True
    
    m_Aux.RiskView.setRealtime True
        
    lblStatus.Visible = True
    pbProgress.Visible = False
    lblProcess.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    lblStatus.Caption = "Real Time"
    lblStatus.Refresh
    
    If g_RvRTQuantity > 0 Then
       g_RvRTQuantity = g_RvRTQuantity - 1
    End If
    
    Screen.MousePointer = vbDefault
    
    AdjustState
    AdjustCaption

    
End Function

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Refresh"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    If Not m_bShutDown Then
        'In real time day shift 0
        m_AuxClc.CalcDate = GetNewYorkTime
        'mov to date shift menu today DATE
        fgFlt.TextMatrix(1, RFC_SIM_DATE) = Now
        ' show this filter
        fgFlt.ColHidden(RFC_SIM_DATE) = m_Aux.RealTime
        Refresh
    End If
End Sub

Private Sub mnuCtxRefreshPrices_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> RefreshPrices"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    If Not m_bShutDown Then RefreshPrices
End Sub


Private Sub mnuCtxAutosizeCol_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_RISKS_POSITIONS
            With fgPos
                 If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> Autosize Column"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
                .AutoSize m_nMenuGridCol, m_nMenuGridCol
                
                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= RPC_SYMBOL And nIdx <= g_RPC_LAST_COLUMN Then
                    m_gdPos.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                End If
                
                m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxAutosizeGrid_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&, i&, nCount&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_RISKS_POSITIONS
            With fgPos
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> Autosize Grid"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
                .AutoSize 0, .Cols - 1
                
                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= RPC_SYMBOL And nIdx <= g_RPC_LAST_COLUMN Then
                        m_gdPos.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next
                
                m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxHideCol_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    Dim i&, nColIdx&, bMove As Boolean
    
    Select Case m_enMenuGrid
        Case GT_RISKS_TOTALS
            If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Totals popup menu. ""RightClick -> Hide Column"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
            bMove = False
            
            If m_gdTot.Idx(1) = RTC_NONE Then Exit Sub
            
            nColIdx = fgTot.ColKey(m_nMenuGridCol)
            For i = 0 To g_RTC_LAST_COLUMN
                If m_gdTot.Idx(i) = nColIdx Then
                    m_gdTot.Idx(i) = RTC_NONE
                    m_gdTot.Col(nColIdx).Visible = False
                    bMove = True
                End If
                
                If bMove Then
                    If i + 1 <= g_RTC_LAST_COLUMN Then
                        m_gdTot.Idx(i) = m_gdTot.Idx(i + 1)
                    Else
                        m_gdTot.Idx(i) = RTC_NONE
                    End If
                End If
            Next
            
            If bMove Then
                m_Aux.FormatTotGrid
                m_Aux.FormatTotColumns
                m_AuxOut.TotalsUpdate
            End If

        Case GT_RISKS_POSITIONS
            If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> Hide Column"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
            bMove = False
            
            nColIdx = fgPos.ColKey(m_nMenuGridCol)
            For i = 1 To g_RPC_LAST_COLUMN
                If m_gdPos.Idx(i) = nColIdx Then
                    m_gdPos.Idx(i) = RPC_NONE
                    m_gdPos.Col(nColIdx).Visible = False
                    bMove = True
                End If
                
                If bMove Then
                    If i + 1 <= g_RPC_LAST_COLUMN Then
                        m_gdPos.Idx(i) = m_gdPos.Idx(i + 1)
                    Else
                        m_gdPos.Idx(i) = RPC_NONE
                    End If
                End If
            Next
            
            If bMove Then
                m_Aux.FormatPosGrid
                m_Aux.FormatPosColumns
                m_AuxOut.UnderlyingsUpdate True
                RefreshPositions
            End If
    End Select
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Select Case m_enMenuGrid
        Case GT_RISKS_FILTER
            If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Filter popup menu. ""RightClick -> Grid Layout"" selected. ", m_frmOwner.GetCaption
            CustomizeFltGridLayout

        Case GT_RISKS_TOTALS
            If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Totals popup menu. ""RightClick -> Grid Layout"" selected. ", m_frmOwner.GetCaption
            CustomizeTotGridLayout
        
        Case GT_RISKS_POSITIONS
            If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> Grid Layout"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
            CustomizePosGridLayout
        
    End Select
End Sub

Private Sub AddTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error GoTo EH
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom
    Dim sKey$
    Dim i&, nCount&, aExp As EtsGeneralLib.EtsMmEntityAtom, bAddUnd As Boolean, bAddPos As Boolean, nRow&, nUndRow&, nGreekRow&
    Dim bGridLocked As Boolean: bGridLocked = False
    Dim aRowData As clsRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim bRatesUpdated As Boolean: bRatesUpdated = False
    Dim aGIdx As EtsGeneralLib.IndexAtom, aIdxComp As EtsGeneralLib.IndexCompAtom
    Dim aNewReqsAll As New EtsMmRisksLib.MmRvReqColl
    Dim bNeedRefresh As Boolean
    
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTrade Enter", m_frmOwner.GetCaption

    If Not m_Aux.CheckTradeFilter(aTrd) Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "TradeFilter passed", m_frmOwner.GetCaption
    
    m_bInProc = True
    AdjustState
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "Add Trade, Symbol: " & aTrd.Symbol, m_frmOwner.GetCaption
    
    Set aUnd = m_Aux.Und(aTrd.UndID)
    If aUnd Is Nothing Then
        Set aUnd = m_RiskView.AddNewUnderlying(aTrd, aNewReqsAll)
        bAddUnd = True
        
        If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "Underlying added, Symbol: " & aUnd.Symbol, m_frmOwner.GetCaption
    End If
    
    Set aPos = aUnd.Pos(aTrd.ContractID)
    If aPos Is Nothing Then
        Set aPos = m_RiskView.AddNewPosition(aTrd, aUnd, aNewReqsAll, -1, True)
        
        If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "Position added, Symbol: " & aPos.Symbol, m_frmOwner.GetCaption
    
    End If
    
    bAddPos = True
    m_RiskView.AddNewTradeToPosition aUnd, aPos, aTrd

    If bAddUnd Then
        Set aGIdx = g_HedgeSymbols(m_Aux.Idx.ID)
        If Not aGIdx Is Nothing Then
            Set aIdxComp = aGIdx.Components(aUnd.ID)
            If Not aIdxComp Is Nothing Then
                aUnd.Beta = aIdxComp.Beta
                Set aIdxComp = Nothing
            End If
            Set aGIdx = Nothing
        End If
    End If

    bRatesUpdated = m_AuxClc.UnderlyingAdjustRates(aUnd, bAddUnd Or bAddPos)
    
    If bAddUnd Or bAddPos Then m_bTradeActionrefreshPositions = True
    
    If Not aNewReqsAll Is Nothing Then
         If aNewReqsAll.Count > 0 Then
             If m_Aux.RealTime Then
                 If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogEnhDebug, "SubscribeToNewPositions Call", m_frmOwner.GetCaption
             Else
                 If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Set Refresh Hint Call", m_frmOwner.GetCaption
                 SetRefreshHint True
             End If
        End If
    End If


    Set aPos = Nothing
    Set aUnd = Nothing

    m_bInProc = False
    m_Aux.Grp.ID = 1
    AdjustState

    If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTrade Exit, Symbol: " & aTrd.Symbol, m_frmOwner.GetCaption

    Exit Sub
    
EH:
    If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTrade OnError, Symbol: " & aTrd.Symbol, m_frmOwner.GetCaption

    m_bInProc = False
    LogEvent EVENT_ERROR, "Risks: Fail to add trade. " & Err.Description
    On Error Resume Next
    If bGridLocked Then m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    AdjustState
End Sub

Private Sub DeleteTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error GoTo EH
    
    If m_bShutDown = True Then
        Exit Sub
    End If
    
    If aTrd Is Nothing Then
        Exit Sub
    End If
    
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom
    Dim sKey$, nRow&, nUndRow&
    Dim bRatesUpdated As Boolean: bRatesUpdated = False
    
    If Not m_Aux.CheckTradeFilter(aTrd) Then Exit Sub
        
    Set aUnd = m_Aux.Und(aTrd.UndID)
    If aUnd Is Nothing Then Exit Sub
    
    Set aPos = aUnd.Pos(aTrd.ContractID)
    If aPos Is Nothing Then
        Set aUnd = Nothing
        Exit Sub
    End If
    
    m_bInProc = True
    AdjustState
    aTrd.Quantity = aTrd.Quantity * -1
    AddTrade aTrd
    aTrd.Quantity = aTrd.Quantity * -1
    m_bInProc = False
    AdjustState
    
    Exit Sub
EH:
    If m_bShutDown = True Then
        Exit Sub
    End If
    m_bInProc = False
    LogEvent EVENT_ERROR, "Risks: Fail to delete trade. " & Err.Description
    AdjustState
End Sub
    
Private Sub tmrUndCalc_Timer()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "UndCalc_Timer Enter.", m_frmOwner.GetCaption
        
    tmrUndCalc.Enabled = False

    If m_bDataLoad Or m_bTmrUndCalcNow Then
    
            m_bIsNewTrades = False
            tmrUndCalc.Enabled = False
            
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogDebug, "UndCalc_Timer Terminate.", m_frmOwner.GetCaption
            Exit Sub
            
    ElseIf m_bInProc Then
    
            m_bIsNewTrades = False
            tmrUndCalc.Enabled = False
            
            If m_bTradeActionrefreshPositions Then
                m_bTradeActionrefreshPositions = False
                RefreshPositions
            End If
            
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogDebug, "UndCalc_Timer Terminate.", m_frmOwner.GetCaption
            Exit Sub
    End If
    
    If m_bIsNewTrades Then
    
        m_bIsNewTrades = False
        tmrUndCalc.Enabled = True
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogDebug, "UndCalc_Timer Exit. Has new trades", m_frmOwner.GetCaption
        Exit Sub
        
    End If

    m_bInProc = True
    m_bTmrUndCalcNow = True
    
    If m_Aux.RealTime Then
    
        tmrRealTime.Enabled = True
        
        If m_bTradeActionrefreshPositions Then
            m_bTradeActionrefreshPositions = False
            RefreshPositions
        Else
            fgPos.Refresh
        End If
        
'    Else
'
'        m_AuxClc.UnderlyingsCalc True, False
'
'        If m_bTradeActionrefreshPositions Then
'            m_bTradeActionrefreshPositions = False
'            RefreshPositions
'        Else
'            fgPos.Refresh
'        End If
'
'        m_AuxOut.TotalsUpdate
        
    End If
    
    m_bInProc = False
    m_bTmrUndCalcNow = False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "UndCalc_Timer Exit.", m_frmOwner.GetCaption
        
End Sub

Private Sub TradeChannel_AssetUpdate(aUndData As MSGSTRUCTLib.UnderlyingUpdate)
    On Error GoTo Exception
        If (m_bShutDown) Then Exit Sub
        Dim bChange As Boolean
        Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    
        bChange = False
        
        If (aUndData.UpdStatus And enUndAggregationUpdate) Then
            Set aUnd = m_Aux.Und(aUndData.UndID)
            If (Not aUnd Is Nothing) Then
            
                'check for modifications
                If (aUnd.ID = aUndData.UndID) Then
                    If (aUndData.UseHead <> aUnd.PriceByHead) Then
                        bChange = True
                        aUnd.PriceByHead = aUndData.UseHead
                    End If
                    If (aUndData.Coeff <> aUnd.Coeff) Then
                        bChange = True
                        aUnd.Coeff = aUndData.Coeff
                    End If
                    aUnd.price.IsDirty = bChange
                End If
                
                'recalculate and refresh all data
                If bChange Then
                    m_AuxClc.UnderlyingsCalc True, True, False, False
                    m_AuxOut.UnderlyingsUpdate False
                    RefreshPositions
                    m_AuxOut.TotalsUpdate
                End If
                
            End If
        End If
    Exit Sub
Exception:
    Debug.Print "Error while try to update Asset configuration"
End Sub

Private Sub TradeChannel_DividendTypeUpdate(aUndData As MSGSTRUCTLib.UnderlyingUpdate)
    On Error GoTo Exception
    
        If (m_bShutDown) Then Exit Sub
        Dim bChange As Boolean
        Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    
        bChange = False
        
        If (aUndData.UpdStatus And enUndDividendUpdate) Then
            Set aUnd = m_Aux.Und(aUndData.UndID)
            If (Not aUnd Is Nothing) Then
            
                'check for modifications
                If (aUnd.ID = aUndData.UndID) Then
                    If (aUndData.DivType = enDivIndexYield) Then
                            aUnd.Dividend.DivType = enDivIndexYield
                            aUnd.Yield = aUndData.Yield
                            bChange = True
                    ElseIf (aUndData.DivType = enDivMarket) Then
                            aUnd.Dividend.DivType = enDivMarket
                            aUnd.Dividend.DivAmt = aUndData.DivAmt
                            aUnd.Dividend.DivDate = aUndData.DivDate
                            aUnd.Dividend.DivFreq = aUndData.DivFreq
                            bChange = True
                    ElseIf (aUndData.DivType = enDivCustomPeriodical) Then
                            aUnd.Dividend.DivType = enDivCustomPeriodical
                            aUnd.Dividend.DivAmtCust = aUndData.DivAmtCust
                            aUnd.Dividend.DivDateCust = aUndData.DivDateCust
                            aUnd.Dividend.DivFreqCust = aUndData.DivFreqCust
                            bChange = True
                    ElseIf (aUndData.DivType = enDivStockBasket) Then
                        aUnd.Dividend.DivType = enDivStockBasket
                        bChange = True
                    ElseIf (aUndData.DivType = enDivCustomStream) Then
                        aUnd.Dividend.DivType = enDivCustomStream
                        bChange = True
                    End If
                End If
               
                'recalculate and refresh all data
                If bChange Then
                    m_AuxClc.UnderlyingsCalc True, True, False, False
                    m_AuxOut.UnderlyingsUpdate False
                    RefreshPositions
                    m_AuxOut.TotalsUpdate
                End If
                
            End If
        End If
    Exit Sub
Exception:
    Debug.Print "Error while try to update dividend Type"
End Sub

Public Sub ManualPriceUpdate(ByVal UndID As Long, ByVal ID As Long, ByVal price As Double, ByVal CtType As EtsContractTypeEnum, ByVal Status As ManualPriceUpdateEnum)
On Error Resume Next

Dim aData As MSGSTRUCTLib.ManualPriceUpdate
    Set aData = New MSGSTRUCTLib.ManualPriceUpdate
    
    If (Not aData Is Nothing) Then
    
        If (Status = MPU_ADD Or Status = MPU_UPDATE) Then
            aData.ActivePrice = price
            aData.Status = enUsAdd
        Else
            aData.ActivePrice = 0#
            aData.Status = enUsDelete
        End If
        
        aData.ContractID = ID
        aData.UndID = UndID
        aData.ContractType = CtType
        
        ManualPriceUpdateEx aData
    End If
End Sub

Public Sub ManualPriceUpdateEx(Data As MSGSTRUCTLib.ManualPriceUpdate)
On Error GoTo Exception
        If (m_bShutDown) Then Exit Sub
        
        Dim bChange As Boolean
        Dim aUnd As EtsMmRisksLib.MmRvUndAtom
        Dim aFut As EtsMmRisksLib.MmRvFutAtom
        Dim aPos As EtsMmRisksLib.MmRvPosAtom
        
        bChange = False
        
        If (Data.ContractType = enCtIndex Or Data.ContractType = enCtStock) Then
                Set aUnd = m_Aux.Und(Data.UndID)
                If (Not aUnd Is Nothing) Then
                
                    If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                        aUnd.price.IsUseManualActive = True
                        aUnd.price.Active = Data.ActivePrice
                        bChange = True
                    ElseIf (Data.Status = enUsDelete) Then
                        aUnd.price.IsUseManualActive = False
                        bChange = True
                    End If
                    
                End If
                
                'Head Component
                For Each aUnd In m_Aux.Und
                    If (Not aUnd.HeadComponent Is Nothing) Then
                        If (aUnd.HeadComponent.ID = Data.UndID) Then
                            If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                                aUnd.HeadComponent.price.IsUseManualActive = True
                                aUnd.HeadComponent.price.Active = Data.ActivePrice
                                bChange = True
                            Else
                                aUnd.HeadComponent.price.IsUseManualActive = False
                                bChange = True
                            End If
                            Exit For
                        End If
                    End If
                Next
                
                'Idx
                If (Not m_Aux.Idx Is Nothing) Then
                    If (m_Aux.Idx.ID = Data.UndID) Then
                        If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                            m_Aux.Idx.price.IsUseManualActive = True
                            m_Aux.Idx.price.Active = Data.ActivePrice
                            bChange = True
                        Else
                            m_Aux.Idx.price.IsUseManualActive = False
                            bChange = True
                        End If
                    End If
                End If
            
            Set aUnd = Nothing
        ElseIf (Data.ContractType = enCtFuture) Then
            Set aUnd = m_Aux.Und(Data.UndID)
            
                If (Not aUnd Is Nothing) Then
                
                    If (Not aUnd.Fut Is Nothing) Then
                        Set aFut = aUnd.Fut(Data.ContractID)
                        If (Not aFut Is Nothing) Then
                            If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                                aFut.price.IsUseManualActive = True
                                aFut.price.Active = Data.ActivePrice
                                bChange = True
                            Else
                                aFut.price.IsUseManualActive = False
                                bChange = True
                            End If
                            Set aFut = Nothing
                        End If
                    End If
                    
                    Set aFut = aUnd.ActiveFuture
                    If (Not aFut Is Nothing) Then
                        If (aFut.ID = Data.ContractID) Then
                            If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                                aFut.price.IsUseManualActive = True
                                aFut.price.Active = Data.ActivePrice
                                bChange = True
                            Else
                                aFut.price.IsUseManualActive = False
                                bChange = True
                            End If
                        End If
                        Set aFut = Nothing
                    End If
                    
                End If
            
                'Idx
                If (Not m_Aux.Idx Is Nothing) Then
                    Set aFut = m_Aux.Idx.ActiveFuture
                    If (Not aFut Is Nothing) Then
                        If (aFut.ID = Data.ContractID) Then
                            If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                                aFut.price.IsUseManualActive = True
                                aFut.price.Active = Data.ActivePrice
                                bChange = True
                            Else
                                aFut.price.IsUseManualActive = False
                                bChange = True
                            End If
                        End If
                        Set aFut = Nothing
                    End If
                End If
                
                'head component
                For Each aUnd In m_Aux.Und
                    If (Not aUnd.HeadComponent Is Nothing) Then
                        Set aFut = aUnd.HeadComponent.ActiveFuture
                        If (Not aFut Is Nothing) Then
                            If (aFut.ID = Data.ContractID) Then
                                If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                                    aFut.price.IsUseManualActive = True
                                    aFut.price.Active = Data.ActivePrice
                                    bChange = True
                                Else
                                    aFut.price.IsUseManualActive = False
                                    bChange = True
                                End If
                                Exit For
                            End If
                            Set aFut = Nothing
                        End If
                    End If
                Next
                            
        ElseIf (Data.ContractType = enCtOption Or Data.ContractType = enCtFutOption) Then
        
            Set aUnd = m_Aux.Und(Data.UndID)
            If (Not aUnd Is Nothing) Then
                Set aPos = aUnd.Pos(Data.ContractID)
                If (Not aPos Is Nothing) Then
                    If (Data.Status = enUsUpdate Or Data.Status = enUsAdd) Then
                        aPos.Quote.price.IsUseManualActive = True
                        aPos.Quote.price.Active = Data.ActivePrice
                        bChange = True
                    Else
                        aPos.Quote.price.IsUseManualActive = False
                        bChange = True
                    End If
                End If
            End If
            
        End If
        
        'Now we can recalculate and refresh screen
        If bChange Then
            m_AuxClc.UnderlyingsCalc True, True, False, False
            m_AuxOut.UnderlyingsUpdate False
            RefreshPositions
            m_AuxOut.TotalsUpdate
        End If
        
    Exit Sub
Exception:
    Debug.Print "Error while trying to update manual price"
End Sub

Private Sub TradeChannel_ManualPriceUpdate(Data As MSGSTRUCTLib.ManualPriceUpdate)
On Error Resume Next
    ManualPriceUpdateEx Data
End Sub

Private Sub TradeChannel_PriceUpdate(aPrcData As MSGSTRUCTLib.PriceUpdate)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Not m_bIsInitialized Then Exit Sub
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, aTrd As EtsGeneralLib.MmTradeInfoAtom
    
    Set aUnd = m_Aux.Und(aPrcData.UndID)
    If Not aUnd Is Nothing Then
        If aPrcData.ContractType = enCtIndex Or aPrcData.ContractType = enCtStock Then _
            aUnd.price.Close = aPrcData.PriceClose
            aUnd.price.TheoClose = aPrcData.TheoPriceClose
        
        If m_Aux.RealTime Then Exit Sub
        
        Set aPos = aUnd.Pos(aPrcData.ContractID)
        If Not aPos Is Nothing Then
            aPos.Quote.price.Close = aPrcData.PriceClose
            aPos.Quote.price.TheoClose = aPrcData.TheoPriceClose
            m_AuxClc.ClearPosQty aPos
            
            For Each aTrd In TradeChannel.Trades.FilterTrades(m_Aux.FilterData, g_UnderlyingGroup, False) 'FilterTrades(m_Aux.Grp.ID, m_Aux.Grp.GroupType, m_Aux.Filter(RFC_TYPE), g_UnderlyingGroup, False)
                If aTrd.ContractID = aPos.ID Then
                    m_AuxClc.ChangeQtyDailyLTD aUnd, aPos, aTrd
                End If
                Set aTrd = Nothing
            Next
        End If
        
        m_AuxClc.UnderlyingsCalc False, False
        m_AuxOut.UnderlyingsUpdate False
        RefreshPositions
   End If
End Sub

Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Not m_bIsInitialized Then Exit Sub
    
    If tmrUndCalc.Enabled = True Then
        m_bIsNewTrades = True
    Else
        tmrUndCalc.Enabled = True
    End If
    
    If m_bDataLoad Then
        
        Debug.Assert Not ((TradeQueue.Add(aNewTrdInfo, aOldTrdInfo, enAction)) Is Nothing)
        Exit Sub
    End If
    
    Select Case enAction
        Case enTaTradeNew
            Debug.Assert Not aNewTrdInfo Is Nothing
            AddTrade aNewTrdInfo
            
        Case enTaTradeUpdate
            Debug.Assert Not aNewTrdInfo Is Nothing
            Debug.Assert Not aOldTrdInfo Is Nothing
            DeleteTrade aOldTrdInfo
            AddTrade aNewTrdInfo

        Case enTaTradeDelete
            Debug.Assert Not aOldTrdInfo Is Nothing
            DeleteTrade aOldTrdInfo
    End Select

End Sub

Private Sub TradeChannel_PositionTransfer(aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, aTrdTo As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    
    If m_bShutDown Then Exit Sub
    If Not m_bIsInitialized Then Exit Sub
    
    Debug.Assert Not aTrdFrom Is Nothing
    Debug.Assert Not aTrdTo Is Nothing
    
    AddTrade aTrdFrom
    AddTrade aTrdTo
End Sub

Private Sub UserControl_Hide()
    On Error Resume Next
    m_Aux.RealTime = False
    m_bDataLoad = False
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If m_bShutDown Or gCmn Is Nothing Then Exit Sub
    
    Dim nGridColWidth&, i&
        
    With fgFlt
        .Top = 0
        .Left = 0
        .Width = ScaleWidth
        
        nGridColWidth = 0&
        For i = 0 To .Cols - 1
            If Not .ColHidden(i) Then nGridColWidth = nGridColWidth + .ColWidth(i) + .GridLineWidth
        Next
            
        If ScaleWidth < nGridColWidth Then
            .ScrollBars = flexScrollBarHorizontal
        Else
            .ScrollBars = flexScrollBarNone
        End If
        
        .Height = .RowHeight(0) + (.rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        If .ScrollBars = flexScrollBarHorizontal Then
            .Height = .Height + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
        End If
    End With

    With fgTot
        .Top = fgFlt.Height + GRID_VERT_SPACE
        .Left = 0
        
        nGridColWidth = 0&
        For i = 0 To .Cols - 1
            If Not .ColHidden(i) Then nGridColWidth = nGridColWidth + .ColWidth(i) + .GridLineWidth
        Next
            
        If ScaleWidth < nGridColWidth Then
            .ScrollBars = flexScrollBarHorizontal
        Else
            .ScrollBars = flexScrollBarNone
        End If
        
        .Height = .RowHeight(0) + (.rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        If .ScrollBars = flexScrollBarHorizontal Then
            .Height = .Height + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
        End If
        .Width = ScaleWidth
    End With

    With fgPos
        .Left = 0
        .Top = fgFlt.Height + fgTot.Height + GRID_VERT_SPACE * 2
        .Width = ScaleWidth
        .Height = ScaleHeight - .Top
    End With
End Sub

Private Sub UserControl_Terminate()
    On Error Resume Next
    Term
End Sub

Public Sub Term()
    On Error Resume Next
    
    If gCmn Is Nothing Then Exit Sub
        
    m_bShutDown = True
    
    m_bDataLoad = False
    m_bInProc = False
    
    Set frmWtdVega = Nothing
    Set frmLayout = Nothing
    Set TradeChannel = Nothing
    Set VolaSource = Nothing
    Set TradeQueue = Nothing
    
    Set gePos = Nothing
    
    lblStatus.Visible = True
    pbProgress.Visible = False
    lblProcess.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
        
    If m_Aux.RealTime Then
        If g_RvRTQuantity > 0 Then
            g_RvRTQuantity = g_RvRTQuantity - 1
        End If
    End If
    
    m_Aux.RealTime = False
    
    ClearViewAndData
    
    Set fgFlt.Font = Nothing
    Set fgPos.Font = Nothing
    Set fgTot.Font = Nothing
    
    m_Aux.Term
    m_AuxOut.Term
    m_AuxClc.Term
    
    Set m_gdFlt = Nothing
    Set m_gdTot = Nothing
    Set m_gdPos = Nothing
    
    Set m_RiskView = Nothing
        
    Set m_AuxOut = Nothing
    Set m_Aux = Nothing
    Set m_AuxClc = Nothing
    
    Set VolaSource = Nothing
    
    Erase m_AggCols
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set aParams = Nothing
End Sub

Private Sub AdjustState()
    On Error Resume Next
    UpdateMenu
    RaiseEvent OnStateChange
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    
    mnuCtxRealTime.Enabled = m_Aux.Grp.ID <> 0 And Not m_bInProc
    mnuCtxRealTime.Checked = m_Aux.RealTime
    
    mnuCtxRefresh.Enabled = mnuCtxRealTime.Enabled And Not m_Aux.RealTime
    mnuCtxRefreshPrices.Enabled = mnuCtxRefresh.Enabled
    mnuCtxWtdVega.Enabled = Not m_bInProc And Not frmWtdVega.IsOpened
    
    fgFlt.ColHidden(RFC_SIM_DATE) = m_Aux.RealTime
End Sub

Public Property Get TotalShownUndPositions() As Integer
    On Error Resume Next
    TotalShownUndPositions = m_nUndPositions
End Property

Public Property Get TotalShownOptPositions() As Integer
    On Error Resume Next
    TotalShownOptPositions = m_nOptPositions
End Property

Public Property Get WtdVegaIsOpened() As Boolean
    On Error Resume Next
    WtdVegaIsOpened = frmWtdVega.IsOpened
End Property

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Public Function GetCaption() As String
    On Error Resume Next
        
    Dim sCaption$
    
    If (m_Aux Is Nothing) Then
        GetCaption = ""
        Exit Function
    End If
    
    sCaption = ""
    If m_Aux.Grp.ID <> 0 Then sCaption = sCaption & m_Aux.Grp.Name & " - "
    If m_Aux.RealTime Then _
        sCaption = sCaption & IIf(m_Aux.RealTimeConnected, "[Real Time] - ", "[Real Time (Disconnected)] - ")
    
    If (SimulationUsed And (Not m_Aux.RealTime)) Then
        sCaption = sCaption & "[Simulation] - "
    End If
    
    sCaption = sCaption & "Risks"
    
    GetCaption = sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    If m_Aux.Grp.ID <> 0 Then
        GetShortCaption = m_Aux.Grp.Name
    Else
        GetShortCaption = "<Empty>"
    End If
End Function

Public Property Get InProc() As Boolean
    InProc = m_bInProc
End Property

Public Property Get RealTime() As Boolean
    RealTime = m_Aux.RealTime
End Property

Public Property Get RealTimeConnected() As Boolean
    RealTimeConnected = m_Aux.RealTime And m_Aux.RealTimeConnected
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
    frmLayout.Execute GT_RISKS_FILTER, RFC_SYMBOL, RFC_LAST_COLUMN, m_gdFlt, m_frmOwner
End Sub

Public Sub CustomizeTotGridLayout()
    On Error Resume Next
    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
    frmLayout.Execute GT_RISKS_TOTALS, RTC_PNL_MTM, g_RTC_LAST_COLUMN, m_gdTot, m_frmOwner
End Sub

Public Sub CustomizePosGridLayout()
    On Error Resume Next
    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
    frmLayout.Execute GT_RISKS_POSITIONS, RPC_SYMBOL, g_RPC_LAST_COLUMN, m_gdPos, m_frmOwner
End Sub

Private Function SavePriceClose(ByVal ContractType As Long, ByVal ContractID As Long, ByVal ClosePrice As Double, ByVal TheoPrice As Double) As Boolean
    On Error GoTo EH
    
    SavePriceClose = False
    
    gDBW.usp_ContractPrice_Save _
        Null, _
        ContractID, _
        Null, _
        Null, _
        Null, _
        Null, _
        Null, _
        Null, _
        ClosePrice, _
        Null, _
        Null, _
        Null, _
        Null, _
        Null, _
        ContractType, _
        Null, _
        TheoPrice
        
        SavePriceClose = True
        
        Exit Function
EH:
    gCmn.ErrorHandler "Error save close prices for '" & CStr(ContractID) & "': "
End Function

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey

    ' common info
    For i = RFC_SYMBOL To RFC_LAST_COLUMN
        aStorage.SetLongValue "RiskFlt" & sKey, "Filter" & CStr(i), m_Aux.Filter(i)
    Next
    
    aStorage.SetLongValue "Coordinates" & sKey, "Left", m_frmOwner.Left
    aStorage.SetLongValue "Coordinates" & sKey, "Top", m_frmOwner.Top
    aStorage.SetLongValue "Coordinates" & sKey, "Width", m_frmOwner.Width
    aStorage.SetLongValue "Coordinates" & sKey, "Height", m_frmOwner.Height
    
    m_gdFlt.WriteToStorage "RiskFltGrid" & sKey, aStorage, True
    m_gdTot.WriteToStorage "RiskTotGrid" & sKey, aStorage
    m_gdPos.WriteToStorage "RiskPosGrid" & sKey, aStorage
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String, _
                        Optional ByVal bRefreshData As Boolean = True, _
                        Optional ByVal bAllowResize As Boolean = True)
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    ' common info
    For i = RFC_SYMBOL To RFC_LAST_COLUMN
        m_Aux.Filter(i) = aStorage.GetLongValue("RiskFlt" & sKey, "Filter" & CStr(i), m_Aux.Filter(i))
    Next
        
    If m_Aux.Filter(RFC_TRADES) < 0 Or m_Aux.Filter(RFC_TRADES) > 6 Then m_Aux.Filter(RFC_TRADES) = 0
    m_nOpenedExpiry = m_Aux.Filter(RFC_EXPIRY)
    
    If (bAllowResize = True) Then
        m_frmOwner.Left = aStorage.GetLongValue("Coordinates" & sKey, "Left", m_frmOwner.Left)
        m_frmOwner.Top = aStorage.GetLongValue("Coordinates" & sKey, "Top", m_frmOwner.Top)
        m_frmOwner.Width = aStorage.GetLongValue("Coordinates" & sKey, "Width", m_frmOwner.Width)
        m_frmOwner.Height = aStorage.GetLongValue("Coordinates" & sKey, "Height", m_frmOwner.Height)
    End If
    
    m_gdFlt.ReadFromStorage "RiskFltGrid" & sKey, aStorage, True
    m_gdTot.ReadFromStorage "RiskTotGrid" & sKey, aStorage
    m_gdPos.ReadFromStorage "RiskPosGrid" & sKey, aStorage
    
    m_Aux.FilterUpdateAll
    
    tmrShow.Enabled = bRefreshData 'True
    Exit Sub
EH:
    gCmn.ErrorHandler ""
    RaiseEvent OnRefreshError
End Sub

Public Sub RefreshView()
    On Error Resume Next
    If m_bShutDown Or Not m_Aux.RealTime Then Exit Sub
    If m_bVolaUpdated Then
       If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshView: Initiate Realtime Refresh", m_frmOwner.GetCaption
            
        tmrRealTime.Enabled = True
    Else
       If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshView: No Realtime Refresh needed", m_frmOwner.GetCaption
    End If
End Sub

Private Sub StoreRecentFilters(Optional ByVal bStoreHidden As Boolean = True)
    On Error Resume Next
    Dim nRow&, aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aRowData As EtsMmRisksLib.MmRvRowData
    
    m_nOpenedExpiry = 0

    m_nOpenedExpiry = m_Aux.Filter(RFC_EXPIRY)
    
End Sub

Public Sub Refresh()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    StoreRecentFilters False
    tmrShow.Enabled = True
End Sub

Public Sub RefreshPrices()
    On Error Resume Next
    If m_bShutDown Or m_Aux.RealTime Then Exit Sub
End Sub

Public Function Group() As EtsMmRisksLib.MmRvGrpAtom
    On Error Resume Next
    Set Group = m_Aux.Grp
End Function

Public Property Get UnderlyingsCount() As Long
    On Error Resume Next
    UnderlyingsCount = m_Aux.Und.Count
End Property

Private Sub VolaSource_VolatilityChanged(ByVal Symbol As String)
    On Error Resume Next
'    If Not g_Params.UseTheoVolatility Or m_bShutDown Then Exit Sub
'    Dim aGUnd As EtsGeneralLib.UndAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, nRow&, sKey$
'
'    With fgPos
'        Set aGUnd = g_Underlying.BySortKey(Symbol)
'        If Not aGUnd Is Nothing Then
'            Set aUnd = m_Aux.Und(aGUnd.ID)
'            Set aGUnd = Nothing
'
'            If Not aUnd Is Nothing Then
'                If m_Aux.RealTime Then
'                    aUnd.VolaUpdated = True
'                    aUnd.CalcGreeks = True
'                    aUnd.CalcTotals = True
'                    m_bVolaUpdated = True
'                    tmrRealTime.Enabled = True
'                Else
'                    CalculateRiskViewUnderlying aUnd
'                    RefreshPositions
'                    m_AuxOut.TotalsUpdate
'                End If
'                Set aUnd = Nothing
'            End If
'        End If
'    End With
End Sub

Public Sub CalculateRiskViewUnderlying(ByRef Und As EtsMmRisksLib.MmRvUndAtom)
On Error GoTo Exception
    Dim sSymbol As String
    
    If (Not Und Is Nothing) Then
        sSymbol = Und.Symbol
        'Check for new trades
        CheckTradeQueue
        'Set calculation date to actual
        m_AuxClc.CalcDate = GetNewYorkTime
        'Update and recalc greeks by current underlying
        m_AuxClc.UnderlyingCalc Und, True, True
        'Update totals
        m_AuxClc.UnderlyingsCalc False, False, False, True
    End If
    
Exit Sub
Exception:
    Debug.Print "Error in CalculateRiskView: " & sSymbol
End Sub

Public Sub InternalVolatilityChanged(ByVal sSymbol As String)
    On Error Resume Next
    If Not m_bShutDown And Not m_bVolaChangedNow Then VolaSource_VolatilityChanged sSymbol
End Sub

Public Sub ShowWeightedVegaSettings()
    On Error Resume Next
    If Not m_bShutDown Then frmWtdVega.Execute m_frmOwner
End Sub

Public Function CurUnderlyingID() As Long
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aRowData As EtsMmRisksLib.MmRvRowData

    CurUnderlyingID = 0
    
    If m_Aux.Filter(RFC_SYMBOL) > 0 Then
        CurUnderlyingID = m_Aux.Filter(RFC_SYMBOL)
    Else
        If fgPos.Row > 0 Then
            Set aRowData = m_RiskView.PosRowData(fgPos.Row)
            Set aUnd = aRowData.Und
            If Not aUnd Is Nothing Then
                CurUnderlyingID = aUnd.ID
                Set aUnd = Nothing
            End If
        End If
    End If
    
End Function

Public Sub ShowNextUnd()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nRow&, i&

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        If m_Aux.Und.Count > 1 Then
            Do
                m_nCurUnd = .GetNodeRow(m_nCurUnd, flexNTNextSibling)
                If m_nCurUnd < 0 Then m_nCurUnd = .GetNodeRow(1, flexNTFirstSibling)
            Loop While .RowHidden(m_nCurUnd)
            
            m_bCurUndChanging = True
            nRow = .GetNodeRow(1, flexNTFirstSibling)
            While nRow > 0
                .IsCollapsed(nRow) = IIf(m_nCurUnd = nRow, flexOutlineExpanded, flexOutlineCollapsed)
                nRow = .GetNodeRow(nRow, flexNTNextSibling)
            Wend
            m_bCurUndChanging = False
        End If
    
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub ShowPrevUnd()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nRow&, i&

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        If m_Aux.Und.Count > 1 Then
            Do
                m_nCurUnd = .GetNodeRow(m_nCurUnd, flexNTPreviousSibling)
                If m_nCurUnd < 0 Then m_nCurUnd = .GetNodeRow(1, flexNTLastSibling)
            Loop While .RowHidden(m_nCurUnd)
            
            m_bCurUndChanging = True
            nRow = .GetNodeRow(1, flexNTFirstSibling)
            While nRow > 0
                .IsCollapsed(nRow) = IIf(m_nCurUnd = nRow, flexOutlineExpanded, flexOutlineCollapsed)
                nRow = .GetNodeRow(nRow, flexNTNextSibling)
            Wend
            m_bCurUndChanging = False
        End If
    
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub ShowAllUnd(ByVal bShow As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nRow&
    
    If bShow Then
        m_RiskView.SetAllRowsExpanded
        fgPos.Outline 3
    Else
        m_RiskView.SetAllRowsCollapsed
        fgPos.Outline 0
    End If
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim aUnd As EtsGeneralLib.UndAtom
    If m_Aux.Filter(RFC_SYMBOL) <> 0 Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            m_Aux.FilterUpdateValue False, RFC_SYMBOL
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
     If Not m_bShutDown Then m_Aux.FilterUpdateValue False, RFC_GROUPS
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    'If Not m_bShutDown And m_Aux.Filter(RFC_GROUP) = TYPE_STRATEGY Then m_Aux.FilterUpdateValue False
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    If m_bShutDown Then Exit Sub
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Name & " Risks", "Risks", fgFlt, fgTot, fgPos
'    Screen.MousePointer = vbDefault
End Sub

Private Sub HandleGridDblClick(ByVal bTradeNewAvailable As Boolean)
    On Error Resume Next
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
        And m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols Then
        
        If IsAltPressed Then
            Select Case True
                Case IsCtrlPressed
                    mnuCtxOrderNewOption_Click
                    
                Case IsShiftPressed
                    'mnuCtxTntCardNew_Click
                
                Case Else
                    mnuCtxOrderNewStock_Click
            End Select
            
            FixDblClickHandled
        
        ElseIf bTradeNewAvailable And Not IsDblClickHandled And Not (IsCtrlPressed Or IsShiftPressed) Then
            mnuCtxTradeNew_Click
            FixDblClickHandled
        End If
    End If
End Sub

Public Sub MakeDataSnapshot(ByVal sFileName$)
    On Error Resume Next
End Sub

Public Sub CallOtcOptionCalcRV()
    On Error Resume Next
    If m_bShutDown Or m_bInProc Then Exit Sub
    Dim nUndID&, nID&, bBuy As Boolean, aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, nColIdx&
    Dim aRowData As EtsMmRisksLib.MmRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    
    Dim sStockSymbol As String
    Dim sOptionSymbol As String
    
    Dim lSymbolType As Long
    
    Dim dStrike As Double
    Dim dBid As Double
    Dim dAsk As Double
    Dim dtExpiryOV As Date
    Dim lCalcModel As Long
    
    Dim lContractType As Long
    Dim dYield As Double
    Dim lDivType As Long
    
    Dim dDivAmount As Double
    
    Dim dRate As Double
    Dim dHTBRate As Double: dHTBRate = BAD_DOUBLE_VALUE
    
    Dim dVola As Double
    
    Dim dDivDate As Double
    Dim dDivFreq As Double
    Dim lCDStockID
    Dim lCDCount
    
    lCDStockID = 0
    lCDCount = 0
    dVola = 0
    
    
    m_bInProc = True
    nUndID = 0
    nID = 0
    bBuy = True
    
    If m_enMenuGrid = GT_RISKS_POSITIONS Then
        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            Set aRowData = m_RiskView.PosRowData(m_nMenuGridRow)
            Set aUnd = aRowData.Und
            Set aPos = aRowData.Pos
            Set aSynthGreek = aRowData.SynthGreeks
        End If
        
        
        If Not aUnd Is Nothing Then
            nColIdx = fgPos.ColKey(m_nMenuGridCol)
            If nColIdx = RPC_BID Then
                bBuy = (g_Params.RiskBidColBuy <> 0)
            ElseIf nColIdx = RPC_ASK Then
                bBuy = (g_Params.RiskBidColBuy = 0)
            Else
                bBuy = (g_Params.RiskOtherColBuy <> 0)
            End If
            
            sStockSymbol = aUnd.Symbol
            sOptionSymbol = aUnd.Symbol
            
            dStrike = aUnd.price.Last
            dBid = aUnd.price.Bid
            dAsk = aUnd.price.Ask


            lSymbolType = 0
            If Not aUnd.IsAmerican Then
                lSymbolType = 1
            End If
            
            
            dtExpiryOV = Now
              
              lDivType = 0 'aUnd.DivType
              dYield = aUnd.Yield
              lContractType = aUnd.ContractType
               
               If aUnd.Dividend.DivType = enDivMarket Then
                    dDivAmount = aUnd.Dividend.DivAmt
                    dDivDate = aUnd.Dividend.DivDate
                    dDivFreq = aUnd.Dividend.DivFreq
                End If
               
               If aUnd.Dividend.DivType = enDivCustomPeriodical Then
                    dDivAmount = aUnd.Dividend.DivAmtCust
                    dDivDate = aUnd.Dividend.DivDateCust
                    dDivFreq = aUnd.Dividend.DivFreqCust
                End If
               
               If aUnd.Dividend.DivType = enDivCustomStream Then
                    If Not aUnd.Dividend.CustomDivs Is Nothing Then
                        lCDStockID = aUnd.ID
                        lCDCount = 0
                    End If
               End If
            
            
            If Not aPos Is Nothing Then
                nUndID = aUnd.ID
                nID = aPos.ID
                sOptionSymbol = aPos.Symbol
                dStrike = aPos.Strike
                dBid = aPos.Quote.price.Bid
                dAsk = aPos.Quote.price.Ask
                dtExpiryOV = aPos.ExpiryOV
                dRate = aPos.Rate
                dHTBRate = aPos.HTBRate
                dVola = aPos.Quote.Vola
                Set aPos = Nothing
            Else
                If aSynthGreek Is Nothing Then
                    nUndID = aUnd.ID
                    nID = nUndID
                    sOptionSymbol = aPos.Symbol
                    For Each aPos In aUnd.Pos
                        If (Not aPos Is Nothing) And (aPos.Expiry > 0) Then
                            sOptionSymbol = aPos.Symbol
                            dStrike = aPos.Strike
                            dBid = aPos.Quote.price.Bid
                            dAsk = aPos.Quote.price.Ask
                            dtExpiryOV = aPos.ExpiryOV
                            dRate = aPos.Rate
                            dHTBRate = aPos.HTBRate
                            dVola = aPos.Quote.Vola
                            Exit For
                        End If
                    Next
                Else
                    nUndID = aSynthGreek.SynthUndID
                    nID = nUndID
                    sOptionSymbol = aSynthGreek.Symbol
                End If
            End If
            
            Set aUnd = Nothing
        End If
        
    ElseIf m_enMenuGrid = GT_RISKS_FILTER Then
        If m_Aux.Filter(RFC_SYMBOL) <> 0 And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_Aux.Filter(RFC_SYMBOL)
            nID = nUndID
            bBuy = (g_Params.RiskOtherColBuy <> 0)
            
        End If
    End If

    lCalcModel = CLng(g_Params.CalcModel)
    
   If aUnd.ContractType = enCtFutUnd Then
        lContractType = enCtFuture
        If Not aRowData.Fut Is Nothing Then
            sStockSymbol = aRowData.Fut.Symbol
        ElseIf Not aRowData.Pos Is Nothing And Not aRowData.Pos.Fut Is Nothing Then
            sStockSymbol = aRowData.Pos.Fut.Symbol
        End If
    ElseIf aUnd.ContractType = enCtIndex Then
        If Not aRowData.Fut Is Nothing Then
            lContractType = enCtFuture
            sStockSymbol = aRowData.Fut.Symbol
        ElseIf Not aRowData.Pos Is Nothing And Not aRowData.Pos.Fut Is Nothing Then
            lContractType = enCtFuture
            sStockSymbol = aRowData.Pos.Fut.Symbol
        End If
    End If
    
    m_bInProc = False
    On Error GoTo EH
    Dim sPath As String
    Dim sParams As String
    sPath = App.Path & "\OTCCalc\OTCOptionCalc.exe"
    sParams = sStockSymbol & " " & sOptionSymbol & " " & CStr(lSymbolType) & " " & CStr(dStrike) & " " & CStr(dBid) & " " & CStr(dAsk) & " " & _
    """" & CStr(dtExpiryOV) & """" & " " & _
    CStr(lContractType) & " " & CStr(dYield) & " " & CStr(lDivType) & " " & _
    CStr(dDivAmount) & " " & CStr(dDivDate) & " " & CStr(dDivFreq) & " " & _
    CStr(lCDStockID) & " " & CStr(lCDCount) & " " & CStr(dRate) & " " & CStr(dVola) & " " & CStr(lCalcModel) & " " & CStr(dHTBRate)

    If ShellExecute(0&, "Open", sPath, sParams, "", SW_SHOWNORMAL) <= 32 Then
         gCmn.MyMsgBox Me, "Fail to open OTC OptionCalc at '" & sPath & "'.", vbCritical
    End If
    
    Exit Sub
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to open OTC Option Calc."
    ResetMenuData
End Sub

Public Function GetStockInfo() As String
    On Error Resume Next
    GetStockInfo = ""
'    Dim sStock As String
'    sStock = m_RiskView.PosRowData(m_nMenuGridRow).Und.Symbol
'    GetStockInfo = IIf(sStock, "Stock symbol = """ & sStock & """. ", "")
End Function

Public Function GetOptionInfo() As String
    On Error Resume Next
    GetOptionInfo = ""
'    Dim sOption As String
'    sOption = m_RiskView.PosRowData(m_nMenuGridRow).Pos.Symbol
'    GetOptionInfo = GetStockInfo & IIf(sOption, "Option symbol = """ & sOption & """. ", "")
End Function

Private Sub RecalculateGreeck()
    m_AuxClc.UnderlyingsCalc True, True
    m_AuxOut.UnderlyingsUpdate False
    m_AuxOut.TotalsUpdate
    RefreshPositions
End Sub

Private Sub RefreshPositions()
On Error GoTo ErrEx
    Dim i&, aRowData As EtsMmRisksLib.MmRvRowData
    Dim nCol&, nAggRow&
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim enP As EtsGeneralLib.EtsReplacePriceStatusEnum
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim dActive As Double
    
    Dim code_line As Long
    
    code_line = 0
    m_bTradeActionrefreshPositions = False
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions Enter", m_frmOwner.GetCaption
        
    If m_bIsPosRefresh Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions Cancel", m_frmOwner.GetCaption
        Exit Sub
    End If
    
    code_line = code_line + 1
    m_bIsPosRefresh = True
    
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
    fgPos.FlexDataSource = Nothing
    m_RiskView.PosColumnsOrder = m_Aux.gdPos.IdxCopy
    
    code_line = code_line + 1
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions m_RiskView.PosColumnsOrder complete", m_frmOwner.GetCaption
    
    fgPos.Subtotal flexSTNone
    
    code_line = code_line + 1
    
    fgPos.Clear
    
    code_line = code_line + 1
    
    m_Aux.FormatPosGrid
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions m_Aux.FormatPosGrid complete", m_frmOwner.GetCaption
    
    m_Aux.FormatPosColumns
    
    code_line = code_line + 1
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions m_Aux.FormatPosColumns complete", m_frmOwner.GetCaption
        
    code_line = code_line + 1
    
    m_Aux.RiskView.Refresh RPC_SYMBOL, m_Aux.Filter(RFC_EXPIRY), m_AggCols, dToleranceValue, enRoundingRule
    
    code_line = code_line + 1
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions m_Aux.RiskView.Refresh complete", m_frmOwner.GetCaption
    
    fgPos.FlexDataSource = m_Aux.RiskView
    
    code_line = code_line + 1
    
    m_nOptPositions = m_RiskView.OptionPositions
    m_nUndPositions = m_RiskView.UndPositions
    
    code_line = code_line + 1
    
    For i = 1 To fgPos.rows - 1
        Set aRowData = m_RiskView.PosRowData(i)
        fgPos.IsSubtotal(i) = aRowData.IsAggregation
        
        If aRowData.IsAggregation Or aRowData.Pos Is Nothing Then
            fgPos.Cell(flexcpFontBold, i, 1, i, fgPos.Cols - 1) = True
        Else
            fgPos.Cell(flexcpFontBold, i, 1, i, fgPos.Cols - 1) = False
        End If

        dActive = 0
        ' set outlining level and draw bad price marks
        If Not aRowData Is Nothing Then
            fgPos.RowOutlineLevel(i) = aRowData.OutlineLevel
            If Not aRowData.IsAggregation And Not aRowData.Pos Is Nothing Or Not aRowData.SynthGreeks Is Nothing Then
                nAggRow = fgPos.GetNodeRow(i, flexNTParent)

                If fgPos.IsCollapsed(nAggRow) = flexOutlineExpanded And Not fgPos.RowHidden(nAggRow) Then
                    fgPos.RowHidden(i) = False
                Else
                    fgPos.RowHidden(i) = True
                End If

                If Not aRowData.Pos Is Nothing Then
                    If Not aRowData.Pos.Quote Is Nothing Then
                        If aRowData.Pos.Quote.ReplacePriceStatus = enRpsAsk Or aRowData.Pos.Quote.ReplacePriceStatus = enRpsBoth Then
                            nCol = fgPos.ColIndex(RPC_ASK)
                            If nCol <> -1 Then fgPos.Cell(flexcpPicture, i, nCol) = imgBadPrice.Picture
                        End If
                        If aRowData.Pos.Quote.ReplacePriceStatus = enRpsBid Or aRowData.Pos.Quote.ReplacePriceStatus = enRpsBoth Then
                            nCol = fgPos.ColIndex(RPC_BID)
                            If nCol <> -1 Then fgPos.Cell(flexcpPicture, i, nCol) = imgBadPrice.Picture
                        End If

                        If aRowData.Pos.Quote.price.IsUseManualActive Then

                            If (aRowData.Pos.ContractType = enCtFuture) Then
                                dActive = g_Main.ContractAll(aRowData.Pos.ID).Fut.manualActivePrice
                            ElseIf (aRowData.Pos.ContractType = enCtIndex Or aRowData.Pos.ContractType = enCtStock) Then
                                If (Not g_Main.ContractAll(aRowData.Pos.ID).Und.PriceByHead) Then
                                    dActive = g_Main.ContractAll(aRowData.Pos.ID).Und.manualActivePrice
                                End If
                            Else
                                dActive = aRowData.Pos.Quote.price.Active
                            End If

                            nCol = fgPos.ColIndex(RPC_ACTIVEPRC)
                            If nCol <> -1 And dActive > 0 Then fgPos.Cell(flexcpPicture, i, nCol) = imgInSpread.Picture

                        End If

                    End If
                Else
                    If (Not aRowData.SynthGreeks Is Nothing) Then
                        If (Not g_Main.ContractAll(aRowData.Und.ID) Is Nothing) Then
                            If (Not g_Main.ContractAll(aRowData.Und.ID).Und.PriceByHead) Then
                                dActive = g_Main.ContractAll(aRowData.Und.ID).Und.manualActivePrice
                            End If
                        End If
                        nCol = fgPos.ColIndex(RPC_ACTIVEPRC)
                        If nCol <> -1 And dActive > 0 Then fgPos.Cell(flexcpPicture, i, nCol) = imgInSpread.Picture
                    End If
                End If

            Else ' Is Aggregation
                nAggRow = fgPos.GetNodeRow(i, flexNTParent)

                If Not aRowData.Fut Is Nothing Then
                        nCol = fgPos.ColIndex(RPC_ACTIVEPRC)
                        dActive = g_Main.ContractAll(aRowData.Fut.ID).Fut.manualActivePrice
                        If nCol <> -1 And aRowData.Fut.price.IsUseManualActive And dActive > 0 Then fgPos.Cell(flexcpPicture, i, nCol) = imgInSpread.Picture
                    ElseIf Not aRowData.Und Is Nothing Then
                        If (Not g_Main.ContractAll(aRowData.Und.ID).Und.PriceByHead) Then
                            dActive = g_Main.ContractAll(aRowData.Und.ID).Und.manualActivePrice
                        End If
                        nCol = fgPos.ColIndex(RPC_ACTIVEPRC)
                        If nCol <> -1 And (nAggRow = -1 Or nAggRow = 1) And aRowData.Und.price.IsUseManualActive And dActive > 0 Then fgPos.Cell(flexcpPicture, i, nCol) = imgInSpread.Picture
                End If

                If nAggRow <> -1 Then
                    fgPos.IsCollapsed(i) = IIf(m_RiskView.IsRowExpanded(i) = True, flexOutlineExpanded, flexOutlineCollapsed)
                    fgPos.RowHidden(i) = (fgPos.IsCollapsed(nAggRow) = flexOutlineCollapsed Or fgPos.RowHidden(nAggRow))
                Else
                    fgPos.IsCollapsed(i) = IIf(m_RiskView.IsRowExpanded(i), flexOutlineExpanded, flexOutlineCollapsed)
                    fgPos.RowHidden(i) = False
                End If

                enP = aRowData.GetAggregationPriceReplaceStatus
                If Not aRowData.OutlineLevel = USD_ID Then
                    If enP = enRpsAsk Or enP = enRpsBoth Then
                        nCol = fgPos.ColIndex(RPC_ASK)
                        If nCol <> -1 Then fgPos.Cell(flexcpPicture, i, nCol) = imgBadPrice.Picture

                    End If
                    If enP = enRpsBid Or enP = enRpsBoth Then
                        nCol = fgPos.ColIndex(RPC_BID)
                      If nCol <> -1 Then fgPos.Cell(flexcpPicture, i, nCol) = imgBadPrice.Picture

                    End If
                Else
                    If Not aRowData.Und Is Nothing Then
                        If enP = enRpsAsk Or enP = enRpsBoth Then
                            nCol = fgPos.ColIndex(RPC_ASK)
                            If nCol <> -1 And aRowData.Und.ContractType <> enCtFutUnd Then fgPos.Cell(flexcpPicture, i, nCol) = imgBadPrice.Picture
                        End If
                        If enP = enRpsBid Or enP = enRpsBoth Then
                            nCol = fgPos.ColIndex(RPC_BID)
                            If nCol <> -1 And aRowData.Und.ContractType <> enCtFutUnd Then fgPos.Cell(flexcpPicture, i, nCol) = imgBadPrice.Picture
                        End If
                    End If
                End If
            End If


            'Begin Coloring
            If (aRowData.OutlineLevel = 0 And aRowData.IsAggregation) Then
                Set aUnd = aRowData.Und

                If Not aUnd Is Nothing Then
                    Dim iClr&, nIdxClr&, nColClr&
                    iClr = 0
                    nIdxClr = m_Aux.gdPos.Idx(0)
                    While nIdxClr >= 0 And iClr <= RPC_LAST_COLUMN
                        nColClr = iClr + 1

                        With fgPos

                        Select Case aUnd.ContractType
                            Case enCtIndex, enCtStock, enCtOption, enCtFuture, enCtFutOption, enCtFutRoot  ' all except enCtFutUnd
                            Select Case nIdxClr

                                Case RPC_PNL_MTM
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadPnlMtm, m_Aux.gdPos.Col(RPC_PNL_MTM).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_PNL_MTM).ForeColor)
                                Case RPC_PNL_THEO
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadPnlTheo, m_Aux.gdPos.Col(RPC_PNL_THEO).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_PNL_THEO).ForeColor)
                                Case RPC_PNL_EDGE
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadPnlMtm Or aUnd.BadPnlTheo, m_Aux.gdPos.Col(RPC_PNL_EDGE).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_PNL_EDGE).ForeColor)
                                Case RPC_NET_DELTA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadNetDelta, m_Aux.gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_NET_DELTA).ForeColor)
                                Case RPC_NET_DELTA_USD
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadDeltaEq, m_Aux.gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_NET_DELTA_USD).ForeColor)

                                Case RPC_OPT_DELTA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadOptDelta, m_Aux.gdPos.Col(RPC_OPT_DELTA).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_OPT_DELTA).ForeColor)
                                Case RPC_GAMMA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadGammaPerc, m_Aux.gdPos.Col(RPC_GAMMA).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_GAMMA).ForeColor)
                                Case RPC_NET_GAMMA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadNetGamma, m_Aux.gdPos.Col(RPC_NET_GAMMA).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_NET_GAMMA).ForeColor)
                                Case RPC_GAMMA_SHARES
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadGamma, m_Aux.gdPos.Col(RPC_GAMMA_SHARES).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_GAMMA_SHARES).ForeColor)
                                Case RPC_VEGA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadVega, m_Aux.gdPos.Col(RPC_VEGA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_VEGA).ForeColor)
                                Case RPC_WTD_VEGA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadWtdVega, m_Aux.gdPos.Col(RPC_WTD_VEGA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_WTD_VEGA).ForeColor)
                                Case RPC_THETA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadTheta, m_Aux.gdPos.Col(RPC_THETA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_THETA).ForeColor)
                                Case RPC_RHO
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadRho, m_Aux.gdPos.Col(RPC_RHO).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_RHO).ForeColor)
                                Case RPC_THETA_DELTA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadThetaDelta, m_Aux.gdPos.Col(RPC_THETA_DELTA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_THETA_DELTA).ForeColor)
                                Case RPC_THETA_GAMMA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadThetaGamma, m_Aux.gdPos.Col(RPC_THETA_GAMMA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_THETA_GAMMA).ForeColor)
                                Case RPC_VEGA_DELTA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadVegaDelta, m_Aux.gdPos.Col(RPC_VEGA_DELTA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_VEGA_DELTA).ForeColor)
                                Case RPC_VEGA_GAMMA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadVegaGamma, m_Aux.gdPos.Col(RPC_VEGA_GAMMA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_VEGA_GAMMA).ForeColor)
                                Case RPC_TIME_VALUE
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadTimeValue, m_Aux.gdPos.Col(RPC_TIME_VALUE).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_TIME_VALUE).ForeColor)
                                Case RPC_BETA_WTD_DELTA_USD
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadBetaWtdDeltaEq, m_Aux.gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColor)
                            End Select

                            Case enCtFutUnd
                            Select Case nIdxClr
                                Case RPC_PNL_MTM
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadPnlMtm, m_Aux.gdPos.Col(RPC_PNL_MTM).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_PNL_MTM).ForeColor)
                                Case RPC_PNL_THEO
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadPnlTheo, m_Aux.gdPos.Col(RPC_PNL_THEO).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_PNL_THEO).ForeColor)
                                Case RPC_PNL_EDGE
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadPnlMtm Or aUnd.BadPnlTheo, m_Aux.gdPos.Col(RPC_PNL_EDGE).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_PNL_EDGE).ForeColor)
                                Case RPC_NET_DELTA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadNetDelta, m_Aux.gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_NET_DELTA).ForeColor)
                                Case RPC_NET_DELTA_USD
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadDeltaEq, m_Aux.gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_NET_DELTA_USD).ForeColor)
                                Case RPC_NET_GAMMA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadNetGamma, m_Aux.gdPos.Col(RPC_NET_GAMMA).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_NET_GAMMA).ForeColor)
                                Case RPC_VEGA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadVega, m_Aux.gdPos.Col(RPC_VEGA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_VEGA).ForeColor)
                                Case RPC_WTD_VEGA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadWtdVega, m_Aux.gdPos.Col(RPC_WTD_VEGA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_WTD_VEGA).ForeColor)
                                Case RPC_THETA
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadTheta, m_Aux.gdPos.Col(RPC_THETA).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_THETA).ForeColor)
                                Case RPC_RHO
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadRho, m_Aux.gdPos.Col(RPC_RHO).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_RHO).ForeColor)
                                Case RPC_TIME_VALUE
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadTimeValue, m_Aux.gdPos.Col(RPC_TIME_VALUE).ForeColorAlt1, _
                                                                                    m_Aux.gdPos.Col(RPC_TIME_VALUE).ForeColor)
                                Case RPC_BETA_WTD_DELTA_USD
                                    .Cell(flexcpForeColor, i, nColClr) = IIf(aUnd.BadBetaWtdDeltaEq, m_Aux.gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                                                                        m_Aux.gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColor)
                            End Select

                        End Select

                        End With

                        iClr = iClr + 1
                        nIdxClr = m_Aux.gdPos.Idx(iClr)
                    Wend
                End If
            End If
            'End Coloring

        End If
        If m_bShutDown Then Exit Sub
    Next
    
    m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    m_bIsPosRefresh = False
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions grid formating complete", m_frmOwner.GetCaption
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshPositions grid values update complete", m_frmOwner.GetCaption
        
    SetLastRowSelected
    Exit Sub
ErrEx:
    MsgBox "RefreshPositions failed with exception " & Err.Number & " : " & Err.Description & "Line: " & code_line
End Sub


Public Sub ActiveFuturesChange(ByVal iUndID As Long, ByVal iActiveFutID As Long)
    On Error Resume Next
    Dim aFutReq As EtsMmRisksLib.MmRvReqAtom

    Dim aUnd As MmRvUndAtom
    Dim aFut As MmRvFutAtom
    Dim bFoundIDX As Boolean
    Dim bFound As Boolean
    Dim sKey As String
    bFound = False
    bFoundIDX = False
    
    If m_RiskView.Idx.ID = iUndID Then
        Set m_RiskView.Idx.ActiveFuture = Nothing
        Set aUnd = m_RiskView.Und(iUndID)
        
        If Not aUnd Is Nothing Then
            Set aFut = aUnd.Fut(iActiveFutID)
            If Not aFut Is Nothing Then
                Set m_RiskView.Idx.ActiveFuture = aFut
                bFoundIDX = True
            Else
                Set aFutReq = m_AuxClc.QuoteReqsAll(CStr(enCtFuture) & "_" & aFut.Symbol)
                ' check weither such request already done
                If aFutReq Is Nothing Then
                    ' Reload index
                    IndexLoad
                Else
                     Set m_RiskView.Idx.ActiveFuture = aFutReq.Fut
                     bFoundIDX = True
                End If
            End If
        Else
            ' add new futures
            ' Reload index
            Set aFutReq = m_AuxClc.QuoteReqsAll(CStr(enCtFuture) & "_" & g_UnderlyingAll(iUndID).ActiveFuture.Symbol)
            If aFutReq Is Nothing Then
                IndexLoad
            Else
                 Set m_RiskView.Idx.ActiveFuture = aFutReq.Fut
                 bFoundIDX = True
            End If
        End If
    End If

    
    Set aUnd = m_RiskView.Und(iUndID)
    If Not aUnd Is Nothing Then
        If aUnd.ActiveFuture Is Nothing And iActiveFutID > 0 Then bFound = True
        If Not aUnd.ActiveFuture Is Nothing And aUnd.ActiveFuture.ID <> iActiveFutID Then bFound = True
    End If
    If bFound Then
        Set aUnd.ActiveFuture = Nothing
        Set aFut = aUnd.Fut(iActiveFutID)
        If Not aFut Is Nothing Then
            Set aUnd.ActiveFuture = aFut
            aUnd.ClearRatios
        Else
            If iActiveFutID <> 0 Then
                Dim aNewReqsAll As New EtsMmRisksLib.MmRvReqColl
                bFound = False
                m_RiskView.AddNewActiveFutures aUnd, aNewReqsAll
            End If
         End If
    End If
    
    If bFound Or bFoundIDX Then
        If Not m_bInProc Then
            m_AuxClc.UnderlyingsCalc True, True, False, False
            RefreshPositions
            m_AuxOut.TotalsUpdate
        End If
    End If
End Sub

Public Sub FuturesParamsChange(ByVal iUndID As Long, ByVal iFutID As Long, ByVal dRatio As Double, ByVal dBasis As Double)
    On Error Resume Next
    Dim aUnd As MmRvUndAtom
    Dim aFut As MmRvFutAtom
    Dim bFound As Boolean
    bFound = False
    If m_bShutDown Then Exit Sub
    
    If m_RiskView.Idx.ID = iUndID Then
        If Not m_RiskView.Idx.ActiveFuture Is Nothing Then
            If m_RiskView.Idx.ActiveFuture.Ratio <> dRatio Or m_RiskView.Idx.ActiveFuture.bAsIs <> dBasis Then bFound = True
            m_RiskView.Idx.ActiveFuture.Ratio = dRatio
            m_RiskView.Idx.ActiveFuture.bAsIs = dBasis
            m_RiskView.Idx.price.IsDirty = bFound
        End If
    End If
    
    Set aUnd = m_RiskView.Und(iUndID)
    If Not aUnd Is Nothing Then
      If Not aUnd.ActiveFuture Is Nothing Then
        If aUnd.ActiveFuture.ID = iFutID Then
            If aUnd.ActiveFuture.Ratio <> dRatio Or aUnd.ActiveFuture.bAsIs <> dBasis Then bFound = True
            aUnd.ActiveFuture.Ratio = dRatio
            aUnd.ActiveFuture.bAsIs = dBasis
            aUnd.price.IsDirty = bFound
        End If
      End If
      If Not aUnd.Fut Is Nothing Then
        Set aFut = aUnd.Fut(iFutID)
        If Not aFut Is Nothing Then
            If aFut.Ratio <> dRatio Or aFut.bAsIs <> dBasis Then bFound = True
            aFut.Ratio = dRatio
            aFut.bAsIs = dBasis
            aFut.price.IsDirty = bFound
        End If
      End If
    End If
    If bFound And Not m_bInProc Then
        m_AuxClc.UnderlyingsCalc True, True, False, False
        RefreshPositions
        m_AuxOut.TotalsUpdate
    End If
End Sub

Private Function IsFutEditable(aFut As MmRvFutAtom) As Boolean
On Error GoTo Exception

    IsFutEditable = False
    Dim aUnd As MmRvUndAtom
    Dim aActFut As MmRvFutAtom
    
    If (aFut Is Nothing) Then Exit Function

    Set aUnd = aFut.Underlying
    
    If (Not aUnd Is Nothing) Then
        Set aActFut = aUnd.ActiveFuture
        If (aUnd.IsHead) Then
            If (Not aActFut Is Nothing) Then
                If (aActFut.ID = aFut.ID) Then
                    IsFutEditable = True
                End If
            Else
                IsFutEditable = True
            End If
        Else
            If (Not aUnd.PriceByHead) Then
                If (Not aActFut Is Nothing) Then
                    If (aActFut.ID = aFut.ID) Then
                        IsFutEditable = True
                    End If
                Else
                    IsFutEditable = True
                End If
            End If
        End If
    End If
    
    Set aActFut = Nothing
    Set aUnd = Nothing
    Exit Function
    
Exception:
    Debug.Print "Error while trying to CheckFutureForEdit"
    
    Set aActFut = Nothing
    Set aUnd = Nothing
End Function

Private Function IsUndEditable(aUnd As MmRvUndAtom) As Boolean
On Error GoTo Exception
    IsUndEditable = False
    
    Dim aActFut As MmRvFutAtom
    If (aUnd Is Nothing) Then Exit Function
    
    Set aActFut = aUnd.ActiveFuture
    
    If (aUnd.IsHead) Then
        If (aActFut Is Nothing) Then
            IsUndEditable = True
        End If
    Else
        If (Not aUnd.PriceByHead And aActFut Is Nothing) Then
            IsUndEditable = True
        End If
    End If
    Set aActFut = Nothing
    
    Exit Function
Exception:
    Debug.Print "Error while trying to CheckUndFotEdit"
    Set aActFut = Nothing
End Function

Private Sub PubManualPrice(dPrice As Double, nContractID As Long, nUndID As Long, enCtType As EtsContractTypeEnum, enStatus As MANUAL_PRICE_UPDATE_STATUS)
On Error GoTo Exception

    Dim Data As MSGSTRUCTLib.ManualPriceUpdate
    Set Data = New MSGSTRUCTLib.ManualPriceUpdate
                                            
    Data.Status = enStatus
    Data.ContractID = nContractID
    Data.UndID = nUndID
    Data.ContractType = enCtType
    Data.ActivePrice = dPrice
                                            
    g_TradeChannel.PubManualPriceUpdate Data
    Exit Sub
    
Exception:
    Debug.Print "Error while trying to pub active price for contract"
End Sub

Private Sub ManualPriceDel(nID As Long)
On Error GoTo Exception
    gDBW.usp_MmManualPrice_Del nID
    Exit Sub
Exception:
    Debug.Print "Error while trying to delete manual price from db RiskView"
End Sub

Private Sub ManualPriceSave(dPrice As Double, nID As Long)
On Error GoTo Exception
    gDBW.usp_MmManualPrice_Save nID, dPrice
    Exit Sub
Exception:
    Debug.Print "Error while trying to save manual price to db RiskView"
End Sub

Private Sub UpdatePosOptManualPrice(aPos As MmRvPosAtom, dPrice As Double, enStatus As MANUAL_PRICE_UPDATE_STATUS)
On Error GoTo Exception
    If (aPos Is Nothing) Then Exit Sub
        
    If (enStatus = enUsAdd Or enStatus = enUsUpdate) Then
        aPos.Quote.price.Active = dPrice
        aPos.Quote.price.IsUseManualActive = True
        'save manual price to db
        ManualPriceSave dPrice, aPos.ID
        'RaiseEvent
        RaiseEvent OnManualPriceChanged(aPos.UndID, aPos.ID, dPrice, aPos.ContractType, MPU_UPDATE)
        'pub manual price
        PubManualPrice dPrice, aPos.ID, aPos.UndID, aPos.ContractType, enStatus
    Else
        aPos.Quote.price.IsUseManualActive = False
        'delete manu lrpice from db
        ManualPriceDel aPos.ID
        'RaiseEvent
        RaiseEvent OnManualPriceChanged(aPos.UndID, aPos.ID, 0#, aPos.ContractType, MPU_DELTE)
        'pub changes
        PubManualPrice 0#, aPos.ID, aPos.UndID, aPos.ContractType, enStatus
    End If
    
    Exit Sub
Exception:
    Debug.Print "Error while trying to Update PosOpt Manual Price"
End Sub

Private Sub UpdateFutManualPrice(aFut As MmRvFutAtom, dPrice As Double, enStatus As MANUAL_PRICE_UPDATE_STATUS)
On Error GoTo Exception
        If (aFut Is Nothing) Then Exit Sub
        
        If (enStatus = enUsUpdate Or enStatus = enUsAdd) Then
            aFut.price.Active = dPrice
            aFut.price.IsUseManualActive = True
            'manual price save to db
            ManualPriceSave dPrice, aFut.ID
            'update global collection
            If (Not g_ContractAll(aFut.ID) Is Nothing) Then
                If (Not g_ContractAll(aFut.ID).Fut Is Nothing) Then
                    g_ContractAll(aFut.ID).Fut.manualActivePrice = dPrice
                End If
            End If
            'RaiseEvent
            RaiseEvent OnManualPriceChanged(aFut.UndID, aFut.ID, dPrice, enCtFuture, MPU_UPDATE)
            'pub changes
            PubManualPrice dPrice, aFut.ID, aFut.UndID, enCtFuture, enStatus
        Else
            aFut.price.IsUseManualActive = False
            'delete manual price
            ManualPriceDel aFut.ID
            'update global colection
            If (Not g_ContractAll(aFut.ID) Is Nothing) Then
                If (Not g_ContractAll(aFut.ID).Fut Is Nothing) Then
                    g_ContractAll(aFut.ID).Fut.manualActivePrice = 0#
                End If
            End If
            'RaiseEvent
            RaiseEvent OnManualPriceChanged(aFut.UndID, aFut.ID, 0#, enCtFuture, MPU_DELTE)
            'pub changes
            PubManualPrice 0#, aFut.ID, aFut.UndID, enCtFuture, enStatus
        End If
        
    Exit Sub
Exception:
    Debug.Print "Error while trying to UpdateFutureManualPrice"
End Sub

Private Sub UpdateUndManualPrice(aUnd As MmRvUndAtom, dPrice As Double, enStatus As MANUAL_PRICE_UPDATE_STATUS)
On Error GoTo Exception
    If (aUnd Is Nothing) Then Exit Sub
    Dim CtType As EtsContractTypeEnum
    
    If (enStatus = enUsAdd Or enStatus = enUsUpdate) Then
        aUnd.price.Active = dPrice
        aUnd.price.IsUseManualActive = True
        'manual price save to DB
        ManualPriceSave dPrice, aUnd.ID
        'update global collection
        If (Not g_ContractAll(aUnd.ID) Is Nothing) Then
            If (Not g_ContractAll(aUnd.ID).Und Is Nothing) Then
                CtType = g_ContractAll(aUnd.ID).ContractType
                g_ContractAll(aUnd.ID).Und.manualActivePrice = dPrice
            End If
        End If
        'RaiseEvent
        RaiseEvent OnManualPriceChanged(aUnd.ID, aUnd.ID, dPrice, CtType, MPU_UPDATE)
        'pub changes
        PubManualPrice dPrice, aUnd.ID, aUnd.ID, CtType, enStatus
    Else
        aUnd.price.IsUseManualActive = False
        'manual price save to DB
        ManualPriceDel aUnd.ID
        'update global collection
        If (Not g_ContractAll(aUnd.ID) Is Nothing) Then
            If (Not g_ContractAll(aUnd.ID).Und Is Nothing) Then
                CtType = g_ContractAll(aUnd.ID).ContractType
                g_ContractAll(aUnd.ID).Und.manualActivePrice = 0#
            End If
        End If
        'RaiseEvent
        RaiseEvent OnManualPriceChanged(aUnd.ID, aUnd.ID, 0#, CtType, MPU_DELTE)
        'pub changes
        PubManualPrice 0#, aUnd.ID, aUnd.ID, CtType, enStatus
    End If
    
    Exit Sub
Exception:
    Debug.Print "Error while trying to Upadte ManualPrice for Underlying"
End Sub

Public Sub ImmediateRefresh()
    On Error Resume Next
    If m_bFirstTime Then m_Aux.Grp.ID = -1
    m_bFirstTime = False
End Sub

Public Function ExportToHTML(ByVal sFileName As String, _
                             ByVal sFilePath As String, _
                             ByVal bShowFilter As Boolean, _
                             ByVal bShowPositions As Boolean) As Boolean
                             '------------------------------------------'
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportToHTML = g_ScreenExport.SaveToHTML(sFileName, _
                                            sFilePath, _
                                            fgPos, _
                                            IIf(bShowFilter, fgTot, Nothing), _
                                            IIf(bShowFilter, fgFlt, Nothing), _
                                            bShowPositions)
    Screen.MousePointer = vbNormal
End Function

Public Function ExportToCSV(ByVal sFileName As String, _
                            ByVal sFilePath As String, _
                            ByVal bShowFilter As Boolean, _
                            ByVal bShowPositions As Boolean) As Boolean
                            '------------------------------------------'
On Error Resume Next


    Screen.MousePointer = vbHourglass
    ExportToCSV = g_ScreenExport.SaveToCSV(sFileName, _
                                            sFilePath, _
                                            fgPos, _
                                            IIf(bShowFilter, fgTot, Nothing), _
                                            IIf(bShowFilter, fgFlt, Nothing), _
                                            bShowPositions)
    Screen.MousePointer = vbNormal

End Function

Public Sub ShowSimulationScenario()
    On Error GoTo error_handler
    '-------------------------'
        
        Set simulation_scenario = frmMarketSimulationScenario.Execute(simulation_scenario)
            
    Exit Sub
'-------------'
error_handler:
    Debug.Print "Error while Show Simulation Scenario. Description: " & _
                Err.Description
End Sub


Public Sub UseSimulation(ByVal use_simulation As Boolean)
    On Error Resume Next
    '-------------------'
    
    If (Not m_Aux Is Nothing) Then
        If (Not m_Aux.RiskView Is Nothing) Then
            
            If (simulation_scenario Is Nothing) Then
                ShowSimulationScenario
            End If
            
            Set m_Aux.RiskView.SimulationScenario = IIf(use_simulation, simulation_scenario, Nothing)
            
            AdjustCaption
            
        End If
    End If
    
End Sub

Private Function SimulationUsed() As Boolean
    On Error Resume Next
    '-------------------'

    If (Not m_Aux Is Nothing) Then
        If (Not m_Aux.RiskView Is Nothing) Then
            SimulationUsed = IIf(m_Aux.RiskView.SimulationScenario Is Nothing, False, True)
            Exit Function
        End If
    End If
    
    SimulationUsed = False
End Function

Private Sub UpdateRecalculationTimeStemp()
    On Error Resume Next
    '-------------------'
    
    If (Not m_Aux Is Nothing) Then
        recalculation_time_stemp = Time
    End If
    
End Sub

Public Function getRecalculationTimeStemp() As Date
    On Error Resume Next
    '-------------------'
    If (Not m_Aux Is Nothing) Then
        getRecalculationTimeStemp = recalculation_time_stemp
        Exit Function
    End If
   
    getRecalculationTimeStemp = 0
    
End Function
