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
   Begin VB.Timer tmrUndCalc 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   3240
      Top             =   4440
   End
   Begin VB.Timer tmrRealTime 
      Enabled         =   0   'False
      Interval        =   10
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
      Begin MSComCtl2.DTPicker dtPick 
         Height          =   280
         Left            =   5400
         TabIndex        =   3
         Top             =   0
         Visible         =   0   'False
         Width           =   1575
         _ExtentX        =   2778
         _ExtentY        =   503
         _Version        =   393216
         DateIsNull      =   -1  'True
         Format          =   49020929
         CurrentDate     =   38680
      End
   End
   Begin VSFlex7Ctl.VSFlexGrid fgTot 
      Height          =   495
      Left            =   0
      TabIndex        =   1
      Top             =   960
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
   Begin VB.Image imgBadPrice 
      Height          =   240
      Left            =   4680
      Picture         =   "ctlRiskView_new.ctx":0000
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
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
         Enabled         =   0   'False
         Visible         =   0   'False
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
      Begin VB.Menu mnuCtxFitTheoForSelected 
         Caption         =   "Fit Theo to MIV for selected equity"
      End
      Begin VB.Menu mnuCtxFitTheoForAll 
         Caption         =   "Fit Theo to MIV for all equities"
      End
      Begin VB.Menu mnuCtxSeparator20 
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
      Begin VB.Menu mnuCtxEditCustomVariable 
         Caption         =   "Edit Custom Variable..."
      End
      Begin VB.Menu mnuCtxSeparator10 
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

Public Event OnRefreshComplete()
Public Event OnRefreshError()
Public Event OnRefreshEmpty()
Public Event OnRefreshCancel()

Private m_gdFlt As New clsGridDef
Private m_gdTot As New clsGridDef
Private m_gdPos As New clsGridDef

Private WithEvents gePos As clsGridEx
Attribute gePos.VB_VarHelpID = -1

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPriceWithNotify
Private m_BatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
Private WithEvents VolaSource As VolatilitySourcesLib.VolatilitySource
Attribute VolaSource.VB_VarHelpID = -1
Private WithEvents TradeChannel As clsTradeChannel
Attribute TradeChannel.VB_VarHelpID = -1
Private WithEvents frmWtdVega As frmWtdVegaSettings
Attribute frmWtdVega.VB_VarHelpID = -1

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Private WithEvents m_frmVarEditor As frmCustomVariableEditor
Attribute m_frmVarEditor.VB_VarHelpID = -1
Private WithEvents m_frmStockShift As frmStockShift
Attribute m_frmStockShift.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As VB.Label
Public lblStatus As VB.Label
Public WithEvents imgStop As VB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As VB.Image

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bSubscribingNow As Boolean
Private m_bInRealTimeCalc As Boolean
Private m_bLastQuoteReqNow As Boolean
Private m_bTmrUndCalcNow As Boolean

Private m_bTradeChannelEnable As Boolean

Private m_OpenedUnd As New EtsGeneralLib.EtsMmFilterAtomColl
Private m_nOpenedExpiry As Long

Private m_bCurUndChanging As Boolean
Private m_nCurUnd As Long

Private m_nQuoteReqCount As Long
Private m_nQuoteReqDone As Long
Private m_nQuoteGroupReqCount As Long
Private m_nQuoteGroupReqDone As Long

Private m_bVolaChangedNow As Boolean
Public ProcessRealTime As Boolean
Private m_bVolaUpdated As Boolean
Public m_frmOwner As Form

Private m_sCurrentOriginalText As String

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_RISKS_FILTER To GT_RISKS_POSITIONS) As Boolean

Private m_bGroupRequest As Boolean
Private m_bGroupRequestSupported As Boolean
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

Private m_Aux As New clsAuxRiskView
Private m_AuxOut As New clsAuxRiskViewOut
Private m_AuxClc As New clsAuxRiskViewCalc
Private m_nUndPositions As Integer
Private m_nOptPositions As Integer

Private TradeQueue As clsTradeQueueColl
Dim m_frmCustDivs As New frmCustomDivs

Private m_bIsNewTrades As Boolean


Private m_bShowInTaskbar As Boolean
Private m_bShowInTaskbarChanging As Boolean

Public Function Init(Optional ByVal bLoadStockShifts As Boolean = True) As Boolean
    On Error GoTo EH
    
    m_bShutDown = False
    m_bVolaChangedNow = False
    m_bVolaUpdated = False
    m_bIsNewTrades = False
    
    m_bTradeChannelEnable = True
    
    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    Set VolaSource = g_VolaSource
    'InitColumns
    
    m_Aux.Grp = New EtsMmRisksLib.MmRvGrpAtom
    m_Aux.Idx = New EtsMmRisksLib.MmRvUndAtom
    
    Set m_Aux.VolaSource = VolaSource
    Set m_Aux.gdFlt = m_gdFlt
    Set m_Aux.gdTot = m_gdTot
    Set m_Aux.gdPos = m_gdPos
    Set m_Aux.fgFlt = fgFlt
    Set m_Aux.fgTot = fgTot
    Set m_Aux.fgPos = fgPos
    
    m_Aux.InitCVContext
    m_Aux.InitRisksGridsWithCV
    
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
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    
    m_bGroupRequest = False
    
    m_bGroupRequestSupported = g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequestSupported Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    Set m_BatchPriceProvider = PriceProvider
    If m_BatchPriceProvider Is Nothing Then LogEvent EVENT_ERROR, "Selected price provider is not compatible with Risk view."
    fgFlt.Col = RFC_GROUP
    fgFlt.Row = 1
    
    fgTot.Col = 0
    fgTot.Row = 1
    
    Set TradeQueue = New clsTradeQueueColl
    Set m_frmVarEditor = New frmCustomVariableEditor
    Set m_frmStockShift = New frmStockShift
    Set frmLayout = New frmGridLayout
    Init = True
    Set aParams = g_Params
    
    If bLoadStockShifts Then m_frmStockShift.LoadData g_Params.StockShiftFile
    m_frmCustDivs.InitRs
    
    AdjustCaption
    AdjustState
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize risk view."
    RaiseEvent OnRefreshError
End Function

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    m_Aux.Filter(RFC_GROUP) = nType
    m_Aux.Filter(RFC_VALUE) = nValue
    m_Aux.FilterUpdateAll
    tmrShow.Enabled = True
End Sub

Public Function InitGrids(Optional ByVal nNewVarID As Long = BAD_LONG_VALUE) As Boolean
    On Error GoTo EH
    
    m_bShutDown = False
    m_bVolaChangedNow = False
    m_bVolaUpdated = False
    
    m_Aux.InitCVContext
    m_Aux.InitRisksGridsWithCV nNewVarID
    InitColumns
    
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
    
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_RISKS_FILTER) = False
    m_bKeyDown(GT_RISKS_TOTALS) = False
    m_bKeyDown(GT_RISKS_POSITIONS) = False

    m_Aux.GridLock(GT_RISKS_FILTER).Init fgFlt
    m_Aux.GridLock(GT_RISKS_TOTALS).Init fgTot
    m_Aux.GridLock(GT_RISKS_POSITIONS).Init fgPos
    
    Set gePos = New clsGridEx
    gePos.Init fgPos
    Set m_Aux.gePos = gePos
    
    m_Aux.FormatFltGrid
    m_Aux.FormatFltColumns
    
    m_Aux.FormatTotGrid
    m_Aux.FormatTotColumns
    
    m_Aux.FormatPosGrid
    m_Aux.FormatPosColumns
    
    fgFlt.Col = RFC_GROUP
    fgFlt.Row = 1
    
    fgTot.Col = 0
    fgTot.Row = 1
    
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize risk view."
    RaiseEvent OnRefreshError
End Function

Public Sub FilterUpdateModel(ByVal bAutosize As Boolean)
    On Error Resume Next
    m_Aux.FilterUpdateModel bAutosize
End Sub

Private Function PositionsLoad() As Boolean
    On Error Resume Next
    PositionsLoad = False
    m_AuxClc.UndCount = 0
    m_AuxClc.OptCount = 0
    
    If m_bInProc Or m_Aux.Filter(RFC_VALUE) = 0 Or m_Aux.Filter(RFC_GROUP) = 0 Then Exit Function
    
    Dim i&, nCount&, aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aExp As EtsGeneralLib.EtsMmEntityAtom
    Dim collExp As New EtsGeneralLib.EtsMmEntityAtomColl, arrExp() As Long
    
    Dim nStart&
    nStart = GetTickCount
    Dim aGrpDataProvider As CVLib.IcvDataProvider
    Dim infoSysVars() As CVLib.cvInfo
    Dim infoSysFuncs() As CVLib.cvInfo
    Dim lUnsupported As Long
    
    m_Aux.Grp.ID = m_Aux.Filter(RFC_VALUE)
    m_Aux.Grp.GroupType = m_Aux.Filter(RFC_GROUP)
    
    ' simulation stuff
    If m_Aux.Simulation = True Then
        If Not m_Aux.Grp.SimulationGrp Is Nothing Then m_Aux.Grp.SimulationGrp = Nothing    'Simulation stuff
        Set m_Aux.Grp.SimulationGrp = New EtsMmRisksLib.MmRvGrpAtom                         'Simulation stuff
    End If
    
    ' cv stuff
    Set aGrpDataProvider = m_Aux.Grp
    Set m_Aux.Grp.cvRTContext = m_Aux.m_aContext    ' If simulation running, simultion atoms will use context from m_Aux.Grp

    m_Aux.m_aContext.GetSystemEnvironment m_Aux.m_nGrpLevelID, infoSysVars, infoSysFuncs
    lUnsupported = aGrpDataProvider.Check(infoSysVars, infoSysFuncs)
    If lUnsupported > 0 Then
        m_Aux.m_aContext.Halt
        gCmn.ErrorMsgBox m_frmOwner, "Custom variables functionality disabled due incorrect version of data provider"
    Else
        m_Aux.Grp.cvPoolID = m_Aux.m_aContext.CreateCVPool(aGrpDataProvider, m_Aux.m_nGrpLevelID)
        If m_Aux.Simulation = True Then
            Set aGrpDataProvider = m_Aux.Grp.SimulationGrp                                  'Simulation stuff
            m_Aux.Grp.SimulationGrp.cvPoolID = m_Aux.m_aContext.CreateCVPool(aGrpDataProvider, m_Aux.m_nGrpLevelID) 'Simulation stuff
        End If
    End If
    
    
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState
    
    Err.Clear
    Select Case m_Aux.Grp.GroupType
        Case TYPE_UNDERLYING
            m_Aux.Grp.Name = g_Underlying(m_Aux.Grp.ID).Symbol
            If Err.Number <> 0 Then m_bDataLoad = False
            
        Case TYPE_GROUP
            m_Aux.Grp.Name = g_UnderlyingGroup(m_Aux.Grp.ID).Name
            If Err.Number <> 0 Then m_bDataLoad = False
            
        Case TYPE_TRADER_GROUP
            m_Aux.Grp.Name = g_TraderGroup(m_Aux.Grp.ID).Name
            If Err.Number <> 0 Then m_bDataLoad = False
        
        Case TYPE_TRADER
            m_Aux.Grp.Name = g_Trader(m_Aux.Grp.ID).Name
            If Err.Number <> 0 Then m_bDataLoad = False
        
        Case TYPE_STRATEGY
            If m_Aux.Grp.ID > 0 Then
                m_Aux.Grp.Name = g_Strategy(m_Aux.Grp.ID).Name
            Else
                m_Aux.Grp.Name = NO_STRATEGY_NAME
            End If
            If Err.Number <> 0 Then m_bDataLoad = False
        
        Case TYPE_ALL
            m_Aux.Grp.Name = "<All>"
        
        Case Else
            m_bDataLoad = False
    End Select
    
    If Not m_bDataLoad Then GoTo Ex
    On Error GoTo EH
    
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Started Positions Loading.", m_frmOwner.GetCaption

    For Each aTrd In g_TradeChannel.Trades.FilterTradesCV(m_Aux.Grp.ID, m_Aux.Grp.GroupType, m_Aux.Filter(RFC_TYPE), g_UnderlyingGroup, False, g_Params.DisplayZeroPositions)
        'If /*m_Aux.CheckTradeFilter(aTrd)*/  Then
        If True Then
        
            Set aUnd = m_Aux.Und(aTrd.UndID)
            If aUnd Is Nothing Then
                Set aUnd = m_AuxClc.AddNewUnderlying(aTrd)
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Loading New Underlying " & aUnd.Symbol, m_frmOwner.GetCaption
                
            End If
            
            Set aPos = aUnd.Pos(aTrd.ContractID)
            If aPos Is Nothing Then
                 Set aPos = m_AuxClc.AddNewPosition(aTrd, aUnd)
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Loading New Position " & aPos.Symbol & " For Underlying: " & aUnd.Symbol, m_frmOwner.GetCaption
            End If
            
            m_AuxClc.AddNewTradeToPosition aUnd, aPos, aTrd
        
            Set aPos = Nothing
            Set aUnd = Nothing
        End If
        
        DoEvents
        If Not m_bDataLoad Then GoTo Ex
        IncProgress pbProgress
        
        Set aTrd = Nothing
    Next
    
    nCount = collExp.Count
    If nCount > 0 Then
        ReDim arrExp(1 To nCount)
        
        i = 1
        For Each aExp In collExp
            arrExp(i) = aExp.ID
            i = i + 1
        Next
        
        If nCount > 1 Then SortArray arrExp, 1, nCount
        
        For i = 1 To nCount
            m_Aux.Exp.Add CStr(arrExp(i)), collExp(CStr(arrExp(i)))
        Next
    End If
    
    m_AuxClc.UnderlyingsAdjustRates True
    PositionsLoad = m_bDataLoad
    If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "Finished Data Loading. Duration " & GetTickCount - nStart & " ms", m_frmOwner.GetCaption
Ex:
    On Error Resume Next
    m_bInProc = False
    m_bDataLoad = False
    Erase arrExp
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load positions."
    RaiseEvent OnRefreshError
    GoTo Ex
End Function

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_RISKS_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_RISKS_TOTALS).CopyTo m_gdTot
    g_DefGrids(GT_RISKS_POSITIONS).CopyTo m_gdPos
End Sub

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    RaiseEvent OnSetRefreshHint(bSet, IIf(bSet, "Use F5 to refresh results.", ""))
End Sub

Private Sub aParams_PriceProfilesChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.GroupType = 0 Or m_Aux.Grp.ID = 0 Or m_bSubscribingNow Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
    Else
        RefreshView
    End If
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.GroupType = 0 Or m_Aux.Grp.ID = 0 Or m_bSubscribingNow Then Exit Sub
    
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
    
    If Not m_Aux.RealTime And Not m_bSubscribingNow Then
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
        If bUpdate And Not m_bSubscribingNow Then RefreshView
    End If
End Sub

Private Sub aParams_UndPriceToleranceChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.GroupType = 0 Or m_Aux.Grp.ID = 0 Or m_bSubscribingNow Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
    Else
        RefreshView
    End If
End Sub

Private Sub dtPick_Change()
    If dtPick.Visible Then
        If dtPick.Value < Date Then
            fgFlt.TextMatrix(1, RFC_SIM_DATE) = Date
            m_Aux.SimulationDate = GetNewYorkTime
        Else
            fgFlt.TextMatrix(1, RFC_SIM_DATE) = dtPick.Value
            m_Aux.SimulationDate = GetNewYorkTime + (dtPick.Value - Date)
        End If
    End If
End Sub

Private Sub dtPick_LostFocus()
    dtPick.Visible = False
    If dtPick.Value < Date Then
        fgFlt.TextMatrix(1, RFC_SIM_DATE) = Date
        m_Aux.SimulationDate = GetNewYorkTime
    Else
        fgFlt.TextMatrix(1, RFC_SIM_DATE) = dtPick.Value
        m_Aux.SimulationDate = GetNewYorkTime + (dtPick.Value - Date)
    End If
End Sub

Private Sub fgFlt_CellButtonClick(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    
    m_frmStockShift.ShowData g_Params.StockShiftFile, m_Aux.m_StockShifts, , , m_Aux.Und, m_Aux.m_nShiftID, m_frmOwner
    
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
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Filter_DblClick Exit", m_frmOwner.GetCaption

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
    If Not m_bShutDown Then m_Aux.PosGridCompare Row1, Row2, Cmp
End Sub

Private Sub fgTot_DblClick()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgTot
        m_enMenuGrid = GT_RISKS_TOTALS
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
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
            m_nMenuGridRows = .Rows
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
              
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Totals Hot key: Ctrl+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
              
                mnuCtxCopy_Click
        End Select
    End If
End Sub

Private Sub frmLayout_OnGridChanged(ByVal enGridType As GridTypeEnum, ByVal nVarID As Long)
    On Error Resume Next
    SaveGridDefAsDefault m_gdTot, GT_RISKS_TOTALS
    SaveGridDefAsDefault m_gdPos, GT_RISKS_POSITIONS
    
    InitGrids nVarID
    ShowData Group.GroupType, Group.ID
End Sub

Private Sub imgStop_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        lblStatus.Caption = "Last quotes request cancelling..."
        lblStatus.Refresh
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
    
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Click on imgStop: Last quotes request cancelling...", m_frmOwner.GetCaption
           
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
            
        m_bInProc = True
        AdjustState
            
        m_AuxClc.UnderlyingsCalc True, True
        m_AuxOut.UnderlyingsUpdate False
        m_AuxOut.TotalsUpdate
        
        m_bInProc = False
        AdjustState
        
    ElseIf m_Aux.RealTime Or m_bSubscribingNow Then
        m_Aux.RealTime = False
        m_Aux.RealTimeConnected = False
        m_bSubscribingNow = False
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        lblStatus.Caption = "Quotes subscription cancelling..."
        lblStatus.Refresh
        
        PriceProvider.UnSubscribeQuote
        If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Click on imgStop: Quotes subscription cancelling...", m_frmOwner.GetCaption
    
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
        
        AdjustState
        AdjustCaption
        
    ElseIf m_bDataLoad Then
        m_bDataLoad = False
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Data loading Canceled by Click on imgStop.", m_frmOwner.GetCaption
    
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bInProc Then
        m_bInProc = False
        AdjustState
    End If
    
    RaiseEvent OnRefreshCancel
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim sValue$, nValue&, sVal$
    Dim sTypeOld As String, sValueOld As String, sTradesOld As String, sIndexOld As String, sExpiryOld As String
    Dim sTypeNew As String, sValueNew As String, sTradesNew As String, sIndexNew As String, sExpiryNew As String

    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Filter_AfterEdit Enter", m_frmOwner.GetCaption
    
    
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = CLng(sValue)
            If Err.Number <> 0 Then nValue = val(sValue)
            If Err.Number = 0 Then
                sTypeOld = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_GROUP)
                sValueOld = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_VALUE)
                sTradesOld = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_TYPE)
                sExpiryOld = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_EXPIRY)
                sIndexOld = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_INDEX)
                Select Case Col
                    Case RFC_GROUP
                        m_Aux.Filter(RFC_GROUP) = nValue
                        If m_Aux.Filter(RFC_GROUP) <> TYPE_ALL Then
                            m_Aux.Filter(RFC_VALUE) = 0
                            m_Aux.FilterUpdateValue True
                            tmrShow.Enabled = True
                            sTypeNew = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_GROUP)
                            If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Editing Filter Type: " & _
                                "Old Type Filter: " & sTypeOld & "; Value filter: " & sValueOld & _
                                "; Trades Filter: " & sTradesOld & "; Expiry Filter: " & sExpiryOld & "; Index Filter: " & sIndexOld & _
                                "; New Type Filter: " & sTypeNew _
                                , m_frmOwner.GetCaption
                        Else
                            StoreRecentFilters
                            m_Aux.Filter(RFC_VALUE) = -1
                            m_Aux.FilterUpdateValue True
                            tmrShow.Enabled = True
                            sTypeNew = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_GROUP)
                            If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Editing Filter Type: " & _
                                "Old Type Filter: " & sTypeOld & _
                                "; Trades Filter: " & sTradesOld & "; Expiry Filter: " & sExpiryOld & "; Index Filter: " & sIndexOld & _
                                "; New Type Filter: " & sTypeNew _
                                , m_frmOwner.GetCaption
                        End If
                        
                    Case RFC_VALUE
                        StoreRecentFilters
                        m_Aux.Filter(RFC_VALUE) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        tmrShow.Enabled = True
                        sValueNew = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_VALUE)
                        If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Editing Filter Value: " & _
                                "Type Filter: " & sTypeOld & "; Old Value filter: " & sValueOld & _
                                "; Trades Filter: " & sTradesOld & "; Expiry Filter: " & sExpiryOld & "; Index Filter: " & sIndexOld & _
                                "; New Value Filter: " & sValueNew _
                                , m_frmOwner.GetCaption

                                                
                    Case RFC_TYPE
                        m_Aux.Filter(RFC_TYPE) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        StoreRecentFilters
                        tmrShow.Enabled = True
                        sTradesNew = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_TYPE)
                        If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Editing Filter Trades: " & _
                                " Type Filter: " & sTypeOld & "; Value filter: " & sValueOld & _
                                ";Old Trades Filter: " & sTradesOld & "; Expiry Filter: " & sExpiryOld & "; Index Filter: " & sIndexOld & _
                                "; New Trades Filter: " & sTradesNew _
                                , m_frmOwner.GetCaption

      
                    Case RFC_EXPIRY
                        m_Aux.Filter(RFC_EXPIRY) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        PositionsApplyFilter
                        
                        m_bInProc = True
                        m_AuxClc.UnderlyingsCalc True, True
                        m_AuxOut.UnderlyingsUpdate True
                        m_AuxOut.TotalsUpdate
                        m_bInProc = False
                        sExpiryNew = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_EXPIRY)
                        If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Editing Filter Expiry: " & _
                                " Type Filter: " & sTypeOld & "; Value filter: " & sValueOld & _
                                ";Trades Filter: " & sTradesOld & "; Old Expiry Filter: " & sExpiryOld & "; Index Filter: " & sIndexOld & _
                                "; New Expiry Filter: " & sExpiryNew _
                                , m_frmOwner.GetCaption
                    Case RFC_INDEX
                        m_Aux.Filter(RFC_INDEX) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        IndexLoad
                        
                        If m_Aux.Und.Count > 0 Then
                            If m_Aux.RealTime Then
                                SubscribeToIndex
                            Else
                                RequestIndexLastQuote
                            End If
                        End If
                        sIndexNew = m_Aux.fgFlt.Cell(flexcpTextDisplay, Row, RFC_INDEX)
                        If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Editing Filter Index: " & _
                                " Type Filter: " & sTypeOld & "; Value filter: " & sValueOld & _
                                ";Trades Filter: " & sTradesOld & "; Expiry Filter: " & sExpiryOld & "; Old Index Filter: " & sIndexOld & _
                                "; New Index Filter: " & sIndexNew _
                                , m_frmOwner.GetCaption
                   Case RFC_SIM
                        m_Aux.Simulation = IIf(m_Aux.Simulation, False, True)
                        If Not m_Aux.Simulation Then
                            m_Aux.SimulationDate = GetNewYorkTime
                            m_Aux.SimulationVola = 0
                            m_Aux.SimulationSpot = 0
                            .TextMatrix(1, RFC_SIM_VOLA) = 0
                            .TextMatrix(1, RFC_SIM_SPOT) = 0
                            .TextMatrix(1, RFC_SIM_DATE) = GetNewYorkTime
                        End If
                        dtPick.Visible = False
                        With fgFlt
                            .ColHidden(RFC_SIM_DATE) = IIf(m_Aux.Simulation, False, True)
                            .ColHidden(RFC_SIM_VOLA) = IIf(m_Aux.Simulation, False, True)
                            .ColHidden(RFC_SIM_SPOT) = IIf(m_Aux.Simulation And m_Aux.m_enSpotShift <> enStockSpecific, False, True)
                            
                            .ColHidden(RFC_SIM_TYPE) = IIf(m_Aux.Simulation, False, True)
                            .ColHidden(RFC_SHIFT_ID) = IIf(m_Aux.Simulation And m_Aux.m_enSpotShift = enStockSpecific, False, True)
                            .ColHidden(RFC_SHIFT_FILE) = IIf(m_Aux.Simulation And m_Aux.m_enSpotShift = enStockSpecific, False, True)
                            .ColHidden(RFC_SIM_VOLA_SHIFT_TYPE) = IIf(.ColHidden(RFC_SIM_VOLA_SHIFT_TYPE), False, True)
                        End With
                    Case RFC_SIM_DATE
                        If dtPick.Value < Date Then
                            fgFlt.TextMatrix(1, RFC_SIM_DATE) = Date
                            m_Aux.SimulationDate = GetNewYorkTime
                        Else
                            fgFlt.TextMatrix(1, RFC_SIM_DATE) = dtPick.Value
                            m_Aux.SimulationDate = GetNewYorkTime + (dtPick.Value - Date)
                        End If
                    Case RFC_SIM_VOLA
                        m_Aux.SimulationVola = nValue
                    Case RFC_SIM_SPOT
                        m_Aux.SimulationSpot = nValue
                    Case RFC_SIM_TYPE
                        m_Aux.m_enSpotShift = nValue
                        
                        .ColHidden(RFC_SHIFT_ID) = IIf(m_Aux.m_enSpotShift = enStockSpecific, False, True)
                        .ColHidden(RFC_SHIFT_FILE) = IIf(m_Aux.m_enSpotShift = enStockSpecific, False, True)
                        
                        .ColHidden(RFC_SIM_DATE) = IIf(m_Aux.Simulation, False, True)
                        .ColHidden(RFC_SIM_VOLA) = IIf(m_Aux.Simulation, False, True)
                        .ColHidden(RFC_SIM_SPOT) = IIf(m_Aux.Simulation And m_Aux.m_enSpotShift <> enStockSpecific, False, True)
                        
                    Case RFC_SHIFT_ID
                        m_Aux.m_nShiftID = nValue
                        
                        If m_Aux.m_nShiftID = -1 Then ExportAllStockShifts
                        
                    Case RFC_SIM_VOLA_SHIFT_TYPE
                        m_Aux.m_bWeightedVolaShift = CBool(nValue)
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
            m_nMenuGridRows = .Rows
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
              
                mnuCtxTntCardNew_Click
            
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
            m_nMenuGridRows = .Rows
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
        If Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Not m_bSubscribingNow _
            And Col >= RFC_GROUP And Col <= RFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            
            If Not Cancel Then Cancel = (Col = RFC_VALUE And m_Aux.Filter(RFC_GROUP) = TYPE_ALL)
            If Not Cancel Then m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
        End If
    End With
    
    If Col = RFC_SIM Then
        fgFlt.TextMatrix(1, RFC_SIM_DATE) = Date
    ElseIf fgFlt.ColDataType(Col) = flexDTDate Then
        ' we'll handle the editing ourselves
        Cancel = True
        
        dtPick.Value = Date
        ' position date picker control over cell
        dtPick.Move fgFlt.CellLeft, fgFlt.CellTop, fgFlt.CellWidth, fgFlt.CellHeight
        
        ' initialize value, save original in tag in case user hits escape
        dtPick.Value = Date
        dtPick.Tag = fgFlt
        
        ' show and activate date picker control
        
        dtPick.Visible = True
        dtPick.SetFocus
        
        ' make it drop down the calendar
'        SendKeys "{f4}"
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Filter_StartEdit Exit", m_frmOwner.GetCaption
    
End Sub

Private Sub fgPos_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nKey&, sValue$, dValue#, bCalcUnd As Boolean, bCalcPos As Boolean, bCalcSynthUnd As Boolean
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, nUndRow&
    Dim aRowData As clsRvRowData, aSynthGreeks As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim lSurfaceID As Long
    
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
                
                Set aRowData = .RowData(Row)
                Set aUnd = aRowData.Und
                Set aPos = aRowData.Pos
                Set aSynthGreeks = aRowData.SynthGreeks
                
                Select Case nKey
                    Case RPC_BID
                        dValue = Abs(ReadDbl(sValue))
                        If Not aPos Is Nothing Then
                            If aPos.PriceBid <> dValue Then
                            If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Position PriceBid Changed " _
                                                                                        & "OldValue=""" & aPos.PriceBid & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                aPos.PriceBid = dValue
                                nUndRow = .GetNodeRow(Row, flexNTParent)
                                
                                Debug.Assert nUndRow > 0
                                
                                Select Case aPos.ContractType
                                    Case enCtOption, enCtFutOption
                                        bCalcPos = True
                                
                                    Case enCtFuture
                                        aPos.Fut.PriceBid = dValue
                                        bCalcUnd = True
                                        
                                    Case Else
                                        aUnd.PriceBid = aPos.PriceBid
                                        aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
                                        bCalcUnd = True
                                        
                                        If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.PriceBid = aUnd.PriceBid
                                        
                                End Select
                            Else
                                 If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Position PriceBid Wasn't Changed " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            End If
                            
                        ElseIf aSynthGreeks Is Nothing Then
                            If aUnd.PriceBid <> dValue Then
                                 If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceBid Changed " _
                                                                                        & "OldValue=""" & aUnd.PriceBid & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                           
                                aUnd.PriceBid = dValue
                                aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
                                bCalcUnd = True
                                nUndRow = Row
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.PriceBid = aUnd.PriceBid
                                If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.PriceBid = aUnd.PriceBid
                            Else
                              If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceBid Wasn't Changed " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            End If
                        Else
                            Set aUnd = m_Aux.Und(aSynthGreeks.SynthUndID)
                            If aUnd.PriceBid <> dValue Then
                            If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceBid Changed " _
                                                                                        & "OldValue=""" & aUnd.PriceBid & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                           
                                aUnd.PriceBid = dValue
                                aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
                                bCalcSynthUnd = True
                                
                                If Not aUnd.Pos Is Nothing And aUnd.Pos.Count > 0 Then
                                    nUndRow = .FindRow(CStr(aUnd.ID) & "_" & CStr(aUnd.ID), 1, RPC_KEY, , True)
                                    bCalcUnd = True
                                End If
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.PriceBid = aUnd.PriceBid
                                If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.PriceBid = aUnd.PriceBid
                            Else
                              If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceBid Wasn't Changed " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            End If
                            
                        End If
                        
                    Case RPC_ASK
                        dValue = ReadDbl(sValue)
                        If Not aPos Is Nothing Then
                            If aPos.PriceAsk <> dValue Then
                                If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Position PriceAsk Changed " _
                                                                                        & "OldValue=""" & aPos.PriceAsk & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                              
                                aPos.PriceAsk = dValue
                                nUndRow = .GetNodeRow(Row, flexNTParent)
                                
                                Debug.Assert nUndRow > 0
                                
                                Select Case aPos.ContractType
                                    Case enCtOption, enCtFutOption
                                        bCalcPos = True
                                
                                    Case enCtFuture
                                        aPos.Fut.PriceAsk = dValue
                                        bCalcUnd = True
                                        
                                    Case Else
                                        aUnd.PriceAsk = aPos.PriceAsk
                                        aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
                                        bCalcUnd = True
                                        
                                        If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.PriceAsk = aUnd.PriceAsk
                                        
                                End Select
                            Else
                              If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions  PriceAsk Wasn't Changed " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            End If
                            
                        ElseIf aSynthGreeks Is Nothing Then
                            If aUnd.PriceAsk <> dValue Then
                            If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceAsk Changed " _
                                                                                        & "OldValue=""" & aUnd.PriceAsk & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                              
                                aUnd.PriceAsk = dValue
                                aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
                                bCalcUnd = True
                                nUndRow = Row
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.PriceAsk = aUnd.PriceAsk
                                If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.PriceAsk = aUnd.PriceAsk
                            Else
                              If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceAsk Wasn't Changed " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            End If
                        Else
                            Set aUnd = m_Aux.Und(aSynthGreeks.SynthUndID)
                            If aUnd.PriceAsk <> dValue Then
                                If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceAsk Changed " _
                                                                                        & "OldValue=""" & aUnd.PriceAsk & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                              
                                aUnd.PriceAsk = dValue
                                aUnd.VolaSrv.UnderlyingPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
                                bCalcSynthUnd = True
                                
                                If Not aUnd.Pos Is Nothing And aUnd.Pos.Count > 0 Then
                                    nUndRow = .FindRow(CStr(aUnd.ID) & "_" & CStr(aUnd.ID), 1, RPC_KEY, , True)
                                    bCalcUnd = True
                                End If
                                
                                Set aPos = aUnd.Pos(aUnd.ID)
                                If Not aPos Is Nothing Then aPos.PriceAsk = aUnd.PriceAsk
                                If m_Aux.Idx.ID = aUnd.ID Then m_Aux.Idx.PriceAsk = aUnd.PriceAsk
                            Else
                              If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Positions Underlying PriceAsk Wasn't Changed " & GetOptionInfo, m_frmOwner.GetCaption
                           
                            End If
                        End If
                        
                    Case RPC_THEO_VOL
                        If Not aUnd Is Nothing And Not aPos Is Nothing Then
                            dValue = ReadDbl(sValue) / 100#
                            lSurfaceID = GetSurfaceByPos(aPos, aUnd)
                            aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID) = dValue
                            
                            m_bVolaChangedNow = True
                            g_Params.FireInternalVolatilityChanged aUnd.Symbol
                            m_bVolaChangedNow = False
                        
                            m_AuxClc.UnderlyingsCalc True, True
                            
                            
                            Dim nFirstRow, nLastRow, nRow As Long
                            nUndRow = .GetNodeRow(Row, flexNTParent)
                            nFirstRow = .GetNodeRow(nUndRow, flexNTFirstChild)
                            nLastRow = .GetNodeRow(nUndRow, flexNTLastChild)
                            
                            ' update only options with the same expiry and strike
                            Dim aRvData As clsRvRowData
                            For nRow = nFirstRow To nLastRow
                                Set aRvData = .RowData(nRow)
                                
                                If Not aRvData.Pos Is Nothing Then
                                    If aRvData.Pos.Expiry = aPos.Expiry And aRvData.Pos.Strike = aPos.Strike Then
                                        m_AuxOut.PositionUpdate nRow, False
                                    End If
                                End If
                            Next
                            
                            'm_AuxOut.PositionsUpdate nUndRow, False
                            m_AuxOut.UnderlyingUpdate nUndRow, False
                            m_AuxOut.TotalsUpdate
                            
                            Exit Sub
                        End If
                        
                End Select
    
                If bCalcUnd Or bCalcPos Or bCalcSynthUnd Then
                    If bCalcPos Then
                        m_AuxClc.CalcOptionGreeks aUnd, aPos, IIf(g_Params.UseTheoVolatility, GM_NONE, GM_ALL)
                        
                        m_AuxClc.UnderlyingsCalc False, False
                        
                        m_AuxOut.UnderlyingUpdate nUndRow, False
                        m_AuxOut.PositionUpdate Row, False
                        
                        If aUnd.HasSynthetic Then m_AuxOut.SyntheticUnderlyingGreeksUpdate aUnd, False
                        If aPos.IsSynthetic Then m_AuxOut.SyntheticPositionGreeksUpdate aUnd.ID, aPos, False
                    End If
                    
                    If bCalcUnd Then
                        m_AuxClc.UnderlyingCalc aUnd, True, True, False, False
                        
                        m_AuxOut.UnderlyingUpdate nUndRow, False
                        m_AuxOut.PositionsUpdate nUndRow, False
                        
                        If Not aUnd.SynthPos Is Nothing Then
                            If aUnd.SynthPos.Count Then
                                m_AuxClc.SyntheticUnderlyingCalc aUnd
                                m_AuxOut.SyntheticPositionsUpdate aUnd
                            End If
                        End If
                    End If
                    If bCalcSynthUnd Then
                        m_AuxClc.SyntheticUnderlyingCalc aUnd
                        m_AuxClc.UnderlyingsCalc True, True
                        m_AuxOut.SyntheticPositionsUpdate aUnd
                    End If
                    
                    m_AuxClc.UnderlyingsCalc True, True, True, True
                    m_AuxOut.TotalsUpdate
                End If
    
            End If
        End If
    End With
End Sub

Private Function GetSurfaceByPos(ByRef pPos As EtsMmRisksLib.MmRvPosAtom, ByRef pUnd As EtsMmRisksLib.MmRvUndAtom) As Long
On Error Resume Next
    Dim lSurfaceID As Long
    
    lSurfaceID = -1
    If (Not pPos Is Nothing And Not pUnd Is Nothing) Then
        lSurfaceID = pUnd.VolaSrv.GetSurfaceByRoot(0)
        If (pPos.ContractType = enCtOption) Then
            lSurfaceID = pUnd.VolaSrv.GetSurfaceByRoot(pPos.OptionRootID)
        ElseIf (pPos.ContractType = enCtFutOption) Then
            If (Not pPos.Fut Is Nothing) Then
                lSurfaceID = pUnd.VolaSrv.GetSurfaceByRoot(pPos.Fut.FutRootID)
            End If
        End If
    End If
    GetSurfaceByPos = lSurfaceID
    
End Function

Private Sub fgPos_AfterCollapse(ByVal Row As Long, ByVal State As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nRow&, aPos As EtsMmRisksLib.MmRvPosAtom, nFirstRow&, nLastRow&
    
    If State <> flexOutlineCollapsed Then
        With fgPos
            m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
            nFirstRow = .GetNodeRow(Row, flexNTFirstChild)
            nLastRow = .GetNodeRow(Row, flexNTLastChild)
            If nFirstRow > 0 And nLastRow > 0 Then
                For nRow = nFirstRow To nLastRow
                    Set aPos = .RowData(nRow).Pos
                    If Not aPos Is Nothing Then
                        .RowHidden(nRow) = Not aPos.Visible
                        Set aPos = Nothing
                    End If
                Next
            End If
            
            m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
        End With
    End If
    
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
    m_Aux.PosBeforeSort Col, Order
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
        m_nMenuGridRows = .Rows
        
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
        If .Col = 1 And .RowOutlineLevel(.Row) = ROL_UND Then
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
            m_nMenuGridRows = .Rows
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
              
                mnuCtxTntCardNew_Click
            
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
            m_nMenuGridRows = .Rows
            
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
    Cancel = True
    If m_bInProc Or m_Aux.RealTime Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub
    If IsDblClickHandled Then Exit Sub
    
    If m_Aux.Grp.ID <> 0 Then
        With fgPos
            nKey = .ColKey(Col)
            If Err.Number = 0 Then Cancel = Not m_gdPos.Col(nKey).CanEdit
            
            If nKey = RPC_THEO_VOL Then
                If Not fgPos.RowData(Row).Pos Is Nothing Then
                    If fgPos.RowData(Row).Pos.ContractType <> enCtOption And _
                                       fgPos.RowData(Row).Pos.ContractType <> enCtFutOption Then
                Cancel = True
                Exit Sub
            End If
                Else
                    Cancel = True
                    Exit Sub
                End If
            End If
            
            If Not Cancel Then
                If Not fgPos.RowData(Row).SynthGreeks Is Nothing Then
                    Cancel = Not (fgPos.RowData(Row).SynthGreeks.IsTotal And fgPos.RowData(Row).SynthGreeks.SynthUndID <> USD_ID)
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
            m_nMenuGridRows = .Rows
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
            
            m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
            m_Aux.FormatPosGrid
            m_AuxOut.UnderlyingsUpdate True
            m_Aux.FormatPosColumns
            m_AuxOut.UnderlyingsUpdateBadStatus
            m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    
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
    m_bInProc = False
End Sub

Private Sub m_frmStockShift_OnShiftsChanged(ByRef aShifts As Collection)
    On Error Resume Next
    
    Set m_Aux.m_StockShifts = Nothing
    Set m_Aux.m_StockShifts = aShifts
    
    m_Aux.GetUniqueStockShiftIDs
        
    fgFlt.ColComboList(RFC_SHIFT_ID) = "#-1;<All>|#0;<None>|"
        
    Dim i As Variant
    For Each i In m_Aux.m_UniqueStockShiftIDs
        fgFlt.ColComboList(RFC_SHIFT_ID) = fgFlt.ColComboList(RFC_SHIFT_ID) & "#" & CStr(i) & ";" & CStr(i) & "|"
    Next
    
    fgFlt.TextMatrix(1, RFC_SHIFT_ID) = m_Aux.m_nShiftID
    fgFlt.TextMatrix(1, RFC_SHIFT_FILE) = Mid(g_Params.StockShiftFile, InStrRev(g_Params.StockShiftFile, "\") + 1)
    
End Sub

Private Sub m_frmVarEditor_OnVariablesChanged(ByVal nNewID As Long)
    On Error Resume Next
    SaveGridDefAsDefault m_gdTot, GT_RISKS_TOTALS
    SaveGridDefAsDefault m_gdPos, GT_RISKS_POSITIONS
    
    InitGrids nNewID
    ShowData Group.GroupType, Group.ID
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

Private Sub mnuCtxEditCustomVariable_Click()
    On Error Resume Next
    Dim nLevel&, nColumnID&, nID
    Dim aRowData As clsRvRowData
    
    nID = BAD_LONG_VALUE
    
    Select Case m_enMenuGrid
        Case GT_RISKS_FILTER
            ' Impossible
            nLevel = BAD_LONG_VALUE
            nColumnID = BAD_LONG_VALUE

        Case GT_RISKS_TOTALS
            nColumnID = m_gdTot.Idx(m_nMenuGridCol)
            nID = m_Aux.m_cvGrpIDs(CStr(nColumnID))
            
            nLevel = m_Aux.m_nGrpLevelID

        Case GT_RISKS_POSITIONS
            nColumnID = m_gdPos.Idx(m_nMenuGridCol - 1)
            nID = m_Aux.m_cvOptIDs(CStr(nColumnID))
            
            Set aRowData = fgPos.RowData(m_nMenuGridRow)
            If aRowData.Pos Is Nothing Then
                nLevel = m_Aux.m_nUndLevelID
            Else
                nLevel = m_Aux.m_nOptLevelID
            End If
    End Select
    
    ' TODO: usage of level (1 - Grp, 2 - Und, 3 - Pos) & columnID -> nID
    m_frmVarEditor.EditVariable m_Aux.m_nContextID, nLevel, nID, m_frmOwner
    
End Sub

Private Sub mnuCtxFitTheoForAll_Click()
    On Error Resume Next
    Dim aPos As EtsMmRisksLib.MmRvPosAtom
    Dim aRowData As clsRvRowData
    Dim nRow As Long
    Dim lSurfaceID As Long
    
    Screen.MousePointer = vbHourglass
    
    nRow = fgPos.GetNodeRow(m_nMenuGridRow, flexNTRoot)
    nRow = fgPos.GetNodeRow(nRow, flexNTFirstSibling)
    
    While nRow <> -1
        Set aRowData = fgPos.RowData(nRow)
        
        For Each aPos In aRowData.Und.Pos
            If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
                lSurfaceID = GetSurfaceByPos(aPos, aRowData.Und)
                aRowData.Und.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID) = aPos.MIV
            End If
        Next
        
        m_bVolaChangedNow = True
        g_Params.FireInternalVolatilityChanged aRowData.Und.Symbol
        m_bVolaChangedNow = False
        
        nRow = fgPos.GetNodeRow(nRow, flexNTNextSibling)
    Wend
    
    m_AuxClc.UnderlyingsCalc True, True
    m_AuxOut.UnderlyingsUpdate False
    m_AuxOut.TotalsUpdate
    
    Screen.MousePointer = vbDefault

End Sub

Private Sub mnuCtxFitTheoForSelected_Click()
    On Error Resume Next

    Dim aRowData As clsRvRowData
    Dim nUndRow As Long
    Dim lSurfaceID As Long

    
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
    
        nUndRow = fgPos.GetNodeRow(m_nMenuGridRow, flexNTParent)
        Set aRowData = fgPos.RowData(m_nMenuGridRow)
        
        If Not aRowData Is Nothing Then
            If Not aRowData.Pos Is Nothing And Not aRowData.Und Is Nothing Then
                lSurfaceID = GetSurfaceByPos(aRowData.Pos, aRowData.Und)
                aRowData.Und.VolaSrv.OptionVola(aRowData.Pos.ExpiryOV, aRowData.Pos.Strike, lSurfaceID) = aRowData.Pos.MIV
                                    
                m_bVolaChangedNow = True
                g_Params.FireInternalVolatilityChanged aRowData.Und.Symbol
                m_bVolaChangedNow = False
                                
                m_AuxClc.UnderlyingsCalc True, True
                
                m_AuxOut.PositionUpdate m_nMenuGridRow, False
                If nUndRow > 0 Then m_AuxOut.UnderlyingUpdate nUndRow, False
                m_AuxOut.TotalsUpdate
            End If
        End If
        
                            
        m_bVolaChangedNow = True
        g_Params.FireInternalVolatilityChanged aRowData.Und.Symbol
        m_bVolaChangedNow = False
                        
        m_AuxClc.UnderlyingsCalc True, True
        
    End If
End Sub

Private Sub mnuCtxOtcOptionCalc_Click()
   If Not g_PerformanceLog Is Nothing Then _
                 g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> OTC Option Calc"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    CallOtcOptionCalcRV
End Sub

Private Sub mnuCtxTradeExercise_Click()
    On Error Resume Next
    If m_bShutDown Or m_bInProc Or m_enMenuGrid <> GT_RISKS_POSITIONS Then Exit Sub
    Dim aRowData As clsRvRowData, bIsSynth As Boolean
    Dim collTrades As New clsTradeExecColl, frmExecTrades As frmExerciseTrades
    
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
        Set aRowData = fgPos.RowData(m_nMenuGridRow)
        
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
    Dim aRowData As clsRvRowData
    Dim collTrades As New clsTradeExecColl, frmExecTrades As frmExerciseTrades
    
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
        Set aRowData = fgPos.RowData(m_nMenuGridRow)
        
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
    Dim aRowData As clsRvRowData, aCurSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    
    bBuy = True
    dPrice = 0#
    nQty = IIf(bIsStock, 100, 10)
    
    If m_enMenuGrid = GT_RISKS_POSITIONS Then
        Set aRowData = fgPos.RowData(m_nMenuGridRow)
        Set aCurUnd = aRowData.Und
        Set aCurPos = aRowData.Pos
        Set aCurSynthGreek = aRowData.SynthGreeks
        
        Set aUnd = g_Underlying(aCurUnd.ID)
        
        If Not aUnd Is Nothing Then
            If bIsStock Then
                If aCurSynthGreek Is Nothing Then
                    nQty = Round(aCurUnd.NetDelta / 100) * 100
                Else
                    Set aCurUnd = m_Aux.Und(aCurSynthGreek.SynthUndID)
                    nQty = Round(aCurSynthGreek.DeltaInShares / 100) * 100
                End If
                
                If nQty >= 0 Then
                    bBuy = False
                    dPrice = aCurUnd.PriceBid
                    If nQty = 0 Then nQty = 100
                    
                ElseIf nQty < 0 Then
                    nQty = Abs(nQty)
                    bBuy = True
                    dPrice = aCurUnd.PriceAsk
                End If
                
                If dPrice <= 0# Then dPrice = aCurUnd.PriceLast
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
                    
                        dPrice = aCurPos.PriceAsk
                        If dPrice <= 0# Then dPrice = aCurPos.PriceLast
                    End If
                End If
            End If
        End If
        
        Set aRowData = Nothing
        Set aCurUnd = Nothing
        Set aCurPos = Nothing
        Set aCurSynthGreek = Nothing
    End If
    
    If aUnd Is Nothing And m_Aux.Filter(RFC_GROUP) = TYPE_UNDERLYING Then _
        Set aUnd = g_Underlying(m_Aux.Filter(RFC_VALUE))
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, aOpt As EtsGeneralLib.EtsOptAtom, nQty&
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection bIsStock, aUnd, aOpt, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, bBuy, nQty, dPrice
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, aOpt, bBuy, nQty, dPrice
'    End If
'
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Risks View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
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

Private Sub mnuCtxTntCardNew_Click()
'    On Error Resume Next
'    If m_bShutDown Or m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, nQty&, aOpt As EtsGeneralLib.EtsOptAtom
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
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Risks View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error Resume Next
    If m_bShutDown Or m_bInProc Then Exit Sub
    Dim nUndID&, nID&, bBuy As Boolean, aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, nColIdx&
    Dim aRowData As clsRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom

    m_bInProc = True
    nUndID = 0
    nID = 0
    bBuy = True
    
    If m_enMenuGrid = GT_RISKS_POSITIONS Then
     If Not g_PerformanceLog Is Nothing Then _
                 g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> New Trade"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            Set aRowData = fgPos.RowData(m_nMenuGridRow)
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
                    nUndID = aUnd.ID
                    nID = nUndID
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

        If m_Aux.Filter(RFC_GROUP) = TYPE_UNDERLYING And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_Aux.Filter(RFC_VALUE)
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
    Dim aRowData As clsRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim sContractDesc$, aGUnd As EtsGeneralLib.UndAtom

    m_bInProc = True
    nUndID = 0
    nID = 0
    
    If m_enMenuGrid = GT_RISKS_POSITIONS Then
     If Not g_PerformanceLog Is Nothing Then _
                 g_PerformanceLog.LogMmInfo enLogUserAction, "Positions popup menu. ""RightClick -> New Position Transfer"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            Set aRowData = fgPos.RowData(m_nMenuGridRow)
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

        If m_Aux.Filter(RFC_GROUP) = TYPE_UNDERLYING And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_Aux.Filter(RFC_VALUE)
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
                        IIf(m_Aux.Filter(RFC_GROUP) = TYPE_TRADER, m_Aux.Filter(RFC_VALUE), 0), _
                        sContractDesc, m_frmOwner
    End If
    Exit Sub
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    ResetMenuData
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    If m_bShutDown Then Exit Sub
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_RISKS_FILTER
'            frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Name & " Risks Filter", "Risks Filter", fgFlt
'
'        Case GT_RISKS_TOTALS
'            frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Name & " Risks Totals", "Risks Totals", fgTot
'
'        Case GT_RISKS_POSITIONS
'            frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Name & " Risks Positions", "Risks Positions", fgPos
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxWtdVega_Click()
    On Error Resume Next
     If Not g_PerformanceLog Is Nothing Then _
                 g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Weighted Vega Settings"" selected. " & GetOptionInfo, m_frmOwner.GetCaption

    If Not m_bShutDown Then ShowWeightedVegaSettings
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim sKey$, aReq As EtsMmRisksLib.MmRvReqAtom, bFinished As Boolean, bCalcGreeks As Boolean
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, nTime&
    Dim nOperation&, nLogTime&
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogFaults, "PriceProvider_OnError: " & Description, m_frmOwner.GetCaption
   
    bFinished = False
    bCalcGreeks = False
    
    If ReqType = enSubscribeQuote Then
        If Not Request Is Nothing And m_bSubscribingNow Then
            If PpIsNonGroupReqType(Request.Type) Then
                sKey = CStr(PpInstrumentTypeContractType(Request.Type)) & "_" & Request.Symbol
                If Len(Request.Exchange) > 0 Then
                    sKey = sKey & "." & Request.Exchange
                End If
                
                If Not m_AuxClc.QuoteReqsAll(sKey) Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
                
                End If
                
                    m_nQuoteReqDone = m_nQuoteReqDone + 1
                    IncProgress pbProgress
                    
                    If m_bGroupRequest Then m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                    bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
                                Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
                
            ElseIf PpIsGroupReqType(Request.Type) Then
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
            End If
        
            If bFinished Then
                m_bSubscribingNow = False
                m_nQuoteReqDone = 0&
                m_nQuoteReqCount = 0&
                m_nQuoteGroupReqDone = 0&
                m_nQuoteGroupReqCount = 0&
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
            
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                lblProcess.Caption = ""
                imgStopDis.Visible = False
                imgStop.Visible = False
                
                m_Aux.RealTime = True
                m_Aux.RealTimeConnected = True
                If m_BatchPriceProvider.IsQuotesUpdated Or m_bVolaUpdated Then tmrRealTime.Enabled = True
                
                AdjustState
                AdjustCaption
            End If
        End If
    
    ElseIf ReqType = enRequestLastQuote Then
        If Not Request Is Nothing And m_bLastQuoteReqNow Then
            If PpIsNonGroupReqType(Request.Type) Then
                sKey = CStr(PpInstrumentTypeContractType(Request.Type)) & "_" & Request.Symbol
                
                Set aReq = m_AuxClc.QuoteReqsAll(sKey)
                If Not aReq Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
                
                End If
                    m_nQuoteReqDone = m_nQuoteReqDone + 1
                    IncProgress pbProgress
                    
                    If m_bGroupRequest Then m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                    bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
                                Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
                
            ElseIf PpIsGroupReqType(Request.Type) Then
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
            End If
            
            If bFinished Then
                
                CheckTradeQueue
                
                m_bLastQuoteReqNow = False
                bCalcGreeks = (m_nQuoteReqCount > 1)
                
                m_nQuoteReqDone = 0&
                m_nQuoteReqCount = 0&
                m_nQuoteGroupReqDone = 0&
                m_nQuoteGroupReqCount = 0&
                
                PriceProvider.CancelLastQuote
                If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
                
                pbProgress.Value = pbProgress.Max
                lblProcess.Caption = "Calculation..."
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
                
                nTime = timeGetTime
                If Not g_PerformanceLog Is Nothing Then _
                    m_nOperation = g_PerformanceLog.BeginLogMmOperation
                
                m_bInProc = True
                AdjustState
                
                If Not bCalcGreeks And Not aReq Is Nothing Then bCalcGreeks = Not aReq.Pos Is Nothing
                
                If bCalcGreeks Then
                   m_AuxClc.UnderlyingsCalc True, True
                Else
                   m_AuxClc.UnderlyingsCalc False, False
                End If
                
                If Not g_PerformanceLog Is Nothing Then _
                    nLogTime = g_PerformanceLog.CheckLogMmOperation(m_nOperation)
                    
                m_nLastRecalcCycle = timeGetTime - nTime
                nTime = timeGetTime
                If Not g_PerformanceLog Is Nothing Then _
                    nOperation = g_PerformanceLog.BeginLogMmOperation
                
                m_AuxOut.UnderlyingsUpdate False
                m_AuxOut.TotalsUpdate
                
                m_nLastOutCycle = timeGetTime - nTime

                If Not g_PerformanceLog Is Nothing Then
                    g_PerformanceLog.ContinueLogMmOperation m_nOperation, OPER_CALCULATION, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated, nLogTime
                    g_PerformanceLog.FinishLogMmOperation nOperation, OPER_REFRESHSCREEN, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated
                    g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_CALCREFRESHSCREEN, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated
                End If
        
                If g_Params.IsDebug Then UpdateStat
                
                m_bInProc = False
                AdjustState
                
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                imgStop.Visible = False
                imgStopDis.Visible = False
                
                RaiseEvent OnRefreshComplete
            End If
        End If
    
    Else
        Select Case ErrorNumber
            Case enProviderConnected
                m_Aux.RealTimeConnected = True
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogFaults, Description, m_frmOwner.GetCaption
                
                AdjustState
                
            Case enProviderConnecting
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogFaults, Description, m_frmOwner.GetCaption
                
            Case enConnectionWasDisconnected, enCouldNotConnectToProvider, enNotConnected
                m_Aux.RealTimeConnected = False
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogFaults, Description, m_frmOwner.GetCaption
                
                AdjustState
                
                
            Case Else
                LogEvent EVENT_ERROR, Description
        End Select
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider_OnLastQuote Started Symbol: " & Params.Symbol & ", Exchange:  " & Params.Exchange & ", Type:  " & Params.Type, m_frmOwner.GetCaption

    Dim sKey$, aReq As EtsMmRisksLib.MmRvReqAtom, dPriceBid#, dPriceAsk#, dPriceLast#, dNetChange#, nLotSize&
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, nTime&, aFut As EtsMmRisksLib.MmRvFutAtom
    Dim bFinished As Boolean, bCalcGreeks As Boolean
    Dim nOperation&, nLogTime&
    
    
    bFinished = False
    
    If PpIsNonGroupReqType(Params.Type) Then
        sKey = CStr(PpInstrumentTypeContractType(Params.Type)) & "_" & Params.Symbol
        Set aReq = m_AuxClc.QuoteReqsAll(sKey)
        If Not aReq Is Nothing Then
            dPriceBid = Results.BidPrice
            dPriceAsk = Results.AskPrice
            dPriceLast = Results.LastPrice
            dNetChange = Results.NetChange
            
            If Not aReq.IndexOnly Then
                If Not aReq.Pos Is Nothing Then
                    If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Pos.PriceBid = dPriceBid
                    If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Pos.PriceAsk = dPriceAsk
                    If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Pos.PriceLast = dPriceLast
                    If dNetChange <> BAD_DOUBLE_VALUE Then aReq.Pos.NetChange = dNetChange
                    
                    If Not aReq.Pos.OppositePos Is Nothing Then
                        If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Pos.OppositePos.OppPriceBid = dPriceBid
                        If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Pos.OppositePos.OppPriceAsk = dPriceAsk
                        If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Pos.OppositePos.OppPriceLast = dPriceLast
                    End If
                End If
                
                If Params.Type <> enOPT Then
                    If m_bGroupRequest Then m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                    
                    If Params.Type = enSTK Or Params.Type = enIDX Then
                        If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Und.PriceBid = dPriceBid
                        If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Und.PriceAsk = dPriceAsk
                        If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Und.PriceLast = dPriceLast
                        If dNetChange <> BAD_DOUBLE_VALUE Then aReq.Und.NetChange = dNetChange
                        
                        Debug.Assert (Not aReq.Und.UndPriceProfile Is Nothing)
                        aReq.Und.VolaSrv.UnderlyingPrice = aReq.Und.UndPriceProfile.GetUndPriceMid(aReq.Und.PriceBid, aReq.Und.PriceAsk, aReq.Und.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                    
                        If m_Aux.Idx.ID = aReq.Und.ID Then
                            If dPriceBid > BAD_DOUBLE_VALUE Then m_Aux.Idx.PriceBid = dPriceBid
                            If dPriceAsk > BAD_DOUBLE_VALUE Then m_Aux.Idx.PriceAsk = dPriceAsk
                            If dPriceLast > BAD_DOUBLE_VALUE Then m_Aux.Idx.PriceLast = dPriceLast
                            If dNetChange <> BAD_DOUBLE_VALUE Then m_Aux.Idx.NetChange = dNetChange
                        End If
                        
                    ElseIf Params.Type = enFUT Then
                        If Not aReq.Pos Is Nothing Then
                            Set aFut = aReq.Pos.Fut
                        ElseIf Not aReq.Fut Is Nothing Then
                            Set aFut = aReq.Fut
                        End If
                    
                        If Not aFut Is Nothing Then
                            If dPriceBid > BAD_DOUBLE_VALUE Then aFut.PriceBid = dPriceBid
                            If dPriceAsk > BAD_DOUBLE_VALUE Then aFut.PriceAsk = dPriceAsk
                            If dPriceLast > BAD_DOUBLE_VALUE Then aFut.PriceLast = dPriceLast
                            If dNetChange <> BAD_DOUBLE_VALUE Then aFut.NetChange = dNetChange

                            Set aFut = Nothing
                        End If
                    End If
                End If
            Else
                Debug.Assert m_Aux.Idx.ID = aReq.Und.ID
                If m_Aux.Idx.ID = aReq.Und.ID Then
                    If m_bGroupRequest Then m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                    
                    If dPriceBid > BAD_DOUBLE_VALUE Then m_Aux.Idx.PriceBid = dPriceBid
                    If dPriceAsk > BAD_DOUBLE_VALUE Then m_Aux.Idx.PriceAsk = dPriceAsk
                    If dPriceLast > BAD_DOUBLE_VALUE Then m_Aux.Idx.PriceLast = dPriceLast
                     If dNetChange <> BAD_DOUBLE_VALUE Then m_Aux.Idx.NetChange = dNetChange
                    
                End If
            End If
        
            If Params.Type = enSTK Or Params.Type = enIDX Then
                m_nUndResponses = m_nUndResponses + 1
                
            ElseIf Params.Type = enFUT Then
                m_nFutResponses = m_nFutResponses + 1
                
            Else
                m_nOptResponses = m_nOptResponses + 1
            End If
            
            m_nQuoteReqDone = m_nQuoteReqDone + 1
            IncProgress pbProgress
        
            bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
                        Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
        End If
        
    ElseIf PpIsGroupReqType(Params.Type) Then
        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
        bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
    End If

    If bFinished Then
        
        CheckTradeQueue
        
        m_bLastQuoteReqNow = False
        bCalcGreeks = (m_nQuoteReqCount > 1)
        
        m_nQuoteReqDone = 0&
        m_nQuoteReqCount = 0&
        m_nQuoteGroupReqDone = 0&
        m_nQuoteGroupReqCount = 0&
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
        
        pbProgress.Value = pbProgress.Max
        lblProcess.Caption = "Calculation..."
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
                
        nTime = timeGetTime
        If Not g_PerformanceLog Is Nothing Then _
            m_nOperation = g_PerformanceLog.BeginLogMmOperation
            
        m_bInProc = True
        AdjustState
    
        If Not bCalcGreeks And Not aReq Is Nothing Then bCalcGreeks = Not aReq.Pos Is Nothing
        
        If bCalcGreeks Then
            m_AuxClc.UnderlyingsCalc True, True
        Else
            m_AuxClc.UnderlyingsCalc False, False
        End If
        
        m_nLastRecalcCycle = timeGetTime - nTime
        
        If Not g_PerformanceLog Is Nothing Then _
            nLogTime = g_PerformanceLog.CheckLogMmOperation(m_nOperation)
            
        nTime = timeGetTime
        If Not g_PerformanceLog Is Nothing Then _
            nOperation = g_PerformanceLog.BeginLogMmOperation
                
        m_AuxOut.UnderlyingsUpdate False
        m_AuxOut.TotalsUpdate
        
        m_nLastOutCycle = timeGetTime - nTime

        If Not g_PerformanceLog Is Nothing Then
            g_PerformanceLog.ContinueLogMmOperation m_nOperation, OPER_CALCULATION, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated, nLogTime
            g_PerformanceLog.FinishLogMmOperation nOperation, OPER_REFRESHSCREEN, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_CALCREFRESHSCREEN, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated
        End If
        
        If g_Params.IsDebug Then UpdateStat
                
        m_bInProc = False
        AdjustState
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        
        RaiseEvent OnRefreshComplete
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider_OnLastQuote Finished Symbol: " & Params.Symbol & ", Exchange:  " & Params.Exchange & ", Type:  " & Params.Type, m_frmOwner.GetCaption
    
    Set aReq = Nothing
End Sub

Private Sub PriceProvider_OnSubscribed(Params As PRICEPROVIDERSLib.QuoteUpdateParams)
    On Error Resume Next
    If m_bShutDown Or Not m_bSubscribingNow Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider_OnSubscribed Started Symbol: " & Params.Symbol & ", Exchange:  " & Params.Exchange & ", Type:  " & Params.Type, m_frmOwner.GetCaption
    
    Dim bFinished As Boolean, sKey$
    
    If PpIsNonGroupReqType(Params.Type) Then
        sKey = CStr(PpInstrumentTypeContractType(Params.Type)) & "_" & Params.Symbol
        If Len(Params.Exchange) > 0 Then
            sKey = sKey & "." & Params.Exchange
        End If
        
        If Not m_AuxClc.QuoteReqsAll(sKey) Is Nothing Then
            If Params.Type <> enOPT And m_bGroupRequest Then
                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
            End If
            
            If Params.Type = enSTK Or Params.Type = enIDX Then
                m_nUndResponses = m_nUndResponses + 1
                
            ElseIf Params.Type = enFUT Then
                m_nFutResponses = m_nFutResponses + 1
                
            Else
                m_nOptResponses = m_nOptResponses + 1
            End If
            
            m_nQuoteReqDone = m_nQuoteReqDone + 1
            IncProgress pbProgress
            
            bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
                        Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
        End If
    ElseIf PpIsGroupReqType(Params.Type) Then
        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
        bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
    End If

    If bFinished Then
        m_bSubscribingNow = False
        m_nQuoteReqDone = 0&
        m_nQuoteReqCount = 0&
        m_nQuoteGroupReqDone = 0&
        m_nQuoteGroupReqCount = 0&
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
    
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
        
        m_Aux.RealTime = True
        m_Aux.RealTimeConnected = True
        If m_BatchPriceProvider.IsQuotesUpdated Or m_bVolaUpdated Then tmrRealTime.Enabled = True
        
        AdjustState
        AdjustCaption
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider_OnSubscribed Finished Symbol: " & Params.Symbol & ", Exchange:  " & Params.Exchange & ", Type:  " & Params.Type, m_frmOwner.GetCaption
End Sub

Private Sub PriceProvider_OnQuoteUpdate()
    On Error Resume Next
    'If m_bShutDown Or Not m_Aux.RealTime Then Exit Sub
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "OnQuoteUpdate", m_frmOwner.GetCaption
        
    If m_bShutDown Then
           If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Did not pass condition: m_bShutDown", m_frmOwner.GetCaption
        
            Exit Sub
    
    End If
    
    If Not m_Aux.RealTime Then
           If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Did not pass condition: Not m_Aux.RealTime", m_frmOwner.GetCaption
        
            Exit Sub
    
    End If
   
    If m_bInRealTimeCalc Then
           If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Did not pass condition: m_bInRealTimeCalc", m_frmOwner.GetCaption
        
            Exit Sub
    
    End If
   
    If m_bSubscribingNow Then
           If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Did not pass condition: m_bSubscribingNow", m_frmOwner.GetCaption
        
            Exit Sub
    
    End If
    
    If Not ProcessRealTime Then
           If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Did not pass condition: Not ProcessRealTime", m_frmOwner.GetCaption
        
            Exit Sub
    
    End If
    
    If m_BatchPriceProvider Is Nothing Then
           If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Did not pass condition: m_BatchPriceProvider Is Nothing", m_frmOwner.GetCaption
        
            Exit Sub
    End If
    
    If tmrRealTime.Enabled = True Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Update flag allready raised up", m_frmOwner.GetCaption
    Else
        tmrRealTime.Enabled = True
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Update flag raised up", m_frmOwner.GetCaption
    End If
   
End Sub

Private Sub PriceProvider_OnUnsubscribed()
    If Not m_bShutDown Then DoEvents
End Sub

Private Sub tmrRealTime_Timer()
    On Error Resume Next
    
    tmrRealTime.Enabled = False
    If Not m_bShutDown And m_Aux.RealTime Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "tmrRealTime_Timer: Start RealTimeQuotesUpdate", m_frmOwner.GetCaption
        RealTimeQuotesUpdate
    Else
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "tmrRealTime_Timer: Risks are not in realtime mode", m_frmOwner.GetCaption
    
    End If
        
End Sub

Private Sub RealTimeQuotesUpdate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Real Time Quotes Update Enter", m_frmOwner.GetCaption
    
    If Not m_Aux.RealTime Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: not in Realtime mode", m_frmOwner.GetCaption
        Exit Sub
    End If
    
    If m_bInRealTimeCalc Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: in realtime calc", m_frmOwner.GetCaption
        Exit Sub
    End If
    
    If m_bTmrUndCalcNow Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: in tmr underlying calc", m_frmOwner.GetCaption
        Exit Sub
    End If
    
    If m_bSubscribingNow Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: in realtime subscribing", m_frmOwner.GetCaption
        Exit Sub
    End If
    If (Not ProcessRealTime) And (Not m_bVolaUpdated) Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: not Process Realtime", m_frmOwner.GetCaption
        Exit Sub
    End If
    If m_BatchPriceProvider Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: m_BatchPriceProvider Is Nothing", m_frmOwner.GetCaption
        Exit Sub
    End If

    
    m_bInRealTimeCalc = True
    
    
    Dim sKey$, aReq As EtsMmRisksLib.MmRvReqAtom, nRow&, nUndRow&, dPriceBid#, dPriceAsk#, dPriceLast#, dNetChange#
    Dim Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo
    Dim Infos() As PRICEPROVIDERSLib.QuoteUpdateFullInfo, nLBound&, nUBound&, i&, nTime&
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, nOpt&, nUnd&, aFut As EtsMmRisksLib.MmRvFutAtom, aSynthUnd As EtsMmRisksLib.MmRvUndAtom
    Dim nOperation&: nOperation = 0
    Dim nOperation2&: nOperation2 = 0
    Dim nUnds&: nUnds = 0
    Dim nOpts&: nOpts = 0
    Dim nFuts&: nFuts = 0
    Dim nLogTime&, aPos As EtsMmRisksLib.MmRvPosAtom, aSyntGreek As EtsMmRisksLib.MmRvSynthGreeksAtom, bOneOfPricesUpdated As Boolean
    
    bOneOfPricesUpdated = False
    If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Get Quotes Update", m_frmOwner.GetCaption
    
    Infos = m_BatchPriceProvider.GetQuotesUpdates
    
    If Err Then
        If Not g_PerformanceLog Is Nothing Then _
               g_PerformanceLog.LogMmInfo enLogSystem, "Get Quotes Update exits with error " & CStr(Err.Number) & " " & Err.Description, m_frmOwner.GetCaption
    Else
        If Not g_PerformanceLog Is Nothing Then _
               g_PerformanceLog.LogMmInfo enLogEnhDebug, "Got Quotes Update " & CStr(UBound(Infos) - LBound(Infos) + 1), m_frmOwner.GetCaption
       End If
    
    nLBound = 0: nUBound = -1
    nLBound = LBound(Infos)
    nUBound = UBound(Infos)
        
    ' Update quote data
    m_nLastDataSize = nUBound - nLBound + 1
    
    For i = nLBound To nUBound
    
        
        Params = Infos(i).Params
        Results = Infos(i).Info
        
     
       If Not g_PerformanceLog Is Nothing Then _
         g_PerformanceLog.LogMmInfo enLogEnhDebug, "Updating quote data, (Symbol: " & Params.Symbol & ", Exchange: " & Params.Exchange & ", Type: " & Params.Type & ")", m_frmOwner.GetCaption
    
        sKey = CStr(PpInstrumentTypeContractType(Params.Type)) & "_" & Params.Symbol
        Set aReq = m_AuxClc.QuoteReqsAll(sKey)
        If Not aReq Is Nothing Then
            dPriceBid = Results.BidPrice
            dPriceAsk = Results.AskPrice
            dPriceLast = Results.LastPrice
            dNetChange = Results.NetChange
            
           If Not g_PerformanceLog Is Nothing Then _
             g_PerformanceLog.LogMmInfo enLogEnhDebug, "Results ( Bid: " & CStr(dPriceBid) & ", Ask: " & CStr(dPriceAsk) & ", Last: " & CStr(dPriceLast) & " )", m_frmOwner.GetCaption
            
            If Not aReq.IndexOnly Then
                If Not aReq.Pos Is Nothing Then
                    If dNetChange <> BAD_DOUBLE_VALUE Then aReq.Pos.NetChange = dNetChange
                    If dPriceBid > BAD_DOUBLE_VALUE And aReq.Pos.PriceBid <> dPriceBid Then
                        aReq.Pos.PriceBid = dPriceBid
                        If Not aReq.Pos.OppositePos Is Nothing Then aReq.Pos.OppositePos.OppPriceBid = dPriceBid
                        aReq.Pos.CalcGreeks = True
                        aReq.Und.CalcTotals = True
                        bOneOfPricesUpdated = True
                        aReq.Und.IsDirty = True
                        aReq.Pos.IsDirty = True
                    End If
                    If dPriceAsk > BAD_DOUBLE_VALUE And aReq.Pos.PriceAsk <> dPriceAsk Then
                        aReq.Pos.PriceAsk = dPriceAsk
                        If Not aReq.Pos.OppositePos Is Nothing Then aReq.Pos.OppositePos.OppPriceAsk = dPriceAsk
                        aReq.Pos.CalcGreeks = True
                        aReq.Und.CalcTotals = True
                        bOneOfPricesUpdated = True
                        aReq.Und.IsDirty = True
                        aReq.Pos.IsDirty = True
                    End If
                    If dPriceLast > BAD_DOUBLE_VALUE And aReq.Pos.PriceLast <> dPriceLast Then
                        aReq.Pos.PriceLast = dPriceLast
                        If Not aReq.Pos.OppositePos Is Nothing Then aReq.Pos.OppositePos.OppPriceLast = dPriceLast
                        aReq.Pos.CalcGreeks = True
                        aReq.Und.CalcTotals = True
                        bOneOfPricesUpdated = True
                        aReq.Und.IsDirty = True
                        aReq.Pos.IsDirty = True
                    End If
                End If
                
                If Params.Type <> enOPT Then
                    If Params.Type = enSTK Or Params.Type = enIDX Then
                        If dNetChange <> BAD_DOUBLE_VALUE Then aReq.Und.NetChange = dNetChange
                        
                        If dPriceBid > BAD_DOUBLE_VALUE And aReq.Und.PriceBid <> dPriceBid Then
                            aReq.Und.PriceBid = dPriceBid
                            aReq.Und.CalcGreeks = True
                            aReq.Und.CalcTotals = True
                            bOneOfPricesUpdated = True
                            aReq.Und.IsDirty = True
                            aReq.Pos.IsDirty = True
                        End If
                        If dPriceAsk > BAD_DOUBLE_VALUE And aReq.Und.PriceAsk <> dPriceAsk Then
                            aReq.Und.PriceAsk = dPriceAsk
                            aReq.Und.CalcGreeks = True
                            aReq.Und.CalcTotals = True
                            bOneOfPricesUpdated = True
                            aReq.Und.IsDirty = True
                            aReq.Pos.IsDirty = True
                        End If
                        If dPriceLast > BAD_DOUBLE_VALUE And aReq.Und.PriceLast <> dPriceLast Then
                            aReq.Und.PriceLast = dPriceLast
                            aReq.Und.CalcGreeks = True
                            aReq.Und.CalcTotals = True
                            bOneOfPricesUpdated = True
                            aReq.Und.IsDirty = True
                            aReq.Pos.IsDirty = True
                        End If
                                        
                        If bOneOfPricesUpdated And Not aReq.Und.SynthPos Is Nothing Then
                            For Each aPos In aReq.Und.SynthPos
                                aPos.CalcGreeks = True
                                Set aSynthUnd = m_Aux.Und(aPos.UndID)
                                
                                Debug.Assert Not aSynthUnd Is Nothing
                                
                                aSynthUnd.CalcTotals = True
                                Set aSynthUnd = Nothing
                            Next
                        End If
                        
                        If m_Aux.Idx.ID = aReq.Und.ID Then
                            If dNetChange <> BAD_DOUBLE_VALUE Then m_Aux.Idx.NetChange = dNetChange
                            
                            If dPriceBid > BAD_DOUBLE_VALUE And m_Aux.Idx.PriceBid <> dPriceBid Then
                                m_Aux.Idx.PriceBid = dPriceBid
                                m_Aux.Idx.CalcTotals = True
                                bOneOfPricesUpdated = True
                            End If
                            If dPriceAsk > BAD_DOUBLE_VALUE And m_Aux.Idx.PriceAsk <> dPriceAsk Then
                                m_Aux.Idx.PriceAsk = dPriceAsk
                                m_Aux.Idx.CalcTotals = True
                                bOneOfPricesUpdated = True
                            End If
                            If dPriceLast > BAD_DOUBLE_VALUE And m_Aux.Idx.PriceLast <> dPriceLast Then
                                m_Aux.Idx.PriceLast = dPriceLast
                                m_Aux.Idx.CalcTotals = True
                                bOneOfPricesUpdated = True
                            End If
                        End If
                        
                        nUnds = nUnds + 1
                    
                    ElseIf Params.Type = enFUT Then
                        If Not aReq.Pos Is Nothing Then
                            Set aFut = aReq.Pos.Fut
                        ElseIf Not aReq.Fut Is Nothing Then
                            Set aFut = aReq.Fut
                        End If
                        If dNetChange <> BAD_DOUBLE_VALUE Then aFut.NetChange = dNetChange
                   
                        If Not aFut Is Nothing Then
                            If dPriceBid > BAD_DOUBLE_VALUE And aFut.PriceBid <> dPriceBid Then
                                aFut.PriceBid = dPriceBid
                                aFut.CalcGreeks = True
                                aReq.Und.CalcTotals = True
                                bOneOfPricesUpdated = True
                                aReq.Und.IsDirty = True
                                aReq.Pos.IsDirty = True
                            End If
                            If dPriceAsk > BAD_DOUBLE_VALUE And aFut.PriceAsk <> dPriceAsk Then
                                aFut.PriceAsk = dPriceAsk
                                aFut.CalcGreeks = True
                                aReq.Und.CalcTotals = True
                                bOneOfPricesUpdated = True
                                aReq.Und.IsDirty = True
                                aReq.Pos.IsDirty = True
                            End If
                            If dPriceLast > BAD_DOUBLE_VALUE And aFut.PriceLast <> dPriceLast Then
                                aFut.PriceLast = dPriceLast
                                aFut.CalcGreeks = True
                                aReq.Und.CalcTotals = True
                                bOneOfPricesUpdated = True
                                aReq.Und.IsDirty = True
                                aReq.Pos.IsDirty = True
                            End If
                            
                            Set aFut = Nothing
                            nFuts = nFuts + 1
                        End If
                    End If
                Else
                    nOpts = nOpts + 1
                End If
            Else
                Debug.Assert m_Aux.Idx.ID = aReq.Und.ID
                If dNetChange <> BAD_DOUBLE_VALUE Then m_Aux.Idx.NetChange = dNetChange
               
                If m_Aux.Idx.ID = aReq.Und.ID Then
                    If dPriceBid > BAD_DOUBLE_VALUE And m_Aux.Idx.PriceBid <> dPriceBid Then
                        m_Aux.Idx.PriceBid = dPriceBid
                        m_Aux.Idx.CalcTotals = True
                        bOneOfPricesUpdated = True
                    End If
                    If dPriceAsk > BAD_DOUBLE_VALUE And m_Aux.Idx.PriceAsk <> dPriceAsk Then
                        m_Aux.Idx.PriceAsk = dPriceAsk
                        m_Aux.Idx.CalcTotals = True
                        bOneOfPricesUpdated = True
                    End If
                    If dPriceLast > BAD_DOUBLE_VALUE And m_Aux.Idx.PriceLast <> dPriceLast Then
                        m_Aux.Idx.PriceLast = dPriceLast
                        m_Aux.Idx.CalcTotals = True
                        bOneOfPricesUpdated = True
                    End If
                End If
                
                nUnds = nUnds + 1
            End If
    
            Set aReq = Nothing
        End If
        
        DoEvents
        
        If m_bShutDown Then Exit Sub
    Next
    
    If m_bVolaUpdated Then
        m_bVolaUpdated = False
        bOneOfPricesUpdated = True
        For Each aUnd In m_Aux.Und
            If aUnd.VolaUpdated Then
                aUnd.CalcGreeks = True
                aUnd.CalcTotals = True
            End If
        Next
    End If

    If bOneOfPricesUpdated Then
        If Not g_PerformanceLog Is Nothing And (nUnds > 0 Or nOpts > 0) Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quotes Updated", m_frmOwner.GetCaption, nUnds, nOpts, 0
    
        nTime = timeGetTime
        
        If m_bShutDown Then Exit Sub
        
        If Not g_PerformanceLog Is Nothing Then _
            nOperation = g_PerformanceLog.BeginLogMmOperation
            
        If (ProcessRealTime) Then
            m_AuxClc.UnderlyingsCalc True, True, True, , True
        Else
            m_bVolaUpdated = False
            For Each aUnd In m_Aux.Und
                If aUnd.VolaUpdated Then
                    aUnd.CalcGreeks = True
                    aUnd.CalcTotals = True
                    CalculateRiskViewUnderlying aUnd
                    aUnd.VolaUpdated = False
                End If
            Next
            m_AuxOut.UnderlyingsUpdate True, True
        End If
        
        If nOperation > 0 And Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.ContinueLogMmOperation nOperation, OPER_CALCULATION, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated
        
        m_nLastRecalcCycle = timeGetTime - nTime
                                
        ' Update grid data
        nTime = timeGetTime
                    
        If m_bShutDown Then Exit Sub
           
        If Not g_PerformanceLog Is Nothing Then _
            nOperation2 = g_PerformanceLog.BeginLogMmOperation

        m_AuxOut.UnderlyingsUpdate True, True
        m_AuxOut.TotalsUpdate
    
        m_nLastOutCycle = timeGetTime - nTime
                            
        If Not g_PerformanceLog Is Nothing Then
            If nOperation2 > 0 Then _
                g_PerformanceLog.FinishLogMmOperation nOperation2, OPER_REFRESHSCREEN, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated
            If nOperation > 0 Then _
                g_PerformanceLog.FinishLogMmOperation nOperation, OPER_CALCREFRESHSCREEN, m_frmOwner.GetCaption, m_AuxClc.UndUpdated, m_AuxClc.OptUpdated, m_AuxClc.FutUpdated
        End If
            
        If g_Params.IsDebug Then UpdateStat
    End If
    
    m_bInRealTimeCalc = False

    If m_bShutDown Or Not m_Aux.RealTime Or m_bSubscribingNow Or Not ProcessRealTime Or m_BatchPriceProvider Is Nothing Then
           If Not g_PerformanceLog Is Nothing Then g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quotes Updated, RT Stopped", m_frmOwner.GetCaption
           Exit Sub
    End If
    If Not g_PerformanceLog Is Nothing Then _
          g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quotes Updated, RT Enabled", m_frmOwner.GetCaption
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
    Dim nOperation&
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Risk Refresh Started.", m_frmOwner.GetCaption
        
    tmrShow.Interval = 10
    tmrShow.Enabled = False
    tmrUndCalc.Enabled = False
    
    If m_bShutDown Or m_bDataLoad Then Exit Sub
    m_bInProc = True
    
    If m_Aux.Grp.ID <> 0 And Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then
            PriceProvider.CancelLastQuote
            If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
        End If
                
        If m_Aux.RealTime Or m_bSubscribingNow Then
            PriceProvider.UnSubscribeQuote
            If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes
        End If
        
        m_bLastQuoteReqNow = False
        m_bSubscribingNow = False
        m_Aux.RealTime = False
        
        If m_bInRealTimeCalc Or m_bTmrUndCalcNow Then
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "FAIL: in calculation mode now.", m_frmOwner.GetCaption
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
    
    If m_Aux.Filter(RFC_GROUP) <> 0 And m_Aux.Filter(RFC_VALUE) <> 0 Then
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
            PositionsShow
        
            If g_Params.RiskDirectlyToRealtime Then
                StartRealTime
            Else
                RequestLastQuotes
            End If
        Else
            If m_Aux.Filter(RFC_GROUP) = TYPE_ALL Then m_Aux.Filter(RFC_GROUP) = TYPE_UNDERLYING
            m_Aux.Filter(RFC_VALUE) = 0
            ClearViewAndData
            m_Aux.FilterUpdateAll
            AdjustCaption
            AdjustState
            
            imgStop.Visible = False
            imgStopDis.Visible = False
            pbProgress.Visible = False
            lblProcess.Visible = False
            lblStatus.Visible = True
        End If
    
    End If
    
    m_nOpenedExpiry = 0
    
    Screen.MousePointer = vbDefault

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Risk Refresh Finished.", m_frmOwner.GetCaption
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation nOperation, OPER_LOADDATA, m_frmOwner.GetCaption, m_AuxClc.UndCount, m_AuxClc.OptCount, m_AuxClc.FutCount
End Sub

Private Sub PositionsShow()
    On Error Resume Next
    Dim nCol&, aPos As EtsMmRisksLib.MmRvPosAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim nRow&, nUndRow&, bHide As Boolean, aRowData As clsRvRowData
    Dim bShow As Boolean
    
    m_nUndPositions = 0
    m_nOptPositions = 0

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        .Rows = 1
        
        For Each aUnd In m_Aux.Und
            If aUnd.Pos.Count > 0 Then
            
                .AddItem ""
                
                Set aRowData = New clsRvRowData
                Set aRowData.Pos = Nothing
                Set aRowData.Und = aUnd
                Set aRowData.SynthGreeks = Nothing
                Set aRowData.SimSynthGreeks = Nothing
                
                nUndRow = .Rows - 1
                .RowData(nUndRow) = aRowData
                
                .TextMatrix(nUndRow, RPC_KEY) = CStr(aUnd.ID) & "_" & CStr(aUnd.ID)
                .IsSubtotal(nUndRow) = True
                .RowOutlineLevel(nUndRow) = ROL_UND
                
                Set aRowData = Nothing

                bHide = m_OpenedUnd(CStr(aUnd.ID)) Is Nothing
                bShow = False
                
                If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aUnd.SynthGreeks
                        Set aRowData = New clsRvRowData
                        Set aRowData.Pos = Nothing
                        Set aRowData.Und = aUnd
                        Set aRowData.SynthGreeks = aSynthGreek
                        Set aRowData.SimSynthGreeks = aUnd.SimulationUnd.SynthGreeks(aSynthGreek.SynthUndID)
                        aSynthGreek.Visible = Not bHide
                        aRowData.SimSynthGreeks.Visible = aSynthGreek.Visible
                        bShow = aSynthGreek.Visible
                        
                        .AddItem ""
                        nRow = .Rows - 1
                        .RowData(nRow) = aRowData
                        Set aRowData = Nothing
                        .TextMatrix(nRow, RPC_KEY) = CStr(aUnd.ID) & "_" & CStr(aSynthGreek.SynthUndID)
                        .RowOutlineLevel(nRow) = ROL_POS
                        
                        m_AuxOut.SyntheticGreeksUpdate nRow, True
                    Next
                End If
                
                For Each aPos In aUnd.Pos
                    Set aRowData = New clsRvRowData
                    Set aRowData.Pos = aPos
                    Set aRowData.Und = aUnd
                    Set aRowData.SynthGreeks = Nothing
                    Set aRowData.SimSynthGreeks = Nothing
                    .AddItem ""
                    nRow = .Rows - 1
                    .RowData(nRow) = aRowData
                    Set aRowData = Nothing
                    .TextMatrix(nRow, RPC_KEY) = CStr(aUnd.ID) & "_" & CStr(aUnd.ID) & "_" & CStr(aPos.ID)
                    .RowOutlineLevel(nRow) = ROL_POS

                    aPos.Visible = m_Aux.CheckPosFilter(aPos)
                    
                    If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then _
                        m_nOptPositions = m_nOptPositions + IIf(aPos.Visible, 1, 0)
                        
                    If aPos.ContractType = enCtStock Or aPos.ContractType = enCtIndex Or aPos.ContractType = enCtFuture Then _
                        m_nUndPositions = m_nUndPositions + IIf(aPos.Visible, 1, 0)
                   
                    If bShow = False Then bShow = aPos.Visible
                    
                    .RowHidden(nRow) = Not aPos.Visible
                    
                    m_AuxOut.PositionUpdate nRow, True
                    
                    If aPos.IsSynthetic And Not aPos.SynthGreeks Is Nothing Then
                        For Each aSynthGreek In aPos.SynthGreeks
                            Set aRowData = New clsRvRowData
                            Set aRowData.Pos = aPos
                            Set aRowData.Und = aUnd
                            Set aRowData.SynthGreeks = aSynthGreek
                            Set aRowData.SimSynthGreeks = aPos.SimulationPos.SynthGreeks(aSynthGreek.SynthUndID)
                            aSynthGreek.Visible = aPos.Visible
                            aRowData.SimSynthGreeks.Visible = aSynthGreek.Visible
                            
                            .AddItem ""
                            nRow = .Rows - 1
                            .RowData(nRow) = aRowData
                            Set aRowData = Nothing
                            .TextMatrix(nRow, RPC_KEY) = CStr(aUnd.ID) & "_" & CStr(aSynthGreek.SynthUndID) & "_" & CStr(aPos.ID)
                            .RowOutlineLevel(nRow) = ROL_POS
                            
                            aSynthGreek.Visible = aPos.Visible
                            
                            .RowHidden(nRow) = Not aSynthGreek.Visible
                            m_AuxOut.SyntheticGreeksUpdate nRow, True
                        Next
                    End If
                    
                    DoEvents
                    If m_bShutDown Then Exit Sub
                Next
            
                m_AuxOut.UnderlyingUpdate nUndRow, True
                
                DoEvents
                If m_bShutDown Then Exit Sub
            End If

            If bHide Then
            .IsCollapsed(nUndRow) = flexOutlineCollapsed
            .RowHidden(nUndRow) = Not bShow
            End If
        Next
        
        m_OpenedUnd.Clear
        
        lblProcess.Caption = "Formatting..."
        lblProcess.Refresh
        
        nCol = -1
        nCol = .ColIndex(RPC_SYMBOL)
        If nCol >= 0 Then
            m_Aux.SortPosNodes nCol, True
            .Col = nCol
            .Row = 1
            m_Aux.SortColKey = RPC_SYMBOL
            .Sort = flexSortCustom
        End If
        
        gePos.ShowSortImage nCol, 1
        
        m_Aux.FormatPosColumns
        
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Private Sub PositionsApplyFilter()
    On Error Resume Next
    Dim nRow&
    Dim bHide As Boolean
    Dim aRowData As clsRvRowData
    Dim nRowSave&
    Dim bHasToshow As Boolean
    m_nUndPositions = 0
    m_nOptPositions = 0
    
    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        Screen.MousePointer = vbHourglass
        imgStop.Visible = False
        imgStopDis.Visible = True
        lblStatus.Visible = False
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Visible = True
        lblProcess.Caption = "Filter applying..."
        lblProcess.Visible = True
        lblProcess.Refresh
        
        pbProgress.Max = .Rows - 1
        bHide = False
        
        For nRow = 1 To .Rows - 1
            Set aRowData = .RowData(nRow)
            If Not aRowData.Pos Is Nothing Then
                aRowData.Pos.Visible = m_Aux.CheckPosFilter(aRowData.Pos)
                
                If aRowData.Pos.ContractType = enCtOption Or aRowData.Pos.ContractType = enCtFutOption Then _
                    m_nOptPositions = m_nOptPositions + IIf(aRowData.Pos.Visible, 1, 0)
                        
                If aRowData.Pos.ContractType = enCtStock Or aRowData.Pos.ContractType = enCtIndex Or aRowData.Pos.ContractType = enCtFuture Then _
                    m_nUndPositions = m_nUndPositions + IIf(aRowData.Pos.Visible, 1, 0)
                    
                If bHide Then .RowHidden(nRow) = Not aRowData.Pos.Visible
                If bHasToshow = False Then bHasToshow = aRowData.Pos.Visible
            Else
                If aRowData.SynthGreeks Is Nothing Then
                    bHide = (.IsCollapsed(nRow) <> flexOutlineCollapsed)
                    .RowHidden(nRow) = False
                    If bHasToshow = False And nRowSave <> 0 Then
                    .RowHidden(nRowSave) = True
                    End If
                    nRowSave = nRow
                    bHasToshow = False
                End If
            End If
            Set aRowData = Nothing
            IncProgress pbProgress
        Next
        If bHasToshow = False And nRowSave <> 0 Then
           .RowHidden(nRowSave) = True
        End If
        
        lblProcess.Caption = "Formatting..."
        lblProcess.Refresh
        
        imgStop.Visible = False
        imgStopDis.Visible = False
        pbProgress.Visible = False
        lblProcess.Visible = False
        lblStatus.Visible = True
        Screen.MousePointer = vbDefault
        
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
        AdjustCaption
    End With
End Sub

Private Sub RequestLastQuotes()
    On Error GoTo EH
    If m_bShutDown Or m_bInProc Or m_bSubscribingNow Then Exit Sub
    Dim aIdxReq As EtsMmRisksLib.MmRvReqAtom, aReq As EtsMmRisksLib.MmRvReqAtom
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aArgParam As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim aBugCounter As Long
    Dim nLoopCounter As Long
    
    aBugCounter = 0
    If Not PriceProvider Is Nothing And Not m_BatchPriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow
        
        m_nUndResponses = 0
        m_nOptResponses = 0
        m_nFutResponses = 0
        
        aBugCounter = 1
        If m_AuxClc.QuoteReqsAll.Count > 0 Then
            m_bGroupRequest = m_bGroupRequestSupported And Not g_Params.RiskReqTypeAlwaysNonGroup
            
            aBugCounter = 2
            If Not g_PerformanceLog Is Nothing Then _
                m_nOperation = g_PerformanceLog.BeginLogMmOperation
            
            m_bInProc = True
            m_bLastQuoteReqNow = True
            aBugCounter = 3
            AdjustState
            
            m_nQuoteReqCount = m_AuxClc.QuoteReqsAll.Count
            m_nQuoteReqDone = 0&
            
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = m_nQuoteReqCount
            
            pbProgress.Visible = True
        
            lblStatus.Visible = False
            lblProcess.Visible = True
            lblProcess.Caption = "Request last quotes..."
            lblProcess.Refresh
            imgStop.Visible = True
            imgStopDis.Visible = False
            
            aBugCounter = 4
            If Not m_bGroupRequest Then
                m_nQuoteGroupReqCount = 0&
                m_nQuoteGroupReqDone = 0&
                
                On Error Resume Next
                
                aBugCounter = 5
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogEnhDebug, "Try ctlRiskView.RequestLastQuotes (Start mutliple request list). BugCounter Value: " & CStr(aBugCounter), m_frmOwner.GetCaption
                    
                Err.Clear
                Dim aQuoteUpdateReqs() As PRICEPROVIDERSLib.QuoteUpdateParams
                ReDim aQuoteUpdateReqs(m_AuxClc.QuoteReqsAll.Count - 1)
                
                If Err Then
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogFaults, "Can't ReDim array, count " & CStr(m_AuxClc.QuoteReqsAll.Count), m_frmOwner.GetCaption
                    GoTo EH
                Else
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogFaults, "Success ReDim array, count " & CStr(m_AuxClc.QuoteReqsAll.Count), m_frmOwner.GetCaption
                End If
                
                aBugCounter = 6
                nLoopCounter = 0
                For Each aReq In m_AuxClc.QuoteReqsAll
                
                    Err.Clear
                    aArgParam = aReq.GetQuoteUpdateParam
                    
                    If Err Then
                        If Not g_PerformanceLog Is Nothing Then _
                            g_PerformanceLog.LogMmInfo enLogFaults, "Returned error on get params (" & Err.Description & ")  (Symbol: " & aReq.GetQuoteUpdateParam.Symbol & ", Exchange: " & aReq.GetQuoteUpdateParam.Exchange & ", Type: " & aReq.GetQuoteUpdateParam.Type & ")", m_frmOwner.GetCaption
                    Else
                        If Not g_PerformanceLog Is Nothing Then _
                            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Success get params (Symbol: " & aReq.GetQuoteUpdateParam.Symbol & ", Exchange: " & aReq.GetQuoteUpdateParam.Exchange & ", Type: " & aReq.GetQuoteUpdateParam.Type & ")", m_frmOwner.GetCaption
                           
                        aQuoteUpdateReqs(nLoopCounter) = aArgParam
                    End If
                    
                    nLoopCounter = nLoopCounter + 1
                Next

                aBugCounter = 7
                If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit Sub
                
                Err.Clear
                m_BatchPriceProvider.RequestMultipleQuotes aQuoteUpdateReqs
                nLoopCounter = 1

                While Err.Number = 438 And nLoopCounter < 10
                    Err.Clear
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogFaults, "Can't start request in " & CStr(nLoopCounter) & " attempt. Bug counter " & CStr(aBugCounter), m_frmOwner.GetCaption

                    If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit Sub
                    DoEvents
                    m_BatchPriceProvider.RequestMultipleQuotes aQuoteUpdateReqs

                    nLoopCounter = nLoopCounter + 1
                Wend
                
                If Err Then
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogFaults, "Returned error (" & Err.Description & ") Can't start multiple request", m_frmOwner.GetCaption
                    GoTo EH
                End If
                     
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogEnhDebug, "Try ctlRiskView.RequestLastQuotes (End mutliple request list). BugCounter Value: " & CStr(aBugCounter), m_frmOwner.GetCaption
            Else
                aBugCounter = 12
                m_nQuoteGroupReqCount = m_AuxClc.QuoteReqsNonGrp.Count + m_AuxClc.QuoteReqsGrp.Count
                m_nQuoteGroupReqDone = 0&
            
                For Each aReq In m_AuxClc.QuoteReqsNonGrp
                    aBugCounter = 13
                    If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit For
                    PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
                    aBugCounter = 14
                    DoEvents
                Next
                
                For Each aReq In m_AuxClc.QuoteReqsGrp
                    aBugCounter = 15
                    If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit For
                    m_GroupPriceProvider.RequestLastGroupQuotes aReq.GetGroupQuoteUpdateParam
                    aBugCounter = 16
                    DoEvents
                Next
            End If
            
            m_bInProc = False
            aBugCounter = 17
            AdjustState
        Else
            aBugCounter = 18
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            imgStop.Visible = False
            imgStopDis.Visible = False
            
            RaiseEvent OnRefreshEmpty
        End If
    Else
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        aBugCounter = 19
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes. Counter Position: " & CStr(aBugCounter)
    On Error Resume Next
    Set aUnd = Nothing
    Set aIdxReq = Nothing
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    AdjustState
    
    PriceProvider.CancelLastQuote
    If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
    
    RaiseEvent OnRefreshError

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
End Sub

Private Function IndexLoad() As Boolean
    On Error Resume Next
    IndexLoad = False
    If m_bInProc Then Exit Function
    Dim aGIdx As EtsGeneralLib.IndexAtom, aComp As EtsGeneralLib.IndexCompAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, sKey$
    Dim aReq As EtsMmRisksLib.MmRvReqAtom, nKey&
    
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
        
        If Not aReq Is Nothing Then
            If aReq.IndexOnly Then
                If Not PriceProvider Is Nothing Then
                    If m_Aux.RealTime Then
                        PriceProvider.UnSubscribeQuote aReq.GetQuoteUpdateParam
                    Else
                        PriceProvider.CancelLastQuote aReq.GetQuoteUpdateParam
                    End If
                End If
                Set aReq = Nothing
                
                m_AuxClc.QuoteReqsNonGrp.Remove sKey
                m_AuxClc.QuoteReqsAll.Remove sKey
                
                m_AuxClc.UndCount = m_AuxClc.UndCount - 1
            End If
        End If
    
        m_Aux.Idx.Clear
    End If
    
    If m_Aux.Filter(RFC_INDEX) <> 0 Then
        nKey = m_Aux.Filter(RFC_INDEX)
'        Set aGIdx = g_Index(nKey)
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
                
                m_AuxClc.QuoteReqsNonGrp.Add sKey, aReq
                
                m_AuxClc.UndCount = m_AuxClc.UndCount + 1
            Else
                m_Aux.Idx.PriceBid = aReq.Und.PriceBid
                m_Aux.Idx.PriceAsk = aReq.Und.PriceAsk
                m_Aux.Idx.PriceLast = aReq.Und.PriceLast
                m_Aux.Idx.NetChange = aReq.Und.NetChange
            End If
            
            Set m_Aux.Idx.UndPriceProfile = aGIdx.UndPriceProfile
            Set m_Aux.Idx.OptPriceProfile = aGIdx.OptPriceProfile
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
    Dim aPos As EtsMmRisksLib.MmRvPosAtom
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    On Error Resume Next
    fgPos.Rows = 1
    fgTot.Cell(flexcpText, 1, 0, 1, fgTot.Cols - 1) = ""
    fgTot.Row = 1

    m_Aux.m_aContext.Clear
    
    ' clear opposite positions references in positions
    For Each aUnd In m_Aux.Und
        For Each aPos In aUnd.Pos
            Set aPos.OppositePos = Nothing
        Next
    Next
    
    m_Aux.Grp.Clear
    m_Aux.Und.Clear
    m_Aux.Exp.Clear
    If Not m_Aux.Grp.SimulationGrp Is Nothing Then m_Aux.Grp.SimulationGrp.Clear
    If Not m_Aux.SimUnd Is Nothing Then m_Aux.SimUnd.Clear
    
    m_AuxClc.QuoteReqsNonGrp.Clear
    m_AuxClc.QuoteReqsGrp.Clear
    m_AuxClc.QuoteReqsAll.Clear
    
    m_nCurUnd = 0
    
    m_Aux.FormatFltGrid
    m_Aux.FormatFltColumns
    
    m_Aux.FormatTotGrid
    m_Aux.FormatTotColumns
    
    m_Aux.FormatPosGrid
    m_Aux.FilterUpdateAll
    
    AdjustCaption
    AdjustState
    
    m_Aux.FilterUpdateExpiry True
    SetRefreshHint False
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    Dim aRowData As clsRvRowData
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub

    'mnuCtxTradeNew         "New Trade..."
    'mnuCtxPositionTransfer "Position Transfer..."
    'mnuCtxOrderNewStock    "New Stock Order..."
    'mnuCtxOrderNewOption   "New Option Order..."
    'mnuCtxTradeExercise    "Exercise Trades..."
    'mnuCtxTradeExpiry      "Expiry Trades..."
    'mnuCtxRealTime         "Real Time Mode"
    'mnuCtxRefresh          "Refresh"
    'mnuCtxCopy             "Copy Grid"
    'mnuCtxPrint            "Print Grid..."
    'mnuCtxWtdVega          "Weighted Vega Settings..."
    'mnuCtxHideCol          "Hide Column"
    'mnuCtxAutosizeCol      "Autosize Column"
    'mnuCtxAutosizeGrid     "Autosize Grid"
    'mnuCtxGridLayout       "Grid Layout..."
    
    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxTradeExercise.Enabled = False
    mnuCtxTradeExpiry.Enabled = False
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    mnuCtxFitTheoForAll.Enabled = False
    mnuCtxFitTheoForSelected.Enabled = False
    
    Select Case m_enMenuGrid
        Case GT_RISKS_FILTER
            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_Aux.Filter(RFC_GROUP) = TYPE_UNDERLYING And m_Aux.Filter(RFC_VALUE) <> 0 _
                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
            
            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
            
            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            
            mnuCtxEditCustomVariable.Enabled = False
            
            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
        Case GT_RISKS_TOTALS
            mnuCtxTradeNew.Enabled = False
            mnuCtxPositionTransfer.Enabled = False
            
            mnuCtxHideCol.Enabled = (m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols And m_gdTot.Idx(1) <> RTC_NONE)
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            
            mnuCtxEditCustomVariable.Enabled = True
            
            PopupMenu mnuCtx
        
        Case GT_RISKS_POSITIONS
            Dim bEnabled As Boolean: bEnabled = False
            Set aRowData = fgPos.RowData(m_nMenuGridRow)
            
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
            
            mnuCtxEditCustomVariable.Enabled = True
            
            mnuCtxFitTheoForAll.Enabled = Not aRowData.Und Is Nothing
            mnuCtxFitTheoForSelected.Enabled = Not aRowData.Pos Is Nothing And Not aRowData.Und Is Nothing
            
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
    If m_bShutDown Or m_bSubscribingNow Then Exit Sub
    
    SetRefreshHint False
    
    If Not m_Aux.RealTime Then
        StartRealTime
    Else
        StopRealTime
    End If
End Sub

Private Sub StopRealTime()
    On Error Resume Next
    
    m_Aux.RealTime = False
    m_bSubscribingNow = False
    
    lblStatus.Visible = True
    pbProgress.Visible = False
    lblProcess.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    lblStatus.Caption = "Real Time (Quotes unsubscription...)"
    lblStatus.Refresh
    
    PriceProvider.UnSubscribeQuote
    If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes
    Screen.MousePointer = vbDefault
    AdjustState
    AdjustCaption
End Sub

' request last index quote if view is not in realtime
Private Sub RequestIndexLastQuote()
    On Error GoTo EH
    If m_bShutDown Or m_bInProc Or m_Aux.RealTime Then Exit Sub
    
    Dim aIdxReq As EtsMmRisksLib.MmRvReqAtom
    
    If m_Aux.Idx.ID > 0 Then Set aIdxReq = m_AuxClc.QuoteReqsAll(CStr(enCtIndex) & "_" & m_Aux.Idx.Symbol)
    
    If Not aIdxReq Is Nothing Then
        If aIdxReq.IndexOnly Then ' no position(s) on this index yet
            m_bLastQuoteReqNow = True
            
            lblStatus.Visible = False
            imgStop.Visible = True
            imgStopDis.Visible = False
            pbProgress.Visible = True
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = 1
            lblProcess.Caption = "Index last quotes request..."
            lblProcess.Visible = True
            lblProcess.Refresh
            
            m_nQuoteReqCount = 1
            m_nQuoteReqDone = 0&
            
            m_nQuoteGroupReqCount = 0&
            m_nQuoteGroupReqDone = 0&
            
            PriceProvider.RequestLastQuote aIdxReq.GetQuoteUpdateParam
        Else
            m_AuxClc.UnderlyingsCalc False, False
            m_AuxOut.TotalsUpdate
        End If
        Set aIdxReq = Nothing
    End If
    
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last index quote."
    On Error Resume Next
    
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    
    AdjustState
    AdjustCaption
    
    If Not aIdxReq Is Nothing Then
        If aIdxReq.IndexOnly Then PriceProvider.CancelLastQuote aIdxReq.GetQuoteUpdateParam
    End If
    
    RaiseEvent OnRefreshError
End Sub

' subscribe to index if view is in realtime
Private Sub SubscribeToIndex()
    On Error GoTo EH
    If m_bShutDown Or m_bInProc Or Not m_Aux.RealTime Then Exit Sub
    
    Dim aIdxReq As EtsMmRisksLib.MmRvReqAtom
    
    If m_Aux.Idx.ID > 0 Then Set aIdxReq = m_AuxClc.QuoteReqsAll(CStr(enCtIndex) & "_" & m_Aux.Idx.Symbol)
    
    If Not aIdxReq Is Nothing Then
        If aIdxReq.IndexOnly Then ' no position(s) on this index yet
            m_bSubscribingNow = True
            
            lblStatus.Visible = False
            imgStop.Visible = True
            imgStopDis.Visible = False
            pbProgress.Visible = True
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = 1
            lblProcess.Caption = "Subscribe to index quotes..."
            lblProcess.Visible = True
            lblProcess.Refresh
            
            m_nQuoteReqCount = 1
            m_nQuoteReqDone = 0&
            
            m_nQuoteGroupReqCount = 0&
            m_nQuoteGroupReqDone = 0&
            
            PriceProvider.SubscribeQuote aIdxReq.GetQuoteUpdateParam
        Else
            m_AuxClc.UnderlyingsCalc False, False
            m_AuxOut.TotalsUpdate
        End If
        Set aIdxReq = Nothing
    End If
    
    Exit Sub
EH:
    m_bSubscribingNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to subscribe to index quotes."
    On Error Resume Next
    
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    
    AdjustState
    AdjustCaption
    
    If Not aIdxReq Is Nothing Then
        If aIdxReq.IndexOnly Then
            aParams(1) = aIdxReq.GetQuoteUpdateParam
            PriceProvider.UnSubscribeQuote aParams
        End If
    End If
End Sub

Private Function StartRealTime() As Boolean
    On Error GoTo EH
    If m_bShutDown Or m_bInProc Or m_bLastQuoteReqNow Then Exit Function
    If m_AuxClc.CantStartRealTime Then Exit Function
    
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aReq As EtsMmRisksLib.MmRvReqAtom
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams
    
    
    If Not PriceProvider Is Nothing And Not m_BatchPriceProvider Is Nothing Then
        Debug.Assert Not m_bSubscribingNow
        
        m_nUndResponses = 0
        m_nOptResponses = 0
        m_nFutResponses = 0
        
        If m_AuxClc.QuoteReqsAll.Count > 0 Then
            m_bGroupRequest = m_bGroupRequestSupported And Not g_Params.RiskReqTypeAlwaysNonGroup
            
            If Not g_PerformanceLog Is Nothing Then _
                m_nOperation = g_PerformanceLog.BeginLogMmOperation
        
            m_bSubscribingNow = True
            m_bInProc = True
            AdjustState
            
            m_nQuoteReqCount = m_AuxClc.QuoteReqsAll.Count
            m_nQuoteReqDone = 0&
            
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = m_nQuoteReqCount
        
            pbProgress.Visible = True
        
            lblStatus.Visible = False
            lblProcess.Visible = True
            lblProcess.Caption = "Quotes subscription..."
            lblProcess.Refresh
            imgStop.Visible = True
            imgStopDis.Visible = False
            
            If Not m_bGroupRequest Then
                m_nQuoteGroupReqCount = 0&
                m_nQuoteGroupReqDone = 0&
                
                Dim aQuoteUpdateReqs() As PRICEPROVIDERSLib.QuoteUpdateParams
                ReDim aQuoteUpdateReqs(m_AuxClc.QuoteReqsAll.Count - 1)
                
                Dim nLoopCounter As Long: nLoopCounter = 0
                For Each aReq In m_AuxClc.QuoteReqsAll
                    If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                    
                    aQuoteUpdateReqs(nLoopCounter) = aReq.GetQuoteUpdateParam
                    nLoopCounter = nLoopCounter + 1
                    
                    DoEvents
                Next

                m_BatchPriceProvider.SubscribeMultipleQuotes aQuoteUpdateReqs
            Else
                m_nQuoteGroupReqCount = m_AuxClc.QuoteReqsNonGrp.Count + m_AuxClc.QuoteReqsGrp.Count
                m_nQuoteGroupReqDone = 0&
            
                For Each aReq In m_AuxClc.QuoteReqsNonGrp
                    If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                    PriceProvider.SubscribeQuote aReq.GetQuoteUpdateParam
                    DoEvents
                Next
                
                For Each aReq In m_AuxClc.QuoteReqsGrp
                    If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                    m_GroupPriceProvider.SubscribeGroupQuotes aReq.GetGroupQuoteUpdateParam
                    DoEvents
                Next
            End If
            
            m_bInProc = False
            AdjustState
        Else
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            imgStop.Visible = False
            imgStopDis.Visible = False
        End If
    Else
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    
    Exit Function
EH:
    m_bSubscribingNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to subscribe to quotes."
    On Error Resume Next
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    AdjustState
    
    PriceProvider.UnSubscribeQuote
    If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
End Function

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Refresh"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    If Not m_bShutDown Then Refresh
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
    Dim sKey$, arrExp() As Long, collExp As EtsGeneralLib.EtsMmEntityAtomColl
    Dim i&, nCount&, aExp As EtsGeneralLib.EtsMmEntityAtom, bAddUnd As Boolean, bAddPos As Boolean, nRow&, nUndRow&, nGreekRow&
    Dim bGridLocked As Boolean: bGridLocked = False
    Dim aRowData As clsRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim bRatesUpdated As Boolean: bRatesUpdated = False
    Dim aGIdx As EtsGeneralLib.IndexAtom, aIdxComp As EtsGeneralLib.IndexCompAtom
    Dim aNewReqsAll As New EtsMmRisksLib.MmRvReqColl
    Dim aNewReqsNonGrp As New EtsMmRisksLib.MmRvReqColl
    Dim aNewReqsGrp As New EtsMmRisksLib.MmRvReqColl
    Dim lSurfaceID As Long
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTrade Enter", m_frmOwner.GetCaption

    
    If Not m_Aux.CheckTradeFilter(aTrd) Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "TradeFilter passed", m_frmOwner.GetCaption
    
    
    m_bInProc = True
    AdjustState
    
    Set collExp = New EtsGeneralLib.EtsMmEntityAtomColl
    For Each aExp In m_Aux.Exp
        collExp.Add CStr(aExp.ID), aExp
    Next
    
    Set aUnd = m_Aux.Und(aTrd.UndID)
    If aUnd Is Nothing Then
        Set aUnd = m_AuxClc.AddNewUnderlying(aTrd, aNewReqsAll, aNewReqsNonGrp)
        bAddUnd = True
        If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "Underlying added, Symbol: " & aTrd.Symbol, m_frmOwner.GetCaption
        
    End If
    
    aUnd.IsDirty = True
    
    Set aPos = aUnd.Pos(aTrd.ContractID)
    If aPos Is Nothing Then
        Set aPos = m_AuxClc.AddNewPosition(aTrd, aUnd, aNewReqsAll, aNewReqsNonGrp, aNewReqsGrp)
        bAddPos = True
        If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "Position added, Symbol: " & aTrd.Symbol, m_frmOwner.GetCaption
    
    End If
    
    aPos.IsDirty = True
    
    m_AuxClc.AddNewTradeToPosition aUnd, aPos, aTrd
    
    If bAddUnd Then
'        Set aGIdx = g_Index(m_Aux.Idx.ID)
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
    
    If m_Aux.Exp.Count <> collExp.Count Then
        m_Aux.Exp.Clear
        nCount = collExp.Count
        If nCount > 0 Then
            ReDim arrExp(1 To nCount)
            
            i = 1
            For Each aExp In collExp
                arrExp(i) = aExp.ID
                i = i + 1
            Next
                
            If nCount > 1 Then SortArray arrExp, 1, nCount
            
            For i = 1 To nCount
                m_Aux.Exp.Add CStr(arrExp(i)), collExp(CStr(arrExp(i)))
            Next
        End If
        m_Aux.FilterUpdateExpiry True
    End If
    
    Erase arrExp
    Set collExp = Nothing
    
    bRatesUpdated = m_AuxClc.UnderlyingAdjustRates(aUnd, bAddUnd Or bAddPos)
    
    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        bGridLocked = True
        
        sKey = CStr(aUnd.ID) & "_" & CStr(aUnd.ID)
        
        If bAddUnd Then
            .AddItem ""
            nUndRow = .Rows - 1
            
            Set aRowData = New clsRvRowData
            Set aRowData.Pos = Nothing
            Set aRowData.Und = aUnd
            Set aRowData.SynthGreeks = Nothing
            
            .RowData(nUndRow) = aRowData
            .TextMatrix(nUndRow, RPC_KEY) = sKey
            .IsSubtotal(nUndRow) = True
            .RowOutlineLevel(nUndRow) = ROL_UND
            
            Set aRowData = Nothing
            
            If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                For Each aSynthGreek In aUnd.SynthGreeks
                    Set aRowData = New clsRvRowData
                    Set aRowData.Pos = Nothing
                    Set aRowData.Und = aUnd
                    Set aRowData.SynthGreeks = aSynthGreek
                    aSynthGreek.Visible = True
                    
                    .AddItem ""
                    nGreekRow = .Rows - 1
                    .RowData(nGreekRow) = aRowData
                    Set aRowData = Nothing
                    .TextMatrix(nGreekRow, RPC_KEY) = CStr(aUnd.ID) & "_" & CStr(aSynthGreek.SynthUndID)
                    .RowOutlineLevel(nGreekRow) = ROL_POS
                Next
            End If
        
'            m_AuxClc.UnderlyingCalc aUnd, True, True
        Else
            nUndRow = .FindRow(sKey, 1, RPC_KEY, , True)
        End If
        
        nRow = nUndRow
        
        If nRow <> -1 Then
            Do While .RowData(nRow).Pos Is Nothing
                If nRow = .Rows - 1 Then
                    nRow = nRow + 1
                    Exit Do
                End If
                nRow = nRow + 1
            Loop

            sKey = CStr(aUnd.ID) & "_" & CStr(aUnd.ID) & "_" & CStr(aPos.ID)

            If bAddPos Then
                .AddItem "", nRow
                Set aRowData = New clsRvRowData
                Set aRowData.Pos = aPos
                Set aRowData.Und = aUnd
                Set aRowData.SynthGreeks = Nothing
                .RowData(nRow) = aRowData
                .TextMatrix(nRow, RPC_KEY) = sKey
                .RowOutlineLevel(nRow) = ROL_POS

                aPos.Visible = m_Aux.CheckPosFilter(aPos)
                If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then _
                    m_nOptPositions = m_nOptPositions + IIf(aPos.Visible, 1, 0)
                    
                If aPos.ContractType = enCtStock Or aPos.ContractType = enCtIndex Or aPos.ContractType = enCtFuture Then _
                    m_nUndPositions = m_nUndPositions + IIf(aPos.Visible, 1, 0)
            
                .RowHidden(nRow) = Not aPos.Visible Or .IsCollapsed(nUndRow) <> flexOutlineExpanded

                nGreekRow = nRow

                If Not bAddUnd And aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aUnd.SynthGreeks
                        
                        sKey = CStr(aUnd.ID) & "_" & CStr(aSynthGreek.SynthUndID)
                        
                        If .FindRow(sKey, 1, RPC_KEY, , True) < 1 Then
                            Set aRowData = New clsRvRowData
                            Set aRowData.Pos = Nothing
                            Set aRowData.Und = aUnd
                            Set aRowData.SynthGreeks = aSynthGreek
                            aSynthGreek.Visible = True
    
                            nGreekRow = nGreekRow + 1
                            nRow = nRow + 1
                            .AddItem "", nUndRow + 1
                            .RowData(nUndRow + 1) = aRowData
                            Set aRowData = Nothing
                            .TextMatrix(nUndRow + 1, RPC_KEY) = CStr(aUnd.ID) & "_" & CStr(aSynthGreek.SynthUndID)
                            .RowOutlineLevel(nUndRow + 1) = ROL_POS
                            .RowHidden(nUndRow + 1) = Not aSynthGreek.Visible Or .IsCollapsed(nUndRow) <> flexOutlineExpanded
                        End If
                    Next
                End If
                
                If aPos.IsSynthetic And Not aPos.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aPos.SynthGreeks
                        Set aRowData = New clsRvRowData
                        Set aRowData.Pos = aPos
                        Set aRowData.Und = aUnd
                        Set aRowData.SynthGreeks = aSynthGreek
                        aSynthGreek.Visible = aPos.Visible

                        nGreekRow = nGreekRow + 1
                        .AddItem "", nGreekRow
                        .RowData(nGreekRow) = aRowData
                        Set aRowData = Nothing
                        .TextMatrix(nGreekRow, RPC_KEY) = CStr(aUnd.ID) & "_" & CStr(aSynthGreek.SynthUndID) & "_" & CStr(aPos.ID)
                        .RowOutlineLevel(nGreekRow) = ROL_POS
                        .RowHidden(nGreekRow) = Not aSynthGreek.Visible Or .IsCollapsed(nUndRow) <> flexOutlineExpanded
                    Next
                End If
                
                If Not bAddUnd And aPos.Visible Then
                    lSurfaceID = GetSurfaceByPos(aPos, aRowData.Und)
                    aPos.Vola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                End If
            Else
                nRow = .FindRow(sKey, 1, RPC_KEY, , True)
            End If
        End If
        
        Debug.Assert nUndRow > 0 And nRow > 0
        
        'm_AuxClc.UnderlyingsCalc True, False
        
        If bRatesUpdated Then
            m_AuxOut.PositionsUpdate nUndRow, True
        Else
            m_AuxOut.PositionUpdate nRow, True
        End If
        
        m_AuxOut.UnderlyingUpdate nUndRow, True
                        
        If aUnd.HasSynthetic Then m_AuxOut.SyntheticUnderlyingGreeksUpdate aUnd, True
        If aPos.IsSynthetic Then m_AuxOut.SyntheticPositionGreeksUpdate aUnd.ID, aPos, True
        
        If bAddUnd Then .IsCollapsed(nUndRow) = flexOutlineCollapsed
        If bAddUnd Or bAddPos Then m_Aux.FormatPosColumns
        
'        m_AuxOut.TotalsUpdate
        
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
        bGridLocked = False
        
        If m_Aux.RealTime Then
            If Not g_PerformanceLog Is Nothing Then _
                   g_PerformanceLog.LogMmInfo enLogEnhDebug, "SubscribeToNewPositions Call", m_frmOwner.GetCaption
            SubscribeToNewPositions aNewReqsAll, aNewReqsNonGrp, aNewReqsGrp
        Else
            If Not g_PerformanceLog Is Nothing Then _
                   g_PerformanceLog.LogMmInfo enLogEnhDebug, "Set Refresh Hint Call", m_frmOwner.GetCaption
            SetRefreshHint True
        End If
    End With
    
    Set aPos = Nothing
    Set aUnd = Nothing
    
    m_bInProc = False
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
    Erase arrExp
    Set collExp = Nothing
End Sub

Private Sub SubscribeToNewPositions(ByRef aNewReqsAll As EtsMmRisksLib.MmRvReqColl, _
                                    ByRef aNewReqsNonGrp As EtsMmRisksLib.MmRvReqColl, _
                                    ByRef aNewReqsGrp As EtsMmRisksLib.MmRvReqColl)
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    Dim aReq As EtsMmRisksLib.MmRvReqAtom
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "SubscribeToNewPositions Enter", m_frmOwner.GetCaption
       

    
    If aNewReqsAll.Count > 0 Then
        m_bSubscribingNow = True
       
       If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "New Positions Count: " & CStr(aNewReqsAll.Count), m_frmOwner.GetCaption
        
        m_nQuoteReqCount = m_nQuoteReqCount + aNewReqsAll.Count
        
        lblStatus.Visible = False
        imgStop.Visible = True
        imgStopDis.Visible = False
        pbProgress.Visible = True
        pbProgress.Min = 0
        pbProgress.Max = m_nQuoteReqCount
        pbProgress.Value = m_nQuoteReqDone
        lblProcess.Caption = "Subscribe to quotes for new positions..."
        lblProcess.Visible = True
        lblProcess.Refresh
        
        If Not m_bGroupRequest Then
            Dim aQuoteUpdateReqs() As PRICEPROVIDERSLib.QuoteUpdateParams
            ReDim aQuoteUpdateReqs(aNewReqsAll.Count - 1)
                
            Dim nLoopCounter As Long: nLoopCounter = 0
            For Each aReq In aNewReqsAll
                If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                If Not g_PerformanceLog Is Nothing Then _
                  g_PerformanceLog.LogMmInfo enLogEnhDebug, "Non-group Subscribe (Symbol: " & aReq.GetQuoteUpdateParam.Symbol & ", Exchange: " & aReq.GetQuoteUpdateParam.Exchange & ", Type: " & aReq.GetQuoteUpdateParam.Type & ")", m_frmOwner.GetCaption
                
                aQuoteUpdateReqs(nLoopCounter) = aReq.GetQuoteUpdateParam
                nLoopCounter = nLoopCounter + 1
                DoEvents
            Next

            m_BatchPriceProvider.SubscribeMultipleQuotes aQuoteUpdateReqs
        
'            For Each aReq In aNewReqsAll
'                If m_bShutDown Or Not m_bSubscribingNow Then Exit For
'                If Not g_PerformanceLog Is Nothing Then _
'                  g_PerformanceLog.LogMmInfo enLogEnhDebug, "Non-group Subscribe (Symbol: " & aReq.GetQuoteUpdateParam.Symbol & ", Exchange: " & aReq.GetQuoteUpdateParam.Exchange & ", Type: " & aReq.GetQuoteUpdateParam.Type & ")", m_frmOwner.GetCaption
'
'                PriceProvider.SubscribeQuote aReq.GetQuoteUpdateParam
'                DoEvents
'            Next
        Else
            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + aNewReqsNonGrp.Count + aNewReqsGrp.Count
        
            For Each aReq In aNewReqsNonGrp
                If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                If Not g_PerformanceLog Is Nothing Then _
                  g_PerformanceLog.LogMmInfo enLogEnhDebug, "Group (group part)  Subscribe (Symbol: " & aReq.GetQuoteUpdateParam.Symbol & ", Exchange: " & aReq.GetQuoteUpdateParam.Exchange & ", Type: " & aReq.GetQuoteUpdateParam.Type & ")", m_frmOwner.GetCaption
                
                PriceProvider.SubscribeQuote aReq.GetQuoteUpdateParam
                DoEvents
            Next
            
            For Each aReq In aNewReqsGrp
                If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                If Not g_PerformanceLog Is Nothing Then _
                  g_PerformanceLog.LogMmInfo enLogEnhDebug, "Group (non group part)  Subscribe (Symbol: " & aReq.GetQuoteUpdateParam.Symbol & ", Exchange: " & aReq.GetQuoteUpdateParam.Exchange & ", Type: " & aReq.GetQuoteUpdateParam.Type & ")", m_frmOwner.GetCaption
                
                m_GroupPriceProvider.SubscribeGroupQuotes aReq.GetGroupQuoteUpdateParam
                DoEvents
            Next
        End If
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "SubscribeToNewPositions Exit", m_frmOwner.GetCaption
    
    Exit Sub
EH:
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogFaults, "SubscribeToNewPositions On Error", m_frmOwner.GetCaption
    
    m_bSubscribingNow = False
    If Not m_bShutDown Then LogEvent EVENT_ERROR, "Risks: Fail to subscribe to quotes for new positions." & Err.Description
    On Error Resume Next
    AdjustState
    AdjustCaption
    
    If Not m_bGroupRequest Then
        For Each aReq In aNewReqsAll
            If m_bShutDown Then Exit For
            aParams(1) = aReq.GetQuoteUpdateParam
            PriceProvider.UnSubscribeQuote aParams
        Next
    Else
        For Each aReq In aNewReqsNonGrp
            If m_bShutDown Then Exit For
            aParams(1) = aReq.GetQuoteUpdateParam
            PriceProvider.UnSubscribeQuote aParams
        Next
        
        For Each aReq In aNewReqsGrp
            If m_bShutDown Then Exit For
            m_GroupPriceProvider.UnSubscribeGroupQuotes aReq.GetGroupQuoteUpdateParam
        Next
    End If
End Sub

Private Sub DeleteTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error GoTo EH
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom
    Dim sKey$, nRow&, nUndRow&
    Dim bRatesUpdated As Boolean: bRatesUpdated = False
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "DeleteTrade Enter", m_frmOwner.GetCaption

    If Not m_Aux.CheckTradeFilter(aTrd) Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogEnhDebug, "TradeFilter passed", m_frmOwner.GetCaption
        
    Set aUnd = m_Aux.Und(aTrd.UndID)
    If aUnd Is Nothing Then Exit Sub
    
    aUnd.IsDirty = True
    
    Set aPos = aUnd.Pos(aTrd.ContractID)
    If aPos Is Nothing Then
        Set aUnd = Nothing
        Exit Sub
    End If
    
    aPos.IsDirty = True
    
    m_bInProc = True
    AdjustState
    
    m_AuxClc.RemoveOldTradeFromPosition aUnd, aPos, aTrd
    
    If aPos.ContractType <> enCtOption Then
        bRatesUpdated = m_AuxClc.UnderlyingAdjustRates(aUnd, False)
    End If
    
    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        sKey = CStr(aUnd.ID) & "_" & CStr(aUnd.ID)
        nUndRow = .FindRow(sKey, 1, RPC_KEY, , True)
        sKey = CStr(aUnd.ID) & "_" & CStr(aUnd.ID) & "_" & CStr(aPos.ID)
        nRow = .FindRow(sKey, 1, RPC_KEY, , True)
        
        Debug.Assert nUndRow > 0 And nRow > 0
        
        If bRatesUpdated Then
            m_AuxClc.UnderlyingCalc aUnd, True, False
            m_AuxOut.PositionsUpdate nUndRow, False
        Else
            m_AuxOut.PositionUpdate nRow, False
        End If
        
        m_AuxOut.UnderlyingUpdate nUndRow, False
        
        If aUnd.HasSynthetic Then m_AuxOut.SyntheticUnderlyingGreeksUpdate aUnd, True
        If aPos.IsSynthetic Then m_AuxOut.SyntheticPositionGreeksUpdate aUnd.ID, aPos, True
        
        m_AuxOut.TotalsUpdate
                            
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
    
    Set aPos = Nothing
    Set aUnd = Nothing

    m_bInProc = False
    AdjustState
    
    If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "DeleteTrade Exit, Symbol: " & aTrd.Symbol, m_frmOwner.GetCaption
    
    Exit Sub
EH:
    m_bInProc = False
    LogEvent EVENT_ERROR, "Risks: Fail to delete trade. " & Err.Description
    AdjustState
End Sub
    
Private Sub tmrUndCalc_Timer()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "UndCalc_Timer Enter.", m_frmOwner.GetCaption
    
    If m_bLastQuoteReqNow Or m_bSubscribingNow Or m_bDataLoad Or _
       m_bInProc Or m_bInRealTimeCalc Or m_bTmrUndCalcNow Then
            m_bIsNewTrades = False
            tmrUndCalc.Enabled = False
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "UndCalc_Timer Terminate.", m_frmOwner.GetCaption
            Exit Sub
    End If
    
    If m_bIsNewTrades Then
        m_bIsNewTrades = False
        Exit Sub
    End If

    m_bInProc = True
    m_bTmrUndCalcNow = True
    tmrUndCalc.Enabled = False
    
    m_AuxClc.UnderlyingsCalc True, False, , , True
    m_AuxOut.UnderlyingsUpdate True
    m_AuxOut.TotalsUpdate
    
    m_bInProc = False
    m_bTmrUndCalcNow = False
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "UndCalc_Timer Exit.", m_frmOwner.GetCaption
End Sub

Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_TradeAction Enter.", m_frmOwner.GetCaption
    
    If tmrUndCalc.Enabled = True Then
        m_bIsNewTrades = True
    Else
        tmrUndCalc.Enabled = True
    End If
    
    If m_bInProc Or m_bLastQuoteReqNow Or m_bDataLoad Or Not m_bTradeChannelEnable Then
        TradeQueue.Add aNewTrdInfo, aOldTrdInfo, enAction
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_TradeAction New Trade added to Queue.", m_frmOwner.GetCaption
        Exit Sub
    End If
    
    'If m_bShutDown Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub
'    DoEvents
    
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
    
'    DoEvents
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_TradeAction Exit.", m_frmOwner.GetCaption
    
End Sub

Private Sub TradeChannel_PositionTransfer(aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, aTrdTo As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
     If m_bShutDown Then Exit Sub
     
     If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_PositionTransfer Enter.", m_frmOwner.GetCaption
    
    If m_bLastQuoteReqNow Then
        Debug.Assert (TradeQueue.Add(aTrdFrom, Null, enTaTradeNew)) Is Nothing
        Debug.Assert (TradeQueue.Add(aTrdTo, Null, enTaTradeNew)) Is Nothing
        Exit Sub
    End If
    
    'If m_bShutDown Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub

    Debug.Assert Not aTrdFrom Is Nothing
    Debug.Assert Not aTrdTo Is Nothing
    
    AddTrade aTrdFrom
    AddTrade aTrdTo
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_PositionTransfer Exit.", m_frmOwner.GetCaption
End Sub

Private Sub UserControl_Hide()
    On Error Resume Next
    m_Aux.RealTime = False
    m_bSubscribingNow = False
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If m_bShutDown Or gCmn Is Nothing Then Exit Sub
    Dim nTotalCols As Long
    Dim nGridColWidth As Long, i As Long, nGridHeight As Long
    
    With fgFlt
        .Top = 0
        .Left = 0
        nGridHeight = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
        
        nTotalCols = -1
        For i = 0 To .Cols - 1
            If Not .ColHidden(i) Then nTotalCols = nTotalCols + 1
        Next
            
        nGridColWidth = 0
        If nTotalCols = .RightCol And .LeftCol = 0 Then
            nGridColWidth = 0&
            For i = 0 To .Cols - 1
                If Not .ColHidden(i) Then nGridColWidth = nGridColWidth + .ColWidth(i) + .GridLineWidth '+ ScaleY(.GridLineWidth, vbPixels, vbTwips)
            Next
            If nGridColWidth > 0 Then nGridColWidth = nGridColWidth - ScaleY(.GridLineWidth, vbPixels, vbTwips)
            
            If ScaleWidth < nGridColWidth Then
                nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
                .ScrollBars = flexScrollBarHorizontal
            Else
                .ScrollBars = flexScrollBarNone
            End If
        ElseIf nTotalCols <> .RightCol Or .LeftCol <> 0 Then
              nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
             .ScrollBars = flexScrollBarHorizontal
        Else
          .ScrollBars = flexScrollBarNone
        End If
        
        .Height = nGridHeight
    End With

    With fgTot
        .Top = fgFlt.Height + GRID_VERT_SPACE
        .Left = 0
        nGridHeight = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
        
        nTotalCols = -1
        For i = 0 To .Cols - 1
            If Not .ColHidden(i) Then nTotalCols = nTotalCols + 1
        Next
        
        nGridColWidth = 0
        If nTotalCols = .RightCol And .LeftCol = 0 Then
            nGridColWidth = 0&
            For i = 0 To .Cols - 1
                If Not .ColHidden(i) Then nGridColWidth = nGridColWidth + .ColWidth(i) + .GridLineWidth '+ ScaleY(.GridLineWidth, vbPixels, vbTwips)
            Next
            If nGridColWidth > 0 Then nGridColWidth = nGridColWidth - ScaleY(.GridLineWidth, vbPixels, vbTwips)
            
            If ScaleWidth < nGridColWidth Then
                nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
                .ScrollBars = flexScrollBarHorizontal
            Else
                .ScrollBars = flexScrollBarNone
            End If
        ElseIf nTotalCols <> .RightCol Or .LeftCol <> 0 Then
              nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
             .ScrollBars = flexScrollBarHorizontal
        Else
          .ScrollBars = flexScrollBarNone
        End If
        
        .Height = nGridHeight
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
    m_AuxClc.Term
    
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
    m_bInProc = False
    
    Set frmWtdVega = Nothing
    Set frmLayout = Nothing
    Set m_frmVarEditor = Nothing
    Set m_frmStockShift = Nothing
    Set TradeChannel = Nothing
    Set VolaSource = Nothing
    Set TradeQueue = Nothing
    
    Set gePos = Nothing
    
    If Not PriceProvider Is Nothing Then
        lblStatus.Visible = True
        pbProgress.Visible = False
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        lblStatus.Caption = "Price provider connection closing..."
        lblStatus.Refresh
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Connection closing", m_frmOwner.GetCaption
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Cancel Last Quote", m_frmOwner.GetCaption

        PriceProvider.CancelLastQuote
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Unsubscribe Quote", m_frmOwner.GetCaption
        
        If m_bSubscribingNow Or m_Aux.RealTime Then
            PriceProvider.UnSubscribeQuote
        End If

        If m_bGroupRequest Then
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Group Cancel Last Quote", m_frmOwner.GetCaption
            
            m_GroupPriceProvider.CancelLastGroupQuotes
            
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Group Unsubscribe Quote", m_frmOwner.GetCaption
            If m_bSubscribingNow Or m_Aux.RealTime Then
                m_GroupPriceProvider.UnSubscribeGroupQuotes
            End If
        End If
        
        If Not g_PerformanceLog Is Nothing Then _
              g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Disconnect", m_frmOwner.GetCaption
        
        PriceProvider.Disconnect
        
        If Not g_PerformanceLog Is Nothing Then _
              g_PerformanceLog.LogMmInfo enLogEnhDebug, "Set PriceProvider To Nothing", m_frmOwner.GetCaption
        
        Set PriceProvider = Nothing
        
        If Not g_PerformanceLog Is Nothing Then _
               g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Connection Closed", m_frmOwner.GetCaption
    
    End If
    Set m_GroupPriceProvider = Nothing
    Set m_BatchPriceProvider = Nothing
    
    ClearViewAndData
    
    m_Aux.Term
    
    m_Aux.RealTime = False
    m_bSubscribingNow = False
    m_Aux.Grp = Nothing
    Set m_Aux.m_aContext = Nothing
    m_Aux.SimUnd = Nothing
    m_Aux.Und = Nothing
    m_Aux.Idx = Nothing
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set aParams = Nothing
    
    m_Aux.Exp = Nothing
    Set m_OpenedUnd = Nothing
    
    Set m_gdFlt = Nothing
    Set m_gdTot = Nothing
    Set m_gdPos = Nothing

End Sub

Private Sub AdjustState()
    On Error Resume Next
    UpdateMenu
    RaiseEvent OnStateChange
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    mnuCtxRealTime.Enabled = m_Aux.Grp.ID <> 0 And Not m_bInProc _
                                And Not m_bLastQuoteReqNow And Not m_bSubscribingNow
    mnuCtxRealTime.Checked = m_Aux.RealTime
    mnuCtxRefresh.Enabled = mnuCtxRealTime.Enabled And Not m_Aux.RealTime
    mnuCtxRefreshPrices.Enabled = mnuCtxRefresh.Enabled
    mnuCtxWtdVega.Enabled = Not m_bInProc And Not frmWtdVega.IsOpened And Not m_bLastQuoteReqNow And Not m_bSubscribingNow
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
    
    sCaption = ""
    If m_Aux.Grp.ID <> 0 Then sCaption = sCaption & m_Aux.Grp.Name & " - "
    If m_Aux.RealTime Then _
        sCaption = sCaption & IIf(m_Aux.RealTimeConnected, "[Real Time] - ", "[Real Time (Disconnected)] - ")
    
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

Public Property Get LastQuoteReqNow() As Boolean
    LastQuoteReqNow = m_bLastQuoteReqNow
End Property

Public Property Get RealTime() As Boolean
    RealTime = m_Aux.RealTime
End Property

Public Property Get RealTimeConnected() As Boolean
    RealTimeConnected = m_Aux.RealTime And m_Aux.RealTimeConnected
End Property

Public Property Get SubscribingNow() As Boolean
    SubscribingNow = m_bSubscribingNow
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
    frmLayout.Execute GT_RISKS_FILTER, RFC_GROUP, RFC_LAST_COLUMN, m_gdFlt, m_frmOwner
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

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey

    ' common info
    For i = RFC_GROUP To RFC_EXPIRY
        aStorage.SetLongValue "RiskFlt" & sKey, "Filter" & CStr(i), m_Aux.Filter(i)
    Next
    
    m_gdFlt.WriteToStorage "RiskFltGrid" & sKey, aStorage, False
    m_gdTot.WriteToStorage "RiskTotGrid" & sKey, aStorage
    m_gdPos.WriteToStorage "RiskPosGrid" & sKey, aStorage
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String, _
                        Optional ByVal bRefreshData As Boolean = True)
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    ' common info
    For i = RFC_GROUP To RFC_EXPIRY
        m_Aux.Filter(i) = aStorage.GetLongValue("RiskFlt" & sKey, "Filter" & CStr(i), m_Aux.Filter(i))
    Next
    
    If m_Aux.Filter(RFC_TYPE) < 0 Or m_Aux.Filter(RFC_TYPE) > 6 Then m_Aux.Filter(RFC_TYPE) = 0
    m_nOpenedExpiry = m_Aux.Filter(RFC_EXPIRY)
    
    m_gdFlt.ReadFromStorage "RiskFltGrid" & sKey, aStorage, False
    m_gdTot.ReadFromStorage "RiskTotGrid" & sKey, aStorage
    m_gdPos.ReadFromStorage "RiskPosGrid" & sKey, aStorage
    
    tmrShow.Enabled = bRefreshData 'True
    Exit Sub
EH:
    gCmn.ErrorHandler ""
    RaiseEvent OnRefreshError
End Sub

Public Sub RefreshView()
    On Error Resume Next
    If m_bShutDown Or Not m_Aux.RealTime Or m_bSubscribingNow Or m_BatchPriceProvider Is Nothing Then Exit Sub
    If m_BatchPriceProvider.IsQuotesUpdated Or m_bVolaUpdated Then
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
    Dim aRowData As clsRvRowData
    
    m_nOpenedExpiry = 0
    m_OpenedUnd.Clear
    
    If m_Aux.Filter(RFC_VALUE) <> 0 Then
        m_nOpenedExpiry = m_Aux.Filter(RFC_EXPIRY)
        
        With fgPos
            nRow = .GetNodeRow(1, flexNTFirstSibling)
            While nRow > 0
                If .IsCollapsed(nRow) <> flexOutlineCollapsed And IIf(bStoreHidden, True, Not .RowHidden(nRow)) Then
                    Set aRowData = .RowData(nRow)
                    Set aUnd = aRowData.Und
                    If Not aUnd Is Nothing Then
                        If m_OpenedUnd(CStr(aUnd.ID)) Is Nothing Then m_OpenedUnd.Add CStr(aUnd.ID)
                        Set aUnd = Nothing
                    End If
                End If
                
                nRow = .GetNodeRow(nRow, flexNTNextSibling)
                Set aRowData = Nothing
            Wend
        End With
    End If
End Sub

Public Sub Refresh()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    StoreRecentFilters False
    m_Aux.UpdateFltData
    tmrShow.Enabled = True
End Sub

Public Sub RefreshPrices()
    On Error Resume Next
    If m_bShutDown Or m_Aux.RealTime Then Exit Sub
    RequestLastQuotes
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
    If Not g_Params.UseTheoVolatility Or m_bShutDown Then Exit Sub
    Dim aGUnd As EtsGeneralLib.UndAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, nRow&, sKey$
    
    With fgPos
        Set aGUnd = g_Underlying.BySortKey(Symbol)
        If Not aGUnd Is Nothing Then
            Set aUnd = m_Aux.Und(aGUnd.ID)
            Set aGUnd = Nothing
            
            If Not aUnd Is Nothing Then
            
                aUnd.VolaUpdated = True
                aUnd.IsDirty = True
                aUnd.CalcTotals = True
                
                If m_Aux.RealTime Then
                    m_bVolaUpdated = True
                    tmrRealTime.Enabled = True
                Else
                    CalculateRiskViewUnderlying aUnd
                    m_AuxOut.UnderlyingsUpdate True, False
                    m_AuxOut.TotalsUpdate
                End If
                Set aUnd = Nothing
            End If
        End If
    End With
End Sub

Public Sub CalculateRiskViewUnderlying(ByRef Und As EtsMmRisksLib.MmRvUndAtom)
On Error GoTo Exception
    Dim sSymbol As String
    
    If (Not Und Is Nothing) Then
        sSymbol = Und.Symbol
        'Check for new trades
        CheckTradeQueue
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
    Dim aRowData As clsRvRowData

    CurUnderlyingID = 0
    
    If m_Aux.Grp.GroupType = TYPE_UNDERLYING Then
        CurUnderlyingID = m_Aux.Grp.ID
    Else
        If fgPos.Row > 0 Then
            Set aRowData = fgPos.RowData(fgPos.Row)
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

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        m_nCurUnd = 0
        If m_Aux.Und.Count > 0 Then
            m_bCurUndChanging = True
            nRow = .GetNodeRow(1, flexNTFirstSibling)
            While nRow > 0
                If Not .RowHidden(nRow) Then
                    .IsCollapsed(nRow) = IIf(bShow, flexOutlineExpanded, flexOutlineCollapsed)
                End If
                nRow = .GetNodeRow(nRow, flexNTNextSibling)
            Wend
            m_bCurUndChanging = False
        End If
    
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim aUnd As EtsGeneralLib.UndAtom
    If m_Aux.Filter(RFC_GROUP) = TYPE_UNDERLYING Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then m_Aux.FilterUpdateValue False
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    If Not m_bShutDown And m_Aux.Filter(RFC_GROUP) = TYPE_GROUP Then m_Aux.FilterUpdateValue False
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    If Not m_bShutDown And m_Aux.Filter(RFC_GROUP) = TYPE_STRATEGY Then m_Aux.FilterUpdateValue False
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
                    mnuCtxTntCardNew_Click
                
                Case Else
                    mnuCtxOrderNewStock_Click
            End Select
            
            FixDblClickHandled
        
        ElseIf bTradeNewAvailable And Not (IsCtrlPressed Or IsShiftPressed) Then
            mnuCtxTradeNew_Click
            FixDblClickHandled
        End If
    End If
End Sub

Public Sub MakeDataSnapshot(ByVal sFileName$)
    On Error Resume Next
'    m_Aux.MakeDataSnapshot sFileName, m_frmOwner
End Sub

Public Sub CallOtcOptionCalcRV()
    On Error Resume Next
    If m_bShutDown Or m_bInProc Then Exit Sub
    Dim nUndID&, nID&, bBuy As Boolean, aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, nColIdx&
    Dim aRowData As clsRvRowData, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    
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
            Set aRowData = fgPos.RowData(m_nMenuGridRow)
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
            
            dStrike = aUnd.PriceLast
            dBid = aUnd.PriceBid
            dAsk = aUnd.PriceAsk


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
                        'lCDCount = aUnd.Dividend.CustomDivs.Count !!!
                    End If
               End If
            
            
            If Not aPos Is Nothing Then
                nUndID = aUnd.ID
                nID = aPos.ID
                sOptionSymbol = aPos.Symbol
                dStrike = aPos.Strike
                dBid = aPos.PriceBid
                dAsk = aPos.PriceAsk
                dtExpiryOV = aPos.ExpiryOV
                dRate = aPos.Rate
                dHTBRate = aPos.HTBRate
                dVola = aPos.Vola
                Set aPos = Nothing
            Else
                If aSynthGreek Is Nothing Then
                    nUndID = aUnd.ID
                    nID = nUndID
                    sOptionSymbol = aPos.Symbol
                    'dtExpiry = aUnd.Ex.Expiry
                    For Each aPos In aUnd.Pos
                        If (Not aPos Is Nothing) And (aPos.Expiry > 0) Then
                            sOptionSymbol = aPos.Symbol
                            dStrike = aPos.Strike
                            dBid = aPos.PriceBid
                            dAsk = aPos.PriceAsk
                            dtExpiryOV = aPos.ExpiryOV
                            dRate = aPos.Rate
                            dHTBRate = aPos.HTBRate
                            dVola = aPos.Vola
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
        
        Set aRowData = Nothing
        Set aSynthGreek = Nothing
        
    ElseIf m_enMenuGrid = GT_RISKS_FILTER Then
        If m_Aux.Filter(RFC_GROUP) = TYPE_UNDERLYING And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_Aux.Filter(RFC_VALUE)
            nID = nUndID
            bBuy = (g_Params.RiskOtherColBuy <> 0)
            
        End If
    End If

    lCalcModel = CLng(g_Params.CalcModel)
    
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

    'For Each aCustDiv In aUnd.CustomDivs
    ' sParams = sParams & " " & aCustDiv.DivDate & " " & aCustDiv.DivAmount
    'Next
    'MsgBox sParams
    If ShellExecute(0&, "Open", sPath, sParams, "", SW_SHOWNORMAL) <= 32 Then
         gCmn.MyMsgBox Me, "Fail to open OTC OptionCalc at '" & sPath & "'.", vbCritical
    End If
    
    Exit Sub
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to open OTC Option Calc."
    ResetMenuData
End Sub

Public Property Get ContextID() As Long
    On Error Resume Next
    ContextID = m_Aux.m_nContextID
End Property

Public Property Get GrpLevelID() As Long
    On Error Resume Next
    GrpLevelID = m_Aux.m_nGrpLevelID
End Property

Public Property Get UndLevelID() As Long
    On Error Resume Next
    UndLevelID = m_Aux.m_nUndLevelID
End Property

Public Property Get OptLevelID() As Long
    On Error Resume Next
    OptLevelID = m_Aux.m_nOptLevelID
End Property
Public Function GetStockInfo() As String
    On Error Resume Next
    Dim sStock As String
    sStock = fgPos.RowData(m_nMenuGridRow).Und.Symbol
    GetStockInfo = IIf(sStock, "Stock symbol = """ & sStock & """. ", "")
End Function

Public Function GetOptionInfo() As String
    On Error Resume Next
    Dim sOption As String
    sOption = fgPos.RowData(m_nMenuGridRow).Pos.Symbol
    GetOptionInfo = GetStockInfo & IIf(sOption, "Option symbol = """ & sOption & """. ", "")
End Function

Public Sub LoadStockShift()
    On Error Resume Next
    
    m_frmStockShift.ShowData g_Params.StockShiftFile, m_Aux.m_StockShifts, False, False, m_Aux.Und, m_Aux.m_nShiftID, m_frmOwner
    
End Sub

Public Function ExportScreen(ByVal sFileName As String, ByVal sFilePath As String, ByVal bAggByHolding As Boolean, _
                             Optional ByVal sHeader As String = "", _
                             Optional ByVal FileSaveType As FileSaveTypeEnum = SE_ADDTOEND, _
                             Optional ByVal bLastPart As Boolean = True) As Boolean
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportScreen = g_ScreenExport.Save(sFileName, sFilePath, fgPos, , Not bAggByHolding, sHeader, FileSaveType, bLastPart)
    Screen.MousePointer = vbNormal
End Function

Public Sub SetSimulationParams(ByVal bSimulate As Boolean, _
                               ByVal iStockShift As Long, _
                               ByVal iVolaShift As Long, _
                               ByVal dtCalcDate As Date)
    On Error Resume Next
    
    m_Aux.Simulation = bSimulate
    
    m_Aux.SimulationSpot = iStockShift
    m_Aux.SimulationVola = iVolaShift
    m_Aux.SimulationDate = dtCalcDate
End Sub

Public Sub ExportAllStockShifts()
    On Error Resume Next
    Dim vbRes As VbMsgBoxResult
    
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not IsStockShiftExecuted Then
        vbRes = gCmn.MyMsgBox(m_frmOwner, "Export this screen for ALL shifts to the directory """ & g_Params.ScreenExportPath & """?", vbQuestion + vbYesNo)
        If vbRes <> vbYes Then Exit Sub
        DisableTradeChannel
    End If
    
    If m_Aux.m_nStockShiftIndex < 0 Or m_Aux.m_nStockShiftIndex > m_Aux.m_UniqueStockShiftIDs.Count - 1 Then
        m_Aux.m_nStockShiftIndex = 0
    End If
    
    m_Aux.m_nStockShiftIndex = m_Aux.m_nStockShiftIndex + 1
    m_Aux.m_nShiftID = CLng(m_Aux.m_UniqueStockShiftIDs(m_Aux.m_nStockShiftIndex))
    Refresh
End Sub

Public Function IsFirstStockShiftExecuted() As Boolean
    On Error Resume Next
    IsFirstStockShiftExecuted = CBool(m_Aux.m_nStockShiftIndex = 1)
End Function

Public Function IsLastStockShiftExecuted() As Boolean
    On Error Resume Next
    IsLastStockShiftExecuted = CBool(m_Aux.m_nStockShiftIndex = m_Aux.m_UniqueStockShiftIDs.Count)
End Function

Public Function IsStockShiftExecuted() As Boolean
    On Error Resume Next
    IsStockShiftExecuted = CBool(m_Aux.m_nStockShiftIndex > 0 And _
                                m_Aux.m_nStockShiftIndex <= m_Aux.m_UniqueStockShiftIDs.Count)
End Function

Public Sub CancelStockShiftExecution()
    On Error Resume Next
    m_Aux.m_nStockShiftIndex = 0
    m_Aux.m_nShiftID = 0
    EnableTradeChannel
End Sub

Public Function GetStockShiftInfo() As String
    On Error Resume Next
    Dim nShiftID As Long
    Dim nVolaShift As Long
    Dim dtDate As Date
    
    nShiftID = CLng(m_Aux.m_UniqueStockShiftIDs(m_Aux.m_nStockShiftIndex))
    nVolaShift = m_Aux.SimulationParams.m_dVolaChangePercent
    dtDate = m_Aux.SimulationParams.m_dDate
    
    GetStockShiftInfo = "Spot Shift#," & CStr(nShiftID) & ",Volatility Shift%," & CStr(nVolaShift) & ",Date," & CStr(dtDate)
End Function

Public Function InitBatchBuffer(ByVal nSpotShiftCount As Long, ByVal nVolaShiftCount As Long) As Boolean
    On Error Resume Next
    InitBatchBuffer = g_ScreenExport.InitBatchBuffer(fgTot.Cols, nSpotShiftCount, nVolaShiftCount)
End Function

Public Function SaveBatchResult(ByVal nSpotShiftIdx As Long, ByVal nSpotShiftValue As Long, _
                                ByVal nVolaShiftIdx As Long, ByVal nVolaShiftValue As Long) As Boolean
    On Error Resume Next
    SaveBatchResult = g_ScreenExport.SaveInBatchBuffer(fgTot, nSpotShiftIdx, nSpotShiftValue, nVolaShiftIdx, nVolaShiftValue)
End Function

Public Function ExportBatchResults(ByVal sFileName As String, ByVal sGroupDesc As String, ByVal bIsNewFile As Boolean)
    On Error Resume Next
    Dim sHeader As String: sHeader = "Date," & CStr(m_Aux.SimulationParams.m_dDate)
    
    ExportBatchResults = g_ScreenExport.DropBatchBufferToFile(sFileName, g_Params.BatchCapabilityPath, , sHeader, sGroupDesc, bIsNewFile)
End Function

Public Sub EnableTradeChannel()
    On Error Resume Next
    m_bTradeChannelEnable = True
    CheckTradeQueue
End Sub

Public Sub DisableTradeChannel()
    On Error Resume Next
    m_bTradeChannelEnable = False
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

