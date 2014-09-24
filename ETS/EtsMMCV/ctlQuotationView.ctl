VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.UserControl ctlQuotationView 
   ClientHeight    =   7650
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9750
   KeyPreview      =   -1  'True
   ScaleHeight     =   7650
   ScaleWidth      =   9750
   Begin MSComctlLib.ImageList ilEvent 
      Left            =   5160
      Top             =   6480
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
            Picture         =   "ctlQuotationView.ctx":0000
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlQuotationView.ctx":015A
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlQuotationView.ctx":02B4
            Key             =   ""
         EndProperty
      EndProperty
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
      Top             =   960
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
   Begin VSFlex7Ctl.VSFlexGrid fgLog 
      Height          =   2055
      Left            =   0
      TabIndex        =   3
      Top             =   3840
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
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator2 
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
      Begin VB.Menu mnuCtxCopy 
         Caption         =   "Copy Grid"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuCtxPrint 
         Caption         =   "Print Grid..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxDefOptSort 
         Caption         =   "Default Options Sorting"
         Shortcut        =   ^D
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
Attribute VB_Name = "ctlQuotationView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Private Const QN_MAX_OPTIONS As Long = 300
'Private Const QN_MAX_STOCKS As Long = 600
'
'Public Event OnSetCaption()
'Public Event OnStateChange()
'Public Event OnExpiriesChange(ByVal bStateOnly As Boolean)
'Public Event OnUndExchangesChange(ByVal bStateOnly As Boolean)
'
'Private m_gdUnd As New clsGridDef
'Private m_gdDiv As New clsGridDef
'Private m_gdOpt As New clsGridDef
'Private m_gdLog As New clsGridDef
'
'Private WithEvents geOpt As clsGridEx
'
'Private m_NewUnd As New clsEntityAtom
'
'Private WithEvents PriceProvider As ISEPROVIDERLib.ISEPriceProvider
'Private m_GroupPriceProvider As ISEPROVIDERLib.IGroupPrice
'Private WithEvents VolaSource As VolatilitySourcesLib.VolatilitySource
'Private WithEvents TradeChannel As clsTradeChannel
'Private WithEvents MmManager As ISEPROVIDERLib.ISEMMManager
'Private WithEvents MsProvider As ISEPROVIDERLib.ISEStructureProvider
'Private WithEvents MmTracer As ISEPROVIDERLib.ISETracer
'Private MmParams As New ISEPROVIDERLib.MMParameters
'
'Private WithEvents frmLayout As frmGridLayout
'
'Public pbProgress As MSComctlLib.ProgressBar
'Public lblProcess As VB.Label
'Public lblStatus As VB.Label
'Public lblValue As VB.Label
'Public WithEvents imgStop As VB.Image
'Public imgStopDis As VB.Image
'
'Private m_sUndComboList As String
'
'Private m_UndList As New clsQnUndColl
'Private m_bUndListGot As Boolean
'
'Private m_Exch As EtsGeneralLib.ExchColl
'Private m_Und As clsQnUndAtom
'Private m_QuoteRequests As New clsQnReqColl
'Private m_UndExch As New EtsGeneralLib.ExchColl
'Private m_OptExch As New EtsGeneralLib.ExchColl
'
'Private m_bUseCustRates As Boolean
'Private m_nIsAmerican As Long
'Private m_CustRates As New clsEntityColl
'Private m_VisibleExp As New clsFilterAtomColl
'Private m_VisibleUndExch As New clsFilterAtomColl
'Private m_VisibleOptExch As New clsFilterAtomColl
'
'Private m_nOptPosExchID As Long
'
'Private m_InOpt As clsQnOptColl
'
'Private Enum QnStateEnum
'    enQsReady = 0
'    enQsStockListReq
'    enQsLastQuoteReq
'    enQsLastMmQuoteReq
'    enQsOptionsReq
'    enQsRealtime
'    enQsDataLoad
'End Enum
'
'Private m_bInProc As Boolean
'Private m_bIsConnected As Boolean
'Private m_enState As QnStateEnum
'
'Private m_nLastQuoteReqCount As Long
'Private m_nLastQuoteReqDone As Long
'Private m_nLastQuoteGroupReqCount As Long
'Private m_nLastQuoteGroupReqDone As Long
'Private m_nLastMmQuoteReqCount As Long
'Private m_nLastMmQuoteReqDone As Long
'
'Private m_nSleepStep As Long
'
'Private m_bVolaChangedNow As Boolean
'Public ProcessRealTime As Boolean
'
'Private m_nCurrentExpiryIdx As Long
''Private m_nCurrentOptExchIdx As Long
'
'Public m_frmOwner As Form
'
'Private m_sCurrentOriginalText As String
'Private m_bKeyDown(GT_QUOTATION_UNDERLYING To GT_QUOTATION_LOG) As Boolean
'Private m_GridLock(GT_QUOTATION_UNDERLYING To GT_QUOTATION_LOG) As New clsGridLock
'
'Private m_bCalcModelVisible As Boolean
'Private m_bExpCalendarVisible As Boolean
'Private m_bDividendsVisible As Boolean
'Private m_bRatesVisible As Boolean
'Private m_bLogVisible As Boolean
'
'Private Enum CalcIvEnum
'    CALC_IV_NONE = &H0&
'
'    CALC_IV = &H1&
'    CALC_IV_BID = &H2&
'    CALC_IV_ASK = &H4&
'
'    CALC_IV_ALL = &HFFFFFFFF
'End Enum
'
'Private m_enMenuGrid As GridTypeEnum
'Private m_nMenuGridCol As Long
'Private m_nMenuGridRow As Long
'Private m_nMenuGridCols As Long
'Private m_nMenuGridRows As Long
'
'Private m_iSeriesOrder As SortSettings
'Private m_nSortSeriesColKey As Long
'Private m_bGroupRequest As Boolean
'Private m_bShutDown As Boolean
'
'Public Function Init() As Boolean
'    On Error GoTo EH
'
'    m_bShutDown = False
'    m_bVolaChangedNow = False
'
'    mnuCtxTradeNew.Caption = "New Order..." & vbTab & "Ins"
'    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
'    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
'
'    m_bKeyDown(GT_QUOTATION_UNDERLYING) = False
'    m_bKeyDown(GT_QUOTATION_DIVIDENDS) = False
'    m_bKeyDown(GT_QUOTATION_OPTIONS) = False
'    m_bKeyDown(GT_QUOTATION_LOG) = False
'
'    m_GridLock(GT_QUOTATION_UNDERLYING).Init fgUnd
'    m_GridLock(GT_QUOTATION_DIVIDENDS).Init fgDiv
'    m_GridLock(GT_QUOTATION_OPTIONS).Init fgOpt
'    m_GridLock(GT_QUOTATION_LOG).Init fgLog
'
'    m_nSortSeriesColKey = -2
'    m_iSeriesOrder = flexSortGenericAscending
'
'    ResetMenuData
'
'    m_nSleepStep = 0
'    m_enState = enQsReady
'    m_bUseCustRates = False
'    m_nIsAmerican = -1
'
'    m_bCalcModelVisible = True
'    m_bExpCalendarVisible = True
'    m_bDividendsVisible = True
'    m_bRatesVisible = True
'    m_bLogVisible = True
'
'    Set m_Exch = Nothing
'    Set m_Und = Nothing
'    Set m_UndExch = Nothing
'    Set m_OptExch = Nothing
'    Set m_QuoteRequests = Nothing
'
'    Set m_Und = New clsQnUndAtom
'    Set m_Exch = New EtsGeneralLib.ExchColl
'    Set m_UndExch = New EtsGeneralLib.ExchColl
'    Set m_OptExch = New EtsGeneralLib.ExchColl
'
'    Set VolaSource = g_VolaSource
'    Set TradeChannel = g_TradeChannel
'
'    InitColumns
'    InitGrids
'
'    FormatLogGrid
'    FormatLogColumns False
'
'    Set geOpt = New clsGridEx
'    geOpt.Init fgOpt
'
'    ClearViewAndData
'    'InitUndList
'
'    LoadExchanges
'    fgUnd.Col = 1
'    fgUnd.Row = 1
'    Init = True
'    UpdateTotals
'    AdjustState
'
'    m_bIsConnected = False
'    m_bUndListGot = False
'    Set frmLayout = New frmGridLayout
'
'    Set PriceProvider = New ISEPROVIDERLib.ISEPriceProvider
'    Set m_GroupPriceProvider = PriceProvider
'    m_bGroupRequest = Not m_GroupPriceProvider Is Nothing
'
'    Set MsProvider = New ISEPROVIDERLib.ISEStructureProvider
'    Set MmManager = New ISEPROVIDERLib.ISEMMManager
'    Set MmTracer = New ISEPROVIDERLib.ISETracer
'    Set MmParams = New ISEPROVIDERLib.MMParameters
'
'    If g_MmManager Is Nothing Then
'        Set g_MmManager = New ISEPROVIDERLib.ISEMMManager
'        g_MmManager.Connect
'    End If
'
'    MmTracer.Bind
'
'    PriceProvider.Connect
'    MsProvider.Connect
'    MmManager.Connect
'
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to init quotation view."
'End Function
'
'Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
'    On Error Resume Next
'    If nType <> TYPE_UNDERLYING Or nValue = 0 Then Exit Sub
'    Dim aUnd As EtsGeneralLib.UndAtom
'
'    m_NewUnd.ID = 0
'    Set aUnd = g_UnderlyingAll(nValue)
'    If Not aUnd Is Nothing Then
'        m_NewUnd.ID = aUnd.ID
'        m_NewUnd.Data = aUnd.UndType
'        m_NewUnd.Data3 = IIf(aUnd.IsTraderContract, 1, 0)
'        Set aUnd = Nothing
'
'        m_VisibleUndExch.Clear
'        If Not g_Params.QuoteUndExchAll Then
'            m_VisibleUndExch.Add(CStr(g_Params.QuoteDefUndExchID)).ID = g_Params.QuoteDefUndExchID
'        End If
'
'        m_VisibleOptExch.Clear
'        If Not g_Params.QuoteOptExchAll Then
'            m_VisibleOptExch.Add(CStr(g_Params.QuoteDefOptExchID)).ID = g_Params.QuoteDefOptExchID
'        End If
'
'    End If
'
'    If m_NewUnd.ID <> 0 Then
'        tmrShow.Enabled = True
'    End If
'End Sub
'
'Private Sub InitUndList()
'    On Error Resume Next
'    Dim i&, nCount&, sValue$, aUnd As clsQnUndAtom
'    Dim nValue&, nCol&
'
'    With fgUnd
'        nCol = .ColIndex(UUC_SYMBOL)
'        If nCol > 0 And nCol < .Cols Then
'            m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'
'            sValue = "0"
'            nValue = 0
'            m_sUndComboList = ""
'            nCount = m_UndList.Count
'            For i = 1 To nCount
'                Set aUnd = m_UndList(i)
'                m_sUndComboList = m_sUndComboList & "|#" & Trim$(Str$(aUnd.ID)) & ";" & aUnd.Symbol
'                If aUnd.ID = m_Und.ID Then
'                    nValue = aUnd.ID
'                    sValue = aUnd.Symbol 'Trim$(Str$(aUnd.ID))
'                End If
'                Set aUnd = Nothing
'            Next
'
'            If Len(m_sUndComboList) > 0 Then
'                m_sUndComboList = Mid$(m_sUndComboList, 2)
'            Else
'                m_sUndComboList = "#0;<None>"
'            End If
'
'            If nValue <> 0 Then .TextMatrix(1, nCol) = sValue
'
'            m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'        End If
'    End With
'End Sub
'
'Private Sub AdjustOptPosExchIDs()
'    On Error Resume Next
'    Dim aExch As EtsGeneralLib.ExchAtom
'
'    m_nOptPosExchID = 0
'    For Each aExch In m_OptExch
'        If aExch.Visible Then
'            m_nOptPosExchID = aExch.ID
'            Exit For
'        End If
'    Next
'End Sub
'
'Private Sub UnderlyingOptionsShow()
'    On Error Resume Next
'    Dim RowData As clsQnQuoteRowData, nExchCount&, aExch As EtsGeneralLib.ExchAtom, sKey$
'    Dim nRow&, i&, k&, nExpCount&, nStrCount&, aExpiry As clsQnExpAtom, aStrike As clsQnStrikeAtom
'    Dim bAllExpVisible As Boolean, bAllExchVisible As Boolean, bShowOnlyDefExch As Boolean
'
'    If m_bInProc Then Exit Sub
'
'    m_bInProc = True
'    AdjustState
'    DoEvents
'
'    m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'    pbProgress.Visible = True
'
'    lblStatus.Visible = False
'    lblProcess.Visible = True
'    lblProcess.Caption = "Formatting..."
'    lblProcess.Refresh
'    imgStop.Visible = True
'    imgStopDis.Visible = False
'
'    With fgOpt
'        .Rows = 1
'        bAllExpVisible = True
'
'        nExpCount = m_Und.Expiry.Count
'        If m_VisibleExp.Count > 0 Then
'            For i = 1 To nExpCount
'                Set aExpiry = m_Und.Expiry(i)
'                If Not m_VisibleExp(CStr(CLng(aExpiry.ExpiryMonth))) Is Nothing Then
'                    Set aExpiry = Nothing
'                    bAllExpVisible = False
'                    Exit For
'                End If
'                Set aExpiry = Nothing
'            Next
'        End If
'
'        nExchCount = m_OptExch.Count
'
'        bAllExchVisible = True
'        bShowOnlyDefExch = False
'        If m_VisibleOptExch.Count > 0 Then
'            For Each aExch In m_OptExch
'                If Not m_VisibleOptExch(CStr(aExch.ID)) Is Nothing Then
'                    bAllExchVisible = False
'                    Exit For
'                End If
'            Next
'            bShowOnlyDefExch = bAllExchVisible
'        End If
'
'        For Each aExch In m_OptExch
'            If Not bAllExchVisible Then
'                aExch.Visible = Not m_VisibleOptExch(CStr(aExch.ID)) Is Nothing
'            Else
'                aExch.Visible = (Not bShowOnlyDefExch Or bShowOnlyDefExch And aExch.ID = 0)
'            End If
'        Next
'
'        AdjustOptPosExchIDs
'
'        For i = 1 To nExpCount
'            Set aExpiry = m_Und.Expiry(i)
'            If Not bAllExpVisible Then
'                aExpiry.Visible = Not m_VisibleExp(CStr(CLng(aExpiry.ExpiryMonth))) Is Nothing
'            Else
'                aExpiry.Visible = True
'            End If
'
'            nStrCount = aExpiry.Strike.Count
'            For k = 1 To nStrCount
'                Set aStrike = aExpiry.Strike(k)
'
'                For Each aExch In m_OptExch
'
'                    Set RowData = New clsQnQuoteRowData
'                    Set RowData.Exp = aExpiry
'                    Set RowData.Exch = aExch
'
'                    If aStrike.aOption(enOtCall).ID <> 0 Then
'                        Set RowData.Opt(enOtCall) = aStrike.aOption(enOtCall)
'                        Set RowData.OptQuote(enOtCall) = aStrike.aOption(enOtCall).Quotes(CStr(aExch.ID))
'                    End If
'
'                    If aStrike.aOption(enOtPut).ID <> 0 Then
'                        Set RowData.Opt(enOtPut) = aStrike.aOption(enOtPut)
'                        Set RowData.OptQuote(enOtPut) = aStrike.aOption(enOtPut).Quotes(CStr(aExch.ID))
'                    End If
'
'                    If Not RowData.OptQuote(enOtCall) Is Nothing Or Not RowData.OptQuote(enOtPut) Is Nothing Then
'                        .AddItem ""
'                        nRow = .Rows - 1
'
'                        .RowData(nRow) = RowData
'
'                        If Not RowData.OptQuote(enOtCall) Is Nothing Then
'                            sKey = RowData.Opt(enOtCall).Symbol
'                            If aExch.ID <> 0 Then
'                                sKey = sKey & "." & aExch.Code
'                            End If
'                            .TextMatrix(nRow, UOC_C_KEY) = sKey
'
'                            OptionUpdateQuote nRow, enOtCall, True, False
'                            IncProgress pbProgress
'                        End If
'
'                        If Not RowData.OptQuote(enOtPut) Is Nothing Then
'                            sKey = RowData.Opt(enOtPut).Symbol
'                            If aExch.ID <> 0 Then
'                                sKey = sKey & "." & aExch.Code
'                            End If
'                            .TextMatrix(nRow, UOC_P_KEY) = sKey
'
'                            OptionUpdateQuote nRow, enOtPut, True, False
'                            IncProgress pbProgress
'                        End If
'
'                        .RowHidden(nRow) = Not RowData.Exp.Visible Or Not RowData.Exch.Visible
'                    End If
'
'                    Set RowData = Nothing
'                Next
'                Set aStrike = Nothing
'            Next
'
'            Set aExpiry = Nothing
'        Next
'        m_VisibleExp.Clear
'
'        FormatOptColumns True, True
'        UserControl_Resize
'
'        i = -1
'        i = .ColIndex(UOC_C_SERIES)
'        If i < 0 Then i = .ColIndex(UOC_P_SERIES)
'        geOpt.ShowSortImage i, 1
'    End With
'
'    RatesUpdate
'    AdjustDivsAndRatesVisible
'
'    m_VisibleExp.Clear
'    m_VisibleOptExch.Clear
'    m_CustRates.Clear
'    m_bUseCustRates = False
'    m_nIsAmerican = -1
'
'    AdjustCaption
'    Set RowData = Nothing
'
'    m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'
'    imgStop.Visible = True
'    imgStopDis.Visible = False
'    lblProcess.Caption = ""
'    m_bInProc = False
'    AdjustState
'End Sub
'
'Private Function UnderlyingShow() As Boolean
'    On Error GoTo EH
'    Dim RowData As clsQnQuoteRowData, aExch As EtsGeneralLib.ExchAtom, sKey$
'    Dim nRow&, aQuote As clsQnQuoteAtom
'    Dim bAllExchVisible As Boolean, bShowOnlyDefExch As Boolean
'
'    UnderlyingShow = False
'    If m_bInProc Then Exit Function
'
'    m_bInProc = True
'    AdjustState
'    DoEvents
'
'    m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'    m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'    m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'    pbProgress.Visible = True
'
'    lblStatus.Visible = False
'    lblProcess.Visible = True
'    lblProcess.Caption = "Formatting..."
'    lblProcess.Refresh
'    imgStop.Visible = True
'    imgStopDis.Visible = False
'
'    With fgUnd
'        .Rows = 1
'
'        bAllExchVisible = True
'        bShowOnlyDefExch = False
'        If m_VisibleUndExch.Count > 0 Then
'            For Each aExch In m_UndExch
'                If Not m_VisibleUndExch(CStr(aExch.ID)) Is Nothing Then
'                    bAllExchVisible = False
'                    Exit For
'                End If
'            Next
'            bShowOnlyDefExch = bAllExchVisible
'        End If
'
'        For Each aExch In m_UndExch
'            If Not bAllExchVisible Then
'                If aExch.ID <> 0 Then
'                    aExch.Visible = Not m_VisibleUndExch(CStr(aExch.ID)) Is Nothing
'                Else
'                    aExch.Visible = True
'                End If
'            Else
'                aExch.Visible = (Not bShowOnlyDefExch Or bShowOnlyDefExch And aExch.ID = 0)
'            End If
'        Next
'
'        For Each aExch In m_UndExch
'            Set aQuote = m_Und.Quotes(CStr(aExch.ID))
'
'            If Not aQuote Is Nothing Then
'                .AddItem ""
'                nRow = .Rows - 1
'
'                Set RowData = New clsQnQuoteRowData
'                Set RowData.Und = m_Und
'                Set RowData.UndQuote = aQuote
'                Set RowData.Exch = aExch
'                .RowData(nRow) = RowData
'
'                sKey = RowData.Und.Symbol
'                If aExch.ID <> 0 Then
'                    sKey = sKey & "." & aExch.Code
'                End If
'                .TextMatrix(nRow, UUC_KEY) = sKey
'                UnderlyingUpdateQuote nRow, True, False
'
'                .RowHidden(nRow) = Not RowData.Exch.Visible
'
'                Set RowData = Nothing
'                IncProgress pbProgress
'            End If
'
'            Set aQuote = Nothing
'        Next
'
'        FormatUndColumns True, True
'    End With
'
'    RatesUpdate
'    AdjustDivsAndRatesVisible
'    DivsUpdate
'    ModelUpdate
'    CalendarUpdate
'    AdjustLogVisible
'
'    UnderlyingShow = True
'
'EX:
'    On Error Resume Next
'    m_VisibleUndExch.Clear
'    m_CustRates.Clear
'    m_bUseCustRates = False
'    m_nIsAmerican = -1
'
'    AdjustCaption
'    Set RowData = Nothing
'
'    m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'    m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'
'    imgStop.Visible = True
'    imgStopDis.Visible = False
'    lblProcess.Caption = ""
'    m_bInProc = False
'    AdjustState
'    Exit Function
'EH:
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlying."
'    UnderlyingShow = False
'    GoTo EX
'End Function
'
'Private Sub UnderlyingUpdate(ByVal bSymbol As Boolean, ByVal bAutosize As Boolean, ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim i&, nRows&
'
'    With fgUnd
'        m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'
'        nRows = .Rows - 1
'        For i = 1 To nRows
'            UnderlyingUpdateQuote i, bSymbol, bUpdateColors
'        Next
'
''        If bAutosize Then
''            .AutoSize 1, 2, , 300
''            .AutoSize 2, .Cols - 1
''        End If
'
'        m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'    End With
'End Sub
'
'Private Sub UnderlyingUpdateQuote(ByVal nRow As Long, ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim aExch As EtsGeneralLib.ExchAtom, nCol&
'    Dim RowData As clsQnQuoteRowData, aUnd As clsQnUndAtom, aQuote As clsQnQuoteAtom
'
'    With fgUnd
'        m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'
'        Set RowData = .RowData(nRow)
'        Set aUnd = RowData.Und
'        Set aQuote = RowData.UndQuote
'
'        If Not aUnd Is Nothing And Not aQuote Is Nothing Then
'            If bSymbol Then
'                nCol = .ColIndex(UUC_SYMBOL)
'                Debug.Assert nCol >= 0
'
'                If nRow = 1 Then
'                    .TextMatrix(nRow, nCol) = aUnd.Symbol
'                Else
'                    .TextMatrix(nRow, nCol) = ""
'                End If
'
'                nCol = .ColIndex(UUC_EXCHANGE)
'                If nCol >= 0 Then
'                    Debug.Assert Not aQuote.Exch Is Nothing
'                    .TextMatrix(nRow, nCol) = aQuote.Exch.Code
'                End If
'
'                nCol = .ColIndex(UUC_BIN_NUM)
'                If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = 0, aUnd.BinNum, "")
'
'                nCol = .ColIndex(UUC_TRADER_ROLE)
'                If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = 0, aUnd.TraderRoleName, "")
'            End If
'
'            nCol = .ColIndex(UUC_STATUS)
'            If nCol >= 0 Then
'                Select Case (aQuote.TradingStatus)
'                    Case ISEPROVIDERLib.UnderlyingTradingStatusEnum.enUtsClosed
'                        .TextMatrix(nRow, nCol) = "Closed"
'                    Case ISEPROVIDERLib.UnderlyingTradingStatusEnum.enUtsFastMarket
'                        .TextMatrix(nRow, nCol) = "FastMarket"
'                    Case ISEPROVIDERLib.UnderlyingTradingStatusEnum.enUtsOpen
'                        .TextMatrix(nRow, nCol) = "Open"
'                    Case ISEPROVIDERLib.UnderlyingTradingStatusEnum.enUtsResumed
'                        .TextMatrix(nRow, nCol) = "Resumed"
'                    Case ISEPROVIDERLib.UnderlyingTradingStatusEnum.enUtsSuspended
'                        .TextMatrix(nRow, nCol) = "Suspended"
'                    Case Else
'                        .TextMatrix(nRow, nCol) = "Unknown"
'                End Select
'            End If
'
'
'            nCol = .ColIndex(UUC_BID)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.BidPrice > BAD_DOUBLE_VALUE, aQuote.BidPrice, STR_NA)
'
'            nCol = .ColIndex(UUC_ASK)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.AskPrice > BAD_DOUBLE_VALUE, aQuote.AskPrice, STR_NA)
'
'            nCol = .ColIndex(UUC_OPEN)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.OpenPrice > BAD_DOUBLE_VALUE, aQuote.OpenPrice, STR_NA)
'
'            nCol = .ColIndex(UUC_CLOSE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.ClosePrice > BAD_DOUBLE_VALUE, aQuote.ClosePrice, STR_NA)
'
'            nCol = .ColIndex(UUC_HIGH)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.HighPrice > BAD_DOUBLE_VALUE, aQuote.HighPrice, STR_NA)
'
'            nCol = .ColIndex(UUC_LOW)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.LowPrice > BAD_DOUBLE_VALUE, aQuote.LowPrice, STR_NA)
'
'            nCol = .ColIndex(UUC_LAST)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.LastPrice > BAD_DOUBLE_VALUE, aQuote.LastPrice, STR_NA)
'
'            nCol = .ColIndex(UUC_VOLUME)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA)
'
'            nCol = .ColIndex(UUC_POS)
'            If nCol >= 0 Then
'                Debug.Assert Not aQuote.Exch Is Nothing
'                If aQuote.Exch.ID = 0 Then
'                    .TextMatrix(nRow, nCol) = IIf(aUnd.QtyInShares > BAD_LONG_VALUE, aUnd.QtyInShares, STR_NA)
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
'
'            nCol = .ColIndex(UUC_NET_CHANGE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA)
'
'            nCol = .ColIndex(UUC_BID_SIZE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA)
'
'            nCol = .ColIndex(UUC_ASK_SIZE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA)
'
'            If bUpdateColors Then
'                UnderlyingUpdateQuoteColors nRow, aQuote
'            End If
'        End If
'
'        m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'    End With
'
'    Set aUnd = Nothing
'    Set aQuote = Nothing
'    Set RowData = Nothing
'    Set aExch = Nothing
'End Sub
'
'Private Sub UnderlyingUpdateColors()
'    On Error Resume Next
'    Dim i&, nRows&
'
'    With fgUnd
'        m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'
'        nRows = .Rows - 1
'        For i = 1 To nRows
'            UnderlyingUpdateQuoteColors i, Nothing
'        Next
'
'        m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'    End With
'End Sub
'
'Private Sub UnderlyingUpdateQuoteColors(ByVal nRow As Long, ByRef aRowQuote As clsQnQuoteAtom)
'    On Error Resume Next
'    Dim aQuote As clsQnQuoteAtom, nCol&
'
'    With fgUnd
'        m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'
'        If aRowQuote Is Nothing Then
'            Set aQuote = .RowData(nRow).UndQuote
'        Else
'            Set aQuote = aRowQuote
'        End If
'
'        If Not aQuote Is Nothing Then
'            nCol = .ColIndex(UUC_BID)
'            If nCol >= 0 Then
'                If aQuote.BidPriceTick = 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(UUC_BID).ForeColor
'                ElseIf aQuote.BidPriceTick < 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(UUC_BID).ForeColorAlt2
'                Else
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(UUC_BID).ForeColorAlt1
'                End If
'            End If
'
'            nCol = .ColIndex(UUC_ASK)
'            If nCol >= 0 Then
'                If aQuote.AskPriceTick = 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(UUC_ASK).ForeColor
'                ElseIf aQuote.AskPriceTick < 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(UUC_ASK).ForeColorAlt2
'                Else
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(UUC_ASK).ForeColorAlt1
'                End If
'            End If
'        End If
'
'        Set aQuote = Nothing
'
'        m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'    End With
'End Sub
'
'Private Sub LoadExchanges()
'    On Error Resume Next
'    Dim aExch As EtsGeneralLib.ExchAtom, aGExch As EtsGeneralLib.ExchAtom
'
'    m_Exch.Clear
'    For Each aGExch In g_Exch
'        Set aExch = m_Exch(aGExch.ID)
'        If aExch Is Nothing Then
'            Set aExch = New EtsGeneralLib.ExchAtom
'            aExch.ID = aGExch.ID
'            aExch.Code = aGExch.Code
'            aExch.Name = aGExch.Name
'            m_Exch.Add aExch.ID, aExch.Code, aExch
'            Set aExch = Nothing
'        End If
'    Next
'End Sub
'
'Private Sub OptionsUpdate(ByVal bSymbol As Boolean, ByVal bAutosize As Boolean, ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim i&, nRows&
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'        nRows = .Rows - 1
'        For i = 1 To nRows
'            OptionUpdateQuote i, enOtCall, bSymbol, bUpdateColors
'            OptionUpdateQuote i, enOtPut, bSymbol, bUpdateColors
'        Next
'
''        If bAutosize Then
''            .AutoSize 2, .Cols - 1
''        End If
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub OptionUpdateQuote(ByVal nRow As Long, ByVal nOptType As Long, ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim RowData As clsQnQuoteRowData, aOpt As clsQnOptAtom, aQuote As clsQnQuoteAtom
'    Dim nCol&, nShift&
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'        Set RowData = .RowData(nRow)
'        Set aOpt = RowData.Opt(nOptType)
'        Set aQuote = RowData.OptQuote(nOptType)
'
'        If Not (aOpt Is Nothing Or aQuote Is Nothing) Then
'            If nOptType = enOtPut Then
'                nShift = UOC_P_SYMBOL - 2
'            Else
'                nShift = 0
'            End If
'
'            If bSymbol Then
'                nCol = .ColIndex(nShift + UOC_C_SYMBOL)
'                If nCol >= 0 Then .TextMatrix(nRow, nCol) = aOpt.Symbol
'
'                nCol = .ColIndex(nShift + UOC_C_EXCHANGE)
'                If nCol >= 0 Then
'                    Debug.Assert Not aQuote.Exch Is Nothing
'                    .TextMatrix(nRow, nCol) = aQuote.Exch.Code
'                End If
'
'                nCol = .ColIndex(nShift + UOC_C_SERIES)
'                If nCol >= 0 Then .TextMatrix(nRow, nCol) = aQuote.Series
'
'                nCol = .ColIndex(nShift + UOC_C_STRIKE)
'                If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aOpt.Strike > BAD_DOUBLE_VALUE, aOpt.Strike, STR_NA)
'
'                nCol = .ColIndex(nShift + UOC_C_EXPIRY)
'                If nCol >= 0 Then
'                    .TextMatrix(nRow, nCol) = aOpt.Expiry
'                End If
'
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_BID)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.BidPrice > BAD_DOUBLE_VALUE, aQuote.BidPrice, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_ASK)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.AskPrice > BAD_DOUBLE_VALUE, aQuote.AskPrice, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_LAST)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.LastPrice > BAD_DOUBLE_VALUE, aQuote.LastPrice, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_VOLUME)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_QTY)
'            If nCol >= 0 Then
'                Debug.Assert Not aQuote.Exch Is Nothing
'                If aQuote.Exch.ID = m_nOptPosExchID Then
'                    .TextMatrix(nRow, nCol) = IIf(aOpt.Qty > BAD_LONG_VALUE, aOpt.Qty, STR_NA)
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_OPEN_INTEREST)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.OpenInterest > BAD_LONG_VALUE, aQuote.OpenInterest, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_THEO_PRICE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.TheoPrice > BAD_DOUBLE_VALUE, aQuote.TheoPrice, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_DELTA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Delta > BAD_DOUBLE_VALUE, aQuote.Delta, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_GAMMA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Gamma > BAD_DOUBLE_VALUE, aQuote.Gamma, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_VEGA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Vega > BAD_DOUBLE_VALUE, aQuote.Vega, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_THETA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Theta > BAD_DOUBLE_VALUE, aQuote.Theta, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_RHO)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.Rho > BAD_DOUBLE_VALUE, aQuote.Rho, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_VEGA_DELTA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.VegaDelta > BAD_DOUBLE_VALUE, aQuote.VegaDelta, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_VEGA_GAMMA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.VegaGamma > BAD_DOUBLE_VALUE, aQuote.VegaGamma, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_THETA_DELTA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.ThetaDelta > BAD_DOUBLE_VALUE, aQuote.ThetaDelta, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_THETA_GAMMA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.ThetaGamma > BAD_DOUBLE_VALUE, aQuote.ThetaGamma, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_GAMMA_PER_THETA)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.GammaPerTheta > BAD_DOUBLE_VALUE, aQuote.GammaPerTheta, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_VOLA)
'            If nCol >= 0 Then
'                If aOpt.Vola > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = aOpt.Vola * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_IV)
'            If nCol >= 0 Then
'                If aQuote.IV > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = aQuote.IV * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_IV_BID)
'            If nCol >= 0 Then
'                If aQuote.IVBid > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = aQuote.IVBid * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_IV_ASK)
'            If nCol >= 0 Then
'                If aQuote.IVAsk > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = aQuote.IVAsk * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_NET_CHANGE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_BID_SIZE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_ASK_SIZE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_ASK_SIZE)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_TRADE_STATUS)
'            If nCol >= 0 Then
'                Select Case aQuote.TradingStatus
'                    Case ISEPROVIDERLib.SeriesTradingStatusEnum.enStsClosed
'                        .TextMatrix(nRow, nCol) = "Closed"
'                    Case ISEPROVIDERLib.SeriesTradingStatusEnum.enStsFastMarket
'                        .TextMatrix(nRow, nCol) = "Fast Market"
'                    Case ISEPROVIDERLib.SeriesTradingStatusEnum.enStsHalt
'                        .TextMatrix(nRow, nCol) = "Halt"
'                    Case ISEPROVIDERLib.SeriesTradingStatusEnum.enStsQueuing
'                        .TextMatrix(nRow, nCol) = "Queuing"
'                    Case ISEPROVIDERLib.SeriesTradingStatusEnum.enStsQueuingAndTryingToOpen
'                        .TextMatrix(nRow, nCol) = "Trying To Open"
'                    Case ISEPROVIDERLib.SeriesTradingStatusEnum.enStsRegular
'                        .TextMatrix(nRow, nCol) = "Regular"
'                    Case Else
'                        .TextMatrix(nRow, nCol) = "Unknown"
'                End Select
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_MM_BID_TICK_LEVEL)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.MmTickWorseLevelBid > BAD_LONG_VALUE, aQuote.MmTickWorseLevelBid, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_MM_BID_QTY)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.MmQuantityBid > BAD_LONG_VALUE, aQuote.MmQuantityBid, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_MM_BID_PREM)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.MmPremiumBid > BAD_DOUBLE_VALUE, aQuote.MmPremiumBid, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_MM_ASK_TICK_LEVEL)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.MmTickWorseLevelAsk > BAD_LONG_VALUE, aQuote.MmTickWorseLevelAsk, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_MM_ASK_QTY)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.MmQuantityAsk > BAD_LONG_VALUE, aQuote.MmQuantityAsk, STR_NA)
'
'            nCol = .ColIndex(nShift + UOC_C_MM_ASK_PREM)
'            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.MmPremiumAsk > BAD_DOUBLE_VALUE, aQuote.MmPremiumAsk, STR_NA)
'
'            If bUpdateColors Then
'                OptionUpdateColors nRow, nOptType, RowData
'            End If
'        End If
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'    End With
'
'    Set aOpt = Nothing
'    Set aQuote = Nothing
'    Set RowData = Nothing
'End Sub
'
'Private Sub OptionsUpdateColors()
'    On Error Resume Next
'    Dim i&, nRows&
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'        nRows = .Rows - 1
'        For i = 1 To nRows
'            OptionUpdateColors i, enOtCall, Nothing
'            OptionUpdateColors i, enOtPut, Nothing
'        Next
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub OptionUpdateColors(ByVal nRow As Long, ByVal nOptType As Long, aRowData As clsQnQuoteRowData)
'    On Error Resume Next
'    Dim aOpt As clsQnOptAtom, aQuote As clsQnQuoteAtom, nCol&, RowData As clsQnQuoteRowData, nShift&
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'        If aRowData Is Nothing Then
'            Set RowData = .RowData(nRow)
'        Else
'            Set RowData = aRowData
'        End If
'        Set aOpt = RowData.Opt(nOptType)
'        Set aQuote = RowData.OptQuote(nOptType)
'
'        If Not (aOpt Is Nothing Or aQuote Is Nothing) Then
'            If nOptType = enOtPut Then
'                nShift = UOC_P_SYMBOL - 2
'            Else
'                nShift = 0
'            End If
'
'            ' Bid
'            nCol = .ColIndex(nShift + UOC_C_MM_BID_PREM)
'            If nCol >= 0 Then
'                If aQuote.MmPremiumBid > aQuote.BidPrice And aQuote.MmPremiumBid <> 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_MM_BID_PREM).ForeColorAlt2
'                ElseIf aQuote.MmPremiumBid = aQuote.BidPrice And aQuote.MmPremiumBid <> 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_MM_BID_PREM).ForeColorAlt1
'                Else
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_MM_BID_PREM).ForeColor
'                End If
'            End If
'
'            ' ask
'            nCol = .ColIndex(nShift + UOC_C_MM_ASK_PREM)
'            If nCol >= 0 Then
'                If aQuote.MmPremiumAsk < aQuote.AskPrice And aQuote.MmPremiumAsk <> 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_MM_ASK_PREM).ForeColorAlt2
'                ElseIf aQuote.MmPremiumAsk = aQuote.AskPrice And aQuote.MmPremiumAsk <> 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_MM_ASK_PREM).ForeColorAlt1
'                Else
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_MM_ASK_PREM).ForeColor
'                End If
'            End If
'
'            ' price
'            nCol = .ColIndex(nShift + UOC_C_BID)
'            If nCol >= 0 Then
'                If aQuote.BidPriceTick = 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_BID).ForeColor
'                ElseIf aQuote.BidPriceTick < 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_BID).ForeColorAlt2
'                Else
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_BID).ForeColorAlt1
'                End If
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_ASK)
'            If nCol >= 0 Then
'                If aQuote.AskPriceTick = 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_ASK).ForeColor
'                ElseIf aQuote.AskPriceTick < 0# Then
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_ASK).ForeColorAlt2
'                Else
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_ASK).ForeColorAlt1
'                End If
'            End If
'
'            nCol = .ColIndex(nShift + UOC_C_THEO_PRICE)
'            If nCol >= 0 Then
'                If aQuote.TheoPrice >= 0# Then
'                    If aQuote.TheoPrice < aQuote.BidPrice Then
'                        .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_THEO_PRICE).ForeColorAlt1
'                    ElseIf aQuote.AskPrice >= 0# And aQuote.TheoPrice > aQuote.AskPrice Then
'                        .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_THEO_PRICE).ForeColorAlt2
'                    Else
'                        .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_THEO_PRICE).ForeColor
'                    End If
'                Else
'                    .Cell(flexcpForeColor, nRow, nCol) = m_gdOpt.Col(nShift + UOC_C_THEO_PRICE).ForeColor
'                End If
'            End If
'        End If
'
'        Set RowData = Nothing
'        Set aOpt = Nothing
'        Set aQuote = Nothing
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'    End With
'End Sub
'
'Public Sub ModelUpdate()
'    On Error Resume Next
'    If m_Und Is Nothing Then Exit Sub
'
'    With fgDiv
'        m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'
'        .TextMatrix(1, UDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
'        .TextMatrix(1, UDC_STYLE) = IIf(m_Und.IsAmerican, 1, 0)
'
'        .AutoSize 0, .Cols - 1
'
'        m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    End With
'End Sub
'
'Public Sub CalendarUpdate()
'    On Error Resume Next
'    If m_Und Is Nothing Then Exit Sub
'
'    Dim sName$
'
'    With fgDiv
'        m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'
'        sName = g_ExpCalendar(m_Und.ExpCalendarID).Name
'        .TextMatrix(1, UDC_CALENDAR) = sName
'
'        .AutoSize 0, .Cols - 1
'
'        m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub DivsUpdate()
'    On Error Resume Next
'    If m_Und Is Nothing Then Exit Sub
'
'    Dim nDivFreq&, dDivAmt#, dtDivDate As Date
'
'    With fgDiv
'        m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'
'        .TextMatrix(1, UDC_DIV) = IIf(m_Und.UseCustDivs, "1", "0")
'        If m_Und.UseCustDivs Then
'            dtDivDate = m_Und.DivDateCust
'            nDivFreq = m_Und.DivFreqCust
'            dDivAmt = m_Und.DivAmtCust
'        Else
'            dtDivDate = m_Und.DivDate
'            nDivFreq = m_Und.DivFreq
'            dDivAmt = m_Und.DivAmt
'        End If
'
'        .TextMatrix(1, UDC_FREQ) = nDivFreq
'        .TextMatrix(1, UDC_DATE) = Format$(dtDivDate, m_gdDiv.Col(UDC_DATE).Format)
'        .TextMatrix(1, UDC_AMT) = IIf(dDivAmt > BAD_DOUBLE_VALUE, dDivAmt, "")
'
'        If m_bDividendsVisible And (m_Und.UndType = enCtStock) Then
'            If IsValidDivFreq(nDivFreq) Then
'                .ColHidden(UDC_DATE) = False
'                .ColHidden(UDC_AMT) = False
'            Else
'                .ColHidden(UDC_DATE) = True
'                .ColHidden(UDC_AMT) = True
'            End If
'        End If
'
'        .AutoSize 0, .Cols - 1
'
'        m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub RatesUpdate()
'    On Error Resume Next
'    If m_Und Is Nothing Then Exit Sub
'
'    Dim nExpCount&, i&
'
'    With fgDiv
'        m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'
'        nExpCount = m_Und.Expiry.Count
'        .Cols = UDC_LAST_COLUMN + nExpCount
'
'        .TextMatrix(1, UDC_RATE) = IIf(m_Und.UseCustRates, "1", "0")
'
'        For i = 1 To nExpCount
'            .ColFormat(i + UDC_RATE) = m_gdDiv.Col(UDC_RATE_VAL).Format
'            .ColDataType(i + UDC_RATE) = m_gdDiv.Col(UDC_RATE_VAL).DataType
'            .TextMatrix(0, i + UDC_RATE) = Format$(m_Und.Expiry(i).ExpiryMonth, "MMM YY")
'            .TextMatrix(1, i + UDC_RATE) = IIf(m_Und.UseCustRates, m_Und.Expiry(i).RateCust * 100#, m_Und.Expiry(i).Rate * 100#)
'
'            .Cell(flexcpBackColor, 1, i + UDC_RATE) = IIf(m_gdDiv.Col(UDC_RATE_VAL).BackColor <> 0, m_gdDiv.Col(UDC_RATE_VAL).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i + UDC_RATE) = IIf(m_gdDiv.Col(UDC_RATE_VAL).ForeColor <> 0, m_gdDiv.Col(UDC_RATE_VAL).ForeColor, RGB(1, 1, 1))
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
'
'        .AutoSize 0, .Cols - 1
'
'        m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    End With
'End Sub
'
'Private Function IsValidDivFreq(ByVal nDivFreq As Long) As Boolean
'    On Error Resume Next
'    IsValidDivFreq = (nDivFreq = 1 Or nDivFreq = 2 Or nDivFreq = 4 Or nDivFreq = 12)
'End Function
'
'Private Function IsValidDivs(ByVal bCustom As Boolean) As Boolean
'    On Error Resume Next
'    If bCustom Then
'        IsValidDivs = (m_Und.DivFreqCust = 1 Or m_Und.DivFreqCust = 2 Or m_Und.DivFreqCust = 4 Or m_Und.DivFreqCust = 12) _
'                    And m_Und.DivAmtCust > 0 And m_Und.DivDateCust > 0
'    Else
'        IsValidDivs = (m_Und.DivFreq = 1 Or m_Und.DivFreq = 2 Or m_Und.DivFreq = 4 Or m_Und.DivFreq = 12) _
'                    And m_Und.DivAmt > 0 And m_Und.DivDate > 0
'    End If
'End Function
'
'Private Sub UnderlyingOptionsLoad(aOptColl As clsQnOptColl)
'    On Error Resume Next
'    Dim dtExpiry As Date, sExpiryKey$, dStrike#, sStrikeKey$
'    Dim aQuote As clsQnQuoteAtom, nOptType&, sKey$, aEnt As clsEntityAtom
'    Dim aExpiry As clsQnExpAtom, aStrike As clsQnStrikeAtom, aOption As clsQnOptAtom, aExch As EtsGeneralLib.ExchAtom
'    Dim nLotSize&, o&, nOptCount&, aInOpt As clsQnOptAtom
'
'    m_bInProc = True
'    AdjustState
'    DoEvents
'
'    nOptCount = aOptColl.Count
'
'    m_Und.OptionsCount = 0
'    m_Und.Expiry.Clear
'
'    For o = 1 To nOptCount
'        Set aInOpt = aOptColl(o)
'
'        dtExpiry = aInOpt.Expiry
'        sExpiryKey = Format$(dtExpiry, "MMMM YY")
'        dStrike = aInOpt.Strike
'        sStrikeKey = CStr(dStrike)
'        nOptType = aInOpt.OptType
'        nLotSize = aInOpt.Quotes(1).LotSize
'
'        Set aExpiry = m_Und.Expiry(sExpiryKey)
'        If aExpiry Is Nothing Then
'            Set aExpiry = m_Und.Expiry.Add(sExpiryKey)
'            aExpiry.Expiry = dtExpiry
'            aExpiry.ExpiryMonth = DateSerial(Year(dtExpiry), Month(dtExpiry), 1)
'
'            Set aEnt = m_CustRates(CStr(CLng(aExpiry.ExpiryMonth)))
'            If Not aEnt Is Nothing Then
'                aExpiry.RateCust = aEnt.Data3
'            End If
'            Set aEnt = Nothing
'        End If
'
'        Set aStrike = aExpiry.Strike(sStrikeKey)
'        If aStrike Is Nothing Then
'            Set aStrike = aExpiry.Strike.Add(sStrikeKey)
'            aStrike.Strike = dStrike
'        End If
'
'        Set aOption = aStrike.aOption(nOptType)
'        If aOption.ID = 0 Then
'            aOption.ID = aInOpt.ID
'            aOption.Symbol = aInOpt.Symbol
'            aOption.OptType = nOptType
'            aOption.Expiry = dtExpiry
'            aOption.Strike = dStrike
'
'            Set aOption.Exp = aExpiry
'
'            Set aOption.Underlying = m_Und
'            aOption.Vola = m_Und.VolaSrv.OptionVola(dtExpiry, dStrike)
'
'            If aOption.Vola < 0 Then
'                aOption.Vola = BAD_DOUBLE_VALUE
'            End If
'
'            m_Und.OptionsCount = m_Und.OptionsCount + 1
'
''            vPos = rsOpt!iQty
''            aOption.Qty = IIf(IsNull(vPos), BAD_LONG_VALUE, ReadLng(vPos))
''
''            vPos = rsOpt!iPosition
''            aOption.QtyInShares = IIf(IsNull(vPos), BAD_LONG_VALUE, ReadLng(vPos))
'        End If
'
'        For Each aExch In m_OptExch
'            Set aQuote = aOption.Quotes(CStr(aExch.ID))
'            If aQuote Is Nothing Then
'                Set aQuote = aOption.Quotes.Add(CStr(aExch.ID))
'                Set aQuote.Exch = aExch
'                aQuote.LotSize = nLotSize
'
'                aQuote.Series = UCase$(Format$(aOption.Expiry, "YYMMM")) & Trim$(Str$(Int(aOption.Strike)))
'                sKey = aOption.Symbol
'
'                If aExch.ID = 0 Then
'                    Set aOption.DefQuote = aQuote
'                Else
'                    aQuote.Series = aQuote.Series & " " & aExch.Code
'                    sKey = sKey & "." & aExch.Code
'                End If
'
'                If m_QuoteRequests(sKey) Is Nothing Then
'                    m_QuoteRequests.Add aExch, , aOption, sKey
'                End If
'            End If
'            Set aQuote = Nothing
'        Next
'
''        DoEvents
''        If m_enState = enQsReady Then GoTo EX
''        IncProgress pbProgress
'
'        Set aOption = Nothing
'        Set aInOpt = Nothing
'    Next
'
'    UnderlyingAdjustRates True
'
'    m_bInProc = False
'    AdjustState
'    DoEvents
'End Sub
'
'Private Function UnderlyingLoad(aNewUnd As clsQnUndAtom, ByVal bIsTraderContract As Boolean) As Boolean
'    On Error GoTo EH
'    Dim rsUnd As ADODB.Recordset
'    Dim rsExch As ADODB.Recordset
'    Dim aQuote As clsQnQuoteAtom, sKey$, vPos As Variant
'    Dim aExch As EtsGeneralLib.ExchAtom, aExch2 As EtsGeneralLib.ExchAtom
'    Dim nLotSize&, aGUnd As EtsGeneralLib.UndAtom
'
'    If m_bInProc Or aNewUnd Is Nothing Then Exit Function
'
'    m_bInProc = True
'    AdjustState
'    DoEvents
'
'    pbProgress.Min = 0
'    pbProgress.Value = pbProgress.Min
'    pbProgress.Visible = True
'
'    lblStatus.Visible = False
'    lblProcess.Visible = True
'    lblProcess.Caption = "Data load..."
'    lblProcess.Refresh
'    imgStop.Visible = True
'    imgStopDis.Visible = False
'
'    DoEvents
'
'    UnderlyingLoad = False
'    m_enState = enQsDataLoad
'    Set m_UndExch = Nothing
'    Set m_OptExch = Nothing
'    Set m_Und = Nothing
'
'    Set m_Und = New clsQnUndAtom
'    Set m_UndExch = New EtsGeneralLib.ExchColl
'    Set m_OptExch = New EtsGeneralLib.ExchColl
'
'    Set aExch = New EtsGeneralLib.ExchAtom
'    aExch.ID = 0
'    aExch.Code = ""
'    aExch.Name = "<Primary>"
'    m_UndExch.Add aExch.ID, aExch.Code, aExch
'    Set aExch = Nothing
'
'    Set aExch = New EtsGeneralLib.ExchAtom
'    aExch.ID = 0
'    aExch.Code = ""
'    aExch.Name = "<Primary>"
'    m_OptExch.Add aExch.ID, aExch.Code, aExch
'    Set aExch = Nothing
'
'    DoEvents
'    If m_enState = enQsReady Then GoTo EX
'
'    Set rsUnd = gDBW.usp_UnderlyingPos_Get(aNewUnd.ID, g_CurTraderID)
'
'    If rsUnd.RecordCount > 0 Then
'        pbProgress.Max = rsUnd.RecordCount * (m_UndExch.Count + 1)
'    End If
'
'    DoEvents
'    If m_enState = enQsReady Then GoTo EX
'
'    If Not rsUnd.EOF Then
'        If aNewUnd.ID = ReadLng(rsUnd!iContractID) Then
'
'            m_Und.ID = aNewUnd.ID
'
'            m_Und.UndType = aNewUnd.UndType
'            m_Und.Symbol = aNewUnd.Symbol
'
'            If m_Und.UndType = enCtStock Then
'                m_Und.DivFreq = aNewUnd.DivFreq
'                m_Und.DivDate = aNewUnd.DivDate
'                m_Und.DivAmt = aNewUnd.DivAmt
'
'                m_Und.UseCustDivs = aNewUnd.UseCustDivs
'                m_Und.DivDateCust = aNewUnd.DivDateCust
'                m_Und.DivFreqCust = aNewUnd.DivFreqCust
'                m_Und.DivAmtCust = aNewUnd.DivAmtCust
'
'                If m_Und.DivDate = 0 Then m_Und.DivDate = Date
'                If m_Und.DivDateCust = 0 Then m_Und.DivDateCust = Date
'           Else
'                Set m_Und.BasketIndex = aNewUnd.BasketIndex
'                m_Und.Yield = aNewUnd.Yield
'            End If
'
'            m_Und.IsAmerican = aNewUnd.IsAmerican
'            m_Und.IsHTB = aNewUnd.IsHTB
'            m_Und.ExpCalendarID = aNewUnd.ExpCalendarID
'            m_Und.Skew = aNewUnd.Skew
'            m_Und.Kurt = aNewUnd.Kurt
'
'            m_Und.BinNum = aNewUnd.BinNum
'            m_Und.TraderRole = aNewUnd.TraderRole
'
'            m_Und.IsTraderContract = bIsTraderContract
'
'            Set aGUnd = g_Underlying(m_Und.ID)
'            If Not aGUnd Is Nothing Then
'                Set m_Und.UndPriceProfile = aGUnd.UndPriceProfile
'                Set m_Und.OptPriceProfile = aGUnd.OptPriceProfile
'                Set aGUnd = Nothing
'            End If
'
'            InitVola m_Und
'
'            vPos = rsUnd!iQty
'            m_Und.QtyInShares = IIf(IsNull(vPos), BAD_LONG_VALUE, ReadLng(vPos))
'            vPos = rsUnd!fPosition
'            m_Und.Pos = IIf(IsNull(vPos), BAD_DOUBLE_VALUE, ReadDbl(vPos))
'
'            If m_nIsAmerican >= 0 Then
'                m_Und.IsAmerican = (m_nIsAmerican <> 0)
'            End If
'
'            nLotSize = 1
'            If Not m_Und.Quotes("0") Is Nothing Then
'                nLotSize = m_Und.Quotes("0").LotSize
'            End If
'
'            m_Und.Quotes.Clear
'            m_Und.Expiry.Clear
'
'            For Each aExch In m_UndExch
'                Set aQuote = m_Und.Quotes(CStr(aExch.ID))
'                If aQuote Is Nothing Then
'                    Set aQuote = m_Und.Quotes.Add(CStr(aExch.ID))
'                    Set aQuote.Exch = aExch
'                    aQuote.LotSize = nLotSize
'
'                    sKey = m_Und.Symbol
'                    If aExch.ID <> 0 Then
'                        sKey = sKey & "." & aExch.Code
'                    End If
'
'                    If m_QuoteRequests(sKey) Is Nothing Then
'                        m_QuoteRequests.Add aExch, m_Und, , sKey
'                    End If
'                End If
'            Next
'        Else
'            Debug.Assert False
'        End If
'
'        DoEvents
'        If m_enState = enQsReady Then GoTo EX
'
'        IncProgress pbProgress
'        DoEvents
'
'        Set aQuote = Nothing
'    End If
'
'    If m_Und.ID <> 0 Then
'        m_Und.UseCustRates = m_bUseCustRates
'        UnderlyingLoad = True
'    End If
'
'EX:
'    On Error Resume Next
'    m_enState = enQsReady
'    Set aQuote = Nothing
'    Set rsUnd = Nothing
'    Set rsExch = Nothing
'    Set aExch = Nothing
'    Set aExch2 = Nothing
'
'    imgStop.Visible = True
'    imgStopDis.Visible = False
'    lblProcess.Caption = ""
'    m_bInProc = False
'    AdjustState
'    Exit Function
'EH:
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlying."
'    GoTo EX
'End Function
'
'Public Sub RefreshView()
'    On Error Resume Next
'    If m_enState <> enQsRealtime Then Exit Sub
'
'    Recalculate False
'    UnderlyingUpdate False, True, True
'End Sub
'
'Public Sub InitGrids()
'    On Error Resume Next
'    With fgUnd
'        .Rows = 2
'        .Cols = 10
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight ' flexFlat
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbd
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExMove
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        '.GridLines = flexGridFlat
'        '.GridLinesFixed = flexGridInset '= flexGridFlat
'        '.GridLineWidth = 1
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarNone
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'
'        '.BorderStyle = flexBorderFlat
'        '.SheetBorder = .BackColorBkg
'        .HighLight = flexHighlightWithFocus
'        .ColWidthMin = 150
'    End With
'
'    With fgDiv
'        .Rows = 2
'        .Cols = UDC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .AutoSizeMode = flexAutoSizeColWidth
'        .Appearance = flex3DLight ' flexFlat
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbd
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExNone
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        '.GridLines = flexGridFlat
'        '.GridLinesFixed = flexGridFlat
'        '.GridLineWidth = 1
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarNone
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'
'        '.BorderStyle = flexBorderFlat
'        '.SheetBorder = .BackColorBkg
'        .HighLight = flexHighlightWithFocus
'        .ColWidthMin = 150
'    End With
'
'    With fgOpt
'        .Rows = 1
'        .Cols = 10
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight ' flexFlat
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbd
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExSortAndMove
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusHeavy
'        .FrozenCols = 0
'        .FrozenRows = 0
'        '.GridLines = flexGridFlat
'        '.GridLinesFixed = flexGridFlat
'        '.GridLineWidth = 1
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarBoth
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionByRow
'        .TabBehavior = flexTabControls
'        .HighLight = flexHighlightAlways
'        .ColWidthMin = 150
'    End With
'
'    With fgLog
'        .Rows = 1
'        .Cols = ULC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = True
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .AutoSizeMode = flexAutoSizeColWidth
'        .Appearance = flex3DLight ' flexFlat
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
'        .ColWidthMin = 1350
'    End With
'
'End Sub
'
'Private Sub InitColumns()
'    On Error Resume Next
'    g_DefGrids(GT_QUOTATION_UNDERLYING).CopyTo m_gdUnd
'    g_DefGrids(GT_QUOTATION_DIVIDENDS).CopyTo m_gdDiv
'    g_DefGrids(GT_QUOTATION_OPTIONS).CopyTo m_gdOpt
'    g_DefGrids(GT_QUOTATION_LOG).CopyTo m_gdLog
'End Sub
'
'Private Sub FormatUndGrid()
'    On Error Resume Next
'    Dim i&, nCols&, nIdx&
'
'    With fgUnd
'        m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'
'        m_gdUnd.Idx(0) = UUC_SYMBOL
'        nCols = 1
'        For i = 1 To UUC_LAST_COLUMN
'            If m_gdUnd.Idx(i) <> UUC_NONE Then
'                nCols = nCols + 1
'            Else
'                Exit For
'            End If
'        Next
'
'        nCols = nCols + 1 ' for key column
'        .Cols = nCols
'
'        .ColHidden(UUC_KEY) = True
'        .ColKey(UUC_KEY) = UUC_NONE
'
'        For i = 0 To UUC_LAST_COLUMN
'            nIdx = m_gdUnd.Idx(i)
'            If nIdx <> UUC_NONE Then
'                .TextMatrix(0, i + 1) = m_gdUnd.Col(nIdx).Caption
'                .ColFormat(i + 1) = m_gdUnd.Col(nIdx).Format
'                .ColDataType(i + 1) = m_gdUnd.Col(nIdx).DataType
'                .ColKey(i + 1) = nIdx
'                .ColWidth(i + 1) = IIf(m_gdUnd.Col(nIdx).Width > 0, m_gdUnd.Col(nIdx).Width, -1)
'            Else
'                Exit For
'            End If
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatDivGrid()
'    On Error Resume Next
'    Dim i&
'
'    With fgDiv
'        m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'
'        .Cols = UDC_LAST_COLUMN
'
'        For i = UDC_MODEL To UDC_RATE
'            .TextMatrix(0, i) = m_gdDiv.Col(i).Caption
'            .ColFormat(i) = m_gdDiv.Col(i).Format
'            .ColDataType(i) = m_gdDiv.Col(i).DataType
'        Next
'
'        .TextMatrix(1, UDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
'        .ColComboList(UDC_STYLE) = "#0;European|#1;American"
'        .TextMatrix(1, UDC_STYLE) = "1"
'
'        .ColEditMask(UDC_DATE) = "99/99/9999"
'
'        .TextMatrix(1, UDC_DIV) = "0"
'        .ColComboList(UDC_DIV) = "#0;Market|#1;Custom"
'        .ColComboList(UDC_FREQ) = "#0;<No Divs>|#1;Annually|#2;Semiannually|#4;Quarterly|#12;Monthly"
'
'        .TextMatrix(1, UDC_RATE) = "0"
'        .ColComboList(UDC_RATE) = "#0;Market|#1;Custom"
'
'        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatOptGrid()
'    On Error Resume Next
'    Dim i&, nCols&, nIdx&
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'        nCols = 0
'        For i = 0 To UOC_LAST_COLUMN
'            If m_gdOpt.Idx(i) <> UOC_NONE Then
'                nCols = nCols + 1
'            Else
'                Exit For
'            End If
'        Next
'
'        If nCols = 0 Then
'            m_gdOpt.Idx(0) = UOC_C_SYMBOL
'            nCols = 1
'        End If
'
'        nCols = nCols + 2 ' for key columns
'        .Cols = nCols
'
'        .ColHidden(UOC_C_KEY) = True
'        .ColKey(UOC_C_KEY) = UOC_NONE
'        .ColHidden(UOC_P_KEY) = True
'        .ColKey(UOC_P_KEY) = UOC_NONE
'
'        For i = 0 To UOC_LAST_COLUMN
'            nIdx = m_gdOpt.Idx(i)
'            If nIdx <> UOC_NONE Then
'                .TextMatrix(0, i + 2) = m_gdOpt.Col(nIdx).Caption
'                .ColFormat(i + 2) = m_gdOpt.Col(nIdx).Format
'                .ColDataType(i + 2) = m_gdOpt.Col(nIdx).DataType
'                .ColKey(i + 2) = nIdx
'                .ColWidth(i + 2) = IIf(m_gdOpt.Col(nIdx).Width > 0, m_gdOpt.Col(nIdx).Width, -1)
'            Else
'                Exit For
'            End If
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'        geOpt.AdjustSortImages
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatLogGrid()
'    On Error Resume Next
'    Dim i&, nCols&, nIdx&
'
'    With fgLog
'        m_GridLock(GT_QUOTATION_LOG).LockRedraw
'
'        nCols = ULC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = 0 To ULC_LAST_COLUMN
'            nIdx = m_gdLog.Idx(i)
'            If nIdx <> ULC_NONE Then
'                .TextMatrix(0, i) = m_gdLog.Col(i).Caption
'                .ColFormat(i) = m_gdLog.Col(i).Format
'                .ColDataType(i) = m_gdLog.Col(i).DataType
'                .ColKey(i) = i
'                .ColWidth(i) = IIf(m_gdLog.Col(i).Width > 0, m_gdLog.Col(i).Width, -1)
'            Else
'                Exit For
'            End If
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatUndColumns(ByVal bAutosize As Boolean, ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
'
'    With fgUnd
'        m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
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
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = 0 To nCols
'            nIdx = .ColKey(i)
'            If nIdx <> UUC_NONE Then
'                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).BackColor <> 0, m_gdUnd.Col(nIdx).BackColor, RGB(1, 1, 1))
'                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).ForeColor <> 0, m_gdUnd.Col(nIdx).ForeColor, RGB(1, 1, 1))
'            End If
'        Next
'
'        If bUpdateColors Then
'            UnderlyingUpdateColors
'        End If
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
'        If nRows > 1 Then
'            .Cell(flexcpBackColor, 2, UUC_SYMBOL, .Rows - 1, UUC_SYMBOL) = .GridColor
'        End If
'
'        .BackColorSel = m_gdUnd.BackColorSel
'        .ForeColorSel = m_gdUnd.ForeColorSel
'
''        If bAutosize Then
''            .AutoSize 1, 2, , 300
''            .AutoSize 2, .Cols - 1
''        End If
'
'        m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'    End With
'End Sub
'
'Private Sub AdjustDivsAndRatesVisible()
'    On Error Resume Next
'    Dim nCols&, i&, bValidDivs As Boolean
'
'    With fgDiv
'        m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'
'        If m_bDividendsVisible _
'            And (m_Und.ID = 0 Or m_Und.ID <> 0 And m_Und.UndType = enCtStock) _
'            Or m_bRatesVisible Or m_bCalcModelVisible Or m_bExpCalendarVisible Then
'
'            If m_Und.ID <> 0 Then
'                bValidDivs = IsValidDivFreq(IIf(m_Und.UseCustDivs, m_Und.DivFreqCust, m_Und.DivFreq))
'            Else
'                bValidDivs = False
'            End If
'
'            nCols = .Cols - 1
'            For i = 0 To nCols
'                If i < UDC_CALENDAR Then
'                    .ColHidden(i) = Not m_bCalcModelVisible
'
'                ElseIf i < UDC_DIV Then
'                    .ColHidden(i) = Not m_bExpCalendarVisible
'
'                ElseIf i < UDC_RATE Then
'                    .ColHidden(i) = Not (m_bDividendsVisible _
'                            And (m_Und.ID = 0 Or m_Und.ID <> 0 And m_Und.UndType = enCtStock) _
'                            And (i <= UDC_FREQ Or i > UDC_FREQ And bValidDivs))
'
'
''                            And (i <= UDC_FREQ Or i > UDC_FREQ And bValidDivs))
'                Else
'                    .ColHidden(i) = Not m_bRatesVisible
'                End If
'            Next
'            .Visible = True
'        Else
'            .Visible = False
'        End If
'
'        m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub AdjustLogVisible()
'    On Error Resume Next
'    fgLog.Visible = m_bLogVisible
'End Sub
'
'Private Sub FormatDivColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
'
'    With fgDiv
'        m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'
'        aFont.Name = m_gdDiv.Font.Name
'        aFont.Size = m_gdDiv.Font.Size
'        aFont.Bold = m_gdDiv.Font.Bold
'        aFont.Italic = m_gdDiv.Font.Italic
'        aFont.Strikethrough = m_gdDiv.Font.Strikethrough
'        aFont.Underline = m_gdDiv.Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = 0 To nCols
'            If i < UDC_RATE_VAL Then
'                nIdx = i
'            Else
'                nIdx = UDC_RATE_VAL
'            End If
'
'            .Cell(flexcpBackColor, 1, i) = IIf(m_gdDiv.Col(nIdx).BackColor <> 0, m_gdDiv.Col(nIdx).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i) = IIf(m_gdDiv.Col(nIdx).ForeColor <> 0, m_gdDiv.Col(nIdx).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If m_gdDiv.HorizLinesVisible Then
'            If m_gdDiv.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdDiv.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdDiv.GridLinesColor
'
'        .BackColorSel = m_gdDiv.BackColorSel
'        .ForeColorSel = m_gdDiv.ForeColorSel
'
'        .AutoSize 0, .Cols - 1
'
'        m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatOptColumns(ByVal bAutosize As Boolean, ByVal bUpdateColors As Boolean)
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'        aFont.Name = m_gdOpt.Font.Name
'        aFont.Size = m_gdOpt.Font.Size
'        aFont.Bold = m_gdOpt.Font.Bold
'        aFont.Italic = m_gdOpt.Font.Italic
'        aFont.Strikethrough = m_gdOpt.Font.Strikethrough
'        aFont.Underline = m_gdOpt.Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = 0 To nCols
'            nIdx = .ColKey(i)
'            If nIdx <> UOC_NONE Then
'                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdOpt.Col(nIdx).BackColor <> 0, m_gdOpt.Col(nIdx).BackColor, RGB(1, 1, 1))
'                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdOpt.Col(nIdx).ForeColor <> 0, m_gdOpt.Col(nIdx).ForeColor, RGB(1, 1, 1))
'            End If
'        Next
'
'        If bUpdateColors Then
'            OptionsUpdateColors
'        End If
'
'        If m_gdOpt.HorizLinesVisible Then
'            If m_gdOpt.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gdOpt.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gdOpt.GridLinesColor
'
'        .BackColorSel = m_gdOpt.BackColorSel
'        .ForeColorSel = m_gdOpt.ForeColorSel
'
''        If bAutosize Then
''            .AutoSize 2, .Cols - 1
''        End If
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatLogColumns(ByVal bAutosize As Boolean)
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
'
'    With fgLog
'        m_GridLock(GT_QUOTATION_LOG).LockRedraw
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
'            If nIdx <> ULC_NONE Then
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
''        If bAutosize Then
''            .AutoSize 2, .Cols - 1
''        End If
'
'        m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
'    End With
'End Sub
'
'Private Sub fgDiv_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If (KeyCode = vbKeyG Or KeyCode = vbKeyA Or KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
'        m_bKeyDown(GT_QUOTATION_DIVIDENDS) = True
'    End If
'End Sub
'
'Private Sub fgDiv_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_QUOTATION_DIVIDENDS) Then
'        m_bKeyDown(GT_QUOTATION_DIVIDENDS) = False
'        With fgDiv
'            m_enMenuGrid = GT_QUOTATION_DIVIDENDS
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        If Shift = vbCtrlMask Then
'            Select Case KeyCode
'                Case vbKeyG
'                    mnuCtxAutosizeGrid_Click
'
'                Case vbKeyA
'                    If m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols Then mnuCtxAutosizeCol_Click
'
'                Case vbKeyC, vbKeyInsert
'                    mnuCtxCopy_Click
'            End Select
'
'        End If
'    End If
'End Sub
'
'Private Sub fgOpt_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim nIdx&
'
'    If Col >= 0 And Row < 0 Then
'        With fgOpt
'            nIdx = .ColKey(Col)
'            If nIdx >= UOC_C_SYMBOL And nIdx <= UOC_LAST_COLUMN Then
'                m_gdOpt.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub fgUnd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim nIdx&
'
'    If Col >= 0 And Row < 0 Then
'        With fgUnd
'            nIdx = .ColKey(Col)
'            If nIdx >= UUC_SYMBOL And nIdx <= UUC_LAST_COLUMN Then
'                m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub geOpt_BeforeSort(ByVal Col As Long, Order As Integer)
'    On Error Resume Next
'    Dim nColKey&
'
'    nColKey = fgOpt.ColKey(Col)
'    If nColKey = UOC_C_SERIES Or nColKey = UOC_P_SERIES Then
'        m_nSortSeriesColKey = nColKey
'        If m_iSeriesOrder = flexSortGenericDescending Then
'            m_iSeriesOrder = flexSortGenericAscending
'        Else
'            m_iSeriesOrder = flexSortGenericDescending
'        End If
'
'        Order = flexSortCustom
'    End If
'End Sub
'
'Private Sub geOpt_AfterSort(ByVal Col As Long, Order As Integer)
'    On Error Resume Next
'    If m_nSortSeriesColKey = fgOpt.ColKey(Col) Then
'        Order = m_iSeriesOrder
'        m_nSortSeriesColKey = -2
'    End If
'End Sub
'
'Private Sub fgOpt_Compare(ByVal Row1 As Long, ByVal Row2 As Long, Cmp As Integer)
'    On Error Resume Next
'    Dim aOpt1 As clsQnOptAtom, aOpt2 As clsQnOptAtom, aOptQuote1 As clsQnQuoteAtom, aOptQuote2 As clsQnQuoteAtom
'
'    If m_nSortSeriesColKey = UOC_C_SERIES Or m_nSortSeriesColKey = UOC_P_SERIES Then
'        Cmp = 0
'
'        If m_nSortSeriesColKey = UOC_C_SERIES Then
'            Set aOpt1 = fgOpt.RowData(Row1).Opt(enOtCall)
'            Set aOpt2 = fgOpt.RowData(Row2).Opt(enOtCall)
'            Set aOptQuote1 = fgOpt.RowData(Row1).OptQuote(enOtCall)
'            Set aOptQuote2 = fgOpt.RowData(Row2).OptQuote(enOtCall)
'        ElseIf m_nSortSeriesColKey = UOC_P_SERIES Then
'            Set aOpt1 = fgOpt.RowData(Row1).Opt(enOtPut)
'            Set aOpt2 = fgOpt.RowData(Row2).Opt(enOtPut)
'            Set aOptQuote1 = fgOpt.RowData(Row1).OptQuote(enOtPut)
'            Set aOptQuote2 = fgOpt.RowData(Row2).OptQuote(enOtPut)
'        End If
'
'        If Not aOpt1 Is Nothing And Not aOpt2 Is Nothing _
'            And Not aOptQuote1 Is Nothing And Not aOptQuote2 Is Nothing Then
'            If aOpt1.Exp.ExpiryMonth > aOpt2.Exp.ExpiryMonth Then
'                Cmp = 1
'            ElseIf aOpt1.Exp.ExpiryMonth < aOpt2.Exp.ExpiryMonth Then
'                Cmp = -1
'            ElseIf aOpt1.Strike > aOpt2.Strike Then
'                Cmp = 1
'            ElseIf aOpt1.Strike < aOpt2.Strike Then
'                Cmp = -1
'            ElseIf aOptQuote1.Exch.Code > aOptQuote2.Exch.Code Then
'                Cmp = 1
'            ElseIf aOptQuote1.Exch.Code < aOptQuote2.Exch.Code Then
'                Cmp = -1
'            End If
'
'            If m_iSeriesOrder <> flexSortGenericAscending Then
'                Cmp = -Cmp
'            End If
'        End If
'
'        Set aOpt1 = Nothing
'        Set aOpt2 = Nothing
'        Set aOptQuote1 = Nothing
'        Set aOptQuote2 = Nothing
'    End If
'End Sub
'
'Private Sub imgStop_Click()
'    On Error Resume Next
'    If m_enState = enQsLastQuoteReq Then
'        m_enState = enQsReady
'        PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        lblProcess.Caption = ""
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'
'        UnderlyingUpdate False, True, True
'        OptionsUpdate False, True, True
'
'        UpdateTotals
'        AdjustState
'
'    ElseIf m_enState = enQsRealtime Then
'        m_enState = enQsReady
'
'    ElseIf m_enState = enQsLastMmQuoteReq Then
'        m_enState = enQsReady
'
'        MmManager.CancelLastMmQuote
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        lblProcess.Caption = ""
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'
'        UnderlyingUpdate False, True, True
'        OptionsUpdate False, True, True
'
'        UpdateTotals
'        AdjustState
'
'    ElseIf m_enState = enQsOptionsReq Then
'        m_enState = enQsReady
'
'        MsProvider.CancelOptions
'        ClearViewAndData
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        lblProcess.Caption = ""
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'
'        UpdateTotals
'        AdjustState
'
'    ElseIf m_enState = enQsStockListReq Then
'        m_enState = enQsReady
'
'        MsProvider.CancelAllStocks
'        m_sUndComboList = ""
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        lblProcess.Caption = ""
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        AdjustState
'
'    ElseIf m_enState = enQsDataLoad Then
'        m_enState = enQsReady
'        AdjustState
'
'    ElseIf m_bInProc Then
'        m_bInProc = False
'        AdjustState
'    End If
'End Sub
'
'Private Sub SaveDividendsInfo()
'    On Error GoTo EH
'    If m_Und.ID = 0 Then Exit Sub
'    Dim aGUnd As EtsGeneralLib.UndAtom
'
'    If m_Und.UseCustDivs Then
'        If IsValidDivs(True) Then
'            gDBW.usp_StockCustomDiv_Save m_Und.ID, 1, m_Und.DivAmtCust, _
'                                        m_Und.DivFreqCust, m_Und.DivDateCust
'
'            Set aGUnd = g_UnderlyingAll(m_Und.ID)
'            If Not aGUnd Is Nothing Then
'                aGUnd.UseCustDivs = True
'                aGUnd.DivAmtCust = m_Und.DivAmtCust
'                aGUnd.DivFreqCust = m_Und.DivFreqCust
'                aGUnd.DivDateCust = m_Und.DivDateCust
'                Set aGUnd = Nothing
'            End If
'        End If
'    Else
'        gDBW.usp_StockCustomDiv_Save m_Und.ID, 0
'        Set aGUnd = g_UnderlyingAll(m_Und.ID)
'        If Not aGUnd Is Nothing Then
'            aGUnd.UseCustDivs = False
'            Set aGUnd = Nothing
'        End If
'    End If
'
'    Exit Sub
'EH:
'    LogEventLocal EVENT_ERROR, "Fail to save dividends info for " & m_Und.Symbol & ". " & Err.Source & " " & Err.Description
'    Set aGUnd = Nothing
'End Sub
'
'Private Sub fgDiv_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim sValue$, nValue&, dValue#, dtValue As Date, bValue As Boolean, nIdx&, bNeedRecalc As Boolean
'    With fgDiv
'        bNeedRecalc = False
'        sValue = Trim$(.TextMatrix(Row, Col))
'        If m_sCurrentOriginalText <> sValue Then
'            If Col = UDC_STYLE Then ' option style
'                nValue = -1
'                nValue = CLng(sValue)
'                If nValue = 0 Or nValue = 1 Then
'                    If m_Und.IsAmerican <> CBool(nValue) Then
'                        m_Und.IsAmerican = CBool(nValue)
'                        bNeedRecalc = True
'                    End If
'                End If
'                ModelUpdate
'
'            ElseIf Col > UDC_CALENDAR And Col < UDC_RATE Then ' dividends
'                Select Case Col
'                    Case UDC_DIV
'                        bValue = (sValue <> "0")
'                        If m_Und.UseCustDivs <> bValue Then
'                            m_Und.UseCustDivs = bValue
'                            SaveDividendsInfo
'                            bNeedRecalc = True
'                        End If
'                        DivsUpdate
'
'                    Case UDC_FREQ
'                        nValue = -1
'                        nValue = ReadLng(sValue)
'                        If nValue = 0 Or nValue = 1 Or nValue = 2 Or nValue = 4 Or nValue = 12 Then
'                            If Not m_Und.UseCustDivs And m_Und.DivFreqCust <> nValue _
'                                Or m_Und.UseCustDivs And m_Und.DivFreqCust <> nValue Then
'
'                                m_Und.DivFreqCust = nValue
'                                If Not m_Und.UseCustDivs Then
'                                    m_Und.DivAmtCust = m_Und.DivAmt
'                                    m_Und.DivDateCust = m_Und.DivDate
'                                    m_Und.UseCustDivs = True
'                                End If
'                                bNeedRecalc = True
'                                SaveDividendsInfo
'                            End If
'                        End If
'                        DivsUpdate
'
'                    Case UDC_DATE
'                        dtValue = 0#
'                        dtValue = ReadDate(sValue)
'                        If dtValue > 0# Then
'                            If Not m_Und.UseCustDivs And m_Und.DivDate <> dtValue _
'                                Or m_Und.UseCustDivs And m_Und.DivDateCust <> dtValue Then
'
'                                m_Und.DivDateCust = dtValue
'                                If Not m_Und.UseCustDivs Then
'                                    m_Und.DivFreqCust = m_Und.DivFreq
'                                    m_Und.DivAmtCust = m_Und.DivAmt
'                                    m_Und.UseCustDivs = True
'                                End If
'                                bNeedRecalc = True
'                                SaveDividendsInfo
'                            End If
'                        End If
'                        DivsUpdate
'
'                    Case UDC_AMT
'                        dValue = 0#
'                        dValue = ReadDbl(sValue)
'                        If dValue > 0# Then
'                            If Not m_Und.UseCustDivs And m_Und.DivAmt <> dValue _
'                                Or m_Und.UseCustDivs And m_Und.DivAmtCust <> dValue Then
'
'                                m_Und.DivAmtCust = dValue
'                                If Not m_Und.UseCustDivs Then
'                                    m_Und.DivFreqCust = m_Und.DivFreq
'                                    m_Und.DivDateCust = m_Und.DivDate
'                                    m_Und.UseCustDivs = True
'                                End If
'                                bNeedRecalc = True
'                                SaveDividendsInfo
'                            End If
'                        End If
'                        DivsUpdate
'
'                End Select
'
'            Else ' rates
'                If Col > UDC_RATE Then
'                    nIdx = Col - UDC_RATE
'                    If nIdx >= 1 And nIdx <= m_Und.Expiry.Count Then
'                        dValue = 0#
'                        dValue = ReadDbl(sValue)
'                        dValue = dValue / 100#
'                        If dValue > 0# Then
'                            If Not m_Und.UseCustRates And m_Und.Expiry(nIdx).Rate <> dValue _
'                                Or m_Und.UseCustRates And m_Und.Expiry(nIdx).RateCust <> dValue Then
'
'                                m_Und.UseCustRates = True
'                                m_Und.Expiry(nIdx).RateCust = dValue
'                                bNeedRecalc = True
'                            End If
'                        End If
'                        RatesUpdate
'                    End If
'                Else
'                    bValue = (sValue <> "0")
'                    If m_Und.UseCustRates <> bValue Then
'                        m_Und.UseCustRates = bValue
'                        bNeedRecalc = True
'                    End If
'                    RatesUpdate
'                End If
'            End If
'
'            If bNeedRecalc Then
'                Recalculate False
'                .AutoSize Col, Col
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgDiv_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgDiv
'            m_enMenuGrid = GT_QUOTATION_DIVIDENDS
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgDiv_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Dim nKey&
'    Cancel = True
'
'    If IsDblClickHandled Then Exit Sub
'    If m_bInProc Or m_enState <> enQsReady Then Exit Sub
'    nKey = IIf(Col > UDC_RATE, UDC_RATE_VAL, Col)
'    Cancel = (m_Und.ID = 0 Or Not m_gdDiv.Col(nKey).CanEdit)
'
'    If Not Cancel Then
'        m_sCurrentOriginalText = Trim$(fgDiv.TextMatrix(Row, Col))
'    End If
'End Sub
'
'Private Sub fgOpt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim nKey&, sValue$, dValue#, aQuote As clsQnQuoteAtom, aQuote2 As clsQnQuoteAtom, bCalcGreeks As Boolean, nCount&, nCount2&, sKey$, nRow&, i&, i2&
'    Dim RowData As clsQnQuoteRowData, aOpt As clsQnOptAtom, aOpt2 As clsQnOptAtom, nOptType&, nOptType2&, bCalcIV As Boolean
'    Dim bCalcOpposite As Boolean
'
'    With fgOpt
'        sValue = Trim$(.TextMatrix(Row, Col))
'        If m_sCurrentOriginalText <> sValue Then
'            nKey = .ColKey(Col)
'            If Err.Number = 0 Then
'                bCalcGreeks = False
'                bCalcIV = False
'                bCalcOpposite = False
'                Set RowData = .RowData(Row)
'
'                If Not RowData Is Nothing Then
'                    Select Case nKey
'                        Case UOC_C_BID, UOC_C_ASK, UOC_C_VOLA
'                            nOptType = enOtCall
'                            nOptType2 = enOtPut
'
'                        Case UOC_P_BID, UOC_P_ASK, UOC_P_VOLA
'                            nOptType = enOtPut
'                            nOptType2 = enOtCall
'
'                    End Select
'
'                    Set aOpt = RowData.Opt(nOptType)
'                    Set aQuote = RowData.OptQuote(nOptType)
'
'                    If Not aOpt Is Nothing And Not aQuote Is Nothing Then
'                        dValue = Abs(ReadDbl(sValue))
'
'                        Select Case nKey
'                            Case UOC_C_BID, UOC_P_BID
'                                If aQuote.BidPrice <> dValue Then
'                                    If aQuote.BidPrice >= 0# Then aQuote.BidPriceTick = dValue - aQuote.BidPrice
'                                    aQuote.BidPrice = dValue
'                                    bCalcIV = True
'                                    bCalcGreeks = Not g_Params.UseTheoVolatility
'                                End If
'
'                            Case UOC_C_ASK, UOC_P_ASK
'                                If aQuote.AskPrice <> dValue Then
'                                    If aQuote.AskPrice >= 0# Then aQuote.AskPriceTick = dValue - aQuote.AskPrice
'                                    aQuote.AskPrice = dValue
'                                    bCalcIV = True
'                                    bCalcGreeks = Not g_Params.UseTheoVolatility
'                                End If
'
'                            Case UOC_C_VOLA, UOC_P_VOLA
'                                dValue = dValue / 100#
'                                If aOpt.Vola <> dValue Then
'                                    aOpt.Vola = dValue
'
'                                    Set aOpt2 = RowData.Opt(nOptType2)
'                                    Set aQuote2 = RowData.OptQuote(nOptType2)
'
'                                    If Not aOpt2 Is Nothing Then
'                                        aOpt2.Vola = dValue
'                                        bCalcOpposite = True
'                                    End If
'
'                                    bCalcGreeks = g_Params.UseTheoVolatility  '(aQuote.Exch.ID = 0)
'                                    aOpt.Underlying.VolaSrv.OptionVola(aOpt.Expiry, aOpt.Strike) = dValue
'
'                                    m_bVolaChangedNow = True
'                                    g_Params.FireInternalVolatilityChanged m_Und.Symbol
'                                    m_bVolaChangedNow = False
'                                End If
'                        End Select
'
'                        If bCalcIV Or bCalcGreeks Then
'                            Dim enCalcIV As CalcIvEnum, nMask&, enCalcIV2 As CalcIvEnum, nMask2&
'
'                            If bCalcIV Then
'                                enCalcIV = GetIvMask(nOptType)
'                                If bCalcOpposite Then enCalcIV2 = GetIvMask(nOptType2)
'                            Else
'                                enCalcIV = CALC_IV_NONE
'                                If bCalcOpposite Then enCalcIV2 = CALC_IV_NONE
'                            End If
'
'                            If bCalcGreeks Then
'                                nMask = GetGreeksMask(nOptType)
'                                If bCalcOpposite Then nMask2 = GetGreeksMask(nOptType2)
'                            Else
'                                nMask = GM_NONE
'                                If bCalcOpposite Then nMask2 = GM_NONE
'                            End If
'
'                            If bCalcGreeks Then
'                                nCount = aOpt.Quotes.Count
'                                If bCalcOpposite Then nCount2 = aOpt2.Quotes.Count
'                                If nCount > 1 Or bCalcOpposite And nCount2 > 1 Then
'                                    Set aQuote = Nothing
'                                    Set aQuote2 = Nothing
'
'                                    i = 1
'                                    i2 = 1
'
'                                    While i <= nCount Or bCalcOpposite And i2 <= nCount2
'                                        If i <= nCount Then
'                                            Set aQuote = aOpt.Quotes(i)
'
'                                            CalcOptionGreeks aOpt, aQuote, nMask, enCalcIV
'
'                                            If aQuote.Exch.Visible Then
'                                                sKey = aOpt.Symbol
'                                                If aQuote.Exch.ID > 0 Then
'                                                    sKey = sKey & "." & aQuote.Exch.Code
'                                                End If
'                                                nRow = fgOpt.FindRow(sKey, , IIf(nOptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'                                                If nRow > 0 Then
'                                                    OptionUpdateQuote nRow, nOptType, False, True
'                                                Else
'                                                    Debug.Assert False
'                                                End If
'                                            End If
'
'                                            Set aQuote = Nothing
'                                        End If
'
'                                        If bCalcOpposite And i2 <= nCount2 Then
'                                            Set aQuote2 = aOpt2.Quotes(i2)
'                                            CalcOptionGreeks aOpt2, aQuote2, nMask2, enCalcIV2
'
'                                            If aQuote2.Exch.Visible Then
'                                                sKey = aOpt2.Symbol
'                                                If aQuote2.Exch.ID > 0 Then
'                                                    sKey = sKey & "." & aQuote2.Exch.Code
'                                                End If
'                                                nRow = fgOpt.FindRow(sKey, , IIf(nOptType2 = enOtCall, UOC_C_KEY, UOC_P_KEY))
'                                                If nRow > 0 Then
'                                                    OptionUpdateQuote nRow, nOptType2, False, True
'                                                Else
'                                                    Debug.Assert False
'                                                End If
'                                            End If
'
'                                            Set aQuote2 = Nothing
'                                        End If
'
'                                        i = i + 1
'                                        i2 = i2 + 1
'                                    Wend
'                                Else
'                                    CalcOptionGreeks aOpt, aQuote, nMask, enCalcIV
'                                    OptionUpdateQuote Row, nOptType, False, True
'                                    If bCalcOpposite Then
'                                        CalcOptionGreeks aOpt2, aQuote2, nMask2, enCalcIV2
'                                        OptionUpdateQuote Row, nOptType2, False, True
'                                    End If
'                                End If
'
'                                CalculateUnderlyingOptions False
'                            Else
'                                CalcOptionGreeks aOpt, aQuote, nMask, enCalcIV
'                                OptionUpdateQuote Row, nOptType, False, True
'                                If bCalcOpposite Then
'                                    CalcOptionGreeks aOpt2, aQuote2, nMask2, enCalcIV2
'                                    OptionUpdateQuote Row, nOptType2, False, True
'                                End If
'                            End If
'                            UpdateTotals
'
''                            .AutoSize 0, .Cols - 1
'                        End If
'
'                        Set aOpt = Nothing
'                        Set aOpt2 = Nothing
'                        Set aQuote = Nothing
'                        Set aQuote2 = Nothing
'                    Else
'                        Debug.Assert False
'                    End If
'
'                    Set RowData = Nothing
'                Else
'                    Debug.Assert False
'                End If
'
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgOpt_AfterMoveColumn(ByVal Col As Long, Position As Long)
'    On Error Resume Next
'    Dim nIdx&, i&, nOldPos&, nNewPos&
'    If Col = Position Then Exit Sub
'
'    nOldPos = Col - UOC_C_SYMBOL
'    nNewPos = Position - UOC_C_SYMBOL
'    nIdx = m_gdOpt.Idx(nOldPos)
'    If nOldPos > nNewPos Then
'        For i = nOldPos To nNewPos Step -1
'            If i > nNewPos Then
'                m_gdOpt.Idx(i) = m_gdOpt.Idx(i - 1)
'            End If
'        Next
'        m_gdOpt.Idx(nNewPos) = nIdx
'    Else
'        For i = nOldPos To nNewPos
'            If i < nNewPos Then
'                m_gdOpt.Idx(i) = m_gdOpt.Idx(i + 1)
'            End If
'        Next
'        m_gdOpt.Idx(nNewPos) = nIdx
'    End If
'End Sub
'
'Private Sub fgOpt_BeforeMoveColumn(ByVal Col As Long, Position As Long)
'    On Error Resume Next
'    Dim nOldIdx&, nNewIdx&
'
'    nOldIdx = fgOpt.ColKey(Col)
'    nNewIdx = fgOpt.ColKey(Position)
'
'    If nOldIdx = UOC_NONE Or nNewIdx = UOC_NONE Then
'        Position = Col
'    ElseIf Not m_gdOpt.Col(nOldIdx).CanChangeOrder Or Not m_gdOpt.Col(nNewIdx).CanChangeOrder Then
'        Position = Col
'    End If
'End Sub
'
'Private Sub fgOpt_DblClick()
'    On Error Resume Next
'    With fgOpt
'        m_enMenuGrid = GT_QUOTATION_OPTIONS
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'        mnuCtxTradeNew_Click
'    End With
'End Sub
'
'Private Sub fgOpt_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If KeyCode = vbKeyInsert And Shift = 0 _
'        Or (KeyCode = vbKeyG Or KeyCode = vbKeyA Or KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
'        m_bKeyDown(GT_QUOTATION_OPTIONS) = True
'    End If
'End Sub
'
'Private Sub fgOpt_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_QUOTATION_OPTIONS) Then
'        m_bKeyDown(GT_QUOTATION_OPTIONS) = False
'        With fgOpt
'            m_enMenuGrid = GT_QUOTATION_OPTIONS
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        If KeyCode = vbKeyInsert And Shift = 0 Then
'            mnuCtxTradeNew_Click
'
'        ElseIf Shift = vbCtrlMask Then
'            Select Case KeyCode
'                Case vbKeyG
'                    mnuCtxAutosizeGrid_Click
'
'                Case vbKeyA
'                    If m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols Then mnuCtxAutosizeCol_Click
'
'                Case vbKeyC, vbKeyInsert
'                    mnuCtxCopy_Click
'            End Select
'        End If
'    End If
'End Sub
'
'Private Sub fgOpt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgOpt
'            m_enMenuGrid = GT_QUOTATION_OPTIONS
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'
'            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows And m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols Then
'                .Row = m_nMenuGridRow
'            End If
'
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgLog_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If (KeyCode = vbKeyG Or KeyCode = vbKeyA Or KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
'        m_bKeyDown(GT_QUOTATION_LOG) = True
'    End If
'End Sub
'
'Private Sub fgLog_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_QUOTATION_LOG) Then
'        m_bKeyDown(GT_QUOTATION_LOG) = False
'        With fgLog
'            m_enMenuGrid = GT_QUOTATION_LOG
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        If Shift = vbCtrlMask Then
'
'            Select Case KeyCode
'                Case vbKeyG
'                    mnuCtxAutosizeGrid_Click
'
'                Case vbKeyA
'                    If m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols Then mnuCtxAutosizeCol_Click
'
'                Case vbKeyC, vbKeyInsert
'                    mnuCtxCopy_Click
'            End Select
'
'        End If
'    End If
'End Sub
'
'Private Sub fgLog_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgLog
'            m_enMenuGrid = GT_QUOTATION_LOG
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'
'            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows And m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols Then
'                .Row = m_nMenuGridRow
'            End If
'
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub UpdateMenu()
'    On Error Resume Next
'    mnuCtxRealTime.Enabled = m_bIsConnected And m_Und.ID <> 0 And Not m_bInProc And (m_enState = enQsReady Or m_enState = enQsRealtime)
'    mnuCtxRealTime.Checked = (m_enState = enQsRealtime)
'    mnuCtxRefresh.Enabled = m_bIsConnected And m_Und.ID <> 0 And Not m_bInProc And m_enState = enQsReady
'    mnuCtxDefOptSort.Enabled = m_bIsConnected And m_Und.ID <> 0 And Not m_bInProc
'End Sub
'
'Private Sub ShowPopup()
'    On Error Resume Next
'    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
'
''    mnuCtxTradeNew          "New Trade..."
''    mnuCtxRealTime          "Real Time Mode"
''    mnuCtxRefresh           "Refresh"
''    mnuCtxCopy              "Copy Grid"
''    mnuCtxPrint             "Print Grid"
''    mnuCtxHideCol           "Hide Column"
''    mnuCtxAutosizeCol       "Autosize Column"
''    mnuCtxAutosizeGrid      "Autosize Grid"
''    mnuCtxDefOptSort        "Default Options Sorting"
''    mnuCtxGridLayout        "Grid Layout..."
''    mnuCtxClearLog          "Clear"
'
'    mnuCtxCopy.Enabled = Not m_bInProc
'
'    Select Case m_enMenuGrid
'        Case GT_QUOTATION_UNDERLYING
'            mnuCtxTradeNew.Enabled = False
'            mnuCtxHideCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeGrid.Enabled = True
'            mnuCtxClearLog.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_QUOTATION_OPTIONS
'            mnuCtxTradeNew.Enabled = m_bIsConnected And (m_Und.ID <> 0 And m_Und.IsTraderContract And m_nMenuGridRow > 0 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxHideCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols And m_gdOpt.Idx(1) <> UOC_NONE)
'            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeGrid.Enabled = True
'            mnuCtxClearLog.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_QUOTATION_DIVIDENDS
'            mnuCtxTradeNew.Enabled = False
'            mnuCtxHideCol.Enabled = False
'            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeGrid.Enabled = True
'            mnuCtxClearLog.Enabled = False
'
'            PopupMenu mnuCtx
'
'        Case GT_QUOTATION_LOG
'            mnuCtxTradeNew.Enabled = False
'            mnuCtxHideCol.Enabled = False
'            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeGrid.Enabled = True
'            mnuCtxClearLog.Enabled = True
'
'            PopupMenu mnuCtx
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
'Private Sub fgOpt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Dim RowData As clsQnQuoteRowData
'    Dim nKey&
'    Cancel = True
'
'    If IsDblClickHandled Then Exit Sub
'    If m_bInProc Or m_enState <> enQsReady Then Exit Sub
'
'    If m_Und.ID <> 0 Then
'        With fgOpt
'            nKey = .ColKey(Col)
'            If Err.Number = 0 Then
'                Cancel = Not m_gdOpt.Col(nKey).CanEdit
'                If Not Cancel Then
'                    Set RowData = .RowData(Row)
'                    If nKey < UOC_P_SYMBOL Then
'                        Cancel = (RowData.Opt(enOtCall) Is Nothing Or RowData.OptQuote(enOtCall) Is Nothing)
'                    Else
'                        Cancel = (RowData.Opt(enOtPut) Is Nothing Or RowData.OptQuote(enOtPut) Is Nothing)
'                    End If
'
'                    Set RowData = Nothing
'                End If
'
'                If Not Cancel Then
'                    Cancel = (Not m_Und.IsTraderContract And (nKey = UOC_C_VOLA Or nKey = UOC_P_VOLA))
'                End If
'            End If
'
'            If Not Cancel Then
'                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub fgUnd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim nKey&, sValue$, dValue#, aQuote As clsQnQuoteAtom, bNeedRecalc As Boolean
'    Dim RowData As clsQnQuoteRowData, aUnd As EtsGeneralLib.UndAtom
'
'    With fgUnd
'        nKey = .ColKey(Col)
'        If Err.Number = 0 Then
'            sValue = Trim$(.TextMatrix(Row, Col))
'
'            If m_sCurrentOriginalText <> sValue Then
'                bNeedRecalc = False
'                If nKey = UUC_SYMBOL Then
'                    Debug.Assert Row = 1
'                    sValue = UCase$(sValue)
'
'                    m_NewUnd.ID = 0
'                    Set aUnd = g_UnderlyingAllBySym(sValue)
'                    If Not aUnd Is Nothing Then
'                        m_NewUnd.ID = aUnd.ID
'                        m_NewUnd.Data = aUnd.UndType
'                        m_NewUnd.Data3 = IIf(aUnd.IsTraderContract, 1, 0)
'                        Set aUnd = Nothing
'                    End If
'
'                    If m_NewUnd.ID <> 0 Then
'                        .TextMatrix(Row, Col) = sValue
'
'                        m_VisibleUndExch.Clear
'                        If Not g_Params.QuoteUndExchAll Then
'                            m_VisibleUndExch.Add(CStr(g_Params.QuoteDefUndExchID)).ID = g_Params.QuoteDefUndExchID
'                        End If
'
'                        m_VisibleOptExch.Clear
'                        If Not g_Params.QuoteOptExchAll Then
'                            m_VisibleOptExch.Add(CStr(g_Params.QuoteDefOptExchID)).ID = g_Params.QuoteDefOptExchID
'                        End If
'
'                        tmrShow.Enabled = True
'                    Else
''                        If gCmn.MyMsgBox("There is no market structure for symbol '" & sValue & "'." & vbCrLf & _
''                                            "Update market structure for this symbol?", vbQuestion Or vbYesNo) = vbYes Then
''
''                            gCmn.MyMsgBox "This feature is not available in this preview release.", vbCritical
''                            .TextMatrix(Row, Col) = m_Und.Symbol
''                        Else
'                            .TextMatrix(Row, Col) = m_Und.Symbol
''                        End If
'                    End If
'                Else
'                    Set RowData = .RowData(Row)
'                    Debug.Assert Not RowData Is Nothing
'
'                    Select Case nKey
'                        Case UUC_BID
'                            Set aQuote = RowData.UndQuote
'                            If Not aQuote Is Nothing Then
'                                dValue = Abs(ReadDbl(sValue))
'                                If aQuote.BidPrice <> dValue Then
'                                    If aQuote.BidPrice >= 0# Then aQuote.BidPriceTick = dValue - aQuote.BidPrice
'                                    aQuote.BidPrice = dValue
'
'                                    .TextMatrix(Row, Col) = aQuote.BidPrice
'                                    UnderlyingUpdateQuoteColors Row, aQuote
'
'                                    bNeedRecalc = (RowData.UndQuote.Exch.ID = 0)
'                                End If
'                                Set aQuote = Nothing
'                            End If
'
'                        Case UUC_ASK
'                            Set aQuote = RowData.UndQuote
'                            If Not aQuote Is Nothing Then
'                                dValue = Abs(ReadDbl(sValue))
'                                If aQuote.AskPrice <> dValue Then
'                                    If aQuote.AskPrice >= 0# Then aQuote.AskPriceTick = dValue - aQuote.AskPrice
'                                    aQuote.AskPrice = dValue
'
'                                    .TextMatrix(Row, Col) = aQuote.AskPrice
'                                    UnderlyingUpdateQuoteColors Row, aQuote
'
'                                    bNeedRecalc = (RowData.UndQuote.Exch.ID = 0)
'                                End If
'                                Set aQuote = Nothing
'                            End If
'
'                    End Select
'
'                    If bNeedRecalc Then
''                        RowData.Und.VolaSrv.UnderlyingPrice = PriceMidEx(RowData.UndQuote.BidPrice, RowData.UndQuote.AskPrice, RowData.UndQuote.LastPrice)
'                        Debug.Assert (Not RowData.Und.UndPriceProfile Is Nothing)
'                        RowData.Und.VolaSrv.UnderlyingPrice = RowData.Und.UndPriceProfile.GetUndPriceMid(RowData.UndQuote.BidPrice, RowData.UndQuote.AskPrice, _
'                            RowData.UndQuote.LastPrice, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'                        Recalculate False
''                        .AutoSize Col, Col
'                    End If
'
'                    Set RowData = Nothing
'                End If
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgUnd_AfterMoveColumn(ByVal Col As Long, Position As Long)
'    On Error Resume Next
'    Dim nIdx&, i&, nOldPos&, nNewPos&
'    If Col = Position Then Exit Sub
'
'    nOldPos = Col - UUC_SYMBOL
'    nNewPos = Position - UUC_SYMBOL
'    nIdx = m_gdUnd.Idx(nOldPos)
'    If nOldPos > nNewPos Then
'        For i = nOldPos To nNewPos Step -1
'            If i > nNewPos Then
'                m_gdUnd.Idx(i) = m_gdUnd.Idx(i - 1)
'            End If
'        Next
'        m_gdUnd.Idx(nNewPos) = nIdx
'    Else
'        For i = nOldPos To nNewPos
'            If i < nNewPos Then
'                m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
'            End If
'        Next
'        m_gdUnd.Idx(nNewPos) = nIdx
'    End If
'End Sub
'
'Private Sub fgUnd_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Dim nIdx&
'
'    With fgUnd
'        nIdx = .ColKey(Col)
'        If Row <> 1 Or nIdx <> UUC_SYMBOL Then
'            .ComboList = ""
'        Else
'            .ComboList = m_sUndComboList
'            .EditMaxLength = 20
'        End If
'
'    End With
'End Sub
'
'Private Sub fgUnd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
'    On Error Resume Next
'    Dim nOldIdx&, nNewIdx&
'
'    nOldIdx = fgUnd.ColKey(Col)
'    nNewIdx = fgUnd.ColKey(Position)
'
'    If nOldIdx = UUC_NONE Or nNewIdx = UUC_NONE Then
'        Position = Col
'    ElseIf Not m_gdUnd.Col(nOldIdx).CanChangeOrder Or Not m_gdUnd.Col(nNewIdx).CanChangeOrder Then
'        Position = Col
'    End If
'End Sub
'
'Private Sub fgUnd_DblClick()
'    On Error Resume Next
'    With fgUnd
'        m_enMenuGrid = GT_QUOTATION_UNDERLYING
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'        mnuCtxTradeNew_Click
'    End With
'End Sub
'
'Private Sub fgUnd_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If KeyCode = vbKeyInsert And Shift = 0 _
'        Or (KeyCode = vbKeyG Or KeyCode = vbKeyA Or KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
'        m_bKeyDown(GT_QUOTATION_UNDERLYING) = True
'    End If
'End Sub
'
'Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_QUOTATION_UNDERLYING) Then
'        m_bKeyDown(GT_QUOTATION_UNDERLYING) = False
'        With fgUnd
'            m_enMenuGrid = GT_QUOTATION_UNDERLYING
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        If KeyCode = vbKeyInsert And Shift = 0 Then
'            mnuCtxTradeNew_Click
'
'        ElseIf Shift = vbCtrlMask Then
'            Select Case KeyCode
'                Case vbKeyG
'                    mnuCtxAutosizeGrid_Click
'
'                Case vbKeyA
'                    If m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then mnuCtxAutosizeCol_Click
'
'                Case vbKeyC, vbKeyInsert
'                    mnuCtxCopy_Click
'            End Select
'        End If
'    End If
'End Sub
'
'Private Sub fgUnd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgUnd
'            m_enMenuGrid = GT_QUOTATION_UNDERLYING
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'
'            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
'                .Row = m_nMenuGridRow
'            End If
'
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgUnd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Dim nKey&
'    Cancel = True
'
'    If IsDblClickHandled Then Exit Sub
'    If m_bInProc Or (m_enState <> enQsReady And m_enState <> enQsRealtime) Then Exit Sub
'
'    With fgUnd
'        nKey = .ColKey(Col)
'        If Err.Number = 0 Then
'            If m_Und.ID <> 0 Then
'                Cancel = Not m_gdUnd.Col(nKey).CanEdit Or (Row <> 1 And nKey = UUC_SYMBOL) Or (nKey <> UUC_SYMBOL And m_enState = enQsRealtime)
'            Else
'                Cancel = Not (Row = 1 And nKey = UUC_SYMBOL)
'            End If
'        End If
'
'        If Not Cancel Then
'            If nKey <> UUC_SYMBOL Then
'                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
'            Else
'                m_sCurrentOriginalText = Trim$(.Cell(flexcpTextDisplay, Row, Col))
'            End If
'        End If
'    End With
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
'        Case GT_QUOTATION_UNDERLYING
'            gdGrid.CopyTo m_gdUnd
'
'            m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'            FormatUndGrid
'            FormatUndColumns False, False
'            UnderlyingUpdate True, True, True
'            m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'
'        Case GT_QUOTATION_DIVIDENDS
'            gdGrid.CopyTo m_gdDiv
'
'            m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'            'FormatDivGrid
'            FormatDivColumns
'            m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'
'        Case GT_QUOTATION_OPTIONS
'            Dim nCallMask&, nPutMask&, enCallIV As CalcIvEnum, enPutIV As CalcIvEnum
'            nCallMask = GetGreeksMask(enOtCall)
'            nPutMask = GetGreeksMask(enOtPut)
'
'            enCallIV = GetIvMask(enOtCall)
'            enPutIV = GetIvMask(enOtPut)
'
'            gdGrid.CopyTo m_gdOpt
'
'            m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'            FormatOptGrid
'            FormatOptColumns False, False
'
'            If mnuCtxRefresh.Enabled _
'                And (nCallMask <> GetGreeksMask(enOtCall) Or _
'                    nPutMask <> GetGreeksMask(enOtPut) Or _
'                    enCallIV <> GetIvMask(enOtCall) Or _
'                    enPutIV <> GetIvMask(enOtPut)) Then
'
'                Recalculate True
'            Else
'                OptionsUpdate True, True, True
'            End If
'
'            m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'
'        Case GT_QUOTATION_LOG
'            gdGrid.CopyTo m_gdDiv
'
'            m_GridLock(GT_QUOTATION_LOG).LockRedraw
'            FormatDivColumns
'            m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
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
'        Case GT_QUOTATION_UNDERLYING
'            With fgUnd
'                m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'                If m_nMenuGridCol <> 1 Then
'                    .AutoSize m_nMenuGridCol, m_nMenuGridCol
'                Else
'                    .AutoSize m_nMenuGridCol, m_nMenuGridCol, , 300
'                End If
'
'                nIdx = .ColKey(m_nMenuGridCol)
'                If nIdx >= UUC_SYMBOL And nIdx <= UUC_LAST_COLUMN Then
'                    m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
'                End If
'
'                m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'            End With
'
'        Case GT_QUOTATION_DIVIDENDS
'            With fgDiv
'                m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'                m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'            End With
'
'        Case GT_QUOTATION_OPTIONS
'            With fgOpt
'                m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'
'                nIdx = .ColKey(m_nMenuGridCol)
'                If nIdx >= UOC_C_SYMBOL And nIdx <= UOC_LAST_COLUMN Then
'                    m_gdOpt.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
'                End If
'
'                m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'            End With
'
'        Case GT_QUOTATION_LOG
'            With fgLog
'                m_GridLock(GT_QUOTATION_LOG).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'                m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
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
'        Case GT_QUOTATION_UNDERLYING
'            With fgUnd
'                m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'                .AutoSize 1, 2, , 300
'                .AutoSize 2, .Cols - 1
'
'                nCount = .Cols - 1
'                For i = 0 To nCount
'                    nIdx = .ColKey(i)
'                    If nIdx >= UUC_SYMBOL And nIdx <= UUC_LAST_COLUMN Then
'                        m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
'                    End If
'                Next
'
'                m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'            End With
'
'        Case GT_QUOTATION_DIVIDENDS
'            With fgDiv
'                m_GridLock(GT_QUOTATION_DIVIDENDS).LockRedraw
'                .AutoSize 0, .Cols - 1
'                m_GridLock(GT_QUOTATION_DIVIDENDS).UnlockRedraw
'            End With
'
'        Case GT_QUOTATION_OPTIONS
'            With fgOpt
'                m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'                .AutoSize 0, .Cols - 1
'
'                nCount = .Cols - 1
'                For i = 0 To nCount
'                    nIdx = .ColKey(i)
'                    If nIdx >= UOC_C_SYMBOL And nIdx <= UOC_LAST_COLUMN Then
'                        m_gdOpt.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
'                    End If
'                Next
'
'                m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'            End With
'
'        Case GT_QUOTATION_LOG
'            With fgLog
'                m_GridLock(GT_QUOTATION_LOG).LockRedraw
'                .AutoSize 0, .Cols - 1
'                m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
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
'        Case GT_QUOTATION_LOG
'            ClearLog
'
'    End Select
'
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxCopy_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_QUOTATION_UNDERLYING
'            g_ClipMgr.CopyGridToClipboard fgUnd
'
'        Case GT_QUOTATION_DIVIDENDS
'            g_ClipMgr.CopyGridToClipboard fgDiv
'
'        Case GT_QUOTATION_OPTIONS
'            g_ClipMgr.CopyGridToClipboard fgOpt
'
'        Case GT_QUOTATION_LOG
'            g_ClipMgr.CopyGridToClipboard fgLog
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxDefOptSort_Click()
'    On Error Resume Next
'    SortOptionsByDefault
'End Sub
'
'Public Sub SortOptionsByDefault()
'    On Error Resume Next
'    If m_Und.ID = 0 Or m_bInProc Then Exit Sub
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'        Screen.MousePointer = vbHourglass
'
'        m_nSortSeriesColKey = UOC_C_SERIES
'        m_iSeriesOrder = flexSortGenericAscending
'
'        .Sort = flexSortCustom
'        geOpt.ShowSortImage .ColIndex(UOC_C_SERIES), 1
'
'        m_nSortSeriesColKey = -2
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'        Screen.MousePointer = vbDefault
'    End With
'End Sub
'
'Private Sub mnuCtxGridLayout_Click()
'    On Error Resume Next
'    Select Case m_enMenuGrid
'        Case GT_QUOTATION_UNDERLYING
'            CustomizeUnderlyingGridLayout
'
'        Case GT_QUOTATION_OPTIONS
'            CustomizeOptionGridLayout
'
'        Case GT_QUOTATION_DIVIDENDS
'            CustomizeDivGridLayout
'
'        Case GT_QUOTATION_LOG
'            CustomizeLogGridLayout
'
'    End Select
'End Sub
'
'Private Sub mnuCtxHideCol_Click()
'    On Error Resume Next
'    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
'
'    Dim i&, nColIdx&, bMove As Boolean
'
'    Select Case m_enMenuGrid
'        Case GT_QUOTATION_UNDERLYING
'            bMove = False
'
'            nColIdx = fgUnd.ColKey(m_nMenuGridCol)
'            For i = 1 To UUC_LAST_COLUMN ' skip UUC_SYMBOL
'                If m_gdUnd.Idx(i) = nColIdx Then
'                    m_gdUnd.Idx(i) = UUC_NONE
'                    m_gdUnd.Col(nColIdx).Visible = False
'                    bMove = True
'                End If
'
'                If bMove Then
'                    If i + 1 <= UUC_LAST_COLUMN Then
'                        m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
'                    Else
'                        m_gdUnd.Idx(i) = UUC_NONE
'                    End If
'                End If
'            Next
'
'            If bMove Then
'                FormatUndGrid
'                FormatUndColumns False, False
'                UnderlyingUpdate True, True, True
'            End If
'
'        Case GT_QUOTATION_OPTIONS
'            bMove = False
'
'            If m_gdOpt.Idx(1) = UOC_NONE Then Exit Sub
'
'            nColIdx = fgOpt.ColKey(m_nMenuGridCol)
'            For i = 0 To UOC_LAST_COLUMN
'                If m_gdOpt.Idx(i) = nColIdx Then
'                    m_gdOpt.Idx(i) = UOC_NONE
'                    m_gdOpt.Col(nColIdx).Visible = False
'                    bMove = True
'                End If
'
'                If bMove Then
'                    If i + 1 <= UOC_LAST_COLUMN Then
'                        m_gdOpt.Idx(i) = m_gdOpt.Idx(i + 1)
'                    Else
'                        m_gdOpt.Idx(i) = UOC_NONE
'                    End If
'                End If
'            Next
'
'            If bMove Then
'                FormatOptGrid
'                FormatOptColumns False, False
'                OptionsUpdate True, True, True
'            End If
'
'    End Select
'End Sub
'
'Private Sub mnuCtxTradeNew_Click()
'    On Error GoTo EH
'    If m_bInProc Or m_nMenuGridCol < 0 Or m_nMenuGridRow < 1 Or m_Und.ID = 0 Or Not m_Und.IsTraderContract Then Exit Sub
'    Dim RowData As clsQnQuoteRowData, nColIdx&, bBuy As Boolean, aOpt As clsQnOptAtom
'
'    m_bInProc = True
'
'    Select Case m_enMenuGrid
''        Case GT_QUOTATION_UNDERLYING
''            m_bInProc = False
''            frmTradeEditor.NewTrade m_Und.ID, m_Und.ID, g_Params.QuoteOtherColBuy, m_frmOwner
'
'        Case GT_QUOTATION_OPTIONS
'            On Error Resume Next
'            Set RowData = fgOpt.RowData(m_nMenuGridRow)
'            On Error GoTo EH
'            nColIdx = fgOpt.ColKey(m_nMenuGridCol)
'            If Not RowData Is Nothing Then
'                Set aOpt = RowData.Opt(IIf(nColIdx < UOC_P_SYMBOL, enOtCall, enOtPut))
'                If Not aOpt Is Nothing Then
'
'                    If nColIdx = UOC_C_BID Or nColIdx = UOC_P_BID Then
'                        bBuy = (g_Params.QuoteBidColBuy <> 0)
'                    ElseIf nColIdx = UOC_C_ASK Or nColIdx = UOC_P_ASK Then
'                        bBuy = (g_Params.QuoteBidColBuy = 0)
'                    Else
'                        bBuy = (g_Params.QuoteOtherColBuy <> 0)
'                    End If
'
'                    m_bInProc = False
'
'                    frmOrderEditor.NewOrder m_Und, aOpt.ID, bBuy, m_frmOwner
'
'                    Set aOpt = Nothing
'                End If
'                Set RowData = Nothing
'            End If
'
'    End Select
'
'    ResetMenuData
'    m_bInProc = False
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to create new order."
'    ResetMenuData
'End Sub
'
'Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_QUOTATION_UNDERLYING
'            frmPrintPreview.Execute m_frmOwner, m_Und.Symbol & " ISE Quotes", "ISE Market Maker Underlying", fgUnd
'
'        Case GT_QUOTATION_DIVIDENDS
'            frmPrintPreview.Execute m_frmOwner, m_Und.Symbol & " ISE Calclulation Params", "ISE Market Maker Calculation Params", fgDiv
'
'        Case GT_QUOTATION_OPTIONS
'            frmPrintPreview.Execute m_frmOwner, m_Und.Symbol & " ISE Options Quotes", "ISE Market Maker Options Quotes", fgOpt
'
'        Case GT_QUOTATION_LOG
'            frmPrintPreview.Execute m_frmOwner, "ISE Market Maker Log", "ISE Market Maker Log", fgLog
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxRealTime_Click()
'    On Error Resume Next
'    StartRealTime IIf(m_enState = enQsRealtime, False, True)
'End Sub
'
'Private Sub mnuCtxRefresh_Click()
'    On Error Resume Next
'    Refresh
'End Sub
'
'Private Sub tmrShow_Timer()
'    On Error Resume Next
'    Dim bSucceed As Boolean, aNewUnd As clsQnUndAtom
'
'    tmrShow.Enabled = False
'    If m_NewUnd.ID <> 0 Then
'        If m_bInProc Or m_NewUnd.Data <> enCtStock And m_NewUnd.Data <> enCtIndex Then Exit Sub
'
'        Screen.MousePointer = vbArrow
'
'        If m_Und.ID <> 0 Then
'            If Not PriceProvider Is Nothing Then
'                If m_enState = enQsLastQuoteReq Then PriceProvider.CancelLastQuote
'                If m_enState = enQsRealtime Then PriceProvider.UnSubscribeQuote
'                If m_bGroupRequest Then
'                    m_GroupPriceProvider.CancelGroup
'                End If
'            End If
'
'            If Not MsProvider Is Nothing Then
'                If m_enState = enQsOptionsReq Then MsProvider.CancelOptions
'                If m_enState = enQsStockListReq Then MsProvider.CancelStock
'            End If
'
'            If Not MmManager Is Nothing Then
'                If m_enState = enQsLastMmQuoteReq Or m_enState = enQsRealtime Then MmManager.CancelLastMmQuote
'            End If
'
'            m_enState = enQsReady
'            m_QuoteRequests.Clear
'        End If
'
'        ClearViewAndData
'
'        bSucceed = False
'
'        Set aNewUnd = m_UndList(CStr(m_NewUnd.ID))
'
'        If Not aNewUnd Is Nothing Then
'            If UnderlyingLoad(aNewUnd, (m_NewUnd.Data3 <> 0)) Then
'                If UnderlyingShow Then
'                    RaiseEvent OnUndExchangesChange(False)
'
'                    fgUnd.Row = 1
'                    fgUnd.Col = UUC_SYMBOL
'                    fgUnd.SetFocus
'
'                    UpdateTotals
'                    RequestOptions
'
'                    bSucceed = True
'                End If
'            End If
'        End If
'
'        If Not bSucceed Then
'            pbProgress.Visible = False
'            lblStatus.Visible = True
'            lblProcess.Visible = False
'            lblProcess.Caption = ""
'            imgStop.Visible = False
'            imgStopDis.Visible = False
'
'            UpdateTotals
'            ClearViewAndData
'
'            fgUnd.Row = 1
'            fgUnd.Col = UUC_SYMBOL
'            fgUnd.SetFocus
'            fgUnd.TextMatrix(1, UUC_SYMBOL) = ""
'
'            AdjustCaption
'        End If
'        m_NewUnd.ID = 0
'
'        Screen.MousePointer = vbDefault
'    End If
'End Sub
'
'Private Sub ClearViewAndData()
'    On Error Resume Next
'    fgUnd.Rows = 2
'    fgUnd.Cell(flexcpText, 1, UUC_SYMBOL + 1, 1, fgUnd.Cols - 1) = ""
'    lblValue.Caption = ""
'
'    fgOpt.Rows = 1
'
'    FormatUndGrid
'    FormatUndColumns True, False
'    FormatOptGrid
'    FormatOptColumns True, False
'
'    With fgDiv
'        .Cols = UDC_LAST_COLUMN
'        .TextMatrix(1, UDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
'        .TextMatrix(1, UDC_STYLE) = "1"
'        .TextMatrix(1, UDC_FREQ) = "0"
'        .TextMatrix(1, UDC_DIV) = "0"
'        .TextMatrix(1, UDC_FREQ) = "0"
'        .TextMatrix(1, UDC_DATE) = ""
'        .TextMatrix(1, UDC_AMT) = ""
'        .TextMatrix(1, UDC_RATE) = "0"
'
'        .ColHidden(UDC_DATE) = True
'        .ColHidden(UDC_AMT) = True
'    End With
'
'    FormatDivGrid
'    FormatDivColumns
'
'    Set m_UndExch = Nothing
'    Set m_OptExch = Nothing
'    Set m_Und = Nothing
'    Set m_InOpt = Nothing
'
'    Set m_Und = New clsQnUndAtom
'    Set m_UndExch = New EtsGeneralLib.ExchColl
'    Set m_OptExch = New EtsGeneralLib.ExchColl
'
'    Set m_InOpt = New clsQnOptColl
'
'    AdjustCaption
'    RaiseEvent OnExpiriesChange(False)
'    RaiseEvent OnUndExchangesChange(False)
'    UserControl_Resize
'End Sub
'
''Private Sub UpdateCurrentValue()
''    On Error Resume Next
'''    Dim sValue$, dValue#, sCaption$
'''
'''    lblValue.Caption = ""
'''    With fgOpt
'''        If .Row > 0 And .Col > UOC_P_KEY Then
'''            sValue = .TextMatrix(.Row, .Col)
'''            If sValue <> "" Then
'''                sCaption = .TextMatrix(0, .Col) & ": "
'''                If sValue <> STR_NA Then
'''                    Select Case .ColDataType(.Col) =
'''                    lblValue.Caption = .TextMatrix(0, .Col) & ": " & Format$(.ValueMatrix(.Row, .Col), "#,##0.##############################")
'''                Else
'''                    lblValue.Caption = sCaption & "N/A"
'''                End If
'''            End If
'''        End If
'''    End With
''End Sub
'
'Private Sub RequestStocksList()
'    On Error GoTo EH
'    If m_bInProc Or Not m_bIsConnected Or m_enState <> enQsReady Then
'        Debug.Assert False
'        Exit Sub
'    End If
'
'    If Not MsProvider Is Nothing Then
'        m_bInProc = True
'        m_enState = enQsStockListReq
'        AdjustState
'
'        m_UndList.Clear
'
'        pbProgress.Min = 0
'        pbProgress.Max = QN_MAX_STOCKS
'        pbProgress.Value = 0
'        pbProgress.Visible = True
'
'        lblStatus.Visible = False
'        lblProcess.Visible = True
'        lblProcess.Caption = "Stocks list request..."
'        imgStop.Visible = True
'        imgStopDis.Visible = False
'        DoEvents
'
'        MsProvider.RequestAllStocks
'
'        m_bInProc = False
'        AdjustState
'    Else
'        gCmn.MyMsgBox m_frmOwner, "You are not connected to market data provider.", vbExclamation
'    End If
'    Exit Sub
'EH:
'    m_enState = enQsReady
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to request stocks list."
'    On Error Resume Next
'
'    MsProvider.CancelAllStocks
'
'    pbProgress.Visible = False
'    lblStatus.Visible = True
'    lblProcess.Visible = False
'    lblProcess.Caption = ""
'    imgStop.Visible = False
'    imgStopDis.Visible = False
'    m_bInProc = False
'    AdjustState
'End Sub
'
'Private Sub RequestOptions()
'    On Error GoTo EH
'    Dim aParam As ISEPROVIDERLib.OptionParams
'    If m_bInProc Or Not m_bIsConnected Or m_enState <> enQsReady Then
'        Debug.Assert False
'        Exit Sub
'    End If
'
'    If Not MsProvider Is Nothing Then
'
'        If m_Und.ID <> 0 Then
'            m_bInProc = True
'            m_enState = enQsOptionsReq
'            AdjustState
'
'            pbProgress.Min = 0
'            pbProgress.Max = QN_MAX_OPTIONS
'            pbProgress.Value = 0
'            pbProgress.Visible = True
'
'            lblStatus.Visible = False
'            lblProcess.Visible = True
'            lblProcess.Caption = "Options list request..."
'            imgStop.Visible = True
'            imgStopDis.Visible = False
'            DoEvents
'
'            aParam.Exchange = ""
'            aParam.UnderlyingSymbol = m_Und.Symbol
'
'            MsProvider.RequestOptions aParam
'
'            m_bInProc = False
'            AdjustState
'        End If
'    Else
'        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are not connected to market data provider.", vbExclamation
'    End If
'    Exit Sub
'EH:
'    m_enState = enQsReady
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request options list."
'    On Error Resume Next
'
'    MsProvider.CancelOptions
'
'    pbProgress.Visible = False
'    lblStatus.Visible = True
'    lblProcess.Visible = False
'    lblProcess.Caption = ""
'    imgStop.Visible = False
'    imgStopDis.Visible = False
'    m_bInProc = False
'    AdjustState
'End Sub
'
'Private Sub MmManager_OnEvent(ByVal EventNumber As ISEPROVIDERLib.EventNumberEnum, ByVal Description As String)
'    On Error Resume Next
'    Select Case EventNumber
'        Case enEvConnected
'            m_bIsConnected = True
'            AdjustState
'
'            If Not m_bUndListGot And m_enState = enQsReady Then
'                RequestStocksList
'            End If
'
'    End Select
'End Sub
'
'Private Sub MmTracer_OnMessage(ByVal MessageType As ISEPROVIDERLib.ISEInfoTypeEnum, ByVal Text As String)
'    On Error Resume Next
'    Dim enType As EventTypeEnum
'
'    Select Case MessageType
'        Case enWarning
'            enType = EVENT_WARNING
'
'        Case enFatal, enError
'            enType = EVENT_ERROR
'
'        Case Else
'            enType = EVENT_INFO
'    End Select
'
'    LogEventLocal enType, Text
'End Sub
'
'Private Sub MsProvider_OnError(ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As ISEPROVIDERLib.RequestsTypeEnum, ByVal Request As Variant)
'    On Error Resume Next
'    If ReqType = enRequestOptions Then
'        If m_enState <> enQsOptionsReq Then Exit Sub
'
'        If Request.UnderlyingSymbol = m_Und.Symbol Then
'            LogEventLocal EVENT_WARNING, m_Und.Symbol & ": " & Description
'        Else
'            If Len(Request.UnderlyingSymbol) > 0 Then
'                LogEventLocal EVENT_WARNING, Request.UnderlyingSymbol & ": " & Description
'            Else
'                LogEventLocal EVENT_WARNING, Request.UnderlyingSymbol & ": " & Description
'            End If
'        End If
'
'    ElseIf ReqType = enRequestStock Then
'        If m_enState <> enQsStockListReq Then Exit Sub
'
'        If Len(Request.UnderlyingSymbol) > 0 Then
'            LogEventLocal EVENT_WARNING, Request.UnderlyingSymbol & ": " & Description
'        Else
'            LogEventLocal EVENT_WARNING, Request.UnderlyingSymbol & ": " & Description
'        End If
'    Else
'        LogEventLocal EVENT_ERROR, Description
'    End If
'End Sub
'
'Private Sub MsProvider_OnStock(Params As ISEPROVIDERLib.StockParams, Results As ISEPROVIDERLib.ISEStockResults, ByVal IsLastStock As Boolean)
'    On Error Resume Next
'    If m_enState <> enQsStockListReq Then Exit Sub
'
'    Dim aGUnd As EtsGeneralLib.UndAtom, aUnd As clsQnUndAtom
'
'    If Not IsLastStock Then
'        Set aGUnd = g_UnderlyingAllBySym(Params.Stock)
'        If Not aGUnd Is Nothing Then
'            Set aUnd = m_UndList(CStr(aGUnd.ID))
'
'            If aUnd Is Nothing Then
'                Set aUnd = m_UndList.Add(CStr(aGUnd.ID))
'                aUnd.ID = aGUnd.ID
'
'                aUnd.UndType = aGUnd.UndType
'                aUnd.Symbol = aGUnd.Symbol
'
'                If aUnd.UndType = enCtStock Then
'                    aUnd.DivFreq = aGUnd.DivFreq
'                    aUnd.DivDate = aGUnd.DivDate
'                    aUnd.DivAmt = aGUnd.DivAmt
'
'                    aUnd.UseCustDivs = aGUnd.UseCustDivs
'
'                    aUnd.DivFreqCust = aGUnd.DivFreqCust
'                    aUnd.DivDateCust = aGUnd.DivDateCust
'                    aUnd.DivAmtCust = aGUnd.DivAmtCust
'                Else
'                    Set aUnd.BasketIndex = g_Index(aUnd.ID)
'                    If Not aUnd.BasketIndex Is Nothing Then
'                        If Not aUnd.BasketIndex.IsBasket Then Set aUnd.BasketIndex = Nothing
'                    End If
'
'                    aUnd.Yield = aGUnd.Yield
'                End If
'
'                aUnd.IsAmerican = aGUnd.IsAmerican
'                aUnd.IsHTB = aGUnd.IsHTB
'                aUnd.ExpCalendarID = aGUnd.ExpCalendarID
'                aUnd.Skew = aGUnd.Skew
'                aUnd.Kurt = aGUnd.Kurt
'
'                aUnd.BinNum = Results.BinNum
'                aUnd.TraderRole = Results.TraderRole
'
'                aUnd.Quotes.Add("0").LotSize = Results.LotSize
'
'                If m_UndList.Count <= QN_MAX_STOCKS Then
'                    IncProgress pbProgress
'                End If
'            End If
'
'            Set aUnd = Nothing
'        End If
'
'        Set aGUnd = Nothing
'    Else
'        m_bUndListGot = True
'        m_enState = enQsReady
'        pbProgress.Value = pbProgress.Max
'
'        SortStocksList
'        InitUndList
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        lblProcess.Caption = ""
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        m_bInProc = False
'        AdjustState
'    End If
'End Sub
'
'Private Sub MsProvider_OnOption(Params As ISEPROVIDERLib.OptionParams, Results As ISEPROVIDERLib.OptionResults, ByVal IsLastOption As Boolean)
'    On Error Resume Next
'    If m_enState <> enQsOptionsReq Then Exit Sub
'    If Params.UnderlyingSymbol <> m_Und.Symbol Then Exit Sub
'
'    Dim aOpt As clsQnOptAtom, aQuote As clsQnQuoteAtom
'
'    If Not IsLastOption Then
'        Set aOpt = m_InOpt(Results.Series)
'        If aOpt Is Nothing Then
'            Set aOpt = m_InOpt.Add(Results.Series)
'
'            aOpt.ID = m_InOpt.Count
'            aOpt.Symbol = Results.Series
'            aOpt.Expiry = Results.ExpirationDate
'            aOpt.Strike = Round(Results.StrikePrice, STRIKE_DECIMALS_COUNT)
'            aOpt.OptType = IIf(Results.OptionType = enCall, enOtCall, enOtPut)
'
'            Set aQuote = aOpt.Quotes.Add(CStr(m_OptExch(0).ID))
'            Set aQuote.Exch = m_OptExch(0)
'
'            aQuote.LotSize = Results.LotSize
'            If Results.LotSize <= 0 Then
'                aQuote.LotSize = 100
'            End If
'
'
'            If m_InOpt.Count <= QN_MAX_OPTIONS Then
'                IncProgress pbProgress
'            End If
'
'            Set aQuote = Nothing
'        End If
'        Set aOpt = Nothing
'    Else
'        m_enState = enQsReady
'        pbProgress.Value = 0
'        pbProgress.Max = m_InOpt.Count
'
'        SortInOptions
'        UnderlyingOptionsLoad m_InOpt
'        UnderlyingOptionsShow
'
'        RaiseEvent OnExpiriesChange(False)
'
'        RequestLastQuotes
'    End If
'End Sub
'
'Private Sub SortStocksList()
'    On Error Resume Next
'    Dim arrUnd() As clsQnUndAtom, nCount&, i&
'
'    nCount = m_UndList.Count
'    If nCount > 1 Then
'        ReDim arrUnd(1 To nCount)
'
'        For i = 1 To nCount
'            Set arrUnd(i) = m_UndList(i)
'        Next
'
'        m_UndList.Clear
'
'        DoSortStocksList arrUnd, 1, nCount
'
'        For i = 1 To nCount
'            m_UndList.Add CStr(arrUnd(i).ID), arrUnd(i)
'        Next
'
'        Erase arrUnd
'    End If
'End Sub
'
'Private Sub DoSortStocksList(ByRef arrUnd() As clsQnUndAtom, ByVal nLBound As Long, ByVal nUBound As Long)
'    Dim aTemp As clsQnUndAtom, aBuffer As clsQnUndAtom, nCurLow&, nCurHigh&, nCurMidPoint&
'
'    nCurLow = nLBound
'    nCurHigh = nUBound
'    If nUBound <= nLBound Then Exit Sub
'    nCurMidPoint = (nLBound + nUBound) \ 2
'    Set aTemp = arrUnd(nCurMidPoint)
'
'    Do While nCurLow <= nCurHigh
'
'        Do While arrUnd(nCurLow).Symbol < aTemp.Symbol
'            nCurLow = nCurLow + 1
'            If nCurLow = nUBound Then Exit Do
'        Loop
'
'        Do While aTemp.Symbol < arrUnd(nCurHigh).Symbol
'            nCurHigh = nCurHigh - 1
'            If nCurHigh = nLBound Then Exit Do
'        Loop
'
'        If nCurLow <= nCurHigh Then
'            Set aBuffer = arrUnd(nCurLow)
'            Set arrUnd(nCurLow) = arrUnd(nCurHigh)
'            Set arrUnd(nCurHigh) = aBuffer
'            nCurLow = nCurLow + 1
'            nCurHigh = nCurHigh - 1
'        End If
'    Loop
'
'    If nLBound < nCurHigh Then
'        DoSortStocksList arrUnd, nLBound, nCurHigh
'    End If
'
'    If nCurLow < nUBound Then
'        DoSortStocksList arrUnd, nCurLow, nUBound
'    End If
'End Sub
'
'Private Sub SortInOptions()
'    On Error Resume Next
'    Dim arrOpt() As clsQnOptAtom, nCount&, i&, aOpt As clsQnOptAtom
'
'    nCount = m_InOpt.Count
'    If nCount > 1 Then
'        ReDim arrOpt(1 To nCount)
'
'        For i = 1 To nCount
'            Set arrOpt(i) = m_InOpt(i)
'        Next
'
'        m_InOpt.Clear
'
'        DoSortInOptions arrOpt, 1, nCount
'
'        For i = 1 To nCount
'            Set aOpt = arrOpt(i)
'            m_InOpt.Add aOpt.Symbol, aOpt
'            Set aOpt = Nothing
'        Next
'
'        Erase arrOpt
'    End If
'End Sub
'
'Private Function InOptLess(aOpt1 As clsQnOptAtom, aOpt2 As clsQnOptAtom) As Boolean
'    On Error Resume Next
'    InOptLess = False
'
'    If DateSerial(Year(aOpt1.Expiry), Month(aOpt1.Expiry), 1) < DateSerial(Year(aOpt2.Expiry), Month(aOpt2.Expiry), 1) Then
'        InOptLess = True
'    ElseIf DateSerial(Year(aOpt1.Expiry), Month(aOpt1.Expiry), 1) = DateSerial(Year(aOpt2.Expiry), Month(aOpt2.Expiry), 1) Then
'        If aOpt1.Strike < aOpt2.Strike Then
'            InOptLess = True
'        ElseIf aOpt1.Strike = aOpt2.Strike Then
'            InOptLess = (aOpt1.OptType > aOpt2.OptType)
'        End If
'    End If
'End Function
'
'' qsort options array sorting
'Private Sub DoSortInOptions(ByRef arrOpt() As clsQnOptAtom, ByVal nLBound As Long, ByVal nUBound As Long)
'    Dim aTemp As clsQnOptAtom, aBuffer As clsQnOptAtom, nCurLow&, nCurHigh&, nCurMidPoint&
'
'    nCurLow = nLBound
'    nCurHigh = nUBound
'    If nUBound <= nLBound Then Exit Sub
'    nCurMidPoint = (nLBound + nUBound) \ 2
'    Set aTemp = arrOpt(nCurMidPoint)
'
'    Do While nCurLow <= nCurHigh
'
'        Do While InOptLess(arrOpt(nCurLow), aTemp)
'            nCurLow = nCurLow + 1
'            If nCurLow = nUBound Then Exit Do
'        Loop
'
'        Do While InOptLess(aTemp, arrOpt(nCurHigh))
'            nCurHigh = nCurHigh - 1
'            If nCurHigh = nLBound Then Exit Do
'        Loop
'
'        If nCurLow <= nCurHigh Then
'            Set aBuffer = arrOpt(nCurLow)
'            Set arrOpt(nCurLow) = arrOpt(nCurHigh)
'            Set arrOpt(nCurHigh) = aBuffer
'            nCurLow = nCurLow + 1
'            nCurHigh = nCurHigh - 1
'        End If
'    Loop
'
'    If nLBound < nCurHigh Then
'        DoSortInOptions arrOpt, nLBound, nCurHigh
'    End If
'
'    If nCurLow < nUBound Then
'        DoSortInOptions arrOpt, nCurLow, nUBound
'    End If
'End Sub
'
'Private Sub RequestLastQuotes()
'    On Error GoTo EH
'    Dim i&, nCount&, aParam As ISEPROVIDERLib.QuoteUpdateParams, sExch$, aExch As EtsGeneralLib.ExchAtom
'
'    If m_bInProc Or Not m_bIsConnected Or m_enState <> enQsReady Then
'        Debug.Assert False
'        Exit Sub
'    End If
'
'    If Not PriceProvider Is Nothing Then
'        Debug.Assert m_enState = enQsReady
'
'        nCount = m_QuoteRequests.Count
'        If nCount > 0 Then
'            m_bInProc = True
'            m_enState = enQsLastQuoteReq
'            AdjustState
'
'            m_nLastQuoteReqCount = nCount
'            m_nLastQuoteReqDone = 0&
'
'            pbProgress.Min = 0
'            pbProgress.Value = 0
'            pbProgress.Max = m_nLastQuoteReqCount
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
'                m_nLastQuoteGroupReqCount = 0&
'                m_nLastQuoteGroupReqDone = 0&
'
'                For i = 1 To nCount
'                    If m_enState = enQsReady Then Exit For
'                    PriceProvider.RequestLastQuote m_QuoteRequests(i).GetQuoteUpdateParam
'                    DoEvents
'                Next
'
'            Else
'                aParam.Symbol = m_Und.Symbol
'                aParam.Type = IIf(m_Und.UndType = enCtStock, enSTK, enIDX)
'
'                m_nLastQuoteGroupReqCount = m_UndExch.Count + 1
'                m_nLastQuoteGroupReqDone = 0&
'
'                For Each aExch In m_UndExch
'                    If m_enState = enQsReady Then Exit For
'                    aParam.Exchange = aExch.Code
'                    PriceProvider.RequestLastQuote aParam
'                    DoEvents
'                Next
'
'                If m_enState = enQsLastQuoteReq Then
'                    sExch = ""
'
'                    For Each aExch In m_OptExch
'                        If aExch.ID <> 0 Then sExch = sExch & "," & aExch.Code
'                    Next
'
'                    aParam.Exchange = sExch
'                    aParam.Type = IIf(m_Und.UndType = enCtStock, enGrSTK, enGrIDX)
'                    m_GroupPriceProvider.RequestGroup aParam, False
'                End If
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
'        gCmn.MyMsgBox m_frmOwner, "You are not connected to market data provider.", vbExclamation
'    End If
'    Exit Sub
'EH:
'    m_enState = enQsReady
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
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
'Private Sub RequestLastMmQuotes()
'    On Error GoTo EH
'    Dim nCount&, aParam As ISEPROVIDERLib.ISEMMQuoteParams
'
'    If m_bInProc Or Not m_bIsConnected Or m_enState <> enQsReady Then
'        Debug.Assert False
'        Exit Sub
'    End If
'
'    If Not MmManager Is Nothing Then
'        Debug.Assert m_enState = enQsReady
'
'        nCount = m_Und.OptionsCount
'        If nCount > 0 Then
'            m_bInProc = True
'            m_enState = enQsLastMmQuoteReq
'            AdjustState
'
'            m_nLastMmQuoteReqCount = nCount * 2
'            m_nLastMmQuoteReqDone = 0&
'
'            pbProgress.Min = 0
'            pbProgress.Value = 0
'            pbProgress.Max = m_nLastMmQuoteReqCount
'            pbProgress.Visible = True
'
'            lblStatus.Visible = False
'            lblProcess.Visible = True
'            lblProcess.Caption = "Last MM quotes request..."
'            imgStop.Visible = True
'            imgStopDis.Visible = False
'            DoEvents
'
'            aParam.BidOrAsk = enBoth
'            aParam.Type = IIf(m_Und.UndType = enCtStock, enSTK, enIDX)
'            aParam.Symbol = m_Und.Symbol
'
'            MmManager.RequestLastMmQuote aParam
'
'            m_bInProc = False
'            AdjustState
'        End If
'    Else
'        gCmn.MyMsgBox m_frmOwner, "You are not connected to market data provider.", vbExclamation
'    End If
'    Exit Sub
'EH:
'    m_enState = enQsReady
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to request last MM quotes."
'    On Error Resume Next
'
'    MmManager.CancelLastMmQuote
'
'    pbProgress.Visible = False
'    lblStatus.Visible = True
'    lblProcess.Visible = False
'    lblProcess.Caption = ""
'    imgStop.Visible = False
'    imgStopDis.Visible = False
'    m_bInProc = False
'    AdjustState
'End Sub
'
'Private Sub MmManager_OnError(ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As ISEPROVIDERLib.RequestsTypeEnum, ByVal Request As Variant)
'    On Error Resume Next
'
'    If ReqType = enRequestLastQuote Then
'        LogEventLocal EVENT_ERROR, Request.Symbol & ": " & Description
'    Else
'        LogEventLocal EVENT_ERROR, Description
'    End If
'End Sub
'
'Private Sub MmQuoteCopy(aSrc As ISEPROVIDERLib.ISEQuote, aDst As clsQnQuoteAtom)
'    On Error Resume Next
'    If aSrc.BidOrAsk = enBid Then
'        aDst.MmOpenQuantityBid = aSrc.OpenQuantity
'        aDst.MmPremiumBid = aSrc.Premium
'        aDst.MmQuantityBid = aSrc.Quantity
'        aDst.MmStepUpBufferBid = aSrc.StepUpBuffer
'    Else
'        aDst.MmOpenQuantityAsk = aSrc.OpenQuantity
'        aDst.MmPremiumAsk = aSrc.Premium
'        aDst.MmQuantityAsk = aSrc.Quantity
'        aDst.MmStepUpBufferAsk = aSrc.StepUpBuffer
'        If aSrc.BidOrAsk = enBoth Then
'            aDst.MmOpenQuantityBid = aSrc.OpenQuantity
'            aDst.MmPremiumBid = aSrc.Premium
'            aDst.MmQuantityBid = aSrc.Quantity
'            aDst.MmStepUpBufferBid = aSrc.StepUpBuffer
'        End If
'    End If
'End Sub
'
'Private Sub MmManager_OnLastMmQuote(Request As ISEPROVIDERLib.ISEMMQuoteParams, Results As ISEPROVIDERLib.ISEQuote, ByVal TickWorseLevel As Long)
'    On Error Resume Next
'    If m_enState <> enQsLastMmQuoteReq And m_enState <> enQsRealtime Then Exit Sub
'
'    Dim sKey$, aRequest As clsQnReqAtom, aQuote As clsQnQuoteAtom, bFinished As Boolean, nRow&
'    bFinished = False
'
'    If Request.Symbol = m_Und.Symbol Then
'        sKey = Results.Series
'
'        Set aRequest = m_QuoteRequests(sKey)
'        If Not aRequest Is Nothing Then
'            Set aQuote = aRequest.Opt.Quotes(CStr(aRequest.Exch.ID))
'
'            Debug.Assert Not aQuote Is Nothing
'
'            MmQuoteCopy Results, aQuote
'
'            If Results.BidOrAsk = enBid Then
'                aQuote.MmTickWorseLevelBid = TickWorseLevel
'            Else
'                aQuote.MmTickWorseLevelAsk = TickWorseLevel
'            End If
'
'            If m_enState = enQsLastMmQuoteReq Then
'                m_nLastMmQuoteReqDone = m_nLastMmQuoteReqDone + 1
'                IncProgress pbProgress
'
'                bFinished = (m_nLastMmQuoteReqDone = m_nLastMmQuoteReqCount)
'            Else
'                nRow = fgOpt.FindRow(sKey, , IIf(aRequest.Opt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'                Debug.Assert nRow > 0 And nRow < fgOpt.Rows
'                Debug.Assert fgOpt.RowData(nRow).Opt(aRequest.Opt.OptType).ID = aRequest.Opt.ID
'
'                OptionUpdateQuote nRow, aRequest.Opt.OptType, False, True
'            End If
'
'            Set aQuote = Nothing
'        End If
'
'        If m_enState = enQsLastMmQuoteReq And bFinished Then
'            m_enState = enQsReady
'            pbProgress.Value = pbProgress.Max
'
'            UnderlyingUpdate False, True, True
'            OptionsUpdate False, True, True
'            AdjustState
'
'            pbProgress.Visible = False
'            lblStatus.Visible = True
'            lblProcess.Visible = False
'            lblProcess.Caption = ""
'            imgStop.Visible = False
'            imgStopDis.Visible = False
'
'            If g_Params.QuoteDirectlyToRealtime Then
'                StartRealTime True
'            End If
'
'            SendQuotes
'        End If
'    End If
'End Sub
'
'Private Function Error2String(ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum) As String
'    On Error Resume Next
'    Select Case ErrorNumber
'        Case enUnknown
'            Error2String = "Unknown error."
'        Case enNoError
'            Error2String = "No error."
'        Case enSymbolNotSupported
'            Error2String = "Symbol does not supported."
'        Case enNoDataAvailableForSymbol
'            Error2String = "No data is available for a symbol"
'        Case enNoAccessForData
'            Error2String = "No access for data."
'        Case enCouldNotConnectToProvider
'            Error2String = "Could not connect to provider."
'        Case enNotConnected
'            Error2String = "Not connected."
'        Case enConnectionWasDisconnected
'            Error2String = "Connection was disconnected."
'        Case enInvalidAccount
'            Error2String = "Invalid account."
'        Case enVersionIsNoLongerSupport
'            Error2String = "Version is no longer support."
'        Case enInternalError
'            Error2String = "Internal error."
'        Case enProviderInternalError
'            Error2String = "Internal provider error."
'        Case Else
'            Error2String = "Unknown error."
'    End Select
'End Function
'
'Private Sub SendQuotes()
'    On Error GoTo EH:
'    If m_Und.ID = 0 Or m_Und.TraderRole = enTrEAM Or m_enState <> enQsRealtime Then Exit Sub
'
'    Dim arrOutQuote() As ISEPROVIDERLib.ISEQuote, nCount&, aQuote As clsQnQuoteAtom, n&, nReqCount&
'    Dim aReq As clsQnReqAtom, aOpt As clsQnOptAtom, nMaxCount&, dPrice#
'    Dim dSpread As Double
'
'    nMaxCount = m_Und.OptionsCount * 2
'    If Not MmManager Is Nothing And nMaxCount > 0 Then
'
'        ReDim arrOutQuote(1 To nMaxCount)
'        nCount = 0
'        nReqCount = m_QuoteRequests.Count
'
'        For n = 1 To nReqCount
'            Set aReq = m_QuoteRequests(n)
'            Set aOpt = aReq.Opt
'
'            If Not aOpt Is Nothing And Not aReq.Exch Is Nothing Then
'                If aReq.Exch.ID = 0 Then
'                    Set aQuote = aOpt.Quotes("0")
'                    If Not aQuote Is Nothing Then
'                        If aQuote.TradingStatus = ISEPROVIDERLib.SeriesTradingStatusEnum.enStsRegular Then
'
'                            If Round(aQuote.TheoPrice, 1) > 0# Then
'
'                                ' calculate maximum spread sheet
'                                dPrice = Round(aQuote.TheoPrice, 1)
'                                Select Case dPrice
'                                    Case 0 To 2#
'                                        dSpread = 0.3
'                                    Case 2# To 5#
'                                        dSpread = 0.4
'                                    Case 5# To 10#
'                                        dSpread = 0.5
'                                    Case 10# To 20#
'                                        dSpread = 0.7
'                                    Case Else
'                                        dSpread = 0.9
'                                End Select
'
'                                'bid
'                                dPrice = Round(aQuote.TheoPrice - dSpread / 2#, 1)
'                                dPrice = IIf(dPrice > 0#, dPrice, Round(aQuote.TheoPrice, 1))
'
'                                If dPrice <> aQuote.MmPremiumBid Then
'                                    nCount = nCount + 1
'                                    arrOutQuote(nCount).Series = aOpt.Symbol
'                                    arrOutQuote(nCount).Premium = dPrice
'                                    arrOutQuote(nCount).BidOrAsk = enBid
'                                    arrOutQuote(nCount).OpenQuantity = 10
'                                    arrOutQuote(nCount).Quantity = 10
'                                    arrOutQuote(nCount).StepUpBuffer = 10
'                                End If
'
'                                'ask
'                                dPrice = Round(aQuote.TheoPrice + dSpread / 2#, 1)
'
'                                If dPrice <> aQuote.MmPremiumAsk Then
'                                    nCount = nCount + 1
'                                    arrOutQuote(nCount).Series = aOpt.Symbol
'                                    arrOutQuote(nCount).Premium = dPrice
'                                    arrOutQuote(nCount).BidOrAsk = enAsk
'                                    arrOutQuote(nCount).OpenQuantity = 10
'                                    arrOutQuote(nCount).Quantity = 10
'                                    arrOutQuote(nCount).StepUpBuffer = 10
'                                End If
'                            Else
'                                If aQuote.MmPremiumAsk <> 0 Or aQuote.MmPremiumBid <> 0 Then
'                                    nCount = nCount + 1
'                                    arrOutQuote(nCount).Series = aOpt.Symbol
'                                    arrOutQuote(nCount).Premium = 0
'                                    arrOutQuote(nCount).BidOrAsk = enBoth
'                                    arrOutQuote(nCount).OpenQuantity = 0
'                                    arrOutQuote(nCount).Quantity = 0
'                                    arrOutQuote(nCount).StepUpBuffer = 0
'                                End If
'                            End If
'                        End If
'                    End If
'                    Set aQuote = Nothing
'                End If
'            End If
'            Set aOpt = Nothing
'            Set aReq = Nothing
'        Next
'
'        If nCount > 0 Then
'            If nCount < nMaxCount Then ReDim Preserve arrOutQuote(1 To nCount)
'
'            LogEventLocal EVENT_INFO, UBound(arrOutQuote) & " quotes sending..."
'            MmManager.SubmitQuotes arrOutQuote
'        End If
'    End If
'
'EX:
'    Erase arrOutQuote
'    Exit Sub
'EH:
'    LogEventLocal EVENT_ERROR, aOpt.Symbol & ": Fail to send quote."
'    GoTo EX
'End Sub
'
'
'
'Private Sub MmManager_OnQuotesSent(Quotes() As ISEPROVIDERLib.ISEQuote, ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal OrderID As String)
'    On Error Resume Next
'    If m_enState <> enQsRealtime Then Exit Sub
'    LogEventLocal EVENT_INFO, UBound(Quotes) + 1 & " quotes sent."
'
'    Dim sKey$, aRequest As clsQnReqAtom, aQuote As clsQnQuoteAtom, aInQuote As ISEPROVIDERLib.ISEQuote
'    Dim nLBound&, nUBound&, i&, nRow&
'
'    nLBound = 0: nUBound = -1
'
'    nLBound = LBound(Quotes)
'    nUBound = UBound(Quotes)
'
'    m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'    m_GridLock(GT_QUOTATION_LOG).LockRedraw
'
'    For i = nLBound To nUBound
'        aInQuote = Quotes(i)
'        sKey = aInQuote.Series
'
'        Set aRequest = m_QuoteRequests(sKey)
'        If Not aRequest Is Nothing Then
'            Set aQuote = aRequest.Opt.Quotes(CStr(aRequest.Exch.ID))
'            Debug.Assert Not aQuote Is Nothing
'
'            If ErrorNumber = enNoError Then
'                MmQuoteCopy aInQuote, aQuote
'                If aInQuote.BidOrAsk = enBid Then
'                    aQuote.MmTickWorseLevelBid = 0
'                Else
'                    aQuote.MmTickWorseLevelAsk = 0
'                    If aInQuote.BidOrAsk = enBoth Then
'                        aQuote.MmTickWorseLevelBid = 0
'                    End If
'                End If
'
'                nRow = fgOpt.FindRow(sKey, , IIf(aRequest.Opt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'                Debug.Assert nRow > 0 And nRow < fgOpt.Rows
'                Debug.Assert fgOpt.RowData(nRow).Opt(aRequest.Opt.OptType).ID = aRequest.Opt.ID
'
'                OptionUpdateQuote nRow, aRequest.Opt.OptType, False, True
'            Else
'                LogEventLocal EVENT_ERROR, aInQuote.Series & ": quote send error. " & Error2String(ErrorNumber)
'            End If
'        End If
'    Next
'
'    m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
'    m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'End Sub
'
'Private Sub MmManager_OnQuoteRejection(Quote As ISEPROVIDERLib.ISEQuote, ByVal Reason As String)
'    On Error Resume Next
'    If m_enState <> enQsRealtime Then Exit Sub
'
'    If Not m_QuoteRequests(Quote.Series) Is Nothing Then
'        LogEventLocal EVENT_WARNING, Quote.Series & " quote rejected: " & Reason
'    End If
'End Sub
'
'Private Sub MmManager_OnQuoteStepUp(Quote As ISEPROVIDERLib.ISEQuote)
'    On Error Resume Next
'    If m_enState <> enQsRealtime Then Exit Sub
'
'    Dim sKey$, aRequest As clsQnReqAtom, aQuote As clsQnQuoteAtom, nRow&
'
'    sKey = Quote.Series
'
'    Set aRequest = m_QuoteRequests(sKey)
'    If Not aRequest Is Nothing Then
'        Set aQuote = aRequest.Opt.Quotes(CStr(aRequest.Exch.ID))
'
'        Debug.Assert Not aQuote Is Nothing
'
'        MmQuoteCopy Quote, aQuote
'
''        If Quote.BidOrAsk = enBid Then
''            aQuote.MmOpenQuantityBid = Quote.OpenQuantity
''            aQuote.MmPremiumBid = Quote.Premium
''            aQuote.MmQuantityBid = Quote.Quantity
''            aQuote.MmStepUpBufferBid = Quote.StepUpBuffer
''        Else
''            aQuote.MmOpenQuantityAsk = Quote.OpenQuantity
''            aQuote.MmPremiumAsk = Quote.Premium
''            aQuote.MmQuantityAsk = Quote.Quantity
''            aQuote.MmStepUpBufferAsk = Quote.StepUpBuffer
''        End If
'
'        nRow = fgOpt.FindRow(sKey, , IIf(aRequest.Opt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'        Debug.Assert nRow > 0 And nRow < fgOpt.Rows
'        Debug.Assert fgOpt.RowData(nRow).Opt(aRequest.Opt.OptType).ID = aRequest.Opt.ID
'
'        OptionUpdateQuote nRow, aRequest.Opt.OptType, False, True
'
'        LogEventLocal EVENT_INFO, Quote.Series & ": quote step up."
'    End If
'End Sub
'
'Private Sub MmManager_OnQuoteTickWorse(Quote As ISEPROVIDERLib.ISEQuote, ByVal TickWorseLevel As Long, ByVal Reason As String)
'    On Error Resume Next
'    If m_enState <> enQsRealtime Then Exit Sub
'
'    Dim sKey$, aRequest As clsQnReqAtom, aQuote As clsQnQuoteAtom, nRow&
'
'    sKey = Quote.Series
'
'    Set aRequest = m_QuoteRequests(sKey)
'    If Not aRequest Is Nothing Then
'        Set aQuote = aRequest.Opt.Quotes(CStr(aRequest.Exch.ID))
'
'        Debug.Assert Not aQuote Is Nothing
'
'        MmQuoteCopy Quote, aQuote
'
'        If Quote.BidOrAsk = enBid Then
'            aQuote.MmTickWorseLevelBid = TickWorseLevel
'        Else
'            aQuote.MmTickWorseLevelAsk = TickWorseLevel
'        End If
'
'        nRow = fgOpt.FindRow(sKey, , IIf(aRequest.Opt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'        Debug.Assert nRow > 0 And nRow < fgOpt.Rows
'        Debug.Assert fgOpt.RowData(nRow).Opt(aRequest.Opt.OptType).ID = aRequest.Opt.ID
'
'        OptionUpdateQuote nRow, aRequest.Opt.OptType, False, True
'
'        LogEventLocal EVENT_INFO, Quote.Series & " tick worse (" & CStr(TickWorseLevel) & "): " & Reason
'    End If
'End Sub
'
'Private Sub PriceProvider_OnError(ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As ISEPROVIDERLib.RequestsTypeEnum, ByVal Request As Variant)
'    On Error Resume Next
'    Dim sKey$, bFinished As Boolean
'    bFinished = False
'
'    If ReqType = enSubscribeQuote Then
'        If Request.Type <> enGrSTK And Request.Type <> enGrIDX Then
'            sKey = Request.Symbol
'            If Len(Request.Exchange) > 0 Then
'                sKey = sKey & "." & Request.Exchange
'            End If
'
'            If Not m_QuoteRequests(sKey) Is Nothing Then
'                LogEventLocal EVENT_WARNING, sKey & ": " & Description
'            End If
'        Else
'            LogEventLocal EVENT_WARNING, Request.Symbol & " options: " & Description
'        End If
'
'    ElseIf ReqType = enRequestLastQuote Then
'        If m_enState = enQsLastQuoteReq Then
'            If Request.Type <> enGrSTK And Request.Type <> enGrIDX Then
'                sKey = Request.Symbol
'                If Len(Request.Exchange) > 0 Then
'                    sKey = sKey & "." & Request.Exchange
'                End If
'
'                If Not m_QuoteRequests(sKey) Is Nothing Then
'                    LogEventLocal EVENT_WARNING, sKey & ": " & Description
'
'                    m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
'                    IncProgress pbProgress
'
'                    If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
'                    bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
'                                Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
'                End If
'            Else
'                LogEventLocal EVENT_WARNING, Request.Symbol & " options: " & Description
'                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
'                bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
'            End If
'
'            If bFinished Then
'                m_enState = enQsReady
'                pbProgress.Value = pbProgress.Max
'
'                m_bInProc = True
'                CalculateUnderlyingOptions True
'                m_bInProc = False
'
''                UnderlyingUpdate False, True, True
''                OptionsUpdate False, True, True
'
'                AdjustState
'                UpdateTotals
'
'                pbProgress.Visible = False
'                lblStatus.Visible = True
'                lblProcess.Visible = False
'                lblProcess.Caption = ""
'                imgStop.Visible = False
'                imgStopDis.Visible = False
'
'                RequestLastMmQuotes
'            End If
'        Else
'            Exit Sub
'        End If
'
'    Else
'        LogEventLocal EVENT_ERROR, Description
'    End If
'
'End Sub
'
'Private Sub PriceProvider_OnLastQuote(Params As ISEPROVIDERLib.QuoteUpdateParams, Results As ISEPROVIDERLib.ISEQuoteUpdateResult)
'    On Error Resume Next
'    If m_enState <> enQsLastQuoteReq Then Exit Sub
'
'    Dim sKey$, aRequest As clsQnReqAtom, aQuote As clsQnQuoteAtom, bFinished As Boolean
'    bFinished = False
'
'    If Params.Type <> enGrSTK And Params.Type <> enGrIDX Then
'        sKey = Params.Symbol
'
'        If Len(Params.Exchange) > 0 Then
'            sKey = sKey & "." & Params.Exchange
'        End If
'
'        Set aRequest = m_QuoteRequests(sKey)
'        If Not aRequest Is Nothing Then
'            If Params.Type = enOPT Then
'                Set aQuote = aRequest.Opt.Quotes(CStr(aRequest.Exch.ID))
'            Else
'                Set aQuote = aRequest.Und.Quotes(CStr(aRequest.Exch.ID))
'                If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
'            End If
'
'            Debug.Assert Not aQuote Is Nothing
'
'            If Results.BidPrice > BAD_DOUBLE_VALUE Then
'                If aQuote.BidPrice >= 0# Then aQuote.BidPriceTick = Results.BidPrice - aQuote.BidPrice
'                aQuote.BidPrice = Results.BidPrice
'            End If
'
'            If Results.AskPrice > BAD_DOUBLE_VALUE Then
'                If aQuote.AskPrice >= 0# Then aQuote.AskPriceTick = Results.AskPrice - aQuote.AskPrice
'                aQuote.AskPrice = Results.AskPrice
'            End If
'
'            aQuote.TradingStatus = Results.TradingStatus
'
''            If aQuote.LotSize <= 0 Then
''                If Results.LotSize > 0 Then
''                    aQuote.LotSize = Results.LotSize
''                Else
''                    If Params.Type = enOPT Then
''                        aQuote.LotSize = 100
''                    Else
''                        aQuote.LotSize = 1
''                    End If
''                End If
''            End If
''
''            aQuote.LastPrice = Results.LastPrice
''            aQuote.ClosePrice = Results.ClosePrice
''            aQuote.LowPrice = Results.LowPrice
''            aQuote.HighPrice = Results.HighPrice
''            aQuote.OpenPrice = Results.OpenPrice
''            aQuote.Volume = Results.Volume
''            aQuote.OpenInterest = Results.OpenInterest
''            aQuote.NetChange = Results.NetChange
'            If Results.BidSize > BAD_LONG_VALUE Then aQuote.SizeBid = Results.BidSize
'            If Results.AskSize > BAD_LONG_VALUE Then aQuote.SizeAsk = Results.AskSize
'
'            If Params.Type <> enOPT And aQuote.Exch.ID = 0 Then
''                aRequest.Und.VolaSrv.UnderlyingPrice = PriceMidEx(aQuote.BidPrice, aQuote.AskPrice, aQuote.LastPrice)
'                 Debug.Assert (Not aRequest.Und.UndPriceProfile Is Nothing)
'                 aRequest.Und.VolaSrv.UnderlyingPrice = aRequest.Und.UndPriceProfile.GetUndPriceMid(aQuote.BidPrice, aQuote.AskPrice, _
'                    aQuote.LastPrice, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'            End If
'
'            Set aQuote = Nothing
'
'            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
'            IncProgress pbProgress
'
'            bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
'                        Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
'        End If
'    Else
'        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
'        bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
'    End If
'
'    If bFinished Then
'        m_enState = enQsReady
'        pbProgress.Value = pbProgress.Max
'
'        m_bInProc = True
'        CalculateUnderlyingOptions True
'        m_bInProc = False
''        UnderlyingUpdate False, True, True
''        OptionsUpdate False, True, True
'
'        AdjustState
'        UpdateTotals
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        lblProcess.Caption = ""
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'
'        RequestLastMmQuotes
'    End If
'End Sub
'
'Private Sub PriceProvider_OnQuoteUpdate(Params As ISEPROVIDERLib.QuoteUpdateParams, Results As ISEPROVIDERLib.ISEQuoteUpdateResult)
'    On Error Resume Next
'    If m_enState <> enQsRealtime Or m_bInProc Then Exit Sub
'
'    Dim nRow&, sKey$, aRequest As clsQnReqAtom, dBidPrice#, dAskPrice#, dLastPrice#
'    Dim aQuote As clsQnQuoteAtom
'    Dim enCalcIV As CalcIvEnum, nMask&
'
'    sKey = Params.Symbol
'    If Len(Params.Exchange) > 0 Then
'        sKey = sKey & "." & Params.Exchange
'    End If
'
'    Set aRequest = m_QuoteRequests(sKey)
'    If Not aRequest Is Nothing Then
'        dBidPrice = Results.BidPrice
'        dAskPrice = Results.AskPrice
'        dLastPrice = BAD_DOUBLE_VALUE 'Results.LastPrice
'
'        If Params.Type = enOPT Then
'            Set aQuote = aRequest.Opt.Quotes(CStr(aRequest.Exch.ID))
'            Debug.Assert Not aQuote Is Nothing
'
''            aQuote.Volume = Results.Volume
''            aQuote.NetChange = Results.NetChange
'            If Results.BidSize > BAD_LONG_VALUE Then aQuote.SizeBid = Results.BidSize
'            If Results.AskSize > BAD_LONG_VALUE Then aQuote.SizeAsk = Results.AskSize
'            aQuote.TradingStatus = Results.TradingStatus
'
'            nRow = fgOpt.FindRow(sKey, , IIf(aRequest.Opt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'
'            Debug.Assert nRow > 0 And nRow < fgOpt.Rows
'            Debug.Assert fgOpt.RowData(nRow).Opt(aRequest.Opt.OptType).ID = aRequest.Opt.ID
'
'            If dBidPrice > BAD_DOUBLE_VALUE And aQuote.BidPrice >= 0# Then aQuote.BidPriceTick = dBidPrice - aQuote.BidPrice
'            If dAskPrice > BAD_DOUBLE_VALUE And aQuote.AskPrice >= 0# Then aQuote.AskPriceTick = dAskPrice - aQuote.AskPrice
'
'            If dBidPrice > BAD_DOUBLE_VALUE And aQuote.BidPrice <> dBidPrice _
'                Or dAskPrice > BAD_DOUBLE_VALUE And aQuote.AskPrice <> dAskPrice _
'                Or dLastPrice > BAD_DOUBLE_VALUE And aQuote.LastPrice <> dLastPrice Then
'
'                If dBidPrice > BAD_DOUBLE_VALUE Then aQuote.BidPrice = dBidPrice
'                If dAskPrice > BAD_DOUBLE_VALUE Then aQuote.AskPrice = dAskPrice
'                If dLastPrice > BAD_DOUBLE_VALUE Then aQuote.LastPrice = dLastPrice
'
'                nMask = GetGreeksMask(aRequest.Opt.OptType)
'                enCalcIV = GetIvMask(aRequest.Opt.OptType)
'
'                m_bInProc = True
'                CalcOptionGreeks aRequest.Opt, aQuote, nMask, enCalcIV
'                CalculateUnderlyingOptions False
'                m_bInProc = False
'
'                OptionUpdateQuote nRow, aRequest.Opt.OptType, False, True
'                UpdateTotals
'            Else
'                OptionUpdateColors nRow, aRequest.Opt.OptType, Nothing
'            End If
'
'            Set aQuote = Nothing
'        Else
'            Set aQuote = aRequest.Und.Quotes(CStr(aRequest.Exch.ID))
'            Debug.Assert Not aQuote Is Nothing
'
''            aQuote.Volume = Results.Volume
''            aQuote.NetChange = Results.NetChange
'            If Results.BidSize > BAD_LONG_VALUE Then aQuote.SizeBid = Results.BidSize
'            If Results.AskSize > BAD_LONG_VALUE Then aQuote.SizeAsk = Results.AskSize
'            aQuote.TradingStatus = Results.TradingStatus
'
'            nRow = fgUnd.FindRow(sKey, , UUC_KEY)
'
'            Debug.Assert nRow > 0 And nRow < fgUnd.Rows
'
'            If dBidPrice > BAD_DOUBLE_VALUE And aQuote.BidPrice >= 0# Then aQuote.BidPriceTick = dBidPrice - aQuote.BidPrice
'            If dAskPrice > BAD_DOUBLE_VALUE And aQuote.AskPrice >= 0# Then aQuote.AskPriceTick = dAskPrice - aQuote.AskPrice
'
'            If dBidPrice > BAD_DOUBLE_VALUE And aQuote.BidPrice <> dBidPrice _
'                Or dAskPrice > BAD_DOUBLE_VALUE And aQuote.AskPrice <> dAskPrice _
'                Or dLastPrice > BAD_DOUBLE_VALUE And aQuote.LastPrice <> dLastPrice Then
'
'                If dBidPrice > BAD_DOUBLE_VALUE Then aQuote.BidPrice = dBidPrice
'                If dAskPrice > BAD_DOUBLE_VALUE Then aQuote.AskPrice = dAskPrice
'                If dLastPrice > BAD_DOUBLE_VALUE Then aQuote.LastPrice = dLastPrice
'
'                m_bInProc = True
'                Debug.Assert Not aQuote.Exch Is Nothing
'                If aQuote.Exch.ID = 0 Then
'
''                    aRequest.Und.VolaSrv.UnderlyingPrice = PriceMidEx(aQuote.BidPrice, aQuote.AskPrice, aQuote.LastPrice)
'                     Debug.Assert (Not aRequest.Und.UndPriceProfile Is Nothing)
'                     aRequest.Und.VolaSrv.UnderlyingPrice = aRequest.Und.UndPriceProfile.GetUndPriceMid(aQuote.BidPrice, aQuote.AskPrice, _
'                        aQuote.LastPrice, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'                    CalculateUnderlyingOptions True
'                    UnderlyingUpdateQuote nRow, False, True
'                    OptionsUpdate False, False, True
'
'                    SendQuotes
'                Else
'                    UnderlyingUpdateQuote nRow, False, True
'                End If
'                m_bInProc = False
'                UpdateTotals
'            Else
'                UnderlyingUpdateQuoteColors nRow, aQuote
'            End If
'
'            Set aQuote = Nothing
'        End If
'
'        Set aRequest = Nothing
'    End If
'End Sub
'
'Private Sub UpdateTotals()
'    On Error Resume Next
'    Dim sTotals$
'    Dim sValue As String ' * 20
'
'    sTotals = "Totals - "
'
'    With m_Und
'        If .ID <> 0 Then
'            sTotals = sTotals & "NetDlt: "
'            If Not IsBadDouble(.NetDelta) Then
'                sValue = Format$(.NetDelta, "#,##0")
'            Else
'                sValue = "N/A"
'            End If
'            sTotals = sTotals & sValue & "    "
'
'            sTotals = sTotals & "OptDlt: "
'            If Not IsBadDouble(.TotalDelta) Then
'                sValue = Format$(.TotalDelta, "#,##0")
'            Else
'                sValue = "N/A"
'            End If
'            sTotals = sTotals & sValue & "    "
'
'            sTotals = sTotals & "Gma$1: "
'            If Not IsBadDouble(.TotalGamma) Then
'                sValue = Format$(.TotalGamma, "#,##0")
'            Else
'                sValue = "N/A"
'            End If
'            sTotals = sTotals & sValue & "    "
'
'            sTotals = sTotals & "Vga: "
'            If Not IsBadDouble(.TotalVega) Then
'                sValue = Format$(.TotalVega, "#,##0")
'            Else
'                sValue = "N/A"
'            End If
'            sTotals = sTotals & sValue & "    "
'
'            sTotals = sTotals & "Tht: "
'            If Not IsBadDouble(.TotalTheta) Then
'                sValue = Format$(.TotalTheta, "#,##0")
'            Else
'                sValue = "N/A"
'            End If
'            sTotals = sTotals & sValue & "    "
'
'            sTotals = sTotals & "Rho: "
'            If Not IsBadDouble(.TotalRho) Then
'                sValue = Format$(.TotalRho, "#,##0")
'            Else
'                sValue = "N/A"
'            End If
'            sTotals = sTotals & sValue
'        Else
'            sTotals = sTotals & "N/A"
'        End If
'    End With
'
'    lblStatus.Caption = sTotals
'End Sub
'
'Private Sub CalculateUnderlyingOptions(ByVal bRecalcAll As Boolean)
'    On Error Resume Next
'    Dim e&, nExpCount&, s&, nStrCount&, Q&, nQuoteCount&
'    Dim aExpiry As clsQnExpAtom, aStrike As clsQnStrikeAtom, aOpt As clsQnOptAtom, aQuote As clsQnQuoteAtom
'    Dim nCallMask As Long, nPutMask As Long, enCallCalcIV As CalcIvEnum, enPutCalcIV As CalcIvEnum, dSpotPrice#
'
'    If bRecalcAll Then
'        nCallMask = GetGreeksMask(enOtCall)
'        nPutMask = GetGreeksMask(enOtPut)
'        enCallCalcIV = GetIvMask(enOtCall)
'        enPutCalcIV = GetIvMask(enOtPut)
'    End If
'
'    m_Und.NetDelta = BAD_DOUBLE_VALUE
'    m_Und.TotalDelta = BAD_DOUBLE_VALUE
'    m_Und.TotalGamma = BAD_DOUBLE_VALUE
'    m_Und.TotalTheta = BAD_DOUBLE_VALUE
'    m_Und.TotalVega = BAD_DOUBLE_VALUE
'    m_Und.TotalRho = BAD_DOUBLE_VALUE
'
''    dSpotPrice = PriceMidEx(m_Und.Quotes("0").BidPrice, m_Und.Quotes("0").AskPrice, m_Und.Quotes("0").LastPrice, g_Params.UseLastPriceForCalcs)
'    Debug.Assert (Not m_Und.UndPriceProfile Is Nothing)
'    dSpotPrice = m_Und.UndPriceProfile.GetUndPriceMid(m_Und.Quotes("0").BidPrice, m_Und.Quotes("0").AskPrice, _
'        m_Und.Quotes("0").LastPrice, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'
'    nExpCount = m_Und.Expiry.Count
'    For e = 1 To nExpCount
'        Set aExpiry = m_Und.Expiry(e)
'
'        nStrCount = aExpiry.Strike.Count
'        For s = 1 To nStrCount
'            Set aStrike = aExpiry.Strike(s)
'
'            Set aOpt = aStrike.aOption(enOtCall)
'            If Not aOpt Is Nothing Then
'                If bRecalcAll Then aOpt.Vola = aOpt.Underlying.VolaSrv.OptionVola(aOpt.Expiry, aOpt.Strike)
'
'                nQuoteCount = aOpt.Quotes.Count
'                For Q = 1 To nQuoteCount
'                    Set aQuote = aOpt.Quotes(Q)
'
'                    If bRecalcAll Then CalcOptionGreeks aOpt, aQuote, nCallMask, enCallCalcIV
'
'                    If aQuote.Exch.ID = 0 Then
'                        If Not IsBadLong(aOpt.QtyInShares) Then
'                            If Not IsBadDouble(aQuote.Delta) Then
'                                If IsBadDouble(m_Und.TotalDelta) Then m_Und.TotalDelta = 0#
'                                m_Und.TotalDelta = m_Und.TotalDelta + aOpt.QtyInShares * aQuote.Delta / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Gamma) And Not IsBadDouble(dSpotPrice) Then
'                                If IsBadDouble(m_Und.TotalGamma) Then m_Und.TotalGamma = 0#
'                                m_Und.TotalGamma = m_Und.TotalGamma + aOpt.QtyInShares * aQuote.Gamma / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Theta) Then
'                                If IsBadDouble(m_Und.TotalTheta) Then m_Und.TotalTheta = 0#
'                                m_Und.TotalTheta = m_Und.TotalTheta + aOpt.QtyInShares * aQuote.Theta / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Vega) Then
'                                If IsBadDouble(m_Und.TotalVega) Then m_Und.TotalVega = 0#
'                                m_Und.TotalVega = m_Und.TotalVega + aOpt.QtyInShares * aQuote.Vega / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Rho) Then
'                                If IsBadDouble(m_Und.TotalRho) Then m_Und.TotalRho = 0#
'                                m_Und.TotalRho = m_Und.TotalRho + aOpt.QtyInShares * aQuote.Rho / 100#
'                            End If
'                        End If
'                    End If
'
'                    Set aQuote = Nothing
'                Next
'            End If
'
'            Set aOpt = aStrike.aOption(enOtPut)
'            If Not aOpt Is Nothing Then
'                If bRecalcAll Then aOpt.Vola = aOpt.Underlying.VolaSrv.OptionVola(aOpt.Expiry, aOpt.Strike)
'
'                nQuoteCount = aOpt.Quotes.Count
'                For Q = 1 To nQuoteCount
'                    Set aQuote = aOpt.Quotes(Q)
'
'                    If bRecalcAll Then CalcOptionGreeks aOpt, aQuote, nPutMask, enPutCalcIV
'
'                    If aQuote.Exch.ID = 0 Then
'                        If Not IsBadDouble(aOpt.QtyInShares) Then
'                            If Not IsBadDouble(aQuote.Delta) Then
'                                If IsBadDouble(m_Und.TotalDelta) Then m_Und.TotalDelta = 0#
'                                m_Und.TotalDelta = m_Und.TotalDelta + aOpt.QtyInShares * aQuote.Delta / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Gamma) Then
'                                If IsBadDouble(m_Und.TotalGamma) Then m_Und.TotalGamma = 0#
'                                m_Und.TotalGamma = m_Und.TotalGamma + aOpt.QtyInShares * aQuote.Gamma / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Theta) Then
'                                If IsBadDouble(m_Und.TotalTheta) Then m_Und.TotalTheta = 0#
'                                m_Und.TotalTheta = m_Und.TotalTheta + aOpt.QtyInShares * aQuote.Theta / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Vega) Then
'                                If IsBadDouble(m_Und.TotalVega) Then m_Und.TotalVega = 0#
'                                m_Und.TotalVega = m_Und.TotalVega + aOpt.QtyInShares * aQuote.Vega / 100#
'                            End If
'                            If Not IsBadDouble(aQuote.Rho) Then
'                                If IsBadDouble(m_Und.TotalRho) Then m_Und.TotalRho = 0#
'                                m_Und.TotalRho = m_Und.TotalRho + aOpt.QtyInShares * aQuote.Rho / 100#
'                            End If
'                        End If
'                    End If
'                    Set aQuote = Nothing
'                Next
'            End If
'
'            Set aOpt = Nothing
'            Set aStrike = Nothing
'        Next
'
'        Set aExpiry = Nothing
'    Next
'
'    If Not IsBadDouble(m_Und.QtyInShares) Then m_Und.NetDelta = m_Und.QtyInShares
'    If Not IsBadDouble(m_Und.TotalDelta) Then
'        If IsBadDouble(m_Und.NetDelta) Then m_Und.NetDelta = 0#
'        m_Und.NetDelta = m_Und.NetDelta + m_Und.TotalDelta
'    End If
'End Sub
'
'Private Function GetGreeksMask(ByVal nOptType As Long) As Long
'    On Error Resume Next
'    Dim nShift&
'
'    GetGreeksMask = GM_DELTA Or GM_GAMMA Or GM_VEGA Or GM_THETA Or GM_RHO
'    If nOptType = enOtPut Then
'        nShift = UOC_P_SYMBOL - 2
'    Else
'        nShift = 0
'    End If
'
'    If m_gdOpt.Col(nShift + UOC_C_THEO_PRICE).Visible Then GetGreeksMask = GetGreeksMask Or GM_THEOPRICE
''    If m_gdOpt.Col(nShift + UOC_C_DELTA).Visible Then GetGreeksMask = GetGreeksMask Or GM_DELTA
''    If m_gdOpt.Col(nShift + UOC_C_GAMMA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA
''    If m_gdOpt.Col(nShift + UOC_C_VEGA).Visible Then GetGreeksMask = GetGreeksMask Or GM_VEGA
''    If m_gdOpt.Col(nShift + UOC_C_THETA).Visible Then GetGreeksMask = GetGreeksMask Or GM_THETA
''    If m_gdOpt.Col(nShift + UOC_C_RHO).Visible Then GetGreeksMask = GetGreeksMask Or GM_RHO
'    If m_gdOpt.Col(nShift + UOC_C_VEGA_DELTA).Visible Then GetGreeksMask = GetGreeksMask Or GM_DELTA_VEGA
'    If m_gdOpt.Col(nShift + UOC_C_VEGA_GAMMA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA_VEGA
'    If m_gdOpt.Col(nShift + UOC_C_THETA_DELTA).Visible Then GetGreeksMask = GetGreeksMask Or GM_DELTA_THETA
'    If m_gdOpt.Col(nShift + UOC_C_THETA_GAMMA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA_THETA
'    If m_gdOpt.Col(nShift + UOC_C_GAMMA_PER_THETA).Visible Then GetGreeksMask = GetGreeksMask Or GM_GAMMA Or GM_THETA
'End Function
'
'Private Function GetIvMask(ByVal nOptType As Long) As CalcIvEnum
'    On Error Resume Next
'    Dim nShift&
'    GetIvMask = CALC_IV_NONE
'
'    If nOptType = enOtPut Then
'        nShift = UOC_P_SYMBOL - 2
'    Else
'        nShift = 0
'    End If
'
'    If g_Params.UseTheoVolatility Then
'        If m_gdOpt.Col(nShift + UOC_C_IV).Visible Then GetIvMask = GetIvMask Or CALC_IV
'    Else
'        GetIvMask = GetIvMask Or CALC_IV
'    End If
'
'    If m_gdOpt.Col(nShift + UOC_C_IV_BID).Visible Then GetIvMask = GetIvMask Or CALC_IV_BID
'    If m_gdOpt.Col(nShift + UOC_C_IV_ASK).Visible Then GetIvMask = GetIvMask Or CALC_IV_ASK
'End Function
'
'Private Sub UnderlyingAdjustRates(ByVal bForceUpdateCustom As Boolean)
'    On Error Resume Next
'    Dim aExp As clsQnExpAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
'    If m_Und.ID = 0 Then Exit Sub
'
'    dPos = g_UnderlyingAll(m_Und.ID).UndPosForRates ' aUnd.UndPos
'
'    If GetIrRuleType = enRateBasedOnPosition Then 'And m_Und.Pos > BAD_DOUBLE_VALUE Then
'        cPosThreshold = Abs(GetPosThreshold)
'        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
'    Else
'        bUseMidRates = True
'    End If
'
'    If bForceUpdateCustom Or m_Und.UseMidRates <> bUseMidRates Or Not bUseMidRates Then
'        m_Und.UseMidRates = bUseMidRates
'
'        For Each aExp In m_Und.Expiry
'            If bUseMidRates Then
'                If Not m_Und.IsHTB Then
'                    aExp.Rate = GetNeutralRate(Date, aExp.Expiry)
'                Else
'                    aExp.Rate = GetNeutralHTBRate(Date, aExp.Expiry)
'                End If
'            Else
'                If Not m_Und.IsHTB Then
'                    aExp.Rate = IIf(dPos < 0#, GetShortRate(Date, aExp.Expiry), GetLongRate(Date, aExp.Expiry))
'                Else
'                    aExp.Rate = IIf(dPos < 0#, GetHTBRate(Date, aExp.Expiry), GetLongRate(Date, aExp.Expiry))
'                End If
'            End If
'
'            If bForceUpdateCustom Or Not m_Und.UseCustRates Then
'                If m_CustRates(CStr(CLng(aExp.ExpiryMonth))) Is Nothing Then
'                    aExp.RateCust = aExp.Rate
'                End If
'            End If
'        Next
'    End If
'End Sub
'
'Private Sub CalcOptionGreeks(aOpt As clsQnOptAtom, aQuote As clsQnQuoteAtom, ByVal nMask As Long, ByVal enCalcIV As CalcIvEnum)
'    On Error Resume Next
'    Dim nDivCount&, RetCount&
'    Dim dDivDte() As Double
'    Dim dDivAmts() As Double, aBaskDivs() As REGULAR_DIVIDENDS, nBaskDivCount&
'    Dim dRate#, dSpotPrice#, dOptPrice#, dIV#, dYield#, dSpotBid#, dSpotAsk#, dSpotLast#
'    Dim aGreeks As GreeksData, dOptBid#, dOptAsk#
'    Dim nModel As EtsGeneralLib.EtsCalcModelTypeEnum, nIsAmerican As Long
'    Dim dtDivDate As Date, nDivFreq&, dDivAmt#, dVola#
'    Dim nFlag&
'
'    aGreeks.nMask = nMask
'    If aGreeks.nMask = GM_NONE And enCalcIV = CALC_IV_NONE Then Exit Sub
'
'    nModel = g_Params.CalcModel
'    nIsAmerican = IIf(m_Und.IsAmerican, 1, 0)
'
'    dSpotPrice = 0#
'    dYield = 0#
'
'    dSpotBid = m_Und.Quotes("0").BidPrice
'    dSpotAsk = m_Und.Quotes("0").AskPrice
'    dSpotLast = m_Und.Quotes("0").LastPrice
'
'    If dSpotBid > 0 Or dSpotAsk > 0 Or dSpotLast > 0 Then
'
''        dSpotPrice = PriceMidEx(dSpotBid, dSpotAsk, dSpotLast, g_Params.UseLastPriceForCalcs)
'        Debug.Assert (Not m_Und.UndPriceProfile Is Nothing)
'        dSpotPrice = m_Und.UndPriceProfile.GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, _
'            g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'
''        If g_Params.UseLastPriceForCalcs Then
''            If dSpotBid <= 0# Then
''                dSpotBid = dSpotLast
''            End If
''
''            If dSpotAsk <= 0# Then
''                dSpotAsk = dSpotLast
''            End If
''        End If
''
'        If enCalcIV <> CALC_IV_NONE Then
'            dOptBid = aQuote.BidPrice
'            dOptAsk = aQuote.AskPrice
'
'            If aQuote.BidPrice > 0 Or aQuote.AskPrice > 0 Or aQuote.LastPrice > 0 Then
''                dOptPrice = PriceMidEx(aQuote.BidPrice, aQuote.AskPrice, aQuote.LastPrice, g_Params.UseLastPriceForCalcs)
'                Debug.Assert (Not m_Und.OptPriceProfile Is Nothing)
'                dOptPrice = m_Und.OptPriceProfile.GetOptPriceMid(aQuote.BidPrice, aQuote.AskPrice, aQuote.LastPrice, _
'                    g_Params.PriceRoundingRule, g_Params.UseTheoVolatility, 0)
'            End If
''
''            If g_Params.UseLastPriceForCalcs Then
''                If dOptBid <= 0# Then
''                    dOptBid = aQuote.LastPrice
''                End If
''
''                If dOptAsk <= 0# Then
''                    dOptAsk = aQuote.LastPrice
''                End If
''            End If
'        End If
'
'        If dSpotPrice > 0# And (aGreeks.nMask <> GM_NONE Or dOptPrice > 0# And enCalcIV <> CALC_IV_NONE) Then
'
'            nDivCount = 0
'            ReDim dDivDte(0 To 0)
'            ReDim dDivAmts(0 To 0)
'
'            If m_Und.UndType = enCtStock Then
'                If m_Und.UseCustDivs Then
'                    dtDivDate = m_Und.DivDateCust
'                    nDivFreq = m_Und.DivFreqCust
'                    dDivAmt = m_Und.DivAmtCust
'                Else
'                    dtDivDate = m_Und.DivDate
'                    nDivFreq = m_Und.DivFreq
'                    dDivAmt = m_Und.DivAmt
'                End If
'
'                If nDivFreq <> 0 Then
'                    nDivCount = GetDividendsCount(Date, aOpt.Expiry - Date, dtDivDate, nDivFreq)
'
'                    If nDivCount > 0 Then
'                        ReDim dDivDte(0 To nDivCount - 1)
'                        ReDim dDivAmts(0 To nDivCount - 1)
'
'                        GetDividends2 Date, aOpt.Expiry - Date, dtDivDate, nDivFreq, dDivAmt, nDivCount, dDivAmts(0), dDivDte(0), RetCount
'                    End If
'                End If
'            Else
'                If Not m_Und.BasketIndex Is Nothing Then
'                    nBaskDivCount = GetBasketIndexDividends(m_Und.BasketIndex, aBaskDivs)
'                    If nBaskDivCount > 0 Then
'                        nDivCount = GetBasketDividendsCount(Date, aOpt.Expiry - Date, aBaskDivs(0), nBaskDivCount)
'                        If nDivCount > 0 Then
'                            ReDim dDivAmts(0 To nDivCount - 1)
'                            ReDim dDivDte(0 To nDivCount - 1)
'                            GetBasketDividends Date, aOpt.Expiry - Date, aBaskDivs(0), nBaskDivCount, dDivAmts(0), dDivDte(0), nDivCount, RetCount
'                        End If
'                    End If
'
'                    Erase aBaskDivs
'                End If
'
'                If nDivCount <= 0 Then
'                    dYield = m_Und.Yield
'                End If
'            End If
'
'            dRate = IIf(m_Und.UseCustRates, aOpt.Exp.RateCust, aOpt.Exp.Rate)
'            Debug.Assert Not aOpt.Underlying Is Nothing
'
''            If aOpt.OptType = enOtCall And Month(aOpt.Expiry) = 5 And Year(aOpt.Expiry) = 2002 And aOpt.Strike = 35 Then
''                dYield = dYield
''            End If
'
'            If (enCalcIV And CALC_IV) = CALC_IV Then
'                If Not g_Params.UseTheoNoBid Or g_Params.UseTheoNoBid And aQuote.BidPrice > 0# Then
'                    dIV = 0#
'                    nFlag = VF_OK
'                    If dOptPrice > 0# Then
'                        dIV = CalcVolatilityMM3(dRate, dYield, dSpotPrice, dOptPrice, aOpt.Strike, aOpt.Expiry - Date, _
'                                aOpt.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, m_Und.Skew, m_Und.Kurt, nModel, nFlag)
'
'                        If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
'                            dIV = aOpt.Vola
'                        End If
'                    End If
'
'                    If dIV > 0 Then
'                        aQuote.IV = dIV
'                    Else
'                        aQuote.IV = BAD_DOUBLE_VALUE
'                    End If
'                Else
'                    aQuote.IV = aOpt.Vola
'                End If
'            End If
'
'            If g_Params.UseTheoVolatility Then
'                dVola = aOpt.Vola
'            Else
'                dVola = IIf(aQuote.IV > BAD_DOUBLE_VALUE, aQuote.IV, 0)
'            End If
'
'            If aGreeks.nMask <> GM_NONE And (g_Params.UseTheoVolatility And aQuote.Exch.ID = 0 Or Not g_Params.UseTheoVolatility) Then
'                RetCount = CalcGreeksMM2(dRate, dYield, dSpotPrice, aOpt.Strike, dVola, aOpt.Expiry - Date, _
'                                    aOpt.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, m_Und.Skew, m_Und.Kurt, nModel, aGreeks)
'            End If
'
'            If (enCalcIV And CALC_IV_BID) = CALC_IV_BID Then
'                If dSpotBid > 0 And dOptBid > 0# Then
'                    dIV = CalcVolatilityMM2(dRate, dYield, dSpotBid, dOptBid, aOpt.Strike, aOpt.Expiry - Date, _
'                                        aOpt.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, m_Und.Skew, m_Und.Kurt, nModel)
'                    If dIV > 0 Then
'                        aQuote.IVBid = dIV
'                    Else
'                        aQuote.IVBid = BAD_DOUBLE_VALUE
'                    End If
'                End If
'            End If
'
'            If (enCalcIV And CALC_IV_ASK) = CALC_IV_ASK Then
'                If dSpotAsk > 0 And dOptAsk > 0# Then
'                    dIV = CalcVolatilityMM2(dRate, dYield, dSpotAsk, dOptAsk, aOpt.Strike, aOpt.Expiry - Date, _
'                                        aOpt.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, m_Und.Skew, m_Und.Kurt, nModel)
'                    If dIV > 0 Then
'                        aQuote.IVAsk = dIV
'                    Else
'                        aQuote.IVAsk = BAD_DOUBLE_VALUE
'                    End If
'                End If
'            End If
'
'            Erase dDivDte
'            Erase dDivAmts
'
'            If aGreeks.nMask <> GM_NONE And (g_Params.UseTheoVolatility And aQuote.Exch.ID = 0 Or Not g_Params.UseTheoVolatility) Then
'                aQuote.ClearValues False
'
'                If RetCount <> 0 Then
'
'                    FixGreeks aGreeks
'
'                    aQuote.TheoPrice = IIf((aGreeks.nMask And GM_THEOPRICE) = GM_THEOPRICE, aGreeks.dTheoPrice, BAD_DOUBLE_VALUE)
'                    aQuote.Delta = IIf((aGreeks.nMask And GM_DELTA) = GM_DELTA, aGreeks.dDelta * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.Gamma = IIf((aGreeks.nMask And GM_GAMMA) = GM_GAMMA, aGreeks.dGamma * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.Theta = IIf((aGreeks.nMask And GM_THETA) = GM_THETA, aGreeks.dTheta * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.Vega = IIf((aGreeks.nMask And GM_VEGA) = GM_VEGA, aGreeks.dVega * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.Rho = IIf((aGreeks.nMask And GM_RHO) = GM_RHO, aGreeks.dRho * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.VegaDelta = IIf((aGreeks.nMask And GM_DELTA_VEGA) = GM_DELTA_VEGA, aGreeks.dDeltaVega * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.VegaGamma = IIf((aGreeks.nMask And GM_GAMMA_VEGA) = GM_GAMMA_VEGA, aGreeks.dGammaVega * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.ThetaDelta = IIf((aGreeks.nMask And GM_DELTA_THETA) = GM_DELTA_THETA, aGreeks.dDeltaTheta * 100#, BAD_DOUBLE_VALUE)
'                    aQuote.ThetaGamma = IIf((aGreeks.nMask And GM_GAMMA_THETA) = GM_GAMMA_THETA, aGreeks.dGammaTheta * 100#, BAD_DOUBLE_VALUE)
'
'                    If (aGreeks.nMask And GM_GAMMA) = GM_GAMMA And (aGreeks.nMask And GM_THETA) = GM_THETA And aGreeks.dTheta <> 0 Then
'                        aQuote.GammaPerTheta = aGreeks.dGamma / aGreeks.dTheta * 100#
'                    End If
'                End If
'
'            ElseIf aGreeks.nMask <> GM_NONE And (g_Params.UseTheoVolatility And aQuote.Exch.ID <> 0) Then
'                aQuote.TheoPrice = aOpt.DefQuote.TheoPrice
'                aQuote.Delta = aOpt.DefQuote.Delta
'                aQuote.Gamma = aOpt.DefQuote.Gamma
'                aQuote.Theta = aOpt.DefQuote.Theta
'                aQuote.Vega = aOpt.DefQuote.Vega
'                aQuote.Rho = aOpt.DefQuote.Rho
'                aQuote.VegaDelta = aOpt.DefQuote.VegaDelta
'                aQuote.VegaGamma = aOpt.DefQuote.VegaGamma
'                aQuote.ThetaDelta = aOpt.DefQuote.ThetaDelta
'                aQuote.ThetaGamma = aOpt.DefQuote.ThetaGamma
'                aQuote.GammaPerTheta = aOpt.DefQuote.GammaPerTheta
'            End If
'
'            'CalcSleep
'        End If
'    End If
'End Sub
'
'Private Sub AddTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim aOpt As clsQnOptAtom, nRow&, nBS&, bUpdate As Boolean
'
'    If g_CurTraderID <> 0 And aTrd.Trader.ID <> g_CurTraderID Or aTrd.Status = enTsSimulated Then Exit Sub
'    If m_Und.ID <> aTrd.UndID Then Exit Sub
'
'    bUpdate = False
'
'    nBS = IIf(aTrd.IsBuy, 1, -1)
'
'    If aTrd.ContractType = enCtOption Then
'
'        Set aOpt = m_Und.Expiry(Format$(aTrd.Opt.Expiry, "MMMM YY")).Strike(CStr(aTrd.Opt.Strike)).aOption(aTrd.Opt.OptType)
'        If Not aOpt Is Nothing Then
'            If IsBadLong(aOpt.Qty) Then aOpt.Qty = 0
'            aOpt.Qty = aOpt.Qty + aTrd.Quantity * nBS
'            If IsBadLong(aOpt.QtyInShares) Then aOpt.QtyInShares = 0
'            aOpt.QtyInShares = aOpt.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
'            bUpdate = True
'        End If
'
'    Else
'        If IsBadLong(m_Und.QtyInShares) Then m_Und.QtyInShares = 0
'        m_Und.QtyInShares = m_Und.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
'        If IsBadDouble(m_Und.Pos) Then m_Und.Pos = 0
'        m_Und.Pos = m_Und.Pos + aTrd.Price * aTrd.Quantity * aTrd.LotSize * nBS
'
'        UnderlyingAdjustRates False
'        RatesUpdate
'        AdjustDivsAndRatesVisible
'
'        bUpdate = True
'    End If
'
'    If bUpdate Then
'        Screen.MousePointer = vbHourglass
'
'        If aTrd.ContractType = enCtOption Then
'            nRow = fgOpt.FindRow(aOpt.Symbol, , IIf(aOpt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'            CalculateUnderlyingOptions False
'            OptionUpdateQuote nRow, aOpt.OptType, False, False
'            UpdateTotals
'        Else
'            UnderlyingUpdateQuote 1, False, False
'        End If
'
'        Screen.MousePointer = vbDefault
'    End If
'    Set aOpt = Nothing
'End Sub
'
'Private Sub UpdateTrade(aNewTrd As EtsGeneralLib.MmTradeInfoAtom, aOldTrd As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim aOpt As clsQnOptAtom, nRow&, nNewBS&, nOldBS&, bUpdate As Boolean
'
'    Debug.Assert aNewTrd.ContractID = aOldTrd.ContractID
'
'    If m_Und.ID <> aNewTrd.UndID Or m_Und.ID <> aOldTrd.UndID Then Exit Sub
'
'    bUpdate = False
'    nNewBS = IIf(aNewTrd.IsBuy, 1, -1)
'    nOldBS = IIf(aOldTrd.IsBuy, 1, -1)
'
'    If aNewTrd.ContractType = enCtOption Then
'
'        Set aOpt = m_Und.Expiry(Format$(aNewTrd.Opt.Expiry, "MMMM YY")).Strike(CStr(aNewTrd.Opt.Strike)).aOption(aNewTrd.Opt.OptType)
'        If Not aOpt Is Nothing Then
'            If (g_CurTraderID = 0 Or aOldTrd.Trader.ID = g_CurTraderID) And aOldTrd.Status <> enTsSimulated Then
'                If IsBadLong(aOpt.Qty) Then aOpt.Qty = 0
'                aOpt.Qty = aOpt.Qty - aOldTrd.Quantity * nOldBS
'                If IsBadLong(aOpt.QtyInShares) Then aOpt.QtyInShares = 0
'                aOpt.QtyInShares = aOpt.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
'                bUpdate = True
'            End If
'
'            If (g_CurTraderID = 0 Or aNewTrd.Trader.ID = g_CurTraderID) And aNewTrd.Status <> enTsSimulated Then
'                If IsBadLong(aOpt.Qty) Then aOpt.Qty = 0
'                aOpt.Qty = aOpt.Qty + aNewTrd.Quantity * nNewBS
'                If IsBadLong(aOpt.QtyInShares) Then aOpt.QtyInShares = 0
'                aOpt.QtyInShares = aOpt.QtyInShares + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
'                bUpdate = True
'            End If
'        End If
'
'    Else
'        If (g_CurTraderID = 0 Or aOldTrd.Trader.ID = g_CurTraderID) And aOldTrd.Status <> enTsSimulated Then
'            If IsBadLong(m_Und.QtyInShares) Then m_Und.QtyInShares = 0
'            m_Und.QtyInShares = m_Und.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
'            If IsBadDouble(m_Und.Pos) Then m_Und.Pos = 0
'            m_Und.Pos = m_Und.Pos - aOldTrd.Price * aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
'            bUpdate = True
'        End If
'
'        If (g_CurTraderID = 0 Or aNewTrd.Trader.ID = g_CurTraderID) And aNewTrd.Status <> enTsSimulated Then
'            If IsBadLong(m_Und.QtyInShares) Then m_Und.QtyInShares = 0
'            m_Und.QtyInShares = m_Und.QtyInShares + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
'            If IsBadDouble(m_Und.Pos) Then m_Und.Pos = 0
'            m_Und.Pos = m_Und.Pos + aNewTrd.Price * aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
'            bUpdate = True
'        End If
'
'        If bUpdate Then
'            UnderlyingAdjustRates False
'            RatesUpdate
'            AdjustDivsAndRatesVisible
'        End If
'    End If
'
'    If bUpdate Then
'        Screen.MousePointer = vbHourglass
'
'        If aNewTrd.ContractType = enCtOption Then
'            nRow = fgOpt.FindRow(aOpt.Symbol, , IIf(aOpt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'            CalculateUnderlyingOptions False
'            OptionUpdateQuote nRow, aOpt.OptType, False, False
'            UpdateTotals
'        Else
'            UnderlyingUpdateQuote 1, False, False
'        End If
'
'        Screen.MousePointer = vbDefault
'    End If
'    Set aOpt = Nothing
'End Sub
'
'Private Sub DeleteTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim aOpt As clsQnOptAtom, nRow&, nBS&, bUpdate As Boolean
'
'    If g_CurTraderID <> 0 And aTrd.Trader.ID <> g_CurTraderID Or aTrd.Status = enTsSimulated Then Exit Sub
'    If m_Und.ID <> aTrd.UndID Then Exit Sub
'
'    bUpdate = False
'    nBS = IIf(aTrd.IsBuy, 1, -1)
'
'    If aTrd.ContractType = enCtOption Then
'
'        Set aOpt = m_Und.Expiry(Format$(aTrd.Opt.Expiry, "MMMM YY")).Strike(CStr(aTrd.Opt.Strike)).aOption(aTrd.Opt.OptType)
'        If Not aOpt Is Nothing Then
'            If Not IsBadLong(aOpt.Qty) Then
'                aOpt.Qty = aOpt.Qty - aTrd.Quantity * nBS
'                bUpdate = True
'            End If
'            If Not IsBadLong(aOpt.QtyInShares) Then
'                aOpt.QtyInShares = aOpt.QtyInShares - aTrd.Quantity * aTrd.LotSize * nBS
'                bUpdate = True
'            End If
'            Set aOpt = Nothing
'        End If
'
'    Else
'        If Not IsBadLong(m_Und.QtyInShares) Then
'            m_Und.QtyInShares = m_Und.QtyInShares - aTrd.Quantity * aTrd.LotSize * nBS
'            bUpdate = True
'        End If
'
'        If Not IsBadDouble(m_Und.Pos) Then
'            m_Und.Pos = m_Und.Pos - aTrd.Price * aTrd.Quantity * aTrd.LotSize * nBS
'            bUpdate = True
'
'            UnderlyingAdjustRates False
'            RatesUpdate
'            AdjustDivsAndRatesVisible
'        End If
'    End If
'
'    If bUpdate Then
'        Screen.MousePointer = vbHourglass
'
'        If aTrd.ContractType = enCtOption Then
'            nRow = fgOpt.FindRow(aOpt.Symbol, , IIf(aOpt.OptType = enOtCall, UOC_C_KEY, UOC_P_KEY))
'            CalculateUnderlyingOptions False
'            OptionUpdateQuote nRow, aOpt.OptType, False, False
'            UpdateTotals
'        Else
'            UnderlyingUpdateQuote 1, False, False
'        End If
'
'        Screen.MousePointer = vbDefault
'    End If
'    Set aOpt = Nothing
'End Sub
'
'Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
'    On Error Resume Next
'    'If m_bInProc Then Exit Sub
'    If m_Und.ID = 0 Then Exit Sub
'
'    ' futures & futures options trades are not supported yet
'    If Not aNewTrdInfo Is Nothing Then _
'        If aNewTrdInfo.ContractType = enCtFuture Or aNewTrdInfo.ContractType = enCtFutOption Then Exit Sub
'    If Not aOldTrdInfo Is Nothing Then _
'        If aOldTrdInfo.ContractType = enCtFuture Or aOldTrdInfo.ContractType = enCtFutOption Then Exit Sub
'
'    Select Case enAction
'        Case enTaTradeNew
'            Debug.Assert Not aNewTrdInfo Is Nothing
'            AddTrade aNewTrdInfo
'
'        Case enTaTradeUpdate
'            Debug.Assert Not aNewTrdInfo Is Nothing
'            Debug.Assert Not aOldTrdInfo Is Nothing
'            UpdateTrade aNewTrdInfo, aOldTrdInfo
'
'        Case enTaTradeDelete
'            Debug.Assert Not aOldTrdInfo Is Nothing
'            DeleteTrade aOldTrdInfo
'    End Select
'End Sub
'
'Private Sub TradeChannel_PositionTransfer(aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, aTrdTo As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    'If m_bInProc Then Exit Sub
'    If m_Und.ID = 0 Then Exit Sub
'
'    Debug.Assert Not aTrdFrom Is Nothing
'    Debug.Assert Not aTrdTo Is Nothing
'
'    ' futures & futures options trades are not supported yet
'    If aTrdFrom.ContractType = enCtFuture Or aTrdFrom.ContractType = enCtFutOption Then Exit Sub
'    If aTrdTo.ContractType = enCtFuture Or aTrdTo.ContractType = enCtFutOption Then Exit Sub
'
'    AddTrade aTrdFrom
'    AddTrade aTrdTo
'End Sub
'
'Private Sub UserControl_Hide()
'    On Error Resume Next
'    m_enState = enQsReady
'End Sub
'
'Private Sub UserControl_Resize()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    Dim nHeight&, nRows&, i&, nVisRows&
'
'    With fgUnd
'        nVisRows = 0
'        nRows = .Rows - 1
'        For i = 1 To nRows
'            If Not .RowHidden(i) Then
'                nVisRows = nVisRows + 1
'            End If
'        Next
'
'        .Top = 0
'        .Left = 0
'        .Height = .RowHeight(0) + nVisRows * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'        .Width = ScaleWidth
'        nHeight = .Height + 60
'    End With
'
'    With fgDiv
'        If m_bDividendsVisible And (m_Und.ID = 0 Or m_Und.ID <> 0 And m_Und.UndType = enCtStock) Or m_bRatesVisible Or m_bCalcModelVisible Or m_bExpCalendarVisible Then
'            .Top = fgUnd.Height + 60
'            .Left = 0
'            .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'            .Width = ScaleWidth
'            nHeight = nHeight + .Height + 60
'        End If
'    End With
'
'    If m_bLogVisible Then
'        With fgLog
'            .Left = 0
'            .Height = 1200
'            .Top = IIf(nHeight > ScaleHeight - .Height, nHeight, ScaleHeight - .Height)
'            .Width = ScaleWidth
'        End With
'    End If
'
'    With fgOpt
'        .Left = 0
'        .Top = nHeight
'        .Width = ScaleWidth
'        .Height = IIf(m_bLogVisible, fgLog.Top - .Top - 60, ScaleHeight - .Top)
'    End With
'
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
'    m_bIsConnected = False
'    m_enState = enQsReady
'    m_bInProc = False
'
'    Set frmLayout = Nothing
'    Set TradeChannel = Nothing
'    Set VolaSource = Nothing
'
'    Set MmTracer = Nothing
'    Set MmParams = Nothing
'
'    If Not PriceProvider Is Nothing Then
'        PriceProvider.CancelLastQuote
'        PriceProvider.UnSubscribeQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'        Set PriceProvider = Nothing
'    End If
'    Set m_GroupPriceProvider = Nothing
'
'    If Not MsProvider Is Nothing Then
'        MsProvider.CancelOptions
'        MsProvider.CancelStock
'        Set MsProvider = Nothing
'    End If
'
'    If Not MmManager Is Nothing Then
'        MmManager.CancelLastMmQuote
'        Set MmManager = Nothing
'    End If
'
'    Set geOpt = Nothing
'    Set m_QuoteRequests = Nothing
'    Set m_UndExch = Nothing
'    Set m_OptExch = Nothing
'    Set m_Und = Nothing
'    Set m_Exch = Nothing
'
'    Set pbProgress = Nothing
'    Set lblProcess = Nothing
'    Set lblStatus = Nothing
'    Set lblValue = Nothing
'    Set imgStop = Nothing
'    Set imgStopDis = Nothing
'End Sub
'
'Public Sub ShowAllUndExchanges()
'    On Error Resume Next
'    Dim aExch As EtsGeneralLib.ExchAtom
'
'    If m_UndExch.Count > 1 Then
'        For Each aExch In m_UndExch
'            aExch.Visible = True
'        Next
'
'        ShowUndExchanges
'    End If
'
'    RaiseEvent OnUndExchangesChange(True)
'End Sub
'
'Public Sub ShowUndExchange(ByVal nIndex As Long, ByVal bShow As Boolean)
'    On Error Resume Next
'    Dim i&, nVisibleCount&, aExch As EtsGeneralLib.ExchAtom, aExch2 As EtsGeneralLib.ExchAtom
'
'    If nIndex > 0 And nIndex <= m_UndExch.Count Then
'
'        If Not bShow Then
'            nVisibleCount = 0
'            If nIndex <> 1 Then
'                i = 1
'                For Each aExch In m_UndExch
'                    If aExch.Visible Then
'                        nVisibleCount = nVisibleCount + 1
'                    End If
'                    If i = nIndex Then Set aExch2 = aExch
'                    i = i + 1
'                Next
'            End If
'        Else
'            i = 1
'            For Each aExch In m_UndExch
'                If i = nIndex Then
'                    Set aExch2 = aExch
'                    Exit For
'                End If
'                i = i + 1
'            Next
'            nVisibleCount = 2
'        End If
'
'        If nVisibleCount > 1 Then
'            aExch2.Visible = bShow
'            Set aExch2 = Nothing
'            ShowUndExchanges
'
'            RaiseEvent OnUndExchangesChange(True)
'        End If
'    End If
'End Sub
'
'Private Sub ShowUndExchanges()
'    On Error Resume Next
'    Dim nRow&, nRows&
'    Dim RowData As clsQnQuoteRowData, bHidden As Boolean
'
'    With fgUnd
'        m_GridLock(GT_QUOTATION_UNDERLYING).LockRedraw
'
'        nRows = .Rows - 1
'        For nRow = 1 To nRows
'            Set RowData = .RowData(nRow)
'
'            Debug.Assert Not RowData Is Nothing
'            Debug.Assert Not RowData.Exch Is Nothing
'
'            bHidden = RowData.Exch.ID <> 0 And Not RowData.Exch.Visible
'            .RowHidden(nRow) = bHidden
'
'            Set RowData = Nothing
'        Next
'
'        m_GridLock(GT_QUOTATION_UNDERLYING).UnlockRedraw
'
'        UserControl_Resize
'    End With
'End Sub
'
'Public Sub ShowAllExpiries()
'    On Error Resume Next
'    Dim i&, nCount&
'
'    nCount = m_Und.Expiry.Count
'    If nCount > 1 Then
'        For i = 1 To nCount
'            m_Und.Expiry(i).Visible = True
'        Next
'
'        ShowExpiriesAndExchanges False
'
'        RaiseEvent OnExpiriesChange(True)
'    End If
'End Sub
'
'Public Sub ShowExpiry(ByVal nIndex As Long, ByVal bShow As Boolean)
'    On Error Resume Next
'    Dim i&, nCount&, nVisibleCount&
'
'    nCount = m_Und.Expiry.Count
'    If nIndex > 0 And nIndex <= nCount Then
'        If Not bShow Then
'            nVisibleCount = 0
'            For i = 1 To nCount
'                If m_Und.Expiry(i).Visible Then
'                    nVisibleCount = nVisibleCount + 1
'                End If
'            Next
'        Else
'            nVisibleCount = 2
'        End If
'
'        If nVisibleCount > 1 Then
'            m_Und.Expiry(nIndex).Visible = bShow
'            ShowExpiriesAndExchanges False
'
'            RaiseEvent OnExpiriesChange(True)
'        End If
'    End If
'End Sub
'
'Public Sub ShowNextExpiry()
'    On Error Resume Next
'    Dim i&, nCount&
'
'    nCount = m_Und.Expiry.Count
'    If nCount > 1 Then
'        m_nCurrentExpiryIdx = m_nCurrentExpiryIdx + 1
'        If m_nCurrentExpiryIdx > nCount Then m_nCurrentExpiryIdx = 1
'
'        For i = 1 To nCount
'            m_Und.Expiry(i).Visible = (i = m_nCurrentExpiryIdx)
'        Next
'
'        ShowExpiriesAndExchanges False
'
'        RaiseEvent OnExpiriesChange(True)
'    End If
'End Sub
'
'Public Sub ShowPrevExpiry()
'    On Error Resume Next
'    Dim i&, nCount&
'
'    nCount = m_Und.Expiry.Count
'    If nCount > 1 Then
'        m_nCurrentExpiryIdx = m_nCurrentExpiryIdx - 1
'        If m_nCurrentExpiryIdx < 1 Then m_nCurrentExpiryIdx = nCount
'
'        For i = 1 To nCount
'            m_Und.Expiry(i).Visible = (i = m_nCurrentExpiryIdx)
'        Next
'
'        ShowExpiriesAndExchanges False
'
'        RaiseEvent OnExpiriesChange(True)
'    End If
'End Sub
'
'Private Sub ShowExpiriesAndExchanges(ByVal bUpdateQuote As Boolean)
'    On Error Resume Next
'    Dim nRow&, nRows&
'    Dim RowData As clsQnQuoteRowData, nOldRow&, bHidden As Boolean
'
'    With fgOpt
'        m_GridLock(GT_QUOTATION_OPTIONS).LockRedraw
'
'        nOldRow = .Row
'
'        nRows = .Rows - 1
'        For nRow = 1 To nRows
'            Set RowData = .RowData(nRow)
'
'            Debug.Assert Not RowData Is Nothing
'            Debug.Assert Not RowData.Exp Is Nothing
'            Debug.Assert Not RowData.Exch Is Nothing
'
'            bHidden = Not RowData.Exp.Visible Or Not RowData.Exch.Visible
'
'            .RowHidden(nRow) = bHidden
'            If bUpdateQuote And Not bHidden Then
'                OptionUpdateQuote nRow, enOtCall, False, False
'                OptionUpdateQuote nRow, enOtPut, False, False
'            End If
'
'            Set RowData = Nothing
'        Next
'
'        If nOldRow > 0 Then
'            If Not .RowHidden(nOldRow) Then
'                .Row = nOldRow
'            Else
'                .Row = .TopRow
'            End If
'        End If
'
'        m_GridLock(GT_QUOTATION_OPTIONS).UnlockRedraw
'    End With
'End Sub
'
'Public Function StartRealTime(ByVal bStart As Boolean) As Boolean
'    On Error GoTo EH
'    Dim i&, nCount&, sExch$
'    Dim aParam As ISEPROVIDERLib.QuoteUpdateParams, aExch As EtsGeneralLib.ExchAtom
'
'    StartRealTime = False
'    If m_bInProc Or (m_enState <> enQsReady And m_enState <> enQsRealtime) Then Exit Function
'
'    If Not PriceProvider Is Nothing Then
'        If bStart And m_enState = enQsReady Then
'            nCount = m_QuoteRequests.Count
'            If nCount > 0 Then
'                Screen.MousePointer = vbArrow
'                m_enState = enQsRealtime
'                m_bInProc = True
'                AdjustState
'
'                pbProgress.Min = 0
'                pbProgress.Value = 0
'                pbProgress.Max = nCount
'
'                pbProgress.Visible = True
'
'                lblStatus.Visible = False
'                lblProcess.Visible = True
'                lblProcess.Caption = "Quotes subscription..."
'                imgStop.Visible = True
'                imgStopDis.Visible = False
'
'                If Not m_bGroupRequest Then
'                    For i = 1 To nCount
'                        DoEvents
'                        If m_enState = enQsReady Then Exit For
'                        PriceProvider.SubscribeQuote m_QuoteRequests(i).GetQuoteUpdateParam
'                        IncProgress pbProgress
'                    Next
'
'                Else
'                    aParam.Symbol = m_Und.Symbol
'                    aParam.Type = IIf(m_Und.UndType = enCtStock, enSTK, enIDX)
'
'                    pbProgress.Max = m_UndExch.Count + 1
'
'                    For Each aExch In m_UndExch
'                        If m_enState = enQsReady Then Exit For
'                        aParam.Exchange = aExch.Code
'                        PriceProvider.SubscribeQuote aParam
'                        IncProgress pbProgress
'                        DoEvents
'                    Next
'
'                    If m_enState = enQsRealtime Then
'                        sExch = ""
'
'                        For Each aExch In m_OptExch
'                            If aExch.ID <> 0 Then sExch = sExch & "," & aExch.Code
'                        Next
'
'                        aParam.Exchange = sExch
'                        aParam.Type = IIf(m_Und.UndType = enCtStock, enGrSTK, enGrIDX)
'                        m_GroupPriceProvider.RequestGroup aParam, True
'                        IncProgress pbProgress
'                    End If
'                End If
'
'                If m_enState = enQsReady Then
'                    On Error Resume Next
'                    PriceProvider.UnSubscribeQuote
'                    If m_bGroupRequest Then
'                        m_GroupPriceProvider.CancelGroup
'                    End If
'                End If
'
'                pbProgress.Visible = False
'                lblStatus.Visible = True
'                lblProcess.Visible = False
'                lblProcess.Caption = ""
'                imgStop.Visible = False
'                imgStopDis.Visible = False
'                StartRealTime = True
'                m_bInProc = False
'                AdjustCaption
'                AdjustState
'                Screen.MousePointer = vbDefault
'            End If
'
'        ElseIf Not bStart And m_enState = enQsRealtime Then
'            m_enState = enQsReady
'            m_bInProc = True
'            AdjustState
'
'            StartRealTime = True
'            On Error Resume Next
'            PriceProvider.UnSubscribeQuote
'            If m_bGroupRequest Then
'                m_GroupPriceProvider.CancelGroup
'            End If
'            m_bInProc = False
'            AdjustCaption
'            AdjustState
'        End If
'    Else
'        gCmn.MyMsgBox m_frmOwner, "You are not connected to market data provider.", vbExclamation
'    End If
'
'    Exit Function
'EH:
'    Dim sMsg$
'    If m_enState = enQsRealtime Then
'        sMsg = "Fail to start real time mode."
'    Else
'        sMsg = "Fail to stop real time mode."
'    End If
'    Screen.MousePointer = vbDefault
'    m_enState = enQsReady
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, sMsg
'    On Error Resume Next
'    AdjustCaption
'    pbProgress.Visible = False
'    lblStatus.Visible = True
'    lblProcess.Visible = False
'    lblProcess.Caption = ""
'    imgStop.Visible = False
'    imgStopDis.Visible = False
'    AdjustState
'
'    PriceProvider.UnSubscribeQuote
'    If m_bGroupRequest Then
'        m_GroupPriceProvider.CancelGroup
'    End If
'End Function
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
'    sCaption = ""
'    If m_Und.ID <> 0 Then sCaption = sCaption & m_Und.Symbol & " - "
'    If m_enState = enQsRealtime Then sCaption = sCaption & "[Real Time] - "
'    sCaption = sCaption & "ISE Market Maker"
'
'    GetCaption = sCaption
'End Function
'
'Public Function GetShortCaption() As String
'    On Error Resume Next
'    If m_Und.ID <> 0 Then
'        GetShortCaption = m_Und.Symbol
'    Else
'        GetShortCaption = "<Empty>"
'    End If
'End Function
'
'Public Property Get InProc() As Boolean
'    InProc = m_bInProc
'End Property
'
'Public Property Get IsConnected() As Boolean
'    IsConnected = m_bIsConnected
'End Property
'
'Public Property Get LastQuoteReqNow() As Boolean
'    LastQuoteReqNow = (m_enState = enQsLastQuoteReq)
'End Property
'
'Public Property Get RealTime() As Boolean
'    RealTime = (m_enState = enQsRealtime)
'End Property
'
'Public Property Get Expiries() As clsQnExpColl
'    On Error Resume Next
'    Set Expiries = m_Und.Expiry
'End Property
'
'Public Property Get UndExchanges() As EtsGeneralLib.ExchColl
'    On Error Resume Next
'    Set UndExchanges = m_UndExch
'End Property
'
'Public Property Get OptExchanges() As EtsGeneralLib.ExchColl
'    On Error Resume Next
'    Set OptExchanges = m_OptExch
'End Property
'
'Public Property Get Underlying() As clsQnUndAtom
'    On Error Resume Next
'    Set Underlying = m_Und
'End Property
'
'Public Sub CustomizeUnderlyingGridLayout()
'    On Error Resume Next
'    If Not frmLayout Is Nothing Then
'        frmLayout.Execute GT_QUOTATION_UNDERLYING, UUC_SYMBOL, UUC_LAST_COLUMN, m_gdUnd, m_frmOwner
'    End If
'End Sub
'
'Public Sub CustomizeDivGridLayout()
'    On Error Resume Next
'    If Not frmLayout Is Nothing Then
'        frmLayout.Execute GT_QUOTATION_DIVIDENDS, UDC_MODEL, UDC_LAST_COLUMN, m_gdDiv, m_frmOwner
'    End If
'End Sub
'
'Public Sub CustomizeOptionGridLayout()
'    On Error Resume Next
'    If Not frmLayout Is Nothing Then
'        frmLayout.Execute GT_QUOTATION_OPTIONS, UOC_C_SYMBOL, UOC_LAST_COLUMN, m_gdOpt, m_frmOwner
'    End If
'End Sub
'
'Public Sub CustomizeLogGridLayout()
'    On Error Resume Next
'    If Not frmLayout Is Nothing Then
'        frmLayout.Execute GT_QUOTATION_LOG, ULC_TIME, ULC_LAST_COLUMN, m_gdLog, m_frmOwner
'    End If
'End Sub
'
'Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&, nCount&, aExp As clsQnExpAtom, aExch As EtsGeneralLib.ExchAtom, sValue$, sValue2$
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    aStorage.SetStringValue "Quotation" & sKey, "Symbol", m_Und.Symbol
'    aStorage.SetLongValue "Quotation" & sKey, "ShowModel", m_bCalcModelVisible
'    aStorage.SetLongValue "Quotation" & sKey, "ShowCalendar", m_bExpCalendarVisible
'    aStorage.SetLongValue "Quotation" & sKey, "ShowDivs", m_bDividendsVisible
'    aStorage.SetLongValue "Quotation" & sKey, "ShowRates", m_bRatesVisible
'    aStorage.SetLongValue "Quotation" & sKey, "ShowLog", m_bLogVisible
''    aStorage.SetLongValue "Quotation" & sKey, "CurrentExpiryIdx", m_nCurrentExpiryIdx
''    aStorage.SetLongValue "Quotation" & sKey, "CurrentOptExchIdx", m_nCurrentOptExchIdx
'
'    sValue = ""
'    sValue2 = ""
'    nCount = m_Und.Expiry.Count
'    For i = 1 To nCount
'        Set aExp = m_Und.Expiry(i)
'        If aExp.Visible Then
'            sValue = sValue & "|" & CLng(aExp.ExpiryMonth)
'        End If
'        sValue2 = sValue2 & "|" & Trim$(Str$(CLng(aExp.ExpiryMonth))) & "," & Trim$(Str$(aExp.RateCust))
'        Set aExp = Nothing
'    Next
'    If Len(sValue) > 0 Then
'        sValue = Mid$(sValue, 2)
'        aStorage.SetStringValue "Quotation" & sKey, "VisibleExp", sValue
'    End If
'
'    aStorage.SetLongValue "Quotation" & sKey, "UseCustRates", m_Und.UseCustRates
'    If Len(sValue2) > 0 Then
'        sValue2 = Mid$(sValue2, 2)
'        aStorage.SetStringValue "Quotation" & sKey, "CustRates", sValue2
'    End If
'
'    If m_Und.ID <> 0 Then
'        aStorage.SetLongValue "Quotation" & sKey, "IsAmerican", IIf(m_Und.IsAmerican, 1, 0)
'    End If
'
'    sValue = ""
'    For Each aExch In m_UndExch
'        If aExch.Visible Then
'            sValue = sValue & "|" & CLng(aExch.ID)
'        End If
'    Next
'
'    If Len(sValue) > 0 Then
'        sValue = Mid$(sValue, 2)
'        aStorage.SetStringValue "Quotation" & sKey, "VisibleUndExch", sValue
'    End If
'
'    sValue = ""
'    For Each aExch In m_OptExch
'        If aExch.Visible Then
'            sValue = sValue & "|" & CLng(aExch.ID)
'        End If
'    Next
'
'    If Len(sValue) > 0 Then
'        sValue = Mid$(sValue, 2)
'        aStorage.SetStringValue "Quotation" & sKey, "VisibleOptExch", sValue
'    End If
'
'    m_gdUnd.WriteToStorage "QuotationUndGrid" & sKey, aStorage
'    m_gdDiv.WriteToStorage "QuotationDivGrid" & sKey, aStorage, False
'    m_gdOpt.WriteToStorage "QuotationOptGrid" & sKey, aStorage
'    Exit Sub
'EH:
'    gCmn.ErrorHandler ""
'End Sub
'
'Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&, sSym$, aValues() As String, sValue$, aValues2() As String
'    Dim nValue&, nLBound&, nUBound&, nLBound2&, nUBound2&, dValue#, aUnd As EtsGeneralLib.UndAtom
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    sSym = Trim$(aStorage.GetStringValue("Quotation" & sKey, "Symbol"))
'    m_bCalcModelVisible = aStorage.GetLongValue("Quotation" & sKey, "ShowModel", True)
'    m_bExpCalendarVisible = aStorage.GetLongValue("Quotation" & sKey, "ShowCalendar", True)
'    m_bDividendsVisible = aStorage.GetLongValue("Quotation" & sKey, "ShowDivs", True)
'    m_bRatesVisible = aStorage.GetLongValue("Quotation" & sKey, "ShowRates", True)
'    m_bLogVisible = aStorage.GetLongValue("Quotation" & sKey, "ShowLog", True)
''    m_nCurrentExpiryIdx = aStorage.GetLongValue("Quotation" & sKey, "CurrentExpiryIdx")
''    m_nCurrentOptExchIdx = aStorage.GetLongValue("Quotation" & sKey, "CurrentOptExchIdx")
'
'    m_VisibleExp.Clear
'    sValue = aStorage.GetStringValue("Quotation" & sKey, "VisibleExp")
'    If Len(sValue) > 0 Then
'        On Error Resume Next
'
'        aValues = Split(sValue, "|")
'        nLBound = 0
'        nUBound = -1
'        nLBound = LBound(aValues)
'        nUBound = UBound(aValues)
'        For i = nLBound To nUBound
'            Err.Clear
'            nValue = 0
'            nValue = CLng(aValues(i))
'            If Err.Number <> 0 Then nValue = Val(aValues(i))
'            If nValue <> 0 Then
'                If m_VisibleExp(CStr(nValue)) Is Nothing Then m_VisibleExp.Add(CStr(nValue)).ID = nValue
'            End If
'        Next
'
'        On Error GoTo EH
'    End If
'    Erase aValues
'
'    m_VisibleUndExch.Clear
'    sValue = aStorage.GetStringValue("Quotation" & sKey, "VisibleUndExch")
'    If Len(sValue) > 0 Then
'        On Error Resume Next
'
'        aValues = Split(sValue, "|")
'        nLBound = 0
'        nUBound = -1
'        nLBound = LBound(aValues)
'        nUBound = UBound(aValues)
'        For i = nLBound To nUBound
'            Err.Clear
'            nValue = 0
'            nValue = CLng(aValues(i))
'            If Err.Number <> 0 Then nValue = Val(aValues(i))
'            If Err.Number = 0 Then
'                If m_VisibleUndExch(CStr(nValue)) Is Nothing Then m_VisibleUndExch.Add(CStr(nValue)).ID = nValue
'            End If
'        Next
'
'        On Error GoTo EH
'    End If
'    Erase aValues
'
'    m_VisibleOptExch.Clear
'    sValue = aStorage.GetStringValue("Quotation" & sKey, "VisibleOptExch")
'    If Len(sValue) > 0 Then
'        On Error Resume Next
'
'        aValues = Split(sValue, "|")
'        nLBound = 0
'        nUBound = -1
'        nLBound = LBound(aValues)
'        nUBound = UBound(aValues)
'        For i = nLBound To nUBound
'            Err.Clear
'            nValue = 0
'            nValue = CLng(aValues(i))
'            If Err.Number <> 0 Then nValue = Val(aValues(i))
'            If Err.Number = 0 Then
'                If m_VisibleOptExch(CStr(nValue)) Is Nothing Then m_VisibleOptExch.Add(CStr(nValue)).ID = nValue
'            End If
'        Next
'
'        On Error GoTo EH
'    End If
'    Erase aValues
'
'    m_bUseCustRates = aStorage.GetLongValue("Quotation" & sKey, "UseCustRates", False)
'    m_CustRates.Clear
'    sValue = aStorage.GetStringValue("Quotation" & sKey, "CustRates")
'    If Len(sValue) > 0 Then
'        On Error Resume Next
'
'        aValues = Split(sValue, "|")
'        nLBound = 0
'        nUBound = -1
'        nLBound = LBound(aValues)
'        nUBound = UBound(aValues)
'        For i = nLBound To nUBound
'            aValues2 = Split(aValues(i), ",")
'            nLBound2 = 0
'            nUBound2 = -1
'            nLBound2 = LBound(aValues2)
'            nUBound2 = UBound(aValues2)
'            If nUBound2 - nLBound2 = 1 Then
'                nValue = ReadLng(aValues2(nLBound2))
'                dValue = ReadDbl(aValues2(nUBound2))
'                If nValue > 0 And dValue > 0# Then
'                    If m_CustRates(CStr(nValue)) Is Nothing Then m_CustRates.Add(CStr(nValue)).Data3 = dValue
'                End If
'            End If
'            Erase aValues2
'        Next
'
'        On Error GoTo EH
'    End If
'    Erase aValues
'
'    m_nIsAmerican = aStorage.GetLongValue("Quotation" & sKey, "IsAmerican", -1)
'
'    m_gdUnd.ReadFromStorage "QuotationUndGrid" & sKey, aStorage
'    m_gdDiv.ReadFromStorage "QuotationDivGrid" & sKey, aStorage, False
'    m_gdOpt.ReadFromStorage "QuotationOptGrid" & sKey, aStorage
'
'    If Len(sSym) > 0 Then
'        m_NewUnd.ID = 0
'        Set aUnd = g_UnderlyingAllBySym(sSym)
'        If Not aUnd Is Nothing Then
'            m_NewUnd.ID = aUnd.ID
'            m_NewUnd.Data = aUnd.UndType
'            m_NewUnd.Data3 = IIf(aUnd.IsTraderContract, 1, 0)
'            Set aUnd = Nothing
'        End If
'
'        If m_NewUnd.ID <> 0 Then
'            tmrShow.Enabled = True
'        'Else
'            ' ask for market structure download here
'        End If
'    End If
'
'    Exit Sub
'EH:
'    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
'    sDescription = Err.Description
'    nHelpContext = Err.HelpContext
'    sHelpFile = Err.HelpFile
'    nNumber = Err.Number
'    sSource = Err.Source
'
'    Erase aValues
'    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
'End Sub
'
'Private Sub Recalculate(ByVal bSymbol As Boolean)
'    On Error Resume Next
'    If m_Und.ID = 0 Then Exit Sub
'
'    Screen.MousePointer = vbHourglass
'    m_bInProc = True
'
'    CalculateUnderlyingOptions True
'    OptionsUpdate bSymbol, True, True
'    UpdateTotals
'
'    m_bInProc = False
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Sub Refresh()
'    On Error Resume Next
'    Dim i&, nCount&, nValue&, sKey$, dValue#, aUnd As EtsGeneralLib.UndAtom, aExch As EtsGeneralLib.ExchAtom
'    If m_Und.ID = 0 Then Exit Sub
'
'    m_bUseCustRates = m_Und.UseCustRates
'    m_nIsAmerican = IIf(m_Und.IsAmerican, 1, 0)
'
'    m_VisibleExp.Clear
'    m_CustRates.Clear
'    nCount = m_Und.Expiry.Count
'    For i = 1 To nCount
'        nValue = CLng(m_Und.Expiry(i).ExpiryMonth)
'        If m_Und.Expiry(i).Visible Then
'            sKey = Trim$(Str$(nValue))
'            If m_VisibleExp(sKey) Is Nothing Then m_VisibleExp.Add(sKey).ID = nValue
'        End If
'        dValue = m_Und.Expiry(i).RateCust
'        If m_CustRates(CStr(nValue)) Is Nothing Then m_CustRates.Add(CStr(nValue)).Data3 = dValue
'    Next
'
'    m_VisibleUndExch.Clear
'    For Each aExch In m_UndExch
'        If aExch.Visible Then
'            nValue = aExch.ID
'            sKey = CStr(nValue)
'            If m_VisibleUndExch(sKey) Is Nothing Then m_VisibleUndExch.Add(sKey).ID = nValue
'        End If
'    Next
'
'    m_VisibleOptExch.Clear
'    For Each aExch In m_OptExch
'        If aExch.Visible Then
'            nValue = aExch.ID
'            sKey = CStr(nValue)
'            If m_VisibleOptExch(sKey) Is Nothing Then m_VisibleOptExch.Add(sKey).ID = nValue
'        End If
'    Next
'
'    m_NewUnd.ID = 0
'    Set aUnd = g_UnderlyingAll(m_Und.ID)
'    If Not aUnd Is Nothing Then
'        m_NewUnd.ID = aUnd.ID
'        m_NewUnd.Data = aUnd.UndType
'        m_NewUnd.Data3 = IIf(aUnd.IsTraderContract, 1, 0)
'        Set aUnd = Nothing
'    End If
'
'    If m_NewUnd.ID <> 0 Then
'        tmrShow.Enabled = True
'    End If
'End Sub
'
'Public Property Get CalcModelVisible() As Boolean
'    On Error Resume Next
'    CalcModelVisible = m_bCalcModelVisible
'End Property
'
'Public Property Let CalcModelVisible(ByVal bVisible As Boolean)
'    On Error Resume Next
'    m_bCalcModelVisible = bVisible
'    AdjustDivsAndRatesVisible
'    UserControl_Resize
'    AdjustState
'End Property
'
'Public Property Get DividendsVisible() As Boolean
'    On Error Resume Next
'    DividendsVisible = m_bDividendsVisible
'End Property
'
'Public Property Let DividendsVisible(ByVal bVisible As Boolean)
'    On Error Resume Next
'    m_bDividendsVisible = bVisible
'    AdjustDivsAndRatesVisible
'    UserControl_Resize
'    AdjustState
'End Property
'
'Public Property Get RatesVisible() As Boolean
'    On Error Resume Next
'    RatesVisible = m_bRatesVisible
'End Property
'
'Public Property Let RatesVisible(ByVal bVisible As Boolean)
'    On Error Resume Next
'    m_bRatesVisible = bVisible
'    AdjustDivsAndRatesVisible
'    UserControl_Resize
'    AdjustState
'End Property
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
'Public Property Get ExpCalendarVisible() As Boolean
'    On Error Resume Next
'    ExpCalendarVisible = m_bExpCalendarVisible
'End Property
'
'Public Property Let ExpCalendarVisible(ByVal bVisible As Boolean)
'    On Error Resume Next
'    m_bExpCalendarVisible = bVisible
'    AdjustDivsAndRatesVisible
'    UserControl_Resize
'    AdjustState
'End Property
'
'Private Sub VolaSource_VolatilityChanged(ByVal Symbol As String)
'    On Error Resume Next
'    If m_enState <> enQsRealtime Or m_Und.ID = 0 Then Exit Sub
'    If Symbol = m_Und.Symbol Then
'        Recalculate False
'        SendQuotes
'    End If
'End Sub
'
'Public Sub InternalVolatilityChanged(ByVal sSymbol As String)
'    On Error Resume Next
'    If m_bVolaChangedNow Then Exit Sub
'    VolaSource_VolatilityChanged sSymbol
'End Sub
'
'Private Sub AdjustState()
'    On Error Resume Next
'    UpdateMenu
'    RaiseEvent OnStateChange
'End Sub
'
'Private Sub LogEventLocal(ByVal enType As EventTypeEnum, ByVal sMessage As String)
'    On Error Resume Next
'    Dim nRow&
'
'    With fgLog
'        m_GridLock(GT_QUOTATION_LOG).LockRedraw
'
'        sMessage = Replace(sMessage, vbCrLf, " ", , , vbTextCompare)
'
'        .AddItem Now & vbTab & sMessage
'        nRow = .Rows - 1
'        Set .Cell(flexcpPicture, nRow, ULC_TIME) = ilEvent.ListImages(enType).Picture
'        .Cell(flexcpPictureAlignment, nRow, ULC_TIME) = flexPicAlignLeftCenter
'        .ShowCell nRow, ULC_TIME
'        .Row = nRow
'        .AutoSize 0, .Cols - 1
'
'        m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
'    End With
'End Sub
'
'Public Sub ClearLog()
'    On Error Resume Next
'    m_GridLock(GT_QUOTATION_LOG).LockRedraw
'    fgLog.Rows = 1
'    m_GridLock(GT_QUOTATION_LOG).UnlockRedraw
'End Sub
'
'
'Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Und.Symbol & " ISE Market Maker Quotes", "ISE Market Maker Quotes", fgUnd, fgDiv, fgOpt, fgLog
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub InitVola(ByRef aUnd As clsQnUndAtom)
'    On Error GoTo EH
'    aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.UndType = enCtStock, enCtStock, enCtIndex), VolaSource
'    Exit Sub
'EH:
'    LogEvent EVENT_WARNING, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
'End Sub
'
'
