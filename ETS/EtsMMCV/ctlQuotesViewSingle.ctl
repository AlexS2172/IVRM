VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.UserControl ctlQuotesViewSingle 
   ClientHeight    =   8460
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9675
   KeyPreview      =   -1  'True
   ScaleHeight     =   8460
   ScaleWidth      =   9675
   Begin VB.Timer tmrTradesProcessing 
      Enabled         =   0   'False
      Interval        =   1500
      Left            =   840
      Top             =   4440
   End
   Begin MSComCtl2.DTPicker dtCalculationDate 
      Height          =   255
      Left            =   4320
      TabIndex        =   4
      Top             =   4200
      Visible         =   0   'False
      Width           =   2295
      _ExtentX        =   4048
      _ExtentY        =   450
      _Version        =   393216
      Format          =   54263809
      CurrentDate     =   38517
   End
   Begin VB.Timer tmrRealTime 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   2640
      Top             =   4440
   End
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   1800
      Top             =   4440
   End
   Begin VSFlex7Ctl.VSFlexGrid fgOpt 
      Height          =   2055
      Left            =   0
      TabIndex        =   3
      Top             =   1800
      Width           =   7575
      _cx             =   13361
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
   Begin VSFlex7Ctl.VSFlexGrid fgUnd 
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
   Begin VSFlex7Ctl.VSFlexGrid fgDiv 
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
   Begin VSFlex7Ctl.VSFlexGrid fgVol 
      Height          =   495
      Left            =   0
      TabIndex        =   2
      Top             =   1200
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
   Begin VB.Image imgBadPrice 
      Height          =   240
      Left            =   4620
      Picture         =   "ctlQuotesViewSingle.ctx":0000
      Top             =   4920
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
      Begin VB.Menu mnuCtxOtcOptionCalc 
         Caption         =   "OTC Option Calc..."
         Shortcut        =   ^{F12}
      End
      Begin VB.Menu mnuCtxSeparator99 
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
      Begin VB.Menu mnuCtxVolaFlat 
         Caption         =   "Flat Vola"
         Begin VB.Menu mnuCtxVolaFlatCur 
            Caption         =   "This Expiry"
         End
         Begin VB.Menu mnuCtxVolaFlatAll 
            Caption         =   "All Expiries"
         End
         Begin VB.Menu mnuCtxVolaFlatAllByCur 
            Caption         =   "All Expiries By This"
         End
      End
      Begin VB.Menu mnuCtxVolaFitToImp 
         Caption         =   "Fit Vola To Implied"
         Begin VB.Menu mnuCtxVolaFitToImpCur 
            Caption         =   "This Expiry"
         End
         Begin VB.Menu mnuCtxVolaFitToImpAll 
            Caption         =   "All Expiries"
         End
      End
      Begin VB.Menu mnuCtxSaveSimulatedVola 
         Caption         =   "Save Simulated Vola"
      End
      Begin VB.Menu mnuCtxSeparator61 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxCustomDividend 
         Caption         =   "Custom Dividends"
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxDefOptSort 
         Caption         =   "Default Options Sorting"
         Shortcut        =   ^D
      End
      Begin VB.Menu mnuCtxSeparator7 
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
         Caption         =   "Refresh Prices "
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
      Begin VB.Menu mnuCtxSeparator8 
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
      Begin VB.Menu mnuCtxSeparator9 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlQuotesViewSingle"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event OnSetCaption()
Public Event OnStateChange()
Public Event OnExpiriesChange(ByVal bStateOnly As Boolean)
Public Event OnOptExchangesChange(ByVal bStateOnly As Boolean)
Public Event OnUndExchangesChange(ByVal bStateOnly As Boolean)
Public Event OnOptRootsChange(ByVal bStateOnly As Boolean)
Public Event OnStrikesChange()

Private WithEvents geOpt As clsGridEx
Attribute geOpt.VB_VarHelpID = -1
Private WithEvents QV As EtsMmQuotesLib.EtsMmQuotesView
Attribute QV.VB_VarHelpID = -1


Private m_nNewGrpID&

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPriceWithNotify
Private m_BatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
Private WithEvents VolaSource As VolatilitySourcesLib.VolatilitySource
Attribute VolaSource.VB_VarHelpID = -1
Private WithEvents TradeChannel As clsTradeChannel
Attribute TradeChannel.VB_VarHelpID = -1

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As VB.Label
Public lblStatus As VB.Label

Public WithEvents imgStop As VB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As VB.Image

Private m_sUndComboList As String

Private m_gdUnd As New clsGridDef
Private m_gdDiv As New clsGridDef
Private m_gdVol As New clsGridDef
Private m_gdOpt As New clsGridDef

Private m_bShutDown As Boolean
Private m_nUndMainRow As Long

Private m_bInProc As Boolean
Private m_bInRealTimeCalc As Boolean
Private m_bLastQuoteReqNow As Boolean
Private m_bSubscribingNow As Boolean
Private m_bDataLoad As Boolean
Private m_bFireEvent As Boolean
Private m_bInitializing As Boolean


Private m_bIsReload As Boolean


Private m_nQuoteReqCount As Long
Private m_nQuoteReqDone As Long
Private m_nQuoteGroupReqCount As Long
Private m_nQuoteGroupReqDone As Long
Private m_DecExpectations As Long

Private m_nLastRecalcCycle As Long
Private m_nLastOutCycle As Long
Private m_nLastDataSize As Long

Private m_bVolaChangedNow As Boolean
Private m_bVolaChangedExt As Boolean
Public ProcessRealTime As Boolean

Private m_nCurrentExpiryIdx As Long
Private m_nCurrentOptExchIdx As Long
Private m_nCurrentOptRootIdx As Long

Public m_frmOwner As Form

Private m_sCurrentOriginalText As String
Private m_bKeyDown(GT_QUOTES_UNDERLYING To GT_QUOTES_OPTIONS) As Boolean

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bGroupRequestSupported As Boolean
Private m_bGroupRequest As Boolean
Private m_bUndListReloadNow As Boolean

Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Private m_nOperation As Long


Private m_Aux As New clsAuxQuotesView
Private m_AuxOut As New clsAuxQuotesViewOut

'Private TradeQueue As clsTradeQueueColl

Private m_bCalculationDateChanged As Boolean
Private m_bDateChanged As Boolean
Private m_frmCustDivs As frmCustomDivs
Private m_bInRefreshMode As Boolean
Private m_bFiersTealtimeCalculation As Boolean

Private m_bIsDblClick As Boolean


Public Function Init() As Boolean
    On Error GoTo EH
    
    LoadParams
    
    m_bShutDown = False
    m_bVolaChangedNow = False
    m_bVolaChangedExt = False
    m_bInRefreshMode = False
    m_bFiersTealtimeCalculation = False
    m_bIsDblClick = False
    
    m_bIsReload = False

    m_bCalculationDateChanged = False
    m_bDateChanged = False

    m_DecExpectations = 0
    
    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxVolaFlatCur.Caption = "This Expiry by ATM" & vbTab & "Ctrl+F"
    mnuCtxVolaFlatAll.Caption = "All Expiries by Their ATM" & vbTab & "Ctrl+Shift+F"
    mnuCtxVolaFlatAllByCur.Caption = "All Expiries by This ATM" & vbTab & "Ctrl+Alt+F"
    mnuCtxVolaFitToImpCur.Caption = "This Expiry" & vbTab & "Ctrl+Shift+I"
    mnuCtxVolaFitToImpAll.Caption = "All Expiries" & vbTab & "Ctrl+Alt+I"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    Set VolaSource = g_VolaSource
    InitColumns
    
'    Set TradeQueue = New clsTradeQueueColl
    
    Set m_Aux.gdUnd = m_gdUnd
    Set m_Aux.gdDiv = m_gdDiv
    Set m_Aux.gdVol = m_gdVol
    Set m_Aux.gdOpt = m_gdOpt
    Set m_Aux.fgUnd = fgUnd
    Set m_Aux.fgDiv = fgDiv
    Set m_Aux.fgVol = fgVol
    Set m_Aux.fgOpt = fgOpt
    m_Aux.RealTime = False
    m_Aux.RealTimeConnected = False
    
    m_AuxOut.Init m_Aux
    Set m_AuxOut.gdUnd = m_gdUnd
    Set m_AuxOut.gdDiv = m_gdDiv
    Set m_AuxOut.gdVol = m_gdVol
    Set m_AuxOut.gdOpt = m_gdOpt
    Set m_AuxOut.fgUnd = fgUnd
    Set m_AuxOut.fgDiv = fgDiv
    Set m_AuxOut.fgVol = fgVol
    Set m_AuxOut.fgOpt = fgOpt
    Set m_AuxOut.imgBadPrice = imgBadPrice
    
    m_bKeyDown(GT_QUOTES_UNDERLYING) = False
    m_bKeyDown(GT_QUOTES_DIVIDENDS) = False
    m_bKeyDown(GT_QUOTES_VOLA) = False
    m_bKeyDown(GT_QUOTES_OPTIONS) = False
    
    m_Aux.GridLock(GT_QUOTES_UNDERLYING).Init fgUnd
    m_Aux.GridLock(GT_QUOTES_DIVIDENDS).Init fgDiv
    m_Aux.GridLock(GT_QUOTES_VOLA).Init fgVol
    m_Aux.GridLock(GT_QUOTES_OPTIONS).Init fgOpt
    
    m_Aux.SortColKey = -2
    m_Aux.SortColOrder = flexSortGenericAscending
    m_Aux.InitGrids
    
    Set geOpt = New clsGridEx
    geOpt.Init fgOpt
    Set m_Aux.geOpt = geOpt
    
    ResetMenuData
    
    m_bInitializing = False
    m_bLastQuoteReqNow = False
    m_bSubscribingNow = False
    m_Aux.UseCustRates = False
    m_Aux.IsAmerican = -1
    m_bFireEvent = False
    
    m_Aux.CalcModelVisible = True
    m_Aux.ExpCalendarVisible = True
    m_Aux.DividendsVisible = True
    m_Aux.VolaVisible = True
    m_Aux.RatesVisible = True
    m_Aux.bFlatVolaToATM = False
    
    InitView
   
    Set TradeChannel = g_TradeChannel
    
    ClearViewAndData
    InitUndList
    
    fgUnd.Col = 1
    fgUnd.Row = 1
    Init = True
    UpdateTotals
    AdjustState
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    Set QV.BatchPriceProvider = PriceProvider

    m_bGroupRequestSupported = g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequestSupported Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    Set m_BatchPriceProvider = PriceProvider
    If m_BatchPriceProvider Is Nothing Then LogEvent EVENT_ERROR, "Selected price provider is not compatible with Risk view."
    
    Set frmLayout = New frmGridLayout
    Set aParams = g_Params
    
    Set m_frmCustDivs = New frmCustomDivs
    m_frmCustDivs.InitRs
    
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to init quotes view."
End Function
Private Sub InitView()
    On Error GoTo EH
    
    Set QV = Nothing
    Set m_Aux.QV = Nothing
    Set m_Aux.Grp = Nothing
    Set m_Aux.QuoteReqsAll = Nothing
    
    Set m_Aux.QV = New EtsMmQuotesLib.EtsMmQuotesView
    Set QV = m_Aux.QV
    Set QV.BatchPriceProvider = PriceProvider
    Set m_Aux.Grp = m_Aux.QV.Grp
    Set m_Aux.QuoteReqsAll = m_Aux.QV.QuoteReqsAll
    
    QV.ProcessorsLoad = g_Params.ProcessorsLoad
   
Exit Sub
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to init quotes view."

End Sub


Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If nType <> TYPE_UNDERLYING Or nValue = 0 Then Exit Sub
    Dim aContract As EtsGeneralLib.EtsContractAtom
    
    m_nNewGrpID = 0
    Set aContract = g_ContractAll(nValue)
    If Not aContract Is Nothing Then
        m_nNewGrpID = aContract.ID
        Set aContract = Nothing
    
        QV.VisibleUndExch.Clear
        If Not g_Params.QuoteUndExchAll Then
            QV.VisibleUndExch.Add(Trim$(Str$(g_Params.QuoteDefUndExchID))).ID = g_Params.QuoteDefUndExchID
        End If
        
        QV.VisibleOptExch.Clear
        If Not g_Params.QuoteOptExchAll Then
            QV.VisibleOptExch.Add(Trim$(Str$(g_Params.QuoteDefOptExchID))).ID = g_Params.QuoteDefOptExchID
        End If
    End If
    
    If m_nNewGrpID <> 0 And Not m_bShutDown Then
        m_bInRefreshMode = False
        tmrShow.Enabled = True
    End If
End Sub

Private Sub InitUndList()
    On Error Resume Next
    Dim sValue$, nValue&, nCol&
    Dim aContract As EtsGeneralLib.EtsContractAtom
    
    With fgUnd
        nCol = .ColIndex(QUC_SYMBOL)
        If nCol > 0 And nCol < .Cols Then
            m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
            
            sValue = "0"
            nValue = 0
            m_sUndComboList = g_Params.UnderlyingWithFuturesComboListAll
            
            If Len(m_sUndComboList) > 0 Then
                m_sUndComboList = "|" & m_sUndComboList
            Else
                m_sUndComboList = "#0;<None>"
            End If
            
            Set aContract = g_ContractAll(m_Aux.Grp.ID)
            If Not aContract Is Nothing Then
                nValue = aContract.ID
                sValue = aContract.Symbol
                Set aContract = Nothing
            End If
        
            If nValue <> 0 Then .TextMatrix(m_nUndMainRow, nCol) = sValue
            
            m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
        End If
    End With
End Sub

Public Sub ModelUpdate()
    On Error Resume Next
    m_AuxOut.ModelUpdate
End Sub

Private Function AddExch(ByRef aExchColl As EtsGeneralLib.ExchColl, ByVal nID As Long, ByVal sCode As String, ByVal sName As String) As EtsGeneralLib.ExchAtom
    On Error GoTo EH
    Dim aExch As EtsGeneralLib.ExchAtom
    Set AddExch = Nothing
    
    Set aExch = New EtsGeneralLib.ExchAtom
    aExch.ID = nID
    aExch.Code = sCode
    aExch.Name = sName
    aExchColl.Add aExch.ID, aExch.Code, aExch
    Set AddExch = aExch
Ex:
    Set aExch = Nothing
    Exit Function
EH:
    Set AddExch = Nothing
    gCmn.ErrorHandler "Fail to add exchange."
    GoTo Ex
End Function

Private Function InitGroupExchanges() As Boolean
    On Error GoTo EH
    Dim rsExch As ADODB.Recordset, aExch As EtsGeneralLib.ExchAtom, nID&
    
    InitGroupExchanges = False
    If m_Aux.Grp.ID = 0 Then Exit Function
    
    Dim aUndExchAtom As ExchAtom
    Dim aOptExchAtom As ExchAtom
    
    
    Set aUndExchAtom = AddExch(m_Aux.Grp.UndExchAll, 0, "", "<Primary>")
    Set aOptExchAtom = AddExch(m_Aux.Grp.OptExchAll, 0, "", "<Primary>")
    
    m_Aux.Grp.Und.UndExch.Add aUndExchAtom.ID, aUndExchAtom.Code, aUndExchAtom
    m_Aux.Grp.Und.OptExch.Add aOptExchAtom.ID, aOptExchAtom.Code, aOptExchAtom
   
    Select Case m_Aux.Grp.ContractType
        Case enCtIndex, enCtStock
            Set rsExch = gDBW.usp_MmUnderlyingExchanges_Get(m_Aux.Grp.ID)
        
            Do While Not rsExch.EOF
                nID = ReadLng(rsExch!iExchangeID)
                Set aExch = g_Exch(nID)
                If Not aExch Is Nothing Then
        
                    If ReadByte(rsExch!tiIsUnderlying) <> 0 Then
                        If m_Aux.Grp.UndExchAll(nID) Is Nothing Then
                          Set aUndExchAtom = AddExch(m_Aux.Grp.UndExchAll, nID, aExch.Code, aExch.Name)
                          m_Aux.Grp.Und.UndExch.Add aUndExchAtom.ID, aUndExchAtom.Code, aUndExchAtom
                        End If
                    End If
        
                    If ReadByte(rsExch!tiIsOption) <> 0 Then
                        If m_Aux.Grp.OptExchAll(nID) Is Nothing Then
                          Set aOptExchAtom = AddExch(m_Aux.Grp.OptExchAll, nID, aExch.Code, aExch.Name)
                          m_Aux.Grp.Und.OptExch.Add aOptExchAtom.ID, aOptExchAtom.Code, aOptExchAtom
                        End If
                    End If
        
                    Set aExch = Nothing
                End If
                
                DoEvents
                If Not m_bDataLoad Then GoTo Ex
                rsExch.MoveNext
            Loop
            
        Case enCtFuture
            Set rsExch = gDBW.usp_MmFutureExchanges_Get(m_Aux.Grp.ID)
            AddExch m_Aux.Grp.Fut.FutExch, 0, "", "<Primary>"
            AddExch m_Aux.Grp.Fut.OptExch, 0, "", "<Primary>"
           
            Do While Not rsExch.EOF
                nID = ReadLng(rsExch!iExchangeID)
                Set aExch = g_Exch(nID)
                If Not aExch Is Nothing Then
        
                    If ReadByte(rsExch!tiIsUnderlying) <> 0 Then
                        If m_Aux.Grp.UndExchAll(nID) Is Nothing Then AddExch m_Aux.Grp.UndExchAll, nID, aExch.Code, aExch.Name
                        If m_Aux.Grp.Fut.FutExch(nID) Is Nothing Then AddExch m_Aux.Grp.Fut.FutExch, nID, aExch.Code, aExch.Name
                    End If
        
                    If ReadByte(rsExch!tiIsOption) <> 0 Then
                        If m_Aux.Grp.OptExchAll(nID) Is Nothing Then AddExch m_Aux.Grp.OptExchAll, nID, aExch.Code, aExch.Name
                        If m_Aux.Grp.Fut.OptExch(nID) Is Nothing Then AddExch m_Aux.Grp.Fut.OptExch, nID, aExch.Code, aExch.Name
                    End If
        
                    Set aExch = Nothing
                End If
                
                DoEvents
                If Not m_bDataLoad Then GoTo Ex
                rsExch.MoveNext
            Loop
            
        Case Else
            GoTo Ex
    End Select
    
    InitGroupExchanges = True
Ex:
    Set rsExch = Nothing
    Set aExch = Nothing
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlying."
    GoTo Ex
End Function

Private Sub InitUnderlying(ByRef aContract As EtsGeneralLib.EtsContractAtom)
    On Error Resume Next
    Dim aIdx As EtsGeneralLib.IndexAtom, sKey$
    
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    m_Aux.Grp.Und.ID = aContract.Und.ID
    m_Aux.Grp.Und.Symbol = aContract.Und.Symbol
    m_Aux.Grp.Und.SymbolName = aContract.Und.ContractName
    m_Aux.Grp.Und.UndType = aContract.Und.UndType
    m_Aux.Grp.Und.IsTraderContract = aContract.Und.IsTraderContract
    
    InitVola m_Aux.Grp.Und
    
    m_Aux.Grp.Und.IsHTB = aContract.Und.IsHTB
    m_Aux.Grp.Und.Skew = aContract.Und.Skew
    m_Aux.Grp.Und.Kurt = aContract.Und.Kurt
    
    Set m_Aux.Grp.Und.UndPriceProfile = aContract.Und.UndPriceProfile
    Set m_Aux.Grp.Und.OptPriceProfile = aContract.Und.OptPriceProfile
    
    m_Aux.Grp.Und.UndPosForRates = 0#
    
    If m_Aux.IsAmerican < 0 Then
        m_Aux.Grp.Und.IsAmerican = aContract.Und.IsAmerican
    Else
        m_Aux.Grp.Und.IsAmerican = (m_Aux.IsAmerican <> 0)
    End If
    
    m_Aux.Grp.UseCustRates = m_Aux.UseCustRates
    
    If (Not aContract.Und.Dividend Is Nothing) Then
        Set m_Aux.Grp.Und.Dividend = aContract.Und.Dividend
        If m_Aux.Grp.Und.Dividend.DivDate = 0 Then m_Aux.Grp.Und.Dividend.DivDate = Date
        If m_Aux.Grp.Und.Dividend.DivDateCust = 0 Then m_Aux.Grp.Und.Dividend.DivDateCust = Date
        
        LoadCustomDivs
    End If
   
    Set aIdx = g_Index(m_Aux.Grp.Und.ID)
    If Not aIdx Is Nothing Then
        Set m_Aux.Grp.Und.BasketIndex = aIdx
        Set aIdx = Nothing
    End If
        
    m_Aux.Grp.Und.Yield = aContract.Und.Yield
       
    m_Aux.Grp.Und.Quote.Clear
    m_Aux.Grp.Und.Exp.Clear
    m_Aux.Grp.Und.OptRoot.Clear

    Set aIdx = Nothing
End Sub

Private Sub InitContracts(ByRef aContract As EtsGeneralLib.EtsContractAtom)
    On Error Resume Next
    Dim sKey$
    
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    If aContract.ContractType = enCtFuture Then
        m_Aux.Grp.FutRoot.ID = aContract.FutRoot.ID
        m_Aux.Grp.FutRoot.Symbol = aContract.FutRoot.Symbol
        m_Aux.Grp.FutRoot.Name = aContract.FutRoot.Name
        m_Aux.Grp.FutRoot.FutLotSize = aContract.FutRoot.FutLotSize
        m_Aux.Grp.FutRoot.OptLotSize = aContract.FutRoot.OptLotSize
        
        m_Aux.Grp.Fut.ID = aContract.Fut.ID
        m_Aux.Grp.Fut.Symbol = aContract.Fut.Symbol
        m_Aux.Grp.Fut.ContractName = aContract.Fut.ContractName
        m_Aux.Grp.Fut.FutRootID = aContract.FutRoot.ID
        Set m_Aux.Grp.Fut.UndPriceProfile = aContract.Fut.UndPriceProfile
        Set m_Aux.Grp.Fut.OptPriceProfile = aContract.Fut.OptPriceProfile
        m_Aux.Grp.Fut.MaturityDate = aContract.Fut.MaturityDate
        m_Aux.Grp.Fut.MaturityMonth = DateSerial(Year(aContract.Fut.MaturityDate), Month(aContract.Fut.MaturityDate), 1)
        m_Aux.Grp.Fut.IsAmerican = aContract.Fut.IsAmerican

        m_Aux.Grp.Fut.Quote.Clear
        m_Aux.Grp.Fut.Exp.Clear
    End If
End Sub

Private Sub InitContractsQuotes(ByRef aContract As EtsGeneralLib.EtsContractAtom)
    On Error Resume Next
    Dim sKey$, aExch As EtsGeneralLib.ExchAtom
    Dim aReq As MmQvRequestAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    For Each aExch In m_Aux.Grp.Und.UndExch

        Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
        Set aQuote.Exch = aExch
        aQuote.LotSize = aContract.Und.LotSize
        aQuote.PriceClose = aContract.Und.PriceClose

        sKey = m_Aux.Grp.Und.Symbol
        If aExch.ID <> 0 Then
            sKey = sKey & "." & aExch.Code
        End If

        Set aReq = m_Aux.QuoteReqsAll(sKey)
        If aReq Is Nothing Then
            Set aReq = m_Aux.QuoteReqsAll.Add(sKey)
            Set aReq.Exch = aExch
            Set aReq.Und = m_Aux.Grp.Und
        End If
        Set aReq = Nothing

        m_Aux.Grp.Und.Quote.Add aExch.ID, aExch.Code, aQuote
        Set aQuote = Nothing
    Next
    
    QV.UndQuotesCount = m_Aux.Grp.Und.UndExch.Count

    If aContract.ContractType = enCtFuture Then
        For Each aExch In m_Aux.Grp.Fut.FutExch
    
            Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
            Set aQuote.Exch = aExch
            aQuote.LotSize = aContract.FutRoot.FutLotSize
    
            sKey = m_Aux.Grp.Fut.Symbol
            If aExch.ID <> 0 Then
                sKey = sKey & "." & aExch.Code
            End If
    
            Set aReq = m_Aux.QuoteReqsAll(sKey)
            If aReq Is Nothing Then
                Set aReq = m_Aux.QuoteReqsAll.Add(sKey)
                Set aReq.Exch = aExch
                Set aReq.Und = m_Aux.Grp.Und
                Set aReq.Fut = m_Aux.Grp.Fut
                Set aReq.FutRoot = m_Aux.Grp.FutRoot
            End If
            Set aReq = Nothing
    
            m_Aux.Grp.Fut.Quote.Add aExch.ID, aExch.Code, aQuote
            Set aQuote = Nothing
        Next
    
        QV.FutQuotesCount = m_Aux.Grp.Fut.FutExch.Count
    End If
End Sub

Private Function InitUnderlyingOptions(ByRef aContract As EtsGeneralLib.EtsContractAtom) As Boolean
    On Error GoTo EH
    
    QV.ConnectionString = g_Params.DbConnection
    Set QV.EtsMain = g_Main
    
    QV.Load m_Aux.Grp.ID
    
    InitUnderlyingOptions = True
Ex:
    Exit Function
EH:
    LogEvent EVENT_ERROR, "Fail to load options for " & m_Aux.Grp.Symbol & ". " & Err.Source & " " & CStr(Err.Number) & " " & Err.Description
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlying."
    GoTo Ex
End Function

Private Function InitFuturesOptions(ByRef aContract As EtsGeneralLib.EtsContractAtom) As Boolean
    On Error GoTo EH
    Dim rsOpt As ADODB.Recordset, aExch As EtsGeneralLib.ExchAtom
    Dim dtExpiry As Date, dStrike#, dtExpiryMonth As Date, aPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom, enOptType As EtsGeneralLib.EtsOptionTypeEnum, sKey$, aEnt As EtsGeneralLib.EtsMmEntityAtom
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom, aOpt As EtsMmQuotesLib.MmQvOptAtom
    Dim aStrAll As EtsMmQuotesLib.MmQvStrikeAtom, aReq As MmQvRequestAtom, aExpAll As EtsMmQuotesLib.MmQvExpAtom
    Dim dtExpiryOV As Date, dtTradingClose As Date
    Dim lSurfaceID As Long
    
    InitFuturesOptions = False
    If m_Aux.Grp.ID = 0 Then Exit Function
            
    Set rsOpt = gDBW.usp_MmOptionByFuture_Get(m_Aux.Grp.ID, Date)
    If rsOpt.RecordCount > 0 Then pbProgress.Max = rsOpt.RecordCount

    DoEvents
    If Not m_bDataLoad Then GoTo Ex
    
    Do While Not rsOpt.EOF
        dtExpiry = ReadDate(rsOpt!dtExpiry)
        dtExpiryMonth = DateSerial(Year(dtExpiry), Month(dtExpiry), Day(dtExpiry))
        dtExpiryOV = ReadDate(rsOpt!dtExpiryOV)
        dtTradingClose = ClipDays(ReadDate(rsOpt!dtTradingClose))

        Set aExp = m_Aux.Grp.Fut.Exp(dtExpiryMonth)
        If aExp Is Nothing Then
            Set aExp = m_Aux.Grp.Fut.Exp.Add(dtExpiryMonth)
            aExp.Expiry = dtExpiry
            aExp.ExpiryMonth = dtExpiryMonth
            aExp.ExpiryOV = dtExpiryOV
            aExp.TradingClose = dtTradingClose
            aExp.Visible = True
            
            Set aEnt = QV.CustRates(CStr(CLng(aExp.ExpiryMonth)))
            If Not aEnt Is Nothing Then
                aExp.RateCust = aEnt.Data3
                Set aEnt = Nothing
            End If
        End If

        Set aExpAll = m_Aux.Grp.ExpAll(dtExpiryMonth)
        If aExpAll Is Nothing Then
            Set aExpAll = m_Aux.Grp.ExpAll.Add(dtExpiryMonth)
            aExpAll.Expiry = dtExpiry
            aExpAll.ExpiryOV = dtExpiryOV
            aExpAll.TradingClose = dtTradingClose
            aExpAll.ExpiryMonth = dtExpiryMonth
            aExpAll.Visible = True
        End If
        
        If InStr(aExpAll.RootNames, m_Aux.Grp.Fut.Symbol) = 0 Then
            If Len(aExpAll.RootNames) > 0 Then
                aExpAll.RootNames = aExpAll.RootNames & "," & m_Aux.Grp.Fut.Symbol
            Else
                aExpAll.RootNames = m_Aux.Grp.Fut.Symbol
            End If
        End If

        Set aExpAll = Nothing

        dStrike = Round(ReadDbl(rsOpt!fStrike), STRIKE_DECIMALS_COUNT)
        
        Set aStr = aExp.Strike(dStrike)
        If aStr Is Nothing Then
            Set aStr = aExp.Strike.Add(dStrike)
            aStr.Strike = dStrike
            aStr.Visible = True
        End If
        
        Set aStrAll = m_Aux.Grp.StrikeAll(dStrike)
        If aStrAll Is Nothing Then
            Set aStrAll = m_Aux.Grp.StrikeAll.Add(dStrike)
            aStrAll.Strike = dStrike
            aStrAll.Visible = True
        End If
        Set aStrAll = Nothing
        
        Set aPair = aStr.OptPair(m_Aux.Grp.Fut.ID)
        If aPair Is Nothing Then
            Set aPair = New EtsMmQuotesLib.MmQvOptPairAtom
            aPair.RootID = m_Aux.Grp.Fut.ID
            
            aStr.OptPair.Add m_Aux.Grp.Fut.ID, m_Aux.Grp.Fut.Symbol, aPair
        End If

        enOptType = IIf(ReadLng(rsOpt!tiIsCall) <> 0, enOtCall, enOtPut)
        
        Set aOpt = aPair.Opt(enOptType)
        If aOpt.ID = 0 Then
            aOpt.ID = ReadLng(rsOpt!iContractID)
            aOpt.Symbol = ReadStr(rsOpt!vcSymbol)
            aOpt.OptType = enOptType
            aOpt.Expiry = dtExpiry
            aOpt.ExpiryOV = dtExpiryOV
            aOpt.TradingClose = dtTradingClose
            aOpt.Strike = dStrike
            aOpt.RootID = m_Aux.Grp.Fut.ID
            lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(m_Aux.Grp.Fut.FutRootID)
            aOpt.Vola = m_Aux.Grp.Und.VolaSrv.OptionVola(dtExpiryOV, dStrike, lSurfaceID)

            If aOpt.Vola < 0 Then
                aOpt.Vola = BAD_DOUBLE_VALUE
            End If

            m_Aux.Grp.Fut.Opt.Add aOpt.ID, aOpt
        End If

        For Each aExch In m_Aux.Grp.Fut.OptExch
            Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
            Set aQuote.Exch = aExch
            aQuote.LotSize = m_Aux.Grp.FutRoot.OptLotSize * m_Aux.Grp.FutRoot.FutLotSize
            
            aQuote.Series = UCase$(Format$(aOpt.Expiry, "YYMMM")) & Trim$(Str$(Int(aOpt.Strike)))
            sKey = aOpt.Symbol
            
            If aExch.ID = 0 Then
                Set aOpt.DefQuote = aQuote
            Else
                aQuote.Series = aQuote.Series & " " & aExch.Code
                sKey = sKey & "." & aExch.Code
            End If
            
            Set aReq = m_Aux.QuoteReqsAll(sKey)
            If aReq Is Nothing Then
                Set aReq = m_Aux.QuoteReqsAll.Add(sKey)
                Set aReq.Exch = aExch
                Set aReq.Opt = aOpt
                Set aReq.Exp = aExp
                Set aReq.Fut = m_Aux.Grp.Fut
                Set aReq.FutRoot = m_Aux.Grp.FutRoot
            End If
            Set aReq = Nothing
            
            aOpt.Quote.Add aExch.ID, aExch.Code, aQuote
            Set aQuote = Nothing
        Next

        DoEvents
        If Not m_bDataLoad Then GoTo Ex
        IncProgress pbProgress

        Set aOpt = Nothing
        Set aPair = Nothing
        Set aStr = Nothing
        Set aExp = Nothing
        
        rsOpt.MoveNext
    Loop
    
    QV.OptQuotesCount = m_Aux.Grp.Fut.Opt.Count * m_Aux.Grp.Fut.OptExch.Count
    
    InitFuturesOptions = True
Ex:
    Set rsOpt = Nothing
    Set aPair = Nothing
    Set aExp = Nothing
    Set aQuote = Nothing
    Set aExch = Nothing
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlying."
    GoTo Ex
End Function

Private Function InitGroup(ByVal nGroupID As Long) As Boolean
    On Error GoTo EH
    Dim aContract As EtsGeneralLib.EtsContractAtom

    Dim nStart&
    nStart = GetTickCount
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Data loading Enter.", m_frmOwner.GetCaption

    InitGroup = False
    
    If m_bInProc Then Exit Function
    
    Set aContract = g_ContractAll(nGroupID)
    If aContract Is Nothing Then Exit Function
    
    m_bDataLoad = True
    m_bInProc = True
    
    QV.UndQuotesCount = 0
    QV.OptQuotesCount = 0
    QV.FutQuotesCount = 0

    pbProgress.Min = 0
    pbProgress.Value = pbProgress.Min
    DoEvents
    

    m_Aux.Grp.ID = aContract.ID
    m_Aux.Grp.ContractType = aContract.ContractType
    m_Aux.Grp.Symbol = aContract.Symbol
    AdjustCaption
    AdjustState
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Data loading Process. " _
                                                & "GrpID=""" & m_Aux.Grp.ID & """ " _
                                                & "ContractType=""" & m_Aux.Grp.ContractType & """ " _
                                                & "Symbol=""" & m_Aux.Grp.Symbol & """ ", m_frmOwner.GetCaption
    
    InitUnderlying aContract
    DoEvents
    If Not m_bDataLoad Or m_bShutDown Then GoTo Ex

    InitContracts aContract
    DoEvents
    If Not m_bDataLoad Or m_bShutDown Then GoTo Ex

    If Not InitGroupExchanges Then GoTo Ex
    
    InitContractsQuotes aContract

    Select Case aContract.ContractType
        Case enCtIndex, enCtStock
            If Not InitUnderlyingOptions(aContract) Then GoTo Ex
            
        Case enCtFuture
            If Not InitFuturesOptions(aContract) Then GoTo Ex
        
        Case Else
            GoTo Ex
    End Select
    
    If m_bShutDown Then GoTo Ex
    UnderlyingUpdatePositions
    UnderlyingAdjustRates True

    InitGroup = True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Data loading Exit. Duration " & GetTickCount - nStart & " ms", m_frmOwner.GetCaption
    
Ex:
    m_bInProc = False
    m_bDataLoad = False
    Set aContract = Nothing
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlying."
    GoTo Ex
End Function

Private Sub ShowGroup()
    On Error Resume Next
    Dim aRowData As MmQvRowData, aExch As EtsGeneralLib.ExchAtom, sKey$, aPair As EtsMmQuotesLib.MmQvOptPairAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim nRow&, i&, aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim bAllExpVisible As Boolean, bAllExchVisible As Boolean, bAllRootVisible As Boolean, bShowOnlyDefExch As Boolean, aRoot As EtsMmQuotesLib.MmQvOptRootAtom
    Dim bAllStrVisible As Boolean, aStrAll As EtsMmQuotesLib.MmQvStrikeAtom, bHidden As Boolean
    
    If m_Aux.Grp.ID = 0 Or m_bShutDown Then Exit Sub
    
    AdjustState
    DoEvents
    
    m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw

    pbProgress.Max = QV.UndQuotesCount + QV.OptQuotesCount + QV.FutQuotesCount

    lblProcess.Caption = "Formatting..."
    lblProcess.Refresh
    
    With fgUnd
        
        .Rows = 1
        On Error GoTo Er
        bAllExchVisible = True
        bShowOnlyDefExch = False
        If QV.VisibleUndExch.Count > 0 Then
            For Each aExch In m_Aux.Grp.UndExchAll
                If Not QV.VisibleUndExch(Trim$(Str$(aExch.ID))) Is Nothing Then
                    bAllExchVisible = False
                    Exit For
                End If
            Next
            bShowOnlyDefExch = bAllExchVisible
        End If
        
        For Each aExch In m_Aux.Grp.UndExchAll
            If Not bAllExchVisible Then
                aExch.Visible = Not QV.VisibleUndExch(Trim$(Str$(aExch.ID))) Is Nothing
            Else
                aExch.Visible = (Not bShowOnlyDefExch Or bShowOnlyDefExch And aExch.ID = 0)
            End If
        Next
        
        AdjustUnderlyingsExchVisible
        AdjustUnderlyingsPosExchID
        On Error Resume Next
        
        If m_Aux.Grp.IsStockOrIndex Then
            For Each aExch In m_Aux.Grp.Und.UndExch
                Set aQuote = m_Aux.Grp.Und.Quote(aExch.ID)
                
                If Not aQuote Is Nothing Then
                    .AddItem ""
                    nRow = .Rows - 1
                
                    Set aRowData = New MmQvRowData
                    Set aRowData.Und = m_Aux.Grp.Und
                    Set aRowData.UndQuote = aQuote
                    Set aRowData.Exch = aExch
                    .RowData(nRow) = aRowData
    
                    sKey = aRowData.Und.Symbol
                    If aExch.ID <> 0 Then sKey = sKey & "." & aExch.Code
                    .TextMatrix(nRow, QUC_KEY) = sKey
                    
                    m_AuxOut.UnderlyingUpdateQuote nRow, True, False
    
                    .RowHidden(nRow) = Not aRowData.Exch.Visible
    
                    Set aRowData = Nothing
                    IncProgress pbProgress
                
                    Set aQuote = Nothing
                End If
            Next
            
            If Not m_Aux.Grp.Und.SynthUnd Is Nothing Then
                For Each aUnd In m_Aux.Grp.Und.SynthUnd
                    For Each aExch In aUnd.UndExch
                        Set aQuote = aUnd.Quote(aExch.ID)
                        If Not aQuote Is Nothing Then
                            .AddItem ""
                            nRow = .Rows - 1
                                                    
                            Set aRowData = New MmQvRowData
                            Set aRowData.Und = aUnd
                            Set aRowData.UndQuote = aQuote
                            Set aRowData.Exch = aExch
                            .RowData(nRow) = aRowData
            
                            sKey = aRowData.Und.Symbol
                            If aExch.ID <> 0 Then sKey = sKey & "." & aExch.Code
                            .TextMatrix(nRow, QUC_KEY) = sKey
                            
                            m_AuxOut.UnderlyingUpdateQuote nRow, True, False
            
                            .RowHidden(nRow) = Not aRowData.Exch.Visible
        
                            Set aRowData = Nothing
                            IncProgress pbProgress
                            
                            Set aQuote = Nothing
                        End If
                    Next
                Next
            End If
        Else
            For Each aExch In m_Aux.Grp.Fut.FutExch
                Set aQuote = m_Aux.Grp.Fut.Quote(aExch.ID)
                
                If Not aQuote Is Nothing Then
                    .AddItem ""
                    nRow = .Rows - 1
                
                    Set aRowData = New MmQvRowData
                    Set aRowData.Und = m_Aux.Grp.Und
                    Set aRowData.Fut = m_Aux.Grp.Fut
                    Set aRowData.FutRoot = m_Aux.Grp.FutRoot
                    Set aRowData.UndQuote = aQuote
                    Set aRowData.Exch = aExch
                    .RowData(nRow) = aRowData
    
                    sKey = aRowData.Fut.Symbol
                    If aExch.ID <> 0 Then sKey = sKey & "." & aExch.Code
                    .TextMatrix(nRow, QUC_KEY) = sKey
                    
                    m_AuxOut.UnderlyingUpdateQuote nRow, True, False
    
                    .RowHidden(nRow) = Not aRowData.Exch.Visible
    
                    Set aRowData = Nothing
                    IncProgress pbProgress
                    
                    Set aQuote = Nothing
                End If
            Next
        End If
        
        m_Aux.FormatUndColumns
        m_AuxOut.UnderlyingUpdateColors
        m_AuxOut.UnderlyingUpdateTotals
    End With

    m_AuxOut.RatesUpdate
    m_Aux.AdjustDivsAndRatesVisible
    m_Aux.AdjustVolaVisible
    m_AuxOut.ProfilesUpdate
    m_AuxOut.DivsUpdate
    m_AuxOut.ModelUpdate
    m_AuxOut.CalendarUpdate
    m_AuxOut.VolaUpdate
    
    With fgOpt
        .Rows = 1

        AdjustOptRootVisible
        
        QV.OptsRefresh True
        QV.OptsColumnsOrder = m_Aux.gdOpt.IdxCopy
        
        AdjustOptExchVisible
        AdjustOptPosExchIDs
        
        .FlexDataSource = QV
                
        QV.VisibleExp.Clear
        QV.VisibleRoot.Clear
        QV.VisibleStr.Clear
        
        m_Aux.FormatOptColumns
        UserControl_Resize
    
        i = -1
        i = .ColIndex(QOC_C_SERIES)
        If i < 0 Then i = .ColIndex(QOC_P_SERIES)
        geOpt.ShowSortImage i, 1
        
        If g_Params.QuoteOptExpirySeparation Then m_AuxOut.OptionsUpdateBackColor True
    End With
    
    QV.VisibleExp.Clear
    QV.VisibleRoot.Clear
    QV.VisibleUndExch.Clear
    QV.VisibleOptExch.Clear
    QV.VisibleStr.Clear
    QV.CustRates.Clear
    m_Aux.UseCustRates = False
    m_Aux.IsAmerican = -1
    
    AdjustCaption
    Set aRowData = Nothing
    
    m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
    
    imgStop.Visible = True
    imgStopDis.Visible = False
    lblProcess.Caption = ""
    m_bInProc = False
    AdjustState
Exit Sub
Er:
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogWarning, "Show Group " & Err.Description, m_frmOwner.GetCaption
    
End Sub

Private Sub UnderlyingUpdatePositions()
    On Error Resume Next
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom
    Dim i&, nCount&, aTrd As EtsGeneralLib.MmTradeInfoAtom, aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, nBS&
    
    If m_Aux.Grp.IsStockOrIndex Then
        m_Aux.Grp.Und.Qty = BAD_LONG_VALUE
        m_Aux.Grp.Und.QtyInShares = BAD_LONG_VALUE
        
        For Each aOpt In m_Aux.Grp.Und.Opt
            aOpt.Qty = BAD_LONG_VALUE
            aOpt.QtyInShares = BAD_LONG_VALUE
        Next
        
        QV.TradeFilter.Data(enFtSymbol) = m_Aux.Grp.Und.ID
        Set aTrdByUnd = TradeChannel.TradesByUnd(CStr(m_Aux.Grp.Und.ID)).FilterTradesCV(-1, 9, 0, Nothing, False, g_Params.DisplayZeroPositions)
        If Not aTrdByUnd Is Nothing Then
            nCount = aTrdByUnd.Count
    
            For Each aTrd In aTrdByUnd
                If aTrd.CheckByFilter(QV.TradeFilter) Then
                    nBS = IIf(aTrd.IsBuy, 1, -1)
    
                    If aTrd.ContractType = enCtOption Then
                        Set aOpt = m_Aux.Grp.Und.Opt(aTrd.ContractID)
                        If Not aOpt Is Nothing Then
                            If aOpt.Qty <= BAD_LONG_VALUE Then aOpt.Qty = 0&
                            aOpt.Qty = aOpt.Qty + aTrd.Quantity * nBS
    
                            If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0&
                            aOpt.QtyInShares = aOpt.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
    
                            Set aOpt = Nothing
                        End If
                        
                    ElseIf aTrd.ContractType = enCtIndex Or aTrd.ContractType = enCtStock Then
                        If m_Aux.Grp.Und.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Und.Qty = 0&
                        m_Aux.Grp.Und.Qty = m_Aux.Grp.Und.Qty + aTrd.Quantity * nBS
    
                        If m_Aux.Grp.Und.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Und.QtyInShares = 0&
                        m_Aux.Grp.Und.QtyInShares = m_Aux.Grp.Und.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
                    End If
                End If
                Set aTrd = Nothing
            Next
            Set aTrdByUnd = Nothing
        End If
    Else
        m_Aux.Grp.Fut.Qty = BAD_LONG_VALUE
        m_Aux.Grp.Fut.QtyInShares = BAD_LONG_VALUE
        
        For Each aOpt In m_Aux.Grp.Fut.Opt
            aOpt.Qty = BAD_LONG_VALUE
            aOpt.QtyInShares = BAD_LONG_VALUE
        Next
        
        Set aTrdByUnd = TradeChannel.TradesByFut(CStr(m_Aux.Grp.Fut.ID)).Clone
        If Not aTrdByUnd Is Nothing Then
            nCount = aTrdByUnd.Count
    
            For Each aTrd In aTrdByUnd
                If aTrd.CheckByFilter(QV.TradeFilter) Then
                    nBS = IIf(aTrd.IsBuy, 1, -1)
    
                    If aTrd.ContractType = enCtFutOption Then
                        Set aOpt = m_Aux.Grp.Fut.Opt(aTrd.ContractID)
                        If Not aOpt Is Nothing Then
                            If aOpt.Qty <= BAD_LONG_VALUE Then aOpt.Qty = 0&
                            aOpt.Qty = aOpt.Qty + aTrd.Quantity * nBS
    
                            If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0&
                            aOpt.QtyInShares = aOpt.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
    
                            Set aOpt = Nothing
                        End If
                        
                    ElseIf aTrd.ContractType = enCtFuture Then
                        If m_Aux.Grp.Fut.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.Qty = 0&
                        m_Aux.Grp.Fut.Qty = m_Aux.Grp.Fut.Qty + aTrd.Quantity * nBS
    
                        If m_Aux.Grp.Fut.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.QtyInShares = 0&
                        m_Aux.Grp.Fut.QtyInShares = m_Aux.Grp.Fut.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
                    End If
                End If
                Set aTrd = Nothing
            Next
            Set aTrdByUnd = Nothing
        End If
    End If
End Sub

Public Sub RefreshView()
    On Error Resume Next
    If m_bShutDown Or Not m_Aux.RealTime Or m_bSubscribingNow Then Exit Sub
    
    If m_BatchPriceProvider.IsQuotesUpdated Then
       If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshView: Initiate Realtime Refresh", m_frmOwner.GetCaption
       tmrRealTime.Enabled = True
    Else
           If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RefreshView: No Realtime Refresh needed", m_frmOwner.GetCaption
    End If
End Sub

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_QUOTES_UNDERLYING).CopyTo m_gdUnd
    g_DefGrids(GT_QUOTES_DIVIDENDS).CopyTo m_gdDiv
    g_DefGrids(GT_QUOTES_VOLA).CopyTo m_gdVol
    g_DefGrids(GT_QUOTES_OPTIONS).CopyTo m_gdOpt
End Sub

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Private Sub UnderlyingCalculate()
    On Error Resume Next
    m_bInProc = True
    AdjustState
    
    CalculateUnderlyingOptions True
                
    m_bInProc = False
    AdjustState
    
    m_AuxOut.UnderlyingUpdate True, True
    m_AuxOut.ProfilesUpdate
    m_AuxOut.DivsUpdate
    m_AuxOut.CalendarUpdate
    m_AuxOut.ModelUpdate
    m_AuxOut.OptionsUpdate False, True, True
    m_AuxOut.VolaUpdateValues
    m_AuxOut.UnderlyingUpdateTotals
    UpdateTotals
End Sub

Private Sub aParams_PriceProfilesChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
        
    ElseIf Not m_bSubscribingNow Then
        UnderlyingCalculate
    End If
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
        
    ElseIf Not m_bSubscribingNow Then
        UnderlyingCalculate
    End If
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ID = 0 Or m_bFireEvent Then Exit Sub
    Dim aUndFilterAtom As EtsGeneralLib.EtsMmFilterAtom, aFutFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    Dim aContract As EtsGeneralLib.EtsContractAtom, bRecalc As Boolean: bRecalc = False
    
    Set aUndFilterAtom = collUpdUnd(CStr(m_Aux.Grp.Und.ID))
    If Not m_Aux.Grp.IsStockOrIndex Then Set aFutFilterAtom = collUpdUnd(CStr(m_Aux.Grp.Fut.ID))
    
    If aUndFilterAtom Is Nothing And aFutFilterAtom Is Nothing Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
    Else
        If Not aUndFilterAtom Is Nothing Then
            Set aContract = g_ContractAll(m_Aux.Grp.Und.ID)
            If Not aContract Is Nothing Then
                m_Aux.Grp.Und.IsAmerican = aContract.Und.IsAmerican
                Set m_Aux.Grp.Und.UndPriceProfile = aContract.Und.UndPriceProfile
                Set m_Aux.Grp.Und.OptPriceProfile = aContract.Und.OptPriceProfile
                Set m_Aux.Grp.Und.Dividend = aContract.Und.Dividend
                m_Aux.Grp.Und.Yield = aContract.Und.Yield
                m_Aux.Grp.Und.Skew = aContract.Und.Skew
                m_Aux.Grp.Und.Kurt = aContract.Und.Kurt
                m_Aux.Grp.Und.IsHTB = aContract.Und.IsHTB
                
                bRecalc = True
            End If
        End If
        
        If Not m_Aux.Grp.IsStockOrIndex And Not aFutFilterAtom Is Nothing Then
        
            Set aContract = g_ContractAll(m_Aux.Grp.Fut.ID)
            If Not aContract Is Nothing Then
                m_Aux.Grp.Fut.IsAmerican = aContract.Fut.IsAmerican
                Set m_Aux.Grp.Fut.UndPriceProfile = aContract.Fut.UndPriceProfile
                Set m_Aux.Grp.Fut.OptPriceProfile = aContract.Fut.OptPriceProfile
                
                bRecalc = True
            End If
        End If
        
        If bRecalc And Not m_bSubscribingNow Then UnderlyingCalculate
        
        Set aUndFilterAtom = Nothing
        Set aFutFilterAtom = Nothing
        Set aContract = Nothing
    End If
End Sub

Private Sub aParams_UndPriceToleranceChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not m_Aux.RealTime Then
        SetRefreshHint True
        
    ElseIf Not m_bSubscribingNow Then
        UnderlyingCalculate
    End If
End Sub


Private Sub dtCalculationDate_Change()
    fgVol.Text = dtCalculationDate.Value
    m_bDateChanged = True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter changed date. New date: " & dtCalculationDate.Value, m_frmOwner.GetCaption
End Sub

Private Sub dtCalculationDate_KeyDown(KeyCode As Integer, Shift As Integer)
    Select Case KeyCode
        Case vbKeyEscape
            fgVol = dtCalculationDate.Tag
            dtCalculationDate.Visible = False
        Case vbKeyReturn
            dtCalculationDate.Visible = False
    End Select
    Dim nUnds&, nOpts&
    If m_bDateChanged And Not dtCalculationDate.Visible And Not m_Aux.RealTime And m_Aux.Grp.ID <> 0 Then
        m_bInProc = True
        CalculateUnderlyingOptions True, , , True
        m_bInProc = False
        nUnds = m_AuxOut.UnderlyingUpdate(False, True)
        nOpts = m_AuxOut.OptionsUpdate(False, True, True)
        m_AuxOut.VolaUpdateValues
        m_bCalculationDateChanged = True
        m_bDateChanged = False
        AdjustState
        m_AuxOut.UnderlyingUpdateTotals
        UpdateTotals
   End If

End Sub

Private Sub dtCalculationDate_LostFocus()
    dtCalculationDate.Visible = False
    Dim nUnds&, nOpts&
    If m_bDateChanged And Not dtCalculationDate.Visible And Not m_Aux.RealTime And m_Aux.Grp.ID <> 0 Then
        m_bInProc = True
        CalculateUnderlyingOptions True, , , True
        m_bInProc = False
        nUnds = m_AuxOut.UnderlyingUpdate(False, True)
        nOpts = m_AuxOut.OptionsUpdate(False, True, True)
        m_AuxOut.VolaUpdateValues
        m_bCalculationDateChanged = True
        m_bDateChanged = False
       
        AdjustState
        m_AuxOut.UnderlyingUpdateTotals
        UpdateTotals
   End If
End Sub

Private Sub fgDiv_DblClick()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter DblClick Enter", m_frmOwner.GetCaption
    
    With fgDiv
        m_enMenuGrid = GT_QUOTES_DIVIDENDS
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter DblClick Exit", m_frmOwner.GetCaption

End Sub

Private Sub fgDiv_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_QUOTES_DIVIDENDS) = True
    End If
End Sub

Private Sub fgDiv_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    
  
    If m_bKeyDown(GT_QUOTES_DIVIDENDS) Then
        m_bKeyDown(GT_QUOTES_DIVIDENDS) = False
        With fgDiv
            m_enMenuGrid = GT_QUOTES_DIVIDENDS
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter Hot key Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter Hot key Alt+Ctrl+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter Hot key Alt+Shift+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter Hot key Ctrl+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxCopy_Click
        End Select
    End If
End Sub

Private Sub fgOpt_AfterSelChange(ByVal OldRowSel As Long, ByVal OldColSel As Long, ByVal NewRowSel As Long, ByVal NewColSel As Long)
    On Error Resume Next
    m_AuxOut.SelectedRowKey = fgOpt.TextMatrix(NewRowSel, QOC_C_KEY)
End Sub

Private Sub fgOpt_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&
    
    If Col >= 0 And Row < 0 Then
        With fgOpt
            nIdx = .ColKey(Col)
            If nIdx >= QOC_C_SYMBOL And nIdx <= QOC_LAST_COLUMN Then
                m_gdOpt.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
            End If
        End With
    End If
End Sub

Private Sub fgUnd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&
    
    If Col >= 0 And Row < 0 Then
        With fgUnd
            nIdx = .ColKey(Col)
            If nIdx >= QUC_SYMBOL And nIdx <= QUC_LAST_COLUMN Then
                m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
            End If
        End With
    End If
End Sub

Private Sub fgUnd_SetupEditStyle(ByVal Row As Long, ByVal Col As Long, ByVal IsCombo As Boolean, style As Long, StyleEx As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Row = m_nUndMainRow And fgUnd.ColKey(Col) = QUC_SYMBOL Then style = style Or CBS_UPPERCASE
End Sub

Private Sub fgVol_BeforeScroll(ByVal OldTopRow As Long, ByVal OldLeftCol As Long, ByVal NewTopRow As Long, ByVal NewLeftCol As Long, Cancel As Boolean)
    If dtCalculationDate.Visible Then Cancel = True
End Sub

Private Sub fgVol_DblClick()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgVol
        m_enMenuGrid = GT_QUOTES_VOLA
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With
End Sub

Private Sub fgVol_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) _
            Or (KeyCode = vbKeyF And (Shift = vbCtrlMask _
                                    Or Shift = vbCtrlMask + vbShiftMask _
                                    Or Shift = vbCtrlMask + vbAltMask)) _
            Or (KeyCode = vbKeyI And (Shift = vbCtrlMask + vbShiftMask _
                                    Or Shift = vbCtrlMask + vbAltMask)) Then
        m_bKeyDown(GT_QUOTES_VOLA) = True
    End If
End Sub

Private Sub fgVol_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bKeyDown(GT_QUOTES_VOLA) Then
        m_bKeyDown(GT_QUOTES_VOLA) = False
        With fgVol
            m_enMenuGrid = GT_QUOTES_VOLA
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Alt+Ctrl+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Alt+Shift+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxTntCardNew_Click
            
            Case KeyCode = vbKeyF And Shift = vbCtrlMask _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Ctrl+F pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFlatCur_Click
                
            Case KeyCode = vbKeyF And Shift = vbCtrlMask + vbShiftMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Ctrl+Shift+F pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFlatAll_Click
                
            Case KeyCode = vbKeyF And Shift = vbCtrlMask + vbAltMask _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Alt+Ctrl+F pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFlatAllByCur_Click
                
            Case KeyCode = vbKeyI And Shift = vbCtrlMask + vbShiftMask _
                And m_nMenuGridCol = 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Ctrl+Shift+I pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFitToImpCur_Click
            
            Case KeyCode = vbKeyI And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Ctrl+Alt+I pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFitToImpAll_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter. Hot key Ctrl+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxCopy_Click
        End Select
    End If
End Sub

Private Sub fgVol_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Button = vbRightButton Then
        With fgVol
            m_enMenuGrid = GT_QUOTES_VOLA
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            
            ShowPopup
        End With
    End If
End Sub

Private Sub fgVol_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error GoTo Erx
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter AfterEdit Enter", m_frmOwner.GetCaption
        
    Dim lSurfaceID As Long
        
    If m_bShutDown Then Exit Sub
        
    If HandleSimulatedVolatility(Row, Col) Then
        Exit Sub
    End If
    
    Dim sValue$, dValue#, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, nRow&, nOptType&, aPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom, aExp As EtsMmQuotesLib.MmQvExpAtom, dAtmVola#
    Dim aStr As EtsMmQuotesLib.MmQvStrikeAtom, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim nCallMask&, nPutMask&
    
    nCallMask = GetGreeksMask(enOtCall)
    nPutMask = GetGreeksMask(enOtPut)
    
    If m_Aux.Grp.IsStockOrIndex Then
        Set aExpColl = m_Aux.Grp.Und.Exp
    Else
        Set aExpColl = m_Aux.Grp.Fut.Exp
    End If
    
    With fgVol
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue And Col > 0 And Col <= aExpColl.Count Then
            Set aExp = .ColData(Col)
            If Not aExp Is Nothing Then
                If True Then
                    dValue = Abs(ReadDbl(sValue))
                    dValue = dValue / 100#
                    
                    lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(0)
                    dAtmVola = m_Aux.Grp.Und.AtmVola(aExp, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, False, lSurfaceID)
                    
                    If dAtmVola <> dValue Then
                        If Not g_PerformanceLog Is Nothing Then _
                            g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter Changed Volatility. " _
                                                                        & "Date=""" & aExp.ExpiryMonth & """ " _
                                                                        & "OldValue=""" & dAtmVola * 100 & """ " _
                                                                        & "NewValue=""" & dValue * 100 & """. " & GetStockInfo, m_frmOwner.GetCaption
                                                                                                            
                        Screen.MousePointer = vbHourglass
                        
                        m_Aux.Grp.Und.VolaSrv.ShiftExpiryVola aExp.ExpiryOV, 100# * (dValue - dAtmVola), lSurfaceID
                        
                        m_bVolaChangedNow = True
                        g_Params.FireInternalVolatilityChanged m_Aux.Grp.Und.Symbol
                        m_bVolaChangedNow = False
                        
                        For Each aStr In aExp.Strike
                            If m_bShutDown Then Exit Sub
                            dValue = m_Aux.Grp.Und.VolaSrv.OptionVola(aExp.ExpiryOV, aStr.Strike, lSurfaceID)
                            For Each aPair In aStr.OptPair
                                For nOptType = enOtPut To enOtCall
                                    If m_bShutDown Then Exit Sub
                                    Set aOpt = aPair.Opt(nOptType)
                                    If aOpt.ID <> 0 Then
                                        aOpt.Vola = dValue
                                    
                                        For Each aQuote In aOpt.Quote
                                            If Not m_bInRealTimeCalc And g_Params.UseTheoVolatility Or aQuote.CalcGreeksIVType = enMmQvCalcGreeksIVTheo Then
                                                CalcOptionGreeks aOpt, aQuote, aExp, _
                                                    IIf(aOpt.OptType = enOtCall, nCallMask, nPutMask), GetIvMask(nOptType), , False
                                            End If
                                        Next
                                    End If
                                    Set aOpt = Nothing
                                Next
                                Set aPair = Nothing
                            Next
                            Set aStr = Nothing
                        Next
                        
                        m_AuxOut.VolaUpdateValues
                        
                        fgOpt.Refresh
                        
                        Screen.MousePointer = vbDefault
                    End If
                End If
            Else
                Debug.Assert False
            End If
            Set aExp = Nothing
        End If
    End With
    
    Set aExpColl = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
         g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter AfterEdit Exit", m_frmOwner.GetCaption
Exit Sub
Erx:
    If Not g_PerformanceLog Is Nothing Then _
         g_PerformanceLog.LogMmInfo enLogFaults, "Vola Filter AfterEdit Error: " & CStr(Err.Number) & " " & Err.Description, m_frmOwner.GetCaption
End Sub

Private Sub fgVol_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter StartEdit Enter", m_frmOwner.GetCaption
    
    Dim nColData As Long: nColData = fgVol.ColData(Col)
    If nColData = QVC_VOLA_SIM Or nColData = QVC_VOLA_FLAT Then
        Cancel = False
        Exit Sub
    End If
    
    Cancel = True
    If m_bShutDown Then Exit Sub
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub
    If IsDblClickHandled Then Exit Sub
    If fgVol.ColDataType(Col) = flexDTDate And m_Aux.Grp.ID <> 0 And Not m_Aux.RealTime Then
        
        Cancel = True
        dtCalculationDate.Move fgVol.CellLeft, fgVol.CellTop + fgVol.Top, fgVol.CellWidth, fgVol.CellHeight
        dtCalculationDate.Value = fgVol
        dtCalculationDate.Tag = fgVol
        dtCalculationDate.CalendarForeColor = fgVol.Cell(flexcpForeColor, 1, fgVol.Cols - 1)
        dtCalculationDate.CalendarBackColor = fgVol.Cell(flexcpBackColor, 1, fgVol.Cols - 1)
        dtCalculationDate.Visible = True
        dtCalculationDate.SetFocus
        SendKeys "{f4}"
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter call calendar. Current date: " & dtCalculationDate.Value, m_frmOwner.GetCaption
        
    Else
        Dim aExp As EtsMmQuotesLib.MmQvExpAtom
        Cancel = (m_Aux.Grp.ID = 0 Or Not m_gdVol.Col(QVC_VOLA_VAL).CanEdit)
    
        If Not Cancel Then
            Cancel = True
            Set aExp = fgVol.ColData(Col)
            If Not aExp Is Nothing Then
                If True Then
                    m_sCurrentOriginalText = Trim$(fgVol.TextMatrix(Row, Col))
                    Cancel = False
                End If
                Set aExp = Nothing
            End If
        End If
    End If
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Vola Filter StartEdit Exit", m_frmOwner.GetCaption

End Sub

Private Sub geOpt_BeforeSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nColKey&

    If m_Aux.SortColOrder = flexSortGenericDescending Then
        QV.OptsSort Col, enSortAscending
       m_Aux.SortColOrder = flexSortGenericAscending
    Else
        QV.OptsSort Col, enSortDescending
       m_Aux.SortColOrder = flexSortGenericDescending
    End If

    geOpt.FinishSorting Col, m_Aux.SortColOrder
   
End Sub

Private Sub geOpt_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nColKey&
    nColKey = fgOpt.ColKey(Col)
    
    If g_Params.QuoteOptExpirySeparation Then m_AuxOut.OptionsUpdateBackColor True '(nColKey = QOC_C_SERIES Or nColKey = QOC_P_SERIES Or nColKey = QOC_C_EXPIRY Or nColKey = QOC_P_EXPIRY)
    
    If m_Aux.SortColKey = nColKey Then
        Order = m_Aux.SortColOrder
        m_Aux.SortColKey = -2
    End If
    
    m_AuxOut.ShowSelectedRow
End Sub

Private Sub fgOpt_Compare(ByVal Row1 As Long, ByVal Row2 As Long, Cmp As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim aOpt1 As EtsMmQuotesLib.MmQvOptAtom, aOpt2 As EtsMmQuotesLib.MmQvOptAtom, aOptQuote1 As EtsMmQuotesLib.MmQvQuoteAtom, aOptQuote2 As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aExp1 As EtsMmQuotesLib.MmQvExpAtom, aExp2 As EtsMmQuotesLib.MmQvExpAtom, aRowData1 As MmQvRowData, aRowData2 As MmQvRowData
    Dim sRootName1$, sRootName2$
    
    If m_Aux.SortColKey = QOC_C_SERIES Or m_Aux.SortColKey = QOC_P_SERIES Then
        Cmp = 0
        
        Set aRowData1 = QV.OptsRowData(Row1)
        Set aRowData2 = QV.OptsRowData(Row2)
        If Not aRowData1 Is Nothing And Not aRowData2 Is Nothing Then
            Set aExp1 = aRowData1.Exp
            Set aExp2 = aRowData2.Exp
                
            If Not aExp1 Is Nothing And Not aExp2 Is Nothing Then
                If m_Aux.SortColKey = QOC_C_SERIES Then
                    Set aOpt1 = aRowData1.Opt(enOtCall)
                    Set aOpt2 = aRowData2.Opt(enOtCall)
                    Set aOptQuote1 = aRowData1.OptQuote(enOtCall)
                    Set aOptQuote2 = aRowData2.OptQuote(enOtCall)
                    
                ElseIf m_Aux.SortColKey = QOC_P_SERIES Then
                    Set aOpt1 = aRowData1.Opt(enOtPut)
                    Set aOpt2 = aRowData2.Opt(enOtPut)
                    Set aOptQuote1 = aRowData1.OptQuote(enOtPut)
                    Set aOptQuote2 = aRowData2.OptQuote(enOtPut)
                End If
                
                If Not aOpt1 Is Nothing And Not aOpt2 Is Nothing _
                    And Not aOptQuote1 Is Nothing And Not aOptQuote2 Is Nothing Then
                    
                    If m_Aux.Grp.IsStockOrIndex Then
                        sRootName1 = aRowData1.OptRoot.Name
                        sRootName2 = aRowData2.OptRoot.Name
                    Else
                        sRootName1 = aRowData1.Fut.Symbol
                        sRootName2 = aRowData1.Fut.Symbol
                    End If
                    
                    If aExp1.ExpiryMonth > aExp2.ExpiryMonth Then
                        Cmp = 1
                    ElseIf aExp1.ExpiryMonth < aExp2.ExpiryMonth Then
                        Cmp = -1
                    ElseIf aOpt1.Strike > aOpt2.Strike Then
                        Cmp = 1
                    ElseIf aOpt1.Strike < aOpt2.Strike Then
                        Cmp = -1
                    ElseIf sRootName1 > sRootName2 Then
                        Cmp = 1
                    ElseIf sRootName1 < sRootName2 Then
                        Cmp = -1
                    ElseIf aOptQuote1.Exch.Code > aOptQuote2.Exch.Code Then
                        Cmp = 1
                    ElseIf aOptQuote1.Exch.Code < aOptQuote2.Exch.Code Then
                        Cmp = -1
                    End If
                    
                    If m_Aux.SortColOrder <> flexSortGenericAscending Then
                        Cmp = -Cmp
                    End If
                End If
            
                Set aOpt1 = Nothing
                Set aOpt2 = Nothing
                Set aOptQuote1 = Nothing
                Set aOptQuote2 = Nothing
            End If
        End If
        
        Set aExp1 = Nothing
        Set aExp2 = Nothing
    End If
    
    Set aRowData1 = Nothing
    Set aRowData2 = Nothing
End Sub

Private Sub imgStop_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Last Quotes request Cancelling.", m_frmOwner.GetCaption
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        lblStatus.Caption = "Last quotes request cancelling..."
        lblStatus.Refresh
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
            
        m_AuxOut.UnderlyingUpdate False, True
        m_AuxOut.OptionsUpdate False, True, True
        m_AuxOut.UnderlyingUpdateTotals
        
        UpdateTotals
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
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quotes subscription Cancelling.", m_frmOwner.GetCaption
        
        PriceProvider.UnSubscribeQuote
        If g_RTQuantity > 0 Then
            g_RTQuantity = g_RTQuantity - 1
        End If
        
        If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
        
        UpdateTotals
        
        AdjustState
        AdjustCaption
        
    ElseIf m_bDataLoad Then
    
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Data loading Cancelling.", m_frmOwner.GetCaption
    
        m_bDataLoad = False
        AdjustState
        
    ElseIf m_bInProc Then
        m_bInProc = False
        AdjustState
        
    End If
End Sub

Private Sub SaveProfilesInfo()
    On Error GoTo EH
    If m_Aux.Grp.ID = 0 Then Exit Sub
    Dim aContract As EtsGeneralLib.EtsContractAtom

    Set aContract = g_ContractAll(m_Aux.Grp.ID)
    If Not aContract Is Nothing Then
        If m_Aux.Grp.ContractType = enCtStock Then
            gDBW.usp_Stock_Save m_Aux.Grp.Und.ID, Null, Null, Null, Null, Null, Null, Null, _
                                Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, 0, _
                                m_Aux.Grp.Und.UndPriceProfile.ID, m_Aux.Grp.Und.OptPriceProfile.ID
                            
            Set aContract.Und.UndPriceProfile = m_Aux.Grp.Und.UndPriceProfile
            Set aContract.Und.OptPriceProfile = m_Aux.Grp.Und.OptPriceProfile
            
        ElseIf m_Aux.Grp.ContractType = enCtIndex Then
            gDBW.usp_Index_Save m_Aux.Grp.Und.ID, Null, Null, m_Aux.Grp.Und.Yield, Null, Null, Null, Null, Null, Null, Null, Null, 0, Null, Null, Null, _
                                m_Aux.Grp.Und.UndPriceProfile.ID, m_Aux.Grp.Und.OptPriceProfile.ID
                                
            Set aContract.Und.UndPriceProfile = m_Aux.Grp.Und.UndPriceProfile
            Set aContract.Und.OptPriceProfile = m_Aux.Grp.Und.OptPriceProfile
            aContract.Und.Yield = m_Aux.Grp.Und.Yield
        
        ElseIf m_Aux.Grp.ContractType = enCtFuture Then
            gDBW.usp_MmFuture_Save m_Aux.Grp.Fut.ID, Null, Null, Null, Null, _
                                m_Aux.Grp.Fut.UndPriceProfile.ID, m_Aux.Grp.Fut.OptPriceProfile.ID
        
            Set aContract.Fut.UndPriceProfile = m_Aux.Grp.Fut.UndPriceProfile
            Set aContract.Fut.OptPriceProfile = m_Aux.Grp.Fut.OptPriceProfile
        End If
        
        Set aContract = Nothing
    End If
    
    Exit Sub
EH:
    LogEvent EVENT_ERROR, "Fail to save profiles info for " & m_Aux.Grp.Symbol & ". " & Err.Source & " " & Err.Description
End Sub

Private Sub SaveDividendsInfo()
    On Error GoTo EH
    If m_Aux.Grp.ID = 0 Or (m_Aux.Grp.ContractType <> enCtStock And m_Aux.Grp.ContractType <> enCtIndex) Then Exit Sub
    Dim aGUnd As EtsGeneralLib.UndAtom
    Dim aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim dYield As Double
    
    Set aUnd = m_Aux.Grp.Und
    
    If aUnd.ID >= 0 Then
    
        If Not aUnd Is Nothing Then
            Set aDiv = aUnd.Dividend
            If Not aDiv Is Nothing Then
                
                'Update global collection
                Set aGUnd = g_Underlying(aUnd.ID)
                If Not aGUnd Is Nothing Then
                    Set aGUnd.Dividend = aDiv
                    dYield = aGUnd.Yield
                End If
                      
                With aDiv
                    
                    'Save data to db
                    If (.DivType = enDivCustomPeriodical) Then
                        gDBW.usp_MmDividend_Save aUnd.ID, .DivType, , , , , .DivAmtCust, .DivFreqCust, IIf(.DivDateCust > 0, .DivDateCust, Null)
                    ElseIf (.DivType = enDivCustomStream) Then
                        gDBW.usp_MmDividend_Save aUnd.ID, .DivType
                    ElseIf (.DivType = enDivIndexYield) Then
                        gDBW.usp_MmDividend_Save aUnd.ID, .DivType, dYield
                    ElseIf (.DivType = enDivMarket) Then
                        gDBW.usp_MmDividend_Save aUnd.ID, .DivType, , .DivAmt, .DivFreq, IIf(.DivDate > 0, .DivDate, Null)
                    ElseIf (.DivType = enDivStockBasket) Then
                        gDBW.usp_MmDividend_Save aUnd.ID, .DivType
                    End If
                  
                End With
            
            End If
        End If
    End If
    Set aGUnd = Nothing
    Exit Sub
EH:
    LogEvent EVENT_ERROR, "Fail to save dividends info for " & m_Aux.Grp.Und.Symbol & ". " & Err.Source & " " & Err.Description
    Set aGUnd = Nothing
End Sub

Private Sub fgDiv_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter AfterEdit Enter", m_frmOwner.GetCaption
    
    If m_bShutDown Then Exit Sub
    Dim sValue$, nValue&, dValue#, dtValue As Date, bValue As Boolean, bNeedRecalc As Boolean
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, collUpdUnd As New EtsGeneralLib.EtsMmFilterAtomColl
    Dim lValue As Long
    Dim enDivType As EtsDivTypeEnum
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim bForceRecalc As Boolean
    Dim bManualEdit As Boolean: bManualEdit = True
    
    With fgDiv
        bNeedRecalc = False
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            Select Case Col
                
                Case QDC_TRADES ' trades filter
                    nValue = 0
                    nValue = CLng(sValue)
                    If QV.TradeFilter.Data(enFtTrades) <> nValue Then
                        QV.TradeFilter.Data(enFtTrades) = nValue
                        UnderlyingUpdatePositions
                        bNeedRecalc = True
                    End If
                    m_AuxOut.TradesFilterUpdate
                    
                Case QDC_FLT_TRADER
                    nValue = 0
                    nValue = CLng(sValue)
                    If QV.TradeFilter.Data(enFtTrader) <> nValue Then
                        QV.TradeFilter.Data(enFtTrader) = nValue
                        UnderlyingUpdatePositions
                        bNeedRecalc = True
                    End If
                    m_AuxOut.TradesFilterUpdate
                    
                Case QDC_FLT_STRATEGY
                    nValue = 0
                    nValue = CLng(sValue)
                    If QV.TradeFilter.Data(enFtStrategy) <> nValue Then
                        QV.TradeFilter.Data(enFtStrategy) = nValue
                        UnderlyingUpdatePositions
                        bNeedRecalc = True
                    End If
                    m_AuxOut.TradesFilterUpdate
                
                Case QDC_STYLE ' option style
                    nValue = -1
                    nValue = CLng(sValue)
                    If nValue = 0 Or nValue = 1 Then
                        If m_Aux.Grp.IsStockOrIndex Then
                            If m_Aux.Grp.Und.IsAmerican <> CBool(nValue) Then
                                m_Aux.Grp.Und.IsAmerican = CBool(nValue)
                                bNeedRecalc = True
                            End If
                        Else
                            If m_Aux.Grp.Fut.IsAmerican <> CBool(nValue) Then
                                m_Aux.Grp.Fut.IsAmerican = CBool(nValue)
                                bNeedRecalc = True
                            End If
                        End If
                    End If
                    m_AuxOut.ModelUpdate
                
                Case QDC_UNDPROFILE
                    nValue = -1
                    nValue = CLng(sValue)
                    If m_Aux.Grp.Und.UndPriceProfile <> nValue Then
                        If m_Aux.Grp.IsStockOrIndex Then
                            Set m_Aux.Grp.Und.UndPriceProfile = Nothing
                            Set m_Aux.Grp.Und.UndPriceProfile = g_PriceProfile(nValue)
                        Else
                            Set m_Aux.Grp.Fut.UndPriceProfile = Nothing
                            Set m_Aux.Grp.Fut.UndPriceProfile = g_PriceProfile(nValue)
                        End If
                        
                        SaveProfilesInfo
                        
                        m_bFireEvent = True
                        collUpdUnd.Add(CStr(m_Aux.Grp.ID)).ID = m_Aux.Grp.ID
                        g_Params.FireUnderlyingsParamsChange collUpdUnd
                        m_bFireEvent = False
                        
                        bNeedRecalc = True
                        bForceRecalc = True
                        bManualEdit = False
                    End If
                    m_AuxOut.ProfilesUpdate
                
                Case QDC_OPTPROFILE
                    nValue = -1
                    nValue = CLng(sValue)
                    If m_Aux.Grp.Und.UndPriceProfile <> nValue Then
                        If m_Aux.Grp.IsStockOrIndex Then
                            Set m_Aux.Grp.Und.OptPriceProfile = Nothing
                            Set m_Aux.Grp.Und.OptPriceProfile = g_PriceProfile(nValue)
                        Else
                            Set m_Aux.Grp.Fut.OptPriceProfile = Nothing
                            Set m_Aux.Grp.Fut.OptPriceProfile = g_PriceProfile(nValue)
                        End If
                        
                        SaveProfilesInfo
                        
                        m_bFireEvent = True
                        collUpdUnd.Add(CStr(m_Aux.Grp.ID)).ID = m_Aux.Grp.ID
                        g_Params.FireUnderlyingsParamsChange collUpdUnd
                        m_bFireEvent = False
                        
                        bNeedRecalc = True
                        bForceRecalc = True
                    End If
                    m_AuxOut.ProfilesUpdate
                
                Case QDC_DIV
                    If m_Aux.Grp.ContractType = enCtStock Or m_Aux.Grp.ContractType = enCtIndex Then
                        lValue = CLng(sValue)
                        enDivType = m_Aux.Grp.Und.Dividend.DivType
                        Set aDiv = m_Aux.Grp.Und.Dividend

                        If CLng(enDivType) <> lValue Then
                        
                            If Not g_PerformanceLog Is Nothing Then _
                                g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter Changed Dividend. " _
                                                                        & "OldValue=""" & CLng(enDivType) & """ " _
                                                                        & "NewValue=""" & lValue & """. " & GetStockInfo, m_frmOwner.GetCaption
                            
                            m_Aux.Grp.Und.Dividend.DivType = lValue
                            
                            If m_Aux.Grp.Und.Dividend.DivType = enDivCustomStream Then
                                LoadCustomDivs
                            End If
                            
                            SaveDividendsInfo
                            bNeedRecalc = True
                            bForceRecalc = True
                        End If
                        
                        
                    End If
                    m_AuxOut.DivsUpdate
                    mnuCtxCustomDividend.Enabled = m_Aux.Grp.ID <> 0
                    
                Case QDC_FREQ
                    If (m_Aux.Grp.ContractType = enCtStock Or m_Aux.Grp.ContractType = enCtIndex) Then
                        nValue = -1
                        nValue = ReadLng(sValue)
                        If nValue = 0 Or nValue = 1 Or nValue = 2 Or nValue = 4 Or nValue = 12 Then
                            Set aDiv = m_Aux.Grp.Und.Dividend
                            enDivType = m_Aux.Grp.Und.Dividend.DivType
                            
                            If (enDivType = enDivMarket And aDiv.DivFreq <> nValue) _
                                Or (enDivType = enDivCustomPeriodical And aDiv.DivFreqCust <> nValue) Then
                                
                                If Not g_PerformanceLog Is Nothing Then _
                                    g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter Changed Dividends Frequency. " _
                                                                        & "OldValue=""" & IIf(enDivType = enDivMarket, aDiv.DivFreq, aDiv.DivFreqCust) & """ " _
                                                                        & "NewValue=""" & nValue & """. " & GetStockInfo, m_frmOwner.GetCaption

                                If (aDiv.DivType = enDivMarket) Then
                                    aDiv.DivFreq = nValue
                                ElseIf (aDiv.DivType = enDivCustomPeriodical) Then
                                    aDiv.DivFreqCust = nValue
                                End If
                                
                            End If
                                
                            SaveDividendsInfo
                                                        
                            bNeedRecalc = True
                            bForceRecalc = True
                        
                        End If
                    End If
                    m_AuxOut.DivsUpdate
                    
                Case QDC_DATE
                    If (m_Aux.Grp.ContractType = enCtStock Or m_Aux.Grp.ContractType = enCtIndex) Then
                        dtValue = 0#
                        dtValue = ReadDate(sValue)
                        If dtValue > 0# Then
                            Set aDiv = m_Aux.Grp.Und.Dividend
                            enDivType = m_Aux.Grp.Und.Dividend.DivType
                        
                            If enDivType = enDivMarket And aDiv.DivDate <> dtValue _
                                Or enDivType = enDivCustomPeriodical And aDiv.DivDateCust <> dtValue Then
                                
                                If (enDivType = enDivMarket) Then
                                    aDiv.DivDate = dtValue
                                ElseIf (enDivType = enDivCustomPeriodical) Then
                                    aDiv.DivDateCust = dtValue
                                End If
                                
                            End If
                        
                        SaveDividendsInfo
                        bNeedRecalc = True
                        bForceRecalc = True
                            
                        End If
                    End If
                    m_AuxOut.DivsUpdate
                
                Case QDC_AMT
                    If (m_Aux.Grp.ContractType = enCtStock Or m_Aux.Grp.ContractType = enCtIndex) Then
                        dValue = 0#
                        dValue = ReadDbl(sValue)
                        If dValue > 0# Then
                            Set aDiv = m_Aux.Grp.Und.Dividend
                            enDivType = aDiv.DivType
                        
                            If enDivType = enDivMarket And aDiv.DivAmt <> dValue _
                                Or enDivType = enDivCustomPeriodical And aDiv.DivAmtCust <> dValue Then
                                
                                If (enDivType = enDivMarket) Then
                                    aDiv.DivAmt = dValue
                                ElseIf (enDivType = enDivCustomPeriodical) Then
                                    aDiv.DivAmtCust = dValue
                                End If
                            ElseIf (m_Aux.Grp.ContractType = enCtIndex And enDivType = enDivIndexYield) Then
                                dValue = dValue / 100#
                                If (dValue >= 0) Then
                                    m_Aux.Grp.Und.Yield = dValue
                                    If (Not g_Underlying(m_Aux.Grp.Und.ID) Is Nothing) Then
                                        g_Underlying(m_Aux.Grp.Und.ID).Yield = dValue
                                    End If
                                End If
                            End If
                            SaveDividendsInfo
                            bNeedRecalc = True
                            bForceRecalc = True
                        End If
                    End If
                    m_AuxOut.DivsUpdate
                                    
                Case Else ' rates
                    If Col >= QDC_RATE_VAL Then
                        Set aExp = .ColData(Col)
                        If Not aExp Is Nothing Then
                            dValue = 0#
                            dValue = ReadDbl(sValue)
                            dValue = dValue / 100#
                            If dValue > 0# Then
                                If Not m_Aux.Grp.UseCustRates And aExp.Rate <> dValue _
                                    Or m_Aux.Grp.UseCustRates And aExp.RateCust <> dValue Then
                                    
                                    If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter Changed Rate. " _
                                                                        & "Date=""" & aExp.ExpiryMonth & """ " _
                                                                        & "OldValue=""" & IIf(m_Aux.Grp.UseCustRates, aExp.RateCust, aExp.Rate) * 100 & """ " _
                                                                        & "NewValue=""" & dValue * 100 & """. " & GetStockInfo, m_frmOwner.GetCaption
                                    
                                    m_Aux.Grp.UseCustRates = True
                                    
                                    aExp.RateCust = dValue
                                    
                                    Dim aExpLoop As EtsMmQuotesLib.MmQvExpAtom
                                    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
                                    Dim strSerach As String
                                    strSerach = fgDiv.TextMatrix(0, Col)
                                    Dim strTmp1 As String
                                                                      
                                    
                                    If m_Aux.Grp.IsStockOrIndex Then
                                        Set aExpColl = m_Aux.Grp.Und.Exp
                                    Else
                                        Set aExpColl = m_Aux.Grp.Fut.Exp
                                    End If
                                    
                                    For Each aExpLoop In aExpColl
                                         Dim aExpGlobal As EtsMmQuotesLib.MmQvExpAtom
                                         Set aExpGlobal = m_Aux.Grp.ExpAll(aExpLoop.ExpiryMonth)
                                         If Not aExpGlobal Is Nothing Then aExpGlobal.RateCust = aExpLoop.RateCust
                                    Next
                                    
                                    bNeedRecalc = True
                                    bForceRecalc = True
                                End If
                            End If
                            Set aExp = Nothing
                        End If
                    Else
                        bValue = (sValue <> "0")
                        If m_Aux.Grp.UseCustRates <> bValue Then
                            m_Aux.Grp.UseCustRates = bValue
                            bNeedRecalc = True
                        End If
                    End If
                    m_AuxOut.RatesUpdate
            End Select
            
            If bNeedRecalc And Not m_bInProc Then
                Recalculate False, bManualEdit, bForceRecalc
                .AutoSize Col, Col
            End If
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter AfterEdit Exit", m_frmOwner.GetCaption

End Sub

Private Sub fgDiv_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Button = vbRightButton Then
        With fgDiv
            m_enMenuGrid = GT_QUOTES_DIVIDENDS
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            
            ShowPopup
        End With
    End If
End Sub

Private Sub fgDiv_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter StartEdit Enter", m_frmOwner.GetCaption
    
   
    Dim nKey&
    Cancel = True
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub
    If IsDblClickHandled Then Exit Sub
    
    nKey = IIf(Col >= QDC_RATE_VAL, QDC_RATE_VAL, Col)
    Cancel = (m_Aux.Grp.ID = 0 Or Not m_gdDiv.Col(nKey).CanEdit)
    
    If Not Cancel Then
        If nKey = QDC_RATE_VAL Then
            Dim aExp As EtsMmQuotesLib.MmQvExpAtom
            Set aExp = fgDiv.ColData(Col)
            Cancel = aExp Is Nothing
            Set aExp = Nothing
        End If
        If Not Cancel Then m_sCurrentOriginalText = Trim$(fgDiv.TextMatrix(Row, Col))
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend Filter StartEdit Exit", m_frmOwner.GetCaption

End Sub

Private Sub fgOpt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nKey&, sValue$, dValue#, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, sKey$, nRow&
    Dim aRowData As MmQvRowData, aOpt As EtsMmQuotesLib.MmQvOptAtom, nOptType&
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom
    Dim enCalcIV As EtsMmQuotesLib.MmQvIvCalcEnum, nMask&, aPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim lSurfaceID As Long
    
    With fgOpt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nKey = .ColKey(Col)
            If Err.Number = 0 Then
                Set aRowData = QV.OptsRowData(Row)
                
                If Not aRowData Is Nothing Then
                    Set aExp = aRowData.Exp
                    Debug.Assert Not aExp Is Nothing
                    
                    Select Case nKey
                        Case QOC_C_BID, QOC_C_ASK, QOC_C_VOLA, QOC_C_LAST
                            nOptType = enOtCall
                            
                        Case QOC_P_BID, QOC_P_ASK, QOC_P_VOLA, QOC_P_LAST
                            nOptType = enOtPut
                        
                    End Select
                    
                    Set aOpt = aRowData.Opt(nOptType)
                    Set aQuote = aRowData.OptQuote(nOptType)
                    Set aStr = aRowData.Strike
                
                    If Not aOpt Is Nothing And Not aQuote Is Nothing And Not aExp Is Nothing And Not aStr Is Nothing Then
                        dValue = Abs(ReadDbl(sValue))
                        
                        If nKey <> QOC_C_VOLA And nKey <> QOC_P_VOLA Then
                            
                            enCalcIV = enMmQvCalcIvNone
                            nMask = GM_NONE
                            
                            Select Case nKey
                                Case QOC_C_BID, QOC_P_BID
                                    If aQuote.IsDirty Then
                                        If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter Changed " & IIf(nKey = QOC_C_BID, "Call", "Put") & " Bid. " _
                                                                                        & "OldValue=""" & aQuote.PriceBid & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                    
                                        enCalcIV = GetIvMask(nOptType)
                                        If Not g_Params.UseTheoVolatility Then nMask = GetGreeksMask(nOptType)
                                    End If
                                    
                                Case QOC_C_ASK, QOC_P_ASK
                                    If aQuote.IsDirty Then
                                        If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter Changed " & IIf(nKey = QOC_C_ASK, "Call", "Put") & " Ask. " _
                                                                                        & "OldValue=""" & aQuote.PriceAsk & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                    
                                        enCalcIV = GetIvMask(nOptType)
                                        If Not g_Params.UseTheoVolatility Then nMask = GetGreeksMask(nOptType)
                                    End If
                                Case QOC_C_LAST, QOC_P_LAST
                                    If aQuote.IsDirty Then
                                        If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter Changed " & IIf(nKey = QOC_C_LAST, "Call", "Put") & " Last. " _
                                                                                        & "OldValue=""" & aQuote.PriceLast & """ " _
                                                                                        & "NewValue=""" & dValue & """. " & GetOptionInfo, m_frmOwner.GetCaption
                                        enCalcIV = GetIvMask(nOptType)
                                        If Not g_Params.UseTheoVolatility Then nMask = GetGreeksMask(nOptType)
                                    End If
                            End Select
                            
                            If enCalcIV <> enMmQvCalcIvNone Or nMask <> GM_NONE Or aQuote.CalcGreeksIVType = enMmQvCalcGreeksIVTheo Then
                                m_bInProc = True
                                CalcOptionGreeks aOpt, aQuote, aExp, nMask, enCalcIV, False
                                
                                CalculateUnderlyingOptions False, , False
                                m_bInProc = False
                                m_AuxOut.UnderlyingUpdateTotals
                                UpdateTotals
                            End If
                            
                        Else ' QOC_C_VOLA, QOC_P_VOLA
                            dValue = dValue / 100#
                            If True Then
                                If Not g_PerformanceLog Is Nothing Then _
                                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter Changed " & IIf(nKey = QOC_C_VOLA, "Call", "Put") & " Volatility. " _
                                                                                & "OldValue=""" & aOpt.Vola * 100 & """ " _
                                                                                & "NewValue=""" & dValue * 100 & """. " & GetOptionInfo, m_frmOwner.GetCaption
                            
                                m_Aux.IsSimulatedFlat = False
                                
                                lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aOpt.RootID)
                                m_Aux.Grp.Und.VolaSrv.OptionVola(aOpt.ExpiryOV, aOpt.Strike, lSurfaceID) = dValue
                                
                                m_bVolaChangedNow = True
                                g_Params.FireInternalVolatilityChanged m_Aux.Grp.Und.Symbol
                                m_bVolaChangedNow = False
                       
                                If g_Params.UseTheoVolatility Then nMask = GetGreeksMask(nOptType)
                            
                                For Each aPair In aStr.OptPair
                                    For nOptType = enOtPut To enOtCall
                                    
                                        lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aPair.RootID)
                                        dValue = m_Aux.Grp.Und.VolaSrv.OptionVola(aExp.ExpiryOV, aStr.Strike, lSurfaceID)
                                        
                                        Set aOpt = aPair.Opt(nOptType)
                                        If aOpt.ID <> 0 Then
                                            aOpt.Vola = dValue
                                            
                                            For Each aQuote In aOpt.Quote
                                                
                                                If nMask <> GM_NONE Or aQuote.CalcGreeksIVType = enMmQvCalcGreeksIVTheo Then
                                                    CalcOptionGreeks aOpt, aQuote, aExp, nMask, GetIvMask(nOptType), , False
                                                End If
                                                                                                
                                            Next
                                        End If
                                        Set aOpt = Nothing
                                        
                                    Next
                                Next
                                
                               If Abs(aOpt.Strike - m_Aux.Grp.Und.AtmStrike(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, False)) <= DBL_EPSILON Then
                                        m_AuxOut.VolaUpdateValues False
                               End If
                                
                                If nMask <> GM_NONE Then
                                    m_bInProc = True
                                    CalculateUnderlyingOptions False, , False
                                    m_AuxOut.UnderlyingUpdateTotals
                                    UpdateTotals
                                    m_bInProc = False
'                                    fgOpt.Refresh
                                End If
                            End If
                        End If
                    End If
                    
                Else
                    Debug.Assert False
                End If
                
                Set aExp = Nothing
                Set aOpt = Nothing
                Set aQuote = Nothing
                Set aStr = Nothing
                Set aRowData = Nothing
            End If
        End If
    End With
End Sub

Private Sub fgOpt_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub
    
    nOldPos = Col - QOC_C_SYMBOL
    nNewPos = Position - QOC_C_SYMBOL
    nIdx = m_gdOpt.Idx(nOldPos)
    If nOldPos > nNewPos Then
        For i = nOldPos To nNewPos Step -1
            If i > nNewPos Then
                m_gdOpt.Idx(i) = m_gdOpt.Idx(i - 1)
            End If
        Next
        m_gdOpt.Idx(nNewPos) = nIdx
    Else
        For i = nOldPos To nNewPos
            If i < nNewPos Then
                m_gdOpt.Idx(i) = m_gdOpt.Idx(i + 1)
            End If
        Next
        m_gdOpt.Idx(nNewPos) = nIdx
    End If
End Sub

Private Sub fgOpt_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nOldIdx&, nNewIdx&
    
    nOldIdx = fgOpt.ColKey(Col)
    nNewIdx = fgOpt.ColKey(Position)
    
    If nOldIdx = QOC_NONE Or nNewIdx = QOC_NONE Then
        Position = Col
    ElseIf Not m_gdOpt.Col(nOldIdx).CanChangeOrder Or Not m_gdOpt.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Sub fgOpt_DblClick()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgOpt
        m_enMenuGrid = GT_QUOTES_OPTIONS
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick True
    End With
End Sub

Private Sub fgOpt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If IsHotKeyGridCommonOrTradeOrderCard(KeyCode, Shift) _
            Or (KeyCode = vbKeyF And (Shift = vbCtrlMask _
                                    Or Shift = vbCtrlMask + vbShiftMask _
                                    Or Shift = vbCtrlMask + vbAltMask)) _
            Or (KeyCode = vbKeyI And (Shift = vbCtrlMask + vbShiftMask _
                                    Or Shift = vbCtrlMask + vbAltMask)) Then
        
        m_bKeyDown(GT_QUOTES_OPTIONS) = True
    End If
End Sub

Private Sub fgOpt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bKeyDown(GT_QUOTES_OPTIONS) Then
        m_bKeyDown(GT_QUOTES_OPTIONS) = False
        With fgOpt
            m_enMenuGrid = GT_QUOTES_OPTIONS
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxTradeNew_Click
                
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Alt+Ctrl+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Shift+Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxTntCardNew_Click
            
            Case KeyCode = vbKeyF And Shift = vbCtrlMask _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+F pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFlatCur_Click
                
            Case KeyCode = vbKeyF And Shift = vbCtrlMask + vbShiftMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+Shift+F pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFlatAll_Click
                
            Case KeyCode = vbKeyF And Shift = vbCtrlMask + vbAltMask _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+Alt+F pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFlatAllByCur_Click
                
            Case KeyCode = vbKeyI And Shift = vbCtrlMask + vbShiftMask _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+Shift+I pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFitToImpCur_Click
            
            Case KeyCode = vbKeyI And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+Alt+I pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFitToImpAll_Click
            
            Case KeyCode = vbKeyG And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+G pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxAutosizeGrid_Click
                
            Case KeyCode = vbKeyA And Shift = vbCtrlMask _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+A pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxAutosizeCol_Click
                
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option Filter. Hot key Ctrl+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxCopy_Click
        End Select
    End If
End Sub

Private Sub fgOpt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Button = vbRightButton Then
        With fgOpt
            m_enMenuGrid = GT_QUOTES_OPTIONS
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            
            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows And m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols Then
                .Row = m_nMenuGridRow
            End If
            
            ShowPopup
        End With
    End If
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    mnuCtxRealTime.Enabled = m_Aux.Grp.ID <> 0 And Not m_bInProc _
                                And Not m_bLastQuoteReqNow And Not m_bSubscribingNow And Not m_bDataLoad And Not m_bInitializing
    mnuCtxRealTime.Checked = m_Aux.RealTime
    mnuCtxRefresh.Enabled = mnuCtxRealTime.Enabled And Not m_Aux.RealTime
    mnuCtxRefreshPrices.Enabled = mnuCtxRefresh.Enabled
    mnuCtxDefOptSort.Enabled = m_Aux.Grp.ID <> 0 And Not m_bInProc
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    Dim sValue$, aRowData As MmQvRowData: Set aRowData = Nothing
    Dim bEnabled As Boolean: bEnabled = False
    Dim nExpCount&
    Dim bEnableCustomDivs As Boolean
    
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub

    'mnuCtxTradeNew                                 "New Trade..."
    'mnuCtxPositionTransfer                         "Position Transfer..."
    'mnuCtxOrderNewStock                            "New Stock Order..."
    'mnuCtxOrderNewOption                           "New Option Order..."
    'mnuCtxTradeExercise                            "Exercise Trades..."
    'mnuCtxTradeExpiry                              "Expiry Trades..."
    'mnuCtxVolaFlat                                 "Flat Vola"
    'mnuCtxVolaFlatCur                              "This Expiry"
    'mnuCtxVolaFlatAll                              "All Expiries"
    'mnuCtxVolaFlatAllByCur                         "All Expiries By This"
    'mnuCtxVolaFitToImp                             "Fit Vola To Implied"
    'mnuCtxVolaFitToImpCur                          "This Expiry"
    'mnuCtxVolaFitToImpAll                          "All Expiries"
    'mnuCtxDefOptSort                               "Default Options Sorting"
    'mnuCtxRealTime                                 "Real Time Mode"
    'mnuCtxRefresh                                  "Refresh"
    'mnuCtxCopy                                     "Copy Grid"
    'mnuCtxPrint                                    "Print Grid..."
    'mnuCtxHideCol                                  "Hide Column"
    'mnuCtxAutosizeCol                              "Autosize Column"
    'mnuCtxAutosizeGrid                             "Autosize Grid"
    'mnuCtxGridLayout                               "Grid Layout..."

    mnuCtxVolaFlatCur.Caption = "This Expiry by ATM" & vbTab & "Ctrl+F"
    mnuCtxVolaFlatAllByCur.Caption = "All Expiries by This ATM" & vbTab & "Ctrl+Alt+F"
    mnuCtxVolaFlatCur.Enabled = False
    mnuCtxVolaFlatAll.Enabled = False
    mnuCtxVolaFlatAllByCur.Enabled = False
    mnuCtxVolaFlat.Enabled = False
    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxTradeExercise.Enabled = False
    mnuCtxTradeExpiry.Enabled = False
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    bEnableCustomDivs = False
    
    
    If Not m_Aux.Grp.Und.Dividend Is Nothing Then
        mnuCtxCustomDividend.Enabled = IIf(m_Aux.Grp.Und.Dividend.DivType = enDivCustomStream, True, False)
    Else
        mnuCtxCustomDividend.Enabled = False
    End If
 
    mnuCtxVolaFitToImpCur.Caption = "This Expiry" & vbTab & "Ctrl+Shift+I"
    mnuCtxVolaFitToImpAll.Caption = "All Expiries" & vbTab & "Ctrl+Alt+I"
    mnuCtxVolaFitToImp.Enabled = False
    mnuCtxVolaFitToImpCur.Enabled = False
    mnuCtxVolaFitToImpAll.Enabled = False
    mnuCtxSaveSimulatedVola.Enabled = False

    If m_Aux.Grp.IsStockOrIndex Then
        nExpCount = m_Aux.Grp.Und.Exp.Count
    Else
        nExpCount = m_Aux.Grp.Fut.Exp.Count
    End If
    
    Select Case m_enMenuGrid
        Case GT_QUOTES_UNDERLYING
            If m_nMenuGridRow > 0 Then Set aRowData = fgUnd.RowData(m_nMenuGridRow)
            
            If Not aRowData Is Nothing Then
                If m_Aux.Grp.IsStockOrIndex Then
                    If Not aRowData.Und Is Nothing Then
                        bEnabled = aRowData.Und.ID <> USD_ID
                    Else
                        bEnabled = True
                    End If
                Else
                    bEnabled = True
                End If
            End If
            
            mnuCtxTradeNew.Enabled = (m_Aux.Grp.ID <> 0 And m_Aux.Grp.Und.IsTraderContract And m_nMenuGridRow > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                        And bEnabled)
            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
            
            mnuCtxHideCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxAutosizeGrid.Enabled = True
            
            mnuCtxVolaFlatAll.Enabled = m_Aux.Grp.ID <> 0 And Not m_bInProc And Not m_bLastQuoteReqNow And nExpCount > 0
            mnuCtxVolaFlat.Enabled = mnuCtxVolaFlatAll.Enabled
            
            mnuCtxVolaFitToImpAll.Enabled = mnuCtxVolaFlatAll.Enabled
            mnuCtxVolaFitToImp.Enabled = mnuCtxVolaFitToImpAll.Enabled
            
            mnuCtxSaveSimulatedVola.Enabled = mnuCtxVolaFlatAll.Enabled And m_Aux.IsVolaSimulated
            
            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
        Case GT_QUOTES_OPTIONS
            mnuCtxTradeNew.Enabled = (m_Aux.Grp.ID <> 0 And m_Aux.Grp.Und.IsTraderContract And m_nMenuGridRow > 0 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxHideCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols And m_gdOpt.Idx(1) <> QOC_NONE)
            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxAutosizeGrid.Enabled = True
            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
            
            If m_nMenuGridRow > 0 Then Set aRowData = QV.OptsRowData(m_nMenuGridRow)
            
            If Not m_bInProc And Not m_bLastQuoteReqNow _
                And m_Aux.Grp.ID <> 0 And m_nMenuGridRow > 0 And m_nMenuGridCol < m_nMenuGridCols Then
                            
                Dim nColIdx&, aExp As EtsMmQuotesLib.MmQvExpAtom, aOpt As EtsMmQuotesLib.MmQvOptAtom
                
                nColIdx = fgOpt.ColKey(m_nMenuGridCol)
                If nColIdx >= QOC_C_SYMBOL And nColIdx <= QOC_LAST_COLUMN Then
                    Set aExp = aRowData.Exp
                    
                    If Not aExp Is Nothing Then
                        sValue = Format$(aExp.Expiry, "MMM YY")
                        mnuCtxVolaFlatCur.Caption = "By " & sValue & " ATM" & vbTab & "Ctrl+F"
                        mnuCtxVolaFlatAllByCur.Caption = "All Expiries by " & sValue & " ATM" & vbTab & "Ctrl+Alt+F"
                        mnuCtxVolaFlat.Enabled = True
                        mnuCtxVolaFlatCur.Enabled = True
                        mnuCtxVolaFlatAll.Enabled = True
                        mnuCtxVolaFlatAllByCur.Enabled = True
                        
                        mnuCtxVolaFitToImpCur.Caption = sValue & vbTab & "Ctrl+Shift+I"
                        mnuCtxVolaFitToImp.Enabled = True
                        mnuCtxVolaFitToImpCur.Enabled = True
                        mnuCtxVolaFitToImpAll.Enabled = True
                        
                        mnuCtxSaveSimulatedVola.Enabled = m_Aux.IsVolaSimulated
                        
                        If nColIdx >= QOC_C_SYMBOL And nColIdx <= QOC_C_UPDATE_TIME _
                            Or nColIdx >= QOC_P_SYMBOL And nColIdx <= QOC_P_UPDATE_TIME Then
                            
                            Set aOpt = aRowData.Opt(IIf(nColIdx >= QOC_P_SYMBOL, enOtPut, enOtCall))
    
                            If Not aOpt Is Nothing Then
                                
                                mnuCtxTradeExercise.Enabled = (aOpt.Qty > BAD_LONG_VALUE)
                                mnuCtxTradeExpiry.Enabled = mnuCtxTradeExercise.Enabled
                                
                                Set aOpt = Nothing
                            End If
                        End If
                    End If
                    
                    Set aExp = Nothing
                End If
            End If
            
            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
        Case GT_QUOTES_DIVIDENDS
            mnuCtxTradeNew.Enabled = False
            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            mnuCtxPositionTransfer.Enabled = False
            
            PopupMenu mnuCtx
        
        Case GT_QUOTES_VOLA
            mnuCtxTradeNew.Enabled = False
            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            mnuCtxPositionTransfer.Enabled = False
            
            If Not m_bInProc And Not m_bLastQuoteReqNow _
                And m_Aux.Grp.ID <> 0 And m_nMenuGridCol > 0 And m_nMenuGridCol <= nExpCount Then
                
                sValue = fgVol.TextMatrix(0, m_nMenuGridCol)
                mnuCtxVolaFlatCur.Caption = "By " & sValue & " ATM" & vbTab & "Ctrl+F"
                mnuCtxVolaFlatAllByCur.Caption = "All Expiries by " & sValue & " ATM" & vbTab & "Ctrl+Alt+F"
                mnuCtxVolaFlat.Enabled = True
                mnuCtxVolaFlatCur.Enabled = True
                mnuCtxVolaFlatAll.Enabled = True
                mnuCtxVolaFlatAllByCur.Enabled = True
            
                mnuCtxVolaFitToImpCur.Caption = sValue & vbTab & "Ctrl+Shift+I"
                mnuCtxVolaFitToImp.Enabled = True
                mnuCtxVolaFitToImpCur.Enabled = True
                mnuCtxVolaFitToImpAll.Enabled = True
                
                mnuCtxSaveSimulatedVola.Enabled = m_Aux.IsVolaSimulated
            End If
            
            PopupMenu mnuCtx
        
    End Select
    
    Set aRowData = Nothing
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
End Sub

Private Sub fgOpt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim aRowData As MmQvRowData
    Dim nKey&
    Cancel = True
    
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub
    If IsDblClickHandled Then Exit Sub
    
    If m_Aux.Grp.ID <> 0 Then
        With fgOpt
            nKey = .ColKey(Col)
            If Err.Number = 0 Then
                If m_Aux.RealTime And nKey <> QOC_C_VOLA And _
                    nKey <> QOC_P_VOLA Then Exit Sub
                Cancel = Not m_gdOpt.Col(nKey).CanEdit
                If Not Cancel Then
                    Set aRowData = QV.OptsRowData(Row)
                    If nKey < QOC_P_SYMBOL Then
                        Cancel = (aRowData.Opt(enOtCall) Is Nothing Or aRowData.OptQuote(enOtCall) Is Nothing)
                    Else
                        Cancel = (aRowData.Opt(enOtPut) Is Nothing Or aRowData.OptQuote(enOtPut) Is Nothing)
                    End If
                    
                    Set aRowData = Nothing
                End If
                
                If Not Cancel Then
                    Cancel = (Not m_Aux.Grp.Und.IsTraderContract And (nKey = QOC_C_VOLA Or nKey = QOC_P_VOLA))
                End If
            End If
            
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End With
    End If
End Sub

Private Sub fgUnd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter AfterEdit Enter", m_frmOwner.GetCaption
    
    If m_bShutDown Then Exit Sub
    Dim nKey&, sValue$, dValue#, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, bNeedRecalc As Boolean
    Dim aRowData As MmQvRowData
    Dim UnderlyingPriceHelp As Double
    Dim enPriceStatusMid As EtsGeneralLib.EtsReplacePriceStatusEnum
         
    With fgUnd
        nKey = .ColKey(Col)
        If Err.Number = 0 Then
            sValue = Trim$(.TextMatrix(Row, Col))
            
            If m_sCurrentOriginalText <> sValue Then
                bNeedRecalc = False
                If nKey = QUC_SYMBOL Then
                    Debug.Assert Row = m_nUndMainRow
                    sValue = UCase$(sValue)
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter AfterEdit New Symbol: " & sValue, m_frmOwner.GetCaption
                    
                    m_nNewGrpID = 0
                    m_nNewGrpID = g_Contract.BySortKey(sValue).ID
                    If m_nNewGrpID = 0 Then
                        If gCmn.MyMsgBox(m_frmOwner, "There is no market structure for symbol '" & sValue & "'." & vbCrLf & _
                                            "Update market structure for this symbol?", vbQuestion Or vbYesNo) = vbYes Then

                            Dim frmMS As New frmMarketStructure
                            
                            m_nNewGrpID = frmMS.Execute(sValue, m_frmOwner)
                            If m_nNewGrpID <> 0 Then
                                DoEvents
                                
                                Screen.MousePointer = vbHourglass
                                m_bUndListReloadNow = True
                                
                                g_Params.MakeUnderlingComboLists
                                InitUndList
                                g_Params.FireNewUnderlyingAdded m_nNewGrpID
                                
                                m_bUndListReloadNow = False
                                Screen.MousePointer = vbDefault
                            End If
                        End If
                    End If
                    
                    If m_nNewGrpID <> 0 Then
                        .TextMatrix(Row, Col) = sValue
                        
                        QV.VisibleUndExch.Clear
                        If Not g_Params.QuoteUndExchAll Then
                            QV.VisibleUndExch.Add(Trim$(Str$(g_Params.QuoteDefUndExchID))).ID = g_Params.QuoteDefUndExchID
                        End If
                        
                        QV.VisibleOptExch.Clear
                        If Not g_Params.QuoteOptExchAll Then
                            QV.VisibleOptExch.Add(Trim$(Str$(g_Params.QuoteDefOptExchID))).ID = g_Params.QuoteDefOptExchID
                        End If
                        
                        If m_Aux.IsVolaSimulated And m_Aux.IsSimulatedFlat Then
                            m_Aux.bFlatVolaToATM = True
                        End If
                        
                        tmrShow.Enabled = True
                    Else
                        .TextMatrix(Row, Col) = m_Aux.Grp.Symbol
                    End If
                Else
                    Set aRowData = .RowData(Row)
                    Debug.Assert Not aRowData Is Nothing
                    Set aQuote = aRowData.UndQuote
                    
                    If Not aRowData Is Nothing And Not aQuote Is Nothing Then
                        Select Case nKey
                            Case QUC_BID
                                If aQuote.PriceBid >= 0# Then aQuote.PriceBidTick = 0
                                m_AuxOut.UnderlyingUpdateQuoteColors Row, aQuote
                                
                                dValue = Abs(ReadDbl(sValue))
                                If aQuote.PriceBid <> dValue Then
                                    If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter Changed Bid. " _
                                                                                    & "OldValue=""" & aQuote.PriceBid & """ " _
                                                                                    & "NewValue=""" & dValue & """. " & GetStockInfo, m_frmOwner.GetCaption
                                
                                    If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter Changed Bid. " _
                                                                                    & "OldValue=""" & aQuote.PriceBid & """ " _
                                                                                    & "NewValue=""" & dValue & """. " & GetStockInfo, m_frmOwner.GetCaption
                                
                                    aQuote.PriceBid = dValue
                                    
                                    .TextMatrix(Row, Col) = aQuote.PriceBid
                                    
                                    bNeedRecalc = (aQuote.Exch.ID = 0)
                                End If
                                
                            Case QUC_ASK
                                If aQuote.PriceAsk >= 0# Then aQuote.PriceAskTick = 0
                                m_AuxOut.UnderlyingUpdateQuoteColors Row, aQuote
                                
                                dValue = Abs(ReadDbl(sValue))
                                If aQuote.PriceAsk <> dValue Then
                                    If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter Changed Ask. " _
                                                                                    & "OldValue=""" & aQuote.PriceAsk & """ " _
                                                                                    & "NewValue=""" & dValue & """. " & GetStockInfo, m_frmOwner.GetCaption
                                
                                    If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter Changed Ask. " _
                                                                                    & "OldValue=""" & aQuote.PriceAsk & """ " _
                                                                                    & "NewValue=""" & dValue & """. " & GetStockInfo, m_frmOwner.GetCaption
                                
                                    aQuote.PriceAsk = dValue
                                    
                                    .TextMatrix(Row, Col) = aQuote.PriceAsk
                                    
                                    bNeedRecalc = (aQuote.Exch.ID = 0)
                                End If
                                
                            Case QUC_LAST
                            
                                dValue = Abs(ReadDbl(sValue))
                                If aQuote.PriceLast <> dValue Then
                                    If Not g_PerformanceLog Is Nothing Then _
                                        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter Changed Last. " _
                                                                                    & "OldValue=""" & aQuote.PriceLast & """ " _
                                                                                    & "NewValue=""" & dValue & """. " & GetStockInfo, m_frmOwner.GetCaption
                                
                                    aQuote.PriceLast = dValue
                                    aQuote.PriceBid = dValue
                                    aQuote.PriceAsk = dValue
                                    .TextMatrix(Row, Col) = aQuote.PriceLast
                              
                                    bNeedRecalc = (aQuote.Exch.ID = 0)
                                End If
                        End Select
                        
                        If bNeedRecalc Then
                            If m_Aux.Grp.IsStockOrIndex Then
                                                              
                                aRowData.Und.VolaSrv.UnderlyingPrice = aRowData.Und.UndPriceProfile.GetUndPriceMid( _
                                                                       aQuote.PriceBid, aQuote.PriceAsk, aQuote.PriceLast, _
                                                                       g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enPriceStatusMid, False)
                            aRowData.UndQuote.ReplacePriceStatus = enPriceStatusMid
                            End If
                            Recalculate False, False
                        End If
                        
                        Set aQuote = Nothing
                    End If
                    
                    Set aRowData = Nothing
                End If
            End If
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter AfterEdit Exit", m_frmOwner.GetCaption

End Sub

Private Sub fgUnd_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub
    
    nOldPos = Col - QUC_SYMBOL
    nNewPos = Position - QUC_SYMBOL
    nIdx = m_gdUnd.Idx(nOldPos)
    If nOldPos > nNewPos Then
        For i = nOldPos To nNewPos Step -1
            If i > nNewPos Then
                m_gdUnd.Idx(i) = m_gdUnd.Idx(i - 1)
            End If
        Next
        m_gdUnd.Idx(nNewPos) = nIdx
    Else
        For i = nOldPos To nNewPos
            If i < nNewPos Then
                m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
            End If
        Next
        m_gdUnd.Idx(nNewPos) = nIdx
    End If
End Sub

Private Sub fgUnd_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&
    
    With fgUnd
        nIdx = .ColKey(Col)
        If Row <> m_nUndMainRow Or nIdx <> QUC_SYMBOL Then
            .ComboList = ""
        Else
            .ComboList = m_sUndComboList
            .EditMaxLength = 20
        End If
        
    End With
End Sub

Private Sub fgUnd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nOldIdx&, nNewIdx&
    
    nOldIdx = fgUnd.ColKey(Col)
    nNewIdx = fgUnd.ColKey(Position)
    
    If nOldIdx = QUC_NONE Or nNewIdx = QUC_NONE Then
        Position = Col
    ElseIf Not m_gdUnd.Col(nOldIdx).CanChangeOrder Or Not m_gdUnd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Sub fgUnd_DblClick()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgUnd
        m_enMenuGrid = GT_QUOTES_UNDERLYING
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick (m_nMenuGridRow <> 1 Or m_nMenuGridRow = 1 And QUC_SYMBOL <> fgUnd.ColKey(m_nMenuGridCol))
    End With
End Sub

Private Sub fgUnd_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If IsHotKeyGridCommonOrTradeOrderCard(KeyCode, Shift) _
            Or (KeyCode = vbKeyF And Shift = vbCtrlMask + vbShiftMask) _
            Or (KeyCode = vbKeyI And Shift = vbCtrlMask + vbAltMask) Then
        
        m_bKeyDown(GT_QUOTES_UNDERLYING) = True
    End If
End Sub

Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bKeyDown(GT_QUOTES_UNDERLYING) Then
        m_bKeyDown(GT_QUOTES_UNDERLYING) = False
        With fgUnd
            m_enMenuGrid = GT_QUOTES_UNDERLYING
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxTradeNew_Click
                
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Ctrl+Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Shift+Alt+Insert pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxTntCardNew_Click
            
            Case KeyCode = vbKeyF And Shift = vbCtrlMask + vbShiftMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Ctrl+Shift+F pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFlatAll_Click
                
            Case KeyCode = vbKeyI And Shift = vbCtrlMask + vbAltMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Ctrl+Alt+I pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxVolaFitToImpAll_Click
            
            Case KeyCode = vbKeyG And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Ctrl+G pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxAutosizeGrid_Click
                
            Case KeyCode = vbKeyA And Shift = vbCtrlMask _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Ctrl+A pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxAutosizeCol_Click
                
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter. Hot key Ctrl+(Insert/C) pressed. " & GetOptionInfo, m_frmOwner.Caption
                mnuCtxCopy_Click
        End Select
    End If
End Sub

Private Sub fgUnd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Button = vbRightButton Then
        With fgUnd
            m_enMenuGrid = GT_QUOTES_UNDERLYING
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

Private Sub fgUnd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter StartEdit Enter", m_frmOwner.GetCaption
    
    If m_bShutDown Then Exit Sub
    Dim nKey&
    Cancel = True
    
    If Not SaveSimulatedVolatility() Then Exit Sub
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub
    If IsDblClickHandled Then Exit Sub
    
    With fgUnd
        nKey = .ColKey(Col)
        If Err.Number = 0 Then
            If m_Aux.Grp.ID <> 0 Then
                Cancel = Not m_gdUnd.Col(nKey).CanEdit _
                        Or (Row <> m_nUndMainRow And nKey = QUC_SYMBOL) _
                        Or (nKey <> QUC_SYMBOL And (m_Aux.RealTime Or m_bSubscribingNow))
            Else
                Cancel = Not (Row = m_nUndMainRow And nKey = QUC_SYMBOL)
            End If
        End If
        
        If Not Cancel Then
            If nKey <> QUC_SYMBOL Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            Else
                m_sCurrentOriginalText = Trim$(.Cell(flexcpTextDisplay, Row, Col))
            End If
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying Filter StartEdit Exit", m_frmOwner.GetCaption

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
        Case GT_QUOTES_UNDERLYING
            gdGrid.CopyTo m_gdUnd
            
            m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
            m_Aux.FormatUndGrid
            m_Aux.FormatUndColumns
            m_AuxOut.UnderlyingUpdate True, True
            m_AuxOut.UnderlyingUpdateTotals
            m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
            
        Case GT_QUOTES_DIVIDENDS
            gdGrid.CopyTo m_gdDiv
            
            m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
            m_Aux.FormatDivColumns
            m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    
        Case GT_QUOTES_VOLA
            gdGrid.CopyTo m_gdVol
            
            m_Aux.GridLock(GT_QUOTES_VOLA).LockRedraw
            m_Aux.FormatVolColumns
            m_Aux.GridLock(GT_QUOTES_VOLA).UnlockRedraw
    
        Case GT_QUOTES_OPTIONS
            Dim nCallMask&, nPutMask&, enCallIV As EtsMmQuotesLib.MmQvIvCalcEnum, enPutIV As EtsMmQuotesLib.MmQvIvCalcEnum
            nCallMask = GetGreeksMask(enOtCall)
            nPutMask = GetGreeksMask(enOtPut)
            
            enCallIV = GetIvMask(enOtCall)
            enPutIV = GetIvMask(enOtPut)
            
            gdGrid.CopyTo m_gdOpt
            
            m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
            m_Aux.FormatOptGrid
            m_Aux.FormatOptColumns
            
            If g_Params.QuoteOptExpirySeparation And m_AuxOut.ExpiryColorSelection Then
                m_AuxOut.ExpiryColorSelection = False
                m_AuxOut.OptionsUpdateBackColor True
            End If
            
            If mnuCtxRefresh.Enabled _
                And (nCallMask <> GetGreeksMask(enOtCall) Or _
                    nPutMask <> GetGreeksMask(enOtPut) Or _
                    enCallIV <> GetIvMask(enOtCall) Or _
                    enPutIV <> GetIvMask(enOtPut)) Then
                
                Recalculate True
            Else
                m_AuxOut.OptionsUpdate True, True, True
            End If
            
            RefreshOptsGrid
            m_AuxOut.ShowSelectedRow
           
            m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
            
    End Select
    
    UserControl_Resize
End Sub

Private Sub mnuCtxAutosizeCol_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_QUOTES_UNDERLYING
            With fgUnd
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying popup menu. ""RightClick -> Autosize Column"" selected. " & GetStockInfo, m_frmOwner.GetCaption
            
                m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
                If m_nMenuGridCol <> 1 Then
                    .AutoSize m_nMenuGridCol, m_nMenuGridCol
                Else
                    .AutoSize m_nMenuGridCol, m_nMenuGridCol, , 300
                End If
                
                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= QUC_SYMBOL And nIdx <= QUC_LAST_COLUMN Then
                    m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                End If
                
                m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
            End With

        Case GT_QUOTES_OPTIONS
            With fgOpt
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Autosize Column"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
            
                m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
                .AutoSize m_nMenuGridCol, m_nMenuGridCol
                
                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= QOC_C_SYMBOL And nIdx <= QOC_LAST_COLUMN Then
                    m_gdOpt.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                End If
                
                m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
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
        Case GT_QUOTES_UNDERLYING
            With fgUnd
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying popup menu. ""RightClick -> Autosize Grid"" selected. " & GetStockInfo, m_frmOwner.GetCaption
                    
                m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
                .AutoSize 1, 2, , 300
                .AutoSize 2, .Cols - 1
                
                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= QUC_SYMBOL And nIdx <= QUC_LAST_COLUMN Then
                        m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next
                
                m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
            End With

        Case GT_QUOTES_OPTIONS
            With fgOpt
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Autosize Grid"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                    
                m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
                .AutoSize 0, .Cols - 1
                
                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= QOC_C_SYMBOL And nIdx <= QOC_LAST_COLUMN Then
                        m_gdOpt.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next
                
                m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_QUOTES_UNDERLYING
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying popup menu. ""RightClick -> Copy Grid"" selected. " & GetStockInfo, m_frmOwner.GetCaption
            g_ClipMgr.CopyGridToClipboard fgUnd

        Case GT_QUOTES_DIVIDENDS
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend popup menu. ""RightClick -> Copy Grid"" selected. " & GetStockInfo, m_frmOwner.GetCaption
            g_ClipMgr.CopyGridToClipboard fgDiv

        Case GT_QUOTES_VOLA
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Vola popup menu. ""RightClick -> Copy Grid"" selected. " & GetStockInfo, m_frmOwner.GetCaption
            g_ClipMgr.CopyGridToClipboard fgVol

        Case GT_QUOTES_OPTIONS
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Copy Grid"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
            g_ClipMgr.CopyGridToClipboard fgOpt

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxCustomDividend_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, IIf(m_bIsDblClick, "DblClick on Dividend Grid (custom dividends). ", "Dividend popup menu. ""RightClick -> Custom Dividends"" selected. ") & GetStockInfo, m_frmOwner.GetCaption
    
    If m_Aux.Grp.IsStockOrIndex Then
        Dim collUpdUnd As New EtsGeneralLib.EtsMmFilterAtomColl
        Dim aShareUnd As EtsGeneralLib.UndAtom
        Dim aCustDivs As EtsGeneralLib.EtsDivColl
        
        Dim expDate As Date
        Dim aExp As MmQvExpAtom
        expDate = Now + 365 * 5
        

        For Each aExp In m_Aux.Grp.Und.Exp
            expDate = aExp.Expiry
        Next

        m_frmCustDivs.Init m_Aux.Grp.Und.ID, m_Aux.Grp.Und.Symbol, expDate, m_Aux.Grp.Und.UndType <> enCtStock, False
        m_frmCustDivs.BasketDivs = m_Aux.Grp.Und.BasketIndex.BasketDivs
        m_frmCustDivs.Show vbModal, Me
        If m_frmCustDivs.GotData Then
        
        
            Set aCustDivs = m_frmCustDivs.CustomDividend
            Set m_Aux.Grp.Und.Dividend.CustomDivs = aCustDivs
            
                Set aShareUnd = g_UnderlyingAll(m_Aux.Grp.Und.ID)
                If Not aShareUnd Is Nothing Then
                    
                    Dim aTmpContract As EtsGeneralLib.EtsContractAtom
                    Dim aTmpUnd As EtsGeneralLib.UndAtom
                    
                    If Not aShareUnd.Dividend Is Nothing Then
                        Set aShareUnd.Dividend.CustomDivs = aCustDivs
                    End If
                    
                    
                    Set aTmpUnd = g_Underlying(m_Aux.Grp.Und.ID)
                    
                    If Not aTmpUnd Is Nothing Then
                        If Not aTmpUnd.Dividend Is Nothing Then
                            Set aTmpUnd.Dividend.CustomDivs = aCustDivs
                        End If
                    End If
                    
                    Set aShareUnd = Nothing
                    Set aCustDivs = Nothing
                End If
            
            
            m_AuxOut.DivsUpdate
            SaveDividendsInfo
            Recalculate False
            
        End If
    End If
End Sub

Private Sub mnuCtxDefOptSort_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Default Options Sorting"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    SortOptionsByDefault
End Sub

Public Sub SortOptionsByDefault()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_Aux.Grp.ID = 0 Or m_bInProc Then Exit Sub
    
    With fgOpt
        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
        Screen.MousePointer = vbHourglass
        
        m_Aux.SortColKey = QOC_C_SERIES
        m_Aux.SortColOrder = flexSortGenericAscending
        
        .Sort = flexSortCustom
        geOpt.ShowSortImage .ColIndex(QOC_C_SERIES), 1
        
        If g_Params.QuoteOptExpirySeparation Then m_AuxOut.OptionsUpdateBackColor True
        
        m_Aux.SortColKey = -2
        
        m_AuxOut.ShowSelectedRow
        
        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
        Screen.MousePointer = vbDefault
    End With
End Sub

Private Sub mnuCtxOtcOptionCalc_Click()
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> OTC Option Calc..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    OTCOptionCalcCall
End Sub

Private Sub mnuCtxSaveSimulatedVola_Click()
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    m_Aux.Grp.Und.VolaSrv.SaveSimulatedVol
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxTradeExercise_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Exercise Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Exercise Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Exercise Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Exercise Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    If m_bInProc Or m_nMenuGridCol < 0 Or m_nMenuGridRow < 1 Or m_Aux.Grp.ID = 0 Or Not m_Aux.Grp.Und.IsTraderContract Then Exit Sub
    If m_enMenuGrid <> GT_QUOTES_OPTIONS Then Exit Sub
    
    Dim aRowData As MmQvRowData, nColIdx&, bBuy As Boolean, aOpt As EtsMmQuotesLib.MmQvOptAtom
    Dim collTrades As New clsTradeExecColl
    Dim frmExecTrades As frmExerciseTrades
    
    nColIdx = fgOpt.ColKey(m_nMenuGridCol)
    If nColIdx >= QOC_C_SYMBOL And nColIdx <= QOC_C_UPDATE_TIME _
        Or nColIdx >= QOC_P_SYMBOL And nColIdx <= QOC_P_UPDATE_TIME Then
        
        Set aRowData = QV.OptsRowData(m_nMenuGridRow)
        If Not aRowData Is Nothing Then
            Set aOpt = aRowData.Opt(IIf(nColIdx >= QOC_P_SYMBOL, enOtPut, enOtCall))
            
            If Not aOpt Is Nothing Then
                If aOpt.Qty >= BAD_LONG_VALUE Then
                    If m_Aux.Grp.IsStockOrIndex Then
                        TradeChannel.MakeTradesListForExec collTrades, m_Aux.Grp.Und.ID, 0, aOpt.ID
                    Else
                        TradeChannel.MakeTradesListForExec collTrades, 0, m_Aux.Grp.Fut.ID, aOpt.ID
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
            Set aOpt = Nothing
            Set aRowData = Nothing
        End If
    End If
    
    ResetMenuData
End Sub

Private Sub mnuCtxTradeExpiry_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Expiry Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
        
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Expiry Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
        
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Expiry Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
        
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Expiry Trades..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
        
    If m_bInProc Or m_nMenuGridCol < 0 Or m_nMenuGridRow < 1 Or m_Aux.Grp.ID = 0 Or Not m_Aux.Grp.Und.IsTraderContract Then Exit Sub
    If m_enMenuGrid <> GT_QUOTES_OPTIONS Then Exit Sub
    
    Dim aRowData As MmQvRowData, nColIdx&, bBuy As Boolean, aOpt As EtsMmQuotesLib.MmQvOptAtom
    Dim collTrades As New clsTradeExecColl
    Dim bValid As Boolean
    Dim frmExecTrades As frmExerciseTrades
    
    nColIdx = fgOpt.ColKey(m_nMenuGridCol)
    If nColIdx >= QOC_C_SYMBOL And nColIdx <= QOC_C_UPDATE_TIME _
        Or nColIdx >= QOC_P_SYMBOL And nColIdx <= QOC_P_UPDATE_TIME Then
        
        Set aRowData = QV.OptsRowData(m_nMenuGridRow)
        If Not aRowData Is Nothing Then
            Set aOpt = aRowData.Opt(IIf(nColIdx >= QOC_P_SYMBOL, enOtPut, enOtCall))
            
            If Not aOpt Is Nothing Then
                If aOpt.Qty >= BAD_LONG_VALUE Then
                    If m_Aux.Grp.IsStockOrIndex Then
                        TradeChannel.MakeTradesListForExec collTrades, m_Aux.Grp.Und.ID, 0, aOpt.ID
                    Else
                        TradeChannel.MakeTradesListForExec collTrades, 0, m_Aux.Grp.Fut.ID, aOpt.ID
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
            Set aOpt = Nothing
            Set aRowData = Nothing
        End If
    End If
    
    ResetMenuData
End Sub
Private Function InterpolateVolas(ByRef arrVola() As VolatilitySourcesLib.ExpiryStrikeVolaData, _
                                                 ByRef findVola As VolatilitySourcesLib.ExpiryStrikeVolaData) As Double
                                                 
    Dim i&
    Dim findVolaBeforeLessStrike    As VolatilitySourcesLib.ExpiryStrikeVolaData
    Dim findVolaBeforeGreaterStrike As VolatilitySourcesLib.ExpiryStrikeVolaData
    Dim findVolaAfterLessStrike     As VolatilitySourcesLib.ExpiryStrikeVolaData
    Dim findVolaAfterGreaterStrike  As VolatilitySourcesLib.ExpiryStrikeVolaData
    
    For i = LBound(arrVola) To UBound(arrVola)
        If arrVola(i).Vola > 0# Then
            If arrVola(i).Expiry < findVola.Expiry And arrVola(i).Strike <= findVola.Strike Then
                 findVolaBeforeLessStrike = arrVola(i)
            End If
            If arrVola(i).Expiry < findVola.Expiry And arrVola(i).Strike > findVola.Strike Then
                 findVolaBeforeGreaterStrike = arrVola(i)
            End If
            
            If arrVola(i).Expiry > findVola.Expiry And arrVola(i).Strike <= findVola.Strike Then
                 findVolaAfterLessStrike = arrVola(i)
            End If
            If arrVola(i).Expiry > findVola.Expiry And arrVola(i).Strike > findVola.Strike Then
                 findVolaAfterGreaterStrike = arrVola(i)
                GoTo findAll
            End If
        End If
    Next
findAll:
Dim dVola1#, dVola2#
    dVola1 = InterpolateStrike(findVola.Strike, findVolaBeforeLessStrike, findVolaBeforeGreaterStrike)
    dVola2 = InterpolateStrike(findVola.Strike, findVolaAfterLessStrike, findVolaAfterGreaterStrike)
    
    If dVola1 > 0# And dVola2 > 0# Then
        Dim dP As Double
        Dim dN As Double
        Dim dChange As Double
        Dim dRatio As Double
        Dim dtNow As Date
                    
        dtNow = GetNewYorkTime
        
        dP = Sqr(findVola.Expiry - dtNow) - Sqr(findVolaBeforeLessStrike.Expiry - dtNow)
        dN = Sqr(findVolaAfterLessStrike.Expiry - dtNow) - Sqr(findVolaBeforeLessStrike.Expiry - dtNow)
                    
        dChange = dVola2 - dVola1
        dRatio = dP / dN
        InterpolateVolas = dVola1 + dChange * dRatio
        ElseIf dVola1 > 0# Then
            InterpolateVolas = dVola1
    ElseIf dVola2 > 0# Then
        InterpolateVolas = dVola2
    Else
        InterpolateVolas = BAD_DOUBLE_VALUE
    End If

End Function

Private Function InterpolateVola(ByRef arrVola() As VolatilitySourcesLib.ExpiryStrikeVolaData, _
                                                 ByRef findVola As VolatilitySourcesLib.ExpiryStrikeVolaData) As Double
                                                 
    Dim i&
    Dim findVolaLessStrike    As VolatilitySourcesLib.ExpiryStrikeVolaData
    Dim findVolaGreaterStrike As VolatilitySourcesLib.ExpiryStrikeVolaData
    
    For i = LBound(arrVola) To UBound(arrVola)
        If arrVola(i).Vola > 0# Then
            If arrVola(i).Expiry = findVola.Expiry And arrVola(i).Strike <= findVola.Strike Then
                 findVolaLessStrike = arrVola(i)
            End If
            If arrVola(i).Expiry = findVola.Expiry And arrVola(i).Strike > findVola.Strike Then
                 findVolaGreaterStrike = arrVola(i)
                 GoTo findAll
            End If
            
            If arrVola(i).Expiry > findVola.Expiry Then GoTo findAll

        End If
    Next
findAll:
Dim dVola#
    dVola = InterpolateStrike(findVola.Strike, findVolaLessStrike, findVolaGreaterStrike)
    
    If dVola > 0# Then
       InterpolateVola = dVola
    Else
        InterpolateVola = BAD_DOUBLE_VALUE
    End If

End Function

Private Function InterpolateStrike(dStrike As Double, _
                                   ByRef aVolaLess As VolatilitySourcesLib.ExpiryStrikeVolaData, _
                                   ByRef aVolaGreater As VolatilitySourcesLib.ExpiryStrikeVolaData) As Double
If aVolaLess.Strike = 0 And aVolaGreater.Strike = 0 Then
    InterpolateStrike = BAD_DOUBLE_VALUE
ElseIf aVolaLess.Strike = 0 Then
    InterpolateStrike = aVolaGreater.Vola
ElseIf aVolaGreater.Strike = 0 Then
    InterpolateStrike = aVolaLess.Vola
Else
    Dim dP As Double
    Dim dN As Double
    Dim dChange As Double
    Dim dRatio As Double
                    
    dP = dStrike - aVolaLess.Strike
    dN = aVolaGreater.Strike - aVolaLess.Strike
                    
    dChange = aVolaGreater.Vola - aVolaLess.Vola
    dRatio = dP / dN
    InterpolateStrike = aVolaLess.Vola + dChange * dRatio
End If
End Function

Private Function GetVola(ByRef arrVola() As VolatilitySourcesLib.ExpiryStrikeVolaData, Expiry As Date, Strike As Double) As Double
    Dim i&
    For i = LBound(arrVola) To UBound(arrVola)
        If arrVola(i).Strike = Strike And Expiry <= arrVola(i).Expiry Then
           GetVola = arrVola(i).Vola / 100#
           GoTo Exi
        End If
    Next
GetVola = 0#
Exi:
End Function


Private Function FitToImpiledInterpolatedMissing(ByRef arrVola() As VolatilitySourcesLib.ExpiryStrikeVolaData) As Long
    On Error GoTo Exi
    Dim nCount As Long: nCount = 0
    Dim i&
    Dim nLastGoodStrike As Long
    Dim dtLastGoodExp As Date
    Dim lastGoodVola As Double
    
    FitToImpiledInterpolatedMissing = 0
    For i = LBound(arrVola) To UBound(arrVola)
        If arrVola(i).Vola = BAD_DOUBLE_VALUE Then
           arrVola(i).Vola = InterpolateVolas(arrVola, arrVola(i))
           If arrVola(i).Vola > 0 Then
                nLastGoodStrike = arrVola(i).Strike
                dtLastGoodExp = arrVola(i).Expiry
                lastGoodVola = arrVola(i).Vola
                nCount = nCount + 1
           End If
        ElseIf arrVola(i).Vola = -1 Then
            If dtLastGoodExp = arrVola(i).Expiry Then
                arrVola(i).Vola = lastGoodVola
                nCount = nCount + 1
            Else
                arrVola(i).Vola = BAD_DOUBLE_VALUE
            End If
        Else
            nLastGoodStrike = arrVola(i).Strike
            dtLastGoodExp = arrVola(i).Expiry
            lastGoodVola = arrVola(i).Vola
            nCount = nCount + 1
        End If
    Next
Exi:
    FitToImpiledInterpolatedMissing = nCount
End Function

Private Function FitToImpFillVolasForExpiry(ByRef aExp As EtsMmQuotesLib.MmQvExpAtom, _
                                    ByRef arrVola() As VolatilitySourcesLib.ExpiryStrikeVolaData, _
                                    bSilent As Boolean, lSurfaceID As Long) As Long
    On Error Resume Next
    Dim aStr As EtsMmQuotesLib.MmQvStrikeAtom, aPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aUndDefQuote As EtsMmQuotesLib.MmQvQuoteAtom, enPriceStatus As EtsGeneralLib.EtsReplacePriceStatusEnum
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim nExactCount&, dUndPrice#, div#, dIVBetter#, dAtmStrike#, nAtmIdx&, nFutureID&
    Dim nFirstIdx&, nLastIdx&, nLastGoodIdx&, nIdx&, i&
    Dim bSomeAdded   As Boolean: bSomeAdded = False
    Dim bSomeRemoved As Boolean: bSomeRemoved = False
    Dim bFlat As Boolean
    Dim nBadBidCount As Long
    Dim bOneVolaExists As Boolean
    Dim aSurfaceStrikeColl As EtsMmQuotesLib.MmQvStrikeColl
    
    nBadBidCount = 0
    bOneVolaExists = False
    FitToImpFillVolasForExpiry = 0
    If aExp.Strike.Count <= 0 Then Exit Function
    
    ''''''''''''''''''''''''''''''''''''''''''''''''''
    ' Generate strike collection for current surface '
    ''''''''''''''''''''''''''''''''''''''''''''''''''
    Set aSurfaceStrikeColl = GetStrikeCollBySurface(lSurfaceID, aExp.Strike)
    
    If (aSurfaceStrikeColl.Count <= 0) Then
        Exit Function
    End If
    
    nExactCount = -1
    nExactCount = UBound(arrVola)
    If nExactCount > 0 Then
        ReDim Preserve arrVola(1 To nExactCount + aSurfaceStrikeColl.Count)
    Else
        nExactCount = 0
        ReDim arrVola(1 To aSurfaceStrikeColl.Count)
    End If
    
    nFirstIdx = nExactCount + 1
    nLastIdx = nExactCount + aSurfaceStrikeColl.Count
    dUndPrice = -1#
    If m_Aux.Grp.IsStockOrIndex Then
        Set aUndDefQuote = m_Aux.Grp.Und.Quote(0)
        dUndPrice = m_Aux.Grp.Und.UndPriceProfile.GetUndPriceMid(aUndDefQuote.PriceBid, aUndDefQuote.PriceAsk, _
                                                        aUndDefQuote.PriceLast, g_Params.UndPriceToleranceValue, _
                                                        g_Params.PriceRoundingRule, enPriceStatus)
        Set aUndDefQuote = Nothing
    Else
        Set aUndDefQuote = m_Aux.Grp.Fut.Quote(0)
        dUndPrice = m_Aux.Grp.Fut.UndPriceProfile.GetUndPriceMid(aUndDefQuote.PriceBid, aUndDefQuote.PriceAsk, _
                                                        aUndDefQuote.PriceLast, g_Params.UndPriceToleranceValue, _
                                                        g_Params.PriceRoundingRule, enPriceStatus)
        Set aUndDefQuote = Nothing
    End If
    
    dAtmStrike = m_Aux.Grp.Und.AtmStrike(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
    nAtmIdx = nFirstIdx '-1
    
    ' OTM - take call if strike >= undprice, and put if strike < undprice
    nIdx = nFirstIdx - 1
    For Each aStr In aSurfaceStrikeColl
        div = 0#
        dIVBetter = 0#
        Dim bAdd As Boolean
        bAdd = False
        bFlat = False
        
        For Each aPair In aStr.OptPair
            Dim dRootID As Long
            
            If Not aPair.Opt(enOtCall) Is Nothing Then
                dRootID = aPair.Opt(enOtCall).RootID
            ElseIf Not aPair.Opt(enOtPut) Is Nothing Then
                dRootID = aPair.Opt(enOtPut).RootID
            End If
                
            If g_Params.QuoteFitToImpCurveType = enFitToImpCurveOTM And dUndPrice > 0 Then
                If aStr.Strike >= dUndPrice Then
                    Set aQuote = aPair.Opt(enOtCall).Quote(0)
                    If aQuote Is Nothing Then Set aQuote = aPair.Opt(enOtPut).Quote(0)
                Else
                    Set aQuote = aPair.Opt(enOtPut).Quote(0)
                    If aQuote Is Nothing Then Set aQuote = aPair.Opt(enOtCall).Quote(0)
                End If
            Else
                If g_Params.QuoteFitToImpCurveType <> enFitToImpCurvePuts Then
                    Set aQuote = aPair.Opt(enOtCall).Quote(0)
                    If aQuote Is Nothing Then Set aQuote = aPair.Opt(enOtPut).Quote(0)
                Else
                    Set aQuote = aPair.Opt(enOtPut).Quote(0)
                    If aQuote Is Nothing Then Set aQuote = aPair.Opt(enOtCall).Quote(0)
                End If
            End If
            
            If Not aQuote Is Nothing Then
                If True Then
                    bAdd = True
                    div = aQuote.IV
                    If div > 0 Then bOneVolaExists = True
                    If div < 0 Then div = 0#
                    
                    If g_Params.QuoteFitToImpFlatNoBid And aQuote.PriceBid <= 0 Then div = 0
                
                    If aQuote.PriceBid <= 0 Then
                        nBadBidCount = nBadBidCount + 1
                    End If
                    
                    If aStr.OptPair.Count > 1 Then
                        If dIVBetter <= 0# And (aQuote.PriceBid > 0 Or aQuote.PriceAsk > 0 Or aQuote.PriceLast > 0) Then
                            dIVBetter = div
                        End If
                    Else
                        dIVBetter = div
                    End If
               Else
                If g_Params.QuoteFitToImpFlatNoBid And aQuote.PriceBid <= 0 Then bFlat = True
               End If
            Else
                Debug.Assert False
                div = 0#
            End If
                
            Set aQuote = Nothing
        Next
        
            nIdx = nIdx + 1
            arrVola(nIdx).Expiry = aExp.ExpiryOV
            arrVola(nIdx).Strike = aStr.Strike
    
        If bAdd Then
            arrVola(nIdx).Vola = IIf(dIVBetter <= 0#, div, dIVBetter) * 100#
            bSomeAdded = True
        Else
            If Not bFlat Then
                arrVola(nIdx).Vola = BAD_DOUBLE_VALUE
            Else
                arrVola(nIdx).Vola = -1
            End If
            
                bSomeRemoved = True
        End If
        
            If dAtmStrike > aStr.Strike Then nAtmIdx = nIdx
        
    Next
    
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'    If nBadBidCount = aSurfaceStrikeColl.Count And aSurfaceStrikeColl.Count > 0 And g_Params.QuoteFitToImpFlatNoBid Then
'        msgStr = "Currently there are no option bids on Quote View."
'        aFrm.SetCheckBoxCaption "Use Flat Vola if No Bid"
'        aFrm.SetCheckBoxChecked g_Params.QuoteFitToImpFlatNoBid
'        aFrm.Init "Information", msgStr, MBM_FIT2IMP_NO_BIDS, True
'        aFrm.Show vbModeless
'        FitToImpFillVolasForExpiry = -2
'        GoTo Ex
'    End If
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    
    If bSomeAdded And bSomeRemoved Then
    ' interpolate missing
        Dim nIdxInterpolate&, nIdxFind&
        Dim dLastGoodVola#: dLastGoodVola = 0#
        
        For nIdxInterpolate = nFirstIdx To nLastIdx
        
            If arrVola(nIdxInterpolate).Vola = BAD_DOUBLE_VALUE Then
                Dim dStrike1#, dStrike2#
                Dim dVola1#, dVola2#
                dStrike1 = 0#
                dStrike2 = 0#

                For nIdxFind = nFirstIdx To nLastIdx
                    If arrVola(nIdxFind).Vola > 0# Then
                        If arrVola(nIdxFind).Strike < arrVola(nIdxInterpolate).Strike Then
                            dStrike1 = arrVola(nIdxFind).Strike
                            dVola1 = arrVola(nIdxFind).Vola
                        End If
                        If arrVola(nIdxFind).Strike > arrVola(nIdxInterpolate).Strike Then
                            dStrike2 = arrVola(nIdxFind).Strike
                            dVola2 = arrVola(nIdxFind).Vola
                            GoTo Found
                        End If
                    End If
                Next
Found:
                
                If dStrike1 = 0# And dStrike2 = 0# Then
                    arrVola(nIdxInterpolate).Vola = BAD_DOUBLE_VALUE
                ElseIf dStrike1 = 0# And dStrike2 > 0# Then
                   arrVola(nIdxInterpolate).Vola = dVola2
                ElseIf dStrike1 <> 0# And dStrike2 = 0# Then
                   arrVola(nIdxInterpolate).Vola = dVola1
                ElseIf dStrike1 <> 0# And dStrike2 <> 0# Then
                    Dim dP As Double
                    Dim dN As Double
                    Dim dChange As Double
                    Dim dRatio As Double
                    
                    dP = arrVola(nIdxInterpolate).Strike - dStrike1
                    dN = dStrike2 - dStrike1
                    dChange = dVola2 - dVola1
                    dRatio = dP / dN
                    arrVola(nIdxInterpolate).Vola = dVola1 + dChange * dRatio
                End If
            ElseIf arrVola(nIdxInterpolate).Vola = -1# And dLastGoodVola > 0# Then
                arrVola(nIdxInterpolate).Vola = dLastGoodVola
            End If
            
            If arrVola(nIdxInterpolate).Vola >= 0# Then
                dLastGoodVola = arrVola(nIdxInterpolate).Vola
            End If
        Next
    ElseIf Not bSomeAdded And bSomeRemoved Then
        FitToImpFillVolasForExpiry = -1
        GoTo Ex
    Else
        FitToImpFillVolasForExpiry = aSurfaceStrikeColl.Count
    End If
    
    ' search for nearest to ATM strike with vola
    If nAtmIdx >= 0 Then
    
        If arrVola(nAtmIdx).Vola <= 0 Then
        
            nLastGoodIdx = -1

            ' search forth
            For nIdx = nAtmIdx + 1 To nLastIdx
                If arrVola(nIdx).Vola > 0 Then
                    nLastGoodIdx = nIdx
                    Exit For
                End If
            Next

            If nLastGoodIdx < 0 Then
                ' search back
                For nIdx = nAtmIdx - 1 To nFirstIdx Step -1
                    If arrVola(nIdx).Vola > 0 Then
                        nLastGoodIdx = nIdx
                        Exit For
                    End If
                Next
            End If

            nAtmIdx = nLastGoodIdx
        End If
        
    End If

    If nAtmIdx > 0 Then
    
        nLastGoodIdx = nAtmIdx
        ' go forth from nearest to ATM strike with vola
        For nIdx = nAtmIdx + 1 To nLastIdx
            If arrVola(nIdx).Vola > 0 Then
                nLastGoodIdx = nIdx
            Else
                If nLastGoodIdx > 0 Then
                    arrVola(nIdx).Vola = arrVola(nLastGoodIdx).Vola
                End If
            End If
        Next

        nLastGoodIdx = nAtmIdx
        ' go back from nearest to ATM strike with vola
        For nIdx = nAtmIdx - 1 To nFirstIdx Step -1
            If arrVola(nIdx).Vola > 0 Then
                nLastGoodIdx = nIdx
            Else
                If nLastGoodIdx > 0 Then
                    arrVola(nIdx).Vola = arrVola(nLastGoodIdx).Vola
                End If
            End If
        Next

        FitToImpFillVolasForExpiry = aSurfaceStrikeColl.Count
        
    Else
    
        If Not bSilent Then _
            LogEvent EVENT_ERROR, m_Aux.Grp.Und.Symbol & _
                    ": Fail to fit vola to implied. No valid implied vola to fit for " _
                    & Format$(aExp.Expiry, "MMMYY") & " expiry."

        If nExactCount > 0 Then
            ReDim Preserve arrVola(1 To nExactCount)
        Else
            If (nExactCount = 0) Then
                nExactCount = aSurfaceStrikeColl.Count
        End If
        End If
        
        If bSomeRemoved Then
            If nExactCount = 0 Then FitToImpFillVolasForExpiry = -1
        Else
            FitToImpFillVolasForExpiry = nExactCount
        End If
        
    End If
Ex:
    Set aSurfaceStrikeColl = Nothing
End Function

Private Sub mnuCtxVolaFitToImpAll_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Fit Vola To Implied -> All Expiries"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom
    Dim arrVola() As VolatilitySourcesLib.ExpiryStrikeVolaData, nExactCount&
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim nCountTmp As Long
    
    Dim vtSurfaceID As Variant
    Dim lSurfaceID As Long
    Dim aSurfaceColl As Collection
    
    If m_bInProc And m_bLastQuoteReqNow Or m_Aux.Grp.ID = 0 _
        Or m_enMenuGrid <> GT_QUOTES_VOLA _
            And m_enMenuGrid <> GT_QUOTES_OPTIONS _
            And m_enMenuGrid <> GT_QUOTES_UNDERLYING Then Exit Sub

    'Prepare expiry collection
    If m_Aux.Grp.IsStockOrIndex Then
        Set aExpColl = m_Aux.Grp.Und.Exp
    Else
        Set aExpColl = m_Aux.Grp.Fut.Exp
    End If

    '''''''''''''''''''''''''''''''''''''''''''''
    'Generate surface coll for all expiry's     '
    '''''''''''''''''''''''''''''''''''''''''''''
    Set aSurfaceColl = GetSurfaceCollByExpiryColl(aExpColl)
    
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ' Process fit2impl for all expiry's in expiry collection
    For Each vtSurfaceID In aSurfaceColl
    
    If aExpColl.Count > 0 Then
        Screen.MousePointer = vbHourglass

            ReDim arrVola(0 To 0)
            lSurfaceID = CLng(vtSurfaceID)
            
        nExactCount = 0
        For Each aExp In aExpColl
                nCountTmp = FitToImpFillVolasForExpiry(aExp, arrVola, False, lSurfaceID)
            nExactCount = nExactCount + IIf(nCountTmp = -1, 0, nCountTmp)
        Next

        If nExactCount > 0 Then
            Err.Clear
           If FitToImpiledInterpolatedMissing(arrVola) > 0 Then
                    m_Aux.Grp.Und.VolaSrv.SetExpiryAndStrikeVolaAll arrVola, lSurfaceID
            If Err.Number = 0 Then
                m_bVolaChangedNow = True
                g_Params.FireInternalVolatilityChanged m_Aux.Grp.Und.Symbol
                m_bVolaChangedNow = False
                    Else
                        LogEvent EVENT_ERROR, m_Aux.Grp.Und.Symbol & ": Fail to fit vola to implied. " & Err.Description
                    End If
                End If
            End If
            
            Screen.MousePointer = vbDefault
        End If
    
    Next

    If g_Params.UseTheoVolatility Then
        OptionsUpdateVola
        CalculateUnderlyingOptions True, , , True
        m_AuxOut.UnderlyingUpdateTotals
        m_AuxOut.VolaUpdateValues
        UpdateTotals
    End If

    m_AuxOut.OptionsUpdate False, True, True
    
EH:
    Set aExpColl = Nothing
    Set aSurfaceColl = Nothing
    Erase arrVola
End Sub

Private Sub mnuCtxVolaFitToImpCur_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Fit Vola To Implied -> This Expiry"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    Dim aStr As EtsMmQuotesLib.MmQvStrikeAtom, nOptType&
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, nMask(enOtPut To enOtCall) As Long, sKey$, nRow&
    Dim aExpInt As EtsMmQuotesLib.MmQvExpAtom, aExp As EtsMmQuotesLib.MmQvExpAtom, nColIdx&, aPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim arrVolaTmp() As VolatilitySourcesLib.ExpiryStrikeVolaData
    Dim arrVola() As VolatilitySourcesLib.ExpiryStrikeVolaData, nExactCount&, dVola#
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
    
    Dim aSurfaceStrikeColl As EtsMmQuotesLib.MmQvStrikeColl
    Dim aSurfaceColl As Collection
    Dim vtSurfaceID As Variant
    Dim lSurfaceID As Long
    
    If m_bInProc And m_bLastQuoteReqNow Or m_Aux.Grp.ID = 0 _
        Or m_enMenuGrid <> GT_QUOTES_VOLA And m_enMenuGrid <> GT_QUOTES_OPTIONS Then Exit Sub
    
    'prepare expiry coll for current contract
    If m_Aux.Grp.IsStockOrIndex Then
        Set aExpColl = m_Aux.Grp.Und.Exp
    Else
        Set aExpColl = m_Aux.Grp.Fut.Exp
    End If
    
    Set aExp = Nothing

    'Get current expiry to process
    If m_enMenuGrid = GT_QUOTES_VOLA Then
        If m_nMenuGridCol < 1 Or m_nMenuGridCol > aExpColl.Count Then Exit Sub
        Set aExp = fgVol.ColData(m_nMenuGridCol)
        
    Else
        nColIdx = fgOpt.ColKey(m_nMenuGridCol)
        If nColIdx >= QOC_C_SYMBOL And nColIdx <= QOC_LAST_COLUMN Then
            Set aExp = QV.OptsRowData(m_nMenuGridRow).Exp
        End If
    End If
    
    If aExp Is Nothing Then GoTo Ex
    
    nMask(enOtPut) = GetGreeksMask(enOtPut)
    nMask(enOtCall) = GetGreeksMask(enOtCall)
    
    Screen.MousePointer = vbHourglass
    m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
    
    '''''''''''''''''''''''''''''''''''''''''''
    'Generate surface coll for current expiry '
    '''''''''''''''''''''''''''''''''''''''''''
    Set aSurfaceColl = GetSurfaceCollByExpiry(aExp)
    
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''
    'Process FIt2Impl for all surfaces in current expiry '
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''
    For Each vtSurfaceID In aSurfaceColl
    
        ReDim arrVola(0 To 0)
        ReDim arrVolaTmp(0 To 0)
        lSurfaceID = CLng(vtSurfaceID)
        
        nExactCount = FitToImpFillVolasForExpiry(aExp, arrVola, True, lSurfaceID)
        
        ''''''''''''''''''''''''''''''''''''''''''''''''''
        ' Generate strike collection for current surface '
        ''''''''''''''''''''''''''''''''''''''''''''''''''
        Set aSurfaceStrikeColl = GetStrikeCollBySurface(lSurfaceID, aExp.Strike)
                
        'in this case we need interpolate value from other expiry's
        If nExactCount = -1 Then 'No one point is added to surface
        
            'fit for all expiry's in temp for future use
        For Each aExpInt In aExpColl
                FitToImpFillVolasForExpiry aExpInt, arrVolaTmp, True, lSurfaceID
        Next
            
            'Interpolate missing values
          FitToImpiledInterpolatedMissing arrVolaTmp

        Dim posCount As Long: posCount = 1
            ReDim arrVola(1 To aSurfaceStrikeColl.Count)
            
            ' fill array
            For Each aStr In aSurfaceStrikeColl
            arrVola(posCount).Expiry = aExp.ExpiryOV
            arrVola(posCount).Strike = aStr.Strike
            arrVola(posCount).Vola = InterpolateVola(arrVolaTmp, arrVola(posCount))
            If arrVola(posCount).Vola > 0# Then posCount = posCount + 1
        Next
        nExactCount = posCount - 1
            
    End If
        
        'Some vola exists process it
    If nExactCount > 0 Then
            
        Err.Clear
            'save volatilities in vme and db then publish changed
            m_Aux.Grp.Und.VolaSrv.SetExpiryAndStrikeVolaAll arrVola, lSurfaceID
        
        If Err.Number = 0 Then
            
            m_bVolaChangedNow = True
            g_Params.FireInternalVolatilityChanged m_Aux.Grp.Und.Symbol
            m_bVolaChangedNow = False
            
                'recalc all options
                Dim bAssignedFromPair As Boolean
                For Each aStr In aSurfaceStrikeColl
                    bAssignedFromPair = False
                For Each aPair In aStr.OptPair
                    For nOptType = enOtPut To enOtCall
                        Set aOpt = aPair.Opt(nOptType)
                        If aOpt.ID <> 0 Then
                                aOpt.Vola = m_Aux.Grp.Und.VolaSrv.OptionVola(aExp.ExpiryOV, aStr.Strike, lSurfaceID)
                            bAssignedFromPair = True
                            For Each aQuote In aOpt.Quote
                                If g_Params.UseTheoVolatility Or aQuote.CalcGreeksIVType = enMmQvCalcGreeksIVTheo Then
                                    CalcOptionGreeks aOpt, aQuote, aExp, nMask(nOptType), GetIvMask(nOptType)
                                End If
                            Next
                        End If
                        Set aOpt = Nothing
                    Next
                Next
                    
                    If Not bAssignedFromPair Then
                    aOpt.Vola = GetVola(arrVolaTmp, aOpt.ExpiryOV, aOpt.Strike)
                    End If
            Next
        Else
            LogEvent EVENT_ERROR, m_Aux.Grp.Und.Symbol & ": Fail to fit vola to implied. " & Err.Description
        End If
        
        m_AuxOut.VolaUpdateValues
    Else
       LogEvent EVENT_ERROR, m_Aux.Grp.Und.Symbol & _
                    ": Fail to fit vola to implied. No valid implied vola to fit for " _
                    & Format$(aExp.Expiry, "MMMYY") & " expiry."
    End If
        Set aSurfaceStrikeColl = Nothing
    Next
    
    'update and recalc
    If g_Params.UseTheoVolatility Then
        m_bInProc = True
        CalculateUnderlyingOptions False
        m_AuxOut.UnderlyingUpdateTotals
        UpdateTotals
        m_bInProc = False
    End If
    
Ex:
    Set aExp = Nothing
    Set aExpColl = Nothing
    Set aSurfaceStrikeColl = Nothing
    Set aSurfaceColl = Nothing
    
    m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
    Erase arrVola
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxVolaFlatAllByCur_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Flat Vola -> All Expiries by This ATM"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, dVola#, nColIdx&, bUpdateOptions As Boolean
    Dim lSurfaceID As Long
    Dim vtSurfaceID As Variant
    Dim aSurfaceColl As Collection
    
    If m_bInProc And m_bLastQuoteReqNow Or m_Aux.Grp.ID = 0 _
        Or m_enMenuGrid <> GT_QUOTES_VOLA And m_enMenuGrid <> GT_QUOTES_OPTIONS Then Exit Sub
    
    'prepare expiry atom for processing
    If m_enMenuGrid = GT_QUOTES_VOLA Then
        If m_Aux.Grp.IsStockOrIndex Then
            If m_nMenuGridCol < 1 Or m_nMenuGridCol > m_Aux.Grp.Und.Exp.Count Then Exit Sub
        Else
            If m_nMenuGridCol < 1 Or m_nMenuGridCol > m_Aux.Grp.Fut.Exp.Count Then Exit Sub
        End If
        Set aExp = fgVol.ColData(m_nMenuGridCol)
    Else 'If m_enMenuGrid = GT_QUOTES_OPTIONS Then
        nColIdx = fgOpt.ColKey(m_nMenuGridCol)
        If nColIdx >= QOC_C_SYMBOL And nColIdx <= QOC_LAST_COLUMN Then
            Set aExp = QV.OptsRowData(m_nMenuGridRow).Exp
        End If
    End If
    
    If aExp Is Nothing Then Exit Sub

    Screen.MousePointer = vbHourglass
    
    'generate surface coll for current expiry
    Set aSurfaceColl = GetSurfaceCollByExpiry(aExp)
    
    'for each surface set atm vola by one expiry
    For Each vtSurfaceID In aSurfaceColl
        lSurfaceID = CLng(vtSurfaceID)
        dVola = m_Aux.Grp.Und.AtmVola(aExp, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, False, lSurfaceID)
        If dVola > 0# Then
            Err.Clear
            'pusblish data to VME
            m_Aux.Grp.Und.VolaSrv.SetExpiryVolaAll dVola, lSurfaceID
            If Err.Number = 0 Then
                bUpdateOptions = True
                m_bVolaChangedNow = True
                g_Params.FireInternalVolatilityChanged m_Aux.Grp.Und.Symbol
                m_bVolaChangedNow = False
            Else
                LogEvent EVENT_ERROR, m_Aux.Grp.Und.Symbol & ": Fail to flat vola. " & Err.Description
            End If
        Else
            bUpdateOptions = True
        End If
    Next
        
    'Update view and recalc new values
        If bUpdateOptions Then
            If g_Params.UseTheoVolatility Then
                m_bInProc = True
                CalculateUnderlyingOptions True
                m_bInProc = False
                m_AuxOut.UnderlyingUpdateTotals
                UpdateTotals
            Else
                OptionsUpdateVola
            End If
            m_AuxOut.OptionsUpdate False, True, True
            m_AuxOut.VolaUpdateValues
        End If
        
    Screen.MousePointer = vbDefault
    
    Set aExp = Nothing
    Set aSurfaceColl = Nothing
End Sub

Private Sub mnuCtxVolaFlatCur_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Flat Vola -> This Expiry by ATM"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    Dim aStr As EtsMmQuotesLib.MmQvStrikeAtom, nOptType&
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, nMask(enOtPut To enOtCall) As Long, sKey$, nRow&
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, dVola#, nColIdx&, aPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim bUpdateOptions As Boolean
    Dim lSurfaceID As Long
    Dim vtSurfaceID As Variant
    Dim aSurfaceColl As Collection
    
    If m_bInProc And m_bLastQuoteReqNow Or m_Aux.Grp.ID = 0 _
        Or m_enMenuGrid <> GT_QUOTES_VOLA And m_enMenuGrid <> GT_QUOTES_OPTIONS Then Exit Sub
    
    'preapare expiry atom for processing
    If m_enMenuGrid = GT_QUOTES_VOLA Then
        If m_Aux.Grp.IsStockOrIndex Then
            If m_nMenuGridCol < 1 Or m_nMenuGridCol > m_Aux.Grp.Und.Exp.Count Then Exit Sub
        Else
            If m_nMenuGridCol < 1 Or m_nMenuGridCol > m_Aux.Grp.Fut.Exp.Count Then Exit Sub
        End If
        
        Set aExp = fgVol.ColData(m_nMenuGridCol)
        
    Else 'If m_enMenuGrid = GT_QUOTES_OPTIONS Then
        
        nColIdx = fgOpt.ColKey(m_nMenuGridCol)
        If nColIdx >= QOC_C_SYMBOL And nColIdx <= QOC_LAST_COLUMN Then
            Set aExp = QV.OptsRowData(m_nMenuGridRow).Exp
        End If
    End If
    
    If aExp Is Nothing Then Exit Sub
    
    'Get surface coll for current expiry
    Set aSurfaceColl = GetSurfaceCollByExpiry(aExp)
    
        Screen.MousePointer = vbHourglass
        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
        
        nMask(enOtPut) = GetGreeksMask(enOtPut)
        nMask(enOtCall) = GetGreeksMask(enOtCall)
        
    'Set atm vola for all existing surface in current expiry
    For Each vtSurfaceID In aSurfaceColl
        lSurfaceID = CLng(vtSurfaceID)
        'Get atm vola for current expiry
        dVola = m_Aux.Grp.Und.AtmVola(aExp, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, False, lSurfaceID)
        If dVola > 0# Then
            Err.Clear
            'Pub data to VME
            m_Aux.Grp.Und.VolaSrv.SetExpiryVola aExp.ExpiryOV, dVola, lSurfaceID
            If Err.Number = 0 Then
                bUpdateOptions = True
                m_bVolaChangedNow = True
                g_Params.FireInternalVolatilityChanged m_Aux.Grp.Und.Symbol
                m_bVolaChangedNow = False
            Else
                LogEvent EVENT_ERROR, m_Aux.Grp.Und.Symbol & ": Fail to flat vola. " & Err.Description
            End If
        Else
            bUpdateOptions = True
        End If
    Next
                
    'update options quote with new vola value
        If bUpdateOptions Then
            For Each aStr In aExp.Strike
                For Each aPair In aStr.OptPair
                lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aPair.RootID)
                dVola = m_Aux.Grp.Und.VolaSrv.OptionVola(aExp.ExpiryOV, aStr.Strike, lSurfaceID)
                    For nOptType = enOtPut To enOtCall
                        Set aOpt = aPair.Opt(nOptType)
                        If aOpt.ID <> 0 Then
                            aOpt.Vola = dVola
                            For Each aQuote In aOpt.Quote
                                If g_Params.UseTheoVolatility Or aQuote.CalcGreeksIVType = enMmQvCalcGreeksIVTheo Then
                                    CalcOptionGreeks aOpt, aQuote, aExp, nMask(nOptType), GetIvMask(nOptType)
                                End If
                            Next
                        End If
                        Set aOpt = Nothing
                    Next
                Next
            Next
        End If
    
    'refresh view port and recalc data
        If g_Params.UseTheoVolatility Then
            m_bInProc = True
            CalculateUnderlyingOptions False
            m_AuxOut.UnderlyingUpdateTotals
            UpdateTotals
            m_bInProc = False
        End If
        m_AuxOut.VolaUpdateValues
        
        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
        
    Set aExp = Nothing
    Set aSurfaceColl = Nothing
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxVolaFlatAll_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Flat Vola -> All Expiries by Their ATM"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    
    Dim nCount&, aExp As EtsMmQuotesLib.MmQvExpAtom, dVola#
    Dim arrVola() As VolatilitySourcesLib.ExpiryVolaData, nExactCount&
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl, bUpdateOptions As Boolean
    Dim lSurfaceID As Long
    Dim vtSurfaceID As Variant
    Dim aSurfaceColl As Collection
    Dim aOrigExpColl As EtsMmQuotesLib.MmQvExpColl

    If m_bInProc And m_bLastQuoteReqNow Or m_Aux.Grp.ID = 0 _
        Or m_enMenuGrid <> GT_QUOTES_VOLA And m_enMenuGrid <> GT_QUOTES_OPTIONS And m_enMenuGrid <> GT_QUOTES_UNDERLYING _
        And (Not m_Aux.IsSimulatedFlat) Then Exit Sub

    'Prepare expiry collection for flatVola processing
    If m_Aux.Grp.IsStockOrIndex Then
        Set aExpColl = m_Aux.Grp.Und.Exp
    Else
        Set aExpColl = m_Aux.Grp.Fut.Exp
    End If
    
    'Check if expiry is exists
    nCount = aExpColl.Count
    
    If nCount > 0 Then
    
        Screen.MousePointer = vbHourglass
        'Get all surfaces for expiry coll
        Set aSurfaceColl = GetSurfaceCollByExpiryColl(aExpColl)
        
        For Each vtSurfaceID In aSurfaceColl
            lSurfaceID = CLng(vtSurfaceID)

            'Get expiry coll in current surface
            Set aOrigExpColl = GetExpiryCollBySurface(lSurfaceID, aExpColl)
            
            'Process flat vola for exp and surface
        nExactCount = 0
        ReDim arrVola(1 To nCount)
            For Each aExp In aOrigExpColl
                dVola = m_Aux.Grp.Und.AtmVola(aExp, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, False, lSurfaceID)
            If dVola > 0# Then
                nExactCount = nExactCount + 1
                arrVola(nExactCount).Expiry = aExp.ExpiryOV
                arrVola(nExactCount).Vola = dVola * 100#
            End If
        Next

            'release OrigExpColl
            Set aOrigExpColl = Nothing
            
            'Send all data to VME
        If nExactCount > 0 Then
            If nExactCount <> nCount Then ReDim Preserve arrVola(1 To nExactCount)
                'will process (expiry, vola) array for current surface
                m_Aux.Grp.Und.VolaSrv.SetExpiryVolaByExpiry arrVola, lSurfaceID
            If Err.Number = 0 Then
                bUpdateOptions = True
                m_bVolaChangedNow = True
                g_Params.FireInternalVolatilityChanged m_Aux.Grp.Und.Symbol
                m_bVolaChangedNow = False
            Else
                LogEvent EVENT_ERROR, m_Aux.Grp.Und.Symbol & ": Fail to flat vola. " & Err.Description
            End If
        Else
            bUpdateOptions = True
        End If
        
        Next
        
        'Update option quote vola, recalc data, resresh view port
        If bUpdateOptions Then
            If g_Params.UseTheoVolatility Then
                m_bInProc = True
                CalculateUnderlyingOptions True, , , True
                m_AuxOut.UnderlyingUpdateTotals
                UpdateTotals
                m_bInProc = False
            Else
                OptionsUpdateVola
            End If
            m_AuxOut.OptionsUpdate False, True, True
            m_AuxOut.VolaUpdateValues
        End If
        
        Screen.MousePointer = vbDefault
    End If
    
    Set aExpColl = Nothing
    Set aSurfaceColl = Nothing
    Erase arrVola
End Sub

'Returns new strike collection with options in current surface
Private Function GetStrikeCollBySurface(ByVal SurfaceID As Long, ByRef pStrikeColl As EtsMmQuotesLib.MmQvStrikeColl) As EtsMmQuotesLib.MmQvStrikeColl
On Error Resume Next
    Dim aStrike As EtsMmQuotesLib.MmQvStrikeAtom
    Dim aOptPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim aNewStrike As EtsMmQuotesLib.MmQvStrikeAtom
    Dim lSurfaceID As Long
    Dim aStrColl As EtsMmQuotesLib.MmQvStrikeColl
    Set aStrColl = New EtsMmQuotesLib.MmQvStrikeColl
    
    If (SurfaceID > 0) Then
        If (Not pStrikeColl Is Nothing) Then
            For Each aStrike In pStrikeColl
                For Each aOptPair In aStrike.OptPair
                    If (m_Aux.Grp.IsStockOrIndex) Then
                        lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aOptPair.RootID)
                    Else
                        If (Not m_Aux.Grp.Fut Is Nothing) Then
                            Set aFut = m_Aux.Grp.Fut
                            lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aFut.FutRootID)
                        End If
                    End If
                    If (lSurfaceID = SurfaceID) Then
                        If (aStrColl(aStrike.Strike) Is Nothing) Then
                            Set aNewStrike = aStrColl.Add(aStrike.Strike)
                            If (Not aNewStrike Is Nothing) Then
                                aNewStrike.OptPair.Add aOptPair.RootID, Str(aOptPair.RootID), aOptPair
                            End If
                        End If
                    End If
                Next
            Next
        End If
    End If
    
    Set GetStrikeCollBySurface = aStrColl
End Function

Private Function GetExpiryCollBySurface(ByVal SurfaceID As Long, ByRef pExpColl As EtsMmQuotesLib.MmQvExpColl) As EtsMmQuotesLib.MmQvExpColl
On Error Resume Next
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom
    Dim aStrike As EtsMmQuotesLib.MmQvStrikeAtom
    Dim aOptPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim lSurfaceID As Long
    
    Set aExpColl = New EtsMmQuotesLib.MmQvExpColl
    
    If (SurfaceID > 0) Then
        If (Not pExpColl Is Nothing) Then
            For Each aExp In pExpColl
                For Each aStrike In aExp.Strike
                    For Each aOptPair In aStrike.OptPair
                    
                        If (m_Aux.Grp.IsStockOrIndex) Then
                            lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aOptPair.RootID)
                        Else
                            If (Not m_Aux.Grp.Fut Is Nothing) Then
                                Set aFut = m_Aux.Grp.Fut
                                lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aFut.FutRootID)
                            End If
                        End If
                        
                        If (lSurfaceID = SurfaceID) Then
                            If (aExpColl(aExp.Expiry) Is Nothing) Then
                                aExpColl.Add aExp.Expiry, aExp
                            End If
                        End If
                        
                    Next
                Next
            Next
        End If
    End If
    
    Set GetExpiryCollBySurface = aExpColl
End Function

Private Function GetSurfaceCollByExpiry(ByRef pExp As EtsMmQuotesLib.MmQvExpAtom) As Collection
On Error Resume Next
    'Generate surface coll for current expiry
    Dim aSurfaceColl As Collection
    Set aSurfaceColl = New Collection
    Dim aStrike As EtsMmQuotesLib.MmQvStrikeAtom
    Dim aOptPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    
    Dim lSurfaceID As Long
    Dim vtSurfInCollID As Long
    For Each aStrike In pExp.Strike
        If (Not aStrike.OptPair Is Nothing) Then
            For Each aOptPair In aStrike.OptPair
                If (m_Aux.Grp.IsStockOrIndex) Then
                    lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aOptPair.RootID)
                Else
                    If (Not m_Aux.Grp.Fut Is Nothing) Then
                        Set aFut = m_Aux.Grp.Fut
                        lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aFut.FutRootID)
                    End If
                End If
                On Error Resume Next
                vtSurfInCollID = aSurfaceColl(Str(lSurfaceID))
                If (CLng(vtSurfInCollID) = 0) Then
                    aSurfaceColl.Add lSurfaceID, Str(lSurfaceID)
                End If
                vtSurfInCollID = 0
            Next
        End If
    Next
    
    Set GetSurfaceCollByExpiry = aSurfaceColl
End Function

Private Function GetSurfaceCollByExpiryColl(ByRef pExpColl As EtsMmQuotesLib.MmQvExpColl) As Collection
On Error Resume Next
    'Generate surface coll for current expiry
    Dim aSurfaceColl As Collection
    Set aSurfaceColl = New Collection
    Dim aStrike As EtsMmQuotesLib.MmQvStrikeAtom
    Dim aOptPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom
    
    Dim lSurfaceID As Long
    Dim vtSurfInCollID As Long
    
    If (Not pExpColl Is Nothing) Then
        For Each aExp In pExpColl
            For Each aStrike In aExp.Strike
                If (Not aStrike.OptPair Is Nothing) Then
                    For Each aOptPair In aStrike.OptPair
                        If (m_Aux.Grp.IsStockOrIndex) Then
                            lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aOptPair.RootID)
                        Else
                            If (Not m_Aux.Grp.Fut Is Nothing) Then
                                Set aFut = m_Aux.Grp.Fut
                                lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aFut.FutRootID)
                            End If
                        End If
                        On Error Resume Next
                        vtSurfInCollID = aSurfaceColl(Str(lSurfaceID))
                        If (CLng(vtSurfInCollID) = 0) Then
                            aSurfaceColl.Add lSurfaceID, Str(lSurfaceID)
                        End If
                        vtSurfInCollID = 0
                    Next
                End If
            Next
        Next
    End If
    
    Set GetSurfaceCollByExpiryColl = aSurfaceColl
End Function

Private Sub OptionsUpdateVola()
    On Error Resume Next
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom, aOpt As EtsMmQuotesLib.MmQvOptAtom
    Dim aPair As EtsMmQuotesLib.MmQvOptPairAtom, dVola#
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim lSurfaceID As Long

    If m_Aux.Grp.IsStockOrIndex Then
        Set aExpColl = m_Aux.Grp.Und.Exp
    Else
        Set aExpColl = m_Aux.Grp.Fut.Exp
    End If

    For Each aExp In aExpColl
        For Each aStr In aExp.Strike
            For Each aPair In aStr.OptPair
                lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(aPair.RootID)
                dVola = m_Aux.Grp.Und.VolaSrv.OptionVola(aExp.ExpiryOV, aStr.Strike, lSurfaceID)
                
                Set aOpt = aPair.Opt(enOtCall)
                If aOpt.ID <> 0 Then aOpt.Vola = dVola
                
                Set aOpt = aPair.Opt(enOtPut)
                If aOpt.ID <> 0 Then aOpt.Vola = dVola
            Next
        Next
    Next
    
    Set aExpColl = Nothing
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Select Case m_enMenuGrid
        Case GT_QUOTES_UNDERLYING
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying popup menu. ""RightClick -> Grid Layout..."" selected. " & GetStockInfo, m_frmOwner.GetCaption
            CustomizeUnderlyingGridLayout

        Case GT_QUOTES_OPTIONS
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Grid Layout..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
            CustomizeOptionGridLayout
        
        Case GT_QUOTES_VOLA
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Vola popup menu. ""RightClick -> Grid Layout..."" selected. " & GetStockInfo, m_frmOwner.GetCaption
            CustomizeVolaGridLayout
        
        Case GT_QUOTES_DIVIDENDS
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Dividend popup menu. ""RightClick -> Grid Layout..."" selected. " & GetStockInfo, m_frmOwner.GetCaption
            CustomizeDivGridLayout
        
    End Select
End Sub

Private Sub mnuCtxHideCol_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    Dim i&, nColIdx&, bMove As Boolean
    
    Select Case m_enMenuGrid
        Case GT_QUOTES_UNDERLYING
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying popup menu. ""RightClick -> Hide Column"" selected. " & GetStockInfo, m_frmOwner.GetCaption
                
            bMove = False
            
            nColIdx = fgUnd.ColKey(m_nMenuGridCol)
            For i = 1 To QUC_LAST_COLUMN ' skip QUC_SYMBOL
                If m_gdUnd.Idx(i) = nColIdx Then
                    m_gdUnd.Idx(i) = QUC_NONE
                    m_gdUnd.Col(nColIdx).Visible = False
                    bMove = True
                End If
                
                If bMove Then
                    If i + 1 <= QUC_LAST_COLUMN Then
                        m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
                    Else
                        m_gdUnd.Idx(i) = QUC_NONE
                    End If
                End If
            Next
            
            If bMove Then
                m_Aux.FormatUndGrid
                m_Aux.FormatUndColumns
                m_AuxOut.UnderlyingUpdate True, True
                m_AuxOut.UnderlyingUpdateTotals
            End If

        Case GT_QUOTES_OPTIONS
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Hide Column"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                
            bMove = False
            
            If m_gdOpt.Idx(1) = QOC_NONE Then Exit Sub
            
            nColIdx = fgOpt.ColKey(m_nMenuGridCol)
            For i = 0 To QOC_LAST_COLUMN
                If m_gdOpt.Idx(i) = nColIdx Then
                    m_gdOpt.Idx(i) = QOC_NONE
                    m_gdOpt.Col(nColIdx).Visible = False
                    bMove = True
                End If
                
                If bMove Then
                    If i + 1 <= QOC_LAST_COLUMN Then
                        m_gdOpt.Idx(i) = m_gdOpt.Idx(i + 1)
                    Else
                        m_gdOpt.Idx(i) = QOC_NONE
                    End If
                End If
            Next
            
            If bMove Then
                m_Aux.FormatOptGrid
                m_Aux.FormatOptColumns
                m_AuxOut.OptionsUpdate True, True, True
            End If
            
    End Select
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByVal bForceStockNetDelta As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, _
                                        ByRef aOpt As EtsGeneralLib.EtsOptAtom, _
                                        ByRef bBuy As Boolean, ByRef dPrice#, ByRef nQty&)
    On Error Resume Next
    Dim aRowData As MmQvRowData, nColIdx&
    Dim aCurOpt As EtsMmQuotesLib.MmQvOptAtom, aCurUnd As EtsMmQuotesLib.MmQvUndAtom
    
    bBuy = True
    dPrice = 0#
    nQty = IIf(bIsStock, 100, 10)
    
    Select Case True
        Case bIsStock And bForceStockNetDelta _
            Or m_enMenuGrid = GT_QUOTES_DIVIDENDS Or m_enMenuGrid = GT_QUOTES_VOLA _
            Or bIsStock And m_enMenuGrid = GT_QUOTES_OPTIONS
            
            Set aUnd = g_Underlying(m_Aux.Grp.Und.ID)
            If Not aUnd Is Nothing And bIsStock And (m_Aux.Grp.IsStockOrIndex) Then
                nQty = Round(m_Aux.Grp.Und.NetDelta / 100) * 100
                If m_Aux.Grp.Und.NetDelta >= 0 Then
                    bBuy = False
                    dPrice = m_Aux.Grp.Und.Quote(0).PriceBid
                    If nQty = 0 Then nQty = 100
                    
                Else
                    nQty = Abs(nQty)
                    bBuy = True
                    dPrice = m_Aux.Grp.Und.Quote(0).PriceAsk
                    If nQty = 0 Then nQty = 100
                End If
                
                If dPrice <= 0# Then dPrice = m_Aux.Grp.Und.Quote(0).PriceLast
            End If
            
        Case m_enMenuGrid = GT_QUOTES_UNDERLYING
            If m_Aux.Grp.IsStockOrIndex Then
                nColIdx = fgUnd.ColKey(m_nMenuGridCol)
                Set aCurUnd = fgUnd.RowData(m_nMenuGridRow).Und
                If Not aCurUnd Is Nothing Then
                    Set aUnd = g_Underlying(aCurUnd.ID)
                    
                    If Not aUnd Is Nothing And bIsStock Then
                        Select Case nColIdx
                            Case QUC_BID
                                bBuy = False
                                dPrice = aCurUnd.Quote(0).PriceBid
                        
                            Case QUC_ASK
                                bBuy = True
                                dPrice = aCurUnd.Quote(0).PriceAsk
                                
                            Case Else
                                nQty = Round(aCurUnd.NetDelta / 100) * 100
                                If aCurUnd.NetDelta >= 0 Then
                                    bBuy = False
                                    dPrice = aCurUnd.Quote(0).PriceBid
                                    If nQty = 0 Then nQty = 100
                                    
                                Else
                                    nQty = Abs(nQty)
                                    bBuy = True
                                    dPrice = aCurUnd.Quote(0).PriceAsk
                                    If nQty = 0 Then nQty = 100
                                End If
                        End Select
                        
                        If dPrice <= 0# Then dPrice = aCurUnd.Quote(0).PriceLast
                    End If
                    Set aCurUnd = Nothing
                End If
            End If
            
            If aUnd Is Nothing Then Set aUnd = g_Underlying(m_Aux.Grp.Und.ID)
        
        Case m_enMenuGrid = GT_QUOTES_OPTIONS
            Set aUnd = g_Underlying(m_Aux.Grp.Und.ID)
            If Not aUnd Is Nothing Then
                If Not bIsStock And (m_Aux.Grp.IsStockOrIndex) Then
                    nColIdx = fgOpt.ColKey(m_nMenuGridCol)
                    Set aCurOpt = QV.OptsRowData(m_nMenuGridRow).Opt(IIf(nColIdx < QOC_P_SYMBOL, enOtCall, enOtPut))
                    If Not aCurOpt Is Nothing Then
                        
                        Set aOpt = New EtsGeneralLib.EtsOptAtom
                        aOpt.ID = aCurOpt.ID
                        aOpt.OptType = aCurOpt.OptType
                        aOpt.RootID = aCurOpt.RootID
                        aOpt.Symbol = aCurOpt.Symbol
                        aOpt.Expiry = aCurOpt.Expiry
                        aOpt.ExpiryOV = aCurOpt.ExpiryOV
                        aOpt.TradingClose = aCurOpt.TradingClose
                        aOpt.Strike = aCurOpt.Strike
                        
                        If nColIdx = QOC_C_ASK Or nColIdx = QOC_P_ASK Then
                            bBuy = True
                            dPrice = aCurOpt.Quote(0).PriceAsk
                        Else
                            bBuy = False
                            dPrice = aCurOpt.Quote(0).PriceBid
                        End If
                        
                        If dPrice <= 0# Then dPrice = aCurOpt.Quote(0).PriceLast
                        
                        Set aCurOpt = Nothing
                    End If
                End If
            End If
        
    End Select
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean, ByVal bForceStockNetDelta As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, nQty&, aOpt As EtsGeneralLib.EtsOptAtom
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection bIsStock, bForceStockNetDelta, aUnd, aOpt, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, bBuy, nQty, dPrice
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, aOpt, bBuy, nQty, dPrice
'    End If
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Quotes View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
End Sub

Private Sub mnuCtxOrderNewOption_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, IIf(m_bIsDblClick, "Ctrl + Alt + DblClick pressed (new option order). ", "Popup menu. ""RightClick -> New Option Order..."" selected. ") & GetOptionInfo, m_frmOwner.GetCaption
    OrderNew False, False
End Sub

Private Sub mnuCtxOrderNewStock_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, IIf(m_bIsDblClick, "Alt + DblClick pressed (new stock order). ", "Popup menu. ""RightClick -> New Stock Order..."" selected. ") & GetOptionInfo, m_frmOwner.GetCaption
    OrderNew True, False
End Sub

Public Sub NewStockOrderAtNetDelta()
    On Error Resume Next
    OrderNew True, True
End Sub

Private Sub mnuCtxTntCardNew_Click()
'    On Error Resume Next
'    If m_bShutDown Then Exit Sub
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, nQty&, aOpt As EtsGeneralLib.EtsOptAtom
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection False, False, aUnd, aOpt, bBuy, dPrice, nQty
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
'    gCmn.ErrorMsgBox m_frmOwner, "Quotes View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bInProc Or m_nMenuGridCol < 0 Or m_nMenuGridRow < 1 Or m_Aux.Grp.ID = 0 Or Not m_Aux.Grp.Und.IsTraderContract Then Exit Sub
    Dim aRowData As MmQvRowData, nColIdx&, bBuy As Boolean, aOpt As EtsMmQuotesLib.MmQvOptAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "mnuCtxTradeNew_Click Enter.", m_frmOwner.GetCaption
        
    If m_bIsReload Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "mnuCtxTradeNew_Click Exit. Can't execute now.", m_frmOwner.GetCaption
        Exit Sub
    End If
    
    Select Case m_enMenuGrid
        Case GT_QUOTES_UNDERLYING
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, IIf(m_bIsDblClick, "DblClick on Underlying Grid (new trade). ", "Underlying popup menu. ""RightClick -> New Trade..."" selected. ") & GetStockInfo, m_frmOwner.GetCaption
        
            Set aRowData = fgUnd.RowData(m_nMenuGridRow)
                 nColIdx = fgUnd.ColKey(m_nMenuGridCol)

            If Not aRowData Is Nothing Then
                Set aUnd = aRowData.Und
                
                If Not aUnd Is Nothing Then
                    If m_Aux.Grp.IsStockOrIndex Then
                        On Error GoTo EH
                        
                        If nColIdx = QUC_BID Then
                             bBuy = (g_Params.QuoteBidColBuy <> 0)
                        ElseIf nColIdx = QUC_ASK Then
                            bBuy = (g_Params.QuoteBidColBuy = 0)
                        Else
                            bBuy = (g_Params.QuoteOtherColBuy <> 0)
                        End If
                        If aUnd.ID <> USD_ID Then frmTradeEditor.NewTrade aUnd.ID, aUnd.ID, bBuy, m_frmOwner
                    Else
                        Set aFut = aRowData.Fut
                        If Not aFut Is Nothing Then
                            On Error GoTo EH
                            
                            frmTradeEditor.NewTrade aFut.ID, aUnd.ID, g_Params.QuoteOtherColBuy, m_frmOwner
                            Set aFut = Nothing
                        End If
                    End If
                    Set aUnd = Nothing
                End If
                Set aRowData = Nothing
            End If
        
        Case GT_QUOTES_OPTIONS
            Set aRowData = QV.OptsRowData(m_nMenuGridRow)
            nColIdx = fgOpt.ColKey(m_nMenuGridCol)
            If Not aRowData Is Nothing Then
                Set aOpt = aRowData.Opt(IIf(nColIdx < QOC_P_SYMBOL, enOtCall, enOtPut))
                If Not aOpt Is Nothing Then
                
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogUserAction, IIf(m_bIsDblClick, "DblClick on Option Grid (new trade). ", "Option popup menu. ""RightClick -> New Trade..."" selected. ") & GetStockInfo & "Option symbol = """ & aOpt.Symbol & """.", m_frmOwner.GetCaption
                    
                    If nColIdx = QOC_C_BID Or nColIdx = QOC_P_BID Then
                        bBuy = (g_Params.QuoteBidColBuy <> 0)
                    ElseIf nColIdx = QOC_C_ASK Or nColIdx = QOC_P_ASK Then
                        bBuy = (g_Params.QuoteBidColBuy = 0)
                    Else
                        bBuy = (g_Params.QuoteOtherColBuy <> 0)
                    End If
                    
                    On Error GoTo EH
                    frmTradeEditor.NewTrade aOpt.ID, m_Aux.Grp.Und.ID, bBuy, m_frmOwner
                    
                    Set aOpt = Nothing
                End If
                Set aRowData = Nothing
            End If
        
    End Select
    
    ResetMenuData
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "mnuCtxTradeNew_Click Exit.", m_frmOwner.GetCaption
    
    Exit Sub
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    ResetMenuData
End Sub

Private Sub mnuCtxPositionTransfer_Click()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_bInProc Or m_nMenuGridCol < 0 Or m_nMenuGridRow < 1 Or m_Aux.Grp.ID = 0 Or Not m_Aux.Grp.Und.IsTraderContract Then Exit Sub
    Dim aRowData As MmQvRowData, nColIdx&, aOpt As EtsMmQuotesLib.MmQvOptAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim sContractDesc$, aFut As EtsMmQuotesLib.MmQvFutAtom
    
    Select Case m_enMenuGrid
        Case GT_QUOTES_UNDERLYING
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Underlying popup menu. ""RightClick -> Position Transfer..."" selected. " & GetStockInfo, m_frmOwner.GetCaption
                
            Set aRowData = fgUnd.RowData(m_nMenuGridRow)
            If Not aRowData Is Nothing Then
                Set aUnd = aRowData.Und
                If Not aUnd Is Nothing Then
                    If m_Aux.Grp.IsStockOrIndex Then
                        If aUnd.ID <> USD_ID Then
                            On Error GoTo EH
                            
                            sContractDesc = aUnd.Symbol & " (" & aUnd.SymbolName & ")"
                            
                            frmPositionTransfer.TransferPosition aUnd.ID, aUnd.ID, 0, sContractDesc, m_frmOwner
                        End If
                    Else
                        Set aFut = aRowData.Fut
                        If Not aFut Is Nothing Then
                            On Error GoTo EH
                            
                            sContractDesc = aFut.Symbol & " (" & aFut.ContractName & ")"
                            
                            frmPositionTransfer.TransferPosition aUnd.ID, aFut.ID, 0, sContractDesc, m_frmOwner
                            Set aFut = Nothing
                        End If
                    End If
                    Set aUnd = Nothing
                End If
                Set aRowData = Nothing
            End If
        
        Case GT_QUOTES_OPTIONS
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Position Transfer..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Position Transfer..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Position Transfer..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option popup menu. ""RightClick -> Position Transfer..."" selected. " & GetOptionInfo, m_frmOwner.GetCaption
                
            Set aRowData = QV.OptsRowData(m_nMenuGridRow)
            nColIdx = fgOpt.ColKey(m_nMenuGridCol)
            If Not aRowData Is Nothing Then
                Set aOpt = aRowData.Opt(IIf(nColIdx < QOC_P_SYMBOL, enOtCall, enOtPut))
                If Not aOpt Is Nothing Then
                    If m_Aux.Grp.IsStockOrIndex Then
                        sContractDesc = aOpt.Symbol & " (" & m_Aux.Grp.Und.Symbol & " " & Format$(aOpt.Expiry, "MMMM YYYY") & " " & _
                                    CStr(aOpt.Strike) & IIf(aOpt.OptType = enOtCall, " Call)", " Put)")
                    
                        On Error GoTo EH
                        frmPositionTransfer.TransferPosition m_Aux.Grp.Und.ID, aOpt.ID, 0, sContractDesc, m_frmOwner
                    Else
                        Set aFut = aRowData.Fut
                        If Not aFut Is Nothing Then
                            sContractDesc = aOpt.Symbol & " (" & m_Aux.Grp.Fut.Symbol & " " & Format$(aOpt.Expiry, "MMMM YYYY") & " " & _
                                        CStr(aOpt.Strike) & IIf(aOpt.OptType = enOtCall, " Call)", " Put)")
                            
                            On Error GoTo EH
                            frmPositionTransfer.TransferPosition m_Aux.Grp.Und.ID, aOpt.ID, 0, sContractDesc, m_frmOwner
                            Set aFut = Nothing
                        End If
                    End If
                    
                    Set aOpt = Nothing
                End If
                Set aRowData = Nothing
            End If
        
    End Select
    
    ResetMenuData
    Exit Sub
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to transfer position."
    ResetMenuData
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    If m_bShutDown Then Exit Sub
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_QUOTES_UNDERLYING
'            frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Symbol & " Quotes", "Underlying Quotes", fgUnd
'
'        Case GT_QUOTES_DIVIDENDS
'            frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Symbol & " Calculation Params", "Underlying Calculation Params", fgDiv
'
'        Case GT_QUOTES_VOLA
'            frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Symbol & " ATM Volas", "ATM Volas", fgVol
'
'        Case GT_QUOTES_OPTIONS
'            frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Symbol & " Options Quotes", "Options Quotes", fgOpt
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRealTime_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Popup menu. ""RightClick -> Real Time Mode"" selected. " & GetOptionInfo, m_frmOwner.GetCaption
    ToggleRealtime
End Sub

Public Sub ToggleRealtime()
    On Error Resume Next
    If m_bShutDown Or m_bSubscribingNow Or m_bDataLoad Or m_bLastQuoteReqNow Or m_Aux.Grp.ID <= 0 Or m_bInitializing Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, IIf(Not m_Aux.RealTime, "Start", "Stop") & " Real Time Mode.", m_frmOwner.GetCaption
    
    If Not m_Aux.RealTime And Not m_bInProc Then
        StartRealTime
    Else
        StopRealTime
    End If
    
    UpdateTotals
End Sub

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

Private Sub PriceProvider_OnUnsubscribed()
    If Not m_bShutDown Then DoEvents
End Sub

Private Sub QV_Progress(ByVal lProgress As Long, ByVal lMaxProgress As Long)
        pbProgress.Visible = True
        pbProgress.Min = 0
        pbProgress.Max = lMaxProgress
        pbProgress.Value = lProgress
End Sub

Private Sub tmrRealTime_Timer()
    On Error Resume Next
    tmrRealTime.Enabled = False
    
    If m_bIsReload Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "tmrRealTime_Timer: Quotes are reloaded now.", m_frmOwner.GetCaption
        Exit Sub
    End If
    
    If Not m_bShutDown And m_Aux.RealTime Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "tmrRealTime_Timer: Start RealTimeQuotesUpdate", m_frmOwner.GetCaption
        RealTimeQuotesUpdate
    Else
        If Not m_bShutDown Then
            QV.LoadQuoteUpdates
            fgOpt.Refresh
            m_AuxOut.UnderlyingUpdate False, True
        End If
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "tmrRealTime_Timer: Quotes are not in realtime mode", m_frmOwner.GetCaption
    
    End If
    
    If m_Aux.bFlatVolaToATM Then
        m_Aux.bFlatVolaToATM = False
        mnuCtxVolaFlatAll_Click
    End If
    
End Sub

Private Sub tmrShow_Timer()
    Dim nOperation&
    Dim bInRefresh As Boolean

    On Error Resume Next
    bInRefresh = m_bInRefreshMode
    'm_bInRefreshMode = False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quotes Refresh Started.", m_frmOwner.GetCaption

    tmrShow.Enabled = False
    If m_bShutDown Or m_bIsReload Then Exit Sub
    
    m_bIsReload = True

    If Not g_PerformanceLog Is Nothing Then _
        nOperation = g_PerformanceLog.BeginLogMmOperation

    'Screen.MousePointer = vbArrow

    If m_Aux.Grp.ID <> 0 And Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then
            m_bLastQuoteReqNow = False
            PriceProvider.CancelLastQuote
            If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
        End If

        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        m_Aux.Grp.Und.StopCalc
        m_bInProc = True
        If m_bInRealTimeCalc Then
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Start stoping calculations.", m_frmOwner.GetCaption
        End If

        Dim i As Long
        While m_bInRealTimeCalc And i < 5
            MMSleep 10
            i = i + 1
        Wend

        If m_bInRealTimeCalc Then
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "FAIL: Can't stop calculations.", m_frmOwner.GetCaption
        End If
        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

        If m_Aux.RealTime Or m_bSubscribingNow Then
            m_bSubscribingNow = False
            m_Aux.RealTime = False
            m_Aux.RealTimeConnected = False
            PriceProvider.UnSubscribeQuote
            If g_RTQuantity > 0 Then
                g_RTQuantity = g_RTQuantity - 1
            End If

            If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes
        End If

        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
    End If

    ClearViewAndData
    
    m_bInProc = False

    If m_nNewGrpID <> 0 Then
        pbProgress.Value = 0
        pbProgress.Visible = True

        lblStatus.Visible = False
        lblProcess.Visible = True
        lblProcess.Caption = "Data load..."
        lblProcess.Refresh
        imgStop.Visible = True
        imgStopDis.Visible = False
        Dim sttime As Long

        m_bInitializing = True
        If InitGroup(m_nNewGrpID) Then

            ShowGroup

            RaiseEvent OnExpiriesChange(False)
            RaiseEvent OnOptRootsChange(False)
            RaiseEvent OnOptExchangesChange(False)
            RaiseEvent OnUndExchangesChange(False)
            RaiseEvent OnStrikesChange

            fgUnd.Row = m_nUndMainRow
            fgUnd.Col = QUC_SYMBOL
            fgUnd.SetFocus

            UpdateTotals
            If g_Params.QuoteDirectlyToRealtime And Not bInRefresh Then
                sttime = GetTickCount
                If Not bInRefresh Then
                    m_bFiersTealtimeCalculation = True
                Else
                    m_bFiersTealtimeCalculation = False
                End If
                StartRealTime
                sttime = GetTickCount - sttime
            Else
                m_bFiersTealtimeCalculation = False
                RequestLastQuotes
            End If
        Else
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            lblProcess.Caption = ""
            imgStop.Visible = False
            imgStopDis.Visible = False

            UpdateTotals
            ClearViewAndData

            fgUnd.Row = m_nUndMainRow
            fgUnd.Col = QUC_SYMBOL
            fgUnd.SetFocus
            fgUnd.TextMatrix(m_nUndMainRow, QUC_SYMBOL) = ""

            AdjustCaption
        End If
        m_nNewGrpID = 0
        m_bInitializing = False
    End If

    m_bInRefreshMode = False
    m_bIsReload = False

    Screen.MousePointer = vbDefault
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quotes Refresh Finished.", m_frmOwner.GetCaption

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation nOperation, OPER_LOADDATA, m_frmOwner.GetCaption, QV.UndQuotesCount, QV.OptQuotesCount, QV.FutQuotesCount
End Sub

Public Sub RefreshOptsGrid(Optional ByVal bShowSelectedRow As Boolean = False)
    On Error Resume Next
    If Not m_bInRealTimeCalc Then
           m_bInRealTimeCalc = True
           CalculateUnderlyingOptions True
           m_bInRealTimeCalc = False
    End If

    fgOpt.FlexDataSource = Nothing
    QV.OptsColumnsOrder = m_Aux.gdOpt.IdxCopy
    
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Start QV.OptsRefresh False", m_frmOwner.GetCaption
    Err.Clear
    QV.OptsRefresh False
    If Err.Number <> 0 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "QV.OptsRefresh False Exit with Error: " & CStr(Err.Number) & " " & Err.Description, m_frmOwner.GetCaption
    End If
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "End QV.OptsRefresh False", m_frmOwner.GetCaption
    
    fgOpt.FlexDataSource = QV
    
    ' for fore color
    m_Aux.FormatOptColumns
    If g_Params.QuoteOptExpirySeparation Then m_AuxOut.OptionsUpdateBackColor True
    
    If bShowSelectedRow Then m_AuxOut.ShowSelectedRow

End Sub

Private Sub ClearViewAndData()
    On Error Resume Next
    Dim sSymbol$
    
    m_AuxOut.ExpiryColorSelection = False
    m_nUndMainRow = 1
    sSymbol = g_ContractAll(m_nNewGrpID).Symbol
    
    With fgUnd
        .Rows = 2
        .RowHidden(m_nUndMainRow) = False
        .RowData(m_nUndMainRow) = Empty
        .Row = m_nUndMainRow
        
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
        
        .Cell(flexcpText, 1, QUC_SYMBOL + 1, 1, .Cols - 1) = ""
        .TextMatrix(m_nUndMainRow, QUC_SYMBOL) = sSymbol
        
        m_Aux.FormatUndGrid
        m_Aux.FormatUndColumns
    
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
    
    With fgOpt
        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
        
        .Rows = 1
        fgOpt.FlexDataSource = Nothing
        'RefreshOptsGrid
        m_Aux.FormatOptGrid
        m_Aux.FormatOptColumns
        
        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
    End With
    
    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
        
        .Cols = QDC_LAST_COLUMN
        .TextMatrix(1, QDC_TRADES) = "0"
        .TextMatrix(1, QDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        .TextMatrix(1, QDC_STYLE) = "1"
        .TextMatrix(1, QDC_FREQ) = "0"
        .TextMatrix(1, QDC_DIV) = "0"
        .TextMatrix(1, QDC_FREQ) = "0"
        .TextMatrix(1, QDC_DATE) = ""
        .TextMatrix(1, QDC_AMT) = ""
        .TextMatrix(1, QDC_RATE) = "0"
        .TextMatrix(1, QDC_FLT_TRADER) = "0"
        .TextMatrix(1, QDC_FLT_STRATEGY) = "0"
        
        .ColHidden(QDC_DATE) = True
        .ColHidden(QDC_AMT) = True
    
        m_Aux.FormatDivGrid
        m_Aux.FormatDivColumns
        
        m_Aux.FormatVolGrid
        m_Aux.FormatVolColumns
        
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
    
    
    fgOpt.FlexDataSource = Nothing
    QV.CleanUp False
    
    AdjustCaption
    RaiseEvent OnExpiriesChange(False)
    RaiseEvent OnOptRootsChange(False)
    RaiseEvent OnOptExchangesChange(False)
    RaiseEvent OnUndExchangesChange(False)
    RaiseEvent OnStrikesChange
    AdjustState
    UserControl_Resize
End Sub

Private Sub RequestLastQuotes()
    On Error GoTo EH
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, sExch$, aReq As MmQvRequestAtom
    Dim aExch As EtsGeneralLib.ExchAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    
    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow
        
        If Not g_PerformanceLog Is Nothing Then _
            m_nOperation = g_PerformanceLog.BeginLogMmOperation

        QV.UndResponses = 0
        QV.OptResponses = 0
        QV.FutResponses = 0
        
        m_DecExpectations = 0
        
        If m_Aux.QuoteReqsAll.Count > 0 Then
            m_bInProc = True
            m_bLastQuoteReqNow = True
            AdjustState
            
            m_bGroupRequest = m_bGroupRequestSupported And (m_Aux.Grp.IsStockOrIndex) And Not g_Params.QuoteReqTypeAlwaysNonGroup
            
            m_nQuoteReqCount = m_Aux.QuoteReqsAll.Count
            m_nQuoteReqDone = 0&
            
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = m_nQuoteReqCount
            pbProgress.Visible = True
            
            lblStatus.Visible = False
            lblProcess.Visible = True
            lblProcess.Caption = "Last quotes request..."
            lblProcess.Refresh
            imgStop.Visible = True
            imgStopDis.Visible = False
            
            If Not m_bGroupRequest Then
                m_nQuoteGroupReqCount = 0&
                m_nQuoteGroupReqDone = 0&
                
                QV.RequestLastQuotes
                
'                aParam.Type = enMStart
'                PriceProvider.RequestLastQuote aParam
'
'                For Each aReq In m_Aux.QuoteReqsAll
'                    If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit For
'                    aParam = aReq.QuoteUpdateParam
'                    PriceProvider.RequestLastQuote aParam
'                    DoEvents
'                Next
'
'                aParam.Type = enMStop
'                PriceProvider.RequestLastQuote aParam
            Else
                aParam.Symbol = m_Aux.Grp.Und.Symbol
                aParam.Type = IIf(m_Aux.Grp.Und.UndType = enCtStock, enSTK, enIDX)
                
                m_nQuoteGroupReqCount = m_Aux.Grp.Und.UndExch.Count + 1
                m_nQuoteGroupReqDone = 0&
                
                For Each aExch In m_Aux.Grp.Und.UndExch
                    If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit For
                    aParam.Exchange = aExch.Code
                    PriceProvider.RequestLastQuote aParam
                    DoEvents
                Next
                
                If Not m_bShutDown And m_bLastQuoteReqNow And Not m_Aux.Grp.Und.SynthUnd Is Nothing Then
                    
                    For Each aUnd In m_Aux.Grp.Und.SynthUnd
                        If aUnd.ID <> USD_ID Then
                            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + aUnd.UndExch.Count
                            
                            aParam.Symbol = aUnd.Symbol
                            aParam.Type = IIf(aUnd.UndType = enCtStock, enSTK, enIDX)
                            
                            For Each aExch In aUnd.UndExch
                                If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit For
                                                                
                                aParam.Exchange = aExch.Code
                                PriceProvider.RequestLastQuote aParam
                                DoEvents
                            Next
                        End If
                    Next
                End If
                aParam.Symbol = m_Aux.Grp.Und.Symbol
                aParam.Type = IIf(m_Aux.Grp.Und.UndType = enCtStock, enSTK, enIDX)
                
                If Not m_bShutDown And m_bLastQuoteReqNow Then
                    sExch = ""
                    
                    For Each aExch In m_Aux.Grp.Und.OptExch
                        If aExch.ID <> 0 Then sExch = sExch & "," & aExch.Code
                    Next
                    
                    aParam.Exchange = sExch
                    aParam.Type = IIf(m_Aux.Grp.Und.UndType = enCtStock, enGrSTK, enGrIDX)
                    m_GroupPriceProvider.RequestLastGroupQuotes aParam
                End If
                
            End If
            
            m_bInProc = False
            AdjustState
        Else
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            lblProcess.Caption = ""
            imgStop.Visible = False
            imgStopDis.Visible = False
        End If
    Else
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
    On Error Resume Next
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    AdjustState
    
    PriceProvider.CancelLastQuote
    If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim sKey$, bFinished As Boolean, nTime&
    Dim nOpts&, nUnds&, nOperation&, nLogTime&, nFuts&
    
    If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "On Error: " & Description & " For " & Request.Symbol & " - " & Request.Exchange & " - " & Request.Type, m_frmOwner.GetCaption
    
    bFinished = False
    
    If ReqType = enSubscribeQuote Then
        If Not Request Is Nothing And m_bSubscribingNow Then
            If PpIsNonGroupReqType(Request.Type) Then
                sKey = Request.Symbol
                If Len(Request.Exchange) > 0 Then
                    sKey = sKey & "." & Request.Exchange
                End If
                
                If Not m_Aux.QuoteReqsAll(sKey) Is Nothing Then
                    LogEvent EVENT_WARNING, sKey & ": " & Description
                
                End If
                    m_nQuoteReqDone = m_nQuoteReqDone + 1
                    IncProgress pbProgress
                    
                    If m_bGroupRequest Then m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                    bFinished = (m_nQuoteReqDone >= m_nQuoteReqCount _
                                Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
                
            ElseIf PpIsGroupReqType(Request.Type) Then
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
                bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
            End If
        
            If bFinished Then
                m_bSubscribingNow = False
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
            
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                lblProcess.Caption = ""
                imgStopDis.Visible = False
                imgStop.Visible = False
                
                m_Aux.RealTime = True
                m_Aux.RealTimeConnected = True
                If m_BatchPriceProvider.IsQuotesUpdated Then tmrRealTime.Enabled = True
                
                UpdateTotals
                AdjustState
                AdjustCaption
                
                g_RTNumber = g_RTNumber + 1
                g_RTQuantity = g_RTQuantity + 1
                m_frmOwner.lRTNum = g_RTNumber
                If g_RTQuantity > g_MaxRTQuantity Then
                    frmMain.StopExtraRT
                End If
                
                If m_Aux.bFlatVolaToATM And tmrRealTime.Enabled = False Then
                    m_Aux.bFlatVolaToATM = False
                    mnuCtxVolaFlatAll_Click
                End If

            End If
        End If
    
    ElseIf ReqType = enRequestLastQuote Then
        If Not Request Is Nothing And m_bLastQuoteReqNow Then
            If PpIsNonGroupReqType(Request.Type) Then
                sKey = Request.Symbol
                
                If Len(Request.Exchange) > 0 Then
                    sKey = sKey & "." & Request.Exchange
                End If
                
                ' Future options symbology has not been changed for Globex's symbols yet,
                ' so that's no need to write about it as about an error... I suppose...
                If Request.Type <> enFOP Or Request.Exchange <> "G" Then

                    m_DecExpectations = m_DecExpectations + 1
                    
                    If Not m_Aux.QuoteReqsAll(sKey) Is Nothing Then
                        LogEvent EVENT_WARNING, sKey & ": " & Description
                    End If
                                                                Else
                    '    m_nQuoteReqCount = m_nQuoteReqCount - 1
                        
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
                m_bLastQuoteReqNow = False
                
                PriceProvider.CancelLastQuote
                If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
                
                pbProgress.Value = pbProgress.Max
                imgStop.Visible = False
                imgStopDis.Visible = True
                lblProcess.Caption = "Calculation..."
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
                
                nTime = timeGetTime
                If Not g_PerformanceLog Is Nothing Then m_nOperation = g_PerformanceLog.BeginLogMmOperation
                
                m_bInProc = True
                
                m_AuxOut.UnderlyingExchUpdate
                CalculateUnderlyingOptions True
                
                m_bInProc = False
                
                If Not g_PerformanceLog Is Nothing Then nLogTime = g_PerformanceLog.CheckLogMmOperation(m_nOperation)
                    
                m_nLastRecalcCycle = timeGetTime - nTime
                nTime = timeGetTime
                
                If Not g_PerformanceLog Is Nothing Then nOperation = g_PerformanceLog.BeginLogMmOperation
                        
                nUnds = m_AuxOut.UnderlyingUpdate(False, True)
                nOpts = m_AuxOut.OptionsUpdate(False, True, True)
                m_AuxOut.VolaUpdateValues
                
                AdjustState
                m_AuxOut.UnderlyingUpdateTotals
                UpdateTotals
                
                m_AuxOut.OptionsUpdateColors
                
                m_nLastOutCycle = timeGetTime - nTime
                If Not g_PerformanceLog Is Nothing Then
                    g_PerformanceLog.ContinueLogMmOperation m_nOperation, OPER_CALCULATION, m_frmOwner.GetCaption, nUnds, nOpts, nFuts, nLogTime
                    g_PerformanceLog.FinishLogMmOperation nOperation, OPER_REFRESHSCREEN, m_frmOwner.GetCaption, nUnds, nOpts, nFuts
                    g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_CALCREFRESHSCREEN, m_frmOwner.GetCaption, nUnds, nOpts, nFuts
                End If
                    
                If g_Params.IsDebug Then UpdateStat
                
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                lblProcess.Caption = ""
                imgStopDis.Visible = False
                
                If m_Aux.bFlatVolaToATM Then
                    m_Aux.bFlatVolaToATM = False
                    mnuCtxVolaFlatAll_Click
                End If
        
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
    If m_bShutDown Then Exit Sub
    If Not m_bLastQuoteReqNow Then Exit Sub
    
    Dim sKey$, aReq As MmQvRequestAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, bFinished As Boolean, nTime&
    Dim nOpts&, nUnds&, aOpt As EtsMmQuotesLib.MmQvOptAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim nOperation&, nLogTime&, nFuts&
    
    bFinished = False
    
    If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "On Last Quote: " & Params.Symbol & " - " & Params.Exchange & " - " & Params.Type, m_frmOwner.GetCaption
    
    If PpIsNonGroupReqType(Params.Type) Then
        QV.Quote(Params) = Results
        m_nQuoteReqDone = m_nQuoteReqDone + 1
        IncProgress pbProgress
        bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
                        Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
    ElseIf PpIsGroupReqType(Params.Type) Then
        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
        bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
    End If

    If bFinished Then
        m_bLastQuoteReqNow = False
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then m_GroupPriceProvider.CancelLastGroupQuotes
        
        pbProgress.Value = pbProgress.Max
        imgStop.Visible = False
        imgStopDis.Visible = True
        lblProcess.Caption = "Calculation..."
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_REQUESTQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
                
        nTime = timeGetTime
        If Not g_PerformanceLog Is Nothing Then m_nOperation = g_PerformanceLog.BeginLogMmOperation
        
        m_bInProc = True
        
        m_AuxOut.UnderlyingExchUpdate
        
        CalculateUnderlyingOptions True
        m_bInProc = False
        
        m_nLastRecalcCycle = timeGetTime - nTime
        
        If Not g_PerformanceLog Is Nothing Then nLogTime = g_PerformanceLog.CheckLogMmOperation(m_nOperation)
            
        nTime = timeGetTime
        
        If Not g_PerformanceLog Is Nothing Then nOperation = g_PerformanceLog.BeginLogMmOperation
        
        nUnds = m_AuxOut.UnderlyingUpdate(False, True)
        nOpts = m_AuxOut.OptionsUpdate(False, True, True)
        m_AuxOut.VolaUpdateValues
        
        AdjustState
        m_AuxOut.UnderlyingUpdateTotals
        UpdateTotals
        
        m_AuxOut.OptionsUpdateColors
        
        m_nLastOutCycle = timeGetTime - nTime
        
        If Not g_PerformanceLog Is Nothing Then
            g_PerformanceLog.ContinueLogMmOperation m_nOperation, OPER_CALCULATION, m_frmOwner.GetCaption, nUnds, nOpts, nFuts, nLogTime
            g_PerformanceLog.FinishLogMmOperation nOperation, OPER_REFRESHSCREEN, m_frmOwner.GetCaption, nUnds, nOpts, nFuts
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_CALCREFRESHSCREEN, m_frmOwner.GetCaption, nUnds, nOpts, nFuts
        End If
                    
        If g_Params.IsDebug Then UpdateStat
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStopDis.Visible = False
        
        If m_Aux.bFlatVolaToATM Then
            m_Aux.bFlatVolaToATM = False
            mnuCtxVolaFlatAll_Click
        End If
        
    End If
End Sub

Private Sub PriceProvider_OnSubscribed(Params As PRICEPROVIDERSLib.QuoteUpdateParams)
 On Error Resume Next
    If m_bShutDown Or Not m_bSubscribingNow Then Exit Sub
    Dim bFinished As Boolean, sKey$
    Dim lAddition As Long
    
    
    
    If PpIsNonGroupReqType(Params.Type) Then
        lAddition = m_BatchPriceProvider.UnNotifiedSubscribtionQuantity(True) + 1
        IncProgressEx pbProgress, lAddition
        m_nQuoteReqDone = m_nQuoteReqDone + lAddition
        If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "On Subscribed: " & Params.Symbol & " - " & Params.Exchange & " - " & Params.Type & "[" & CStr(lAddition) & "]", m_frmOwner.GetCaption
            
        bFinished = (m_nQuoteReqDone >= m_nQuoteReqCount _
                    Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
            
    ElseIf PpIsGroupReqType(Params.Type) Then
        If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "On Subscribed: " & Params.Symbol & " - " & Params.Exchange & " - " & Params.Type, m_frmOwner.GetCaption
        
        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
        bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
    End If

    If bFinished Then
        m_bSubscribingNow = False
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
        m_BatchPriceProvider.UnNotifiedSubscribtionQuantity True

        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
        
        m_Aux.RealTime = True
        m_Aux.RealTimeConnected = True
        If m_BatchPriceProvider.IsQuotesUpdated Then tmrRealTime.Enabled = True
        
        UpdateTotals
        AdjustState
        AdjustCaption
        
        g_RTNumber = g_RTNumber + 1
        g_RTQuantity = g_RTQuantity + 1
        m_frmOwner.lRTNum = g_RTNumber
        If g_RTQuantity > g_MaxRTQuantity Then
            frmMain.StopExtraRT
        End If
        
        If m_Aux.bFlatVolaToATM And tmrRealTime.Enabled = False Then
            m_Aux.bFlatVolaToATM = False
            mnuCtxVolaFlatAll_Click
        End If
        
    End If
End Sub

Private Sub PriceProvider_OnQuoteUpdate()
    On Error Resume Next
    
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
    
'    If m_bInRealTimeCalc Then
'           If Not g_PerformanceLog Is Nothing Then _
'                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Did not pass condition: m_bInRealTimeCalc", m_frmOwner.GetCaption
'
'            Exit Sub
'
'    End If
    
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

Private Sub RealTimeQuotesUpdate()
    On Error Resume Next
    If Not m_Aux.RealTime Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: not in Realtime mode", m_frmOwner.GetCaption
        Exit Sub
    End If
    If m_bInProc Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: InProc", m_frmOwner.GetCaption
        Exit Sub
    End If
    If m_bInRealTimeCalc Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: In Realtime calc", m_frmOwner.GetCaption
        Exit Sub
    End If
    If m_bSubscribingNow Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: Subscribing now", m_frmOwner.GetCaption
        Exit Sub
    End If

    If m_BatchPriceProvider Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RealTimeQuotesUpdate: Price provider is not set", m_frmOwner.GetCaption
        Exit Sub
    End If

    Dim nRow&, sKey$, aReq As MmQvRequestAtom, dBidPrice#, dAskPrice#, dLastPrice#
    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim enCalcIV As EtsMmQuotesLib.MmQvIvCalcEnum, nMask&, nTime&
    Dim Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo
    'Dim Infos() As PRICEPROVIDERSLib.QuoteUpdateFullInfo, nLBound&, nUBound&, i&
    Dim bOptUpd As Boolean, bUndUpd As Boolean
    Dim aRoot As EtsMmQuotesLib.MmQvOptRootAtom
    Dim aSynthRoots As New EtsMmQuotesLib.MmQvOptRootColl
    Dim aEntity As EtsGeneralLib.EtsMmEntityAtom
    Dim nOperation&: nOperation = 0
    Dim nOperation2&: nOperation2 = 0
    Dim nUnds&: nUnds = 0
    Dim nOpts&: nOpts = 0
    Dim nFuts&: nFuts = 0
    Dim nLogTime&
    Dim bVolaChangedExt As Boolean: bVolaChangedExt = False
    
    m_bInRealTimeCalc = True
    
    Dim dTimer As Long
    
    If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogEnhDebug, "Get Quotes Update", m_frmOwner.GetCaption
    
    dTimer = GetTickCount
    QV.LoadQuoteUpdates
    If m_bFiersTealtimeCalculation Then
        fgOpt.Refresh
        lblStatus.Caption = "Calculating..."
        lblStatus.Visible = True
        lblStatus.Refresh
    End If
    dTimer = GetTickCount - dTimer
    
    nTime = timeGetTime
    Dim bForceRecalc As Boolean
    
    bForceRecalc = m_bFiersTealtimeCalculation
    m_bFiersTealtimeCalculation = False
    
    If (ProcessRealTime And QV.IsMarketDataChanged) Or bForceRecalc Then
        
        If Not g_PerformanceLog Is Nothing Then nOperation = g_PerformanceLog.BeginLogMmOperation
                
        Dim bIsCanceled As Boolean
        CalculateUnderlyingOptions True, , , , bIsCanceled
        If bIsCanceled Then
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "Calculation canceled by user", m_frmOwner.GetCaption
            GoTo Ex
        End If
       bUndUpd = True
      
'       DoEvents
       If m_bShutDown Then Exit Sub
        
       If Not g_PerformanceLog Is Nothing Then nLogTime = g_PerformanceLog.CheckLogMmOperation(nOperation)
       m_bCalculationDateChanged = False
    End If
        
    m_nLastRecalcCycle = timeGetTime - nTime
                            
'   Update grid data
    nTime = timeGetTime
                        
    If (ProcessRealTime Or bForceRecalc) And Not m_bShutDown Then
        If Not g_PerformanceLog Is Nothing Then nOperation2 = g_PerformanceLog.BeginLogMmOperation
        
        nUnds = m_AuxOut.UnderlyingUpdate(False, True)
        nOpts = m_AuxOut.OptionsUpdate(False, True, bUndUpd Or bVolaChangedExt)
        
        If bUndUpd Or bVolaChangedExt Then m_AuxOut.VolaUpdateValues
        m_AuxOut.UnderlyingUpdateTotals
        UpdateTotals
    Else
        nUnds = m_AuxOut.UnderlyingUpdate(False, True)
    End If
    m_AuxOut.OptionsUpdateColors
    m_nLastOutCycle = timeGetTime - nTime
                        
    If Not g_PerformanceLog Is Nothing Then
        If nOperation > 0 Then _
            g_PerformanceLog.ContinueLogMmOperation nOperation, OPER_CALCULATION, m_frmOwner.GetCaption, nUnds, nOpts, nFuts, nLogTime
        If nOperation2 > 0 Then _
            g_PerformanceLog.FinishLogMmOperation nOperation2, OPER_REFRESHSCREEN, m_frmOwner.GetCaption, nUnds, nOpts, nFuts
        If nOperation > 0 Then _
            g_PerformanceLog.FinishLogMmOperation nOperation, OPER_CALCREFRESHSCREEN, m_frmOwner.GetCaption, nUnds, nOpts, nFuts
    End If
Ex:
    If g_Params.IsDebug Then UpdateStat
    m_bFiersTealtimeCalculation = False
    m_bInRealTimeCalc = False
End Sub

'comment this sub in production for prevent statistic output
Private Sub UpdateStat()
    On Error Resume Next
    Debug.Print Format$(m_nLastDataSize, "#0000") & _
                vbTab & Format$(m_nLastRecalcCycle, "#000000") & _
                vbTab & Format$(m_nLastOutCycle, "#000000") & _
                vbTab & Format$(m_nLastRecalcCycle + m_nLastOutCycle, "#000000")
End Sub

Private Sub UpdateTotals()
    On Error Resume Next
    lblStatus.Caption = m_AuxOut.GetTotalsString
    lblStatus.Refresh
    
End Sub

Private Sub CalculateUnderlyingOptions(ByVal bRecalcAll As Boolean, Optional aSynthRoots As EtsMmQuotesLib.MmQvOptRootColl = Nothing, Optional ManualEdit As Boolean = True, _
                                                                    Optional ByVal bForceRecalc As Boolean = False, _
                                                                    Optional ByRef bIsCanceled As Boolean)
    On Error Resume Next
    Dim nCallMask As Long, nPutMask As Long, enCallCalcIV As EtsMmQuotesLib.MmQvIvCalcEnum, enPutCalcIV As EtsMmQuotesLib.MmQvIvCalcEnum
    Dim nCalcSleepFreq&: nCalcSleepFreq = 0&
    Dim nCalcSleepAmt&: nCalcSleepAmt = 0&
    Dim dDayShift As Double
    Dim dtToday As Date
    Dim dtNow As Date
    Dim sDate As String
    Dim dtCalcDate As Date
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Calculation Underlying Options Enter.", m_frmOwner.GetCaption

    If bRecalcAll Then
        nCallMask = GetGreeksMask(enOtCall)
        nPutMask = GetGreeksMask(enOtPut)
        enCallCalcIV = GetIvMask(enOtCall)
        enPutCalcIV = GetIvMask(enOtPut)
    Else
        nCallMask = GM_NONE
        nPutMask = GM_NONE
        enCallCalcIV = enMmQvCalcIvNone
        enPutCalcIV = enMmQvCalcIvNone
    End If
    
    If g_Params.UseCalcSleep And g_Params.CalcSleepAmt <> 0 Then
        nCalcSleepFreq = g_Params.CalcSleepFreq
        nCalcSleepAmt = g_Params.CalcSleepAmt
    End If
    
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
    If m_Aux.Grp.IsStockOrIndex Then
         Set aExpColl = m_Aux.Grp.Und.Exp
    Else
         Set aExpColl = m_Aux.Grp.Fut.Exp
     End If
    
    dtCalcDate = GetNewYorkTime
    If ClipTime(dtCalculationDate.Value) > ClipTime(Now) Then
        dDayShift = IIf(m_Aux.RealTime, 0, ClipTime(dtCalculationDate.Value) - ClipTime(Now))
    Else
        dDayShift = 0
    End If
    dtCalcDate = dtCalcDate + dDayShift
    
    If m_Aux.Grp.IsStockOrIndex Then
        m_Aux.Grp.Und.CalcAllOptions nCallMask, nPutMask, enCallCalcIV, enPutCalcIV, g_Params.CalcModel, _
                            g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
                            bRecalcAll, aSynthRoots, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
                            m_Aux.Grp.UseCustRates, nCalcSleepFreq, nCalcSleepAmt, dtCalcDate, ManualEdit, bForceRecalc, _
                            g_Main.CalculationParametrs
    Else
        m_Aux.Grp.Fut.CalcAllOptions m_Aux.Grp.Und, nCallMask, nPutMask, enCallCalcIV, enPutCalcIV, g_Params.CalcModel, _
                            g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
                            bRecalcAll, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
                            m_Aux.Grp.UseCustRates, nCalcSleepFreq, nCalcSleepAmt, dtCalcDate, ManualEdit, _
                            g_Main.CalculationParametrs
                            
    End If
    
    bIsCanceled = CBool(Err.Number = ERROR_CANCELLED)
    
    If Err.Number <> 0 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Calculation Underlying Options Exit with Error: " & CStr(Err.Number) & " " & Err.Description, m_frmOwner.GetCaption
    Else
        fgOpt.Refresh
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Calculation Underlying Options Exit.", m_frmOwner.GetCaption
    End If
                            
End Sub

Private Sub CalcOptionGreeks(aOpt As EtsMmQuotesLib.MmQvOptAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, _
                            aExp As EtsMmQuotesLib.MmQvExpAtom, ByVal nMask As Long, ByVal enCalcIV As EtsMmQuotesLib.MmQvIvCalcEnum, Optional ManualEdit As Boolean = True, _
                            Optional ByVal bRefresh As Boolean = True)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Calculation Greeks Enter.", m_frmOwner.GetCaption
    
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim dtCalcDate As Date
    Dim dDayShift As Double
    Dim sDate As String
    
    dtCalcDate = GetNewYorkTime
    If m_Aux.Grp.IsStockOrIndex Then
         Set aExpColl = m_Aux.Grp.Und.Exp
    Else
         Set aExpColl = m_Aux.Grp.Fut.Exp
    End If
    
    sDate = fgVol.TextMatrix(1, QVC_VOLA_VAL + aExpColl.Count)
    dDayShift = 0

    dDayShift = IIf(m_Aux.RealTime, 0, ClipTime(CDate(sDate)) - Date)
    dtCalcDate = dtCalcDate + dDayShift
    
           
    If m_Aux.Grp.IsStockOrIndex Then
        m_Aux.Grp.Und.CalcOptionGreeks aOpt, aQuote, aExp, m_Aux.Grp.Und.OptRoot(aOpt.RootID), nMask, enCalcIV, g_Params.CalcModel, _
                                g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
                                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, m_Aux.Grp.UseCustRates, dtCalcDate, ManualEdit, _
                                g_Main.CalculationParametrs
    Else
        m_Aux.Grp.Fut.CalcOptionGreeks m_Aux.Grp.Und, aOpt, aQuote, aExp, nMask, enCalcIV, g_Params.CalcModel, _
                                g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
                                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, m_Aux.Grp.UseCustRates, dtCalcDate, ManualEdit, _
                                g_Main.CalculationParametrs
    End If
    
    If bRefresh Then fgOpt.Refresh
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Calculation Greeks Exit.", m_frmOwner.GetCaption
                            
    Debug.Assert Err.Number = 0
End Sub

Private Function GetGreeksMask(ByVal nOptType As Long) As Long
    On Error Resume Next
    Dim nShift&
    
    If nOptType = enOtPut Then
        nShift = QOC_P_SYMBOL - 2
    Else
        nShift = 0
    End If
    
    If m_gdOpt.Col(nShift + QOC_C_THEO_PRICE).Visible Then GetGreeksMask = GetGreeksMask Or GM_THEOPRICE
    If m_gdOpt.Col(nShift + QOC_C_DELTA).Visible Then GetGreeksMask = GetGreeksMask Or GM_DELTA
    If m_gdOpt.Col(nShift + QOC_C_GAMMA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA
    If m_gdOpt.Col(nShift + QOC_C_VEGA).Visible Then GetGreeksMask = GetGreeksMask Or GM_VEGA
    If m_gdOpt.Col(nShift + QOC_C_THETA).Visible Then GetGreeksMask = GetGreeksMask Or GM_THETA
    If m_gdOpt.Col(nShift + QOC_C_RHO).Visible Then GetGreeksMask = GetGreeksMask Or GM_RHO
    If m_gdOpt.Col(nShift + QOC_C_VEGA_DELTA).Visible Then GetGreeksMask = GetGreeksMask Or GM_DELTA_VEGA
    If m_gdOpt.Col(nShift + QOC_C_VEGA_GAMMA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA_VEGA
    If m_gdOpt.Col(nShift + QOC_C_THETA_DELTA).Visible Then GetGreeksMask = GetGreeksMask Or GM_DELTA_THETA
    If m_gdOpt.Col(nShift + QOC_C_THETA_GAMMA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA_THETA
    If m_gdOpt.Col(nShift + QOC_C_GAMMA_PER_THETA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA Or GM_THETA
    
    If m_gdOpt.Col(nShift + QOC_C_VOLGA).Visible Then GetGreeksMask = GetGreeksMask Or GM_VOLGA
End Function

Private Function GetIvMask(ByVal nOptType As Long) As EtsMmQuotesLib.MmQvIvCalcEnum
    On Error Resume Next
    Dim nShift&
    GetIvMask = enMmQvCalcIvNone
    
    If nOptType = enOtPut Then
        nShift = QOC_P_SYMBOL - 2
    Else
        nShift = 0
    End If
    
    If g_Params.UseTheoVolatility Then
        If m_gdOpt.Col(nShift + QOC_C_IV).Visible Then GetIvMask = GetIvMask Or enMmQvCalcIv
    Else
        GetIvMask = GetIvMask Or enMmQvCalcIv
    End If
    
    If Not m_gdOpt.Col(nShift + QOC_C_IV_SPREAD).Visible Then
        If m_gdOpt.Col(nShift + QOC_C_IV_BID).Visible Then GetIvMask = GetIvMask Or enMmQvCalcIvBid
        If m_gdOpt.Col(nShift + QOC_C_IV_ASK).Visible Then GetIvMask = GetIvMask Or enMmQvCalcIvAsk
    Else
        GetIvMask = GetIvMask Or enMmQvCalcIvBid Or enMmQvCalcIvAsk
    End If
End Function

Private Function UnderlyingAdjustRates(ByVal bForceUpdateCustom As Boolean) As Boolean
    On Error Resume Next
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
    Dim bForceRateUpdate As Boolean, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim dtNow As Date
    dtNow = GetNewYorkTime
    
    UnderlyingAdjustRates = False
    If m_Aux.Grp.ID = 0 Then Exit Function
    
    bForceRateUpdate = False
    
    dPos = g_UnderlyingAll(m_Aux.Grp.Und.ID).UndPosForRates
    
    If GetIrRuleType = enRateBasedOnPosition Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    
        If Not bUseMidRates Then _
            bForceRateUpdate = (Sgn(dPos) * Sgn(m_Aux.Grp.Und.UndPosForRates) < 0)
    
    Else
        bUseMidRates = True
    End If
    
    If bForceUpdateCustom Or m_Aux.Grp.Und.UseMidRates <> bUseMidRates Or bForceRateUpdate Then
        m_Aux.Grp.Und.UseMidRates = bUseMidRates
        m_Aux.Grp.Und.UndPosForRates = dPos
        
        If m_Aux.Grp.IsStockOrIndex Then
            Set aExpColl = m_Aux.Grp.Und.Exp
        Else
            Set aExpColl = m_Aux.Grp.Fut.Exp
        End If
        
        Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(m_Aux.Grp.Und.ID)
        
        For Each aExp In aExpColl
            If bUseMidRates Then
                aExp.Rate = GetNeutralRate(dtNow, aExp.ExpiryOV)
            Else
                aExp.Rate = IIf(dPos < 0#, GetShortRate(dtNow, aExp.ExpiryOV), GetLongRate(dtNow, aExp.ExpiryOV))
            End If
            
            If bIsHTBRatesExist Then
                aExp.HTBRate = GetHTBRate(m_Aux.Grp.Und.ID, dtNow, aExp.ExpiryOV)
            Else
                aExp.HTBRate = BAD_DOUBLE_VALUE
            End If
            
            If bForceUpdateCustom Or Not m_Aux.Grp.UseCustRates Then
                If QV.CustRates(CStr(CLng(aExp.ExpiryMonth))) Is Nothing Then
                    aExp.RateCust = aExp.Rate
                End If
            End If
        Next
        
        UnderlyingAdjustRates = True
    End If
End Function

Private Sub AddTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom, nRow&, nBS&, bUpdate As Boolean, dtExpiryMonth As Date
    Dim bRecalcOption As Boolean, aExp As EtsMmQuotesLib.MmQvExpAtom, bRatesUpdated As Boolean
    Dim bUpdatePos As Boolean
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "AddTrade Enter.", m_frmOwner.GetCaption
    
    If (Not aTrd.CheckByFilter(QV.TradeFilter)) Then Exit Sub
    
    bUpdate = False
    bRatesUpdated = False
    bRecalcOption = False
    bUpdatePos = True
    
    If aTrd.ContractType = enCtIndex Or aTrd.ContractType = enCtStock Then
        bRatesUpdated = UnderlyingAdjustRates(False) And Not m_Aux.Grp.UseCustRates
        If bRatesUpdated Then
            m_AuxOut.RatesUpdate
            m_Aux.AdjustDivsAndRatesVisible
            m_Aux.Grp.Und.SetDirty
        End If
    End If
        
    nBS = IIf(aTrd.IsBuy, 1, -1)
    
    If bUpdatePos Then
        If m_Aux.Grp.IsStockOrIndex Then
            If aTrd.ContractType = enCtOption Then
                dtExpiryMonth = DateSerial(Year(aTrd.Opt.Expiry), Month(aTrd.Opt.Expiry), Day(aTrd.Opt.Expiry))
                
                Set aExp = m_Aux.Grp.Und.Exp(dtExpiryMonth)
                Set aOpt = m_Aux.Grp.Und.Opt(aTrd.ContractID)
                If Not aOpt Is Nothing Then
                    If aOpt.ID <> 0 Then
                        If aOpt.Qty <= BAD_LONG_VALUE Then
                            aOpt.Qty = 0
                            bRecalcOption = True
                        End If
                        aOpt.Qty = aOpt.Qty + aTrd.Quantity * nBS
                        If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0
                        aOpt.QtyInShares = aOpt.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
                        bUpdate = True
                    End If
                End If
                
            ElseIf aTrd.ContractType = enCtIndex Or aTrd.ContractType = enCtStock Then
                If m_Aux.Grp.Und.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Und.Qty = 0
                m_Aux.Grp.Und.Qty = m_Aux.Grp.Und.Qty + aTrd.Quantity * nBS
                
                If m_Aux.Grp.Und.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Und.QtyInShares = 0
                m_Aux.Grp.Und.QtyInShares = m_Aux.Grp.Und.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
            
                bUpdate = True
            End If
        Else
            If aTrd.ContractType = enCtFutOption Then
                dtExpiryMonth = DateSerial(Year(aTrd.FutOpt.Expiry), Month(aTrd.FutOpt.Expiry), Day(aTrd.FutOpt.Expiry))
                
                Set aExp = m_Aux.Grp.Fut.Exp(dtExpiryMonth)
                Set aOpt = m_Aux.Grp.Fut.Opt(aTrd.ContractID)
                If Not aOpt Is Nothing Then
                    If aOpt.ID <> 0 Then
                        If aOpt.Qty <= BAD_LONG_VALUE Then
                            aOpt.Qty = 0
                            bRecalcOption = True
                        End If
                        aOpt.Qty = aOpt.Qty + aTrd.Quantity * nBS
                        If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0
                        aOpt.QtyInShares = aOpt.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
                        bUpdate = True
                    End If
                End If
                
            ElseIf aTrd.ContractType = enCtFuture Then
                If m_Aux.Grp.Fut.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.Qty = 0
                m_Aux.Grp.Fut.Qty = m_Aux.Grp.Fut.Qty + aTrd.Quantity * nBS
                
                If m_Aux.Grp.Fut.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.QtyInShares = 0
                m_Aux.Grp.Fut.QtyInShares = m_Aux.Grp.Fut.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
                
                bUpdate = True
            End If
        End If
    End If
    
    If bUpdate Or bRatesUpdated Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogDebug, "AddTrade tmrTradesProcessing Enabled.", m_frmOwner.GetCaption
    
        tmrTradesProcessing.Enabled = True
    End If
    
    Set aExp = Nothing
    Set aOpt = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "AddTrade Exit.", m_frmOwner.GetCaption
    
End Sub

Private Sub UpdateTrade(aNewTrd As EtsGeneralLib.MmTradeInfoAtom, aOldTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom, nRow&, nNewBS&, nOldBS&, bUpdate As Boolean, dtExpiryMonth As Date
    Dim bRecalcOption As Boolean, aExp As EtsMmQuotesLib.MmQvExpAtom, bRatesUpdated As Boolean
    Dim bAddPos As Boolean, bDelPos As Boolean
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "UpdateTrade Enter.", m_frmOwner.GetCaption
    
    Debug.Assert aNewTrd.ContractID = aOldTrd.ContractID
    
    bAddPos = False
    If aNewTrd.CheckByFilter(QV.TradeFilter) Then 'check is passed
        bAddPos = True
    End If
    
    bDelPos = False
    If aOldTrd.CheckByFilter(QV.TradeFilter) Then 'check is passed
        bDelPos = True
    End If
    
    bRecalcOption = False
    bUpdate = False
    bRatesUpdated = False
    
    
    If aNewTrd.ContractType = enCtIndex Or aNewTrd.ContractType = enCtStock _
        Or aOldTrd.ContractType = enCtIndex Or aOldTrd.ContractType = enCtStock Then
        
        bRatesUpdated = UnderlyingAdjustRates(False) And Not m_Aux.Grp.UseCustRates
        If bRatesUpdated Then
            m_AuxOut.RatesUpdate
            m_Aux.AdjustDivsAndRatesVisible
            m_Aux.Grp.Und.SetDirty
        End If
    End If
    
    nNewBS = IIf(aNewTrd.IsBuy, 1, -1)
    nOldBS = IIf(aOldTrd.IsBuy, 1, -1)
        
    If bAddPos Or bDelPos Then
        If m_Aux.Grp.IsStockOrIndex Then
            If aNewTrd.ContractType = enCtOption Then
                
                dtExpiryMonth = DateSerial(Year(aNewTrd.Opt.Expiry), Month(aNewTrd.Opt.Expiry), Day(aNewTrd.Opt.Expiry))
                
                Set aExp = m_Aux.Grp.Und.Exp(dtExpiryMonth)
                Set aOpt = m_Aux.Grp.Und.Opt(aNewTrd.ContractID)
                If Not aOpt Is Nothing Then
                    If aOpt.ID <> 0 Then
                        If bAddPos And bDelPos Then
                            If aOpt.Qty <= BAD_LONG_VALUE Then
                                aOpt.Qty = 0
                                bRecalcOption = True
                            End If
                            aOpt.Qty = aOpt.Qty - aOldTrd.Quantity * nOldBS + aNewTrd.Quantity * nNewBS
                            If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0
                            aOpt.QtyInShares = aOpt.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
                            bUpdate = True
                        End If

                        If bDelPos And Not bAddPos Then
                            If aOpt.Qty <= BAD_LONG_VALUE Then
                                aOpt.Qty = 0
                                bRecalcOption = True
                            End If
                            aOpt.Qty = aOpt.Qty - aOldTrd.Quantity * nOldBS
                            If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0
                            aOpt.QtyInShares = aOpt.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
                            bUpdate = True
                        End If
                        
                        If Not bDelPos And bAddPos Then
                            If aOpt.Qty <= BAD_LONG_VALUE Then
                                aOpt.Qty = 0
                                bRecalcOption = True
                            End If
                            aOpt.Qty = aOpt.Qty + aNewTrd.Quantity * nNewBS
                            If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0
                            aOpt.QtyInShares = aOpt.QtyInShares + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
                            bUpdate = True
                        End If
                    End If
                End If
                
            ElseIf aNewTrd.ContractType = enCtIndex Or aNewTrd.ContractType = enCtStock Then
                If bAddPos Then
                    If m_Aux.Grp.Und.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Und.Qty = 0
                    m_Aux.Grp.Und.Qty = m_Aux.Grp.Und.Qty - aOldTrd.Quantity * nOldBS
                    
                    If m_Aux.Grp.Und.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Und.QtyInShares = 0
                    m_Aux.Grp.Und.QtyInShares = m_Aux.Grp.Und.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
                    bUpdate = True
                End If
                                
                If bDelPos Then
                    If m_Aux.Grp.Und.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Und.Qty = 0
                    m_Aux.Grp.Und.Qty = m_Aux.Grp.Und.Qty + aNewTrd.Quantity * nNewBS
                    
                    If m_Aux.Grp.Und.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Und.QtyInShares = 0
                    m_Aux.Grp.Und.QtyInShares = m_Aux.Grp.Und.QtyInShares + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
                    bUpdate = True
                End If
            End If
        Else
            If aNewTrd.ContractType = enCtFutOption Then
                
                dtExpiryMonth = DateSerial(Year(aNewTrd.FutOpt.Expiry), Month(aNewTrd.FutOpt.Expiry), Day(aNewTrd.FutOpt.Expiry))
                
                Set aExp = m_Aux.Grp.Fut.Exp(dtExpiryMonth)
                Set aOpt = m_Aux.Grp.Fut.Opt(aNewTrd.ContractID)
                If Not aOpt Is Nothing Then
                    If aOpt.ID <> 0 Then
                        If bAddPos Then
                            If aOpt.Qty <= BAD_LONG_VALUE Then
                                aOpt.Qty = 0
                                bRecalcOption = True
                            End If
                            aOpt.Qty = aOpt.Qty - aOldTrd.Quantity * nOldBS
                            If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0
                            aOpt.QtyInShares = aOpt.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
                            bUpdate = True
                        End If
                        
                        If bDelPos Then
                            If aOpt.Qty <= BAD_LONG_VALUE Then
                                aOpt.Qty = 0
                                bRecalcOption = True
                            End If
                            aOpt.Qty = aOpt.Qty + aNewTrd.Quantity * nNewBS
                            If aOpt.QtyInShares <= BAD_LONG_VALUE Then aOpt.QtyInShares = 0
                            aOpt.QtyInShares = aOpt.QtyInShares + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
                            bUpdate = True
                        End If
                    End If
                End If
                
            ElseIf aNewTrd.ContractType = enCtFuture Then
                If bAddPos Then
                    If m_Aux.Grp.Fut.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.Qty = 0
                    m_Aux.Grp.Fut.Qty = m_Aux.Grp.Fut.Qty - aOldTrd.Quantity * nOldBS
                    
                    If m_Aux.Grp.Fut.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.QtyInShares = 0
                    m_Aux.Grp.Fut.QtyInShares = m_Aux.Grp.Fut.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
                    bUpdate = True
                End If
                
                If bDelPos Then
                    If m_Aux.Grp.Fut.Qty <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.Qty = 0
                    m_Aux.Grp.Fut.Qty = m_Aux.Grp.Fut.Qty + aNewTrd.Quantity * nNewBS
                    
                    If m_Aux.Grp.Fut.QtyInShares <= BAD_LONG_VALUE Then m_Aux.Grp.Fut.QtyInShares = 0
                    m_Aux.Grp.Fut.QtyInShares = m_Aux.Grp.Fut.QtyInShares + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
                    bUpdate = True
                End If
            End If
        End If
    End If
    
    If bUpdate Or bRatesUpdated Then
            tmrTradesProcessing.Enabled = True
    End If
    Set aExp = Nothing
    Set aOpt = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "UpdateTrade Exit.", m_frmOwner.GetCaption
    
End Sub

Private Sub DeleteTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom, nRow&, nBS&, bUpdate As Boolean, dtExpiryMonth As Date
    Dim bRatesUpdated As Boolean, bUpdatePos As Boolean
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "DeleteTrade Enter.", m_frmOwner.GetCaption
    
    If (Not aTrd.CheckByFilter(QV.TradeFilter)) Then Exit Sub
    
    bUpdate = False
    bRatesUpdated = False
    
    bUpdatePos = True
    
    If aTrd.ContractType = enCtIndex Or aTrd.ContractType = enCtStock Then
        bRatesUpdated = UnderlyingAdjustRates(False) And Not m_Aux.Grp.UseCustRates
        If bRatesUpdated Then
            m_AuxOut.RatesUpdate
            m_Aux.AdjustDivsAndRatesVisible
            m_Aux.Grp.Und.SetDirty
        End If
    End If
    
    nBS = IIf(aTrd.IsBuy, 1, -1)
    
    If bUpdatePos Then
        If m_Aux.Grp.IsStockOrIndex Then
            If aTrd.ContractType = enCtOption Then
                
                dtExpiryMonth = DateSerial(Year(aTrd.Opt.Expiry), Month(aTrd.Opt.Expiry), Day(aTrd.Opt.Expiry))
                
                Set aOpt = m_Aux.Grp.Und.Opt(aTrd.ContractID)
                If Not aOpt Is Nothing Then
                    If aOpt.ID <> 0 Then
                        If aOpt.Qty > BAD_LONG_VALUE Then
                            aOpt.Qty = aOpt.Qty - aTrd.Quantity * nBS
                            bUpdate = True
                        End If
                        
                        If aOpt.QtyInShares > BAD_LONG_VALUE Then
                            aOpt.QtyInShares = aOpt.QtyInShares - aTrd.Quantity * aTrd.LotSize * nBS
                            bUpdate = True
                        End If
                    End If
                End If
                
            ElseIf aTrd.ContractType = enCtIndex Or aTrd.ContractType = enCtStock Then
                If m_Aux.Grp.Und.Qty > BAD_LONG_VALUE Then
                    m_Aux.Grp.Und.Qty = m_Aux.Grp.Und.Qty - aTrd.Quantity * nBS
                    bUpdate = True
                End If
            
                If m_Aux.Grp.Und.QtyInShares > BAD_LONG_VALUE Then
                    m_Aux.Grp.Und.QtyInShares = m_Aux.Grp.Und.QtyInShares - aTrd.Quantity * aTrd.LotSize * nBS
                    bUpdate = True
                End If
            End If
        Else
            If aTrd.ContractType = enCtFutOption Then
                
                dtExpiryMonth = DateSerial(Year(aTrd.FutOpt.Expiry), Month(aTrd.FutOpt.Expiry), Day(aTrd.FutOpt.Expiry))
                
                Set aOpt = m_Aux.Grp.Fut.Opt(aTrd.ContractID)
                If Not aOpt Is Nothing Then
                    If aOpt.ID <> 0 Then
                        If aOpt.Qty > BAD_LONG_VALUE Then
                            aOpt.Qty = aOpt.Qty - aTrd.Quantity * nBS
                            bUpdate = True
                        End If
                        
                        If aOpt.QtyInShares > BAD_LONG_VALUE Then
                            aOpt.QtyInShares = aOpt.QtyInShares - aTrd.Quantity * aTrd.LotSize * nBS
                            bUpdate = True
                        End If
                    End If
                End If
                
            ElseIf aTrd.ContractType = enCtFuture Then
                If m_Aux.Grp.Fut.Qty > BAD_LONG_VALUE Then
                    m_Aux.Grp.Fut.Qty = m_Aux.Grp.Fut.Qty - aTrd.Quantity * nBS
                    bUpdate = True
                End If
            
                If m_Aux.Grp.Fut.QtyInShares > BAD_LONG_VALUE Then
                    m_Aux.Grp.Fut.QtyInShares = m_Aux.Grp.Fut.QtyInShares - aTrd.Quantity * aTrd.LotSize * nBS
                    bUpdate = True
                End If
            End If
        End If
    End If
    
    If bUpdate Or bRatesUpdated Then
       tmrTradesProcessing.Enabled = True
    End If
    Set aOpt = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "DeleteTrade Exit.", m_frmOwner.GetCaption
    
End Sub

Private Sub tmrTradesProcessing_Timer()
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "tmrTradesProcessing_Timer Enter.", m_frmOwner.GetCaption
    
    tmrTradesProcessing.Enabled = False
        
    If m_bDataLoad Or m_bLastQuoteReqNow Or m_bSubscribingNow Or m_bShutDown Then Exit Sub
    
    If Not m_Aux.RealTime Then
        CalculateUnderlyingOptions True
        m_AuxOut.UnderlyingUpdateQuote 1, False, False
        m_AuxOut.UnderlyingUpdateTotals
        UpdateTotals
    Else
        tmrRealTime.Enabled = True
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "tmrTradesProcessing_Timer Exit.", m_frmOwner.GetCaption
    
End Sub

Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
    On Error Resume Next
    If m_bShutDown Or m_bDataLoad Then Exit Sub
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_TradeAction Enter.", m_frmOwner.GetCaption
    
    If m_bDataLoad Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_TradeAction DatLoad Now.", m_frmOwner.GetCaption
        
        Exit Sub
    End If
    
'    If m_bLastQuoteReqNow Or m_bDataLoad Then
'        TradeQueue.Add aNewTrdInfo, aOldTrdInfo, enAction
'        Exit Sub
'    End If
'
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
    If m_bShutDown Or m_bDataLoad Then Exit Sub
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "TradeChannel_PositionTransfer Enter.", m_frmOwner.GetCaption

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
    m_bDataLoad = False
    m_bSubscribingNow = False
    m_bLastQuoteReqNow = False
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If m_bShutDown Or gCmn Is Nothing Then Exit Sub
    Dim nHeight&, nRows&, i&, nVisRows&, nGridHeight&, nGridColWidth&
    
    With fgUnd
        nVisRows = 0
        nRows = .Rows - 1
        For i = 1 To nRows
            If Not .RowHidden(i) Then nVisRows = nVisRows + 1
        Next
        
        .Top = 0
        .Left = 0
        
        nGridHeight = .RowHeight(0) + nVisRows * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        
        .Height = nGridHeight
        .Width = ScaleWidth
        nHeight = fgUnd.Height + GRID_VERT_SPACE
    End With
    
    If m_Aux.DividendsVisible And (m_Aux.Grp.ID = 0 Or m_Aux.Grp.ID <> 0 And m_Aux.Grp.ContractType = enCtStock) Or m_Aux.RatesVisible Or m_Aux.CalcModelVisible Or m_Aux.ExpCalendarVisible Then
        fgDiv.Top = fgUnd.Height + GRID_VERT_SPACE
        fgDiv.Left = 0
        fgDiv.Height = fgDiv.RowHeight(0) + (fgDiv.Rows - 1) * fgDiv.RowHeight(1) + ScaleY(fgDiv.GridLineWidth * 2, vbPixels, vbTwips)
        fgDiv.Width = ScaleWidth
        nHeight = nHeight + fgDiv.Height + GRID_VERT_SPACE
    End If
    
    If m_Aux.VolaVisible Then
        fgVol.Top = nHeight
        fgVol.Left = 0
        fgVol.Height = fgVol.RowHeight(0) + (fgVol.Rows - 1) * fgVol.RowHeight(1) + ScaleY(fgVol.GridLineWidth * 2, vbPixels, vbTwips)
        fgVol.Width = ScaleWidth
        nHeight = nHeight + fgVol.Height + GRID_VERT_SPACE
    End If
    
    With fgOpt
        .Left = 0
        .Top = nHeight
        .Width = ScaleWidth
        .Height = ScaleHeight - .Top
    End With
End Sub

Private Sub UserControl_Terminate()
    On Error Resume Next
    If Not m_bShutDown Then Term
End Sub

Public Sub Term()
    On Error Resume Next
    m_bShutDown = True
    
    If gCmn Is Nothing Then Exit Sub
    m_AuxOut.Term
    
    QV.Cancel
    tmrShow.Enabled = False
    
    m_bSubscribingNow = False
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
    
    ProcessRealTime = False
    m_Aux.Grp.Und.StopCalc
    
    m_bInProc = False
    m_bFireEvent = False
    
    Set frmLayout = Nothing
    Set TradeChannel = Nothing
    Set VolaSource = Nothing
    
    If Not PriceProvider Is Nothing Then
        lblStatus.Visible = True
        pbProgress.Visible = False
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        lblStatus.Caption = "Price provider connection closing..."
        lblStatus.Refresh
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Connection Close Start", m_frmOwner.GetCaption
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Cancel Last Quote", m_frmOwner.GetCaption
        
        If Not m_bSubscribingNow And Not m_Aux.RealTime Then
            PriceProvider.CancelLastQuote
        End If
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Unsubscribe Quote", m_frmOwner.GetCaption
        
        If m_bSubscribingNow Or m_Aux.RealTime Then
            PriceProvider.UnSubscribeQuote
            If g_RTQuantity > 0 Then
                g_RTQuantity = g_RTQuantity - 1
            End If
        End If
        If m_bGroupRequest Then
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Cancel Last Group Quote", m_frmOwner.GetCaption
            If Not m_bSubscribingNow And Not m_Aux.RealTime Then
                m_GroupPriceProvider.CancelLastGroupQuotes
            End If
            
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Unsubscribe Group Quote", m_frmOwner.GetCaption
            If m_bSubscribingNow Or m_Aux.RealTime Then
                m_GroupPriceProvider.UnSubscribeGroupQuotes
            End If
        End If
       
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Disconnect", m_frmOwner.GetCaption
        
        PriceProvider.Disconnect
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Destroy", m_frmOwner.GetCaption
        
        Set PriceProvider = Nothing
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider Connection Closed", m_frmOwner.GetCaption
    
    End If
    Set m_GroupPriceProvider = Nothing
    Set m_BatchPriceProvider = Nothing
    
    ClearViewAndData
    fgUnd.Rows = 1
    fgOpt.Rows = 1
    fgDiv.Rows = 1
    fgVol.Rows = 1
    
    Set geOpt = Nothing
    Set m_Aux.QuoteReqsAll = Nothing
    Set m_Aux.Grp = Nothing
    Set m_Aux.QV = Nothing
    Set QV = Nothing

    m_Aux.RealTime = False
    m_Aux.Term

    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set aParams = Nothing
End Sub

Public Sub ShowAllOptExchanges()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.OptExchAll.Count <= 1 Then Exit Sub
    Dim aExch As EtsGeneralLib.ExchAtom
    
    For Each aExch In m_Aux.Grp.OptExchAll
        aExch.Visible = True
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExch In m_Aux.Grp.Und.OptExch
            aExch.Visible = True
        Next
    Else
        For Each aExch In m_Aux.Grp.Fut.OptExch
            aExch.Visible = True
        Next
    End If
        
    AdjustOptPosExchIDs
    RefreshOptsGrid True

    RaiseEvent OnOptExchangesChange(True)
End Sub

Public Sub ShowAllUndExchanges()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Dim aExch As EtsGeneralLib.ExchAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    
    For Each aExch In m_Aux.Grp.UndExchAll
        aExch.Visible = True
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExch In m_Aux.Grp.Und.UndExch
            aExch.Visible = True
        Next
            
        If Not m_Aux.Grp.Und.SynthUnd Is Nothing Then
            For Each aUnd In m_Aux.Grp.Und.SynthUnd
                For Each aExch In aUnd.UndExch
                    aExch.Visible = True
                Next
            Next
        End If
    Else
        For Each aExch In m_Aux.Grp.Fut.FutExch
            aExch.Visible = True
        Next
    End If
    
    ShowUndExchanges
    
    RaiseEvent OnUndExchangesChange(True)
End Sub

Public Sub ShowNextOptExchange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.OptExchAll.Count <= 1 Then Exit Sub
    Dim i&, aExch As EtsGeneralLib.ExchAtom
    
    m_nCurrentOptExchIdx = m_nCurrentOptExchIdx + 1
    If m_nCurrentOptExchIdx > m_Aux.Grp.OptExchAll.Count Then m_nCurrentOptExchIdx = 1
    
    i = 1
    For Each aExch In m_Aux.Grp.OptExchAll
        aExch.Visible = (i = m_nCurrentOptExchIdx)
        i = i + 1
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExch In m_Aux.Grp.Und.OptExch
            aExch.Visible = m_Aux.Grp.OptExchAll(aExch.ID).Visible
        Next
    Else
        For Each aExch In m_Aux.Grp.Und.OptExch
            aExch.Visible = m_Aux.Grp.OptExchAll(aExch.ID).Visible
        Next
    End If
    
    AdjustOptExchVisible
    AdjustOptPosExchIDs
    
    RefreshOptsGrid True
    
    RaiseEvent OnOptExchangesChange(True)
End Sub

Public Sub ShowPrevOptExchange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.OptExchAll.Count <= 1 Then Exit Sub
    
    Dim i&, aExch As EtsGeneralLib.ExchAtom
    
    m_nCurrentOptExchIdx = m_nCurrentOptExchIdx - 1
    If m_nCurrentOptExchIdx < 1 Then m_nCurrentOptExchIdx = m_Aux.Grp.OptExchAll.Count
    
    i = 1
    For Each aExch In m_Aux.Grp.OptExchAll
        aExch.Visible = (i = m_nCurrentOptExchIdx)
        i = i + 1
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExch In m_Aux.Grp.Und.OptExch
            aExch.Visible = m_Aux.Grp.OptExchAll(aExch.ID).Visible
        Next
    Else
        For Each aExch In m_Aux.Grp.Fut.OptExch
            aExch.Visible = m_Aux.Grp.OptExchAll(aExch.ID).Visible
        Next
    End If
    
    AdjustOptExchVisible
    AdjustOptPosExchIDs
    
    RefreshOptsGrid True
    
    RaiseEvent OnOptExchangesChange(True)
End Sub

Public Sub ShowOptExchange(ByVal nID As Long, ByVal bShow As Boolean, Optional ByVal bUpdate As Boolean = True)
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.OptExchAll.Count <= 1 Then Exit Sub
    
    Dim nVisibleCount&, aExch As EtsGeneralLib.ExchAtom, aExchAll As EtsGeneralLib.ExchAtom
    
    If Not bShow Then
        For Each aExch In m_Aux.Grp.OptExchAll
            If aExch.Visible Then nVisibleCount = nVisibleCount + 1
        Next
    Else
        nVisibleCount = 2
    End If
    
    If nVisibleCount > 1 Then
        Set aExchAll = m_Aux.Grp.OptExchAll(nID)
        
        If Not aExchAll Is Nothing Then
        
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Exchange -> """ & aExchAll.Code & " - [" & aExchAll.Name & "]""" & IIf(bShow, " Selected.", " Unselected."), m_frmOwner.GetCaption
        
            aExchAll.Visible = bShow
            If m_Aux.Grp.IsStockOrIndex Then
                Set aExch = m_Aux.Grp.Und.OptExch(nID)
            Else
                Set aExch = m_Aux.Grp.Fut.OptExch(nID)
            End If
            
            If Not aExch Is Nothing Then
                aExch.Visible = bShow
                
                AdjustOptExchVisible
                AdjustOptPosExchIDs
                
                Set aExch = Nothing
            End If
            If bUpdate Then RefreshOptsGrid True
           
            Set aExchAll = Nothing
        Else
            Debug.Assert False
        End If
        
        RaiseEvent OnOptExchangesChange(True)
    End If
End Sub

Private Sub AdjustOptRootVisible()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Dim aOptRoot As EtsMmQuotesLib.MmQvOptRootAtom
    Dim aOptRootColl As EtsMmQuotesLib.MmQvOptRootColl
    
    If m_Aux.Grp.IsStockOrIndex Then
        Set aOptRootColl = m_Aux.Grp.Und.OptRoot
    Else
        Exit Sub
    End If
    
    Dim aFilter As EtsGeneralLib.EtsMmFilterAtomColl
    Set aFilter = g_InVisibleOptRoots(m_Aux.Grp.Symbol)
                            
    For Each aOptRoot In aOptRootColl
        aOptRoot.Visible = True
        If Not aFilter Is Nothing Then
            If Not aFilter(aOptRoot.Name) Is Nothing Then
                aOptRoot.Visible = False
            End If
        End If
    Next
    
    Set aFilter = Nothing
    Set aOptRootColl = Nothing
    
End Sub

Private Sub AdjustOptExchVisible()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Dim aExch As EtsGeneralLib.ExchAtom, aOptExch As EtsGeneralLib.ExchAtom
    Dim bNoVisibleExch As Boolean, aExchColl As EtsGeneralLib.ExchColl
    
    If m_Aux.Grp.IsStockOrIndex Then
        Set aExchColl = m_Aux.Grp.Und.OptExch
    Else
        Set aExchColl = m_Aux.Grp.Fut.OptExch
    End If
    
    bNoVisibleExch = True
    For Each aExch In aExchColl
        Set aOptExch = m_Aux.Grp.OptExchAll(aExch.ID)
        If Not aOptExch Is Nothing Then
            aExch.Visible = aOptExch.Visible
            If aExch.Visible Then bNoVisibleExch = False
            Set aOptExch = Nothing
        End If
    Next
    
    If bNoVisibleExch Then aExchColl(0).Visible = True

    Set aExchColl = Nothing
End Sub

Private Sub AdjustOptPosExchIDs()
    On Error Resume Next
    Dim aExch As EtsGeneralLib.ExchAtom
    
    If m_Aux.Grp.IsStockOrIndex Then
        m_Aux.Grp.Und.OptPosExchID = 0
        For Each aExch In m_Aux.Grp.Und.OptExch
            If aExch.Visible Then
                m_Aux.Grp.Und.OptPosExchID = aExch.ID
                Exit For
            End If
        Next
    Else
        m_Aux.Grp.Fut.OptPosExchID = 0
        For Each aExch In m_Aux.Grp.Fut.OptExch
            If aExch.Visible Then
                m_Aux.Grp.Fut.OptPosExchID = aExch.ID
                Exit For
            End If
        Next
    End If
    
End Sub

Public Sub ShowUndExchange(ByVal nID As Long, ByVal bShow As Boolean)
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.UndExchAll.Count <= 1 Then Exit Sub
    
    Dim nVisibleCount&, aExch As EtsGeneralLib.ExchAtom, aExchAll As EtsGeneralLib.ExchAtom
    
    If Not bShow Then
        For Each aExch In m_Aux.Grp.UndExchAll
            If aExch.Visible Then nVisibleCount = nVisibleCount + 1
        Next
    Else
        nVisibleCount = 2
    End If
    
    If nVisibleCount > 1 Then
        Set aExchAll = m_Aux.Grp.UndExchAll(nID)
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Exchange -> """ & aExchAll.Code & " - [" & aExchAll.Name & "]""" & IIf(bShow, " Selected.", " Unselected."), m_frmOwner.GetCaption
        
        If Not aExchAll Is Nothing Then
            aExchAll.Visible = bShow
            
            AdjustUnderlyingsExchVisible
            AdjustUnderlyingsPosExchID
            
            ShowUndExchanges
            
            Set aExchAll = Nothing
        Else
            Debug.Assert False
        End If
        
        RaiseEvent OnUndExchangesChange(True)
    End If
End Sub

Private Sub ShowUndExchanges()
    On Error Resume Next
    m_AuxOut.ShowUndExchanges
    m_AuxOut.UnderlyingUpdateTotals
    UserControl_Resize
End Sub

Private Sub AdjustUnderlyingsExchVisible()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Dim aExch As EtsGeneralLib.ExchAtom, aUndExch As EtsGeneralLib.ExchAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim bNoVisibleExch As Boolean
    
    bNoVisibleExch = True
    m_nUndMainRow = 1
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExch In m_Aux.Grp.Und.UndExch
            Set aUndExch = m_Aux.Grp.UndExchAll(aExch.ID)
            If Not aUndExch Is Nothing Then
                aExch.Visible = aUndExch.Visible
                If aExch.Visible Then
                    bNoVisibleExch = False
                Else
                    If bNoVisibleExch Then m_nUndMainRow = m_nUndMainRow + 1
                End If
                Set aUndExch = Nothing
            End If
        Next
        
        If bNoVisibleExch Then
            m_Aux.Grp.Und.UndExch(0).Visible = True
            m_nUndMainRow = 1
        End If
    
        If Not m_Aux.Grp.Und.SynthUnd Is Nothing Then
            For Each aUnd In m_Aux.Grp.Und.SynthUnd
                
                bNoVisibleExch = True
                For Each aExch In aUnd.UndExch
                    Set aUndExch = m_Aux.Grp.UndExchAll(aExch.ID)
                    If Not aUndExch Is Nothing Then
                        aExch.Visible = aUndExch.Visible
                        If aExch.Visible Then bNoVisibleExch = False
                        Set aUndExch = Nothing
                    End If
                Next
                
                If bNoVisibleExch Then aUnd.UndExch(0).Visible = True
            Next
        End If
    Else
        For Each aExch In m_Aux.Grp.Fut.FutExch
            Set aUndExch = m_Aux.Grp.UndExchAll(aExch.ID)
            If Not aUndExch Is Nothing Then
                aExch.Visible = aUndExch.Visible
                If aExch.Visible Then
                    bNoVisibleExch = False
                Else
                    If bNoVisibleExch Then m_nUndMainRow = m_nUndMainRow + 1
                End If
                Set aUndExch = Nothing
            End If
        Next
        
        If bNoVisibleExch Then
            m_Aux.Grp.Fut.FutExch(0).Visible = True
            m_nUndMainRow = 1
        End If
    End If
End Sub

Private Sub AdjustUnderlyingsPosExchID()
    On Error Resume Next
    Dim aExch As EtsGeneralLib.ExchAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExch In m_Aux.Grp.Und.UndExch
            If aExch.Visible Then
                m_Aux.Grp.Und.PosExchID = aExch.ID
                Exit For
            End If
        Next
        
        If Not m_Aux.Grp.Und.SynthUnd Is Nothing Then
            For Each aUnd In m_Aux.Grp.Und.SynthUnd
                For Each aExch In aUnd.UndExch
                    If aExch.Visible Then
                        aUnd.PosExchID = aExch.ID
                        Exit For
                    End If
                Next
            Next
        End If
    Else
        For Each aExch In m_Aux.Grp.Fut.FutExch
            If aExch.Visible Then
                m_Aux.Grp.Fut.PosExchID = aExch.ID
                Exit For
            End If
        Next
    End If
End Sub

Public Sub ShowAllStrikes()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.StrikeAll.Count <= 1 Then Exit Sub
    
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom
    
    For Each aStr In m_Aux.Grp.StrikeAll
        aStr.Visible = True
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExp In m_Aux.Grp.Und.Exp
            For Each aStr In aExp.Strike
                aStr.Visible = True
            Next
        Next
    Else
        For Each aExp In m_Aux.Grp.Fut.Exp
            For Each aStr In aExp.Strike
                aStr.Visible = True
            Next
        Next
    End If
    RefreshOptsGrid True
End Sub

Public Sub ShowOptStrike(ByRef aStrikes As EtsGeneralLib.EtsMmEntityAtomColl)
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.StrikeAll.Count <= 1 Then Exit Sub
    
    Dim aEnt As EtsGeneralLib.EtsMmEntityAtom, aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom
    
    For Each aEnt In aStrikes
        m_Aux.Grp.StrikeAll(aEnt.Data3).Visible = (aEnt.Data <> 0)
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExp In m_Aux.Grp.Und.Exp
            For Each aStr In aExp.Strike
                aStr.Visible = m_Aux.Grp.StrikeAll(aStr.Strike).Visible
            Next
        Next
    Else
        For Each aExp In m_Aux.Grp.Fut.Exp
            For Each aStr In aExp.Strike
                aStr.Visible = m_Aux.Grp.StrikeAll(aStr.Strike).Visible
            Next
        Next
    End If
    RefreshOptsGrid True
End Sub

Public Sub ShowAllExpiries()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ExpAll.Count <= 1 Then Exit Sub
    
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom
    
    For Each aExp In m_Aux.Grp.ExpAll
        aExp.Visible = True
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExp In m_Aux.Grp.Und.Exp
            aExp.Visible = True
        Next
    Else
        For Each aExp In m_Aux.Grp.Fut.Exp
            aExp.Visible = True
        Next
    End If
    
    RefreshOptsGrid True
    
    RaiseEvent OnExpiriesChange(True)
End Sub

Public Sub ShowExpiry(ByVal nIndex As Long, ByVal bShow As Boolean)
    On Error Resume Next
    If m_bShutDown Or nIndex < 1 Or nIndex > m_Aux.Grp.ExpAll.Count Then Exit Sub
    
    Dim nVisibleCount&, i&, aExpAll As EtsMmQuotesLib.MmQvExpAtom, aExp As EtsMmQuotesLib.MmQvExpAtom
    
    If Not bShow Then
        i = 1
        For Each aExp In m_Aux.Grp.ExpAll
            If aExp.Visible Then nVisibleCount = nVisibleCount + 1
            If i = nIndex Then Set aExpAll = aExp
            i = i + 1
        Next
    Else
        i = 1
        For Each aExp In m_Aux.Grp.ExpAll
            If i = nIndex Then
                Set aExpAll = aExp
                Exit For
            End If
            i = i + 1
        Next
        nVisibleCount = 2
    End If
    
    If nVisibleCount > 1 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Expiry -> """ & nIndex & Format$(aExpAll.ExpiryMonth, " - mmm yy (") & aExpAll.RootNames & ")""" & IIf(bShow, " Selected.", " Unselected."), m_frmOwner.GetCaption
    
        aExpAll.Visible = bShow
        
        If m_Aux.Grp.IsStockOrIndex Then
            m_Aux.Grp.Und.Exp(aExpAll.ExpiryMonth).Visible = bShow
        Else
            m_Aux.Grp.Fut.Exp(aExpAll.ExpiryMonth).Visible = bShow
        End If
        
        RefreshOptsGrid True
        
        RaiseEvent OnExpiriesChange(True)
    End If
    
    Set aExpAll = Nothing
End Sub

Public Sub ShowNextExpiry()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ExpAll.Count <= 1 Then Exit Sub
    
    Dim nIdx&, aExp As EtsMmQuotesLib.MmQvExpAtom
    
    m_nCurrentExpiryIdx = m_nCurrentExpiryIdx + 1
    If m_nCurrentExpiryIdx > m_Aux.Grp.ExpAll.Count Then m_nCurrentExpiryIdx = 1
    
    nIdx = 1
    For Each aExp In m_Aux.Grp.ExpAll
        aExp.Visible = (nIdx = m_nCurrentExpiryIdx)
        nIdx = nIdx + 1
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExp In m_Aux.Grp.Und.Exp
            aExp.Visible = m_Aux.Grp.ExpAll(aExp.ExpiryMonth).Visible
        Next
    Else
        For Each aExp In m_Aux.Grp.Fut.Exp
            aExp.Visible = m_Aux.Grp.ExpAll(aExp.ExpiryMonth).Visible
        Next
    End If
    RefreshOptsGrid True
    
    RaiseEvent OnExpiriesChange(True)
End Sub

Public Sub ShowPrevExpiry()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ExpAll.Count <= 1 Then Exit Sub
    
    Dim nIdx&, aExp As EtsMmQuotesLib.MmQvExpAtom
    
    m_nCurrentExpiryIdx = m_nCurrentExpiryIdx - 1
    If m_nCurrentExpiryIdx < 1 Then m_nCurrentExpiryIdx = m_Aux.Grp.ExpAll.Count
    
    nIdx = 1
    For Each aExp In m_Aux.Grp.ExpAll
        aExp.Visible = (nIdx = m_nCurrentExpiryIdx)
        nIdx = nIdx + 1
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aExp In m_Aux.Grp.Und.Exp
            aExp.Visible = m_Aux.Grp.ExpAll(aExp.ExpiryMonth).Visible
        Next
    Else
        For Each aExp In m_Aux.Grp.Fut.Exp
            aExp.Visible = m_Aux.Grp.ExpAll(aExp.ExpiryMonth).Visible
        Next
    End If
    
    RefreshOptsGrid True
    
    RaiseEvent OnExpiriesChange(True)
End Sub

Public Sub ShowAllOptRoots()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.Und.OptRoot.Count <= 1 Then Exit Sub
    
    Dim aRoot As EtsMmQuotesLib.MmQvOptRootAtom

    If m_Aux.Grp.IsStockOrIndex Then
        For Each aRoot In m_Aux.Grp.Und.OptRoot
            aRoot.Visible = True
        Next
    
        RefreshOptsGrid True
    
        RaiseEvent OnOptRootsChange(True)
    End If
End Sub

Public Sub ShowOptRoot(ByVal nIndex As Long, ByVal bShow As Boolean, Optional ByVal bRefresh As Boolean = True)
    On Error Resume Next
    If m_bShutDown Or nIndex < 1 Or nIndex > m_Aux.Grp.Und.OptRoot.Count Then Exit Sub
    
    Dim nVisibleCount&, i&, aRoot As EtsMmQuotesLib.MmQvOptRootAtom, aRoot2 As EtsMmQuotesLib.MmQvOptRootAtom

    If m_Aux.Grp.IsStockOrIndex Then
        If Not bShow Then
            i = 1
            For Each aRoot In m_Aux.Grp.Und.OptRoot
                If aRoot.Visible Then nVisibleCount = nVisibleCount + 1
                If i = nIndex Then Set aRoot2 = aRoot
                i = i + 1
            Next
        Else
            i = 1
            For Each aRoot In m_Aux.Grp.Und.OptRoot
                If i = nIndex Then
                    Set aRoot2 = aRoot
                    Exit For
                End If
                i = i + 1
            Next
            nVisibleCount = 2
        End If
    
        If nVisibleCount > 1 Then
        
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Option Root -> """ & nIndex & " - " & aRoot2.Name & IIf(bShow, """ Selected.", """ Unselected."), m_frmOwner.GetCaption
                
                
            Dim aFilter As EtsGeneralLib.EtsMmFilterAtomColl
            Set aFilter = g_InVisibleOptRoots(m_Aux.Grp.Symbol)
                
            If bShow Then
                If Not aFilter Is Nothing Then
                    If Not aFilter(aRoot2.Name) Is Nothing Then
                        aFilter.Remove aRoot2.Name
                    End If
                End If
            Else
                If aFilter Is Nothing Then
                    Set aFilter = New EtsGeneralLib.EtsMmFilterAtomColl
                    aFilter.Name = m_Aux.Grp.Symbol
                    g_InVisibleOptRoots.Add aFilter, m_Aux.Grp.Symbol
                End If
                
                Dim aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
                
                Set aFilterAtom = aFilter.Add(aRoot2.Name)
                aFilterAtom.ID = aRoot2.ID
                aFilterAtom.Name = aRoot2.Name
                
                Set aFilterAtom = Nothing
            End If
                
            Set aFilter = Nothing
            
        
            aRoot2.Visible = bShow
            Set aRoot2 = Nothing

            If bRefresh Then RefreshOptsGrid True

    
            RaiseEvent OnOptRootsChange(True)
        End If
    End If
End Sub

Public Sub ShowNextOptRoot()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.Und.OptRoot.Count <= 1 Then Exit Sub
    
    Dim i&, aRoot As EtsMmQuotesLib.MmQvOptRootAtom

    If m_Aux.Grp.IsStockOrIndex Then
        m_nCurrentOptRootIdx = m_nCurrentOptRootIdx + 1
        If m_nCurrentOptRootIdx > m_Aux.Grp.Und.OptRoot.Count Then m_nCurrentOptRootIdx = 1
    
        i = 1
        For Each aRoot In m_Aux.Grp.Und.OptRoot
            aRoot.Visible = (i = m_nCurrentOptRootIdx)
            i = i + 1
        Next
    
        RefreshOptsGrid True
    
        RaiseEvent OnOptRootsChange(True)
    End If
End Sub

Public Sub ShowPrevOptRoot()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.Und.OptRoot.Count <= 1 Then Exit Sub
    
    Dim i&, aRoot As EtsMmQuotesLib.MmQvOptRootAtom

    If m_Aux.Grp.IsStockOrIndex Then
        m_nCurrentOptRootIdx = m_nCurrentOptRootIdx - 1
        If m_nCurrentOptRootIdx < 1 Then m_nCurrentOptRootIdx = m_Aux.Grp.Und.OptRoot.Count
    
        i = 1
        For Each aRoot In m_Aux.Grp.Und.OptRoot
            aRoot.Visible = (i = m_nCurrentOptRootIdx)
            i = i + 1
        Next
    
        RefreshOptsGrid True
    
        RaiseEvent OnOptRootsChange(True)
    End If
End Sub

Private Sub StopRealTime()
    On Error Resume Next
    m_Aux.RealTime = False
    m_bSubscribingNow = False
    'Screen.MousePointer = vbArrow

    
    lblStatus.Visible = True
    pbProgress.Visible = False
    lblProcess.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    lblStatus.Caption = "Real Time (Quotes unsubscription...)"
    lblStatus.Refresh
    
    PriceProvider.UnSubscribeQuote
    If g_RTQuantity > 0 Then
       g_RTQuantity = g_RTQuantity - 1
    End If

    If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes
    
    UpdateTotals
    
    AdjustCaption
    AdjustState
    Screen.MousePointer = vbDefault
End Sub

Private Sub StartRealTime()
    On Error GoTo EH
    Dim sExch$, aReq As MmQvRequestAtom, aExch As EtsGeneralLib.ExchAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim tmr As Long
    Dim ttt As Long
    
    If m_bShutDown Or m_bInProc Or m_bLastQuoteReqNow Then Exit Sub
    
    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bSubscribingNow
    
        QV.UndResponses = 0
        QV.OptResponses = 0
        QV.FutResponses = 0
    
        If m_Aux.QuoteReqsAll.Count > 0 Then
            If Not g_PerformanceLog Is Nothing Then m_nOperation = g_PerformanceLog.BeginLogMmOperation
    
            m_bSubscribingNow = True
            m_bInProc = True
            AdjustState
            
            m_bGroupRequest = m_bGroupRequestSupported And (m_Aux.Grp.IsStockOrIndex) And Not g_Params.QuoteReqTypeAlwaysNonGroup
            
            m_nQuoteReqCount = m_Aux.QuoteReqsAll.Count '- m_DecExpectations
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
                tmr = GetTickCount
                QV.StartRealTime
                tmr = GetTickCount - tmr
                ttt = 0
            Else
                aParam.Symbol = m_Aux.Grp.Und.Symbol
                aParam.Type = IIf(m_Aux.Grp.Und.UndType = enCtStock, enSTK, enIDX)
                
                m_nQuoteGroupReqCount = m_Aux.Grp.Und.UndExch.Count + 1
                m_nQuoteGroupReqDone = 0&
                
                For Each aExch In m_Aux.Grp.Und.UndExch
                    If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                    aParam.Exchange = aExch.Code
                    PriceProvider.SubscribeQuote aParam
                    DoEvents
                Next
                
                If Not m_bShutDown And m_bSubscribingNow And Not m_Aux.Grp.Und.SynthUnd Is Nothing Then
                    
                    For Each aUnd In m_Aux.Grp.Und.SynthUnd
                        If aUnd.ID <> USD_ID Then
                            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + aUnd.UndExch.Count
                        
                            aParam.Symbol = aUnd.Symbol
                            aParam.Type = IIf(aUnd.UndType = enCtStock, enSTK, enIDX)
                                        
                            For Each aExch In aUnd.UndExch
                                If m_bShutDown Or Not m_bSubscribingNow Then Exit For
                                
                                aParam.Exchange = aExch.Code
                                PriceProvider.SubscribeQuote aParam
                                DoEvents
                            Next
                        End If
                    Next
                End If
                    
                aParam.Symbol = m_Aux.Grp.Und.Symbol
                aParam.Type = IIf(m_Aux.Grp.Und.UndType = enCtStock, enSTK, enIDX)
                
                If Not m_bShutDown And m_bSubscribingNow Then
                    sExch = ""
                    
                    For Each aExch In m_Aux.Grp.Und.OptExch
                        If aExch.ID <> 0 Then sExch = sExch & "," & aExch.Code
                    Next
                    
                    aParam.Exchange = sExch
                    aParam.Type = IIf(m_Aux.Grp.Und.UndType = enCtStock, enGrSTK, enGrIDX)
                    m_GroupPriceProvider.SubscribeGroupQuotes aParam
                End If
                
            End If
            
            m_bInProc = False
            AdjustState
            
        Else
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            lblProcess.Caption = ""
            imgStop.Visible = False
            imgStopDis.Visible = False
        End If
    Else
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
        gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If

    Exit Sub
EH:
    m_bSubscribingNow = False
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to start real time mode."
    On Error Resume Next
    AdjustCaption
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    AdjustState
    AdjustCaption
    
    PriceProvider.UnSubscribeQuote
    If m_bGroupRequest Then m_GroupPriceProvider.UnSubscribeGroupQuotes

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, QV.UndResponses, QV.OptResponses, QV.FutResponses
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    Dim sCaption$
    
    sCaption = ""
    If m_Aux.Grp.ID <> 0 Then sCaption = sCaption & m_Aux.Grp.Symbol & " - "
    If m_Aux.RealTime Then _
        sCaption = sCaption & IIf(m_Aux.RealTimeConnected, "[Real Time] - ", "[Real Time (Disconnected)] - ")
    sCaption = sCaption & "Quotes"
    
    GetCaption = sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    If m_Aux.Grp.ID <> 0 Then
        GetShortCaption = m_Aux.Grp.Symbol
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

Public Property Get Expiries() As EtsMmQuotesLib.MmQvExpColl
    Set Expiries = m_Aux.Grp.ExpAll
End Property

Public Property Get UndExchanges() As EtsGeneralLib.ExchColl
    Set UndExchanges = m_Aux.Grp.UndExchAll
End Property

Public Property Get OptExchanges() As EtsGeneralLib.ExchColl
    Set OptExchanges = m_Aux.Grp.OptExchAll
End Property

Public Property Get OptRoots() As EtsMmQuotesLib.MmQvOptRootColl
    Set OptRoots = m_Aux.Grp.Und.OptRoot
End Property

Public Property Get Strikes() As EtsMmQuotesLib.MmQvStrikeColl
    Set Strikes = m_Aux.Grp.StrikeAll
End Property

Public Property Get Underlying() As EtsMmQuotesLib.MmQvUndAtom
    Set Underlying = m_Aux.Grp.Und
End Property

Public Property Get Group() As EtsMmQuotesLib.MmQvGrpAtom
    Set Group = m_Aux.Grp
End Property

Public Sub CustomizeUnderlyingGridLayout()
    On Error Resume Next
    If Not m_bShutDown And Not frmLayout Is Nothing Then frmLayout.Execute GT_QUOTES_UNDERLYING, QUC_SYMBOL, QUC_LAST_COLUMN, m_gdUnd, m_frmOwner
End Sub

Public Sub CustomizeDivGridLayout()
    On Error Resume Next
    If Not m_bShutDown And Not frmLayout Is Nothing Then frmLayout.Execute GT_QUOTES_DIVIDENDS, QDC_TRADES, QDC_LAST_COLUMN, m_gdDiv, m_frmOwner
End Sub

Public Sub CustomizeOptionGridLayout()
    On Error Resume Next
    If Not m_bShutDown And Not frmLayout Is Nothing Then frmLayout.Execute GT_QUOTES_OPTIONS, QOC_C_SYMBOL, QOC_LAST_COLUMN, m_gdOpt, m_frmOwner
End Sub

Public Sub CustomizeVolaGridLayout()
    On Error Resume Next
    If Not m_bShutDown And Not frmLayout Is Nothing Then frmLayout.Execute GT_QUOTES_VOLA, QVC_VOLA_VAL, QVC_LAST_COLUMN, m_gdVol, m_frmOwner
End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aExch As EtsGeneralLib.ExchAtom, sValue$, aRoot As EtsMmQuotesLib.MmQvOptRootAtom
    Dim aStr As EtsMmQuotesLib.MmQvStrikeAtom
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    SaveParams

    ' common info
    aStorage.SetStringValue "Quote" & sKey, "Symbol", m_Aux.Grp.Symbol
    aStorage.SetLongValue "Quote" & sKey, "ShowModel", m_Aux.CalcModelVisible
    aStorage.SetLongValue "Quote" & sKey, "ShowCalendar", m_Aux.ExpCalendarVisible
    aStorage.SetLongValue "Quote" & sKey, "ShowDivs", m_Aux.DividendsVisible
    aStorage.SetLongValue "Quote" & sKey, "ShowVola", m_Aux.VolaVisible
    aStorage.SetLongValue "Quote" & sKey, "ShowRates", m_Aux.RatesVisible
'    aStorage.SetLongValue "Quote" & sKey, "CurrentExpiryIdx", m_nCurrentExpiryIdx
'    aStorage.SetLongValue "Quote" & sKey, "CurrentOptExchIdx", m_nCurrentOptExchIdx
    aStorage.SetLongValue "Quote" & sKey, "TradesFilter", QV.TradeFilter.Data(enFtTrades)
    aStorage.SetLongValue "Quote" & sKey, "TradesStrategy", QV.TradeFilter.Data(enFtStrategy)
    aStorage.SetLongValue "Quote" & sKey, "TradesTrader", QV.TradeFilter.Data(enFtTrader)
    aStorage.SetLongValue "Quote" & sKey, "IsVolaSimulated", m_Aux.IsVolaSimulated

    StoreVisibles
    
    If QV.VisibleExp.Count < m_Aux.Grp.ExpAll.Count Then
        sValue = ""
        For Each aExp In m_Aux.Grp.ExpAll
            If aExp.Visible Then
                sValue = sValue & "|" & CLng(aExp.ExpiryMonth)
            End If
        Next
        
        If Len(sValue) > 0 Then
            sValue = Mid$(sValue, 2)
            aStorage.SetStringValue "Quote" & sKey, "VisibleExp", sValue
        End If
    End If

    If m_Aux.Grp.IsStockOrIndex Then
        sValue = ""
        For Each aExp In m_Aux.Grp.Und.Exp
            sValue = sValue & "|" & Trim$(Str$(CLng(aExp.ExpiryMonth))) & "," & Trim$(Str$(aExp.RateCust))
        Next
    Else
        sValue = ""
        For Each aExp In m_Aux.Grp.Fut.Exp
            sValue = sValue & "|" & Trim$(Str$(CLng(aExp.ExpiryMonth))) & "," & Trim$(Str$(aExp.RateCust))
        Next
    End If

    aStorage.SetLongValue "Quote" & sKey, "UseCustRates", m_Aux.Grp.UseCustRates
    If Len(sValue) > 0 Then
        sValue = Mid$(sValue, 2)
        aStorage.SetStringValue "Quote" & sKey, "CustRates", sValue
    End If

    If QV.VisibleStr.Count < m_Aux.Grp.StrikeAll.Count Then
        sValue = ""
        For Each aStr In m_Aux.Grp.StrikeAll
            If aStr.Visible Then
                sValue = sValue & "|" & Trim$(Str$(aStr.Strike))
            End If
        Next
        
        If Len(sValue) > 0 Then
            sValue = Mid$(sValue, 2)
            aStorage.SetStringValue "Quote" & sKey, "VisibleStr", sValue
        End If
    End If

    If m_Aux.Grp.IsStockOrIndex Then
        If QV.VisibleRoot.Count < m_Aux.Grp.Und.OptRoot.Count Then
            sValue = ""
            For Each aRoot In m_Aux.Grp.Und.OptRoot
                If aRoot.Visible Then
                    sValue = sValue & "|" & aRoot.Name
                End If
            Next
            
            If Len(sValue) > 0 Then
                sValue = Mid$(sValue, 2)
                aStorage.SetStringValue "Quote" & sKey, "VisibleRoot", sValue
            End If
        End If
    End If

    If m_Aux.Grp.ID <> 0 Then
        If m_Aux.Grp.IsStockOrIndex Then
            aStorage.SetLongValue "Quote" & sKey, "IsAmerican", IIf(m_Aux.Grp.Und.IsAmerican, 1, 0)
        Else
            aStorage.SetLongValue "Quote" & sKey, "IsAmerican", IIf(m_Aux.Grp.Fut.IsAmerican, 1, 0)
        End If
    End If

    If QV.VisibleUndExch.Count < m_Aux.Grp.UndExchAll.Count Then
        sValue = ""
        For Each aExch In m_Aux.Grp.UndExchAll
            If aExch.Visible Then
                sValue = sValue & "|" & CLng(aExch.ID)
            End If
        Next
    
        If Len(sValue) > 0 Then
            sValue = Mid$(sValue, 2)
            aStorage.SetStringValue "Quote" & sKey, "VisibleUndExch", sValue
        End If
    End If

    If QV.VisibleOptExch.Count < m_Aux.Grp.OptExchAll.Count Then
        sValue = ""
        For Each aExch In m_Aux.Grp.OptExchAll
            If aExch.Visible Then
                sValue = sValue & "|" & CLng(aExch.ID)
            End If
        Next
    
        If Len(sValue) > 0 Then
            sValue = Mid$(sValue, 2)
            aStorage.SetStringValue "Quote" & sKey, "VisibleOptExch", sValue
        End If
    End If
    
    
    
    Dim i, j As Long
    Dim aFilter As EtsGeneralLib.EtsMmFilterAtomColl
    Dim aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    
    i = 0
    For Each aFilter In g_InVisibleOptRoots
        aStorage.SetStringValue "InvisibleOptRoots" & CStr(i), "Symbol", aFilter.Name
    
        j = 0
        For Each aFilterAtom In aFilter
            aStorage.SetLongValue "InvisibleOptRoots" & CStr(i), "ID" & CStr(j), aFilterAtom.ID
            aStorage.SetStringValue "InvisibleOptRoots" & CStr(i), "OptRootSymbol" & CStr(j), aFilterAtom.Name
            j = j + 1
        Next
        i = i + 1
    Next
                            
    Set aFilterAtom = Nothing
    Set aFilter = Nothing
    
    
    
    m_gdUnd.WriteToStorage "QuoteUndGrid" & sKey, aStorage
    m_gdDiv.WriteToStorage "QuoteDivGrid" & sKey, aStorage, False
    m_gdVol.WriteToStorage "QuoteVolGrid" & sKey, aStorage, False
    m_gdOpt.WriteToStorage "QuoteOptGrid" & sKey, aStorage
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    Dim i&, sSym$, aValues() As String, sValue$, aValues2() As String, sItemKey$
    Dim nValue&, nLBound&, nUBound&, nLBound2&, nUBound2&, dValue#
    Dim aContract As EtsGeneralLib.EtsContractAtom
    
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    LoadParams
    
    ' common info
    sSym = UCase$(Trim$(aStorage.GetStringValue("Quote" & sKey, "Symbol")))
    m_Aux.CalcModelVisible = aStorage.GetLongValue("Quote" & sKey, "ShowModel", True)
    m_Aux.ExpCalendarVisible = aStorage.GetLongValue("Quote" & sKey, "ShowCalendar", True)
    m_Aux.DividendsVisible = aStorage.GetLongValue("Quote" & sKey, "ShowDivs", True)
    m_Aux.VolaVisible = aStorage.GetLongValue("Quote" & sKey, "ShowVola", True)
    m_Aux.RatesVisible = aStorage.GetLongValue("Quote" & sKey, "ShowRates", True)
'    m_nCurrentExpiryIdx = aStorage.GetLongValue("Quote" & sKey, "CurrentExpiryIdx")
'    m_nCurrentOptExchIdx = aStorage.GetLongValue("Quote" & sKey, "CurrentOptExchIdx")
    QV.TradeFilter.Data(enFtTrades) = aStorage.GetLongValue("Quote" & sKey, "TradesFilter", 0)
    QV.TradeFilter.Data(enFtStrategy) = aStorage.GetLongValue("Quote" & sKey, "TradesStrategy", 0)
    QV.TradeFilter.Data(enFtTrader) = aStorage.GetLongValue("Quote" & sKey, "TradesTrader", 0)
    m_Aux.IsVolaSimulated = aStorage.GetLongValue("Quote" & sKey, "IsVolaSimulated", 0)
    
    QV.VisibleExp.Clear
    sValue = aStorage.GetStringValue("Quote" & sKey, "VisibleExp")
    If Len(sValue) > 0 Then
        On Error Resume Next
        
        aValues = Split(sValue, "|")
        nLBound = 0
        nUBound = -1
        nLBound = LBound(aValues)
        nUBound = UBound(aValues)
        For i = nLBound To nUBound
            Err.Clear
            nValue = 0
            nValue = CLng(aValues(i))
            If Err.Number <> 0 Then nValue = val(aValues(i))
            If nValue <> 0 Then
                sItemKey = Trim$(Str$(nValue))
                If QV.VisibleExp(sItemKey) Is Nothing Then QV.VisibleExp.Add(sItemKey).ID = nValue
            End If
        Next
        
        On Error GoTo EH
    End If
    Erase aValues
    
    QV.VisibleStr.Clear
    sValue = aStorage.GetStringValue("Quote" & sKey, "VisibleStr")
    If Len(sValue) > 0 Then
        On Error Resume Next
        
        aValues = Split(sValue, "|")
        nLBound = 0
        nUBound = -1
        nLBound = LBound(aValues)
        nUBound = UBound(aValues)
        For i = nLBound To nUBound
            Err.Clear
            dValue = 0#
            dValue = CDbl(aValues(i))
            If Err.Number <> 0 Then dValue = val(aValues(i))
            If dValue > 0# Then
                sItemKey = Trim$(Str$(dValue))
                If QV.VisibleStr(sItemKey) Is Nothing Then QV.VisibleStr.Add(sItemKey).Data3 = dValue
            End If
        Next
        
        On Error GoTo EH
    End If
    Erase aValues
    
    QV.VisibleRoot.Clear
    sValue = aStorage.GetStringValue("Quote" & sKey, "VisibleRoot")
    If Len(sValue) > 0 Then
        On Error Resume Next
        
        aValues = Split(sValue, "|")
        nLBound = 0
        nUBound = -1
        nLBound = LBound(aValues)
        nUBound = UBound(aValues)
        For i = nLBound To nUBound
            sValue = UCase$(Trim$(aValues(i)))
            If sValue <> "" Then
                If QV.VisibleRoot(sValue) Is Nothing Then QV.VisibleRoot.Add sValue
            End If
        Next
        
        On Error GoTo EH
    End If
    Erase aValues
    
    QV.VisibleUndExch.Clear
    sValue = aStorage.GetStringValue("Quote" & sKey, "VisibleUndExch")
    If Len(sValue) > 0 Then
        On Error Resume Next
        
        aValues = Split(sValue, "|")
        nLBound = 0
        nUBound = -1
        nLBound = LBound(aValues)
        nUBound = UBound(aValues)
        For i = nLBound To nUBound
            Err.Clear
            nValue = 0
            nValue = CLng(aValues(i))
            If Err.Number <> 0 Then nValue = val(aValues(i))
            If Err.Number = 0 Then
                sItemKey = Trim$(Str$(nValue))
                If QV.VisibleUndExch(sItemKey) Is Nothing Then QV.VisibleUndExch.Add(sItemKey).ID = nValue
            End If
        Next
        
        On Error GoTo EH
    End If
    Erase aValues
    
    QV.VisibleOptExch.Clear
    sValue = aStorage.GetStringValue("Quote" & sKey, "VisibleOptExch")
    If Len(sValue) > 0 Then
        On Error Resume Next
        
        aValues = Split(sValue, "|")
        nLBound = 0
        nUBound = -1
        nLBound = LBound(aValues)
        nUBound = UBound(aValues)
        For i = nLBound To nUBound
            Err.Clear
            nValue = 0
            nValue = CLng(aValues(i))
            If Err.Number <> 0 Then nValue = val(aValues(i))
            If Err.Number = 0 Then
                sItemKey = Trim$(Str$(nValue))
                If QV.VisibleOptExch(sItemKey) Is Nothing Then QV.VisibleOptExch.Add(sItemKey).ID = nValue
            End If
        Next
        
        On Error GoTo EH
    End If
    Erase aValues
    
    m_Aux.UseCustRates = aStorage.GetLongValue("Quote" & sKey, "UseCustRates", False)
    QV.CustRates.Clear
    sValue = aStorage.GetStringValue("Quote" & sKey, "CustRates")
    If Len(sValue) > 0 Then
        On Error Resume Next
        
        aValues = Split(sValue, "|")
        nLBound = 0
        nUBound = -1
        nLBound = LBound(aValues)
        nUBound = UBound(aValues)
        For i = nLBound To nUBound
            aValues2 = Split(aValues(i), ",")
            nLBound2 = 0
            nUBound2 = -1
            nLBound2 = LBound(aValues2)
            nUBound2 = UBound(aValues2)
            If nUBound2 - nLBound2 = 1 Then
                nValue = ReadLng(aValues2(nLBound2))
                dValue = ReadDbl(aValues2(nUBound2))
                If nValue > 0 And dValue > 0# Then
                    If QV.CustRates(CStr(nValue)) Is Nothing Then QV.CustRates.Add(CStr(nValue)).Data3 = dValue
                End If
            End If
            Erase aValues2
        Next
        
        On Error GoTo EH
    End If
    Erase aValues
    
    
    
    
    Dim nOptRootID As Long
    Dim nUndIdx, nOptRootIdx As Long
    Dim sSymbol, sOptRootSymbol As String
    Dim aFilter As EtsGeneralLib.EtsMmFilterAtomColl
    Dim aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    
    Set g_InVisibleOptRoots = Nothing
    Set g_InVisibleOptRoots = New Collection
    
    nUndIdx = 0
    While nUndIdx > IDX_NONE
        sSymbol = aStorage.GetStringValue("InvisibleOptRoots" & CStr(nUndIdx), "Symbol", STR_NA)
        
        If sSymbol = STR_NA Then
            nUndIdx = IDX_NONE
        Else
            Set aFilter = New EtsGeneralLib.EtsMmFilterAtomColl
            aFilter.Name = sSymbol
            g_InVisibleOptRoots.Add aFilter, sSymbol
        
            nOptRootIdx = 0
            While nOptRootIdx > IDX_NONE
                nOptRootID = aStorage.GetLongValue("InvisibleOptRoots" & CStr(nUndIdx), "ID" & CStr(nOptRootIdx), IDX_NONE)
                sOptRootSymbol = aStorage.GetStringValue("InvisibleOptRoots" & CStr(nUndIdx), "OptRootSymbol" & CStr(nOptRootIdx), STR_NA)
                
                If nOptRootID = IDX_NONE Then
                    nOptRootIdx = IDX_NONE
                Else
                    Set aFilterAtom = New EtsGeneralLib.EtsMmFilterAtom
                    aFilterAtom.ID = nOptRootID
                    aFilterAtom.Name = sOptRootSymbol
                    
                    aFilter.Add sOptRootSymbol, aFilterAtom
                
                    nOptRootIdx = nOptRootIdx + 1
                    Set aFilterAtom = Nothing
                End If
            Wend
            
            nUndIdx = nUndIdx + 1
            Set aFilter = Nothing
        End If
    Wend
    
    
    
    m_Aux.IsAmerican = aStorage.GetLongValue("Quote" & sKey, "IsAmerican", -1)
    
    m_gdUnd.ReadFromStorage "QuoteUndGrid" & sKey, aStorage
    m_gdDiv.ReadFromStorage "QuoteDivGrid" & sKey, aStorage, False
    m_gdVol.ReadFromStorage "QuoteVolGrid" & sKey, aStorage, False
    m_gdOpt.ReadFromStorage "QuoteOptGrid" & sKey, aStorage
    
    m_nNewGrpID = 0
    
    If Len(sSym) > 0 Then
        Set aContract = g_Contract.BySortKey(sSym)
        If Not aContract Is Nothing Then
            m_nNewGrpID = aContract.ID
            Set aContract = Nothing
        End If
    End If
    
    tmrShow.Enabled = True
    
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = Err.Description
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source

    Erase aValues
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

Private Sub Recalculate(ByVal bSymbol As Boolean, Optional ManualEdit As Boolean = True, _
                                                  Optional ByVal bForceRecalc As Boolean = False)
    On Error Resume Next

    If m_Aux.Grp.ID = 0 Or m_bInProc Or m_bShutDown Then Exit Sub
    
    If Not m_Aux.RealTime Then
        Screen.MousePointer = vbHourglass
        m_bInProc = True
        
        QV.Grp.Und.SetDirty
        CalculateUnderlyingOptions True, , ManualEdit, bForceRecalc
        m_AuxOut.OptionsUpdate bSymbol, True, True
        m_AuxOut.UnderlyingUpdate bSymbol, True, ManualEdit
        m_AuxOut.UnderlyingUpdateTotals
        m_AuxOut.DivsUpdate
        UpdateTotals
        m_AuxOut.VolaUpdateValues ManualEdit
    
        m_bInProc = False
        Screen.MousePointer = vbDefault
    Else
        QV.Grp.Und.SetDirty
        tmrRealTime.Enabled = True
    End If
End Sub

Public Sub Refresh()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    StoreVisibles
    
    m_nNewGrpID = m_Aux.Grp.ID
    m_bInRefreshMode = True


    If m_nNewGrpID <> 0 Then tmrShow.Enabled = True
End Sub

Private Sub StoreVisibles()
    On Error Resume Next
    Dim nValue&, sKey$, dValue#, aExp As EtsMmQuotesLib.MmQvExpAtom, aExch As EtsGeneralLib.ExchAtom
    Dim aRoot As EtsMmQuotesLib.MmQvOptRootAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom
    
    QV.VisibleExp.Clear
    QV.CustRates.Clear
    QV.VisibleStr.Clear
    QV.VisibleRoot.Clear
    QV.VisibleUndExch.Clear
    QV.VisibleOptExch.Clear
    
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    m_Aux.UseCustRates = m_Aux.Grp.UseCustRates
    
    If m_Aux.Grp.IsStockOrIndex Then
        m_Aux.IsAmerican = IIf(m_Aux.Grp.Und.IsAmerican, 1, 0)
    Else
        m_Aux.IsAmerican = IIf(m_Aux.Grp.Fut.IsAmerican, 1, 0)
    End If
    
    For Each aExp In m_Aux.Grp.ExpAll
        nValue = CLng(aExp.ExpiryMonth)
        If aExp.Visible Then
            sKey = Trim$(Str$(nValue))
            If QV.VisibleExp(sKey) Is Nothing Then QV.VisibleExp.Add(sKey).ID = nValue
        End If
        dValue = aExp.RateCust
        If QV.CustRates(CStr(nValue)) Is Nothing Then QV.CustRates.Add(CStr(nValue)).Data3 = dValue
    Next
    
    For Each aStr In m_Aux.Grp.StrikeAll
        If aStr.Visible Then
            sKey = Trim$(Str$(aStr.Strike))
            If QV.VisibleStr(sKey) Is Nothing Then QV.VisibleStr.Add(sKey).Data3 = aStr.Strike
        End If
    Next
    
    If m_Aux.Grp.IsStockOrIndex Then
        For Each aRoot In m_Aux.Grp.Und.OptRoot
            If aRoot.Visible Then
                If QV.VisibleRoot(aRoot.Name) Is Nothing Then QV.VisibleRoot.Add aRoot.Name
            End If
        Next
    End If
    
    For Each aExch In m_Aux.Grp.UndExchAll
        If aExch.Visible Then
            nValue = aExch.ID
            sKey = Trim$(Str$(nValue))
            If QV.VisibleUndExch(sKey) Is Nothing Then QV.VisibleUndExch.Add(sKey).ID = nValue
        End If
    Next
    
    For Each aExch In m_Aux.Grp.OptExchAll
        If aExch.Visible Then
            nValue = aExch.ID
            sKey = Trim$(Str$(nValue))
            If QV.VisibleOptExch(sKey) Is Nothing Then QV.VisibleOptExch.Add(sKey).ID = nValue
        End If
    Next
End Sub

Public Property Get CalcModelVisible() As Boolean
    On Error Resume Next
    CalcModelVisible = m_Aux.CalcModelVisible
End Property

Public Property Let CalcModelVisible(ByVal bVisible As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Property
    m_Aux.CalcModelVisible = bVisible
    m_Aux.AdjustDivsAndRatesVisible
    UserControl_Resize
    AdjustState
End Property

Public Property Get DividendsVisible() As Boolean
    On Error Resume Next
    DividendsVisible = m_Aux.DividendsVisible
End Property

Public Property Let DividendsVisible(ByVal bVisible As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Property
    m_Aux.DividendsVisible = bVisible
    m_Aux.AdjustDivsAndRatesVisible
    UserControl_Resize
    AdjustState
End Property

Public Property Get VolaVisible() As Boolean
    On Error Resume Next
    VolaVisible = m_Aux.VolaVisible
End Property

Public Property Let VolaVisible(ByVal bVisible As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Property
    m_Aux.VolaVisible = bVisible
    m_Aux.AdjustVolaVisible
    UserControl_Resize
    AdjustState
End Property

Public Property Get RatesVisible() As Boolean
    On Error Resume Next
    RatesVisible = m_Aux.RatesVisible
End Property

Public Property Let RatesVisible(ByVal bVisible As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Property
    m_Aux.RatesVisible = bVisible
    m_Aux.AdjustDivsAndRatesVisible
    UserControl_Resize
    AdjustState
End Property

Public Property Get ExpCalendarVisible() As Boolean
    On Error Resume Next
    ExpCalendarVisible = m_Aux.ExpCalendarVisible
End Property

Public Property Let ExpCalendarVisible(ByVal bVisible As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Property
    m_Aux.ExpCalendarVisible = bVisible
    m_Aux.AdjustDivsAndRatesVisible
    UserControl_Resize
    AdjustState
End Property

Private Sub VolaSource_VolatilityChanged(ByVal Symbol As String)
    On Error Resume Next
    If m_bShutDown Or m_bSubscribingNow Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    If Symbol = m_Aux.Grp.Und.Symbol Then
        If Not m_bSubscribingNow Then
            Recalculate False
        Else
            m_bVolaChangedExt = True
        End If
    End If
End Sub

Public Sub InternalVolatilityChanged(ByVal sSymbol As String)
    On Error Resume Next
    If m_bShutDown Or m_bVolaChangedNow Then Exit Sub
    VolaSource_VolatilityChanged sSymbol
End Sub

Private Sub AdjustState()
    On Error Resume Next
    
    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
    If m_Aux.Grp.IsStockOrIndex Then
         Set aExpColl = m_Aux.Grp.Und.Exp
    Else
         Set aExpColl = m_Aux.Grp.Fut.Exp
     End If
    
    fgVol.ColHidden(QVC_VOLA_VAL + aExpColl.Count) = m_Aux.RealTime
    If m_Aux.RealTime Then
        fgVol.TextMatrix(1, QVC_VOLA_VAL + aExpColl.Count) = Now
        dtCalculationDate.Value = Now
    End If
   
    UpdateMenu
    RaiseEvent OnStateChange
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    If Not m_bShutDown And Not m_bUndListReloadNow Then InitUndList
End Sub

Public Sub OptExpirySeparationChange()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp.ID = 0 Then Exit Sub
    
    Dim nColKey&
    
    If m_AuxOut.ExpiryColorSelection And Not g_Params.QuoteOptExpirySeparation Then
        m_AuxOut.OptionsUpdateBackColor False
    
    ElseIf Not m_AuxOut.ExpiryColorSelection And g_Params.QuoteOptExpirySeparation Then
        nColKey = geOpt.SortColKey
        If (nColKey = QOC_C_SERIES Or nColKey = QOC_P_SERIES Or nColKey = QOC_C_EXPIRY Or nColKey = QOC_P_EXPIRY) Then
            m_AuxOut.OptionsUpdateBackColor True
        End If
    End If
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    If m_bShutDown Then Exit Sub
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Aux.Grp.Symbol & " Quotes", "Quotes", fgUnd, fgDiv, fgVol, fgOpt
'    Screen.MousePointer = vbDefault
End Sub

Private Sub InitVola(ByRef aUnd As EtsMmQuotesLib.MmQvUndAtom)
'    On Error GoTo EH
    On Error Resume Next
    Dim nLoopCounter As Long: nLoopCounter = 1
    
    Err.Clear
    aUnd.VolaSrv.Init aUnd.Symbol, aUnd.UndType, VolaSource, m_Aux.IsVolaSimulated
    
    While Err.Number <> 0 And nLoopCounter < 10
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogFaults, "Fail to init vola in " & CStr(nLoopCounter) & " attempt for symbol '" & aUnd.Symbol & "'.", m_frmOwner.GetCaption

        MMSleep 20
        Err.Clear
        aUnd.VolaSrv.Init aUnd.Symbol, aUnd.UndType, VolaSource, m_Aux.IsVolaSimulated

        nLoopCounter = nLoopCounter + 1
    Wend
    
    If Err.Number <> 0 Then GoTo EH
    
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
End Sub

Private Sub HandleGridDblClick(ByVal bTradeNewAvailable As Boolean)
    On Error Resume Next
    m_bIsDblClick = True
    
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
        
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
        ElseIf m_enMenuGrid = GT_QUOTES_DIVIDENDS And m_nMenuGridRow = 1 And (m_nMenuGridCol = QDC_DATE Or m_nMenuGridCol = QDC_AMT) Then
            If m_Aux.Grp.Und.Dividend.DivType = enDivCustomStream Then mnuCtxCustomDividend_Click
        End If
    End If
    
    m_bIsDblClick = False
End Sub

Public Sub MakeDataSnapshot(ByVal sFileName$)
    On Error Resume Next
'    m_Aux.MakeDataSnapshot sFileName, m_frmOwner
End Sub

Public Sub OTCOptionCalcCall()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim aRowData As MmQvRowData, nColIdx&, bBuy As Boolean, aOpt As EtsMmQuotesLib.MmQvOptAtom, aUnd As EtsMmQuotesLib.MmQvUndAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom
    Dim dtDate As Date
    Dim lCalcModel As Long
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom

    
    Dim sStockSymbol As String
    Dim dVola As Double
    Dim sOptionSymbol As String
    
    Dim lSymbolType As Long
    
    Dim dStrike As Double
    Dim dBid As Double
    Dim dAsk As Double
    Dim dtExpiry As Date
    Dim dtExpiryOV As Date
    
    Dim lContractType As Long
    Dim dYield As Double
    Dim lDivType As Long
    
    Dim dDivAmount As Double
    
    Dim dRate As Double
    Dim dHTBRate As Double: dHTBRate = BAD_DOUBLE_VALUE
    
    Dim dDivDate As Double
    Dim dDivFreq As Double
    Dim lCDStockID
    Dim lCDCount
    
    lCDStockID = 0
    lCDCount = 0
    dVola = -1
    
    lContractType = m_Aux.Grp.ContractType
    
    If (Not m_Aux.Grp.Fut Is Nothing) Then
        If (m_Aux.Grp.Fut.ID > 0) Then
            lSymbolType = 0
            If Not m_Aux.Grp.Fut.IsAmerican Then
                lSymbolType = 1
            End If
        End If
    End If
    
    If (Not m_Aux.Grp.Und Is Nothing) Then

       sStockSymbol = m_Aux.Grp.Und.Symbol
       sOptionSymbol = m_Aux.Grp.Und.Symbol
       
       
       Set aDiv = m_Aux.Grp.Und.Dividend
       If Not aDiv Is Nothing Then
            If aDiv.DivType = enDivCustomPeriodical Then
              dDivAmount = aDiv.DivAmtCust
              dDivDate = aDiv.DivDateCust
              dDivFreq = aDiv.DivFreqCust
            End If
           
            If aDiv.DivType = enDivCustomPeriodical Then
              dDivAmount = aDiv.DivAmt
              dDivDate = aDiv.DivDate
              dDivFreq = aDiv.DivFreq
            End If
       End If
       Set aDiv = Nothing
       
       lSymbolType = 0
       If Not m_Aux.Grp.Und.IsAmerican Then
           lSymbolType = 1
       End If
       dYield = m_Aux.Grp.Und.Yield
       lContractType = m_Aux.Grp.ContractType
       
        If Not m_Aux.Grp.Fut Is Nothing Then
            If Len(m_Aux.Grp.Fut.Symbol) > 0 Then
               sStockSymbol = m_Aux.Grp.Fut.Symbol
               sOptionSymbol = m_Aux.Grp.Fut.Symbol
               lContractType = 4
            End If
        End If
    
    End If
   
            Set aRowData = QV.OptsRowData(m_nMenuGridRow)
            nColIdx = fgOpt.ColKey(m_nMenuGridCol)
            If aRowData Is Nothing Then
                Set aRowData = QV.OptsRowData(fgOpt.Row)
            End If
            If Not aRowData Is Nothing Then
                Set aOpt = aRowData.Opt(IIf(nColIdx < QOC_P_SYMBOL, enOtCall, enOtPut))
                If Not aOpt Is Nothing Then
                        sOptionSymbol = aOpt.Symbol
                        
                        dStrike = aOpt.Strike
                        dtExpiry = aOpt.Expiry
                        dtExpiryOV = aOpt.ExpiryOV
                        dVola = aOpt.Vola
                        
                        If m_enMenuGrid = GT_QUOTES_UNDERLYING Then
                            dVola = 0
                            dStrike = 0
                        End If
                        Set aExp = m_Aux.Grp.Und.Exp(dtExpiry)
                        
                        'dtDate = CDate(CDbl(dtExpiry) - CDbl(Day(dtExpiry)))
                        For Each aExp In m_Aux.Grp.Und.Exp
                            If aExp.ExpiryMonth >= dtExpiry Then
                                dRate = aExp.Rate
                                If (aExp.RateCust > 0) And (aExp.Rate = 0) Then
                                   dRate = aExp.RateCust
                                End If
                                dHTBRate = aExp.HTBRate
                                
                                Exit For
                            End If
                        Next
                        'lContractType = 3
                        
                        If Not aOpt.DefQuote Is Nothing Then
                            dBid = 0
                            dAsk = 0
                            If aOpt.DefQuote.PriceBid <> BAD_DOUBLE_VALUE Then
                                dBid = aOpt.DefQuote.PriceBid
                                                                    Else
                                                                    
                                If aOpt.DefQuote.PriceLast <> BAD_DOUBLE_VALUE Then
                                    dBid = aOpt.DefQuote.PriceLast
                                End If
                            
                            End If
                            
                            If aOpt.DefQuote.PriceAsk <> BAD_DOUBLE_VALUE Then
                                dAsk = aOpt.DefQuote.PriceAsk
                                                                    Else
                                                                    
                                If aOpt.DefQuote.PriceLast <> BAD_DOUBLE_VALUE Then
                                    dAsk = aOpt.DefQuote.PriceLast
                                End If
                            
                            End If
                        End If
                    Set aOpt = Nothing
                End If
                Set aRowData = Nothing
            End If
        
    'End Select
    lCalcModel = CLng(g_Params.CalcModel)
    

    m_bInProc = False
    On Error GoTo EH
    Dim sPath As String
    Dim sParams As String
    sPath = App.Path & "\OTCCalc\OTCOptionCalc.exe"
    sParams = sStockSymbol & " " & sOptionSymbol & " " & CStr(lSymbolType) & " " & CStr(dStrike) & " " & CStr(dBid) & " " & CStr(dAsk) & " " & _
    """" & CStr(dtExpiryOV) & """" & " " & _
    CStr(lContractType) & " " & CStr(dYield * 100) & " " & CStr(lDivType) & " " & _
    CStr(dDivAmount) & " " & CStr(dDivDate) & " " & CStr(dDivFreq) & " " & _
    CStr(lCDStockID) & " " & CStr(lCDCount) & " " & CStr(dRate) & " " & CStr(dVola) & " " & CStr(lCalcModel) & " " & CStr(dHTBRate)

    If ShellExecute(0&, "Open", sPath, sParams, "", SW_SHOWNORMAL) <= 32 Then
         gCmn.MyMsgBox Me, "Fail to open OTC OptionCalc at '" & sPath & "'.", vbCritical
    Else
           If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "OTCOptionCalc start [" & sParams & "]", "OTC Calc"
    End If
    
   Exit Sub
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to open OTC Option Calc."
    ResetMenuData
    
End Sub

Public Sub ctlStopRT()
    On Error Resume Next
    If m_bShutDown Or m_bSubscribingNow Then Exit Sub
    If m_Aux.RealTime Then
        StopRealTime
    End If
    UpdateTotals
End Sub

Public Function GetStockInfo() As String
    On Error Resume Next
    Dim sStock As String
    sStock = fgUnd.RowData(m_nUndMainRow).Und.Symbol
    GetStockInfo = "Stock symbol = """ & sStock & """. "
End Function

Public Function GetOptionInfo() As String
    On Error Resume Next
    Dim sOptionC As String
    Dim sOptionP As String
    sOptionC = QV.OptsRowData(fgOpt.Row).Opt(enOtCall).Symbol
    sOptionP = QV.OptsRowData(fgOpt.Row).Opt(enOtPut).Symbol
    GetOptionInfo = GetStockInfo & "Option symbol = """ & sOptionC & "/" & sOptionP & """. "
End Function

Private Function HandleSimulatedVolatility(ByVal Row As Long, ByVal Col As Long) As Boolean
    On Error Resume Next
    
    HandleSimulatedVolatility = False
    
    Dim nColData As Long: nColData = fgVol.ColData(Col)
    If nColData = QVC_VOLA_SIM Then
        
        HandleSimulatedVolatility = True
        
        If fgVol.TextMatrix(Row, Col) = False Then
            If SaveSimulatedVolatility(True) Then
                DoEvents
                
                Screen.MousePointer = vbHourglass
                
                If g_Params.UseTheoVolatility Then
                    m_bInProc = True
                    CalculateUnderlyingOptions True, , , True
                    m_AuxOut.UnderlyingUpdateTotals
                    UpdateTotals
                    m_bInProc = False
                End If
        
                OptionsUpdateVola
                m_AuxOut.OptionsUpdate False, True, True
                m_AuxOut.VolaUpdateValues
                
                Screen.MousePointer = vbDefault
                
            Else
                fgVol.TextMatrix(Row, Col) = Not fgVol.TextMatrix(Row, Col)
            End If
        Else
            m_Aux.IsSimulatedFlat = False
            m_Aux.Grp.Und.VolaSrv.Init m_Aux.Grp.Und.Symbol, m_Aux.Grp.Und.UndType, VolaSource, Not m_Aux.Grp.Und.VolaSrv.SimulatedVol
            DoEvents
            m_AuxOut.VolaUpdateValues
        End If
    
        m_Aux.IsVolaSimulated = m_Aux.Grp.Und.VolaSrv.SimulatedVol
        
        Exit Function
    End If
    
    If nColData = QVC_VOLA_FLAT Then
        m_Aux.IsSimulatedFlat = fgVol.TextMatrix(Row, Col)
        
        If fgVol.TextMatrix(Row, Col) = True Then
            mnuCtxVolaFlatAll_Click
        End If
        
        HandleSimulatedVolatility = True
        Exit Function
    End If
    
End Function

Public Function SaveSimulatedVolatility(Optional ByVal bInit As Boolean = False) As Boolean
    On Error Resume Next
    
    SaveSimulatedVolatility = True
    
    If m_Aux.Grp.ID = 0 Or Not m_Aux.IsVolaSimulated Then Exit Function
    
    Dim nMsgResult As VbMsgBoxResult
    nMsgResult = vbNo
            
    If nMsgResult = vbCancel Or nMsgResult = 0 Then
        SaveSimulatedVolatility = False
        Exit Function
    End If
            
    If nMsgResult = vbNo Then
        If bInit Then _
            m_Aux.Grp.Und.VolaSrv.Init m_Aux.Grp.Und.Symbol, m_Aux.Grp.Und.UndType, VolaSource, Not m_Aux.Grp.Und.VolaSrv.SimulatedVol
    End If
            
    If nMsgResult = vbYes Then
        Screen.MousePointer = vbHourglass
        m_Aux.Grp.Und.VolaSrv.SaveSimulatedVol
        If bInit Then _
            m_Aux.Grp.Und.VolaSrv.Init m_Aux.Grp.Und.Symbol, m_Aux.Grp.Und.UndType, VolaSource, Not m_Aux.Grp.Und.VolaSrv.SimulatedVol
        Screen.MousePointer = vbDefault
    End If
    
End Function

Private Sub LoadParams()
    On Error Resume Next
    
    m_Aux.IsVolaSimulated = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "SimulatedVola", 0) <> 0)
End Sub

Private Sub SaveParams()
    On Error Resume Next
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "SimulatedVola", IIf(m_Aux.IsVolaSimulated, 1, 0)
End Sub

'Private Sub CheckTradeQueue()
'    ' TradeQueue manage
'    Dim TrdTmp As clsTradeQueueAtom
'    For Each TrdTmp In TradeQueue
'        If Not IsNull(TrdTmp) Then
'            DoTradeAction TrdTmp
'            Set TrdTmp = Nothing
'        End If
'    Next
'
'    TradeQueue.Clear
'End Sub

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

Private Function LoadCustomDivs() As Boolean
    On Error Resume Next
    Dim expDate As Date
    Dim aExp As MmQvExpAtom
    
    LoadCustomDivs = False
    
    expDate = Now + 365 * 5
    For Each aExp In m_Aux.Grp.Und.Exp
        expDate = aExp.Expiry
    Next
        
    m_frmCustDivs.Init m_Aux.Grp.Und.ID, m_Aux.Grp.Und.Symbol, expDate, m_Aux.Grp.Und.Dividend.DivType = enDivStockBasket, m_Aux.Grp.Und.Dividend.DivType = enDivCustomStream
    m_frmCustDivs.BasketDivs = m_Aux.Grp.Und.BasketIndex.BasketDivs
            
    If m_frmCustDivs.UpdateCollection Then
        Set m_Aux.Grp.Und.Dividend.CustomDivs = m_frmCustDivs.CustomDividend
        LoadCustomDivs = True
    End If
End Function

Public Sub RefreshPrices()
    On Error Resume Next
    If m_bShutDown Or m_Aux.RealTime Then Exit Sub
    If m_Aux.Grp.ID = 0 Then Exit Sub
    
    m_bFiersTealtimeCalculation = False
    RequestLastQuotes
    
End Sub

Public Function ExportScreen(ByVal sFileName As String, ByVal sFilePath As String, _
                             Optional ByVal sHeader As String = "", _
                             Optional ByVal FileSaveType As FileSaveTypeEnum = SE_ADDTOEND) As Boolean
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportScreen = g_ScreenExport.Save(sFileName, sFilePath, fgOpt, , , sHeader, FileSaveType)
    Screen.MousePointer = vbNormal
End Function
